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

#ifndef _VOICECHATCLIENTSOCKET_H
#define _VOICECHATCLIENTSOCKET_H

#ifdef VOICE_CHAT

class VoiceChatClientSocket : public Socket
{
	uint16 op;
	uint16 remaining;
public:
	VoiceChatClientSocket(uint32 fd);
	void OnDisconnect();
	void OnRead();
	void SendPacket(WorldPacket* data);
	time_t next_ping;
	time_t last_pong;
};

#endif		// VOICE_CHAT
#endif		// _VOICECHATCLIENTSOCKET_H

