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

#define HORDE 0
#define ALLIANCE 1
#define ROUND_LENGTH 600 //in secs

StrandOfTheAncients::StrandOfTheAncients(MapMgrPointer mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
{
	Attackers = RandomUInt(2)-1;
	BattleRound = 1;
}

StrandOfTheAncients::~StrandOfTheAncients()
{
}

void StrandOfTheAncients::Init()
{
	CBattleground::Init();
}

bool StrandOfTheAncients::HookHandleRepop(PlayerPointer plr)
{
	return false;
}

void StrandOfTheAncients::HookOnAreaTrigger(PlayerPointer plr, uint32 id)
{
}

void StrandOfTheAncients::HookOnPlayerDeath(PlayerPointer plr)
{
}

void StrandOfTheAncients::HookFlagDrop(PlayerPointer plr, GameObjectPointer obj)
{
}

void StrandOfTheAncients::HookFlagStand(PlayerPointer plr, GameObjectPointer obj)
{
}

bool StrandOfTheAncients::HookSlowLockOpen(GameObjectPointer pGo, PlayerPointer pPlayer, SpellPointer pSpell)
{
	return true;
}

void StrandOfTheAncients::HookOnMount(PlayerPointer plr)
{
}

void StrandOfTheAncients::OnAddPlayer(PlayerPointer plr)
{
}

void StrandOfTheAncients::OnRemovePlayer(PlayerPointer plr)
{
}

void StrandOfTheAncients::OnCreate()
{
	WorldStateManager& sm = m_mapMgr->GetStateManager();
	sm.CreateWorldState( WORLDSTATE_SOTA_CAPTURE_BAR_DISPLAY, 0 );
	sm.CreateWorldState( WORLDSTATE_SOTA_CAPTURE_BAR_VALUE, 0 );
	PrepareRound();
	sm.CreateWorldState( WORLDSTATE_SOTA_BONUS_TIME, 0 );
	sm.CreateWorldState( WORLDSTATE_SOTA_TIMER_1, 0 );
	sm.CreateWorldState( WORLDSTATE_SOTA_TIMER_2, 0 );
	sm.CreateWorldState( WORLDSTATE_SOTA_TIMER_3, 0 );
}

void StrandOfTheAncients::HookOnPlayerKill(PlayerPointer plr, UnitPointer pVictim)
{
}

void StrandOfTheAncients::HookOnHK(PlayerPointer plr)
{
}

LocationVector StrandOfTheAncients::GetStartingCoords(uint32 Team)
{
	uint32 sTeam = ( Team == Attackers ? 1 : 0 );
	return LocationVector( SOTAStartLocations[sTeam][0], 
		SOTAStartLocations[sTeam][1],
		SOTAStartLocations[sTeam][2] );
}

void StrandOfTheAncients::OnStart()
{
	SetTime(ROUND_LENGTH, 0);
	sEventMgr.AddEvent( TO_SOTA( shared_from_this() ), &StrandOfTheAncients::TimeTick, EVENT_SOTA_TIMER, MSTIME_SECOND * 5, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void StrandOfTheAncients::HookGenerateLoot(PlayerPointer plr, CorpsePointer pCorpse)
{
}

void StrandOfTheAncients::HookOnShadowSight() 
{
}

void StrandOfTheAncients::SetIsWeekend(bool isweekend) 
{
}

// time in seconds
void StrandOfTheAncients::SetTime(uint32 secs, uint32 WorldState)
{
	uint32 minutes = secs / TIME_MINUTE;
	uint32 seconds = secs % TIME_MINUTE;
	uint32 digits[3];
	digits[0] = minutes;
	digits[1] = seconds / 10;
	digits[2] = seconds % 10;

	//m_mapMgr->GetStateManager().UpdateWorldState( WorldState, 1 );
	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_SOTA_TIMER_1, digits[0] );
	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_SOTA_TIMER_2, digits[1] );
	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_SOTA_TIMER_3, digits[2] );
	SetRoundTime( secs );
}

void StrandOfTheAncients::PrepareRound()
{
	m_mapMgr->GetStateManager().CreateWorldState( WORLDSTATE_SOTA_ALLIANCE_DEFENDER, Attackers == HORDE ? 1 : 0 );
	m_mapMgr->GetStateManager().CreateWorldState( WORLDSTATE_SOTA_ALLIANCE_ATTACKER, Attackers == HORDE ? 0 : 1 );
};

void StrandOfTheAncients::TimeTick()
{
	SetTime(GetRoundTime() - 5,0);
	if(GetRoundTime() == 0)
	{
		sEventMgr.RemoveEvents(TO_SOTA( shared_from_this() ), EVENT_SOTA_TIMER);
	}
};
