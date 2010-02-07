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

#include "StdAfx.h"
#ifndef CLUSTERING
initialiseSingleton( ChannelMgr );

void WorldSession::HandleChannelJoin(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	uint32 dbc_id = 0;
	uint16 crap;		// crap = some sort of channel type?
	uint32 i;
	Channel * chn;

	recvPacket >> dbc_id >> crap;
	recvPacket >> channelname;
	recvPacket >> pass;

	if(!stricmp(channelname.c_str(), "LookingForGroup") && !sWorld.m_lfgForNonLfg)
	{
		// make sure we have lfg dungeons
		for(i = 0; i < 3; ++i)
		{
			if(_player->LfgDungeonId[i] != 0)
				break;
		}

		if(i == 3)
			return;		// don't join lfg
	}

	if( sWorld.GmClientChannel.size() && !stricmp(sWorld.GmClientChannel.c_str(), channelname.c_str()) && !GetPermissionCount())
		return;
	
	chn = channelmgr.GetCreateChannel(channelname.c_str(), _player, dbc_id);
	if(chn == NULL)
		return;

	chn->AttemptJoin(_player, pass.c_str());
	DEBUG_LOG("ChannelJoin", "%s", channelname.c_str());
}

void WorldSession::HandleChannelLeave(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	uint32 code = 0;
	Channel * chn;

	recvPacket >> code;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->Part(_player, false);
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;

	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	recvPacket >> pass;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if( chn != NULL )
		chn->Password(_player, pass.c_str());
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->SetOwner(_player, plr);
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if( chn != NULL )
		chn->GetOwner(_player);
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->GiveModerator(_player, plr);
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->TakeModerator(_player, plr);
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Mute(_player, plr);
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Unmute(_player, plr);
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Invite(_player, plr);
}
void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Kick(_player, plr, false);
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Kick(_player, plr, true);
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	PlayerInfo * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayerInfoByName(newp.c_str());
	if( chn != NULL && plr != NULL )
		chn->Unban(_player, plr);
}

void WorldSession::HandleChannelAnnounce(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->Announce(_player);
}

void WorldSession::HandleChannelModerate(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->Moderate(_player);
}

void WorldSession::HandleChannelRosterQuery(WorldPacket & recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->List(_player);
}

void WorldSession::HandleChannelNumMembersQuery(WorldPacket & recvPacket)
{
	string channel_name;
	WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, recvPacket.size() + 4);
	Channel *chn;
	recvPacket >> channel_name;
	chn = channelmgr.GetChannel(channel_name.c_str(), _player);
	if( chn != NULL )
	{
		data << channel_name;
		data << uint8(chn->m_flags);
		data << uint32(chn->GetNumMembers());
		SendPacket(&data);
	}
}

#endif