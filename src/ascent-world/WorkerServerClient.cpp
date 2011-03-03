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

#include "StdAfx.h"
#ifdef CLUSTERING

WSClient::WSClient(SOCKET fd) : Socket(fd, 1024576, 1024576)
{
	_remaining = 0;
	_cmd = 0;
}

WSClient::~WSClient()
{

}

void WSClient::OnRead()
{
	for(;;)
	{
		if(!_cmd)
		{
			if(readBuffer.GetSize() < 6)
				break;

			readBuffer.Read((uint8*)&_cmd, 2);
			readBuffer.Read((uint8*)&_remaining, 4);
		}

		if(_remaining && readBuffer.GetSize() < _remaining)
			break;

		if(_cmd == ISMSG_WOW_PACKET)
		{
			/*
			uint8 * ReceiveBuffer = (uint8*)GetReadBuffer().GetBufferStart();
						/ * optimized version for packet passing, to reduce latency! ;) * /
						uint32 sid = *(uint32*)&ReceiveBuffer[0];
						uint16 op  = *(uint16*)&ReceiveBuffer[4];
						uint32 sz  = *(uint32*)&ReceiveBuffer[6];			
						WorldSession * session = sClusterInterface.GetSession(sid);
						if(session != NULL)
						{
							WorldPacket * pck = new WorldPacket(op, sz);
							pck->resize(sz);
							memcpy((void*)pck->contents(), &ReceiveBuffer[10], sz);
							session->QueuePacket(pck);
						}
						readBuffer.Remove(sz + 10/ *header* /);
						_cmd = 0;
						continue;*/
			

			uint32 sid = 0;
			uint16 op = 0;
			uint32 sz = 0;
			GetReadBuffer().Read(&sid, 4);
			GetReadBuffer().Read(&op, 2);
			GetReadBuffer().Read(&sz, 4);

			WorldSession * session = sClusterInterface.GetSession(sid);
			if(session != NULL)
			{
				WorldPacket * pck = new WorldPacket(op, sz);
				if (sz > 0)
				{
					pck->resize(sz);
					GetReadBuffer().Read((void*)pck->contents(), sz);
				}
				if(session) session->QueuePacket(pck);
				else delete pck;
			}
			_cmd = 0;
			continue;
		}

		WorldPacket * pck = new WorldPacket(_cmd, _remaining);
		_cmd = 0;
		pck->resize(_remaining);
		readBuffer.Read((uint8*)pck->contents(), _remaining);

		/* we could handle auth here */
		switch(_cmd)
		{
		case ISMSG_AUTH_REQUEST:
			sClusterInterface.HandleAuthRequest(*pck);
			delete pck;
			break;
		default:
			sClusterInterface.QueuePacket(pck);
		}		
	}
}

void WSClient::OnConnect()
{
	sClusterInterface.SetSocket(this);
}

void WSClient::OnDisconnect()
{
	sClusterInterface.ConnectionDropped();
	sSocketGarbageCollector.QueueSocket(this);
}

void WSClient::SendPacket(WorldPacket * data)
{
	bool rv;
	uint32 size = data->size();
	uint16 opcode = data->GetOpcode();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)data->contents(), size);

	if(rv) BurstPush();
	BurstEnd();
}

#endif
