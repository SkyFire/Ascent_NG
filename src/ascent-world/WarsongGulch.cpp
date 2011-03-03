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

#include "StdAfx.h"

//										   <10 <20 <30 <40 <50 <60 <70  70
static int flagHonorTable[9]			= {  0,  5,  8, 14, 23, 38, 40, 40, 45 };
static int winHonorTable[9]				= {  0,  2,  4,  7, 11, 19, 20, 20, 25 };
static int extraCompleteHonorTable[9]	= {  0,  7, 12, 20, 34, 57, 59, 59, 65 }; // extras only for weekends
static int extraWinHonorTable[9]		= {  0,  5,  8, 14, 23, 38, 40, 40, 45 };

WarsongGulch::WarsongGulch(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	m_pvpData.clear();
	m_resurrectMap.clear();

	m_flagHolders[0] = m_flagHolders[1] = 0;

	m_FlagCaptureHonor = 2*HonorHandler::CalculateHonorPointsFormula(lgroup*10,lgroup*10);
	m_WinHonor = HonorHandler::CalculateHonorPointsFormula(lgroup*10,lgroup*10);
	m_CompleteHonor = 2*HonorHandler::CalculateHonorPointsFormula(lgroup*10,lgroup*10);

	m_flagAtBase[0] = m_flagAtBase[1] = 0;
	m_scores[0] = m_scores[1] = 0;

	m_playerCountPerTeam = 10;
	m_lgroup = lgroup;
}

void WarsongGulch::Init()
{
	CBattleground::Init();
	uint32 i;

	/* create the buffs */
	for(i = 0; i < 6; ++i)
		SpawnBuff(i);

	/* take note: these are swapped around for performance bonus */
	// warsong flag - horde base
	m_homeFlags[0] = SpawnGameObject(179831, 915.367f, 1433.78f, 346.089f, 3.17301f, 0, 210, 2.5f);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 24);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);

	// silverwing flag - alliance base
	m_homeFlags[1] = SpawnGameObject(179830, 1540.29f, 1481.34f, 352.64f, 3.17301f, 0,1314, 2.5f);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 24);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);

	// dropped flags
	m_dropFlags[1] = m_mapMgr->CreateGameObject(179786);
	if( m_dropFlags[1] == NULL || !m_dropFlags[1]->CreateFromProto(179785, 489, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		Log.Warning("WarsongGulch", "Could not create dropped flag 1");

	m_dropFlags[0] = m_mapMgr->CreateGameObject(179786);
	if(m_dropFlags[0] == NULL || !m_dropFlags[0]->CreateFromProto(179786, 489, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		Log.Warning("WarsongGulch", "Could not create dropped flag 0");

	for(i = 0; i < 2; ++i)
	{
		m_dropFlags[i]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
		m_dropFlags[i]->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.5f);
	}

}

WarsongGulch::~WarsongGulch()
{
	// gates are always spawned, so mapmgr will clean them up
	for(uint32 i = 0; i < 6; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && !m_buffs[i]->IsInWorld())
		{
			m_buffs[i]->Destructor();
		}
	}
	for (uint8 i = 0; i < 2; i++)
	{
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}

	m_pvpData.clear();
	m_resurrectMap.clear();
}

void WarsongGulch::HookOnAreaTrigger(Player* plr, uint32 id)
{
	int32 buffslot = -1;
	switch(id)
	{
	case 3686:	  // Speed
		buffslot = 0;
		break;
	case 3687:	  // Speed (Horde)
		buffslot = 1;
		break;
	case 3706:	  // Restoration
		buffslot = 2;
		break;
	case 3708:	  // Restoration (Horde)
		buffslot = 3;
		break;
	case 3707:	  // Berserking
		buffslot = 4;
		break;
	case 3709:	  // Berserking (Horde)
		buffslot = 5;
		break;
	}

	if(buffslot > -1)
	{
		if(m_buffs[buffslot] != 0 && m_buffs[buffslot]->IsInWorld())
		{
			/* apply the buff */
			SpellEntry * sp = dbcSpell.LookupEntry(m_buffs[buffslot]->GetInfo()->sound3);
			Spell* s(new Spell(plr, sp, true, NULL));
			SpellCastTargets targets(plr->GetGUID());
			s->prepare(&targets);

			/* despawn the gameobject (not delete!) */
			m_buffs[buffslot]->Despawn(BUFF_RESPAWN_TIME);
		}
		return;
	}

	if(((id == 3646 && plr->GetTeam() == 0) || (id == 3647 && plr->GetTeam() == 1)) && (plr->m_bgHasFlag && m_flagHolders[plr->GetTeam()] == plr->GetLowGUID()))
	{
		if(m_flagHolders[plr->GetTeam() ? 0 : 1] != 0 || m_dropFlags[plr->GetTeam() ? 0 : 1]->IsInWorld())
		{
			/* can't cap while flag dropped */
			return;
		}

		/* remove the bool from the player so the flag doesn't drop */
		m_flagHolders[plr->GetTeam()] = 0;
		plr->m_bgHasFlag = 0;

		/* remove flag aura from PlayerPointer */
		plr->RemoveAura(23333+(plr->GetTeam() * 2));

		/* capture flag points */
		plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAG_CAPTURES]++;

		PlaySoundToAll( plr->GetTeam() ? SOUND_HORDE_SCORES : SOUND_ALLIANCE_SCORES );

		if( plr->GetTeam() == 1 )
			SendChatMessage( CHAT_MSG_BG_SYSTEM_HORDE, plr->GetGUID(), "%s captured the Alliance flag!", plr->GetName() );
		else
			SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE, plr->GetGUID(), "%s captured the Horde flag!", plr->GetName() );

		m_mapMgr->GetStateManager().UpdateWorldState(plr->GetTeam() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1 );

		/* respawn the home flag */
		if( !m_homeFlags[plr->GetTeam()]->IsInWorld() )
			m_homeFlags[plr->GetTeam()]->PushToWorld(m_mapMgr);

		/* give each player on that team a bonus according to flagHonorTable */
		for(set< Player* >::iterator itr = m_players[plr->GetTeam()].begin(); itr != m_players[plr->GetTeam()].end(); ++itr)
		{
			(*itr)->m_bgScore.BonusHonor += m_FlagCaptureHonor;
			HonorHandler::AddHonorPointsToPlayer((*itr), m_FlagCaptureHonor);
		}

		m_scores[plr->GetTeam()]++;
		if(m_scores[plr->GetTeam()] == 3)
		{
			/* victory! */
			m_ended = true;
			m_losingteam = plr->GetTeam() ? 0 : 1;
			m_nextPvPUpdateTime = 0;

			sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
			sEventMgr.AddEvent(TO_CBATTLEGROUND(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

			m_mainLock.Acquire();
			/* add the marks of honor to all players */
			for(uint32 i = 0; i < 2; ++i)
			{
				for(set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				{
					(*itr)->Root();

					if( (*itr)->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK) )
						continue;
					
					uint32 item_count = (i == m_losingteam) ? 1 : 3;
					uint8 slot = (*itr)->GetItemInterface()->GetInventorySlotById(20558);
					if(slot != ITEM_NO_SLOT_AVAILABLE)
						item_count += (*itr)->GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, slot)->GetUInt32Value( ITEM_FIELD_STACK_COUNT );
					HonorHandler::UpdateCurrencyItem((*itr), 20558, item_count);

					if(i == m_losingteam)
					{
						(*itr)->m_bgScore.BonusHonor += m_CompleteHonor;
						HonorHandler::AddHonorPointsToPlayer((*itr), m_CompleteHonor);
					}
					else
					{
						(*itr)->m_bgScore.BonusHonor += m_CompleteHonor + m_WinHonor;
						HonorHandler::AddHonorPointsToPlayer((*itr), m_CompleteHonor + m_WinHonor);
						uint32 diff = abs(int32(m_scores[i] - m_scores[i ? 0 : 1]));
						(*itr)->GetAchievementInterface()->HandleAchievementCriteriaWinBattleground( m_mapMgr->GetMapId(), diff, ((uint32)UNIXTIME - m_startTime) / 1000, TO_CBATTLEGROUND(this));
					}
				}
			}
			m_mainLock.Release();
		}

		/* increment the score world state */
		m_mapMgr->GetStateManager().UpdateWorldState(plr->GetTeam() ? WORLDSTATE_WSG_HORDE_SCORE : WORLDSTATE_WSG_ALLIANCE_SCORE, m_scores[plr->GetTeam()]);

		UpdatePvPData();
	}

}

void WarsongGulch::DropFlag(Player* plr)
{
	if(!plr->m_bgHasFlag || m_dropFlags[plr->GetTeam()]->IsInWorld())
		return;

	plr->m_bgHasFlag = false;	// don't want to get here second time when doing RemoveAura

	plr->RemoveAura(23333+(plr->GetTeam() * 2));

	/* drop the flag! */
	m_dropFlags[plr->GetTeam()]->SetPosition(plr->GetPosition());
	m_dropFlags[plr->GetTeam()]->PushToWorld(m_mapMgr);
	m_flagHolders[plr->GetTeam()] = 0;
	m_mapMgr->GetStateManager().UpdateWorldState(plr->GetTeam() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);

	plr->CastSpell(plr, BG_RECENTLY_DROPPED_FLAG, true);

	sEventMgr.AddEvent( TO_WARSONGGULCH(this), &WarsongGulch::ReturnFlag, plr->GetTeam(), EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + plr->GetTeam(), 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

	if( plr->GetTeam() == 1 )
		SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE, plr->GetGUID(), "The Alliance flag was dropped by %s!", plr->GetName() );
	else
		SendChatMessage( CHAT_MSG_BG_SYSTEM_HORDE, plr->GetGUID(), "The Horde flag was dropped by %s!", plr->GetName() );
}

void WarsongGulch::HookFlagDrop(Player* plr, GameObject* obj)
{
	/* picking up a dropped flag */
	if(m_dropFlags[plr->GetTeam()] != obj)
	{
		/* are we returning it? */
		if( (obj->GetEntry() == 179785 && plr->GetTeam() == 0) ||
			(obj->GetEntry() == 179786 && plr->GetTeam() == 1) )
		{
			uint32 x = plr->GetTeam() ? 0 : 1;
			sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + plr->GetTeam());

			if( m_dropFlags[x]->IsInWorld() )
				m_dropFlags[x]->RemoveFromWorld(false);

			if(m_homeFlags[x]->IsInWorld() == false)
				m_homeFlags[x]->PushToWorld(m_mapMgr);

			plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAG_RETURNS]++;
			UpdatePvPData();

			if( plr->GetTeam() == 1 )
				SendChatMessage( CHAT_MSG_BG_SYSTEM_HORDE, plr->GetGUID(), "The Horde flag was returned to its base by %s!", plr->GetName() );
			else
				SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE, plr->GetGUID(), "The Alliance flag was returned to its base by %s!", plr->GetName() );

			m_mapMgr->GetStateManager().UpdateWorldState(plr->GetTeam() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);
			PlaySoundToAll(plr->GetTeam() ? SOUND_HORDE_RETURNED : SOUND_ALLIANCE_RETURNED);
		}
		return;
	}

	// check forcedreaction 1059, meaning do we recently dropped a flag?
	map<uint32,uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if (itr != plr->m_forcedReactions.end()) {
		return;
	}

	if( plr->GetTeam() == 0 )
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG);
	else
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + 1);

	if( m_dropFlags[plr->GetTeam()]->IsInWorld() )
		m_dropFlags[plr->GetTeam()]->RemoveFromWorld(false);

	m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
	plr->m_bgHasFlag = true;

	/* This is *really* strange. Even though the A9 create sent to the client is exactly the same as the first one, if
	 * you spawn and despawn it, then spawn it again it will not show. So we'll assign it a new guid, hopefully that
	 * will work.
	 * - Burlex
	 */
	m_dropFlags[plr->GetTeam()]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
	
	SpellEntry * pSp = dbcSpell.LookupEntry(23333 + (plr->GetTeam() * 2));
	Spell* sp(new Spell(plr, pSp, true, NULL));
	SpellCastTargets targets(plr->GetGUID());
	sp->prepare(&targets);
	m_mapMgr->GetStateManager().UpdateWorldState(plr->GetTeam() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 2);
}

void WarsongGulch::ReturnFlag(uint32 team)
{
	if(m_flagAtBase[team])
		return;

	if (m_dropFlags[team]->IsInWorld())
		m_dropFlags[team]->RemoveFromWorld(false);
	
	if( !m_homeFlags[team]->IsInWorld() )
		m_homeFlags[team]->PushToWorld(m_mapMgr);
	
	if( team )
		SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE, 0, "The Alliance flag was returned to its base!" );
	else
		SendChatMessage( CHAT_MSG_BG_SYSTEM_HORDE, 0, "The Horde flag was returned to its base!" );

	m_flagAtBase[team] = true;
}

void WarsongGulch::HookFlagStand(Player* plr, GameObject* obj)
{
	if(m_flagHolders[plr->GetTeam()] || m_homeFlags[plr->GetTeam()] != obj)
	{
		// cheater!
		return;
	}

	// check forcedreaction 1059, meaning do we recently dropped a flag?
	map<uint32,uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if (itr != plr->m_forcedReactions.end()) {
		return;
	}

	if( plr->m_CurrentVehicle )
		plr->m_CurrentVehicle->RemovePassenger(plr);

	if( plr->m_stealth )
		plr->RemoveAura( plr->m_stealth );

	if( plr->m_bgFlagIneligible )
		return;

	SpellEntry * pSp = dbcSpell.LookupEntry(23333 + (plr->GetTeam() * 2));
	Spell* sp(new Spell(plr, pSp, true, NULL));
	SpellCastTargets targets(plr->GetGUID());
	sp->prepare(&targets);

	/* set the flag holder */
	m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
	if(m_homeFlags[plr->GetTeam()]->IsInWorld())
		m_homeFlags[plr->GetTeam()]->RemoveFromWorld(false);

	plr->m_bgHasFlag = true;
	m_flagAtBase[plr->GetTeam()] = false;

	if( plr->GetTeam() == 1 )
		SendChatMessage( CHAT_MSG_BG_SYSTEM_HORDE, plr->GetGUID(), "The Alliance flag was picked up by %s!", plr->GetName() );
	else
		SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE, plr->GetGUID(), "The Horde flag was picked up by %s!", plr->GetName() );

	PlaySoundToAll(plr->GetTeam() ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE);
	m_mapMgr->GetStateManager().UpdateWorldState(plr->GetTeam() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 2);
}

void WarsongGulch::HookOnPlayerKill(Player* plr, Unit* pVictim)
{
	if(pVictim->IsPlayer())
	{
		plr->m_bgScore.KillingBlows++;
		UpdatePvPData();
	}
}

void WarsongGulch::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void WarsongGulch::OnAddPlayer(Player* plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
}

void WarsongGulch::OnRemovePlayer(Player* plr)
{
	/* drop the flag if we have it */
	if(plr->m_bgHasFlag)
		HookOnMount(plr);

	plr->RemoveAura(BG_PREPARATION);
}

LocationVector WarsongGulch::GetStartingCoords(uint32 Team)
{
	if(Team)		// Horde
		return LocationVector(933.989685f, 1430.735840f, 345.537140f, 3.141593f);
	else			// Alliance
		return LocationVector(1519.530273f, 1481.868408f, 352.023743f, 3.141593f);
}

void WarsongGulch::HookOnPlayerDeath(Player* plr)
{
	plr->m_bgScore.Deaths++;

	/* do we have the flag? */
	if(plr->m_bgHasFlag)
		plr->RemoveAura( 23333 + (plr->GetTeam() * 2) );

	UpdatePvPData();
}

void WarsongGulch::HookOnMount(Player* plr)
{
	/* do we have the flag? */
	if(plr->m_bgHasFlag)
		DropFlag( plr ); // Well that was better now wasn't it? :)
}

bool WarsongGulch::HookHandleRepop(Player* plr)
{
    LocationVector dest;
	if(plr->GetTeam())
		dest.ChangeCoords(1032.644775f, 1388.316040f, 340.559937f, 0.043200f);
	else
		dest.ChangeCoords(1423.218872f, 1554.663574f, 342.833801f, 3.124139f);
	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void WarsongGulch::SpawnBuff(uint32 x)
{
    switch(x)
	{
	case 0:
		m_buffs[x] = SpawnGameObject(179871, 1449.9296875f, 1470.70971679688f, 342.634552001953f, -1.64060950279236f, 0, 114, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		break;
	case 1:
		m_buffs[x] = SpawnGameObject(179899, 1005.17071533203f, 1447.94567871094f, 335.903228759766f, 1.64060950279236f, 0, 114, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		break;
	case 2:
		m_buffs[x] = SpawnGameObject(179904, 1317.50573730469f, 1550.85070800781f, 313.234375f, -0.26179963350296f, 0, 114, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		break;
	case 3:
		m_buffs[x] = SpawnGameObject(179906, 1110.45129394531f, 1353.65563964844f, 316.518096923828f, -0.68067866563797f, 0, 114, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		break;
	case 4:
		m_buffs[x] = SpawnGameObject(179905, 1320.09375f, 1378.78967285156f, 314.753234863281f, 1.18682384490967f, 0, 114, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		break;
	case 5:
		m_buffs[x] = SpawnGameObject(179907, 1139.68774414063f, 1560.28771972656f, 306.843170166016f, -2.4434609413147f, 0, 114, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		break;
	}
}

void WarsongGulch::OnCreate()
{
	/* add the buffs to the world */
	for(int i = 0; i < 6; ++i)
	{
		if(!m_buffs[i]->IsInWorld())
			m_buffs[i]->PushToWorld(m_mapMgr);
	}

	// Alliance Gates
	GameObject* gate = SpawnGameObject(179921, 1471.554688f, 1458.778076f, 362.633240f, 0, 33, 114, 2.33271f);
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179919, 1492.477783f, 1457.912354f, 342.968933f, 0, 33, 114, 2.68149f);
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179918, 1503.335327f, 1493.465820f, 352.188843f, 0, 33, 114, 2.26f);
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// Horde Gates
	gate = SpawnGameObject(179916, 949.1663208f, 1423.7717285f, 345.6241455f, 5.56f, 32, 114, 0.900901f);
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179917, 953.0507202f, 1459.8424072f, 340.6525573f, 4.02f, 32, 114, 0.854700f);   
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// create world state templates
	WorldStateManager& sm = m_mapMgr->GetStateManager();

	sm.CreateWorldState(0x8D8, 0);
	sm.CreateWorldState(0x8D7, 0);
	sm.CreateWorldState(0x8D6, 0);
	sm.CreateWorldState(0x8D5, 0);
	sm.CreateWorldState(0x8D4, 0);
	sm.CreateWorldState(0x8D3, 0);
	sm.CreateWorldState(0x60B, 0);
	sm.CreateWorldState(0x60A, 0);
	sm.CreateWorldState(0x609, 0);

	sm.CreateWorldState(WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY, 1);
	sm.CreateWorldState(WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);
	sm.CreateWorldState(WORLDSTATE_WSG_MAX_SCORE, 3);
	sm.CreateWorldState(WORLDSTATE_WSG_ALLIANCE_SCORE, 0);
	sm.CreateWorldState(WORLDSTATE_WSG_HORDE_SCORE, 0);

	/* spawn spirit guides */
	AddSpiritGuide(SpawnSpiritGuide(1423.218872f, 1554.663574f, 342.833801f, 3.124139f, 0));
	AddSpiritGuide(SpawnSpiritGuide(1032.644775f, 1388.316040f, 340.559937f, 0.043200f, 1));
}

void WarsongGulch::OnStart()
{
	for(uint32 i = 0; i < 2; ++i) {
		for(set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr) {
			(*itr)->RemoveAura(BG_PREPARATION);
		}
	}

	/* open the gates */
	for(list<GameObject* >::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
		(*itr)->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 0);
		(*itr)->Despawn(5000);
	}

	/* add the flags to the world */
	for(int i = 0; i < 2; ++i)
	{
		if( !m_homeFlags[i]->IsInWorld() )
			m_homeFlags[i]->PushToWorld(m_mapMgr);
	}

	SendChatMessage( CHAT_MSG_BG_SYSTEM_NEUTRAL, 0, "The flags are now placed at their bases." );

	/* correct? - burlex */
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	m_started = true;
}

void WarsongGulch::HookGenerateLoot(Player* plr, Corpse* pCorpse)
{
	// add some money
	float gold = ((float(plr->getLevel()) / 2.5f)+1) * 100.0f;			// fix this later
	gold *= sWorld.getRate(RATE_MONEY);

	// set it
	pCorpse->m_loot.gold = float2int32(gold);
}

void WarsongGulch::HookOnShadowSight() 
{
}

void WarsongGulch::SetIsWeekend(bool isweekend) 
{
	m_isWeekend = isweekend;
	if (isweekend)
	{
		m_FlagCaptureHonor = 2*HonorHandler::CalculateHonorPointsFormula(m_lgroup*10,m_lgroup*10);
		m_WinHonor = 3*HonorHandler::CalculateHonorPointsFormula(m_lgroup*10,m_lgroup*10);
		m_CompleteHonor = 4*HonorHandler::CalculateHonorPointsFormula(m_lgroup*10,m_lgroup*10);
	} else {
		m_FlagCaptureHonor = 2*HonorHandler::CalculateHonorPointsFormula(m_lgroup*10,m_lgroup*10);
		m_WinHonor = HonorHandler::CalculateHonorPointsFormula(m_lgroup*10,m_lgroup*10);
		m_CompleteHonor = 2*HonorHandler::CalculateHonorPointsFormula(m_lgroup*10,m_lgroup*10);
	}
}


