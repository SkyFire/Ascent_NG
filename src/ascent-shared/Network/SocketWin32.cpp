/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2010 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the AscentNG Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the AscentNG is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#include "Network.h"
#ifdef CONFIG_USE_IOCP

void Socket::WriteCallback()
{
	if(m_deleted || !m_connected)
		return;

	//printf("\nSocket::Writecallback(): sendsize : %u\n", this->m_writeByteCount);
	// We don't want any writes going on while this is happening.
	m_writeMutex.Acquire();
	if(writeBuffer.GetContiguiousBytes())
	{
		DWORD w_length = 0;
		DWORD flags = 0;

		// attempt to push all the data out in a non-blocking fashion.
		WSABUF buf;
		buf.len = (ULONG)writeBuffer.GetContiguiousBytes();
		buf.buf = (char*)writeBuffer.GetBufferStart();

		/*OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_EVENT_WRITE_END);
		int r = WSASend(m_fd, &buf, 1, &w_length, flags, &ov->m_overlap, 0);
		if(r == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSA_IO_PENDING)
			{
				DecSendLock();
				Disconnect();
			}
		}*/

		m_writeEvent.Mark();
		m_writeEvent.Reset(SOCKET_IO_EVENT_WRITE_END);
		int r = WSASend(m_fd, &buf, 1, &w_length, flags, &m_writeEvent.m_overlap, 0);
		if(r == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSA_IO_PENDING)
			{
				m_writeEvent.Unmark();
				DecSendLock();
				Disconnect();
			}
		}
	}
	else
	{
		// Write operation is completed.
		DecSendLock();
	}
	m_writeMutex.Release();
}

void Socket::SetupReadEvent()
{
	if(m_deleted || !m_connected)
		return;

	m_readMutex.Acquire();
	DWORD r_length = 0;
	DWORD flags = 0;
	WSABUF buf;
	buf.len = (ULONG)readBuffer.GetSpace();
	buf.buf = (char*)readBuffer.GetBuffer();	

	// event that will trigger after data is receieved
	/*OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_EVENT_READ_COMPLETE);

	if(WSARecv(m_fd, &buf, 1, &r_length, &flags, &ov->m_overlap, 0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
			Disconnect();
	}*/

	m_readEvent.Mark();
	m_readEvent.Reset(SOCKET_IO_EVENT_READ_COMPLETE);
	if(WSARecv(m_fd, &buf, 1, &r_length, &flags, &m_readEvent.m_overlap, 0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			m_readEvent.Unmark();
			Disconnect();
		}
	}
	//m_readEvent = ov;
	m_readMutex.Release();
}

void Socket::ReadCallback(uint32 len)
{
	readBuffer.IncrementWritten(len);
	OnRead();
	SetupReadEvent();
}

void Socket::AssignToCompletionPort()
{
	/*HANDLE h = */CreateIoCompletionPort((HANDLE)m_fd, m_completionPort, (ULONG_PTR)this, 0);
	//__asm int 3;
}

void Socket::BurstPush()
{
	if(AcquireSendLock())
		WriteCallback();
}

#endif
