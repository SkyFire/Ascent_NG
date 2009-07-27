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

#include "RStdAfx.h"

SessionPacketHandler Session::Handlers[NUM_MSG_TYPES];
void Session::InitHandlers()
{
	memset(Handlers, 0, sizeof(void*) * NUM_MSG_TYPES);
	Handlers[CMSG_PLAYER_LOGIN] = &Session::HandlePlayerLogin;
	Handlers[CMSG_CHAR_ENUM] = &Session::HandleCharacterEnum;
	Handlers[CMSG_CHAR_CREATE] = &Session::HandleCharacterCreate;
	Handlers[CMSG_ITEM_QUERY_SINGLE] = &Session::HandleItemQuerySingleOpcode;
	Handlers[CMSG_NAME_QUERY] = &Session::HandleNameQueryOpcode;
}

void Session::HandleNameQueryOpcode(WorldPacket & pck)
{
}

void Session::HandleCharacterCreate(WorldPacket & pck)
{
	if(pck.size() < 10)
	{
		sClientMgr.DestroySession(GetSessionId());
		return;
	}

	std::string name;
	uint8 race, class_;

	pck >> name >> race >> class_;
	pck.rpos(0);

	if(!VerifyName(name.c_str(), name.length(), true))
	{
		GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x32");
		return;
	}

	if (sClientMgr.GetRPlayer(name) != NULL)
	{
		GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x32");
		return;
	}

	QueryResult * result = Database_Character->Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x51"); // You cannot use that name
			delete result;
			return;
		}
		delete result;
	}

	Instance* i = sClusterMgr.GetAnyInstance();

	if (i == NULL)
	{
		GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x32");
		return;
	}

	WorldPacket data(ISMSG_CREATE_PLAYER, 4+6+pck.size());
	data << GetAccountId() << pck.GetOpcode() << uint32(pck.size());
	data.resize(10 + pck.size());
	memcpy((void*)(data.contents() + 10), pck.contents(), pck.size());
	i->Server->SendPacket(&data);
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

void Session::Update()
{
	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_readQueue.Pop()))
	{
		opcode = pck->GetOpcode();

		/* can we handle it ourselves? */
		if(opcode < NUM_MSG_TYPES && Session::Handlers[opcode] != 0)
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

Session::~Session()
{
	if (m_server != NULL)
		m_server = NULL;

	WorldPacket data(ISMSG_SESSION_REMOVED, 4);
	data << m_sessionId;
	sClusterMgr.DistributePacketToAll(&data);

	if (m_socket != NULL)
		m_socket->Delete();

	WorldPacket * pck = NULL;
	while((pck = m_readQueue.Pop()))
	{
		delete pck;
	}
}
