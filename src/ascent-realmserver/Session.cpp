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

SessionPacketHandler Session::Handlers[NUM_MSG_TYPES];
void Session::InitHandlers()
{
	memset(Handlers, 0, sizeof(void*) * NUM_MSG_TYPES);
	Handlers[CMSG_PLAYER_LOGIN] = &Session::HandlePlayerLogin;
	Handlers[CMSG_CHAR_ENUM] = &Session::HandleCharacterEnum;
	Handlers[CMSG_ITEM_QUERY_SINGLE] = &Session::HandleItemQuerySingleOpcode;
	Handlers[CMSG_REALM_SPLIT] = &Session::HandleRealmSplitQuery;
	Handlers[CMSG_CREATURE_QUERY] = &Session::HandleCreatureQueryOpcode;
	Handlers[CMSG_GAMEOBJECT_QUERY] = &Session::HandleGameObjectQueryOpcode;
	Handlers[CMSG_ITEM_NAME_QUERY] = &Session::HandleItemNameQueryOpcode;
	Handlers[CMSG_PAGE_TEXT_QUERY] = &Session::HandlePageTextQueryOpcode;
	Handlers[CMSG_QUERY_TIME] = &Session::HandleQueryTimeOpcode;
	Handlers[CMSG_NAME_QUERY] = &Session::HandleNameQueryOpcode;
	// Channels
	Handlers[CMSG_JOIN_CHANNEL]	= &Session::HandleChannelJoin;
	Handlers[CMSG_LEAVE_CHANNEL] = &Session::HandleChannelLeave;
	Handlers[CMSG_CHANNEL_LIST]	= &Session::HandleChannelList;
	Handlers[CMSG_CHANNEL_PASSWORD]	= &Session::HandleChannelPassword;
	Handlers[CMSG_CHANNEL_SET_OWNER] = &Session::HandleChannelSetOwner;
	Handlers[CMSG_CHANNEL_OWNER] = &Session::HandleChannelOwner;
	Handlers[CMSG_CHANNEL_MODERATOR] = &Session::HandleChannelModerator;
	Handlers[CMSG_CHANNEL_UNMODERATOR] = &Session::HandleChannelUnmoderator;
	Handlers[CMSG_CHANNEL_MUTE]	= &Session::HandleChannelMute;
	Handlers[CMSG_CHANNEL_UNMUTE] = &Session::HandleChannelUnmute;
	Handlers[CMSG_CHANNEL_INVITE] = &Session::HandleChannelInvite;
	Handlers[CMSG_CHANNEL_KICK]	= &Session::HandleChannelKick;
	Handlers[CMSG_CHANNEL_BAN] = &Session::HandleChannelBan;
	Handlers[CMSG_CHANNEL_UNBAN] = &Session::HandleChannelUnban;
	Handlers[CMSG_CHANNEL_ANNOUNCEMENTS] = &Session::HandleChannelAnnounce;
	Handlers[CMSG_CHANNEL_MODERATE]	= &Session::HandleChannelModerate;
	Handlers[CMSG_GET_CHANNEL_MEMBER_COUNT]	= &Session::HandleChannelNumMembersQuery;
	Handlers[CMSG_CHANNEL_DISPLAY_LIST]	= &Session::HandleChannelRosterQuery;
	Handlers[CMSG_MESSAGECHAT] = &Session::HandleMessagechatOpcode;
}

Session::Session(uint32 id) : m_sessionId(id)
{
	deleted = false;
	m_socket = 0;
	m_server = 0;
	m_accountId = 0;
	m_currentPlayer = 0;
	m_latency = 0;
	m_accountFlags = 0;
	m_build = 0;
	m_nextServer = 0;
}

Session::~Session()
{
}

void Session::Update()
{
	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_readQueue.Pop()))
	{
		opcode = pck->GetOpcode();

		/* can we handle it ourselves? */
		if(Session::Handlers[opcode] != 0)
		{
			(this->*Session::Handlers[opcode])(*pck);
		}
		else
		{
			/* no? pass it back to the worker server for handling. */
			if(m_server) m_server->SendWoWPacket(this, pck);
		}
		delete pck;
	}
}
