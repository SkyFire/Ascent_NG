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

				GetPlayer()->m_pendingBattleground[i]->PortPlayer(GetPlayer());
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
	uint32 battlegroundType;
	uint8 requestType; // 0 = ShowBattlefieldList, 1 = RequestBattlegroundInstanceInfo

	recv_data >> battlegroundType >> requestType;

	CHECK_INWORLD_RETURN;

	//if( GetPlayer()->HasBGQueueSlotOfType(type) == 4)
	//	return;
	
	BattlegroundManager.HandleBattlegroundListPacket(this, battlegroundType, false);
}

// Returns -1 if indeterminable.
int32 GetBattlegroundTypeFromCreature(CreaturePointer pCreature)
{
	if( pCreature->m_factionDBC->ID == 890 ) // Silverwing Sentinels
		return BATTLEGROUND_WARSONG_GULCH;

	if( pCreature->m_factionDBC->ID == 889 ) // Warsong Outriders
		return BATTLEGROUND_WARSONG_GULCH;

	if( pCreature->m_factionDBC->ID == 509 ) // League of Arathor
		return BATTLEGROUND_ARATHI_BASIN;

	if( pCreature->m_factionDBC->ID == 510 ) // The Defilers
		return BATTLEGROUND_ARATHI_BASIN;

	if( pCreature->m_factionDBC->ID == 730 ) // Stormpike Guard
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( pCreature->m_factionDBC->ID == 729 ) // The Frostwolf
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( string(pCreature->GetCreatureInfo()->Name).find("Eye of the Storm") != string::npos )
		return BATTLEGROUND_EYE_OF_THE_STORM;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Eye of the Storm") != string::npos )
		return BATTLEGROUND_EYE_OF_THE_STORM;

	if( string(pCreature->GetCreatureInfo()->Name).find("Warsong Gulch") != string::npos )
		return BATTLEGROUND_WARSONG_GULCH;

	if( string(pCreature->GetCreatureInfo()->Name).find("Arathi Basin") != string::npos )
		return BATTLEGROUND_ARATHI_BASIN;

	if( string(pCreature->GetCreatureInfo()->Name).find("Alterac Valley") != string::npos )
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( string(pCreature->GetCreatureInfo()->Name).find("Arena") != string::npos )
		return BATTLEGROUND_ARENA_2V2;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Warsong Gulch") != string::npos )
		return BATTLEGROUND_WARSONG_GULCH;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Arathi Basin") != string::npos )
		return BATTLEGROUND_ARATHI_BASIN;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Alterac Valley") != string::npos )
		return BATTLEGROUND_ALTERAC_VALLEY;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Arena") != string::npos )
		return BATTLEGROUND_ARENA_2V2;

	if( string(pCreature->GetCreatureInfo()->SubName).find("Strand of the Ancients") != string::npos )
		return BATTLEGROUND_STRAND_OF_THE_ANCIENTS;

	if( string(pCreature->GetCreatureInfo()->Name).find("Strand of the Ancients") != string::npos )
		return BATTLEGROUND_STRAND_OF_THE_ANCIENTS;

	return -1;
}

void WorldSession::SendBattlegroundList(CreaturePointer pCreature, uint32 mapid)
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
	CreaturePointer pCreature = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
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

	CreaturePointer psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
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
	CreaturePointer psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == NULL)
		return;

	_player->m_bg->QueuePlayerForResurrect(_player, psg);
}

void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket &recv_data)
{
	// empty opcode
	BattlegroundPointer bg = _player->m_bg;
	if(!_player->IsInWorld() || !bg)
		return;

	uint8 buf[100];
	StackPacket data( MSG_BATTLEGROUND_PLAYER_POSITIONS, buf, 100 );

	if(bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
	{
		uint32 count1 = 0;
		uint32 count2 = 0;

		PlayerPointer ap = objmgr.GetPlayer((CAST(WarsongGulch, bg))->GetAllianceFlagHolderGUID());
		if(ap) ++count2;

		PlayerPointer hp = objmgr.GetPlayer((CAST(WarsongGulch, bg))->GetHordeFlagHolderGUID());
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

		PlayerPointer ap = objmgr.GetPlayer((CAST(EyeOfTheStorm, bg))->GetFlagHolderGUID());
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

	PlayerPointer player =  _player->GetMapMgr()->GetPlayer( (uint32)guid );
	if( player == NULL )
		return;
	
	uint8 buf[100];
	StackPacket data( MSG_INSPECT_HONOR_STATS, buf, 100 );

	data << player->GetGUID() << (uint8)player->GetUInt32Value( PLAYER_FIELD_HONOR_CURRENCY );
	data << player->GetUInt32Value( PLAYER_FIELD_KILLS );
	data << player->GetUInt32Value( PLAYER_FIELD_TODAY_CONTRIBUTION );
	data << player->GetUInt32Value( PLAYER_FIELD_YESTERDAY_CONTRIBUTION );
	data << player->GetUInt32Value( PLAYER_FIELD_LIFETIME_HONORBALE_KILLS );

	SendPacket( &data );
}

void WorldSession::HandleInspectArenaStatsOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE( recv_data, 8 );
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	PlayerPointer player =  _player->GetMapMgr()->GetPlayer( (uint32)guid );
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


