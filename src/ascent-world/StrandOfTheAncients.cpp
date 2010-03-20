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
#define ROUND_LENGTH 600 //in secs

StrandOfTheAncients::StrandOfTheAncients(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
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

bool StrandOfTheAncients::HookHandleRepop(Player* plr)
{
	return false;
}

void StrandOfTheAncients::HookOnAreaTrigger(Player* plr, uint32 id)
{
}

void StrandOfTheAncients::HookOnPlayerDeath(Player* plr)
{
}

void StrandOfTheAncients::HookFlagDrop(Player* plr, GameObject* obj)
{
}

void StrandOfTheAncients::HookFlagStand(Player* plr, GameObject* obj)
{
}

bool StrandOfTheAncients::HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell)
{
	return true;
}

void StrandOfTheAncients::HookOnMount(Player* plr)
{
}

void StrandOfTheAncients::OnAddPlayer(Player* plr)
{
}

void StrandOfTheAncients::OnRemovePlayer(Player* plr)
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

void StrandOfTheAncients::HookOnPlayerKill(Player* plr, Unit* pVictim)
{
}

void StrandOfTheAncients::HookOnHK(Player* plr)
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
	sEventMgr.AddEvent( TO_SOTA( (this) ), &StrandOfTheAncients::TimeTick, EVENT_SOTA_TIMER, MSTIME_SECOND * 5, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void StrandOfTheAncients::HookGenerateLoot(Player* plr, Corpse* pCorpse)
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
		sEventMgr.RemoveEvents(TO_SOTA(this), EVENT_SOTA_TIMER);
	}
};
