/*
 * Scripts for Ascent MMORPG Server
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

//general defs
#define HORDE 1
#define ALLIANCE 0

#define SILITHYST_SPELL 29519

// gameobj
#define SILITHYST_MOUND		181597
#define SILITHYST_GEYSER	181598

// buffs
#define TRACES_OF_SILITHYST 29534
#define CENARION_FAVOR 30754

// how much we need to win?
#define SILITHYST_MAX 200

// areatriggers
#define ALLIANCE_RETURN 4162
#define HORDE_RETURN 4168

// quests
#define ALLIANCE_SILITHYST_QUEST 17090
#define HORDE_SILITHYST_QUEST 18199

// rewards
#define REWARD_HONOR 19

#define REWARD_REPUTATION_FACTION 609
#define REWARD_REPUTATION_VALUE 20


int32 silithyst_gathered[2] = { 0, 0 };

int32 winners = -1;
bool locked = false;

void InitWorldStates(MapMgr* pmgr)
{
	if(!pmgr)
		return;
	if(pmgr->GetMapId() == 1 ) // Kalimdor
	{
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_SILITHUS_SILITHYST_MAX, SILITHYST_MAX);
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_SILITHUS_GATHERED_ALLIANCE, 0);			
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_SILITHUS_GATHERED_HORDE, 0);
	}
}

void SilithusZoneHook(Player* plr, uint32 Zone, uint32 OldZone)
{
	if(!plr)
		return;

	if( Zone == ZONE_SILITHUS )
	{
		if( winners == plr->GetTeam() )
			plr->CastSpell(plr, dbcSpell.LookupEntry(CENARION_FAVOR), true);
	}
	else if( OldZone == ZONE_SILITHUS )
	{
		if( winners == plr->GetTeam() )
			plr->RemovePositiveAura(CENARION_FAVOR);
	}
}

void AreatriggerHook(Player* pPlayer, uint32 triggerID)
{
	if(!pPlayer)
		return;

	if( triggerID == ALLIANCE_RETURN || 
		triggerID == HORDE_RETURN )
	{
		if( pPlayer->HasAura( SILITHYST_SPELL ) ) // if we doesn't have it, do nothing
		{
			pPlayer->RemoveAura( SILITHYST_SPELL );
			
			/* Rewards */
			pPlayer->CastSpell(pPlayer, TRACES_OF_SILITHYST, true);
			pPlayer->m_honorPoints += REWARD_HONOR;
			pPlayer->ModStanding(REWARD_REPUTATION_FACTION, REWARD_REPUTATION_VALUE);
	
			uint32 quest = pPlayer->GetTeam() == ALLIANCE ? ALLIANCE_SILITHYST_QUEST : HORDE_SILITHYST_QUEST;
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(quest);
			if( qle )
				qle->SendQuestComplete();
			/***********/
			
			if( locked )
				return;
			silithyst_gathered[ pPlayer->GetTeam() ]++;
			uint32 worldstate = pPlayer->GetTeam() == ALLIANCE ? WORLDSTATE_SILITHUS_GATHERED_ALLIANCE : WORLDSTATE_SILITHUS_GATHERED_HORDE;
			pPlayer->GetMapMgr()->GetStateManager().UpdateWorldState( worldstate , silithyst_gathered[ pPlayer->GetTeam() ] );
			if( silithyst_gathered[ pPlayer->GetTeam() ] >= SILITHYST_MAX )
			{
				locked = true; // we win, so opposite team must wait for server restart
				winners = pPlayer->GetTeam();
				pPlayer->GetMapMgr()->CastSpellOnPlayers(winners, CENARION_FAVOR);
				pPlayer->GetMapMgr()->RemovePositiveAuraFromPlayers( winners == ALLIANCE ? HORDE : ALLIANCE, CENARION_FAVOR);
			}
		}
	}
}

class SilithystPickup : public GameObjectAIScript
{
public:
	SilithystPickup(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new SilithystPickup(GO); }

	void OnActivate(Player*  pPlayer)
	{
		if( pPlayer && !pPlayer->HasAura( SILITHYST_SPELL ) )
			pPlayer->CastSpell(pPlayer, SILITHYST_SPELL, true);

		if( _gameobject )
			_gameobject->Despawn(0);		
	}
};

void DropFlag(Player*  pPlayer, uint32 spellID)
{
	if( !pPlayer || spellID != SILITHYST_SPELL )
		return;
	uint32 triggerID = pPlayer->GetTeam() == ALLIANCE ? ALLIANCE_RETURN : HORDE_RETURN;
	// we have to use AreaTrigger.dbc here
	AreaTrigger* pAreaTrigger = AreaTriggerStorage.LookupEntry( triggerID );
	if( pAreaTrigger )
		if( pPlayer->CalcDistance(pAreaTrigger->x,pAreaTrigger->y,pAreaTrigger->z) > 10.0f )
		{
			GameObject* pGo = pPlayer->GetMapMgr()->GetInterface()->SpawnGameObject(SILITHYST_MOUND, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, true, 0, 0);
			if( pGo == NULL )
			{
				pGo->Destructor();
				pGo = NULL;
			}
		};
}

void SetupPvPSilithus(ScriptMgr *mgr)
{
	mgr->register_gameobject_script(SILITHYST_GEYSER, &SilithystPickup::Create);
	mgr->register_gameobject_script(SILITHYST_MOUND, &SilithystPickup::Create);
	
	mgr->register_hook(SERVER_HOOK_EVENT_ON_CONTINENT_CREATE, InitWorldStates);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, SilithusZoneHook);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_AREATRIGGER, AreatriggerHook);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_AURA_REMOVE, DropFlag);
	
	SpellEntry* sp = dbcSpell.LookupEntryForced( SILITHYST_SPELL );
	if( sp != NULL )
		sp->AuraInterruptFlags = AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN | AURA_INTERRUPT_ON_MOUNT;
}
