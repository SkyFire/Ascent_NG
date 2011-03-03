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

static const char * EOTSCPNames[EOTS_TOWER_COUNT] = {
	"Blood Elf Tower",
	"Fel Reaver Ruins",
	"Mage Tower",
	"Draenei Ruins",
};

static float EOTSBuffCoordinates[4][4] = {
	{ 2050.542236f, 1372.680176f, 1194.561279f, 1.67552f },
	{ 2047.728271f, 1749.736084f, 1190.198608f, -0.872665f },
	{ 2283.300049f, 1748.891235f, 1189.706787f, 1.76278f },
	{ 2302.68994140625f, 1391.27001953125f, 1197.77001953125f, -1.50098f},
};

static float EOTSBuffRotations[4][2] = {
	{ 0.681998f, -0.731354f },
	{ 0.771625f, 0.636078f },
	{ 0.422618f, -0.906308f },
	{ 0.743145f, 0.669131f },
};

static uint32 EOTSbuffentrys[4] = {
	184964,
	184971,
	184978,
	184973
};

static const float EOTSGraveyardLocations[EOTS_TOWER_COUNT][3] = {
	{ 2012.403442f, 1455.412354f, 1172.201782f },			// BE Tower
	{ 2013.061890f, 1677.238037f, 1182.125732f },			// Fel Reaver Ruins
	{ 2355.297852f, 1683.713989f, 1173.153687f },			// Mage Tower
	{ 2351.785400f, 1455.399048f, 1185.333374f },			// Draenei Ruins
};

static const float EOTSCPLocations[EOTS_TOWER_COUNT][3] = {
	{ 2048.290039f, 1393.757690f, 1194.363525f },			// BE Tower
	{ 2043.571533f, 1729.117310f, 1189.911865f },			// Fel Reaver Ruins
	{ 2284.930664f, 1731.128488f, 1189.874512f },			// Mage Tower
	{ 2285.848877f, 1402.939575f, 1197.128540f },			// Draenei Ruins
};

static const float EOTSFlagLocation[3] = { 2174.718750f, 1568.766113f, 1159.958740f };
static const float EOTSStartLocations[2][3] = {
	{ 2523.686035f, 1596.597290f, 1269.347656f },
	{ 1807.735962f, 1539.415649f, 1267.627319f },
};

const float EOTSBubbleLocations[2][5] = {
	{ 184719, 1803.21f, 1539.49f, 1261.09f, 3.14159f },
	{ 184720, 2527.6f, 1596.91f, 1262.13f, -3.12414f },
};

const float EOTSBubbleRotations[2][4] = {
	{ -0.173642f, -0.001515f, 0.984770f, -0.008594f },
	{ -0.173642f, -0.001515f, 0.984770f, -0.008594f },
};

static const uint32 EOTSNeturalDisplayFields[EOTS_TOWER_COUNT] = {
	WORLDSTATE_EOTS_BLOODELF_TOWER_NEUTRAL_DISPLAY,
	WORLDSTATE_EOTS_FELREAVER_RUINS_NETURAL_DISPLAY,
	WORLDSTATE_EOTS_MAGE_TOWER_NETURAL_DISPLAY,
	WORLDSTATE_EOTS_DRAENEI_RUINS_NETURAL_DISPLAY,
};

static const uint32 EOTSAllianceDisplayFields[EOTS_TOWER_COUNT] = {
	WORLDSTATE_EOTS_BLOODELF_TOWER_ALLIANCE_DISPLAY,
	WORLDSTATE_EOTS_FELREAVER_RUINS_ALLIANCE_DISPLAY,
	WORLDSTATE_EOTS_MAGE_TOWER_ALLIANCE_DISPLAY,
	WORLDSTATE_EOTS_DRAENEI_RUINS_ALLIANCE_DISPLAY,
};

static const uint32 EOTSHordeDisplayFields[EOTS_TOWER_COUNT] = {
	WORLDSTATE_EOTS_BLOODELF_TOWER_HORDE_DISPLAY,
	WORLDSTATE_EOTS_FELREAVER_RUINS_HORDE_DISPLAY,
	WORLDSTATE_EOTS_MAGE_TOWER_HORDE_DISPLAY,
	WORLDSTATE_EOTS_DRAENEI_RUINS_HORDE_DISPLAY,
};

//===================================================
// 184083 - Draenei Tower Cap Pt, 184082 - Human Tower Cap Pt, 184081 - Fel Reaver Cap Pt, 184080 - BE Tower Cap Pt
#define EOTS_GO_BE_TOWER 184080
#define EOTS_GO_FELREAVER 184081
#define EOTS_GO_MAGE_TOWER 184082
#define EOTS_GO_DRAENEI_TOWER 184083

#define EOTS_TOWER_BE 0
#define EOTS_TOWER_FELREAVER 1
#define EOTS_TOWER_MAGE 2
#define EOTS_TOWER_DRAENEI 3

#define EOTS_BANNER_NEUTRAL 184382
#define EOTS_BANNER_ALLIANCE 184381
#define EOTS_BANNER_HORDE 184380

#define EOTS_STANDFLAG 184141
#define EOTS_DROPFLAG 184142

#define EOTS_MARK_ID 29024
#define EOTS_CAPTURE_DISTANCE 900 /*30*/
const uint32 EOTSTowerIds[EOTS_TOWER_COUNT] = { EOTS_GO_BE_TOWER, EOTS_GO_FELREAVER, EOTS_GO_MAGE_TOWER, EOTS_GO_DRAENEI_TOWER };

/**
 * WorldStates
 */
#define EOTS_NETHERWING_FLAG_SPELL 34976
//#define EOTS_CAPTURE_RATE 20

EyeOfTheStorm::EyeOfTheStorm( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
{
	uint32 i;

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		m_EOTSbuffs[i] = NULL;
		m_CPStatus[i] = 50;
		m_CPBanner[i] = NULL;
		m_CPStatusGO[i] = NULL;

		m_spiritGuides[i] = NULL;
	}

	m_bonusHonor = HonorHandler::CalculateHonorPointsFormula(lgroup*10,lgroup*10);
	m_resToGainBH = 330;

	m_flagHolder = 0;
	m_points[0] = m_points[1] = 0;
	m_towerCount[0] = m_towerCount[1] = 0;
	m_playerCountPerTeam = 15;

	m_resourceRewards[0] = m_resourceRewards[1] = 330;

	m_resurrectMap.clear();
}

void EyeOfTheStorm::Init()
{
	CBattleground::Init();
}

EyeOfTheStorm::~EyeOfTheStorm()
{
	for(uint32 i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if(m_EOTSbuffs[i] != NULL)
		{
			m_EOTSbuffs[i]->m_battleground = NULL;
			if( !m_EOTSbuffs[i]->IsInWorld() )
			{
				m_EOTSbuffs[i]->Destructor();
			}
		}
	}
	m_resurrectMap.clear();
}

void EyeOfTheStorm::RepopPlayersOfTeam(int32 team, Creature* sh)
{
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(sh);
	if( itr != m_resurrectMap.end() )
	{
		for( set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end(); ++it2 )
		{
			Player* r_plr = m_mapMgr->GetPlayer( *it2 );
			if( r_plr != NULL && (team < 0 || (int32)r_plr->GetTeam() == team) && r_plr->isDead() )
			{
				HookHandleRepop( r_plr );
				QueueAtNearestSpiritGuide(r_plr, sh);
			}
		}
	}
}

bool EyeOfTheStorm::HookHandleRepop(Player* plr)
{
	uint32 i;
	uint32 t = plr->GetTeam();
	float dist = 999999.0f;
	float distcur;
	LocationVector dest;

	dest.ChangeCoords( EOTSStartLocations[t][0], EOTSStartLocations[t][1], EOTSStartLocations[t][2], 0 );

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if( m_CPBanner[i] && m_CPBanner[i]->GetEntry() == EOTS_BANNER_ALLIANCE && t == 0 ||
			m_CPBanner[i]->GetEntry() == EOTS_BANNER_HORDE && t == 1)
		{
			distcur = plr->GetPositionNC().Distance2DSq( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1] );
			if( distcur < dist )
			{
				dist = distcur;
				dest.ChangeCoords( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1], EOTSGraveyardLocations[i][2], 0 );
			}
		}
	}

	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void EyeOfTheStorm::HookOnAreaTrigger(Player* plr, uint32 id)
{
	int32 tid = -1;
	int32 bonusid = -1;
	switch(id)
	{
	case 4476:			// BE Tower
		tid = EOTS_TOWER_BE;
		break;
	case 4568:			// BE Tower bonus
		bonusid = EOTS_TOWER_BE;
		break;
	case 4514:			// Fel Reaver Tower
		tid = EOTS_TOWER_FELREAVER;
		break;
	case 4569:			// Fel Reaver Tower bonus
		bonusid = EOTS_TOWER_FELREAVER;
		break;
	case 4518:			// Draenei Tower
		tid = EOTS_TOWER_DRAENEI;
		break;
	case 4571:			// Draenei Tower bonus
		bonusid = EOTS_TOWER_DRAENEI;
		break;
	case 4516:			// Mage Tower
		tid = EOTS_TOWER_MAGE;
		break;
	case 4570:			// Mage Tower bonus
		bonusid = EOTS_TOWER_MAGE;
		break;
	}

	if(bonusid > -1){
		uint32 spellid=0;
		uint32 x = (uint32)bonusid;
		if(m_EOTSbuffs[x] && m_EOTSbuffs[x]->IsInWorld())
		{
			spellid = m_EOTSbuffs[x]->GetInfo()->sound3;
			SpellEntry * sp = dbcSpell.LookupEntryForced(spellid);
			if(sp)
			{
				Spell* pSpell = (new Spell(plr, sp, true, NULL));
				SpellCastTargets targets(plr->GetGUID());
				pSpell->prepare(&targets);
			}
			m_EOTSbuffs[x]->Despawn(EOTS_BUFF_RESPAWN_TIME);
		}
	}

	if( tid < 0 )
		return;

#ifdef BG_ANTI_CHEAT
	if(!m_started || m_ended)
	{
		SendChatMessage(CHAT_MSG_BG_SYSTEM_NEUTRAL, plr->GetGUID(), "%s has removed from the battleground for cheating.",  plr->GetName());
		plr->SoftDisconnect();
		return;
	}
#endif

	uint32 team = plr->GetTeam();
	if( plr->GetLowGUID() != m_flagHolder )
		return;

	uint32 i;
	uint32 towers = 0;

	if( m_CPStatus[tid] < 50 && team == 0 )
		return;

	if( m_CPStatus[tid] > 50 && team == 1 )
		return;

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if(m_CPStatus[i] < 50 && team == 1)
			towers++;

		if(m_CPStatus[i] > 50 && team == 0)
			towers++;
	}

	/*
	Points from flag captures

	* 1 towers controlled = 75 points
	* 2 towers controlled = 85 points
	* 3 towers controlled = 100 points
	* 4 towers controlled = 500 points 
	*/

	// 25 is guessed
	const static uint32 points[5] = { 25, 75, 85, 100, 500 };
	const char * msgs[2] = { "The Alliance have captured the flag.", "The Horde have captured the flag." };

	plr->m_bgScore.MiscData[BG_SCORE_EOTS_FLAG_CAPTURES]++;
	plr->m_bgHasFlag = false;
	SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE + team, 0, msgs[team] );
	if( !GivePoints( team, points[towers] ) )
	{
		PlaySoundToAll( plr->GetTeam() ? SOUND_HORDE_SCORES : SOUND_ALLIANCE_SCORES );
		UpdatePvPData();
		m_standFlag->PushToWorld( m_mapMgr );
		m_flagHolder = 0;
		m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_FLAG_NEUTRAL_DISPLAY, 1 );
	}
	// else bg ended
	
	plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
}

void EyeOfTheStorm::HookOnPlayerDeath(Player* plr)
{
		
	if(plr->m_bgHasFlag)
		plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
	
	plr->m_bgHasFlag = false;
	plr->m_bgScore.Deaths++;
	UpdatePvPData();
}

void EyeOfTheStorm::HookFlagDrop(Player* plr, GameObject* obj)
{
	if( !m_dropFlag->IsInWorld() )
		return;

	// check forcedreaction 1059, meaning do we recently dropped a flag?
	map<uint32,uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if (itr != plr->m_forcedReactions.end()) {
		return;
	}

	m_dropFlag->RemoveFromWorld(false);
	plr->CastSpell( plr->GetGUID(), EOTS_NETHERWING_FLAG_SPELL, true );

	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_FLAG_NEUTRAL_DISPLAY, 0 );
	PlaySoundToAll( 8212 );
	SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE + plr->GetTeam(), plr->GetGUID(), "$n has taken the flag!" );
	m_flagHolder = plr->GetLowGUID();

	event_RemoveEvents( EVENT_EOTS_RESET_FLAG );
}

void EyeOfTheStorm::HookFlagStand(Player* plr, GameObject* obj)
{

}

bool EyeOfTheStorm::HookSlowLockOpen( GameObject* pGo, Player* pPlayer, Spell* pSpell)
{
	if( m_flagHolder != 0 )
		return false;

	if(m_standFlag->IsInWorld())
		m_standFlag->RemoveFromWorld(false);
	pPlayer->CastSpell( pPlayer->GetGUID(), EOTS_NETHERWING_FLAG_SPELL, true );
	pPlayer->m_bgHasFlag = true;

	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_FLAG_NEUTRAL_DISPLAY, 0 );
	PlaySoundToAll( pPlayer->GetTeam() ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE );
	SendChatMessage( CHAT_MSG_BG_SYSTEM_ALLIANCE + pPlayer->GetTeam(), pPlayer->GetGUID(), "$n has taken the flag!" );
	m_flagHolder = pPlayer->GetLowGUID();

	CBattleground::HookSlowLockOpen(pGo, pPlayer, pSpell);
	return true;
}

void EyeOfTheStorm::HookOnMount(Player* plr)
{
	if( m_flagHolder == plr->GetLowGUID() )
	{
		plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
		//DropFlag( plr );
	}
}

void EyeOfTheStorm::OnAddPlayer(Player* plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
}

void EyeOfTheStorm::OnRemovePlayer(Player* plr)
{
	if( m_flagHolder == plr->GetLowGUID() )
	{
		plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
		//DropFlag( plr );
	}
	
	if(!m_started)
		plr->RemoveAura(BG_PREPARATION);
}

void EyeOfTheStorm::DropFlag(Player* plr)
{
	if( m_flagHolder != plr->GetLowGUID() )
		return;

	plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );

	plr->CastSpell(plr, BG_RECENTLY_DROPPED_FLAG, true);

	/**
	* let's apply the same rules as wsg, reallocate guid, 
	* reposition, spawn.
	 */
	m_dropFlag->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
	m_dropFlag->SetPosition( plr->GetPosition() );
	m_dropFlag->PushToWorld( m_mapMgr );
	m_flagHolder = 0;

	sEventMgr.AddEvent( TO_EYEOFTHESTORM(this), &EyeOfTheStorm::EventResetFlag, EVENT_EOTS_RESET_FLAG, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void EyeOfTheStorm::EventResetFlag()
{
	if(!m_dropFlag->IsInWorld())
		return;

	m_dropFlag->RemoveFromWorld(false);
	m_standFlag->PushToWorld(m_mapMgr);

	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_FLAG_NEUTRAL_DISPLAY, 1 );
	PlaySoundToAll( 8192 );
	SendChatMessage( CHAT_MSG_BG_SYSTEM_NEUTRAL, 0, "The flag has been reset." );
	m_flagHolder = 0;
}

void EyeOfTheStorm::OnCreate()
{
	uint32 i;

	WorldStateManager& sm = m_mapMgr->GetStateManager();

	// create world state templates
	sm.CreateWorldState(2565, 142);
	sm.CreateWorldState(2720, 0);
	sm.CreateWorldState(2719, 0);
	sm.CreateWorldState(2718, 0);
	sm.CreateWorldState(2260, 0);
	sm.CreateWorldState(2264, 0);
	sm.CreateWorldState(2263, 0);
	sm.CreateWorldState(2262, 0);
	sm.CreateWorldState(2261, 0);
	sm.CreateWorldState(2259, 0);
	sm.CreateWorldState(2742, 0);
	sm.CreateWorldState(2741, 0);
	sm.CreateWorldState(2740, 0);
	sm.CreateWorldState(2739, 0);
	sm.CreateWorldState(2738, 0);
	sm.CreateWorldState(2737, 0);
	sm.CreateWorldState(2736, 0);
	sm.CreateWorldState(2735, 0);
	sm.CreateWorldState(2733, 0);
	sm.CreateWorldState(2732, 0);
	sm.CreateWorldState(2731, 1);
	sm.CreateWorldState(2730, 0);
	sm.CreateWorldState(2729, 0);
	sm.CreateWorldState(2728, 1);
	sm.CreateWorldState(2727, 0);
	sm.CreateWorldState(2726, 0);
	sm.CreateWorldState(2725, 1);
	sm.CreateWorldState(2724, 0);
	sm.CreateWorldState(2723, 0);
	sm.CreateWorldState(2722, 1);
	sm.CreateWorldState(2757, 1);
	sm.CreateWorldState(2753, 0);
	sm.CreateWorldState(2752, 0);
	sm.CreateWorldState(2770, 1);
	sm.CreateWorldState(2769, 1);
	sm.CreateWorldState(2750, 0);
	sm.CreateWorldState(2749, 0);
	sm.CreateWorldState(3191, 2);
	sm.CreateWorldState(3218, 0);
	sm.CreateWorldState(3217, 0);
	sm.CreateWorldState(3085, 379);

	// decyphered world states
	sm.CreateWorldState(WORLDSTATE_EOTS_HORDE_VICTORY_POINTS_DISPLAY, 1);
	sm.CreateWorldState(WORLDSTATE_EOTS_ALLIANCE_VICTORY_POINTS_DISPLAY, 1);

	sm.CreateWorldState(WORLDSTATE_EOTS_ALLIANCE_VICTORYPOINTS, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_HORDE_VICTORYPOINTS, 0);

	sm.CreateWorldState(WORLDSTATE_EOTS_ALLIANCE_BASES, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_HORDE_BASES, 0);

	sm.CreateWorldState(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_VALUE, 0);	

	sm.CreateWorldState(WORLDSTATE_EOTS_DRAENEI_RUINS_HORDE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_DRAENEI_RUINS_ALLIANCE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_DRAENEI_RUINS_NETURAL_DISPLAY, 1);
	
	sm.CreateWorldState(WORLDSTATE_EOTS_MAGE_TOWER_ALLIANCE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_MAGE_TOWER_HORDE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_MAGE_TOWER_NETURAL_DISPLAY, 1);
	
	sm.CreateWorldState(WORLDSTATE_EOTS_FELREAVER_RUINS_HORDE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_FELREAVER_RUINS_ALLIANCE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_FELREAVER_RUINS_NETURAL_DISPLAY, 1);
	
	sm.CreateWorldState(WORLDSTATE_EOTS_BLOODELF_TOWER_HORDE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_BLOODELF_TOWER_ALLIANCE_DISPLAY, 0);
	sm.CreateWorldState(WORLDSTATE_EOTS_BLOODELF_TOWER_NEUTRAL_DISPLAY, 1);

	sm.CreateWorldState(WORLDSTATE_EOTS_FLAG_NEUTRAL_DISPLAY, 1);

	/* create gameobjects */
	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		m_CPStatusGO[i] = m_mapMgr->CreateGameObject(EOTSTowerIds[i]);
		if(m_CPStatusGO[i] == NULL || !m_CPStatusGO[i]->CreateFromProto( EOTSTowerIds[i], m_mapMgr->GetMapId(), EOTSCPLocations[i][0], EOTSCPLocations[i][1], EOTSCPLocations[i][2], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobject %u.", EOTSTowerIds[i]);
			abort();
			return;
		}
		m_CPStatusGO[i]->PushToWorld( m_mapMgr );

		m_CPBanner[i] = m_mapMgr->CreateGameObject(EOTS_BANNER_NEUTRAL);
		if( m_CPBanner[i] == NULL || !m_CPBanner[i]->CreateFromProto( EOTS_BANNER_NEUTRAL, m_mapMgr->GetMapId(), EOTSCPLocations[i][0], EOTSCPLocations[i][1], EOTSCPLocations[i][2], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobjects %u",EOTS_BANNER_NEUTRAL);
			abort();
			return;
		}
		m_CPBanner[i]->PushToWorld( m_mapMgr );
	}

	/* BUBBLES! */
	for( i = 0; i < 2; ++i )
	{
		m_bubbles[i] = m_mapMgr->CreateGameObject((uint32)EOTSBubbleLocations[i][0]);
		if( m_bubbles[i] == NULL || !m_bubbles[i]->CreateFromProto( (uint32)EOTSBubbleLocations[i][0], m_mapMgr->GetMapId(), EOTSBubbleLocations[i][1], EOTSBubbleLocations[i][2], EOTSBubbleLocations[i][3], EOTSBubbleLocations[i][4], 0.0f, 0.0f, 0.0f, 0.0f ) )
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobjects. Terminating.");
			abort();
			return;
		}

		m_bubbles[i]->SetFloatValue(OBJECT_FIELD_SCALE_X,0.1f);
		m_bubbles[i]->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, 1);
		m_bubbles[i]->SetUInt32Value(GAMEOBJECT_FLAGS,32);
		m_bubbles[i]->SetUInt32Value(GAMEOBJECT_FACTION, 1375);
		m_bubbles[i]->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);

		m_bubbles[i]->PushToWorld( m_mapMgr );
	}

	/* Buffs */
	SpawnBuff(EOTS_TOWER_DRAENEI);
	SpawnBuff(EOTS_TOWER_MAGE);
	SpawnBuff(EOTS_TOWER_FELREAVER);
	SpawnBuff(EOTS_TOWER_BE);

	/* Flag */
	m_standFlag = m_mapMgr->CreateGameObject(EOTS_STANDFLAG);
	if( m_standFlag == NULL ||!	m_standFlag->CreateFromProto( EOTS_STANDFLAG, m_mapMgr->GetMapId(), 2174.782227f, 1569.054688f, 1160.361938f, -1.448624f, 0.0f, 0.0f, 0.0f, 0.0f ))
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobject %u",EOTS_STANDFLAG);
		abort();
		return;
	}
	m_standFlag->SetFloatValue( OBJECT_FIELD_SCALE_X, 2.5f );
	m_standFlag->PushToWorld( m_mapMgr );

	m_dropFlag = m_mapMgr->CreateGameObject(EOTS_DROPFLAG);
	if( m_dropFlag == NULL || !m_dropFlag->CreateFromProto( EOTS_DROPFLAG, m_mapMgr->GetMapId(), 2174.782227f, 1569.054688f, 1160.361938f, -1.448624f, 0.0f, 0.0f, 0.0f, 0.0f ))
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobject %u",EOTS_DROPFLAG);
		abort();
		return;
	}
	m_dropFlag->SetFloatValue( OBJECT_FIELD_SCALE_X, 2.5f );
}

void EyeOfTheStorm::RespawnCPFlag(uint32 i, uint32 id)
{
	m_CPBanner[i]->RemoveFromWorld(false);
	m_CPBanner[i]->SetNewGuid( m_mapMgr->GenerateGameobjectGuid() );
	m_CPBanner[i]->CreateFromProto( id, m_mapMgr->GetMapId(), m_CPBanner[i]->GetPositionX(), m_CPBanner[i]->GetPositionY(), m_CPBanner[i]->GetPositionZ(), m_CPBanner[i]->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f );
	m_CPBanner[i]->PushToWorld( m_mapMgr );
}

void EyeOfTheStorm::UpdateCPs()
{
	uint32 plrcounts[2] = { 0, 0 };

	// details:
	//   loop through inrange players, for new ones, send the enable CP worldstate.
	//   the value of the map is a timestamp of the last update, to avoid cpu time wasted
	//   doing lookups of objects that have already been updated

	unordered_set<Player*  >::iterator itr;
	unordered_set<Player*  >::iterator itrend;
	map<uint32,uint32>::iterator it2, it3;
	uint32 timeptr = (uint32)UNIXTIME;
	bool in_range;
	bool is_valid;
	Player* plr;
	uint32 i;

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		itr = m_CPStatusGO[i]->GetInRangePlayerSetBegin();
		itrend = m_CPStatusGO[i]->GetInRangePlayerSetEnd();
		plrcounts[0] = plrcounts[1] = 0;

		for(; itr != itrend; ++itr)
		{ 
			if( !(*itr)->IsPvPFlagged() || (*itr)->InStealth() || (*itr)->m_invisible || (*itr)->SchoolImmunityList[0] || (*itr)->m_bgFlagIneligible )
				is_valid = false;
			else
				is_valid = true;

			in_range = ((*itr)->isAlive() && m_CPStatusGO[i]->GetDistanceSq((*itr)) <= EOTS_CAPTURE_DISTANCE) ? true : false;

			it2 = m_CPStored[i].find((*itr)->GetLowGUID());
			if( it2 == m_CPStored[i].end() )
			{
				// new player :)
				if( in_range )
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_DISPLAY, 1);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_VALUE, m_CPStatus[i]);
					m_CPStored[i].insert(make_pair((*itr)->GetLowGUID(), timeptr));

					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
			else
			{
				// oldie
				if( !in_range )
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_DISPLAY, 0);
					m_CPStored[i].erase(it2);
				}
				else
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_VALUE, m_CPStatus[i]);
					it2->second = timeptr;
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
		}

		// handle stuff for the last tick
		// change the flag depending on cp status
		if( m_CPStatus[i] < 50 )
		{
			if( m_CPBanner[i] && m_CPBanner[i]->GetEntry() != EOTS_BANNER_HORDE )
			{
				RespawnCPFlag(i, EOTS_BANNER_HORDE);
				if( m_spiritGuides[i] != NULL )
				{
					RepopPlayersOfTeam( 0, m_spiritGuides[i] );
					m_spiritGuides[i]->Despawn( 0, 0 );
					RemoveSpiritGuide( m_spiritGuides[i] );
					m_spiritGuides[i] = NULL;
				}

				m_spiritGuides[i] = SpawnSpiritGuide( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1], EOTSGraveyardLocations[i][2], 0, 1 );
				AddSpiritGuide( m_spiritGuides[i] );
				SendChatMessage(CHAT_MSG_BG_SYSTEM_HORDE, 0, "The Horde have captured the %s.", EOTSCPNames[i]);

				// set some world states
				m_mapMgr->GetStateManager().UpdateWorldState(EOTSNeturalDisplayFields[i], 0);
				m_mapMgr->GetStateManager().UpdateWorldState(EOTSHordeDisplayFields[i], 1);
				m_mapMgr->GetStateManager().UpdateWorldState(EOTSAllianceDisplayFields[i], 0);
				m_towerCount[1]++;
				m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_HORDE_BASES, m_towerCount[1] );
			}
		}
		else if( m_CPStatus[i] > 50 )
		{
			if( m_CPBanner[i] && m_CPBanner[i]->GetEntry() != EOTS_BANNER_ALLIANCE )
			{
				RespawnCPFlag(i, EOTS_BANNER_ALLIANCE);
				if( m_spiritGuides[i] != NULL )
				{
					RepopPlayersOfTeam( 1, m_spiritGuides[i] );
					m_spiritGuides[i]->Despawn( 0, 0 );
					RemoveSpiritGuide( m_spiritGuides[i] );
					m_spiritGuides[i] = NULL;
				}

				m_spiritGuides[i] = SpawnSpiritGuide( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1], EOTSGraveyardLocations[i][2], 0, 0 );
				AddSpiritGuide( m_spiritGuides[i] );
				SendChatMessage(CHAT_MSG_BG_SYSTEM_ALLIANCE, 0, "The Alliance have captured the %s.", EOTSCPNames[i]);

				// set some world states
				m_mapMgr->GetStateManager().UpdateWorldState(EOTSNeturalDisplayFields[i], 0);
				m_mapMgr->GetStateManager().UpdateWorldState(EOTSHordeDisplayFields[i], 0);
				m_mapMgr->GetStateManager().UpdateWorldState(EOTSAllianceDisplayFields[i], 1);
				m_towerCount[0]++;
				m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_ALLIANCE_BASES, m_towerCount[0] );
			}
		}
		else
		{
			if( m_CPBanner[i] && m_CPBanner[i]->GetEntry() != EOTS_BANNER_NEUTRAL )
			{
				// has to be below or equal to 50, or above/equal
				if( ( m_CPBanner[i]->GetEntry() == EOTS_BANNER_ALLIANCE && m_CPStatus[i] <= 50 ) || 
					( m_CPBanner[i]->GetEntry() == EOTS_BANNER_HORDE && m_CPStatus[i] >= 50 ) )
				{
					if( m_CPBanner[i] && m_CPBanner[i]->GetEntry() == EOTS_BANNER_ALLIANCE )
					{
						SendChatMessage(CHAT_MSG_BG_SYSTEM_NEUTRAL, 0, "The Alliance have lost control of the %s.", EOTSCPNames[i]);
						m_towerCount[0]--;
						if( m_towerCount[0] < 0 )
							m_towerCount[0] = 0;

						m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_ALLIANCE_BASES, m_towerCount[0] );
					}
					else
					{
						SendChatMessage(CHAT_MSG_BG_SYSTEM_NEUTRAL, 0, "The Horde have lost control of the %s.", EOTSCPNames[i]);
						m_towerCount[1]--;
						if( m_towerCount[1] < 0 )
							m_towerCount[1] = 0;

						
						m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_HORDE_BASES, m_towerCount[1] );
					}

					RespawnCPFlag(i, EOTS_BANNER_NEUTRAL);
					if( m_spiritGuides[i] != NULL )
					{
						RepopPlayersOfTeam( -1, m_spiritGuides[i] );
						m_spiritGuides[i]->Despawn( 0, 0 );
						RemoveSpiritGuide( m_spiritGuides[i] );
						m_spiritGuides[i] = NULL;
					}

					// set some world states
					m_mapMgr->GetStateManager().UpdateWorldState(EOTSNeturalDisplayFields[i], 1);
					m_mapMgr->GetStateManager().UpdateWorldState(EOTSHordeDisplayFields[i], 0);
					m_mapMgr->GetStateManager().UpdateWorldState(EOTSAllianceDisplayFields[i], 0);
				}
			}
		}

		// send any out of range players the disable flag
		for(it2 = m_CPStored[i].begin(); it2 != m_CPStored[i].end(); )
		{
			it3 = it2;
			++it2;

			if( it3->second != timeptr )
			{
				plr = m_mapMgr->GetPlayer(it3->first);

				// they WILL be out of range at this point. this is guaranteed. means they left the set rly quickly.
				if( plr != NULL )
					plr->SendWorldStateUpdate(WORLDSTATE_EOTS_PVP_CAPTURE_BAR_DISPLAY, 0);

				m_CPStored[i].erase(it3);
			}
		}

		// work out current status for next tick
		int32 delta;
		if( plrcounts[0] > plrcounts[1] )
		{
			delta = plrcounts[0] - plrcounts[1];
			delta *= (uint32)sWorld.getRate(RATE_EOTS_CAPTURERATE);

			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			m_CPStatus[i] += delta;
			if( m_CPStatus[i] < 0 )
				m_CPStatus[i] = 0;
		}
		else if( plrcounts[1] > plrcounts[0] )
		{
			delta = plrcounts[1] - plrcounts[0];
			delta *= (uint32)sWorld.getRate(RATE_EOTS_CAPTURERATE);

			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			
			m_CPStatus[i] -= delta;
			if( m_CPStatus[i] > 100 )
				m_CPStatus[i] = 100;
		}
	}


}

void EyeOfTheStorm::GeneratePoints()
{
	uint32 i;
	uint32 towers[2] = {0,0};

	/*
	#  Unlike Arathi Basin, points are always generated in 2 seconds intervals no matter how many towers are controlled by both teams.
	# Each claimed tower generates victory points for the controlling team. The more towers your team owns, the faster your team gains points

	* 1 tower controlled = 1 point/tick (0.5 points per second)
	* 2 towers controlled = 2 points/tick (1 point per second)
	* 3 towers controlled = 5 points/tick (2.5 points per second)
	* 4 towers controlled = 10 points/tick (5 points per second) 

	*/
	uint32 pointspertick[5] = { 0, 1, 2, 5, 10 };

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if(m_CPStatus[i] > 50)
			towers[0]++;
		else if(m_CPStatus[i] < 50)
			towers[1]++;
	}

	for( i = 0; i < 2; ++i )
	{
		if( towers[i] == 0 )
		{
			//printf("EOTS: No points on team %u\n", i);
			continue;
		}

		if( GivePoints( i, pointspertick[towers[i]] ) )
			return;
	}
}

bool EyeOfTheStorm::GivePoints(uint32 team, uint32 points)
{
	if(m_ended || !m_started)
		return false;

	//printf("EOTS: Give team %u %u points.\n", team, points);

	m_points[team] += points;

	if( m_points[team] >= m_resourceRewards[team] )
	{
		m_resourceRewards[team] += m_resToGainBH;

		for(set<Player*  >::iterator itx = m_players[team].begin(); itx != m_players[team].end(); ++itx)
		{
			Player* plr = (*itx);
			if(!plr) continue;
			(*itx)->m_bgScore.BonusHonor += m_bonusHonor;
			HonorHandler::AddHonorPointsToPlayer( plr, m_bonusHonor );
		}
	}

	if( m_points[team] >= 1600 )
	{
		m_points[team] = 1600;
		m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_ALLIANCE_VICTORYPOINTS + team, m_points[team] );

		m_ended = true;
		m_losingteam = (team) ? 0 : 1;
		m_nextPvPUpdateTime = 0;

		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(TO_CBATTLEGROUND(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

		/* add the marks of honor to all players */
		m_mainLock.Acquire();

		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				(*itr)->Root();

				if( (*itr)->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK) )
					continue;

				// create eye of the storm mark of honor
				uint32 item_count = (i == m_losingteam) ? 1 : 3;

				if( i != m_losingteam )
				{
					(*itr)->m_bgScore.BonusHonor += 2*m_bonusHonor;
					HonorHandler::AddHonorPointsToPlayer( (*itr), 2*m_bonusHonor );
				}
				else
				{
					(*itr)->m_bgScore.BonusHonor += m_bonusHonor;
					HonorHandler::AddHonorPointsToPlayer( (*itr), m_bonusHonor );
					uint32 diff = abs((int32)(m_points[i] - m_points[i ? 0 : 1]));
					(*itr)->GetAchievementInterface()->HandleAchievementCriteriaWinBattleground( m_mapMgr->GetMapId(), diff, ((uint32)UNIXTIME - m_startTime) / 1000, TO_CBATTLEGROUND(this));
				}

				uint8 slot = (*itr)->GetItemInterface()->GetInventorySlotById(EOTS_MARK_ID);
				if(slot != ITEM_NO_SLOT_AVAILABLE)
					item_count += (*itr)->GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, slot)->GetUInt32Value( ITEM_FIELD_STACK_COUNT );
				HonorHandler::UpdateCurrencyItem((*itr), EOTS_MARK_ID, item_count);
			}
		}
		UpdatePvPData();
		m_mainLock.Release();
		return true;
	}

	m_mapMgr->GetStateManager().UpdateWorldState( WORLDSTATE_EOTS_ALLIANCE_VICTORYPOINTS + team, m_points[team] );
	return false;
}

void EyeOfTheStorm::HookOnPlayerKill(Player* plr, Unit* pVictim)
{
	if ( pVictim->IsPlayer() )
	{
		plr->m_bgScore.KillingBlows++;
		UpdatePvPData();
	}
}

void EyeOfTheStorm::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void EyeOfTheStorm::SpawnBuff(uint32 x)
{
	uint32 rand_buffid = EOTSbuffentrys[RandomUInt(3)];
	GameObjectInfo * goi = GameObjectNameStorage.LookupEntry(rand_buffid);
	if(goi == NULL)
		return;

	if(m_EOTSbuffs[x] == NULL)
	{
		m_EOTSbuffs[x] = SpawnGameObject(rand_buffid, EOTSBuffCoordinates[x][0], EOTSBuffCoordinates[x][1], EOTSBuffCoordinates[x][2], EOTSBuffCoordinates[x][3], 0, 114, 1);

		m_EOTSbuffs[x]->SetFloatValue(GAMEOBJECT_ROTATION_02, EOTSBuffRotations[x][0]);
		m_EOTSbuffs[x]->SetFloatValue(GAMEOBJECT_ROTATION_03, EOTSBuffRotations[x][1]);
		m_EOTSbuffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);		//STATE
		m_EOTSbuffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);      //GAMEOBJECT_TYPEID
		m_EOTSbuffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);	//ANIM_PROGRESS
		m_EOTSbuffs[x]->PushToWorld(m_mapMgr);
	}
	else
	{
		if(m_EOTSbuffs[x]->IsInWorld())
			m_EOTSbuffs[x]->RemoveFromWorld(false);

		if(rand_buffid != m_EOTSbuffs[x]->GetEntry())
		{
			m_EOTSbuffs[x]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
			m_EOTSbuffs[x]->SetUInt32Value(OBJECT_FIELD_ENTRY, rand_buffid);
			m_EOTSbuffs[x]->SetInfo(goi);
		}

		m_EOTSbuffs[x]->PushToWorld(m_mapMgr);
	}
}


LocationVector EyeOfTheStorm::GetStartingCoords(uint32 Team)
{
	return LocationVector( EOTSStartLocations[Team][0], 
		EOTSStartLocations[Team][1],
		EOTSStartLocations[Team][2] );
}

void EyeOfTheStorm::OnStart()
{
	for(uint32 i = 0; i < 2; ++i)
	{
		for(set<Player*  >::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
		{
			(*itr)->RemoveAura(BG_PREPARATION);
		}
	}

	uint32 i;

	/* start the events */
	sEventMgr.AddEvent(TO_EYEOFTHESTORM(this), &EyeOfTheStorm::GeneratePoints, EVENT_EOTS_GIVE_POINTS, 2000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	sEventMgr.AddEvent(TO_EYEOFTHESTORM(this), &EyeOfTheStorm::UpdateCPs, EVENT_EOTS_CHECK_CAPTURE_POINT_STATUS, 5000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	/* spirit guides */
	AddSpiritGuide(SpawnSpiritGuide( EOTSStartLocations[0][0], EOTSStartLocations[0][1], EOTSStartLocations[0][2], 0, 0 ));
	AddSpiritGuide(SpawnSpiritGuide( EOTSStartLocations[1][0], EOTSStartLocations[1][1], EOTSStartLocations[1][2], 0, 1 ));

	/* remove the bubbles */
	for( i = 0; i < 2; ++i )
	{
		m_bubbles[i]->RemoveFromWorld(false);
		m_bubbles[i]->Destructor();
	}

	m_started = true;
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);
}

void EyeOfTheStorm::HookGenerateLoot(Player* plr, Corpse* pCorpse)
{
	// add some money
	float gold = ((float(plr->getLevel()) / 2.5f)+1) * 100.0f;			// fix this later
	gold *= sWorld.getRate(RATE_MONEY);

	// set it
	pCorpse->m_loot.gold = float2int32(gold);
}

void EyeOfTheStorm::HookOnShadowSight() 
{
}

void EyeOfTheStorm::SetIsWeekend(bool isweekend) 
{
	m_isWeekend = isweekend;
	if (isweekend)
	{
		m_resToGainBH = 200;
	}
	else
	{
		m_resToGainBH = 330;
	}
}