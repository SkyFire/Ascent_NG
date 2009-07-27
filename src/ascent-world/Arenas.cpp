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

#define ARENA_PREPARATION 32727
#define GREEN_TEAM 0
#define GOLD_TEAM 1

Arena::Arena( MapMgrPointer mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	m_pvpData.clear();
	m_resurrectMap.clear();

	m_started = false;
	m_shadowsightspawned = false;
	m_playerCountPerTeam = players_per_side;
	switch(t)
	{
	case BATTLEGROUND_ARENA_5V5:
		m_arenateamtype=2;
		break;

	case BATTLEGROUND_ARENA_3V3:
		m_arenateamtype=1;
		break;
		
	case BATTLEGROUND_ARENA_2V2:
		m_arenateamtype=0;
		break;

	default:
		m_arenateamtype=0;
		break;
	}
	rated_match=false;

	m_buffs[0] = m_buffs[1] = NULLGOB;
	m_playersCount[0] = m_playersCount[1] = 0;

	m_playersAlive = hashmap_new();
	m_players2[0] = hashmap_new();
	m_players2[1] = hashmap_new();
	m_teams[0] = m_teams[1] = -1;
	m_deltaRating[0] = m_deltaRating[1] = 0;
}

void Arena::Init()
{
	CBattleground::Init();
}

Arena::~Arena()
{
	int i;

	for(i = 0; i < 2; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && m_buffs[i]->IsInWorld()==false)
		{
			m_buffs[i]->Destructor();
			m_buffs[i] = NULLGOB;
		}
	}

	if (m_playersAlive) {
		hashmap_free(m_playersAlive);
		m_playersAlive = NULL;
	}

	for (i=0; i<2; i++) {
		if (m_players2[i]) {
			hashmap_free(m_players2[i]);
			m_players2[i] = NULL;
		}
	}
}

void Arena::OnAddPlayer(PlayerPointer plr)
{
	plr->m_deathVision = true;

	if( plr->m_isGmInvisible )
		return;

	// remove all buffs (exclude talents, include flasks)
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(plr->m_auras[x])
		{
			if(plr->m_auras[x] && !plr->m_auras[x]->GetSpellProto()->DurationIndex && plr->m_auras[x]->GetSpellProto()->Flags4 & CAN_PERSIST_AND_CASTED_WHILE_DEAD)
				continue;
			else
			{
				plr->m_auras[x]->Remove();
			}
		}
	}
	plr->GetItemInterface()->RemoveAllConjured();
	plr->ResetAllCooldowns();

	if( !m_started )
		plr->CastSpell(plr, ARENA_PREPARATION, true);

	m_playersCount[plr->GetTeam()]++;
	UpdatePlayerCounts();

	/* Add the green/gold team flag */
	AuraPointer aura(new Aura(dbcSpell.LookupEntry(32724+plr->m_bgTeam), -1, plr, plr));
	plr->AddAura(aura);
	
	/* Set FFA PvP Flag */
	plr->SetFFAPvPFlag();

	hashmap_put(m_playersAlive, plr->GetLowGUID(), (any_t)1);
	if(Rated())
	{
		// Store the players who join so that we can change their rating even if they leave before arena finishes
		hashmap_put(m_players2[plr->GetTeam()], plr->GetLowGUID(), (any_t)1);
		if(m_teams[plr->GetTeam()] == -1 && plr->m_playerInfo && plr->m_playerInfo->arenaTeam[m_arenateamtype] != NULL)
		{
			m_teams[plr->GetTeam()] = plr->m_playerInfo->arenaTeam[m_arenateamtype]->m_id;
		}
	}
}

void Arena::OnRemovePlayer(PlayerPointer plr)
{
	/* remove arena readyness buff */
	plr->m_deathVision = false;

	if( plr->m_isGmInvisible )
		return;

	plr->RemoveAura(ARENA_PREPARATION);

	/* plr left arena, call HookOnPlayerDeath as if he died */
	HookOnPlayerDeath(plr);
	
	plr->RemoveAura(32724+plr->m_bgTeam);
	plr->RemoveFFAPvPFlag();
	plr->m_bgRatedQueue = false;
}

void Arena::HookOnPlayerKill(PlayerPointer plr, UnitPointer pVictim)
{
	if( !pVictim->IsPlayer() )
		return;

	plr->m_bgScore.KillingBlows++;
	UpdatePlayerCounts();
}

void Arena::HookOnHK(PlayerPointer plr)
{
	plr->m_bgScore.HonorableKills++;
}

void Arena::HookOnPlayerDeath(PlayerPointer plr)
{
	ASSERT(plr != NULL);

	if (hashmap_get(m_playersAlive, plr->GetLowGUID(), NULL) == MAP_OK) {
		m_playersCount[plr->GetTeam()]--;
		UpdatePlayerCounts();
		hashmap_remove(m_playersAlive, plr->GetLowGUID());
	}
}

void Arena::OnCreate()
{
	GameObjectPointer obj;
	WorldStateManager &sm = m_mapMgr->GetStateManager();

	switch(m_mapMgr->GetMapId())
	{
		/* loraedeon */
	case 572:
		{
			obj = SpawnGameObject(185917, 1278.647705f, 1730.556641f, 31.605574f, 1.68f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			m_gates.insert(obj);

			obj = SpawnGameObject(185918, 1293.560791f, 1601.937988f, 31.605574f, 4.86f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			m_gates.insert(obj);

			m_pcWorldStates[GREEN_TEAM] = WORLDSTATE_ARENA_LORDAERON_GREEN_PLAYER_COUNT;
			m_pcWorldStates[GOLD_TEAM] = WORLDSTATE_ARENA_LORDAERON_GOLD_PLAYER_COUNT;
			sm.CreateWorldState(WORLDSTATE_ARENA_LORDAERON_SCORE_SHOW, 1);
		}break;

		/* blades edge arena */
	case 562:
		{
			obj = SpawnGameObject(183972, 6177.707520f, 227.348145f, 3.604374f, -2.260201f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(183973, 6189.546387f, 241.709854f, 3.101481f, 0.881392f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			m_gates.insert(obj);

			obj = SpawnGameObject(183970, 6299.115723f, 296.549438f, 3.308032f, 0.881392f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(183971, 6287.276855f, 282.187714f, 3.810925f, -2.260201f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			m_gates.insert(obj);

			m_pcWorldStates[GREEN_TEAM] = WORLDSTATE_ARENA_BLADESEDGE_GREEN_PLAYER_COUNT;
			m_pcWorldStates[GOLD_TEAM] = WORLDSTATE_ARENA_BLADESEDGE_GOLD_PLAYER_COUNT;
			sm.CreateWorldState(WORLDSTATE_ARENA_BLADESEDGE_SCORE_SHOW, 1);
		}break;

		/* nagrand arena */
	case 559:
		{
			obj = SpawnGameObject(183979, 4090.064453f, 2858.437744f, 10.236313f, 0.492805f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(183980, 4081.178955f, 2874.970459f, 12.391714f, 0.492805f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			m_gates.insert(obj);

			obj = SpawnGameObject(183977, 4023.709473f, 2981.776611f, 10.701169f, -2.648788f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(183978, 4031.854248f, 2966.833496f, 12.646200f, -2.648788f, 32, 1375, 1.0f);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
			obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
			m_gates.insert(obj);

			m_pcWorldStates[GREEN_TEAM] = WORLDSTATE_ARENA_NAGRAND_GREEN_PLAYER_COUNT;
			m_pcWorldStates[GOLD_TEAM] = WORLDSTATE_ARENA_NAGRAND_GOLD_PLAYER_COUNT;
			sm.CreateWorldState(WORLDSTATE_ARENA_NAGRAND_SCORE_SHOW, 1);
		}break;
		/* Dalaran Sewers */
	case 617:
		{
			obj = SpawnGameObject(192643, 1256.33996582031f, 770.06201171875f, 20.5f, 0.0f, 32, 1375, 2.0f);
			if(obj)
			{
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
				m_gates.insert(obj);
			}
			
			obj = SpawnGameObject(192642, 1327.2099609375f, 813.239990234375f, 20.5f, 0.0f, 32, 1375, 2.0f);
			if(obj)
			{
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
				m_gates.insert(obj);
			}

			// not rly blizzlike, should be spawn later, not on create, but who cares, will fix it later
			obj = SpawnGameObject(191877, 1291.974487f, 791.844666f, 9.339742f, 3.116816f, 32, 1375, 1.0f);
			if(obj)
				obj->PushToWorld(m_mapMgr);

			m_pcWorldStates[GREEN_TEAM] = WORLDSTATE_ARENA_WOTLK_GREEN_PLAYER_COUNT;
			m_pcWorldStates[GOLD_TEAM] = WORLDSTATE_ARENA_WOTLK_GOLD_PLAYER_COUNT;
			sm.CreateWorldState(WORLDSTATE_ARENA_WOTLK_SCORE_SHOW, 1);
		}break;
		/* The Ring of Valor */
	case 618:
		{
			/*TODO:
			 * - fix these elevators, i ran out of ideas, lost few hours...
			 * - spawn rest of those stupid gameobj, im too lazy to get their coords
			 * - that moving things shouldn't move all the time...
			 */

			//elevators
			obj = SpawnGameObject(194030, 763.93f, -295.0f, 26.0f/*0.0f*/, 0.0f, 40, 1375, 1.0f);
			if(obj)
			{
				obj->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID, 5);
				obj->PushToWorld(m_mapMgr);
			}

			obj = SpawnGameObject(194031, 763.93f, -274.0f, 26.0f/*0.0f*/, 0.0f, 40, 1375, 1.0f);
			if(obj)
			{
				obj->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID, 5);
				obj->PushToWorld(m_mapMgr);
			}
		
			//moving 'things'
			obj = SpawnGameObject(193458, 763.630f, -261.783f, 26.0f, 0.0f, 40, 1375, 1.0f);
			if(obj)
				obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(193459, 763.761f, -306.230f, 26.0f, 0.0f, 40, 1375, 1.0f);
			if(obj)
				obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(193460, 802.313f, -284.349f, 24.6f, 0.0f, 40, 1375, 1.0f);
			if(obj)
				obj->PushToWorld(m_mapMgr);

			obj = SpawnGameObject(193461, 723.522f, -284.428f, 24.6f, 0.0f, 40, 1375, 1.0f);
			if(obj)	
				obj->PushToWorld(m_mapMgr);

			//gates
			obj = SpawnGameObject(192392, 763.93f, -295.0f, 27.0f, 0.0f, 32, 1375, 1.0f);
			if(obj)
			{
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
				m_gates.insert(obj);
			}

			obj = SpawnGameObject(192391, 763.93f, -274.0f, 27.0f, 0.0f, 32, 1375, 1.0f);
			if(obj)
			{
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
				obj->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
				m_gates.insert(obj);
			}

			m_pcWorldStates[GREEN_TEAM] = WORLDSTATE_ARENA_WOTLK_GREEN_PLAYER_COUNT;
			m_pcWorldStates[GOLD_TEAM] = WORLDSTATE_ARENA_WOTLK_GOLD_PLAYER_COUNT;
			sm.CreateWorldState(WORLDSTATE_ARENA_WOTLK_SCORE_SHOW, 1);
		}break;
	}

	/* push gates into world */
	for(set< GameObjectPointer >::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
		(*itr)->PushToWorld(m_mapMgr);

	
	// known world states
	sm.CreateWorldState(m_pcWorldStates[GREEN_TEAM], 0);
	sm.CreateWorldState(m_pcWorldStates[GOLD_TEAM], 0);

	// unknown world states
	sm.CreateWorldState(0x08D4	,0x0000);
	sm.CreateWorldState(0x08D8	,0x0000);
	sm.CreateWorldState(0x08D7	,0x0000);
	sm.CreateWorldState(0x08D6	,0x0000);
	sm.CreateWorldState(0x08D5	,0x0000);
	sm.CreateWorldState(0x08D3	,0x0000);
	sm.CreateWorldState(0x0C0D	,0x017B);
}

void Arena::OnStart()
{
	/* remove arena readyness buff */
	for(uint32 i = 0; i < 2; ++i) {
		for(set<PlayerPointer  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr) {
			PlayerPointer plr = *itr;
			plr->RemoveAura(ARENA_PREPARATION);
		}
	}

	/* open gates */
	for(set< GameObjectPointer >::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
		(*itr)->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 0);
	}

	m_started = true;

	/* Incase all players left */
	UpdatePlayerCounts();

	// soundz!
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	sEventMgr.RemoveEvents(shared_from_this(), EVENT_ARENA_SHADOW_SIGHT);
	sEventMgr.AddEvent(TO_CBATTLEGROUND(shared_from_this()), &CBattleground::HookOnShadowSight, EVENT_ARENA_SHADOW_SIGHT, 90000, 1,0);
}

void Arena::UpdatePlayerCounts()
{
	if(m_ended)
		return;

	m_mapMgr->GetStateManager().UpdateWorldState(m_pcWorldStates[GOLD_TEAM], m_playersCount[GOLD_TEAM]);
	m_mapMgr->GetStateManager().UpdateWorldState(m_pcWorldStates[GREEN_TEAM], m_playersCount[GREEN_TEAM]);

	if(!m_started)
		return;

	if(m_playersCount[GOLD_TEAM] == 0)
		m_losingteam = GOLD_TEAM;
	else if(m_playersCount[GREEN_TEAM] == 0)
		m_losingteam = GREEN_TEAM;
	else
		return;

	Finish();
}

int32 Arena::CalcDeltaRating(uint32 oldRating, uint32 opponentRating, bool outcome) 
{
	// ---- Elo Rating System ----
	// Expected Chance to Win for Team A vs Team B
	//                     1
	// -------------------------------------------
	//                   (PB - PA)/400
	//              1 + 10

	double power = (int)(opponentRating - oldRating) / 400.0f;
	double divisor = pow(((double)(10.0)), power);
	divisor += 1.0;

	double winChance = 1.0 / divisor;

	// New Rating Calculation via Elo
	// New Rating = Old Rating + K * (outcome - Expected Win Chance)
	// outcome = 1 for a win and 0 for a loss (0.5 for a draw ... but we cant have that)
	// K is the maximum possible change
	// Through investigation, K was estimated to be 32 (same as chess)
	double multiplier = (outcome ? 1.0 : 0.0) - winChance;
	return long2int32(32.0 * multiplier);
}

void Arena::Finish()
{
	m_ended = true;
	ArenaTeam * teams[2] = {NULL, NULL};
	if(rated_match)
	{
		teams[0] = objmgr.GetArenaTeamById(m_teams[0]);
		teams[1] = objmgr.GetArenaTeamById(m_teams[1]);
	}
	/* update arena team stats */
	if(rated_match && teams[0] && teams[1])
	{
		for (uint32 i = 0; i < 2; ++i) {
			uint32 j = i ? 0 : 1; // opposing side
			bool outcome = (i != m_losingteam);
			if (outcome) {
				teams[i]->m_stat_gameswonseason++;
				teams[i]->m_stat_gameswonweek++;
			}

			m_deltaRating[i] = CalcDeltaRating(teams[i]->m_stat_rating, teams[j]->m_stat_rating, outcome);
			teams[i]->m_stat_rating += m_deltaRating[i];
			if ((int32)teams[i]->m_stat_rating < 0) teams[i]->m_stat_rating = 0;

			for (int x=0; x<hashmap_length(m_players2[i]); x++) {
				uint32 key;
				if (MAP_OK == hashmap_get_index(m_players2[i], x, (int*)&key, (any_t*) NULL)) {
					PlayerInfo * info = objmgr.GetPlayerInfo(key);
					if (info) {
						ArenaTeamMember * tp = teams[i]->GetMember(info);

						if(tp != NULL) {
							tp->PersonalRating += CalcDeltaRating(tp->PersonalRating, teams[j]->m_stat_rating, outcome);
							if ((int32)tp->PersonalRating < 0) tp->PersonalRating = 0;

							if(outcome) {
								tp->Won_ThisWeek++;
								tp->Won_ThisSeason++;
							}
						}
					}
				}
			}
			
			teams[i]->SaveToDB();
			// send arena team stats update
			WorldPacket data(256);
			teams[i]->Stat(data);
			teams[i]->SendPacket(&data);
		}
		objmgr.UpdateArenaTeamRankings();
	}

	m_nextPvPUpdateTime = 0;
	UpdatePvPData();
	PlaySoundToAll(m_losingteam ? SOUND_ALLIANCEWINS : SOUND_HORDEWINS);

	sEventMgr.RemoveEvents(shared_from_this(), EVENT_BATTLEGROUND_CLOSE);
	sEventMgr.RemoveEvents(shared_from_this(), EVENT_ARENA_SHADOW_SIGHT);
	sEventMgr.AddEvent(TO_CBATTLEGROUND(shared_from_this()), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

	for(int i = 0; i < 2; i++)
	{
		bool victorious = (i != m_losingteam);
		set<PlayerPointer  >::iterator itr = m_players[i].begin();
		for(; itr != m_players[i].end(); itr++)
		{
			PlayerPointer plr = (PlayerPointer )(*itr);
			plr->Root();

			if( plr->m_bgScore.DamageDone == 0 && plr->m_bgScore.HealingDone == 0 )
				continue;

			sHookInterface.OnArenaFinish(plr, m_arenateamtype, plr->m_playerInfo->arenaTeam[m_arenateamtype], victorious, rated_match);
		}
	}
}

LocationVector Arena::GetStartingCoords(uint32 Team)
{
	// 559, 562, 572
	/*
	A start
	H start
	Repop
	572 1295.322388 1585.953369 31.605387
	572 1277.105103 1743.956177 31.603209
	572 1286.112061 1668.334961 39.289127

	562 6184.806641 236.643463 5.037095
	562 6292.032227 287.570343 5.003577
	562 6241.171875 261.067322 0.891833

	559 4085.861328 2866.750488 12.417445
	559 4027.004883 2976.964844 11.600499
	559 4057.042725 2918.686523 13.051933
	*/
	switch(m_mapMgr->GetMapId())
	{
		/* loraedeon */
	case 572:
		{
			if(Team)
				return LocationVector(1277.105103f, 1743.956177f, 31.603209f);
			else
				return LocationVector(1295.322388f, 1585.953369f, 31.605387f);
		}break;

		/* blades edge arena */
	case 562:
		{
			if(Team)
				return LocationVector(6292.032227f, 287.570343f, 5.003577f);
			else
				return LocationVector(6184.806641f, 236.643463f, 5.037095f);
		}break;

		/* nagrand arena */
	case 559:
		{
			if(Team)
				return LocationVector(4027.004883f, 2976.964844f, 11.600499f);
			else
				return LocationVector(4085.861328f, 2866.750488f, 12.417445f);
		}break;
		/* Dalaran Sewers */
	case 617:
		{
			if(Team)
				return LocationVector(1235.88f, 771.08f, 15.5f);
			else
				return LocationVector(1347.43f, 813.0f, 15.5f);
		}break;
		/* The Ring of Valor */
	case 618:
		{
			//temp workaround
			if(Team)
				return LocationVector(763.56f, -274.0f, 28.27f/*3.55f*/);
			else
				return LocationVector(763.93f, -295.01f, 28.27f/*3.55f*/);
		}break;
	}

	return LocationVector(0,0,0,0);
}

bool Arena::HookHandleRepop(PlayerPointer plr)
{
	// 559, 562, 572
	/*
	A start
	H start
	Repop
	572 1295.322388 1585.953369 31.605387
	572 1277.105103 1743.956177 31.603209
	572 1286.112061 1668.334961 39.289127

	562 6184.806641 236.643463 5.037095
	562 6292.032227 287.570343 5.003577
	562 6241.171875 261.067322 0.891833

	559 4085.861328 2866.750488 12.417445
	559 4027.004883 2976.964844 11.600499
	559 4057.042725 2918.686523 13.051933
	*/
	LocationVector dest(0,0,0,0);
	switch(m_mapMgr->GetMapId())
	{
		/* loraedeon */
	case 572: {
			dest.ChangeCoords(1286.112061f, 1668.334961f, 39.289127f);
		}break;

		/* blades edge arena */
	case 562: {
			dest.ChangeCoords(6241.171875f, 261.067322f, 0.891833f);
		}break;

		/* nagrand arena */
	case 559: {
			dest.ChangeCoords(4057.042725f, 2918.686523f, 13.051933f);
		}break;
		/* Dalaran Sewers */
	case 617: {
			dest.ChangeCoords(1292.51f, 792.05f, 9.34f);
		}break;
		/* The Ring of Valor */
	case 618: {
			dest.ChangeCoords(762.91f, -284.28f, 28.28f);
		}break;
	}

	plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), dest);
	return true;
}

void Arena::HookOnAreaTrigger(PlayerPointer plr, uint32 id)
{
	if(!m_started)
		return;

	int32 buffslot = -1;

	ASSERT(plr != NULL);

	switch (id) 
	{
		case 4536:
		case 4538:
		case 4696:
			buffslot = 0;
			break;
		case 4537:
		case 4539:
		case 4697:
			buffslot = 1;
			break;
		default:
			{
				Log.Error("Arena", "Encountered unhandled areatrigger id %u", id);
				return;
			}break;
	}

	if(!m_shadowsightspawned)
		return;

	if(buffslot >= 0)
	{
		if(m_buffs[buffslot] != NULL && m_buffs[buffslot]->IsInWorld())
		{
			/* apply the buff */
			plr->CastSpell(plr, m_buffs[buffslot]->GetInfo()->sound3, true);

			/* despawn the gameobject (not delete!) */
			m_buffs[buffslot]->Despawn(BUFF_RESPAWN_TIME);
		}
	}
}

void Arena::HookOnShadowSight()
{
	switch(m_mapMgr->GetMapId())
	{
		/* ruins of lordaeron */
	case 572:
		m_buffs[0] = SpawnGameObject(184664, 1328.729268f, 1632.738403f, 34.838585f, 2.611449f, 32, 1375, 1.0f);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		m_buffs[0]->PushToWorld(m_mapMgr);

		m_buffs[1] = SpawnGameObject(184664, 1243.306763f, 1699.334351f, 34.837566f, 5.713773f, 32, 1375, 1.0f);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		m_buffs[1]->PushToWorld(m_mapMgr);
		break;

		/* blades edge arena */
	case 562:
		m_buffs[0] = SpawnGameObject(184664, 6249.276855f, 275.187714f, 11.201481f, -2.260201f, 32, 1375, 1.0f);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		m_buffs[0]->PushToWorld(m_mapMgr);

		m_buffs[1] = SpawnGameObject(184664, 6228.546387f, 249.709854f, 11.201481f, 0.881392f, 32, 1375, 1.0f);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		m_buffs[1]->PushToWorld(m_mapMgr);
		break;

		/* nagrand arena */
	case 559:
		m_buffs[0] = SpawnGameObject(184664, 4011.113232f, 2896.879980f, 12.523950f, 0.486944f, 32, 1375, 1.0f);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[0]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		m_buffs[0]->PushToWorld(m_mapMgr);

		m_buffs[1] = SpawnGameObject(184664, 4102.111426f, 2945.843262f, 12.662578f, 3.628544f, 32, 1375, 1.0f);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_TYPE_ID, 6);
		m_buffs[1]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		m_buffs[1]->PushToWorld(m_mapMgr);
		break;
	}
}
