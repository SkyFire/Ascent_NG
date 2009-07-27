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

WServerHandler WServer::PHandlers[IMSG_NUM_TYPES];

void WServer::InitHandlers()
{
	memset(PHandlers, 0, sizeof(void*) * IMSG_NUM_TYPES);
	PHandlers[ICMSG_REGISTER_WORKER] = &WServer::HandleRegisterWorker;
	PHandlers[ICMSG_WOW_PACKET] = &WServer::HandleWoWPacket;
	PHandlers[ICMSG_PLAYER_LOGIN_RESULT] = &WServer::HandlePlayerLoginResult;
	PHandlers[ICMSG_PLAYER_LOGOUT] = &WServer::HandlePlayerLogout;
	PHandlers[ICMSG_TELEPORT_REQUEST] = &WServer::HandleTeleportRequest;
	PHandlers[ICMSG_ERROR_HANDLER] = &WServer::HandleError;
	PHandlers[ICMSG_SWITCH_SERVER] = &WServer::HandleSwitchServer;
	PHandlers[ICMSG_SAVE_ALL_PLAYERS] = &WServer::HandleSaveAllPlayers;
	PHandlers[ICMSG_TRANSPORTER_MAP_CHANGE] = &WServer::HandleTransporterMapChange;
	PHandlers[ICMSG_PLAYER_TELEPORT] = &WServer::HandlePlayerTeleport;
	PHandlers[ICMSG_CREATE_PLAYER] = &WServer::HandleCreatePlayerResult;
}

WServer::WServer(uint32 id, WSSocket * s) : m_id(id), m_socket(s)
{

}

WServer::~WServer()
{
	sSocketGarbageCollector.QueueSocket(m_socket);
}

void WServer::HandleCreatePlayerResult(WorldPacket & pck)
{
	uint32 accountid;
	uint8 result;
	pck >> accountid >> result;

	//ok, we need session by account id... gay
	Session* s = sClientMgr.GetSessionByAccountId(accountid);
	if (s == NULL)
		return;

	s->GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, &result);
}

void WServer::HandlePlayerTeleport(WorldPacket & pck)
{
	//this is used to teleport to another player (gm appear/summon)
	uint8 result, method;
	uint32 sessionid, mapid, instanceid;
	LocationVector location;
	std::string player_name;
	uint32 player_guid;

	pck >> result >> method >> sessionid >> mapid >> instanceid >> location;

	Session* s=sClientMgr.GetSession(sessionid);
	if (s == NULL)
		return;

	RPlayerInfo* rp = NULL;
	if (result == 2)
	{
		//this is a callback, reserved
	}
	else if (result == 1)
	{
		pck >> player_name;
		rp = sClientMgr.GetRPlayer(player_name);
	}
	else if (result == 0)
	{
		pck >> player_guid;
		rp = sClientMgr.GetRPlayer(player_guid);
	}
	
	if (rp == NULL)
		return;

	Session* s2 = sClientMgr.GetSessionByRPInfo(rp);

	if (s2 == NULL)
		return;

	//send the info to the target's server, target's server will either handle it, or send info back (for say an appear command)
	WorldPacket data(ISMSG_PLAYER_TELEPORT);
	data << uint8(2) << method << sessionid << mapid << instanceid << location << s2->GetSessionId();
	s->GetServer()->SendPacket(&data);
}

void WServer::HandleTransporterMapChange(WorldPacket & pck)
{
	Log.Debug("WServer", "Recieved ICMSG_TRANSPORTER_MAP_CHANGE");
	uint32 transporterentry, mapid, oldmapid;
	float x, y, z;

	pck >> transporterentry >> mapid >> oldmapid >> x >> y >> z;

	Instance* dest = sClusterMgr.GetInstanceByMapId(mapid);

	if (dest == NULL)
		return;

	//ok so we need to send ISMSG_TRANSPORTER_MAP_CHANGE, then world server should SafeTeleport the player across :)
	WorldPacket data(ISMSG_TRANSPORTER_MAP_CHANGE, 20);
	data << transporterentry << mapid << x << y << z;
	dest->Server->SendPacket(&data);
}

void WServer::HandleSaveAllPlayers(WorldPacket & pck)
{
	//relay this to all world servers
	WorldPacket data(ISMSG_SAVE_ALL_PLAYERS, 1);
	data << uint8(0);
	sClusterMgr.DistributePacketToAll(&data);
}

void WServer::HandleSwitchServer(WorldPacket & pck)
{
	uint32 sessionid, guid, mapid, instanceid;
	LocationVector location;
	float o;

	pck >> sessionid >> guid >> mapid >> instanceid >> location >> o;
	Session* s=sClientMgr.GetSession(sessionid);

	if (s == NULL)
		return;

	//so, switch our server, fuck the old one
	s->SetNextServer();

	WorldPacket data;
	data.SetOpcode(ISMSG_PLAYER_LOGIN);
	data << guid << mapid << instanceid;
	data << s->GetAccountId() << s->GetAccountFlags() << s->GetSessionId();
	data << s->GetAccountPermissions() << s->GetAccountName();

	s->GetServer()->SendPacket(&data);
}

void WServer::HandleRegisterWorker(WorldPacket & pck)
{
	std::vector<uint32> maps;
	std::vector<uint32> instancedmaps;
	uint32 build;
	pck >> build >> maps >> instancedmaps;

	/* send a packed packet of all online players to this server */
	sClientMgr.SendPackedClientInfo(this);

	/* allocate initial instances for this worker */
	sClusterMgr.AllocateInitialInstances(this, maps);

	for (std::vector<uint32>::iterator itr=instancedmaps.begin(); itr!=instancedmaps.end(); ++itr)
	{
		Instance* i=new Instance;
		i->InstanceId = 0;
		i->MapId = (*itr);
		i->MapCount = 0;
		i->Server = this;
		sClusterMgr.InstancedMaps.insert(std::pair<uint32, Instance*>((*itr), i));
		Log.Debug("ClusterMgr", "Allocating instance prototype on map %u to worker %u", (*itr), GetID());
	}
}

void WServer::HandleWoWPacket(WorldPacket & pck)
{
	uint32 sessionid, size;
	uint16 opcode;

	/* get session */
	pck >> sessionid >> opcode >> size;
	Session * session = sClientMgr.GetSession(sessionid);
	if(!session) return;

	/* write it to that session's output buffer */
	WorldSocket * s = session->GetSocket();
	if(s) s->OutPacket(opcode, size, size ? ((const void*)(pck.contents() + 10)) : 0);
}

void WServer::HandlePlayerLogout(WorldPacket & pck)
{
	uint32 sessionid, guid;
	pck >> sessionid >> guid;
	RPlayerInfo * pi = sClientMgr.GetRPlayer(guid);
	Session * s = sClientMgr.GetSession(sessionid);
	if(pi && s)
	{
		/* tell all other servers this player has gone offline */
		WorldPacket data(ISMSG_DESTROY_PLAYER_INFO, 4);
		data << guid;
		sClusterMgr.DistributePacketToAll(&data, this);

		/* clear the player from the session */
		s->ClearCurrentPlayer();
		s->ClearServers();

		/* destroy the playerinfo struct here */
		sClientMgr.DestroyRPlayerInfo(guid);
	}
}

void WServer::HandleTeleportRequest(WorldPacket & pck)
{
	WorldPacket data(ISMSG_TELEPORT_RESULT, 100);
	RPlayerInfo * pi;
	Session * s;
	Instance * dest;
	uint32 mapid, sessionid, instanceid;

	/* this packet is only used upon changing main maps! */
	pck >> sessionid >> mapid >> instanceid;	

	s = sClientMgr.GetSession(sessionid);
	if(s)
	{
		pi = s->GetPlayer();
		ASSERT(pi);

		/* find the destination server */
		if(instanceid == 0)
			dest = sClusterMgr.GetInstanceByMapId(mapid);
		else
		{
			dest = sClusterMgr.GetInstanceByInstanceId(instanceid);

		}

		//try and find a prototype instance, and its server
		if (dest == NULL)
			dest = sClusterMgr.GetPrototypeInstanceByMapId(mapid);

		/* server up? */
		if(dest == NULL)
		{
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << uint32(0x02);	// INSTANCE_ABORT_NOT_FOUND
			s->SendPacket(&data);
		}
		else
		{
			/* server found! */
			LocationVector vec;
			pck >> vec >> vec.o;

			pi->MapId = mapid;
			pi->InstanceId = dest->InstanceId;
			pi->PositionX = vec.x;
			pi->PositionY = vec.y;

			if(dest->Server == s->GetServer())
			{
				/* we're not changing servers, the new instance is on the same server */
				data << sessionid << uint8(1) << mapid << instanceid << vec << vec.o;
				SendPacket(&data);
			}
			else
			{
				/* notify the old server to pack the player info together to send to the new server, and delete the player */
				data << sessionid << uint8(0) << mapid << instanceid << vec << vec.o;
				//cache this to next server and switch servers when were ready :P
				s->SetNextServer(dest->Server);
				SendPacket(&data);
			}

			data.Initialize(ISMSG_PLAYER_INFO);
			pi->Pack(data);
			sClusterMgr.DistributePacketToAll(&data, this);
		}
	}
}
void WServer::HandlePlayerLoginResult(WorldPacket & pck)
{
	uint32 guid, sessionid;
	uint8 result;
	pck >> guid >> sessionid >> result;
	if(result)
	{
		Log.Success("WServer", "Worldserver %u reports successful login of player %u", m_id, guid);
		Session * s = sClientMgr.GetSession(sessionid);
		if(s)
		{
			/* update server */
			s->SetNextServer();
			
			/* pack together a player info packet and distribute it to all the other servers */
			ASSERT(s->GetPlayer());
			
			WorldPacket data(ISMSG_PLAYER_INFO, 100);
			s->GetPlayer()->Pack(data);
			sClusterMgr.DistributePacketToAll(&data, this);
		}
	}
	else
	{
		Log.Error("WServer", "Worldserver %u reports failed login of player %u", m_id, guid);
		Session * s = sClientMgr.GetSession(sessionid);
		if(s)
		{
			s->ClearCurrentPlayer();
			s->ClearServers();
		}

		sClientMgr.DestroyRPlayerInfo(guid);
	}
}

void WServer::Update()
{
	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_recvQueue.Pop()))
	{
		opcode = pck->GetOpcode();
		if(opcode < IMSG_NUM_TYPES && WServer::PHandlers[opcode] != 0)
			(this->*WServer::PHandlers[opcode])(*pck);
		else
			Log.Error("WServer", "Unhandled packet %u\n", opcode);
	}
}

void WServer::HandleError(WorldPacket & pck)
{
	uint32 sessionid;
	uint8 errorcode;

	pck >> sessionid >> errorcode;

	switch (errorcode)
	{
	case 1: //no session
		sClientMgr.DestroySession(sessionid);
		break;
	}
}



