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
#include "ChannelMgr.h"

initialiseSingleton( ChannelMgr );

void Session::HandleChannelJoin(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	uint32 dbc_id = 0;
	uint16 unk;		// some sort of channel type?
	Channel * chn;

	recvPacket >> dbc_id >> unk;
	recvPacket >> channelname;
	recvPacket >> pass;

	if(!stricmp(channelname.c_str(), "LookingForGroup") && !sRMaster.m_lfgForNonLfg)
	{
		WorldPacket data(ISMSG_CHANNEL_LFG_DUNGEON_STATUS_REQUEST, 4+4+2+channelname.size()+pass.size());
		data << m_currentPlayer->Guid;
		data << dbc_id;
		data << unk;
		data << channelname;
		data << pass;
		GetServer()->SendPacket(&data);
		return;
	}


	if( sRMaster.GmClientChannel.size() && !stricmp(sRMaster.GmClientChannel.c_str(), channelname.c_str()) && !m_GMPermissions.size())
		return;
		

	chn = channelmgr.GetCreateChannel(channelname.c_str(), m_currentPlayer, dbc_id);
	if(chn == NULL)
		return;

	chn->AttemptJoin(m_currentPlayer, pass.c_str());
	DEBUG_LOG("ChannelJoin", "%s, unk %u", channelname.c_str(), unk);
}

void Session::HandleChannelLeave(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	uint32 code = 0;
	Channel * chn;

	recvPacket >> code;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if( chn != NULL )
		chn->Part(m_currentPlayer, false);
}

void Session::HandleChannelList(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;

	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if( chn != NULL )
		chn->List(m_currentPlayer);
}

void Session::HandleChannelPassword(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	recvPacket >> pass;
	chn = channelmgr.GetChannel(channelname.c_str(),m_currentPlayer);
	if( chn != NULL )
		chn->Password(m_currentPlayer, pass.c_str());
}

void Session::HandleChannelSetOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->SetOwner(m_currentPlayer, plr);
}

void Session::HandleChannelOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),m_currentPlayer);
	if( chn != NULL )
		chn->GetOwner(m_currentPlayer);
}

void Session::HandleChannelModerator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->GiveModerator(m_currentPlayer, plr);
}

void Session::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->TakeModerator(m_currentPlayer, plr);
}

void Session::HandleChannelMute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->Mute(m_currentPlayer, plr);
}

void Session::HandleChannelUnmute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->Unmute(m_currentPlayer, plr);
}

void Session::HandleChannelInvite(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->Invite(m_currentPlayer, plr);
}
void Session::HandleChannelKick(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->Kick(m_currentPlayer, plr, false);
}

void Session::HandleChannelBan(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->Kick(m_currentPlayer, plr, true);
}

void Session::HandleChannelUnban(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp);
	if( chn != NULL && plr != NULL )
		chn->Unban(m_currentPlayer, plr);
}

void Session::HandleChannelAnnounce(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if( chn != NULL )
		chn->Announce(m_currentPlayer);
}

void Session::HandleChannelModerate(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if( chn != NULL )
		chn->Moderate(m_currentPlayer);
}

void Session::HandleChannelRosterQuery(WorldPacket & recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if( chn != NULL )
		chn->List(m_currentPlayer);
}

void Session::HandleChannelNumMembersQuery(WorldPacket & recvPacket)
{
	string channel_name;
	WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, recvPacket.size() + 4);
	Channel *chn;
	recvPacket >> channel_name;
	chn = channelmgr.GetChannel(channel_name.c_str(), m_currentPlayer);
	if( chn != NULL )
	{
		data << channel_name;
		data << uint8(chn->m_flags);
		data << uint32(chn->GetNumMembers());
		SendPacket(&data);
	}
}

