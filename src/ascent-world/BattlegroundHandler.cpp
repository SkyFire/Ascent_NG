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

void WorldSession::HandleBattlefieldPortOpcode(WorldPacket &recv_data)
{
	uint16 mapinfo, unk;
	uint8 action;
	uint32 bgtype;

	CHECK_INWORLD_RETURN;
	recv_data >> unk >> bgtype >> mapinfo >> action;

	if(action == 0)
	{
		uint32 BGQueueSlot = _player->GetBGQueueSlotByBGType(bgtype);
		_player->RemoveFromBattlegroundQueue(BGQueueSlot);
		//BattlegroundManager.RemovePlayerFromQueues(_player);
	}
	else
	{
		for(uint32 i = 0; i < 3; ++i)
		{
			if( GetPlayer()->m_pendingBattleground[i] && 
				(GetPlayer()->m_pendingBattleground[i]->GetType() == bgtype || bgtype == BATTLEGROUND_ARENA_5V5))
			{
				if( GetPlayer()->m_bg )
				{
					GetPlayer()->m_bg->RemovePlayer(GetPlayer(), true); // Send Logout = true so we can TP him now.
				}

				GetPlayer()->m_pendingBattleground[i]->PortPlayer(GetPlayer(),false);
				return;
			}
		}
	}
}

void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket &recv_data)
{
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 0);
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 1);
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 2);
}

void WorldSession::HandleBattlefieldListOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN;

	uint32 battlegroundType;
	uint8 requestType, unk1; // 0 = ShowBattlefieldList, 1 = RequestBattlegroundInstanceInfo

	recv_data >> battlegroundType >> requestType >> unk1;

	//if( GetPlayer()->HasBGQueueSlotOfType(type) == 4)
	//	return;
	
	BattlegroundManager.HandleBattlegroundListPacket(this, battlegroundType, false);
}

// Returns -1 if indeterminable.
int32 GetBattlegroundTypeFromCreature(Creature* pCreature)
{
	switch (pCreature->m_factionDBC->ID)
	{
		case 509: // League of Arathor
		case 510: // The Defilers
		{
			return BATTLEGROUND_ARATHI_BASIN;
		}break;

		case 729: // The Frostwolf
		case 730: // Stormpike Guard
		{
			return BATTLEGROUND_ALTERAC_VALLEY;
		}break;

		case 889: // Warsong Outriders
		case 890: // Silverwing Sentinels
		{
			return BATTLEGROUND_WARSONG_GULCH;
		}break;
	}

	// Arathi Basin
	if( string(pCreature->GetCreatureInfo()->Name).find("Arathi Basin") != string::npos )
		return BATTLEGROUND_ARATHI_BASIN;
	else if( string(pCreature->GetCreatureInfo()->SubName).find("Arathi Basin") != string::npos )
		return BATTLEGROUND_ARATHI_BASIN;

	//Arena
	if( string(pCreature->GetCreatureInfo()->Name).find("Arena") != string::npos )
		return BATTLEGROUND_ARENA_2V2;
	else if( string(pCreature->GetCreatureInfo()->SubName).find("Arena") != string::npos )
		return BATTLEGROUND_ARENA_2V2;

	//Alterac Valley
	if( string(pCreature->GetCreatureInfo()->Name).find("Alterac Valley") != string::npos )
		return BATTLEGROUND_ALTERAC_VALLEY;
	else if( string(pCreature->GetCreatureInfo()->SubName).find("Alterac Valley") != string::npos )
		return BATTLEGROUND_ALTERAC_VALLEY;

	//Eye of the Storm
	if( string(pCreature->GetCreatureInfo()->Name).find("Eye of the Storm") != string::npos )
		return BATTLEGROUND_EYE_OF_THE_STORM;
	else if( string(pCreature->GetCreatureInfo()->SubName).find("Eye of the Storm") != string::npos )
		return BATTLEGROUND_EYE_OF_THE_STORM;

	//Isle os Conquest
	if( string(pCreature->GetCreatureInfo()->Name).find("Isle of Conquest") != string::npos )
		return BATTLEGROUND_ISLE_OF_CONQUEST;

	else if( string(pCreature->GetCreatureInfo()->SubName).find("Isle of Conquest") != string::npos )
		return BATTLEGROUND_ISLE_OF_CONQUEST;

	//Strand of the Ancients
	if( string(pCreature->GetCreatureInfo()->SubName).find("Strand of the Ancients") != string::npos )
		return BATTLEGROUND_STRAND_OF_THE_ANCIENTS;

	else if( string(pCreature->GetCreatureInfo()->Name).find("Strand of the Ancients") != string::npos )
		return BATTLEGROUND_STRAND_OF_THE_ANCIENTS;

	//Warsong Gulch
	if( string(pCreature->GetCreatureInfo()->Name).find("Warsong Gulch") != string::npos )
		return BATTLEGROUND_WARSONG_GULCH;
	else if( string(pCreature->GetCreatureInfo()->SubName).find("Warsong Gulch") != string::npos )
		return BATTLEGROUND_WARSONG_GULCH;

	//General Battlemasters
	if( string(pCreature->GetCreatureInfo()->Name).find("Battlemaster") != string::npos )
		return BATTLEGROUND_ALL_BATTLEGROUND;
	else if( string(pCreature->GetCreatureInfo()->SubName).find("Battlemaster") != string::npos )
		return BATTLEGROUND_ALL_BATTLEGROUND;

	return -1;
}

void WorldSession::SendBattlegroundList(Creature* pCreature, uint32 mapid)
{
	if(!pCreature)
		return;

	int32 type = GetBattlegroundTypeFromCreature( pCreature );

	if( type == -1 )
		SystemMessage("Sorry, invalid battlemaster.");
	else
		BattlegroundManager.HandleBattlegroundListPacket(this, type, true);
}

void WorldSession::HandleBattleMasterHelloOpcode(WorldPacket &recv_data)
{
	uint64 guid;
	recv_data >> guid;

	CHECK_INWORLD_RETURN;
	Creature* pCreature = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
	if( pCreature == NULL )
		return;

	SendBattlegroundList( pCreature, 0 );
}

void WorldSession::HandleLeaveBattlefieldOpcode(WorldPacket &recv_data)
{
	if(_player->m_bg && _player->IsInWorld())
		_player->m_bg->RemovePlayer(_player, false);
}

void WorldSession::HandleAreaSpiritHealerQueryOpcode(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || !_player->m_bg) return;
	uint64 guid;
	recv_data >> guid;

	Creature* psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == NULL)
		return;
	
	uint32 restime = _player->m_bg->GetLastResurrect() + 30;
	if((uint32)UNIXTIME > restime)
		restime = 1000;
	else
		restime = (restime - (uint32)UNIXTIME) * 1000;

	WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
	data << guid << restime;
	SendPacket(&data);
}

void WorldSession::HandleAreaSpiritHealerQueueOpcode(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || !_player->m_bg) return;
	uint64 guid;
	recv_data >> guid;
	Creature* psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == NULL)
		return;

	_player->m_bg->QueuePlayerForResurrect(_player, psg);
}

void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket &recv_data)
{
	// empty opcode
	CBattleground* bg = _player->m_bg;
	if(!_player->IsInWorld() || !bg)
		return;

	uint8 buf[100];
	StackPacket data( MSG_BATTLEGROUND_PLAYER_POSITIONS, buf, 100 );

	if(bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
	{
		uint32 count1 = 0;
		uint32 count2 = 0;

		Player* ap = objmgr.GetPlayer((CAST(WarsongGulch, bg))->GetAllianceFlagHolderGUID());
		if(ap) ++count2;

		Player* hp = objmgr.GetPlayer((CAST(WarsongGulch, bg))->GetHordeFlagHolderGUID());
		if(hp) ++count2;

		data << count1;
		data << count2;
		if(ap)
		{
			data << (uint64)ap->GetGUID();
			data << (float)ap->GetPositionX();
			data << (float)ap->GetPositionY();
		}
		if(hp)
		{
			data << (uint64)hp->GetGUID();
			data << (float)hp->GetPositionX();
			data << (float)hp->GetPositionY();
		}

		SendPacket(&data);
	}
	else if(bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
	{
		uint32 count1 = 0;
		uint32 count2 = 0;

		Player* ap = objmgr.GetPlayer((CAST(EyeOfTheStorm, bg))->GetFlagHolderGUID());
		if(ap) ++count2;

		data << count1;
		data << count2;
		if(ap)
		{
			data << (uint64)ap->GetGUID();
			data << (float)ap->GetPositionX();
			data << (float)ap->GetPositionY();
		}

		SendPacket(&data);
	}
}

void WorldSession::HandleBattleMasterJoinOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->GetGroup() && _player->GetGroup()->m_isqueued)
	{
		SystemMessage("You are in a group that is already queued for a battleground or inside a battleground. Leave the group first.");
		return;
	}

	/* are we already in a queue? */
	if(_player->m_bgIsQueued)
		BattlegroundManager.RemovePlayerFromQueues(_player);

	if(_player->IsInWorld())
		BattlegroundManager.HandleBattlegroundJoin(this, recv_data);
}

void WorldSession::HandleArenaJoinOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->GetGroup() && _player->GetGroup()->m_isqueued)
	{
		SystemMessage("You are in a group that is already queued for a battleground or inside a battleground. Leave the group first.");
		return;
	}

	/* are we already in a queue? */
	if(_player->m_bgIsQueued)
		BattlegroundManager.RemovePlayerFromQueues(_player);

	uint32 bgtype=0;
	uint64 guid;
	uint8 arenacategory;
	uint8 as_group;
	uint8 rated_match;
	recv_data >> guid >> arenacategory >> as_group >> rated_match;
	switch(arenacategory)
	{
	case 0:		// 2v2
		bgtype = BATTLEGROUND_ARENA_2V2;
		break;

	case 1:		// 3v3
		bgtype = BATTLEGROUND_ARENA_3V3;
		break;

	case 2:		// 5v5
		bgtype = BATTLEGROUND_ARENA_5V5;
		break;
	}

	if(bgtype != 0)
		BattlegroundManager.HandleArenaJoin(this, bgtype, as_group, rated_match);
}

void WorldSession::HandleInspectHonorStatsOpcode( WorldPacket &recv_data )
{
	CHECK_PACKET_SIZE( recv_data, 8 );
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Player* player =  _player->GetMapMgr()->GetPlayer( (uint32)guid );
	if( player == NULL )
		return;
	
	uint8 buf[100];
	StackPacket data( MSG_INSPECT_HONOR_STATS, buf, 100 );

	/*data << player->GetGUID() << (uint8)player->GetUInt32Value( PLAYER_FIELD_HONOR_CURRENCY );
	data << player->GetUInt32Value( PLAYER_FIELD_KILLS );
	data << player->GetUInt32Value( PLAYER_FIELD_TODAY_CONTRIBUTION );
	data << player->GetUInt32Value( PLAYER_FIELD_YESTERDAY_CONTRIBUTION );
	data << player->GetUInt32Value( PLAYER_FIELD_LIFETIME_HONORBALE_KILLS );*/ //TODO: Fix - CMB

	SendPacket( &data );
}

void WorldSession::HandleInspectArenaStatsOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE( recv_data, 8 );
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Player* player =  _player->GetMapMgr()->GetPlayer( (uint32)guid );
	if( player == NULL )
		return;

	ArenaTeam *team;
	uint32 i;

	for( i = 0; i < 3; i++ )
	{
		team = player->m_playerInfo->arenaTeam[i];
		if( team != NULL )
		{
			ArenaTeamMember * tp = team->GetMember(player->m_playerInfo);
			uint8 buf[100];
			StackPacket data( MSG_INSPECT_ARENA_TEAMS, buf, 100 );
			data << player->GetGUID();
			data << uint8(team->m_type);
			data << team->m_id;
			data << team->m_stat_rating;
			if(tp) // send personal stats
			{
				data << tp->Played_ThisSeason;
				data << tp->Won_ThisSeason;
				data << tp->Played_ThisWeek;
				data << tp->PersonalRating;
			} else // send team stats
			{
				data << team->m_stat_gamesplayedweek;
				data << team->m_stat_gameswonweek;
				data << team->m_stat_gamesplayedseason;
				data << uint32(0);
			}
			SendPacket( &data );
		}
	}
}


void WorldSession::HandlePVPLogDataOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->m_bg)
		_player->m_bg->SendPVPData(_player);
}


