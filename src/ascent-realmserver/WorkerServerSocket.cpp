/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#include "RStdAfx.h"
#include "svn_revision.h"

WSSocket::WSSocket(SOCKET fd) : Socket(fd, 100000, 100000)
{
	_authenticated = false;
	_remaining = 0;
	_cmd = 0;
	_ws = NULL;
}

WSSocket::~WSSocket()
{

}

void WSSocket::HandleAuthRequest(WorldPacket & pck)
{
	uint8 key[20];
	uint32 build;
	string ver;
	pck.read(key, 20);
	pck >> build;
	pck >> ver;

	Log.Notice("WSSocket", "Auth reply, server is %s build %u", ver.c_str(), build);

	// accept it
	WorldPacket data(ISMSG_AUTH_RESULT, 4);
	data << uint32(1);
	SendPacket(&data);
	_authenticated = true;
}

void WSSocket::OnRead()
{
    for(;;)
	{
		if(!_cmd)
		{
			if(readBuffer.GetSize() < 6)
				break;

			readBuffer.Read(&_cmd, 2);
			readBuffer.Read(&_remaining, 4);
			//_remaining = ntohl(_remaining);
		}

        if(_remaining && readBuffer.GetSize() < _remaining)
			break;

		if(_cmd == ICMSG_WOW_PACKET)
		{
			/* optimized version for packet passing, to reduce latency! ;) */
			/*uint32 sid = *(uint32*)&m_readBuffer[0];
			uint16 op  = *(uint16*)&m_readBuffer[4];
			uint32 sz  = *(uint32*)&m_readBuffer[6];
			Session * session = sClientMgr.GetSession(sid);
			if(session != NULL && session->GetSocket() != NULL)
				session->GetSocket()->OutPacket(op, sz, m_readBuffer + 10);*/

			//RemoveReadBufferBytes(sz + 10/*header*/, false);
			uint32 sid;
			uint16 op;
			uint32 sz;


			GetReadBuffer().Read(&sid, 4);
			GetReadBuffer().Read(&op, 2);
			GetReadBuffer().Read(&sz, 4);

			Session * session = sClientMgr.GetSession(sid);
			if(session != NULL && session->GetSocket() != NULL)
			{
				uint8* buf = new uint8[sz];
				GetReadBuffer().Read(buf, sz);
				session->GetSocket()->OutPacket(op, sz, buf);
				delete [] buf;
			}
			else
				GetReadBuffer().Remove(sz);

			_cmd = 0;

			continue;
		}
		WorldPacket * pck = new WorldPacket(_cmd, _remaining);
		_cmd = 0;
		pck->resize(_remaining);
		readBuffer.Read((uint8*)pck->contents(), _remaining);

		if(_authenticated)
		{
			// push to queue
			if(!_ws)
			{
				if(pck->GetOpcode() == ICMSG_REGISTER_WORKER)
				{
					// handle register worker
					HandleRegisterWorker(*pck);
				}

				/* I deliberately don't delete pck here for a reason :P */
			}
			else
			{
				_ws->QueuePacket(pck);
			}
		}
		else
		{
			if(pck->GetOpcode() != ICMSG_AUTH_REPLY)
				Disconnect();
			else
				HandleAuthRequest(*pck);
			
			delete pck;
		}
	}
}

void WSSocket::HandleRegisterWorker(WorldPacket & pck)
{
	uint32 build;
	pck >> build;

	// TODO: Check the build of the server
	WServer * new_server = sClusterMgr.CreateWorkerServer(this);
    if(new_server == 0)
	{
		WorldPacket data(ISMSG_REGISTER_RESULT, 10);
		data << uint32(0);
		SendPacket(&data);
		delete &pck;
		return;
	}

	/* because we don't have any locks in the managers, this has to execute
	   in the other thread. this is why I haven't deleted the packet yet
	 */
    _ws = new_server;
	pck.rpos(0);
	_ws->QueuePacket(&pck);
}

void WSSocket::SendPacket(WorldPacket * pck)
{
	bool rv;
	size_t size = pck->size();
	uint16 opcode = pck->GetOpcode();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)pck->contents(), uint32(size));

	if(rv) BurstPush();
	BurstEnd();
}

void WSSocket::SendWoWPacket(Session * from, WorldPacket * pck)
{
	bool rv;
	size_t size1 = pck->size();
	uint16 opcode1 = pck->GetOpcode();
	size_t size2 = size1 + 10;
	uint32 opcode2 = ISMSG_WOW_PACKET;
	uint32 id = from->GetSessionId();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	BurstSend((const uint8*)&opcode2, 2);
	BurstSend((const uint8*)&size2, 4);
	BurstSend((const uint8*)&id, 4);
	BurstSend((const uint8*)&opcode1, 2);
	rv=BurstSend((const uint8*)&size1, 4);	

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size1 > 0 && rv)
		rv = BurstSend(pck->contents(), uint32(size1));

	if(rv) BurstPush();
	BurstEnd();
}

void WSSocket::OnConnect()
{
	WorldPacket data(ISMSG_AUTH_REQUEST, 4);
	data << uint32(BUILD_REVISION);
	SendPacket(&data);
}

void WSSocket::OnDisconnect()
{
	sClusterMgr.OnServerDisconnect(_ws);
}