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

#ifndef _VOICECHATHANDLER_H
#define _VOICECHATHANDLER_H

#ifdef VOICE_CHAT

#include "../ascent-voicechat/ascent_opcodes.h"
#include "VoiceChatClientSocket.h"

class VoiceChatClientSocket;
struct VoiceChatChannelRequest
{
	string channel_name;
	uint32 team;
	uint32 id;
	uint32 groupid;
};

struct VoiceChannel
{
	void* miscPointer;
	uint8 type;
	uint8 team;
	uint32 channelId;
};

class VoiceChatHandler : public Singleton<VoiceChatHandler>
{
	VoiceChatClientSocket * m_client;
	Mutex m_lock;
	vector<VoiceChatChannelRequest> m_requests;
	uint32 request_high;
	uint32 ip;
	uint16 port;
	time_t next_connect;
	bool enabled;
	string ip_s;
	HM_NAMESPACE::hash_map<uint32, VoiceChannel*> m_voiceChannels;
public:
	VoiceChatHandler();
	void Startup();
	void Update();
	void SocketDisconnected();
	void OnRead(WorldPacket* pck);

	ASCENT_INLINE uint32 GetVoiceServerIP() { return ip; }
	ASCENT_INLINE uint16 GetVoiceServerPort() { return port; }
	void CreateVoiceChannel(Channel * chn);
	void DestroyVoiceChannel(Channel * chn);
	bool CanCreateVoiceChannel(Channel * chn);
	bool CanUseVoiceChat();

	void CreateGroupChannel(Group * pGroup);
	void DestroyGroupChannel(Group * pGroup);
	void ActivateChannelSlot(uint16 channel_id, uint8 slot_id);
	void DeactivateChannelSlot(uint16 channel_id, uint8 slot_id);
};

#define sVoiceChatHandler VoiceChatHandler::getSingleton()

#endif		// VOICE_CHAT
#endif		// _VOICECHATHANDLER_H
