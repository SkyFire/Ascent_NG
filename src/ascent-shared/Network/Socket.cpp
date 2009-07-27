/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "Network.h"

initialiseSingleton(SocketGarbageCollector);

Socket::Socket(SOCKET fd, uint32 sendbuffersize, uint32 recvbuffersize) : m_fd(fd), m_connected(false),	m_deleted(false)
{
	// Allocate Buffers
	readBuffer.Allocate(recvbuffersize);
	writeBuffer.Allocate(sendbuffersize);

	// IOCP Member Variables
#ifdef CONFIG_USE_IOCP
	m_writeLock = 0;
	m_completionPort = 0;
#else
	m_writeLock = 0;
#endif

	// Check for needed fd allocation.
	if(m_fd == 0)
		m_fd = SocketOps::CreateTCPFileDescriptor();
}

Socket::~Socket()
{

}

bool Socket::Connect(const char * Address, uint32 Port)
{
	struct hostent * ci = gethostbyname(Address);
	if(ci == 0)
		return false;

	m_client.sin_family = ci->h_addrtype;
	m_client.sin_port = ntohs((u_short)Port);
	memcpy(&m_client.sin_addr.s_addr, ci->h_addr_list[0], ci->h_length);

	SocketOps::Blocking(m_fd);
	if(connect(m_fd, (const sockaddr*)&m_client, sizeof(m_client)) == -1)
		return false;

	// at this point the connection was established
#ifdef CONFIG_USE_IOCP
	m_completionPort = sSocketMgr.GetCompletionPort();
#endif
	_OnConnect();
	return true;
}

void Socket::Accept(sockaddr_in * address)
{
	memcpy(&m_client, address, sizeof(*address));
	_OnConnect();
}

void Socket::_OnConnect()
{
	// set common parameters on the file descriptor
	SocketOps::Nonblocking(m_fd);
	SocketOps::DisableBuffering(m_fd);
/*	SocketOps::SetRecvBufferSize(m_fd, m_writeBufferSize);
	SocketOps::SetSendBufferSize(m_fd, m_writeBufferSize);*/
	m_connected = true;

	// IOCP stuff
#ifdef CONFIG_USE_IOCP
	AssignToCompletionPort();
	SetupReadEvent();
#endif
	sSocketMgr.AddSocket(this);

	// Call virtual onconnect
	OnConnect();
}

bool Socket::Send(const uint8 * Bytes, uint32 Size)
{
	bool rv;

	// This is really just a wrapper for all the burst stuff.
	BurstBegin();
	rv = BurstSend(Bytes, Size);
	if(rv)
		BurstPush();
	BurstEnd();

	return rv;
}

bool Socket::BurstSend(const uint8 * Bytes, uint32 Size)
{
	return writeBuffer.Write(Bytes, Size);
}

string Socket::GetRemoteIP()
{
	char* ip = (char*)inet_ntoa( m_client.sin_addr );
	if( ip != NULL )
		return string( ip );
	else
		return string( "noip" );
}

void Socket::Disconnect()
{
	m_connected = false;

	// remove from mgr
	sSocketMgr.RemoveSocket(this);

	SocketOps::CloseSocket(m_fd);

	// Call virtual ondisconnect
	OnDisconnect();

	if(!m_deleted) Delete();
}

void Socket::Delete()
{
	if(m_deleted) return;
	m_deleted = true;

	if(m_connected) Disconnect();
	sSocketGarbageCollector.QueueSocket(this);
}

