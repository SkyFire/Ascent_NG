/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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
UpdateMask Player::m_visibleUpdateMask;
#define COLLISION_MOUNT_CHECK_INTERVAL 1000

const uint32 Player::PvPRanks[] = { 
	PVPTITLE_NONE,			// 0
	PVPTITLE_PRIVATE,		// 1
	PVPTITLE_CORPORAL,		// 2
	PVPTITLE_SERGEANT,		// 3
	PVPTITLE_MASTER_SERGEANT, // 4
	PVPTITLE_SERGEANT_MAJOR,	// 5
	PVPTITLE_KNIGHT,			// 6
	PVPTITLE_KNIGHT_LIEUTENANT, // 7
	PVPTITLE_KNIGHT_CAPTAIN,	// 8
	PVPTITLE_KNIGHT_CHAMPION,	// 9
	PVPTITLE_LIEUTENANT_COMMANDER,	// 10
	PVPTITLE_COMMANDER,				// 11
	PVPTITLE_MARSHAL,				// 12
	PVPTITLE_FIELD_MARSHAL,			// 13
	PVPTITLE_GRAND_MARSHAL,			// 14
	PVPTITLE_NONE,					// 15
	PVPTITLE_SCOUT, 
	PVPTITLE_GRUNT, 
	PVPTITLE_HSERGEANT, 
	PVPTITLE_SENIOR_SERGEANT, 
	PVPTITLE_FIRST_SERGEANT, 
	PVPTITLE_STONE_GUARD, 
	PVPTITLE_BLOOD_GUARD, 
	PVPTITLE_LEGIONNAIRE, 
	PVPTITLE_CENTURION, 
	PVPTITLE_CHAMPION, 
	PVPTITLE_LIEUTENANT_GENERAL, 
	PVPTITLE_GENERAL, 
	PVPTITLE_WARLORD, 
	PVPTITLE_HIGH_WARLORD 
};

Player::Player( uint32 guid ) : m_mailBox(guid)
{
	m_objectTypeId = TYPEID_PLAYER;
	m_valuesCount = PLAYER_END;
	m_uint32Values = _fields;

	memset(m_uint32Values, 0,(PLAYER_END)*sizeof(uint32));
	m_updateMask.SetCount(PLAYER_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_PLAYER|TYPE_UNIT|TYPE_OBJECT);
	SetUInt32Value( OBJECT_FIELD_GUID,guid);
	m_wowGuid.Init(GetGUID());

	mAngerManagement = false;

	m_finishingmovesdodge = false;
	iActivePet			  = 0;
	resurrector			 = 0;
	SpellCrtiticalStrikeRatingBonus=0;
	SpellHasteRatingBonus   = 0;
	m_lifetapbonus		  = 0;
	info					= NULL;				 // Playercreate info
	SoulStone			   = 0;
	SoulStoneReceiver		= 0;
	bReincarnation			= false;
	m_furorChance			= 0;
	Seal					= 0;
	judgespell			  = 0;
	m_session			   = 0;
	TrackingSpell		   = 0;
	m_status				= 0;
	offhand_dmg_mod		 = 0.5;
	m_walkSpeed			= 2.5f;
	m_runSpeed			  = PLAYER_NORMAL_RUN_SPEED;
	m_isMoving			  = false;
	m_ShapeShifted		  = 0;
	m_curSelection		  = 0;
	m_lootGuid			  = 0;
	m_Summon				= NULL;

	m_PetNumberMax		  = 0;
	m_lastShotTime		  = 0;

	m_H_regenTimer			= 0;
	m_P_regenTimer			= 0;
	m_onTaxi				= false;
	
	m_taxi_pos_x			= 0;
	m_taxi_pos_y			= 0;
	m_taxi_pos_z			= 0;
	m_taxi_ride_time		= 0;

	m_lastCheatDeath = 0;
	// Attack related variables
	m_blockfromspellPCT	 = 0;
	m_blockfromspell		= 0;
	m_critfromspell		 = 0;
	m_spellcritfromspell	= 0;
	m_dodgefromspell		= 0;
	m_parryfromspell		= 0;
	m_hitfromspell		  = 0; 
	m_hitfrommeleespell	 = 0;
	m_meleeattackspeedmod   = 1.0f;
	m_rangedattackspeedmod  = 1.0f;

	m_cheatDeathRank = 0;

	m_healthfromspell	   = 0;
	m_manafromspell		 = 0;
	m_healthfromitems	   = 0;
	m_manafromitems		 = 0;

	m_talentresettimes	  = 0;

	m_nextSave			  = getMSTime() + sWorld.getIntRate(INTRATE_SAVE);

	m_currentSpell		  = NULL;
	m_resurrectHealth	   = m_resurrectMana = 0;

	m_GroupInviter		  = 0;
	
	Lfgcomment = "";
	m_flyHackChances = 5;
	
	for(int i=0;i<3;i++)
	{
		LfgType[i]=0;
		LfgDungeonId[i]=0;
	}
	
	m_Autojoin = false;
	m_AutoAddMem = false;
	LfmDungeonId=0;
	LfmType=0;

	m_invitersGuid		  = 0;

	m_currentMovement	   = MOVE_UNROOT;
	m_isGmInvisible		 = false;

	//DK
	m_invitersGuid		  = 0;

	//Trade
	ResetTradeVariables();
	mTradeTarget = 0;

	//Duel
	DuelingWith			 = NULL;
	m_duelCountdownTimer	= 0;
	m_duelStatus			= 0;
	m_duelState			 = DUEL_STATE_FINISHED;		// finished

	//WayPoint
	waypointunit			= NULL;

	//PVP
	//PvPTimeoutEnabled	   = false;

	//Tutorials
	for ( int aX = 0 ; aX < 8 ; aX++ )
		m_Tutorials[ aX ] = 0x00;

	m_lastRestUpdate		= 0;

	m_lootGuid			  = 0;
	m_banned				= false;

	//Bind possition
	m_bind_pos_x			= 0;
	m_bind_pos_y			= 0;
	m_bind_pos_z			= 0;
	m_bind_mapid			= 0;
	m_bind_zoneid		   = 0;

	// Rest
	m_timeLogoff			= 0;
	m_isResting			 = 0;
	m_restState			 = 0;
	m_restAmount			= 0;
	m_afk_reason			= "";
	m_playedtime[0]		 = 0;
	m_playedtime[1]		 = 0;
	m_playedtime[2]		 = (uint32)UNIXTIME;

	m_AllowAreaTriggerPort  = true;

	// Battleground
	m_bgEntryPointMap	   = 0;
	m_bgEntryPointX		 = 0;	
	m_bgEntryPointY		 = 0;
	m_bgEntryPointZ		 = 0;
	m_bgEntryPointO		 = 0;
	m_bgQueueType = 0;
	m_bgQueueInstanceId = 0;
	m_bgIsQueued = false;
	m_bg = 0;

	m_bgHasFlag			 = false;
	m_bgEntryPointInstance  = 0;

	// gm stuff
	//m_invincible			= false;
	bGMTagOn				= false;
	CooldownCheat		   = false;
	CastTimeCheat		   = false;
	PowerCheat			  = false;
	GodModeCheat			= false;
	FlyCheat				= false;
	
	//FIX for professions
	weapon_proficiency	  = 0x4000;//2^14
	//FIX for shit like shirt etc
	armor_proficiency	   = 1;

	m_bUnlimitedBreath	  = false;
	m_UnderwaterState	   = 0;
	m_UnderwaterTime		= 60000;
	m_UnderwaterMaxTime	 = 60000;
	m_UnderwaterLastDmg	 = getMSTime();
	m_SwimmingTime		  = 0;
	m_BreathDamageTimer	 = 0;

	//transport shit
	m_TransporterGUID	   = 0;
	m_TransporterX		  = 0.0f;
	m_TransporterY		  = 0.0f;
	m_TransporterZ		  = 0.0f;
	m_TransporterO		  = 0.0f;
	m_TransporterUnk		= 0.0f;
	m_lockTransportVariables= false;

	// Autoshot variables
	m_AutoShotTarget		= 0;
	m_onAutoShot			= false;
	m_AutoShotDuration	  = 0;
	m_AutoShotAttackTimer   = 0;
	m_AutoShotSpell		 = NULL;

	m_AttackMsgTimer		= 0;

	timed_quest_slot		= 0;
	m_GM_SelectedGO		 = NULL;

	for(uint32 x = 0;x < 7; x++)
	{
		FlatResistanceModifierPos[x] = 0;
		FlatResistanceModifierNeg[x] = 0;
		BaseResistanceModPctPos[x] = 0;
		BaseResistanceModPctNeg[x] = 0; 
		ResistanceModPctPos[x] = 0;
		ResistanceModPctNeg[x] = 0;
		SpellDelayResist[x] = 0;
		m_casted_amount[x] = 0;
	} 
		
	for(uint32 a = 0; a < 6; a++)
		for(uint32 x = 0; x < 7; x++)
		{	
			SpellDmgDoneByAttribute[a][x] = 0;
			SpellHealDoneByAttribute[a][x] = 0;
		}

	for(uint32 x = 0; x < 5; x++)
	{
		FlatStatModPos[x] = 0;
		FlatStatModNeg[x] = 0;
		StatModPctPos[x] = 0;
		StatModPctNeg[x] = 0;
		TotalStatModPctPos[x] = 0;
		TotalStatModPctNeg[x] = 0;
	}


	for(uint32 x = 0; x < 12; x++)
	{
		IncreaseDamageByType[x] = 0;
		IncreaseDamageByTypePCT[x] = 0;
		IncreaseCricticalByTypePCT[x] = 0;
	}

	PctIgnoreRegenModifier  = 0.0f;
	m_retainedrage          = 0;
	DetectedRange		   = 0;

	m_targetIcon			= 0;
	m_MountSpellId		  = 0;
	bHasBindDialogOpen	  = false;
	m_CurrentCharm		  = NULL;
	m_CurrentTransporter	= NULL;
	m_SummonedObject		= NULL;
	m_currentLoot		   = (uint64)NULL;
	pctReputationMod		= 0;
	roll					= 0;
	mUpdateCount			= 0;
    mCreationCount          = 0;
    bCreationBuffer.reserve(40000);
	bUpdateBuffer.reserve(30000);//ought to be > than enough ;)
	mOutOfRangeIds.reserve(1000);
	mOutOfRangeIdCount	  = 0;

	bProcessPending		 = false;
	for(int i = 0; i < 25; ++i)
		m_questlog[i] = NULL;

	m_ItemInterface		 = new ItemInterface(this);
	CurrentGossipMenu	   = NULL;

	ResetHeartbeatCoords();
	cannibalize			 = false;

	m_AreaID				= 0;
	m_areaDBC				= NULL;
	m_actionsDirty		  = false;
	cannibalizeCount		= 0;
	rageFromDamageDealt	 = 0;

	m_honorToday			= 0;
	m_honorYesterday		= 0;
	m_honorPoints		   = 0;
	m_killsToday			= 0;
	m_killsYesterday		= 0;
	m_killsLifetime		 = 0;
	m_honorless			 = false;
	m_lastSeenWeather	   = 0;
	m_attacking			 = false;
	
	myCorpse				= 0;

	blinked				 = false;
	m_speedhackChances	  = 3;
	m_explorationTimer	  = getMSTime();
	linkTarget			  = 0;
	stack_cheat			 = false;
	triggerpass_cheat = false;
	m_pvpTimer			  = 0;
	m_globalCooldown = 0;
	m_lastHonorResetTime	= 0;
	memset(&mActions, 0, PLAYER_ACTION_BUTTON_SIZE);
	tutorialsDirty = true;
	m_TeleportState = 1;
	m_beingPushed = false;
	flying_aura = 0;
	resend_speed = false;
	rename_pending = false;
	iInstanceType		   = 0;
	memset(reputationByListId, 0, sizeof(FactionReputation*) * 128);

	m_comboTarget = 0;
	m_comboPoints = 0;

	SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 0.0f);
	SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 0.0f);

	UpdateLastSpeeds();

	m_resist_critical[0]=m_resist_critical[1]=0;
	for (uint32 x =0;x<3;x++)
	{
		m_resist_hit[x]=0;
	}
	ok_to_remove = false;
	trigger_on_stun = 0;
	trigger_on_stun_chance = 100;
	m_modphyscritdmgPCT = 0;
	m_RootedCritChanceBonus = 0;

	m_ModInterrMRegenPCT = 0;
	m_ModInterrMRegen =0;
	m_RegenManaOnSpellResist=0;
	m_rap_mod_pct = 0;
	m_modblockabsorbvalue = 0;
	m_modblockvaluefromspells = 0;
	m_summoner = m_summonInstanceId = m_summonMapId = 0;
	m_lastMoveType = 0;
	m_tempSummon = 0;
	m_spellcomboPoints = 0;
	m_pendingBattleground = 0;
	m_deathVision = false;
	m_retainComboPoints = false;
	last_heal_spell = NULL;
	m_playerInfo = NULL;
	m_sentTeleportPosition.ChangeCoords(999999.0f,999999.0f,999999.0f);
	m_speedChangeCounter=1;
	memset(&m_bgScore,0,sizeof(BGScore));
	m_arenaPoints = 0;
	memset(&m_spellIndexTypeTargets, 0, sizeof(uint64)*NUM_SPELL_TYPE_INDEX);
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;
	m_arenateaminviteguid=0;
	m_arenaPoints=0;
	m_honorRolloverTime=0;
	hearth_of_wild_pct = 0;
	raidgrouponlysent=false;
	m_waterwalk=false;
	m_setwaterwalk=false;
	m_areaSpiritHealer_guid=0;
	m_CurrentTaxiPath=NULL;
	m_setflycheat = false;
	m_fallDisabledUntil = 0;
	m_lfgMatch = NULL;
	m_lfgInviterGuid = 0;
	m_mountCheckTimer = 0;
	m_taxiMapChangeNode = 0;
	m_startMoveTime = 0;
	m_canCastSpellsWhileDead = false;
	this->OnLogin();

#ifdef ENABLE_COMPRESSED_MOVEMENT
	m_movementBuffer.reserve(5000);
#endif

	m_heartbeatDisable = 0;
	m_safeFall = 0;
	m_noFallDamage = false;
	z_axisposition = 0.0f;
	m_KickDelay = 0;
	m_speedhackCheckTimer = 0;
	m_speedChangeInProgress = false;
	m_passOnLoot = false;
	m_changingMaps = true;
	m_outStealthDamageBonusPct = m_outStealthDamageBonusPeriod = m_outStealthDamageBonusTimer = 0;
	m_vampiricEmbrace = m_vampiricTouch = 0;
	m_magnetAura = NULL;
	m_lastMoveTime = 0;
	m_lastMovementPacketTimestamp = 0;
	m_cheatEngineChances = 2;
	m_mageInvisibility = false;

	mWeakenedSoul = false;
	mForbearance = false;
	mHypothermia = false;
	m_flyhackCheckTimer = 0;
	m_bgFlagIneligible = 0;
	m_moltenFuryDamageIncreasePct = 0;
	m_insigniaTaken = false;
}

void Player::OnLogin()
{

}


Player::~Player ( )
{
	if(!ok_to_remove)
	{
		printf("Player deleted from non-logoutplayer!\n");
		OutputCrashLogLine("Player deleted from non-logoutplayer!");
#ifdef WIN32
		CStackWalker sw;
		sw.ShowCallstack();
#endif
		objmgr.RemovePlayer(this);
	}

	if(m_session)
		m_session->SetPlayer(0);

	Player * pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->mTradeTarget = 0;
	}

	pTarget = objmgr.GetPlayer(GetInviter());
	if(pTarget)
		pTarget->SetInviter(0);

	if(m_Summon)
	{
		m_Summon->Dismiss(true);
		m_Summon->ClearPetOwner();
	}

	mTradeTarget = 0;

	if(DuelingWith != 0)
		DuelingWith->DuelingWith = 0;
	DuelingWith = 0;

	CleanupGossipMenu();
	ASSERT(!IsInWorld());

	sEventMgr.RemoveEvents(this);

	// delete m_talenttree

	CleanupChannels();
	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] != NULL)
		{
			delete m_questlog[i];
		}
	}

	for(SplineMap::iterator itr = _splineMap.begin(); itr != _splineMap.end(); ++itr)
		delete itr->second;

	if(m_ItemInterface)
		delete m_ItemInterface;

	for(ReputationMap::iterator itr = m_reputation.begin(); itr != m_reputation.end(); ++itr)
		delete itr->second;
	m_objectTypeId = TYPEID_UNUSED;

	if(m_playerInfo)
		m_playerInfo->m_loggedInPlayer=NULL;

	while( delayedPackets.size() )
	{
		WorldPacket * pck = delayedPackets.next();
		delete pck;
	}
}

ASCENT_INLINE uint32 GetSpellForLanguage(uint32 SkillID)
{
	switch(SkillID)
	{
	case SKILL_LANG_COMMON:
		return 668;
		break;

	case SKILL_LANG_ORCISH:
		return 669;
		break;

	case SKILL_LANG_TAURAHE:
		return 670;
		break;

	case SKILL_LANG_DARNASSIAN:
		return 671;
		break;

	case SKILL_LANG_DWARVEN:
		return 672;
		break;

	case SKILL_LANG_THALASSIAN:
		return 813;
		break;

	case SKILL_LANG_DRACONIC:
		return 814;
		break;

	case SKILL_LANG_DEMON_TONGUE:
		return 815;
		break;

	case SKILL_LANG_TITAN:
		return 816;
		break;

	case SKILL_LANG_OLD_TONGUE:
		return 817;
		break;

	case SKILL_LANG_GNOMISH:
		return 7430;
		break;

	case SKILL_LANG_TROLL:
		return 7341;
		break;

	case SKILL_LANG_GUTTERSPEAK:
		return 17737;
		break;

	case SKILL_LANG_DRAENEI:
		return 29932;
		break;
	}

	return 0;
}

///====================================================================
///  Create
///  params: p_newChar
///  desc:   data from client to create a new character
///====================================================================
bool Player::Create(WorldPacket& data )
{
	uint8 race,class_,gender,skin,face,hairStyle,hairColor,facialHair,outfitId;

	// unpack data into member variables
	data >> m_name;
	
	// correct capitalization
	CapitalizeString(m_name);

	data >> race >> class_ >> gender >> skin >> face;
	data >> hairStyle >> hairColor >> facialHair >> outfitId;

	info = objmgr.GetPlayerCreateInfo(race, class_);
	if(!info)
	{
		// info not found... disconnect
		//sCheatLog.writefromsession(m_session, "tried to create invalid player with race %u and class %u", race, class_);
		m_session->Disconnect();
		return false;
	}

	// check that the account CAN create TBC characters, if we're making some
	if(race >= RACE_BLOODELF && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_01))
	{
		//sCheatLog.writefromsession(m_session, "tried to create player with race %u and class %u but no expansion flags", race, class_);
		m_session->Disconnect();
		return false;
	}

	m_mapId = info->mapId;
	m_zoneId = info->zoneId;
	m_position.ChangeCoords(info->positionX, info->positionY, info->positionZ);
	m_bind_pos_x = info->positionX;
	m_bind_pos_y = info->positionY;
	m_bind_pos_z = info->positionZ;
	m_bind_mapid = info->mapId;
	m_bind_zoneid = info->zoneId;
	m_isResting = 0;
	m_restAmount = 0;
	m_restState = 0;

	memset(m_taximask, 0, sizeof(uint32)*8);
	
	// set race dbc
	myRace = dbcCharRace.LookupEntry(race);
	myClass = dbcCharClass.LookupEntry(class_);
	if(!myRace || !myClass)
	{
		// information not found
		sCheatLog.writefromsession(m_session, "tried to create invalid player with race %u and class %u, dbc info not found", race, class_);
		m_session->Disconnect();
		return false;
	}

	if(myRace->team_id == 7)
		m_team = 0;
	else
		m_team = 1;

	uint8 powertype = myClass->power_type;

	// Automatically add the race's taxi hub to the character's taximask at creation time ( 1 << (taxi_node_id-1) )
	memset(m_taximask,0,sizeof(m_taximask));
	switch(race)
	{
	case RACE_TAUREN:	m_taximask[0]= 2097152;		break;
	case RACE_HUMAN:	m_taximask[0]= 2;			break;
	case RACE_DWARF:	m_taximask[0]= 32;			break;
	case RACE_GNOME:	m_taximask[0]= 32;			break;
	case RACE_ORC:		m_taximask[0]= 4194304;		break;
	case RACE_TROLL:	m_taximask[0]= 4194304;		break;
	case RACE_UNDEAD:	m_taximask[0]= 1024;		break;
	case RACE_NIGHTELF:	m_taximask[0]= 100663296;	break;
	case RACE_BLOODELF:	m_taximask[2]= 131072;		break;
	case RACE_DRAENEI:	m_taximask[2]= 536870912;	break;
	}

	// Set Starting stats for char
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((race==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	//SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // this gets devided by 10
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
   
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	
	//THIS IS NEEDED
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana );
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate );
	SetUInt32Value(UNIT_FIELD_LEVEL, 1 );
	
	SetUInt32Value(UNIT_FIELD_BYTES_0, ( ( race ) | ( class_ << 8 ) | ( gender << 16 ) | ( powertype << 24 ) ) );
	//UNIT_FIELD_BYTES_1	(standstate) | (unk1) | (unk2) | (attackstate)
	SetUInt32Value(UNIT_FIELD_BYTES_2, (0x28 << 8) );
	if(class_ == WARRIOR)
		SetShapeShift(FORM_BATTLESTANCE);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
	SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.388999998569489f );
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f   );
	if(race != 10)
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId + gender );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId + gender );
	} else	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId - gender );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId - gender );
	}
	//SetFloatValue(UNIT_FIELD_MINDAMAGE, info->mindmg );
	//SetFloatValue(UNIT_FIELD_MAXDAMAGE, info->maxdmg );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );
	SetUInt32Value(PLAYER_BYTES, ((skin) | (face << 8) | (hairStyle << 16) | (hairColor << 24)));
	//PLAYER_BYTES_2							   GM ON/OFF	 BANKBAGSLOTS   RESTEDSTATE
   // SetUInt32Value(PLAYER_BYTES_2, (facialHair | (0xEE << 8) | (0x01 << 16) | (0x02 << 24)));
	SetUInt32Value(PLAYER_BYTES_2, (facialHair /*| (0xEE << 8)*/  | (0x02 << 24)));//no bank slot by default!

	//PLAYER_BYTES_3						   DRUNKENSTATE				 PVPRANK
	SetUInt32Value(PLAYER_BYTES_3, ((gender) | (0x00 << 8) | (0x00 << 16) | (GetPVPRank() << 24)));
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, 400);
	SetUInt32Value(PLAYER_FIELD_BYTES, 0x08 );
	SetUInt32Value(PLAYER_CHARACTER_POINTS2,2);
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);
  
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

	SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, 0xEEEEEEEE);

	m_StableSlotCount = 0;
	Item *item;

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		mSpells.insert((*sp));
	}

	m_FirstLogin = true;

	skilllineentry * se;
	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
	{
		se = dbcSkillLine.LookupEntry(ss->skillid);
		if(se->type != SKILL_TYPE_LANGUAGE)
			_AddSkillLine(se->id, ss->currentval, ss->maxval);
	}
	//Chances depend on stats must be in this order!
//	UpdateStats();
	//UpdateChances();
	
	_InitialReputation();

	// Add actionbars
	for(std::list<CreateInfo_ActionBarStruct>::iterator itr = info->actionbars.begin();itr!=info->actionbars.end();++itr)
	{
		setAction(itr->button, itr->action, itr->type, itr->misc);
	}

	for(std::list<CreateInfo_ItemStruct>::iterator is = info->items.begin(); is!=info->items.end(); is++)
	{
		if ( (*is).protoid != 0)
		{
			item=objmgr.CreateItem((*is).protoid,this);
			if(item)
			{
				item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,(*is).amount);
				if((*is).slot<INVENTORY_SLOT_BAG_END)
				{
					if( !GetItemInterface()->SafeAddItem(item, INVENTORY_SLOT_NOT_SET, (*is).slot) )
						delete item;
				}
				else
				{
					if( !GetItemInterface()->AddItemToFreeSlot(item) )
						delete item;
				}
			}
		}
	}

	sHookInterface.OnCharacterCreate(this);

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];
	return true;
}


void Player::Update( uint32 p_time )
{
	if(!IsInWorld())
		return;

	Unit::Update( p_time );
	uint32 mstime = getMSTime();

	if(m_attacking)
	{
		// Check attack timer.
		if(mstime >= m_attackTimer)
			_EventAttack(false);

		if(m_duelWield && mstime >= m_attackTimer_1)
			_EventAttack(true);
	}

	if( m_onAutoShot)
	{
		if( m_AutoShotAttackTimer > p_time )
		{
			//DEBUG_LOG( "HUNTER AUTOSHOT 0) %i, %i", m_AutoShotAttackTimer, p_time );
			m_AutoShotAttackTimer -= p_time;
		}
		else
		{
			//DEBUG_LOG( "HUNTER AUTOSHOT 1) %i", p_time );
			EventRepeatSpell();
		}
	}
	else if(m_AutoShotAttackTimer > 0)
	{
		if(m_AutoShotAttackTimer > p_time)
			m_AutoShotAttackTimer -= p_time;
		else
			m_AutoShotAttackTimer = 0;
	}
	
	// Breathing
	if( m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		// keep subtracting timer
		if( m_UnderwaterTime )
		{
			// not taking dmg yet
			if(p_time >= m_UnderwaterTime)
				m_UnderwaterTime = 0;
			else
				m_UnderwaterTime -= p_time;
		}

		if( !m_UnderwaterTime )
		{
			// check last damage dealt timestamp, and if enough time has elapsed deal damage
			if( mstime >= m_UnderwaterLastDmg )
			{
				uint32 damage = m_uint32Values[UNIT_FIELD_MAXHEALTH] / 10;
				WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
				data << GetGUID() << uint8(DAMAGE_DROWNING) << damage << uint64(0);
				SendMessageToSet(&data, true);

				DealDamage(this, damage, 0, 0, 0);
				m_UnderwaterLastDmg = mstime + 1000;
			}
		}
	}
	else
	{
		// check if we're not on a full breath timer
		if(m_UnderwaterTime < m_UnderwaterMaxTime)
		{
			// regenning
			m_UnderwaterTime += (p_time * 10);

			if(m_UnderwaterTime >= m_UnderwaterMaxTime)
			{
				m_UnderwaterTime = m_UnderwaterMaxTime;
				StopMirrorTimer(1);
			}
		}
	}

	// Lava Damage
	if(m_UnderwaterState & UNDERWATERSTATE_LAVA)
	{
		// check last damage dealt timestamp, and if enough time has elapsed deal damage
		if(mstime >= m_UnderwaterLastDmg)
		{
			uint32 damage = m_uint32Values[UNIT_FIELD_MAXHEALTH] / 5;
			WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
			data << GetGUID() << uint8(DAMAGE_LAVA) << damage << uint64(0);
			SendMessageToSet(&data, true);

			DealDamage(this, damage, 0, 0, 0);
			m_UnderwaterLastDmg = mstime + 1000;
		}
	}

	// Autosave
	if(mstime >= m_nextSave)
		SaveToDB(false);

	if(m_CurrentTransporter && !m_lockTransportVariables)
	{
		// Update our position, using trnasporter X/Y
		float c_tposx = m_CurrentTransporter->GetPositionX() + m_TransporterX;
		float c_tposy = m_CurrentTransporter->GetPositionY() + m_TransporterY;
		float c_tposz = m_CurrentTransporter->GetPositionZ() + m_TransporterZ;
		SetPosition(c_tposx, c_tposy, c_tposz, GetOrientation(), false);
	}

	// Exploration
	if(mstime >= m_explorationTimer)
	{
		_EventExploration();
		m_explorationTimer = mstime + 3000;
	}

	if(m_pvpTimer)
	{
		if(p_time >= m_pvpTimer)
		{
			RemovePvPFlag();
			m_pvpTimer = 0;
		}
		else
			m_pvpTimer -= p_time;
	}

#ifdef COLLISION
	if(m_MountSpellId != 0)
	{
		if( mstime >= m_mountCheckTimer )
		{
			//if( CollideInterface.IsIndoor( m_mapId, m_position ) )
			if( CollideInterface.IsIndoor( m_mapId, m_position.x, m_position.y, m_position.z ) )
			{
				RemoveAura( m_MountSpellId );
				m_MountSpellId = 0;
			}
			else
			{
				m_mountCheckTimer = mstime + COLLISION_MOUNT_CHECK_INTERVAL;
			}
		}
	}
#endif

	if( mstime >= m_speedhackCheckTimer )
	{
		_SpeedhackCheck();
		m_speedhackCheckTimer = mstime + 1000;
	}

	if( mstime >= m_flyhackCheckTimer )
	{
		_FlyhackCheck();
		m_flyhackCheckTimer = mstime + 5000;
	}
}

void Player::EventDismount(uint32 money, float x, float y, float z)
{
	ModUnsigned32Value( PLAYER_FIELD_COINAGE , -(int32)money );

	SetPosition(x, y, z, true);
	if(!m_taxiPaths.size())
		SetTaxiState(false);

	SetTaxiPath(NULL);
	UnSetTaxiPos();
	m_taxi_ride_time = 0;

	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetPlayerSpeed(RUN, m_runSpeed);

	sEventMgr.RemoveEvents(this, EVENT_PLAYER_TAXI_INTERPOLATE);

	// Save to database on dismount
	SaveToDB(false);

	// If we have multiple "trips" to do, "jump" on the next one :p
	if(m_taxiPaths.size())
	{
		TaxiPath * p = *m_taxiPaths.begin();
		m_taxiPaths.erase(m_taxiPaths.begin());
		TaxiStart(p, taxi_model_id, 0);
	}

	ResetHeartbeatCoords();
}

void Player::_EventAttack( bool offhand )
{
	if (m_currentSpell)
	{
		m_currentSpell->cancel();
		setAttackTimer(500, offhand);
		return;
	}

	/*if( IsFeared() || IsStunned() )
		return;*/

	if( m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP | UNIT_STATE_STUN | UNIT_STATE_CONFUSE ) || IsStunned() || IsFeared() )
		return;

	Unit *pVictim = NULL;
	if(m_curSelection)
		pVictim = GetMapMgr()->GetUnit(m_curSelection);
	
	//Can't find victim, stop attacking
	if (!pVictim)
	{
		DEBUG_LOG("Player::Update:  No valid current selection to attack, stopping attack\n");
		setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		EventAttackStop();
		return;
	}

	if (!canReachWithAttack(pVictim))
	{
		if(m_AttackMsgTimer != 1)
		{
			m_session->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
			m_AttackMsgTimer = 1;
		}
		setAttackTimer(300, offhand);
	}
	else if(!isInFront(pVictim))
	{
		// We still have to do this one.
		if(m_AttackMsgTimer != 2)
		{
			m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
			m_AttackMsgTimer = 2;
		}
		setAttackTimer(300, offhand);
	}
	else
	{
		m_AttackMsgTimer = 0;
		
		// Set to weapon time.
		setAttackTimer(0, offhand);

		//pvp timeout reset
		if(pVictim->IsPlayer())
		{
			if (static_cast< Player* >(pVictim)->cannibalize)
			{
				sEventMgr.RemoveEvents(pVictim, EVENT_CANNIBALIZE);
				pVictim->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				static_cast< Player* >(pVictim)->cannibalize = false;
			}
		}

		if(this->IsStealth())
		{
			RemoveAura( m_stealth );
			SetStealth(0);
		}

		if (!GetOnMeleeSpell() || offhand)
		{
			Strike( pVictim, ( offhand ? OFFHAND : MELEE ), NULL, 0, 0, 0, false, false );
				
		} 
		else 
		{ 
			SpellEntry *spellInfo = dbcSpell.LookupEntry(GetOnMeleeSpell());
			SetOnMeleeSpell(0);
			Spell *spell = new Spell(this,spellInfo,true,NULL);
			SpellCastTargets targets;
			targets.m_unitTarget = GetSelection();
			spell->prepare(&targets);
		}
	}
}

void Player::_EventCharmAttack()
{
	if(!m_CurrentCharm)
		return;

	Unit *pVictim = NULL;
	if(!IsInWorld())
	{
		m_CurrentCharm=NULL;
		sEventMgr.RemoveEvents(this,EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	if(m_curSelection == 0)
	{
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	pVictim= GetMapMgr()->GetUnit(m_curSelection);

	//Can't find victim, stop attacking
	if (!pVictim)
	{
		DEBUG_LOG( "WORLD: "I64FMT" doesn't exist.",m_curSelection);
		DEBUG_LOG("Player::Update:  No valid current selection to attack, stopping attack\n");
		this->setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		clearStateFlag(UF_ATTACKING);
		EventAttackStop();
	}
	else
	{
		if (!m_CurrentCharm->canReachWithAttack(pVictim))
		{
			if(m_AttackMsgTimer == 0)
			{
				//m_session->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
				m_AttackMsgTimer = 2000;		// 2 sec till next msg.
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(this, EVENT_PLAYER_CHARM_ATTACK, 100);	
		}
		else if(!m_CurrentCharm->isInFront(pVictim))
		{
			if(m_AttackMsgTimer == 0)
			{
				m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
				m_AttackMsgTimer = 2000;		// 2 sec till next msg.
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(this, EVENT_PLAYER_CHARM_ATTACK, 100);	
		}
		else
		{
			//if(pVictim->GetTypeId() == TYPEID_UNIT)
			//	pVictim->GetAIInterface()->StopMovement(5000);

			//pvp timeout reset
			/*if(pVictim->IsPlayer())
			{
				if( static_cast< Player* >( pVictim )->DuelingWith == NULL)//Dueling doesn't trigger PVP
					static_cast< Player* >( pVictim )->PvPTimeoutUpdate(false); //update targets timer

				if(DuelingWith == NULL)//Dueling doesn't trigger PVP
					PvPTimeoutUpdate(false); //update casters timer
			}*/

			if (!m_CurrentCharm->GetOnMeleeSpell())
			{
				m_CurrentCharm->Strike( pVictim, MELEE, NULL, 0, 0, 0, false, false );
			} 
			else 
			{ 
				SpellEntry *spellInfo = dbcSpell.LookupEntry(m_CurrentCharm->GetOnMeleeSpell());
				m_CurrentCharm->SetOnMeleeSpell(0);
				Spell *spell = new Spell(m_CurrentCharm,spellInfo,true,NULL);
				SpellCastTargets targets;
				targets.m_unitTarget = GetSelection();
				spell->prepare(&targets);
				//delete spell;		 // deleted automatically, no need to do this.
			}
		}
	}   
}

void Player::EventAttackStart()
{
	m_attacking = true;
	if(m_MountSpellId)
        RemoveAura(m_MountSpellId);
}

void Player::EventAttackStop()
{
	if(m_CurrentCharm != NULL)
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHARM_ATTACK);

	m_attacking = false;
}

void Player::_EventExploration()
{
	if (isDead())
		return;

	if (!IsInWorld())
		return;

	if(m_position.x > _maxX || m_position.x < _minX || m_position.y > _maxY || m_position.y < _minY)
		return;

	if(GetMapMgr()->GetCellByCoords(GetPositionX(),GetPositionY()) == NULL) 
		return;

	uint16 AreaId = GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY());
	
	if(!AreaId || AreaId == 0xFFFF)
		return;
	AreaTable * at = dbcArea.LookupEntry(AreaId);
	if(at == 0)
		return;

	/*char areaname[200];
	if(at)
	{
		strcpy(areaname, sAreaStore.LookupString((uint32)at->name));
	}
	else
	{
		strcpy(areaname, "UNKNOWN");
	}
    sChatHandler.BlueSystemMessageToPlr(this,areaname);*/

	int offset = at->explorationFlag / 32;
	offset += PLAYER_EXPLORED_ZONES_1;

	uint32 val = (uint32)(1 << (at->explorationFlag % 32));
	uint32 currFields = GetUInt32Value(offset);

	if(AreaId != m_AreaID)
	{
		m_AreaID = AreaId;
		m_areaDBC = dbcArea.LookupEntryForced(m_AreaID);
		UpdatePvPArea();
		if(GetGroup())
            GetGroup()->UpdateOutOfRangePlayer(this, 128, true, NULL);
	}

	if(at->ZoneId != 0)
	{
		if( m_zoneId != at->ZoneId )
			ZoneUpdate(at->ZoneId);
	}
	else if( m_zoneId != at->AreaId )
		ZoneUpdate(at->AreaId);

	// Check for a restable area
    if(at->AreaFlags & AREA_CITY_AREA || at->AreaFlags & AREA_CITY)
	{
		// check faction
		if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at->category == AREAC_HORDE_TERRITORY && GetTeam() == 1) )
		{
			if(!m_isResting) ApplyPlayerRestState(true);
		}
        else if(at->category != AREAC_ALLIANCE_TERRITORY && at->category != AREAC_HORDE_TERRITORY)
		{
			if(!m_isResting) ApplyPlayerRestState(true);
		}
        else
        {
            if(m_isResting) ApplyPlayerRestState(false);
        }
	}
	else
	{
        //second AT check for subzones.
        if(at->ZoneId)
        {
            AreaTable * at2 = dbcArea.LookupEntry(at->ZoneId);
            if(at2 && at2->AreaFlags & AREA_CITY_AREA || at2 && at2->AreaFlags & AREA_CITY)
            {
                if((at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 1) )
		        {
			        if(!m_isResting) ApplyPlayerRestState(true);
		        }
                else if(at2->category != AREAC_ALLIANCE_TERRITORY && at2->category != AREAC_HORDE_TERRITORY)
		        {
			        if(!m_isResting) ApplyPlayerRestState(true);
		        }
                else
                {
                    if(m_isResting) ApplyPlayerRestState(false);
                }
            }
			else
			{
				if(m_isResting)
					ApplyPlayerRestState(false);
			}
        }
        else
        {
		    if(m_isResting) ApplyPlayerRestState(false);
        }
	}

	if( !(currFields & val) && !GetTaxiState() && !m_TransporterGUID)//Unexplored Area		// bur: we dont want to explore new areas when on taxi
	{
		SetUInt32Value(offset, (uint32)(currFields | val));

		uint32 explore_xp = at->level * 10;
		WorldPacket data(SMSG_EXPLORATION_EXPERIENCE, 8);
		data << at->AreaId << explore_xp;
		m_session->SendPacket(&data);

		if(getLevel() < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL) && explore_xp)
			GiveXP(explore_xp, 0, false);
	}
}

void Player::EventDeath()
{
	if (m_state & UF_ATTACKING)
		EventAttackStop();

	if (m_onTaxi)
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_TAXI_DISMOUNT);

	if(!IS_INSTANCE(GetMapId()) && !sEventMgr.HasEvent(this,EVENT_PLAYER_FORECED_RESURECT)) //Should never be true 
		sEventMgr.AddEvent(this,&Player::EventRepopRequestedPlayer,EVENT_PLAYER_FORECED_RESURECT,PLAYER_FORCED_RESURECT_INTERVAL,1,0); //in case he forgets to release spirit (afk or something)
}

///  This function sends the message displaying the purple XP gain for the char
///  It assumes you will send out an UpdateObject packet at a later time.
void Player::GiveXP(uint32 xp, const uint64 &guid, bool allowbonus)
{
	if ( xp < 1 )
		return;

	if(getLevel() >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		return;

	uint32 restxp = xp;

	if(m_restState == RESTSTATE_RESTED && allowbonus)
	{
		restxp = SubtractRestXP(xp);
		xp += restxp;
	}

	UpdateRestState();
	SendLogXPGain(guid,xp,restxp,guid == 0 ? true : false);

	/*
	uint32 curXP = GetUInt32Value(PLAYER_XP);
	uint32 nextLvlXP = GetUInt32Value(PLAYER_NEXT_LEVEL_XP);
	uint32 newXP = curXP + xp;
	uint32 level = GetUInt32Value(UNIT_FIELD_LEVEL);
	bool levelup = false;

	if(m_Summon != NULL && m_Summon->GetUInt32Value(UNIT_CREATED_BY_SPELL) == 0)
		m_Summon->GiveXP(xp);

	uint32 TotalHealthGain = 0, TotalManaGain = 0;
	uint32 cl=getClass();
	// Check for level-up
	while (newXP >= nextLvlXP)
	{
		levelup = true;
		// Level-Up!
		newXP -= nextLvlXP;  // reset XP to 0, but add extra from this xp add
		level ++;	// increment the level
		if( level > 9)
		{
			//Give Talent Point
			uint32 curTalentPoints = GetUInt32Value(PLAYER_CHARACTER_POINTS1);
			SetUInt32Value(PLAYER_CHARACTER_POINTS1,curTalentPoints+1);
		}
	}
	*/

	int32 newxp = m_uint32Values[PLAYER_XP] + xp;
	int32 nextlevelxp = lvlinfo->XPToNextLevel;
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	LevelInfo * li;
	bool levelup = false;

	while(newxp >= nextlevelxp && newxp > 0)
	{
		++level;
		li = objmgr.GetLevelInfo(getRace(), getClass(), level);
		newxp -= nextlevelxp;
		nextlevelxp = li->XPToNextLevel;
		levelup = true;

		if(level > 9)
			ModUnsigned32Value(PLAYER_CHARACTER_POINTS1, 1);

		if(level >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
			break;
	}

	if(level > GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		level = GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);

	if(levelup)
	{
		m_playedtime[0] = 0; //Reset the "Current level played time"

		SetUInt32Value(UNIT_FIELD_LEVEL, level);
		LevelInfo * oldlevel = lvlinfo;
		lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), level);
		CalculateBaseStats();

		// Generate Level Info Packet and Send to client
        SendLevelupInfo(
            level,
            lvlinfo->HP - oldlevel->HP,
            lvlinfo->Mana - oldlevel->Mana,
            lvlinfo->Stat[0] - oldlevel->Stat[0],
            lvlinfo->Stat[1] - oldlevel->Stat[1],
            lvlinfo->Stat[2] - oldlevel->Stat[2],
            lvlinfo->Stat[3] - oldlevel->Stat[3],
            lvlinfo->Stat[4] - oldlevel->Stat[4]);
	
		_UpdateMaxSkillCounts();
		UpdateStats();
		//UpdateChances();
		
		// Set next level conditions
		SetUInt32Value(PLAYER_NEXT_LEVEL_XP, lvlinfo->XPToNextLevel);

		// Set stats
		for(uint32 i = 0; i < 5; ++i)
		{
			BaseStats[i] = lvlinfo->Stat[i];
			CalcStat(i);
		}
		//set full hp and mana
		SetUInt32Value(UNIT_FIELD_HEALTH,GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		SetUInt32Value(UNIT_FIELD_POWER1,GetUInt32Value(UNIT_FIELD_MAXPOWER1));
	}

	// Set the update bit
	SetUInt32Value(PLAYER_XP, newxp);
	
	HandleProc(PROC_ON_GAIN_EXPIERIENCE, this, NULL);
	m_procCounter = 0;
}

void Player::smsg_InitialSpells()
{
	PlayerCooldownMap::iterator itr, itr2;

	uint16 spellCount = (uint16)mSpells.size();
	size_t itemCount = m_cooldownMap[0].size() + m_cooldownMap[1].size();
	uint32 mstime = getMSTime();
	size_t pos;

	WorldPacket data(SMSG_INITIAL_SPELLS, 5 + (spellCount * 4) + (itemCount * 4) );
	data << uint8(0);
	data << uint16(spellCount); // spell count

	SpellSet::iterator sitr;
	for (sitr = mSpells.begin(); sitr != mSpells.end(); ++sitr)
	{
		// todo: check out when we should send 0x0 and when we should send 0xeeee
		// this is not slot,values is always eeee or 0,seems to be cooldown
		data << uint16(*sitr);				   // spell id
		data << uint16(0x0000);	 
	}

	pos = data.wpos();
	data << uint16( 0 );		// placeholder

	itemCount = 0;
	for( itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].begin(); itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end(); )
	{
		itr2 = itr++;

		// don't keep around expired cooldowns
		if( itr2->second.ExpireTime < mstime || (itr2->second.ExpireTime - mstime) < 10000 )
		{
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr2 );
			continue;
		}

		data << uint16( itr2->first );						// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( 0 );								// spell category
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for spell)
		data << uint32( 0 );								// cooldown remaining in ms (for category)

		++itemCount;

		DEBUG_LOG("sending spell cooldown for spell %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
	}

	for( itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].begin(); itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end(); )
	{
		itr2 = itr++;

		// don't keep around expired cooldowns
		if( itr2->second.ExpireTime < mstime || (itr2->second.ExpireTime - mstime) < 10000 )
		{
			m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr2 );
			continue;
		}

		data << uint16( itr2->second.SpellId );				// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( itr2->first );						// spell category
		data << uint32( 0 );								// cooldown remaining in ms (for spell)
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for category)

		++itemCount;

		DEBUG_LOG("InitialSpells", "sending category cooldown for cat %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
	}

	
	*(uint16*)&data.contents()[pos] = (uint16)itemCount;
	GetSession()->SendPacket(&data);

	uint32 v = 0;
	GetSession()->OutPacket(0x041d, 4, &v);
}

void Player::_SavePet(QueryBuffer * buf)
{
	// Remove any existing info
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));
	else
		buf->AddQuery("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));

	if(m_Summon&&m_Summon->IsInWorld()&&m_Summon->GetPetOwner()==this)	// update PlayerPets array with current pet's info
	{
		PlayerPet*pPet = GetPlayerPet(m_Summon->m_PetNumber);
		if(!pPet || pPet->active == false)
			m_Summon->UpdatePetInfo(true);
		else m_Summon->UpdatePetInfo(false);

		if(!m_Summon->Summon)	   // is a pet
		{
			// save pet spellz
			PetSpellMap::iterator itr = m_Summon->mSpells.begin();
			uint32 pn = m_Summon->m_PetNumber;
			if(buf == NULL)
				CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE petnumber=%u", pn);
			else
				buf->AddQuery("DELETE FROM playerpetspells WHERE petnumber=%u", pn);

			for(; itr != m_Summon->mSpells.end(); ++itr)
			{
				if(buf == NULL)
					CharacterDatabase.Execute("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
				else
					buf->AddQuery("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
			}
		}
	}

	std::stringstream ss;

	for(std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin(); itr != m_Pets.end(); itr++)
	{
		ss.rdbuf()->str("");
		ss << "INSERT INTO playerpets VALUES('"
			<< GetLowGUID() << "','"
			<< itr->second->number << "','"
			<< CharacterDatabase.EscapeString(itr->second->name) << "','"
			<< itr->second->entry << "','"
			<< itr->second->fields << "','"
			<< itr->second->xp << "','"
			<< (itr->second->active ?  1 : 0) + itr->second->stablestate * 10 << "','"
			<< itr->second->level << "','"
			<< itr->second->loyaltyxp << "','"
			<< itr->second->actionbar << "','"
			<< itr->second->happinessupdate << "','"
			<< itr->second->summon << "','"
			<< itr->second->loyaltypts << "','"
			<< itr->second->loyaltyupdate << "')";
			
		if(buf == NULL)
			CharacterDatabase.ExecuteNA(ss.str().c_str());
		else
			buf->AddQueryStr(ss.str());
	}
}

void Player::_SavePetSpells(QueryBuffer * buf)
{	
	// Remove any existing
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playersummonspells WHERE ownerguid=%u", GetLowGUID());
	else
		buf->AddQuery("DELETE FROM playersummonspells WHERE ownerguid=%u", GetLowGUID());

	// Save summon spells
	map<uint32, set<uint32> >::iterator itr = SummonSpells.begin();
	for(; itr != SummonSpells.end(); ++itr)
	{
		set<uint32>::iterator it = itr->second.begin();
		for(; it != itr->second.end(); ++it)
		{
			if(buf == NULL)
				CharacterDatabase.Execute("INSERT INTO playersummonspells VALUES(%u, %u, %u)", GetLowGUID(), itr->first, (*it));
			else
				buf->AddQuery("INSERT INTO playersummonspells VALUES(%u, %u, %u)", GetLowGUID(), itr->first, (*it));
		}
	}
}

void Player::AddSummonSpell(uint32 Entry, uint32 SpellID)
{
	SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr == SummonSpells.end())
		SummonSpells[Entry].insert(SpellID);
	else
	{
		set<uint32>::iterator it3;
		for(set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end();)
		{
			it3 = it2++;
			if(dbcSpell.LookupEntry(*it3)->NameHash == sp->NameHash)
				itr->second.erase(it3);
		}
		itr->second.insert(SpellID);
	}
}

void Player::RemoveSummonSpell(uint32 Entry, uint32 SpellID)
{
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr != SummonSpells.end())
	{
		itr->second.erase(SpellID);
		if(itr->second.size() == 0)
			SummonSpells.erase(itr);
	}
}

set<uint32>* Player::GetSummonSpells(uint32 Entry)
{
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr != SummonSpells.end())
	{
		return &(itr->second);
	}
	return 0;
}

void Player::_LoadPet(QueryResult * result)
{
	m_PetNumberMax=0;
	if(!result)
		return;

	do
	{
		Field *fields = result->Fetch();
		fields = result->Fetch();

		PlayerPet *pet = new PlayerPet;
		pet->number  = fields[1].GetUInt32();
		pet->name	= fields[2].GetString();
		pet->entry   = fields[3].GetUInt32();
		pet->fields  = fields[4].GetString();
		pet->xp	  = fields[5].GetUInt32();
		pet->active  = fields[6].GetInt8()%10 > 0 ? true : false;
		pet->stablestate = fields[6].GetInt8() / 10;
		pet->level   = fields[7].GetUInt32();
		pet->loyaltyxp = fields[8].GetUInt32();
		pet->actionbar = fields[9].GetString();
		pet->happinessupdate = fields[10].GetUInt32();
		pet->summon = (fields[11].GetUInt32()>0 ? true : false);
		pet->loyaltypts = fields[12].GetUInt32();
		pet->loyaltyupdate = fields[13].GetUInt32();

		m_Pets[pet->number] = pet;
		if(pet->active)
		{
			if(iActivePet)  // how the hell can this happen
			{
				//printf("pet warning - >1 active pet.. weird..");
			}
			else
				iActivePet = pet->number;
		}	   
		
		if(pet->number > m_PetNumberMax)
			m_PetNumberMax =  pet->number;
	}while(result->NextRow());
}

void Player::SpawnPet(uint32 pet_number)
{
	std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(pet_number);
	if(itr == m_Pets.end())
	{
		DEBUG_LOG("PET SYSTEM: "I64FMT" Tried to load invalid pet %d", GetGUID(), pet_number);
		return;
	}

	if( m_Summon != NULL )
	{
		m_Summon->Remove(true, true, true);
		m_Summon = NULL;
	}

	Pet *pPet = objmgr.CreatePet();
	pPet->SetInstanceID(GetInstanceID());
	pPet->LoadFromDB(this, itr->second);
}

void Player::_LoadPetSpells(QueryResult * result)
{
	std::stringstream query;
	std::map<uint32, std::list<uint32>* >::iterator itr;
	uint32 entry = 0;
	uint32 spell = 0;

	if(result)
	{
		do
		{
			Field *fields = result->Fetch();
			entry = fields[1].GetUInt32();
			spell = fields[2].GetUInt32();
			AddSummonSpell(entry, spell);
		}
		while( result->NextRow() ); 
	}
}

void Player::addSpell(uint32 spell_id)
{
	SpellSet::iterator iter = mSpells.find(spell_id);
	if(iter != mSpells.end())
		return;
		
	mSpells.insert(spell_id);
	if(IsInWorld())
		m_session->OutPacket(SMSG_LEARNED_SPELL, 4, &spell_id);

	// Check if we're a deleted spell
	iter = mDeletedSpells.find(spell_id);
	if(iter != mDeletedSpells.end())
		mDeletedSpells.erase(iter);

	// Check if we're logging in.
	if(!IsInWorld())
		return;

	// Add the skill line for this spell if we don't already have it.
	skilllinespell * sk = objmgr.GetSpellSkill(spell_id);
	if(sk && !_HasSkillLine(sk->skilline))
	{
		skilllineentry * skill = dbcSkillLine.LookupEntry(sk->skilline);
		uint32 max = 5 * getLevel();

		switch(skill->type)
		{
		case SKILL_TYPE_WEAPON:
		case SKILL_TYPE_SECONDARY:
		case SKILL_TYPE_LANGUAGE:
		case SKILL_TYPE_PROFESSION:
			return;
		}

		if(skill->type==SKILL_TYPE_PROFESSION)
			ModUnsigned32Value(PLAYER_CHARACTER_POINTS2,-1);

		_AddSkillLine(sk->skilline, 1, max);
	}
}

//===================================================================================================================
//  Set Create Player Bits -- Sets bits required for creating a player in the updateMask.
//  Note:  Doesn't set Quest or Inventory bits
//  updateMask - the updatemask to hold the set bits
//===================================================================================================================
void Player::_SetCreateBits(UpdateMask *updateMask, Player *target) const
{
	if(target == this)
	{
		Object::_SetCreateBits(updateMask, target);
	}
	else
	{
		for(uint32 index = 0; index < m_valuesCount; index++)
		{
			if(m_uint32Values[index] != 0 && Player::m_visibleUpdateMask.GetBit(index))
				updateMask->SetBit(index);
		}
	}
}


void Player::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
	if(target == this)
	{
		Object::_SetUpdateBits(updateMask, target);
	}
	else
	{
		Object::_SetUpdateBits(updateMask, target);
		*updateMask &= Player::m_visibleUpdateMask;
	}
}


void Player::InitVisibleUpdateBits()
{
	Player::m_visibleUpdateMask.SetCount(PLAYER_END);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_GUID);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_TYPE);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_SCALE_X);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_SUMMON);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_SUMMON+1);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_TARGET);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_TARGET+1);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER5);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXHEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER5);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_LEVEL);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FACTIONTEMPLATE);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_0);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS_2);
	for(uint32 i = UNIT_FIELD_AURA; i <= UNIT_FIELD_AURASTATE; i ++)
		Player::m_visibleUpdateMask.SetBit(i);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASEATTACKTIME);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASEATTACKTIME+1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BOUNDINGRADIUS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_COMBATREACH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_DISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_NATIVEDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MOUNTDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MOUNTDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_PETNUMBER);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_PET_NAME_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT+1);
	Player::m_visibleUpdateMask.SetBit(UNIT_CHANNEL_SPELL);
	Player::m_visibleUpdateMask.SetBit(UNIT_DYNAMIC_FLAGS);

	Player::m_visibleUpdateMask.SetBit(PLAYER_FLAGS);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_2);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_3);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILD_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_TEAM);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER+1);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDID);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDRANK);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_2);

	for(uint16 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		Player::m_visibleUpdateMask.SetBit((uint16)(PLAYER_VISIBLE_ITEM_1_0 + (i*16))); // visual items for other players
		Player::m_visibleUpdateMask.SetBit((uint16)(PLAYER_VISIBLE_ITEM_1_0+1 + (i*16))); // visual items for other players
	}

	Player::m_visibleUpdateMask.SetBit(PLAYER_CHOSEN_TITLE);

	/*for(uint16 i = PLAYER_FIELD_ARENA_TEAM_INFO_1_1; i <= PLAYER_FIELD_ARENA_TEAM_INFO_1_17; ++i)
		Player::m_visibleUpdateMask.SetBit(i);*/

	/* fuck i hate const - burlex */
	/*if(target && target->GetGroup() == const_cast<Player*>(this)->GetGroup() && const_cast<Player*>(this)->GetSubGroup() == target->GetSubGroup())
	{
	// quest fields are the same for party members
	for(uint32 i = PLAYER_QUEST_LOG_1_01; i < PLAYER_QUEST_LOG_25_2; ++i)
	Player::m_visibleUpdateMask.SetBit(i);
	}*/
}


void Player::DestroyForPlayer( Player *target ) const
{
	Unit::DestroyForPlayer( target );
}

#define IS_ARENA(x) ( (x) >= BATTLEGROUND_ARENA_2V2 && (x) <= BATTLEGROUND_ARENA_5V5 )

void Player::SaveToDB(bool bNewCharacter /* =false */)
{
	bool in_arena = false;
	QueryBuffer * buf = NULL;
	if(!bNewCharacter)
		buf = new QueryBuffer;

	if( m_bg != NULL && IS_ARENA( m_bg->GetType() ) )
		in_arena = true;

	if(m_uint32Values[PLAYER_CHARACTER_POINTS2]>2)
		m_uint32Values[PLAYER_CHARACTER_POINTS2]=2;
 
	//Calc played times
	uint32 playedt = (uint32)UNIXTIME - m_playedtime[2];
	m_playedtime[0] += playedt;
	m_playedtime[1] += playedt;
	m_playedtime[2] += playedt;
	
	std::stringstream ss;
	ss << "REPLACE INTO characters VALUES ("
		
	<< GetLowGUID() << ", "
	<< GetSession()->GetAccountId() << ","

	// stat saving
	<< "'" << m_name << "', "
	<< uint32(getRace()) << ","
	<< uint32(getClass()) << ","
	<< uint32(getGender()) << ",";

	if(m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] != info->factiontemplate)
		ss << m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] << ",";
	else
		ss << "0,";

	ss << uint32(getLevel()) << ","
	<< m_uint32Values[PLAYER_XP] << ","
	
	// dump exploration data
	<< "'";

	for(uint32 i = 0; i < 64; ++i)
		ss << m_uint32Values[PLAYER_EXPLORED_ZONES_1 + i] << ",";

	ss << "','";

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first && itr->second.Skill->type != SKILL_TYPE_LANGUAGE)
		{
			ss << itr->first << ";"
				<< itr->second.CurrentValue << ";"
				<< itr->second.MaximumValue << ";";
		}
	}

	uint32 player_flags = m_uint32Values[PLAYER_FLAGS];
	{
		// Remove un-needed and problematic player flags from being saved :p
		if(player_flags & PLAYER_FLAG_PARTY_LEADER)
			player_flags &= ~PLAYER_FLAG_PARTY_LEADER;
		if(player_flags & PLAYER_FLAG_AFK)
			player_flags &= ~PLAYER_FLAG_AFK;
		if(player_flags & PLAYER_FLAG_DND)
			player_flags &= ~PLAYER_FLAG_DND;
		if(player_flags & PLAYER_FLAG_GM)
			player_flags &= ~PLAYER_FLAG_GM;
		if(player_flags & PLAYER_FLAG_PVP_TOGGLE)
			player_flags &= ~PLAYER_FLAG_PVP_TOGGLE;
		if(player_flags & PLAYER_FLAG_FREE_FOR_ALL_PVP)
			player_flags &= ~PLAYER_FLAG_FREE_FOR_ALL_PVP;
	}

	ss << "', "
	<< m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX] << ","
	<< m_uint32Values[PLAYER_CHOSEN_TITLE] << ","
	<< m_uint32Values[PLAYER__FIELD_KNOWN_TITLES] << ","
	<< m_uint32Values[PLAYER_FIELD_COINAGE] << ","
	<< m_uint32Values[PLAYER_AMMO_ID] << ","
	<< m_uint32Values[PLAYER_CHARACTER_POINTS2] << ","
	<< m_uint32Values[PLAYER_CHARACTER_POINTS1] << ","
	<< load_health << ","
	<< load_mana << ","
	<< uint32(GetPVPRank()) << ","
	<< m_uint32Values[PLAYER_BYTES] << ","
	<< m_uint32Values[PLAYER_BYTES_2] << ","
	<< player_flags << ","
	<< m_uint32Values[PLAYER_FIELD_BYTES] << ",";

	if( in_arena )
	{
		// if its an arena, save the entry coords instead
		ss << m_bgEntryPointX << ", ";
		ss << m_bgEntryPointY << ", ";
		ss << m_bgEntryPointZ << ", ";
		ss << m_bgEntryPointO << ", ";
		ss << m_bgEntryPointMap << ", ";
	}
	else
	{
		// save the normal position
		ss << m_position.x << ", "
			<< m_position.y << ", "
			<< m_position.z << ", "
			<< m_position.o << ", "
			<< m_mapId << ", ";
	}

	ss << m_zoneId << ", '";
		
	for(uint32 i = 0; i < 8; i++ )
		ss << m_taximask[i] << " ";
	ss << "', "
	
	<< m_banned << ", '"
	<< CharacterDatabase.EscapeString(m_banreason) << "', "
	<< (uint32)UNIXTIME << ",";
	
	//online state
	if(GetSession()->_loggingOut || bNewCharacter)
	{
		ss << "0,";
	}else
	{
		ss << "1,";
	}

	ss	 
	<< m_bind_pos_x			 << ", "
	<< m_bind_pos_y			 << ", "
	<< m_bind_pos_z			 << ", "
	<< m_bind_mapid			 << ", "
	<< m_bind_zoneid			<< ", "
		
	<< uint32(m_isResting)	  << ", "
	<< uint32(m_restState)	  << ", "
	<< uint32(m_restAmount)	 << ", '"
	  
	<< uint32(m_playedtime[0])  << " "
	<< uint32(m_playedtime[1])  << " "
	<< uint32(playedt)		  << " ', "
	<< uint32(m_deathState)	 << ", "

	<< m_talentresettimes	   << ", "
	<< m_FirstLogin			 << ", "
	<< rename_pending
	<< "," << m_arenaPoints << ","
		<< (uint32)m_StableSlotCount << ",";
	
	// instances
	if( in_arena )
	{
		ss << m_bgEntryPointInstance << ", ";
	}
	else
	{
		ss << m_instanceId		   << ", ";
	}

	ss << m_bgEntryPointMap	  << ", " 
	<< m_bgEntryPointX		<< ", " 
	<< m_bgEntryPointY		<< ", " 
	<< m_bgEntryPointZ		<< ", "
	<< m_bgEntryPointO		<< ", "
	<< m_bgEntryPointInstance << ", ";

	// taxi
	if(m_onTaxi&&m_CurrentTaxiPath) {
		ss << m_CurrentTaxiPath->GetID() << ", ";
		ss << lastNode << ", ";
		ss << GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID);
	} else {
		ss << "0, 0, 0";
	}
	
	ss << "," << (m_CurrentTransporter ? m_CurrentTransporter->GetEntry() : (uint32)0);
	ss << ",'" << m_TransporterX << "','" << m_TransporterY << "','" << m_TransporterZ << "'";
	ss << ",'";

	// Dump spell data to stringstream
	SpellSet::iterator spellItr = mSpells.begin();
	for(; spellItr != mSpells.end(); ++spellItr)
	{
		ss << uint32(*spellItr) << ",";
	}
	ss << "','";
	// Dump deleted spell data to stringstream
	spellItr = mDeletedSpells.begin();
	for(; spellItr != mDeletedSpells.end(); ++spellItr)
	{
		ss << uint32(*spellItr) << ",";
	}

	ss << "','";
	// Dump reputation data
	ReputationMap::iterator iter = m_reputation.begin();
	for(; iter != m_reputation.end(); ++iter)
	{
		ss << int32(iter->first) << "," << int32(iter->second->flag) << "," << int32(iter->second->baseStanding) << "," << int32(iter->second->standing) << ",";
	}
	ss << "','";
	
	// Add player action bars
	for(uint32 i = 0; i < 120; ++i)
	{
		ss << uint32(mActions[i].Action) << ","
			<< uint32(mActions[i].Misc) << ","
			<< uint32(mActions[i].Type) << ",";
	}
	ss << "','";

	if(!bNewCharacter)
		SaveAuras(ss);

//	ss << LoadAuras;
	ss << "','";

	// Add player finished quests
	set<uint32>::iterator fq = m_finishedQuests.begin();
	for(; fq != m_finishedQuests.end(); ++fq)
	{
		ss << (*fq) << ",";
	}

	ss << "', ";
	ss << m_honorRolloverTime << ", ";
	ss << m_killsToday << ", " << m_killsYesterday << ", " << m_killsLifetime << ", ";
	ss << m_honorToday << ", " << m_honorYesterday << ", ";
	ss << m_honorPoints << ", ";
    ss << iInstanceType << ")";
	
	if(bNewCharacter)
		CharacterDatabase.WaitExecuteNA(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());

	//Save Other related player stuff

	// Inventory
	 GetItemInterface()->mSaveItemsToDatabase(bNewCharacter, buf);

	// save quest progress
	_SaveQuestLogEntry(buf);

	// Tutorials
	_SaveTutorials(buf);

	// GM Ticket
	objmgr.SaveGMTicket(GetGUID(), buf);

	// Cooldown Items
	_SavePlayerCooldowns( buf );
	
	// Pets
	if(getClass() == HUNTER || getClass() == WARLOCK)
	{
		_SavePet(buf);
		_SavePetSpells(buf);
	}
	m_nextSave = getMSTime() + sWorld.getIntRate(INTRATE_SAVE);

	if(buf)
		CharacterDatabase.AddQueryBuffer(buf);
}

void Player::_SaveQuestLogEntry(QueryBuffer * buf)
{
	for(std::set<uint32>::iterator itr = m_removequests.begin(); itr != m_removequests.end(); ++itr)
	{
		if(buf == NULL)
			CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", GetLowGUID(), (*itr));
		else
			buf->AddQuery("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", GetLowGUID(), (*itr));
	}

	m_removequests.clear();

	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] != NULL)
			m_questlog[i]->SaveToDB(buf);
	}
}

bool Player::canCast(SpellEntry *m_spellInfo)
{
	if (m_spellInfo->EquippedItemClass != 0)
	{
		if(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND))
		{
			if((int32)this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (pow(2.0,(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->SubClass) != m_spellInfo->EquippedItemSubClass))
							return false;
					}
				}
			}
		}
		else if(m_spellInfo->EquippedItemSubClass == 173555)
			return false;

		if (this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED))
		{
			if((int32)this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (pow(2.0,(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->SubClass) != m_spellInfo->EquippedItemSubClass))							return false;
					}
				}
			}
		}
		else if
			(m_spellInfo->EquippedItemSubClass == 262156)
			return false;
	}
	return true;
}

void Player::RemovePendingPlayer()
{
	if(m_session)
	{
		uint8 respons = 0x42;		// CHAR_LOGIN_NO_CHARACTER
		m_session->OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		m_session->m_loggingInPlayer = NULL;
	}

	ok_to_remove = true;
	delete this;
}

bool Player::LoadFromDB(uint32 guid)
{
	AsyncQuery * q = new AsyncQuery( new SQLClassCallbackP0<Player>(this, &Player::LoadFromDBProc) );
	q->AddQuery("SELECT * FROM characters WHERE guid=%u AND forced_rename_pending = 0",guid);
	q->AddQuery("SELECT * FROM tutorials WHERE playerId=%u",guid);
	q->AddQuery("SELECT cooldown_type, cooldown_misc, cooldown_expire_time, cooldown_spellid, cooldown_itemid FROM playercooldowns WHERE player_guid=%u", guid);
	q->AddQuery("SELECT * FROM questlog WHERE player_guid=%u",guid);
	q->AddQuery("SELECT * FROM playeritems WHERE ownerguid=%u ORDER BY containerslot ASC", guid);
	q->AddQuery("SELECT * FROM playerpets WHERE ownerguid=%u ORDER BY petnumber", guid);
	q->AddQuery("SELECT * FROM playersummonspells where ownerguid=%u ORDER BY entryid", guid);
	q->AddQuery("SELECT * FROM mailbox WHERE player_guid = %u", guid);

	// social
	q->AddQuery("SELECT friend_guid, note FROM social_friends WHERE character_guid = %u", guid);
	q->AddQuery("SELECT character_guid FROM social_friends WHERE friend_guid = %u", guid);
	q->AddQuery("SELECT ignore_guid FROM social_ignores WHERE character_guid = %u", guid);

	// queue it!
	m_uint32Values[OBJECT_FIELD_GUID] = guid;
	CharacterDatabase.QueueAsyncQuery(q);
	return true;
}

void Player::LoadFromDBProc(QueryResultVector & results)
{
	uint32 field_index = 2;
#define get_next_field fields[field_index++]

	// set playerinfo
	m_playerInfo = objmgr.GetPlayerInfo(GetLowGUID());
	if( m_playerInfo == NULL )
	{
		RemovePendingPlayer();
		return;
	}

	if(GetSession() == NULL || results.size() < 8)		// should have 8 queryresults for aplayer load.
	{
		RemovePendingPlayer();
		return;
	}

	QueryResult *result = results[0].result;
	if(!result)
	{
		printf("Player login query failed., guid %u\n", GetLowGUID());
		RemovePendingPlayer();
		return;
	}

	Field *fields = result->Fetch();

	if(fields[1].GetUInt32() != m_session->GetAccountId())
	{
		sCheatLog.writefromsession(m_session, "player tried to load character not belonging to them (guid %u, on account %u)",
			fields[0].GetUInt32(), fields[1].GetUInt32());
		RemovePendingPlayer();
		return;
	}

	uint32 banned = fields[32].GetUInt32();
	if(banned && (banned < 100 || banned > (uint32)UNIXTIME))
	{
		RemovePendingPlayer();
		return;
	}

	// Load name
	m_name = get_next_field.GetString();
   
	// Load race/class from fields
	setRace(get_next_field.GetUInt8());
	setClass(get_next_field.GetUInt8());
	setGender(get_next_field.GetUInt8());
	uint32 cfaction = get_next_field.GetUInt32();
	
	// set race dbc
	myRace = dbcCharRace.LookupEntryForced(getRace());
	myClass = dbcCharClass.LookupEntryForced(getClass());
	if(!myClass || !myRace)
	{
		// bad character
		printf("guid %u failed to login, no race or class dbc found. (race %u class %u)\n", (unsigned int)GetLowGUID(), (unsigned int)getRace(), (unsigned int)getClass());
		RemovePendingPlayer();
		return;
	}

	if(myRace->team_id == 7)
	{
		m_bgTeam = m_team = 0;
	}
	else
	{
		m_bgTeam = m_team = 1;
	}

	SetNoseLevel();

	// set power type
	SetPowerType(myClass->power_type);

	// obtain player create info
	info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	assert(info);

	// set level
	m_uint32Values[UNIT_FIELD_LEVEL] = get_next_field.GetUInt32();
	/*if(m_uint32Values[UNIT_FIELD_LEVEL] > 70)
		m_uint32Values[UNIT_FIELD_LEVEL] = 70;*/

	// obtain level/stats information
	lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), getLevel());
	
	if(!lvlinfo)
	{
		printf("guid %u level %u class %u race %u levelinfo not found!\n", (unsigned int)GetLowGUID(), (unsigned int)getLevel(), (unsigned int)getClass(), (unsigned int)getRace());
		RemovePendingPlayer();
		return;
	}
	
	CalculateBaseStats();

	// set xp
	m_uint32Values[PLAYER_XP] = get_next_field.GetUInt32();
	
	// Process exploration data.
	uint32 Counter = 0;
	char * end;
	char * start = (char*)get_next_field.GetString();//buff;
	while(Counter <64) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value(PLAYER_EXPLORED_ZONES_1 + Counter, atol(start));
		start = end +1;
		Counter++;
	}

	// Process skill data.
	Counter = 0;
	start = (char*)get_next_field.GetString();//buff;
	
	// new format
	const ItemProf * prof;
	if(!strchr(start, ' ') && !strchr(start,';'))
	{
		/* no skills - reset to defaults */
		for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
		{
			if(ss->skillid && ss->currentval && ss->maxval && !::GetSpellForLanguage(ss->skillid))
				_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);		
		}
	}
	else
	{
		char * f = strdup(start);
		start = f;
		if(!strchr(start,';'))
		{
			/* old skill format.. :< */
			uint32 v1,v2,v3;
			PlayerSkill sk;
			for(;;)
			{
				end = strchr(start, ' ');
				if(!end)
					break;

				*end = 0;
				v1 = atol(start);
				start = end + 1;

				end = strchr(start, ' ');
				if(!end)
					break;

				*end = 0;
				v2 = atol(start);
				start = end + 1;

				end = strchr(start, ' ');
				if(!end)
					break;

				v3 = atol(start);
				start = end + 1;
				if(v1 & 0xffff)
				{
					sk.Reset(v1 & 0xffff);
					sk.CurrentValue = v2 & 0xffff;
					sk.MaximumValue = (v2 >> 16) & 0xffff;

					if( !sk.CurrentValue )
						sk.CurrentValue = 1;

					m_skills.insert( make_pair(sk.Skill->id, sk) );
				}
			}
		}
		else
		{
			uint32 v1,v2,v3;
			PlayerSkill sk;
			for(;;)
			{
				end = strchr(start, ';');
				if(!end)
					break;

				*end = 0;
				v1 = atol(start);
				start = end + 1;

				end = strchr(start, ';');
				if(!end)
					break;

				*end = 0;
				v2 = atol(start);
				start = end + 1;

				end = strchr(start, ';');
				if(!end)
					break;

				v3 = atol(start);
				start = end + 1;

				/* add the skill */
				if(v1)
				{
					sk.Reset(v1);
					sk.CurrentValue = v2;
					sk.MaximumValue = v3;

					if( !sk.CurrentValue )
						sk.CurrentValue = 1;

					m_skills.insert(make_pair(v1, sk));
				}
			}
		}
		free(f);
	}

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first == SKILL_RIDING)
		{
			itr->second.CurrentValue = itr->second.MaximumValue;
		}

		prof = GetProficiencyBySkill(itr->first);
		if(prof)
		{
			if(prof->itemclass==4)
				armor_proficiency|=prof->subclass;
			else
				weapon_proficiency|=prof->subclass;
		}
	}

	// set the rest of the shit
	m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX]  = get_next_field.GetUInt32();
	m_uint32Values[PLAYER_CHOSEN_TITLE]				 = get_next_field.GetUInt32();
	field_index++;
	m_uint32Values[PLAYER_FIELD_COINAGE]				= get_next_field.GetUInt32();
	m_uint32Values[PLAYER_AMMO_ID]					  = get_next_field.GetUInt32();
	m_uint32Values[PLAYER_CHARACTER_POINTS2]			= get_next_field.GetUInt32();
	m_uint32Values[PLAYER_CHARACTER_POINTS1]			= get_next_field.GetUInt32();
	load_health										 = get_next_field.GetUInt32();
	load_mana										   = get_next_field.GetUInt32();
	SetUInt32Value(UNIT_FIELD_HEALTH, load_health);
	uint8 pvprank = get_next_field.GetUInt8();
	SetUInt32Value(PLAYER_BYTES, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_BYTES_2, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_BYTES_3, getGender() | (pvprank << 24));
	uint32 offset = 15 * GetTeam();
	SetUInt32Value(PLAYER__FIELD_KNOWN_TITLES, PvPRanks[GetPVPRank() + offset]);
	SetUInt32Value(PLAYER_FLAGS, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_FIELD_BYTES, get_next_field.GetUInt32());
	//m_uint32Values[0x22]=(m_uint32Values[0x22]>0x46)?0x46:m_uint32Values[0x22];

	m_position.x										= get_next_field.GetFloat();
	m_position.y										= get_next_field.GetFloat();
	m_position.z										= get_next_field.GetFloat();
	m_position.o										= get_next_field.GetFloat();

	m_mapId											 = get_next_field.GetUInt32();
	m_zoneId											= get_next_field.GetUInt32();

	// Calculate the base stats now they're all loaded
	for(uint32 i = 0; i < 5; ++i)
		CalcStat(i);

  //  for(uint32 x = PLAYER_SPELL_CRIT_PERCENTAGE1; x < PLAYER_SPELL_CRIT_PERCENTAGE06 + 1; ++x)
	///	SetFloatValue(x, 0.0f);

	for(uint32 x = PLAYER_FIELD_MOD_DAMAGE_DONE_PCT; x < PLAYER_FIELD_MOD_HEALING_DONE_POS; ++x)
		SetFloatValue(x, 1.0f);

	// Normal processing...
//	UpdateMaxSkills();
	UpdateStats();
	//UpdateChances();

	// Initialize 'normal' fields
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((getRace()==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	//SetUInt32Value(UNIT_FIELD_POWER2, 0);
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus);
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	if(getClass() == WARRIOR)
		SetShapeShift(FORM_BATTLESTANCE);

	SetUInt32Value(UNIT_FIELD_BYTES_2, (0x28 << 8) );
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.388999998569489f );
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f   );

	if(getRace() != 10)
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId + getGender() );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId + getGender() );
	} else	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId - getGender() );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId - getGender() );
	}

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate);
	if(cfaction)
	{
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate);
		// re-calculate team
		switch(cfaction)
		{
		case 1:	// human
		case 3:	// dwarf
		case 4: // ne
		case 8:	// gnome
		case 927:	// dreinei
			m_team = m_bgTeam = 0;
			break;

		default:
			m_team = m_bgTeam = 1;
			break;
		}
	}
 
	LoadTaxiMask( get_next_field.GetString() );

	m_banned = get_next_field.GetUInt32(); //Character ban
	m_banreason = get_next_field.GetString();
	m_timeLogoff = get_next_field.GetUInt32();
	field_index++;
	
	m_bind_pos_x = get_next_field.GetFloat();
	m_bind_pos_y = get_next_field.GetFloat();
	m_bind_pos_z = get_next_field.GetFloat();
	m_bind_mapid = get_next_field.GetUInt32();
	m_bind_zoneid = get_next_field.GetUInt32();

	m_isResting = get_next_field.GetUInt8();
	m_restState = get_next_field.GetUInt8();
	m_restAmount = get_next_field.GetUInt32();


	std::string tmpStr = get_next_field.GetString();
	m_playedtime[0] = (uint32)atoi((const char*)strtok((char*)tmpStr.c_str()," "));
	m_playedtime[1] = (uint32)atoi((const char*)strtok(NULL," "));

	m_deathState = (DeathState)get_next_field.GetUInt32();
	m_talentresettimes = get_next_field.GetUInt32();
	m_FirstLogin = get_next_field.GetBool();
	rename_pending = get_next_field.GetBool();
	m_arenaPoints = get_next_field.GetUInt32();
	m_StableSlotCount = get_next_field.GetUInt32();
	m_instanceId = get_next_field.GetUInt32();
	m_bgEntryPointMap = get_next_field.GetUInt32();
	m_bgEntryPointX = get_next_field.GetFloat();
	m_bgEntryPointY = get_next_field.GetFloat();
	m_bgEntryPointZ = get_next_field.GetFloat();
	m_bgEntryPointO = get_next_field.GetFloat();
	m_bgEntryPointInstance = get_next_field.GetUInt32();	

	uint32 taxipath = get_next_field.GetUInt32();
	TaxiPath *path = NULL;
	if(taxipath)
	{
		path = sTaxiMgr.GetTaxiPath(taxipath);
		lastNode = get_next_field.GetUInt32();
		if(path)
		{
			SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, get_next_field.GetUInt32());
			SetTaxiPath(path);
			m_onTaxi = true;
		}
		else
			field_index++;
	}
	else
	{
		field_index++;
		field_index++;
	}

	m_TransporterGUID = get_next_field.GetUInt32();
	if(m_TransporterGUID)
	{
		Transporter * t = objmgr.GetTransporter(GUID_LOPART(m_TransporterGUID));
		m_TransporterGUID = t ? t->GetGUID() : 0;
	}

	m_TransporterX = get_next_field.GetFloat();
	m_TransporterY = get_next_field.GetFloat();
	m_TransporterZ = get_next_field.GetFloat();
	
	// Load Spells from CSV data.
	start = (char*)get_next_field.GetString();//buff;
	SpellEntry * spProto;
	while(true) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		//mSpells.insert(atol(start));
		spProto = dbcSpell.LookupEntryForced(atol(start));
//#define _language_fix_ 1
#ifndef _language_fix_
		if(spProto)
			mSpells.insert(spProto->Id);
#else		
		if (spProto)
		{
			skilllinespell * _spell = objmgr.GetSpellSkill(spProto->Id);
			if (_spell)
			{
				skilllineentry * _skill = dbcSkillLine.LookupEntry(_spell->skilline);
				if (_skill && _skill->type != SKILL_TYPE_LANGUAGE)
				{
					mSpells.insert(spProto->Id);
				}
			}
			else
			{
				mSpells.insert(spProto->Id);

				// Cheat Death hack!
				if(_spell->Id == 31230)
					m_cheatDeathRank = 3;
				else if(_spell->Id == 31329)
					m_cheatDeathRank = 2;
				else if(_spell->Id == 31328)
					m_cheatDeathRank = 1;
			}
		}
#endif
//#undef _language_fix_
			
		start = end +1;
	}

	start = (char*)get_next_field.GetString();//buff;
	while(true) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		spProto = dbcSpell.LookupEntryForced(atol(start));
		if(spProto)
			mDeletedSpells.insert(spProto->Id);
		start = end +1;
	}

	// Load Reputatation CSV Data
	start =(char*) get_next_field.GetString();
	FactionDBC * factdbc ;
	FactionReputation * rep;
	uint32 id;
	int32 basestanding;
	int32 standing;
	uint32 fflag;
	while(true) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		id = atol(start);
		start = end +1;

		end = strchr(start,',');
		if(!end)break;
		*end=0;
		fflag = atol(start);
		start = end +1;
		
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		basestanding = atoi(start);//atol(start);
		start = end +1;
		
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		standing  = atoi(start);// atol(start);
		start = end +1;

		// listid stuff
		factdbc = dbcFaction.LookupEntryForced(id);
		if(!factdbc) continue;
		ReputationMap::iterator rtr = m_reputation.find(id);
		if(rtr != m_reputation.end())
			delete rtr->second;

		rep = new FactionReputation;
		rep->baseStanding = basestanding;
		rep->standing = standing;
		rep->flag = fflag;
		m_reputation[id]=rep;

		// do listid stuff
		if(factdbc->RepListId >= 0)
			reputationByListId[factdbc->RepListId] = rep;
	}

	if(!m_reputation.size())
		_InitialReputation();

	// Load saved actionbars
	start =  (char*)get_next_field.GetString();
	Counter =0;
	while(Counter < 120)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Action = (uint16)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Misc = (uint8)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter++].Type = (uint8)atol(start);
		start = end +1;
	}
	
	//LoadAuras = get_next_field.GetString();
	start = (char*)get_next_field.GetString();//buff;
	do 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		LoginAura la;
		la.id = atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		la.dur = atol(start);
		start = end +1;
		loginauras.push_back(la);
	} while(true);

	// Load saved finished quests

	start =  (char*)get_next_field.GetString();
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		m_finishedQuests.insert(atol(start));
		start = end +1;
	}
	
	m_honorRolloverTime = get_next_field.GetUInt32();
	m_killsToday = get_next_field.GetUInt32();
	m_killsYesterday = get_next_field.GetUInt32();
	m_killsLifetime = get_next_field.GetUInt32();
	
	m_honorToday = get_next_field.GetUInt32();
	m_honorYesterday = get_next_field.GetUInt32();
	m_honorPoints = get_next_field.GetUInt32();

	RolloverHonor();
    iInstanceType = get_next_field.GetUInt32();

	HonorHandler::RecalculateHonorFields(this);
	
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);
  
	_setFaction();
   
	//class fixes
	switch(getClass())
	{
	case PALADIN:
		armor_proficiency|=(1<<7);//LIBRAM
		break;
	case DRUID:
		armor_proficiency|=(1<<8);//IDOL
		break;
	case SHAMAN:
		armor_proficiency|=(1<<9);//TOTEM
		break;
	case WARLOCK:
	case HUNTER:
		_LoadPet(results[5].result);
        _LoadPetSpells(results[6].result);
	break;
	}

	if(m_session->CanUseCommand('c'))
		_AddLanguages(true);
	else
		_AddLanguages(false);

	OnlineTime	= (uint32)UNIXTIME;
	if(GetGuildId())
		SetUInt32Value(PLAYER_GUILD_TIMESTAMP, (uint32)UNIXTIME);

#undef get_next_field

	// load properties
	_LoadTutorials(results[1].result);
	_LoadPlayerCooldowns(results[2].result);
	_LoadQuestLogEntry(results[3].result);
	m_ItemInterface->mLoadItemsFromDatabase(results[4].result);
	m_mailBox.Load(results[7].result);

	// SOCIAL
	if( results[8].result != NULL )			// this query is "who are our friends?"
	{
		result = results[8].result;
		do 
		{
			fields = result->Fetch();
			if( strlen( fields[1].GetString() ) )
				m_friends.insert( make_pair( fields[0].GetUInt32(), strdup(fields[1].GetString()) ) );
			else
				m_friends.insert( make_pair( fields[0].GetUInt32(), (char*)NULL) );

		} while (result->NextRow());
	}

	if( results[9].result != NULL )			// this query is "who has us in their friends?"
	{
		result = results[9].result;
		do 
		{
			m_hasFriendList.insert( result->Fetch()[0].GetUInt32() );
		} while (result->NextRow());
	}

	if( results[10].result != NULL )		// this query is "who are we ignoring"
	{
		result = results[10].result;
		do 
		{
			m_ignores.insert( result->Fetch()[0].GetUInt32() );
		} while (result->NextRow());
	}

	// END SOCIAL

	UpdateTalentInspectBuffer();
	m_session->FullLogin(this);
	m_session->m_loggingInPlayer=NULL;

	if( !isAlive() )
		myCorpse = objmgr.GetCorpseByOwner(GetLowGUID());
}

void Player::RolloverHonor()
{
	uint32 current_val = (g_localTime.tm_year << 16) | g_localTime.tm_yday;
	if( current_val != m_honorRolloverTime )
	{
		m_honorRolloverTime = current_val;
		m_honorYesterday = m_honorToday;
		m_killsYesterday = m_killsToday;
		m_honorToday = m_killsToday = 0;
	}
}

bool Player::HasSpell(uint32 spell)
{
	return mSpells.find(spell) != mSpells.end();
}

void Player::_LoadQuestLogEntry(QueryResult * result)
{
	QuestLogEntry *entry;
	Quest *quest;
	Field *fields;
	uint32 questid;
	uint32 baseindex;
	
	// clear all fields
	for(int i = 0; i < 25; ++i)
	{
		baseindex = PLAYER_QUEST_LOG_1_1 + (i * 4);
		SetUInt32Value(baseindex + 0, 0);
		SetUInt32Value(baseindex + 1, 0);
		SetUInt32Value(baseindex + 2, 0);
		SetUInt32Value(baseindex + 3, 0);
	}

	int slot = 0;

	if(result)
	{
		do 
		{
			fields = result->Fetch();
			questid = fields[1].GetUInt32();
			quest = QuestStorage.LookupEntry(questid);
			slot = fields[2].GetUInt32();
			ASSERT(slot != -1);
			
			// remove on next save if bad quest
			if(!quest)
			{
				m_removequests.insert(questid);
				continue;
			}
			if(m_questlog[slot] != 0)
				continue;
			
			entry = new QuestLogEntry;
			entry->Init(quest, this, slot);
			entry->LoadFromDB(fields);
			entry->UpdatePlayerFields();

		} while(result->NextRow());
	}
}

QuestLogEntry* Player::GetQuestLogForEntry(uint32 quest)
{
	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] == ((QuestLogEntry*)0x00000001))
			m_questlog[i] = NULL;

		if(m_questlog[i] != NULL)
		{
			if(m_questlog[i]->GetQuest() && m_questlog[i]->GetQuest()->id == quest)
				return m_questlog[i];
		}
	}
	return NULL;
	/*uint32 x = PLAYER_QUEST_LOG_1_1;
	uint32 y = 0;
	for(; x < PLAYER_VISIBLE_ITEM_1_CREATOR && y < 25; x += 3, y++)
	{
		if(m_uint32Values[x] == quest)
			return m_questlog[y];
	}
	return NULL;*/
}

void Player::SetQuestLogSlot(QuestLogEntry *entry, uint32 slot)
{
	m_questlog[slot] = entry;
}

void Player::AddToWorld()
{
	FlyCheat = false;
	m_setflycheat=false;
	// check transporter
	if(m_TransporterGUID && m_CurrentTransporter)
	{
		SetPosition(m_CurrentTransporter->GetPositionX() + m_TransporterX,
			m_CurrentTransporter->GetPositionY() + m_TransporterY,
			m_CurrentTransporter->GetPositionZ() + m_TransporterZ,
			GetOrientation(), false);
	}

	// If we join an invalid instance and get booted out, this will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld();
	
	// Add failed.
	if(m_mapMgr == NULL)
	{
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::AddToWorld(MapMgr * pMapMgr)
{
	FlyCheat = false;
	m_setflycheat=false;
	// check transporter
	if(m_TransporterGUID && m_CurrentTransporter)
	{
		SetPosition(m_CurrentTransporter->GetPositionX() + m_TransporterX,
			m_CurrentTransporter->GetPositionY() + m_TransporterY,
			m_CurrentTransporter->GetPositionZ() + m_TransporterZ,
			GetOrientation(), false);
	}

	// If we join an invalid instance and get booted out, this will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld(pMapMgr);

	// Add failed.
	if(m_mapMgr == NULL)
	{
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::OnPrePushToWorld()
{
	SendInitialLogonPackets();
}

void Player::OnPushToWorld()
{
	if(m_TeleportState == 2)   // Worldport Ack
		OnWorldPortAck();

	ResetSpeedHack();
	m_beingPushed = false;
	AddItemsToWorld();
	m_lockTransportVariables = false;

	// delay the unlock movement packet
	WorldPacket * data = new WorldPacket(SMSG_TIME_SYNC_REQ, 4);
	*data << uint32(0);
	delayedPackets.add(data);
	sWorld.mInWorldPlayerCount++;

	// Update PVP Situation
	LoginPvPSetup();

	Unit::OnPushToWorld();
   
	if(m_FirstLogin)
	{
		sHookInterface.OnFirstEnterWorld(this);
		m_FirstLogin = false;
	}

	// force area update (needed for world states)
	ForceAreaUpdate();

	// send world states
	if( m_mapMgr != NULL )
		m_mapMgr->GetStateManager().SendWorldStates(this);

	// execute some of zeh hooks
	sHookInterface.OnEnterWorld(this);
	sHookInterface.OnZone(this, m_zoneId, 0);

	if(m_TeleportState == 1)		// First world enter
		CompleteLoading();

	m_TeleportState = 0;

	if(GetTaxiState())
	{
		if( m_taxiMapChangeNode != 0 )
		{
			lastNode = m_taxiMapChangeNode;
		}

		// Create HAS to be sent before this!
		ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		TaxiStart(GetTaxiPath(), 
			GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID),
			lastNode);

		m_taxiMapChangeNode = 0;
	}

	ResetHeartbeatCoords();
	m_heartbeatDisable = 0;
	m_speedChangeInProgress =false;
	m_lastMoveType = 0;
	
	/* send weather */
	sWeatherMgr.SendWeather(this);

	if( load_health > 0 )
	{
		if( load_health > m_uint32Values[UNIT_FIELD_MAXHEALTH] )
			SetUInt32Value( UNIT_FIELD_HEALTH, m_uint32Values[UNIT_FIELD_MAXHEALTH] );
		else
			SetUInt32Value(UNIT_FIELD_HEALTH, load_health);
	}

	if( load_mana > m_uint32Values[UNIT_FIELD_MAXPOWER1] )
		SetUInt32Value( UNIT_FIELD_POWER1, m_uint32Values[UNIT_FIELD_MAXPOWER1] );
	else
		SetUInt32Value(UNIT_FIELD_POWER1, load_mana);

	if( !GetSession()->HasGMPermissions() )
		GetItemInterface()->CheckAreaItems(); 

#ifdef ENABLE_COMPRESSED_MOVEMENT
	//sEventMgr.AddEvent(this, &Player::EventDumpCompressedMovement, EVENT_PLAYER_FLUSH_MOVEMENT, World::m_movementCompressInterval, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	MovementCompressor->AddPlayer(this);
#endif

	if( m_mapMgr && m_mapMgr->m_battleground != NULL && m_bg != m_mapMgr->m_battleground )
	{
		m_bg = m_mapMgr->m_battleground;
		m_bg->PortPlayer( this, true );
	}

	if( m_bg != NULL && m_mapMgr != NULL )
		m_bg->OnPlayerPushed( this );

	z_axisposition = 0.0f;
	m_changingMaps = false;
}

void Player::ResetHeartbeatCoords()
{
	m_lastHeartbeatPosition = m_position;
	m_lastHeartbeatV = m_runSpeed;
	if( m_isMoving )
		m_startMoveTime = m_lastMoveTime;
	else
		m_startMoveTime = 0;

	m_cheatEngineChances = 2;
	//_lastHeartbeatT = getMSTime();
}

void Player::RemoveFromWorld()
{
	if( m_CurrentCharm != NULL )
		UnPossess();

	if( m_uint32Values[UNIT_FIELD_CHARMEDBY] != 0 && IsInWorld() )
	{
		Player *charmer = m_mapMgr->GetPlayer(m_uint32Values[UNIT_FIELD_CHARMEDBY]);
		if( charmer != NULL )
			charmer->UnPossess();
	}

	m_resurrectHealth = 0;
	m_resurrectMana = 0;
	resurrector = 0;

	// just in case
	m_uint32Values[UNIT_FIELD_FLAGS] &= ~(UNIT_FLAG_SKINNABLE);

	if(raidgrouponlysent)
		event_RemoveEvents(EVENT_PLAYER_EJECT_FROM_INSTANCE);

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];

	if(m_bg)
	{
		m_bg->RemovePlayer(this, true);
		m_bg = NULL;
	}

	if(m_tempSummon)
	{
		m_tempSummon->RemoveFromWorld(false, true);
		if(m_tempSummon)
			m_tempSummon->SafeDelete();

		m_tempSummon = 0;
		SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	}

	// Cancel trade if it's active.
	Player * pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->ResetTradeVariables();

		ResetTradeVariables();
	}
	//clear buyback
	GetItemInterface()->EmptyBuyBack();
	
	for(uint32 x=0;x<4;x++)
	{
		if(m_TotemSlots[x])
			m_TotemSlots[x]->TotemExpire();
	}

	ResetHeartbeatCoords();
	ClearSplinePackets();

	if(m_Summon)
	{
		m_Summon->GetAIInterface()->SetPetOwner(0);
		m_Summon->Remove(true, true, false);
		if(m_Summon)
		{	
			m_Summon->ClearPetOwner();
			m_Summon=NULL;
		}
	}

	if(m_SummonedObject)
	{
		if(m_SummonedObject->GetInstanceID() != GetInstanceID())
		{
			sEventMgr.AddEvent(m_SummonedObject, &Object::Delete, EVENT_GAMEOBJECT_EXPIRE, 100, 1,0);
		}else
		{
			if(m_SummonedObject->GetTypeId() == TYPEID_PLAYER)
			{

			}
			else
			{
				if(m_SummonedObject->IsInWorld())
				{
					m_SummonedObject->RemoveFromWorld(true);
				}
				delete m_SummonedObject;
			}
		}
		m_SummonedObject = NULL;
	}

	if(IsInWorld())
	{
		RemoveItemsFromWorld();
		Unit::RemoveFromWorld(false);
	}

	sWorld.mInWorldPlayerCount--;
#ifdef ENABLE_COMPRESSED_MOVEMENT
	MovementCompressor->RemovePlayer(this);
	m_movementBufferLock.Acquire();
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
	//sEventMgr.RemoveEvents(this, EVENT_PLAYER_FLUSH_MOVEMENT);
	
#endif

	if(GetTaxiState())
		event_RemoveEvents( EVENT_PLAYER_TAXI_INTERPOLATE );

	if( m_CurrentTransporter && !m_lockTransportVariables )
	{
		m_CurrentTransporter->RemovePlayer(this);
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;
	}

	m_changingMaps = true;
}

// TODO: perhaps item should just have a list of mods, that will simplify code
void Player::_ApplyItemMods(Item* item, int8 slot, bool apply, bool justdrokedown /* = false */, bool skip_stat_apply /* = false  */)
{
	if (slot >= INVENTORY_SLOT_BAG_END)
		return;

	ASSERT( item );
	ItemPrototype* proto = item->GetProto();

	//fast check to skip mod applying if the item doesnt meat the requirements.
	if( item->GetUInt32Value( ITEM_FIELD_DURABILITY ) == 0 && item->GetUInt32Value( ITEM_FIELD_MAXDURABILITY ) && justdrokedown == false )
	{
		return;
	}

	//check for rnd prop
	item->ApplyRandomProperties( true );

	//Items Set check
	uint32 setid = proto->ItemSet;

	// These season pvp itemsets are interchangeable and each set group has the same
	// bonuses if you have a full set made up of parts from any of the 3 similar sets
	// you will get the highest sets bonus

	// TODO: make a config for server so they can configure which season is active season

	// * Gladiator's Battlegear
	if( setid == 701 || setid == 736 || setid == 567 )
		setid = 746;
	
	// * Gladiator's Dreadgear
	if( setid == 702 || setid == 734 || setid == 568 )
		setid = 734;

	// * Gladiator's Earthshaker
	if( setid == 703 || setid == 732 || setid == 578 )
		setid= 732;

	// * Gladiator's Felshroud
	if( setid == 704 || setid == 735 || setid == 615 )
		setid = 735;

	// * Gladiator's Investiture
	if( setid == 705 || setid == 728 || setid == 687 )
		setid = 728;

	// * Gladiator's Pursuit
	if( setid == 706 || setid == 723 || setid == 586 )
		setid = 723;

	// * Gladiator's Raiment
	if( setid == 707 || setid == 729 || setid == 581 )
		setid = 729;

	// * Gladiator's Redemption
	if( setid == 708 || setid == 725 || setid == 690 )
		setid = 725;

	// * Gladiator's Refuge
	if( setid == 709 || setid == 720 || setid == 685 )
		setid = 720;

	// * Gladiator's Regalia
	if( setid == 710 || setid == 724 || setid == 579 )
		setid = 724;

	// * Gladiator's Sanctuary
	if( setid == 711 || setid == 721 || setid == 584 )
		setid = 721;

	// * Gladiator's Thunderfist
	if( setid == 712 || setid == 733 || setid == 580 )
		setid = 733;

	// * Gladiator's Vestments
	if( setid == 713 || setid == 730 || setid == 577 )
		setid = 730;

	// * Gladiator's Vindication
	if( setid == 714 || setid == 726 || setid == 583 )
		setid = 726;

	// * Gladiator's Wartide
	if( setid == 715 || setid == 731 || setid == 686 )
		setid = 731;

	// * Gladiator's Wildhide
	if( setid == 716 || setid == 722 || setid == 585 )
		setid = 722;

	// Set
	if( setid != 0 )
	{
		ItemSetEntry* set = dbcItemSet.LookupEntry( setid );
		ASSERT( set );
		ItemSet* Set = NULL;
		std::list<ItemSet>::iterator i;
		for( i = m_itemsets.begin(); i != m_itemsets.end(); i++ )
		{
			if( i->setid == setid )
			{   
				Set = &(*i);
				break;
			}
		}

		if( apply )
		{
			if( Set == NULL ) 
			{
				Set = new ItemSet;
				memset( Set, 0, sizeof( ItemSet ) );
				Set->itemscount = 1;
				Set->setid = setid;
			}
			else
				Set->itemscount++;

			if( !set->RequiredSkillID || ( _GetSkillLineCurrent( set->RequiredSkillID, true ) >= set->RequiredSkillAmt ) )
			{
				for( uint32 x=0;x<8;x++)
				{
					if( Set->itemscount==set->itemscount[x])
					{//cast new spell
						SpellEntry *info = dbcSpell.LookupEntry( set->SpellID[x] );
						Spell * spell = new Spell( this, info, true, NULL );
						SpellCastTargets targets;
						targets.m_unitTarget = this->GetGUID();
						spell->prepare( &targets );
					}
				}
			}
			if( i == m_itemsets.end() )
			{
				m_itemsets.push_back( *Set );
				delete Set;
			}
		}
		else
		{
			if( Set )
			{
				for( uint32 x = 0; x < 8; x++ )
				if( Set->itemscount == set->itemscount[x] )
				{
					this->RemoveAura( set->SpellID[x], GetGUID() );
				}
	   
				if(!(--Set->itemscount))
				m_itemsets.erase(i);
			}
		}
	}
 
	// Armor
	if( proto->Armor )
	{
		if( apply )BaseResistance[0 ]+= proto->Armor;  
		else  BaseResistance[0] -= proto->Armor;
		CalcResistance( 0 );
	}

	// Resistances
	//TODO: FIXME: can there be negative resistances from items?
	if( proto->FireRes )
	{
		if( apply )
			FlatResistanceModifierPos[2] += proto->FireRes;
		else
			FlatResistanceModifierPos[2] -= proto->FireRes;
		CalcResistance( 2 );
	}
		
	if( proto->NatureRes )
	{
		if( apply )
			FlatResistanceModifierPos[3] += proto->NatureRes;
		else
			FlatResistanceModifierPos[3] -= proto->NatureRes;
		CalcResistance( 3 );
	}

	if( proto->FrostRes )
	{
		if( apply )
			FlatResistanceModifierPos[4] += proto->FrostRes;
		else
			FlatResistanceModifierPos[4] -= proto->FrostRes;
		CalcResistance( 4 );	
	}

	if( proto->ShadowRes )
	{
		if( apply )
			FlatResistanceModifierPos[5] += proto->ShadowRes;
		else
			FlatResistanceModifierPos[5] -= proto->ShadowRes;
		CalcResistance( 5 );	
	}
	 
	if( proto->ArcaneRes )
	{
		if( apply )
			FlatResistanceModifierPos[6] += proto->ArcaneRes;
		else
			FlatResistanceModifierPos[6] -= proto->ArcaneRes;
		CalcResistance( 6 );
	}
	
	// Stats
	for( int i = 0; i < 10; i++ )
	{
		int32 val = proto->Stats[i].Value;
		if( val == 0 )
			continue;
		ModifyBonuses( proto->Stats[i].Type, apply ? val : -val );
	}

	// Damage
	if( proto->Damage[0].Min )
	{
		if( proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_RANGEDRIGHT || proto->InventoryType == INVTYPE_THROWN )	
		{	
			BaseRangedDamage[0] += apply ? proto->Damage[0].Min : -proto->Damage[0].Min;
			BaseRangedDamage[1] += apply ? proto->Damage[0].Max : -proto->Damage[0].Max;
		}
		else
		{
			if( slot == EQUIPMENT_SLOT_OFFHAND )
			{
				BaseOffhandDamage[0] = apply ? proto->Damage[0].Min : 0;
				BaseOffhandDamage[1] = apply ? proto->Damage[0].Max : 0;
			}
			else
			{
				BaseDamage[0] = apply ? proto->Damage[0].Min : 1;
				BaseDamage[1] = apply ? proto->Damage[0].Max : 1;
			}
		}
	}

	// Misc
	if( apply )
	{
		// Apply all enchantment bonuses
		item->ApplyEnchantmentBonuses();

		for( int k = 0; k < 5; k++ )
		{
			// stupid fucked dbs
			if( item->GetProto()->Spells[k].Id == 0 )
				continue;

			if( item->GetProto()->Spells[k].Trigger == 1 )
			{
				SpellEntry* spells = dbcSpell.LookupEntry( item->GetProto()->Spells[k].Id );
				if( spells->RequiredShapeShift )
				{
					AddShapeShiftSpell( spells->Id );
					continue;
				}

				Spell *spell = new Spell( this, spells ,true, NULL );
				SpellCastTargets targets;
				targets.m_unitTarget = this->GetGUID();
				spell->castedItemId = item->GetEntry();
				spell->prepare( &targets );

			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				ProcTriggerSpell ts;
				ts.origId = 0;
				ts.spellId = item->GetProto()->Spells[k].Id;
				ts.procChance = 5;
				ts.caster = this->GetGUID();
				ts.procFlags = PROC_ON_MELEE_ATTACK;
				ts.deleted = false;
				m_procSpells.push_front( ts );			
			}
		}
	}
	else
	{
		// Remove all enchantment bonuses
		item->RemoveEnchantmentBonuses();
		for( int k = 0; k < 5; k++ )
		{
			if( item->GetProto()->Spells[k].Trigger == 1 )
			{
				SpellEntry* spells = dbcSpell.LookupEntry( item->GetProto()->Spells[k].Id );
				if( spells->RequiredShapeShift )
					RemoveShapeShiftSpell( spells->Id );
				else
					RemoveAura( item->GetProto()->Spells[k].Id ); 
			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				std::list<struct ProcTriggerSpell>::iterator i;
				// Debug: i changed this a bit the if was not indented to the for
				// so it just set last one to deleted looks like unintended behaviour
				// because you can just use end()-1 to remove last so i put the if
				// into the for
				for( i = m_procSpells.begin(); i != m_procSpells.end(); i++ )
				{
					if( (*i).spellId == item->GetProto()->Spells[k].Id && !(*i).deleted )
					{
						//m_procSpells.erase(i);
						i->deleted = true;
						break;
					}
				}
			}
		}
	}
	
	if( !skip_stat_apply )
		UpdateStats();
}


void Player::SetMovement(uint8 pType, uint32 flag)
{
	//WorldPacket data(13);
	uint8 buf[20];
	StackPacket data(SMSG_FORCE_MOVE_ROOT, buf, 20);

	switch(pType)
	{
	case MOVE_ROOT:
		{
			data.SetOpcode(SMSG_FORCE_MOVE_ROOT);
			data << GetNewGUID();
			data << flag;
			m_currentMovement = MOVE_ROOT;
		}break;
	case MOVE_UNROOT:
		{
			data.SetOpcode(SMSG_FORCE_MOVE_UNROOT);
			data << GetNewGUID();
			data << flag;
			m_currentMovement = MOVE_UNROOT;
		}break;
	case MOVE_WATER_WALK:
		{
			m_setwaterwalk=true;
			data.SetOpcode(SMSG_MOVE_WATER_WALK);
			data << GetNewGUID();
			data << flag;
		}break;
	case MOVE_LAND_WALK:
		{
			m_setwaterwalk=false;
			data.SetOpcode(SMSG_MOVE_LAND_WALK);
			data << GetNewGUID();
			data << flag;
		}break;
	default:break;
	}

	if(data.GetSize() > 0)
		SendMessageToSet(&data, true);
}

void Player::SetPlayerSpeed(uint8 SpeedType, float value)
{
	if( value < 0.5f )
		value = 0.5f;

	uint8 buf[200];
	StackPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, buf, 200);

	if( SpeedType != SWIMBACK )
	{
		data << GetNewGUID();
		data << m_speedChangeCounter++;
		if( SpeedType == RUN )
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << m_position.x;
		data << m_position.y;
		data << m_position.z;
		data << m_position.o;
		data << uint32(0);
		data << value;
	}
	
	switch(SpeedType)
	{
	case RUN:
		{
			if(value == m_lastRunSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			m_runSpeed = value;
			m_lastRunSpeed = value;
		}break;
	case RUNBACK:
		{
			if(value == m_lastRunBackSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
			m_lastRunBackSpeed = value;
		}break;
	case SWIM:
		{
			if(value == m_lastSwimSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
			m_lastSwimSpeed = value;
		}break;
	case SWIMBACK:
		{
			if(value == m_lastBackSwimSpeed)
				break;

			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			m_backSwimSpeed = value;
			m_lastBackSwimSpeed = value;
		}break;
	case FLY:
		{
			if(value == m_lastFlySpeed)
				return;

			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
			m_lastFlySpeed = value;
		}break;
	default:return;
	}
	
	SendMessageToSet(&data , true);

	// dont mess up on these
	m_speedChangeInProgress = true;
}

void Player::BuildPlayerRepop()
{
	SetUInt32Value( UNIT_FIELD_HEALTH, 1 );

	//8326 --for all races but ne,  9036 20584--ne
	SpellCastTargets tgt;
	tgt.m_unitTarget=this->GetGUID();
   
	if(getRace()==RACE_NIGHTELF)
	{
		SpellEntry *inf=dbcSpell.LookupEntry(20584);
		Spell*sp=new Spell(this,inf,true,NULL);
		sp->prepare(&tgt);
		inf=dbcSpell.LookupEntry(9036);
		sp=new Spell(this,inf,true,NULL);
		sp->prepare(&tgt);
	}
	else
	{
	
		SpellEntry *inf=dbcSpell.LookupEntry(8326);
		Spell*sp=new Spell(this,inf,true,NULL);
		sp->prepare(&tgt);
	}

	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);
	
	SetFlag(PLAYER_FLAGS, 0x10);

	SetMovement(MOVE_UNROOT, 1);
	SetMovement(MOVE_WATER_WALK, 1);
}

Corpse *Player::RepopRequestedPlayer()
{
	if( myCorpse != NULL )
	{
		GetSession()->SendNotification( NOTIFICATION_MESSAGE_NO_PERMISSION );
		return NULL;
	}

	if( m_CurrentTransporter != NULL )
	{
		m_CurrentTransporter->RemovePlayer( this );
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;

		ResurrectPlayer(NULL);
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		return NULL;
	}

	MapInfo * pMapinfo;

	sEventMgr.RemoveEvents( this, EVENT_PLAYER_FORECED_RESURECT ); //in case somebody resurrected us before this event happened

	// Set death state to corpse, that way players will lose visibility
	setDeathState( CORPSE );
	
	// Update visibility, that way people wont see running corpses :P
	UpdateVisibility();

	// If we're in battleground, remove the skinnable flag.. has bad effects heheh
	RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE );

	bool corpse = (m_bg != NULL) ? m_bg->CreateCorpse( this ) : true;
	Corpse *ret = NULL;

	if( corpse )
		ret = CreateCorpse();
	
	BuildPlayerRepop();

	pMapinfo = WorldMapInfoStorage.LookupEntry( GetMapId() );
	if( pMapinfo != NULL )
	{
		if( pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_PVP )
		{
			RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		}
		else
		{
			RepopAtGraveyard( pMapinfo->repopx, pMapinfo->repopy, pMapinfo->repopz, pMapinfo->repopmapid );
		}
	}
	else
	{
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
	}
	
	if( corpse )
	{
		/* Send Spirit Healer Location */
		WorldPacket data( SMSG_DEATH_RELEASE_LOC, 16 );
		data << m_mapId << m_position;
		m_session->SendPacket( &data );

		/* Corpse reclaim delay */
		WorldPacket data2( SMSG_CORPSE_RECLAIM_DELAY, 4 );
		data2 << (uint32)( CORPSE_RECLAIM_TIME_MS );
		GetSession()->SendPacket( &data2 );
	}

	if( myCorpse != NULL )
		myCorpse->ResetDeathClock();

	return ret;
}

void Player::ResurrectPlayer(Player *pResurrector)
{
	sEventMgr.RemoveEvents(this,EVENT_PLAYER_FORECED_RESURECT); //in case somebody resurected us before this event happened
	if( m_resurrectHealth )
		SetUInt32Value( UNIT_FIELD_HEALTH, (uint32)min( m_resurrectHealth, m_uint32Values[UNIT_FIELD_MAXHEALTH] ) );
	if( m_resurrectMana )
		SetUInt32Value( UNIT_FIELD_POWER1, (uint32)min( m_resurrectMana, m_uint32Values[UNIT_FIELD_MAXPOWER1] ) );

	m_resurrectHealth = m_resurrectMana = 0;

	SpawnCorpseBones();
	
	if(getRace()==RACE_NIGHTELF)
	{
		RemoveAura(20584);
		RemoveAura(9036);
	}else
		RemoveAura(8326);

	RemoveFlag(PLAYER_FLAGS, 0x10);
	setDeathState(ALIVE);
	UpdateVisibility();
	SetMovement(MOVE_LAND_WALK, 1);

	if(pResurrector != NULL && IsInWorld())
	{
		if( m_resurrectLoction.x == 0.0f && m_resurrectLoction.y == 0.0f && m_resurrectLoction.z == 0.0f )
		{
			SafeTeleport(pResurrector->GetMapId(),pResurrector->GetInstanceID(),pResurrector->GetPosition());
		}
		else
		{
			SafeTeleport(GetMapId(), GetInstanceID(), m_resurrectLoction);
			m_resurrectLoction.ChangeCoords(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		// update player counts in arenas
		if( m_bg != NULL && m_bg->IsArena() )
			static_cast<Arena*>(m_bg)->UpdatePlayerCounts();			
	}
}

void Player::KillPlayer()
{
	setDeathState(JUST_DIED);

	// Battleground stuff
	if(m_bg)
		m_bg->HookOnPlayerDeath(this);

	EventDeath();
	
	m_session->OutPacket(SMSG_CANCEL_COMBAT);
	m_session->OutPacket(SMSG_CANCEL_AUTO_REPEAT);

	SetMovement(MOVE_ROOT, 0);

	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED); //player death animation, also can be used with DYNAMIC_FLAGS <- huh???
	SetUInt32Value( UNIT_DYNAMIC_FLAGS, 0x00 );
	if(this->getClass() == WARRIOR) //rage resets on death
		SetUInt32Value(UNIT_FIELD_POWER2, 0);

	// combo points reset upon death
	NullComboPoints();

	sHookInterface.OnDeath(this);
}

Corpse *Player::CreateCorpse()
{
	Corpse *pCorpse;
	uint32 _uf, _pb, _pb2, _cfb1, _cfb2;

	objmgr.DelinkPlayerCorpses(this);
	pCorpse = objmgr.CreateCorpse();
	pCorpse->SetInstanceID(GetInstanceID());
	pCorpse->Create(this, GetMapId(), GetPositionX(),
		GetPositionY(), GetPositionZ(), GetOrientation());

	_uf = GetUInt32Value(UNIT_FIELD_BYTES_0);
	_pb = GetUInt32Value(PLAYER_BYTES);
	_pb2 = GetUInt32Value(PLAYER_BYTES_2);

	uint8 race	   = (uint8)(_uf);
	uint8 skin	   = (uint8)(_pb);
	uint8 face	   = (uint8)(_pb >> 8);
	uint8 hairstyle  = (uint8)(_pb >> 16);
	uint8 haircolor  = (uint8)(_pb >> 24);
	uint8 facialhair = (uint8)(_pb2);

	_cfb1 = ((0x00) | (race << 8) | (0x00 << 16) | (skin << 24));
	_cfb2 = ((face) | (hairstyle << 8) | (haircolor << 16) | (facialhair << 24));

	pCorpse->SetZoneId( GetZoneId() );
	pCorpse->SetUInt32Value( CORPSE_FIELD_BYTES_1, _cfb1 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_BYTES_2, _cfb2 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_FLAGS, 4 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_DISPLAY_ID, GetUInt32Value(UNIT_FIELD_DISPLAYID) );

	uint32 iDisplayID = 0;
	uint16 iIventoryType = 0;
	uint32 _cfi = 0;
	Item * pItem;
	for (int8 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		if(( pItem = GetItemInterface()->GetInventoryItem(i)))
		{
			iDisplayID = pItem->GetProto()->DisplayInfoID;
			iIventoryType = (uint16)pItem->GetProto()->InventoryType;

			_cfi =  (uint16(iDisplayID)) | (iIventoryType)<< 24;
			pCorpse->SetUInt32Value(CORPSE_FIELD_ITEM + i,_cfi);
		}
	}

	// are we going to bones straight away?
	if(m_insigniaTaken)
	{
		m_insigniaTaken = false;   // for next time
		pCorpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 5);
		pCorpse->SetUInt64Value(CORPSE_FIELD_OWNER, 0); // remove corpse owner association
		//remove item association
		for (int i = 0; i < EQUIPMENT_SLOT_END; i++)
		{
			if(pCorpse->GetUInt32Value(CORPSE_FIELD_ITEM + i))
				pCorpse->SetUInt32Value(CORPSE_FIELD_ITEM + i, 0);
		}
		pCorpse->SetCorpseState(CORPSE_STATE_BONES);
	}
	else
	{
		//save corpse in db for future use
		if(m_mapMgr && m_mapMgr->GetMapInfo()->type == INSTANCE_NULL)
			pCorpse->SaveToDB();

		myCorpse = pCorpse;

		// insignia stuff
		if( m_bg != NULL && m_bg->SupportsPlayerLoot() )
		{
			if( !m_insigniaTaken )
				pCorpse->SetFlag(CORPSE_FIELD_FLAGS, 60);
		}
	}

	// spawn
	if( m_mapMgr == NULL )
		pCorpse->AddToWorld();
	else
		pCorpse->PushToWorld(m_mapMgr);

	// add deletion event if bone corpse
	if( pCorpse->GetUInt64Value(CORPSE_FIELD_OWNER) == 0 )
		objmgr.CorpseAddEventDespawn(pCorpse);

	return pCorpse;
}

void Player::SpawnCorpseBones()
{
	Corpse *pCorpse;
	pCorpse = objmgr.GetCorpseByOwner(GetLowGUID());
	myCorpse = 0;
	if(pCorpse)
	{
		if (pCorpse->IsInWorld() && pCorpse->GetCorpseState() == CORPSE_STATE_BODY)
		{
			if(pCorpse->GetInstanceID() != GetInstanceID())
			{
				sEventMgr.AddEvent(pCorpse, &Corpse::SpawnBones, EVENT_CORPSE_SPAWN_BONES, 100, 1,0);
			}
			else
				pCorpse->SpawnBones();
		}
		else
		{
			//Cheater!
		}
	}
}

void Player::DeathDurabilityLoss(double percent)
{
	m_session->OutPacket(SMSG_DURABILITY_DAMAGE_DEATH);
	uint32 pDurability;
	uint32 pMaxDurability;
	int32 pNewDurability;
	Item * pItem;

	for (int8 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		if((pItem = GetItemInterface()->GetInventoryItem(i)))
		{
			pMaxDurability = pItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
			pDurability =  pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);
			if(pDurability)
			{
				pNewDurability = (uint32)(pMaxDurability*percent);
				pNewDurability = (pDurability - pNewDurability);
				if(pNewDurability < 0) 
					pNewDurability = 0;

				if(pNewDurability <= 0) 
				{ 
					ApplyItemMods(pItem, i, false, true);
				}

				pItem->SetUInt32Value(ITEM_FIELD_DURABILITY,(uint32)pNewDurability);
				pItem->m_isDirty = true;
			}
		}
	}
}

void Player::RepopAtGraveyard(float ox, float oy, float oz, uint32 mapid)
{   
	bool first = true;
	//float closestX = 0, closestY = 0, closestZ = 0, closestO = 0;
	StorageContainerIterator<GraveyardTeleport> * itr;

	LocationVector src(ox, oy, oz);
	LocationVector dest(0, 0, 0, 0);
	LocationVector temp;
	float closest_dist = 999999.0f;
	float dist;

	if(m_bg && m_bg->HookHandleRepop(this))
	{
		return;
	}
	else
	{
		uint32 areaid = sInstanceMgr.GetMap(mapid)->GetAreaID(ox,oy);
		AreaTable * at = dbcArea.LookupEntryForced(areaid);
		if(areaid != 0 && at != NULL)
		{
			uint32 mzone = ( at->ZoneId ? at->ZoneId : at->AreaId);

			itr = GraveyardStorage.MakeIterator();
			while(!itr->AtEnd())
			{
				GraveyardTeleport *pGrave = itr->Get();
				if((pGrave->MapId == mapid && pGrave->ZoneId == mzone && pGrave->FactionID == GetTeam() || pGrave->MapId == mapid && pGrave->ZoneId == mzone && pGrave->FactionID == 3)
					|| (pGrave->MapId == mapid && pGrave->AdjacentZoneId == mzone && pGrave->FactionID == GetTeam() || pGrave->MapId == mapid && pGrave->AdjacentZoneId == mzone && pGrave->FactionID == 3))
				{
					temp.ChangeCoords(pGrave->X, pGrave->Y, pGrave->Z);
					dist = src.DistanceSq(temp);
					if( first || dist < closest_dist )
					{
						first = false;
						closest_dist = dist;
						dest = temp;
					}
				}

				if(!itr->Inc())
					break;
			}
			itr->Destruct();
		}
		else
			dest.ChangeCoords(ox, oy, oz);
	}

	if(sHookInterface.OnRepop(this) && dest.x != 0 && dest.y != 0 && dest.z != 0)
	{
		SafeTeleport(mapid, 0, dest);
	}


//	//correct method as it works on official server, and does not require any damn sql
//	//no factions! no zones! no sqls! 1word: blizz-like
//	float closestX , closestY , closestZ ;
//	uint32 entries=sWorldSafeLocsStore.GetNumRows();
//	GraveyardEntry*g;
//	uint32 mymapid=mapid
//	float mx=ox,my=oy;
//	float last_distance=9e10;
//
//	for(uint32 x=0;x<entries;x++)
//	{
//		g=sWorldSafeLocsStore.LookupEntry(x);
//		if(g->mapid!=mymapid)continue;
//		float distance=(mx-g->x)*(mx-g->x)+(my-g->y)*(my-g->y);
//		if(distance<last_distance)
//		{
//			closestX=g->x;
//			closestY=g->y;
//			closestZ=g->z;
//			last_distance=distance;
//		}
//	
//	
//	}
//	if(last_distance<1e10)
//#endif

	
}

void Player::JoinedChannel(Channel *c)
{
	if( c != NULL )
		m_channels.insert(c->m_channelId);
}

void Player::LeftChannel(Channel *c)
{
	if( c != NULL )
		m_channels.erase(c->m_channelId);
}

void Player::CleanupChannels()
{
	set<uint32>::iterator i;
	Channel * c;
	uint32 cid;
	for(i = m_channels.begin(); i != m_channels.end();)
	{
		cid = *i;
		++i;
		
		c = channelmgr.GetChannel(cid);
		if( c != NULL )
			c->Part(this, false);
	}
}

void Player::SendInitialActions()
{
	m_session->OutPacket(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE, &mActions);
}

void Player::setAction(uint8 button, uint16 action, uint8 type, uint8 misc)
{
	assert(button < 120);
	mActions[button].Action = action;
	mActions[button].Type = type;
	mActions[button].Misc = misc;
}

//Groupcheck
bool Player::IsGroupMember(Player *plyr)
{
	if(m_playerInfo->m_Group != NULL)
		return m_playerInfo->m_Group->HasMember(plyr->m_playerInfo);

	return false;
}

int32 Player::GetOpenQuestSlot()
{
	for (uint32 i = 0; i < 25; ++i)
		if (m_questlog[i] == NULL)
			return i;

	return -1;
}

void Player::AddToFinishedQuests(uint32 quest_id)
{
	//maybe that shouldn't be an assert, but i'll leave it for now
	//ASSERT(m_finishedQuests.find(quest_id) == m_finishedQuests.end());
	//Removed due to crash
	//If it failed though, then he's probably cheating. 
	if (m_finishedQuests.find(quest_id) != m_finishedQuests.end())
		return;

	m_finishedQuests.insert(quest_id);
}

bool Player::HasFinishedQuest(uint32 quest_id)
{
	return (m_finishedQuests.find(quest_id) != m_finishedQuests.end());
}

//From Mangos Project
void Player::_LoadTutorials(QueryResult * result)
{	
	if(result)
	{
		 Field *fields = result->Fetch();
		 for (int iI=0; iI<8; iI++) 
			 m_Tutorials[iI] = fields[iI + 1].GetUInt32();
	}
	tutorialsDirty = false;
}

void Player::_SaveTutorials(QueryBuffer * buf)
{
	if(tutorialsDirty)
	{
		if(buf == NULL)
			CharacterDatabase.Execute("REPLACE INTO tutorials VALUES('%u','%u','%u','%u','%u','%u','%u','%u','%u')", GetLowGUID(), m_Tutorials[0], m_Tutorials[1], m_Tutorials[2], m_Tutorials[3], m_Tutorials[4], m_Tutorials[5], m_Tutorials[6], m_Tutorials[7]);
		else
			buf->AddQuery("REPLACE INTO tutorials VALUES('%u','%u','%u','%u','%u','%u','%u','%u','%u')", GetLowGUID(), m_Tutorials[0], m_Tutorials[1], m_Tutorials[2], m_Tutorials[3], m_Tutorials[4], m_Tutorials[5], m_Tutorials[6], m_Tutorials[7]);

		tutorialsDirty = false;
	}
}

uint32 Player::GetTutorialInt(uint32 intId )
{
	ASSERT( intId < 8 );
	return m_Tutorials[intId];
}

void Player::SetTutorialInt(uint32 intId, uint32 value)
{
	if(intId >= 8)
		return;

	ASSERT( (intId < 8) );
	m_Tutorials[intId] = value;
	tutorialsDirty = true;
}

//Player stats calculation for saving at lvl up, etc
/*void Player::CalcBaseStats()
{//static_cast< Player* >( this )->getClass() == HUNTER ||
	//TODO take into account base stats at create
	uint32 AP, RAP;
	//Save AttAck power
	if(getClass() == ROGUE || getClass() == HUNTER)
	{
		AP = GetBaseUInt32Value(UNIT_FIELD_STAT0) + GetBaseUInt32Value(UNIT_FIELD_STAT1);
		RAP = (GetBaseUInt32Value(UNIT_FIELD_STAT1) * 2);
		SetBaseUInt32Value(UNIT_FIELD_ATTACK_POWER, AP);
		SetBaseUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, RAP);
	}
	else
	{
		AP = (GetBaseUInt32Value(UNIT_FIELD_STAT0) * 2);
		RAP = (GetBaseUInt32Value(UNIT_FIELD_STAT1) * 2);
		SetBaseUInt32Value(UNIT_FIELD_ATTACK_POWER, AP);
		SetBaseUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, RAP);
	}

}*/

void Player::UpdateHit(int32 hit)
{
   /*std::list<Affect*>::iterator i;
	Affect::ModList::const_iterator j;
	Affect *aff;
	uint32 in = hit;
	for (i = GetAffectBegin(); i != GetAffectEnd(); i++)
	{
		aff = *i;
		for (j = aff->GetModList().begin();j != aff->GetModList().end(); j++)
		{
			Modifier mod = (*j);
			if ((mod.GetType() == SPELL_AURA_MOD_HIT_CHANCE))
			{
				SpellEntry *spellInfo = sSpellStore.LookupEntry(aff->GetSpellId());
				if (this->canCast(spellInfo))
					in += mod.GetAmount();
			}
		}
	}
	SetHitFromSpell(in);*/
}

void Player::UpdateChances()
{
	uint32 pClass = (uint32)getClass();
	uint32 pLevel = (getLevel() > 70) ? 70 : getLevel();

	float tmp = 0;
	float defence_contribution = 0;

	// defence contribution estimate
	defence_contribution = ( float( _GetSkillLineCurrent( SKILL_DEFENSE, true ) ) - ( float( pLevel ) * 5.0f ) ) * 0.04f;
	defence_contribution += CalcRating( PLAYER_RATING_MODIFIER_DEFENCE ) * 0.04f;

	// dodge
	tmp = baseDodge[pClass] + float( GetUInt32Value( UNIT_FIELD_STAT1 ) / dodgeRatio[69][pClass] );
	tmp += CalcRating( PLAYER_RATING_MODIFIER_DODGE ) + this->GetDodgeFromSpell();
	tmp += defence_contribution;
	if( tmp < 0.0f )tmp = 0.0f;

	SetFloatValue( PLAYER_DODGE_PERCENTAGE, min( tmp, 95.0f ) );

	// block
	Item* it = this->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( it != NULL && it->GetProto()->InventoryType == INVTYPE_SHIELD )
	{
		tmp = 5.0f + CalcRating( PLAYER_RATING_MODIFIER_BLOCK ) + GetBlockFromSpell();
		tmp += defence_contribution;
		if( tmp < 0.0f )tmp = 0.0f;
	}
	else
		tmp = 0.0f;
	
	SetFloatValue( PLAYER_BLOCK_PERCENTAGE, min( tmp, 95.0f ) );

	//parry
	tmp = 5.0f + CalcRating( PLAYER_RATING_MODIFIER_PARRY ) + GetParryFromSpell();
	tmp += defence_contribution;
	if( tmp < 0.0f )tmp = 0.0f;

	SetFloatValue( PLAYER_PARRY_PERCENTAGE, std::max( 0.0f, std::min( tmp, 95.0f ) ) ); //let us not use negative parry. Some spells decrease it

	//critical
	gtFloat* baseCrit = dbcMeleeCritBase.LookupEntry(pClass-1);
	gtFloat* CritPerAgi = dbcMeleeCrit.LookupEntry(pLevel - 1 + (pClass-1)*100);

	tmp = 100*(baseCrit->val + GetUInt32Value( UNIT_FIELD_STAT1 ) * CritPerAgi->val);

	//std::list<WeaponModifier>::iterator i = tocritchance.begin();
	map< uint32, WeaponModifier >::iterator itr = tocritchance.begin();

	Item* tItemMelee = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
	Item* tItemRanged = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );

	float melee_bonus = 0;
	float ranged_bonus = 0;

	//-1 = any weapon

	for(; itr != tocritchance.end(); ++itr )
	{
		if( itr->second.wclass == ( uint32 )-1 || ( tItemMelee != NULL && ( 1 << tItemMelee->GetProto()->SubClass & itr->second.subclass ) ) )
		{
			melee_bonus += itr->second.value;
		}
		if( itr->second.wclass == ( uint32 )-1 || ( tItemRanged != NULL && ( 1 << tItemRanged->GetProto()->SubClass & itr->second.subclass ) ) )
		{
			ranged_bonus += itr->second.value;
		}
	}

	float cr = tmp + CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT ) + melee_bonus;
	SetFloatValue( PLAYER_CRIT_PERCENTAGE, min( cr, 95.0f ) );

	float rcr = tmp + CalcRating( PLAYER_RATING_MODIFIER_RANGED_CRIT ) + ranged_bonus;
	SetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE, min( rcr, 95.0f ) );

	gtFloat* SpellCritBase  = dbcSpellCritBase.LookupEntry(pClass-1);
	gtFloat* SpellCritPerInt = dbcSpellCrit.LookupEntry(pLevel - 1 + (pClass-1)*100);

	spellcritperc = 100*(SpellCritBase->val + GetUInt32Value( UNIT_FIELD_STAT3 ) * SpellCritPerInt->val) +
		this->GetSpellCritFromSpell() +
		this->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT );
	UpdateChanceFields();
}

void Player::UpdateChanceFields()
{
	// Update spell crit values in fields
	for(uint32 i = 0; i < 7; ++i)
	{
		SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + i, SpellCritChanceSchool[i]+spellcritperc);
	}
}

void Player::UpdateAttackSpeed()
{
	uint32 speed=2000;
	Item *weap ;
	if(GetShapeShift()==FORM_CAT)//cat form
	{
		speed = 1000;
	}else if(GetShapeShift()==FORM_BEAR || GetShapeShift()==FORM_DIREBEAR)
	{
		speed = 2500;
	}
	else //regular
	if( !disarmed )
	{
		weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND) ;
		if( weap != NULL )
			speed = weap->GetProto()->Delay;
	}
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 
				     uint32(( speed * m_meleeattackspeedmod ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) ) / 100.0f ) ));
	
	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( weap != NULL && weap->GetProto()->Class == 2 )// 2 is a weapon
	{
		speed = weap->GetProto()->Delay;
		SetUInt32Value( UNIT_FIELD_BASEATTACKTIME_01, 
					    uint32(( speed * m_meleeattackspeedmod ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) ) / 100.0f ) ));
	}
	  
	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
	if( weap != NULL )
	{
		speed = weap->GetProto()->Delay;
		SetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME,
			uint32(( speed * m_rangedattackspeedmod ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_RANGED_HASTE ) ) / 100.0f ) ));
	}
}

void Player::UpdateStats()
{   
	UpdateAttackSpeed();

	// formulas from wowwiki

	int32 AP = 0;
	int32 RAP = 0;
	int32 hpdelta = 128;
	int32 manadelta = 128;

	uint32 str = GetUInt32Value(UNIT_FIELD_STAT0);
	uint32 agi = GetUInt32Value(UNIT_FIELD_STAT1);
	uint32 lev = getLevel();

	// Attack power
	uint32 cl = getClass();   
	switch (cl)
	{
	case DRUID:
		AP = str * 2 - 20;

		if( GetShapeShift() == FORM_CAT )
			AP += agi + lev * 2;

		if( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR )
			AP += lev * 3;

		break;

	case ROGUE:
		//AP = lev * 2 + str + agi - 20;
		//RAP = lev + agi * 2 - 20;
		//AP = str + agi - 20;
		AP = lev * 2 + str + agi - 20;
		RAP = lev + agi - 10;
		break;

	case HUNTER:
		//AP = lev* 2 + str + agi - 20;
		//RAP = (lev + agi)*2 - 20;
		AP = lev * 2 + str + agi - 20;
		RAP = lev * 2 + agi - 10;
		break;

	case SHAMAN:
		AP = (lev+str)*2 - 20;
		break;

	case PALADIN:
		//AP = lev * 3 + str * 2 - 20;
		//AP = str * 2 - 20;
		AP = lev * 3 + str * 2 - 20;
		break;

	case WARRIOR:
		//AP = lev * 3 + str * 2 - 20;
		//RAP = (lev+agi)*2 - 20;
		//AP = str * 2 - 20;
		AP = lev * 3 + str * 2 - 20;
		RAP = lev + agi - 10;
		break;

	default://mage,priest,warlock
		AP = str - 10;
	}

	/* modifiers */
	RAP += int32(float(float(m_rap_mod_pct) * float(float(m_uint32Values[UNIT_FIELD_STAT3]) / 100.0f)));

	if( RAP < 0 )RAP = 0;
	if( AP < 0 )AP = 0;

	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, AP );
	SetUInt32Value( UNIT_FIELD_RANGED_ATTACK_POWER, RAP ); 

	LevelInfo* lvlinfo = objmgr.GetLevelInfo( this->getRace(), this->getClass(), lev );

	if( lvlinfo != NULL )
	{
		hpdelta = lvlinfo->Stat[2] * 10;
		manadelta = lvlinfo->Stat[3] * 15;
	}

	lvlinfo = objmgr.GetLevelInfo( this->getRace(), this->getClass(), 1 );

	if( lvlinfo != NULL )
	{
		hpdelta -= lvlinfo->Stat[2] * 10;
		manadelta -= lvlinfo->Stat[3] * 15;
	}

	int32 hp = GetUInt32Value( UNIT_FIELD_BASE_HEALTH );

	int32 stat_bonus = GetUInt32Value( UNIT_FIELD_POSSTAT2 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT2 );
	if ( stat_bonus < 0 )
		stat_bonus = 0; //avoid of having negative health
	int32 bonus = stat_bonus * 10 + m_healthfromspell + m_healthfromitems;

	int32 res = hp + bonus + hpdelta;
	int32 oldmaxhp = GetUInt32Value( UNIT_FIELD_MAXHEALTH );

	if( res < hp ) res = hp;
	SetUInt32Value( UNIT_FIELD_MAXHEALTH, res );

	if( ( int32 )GetUInt32Value( UNIT_FIELD_HEALTH ) > res )
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	else if( ( cl == DRUID) && ( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR ) )
	{
		res = float2int32( ( float )GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * ( float )GetUInt32Value( UNIT_FIELD_HEALTH ) / float( oldmaxhp ) );
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	}

	if( cl != WARRIOR && cl != ROGUE )
	{
		// MP
		int32 mana = GetUInt32Value( UNIT_FIELD_BASE_MANA );

		stat_bonus = GetUInt32Value( UNIT_FIELD_POSSTAT3 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT3 );
		if ( stat_bonus < 0 )
			stat_bonus = 0; //avoid of having negative mana
		bonus = stat_bonus * 15 + m_manafromspell + m_manafromitems ;

		res = mana + bonus + manadelta;
		if( res < mana )res = mana;	
		SetUInt32Value(UNIT_FIELD_MAXPOWER1, res);

		if((int32)GetUInt32Value(UNIT_FIELD_POWER1)>res)
			SetUInt32Value(UNIT_FIELD_POWER1,res);

		//Manaregen
		const static float ClassMultiplier[12] = {0.0f,0.0f,0.2f,0.2f,0.0f,0.25f,0.0f,0.2f,0.25f,0.2f,0.0f,0.225f};
		const static float ClassFlatMod[12] = {0.0f,0.0f,15.0f,15.0f,0.0f,12.5f,0.0f,15.0f,12.5f,15.0f,0.0f,15.0f};
		uint32 Spirit = GetUInt32Value( UNIT_FIELD_STAT4 );
		float amt = (Spirit*ClassMultiplier[cl]+ClassFlatMod[cl])*PctPowerRegenModifier[POWER_TYPE_MANA]*0.5f;
		SetFloatValue(PLAYER_FIELD_MOD_MANA_REGEN,amt+m_ModInterrMRegen/5.0f);
		SetFloatValue(PLAYER_FIELD_MOD_MANA_REGEN_INTERRUPT,amt*m_ModInterrMRegenPCT/100.0f+m_ModInterrMRegen/5.0f);
	}

	/////////////////////RATINGS STUFF/////////////////
	float cast_speed = CalcRating( PLAYER_RATING_MODIFIER_SPELL_HASTE );
	if( cast_speed != SpellHasteRatingBonus )
	{
		ModFloatValue( UNIT_MOD_CAST_SPEED, ( SpellHasteRatingBonus - cast_speed ) / 100.0f);
		SpellHasteRatingBonus = cast_speed;
	}
	////////////////////RATINGS STUFF//////////////////////

	// Shield Block
	Item* shield = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
	if( shield != NULL && shield->GetProto()->InventoryType == INVTYPE_SHIELD )
	{
		float block_multiplier = ( 100.0f + float( m_modblockabsorbvalue ) ) / 100.0f;
		if( block_multiplier < 1.0f )block_multiplier = 1.0f;

		int32 blockable_damage = float2int32( float( shield->GetProto()->Block ) +( float( m_modblockvaluefromspells + GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) ) * block_multiplier ) + ( ( float( str ) / 20.0f ) - 1.0f ) );
		SetUInt32Value( PLAYER_SHIELD_BLOCK, blockable_damage );
	}
	else
	{
		SetUInt32Value( PLAYER_SHIELD_BLOCK, 0 );
	}

	UpdateChances();
	CalcDamage();
}

void Player::AddRestXP(uint32 amount)
{
	if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you're >= 60
		return;
	m_restAmount += amount;
	SetUInt32Value(PLAYER_REST_STATE_EXPERIENCE, (uint32)(m_restAmount*0.5));
	UpdateRestState();
}

uint32 Player::SubtractRestXP(uint32 amount)
{
	if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you're >= 70
		return 0;
	uint32 amt = amount;
	int32 tmp = m_restAmount - amount;
	int32 pos = m_restAmount - (amount*2);
	if(pos < 0) pos = 0;

	if(tmp < 0)
	{
		amt = m_restAmount;
		m_restAmount = 0;
	}
	else
		m_restAmount -= amount;

	
	SetUInt32Value(PLAYER_REST_STATE_EXPERIENCE, pos);
	UpdateRestState();
	return amt;
}

uint32 Player::CalculateRestXP(uint32 seconds)
{
	float rate = sWorld.getRate(RATE_RESTXP);
	float xp = 0;
	if(seconds < 60)
	{
		xp = 1 * rate;
	}
	else
	{
		xp = ((seconds / 60) * rate);
	}
	return uint32(xp);
}

void Player::EventPlayerRest()
{
	if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you're >= 70
	{
		EventMgr::getSingleton().RemoveEvents(this, EVENT_PLAYER_REST);
		return;
	}
	// Rest timer
	uint32 diff = (uint32)UNIXTIME - m_lastRestUpdate;
	m_lastRestUpdate = (uint32)UNIXTIME;
	uint32 RestXP = CalculateRestXP((uint32)diff);
	DEBUG_LOG("REST: Adding %d rest XP for %.0f seconds of rest time", RestXP, diff);
	AddRestXP(RestXP);
}

void Player::UpdateRestState()
{
	if(m_restAmount && GetUInt32Value(UNIT_FIELD_LEVEL) < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		m_restState = RESTSTATE_RESTED;
	else
		m_restState = RESTSTATE_NORMAL;

	// Update needle position
	SetUInt32Value(PLAYER_BYTES_2, ((GetUInt32Value(PLAYER_BYTES_2) & 0x00FFFFFF) | (m_restState << 24)));
}

void Player::ApplyPlayerRestState(bool apply)
{
	if(apply)
	{
		m_restState = RESTSTATE_RESTED;
		m_isResting = true;

		SetFlag(PLAYER_FLAGS, PLAYER_FLAG_RESTING);	//put zzz icon

		UpdateRestState();
		m_lastRestUpdate = (uint32)UNIXTIME;

		if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you're >= 70
			return;
		sEventMgr.AddEvent(this, &Player::EventPlayerRest, EVENT_PLAYER_REST, (uint32)60000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		m_isResting = false;
		RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_RESTING);	//remove zzz icon
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_REST);
		UpdateRestState();
	}
}

#define CORPSE_VIEW_DISTANCE 1600 // 40*40

bool Player::CanSee(Object* obj) // * Invisibility & Stealth Detection - Partha *
{
	if (obj == this)
	   return true;

	uint32 object_type = obj->GetTypeId();

	if(obj->IsPlayer() && TO_PLAYER(obj)->m_isGmInvisible)
	{
		if(bGMTagOn)
			return true;

		return false;
	}

	if(getDeathState() == CORPSE) // we are dead and we have released our spirit
	{
		if(object_type == TYPEID_PLAYER)
		{
			Player *pObj = static_cast< Player* >(obj);

			if(myCorpse && myCorpse->GetDistanceSq(obj) <= CORPSE_VIEW_DISTANCE)
				return !pObj->m_isGmInvisible; // we can see all players within range of our corpse except invisible GMs

			if(m_deathVision) // if we have arena death-vision we can see all players except invisible GMs
				return !pObj->m_isGmInvisible;

			return (pObj->getDeathState() == CORPSE); // we can only see players that are spirits
		}

		if(myCorpse)
		{
			if(myCorpse == obj) 
				return true;

			if(myCorpse->GetDistanceSq(obj) <= CORPSE_VIEW_DISTANCE)
				return true; // we can see everything within range of our corpse
		}

		if(m_deathVision) // if we have arena death-vision we can see everything
		{
			if(obj->IsPlayer() && static_cast<Player*>(obj)->IsStealth())
				return false;

			return true;
		}

		if(object_type == TYPEID_UNIT)
		{
			Unit *uObj = static_cast<Unit *>(obj);

			return uObj->IsSpiritHealer(); // we can't see any NPCs except spirit-healers
		}

		return false;
	}
	//------------------------------------------------------------------

	// mage invisibility - ugh.
	if( IsPlayer() && m_mageInvisibility )
	{
		if( object_type == TYPEID_PLAYER )
			return TO_PLAYER(obj)->m_mageInvisibility;

		if( object_type == TYPEID_UNIT )
			return false;

		return true;
	}

	switch(object_type) // we are alive or we haven't released our spirit yet
	{			
		case TYPEID_PLAYER:
			{
				Player *pObj = static_cast< Player* >(obj);

				if( pObj->m_mageInvisibility )
					return false;

				if(pObj->m_invisible) // Invisibility - Detection of Players
				{
					if(pObj->getDeathState() == CORPSE)
						return bGMTagOn; // only GM can see players that are spirits

					if(GetGroup() && pObj->GetGroup() == GetGroup() // can see invisible group members except when dueling them
							&& DuelingWith != pObj)
						return true;

					if(pObj->stalkedby == GetGUID()) // Hunter's Mark / MindVision is visible to the caster
						return true;

					if(m_invisDetect[INVIS_FLAG_NORMAL] < 1 // can't see invisible without proper detection
							|| pObj->m_isGmInvisible) // can't see invisible GM
						return bGMTagOn; // GM can see invisible players
				}

				if(pObj->IsStealth()) // Stealth Detection
				{
					if(GetGroup() && pObj->GetGroup() == GetGroup() // can see stealthed group members except when dueling them
							&& DuelingWith != pObj)
						return true;

					uint64 stalker = pObj->stalkedby;
					if(stalker)
					{
						Object * pStalker = GetMapMgr() ? GetMapMgr()->_GetObject(stalker) : NULL;
						if(pStalker == this)
							return true;

						if(pStalker && pStalker->IsPlayer())
						{
							if(GetGroup() && GetGroup()->HasMember(TO_PLAYER(pStalker)))
								return true;
						}
					}
					
					float base_range = 28.0f;
					if(getLevel() > pObj->getLevel())
					{
						uint32 level_diff = getLevel() - pObj->getLevel();
						base_range += (level_diff) * 2.5f;
					}

					uint32 bonus = GetStealthDetectBonus();
					base_range += (uint32)(bonus / 2.0f);

					if(pObj->isInBack(this))
						base_range /= 2.0f;

					float distance = GetDistance2dSq(pObj);
					if(distance < base_range)
						return true;
					else
						return false;
				}

				return true;
			}
		//------------------------------------------------------------------

		case TYPEID_UNIT:
			{	
				Unit *uObj = static_cast<Unit *>(obj);
					
				if(uObj->IsSpiritHealer()) // can't see spirit-healers when alive
					return false;

				if(uObj->m_invisible // Invisibility - Detection of Units
						&& m_invisDetect[uObj->m_invisFlag] < 1) // can't see invisible without proper detection
					return bGMTagOn; // GM can see invisible units

				return true;
			}
		//------------------------------------------------------------------

		case TYPEID_GAMEOBJECT:
			{
				GameObject *gObj = static_cast<GameObject *>(obj);

				if(gObj->invisible) // Invisibility - Detection of GameObjects
				{
					uint64 owner = gObj->GetUInt64Value(OBJECT_FIELD_CREATED_BY);

					if(GetGUID() == owner) // the owner of an object can always see it
						return true;

					if(GetGroup())
					{
						PlayerInfo * inf = objmgr.GetPlayerInfo((uint32)owner);
						if(inf && GetGroup()->HasMember(inf))
							return true;
					}

					if(m_invisDetect[gObj->invisibilityFlag] < 1) // can't see invisible without proper detection
						return bGMTagOn; // GM can see invisible objects
				}

				return true;
			}					
		//------------------------------------------------------------------

		default:
			return true;
	}
}

void Player::AddInRangeObject(Object* pObj)
{
	//Send taxi move if we're on a taxi
	if (m_CurrentTaxiPath && (pObj->GetTypeId() == TYPEID_PLAYER))
	{
		uint32 ntime = getMSTime();

		if (ntime > m_taxi_ride_time)
			m_CurrentTaxiPath->SendMoveForTime( this, static_cast< Player* >( pObj ), ntime - m_taxi_ride_time);
		/*else
			m_CurrentTaxiPath->SendMoveForTime( this, static_cast< Player* >( pObj ), m_taxi_ride_time - ntime);*/
	}

	Unit::AddInRangeObject(pObj);

	//if the object is a unit send a move packet if they have a destination
	if(pObj->GetTypeId() == TYPEID_UNIT)
	{
		//add an event to send move update have to send guid as pointer was causing a crash :(
		//sEventMgr.AddEvent( static_cast< Creature* >( pObj )->GetAIInterface(), &AIInterface::SendCurrentMove, this->GetGUID(), EVENT_UNIT_SENDMOVE, 200, 1);
		static_cast< Creature* >( pObj )->GetAIInterface()->SendCurrentMove(this);
	}
}

void Player::OnRemoveInRangeObject(Object* pObj)
{
	//if (/*!CanSee(pObj) && */IsVisible(pObj))
	//{
		//RemoveVisibleObject(pObj);
	//}
	if(m_tempSummon == pObj)
	{
		m_tempSummon->RemoveFromWorld(false, true);
		if(m_tempSummon)
			m_tempSummon->SafeDelete();

		m_tempSummon = 0;
		SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	}

	m_visibleObjects.erase(pObj);
	Unit::OnRemoveInRangeObject(pObj);

	if( pObj == m_CurrentCharm )
	{
		Unit * p = m_CurrentCharm;

		this->UnPossess();
		if(m_currentSpell)
			m_currentSpell->cancel();	   // cancel the spell
		m_CurrentCharm=NULL;

		if( p->m_temp_summon&&p->GetTypeId() == TYPEID_UNIT )
			static_cast< Creature* >( p )->SafeDelete();
	}
 
	if(pObj == m_Summon)
	{
		if(m_Summon->IsSummon())
		{
			m_Summon->Dismiss(true);
		}
		else
		{
			m_Summon->Remove(true, true, false);
		}
		if(m_Summon)
		{
			m_Summon->ClearPetOwner();
			m_Summon = 0;
		}
	}

	/* wehee loop unrolling */
/*	if(m_spellTypeTargets[0] == pObj)
		m_spellTypeTargets[0] = NULL;
	if(m_spellTypeTargets[1] == pObj)
		m_spellTypeTargets[1] = NULL;
	if(m_spellTypeTargets[2] == pObj)
		m_spellTypeTargets[2] = NULL;*/
	if(pObj->IsUnit())
	{
		for(uint32 x = 0; x < NUM_SPELL_TYPE_INDEX; ++x)
			if(m_spellIndexTypeTargets[x] == pObj->GetGUID())
				m_spellIndexTypeTargets[x] = 0;
	}

	if( pObj == DuelingWith )
	{
		//EndDuel(DUEL_WINNER_RETREAT);
		sEventMgr.AddEvent(this, &Player::EndDuel, (uint8)DUEL_WINNER_RETREAT, EVENT_PLAYER_DUEL_COUNTDOWN, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::ClearInRangeSet()
{
	m_visibleObjects.clear();
	Unit::ClearInRangeSet();
}

void Player::EventCannibalize(uint32 amount)
{
	uint32 amt = (GetUInt32Value(UNIT_FIELD_MAXHEALTH)*amount)/100;
	
	uint32 newHealth = GetUInt32Value(UNIT_FIELD_HEALTH) + amt;
	
	if(newHealth <= GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
	else
		SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	cannibalizeCount++;
	if(cannibalizeCount == 5)
		SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);

	WorldPacket data(SMSG_PERIODICAURALOG, 38);
	data << GetNewGUID();				   // caster guid
	data << GetNewGUID();				   // target guid
	data << (uint32)(20577);				// spellid
	data << (uint32)1;					  // unknown?? need resource?
	data << (uint32)FLAG_PERIODIC_HEAL;		// aura school
	data << amt;							// amount of done to target / heal / damage
	data << (uint32)0;					  // unknown in some sniff this was 0x0F
	SendMessageToSet(&data, true);
}

void Player::EventReduceDrunk(bool full)
{
	uint8 drunk = ((GetUInt32Value(PLAYER_BYTES_3) >> 24) & 0xFF);
	if(full) drunk = 0;
	else drunk -= 10;
	SetUInt32Value(PLAYER_BYTES_3, ((GetUInt32Value(PLAYER_BYTES_3) & 0xFFFF00FF) | (drunk << 8)));
	if(drunk == 0) sEventMgr.RemoveEvents(this, EVENT_PLAYER_REDUCEDRUNK);
}

void Player::LoadTaxiMask(const char* data)
{
	vector<string> tokens = StrSplit(data, " ");

	int index;
	vector<string>::iterator iter;

	for (iter = tokens.begin(), index = 0;
		(index < 8) && (iter != tokens.end()); ++iter, ++index)
	{
		m_taximask[index] = atol((*iter).c_str());
	}
}

bool Player::HasQuestForItem(uint32 itemid)
{
	Quest *qst;
	for( uint32 i = 0; i < 25; ++i )
	{
		if( m_questlog[i] != NULL )
		{
			qst = m_questlog[i]->GetQuest();
			if( !qst->count_required_item )
				continue;

			for( uint32 j = 0; j < 4 && j < 4; ++j )
				if( qst->required_item[j] == itemid && ( GetItemInterface()->GetItemCount( itemid ) < qst->required_itemcount[j] ) )
					return true;
		}
	}
	return false;
}

/*Loot type MUST be
1-corpse, go
2-skinning/herbalism/minning
3-Fishing
*/
void Player::SendLoot(uint64 guid,uint8 loot_type)
{	
	Group * m_Group = m_playerInfo->m_Group;
	if(!IsInWorld()) return;
	Object *lootObj;
	
	// handle items
	if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_ITEM)
	{
		lootObj = m_ItemInterface->GetItemByGUID(guid);
	}
	else
	{
		lootObj = m_mapMgr->_GetObject(guid);
	}

	if( lootObj == NULL )
		return;

	int8 loot_method = -1;
	if( lootObj->GetTypeId() == TYPEID_UNIT )
		loot_method = TO_CREATURE(lootObj)->m_lootMethod;

	if( loot_method < 0 )
	{
		// not set
		if( m_Group != NULL )
			loot_method = m_Group->GetMethod();
		else
			loot_method = PARTY_LOOT_FFA;
	}

	// add to looter set
	lootObj->m_loot.looters.insert(GetLowGUID());
		
	WorldPacket data, data2(28);
	data.SetOpcode (SMSG_LOOT_RESPONSE);
   
   
	m_lootGuid = guid;

			
	data << guid;
	data << loot_type;//loot_type;
	data << lootObj->m_loot.gold;
	data << (uint8) 0;//loot size reserve

	  
	std::vector<__LootItem>::iterator iter=lootObj->m_loot.items.begin();
	uint32 count=0;
	uint8 slottype = 0;
   
	for(uint32 x=0;iter!=lootObj->m_loot.items.end();iter++,x++)
	{ 
		if (iter->iItemsCount == 0)
			continue;

		LooterSet::iterator itr = iter->has_looted.find(GetLowGUID());
		if (iter->has_looted.end() != itr)
			continue;

		ItemPrototype* itemProto =iter->item.itemproto;
		if (!itemProto)		   
			continue;
        //quest items check. type 4/5
        //quest items that dont start quests.
        if((itemProto->Bonding == ITEM_BIND_QUEST) && !(itemProto->QuestId) && !HasQuestForItem(iter->item.itemproto->ItemId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && !(itemProto->QuestId) && !HasQuestForItem(iter->item.itemproto->ItemId))
            continue;

        //quest items that start quests need special check to avoid drops all the time.
        if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
            continue;

        if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;

        //check for starting item quests that need questlines.
        if((itemProto->QuestId && itemProto->Bonding != ITEM_BIND_QUEST && itemProto->Bonding != ITEM_BIND_QUEST2))
        {
            bool HasRequiredQuests = true;
            Quest * pQuest = QuestStorage.LookupEntry(itemProto->QuestId);
            if(pQuest)
            {
                //check if its a questline.
                for(uint32 i = 0; i < pQuest->count_requiredquests; i++)
                {
                    if(pQuest->required_quests[i])
                    {
                        if(!HasFinishedQuest(pQuest->required_quests[i]) || GetQuestLogForEntry(pQuest->required_quests[i]))
                        {
                            HasRequiredQuests = false;
                            break;
                        }
                    }
                }
                if(!HasRequiredQuests)
                    continue;
            }
        } 


		slottype = 0;
		if(m_Group != NULL && loot_type < 2)
		{
			switch(loot_method)
			{
			case PARTY_LOOT_MASTER:
				slottype = 2;
				break;
			case PARTY_LOOT_GROUP:
			case PARTY_LOOT_RR:
			case PARTY_LOOT_NBG:
				slottype = 1;
				break;
			default:
				slottype = 0;
				break;
			}
			// only quality items are distributed
			if(itemProto->Quality < m_Group->GetThreshold())
			{
				slottype = 0;
			}

			/* if all people passed anyone can loot it? :P */
			if(iter->passed)
				slottype = 0;					// All players passed on the loot

			//if it is ffa loot and not an masterlooter
			if(iter->ffa_loot)
				slottype = 0;
		}

		data << uint8(x); 
		data << uint32(itemProto->ItemId);
		data << uint32(iter->iItemsCount);//nr of items of this type
		data << uint32(iter->item.displayid); 
		//data << uint32(iter->iRandomSuffix ? iter->iRandomSuffix->id : 0);
		//data << uint32(iter->iRandomProperty ? iter->iRandomProperty->ID : 0);
		if(iter->iRandomSuffix)
		{
			data << Item::GenerateRandomSuffixFactor(itemProto);
			data << uint32(-int32(iter->iRandomSuffix->id));
		}
		else if(iter->iRandomProperty)
		{
			data << uint32(0);
			data << uint32(iter->iRandomProperty->ID);
		}
		else
		{
			data << uint32(0);
			data << uint32(0);
		}

		data << slottype;   // "still being rolled for" flag
		
		if(slottype == 1)
		{
			if(iter->roll == NULL && !iter->passed)
			{
				int32 ipid = 0;
				uint32 factor=0;
				if(iter->iRandomProperty)
					ipid=iter->iRandomProperty->ID;
				else if(iter->iRandomSuffix)
				{
					ipid = -int32(iter->iRandomSuffix->id);
					factor=Item::GenerateRandomSuffixFactor(iter->item.itemproto);
				}

				if(iter->item.itemproto)
				{
					iter->roll = new LootRoll(60000, (m_Group != NULL ? m_Group->MemberCount() : 1),  guid, x, iter->item.itemproto->ItemId, factor, uint32(ipid), GetMapMgr());
					
					data2.Initialize(SMSG_LOOT_START_ROLL);
					data2 << guid;
					data2 << x;
					data2 << uint32(iter->item.itemproto->ItemId);
					data2 << uint32(factor);
					if(iter->iRandomProperty)
						data2 << uint32(iter->iRandomProperty->ID);
					else if(iter->iRandomSuffix)
						data2 << uint32(ipid);
					else
						data2 << uint32(0);

					data2 << uint32(60000); // countdown
				}

				Group * pGroup = m_playerInfo->m_Group;
				if(pGroup)
				{
					pGroup->Lock();
					for(uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i)
					{
						for(GroupMembersSet::iterator itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
						{
							if((*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetItemInterface()->CanReceiveItem(itemProto, iter->iItemsCount, NULL) == 0)
							{
								if( (*itr)->m_loggedInPlayer->m_passOnLoot )
									iter->roll->PlayerRolled( (*itr)->m_loggedInPlayer, 3 );		// passed
								else
									(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&data2);
							}
						}
					}
					pGroup->Unlock();
				}
				else
				{
					GetSession()->SendPacket(&data2);
				}
			}			
		}
		count++;
	}
	data.wpos (13);
	data << (uint8)count;

	GetSession ()->SendPacket(&data);
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
}

void Player::EventAllowTiggerPort(bool enable)
{
	m_AllowAreaTriggerPort = enable;
}

uint32 Player::CalcTalentResetCost(uint32 resetnum)
{
	
	if(resetnum ==0 ) 
		return  10000;
	else
	{
		if(resetnum>10)
		return  500000;
		else return resetnum*50000;
	}
}

void Player::SendTalentResetConfirm()
{
	WorldPacket data(MSG_TALENT_WIPE_CONFIRM, 12);
	data << GetGUID();
	data << CalcTalentResetCost(GetTalentResetTimes());
	GetSession()->SendPacket(&data);
}
void Player::SendPetUntrainConfirm()
{
	Pet* pPet = GetSummon();
	if( pPet == NULL )
		return;
	WorldPacket data( SMSG_PET_UNLEARN_CONFIRM, 12 );
	data << pPet->GetGUID();
	data << pPet->GetUntrainCost();
	GetSession()->SendPacket( &data );
}

int32 Player::CanShootRangedWeapon( uint32 spellid, Unit* target, bool autoshot )
{
	uint8 fail = 0;

	SpellEntry* spellinfo = dbcSpell.LookupEntry( spellid );

	if( spellinfo == NULL )
		return -1;
	//sLog.outString( "Canshootwithrangedweapon!?!? spell: [%u] %s" , spellinfo->Id , spellinfo->Name );

	// Check ammo
	Item* itm = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
	if( itm == NULL )
		fail = SPELL_FAILED_NO_AMMO;

	// Player has clicked off target. Fail spell.
	if( m_curSelection != m_AutoShotTarget )
		fail = SPELL_FAILED_INTERRUPTED;

	if( target->isDead() )
		fail = SPELL_FAILED_TARGETS_DEAD;

	if( GetCurrentSpell() )
		return -1;
   
	if( fail > 0 )
		return -1;	

	// Supalosa - The hunter ability Auto Shot is using Shoot range, which is 5 yards shorter.
	// So we'll use 114, which is the correct 35 yard range used by the other Hunter abilities (arcane shot, concussive shot...)
	uint32 rIndex = autoshot ? 114 : spellinfo->rangeIndex;
	SpellRange* range = dbcSpellRange.LookupEntry( rIndex );
	float minrange = GetMinRange( range );
	float dist = CalcDistance( this, target );
	float maxr = GetMaxRange( range ) + 2.52f;

	if( spellinfo->SpellGroupType )
	{
		SM_FFValue( this->SM_FRange, &maxr, spellinfo->SpellGroupType );
		SM_PFValue( this->SM_PRange, &maxr, spellinfo->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		float spell_pct_modifers=0;
		SM_FFValue(this->SM_FRange,&spell_flat_modifers,spellinfo->SpellGroupType);
		SM_FFValue(this->SM_PRange,&spell_pct_modifers,spellinfo->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!spell range bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u\n",spell_flat_modifers,spell_pct_modifers,maxr,spellinfo->SpellGroupType);
#endif
	}

	//float bonusRange = 0;
	// another hackfix: bonus range from hunter talent hawk eye: +2/4/6 yard range to ranged weapons
	//if(autoshot)
	//SM_FFValue( SM_FRange, &bonusRange, dbcSpell.LookupEntry( 75 )->SpellGroupType ); // HORRIBLE hackfixes :P
	// Partha: +2.52yds to max range, this matches the range the client is calculating.
	// see extra/supalosa_range_research.txt for more info
	//bonusRange = 2.52f;
	//sLog.outString( "Bonus range = %f" , bonusRange );

	// Check for close
	if( spellid != SPELL_RANGED_WAND )//no min limit for wands
		if( minrange > dist )
			fail = SPELL_FAILED_TOO_CLOSE;
	
	if( dist > maxr )
	{
		//	sLog.outString( "Auto shot failed: out of range (Maxr: %f, Dist: %f)" , maxr , dist );
		fail = SPELL_FAILED_OUT_OF_RANGE;
	}

	if( spellid == SPELL_RANGED_THROW )
	{
		if( itm != NULL ) // no need for this
			if( itm->GetProto() )
				if( GetItemInterface()->GetItemCount( itm->GetProto()->ItemId ) == 0 )
					fail = SPELL_FAILED_NO_AMMO;
	} 
/*  else
	{
		if(GetUInt32Value(PLAYER_AMMO_ID))//for wand
			if(this->GetItemInterface()->GetItemCount(GetUInt32Value(PLAYER_AMMO_ID)) == 0)
				fail = SPELL_FAILED_NO_AMMO;
	}
*/

#ifdef COLLISION
	if( !CollideInterface.CheckLOS(m_mapId, GetPositionX(), GetPositionY(), GetPositionZ(), 
		target->GetPositionX(), target->GetPositionY(), target->GetPositionZ()) )
	{
		fail = SPELL_FAILED_LINE_OF_SIGHT;
	}
#endif
	if( fail > 0 )// && fail != SPELL_FAILED_OUT_OF_RANGE)
	{
		//SendCastResult( autoshot ? 75 : spellid, fail, 0, 0 );
		packetSMSG_CASTRESULT cr;
		cr.SpellId = autoshot ? 75 : spellid;
		cr.ErrorMessage = fail;
		cr.MultiCast = 0;
		m_session->OutPacket( SMSG_CAST_FAILED, sizeof(packetSMSG_CASTRESULT), &cr );
		if( fail != SPELL_FAILED_OUT_OF_RANGE )
		{
			uint32 spellid2 = autoshot ? 75 : spellid;
			m_session->OutPacket( SMSG_CANCEL_AUTO_REPEAT, 4, &spellid2 );
		}
		//sLog.outString( "Result for CanShootWIthRangedWeapon: %u" , fail );
		//DEBUG_LOG( "Can't shoot with ranged weapon: %u (Timer: %u)" , fail , m_AutoShotAttackTimer );
		return fail;
	}

	return 0;
}

void Player::EventRepeatSpell()
{
	if( !m_curSelection )
		return;

	if( m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP | UNIT_STATE_STUN | UNIT_STATE_CONFUSE ) || IsStunned() || IsFeared() )
		return;
	
	Unit* target = GetMapMgr()->GetUnit( m_curSelection );
	if( target == NULL )
	{
		m_AutoShotAttackTimer = 0; //avoid flooding client with error mesages
		m_onAutoShot = false;
		//DEBUG_LOG( "Can't cast Autoshot: Target changed! (Timer: %u)" , m_AutoShotAttackTimer );
		return;
	}

	m_AutoShotDuration = m_uint32Values[UNIT_FIELD_RANGEDATTACKTIME];

	if( m_isMoving )
	{
		//DEBUG_LOG( "HUNTER AUTOSHOT 2) %i, %i", m_AutoShotAttackTimer, m_AutoShotDuration );
		//m_AutoShotAttackTimer = m_AutoShotDuration;//avoid flooding client with error mesages
		//DEBUG_LOG( "Can't cast Autoshot: You're moving! (Timer: %u)" , m_AutoShotAttackTimer );
		m_AutoShotAttackTimer = 100; // shoot when we can
		return;
	}

	int32 f = this->CanShootRangedWeapon( m_AutoShotSpell->Id, target, true );

	if( f != 0 )
	{
		if( f != SPELL_FAILED_OUT_OF_RANGE )
		{
			m_AutoShotAttackTimer = 0; 
			m_onAutoShot=false;
		}
		else
		{
			m_AutoShotAttackTimer = m_AutoShotDuration;//avoid flooding client with error mesages
		}
		return;
	}
	else
	{		
		m_AutoShotAttackTimer = m_AutoShotDuration;
	
		Spell* sp = new Spell( this, m_AutoShotSpell, true, NULL );
		SpellCastTargets tgt;
		tgt.m_unitTarget = m_curSelection;
		tgt.m_targetMask = TARGET_FLAG_UNIT;
		sp->prepare( &tgt );
	}
}

void Player::removeSpellByHashName(uint32 hash)
{
	SpellSet::iterator it,iter;
	
	for(iter= mSpells.begin();iter != mSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
		{
			if(info->spell_list.find(e->Id) != info->spell_list.end())
				continue;

			RemoveAura(SpellID,GetGUID());
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);		
			mSpells.erase(it);
		}
	}

	for(iter= mDeletedSpells.begin();iter != mDeletedSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
		{
			if(info->spell_list.find(e->Id) != info->spell_list.end())
				continue;

			RemoveAura(SpellID,GetGUID());
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);		
			mDeletedSpells.erase(it);
		}
	}
}

bool Player::removeSpell(uint32 SpellID, bool MoveToDeleted, bool SupercededSpell, uint32 SupercededSpellID)
{
	SpellSet::iterator iter = mSpells.find(SpellID);
	if(iter != mSpells.end())
	{
		mSpells.erase(iter);
		RemoveAura(SpellID,GetGUID());
	}
	else
	{
		return false;
	}

	if(MoveToDeleted)
		mDeletedSpells.insert(SpellID);

	if(!IsInWorld())
		return true;

	if(SupercededSpell)
	{
		WorldPacket data(SMSG_SUPERCEDED_SPELL, 8);
		data << SpellID << SupercededSpellID;
		m_session->SendPacket(&data);
	}
	else
		m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);		

	return true;
}

void Player::EventTimedQuestExpire(Quest *qst, QuestLogEntry *qle, uint32 log_slot)
{
	WorldPacket fail;
	sQuestMgr.BuildQuestFailed(&fail, qst->id);
	GetSession()->SendPacket(&fail);
	CALL_QUESTSCRIPT_EVENT(qle, OnQuestCancel)(this);
	qle->Finish();
}

void Player::SendInitialLogonPackets()
{
	// Initial Packets... they seem to be re-sent on port.
	m_session->OutPacket(SMSG_SET_REST_START, 4, &m_timeLogoff);

    WorldPacket data(SMSG_BINDPOINTUPDATE, 32);
    data << m_bind_pos_x;
    data << m_bind_pos_y;
    data << m_bind_pos_z;
    data << m_bind_mapid;
    data << m_bind_zoneid;
    GetSession()->SendPacket( &data );

	//Proficiencies
    //SendSetProficiency(4,armor_proficiency);
    //SendSetProficiency(2,weapon_proficiency);
	packetSMSG_SET_PROFICICENCY pr;
	pr.ItemClass = 4;
	pr.Profinciency = armor_proficiency;
	m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof(packetSMSG_SET_PROFICICENCY), &pr );
	pr.ItemClass = 2;
	pr.Profinciency = weapon_proficiency;
	m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof(packetSMSG_SET_PROFICICENCY), &pr );
  
	//Tutorial Flags
	data.Initialize( SMSG_TUTORIAL_FLAGS );
	for (int i = 0; i < 8; i++)
		data << uint32( m_Tutorials[i] );
	GetSession()->SendPacket(&data);

	//Initial Spells
	smsg_InitialSpells();

	//Initial Actions
	SendInitialActions();

	//Factions
	smsg_InitialFactions();


    /* Some minor documentation about the time field
    // MOVE THIS DOCUMENATION TO THE WIKI
    
    minute's = 0x0000003F                  00000000000000000000000000111111
    hour's   = 0x000007C0                  00000000000000000000011111000000
    weekdays = 0x00003800                  00000000000000000011100000000000
    days     = 0x000FC000                  00000000000011111100000000000000
    months   = 0x00F00000                  00000000111100000000000000000000
    years    = 0x1F000000                  00011111000000000000000000000000
    unk	     = 0xE0000000                  11100000000000000000000000000000
    */

	data.Initialize(SMSG_LOGIN_SETTIMESPEED);
	time_t minutes = sWorld.GetGameTime( ) / 60;
	time_t hours = minutes / 60; minutes %= 60;
    time_t gameTime = 0;

    // TODO: Add stuff to handle these variable's

	uint32 DayOfTheWeek = -1;		//	(0b111 = (any) day, 0 = Monday ect)
    uint32 DayOfTheMonth = 20-1;	//	Day - 1 (0 is actual 1) its now the 20e here. TODO: replace this one with the proper date
    uint32 CurrentMonth = 9-1;		//	Month - 1 (0 is actual 1) same as above. TODO: replace it with the proper code
    uint32 CurrentYear = 7;			//	2000 + this number results in a correct value for this crap. TODO: replace this with the propper code

   #define MINUTE_BITMASK      0x0000003F
    #define HOUR_BITMASK        0x000007C0
    #define WEEKDAY_BITMASK     0x00003800
    #define DAY_BITMASK         0x000FC000
    #define MONTH_BITMASK       0x00F00000
    #define YEAR_BITMASK        0x1F000000
    #define UNK_BITMASK         0xE0000000

    #define MINUTE_SHIFTMASK    0
    #define HOUR_SHIFTMASK      6
    #define WEEKDAY_SHIFTMASK   11
    #define DAY_SHIFTMASK       14
    #define MONTH_SHIFTMASK     20
    #define YEAR_SHIFTMASK      24
    #define UNK_SHIFTMASK       29

    gameTime = ((minutes << MINUTE_SHIFTMASK) & MINUTE_BITMASK);
    gameTime|= ((hours << HOUR_SHIFTMASK) & HOUR_BITMASK);
    gameTime|= ((DayOfTheWeek << WEEKDAY_SHIFTMASK) & WEEKDAY_BITMASK);
    gameTime|= ((DayOfTheMonth << DAY_SHIFTMASK) & DAY_BITMASK);
    gameTime|= ((CurrentMonth << MONTH_SHIFTMASK) & MONTH_BITMASK);
    gameTime|= ((CurrentYear << YEAR_SHIFTMASK) & YEAR_BITMASK);

    data << (uint32)gameTime;
	data << (float)0.0166666669777748f;  // Normal Game Speed
	GetSession()->SendPacket( &data );

	DEBUG_LOG("WORLD: Sent initial logon packets for %s.", GetName());
}

void Player::Reset_Spells()
{
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	ASSERT(info);

	std::list<uint32> spelllist;
	
	for(SpellSet::iterator itr = mSpells.begin(); itr!=mSpells.end(); itr++)
	{
		spelllist.push_back((*itr));
	}

	for(std::list<uint32>::iterator itr = spelllist.begin(); itr!=spelllist.end(); itr++)
	{
		removeSpell((*itr), false, false, 0);
	}

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		if(*sp)
		{
			addSpell(*sp);
		}
	}
}

void Player::Reset_Talents()
{
	unsigned int numRows = dbcTalent.GetNumRows();
	for (unsigned int i = 0; i < numRows; i++)		  // Loop through all talents.
	{
		TalentEntry *tmpTalent = dbcTalent.LookupRow(i);
		if(!tmpTalent)
			continue; //should not ocur
		//this is a normal talent (i hope )
		for (int j = 0; j < 5; j++)
		{
			if (tmpTalent->RankID[j] != 0)
			{
				SpellEntry *spellInfo;
				spellInfo = dbcSpell.LookupEntry( tmpTalent->RankID[j] );
				if(spellInfo)
				{
					if(spellInfo->NameHash == SPELL_HASH_DUAL_WIELD && getClass() != SHAMAN )
						continue;

					for(int k=0;k<3;k++)
						if(spellInfo->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
						{
							//removeSpell(spellInfo->EffectTriggerSpell[k], false, 0, 0);
							//remove higher ranks of this spell too (like earth shield lvl 1 is talent and the rest is thought from trainer) 
							SpellEntry *spellInfo2;
							spellInfo2 = dbcSpell.LookupEntry( spellInfo->EffectTriggerSpell[k] );
							if(spellInfo2)
								removeSpellByHashName(spellInfo2->NameHash);
						}
					//remove them all in 1 shot
					removeSpellByHashName(spellInfo->NameHash);
				}
			}
			else
				break;
		}
	}
	uint32 l=getLevel();
	if(l>9)
	{
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, l - 9); 
	}
	else
	{
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, 0); 
	}
	m_cheatDeathRank = 0;
}

void Player::Reset_ToLevel1()
{
	RemoveAllAuras();
	// clear aura fields
	for(int i=UNIT_FIELD_AURA;i<UNIT_FIELD_AURASTATE;++i)
	{
		SetUInt32Value(i, 0);
	}
	SetUInt32Value(UNIT_FIELD_LEVEL, 1);
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	ASSERT(info);

	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // this gets devided by 10
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );
	SetUInt32Value(PLAYER_CHARACTER_POINTS1,0);
	SetUInt32Value(PLAYER_CHARACTER_POINTS2,2);
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

}

void Player::CalcResistance(uint32 type)
{
	int32 res;
	int32 pos;
	int32 neg;
	ASSERT(type < 7);
	pos=(BaseResistance[type]*BaseResistanceModPctPos[type])/100;
	neg=(BaseResistance[type]*BaseResistanceModPctNeg[type])/100;

	pos+=FlatResistanceModifierPos[type];
	neg+=FlatResistanceModifierNeg[type];
	res=BaseResistance[type]+pos-neg;
	if(type==0)res+=m_uint32Values[UNIT_FIELD_STAT1]*2;//fix armor from agi
	if(res<0)res=0;
	pos+=(res*ResistanceModPctPos[type])/100;
	neg+=(res*ResistanceModPctNeg[type])/100;
	res=pos-neg+BaseResistance[type];
	if(type==0)res+=m_uint32Values[UNIT_FIELD_STAT1]*2;//fix armor from agi

	if( res < 0 )
		res = 1;

	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE+type,pos);
	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE+type,neg);
	SetUInt32Value(UNIT_FIELD_RESISTANCES+type,res>0?res:0);

}


void Player::UpdateNearbyGameObjects()
{
	for (Object::InRangeSet::iterator itr = GetInRangeSetBegin(); itr != GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			bool activate_quest_object = false;
			GameObject *go = ((GameObject*)*itr);
			GameObjectInfo *info;

			info = go->GetInfo();
			if( info->InvolvedQuestIds != NULL )
			{
				uint32 v = 0;
				for(; v < info->InvolvedQuestCount; ++v)
				{
					if( GetQuestLogForEntry(info->InvolvedQuestIds[v]) != NULL )
					{
						go->BuildFieldUpdatePacket(this, GAMEOBJECT_DYN_FLAGS, GO_DYNFLAG_QUEST);
						go->BuildFieldUpdatePacket(this, GAMEOBJECT_STATE, 1);
						go->BuildFieldUpdatePacket(this, GAMEOBJECT_FLAGS, 0);
						break;
					}
				}

				if( v == info->InvolvedQuestCount )
				{
					// not on the quest
					go->BuildFieldUpdatePacket(this, GAMEOBJECT_DYN_FLAGS, 0);
					go->BuildFieldUpdatePacket(this, GAMEOBJECT_STATE, 0);
					go->BuildFieldUpdatePacket(this, GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
				}
			}
		}
	}
}


void Player::EventTaxiInterpolate()
{
	if(!m_CurrentTaxiPath || m_mapMgr==NULL) return;

	float x,y,z;
	uint32 ntime = getMSTime();

	if (ntime > m_taxi_ride_time)
		m_CurrentTaxiPath->SetPosForTime(x, y, z, ntime - m_taxi_ride_time, &lastNode, m_mapId);
	/*else
		m_CurrentTaxiPath->SetPosForTime(x, y, z, m_taxi_ride_time - ntime, &lastNode);*/

	if(x < _minX || x > _maxX || y < _minY || y > _maxX)
		return;

	SetPosition(x,y,z,0);
}

void Player::TaxiStart(TaxiPath *path, uint32 modelid, uint32 start_node)
{
	int32 mapchangeid = -1;
	float mapchangex;
	float mapchangey;
	float mapchangez;
	uint32 cn = m_taxiMapChangeNode;

	m_taxiMapChangeNode = 0;

	if(this->m_MountSpellId)
		RemoveAura(m_MountSpellId);
	//also remove morph spells
	if(GetUInt32Value(UNIT_FIELD_DISPLAYID)!=GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
	{
		RemoveAllAuraType(SPELL_AURA_TRANSFORM);
		RemoveAllAuraType(SPELL_AURA_MOD_SHAPESHIFT);
	}
	
	SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, modelid );
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetTaxiPath(path);
	SetTaxiPos();
	SetTaxiState(true);
	m_taxi_ride_time = getMSTime();

	//uint32 traveltime = uint32(path->getLength() * TAXI_TRAVEL_SPEED); // 36.7407
	float traveldist = 0;

	float lastx = 0, lasty = 0, lastz = 0;
	TaxiPathNode *firstNode = path->GetPathNode(start_node);
	uint32 add_time = 0;
	if(start_node)
	{
		TaxiPathNode *pn = path->GetPathNode(0);
		float dist = 0;
		lastx = pn->x;
		lasty = pn->y;
		lastz = pn->z;
		for(uint32 i = 1; i <= start_node; ++i)
		{
			pn = path->GetPathNode(i);
			if(!pn)
			{
				JumpToEndTaxiNode(path);
				return;
			}

			dist += CalcDistance(lastx, lasty, lastz, pn->x, pn->y, pn->z);
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		}
		add_time = uint32( dist * TAXI_TRAVEL_SPEED );
		lastx = lasty = lastz = 0;
	}
	size_t endn = path->GetNodeCount();
	if(m_taxiPaths.size())
		endn-= 2;

	for(uint32 i = start_node; i < endn; ++i)
	{
		TaxiPathNode *pn = path->GetPathNode(i);
		if(!pn)
		{
			JumpToEndTaxiNode(path);
			return;
		}

		if( pn->mapid != m_mapId )
		{
			endn = (i - 1);
			m_taxiMapChangeNode = i;

			mapchangeid = (int32)pn->mapid;
			mapchangex = pn->x;
			mapchangey = pn->y;
			mapchangez = pn->z;
			break;
		}

		if(!lastx || !lasty || !lastz)
		{
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		} else {			
			float dist = CalcDistance(lastx,lasty,lastz,
				pn->x,pn->y,pn->z);
			traveldist += dist;
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		}
	}

	uint32 traveltime = uint32(traveldist * TAXI_TRAVEL_SPEED);

	if( start_node > endn || (endn - start_node) > 200 )
		return;

	WorldPacket data(SMSG_MONSTER_MOVE, 38 + ( (endn - start_node) * 12 ) );
	data << GetNewGUID();
	data << firstNode->x << firstNode->y << firstNode->z;
	data << m_taxi_ride_time;
	data << uint8( 0 );
	data << uint32( 0x00000300 );
	data << uint32( traveltime );

	if(!cn)
		m_taxi_ride_time -= add_time;
	
	data << uint32( endn - start_node );
//	uint32 timer = 0, nodecount = 0;
//	TaxiPathNode *lastnode = NULL;

	for(uint32 i = start_node; i < endn; i++)
	{
		TaxiPathNode *pn = path->GetPathNode(i);
		if(!pn)
		{
			JumpToEndTaxiNode(path);
			return;
		}

		data << pn->x << pn->y << pn->z;
	}

	SendMessageToSet(&data, true);
  
	sEventMgr.AddEvent(this, &Player::EventTaxiInterpolate, 
		EVENT_PLAYER_TAXI_INTERPOLATE, 900, 0,0);

	if( mapchangeid < 0 )
	{
		TaxiPathNode *pn = path->GetPathNode((uint32)path->GetNodeCount() - 1);
		sEventMgr.AddEvent(this, &Player::EventDismount, path->GetPrice(), 
			pn->x, pn->y, pn->z, EVENT_PLAYER_TAXI_DISMOUNT, traveltime, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); 
	}
	else
	{
		sEventMgr.AddEvent( this, &Player::EventTeleport, (uint32)mapchangeid, mapchangex, mapchangey, mapchangez, EVENT_PLAYER_TELEPORT, traveltime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::JumpToEndTaxiNode(TaxiPath * path)
{
	// this should *always* be safe in case it cant build your position on the path!
	TaxiPathNode * pathnode = path->GetPathNode((uint32)path->GetNodeCount()-1);
	if(!pathnode) return;

	SetTaxiState(false);
	SetTaxiPath(NULL);
	UnSetTaxiPos();
	m_taxi_ride_time = 0;

	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetPlayerSpeed(RUN, m_runSpeed);

	SafeTeleport(pathnode->mapid, 0, LocationVector(pathnode->x, pathnode->y, pathnode->z));
}

void Player::RemoveSpellsFromLine(uint32 skill_line)
{
	uint32 cnt = dbcSkillLineSpell.GetNumRows();
	for(uint32 i=0; i < cnt; i++)
	{
		skilllinespell * sp = dbcSkillLineSpell.LookupRow(i);
		if(sp)
		{
			if(sp->skilline == skill_line)
			{
				// Check ourselves for this spell, and remove it..
					removeSpell(sp->spell, 0, 0, 0);
			}
		}
	}
}

void Player::CalcStat(uint32 type)
{
	int32 res;
	ASSERT( type < 5 );
	int32 pos = float2int32(((int32)BaseStats[type] * (int32)StatModPctPos[type] ) / 100.0f + (int32)FlatStatModPos[type]);
	int32 neg = float2int32(((int32)BaseStats[type] * (int32)StatModPctNeg[type] ) / 100.0f + (int32)FlatStatModNeg[type]);
	res = pos + int32(BaseStats[type]) - neg;

	if( res <= 0 )
	{
		res = 1;
	}
	else
	{
		pos += float2int32(( res * (int32)static_cast< Player* >( this )->TotalStatModPctPos[type] ) / 100.0f);
		neg += float2int32(( res * (int32)static_cast< Player* >( this )->TotalStatModPctNeg[type] ) / 100.0f);
		res = pos + int32(BaseStats[type]) - neg;
		if( res <= 0 )
			res = 1;
	}

	SetUInt32Value( UNIT_FIELD_POSSTAT0 + type, pos );
	SetUInt32Value( UNIT_FIELD_NEGSTAT0 + type, neg );
	SetUInt32Value( UNIT_FIELD_STAT0 + type, res > 0 ?res : 0 );
	if( type == 1 )
	   CalcResistance( 0 );
}

void Player::RegenerateMana(bool is_interrupted)
{
	//if (m_interruptRegen)
	//	return;

	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER1);
	uint32 mm = GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if(cur >= mm)return;
	float amt = (is_interrupted) ? GetFloatValue(PLAYER_FIELD_MOD_MANA_REGEN_INTERRUPT) : GetFloatValue(PLAYER_FIELD_MOD_MANA_REGEN);
	amt *= 2; //floats are Mana Regen Per Sec. Regen Applied every 2 secs so real value =X*2 . Shady
	//Apply shit from conf file
	amt *= sWorld.getRate(RATE_POWER1);

	if((amt<=1.0)&&(amt>0))//this fixes regen like 0.98
	{
		if(is_interrupted)
			return;
		cur++;
	}
	else
		cur += float2int32(amt);	
	SetUInt32Value(UNIT_FIELD_POWER1,(cur >= mm) ? mm : cur);
}

void Player::RegenerateHealth( bool inCombat )
{
	gtFloat* HPRegenBase = dbcHPRegenBase.LookupEntry(getLevel()-1 + (getClass()-1)*100);
	gtFloat* HPRegen =  dbcHPRegen.LookupEntry(getLevel()-1 + (getClass()-1)*100);

	uint32 cur = GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(cur >= mh)
		return;

	float amt = (m_uint32Values[UNIT_FIELD_STAT4]*HPRegen->val+HPRegenBase->val*100)*(1+PctRegenModifier);
	amt *= sWorld.getRate(RATE_HEALTH);//Apply shit from conf file

	if(inCombat)
		amt *= PctIgnoreRegenModifier;

	if(amt > 0)
	{
		if(amt <= 1.0f)//this fixes regen like 0.98
			cur++;
		else
			cur += float2int32(amt);
		SetUInt32Value(UNIT_FIELD_HEALTH,(cur>=mh) ? mh : cur);
	}
}

void Player::LooseRage(int32 decayValue)
{
	if( m_interruptRegen > 0 )
		return;

	//Rage is lost at a rate of 3 rage every 3 seconds. 
	//The Anger Management talent changes this to 2 rage every 3 seconds.
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER2);
    uint32 newrage = ((int)cur <= decayValue) ? 0 : cur-decayValue;
    if (newrage > 1000 )
	  newrage = 1000;

    SetUInt32Value(UNIT_FIELD_POWER2,newrage);
}

void Player::RegenerateEnergy()
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER4);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXPOWER4);
	if(cur >= mh)
		return;
	float amt = 20.0f * PctPowerRegenModifier[POWER_TYPE_ENERGY];

	cur += float2int32(amt);
	SetUInt32Value(UNIT_FIELD_POWER4,(cur>=mh) ? mh : cur);
}

uint32 Player::GeneratePetNumber()
{
	uint32 val = m_PetNumberMax + 1;
	for (uint32 i = 1; i < m_PetNumberMax; i++)
		if(m_Pets.find(i) == m_Pets.end())
			return i;					   // found a free one

	return val;
}

void Player::RemovePlayerPet(uint32 pet_number)
{
	std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(pet_number);
	if(itr != m_Pets.end())
	{
		delete itr->second;
		m_Pets.erase(itr);
		EventDismissPet();
	}
}
#ifndef CLUSTERING
void Player::_Relocate(uint32 mapid, const LocationVector & v, bool sendpending, bool force_new_world, uint32 instance_id)
{
	//this func must only be called when switching between maps!
	WorldPacket data(41);
	if(sendpending && mapid != m_mapId && force_new_world)
	{
		data.SetOpcode(SMSG_TRANSFER_PENDING);
		data << mapid;
		GetSession()->SendPacket(&data);
	}

	if(m_mapId != mapid || force_new_world)
	{
		uint32 status = sInstanceMgr.PreTeleport(mapid, this, instance_id);
		if(status != INSTANCE_OK)
		{
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << mapid << status;
			GetSession()->SendPacket(&data);
			return;
		}

		if(instance_id)
			m_instanceId=instance_id;

		if(IsInWorld())
		{
			RemoveFromWorld();
		}

		data.Initialize(SMSG_NEW_WORLD);
		data << (uint32)mapid << v << v.o;
		GetSession()->SendPacket( &data );
		SetMapId(mapid);
		SetPlayerStatus(TRANSFER_PENDING);
	}
	else
	{
		// via teleport ack msg
		WorldPacket * data = BuildTeleportAckMsg(v);
		m_session->SendPacket(data);
		delete data;

		if( m_CurrentTransporter && !m_lockTransportVariables )
		{
			m_CurrentTransporter->RemovePlayer(this);
			m_CurrentTransporter = NULL;
			m_TransporterGUID = 0;
		}
	}
	
	m_sentTeleportPosition = v;
	SetPosition(v);
	ResetHeartbeatCoords();

	z_axisposition = 0.0f;
}
#endif

// Player::AddItemsToWorld
// Adds all items to world, applies any modifiers for them.

void Player::AddItemsToWorld()
{
	Item * pItem;
	for(uint32 i = 0; i < INVENTORY_KEYRING_END; i++)
	{
		pItem = GetItemInterface()->GetInventoryItem(i);
		if( pItem != NULL )
		{
			pItem->PushToWorld(m_mapMgr);
			
			if(i < INVENTORY_SLOT_BAG_END)	  // only equipment slots get mods.
			{
				_ApplyItemMods(pItem, i, true, false, true);
			}

			if(pItem->IsContainer() && GetItemInterface()->IsBagSlot(i))
			{
				for(uint32 e=0; e < pItem->GetProto()->ContainerSlots; e++)
				{
					Item *item = ((Container*)pItem)->GetItem(e);
					if(item)
					{
						item->PushToWorld(m_mapMgr);
					}
				}
			}
		}
	}

	UpdateStats();
}

// Player::RemoveItemsFromWorld
// Removes all items from world, reverses any modifiers.

void Player::RemoveItemsFromWorld()
{
	Item * pItem;
	for(uint32 i = 0; i < INVENTORY_KEYRING_END; i++)
	{
		pItem = m_ItemInterface->GetInventoryItem((int8)i);
		if(pItem)
		{
			if(pItem->IsInWorld())
			{
				if(i < INVENTORY_SLOT_BAG_END)	  // only equipment slots get mods.
				{
					_ApplyItemMods(pItem, i, false, false, true);
				}
				pItem->RemoveFromWorld();
			}
	
			if(pItem->IsContainer() && GetItemInterface()->IsBagSlot(i))
			{
				for(uint32 e=0; e < pItem->GetProto()->ContainerSlots; e++)
				{
					Item *item = ((Container*)pItem)->GetItem(e);
					if(item && item->IsInWorld())
					{
						item->RemoveFromWorld();
					}
				}
			}
		}
	}

	UpdateStats();
}

uint32 Player::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target )
{
	int count = 0;
	if(target == this)
	{
		// we need to send create objects for all items.
		count += GetItemInterface()->m_CreateForPlayer(data);
	}
	count += Unit::BuildCreateUpdateBlockForPlayer(data, target);
	return count;
}

void Player::Kick(uint32 delay /* = 0 */)
{
	if(!delay)
	{
		m_KickDelay = 0;
		_Kick();
	} else {
		m_KickDelay = delay;
		sEventMgr.AddEvent(this, &Player::_Kick, EVENT_PLAYER_KICK, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::_Kick()
{
	if(!m_KickDelay)
	{
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_KICK);
		// remove now
		//GetSession()->LogoutPlayer(true);
		m_session->Disconnect();
	} else {
		if((m_KickDelay - 1000) < 500)
		{
			m_KickDelay = 0;
		} else {
			m_KickDelay -= 1000;
		}
		sChatHandler.BlueSystemMessageToPlr(this, "You will be removed from the server in %u seconds.", (uint32)(m_KickDelay/1000));
	}
}

bool Player::HasDeletedSpell(uint32 spell)
{
	return (mDeletedSpells.count(spell) > 0);
}

void Player::ClearCooldownForSpell(uint32 spell_id)
{
	if( IsInWorld() )
	{
		WorldPacket data(12);
		data.SetOpcode(SMSG_CLEAR_COOLDOWN);
		data << spell_id << GetGUID();
		GetSession()->SendPacket(&data);
	}

	// remove cooldown data from Server side lists
	uint32 i;
	PlayerCooldownMap::iterator itr, itr2;
	SpellEntry * spe = dbcSpell.LookupEntry(spell_id);
	if(!spe) return;

	for(i = 0; i < NUM_COOLDOWN_TYPES; ++i)
	{
		for( itr = m_cooldownMap[i].begin(); itr != m_cooldownMap[i].end(); )
		{
			itr2 = itr++;
			if( ( i == COOLDOWN_TYPE_CATEGORY && itr2->first == spe->Category ) ||
				( i == COOLDOWN_TYPE_SPELL && itr2->first == spe->Id ) )
			{
				m_cooldownMap[i].erase( itr2 );
			}
		}
	}
}

void Player::ClearCooldownsOnLine(uint32 skill_line, uint32 called_from)
{
	// found an easier way.. loop spells, check skill line
	SpellSet::const_iterator itr = mSpells.begin();
	skilllinespell *sk;
	for(; itr != mSpells.end(); ++itr)
	{
		if((*itr) == called_from)	   // skip calling spell.. otherwise spammies! :D
			continue;

		sk = objmgr.GetSpellSkill((*itr));
		if(sk && sk->skilline == skill_line)
			ClearCooldownForSpell((*itr));
	}
}

void Player::ResetAllCooldowns()
{
	uint32 guid = (uint32)GetSelection();

	switch(getClass())
	{
		case WARRIOR:
		{
			ClearCooldownsOnLine(26, guid);
			ClearCooldownsOnLine(256, guid);
			ClearCooldownsOnLine(257 , guid);
		} break;
		case PALADIN:
		{
			ClearCooldownsOnLine(594, guid);
			ClearCooldownsOnLine(267, guid);
			ClearCooldownsOnLine(184, guid);
		} break;
		case HUNTER:
		{
			ClearCooldownsOnLine(50, guid);
			ClearCooldownsOnLine(51, guid);
			ClearCooldownsOnLine(163, guid);
		} break;
		case ROGUE:
		{
			ClearCooldownsOnLine(253, guid);
			ClearCooldownsOnLine(38, guid);
			ClearCooldownsOnLine(39, guid);
		} break;
		case PRIEST:
		{
			ClearCooldownsOnLine(56, guid);
			ClearCooldownsOnLine(78, guid);
			ClearCooldownsOnLine(613, guid);
		} break;
		case SHAMAN:
		{
			ClearCooldownsOnLine(373, guid);
			ClearCooldownsOnLine(374, guid);
			ClearCooldownsOnLine(375, guid);
		} break;
		case MAGE:
		{
			ClearCooldownsOnLine(6, guid);
			ClearCooldownsOnLine(8, guid);
			ClearCooldownsOnLine(237, guid);
		} break;
		case WARLOCK:
		{
			ClearCooldownsOnLine(355, guid);
			ClearCooldownsOnLine(354, guid);
			ClearCooldownsOnLine(593, guid);
		} break;
		case DRUID:
		{
			ClearCooldownsOnLine(573, guid);
			ClearCooldownsOnLine(574, guid);
			ClearCooldownsOnLine(134, guid);
		} break;

		default: return; break;
	}
}

void Player::PushUpdateData(ByteBuffer *data, uint32 updatecount)
{
	// imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->size() + bUpdateBuffer.size() ) >= 63000 )
	{
		if( IsInWorld() )
			ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		else
			bUpdateBuffer.clear();
	}

	mUpdateCount += updatecount;
	bUpdateBuffer.append(*data);

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	
	_bufferS.Release();
}

void Player::PushUpdateData(StackBuffer *data, uint32 updatecount)
{
	// imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->GetSize() + bUpdateBuffer.size() ) >= 63000 )
	{
		if( IsInWorld() )
			ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		else
			bUpdateBuffer.clear();
	}

	mUpdateCount += updatecount;
	bUpdateBuffer.append((const uint8*)data->GetBufferPointer(), data->GetSize());

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}

	_bufferS.Release();
}

void Player::PushOutOfRange(const WoWGuid & guid)
{
	_bufferS.Acquire();
	mOutOfRangeIds << guid;
	++mOutOfRangeIdCount;

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	_bufferS.Release();
}

void Player::PushCreationData(ByteBuffer *data, uint32 updatecount)
{
    // imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->size() + bCreationBuffer.size() + mOutOfRangeIds.size() ) >= 63000 )
	{
		if( IsInWorld() )
			ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		else
			bCreationBuffer.clear();
	}

	mCreationCount += updatecount;
	bCreationBuffer.append(*data);

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	
	_bufferS.Release();

}

void Player::ProcessPendingUpdates(ByteBuffer *pBuildBuffer, ByteBuffer *pCompressionBuffer)
{
	_bufferS.Acquire();
    if(!bUpdateBuffer.size() && !mOutOfRangeIds.size() && !bCreationBuffer.size())
	{
		_bufferS.Release();
		return;
	}

	size_t bBuffer_size =  (bCreationBuffer.size() > bUpdateBuffer.size() ? bCreationBuffer.size() : bUpdateBuffer.size()) + 10 + (mOutOfRangeIds.size() * 9);
    //uint8 * update_buffer = new uint8[bBuffer_size];
	pBuildBuffer->resize(bBuffer_size);
	uint8 * update_buffer = (uint8*)pBuildBuffer->contents();
	size_t c = 0;

    //build out of range updates if creation updates are queued
    if(bCreationBuffer.size() || mOutOfRangeIdCount)
    {
        *(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mCreationCount + 1) : mCreationCount);	c += 4;
        update_buffer[c] = 1;																			   ++c;

            // append any out of range updates
	    if(mOutOfRangeIdCount)
	    {
		    update_buffer[c]				= UPDATETYPE_OUT_OF_RANGE_OBJECTS;			 ++c;
            *(uint32*)&update_buffer[c]	 = mOutOfRangeIdCount;						  c += 4;
		    memcpy(&update_buffer[c], mOutOfRangeIds.contents(), mOutOfRangeIds.size());   c += mOutOfRangeIds.size();
		    mOutOfRangeIds.clear();
		    mOutOfRangeIdCount = 0;
	    }

        if(bCreationBuffer.size())
			memcpy(&update_buffer[c], bCreationBuffer.contents(), bCreationBuffer.size());  c += bCreationBuffer.size();

        // clear our update buffer
	    bCreationBuffer.clear();
	    mCreationCount = 0;

        // compress update packet
	    // while we said 350 before, i'm gonna make it 500 :D
	    if(c < (size_t)sWorld.compression_threshold || !CompressAndSendUpdateBuffer((uint32)c, update_buffer, pCompressionBuffer))
	    {
		    // send uncompressed packet -> because we failed
		    m_session->OutPacket(SMSG_UPDATE_OBJECT, (uint16)c, update_buffer);
	    }
    }

	if(bUpdateBuffer.size())
	{
		c = 0;
		*(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mUpdateCount + 1) : mUpdateCount);	c += 4;
		update_buffer[c] = 1;																			   ++c;
		memcpy(&update_buffer[c], bUpdateBuffer.contents(), bUpdateBuffer.size());  c += bUpdateBuffer.size();

		// clear our update buffer
		bUpdateBuffer.clear();
		mUpdateCount = 0;

		// compress update packet
		// while we said 350 before, i'm gonna make it 500 :D
		if(c < (size_t)sWorld.compression_threshold || !CompressAndSendUpdateBuffer((uint32)c, update_buffer, pCompressionBuffer))
		{
			// send uncompressed packet -> because we failed
			m_session->OutPacket(SMSG_UPDATE_OBJECT, (uint16)c, update_buffer);
		}
	}
	
	bProcessPending = false;
	_bufferS.Release();
	//delete [] update_buffer;
	pBuildBuffer->clear();

	// send any delayed packets
	WorldPacket * pck;
	while(delayedPackets.size())
	{
		pck = delayedPackets.next();
		//printf("Delayed packet opcode %u sent.\n", pck->GetOpcode());
		m_session->SendPacket(pck);
		delete pck;
	}

	// resend speed if needed
	if(resend_speed)
	{
		SetPlayerSpeed(RUN, m_runSpeed);
		SetPlayerSpeed(FLY, m_flySpeed);
		resend_speed=false;
	}
}

bool Player::CompressAndSendUpdateBuffer(uint32 size, const uint8* update_buffer, ByteBuffer *pCompressionBuffer)
{
	uint32 destsize = size + size/10 + 16;
	int rate = sWorld.getIntRate(INTRATE_COMPRESSION);
	if(size >= 40000 && rate < 6)
		rate = 6;

	// set up stream
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree  = 0;
	stream.opaque = 0;
	
	if(deflateInit(&stream, rate) != Z_OK)
	{
		DEBUG_LOG("deflateInit failed.");
		return false;
	}

	//uint8 *buffer = new uint8[destsize];
	//memset(buffer,0,destsize);	/* fix umr - burlex */
	pCompressionBuffer->resize(destsize);
	uint8 *buffer = (uint8*)pCompressionBuffer->contents();
	
	// set up stream pointers
	stream.next_out  = (Bytef*)buffer+4;
	stream.avail_out = destsize;
	stream.next_in   = (Bytef*)update_buffer;
	stream.avail_in  = size;

	// call the actual process
	if(deflate(&stream, Z_NO_FLUSH) != Z_OK ||
		stream.avail_in != 0)
	{
		DEBUG_LOG("deflate failed.");
		delete [] buffer;
		return false;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		DEBUG_LOG("deflate failed: did not end stream");
		delete [] buffer;
		return false;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		DEBUG_LOG("deflateEnd failed.");
		delete [] buffer;
		return false;
	}

	// fill in the full size of the compressed stream
	*(uint32*)&buffer[0] = size;

	// send it
	m_session->OutPacket(SMSG_COMPRESSED_UPDATE_OBJECT, (uint16)stream.total_out + 4, buffer);

	// cleanup memory
	//delete [] buffer;
	pCompressionBuffer->clear();
	
	return true;
}

void Player::ClearAllPendingUpdates()
{
	_bufferS.Acquire();
	bProcessPending = false;
	mUpdateCount = 0;
	bUpdateBuffer.clear();
	_bufferS.Release();
}

void Player::AddSplinePacket(uint64 guid, ByteBuffer* packet)
{
	SplineMap::iterator itr = _splineMap.find(guid);
	if(itr != _splineMap.end())
	{
		delete itr->second;
		_splineMap.erase(itr);
	}
	_splineMap.insert( SplineMap::value_type( guid, packet ) );
}

ByteBuffer* Player::GetAndRemoveSplinePacket(uint64 guid)
{
	SplineMap::iterator itr = _splineMap.find(guid);
	if(itr != _splineMap.end())
	{
		ByteBuffer *buf = itr->second;
		_splineMap.erase(itr);
		return buf;
	}
	return NULL;
}

void Player::ClearSplinePackets()
{
	SplineMap::iterator it2;
	for(SplineMap::iterator itr = _splineMap.begin(); itr != _splineMap.end();)
	{
		it2 = itr;
		++itr;
		delete it2->second;
		_splineMap.erase(it2);
	}
	_splineMap.clear();
}



bool Player::ExitInstance()
{
	if(!m_bgEntryPointX)
		return false;

	RemoveFromWorld();

	SafeTeleport(m_bgEntryPointMap, m_bgEntryPointInstance, LocationVector(
		m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO));

	return true;
}

void Player::SaveEntryPoint(uint32 mapId)
{	
	if(IS_INSTANCE(GetMapId()))
		return; // dont save if we're not on the main continent.
	//otherwise we could end up in an endless loop :P
	MapInfo * pMapinfo = WorldMapInfoStorage.LookupEntry(mapId);

	if(pMapinfo)
	{
		m_bgEntryPointX = pMapinfo->repopx;
		m_bgEntryPointY = pMapinfo->repopy;
		m_bgEntryPointZ = pMapinfo->repopz;
		m_bgEntryPointO = GetOrientation();
		m_bgEntryPointMap = pMapinfo->repopmapid;
		m_bgEntryPointInstance = GetInstanceID();
	}
	else
	{
		m_bgEntryPointMap	 = 0;
		m_bgEntryPointX		 = 0;	
		m_bgEntryPointY		 = 0;
		m_bgEntryPointZ		 = 0;
		m_bgEntryPointO		 = 0;
		m_bgEntryPointInstance  = 0;
	}
}

void Player::CleanupGossipMenu()
{
	if(CurrentGossipMenu)
	{
		delete CurrentGossipMenu;
		CurrentGossipMenu = NULL;
	}
}

void Player::Gossip_Complete()
{
	GetSession()->OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);
	CleanupGossipMenu();
}

void Player::ForceAreaUpdate()
{
	// force update area id
	if(m_position.x > _maxX || m_position.x < _minX || m_position.y > _maxY || m_position.y < _minY || !IsInWorld())
		return;

	m_areaDBC = NULL;
	m_AreaID = m_mapMgr->GetAreaID(m_position.x, m_position.y);
	if( m_AreaID == 0xffff )
		m_AreaID = 0;
	else
		m_areaDBC = dbcArea.LookupEntryForced(m_AreaID);

	if( m_areaDBC != NULL )
	{
		// parent id actually would be a better name
		if( m_areaDBC->ZoneId && m_zoneId != m_areaDBC->ZoneId )
			m_zoneId = m_areaDBC->ZoneId;
	}
}

void Player::ZoneUpdate(uint32 ZoneId)
{
	uint32 oldzone = m_zoneId;
	m_zoneId = ZoneId;

	/* how the f*ck is this happening */
	if( m_playerInfo == NULL )
	{
		m_playerInfo = objmgr.GetPlayerInfo(GetLowGUID());
		if( m_playerInfo == NULL )
		{
			m_session->Disconnect();
			return;
		}
	}

	m_playerInfo->lastZone = ZoneId;
	sHookInterface.OnZone(this, ZoneId, oldzone);

	AreaTable *at = dbcArea.LookupEntry(GetAreaID());
	if(at && at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY)
	{
		Unit * pUnit = (GetSelection() == 0) ? 0 : (m_mapMgr ? m_mapMgr->GetUnit(GetSelection()) : 0);
		if(pUnit && DuelingWith != pUnit)
		{
			EventAttackStop();
			smsg_AttackStop(pUnit);
		}

		if(m_currentSpell)
		{
			Unit * target = m_currentSpell->GetUnitTarget();
			if(target && target != DuelingWith && target != this)
				m_currentSpell->cancel();
		}
	}


#ifdef OPTIMIZED_PLAYER_SAVING
	save_Zone();
#endif

	// send new world states
	ForceAreaUpdate();

	at = dbcArea.LookupEntryForced(m_zoneId);
	if( m_session->HasGMPermissions() && m_zoneId != 0 )
	{
		if( at != NULL )
		{
			BroadcastMessage("Entered zone: %s.", at->name);
		}
	}

	if( at != NULL )
	{
		// update channelz!!!
		set<uint32>::iterator itr = m_channels.begin();
		set<uint32>::iterator itr2;
		Channel *p;
		uint32 cid;
		char strbuf[200];
		uint32 dbcid;
		for( ; itr != m_channels.end(); )
		{
			cid = *itr;
			itr2 = itr;
			++itr;

			p = channelmgr.GetChannel(cid);
			if( p == NULL )
			{
				m_channels.erase(itr2);
				continue;
			}
			
			if( p->m_flags & 0x10 && !(p->m_flags & 0x40) && p->pDBC )
			{
				// general/changable (and not lfg)
				dbcid = p->pDBC->id;
				snprintf(strbuf, 200, p->pDBC->pattern, at->name);
				if( stricmp(strbuf, p->m_name.c_str()) )
				{
					// different channel name. :O
					p->Part(this, true);
				}

				p = channelmgr.GetCreateChannel(strbuf, this, dbcid);
				p->AttemptJoin(this, "");
			}
		}
		// flying auras
		if( flying_aura != 0 && !(at->AreaFlags & AREA_FLYING_PERMITTED) )
		{
			// remove flying buff
			RemoveAura(flying_aura);
		}
	}

	if( IsInWorld() )				// should be
		m_mapMgr->GetStateManager().SendWorldStates(this);
}

void Player::SendTradeUpdate()
{
	Player * pTarget = GetTradeTarget();
	if(!pTarget)
		return;

	WorldPacket data(SMSG_TRADE_STATUS_EXTENDED, 500);
	/*data << uint8(1);
	data << uint32(2) << uint32(2);
	data << mTradeGold << uint32(0);*/
	data << uint8(1);
	data << uint32(0x19);
	data << m_tradeSequence;
	data << m_tradeSequence++;
	data << mTradeGold << uint32(0);
		
	// Items
	for(uint32 Index = 0; Index < 7; ++Index)
	{
		Item * pItem = mTradeItems[Index];
		if(pItem != 0)
		{
			ItemPrototype * pProto = pItem->GetProto();
			ASSERT(pProto != 0);

			data << uint8(Index);

			data << pProto->ItemId;
			data << pProto->DisplayInfoID;
			data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);  // Amount		   OK
			
			// Enchantment stuff
			data << uint32(0);									  // unknown
			data << pItem->GetUInt64Value(ITEM_FIELD_GIFTCREATOR);  // gift creator	 OK
			data << pItem->GetUInt32Value(ITEM_FIELD_ENCHANTMENT);  // Item Enchantment OK
			data << uint32(0);									  // unknown
			data << uint32(0);									  // unknown
			data << uint32(0);									  // unknown
			data << pItem->GetUInt64Value(ITEM_FIELD_CREATOR);	  // item creator	 OK
			data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);  // Spell Charges	OK

			data << uint32(0);									  // seems like time stamp or something like that
			data << pItem->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID);
			data << pProto->LockId;								 // lock ID		  OK
			data << pItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
			data << pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);
		}
	}

	pTarget->GetSession()->SendPacket(&data);
}

void Player::RequestDuel(Player *pTarget)
{
	// We Already Dueling or have already Requested a Duel

	if( DuelingWith != NULL )
		return;

	if( m_duelState != DUEL_STATE_FINISHED )
		return;

	SetDuelState( DUEL_STATE_REQUESTED );

	//Setup Duel
	pTarget->DuelingWith = this;
	DuelingWith = pTarget;

	//Get Flags position
	float dist = CalcDistance(pTarget);
	dist = dist * 0.5f; //half way
	float x = (GetPositionX() + pTarget->GetPositionX()*dist)/(1+dist) + cos(GetOrientation()+(float(M_PI)/2))*2;
	float y = (GetPositionY() + pTarget->GetPositionY()*dist)/(1+dist) + sin(GetOrientation()+(float(M_PI)/2))*2;
	float z = (GetPositionZ() + pTarget->GetPositionZ()*dist)/(1+dist);

	//Create flag/arbiter
	GameObject* pGameObj = GetMapMgr()->CreateGameObject(21680);
	pGameObj->CreateFromProto(21680,GetMapId(), x, y, z, GetOrientation());
	pGameObj->SetInstanceID(GetInstanceID());

	//Spawn the Flag
	pGameObj->SetUInt64Value(OBJECT_FIELD_CREATED_BY, GetGUID());
	pGameObj->SetUInt32Value(GAMEOBJECT_FACTION, GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, GetUInt32Value(UNIT_FIELD_LEVEL));

	//Assign the Flag 
	SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
	pTarget->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());

	pGameObj->PushToWorld(m_mapMgr);

	WorldPacket data(SMSG_DUEL_REQUESTED, 16);
	data << pGameObj->GetGUID();
	data << GetGUID();
	pTarget->GetSession()->SendPacket(&data);
}

void Player::DuelCountdown()
{
	if( DuelingWith == NULL )
		return;

	m_duelCountdownTimer -= 1000;

	if( m_duelCountdownTimer < 0 )
		m_duelCountdownTimer = 0;

	if( m_duelCountdownTimer == 0 )
	{
		// Start Duel.
		SetUInt32Value( UNIT_FIELD_POWER2, 0 );
		DuelingWith->SetUInt32Value( UNIT_FIELD_POWER2, 0 );

		//Give the players a Team
		DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 1 ); // Duel Requester
		SetUInt32Value( PLAYER_DUEL_TEAM, 2 );

		SetDuelState( DUEL_STATE_STARTED );
		DuelingWith->SetDuelState( DUEL_STATE_STARTED );

		sEventMgr.AddEvent( this, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
		sEventMgr.AddEvent( DuelingWith, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
	}
}

void Player::DuelBoundaryTest()
{
	//check if in bounds
	if(!IsInWorld())
		return;

	GameObject * pGameObject = GetMapMgr()->GetGameObject(GET_LOWGUID_PART(GetUInt64Value(PLAYER_DUEL_ARBITER)));
	if(!pGameObject)
	{
		EndDuel(DUEL_WINNER_RETREAT);
		return;
	}

	float Dist = CalcDistance((Object*)pGameObject);

	if(Dist > 75.0f)
	{
		// Out of bounds
		if(m_duelStatus == DUEL_STATUS_OUTOFBOUNDS)
		{
			// we already know, decrease timer by 500
			m_duelCountdownTimer -= 500;
			if(m_duelCountdownTimer == 0)
			{
				// Times up :p
				DuelingWith->EndDuel(DUEL_WINNER_RETREAT);
			}
		}
		else
		{
			// we just went out of bounds
			// set timer
			m_duelCountdownTimer = 10000;

			// let us know
			m_session->OutPacket(SMSG_DUEL_OUTOFBOUNDS, 4, &m_duelCountdownTimer);
			m_duelStatus = DUEL_STATUS_OUTOFBOUNDS;
		}
	}
	else
	{
		// we're in range
		if(m_duelStatus == DUEL_STATUS_OUTOFBOUNDS)
		{
			// just came back in range
			m_session->OutPacket(SMSG_DUEL_INBOUNDS);
			m_duelStatus = DUEL_STATUS_INBOUNDS;
		}
	}
}

void Player::EndDuel(uint8 WinCondition)
{
	if( m_duelState == DUEL_STATE_FINISHED )
		return;

	// Remove the events
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_DUEL_COUNTDOWN );
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );

	for( uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x )
	{
		if( m_auras[x] == NULL )
			continue;
		if( m_auras[x]->WasCastInDuel() )
			m_auras[x]->Remove();
	}

	m_duelState = DUEL_STATE_FINISHED;

	if( DuelingWith == NULL )
		return;

	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );
	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_COUNTDOWN );

	// spells waiting to hit
	sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);

	for( uint32 x = 0; x < MAX_AURAS; ++x )
	{
		if( DuelingWith->m_auras[x] == NULL )
			continue;
		if( DuelingWith->m_auras[x]->WasCastInDuel() )
			DuelingWith->m_auras[x]->Remove();
	}

	DuelingWith->m_duelState = DUEL_STATE_FINISHED;

	//Announce Winner
	WorldPacket data( SMSG_DUEL_WINNER, 500 );
	data << uint8( WinCondition );
	data << GetName() << DuelingWith->GetName();
	SendMessageToSet( &data, true );

	data.Initialize( SMSG_DUEL_COMPLETE );
	data << uint8( 1 );
	SendMessageToSet( &data, true );

	//Clear Duel Related Stuff

	GameObject* arbiter = m_mapMgr ? GetMapMgr()->GetGameObject(GET_LOWGUID_PART(GetUInt64Value(PLAYER_DUEL_ARBITER))) : 0;

	if( arbiter != NULL )
	{
		arbiter->RemoveFromWorld( true );
		delete arbiter;
	}

	SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
	DuelingWith->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );

	SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
	DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 0 );

	EventAttackStop();
	DuelingWith->EventAttackStop();

	// Call off pet
	if( this->GetSummon() != NULL )
	{
		this->GetSummon()->CombatStatus.Vanished();
		this->GetSummon()->GetAIInterface()->SetUnitToFollow( this );
		this->GetSummon()->GetAIInterface()->HandleEvent( EVENT_FOLLOWOWNER, this->GetSummon(), 0 );
		this->GetSummon()->GetAIInterface()->WipeTargetList();
	}

	// removing auras that kills players after if low HP
	/*RemoveNegativeAuras(); NOT NEEDED. External targets can always gank both duelers with DoTs. :D
	DuelingWith->RemoveNegativeAuras();*/

	//Stop Players attacking so they don't kill the other player
	m_session->OutPacket( SMSG_CANCEL_COMBAT );
	DuelingWith->m_session->OutPacket( SMSG_CANCEL_COMBAT );

	smsg_AttackStop( DuelingWith );
	DuelingWith->smsg_AttackStop( this );

	DuelingWith->m_duelCountdownTimer = 0;
	m_duelCountdownTimer = 0;

	DuelingWith->DuelingWith = NULL;
	DuelingWith = NULL;
}

void Player::StopMirrorTimer(uint32 Type)
{
	m_session->OutPacket(SMSG_STOP_MIRROR_TIMER, 4, &Type);
}

void Player::EventTeleport(uint32 mapid, float x, float y, float z)
{
	SafeTeleport(mapid, 0, LocationVector(x, y, z));
}

void Player::ApplyLevelInfo(LevelInfo* Info, uint32 Level)
{
	// Apply level
	SetUInt32Value(UNIT_FIELD_LEVEL, Level);

	// Set next level conditions
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, Info->XPToNextLevel);

	// Set stats
	for(uint32 i = 0; i < 5; ++i)
	{
		BaseStats[i] = Info->Stat[i];
		CalcStat(i);
	}

	// Set health / mana
	SetUInt32Value(UNIT_FIELD_HEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_POWER1, Info->Mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, Info->Mana);

	// Calculate talentpoints
	uint32 TalentPoints = 0;
	if(Level >= 10)
		TalentPoints = Level - 9;

	SetUInt32Value(PLAYER_CHARACTER_POINTS1, TalentPoints);

	// Set base fields
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, Info->Mana);

	_UpdateMaxSkillCounts();
	UpdateStats();
	//UpdateChances();	
	m_playerInfo->lastLevel = Level;

	DEBUG_LOG("Player %s set parameters to level %u", GetName(), Level);
}

void Player::BroadcastMessage(const char* Format, ...)
{
	va_list l;
	va_start(l, Format);
	char Message[1024];
	vsnprintf(Message, 1024, Format, l);
	va_end(l);

	WorldPacket * data = sChatHandler.FillSystemMessageData(Message);
	m_session->SendPacket(data);
	delete data;
}
/*
const double BaseRating []= {
	2.5,//weapon_skill_ranged!!!!
	1.5,//defense=comba_r_1
	12,//dodge
	20,//parry=3
	5,//block=4
	10,//melee hit
	10,//ranged hit
	8,//spell hit=7
	14,//melee critical strike=8
	14,//ranged critical strike=9
	14,//spell critical strike=10
	0,//
	0,
	0,
	25,//resilience=14
	25,//resil .... meaning unknown
	25,//resil .... meaning unknown
	10,//MELEE_HASTE_RATING=17
	10,//RANGED_HASTE_RATING=18
	10,//spell_haste_rating = 19???
	2.5,//melee weapon skill==20
	2.5,//melee second hand=21

};
*/
float Player::CalcRating( uint32 index )
{
	uint32 relative_index = index - (PLAYER_FIELD_COMBAT_RATING_1);
	float rating = float(m_uint32Values[index]);
	/*if( relative_index <= 10 || ( relative_index >= 14 && relative_index <= 21 ) )
	{
		double rating = (double)m_uint32Values[index];
		int level = getLevel();
		if( level < 10 )//this is not dirty fix -> it is from wowwiki
			level = 10;
		double cost;
		if( level < 60 )
			cost = ( double( level ) - 8.0 ) / 52.0;
		else
			cost = 82.0 / ( 262.0 - 3.0 *  double( level ) );
		return float( rating / ( BaseRating[relative_index] * cost ) );
	}
	else
		return 0.0f;*/

	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	if( level > 100 )
		level = 100;

	CombatRatingDBC * pDBCEntry = dbcCombatRating.LookupEntryForced( relative_index * 100 + level - 1 );
	if( pDBCEntry == NULL )
		return rating;
	else
		return (rating / pDBCEntry->val);
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O)
{
	return SafeTeleport(MapID, InstanceID, LocationVector(X, Y, Z, O));
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, const LocationVector & vec)
{
	if(DuelingWith && (MapID != GetMapId() || m_position.Distance2DSq(vec) >= 900) )		// 30
	{
		EndDuel(DUEL_WINNER_RETREAT);
	}

#ifdef CLUSTERING
	/* Clustering Version */
	MapInfo * mi = WorldMapInfoStorage.LookupEntry(MapID);

	// Lookup map info
	if(mi && mi->flags & WMI_INSTANCE_XPACK_01 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_01))
	{
		WorldPacket msg(SMSG_BROADCAST_MSG, 50);
		msg << uint32(3) << "You must have The Burning Crusade Expansion to access this content." << uint8(0);
		m_session->SendPacket(&msg);
		return false;
	}

	uint32 instance_id;
	bool map_change = false;
	if(mi && mi->type == 0)
	{
		// single instance map
		if(MapID != m_mapId)
		{
			map_change = true;
			instance_id = 0;
		}
	}
	else
	{
		// instanced map
		if(InstanceID != GetInstanceID())
			map_change = true;

		instance_id = InstanceID;
	}

	if(map_change)
	{
		WorldPacket data(SMSG_TRANSFER_PENDING, 4);
		data << uint32(MapID);
		GetSession()->SendPacket(&data);
		sClusterInterface.RequestTransfer(this, MapID, instance_id, vec);
		return;
	}

	m_sentTeleportPosition = vec;
	SetPosition(vec);
	ResetHeartbeatCoords();

	WorldPacket * data = BuildTeleportAckMsg(vec);
	m_session->SendPacket(data);
	delete data;
#else
	/* Normal Version */
	bool instance = false;
	MapInfo * mi = WorldMapInfoStorage.LookupEntry(MapID);

	if(InstanceID && (uint32)m_instanceId != InstanceID)
	{
		instance = true;
		this->SetInstanceID(InstanceID);
		// if we are mounted remove it
		if( m_MountSpellId )
			RemoveAura( m_MountSpellId );
	}
	else if(m_mapId != MapID)
	{
		instance = true;
		// if we are mounted remove it
		if( m_MountSpellId )
			RemoveAura( m_MountSpellId );
	}

	// make sure player does not drown when teleporting from under water
	if (m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
		m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;

	if(flying_aura && MapID != 530)
	{
		RemoveAura(flying_aura);
		flying_aura = 0;
	}

	// Lookup map info
	if(mi && mi->flags & WMI_INSTANCE_XPACK_01 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_01))
		BroadcastMessage("You must have The Burning Crusade Expansion to access this content.");

	_Relocate(MapID, vec, true, instance, InstanceID);
	return true;
#endif
}

void Player::SafeTeleport(MapMgr * mgr, const LocationVector & vec)
{
	if(IsInWorld())
		RemoveFromWorld();

	m_mapId = mgr->GetMapId();
	m_instanceId = mgr->GetInstanceID();
	WorldPacket data(SMSG_TRANSFER_PENDING, 20);
	data << mgr->GetMapId();
	GetSession()->SendPacket(&data);

	data.Initialize(SMSG_NEW_WORLD);
	data << mgr->GetMapId() << vec << vec.o;
	GetSession()->SendPacket(&data);

	SetPlayerStatus(TRANSFER_PENDING);
	m_sentTeleportPosition = vec;
	SetPosition(vec);
	ResetHeartbeatCoords();

	if(DuelingWith)
	{
		EndDuel(DUEL_WINNER_RETREAT);
	}
}

void Player::SetGuildId(uint32 guildId)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDID;
		sEventMgr.AddEvent(((Object*)this), &Object::EventSetUInt32Value, field, guildId, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDID,guildId);
	}
}

void Player::SetGuildRank(uint32 guildRank)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDRANK;
		sEventMgr.AddEvent(((Object*)this), &Object::EventSetUInt32Value, field, guildRank, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDRANK,guildRank);
	}
}

void Player::UpdatePvPArea()
{
	if( m_areaDBC == NULL )
		return;

	// This is where all the magic happens :P
    if((m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
	{
		if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && !m_pvpTimer)
		{
			// I'm flagged and I just walked into a zone of my type. Start the 5min counter.
			ResetPvPTimer();
		}
        return;
	}
	else
	{
        //Enemy city check
        if(m_areaDBC->AreaFlags & AREA_CITY_AREA || m_areaDBC->AreaFlags & AREA_CITY)
        {
            if((m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
            {
                if(!IsPvPFlagged()) SetPvPFlag();
                    StopPvPTimer();
                return;
            }
        }

        //fix for zone areas.
        if(m_areaDBC->ZoneId)
        {
            AreaTable * at2 = dbcArea.LookupEntry(m_areaDBC->ZoneId);
            if(at2 && (at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || at2 && (at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
            {
                if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && !m_pvpTimer)
		        {
			        // I'm flagged and I just walked into a zone of my type. Start the 5min counter.
			        ResetPvPTimer();
		        }
                return;
            }
            //enemy territory check
            if(at2 && at2->AreaFlags & AREA_CITY_AREA || at2 && at2->AreaFlags & AREA_CITY)
            {
                if(at2 && (at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || at2 && (at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
                {
                   if(!IsPvPFlagged()) SetPvPFlag();
                       StopPvPTimer();
                   return;
                }
            }
        }
        
		// I just walked into either an enemies town, or a contested zone.
		// Force flag me if i'm not already.
        if(m_areaDBC->category == AREAC_SANCTUARY || m_areaDBC->AreaFlags & AREA_SANCTUARY)
        {
            if(IsPvPFlagged()) RemovePvPFlag();

			RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);

            StopPvPTimer();
        }
        else
        {
            //contested territory
            if(sWorld.GetRealmType() == REALM_PVP)
            {
                //automaticaly sets pvp flag on contested territorys.
                if(!IsPvPFlagged()) SetPvPFlag();
                StopPvPTimer();
            }

            if(sWorld.GetRealmType() == REALM_PVE)
            {
                if(HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
                {
                    if(!IsPvPFlagged()) SetPvPFlag();
                }
                else if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && IsPvPFlagged() && !m_pvpTimer)
                {
                    ResetPvPTimer();
                }
            }

            if(m_areaDBC->AreaFlags & AREA_PVP_ARENA)			/* ffa pvp arenas will come later */
            {
                if(!IsPvPFlagged()) SetPvPFlag();

			    SetFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);
            }
            else
            {
			    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);
            }
        }
	}
}

void Player::BuildFlagUpdateForNonGroupSet(uint32 index, uint32 flag)
{
    Object *curObj;
    for (Object::InRangeSet::iterator iter = GetInRangeSetBegin(); iter != GetInRangeSetEnd();)
	{
		curObj = *iter;
		iter++;
        if(curObj->IsPlayer())
        {
            Group* pGroup = static_cast< Player* >( curObj )->GetGroup();
            if( pGroup != NULL && pGroup == GetGroup())
            {
				//TODO: huh? if this is unneeded change the if to the inverse and don't waste jmp space
            }
            else
            {
                BuildFieldUpdatePacket( static_cast< Player* >( curObj ), index, flag );
            }
        }
    }
}

void Player::LoginPvPSetup()
{
	// Make sure we know our area ID.
	_EventExploration();

    if(isAlive())
        CastSpell(this, PLAYER_HONORLESS_TARGET_SPELL, true);
}

void Player::PvPToggle()
{
    if(sWorld.GetRealmType() == REALM_PVE)
    {
	    if(m_pvpTimer > 0)
	    {
		    // Means that we typed /pvp while we were "cooling down". Stop the timer.
		    StopPvPTimer();

		    SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);

            if(!IsPvPFlagged()) SetPvPFlag();
	    }
	    else
	    {
		    if(IsPvPFlagged())
		    {
                if(m_areaDBC != NULL && ( m_areaDBC->AreaFlags & AREA_CITY_AREA || m_areaDBC->AreaFlags & AREA_CITY) )
                {
                    if((m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
                    {
                    }
                    else
                    {
                        // Start the "cooldown" timer.
			            ResetPvPTimer();
                    }
                }
                else
                {
                    // Start the "cooldown" timer.
			        ResetPvPTimer();
                }
			    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
		    }
		    else
		    {
			    // Move into PvP state.
			    SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);

			    StopPvPTimer();
			    SetPvPFlag();
		    }		
	    }
    }
    else if(sWorld.GetRealmType() == REALM_PVP)
    {
	    if(m_areaDBC == NULL)
            return;

	    // This is where all the magic happens :P
        if((m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 1))
	    {
            if(m_pvpTimer > 0)
	        {
		        // Means that we typed /pvp while we were "cooling down". Stop the timer.
		        StopPvPTimer();

		        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);

                if(!IsPvPFlagged()) SetPvPFlag();
	        }
	        else
	        {
		        if(IsPvPFlagged())
		        {
                    // Start the "cooldown" timer.
			        ResetPvPTimer();

			        RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
		        }
		        else
		        {
			        // Move into PvP state.
			        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);

			        StopPvPTimer();
			        SetPvPFlag();
		        }		
	        }
        }
        else
        {
            if(m_areaDBC->ZoneId)
            {
                AreaTable * at2 = dbcArea.LookupEntry(m_areaDBC->ZoneId);
                if( at2 != NULL && ((at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (at2->category == AREAC_HORDE_TERRITORY && GetTeam() == 1)) )
                {
                    if(m_pvpTimer > 0)
	                {
		                // Means that we typed /pvp while we were "cooling down". Stop the timer.
		                StopPvPTimer();

		                SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);

                        if(!IsPvPFlagged()) SetPvPFlag();
	                }
	                else
	                {
		                if(IsPvPFlagged())
		                {
			                // Start the "cooldown" timer.
			                ResetPvPTimer();

			                RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
		                }
		                else
		                {
			                // Move into PvP state.
			                SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);

			                StopPvPTimer();
			                SetPvPFlag();
		                }		
	                }
                    return;
                }
            }

            if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
		        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
            else
            {
			    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
            }
        }
    }
}

void Player::ResetPvPTimer()
{
	 m_pvpTimer = sWorld.getIntRate(INTRATE_PVPTIMER);
}

void Player::CalculateBaseStats()
{
	if(!lvlinfo) return;

	memcpy(BaseStats, lvlinfo->Stat, sizeof(uint32) * 5);

	LevelInfo * levelone = objmgr.GetLevelInfo(this->getRace(),this->getClass(),1);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, lvlinfo->HP);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, lvlinfo->HP - (lvlinfo->Stat[2]-levelone->Stat[2])*10);
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, lvlinfo->XPToNextLevel);
	
	
	if(GetPowerType() == POWER_TYPE_MANA)
	{
		SetUInt32Value(UNIT_FIELD_BASE_MANA, lvlinfo->Mana - (lvlinfo->Stat[3]-levelone->Stat[3])*15);
		SetUInt32Value(UNIT_FIELD_MAXPOWER1, lvlinfo->Mana);
	}
}

void Player::CompleteLoading()
{	
    // cast passive initial spells	  -- grep note: these shouldnt require plyr to be in world
	SpellSet::iterator itr;
	SpellEntry *info;
	SpellCastTargets targets;
	targets.m_unitTarget = this->GetGUID();
	targets.m_targetMask = 0x2;

	// warrior has to have battle stance
	if( getClass() == WARRIOR )
	{
		// battle stance passive
		CastSpell(this, dbcSpell.LookupEntry(2457), true);
	}


	for (itr = mSpells.begin(); itr != mSpells.end(); ++itr)
	{
		info = dbcSpell.LookupEntry(*itr);

		if(	info  
			&& (info->Attributes & ATTRIBUTES_PASSIVE)  // passive
			&& !( info->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET ) //on pet summon talents
			 ) 
		{
			if( info->RequiredShapeShift )
			{
				if( !( ((uint32)1 << (GetShapeShift()-1)) & info->RequiredShapeShift ) )
					continue;
			}

			Spell * spell=new Spell(this,info,true,NULL);
			spell->prepare(&targets);
		}
	}

   
	std::list<LoginAura>::iterator i =  loginauras.begin();

    for(;i!=loginauras.end(); i++)
	{

		//check if we already have this aura
//		if(this->HasActiveAura((*i).id))
//			continue;
		//how many times do we intend to put this oura on us
/*		uint32 count_appearence=0;
		std::list<LoginAura>::iterator i2 =  i;
		for(;i2!=loginauras.end();i2++)
			if((*i).id==(*i2).id)
			{
				count_appearence++;
			}
*/

		// this stuff REALLY needs to be fixed - Burlex
		SpellEntry * sp = dbcSpell.LookupEntry((*i).id);

		if ( sp->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
			continue; //do not load auras that only exist while pet exist. We should recast these when pet is created anyway

		// bad auras!
		if( sp->Attributes & ATTRIBUTES_PASSIVE || (sp->Attributes & ATTRIBUTES_PASSIVE && sp->AttributesEx & 1024) )
			continue;

		Aura * a;
		if(sp->Id == 8326 || sp->Id == 9036 || sp->Id == 20584 || sp->Id == 15007)		// death auras
		{
			if(!isDead())
				continue;

			a = new Aura(sp,(*i).dur,this,this);
			a->SetNegative();
		}
		else
		{
			a = new Aura(sp,(*i).dur,this,this);
		}
		

		for(uint32 x =0;x<3;x++)
        {
		    if(sp->Effect[x]==SPELL_EFFECT_APPLY_AURA)
		    {
			    a->AddMod(sp->EffectApplyAuraName[x],sp->EffectBasePoints[x]+1,sp->EffectMiscValue[x],x);
		    }
        }


		this->AddAura(a, NULL);		//FIXME: must save amt,pos/neg
		//Somehow we should restore number of appearence. Right now i have no idea how :(
//		if(count_appearence>1)
//			this->AddAuraVisual((*i).id,count_appearence-1,a->IsPositive());
	}
	loginauras.clear();

	// this needs to be after the cast of passive spells, because it will cast ghost form, after the remove making it in ghost alive, if no corpse.

	if(iActivePet)
		SpawnPet(iActivePet);	   // only spawn if >0

	// Banned
	if(IsBanned())
	{
		Kick(10000);
		BroadcastMessage("This character is not allowed to play.");
		BroadcastMessage("You have been banned for: %s", GetBanReason().c_str());
	}

	if(m_playerInfo->m_Group)
	{
		sEventMgr.AddEvent(this, &Player::EventGroupFullUpdate, EVENT_UNK, 100, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		//m_playerInfo->m_Group->Update();
	}

	if(raidgrouponlysent)
	{
		WorldPacket data2(SMSG_RAID_GROUP_ONLY, 8);
		data2 << uint32(0xFFFFFFFF) << uint32(0);
		GetSession()->SendPacket(&data2);
		raidgrouponlysent=false;
	}

	sInstanceMgr.BuildSavedInstancesForPlayer(this);
}

void Player::OnWorldPortAck()
{
	//only rezz if player is porting to a instance portal
	MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(GetMapId());
	if(isDead())
	{
		if(pMapinfo)
		{
			if(pMapinfo->type != INSTANCE_NULL && pMapinfo->type != INSTANCE_PVP)
				ResurrectPlayer(NULL);
		}
	}

	if(pMapinfo)
	{
		WorldPacket data(4);
		if(pMapinfo->HasFlag(WMI_INSTANCE_WELCOME) && GetMapMgr())
		{
			std::string welcome_msg;
			welcome_msg = "Welcome to ";
			welcome_msg += pMapinfo->name;
			welcome_msg += ". ";
            if(pMapinfo->type != INSTANCE_NONRAID && m_mapMgr->pInstance)
			{
				/*welcome_msg += "This instance is scheduled to reset on ";
				welcome_msg += asctime(localtime(&m_mapMgr->pInstance->m_expiration));*/
				welcome_msg += "This instance is scheduled to reset on ";
				welcome_msg += ConvertTimeStampToDataTime((uint32)m_mapMgr->pInstance->m_expiration);
			}
			sChatHandler.SystemMessage(m_session, welcome_msg.c_str());
		}
	}

	ResetHeartbeatCoords();
}

void Player::ModifyBonuses(uint32 type,int32 val)
{
	// Added some updateXXXX calls so when an item modifies a stat they get updated
	// also since this is used by auras now it will handle it for those
	switch (type) 
		{
		case POWER:
			ModUnsigned32Value( UNIT_FIELD_MAXPOWER1, val );
			m_manafromitems += val;
			break;
		case HEALTH:
			ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, val );
			m_healthfromitems += val;
			break;
		case AGILITY: // modify agility				
			FlatStatModPos[1] += val;
			CalcStat( 1 );
			break;
		case STRENGTH: //modify strength
			FlatStatModPos[0] += val;
			CalcStat( 0 );
			break;
		case INTELLECT: //modify intellect 
			FlatStatModPos[3] += val;
			CalcStat( 3 );
			break;
		 case SPIRIT: //modify spirit
			FlatStatModPos[4] += val;
			CalcStat( 4 );
			break;
		case STAMINA: //modify stamina
			FlatStatModPos[2] += val;
			CalcStat( 2 );
			break;
		case WEAPON_SKILL_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, val ); // ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, val ); // melee main hand
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, val ); // melee off hand
			}break;
		case DEFENSE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DEFENCE, val );
			}break;
		case DODGE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DODGE, val );
			}break;
		case PARRY_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_PARRY, val );
			}break;
		case SHIELD_BLOCK_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_BLOCK, val );
			}break;
		case MELEE_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );
			}break;
		case RANGED_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );
			}break;
		case SPELL_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;
		case MELEE_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );
			}break;
		case RANGED_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );
			}break;
		case SPELL_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;
		case MELEE_HIT_AVOIDANCE_RATING:
			{
				
			}break;
		case RANGED_HIT_AVOIDANCE_RATING:
			{

			}break;
		case SPELL_HIT_AVOIDANCE_RATING:
			{

			}break;
		case MELEE_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case RANGED_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case SPELL_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case MELEE_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
			}break;
		case RANGED_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
			}break;
		case SPELL_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );//spell
			}break;
		case HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );//ranged
				// maybe should do spell here? (7)
			}break;
		case CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );//ranged
				// maybe should do spell here? (10)
			}break;
		case HIT_AVOIDANCE_RATING:// this is guessed based on layout of other fields
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );//spell
			}break;
		case EXPERTISE_RATING:
		case EXPERTISE_RATING_2:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_EXPERTISE, val );
				ModUnsigned32Value( PLAYER_EXPERTISE, val );
			}break;
		case RESILIENCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE, val );//spell
			}break;
		case HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
				// maybe should do spell here? (19)
			}break;
		}
}

bool Player::CanSignCharter(Charter * charter, Player * requester)
{
	if(charter->CharterType >= CHARTER_TYPE_ARENA_2V2 && m_playerInfo->arenaTeam[charter->CharterType-1] != NULL)
		return false;
	
	if(charter->CharterType == CHARTER_TYPE_GUILD && IsInGuild())
		return false;

	if(m_playerInfo->charterId[charter->CharterType] != 0 || requester->GetTeam() != GetTeam())
		return false;
	else
		return true;
}

void Player::SaveAuras(stringstream &ss)
{
   
	// Add player auras
//	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			Aura *aur=m_auras[x];
			bool skip = false;
			for(uint32 i = 0; i < 3; ++i)
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT)
				{
					skip = true;
					break;
				}
			}

			if( aur->pSpellId )
				skip = true; //these auras were gained due to some proc. We do not save these eighter to avoid exploits of not removing them

			if ( aur->m_spellProto->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
				skip = true;

			// skipped spells due to bugs
			switch(aur->m_spellProto->Id)
			{
			case 12043: // Presence of mind
			case 11129: // Combustion
			case 28682: // Combustion proc
			case 16188: // Natures Swiftness
			case 17116: // Natures Swiftness
			case 34936: // Backlash
			case 35076: // Blessing of A'dal
			case 23333:	// WSG
			case 23335:	// WSG
			case 45438:	// Ice Block
			case 642:
			case 1020:	//divine shield
			case 32724:
			case 32725:
			case 32727:
			case 32071:		// Hellfire Superority
			case 32049:		// Hellfire Superority
			case 44521:		// Preparation?

				skip = true;
				break;
			}

			//disabled proc spells until proper loading is fixed. Some spells tend to block or not remove when restored
			if(aur->GetSpellProto()->procFlags)
			{
//				DEBUG_LOG("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
				skip = true;
			}
			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
//				DEBUG_LOG("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
				skip = true;
			}
			//we are going to cast passive spells anyway on login so no need to save auras for them
            if(aur->IsPassive())
				skip = true;
			//shapeshift
//			if(m_ShapeShifted==aur->m_spellProto->Id)
//				skip=true;

			if(skip)continue;
			uint32 d=aur->GetTimeLeft();
			if(d>3000)
				ss  << aur->GetSpellId() << "," << d << ",";
		}
	}

  
}

void Player::SetShapeShift(uint8 ss)
{
	uint8 old_ss = GetByte( UNIT_FIELD_BYTES_2, 3 );
	SetByte( UNIT_FIELD_BYTES_2, 3, ss );

	//remove auras that we should not have
	for( uint32 x = 0; x < MAX_AURAS + MAX_PASSIVE_AURAS; x++ )
	{
		if( m_auras[x] != NULL )
		{
			uint32 reqss = m_auras[x]->GetSpellProto()->RequiredShapeShift;
			if( reqss != 0 && m_auras[x]->IsPositive() )
			{
				if( old_ss > 0 )
				{
					if(  ( ((uint32)1 << (old_ss-1)) & reqss ) &&		// we were in the form that required it
						!( ((uint32)1 << (ss-1) & reqss) ) )			// new form doesnt have the right form
					{
						m_auras[x]->Remove();
						continue;
					}
				}
			}

			if( this->getClass() == DRUID )
			{
				for (uint32 y = 0; y < 3; ++y )
				{
					switch( m_auras[x]->GetSpellProto()->EffectApplyAuraName[y])
					{
					case SPELL_AURA_MOD_ROOT: //Root
					case SPELL_AURA_MOD_DECREASE_SPEED: //Movement speed
					case SPELL_AURA_MOD_CONFUSE:  //Confuse (polymorph)
						{
							m_auras[x]->Remove();
						}
						break;
					default:
						break;
					}

					if( m_auras[x] == NULL )
						break;
				}
			}
		} 
	}

	// apply any talents/spells we have that apply only in this form.
	set<uint32>::iterator itr;
	SpellEntry * sp;
	Spell * spe;
	SpellCastTargets t(GetGUID());

	for( itr = mSpells.begin(); itr != mSpells.end(); ++itr )
	{
		sp = dbcSpell.LookupEntry( *itr );
		if( sp->apply_on_shapeshift_change || sp->Attributes & 64 )		// passive/talent
		{
			if( sp->RequiredShapeShift && ((uint32)1 << (ss-1)) & sp->RequiredShapeShift )
			{
				spe = new Spell( this, sp, true, NULL );
				spe->prepare( &t );
			}
		}
	}

	// now dummy-handler stupid hacky fixed shapeshift spells (leader of the pack, etc)
	for( itr = mShapeShiftSpells.begin(); itr != mShapeShiftSpells.end(); )
	{
		sp = dbcSpell.LookupEntry( *itr );
		++itr;

		if( sp->RequiredShapeShift && ((uint32)1 << (ss-1)) & sp->RequiredShapeShift )
		{
			spe = new Spell( this, sp, true, NULL );
			spe->prepare( &t );
		}
	}

	// kill speedhack detection for 2 seconds (not needed with new code but bleh)
	DelaySpeedHack( 2000 );

	UpdateStats();
	UpdateChances();
}

void Player::CalcDamage()
{
	float delta;
	float r;
	int ss = GetShapeShift();
/////////////////MAIN HAND
		float ap_bonus = GetAP()/14000.0f;
		delta = (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS ) - (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG );

		if(IsInFeralForm())
		{
			uint32 lev = getLevel();
			/*if(ss==FORM_CAT)
				r = delta + ap_bonus * 1000.0;
			else
				r = delta + ap_bonus * 2500.0;*/

			if(ss == FORM_CAT)
				r = lev + delta + ap_bonus * 1000.0f;
			else
				r = lev + delta + ap_bonus * 2500.0f;
			
			//SetFloatValue(UNIT_FIELD_MINDAMAGE,r);
			//SetFloatValue(UNIT_FIELD_MAXDAMAGE,r);

			r *= 0.9f;
			r *= 1.1f;

			SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);
			SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);

			return;
		}
//////no druid ss	
		uint32 speed=2000;
		Item *it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
		
		if(!disarmed)
		{	
			if(it)
				speed = it->GetProto()->Delay;
		}
		
		float bonus=ap_bonus*speed;
		float tmp = 1;
		map<uint32, WeaponModifier>::iterator i;
		for(i = damagedone.begin();i!=damagedone.end();i++)
		{
			if((i->second.wclass == (uint32)-1) || //any weapon
				(it && ((1 << it->GetProto()->SubClass) & i->second.subclass) )
				)
					tmp+=i->second.value/100.0f;
		}
		
		r = BaseDamage[0]+delta+bonus;
		r *= tmp;
		SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);
	
		r = BaseDamage[1]+delta+bonus;
		r *= tmp;
		SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);

		uint32 cr = 0;
		if( it )
		{
			if( static_cast< Player* >( this )->m_wratings.size() )
			{
				std::map<uint32, uint32>::iterator itr = m_wratings.find( it->GetProto()->SubClass );
				if( itr != m_wratings.end() )
					cr=itr->second;
			}
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, cr );
		/////////////// MAIN HAND END

		/////////////// OFF HAND START
		cr = 0;
		it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
		if(it)
		{
			if(!disarmed)
			{
				speed =it->GetProto()->Delay;
			}
			else speed  = 2000;
			
			bonus = ap_bonus * speed;
			i = damagedone.begin();
			tmp = 1;
			for(;i!=damagedone.end();i++)
			{
				if((i->second.wclass==(uint32)-1) || //any weapon
					(( (1 << it->GetProto()->SubClass) & i->second.subclass)  )
					)
					tmp+=i->second.value/100.0f;
			}

			r = (BaseOffhandDamage[0]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE,r>0?r:0);
			r = (BaseOffhandDamage[1]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE,r>0?r:0);
			if(m_wratings.size ())
			{
				std::map<uint32, uint32>::iterator itr=m_wratings.find(it->GetProto()->SubClass);
				if(itr!=m_wratings.end())
					cr=itr->second;
			}
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, cr );

/////////////second hand end
///////////////////////////RANGED
		cr=0;
		if((it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)))
		{
			i = damagedone.begin();
			tmp = 1;
			for(;i != damagedone.end();i++)
			{
				if( 
					(i->second.wclass == (uint32)-1) || //any weapon
					( ((1 << it->GetProto()->SubClass) & i->second.subclass)  )
					)
				{
					tmp+=i->second.value/100.0f;
				}
			}

			if(it->GetProto()->SubClass != 19)//wands do not have bonuses from RAP & ammo
			{
//				ap_bonus = (GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER)+(int32)GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS))/14000.0;
				//modified by Zack : please try to use premade functions if possible to avoid forgetting stuff
				ap_bonus = GetRAP()/14000.0f;
				bonus = ap_bonus*it->GetProto()->Delay;
				
				if(GetUInt32Value(PLAYER_AMMO_ID))
				{
					ItemPrototype * xproto=ItemPrototypeStorage.LookupEntry(GetUInt32Value(PLAYER_AMMO_ID));
					if(xproto)
					{
						bonus+=((xproto->Damage[0].Min+xproto->Damage[0].Max)*it->GetProto()->Delay)/2000.0f;
					}
				}
			}else bonus =0;
			
			r = BaseRangedDamage[0]+delta+bonus;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,r>0?r:0);

			r = BaseRangedDamage[1]+delta+bonus;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,r>0?r:0);
			
		
			if(m_wratings.size ())
			{
				std::map<uint32, uint32>::iterator i=m_wratings.find(it->GetProto()->SubClass);
				if(i != m_wratings.end())
					cr=i->second;
			}
		
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, cr );

/////////////////////////////////RANGED end

		//tmp = 1;
		tmp = 0;
		for(i = damagedone.begin();i != damagedone.end();i++)
		if(i->second.wclass==(uint32)-1)  //any weapon
			tmp += i->second.value/100.0f;
		
		//display only modifiers for any weapon

		// OMG?
		ModFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT ,tmp);
 
}

uint32 Player::GetMainMeleeDamage(uint32 AP_owerride)
{
	float min_dmg,max_dmg;
	float delta;
	float r;
	int ss = GetShapeShift();
/////////////////MAIN HAND
	float ap_bonus;
	if(AP_owerride) 
		ap_bonus = AP_owerride/14000.0f;
	else 
		ap_bonus = GetAP()/14000.0f;
	delta = (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS ) - (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG );
	if(IsInFeralForm())
	{
		uint32 lev = getLevel();
		if(ss == FORM_CAT)
			r = lev + delta + ap_bonus * 1000.0f;
		else
			r = lev + delta + ap_bonus * 2500.0f;
		min_dmg = r * 0.9f;
		max_dmg = r * 1.1f;
		return float2int32(std::max((min_dmg + max_dmg)/2.0f,0.0f));
	}
//////no druid ss	
	uint32 speed=2000;
	Item *it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	if(!disarmed)
	{	
		if(it)
			speed = it->GetProto()->Delay;
	}
	float bonus=ap_bonus*speed;
	float tmp = 1;
	map<uint32, WeaponModifier>::iterator i;
	for(i = damagedone.begin();i!=damagedone.end();i++)
	{
		if((i->second.wclass == (uint32)-1) || //any weapon
			(it && ((1 << it->GetProto()->SubClass) & i->second.subclass) )
			)
				tmp+=i->second.value/100.0f;
	}
	
	r = BaseDamage[0]+delta+bonus;
	r *= tmp;
	min_dmg = r * 0.9f;
	r = BaseDamage[1]+delta+bonus;
	r *= tmp;
	max_dmg = r * 1.1f;

	return float2int32(std::max((min_dmg + max_dmg)/2.0f,0.0f));
}

void Player::EventPortToGM(uint32 guid)
{
	if( !IsInWorld() )
		return;

	Player *p = objmgr.GetPlayer(guid);
	if( p == NULL )
		return;

	SafeTeleport(p->GetMapId(),p->GetInstanceID(),p->GetPosition());
}

void Player::UpdateComboPoints()
{
	// fuck bytebuffers :D
	unsigned char buffer[10];
	uint32 c = 2;

	// check for overflow
	if(m_comboPoints > 5)
		m_comboPoints = 5;
	
	if(m_comboPoints < 0)
		m_comboPoints = 0;

	if(m_comboTarget != 0)
	{
		Unit * target = (m_mapMgr != NULL) ? m_mapMgr->GetUnit(m_comboTarget) : NULL;
		if(!target || target->isDead() || GetSelection() != m_comboTarget)
		{
			buffer[0] = buffer[1] = 0;
		}
		else
		{
			c = FastGUIDPack(m_comboTarget, buffer, 0);
			buffer[c++] = m_comboPoints;
		}
	}
	else
		buffer[0] = buffer[1] = 0;

	m_session->OutPacket(SMSG_UPDATE_COMBO_POINTS, c, buffer);
}

void Player::SendAreaTriggerMessage(const char * message, ...)
{
	va_list ap;
	va_start(ap, message);
	char msg[500];
	vsnprintf(msg, 500, message, ap);
	va_end(ap);

	WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 6 + strlen(msg));
	data << (uint32)0 << msg << (uint8)0x00;
	m_session->SendPacket(&data);
}

void Player::removeSoulStone()
{
	if(!this->SoulStone) return;
	uint32 sSoulStone = 0;
	switch(this->SoulStone)
	{
	case 3026:
		{
			sSoulStone = 20707;
		}break;
	case 20758:
		{
			sSoulStone = 20762;
		}break;
	case 20759:
		{
			sSoulStone = 20763;
		}break;
	case 20760:
		{
			sSoulStone = 20764;
		}break;
	case 20761:
		{
			sSoulStone = 20765;
		}break;
	case 27240:
		{
			sSoulStone = 27239;
		}break;
	}
	this->RemoveAura(sSoulStone);
	this->SoulStone = this->SoulStoneReceiver = 0; //just incase
}

void Player::SoftDisconnect()
{/*
      sEventMgr.RemoveEvents(this, EVENT_PLAYER_SOFT_DISCONNECT);
	  WorldSession *session=GetSession();
      session->LogoutPlayer(true);
	  session->Disconnect();*/

	// NOOB NOOB BIG BIG NOOB NOOB
	m_session->Disconnect();
}

void Player::SetNoseLevel()
{
	// Set the height of the player
	switch (getRace())
	{
		case RACE_HUMAN:
		// female
			if (getGender()) m_noseLevel = 1.72f;
			// male
			else m_noseLevel = 1.78f;
		break;
		case RACE_ORC:
			if (getGender()) m_noseLevel = 1.82f;
			else m_noseLevel = 1.98f;
		break;
		case RACE_DWARF:
		if (getGender()) m_noseLevel = 1.27f;
			else m_noseLevel = 1.4f;
		break;
		case RACE_NIGHTELF:
			if (getGender()) m_noseLevel = 1.84f;
			else m_noseLevel = 2.13f;
		break;
		case RACE_UNDEAD:
			if (getGender()) m_noseLevel = 1.61f;
			else m_noseLevel = 1.8f;
		break;
		case RACE_TAUREN:
			if (getGender()) m_noseLevel = 2.48f;
			else m_noseLevel = 2.01f;
		break;
		case RACE_GNOME:
			if (getGender()) m_noseLevel = 1.06f;
			else m_noseLevel = 1.04f;
		break;
		case RACE_TROLL:
			if (getGender()) m_noseLevel = 2.02f;
			else m_noseLevel = 1.93f;
		break;
		case RACE_BLOODELF:
			if (getGender()) m_noseLevel = 1.83f;
			else m_noseLevel = 1.93f;
		break;
		case RACE_DRAENEI:
			if (getGender()) m_noseLevel = 2.09f;
			else m_noseLevel = 2.36f;
		break;
	}
}

void Player::Possess(Unit * pTarget)
{
	if(m_Summon || m_CurrentCharm)
		return;

	ResetHeartbeatCoords();
	if( pTarget->IsPlayer() )
		TO_PLAYER(pTarget)->ResetHeartbeatCoords();

	m_CurrentCharm = pTarget;
	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->setAItoUse(false);
		pTarget->GetAIInterface()->StopMovement(0);
		pTarget->m_redirectSpellPackets = this;
	}

	m_noInterrupt++;
	SetUInt64Value(UNIT_FIELD_CHARM, pTarget->GetGUID());
	SetUInt64Value(PLAYER_FARSIGHT, pTarget->GetGUID());

	pTarget->SetUInt64Value(UNIT_FIELD_CHARMEDBY, GetGUID());
	pTarget->SetCharmTempVal(pTarget->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
	
	/* send "switch mover" packet */
	WorldPacket data1(SMSG_CLIENT_CONTROL_UPDATE, 10);		/* burlex: this should be renamed SMSG_SWITCH_ACTIVE_MOVER :P */
	data1 << pTarget->GetNewGUID() << uint8(1);
	m_session->SendPacket(&data1);

	/* update target faction set */
	pTarget->_setFaction();
	pTarget->UpdateOppFactionSet();

	list<uint32> avail_spells;
	for(list<AI_Spell*>::iterator itr = pTarget->GetAIInterface()->m_spells.begin(); itr != pTarget->GetAIInterface()->m_spells.end(); ++itr)
	{
		if((*itr)->agent == AGENT_SPELL)
			avail_spells.push_back((*itr)->spell->Id);
	}
	list<uint32>::iterator itr = avail_spells.begin();

	/* build + send pet_spells packet */
	if(pTarget->m_temp_summon)
		return;
	
	WorldPacket data(SMSG_PET_SPELLS, pTarget->GetAIInterface()->m_spells.size() * 4 + 20);
	data << pTarget->GetGUID();
	data << uint32(0x00000000);//unk1
	data << uint32(0x00000101);//unk2

	// First spell is attack.
	data << uint32(PET_SPELL_ATTACK);

	// Send the actionbar
	for(uint32 i = 1; i < 10; ++i)
	{
		if(itr != avail_spells.end())
		{
			data << uint16((*itr)) << uint16(DEFAULT_SPELL_STATE);
			++itr;
		}
		else
			data << uint16(0) << uint8(0) << uint8(i+5);
	}

	// Send the rest of the spells.
	data << uint8(avail_spells.size());
	for(itr = avail_spells.begin(); itr != avail_spells.end(); ++itr)
		data << uint16(*itr) << uint16(DEFAULT_SPELL_STATE);
	
	data << uint64(0);
	m_session->SendPacket(&data);
}

void Player::UnPossess()
{
	if(m_Summon || !m_CurrentCharm)
		return;

	Unit * pTarget = m_CurrentCharm; 
	m_CurrentCharm = 0;

	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->setAItoUse(true);
		pTarget->m_redirectSpellPackets = 0;
	}

	ResetHeartbeatCoords();
	DelaySpeedHack(5000);
	if( pTarget->GetTypeId() == TYPEID_PLAYER )
		TO_PLAYER(pTarget)->DelaySpeedHack(5000);

	m_noInterrupt--;
	SetUInt64Value(PLAYER_FARSIGHT, 0);
	SetUInt64Value(UNIT_FIELD_CHARM, 0);
	pTarget->SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
	pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);
	pTarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pTarget->GetCharmTempVal());
	pTarget->_setFaction();
	pTarget->UpdateOppFactionSet();

	/* send "switch mover" packet */
	WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE, 10);
	data << GetNewGUID() << uint8(1);
	m_session->SendPacket(&data);

	if(pTarget->m_temp_summon)
		return;

	data.Initialize(SMSG_PET_SPELLS);
	data << uint64(0);
	m_session->SendPacket(&data);
}

//what is an Immobilize spell ? Have to add it later to spell effect handler
void Player::EventStunOrImmobilize(Unit *proc_target)
{
	if(trigger_on_stun)
	{
		if(trigger_on_stun_chance<100 && !Rand(trigger_on_stun_chance))
			return;
		SpellEntry *spellInfo = dbcSpell.LookupEntry(trigger_on_stun);
		if(!spellInfo)
			return;

		if(trigger_on_stun == 29838 || trigger_on_stun == 29834) // Second Wind
		{
			if(HasAura(trigger_on_stun))
				return;
		}

		Spell *spell = new Spell(this, spellInfo ,true, NULL);
		SpellCastTargets targets;

		if(proc_target)
			targets.m_unitTarget = proc_target->GetGUID();
		else targets.m_unitTarget = GetGUID();
		spell->prepare(&targets);
	}
}

void Player::SummonRequest(uint32 Requestor, uint32 ZoneID, uint32 MapID, uint32 InstanceID, const LocationVector & Position)
{
	m_summonInstanceId = InstanceID;
	m_summonPos = Position;
	m_summoner = Requestor;
	m_summonMapId = MapID;

	WorldPacket data(SMSG_SUMMON_REQUEST, 16);
	data << uint64(Requestor) << ZoneID << uint32(120000);		// 2 minutes
	m_session->SendPacket(&data);
}

void Player::RemoveFromBattlegroundQueue()
{
	if(!m_pendingBattleground)
		return;

	m_pendingBattleground->RemovePendingPlayer(this);
	sChatHandler.SystemMessage(m_session, "You were removed from the queue for the battleground for not joining after 2 minutes.");
	m_pendingBattleground = 0;
}

#ifdef CLUSTERING
void Player::EventRemoveAndDelete()
{

}
#endif

void Player::_AddSkillLine(uint32 SkillLine, uint32 Current, uint32 Max)
{
	ItemProf * prof;
	SkillMap::iterator itr = m_skills.find(SkillLine);

	if( !Current )
		Current = 1;

	if(itr != m_skills.end())
	{
		if( (Current > itr->second.CurrentValue && Max >= itr->second.MaximumValue) ||
			(Current == itr->second.CurrentValue && Max > itr->second.MaximumValue) )
		{
			itr->second.CurrentValue = Current;
			itr->second.MaximumValue = Max;

			if(itr->second.CurrentValue>itr->second.MaximumValue)
				itr->second.CurrentValue=itr->second.MaximumValue;
			_UpdateSkillFields();
		}
	}
	else
	{
		PlayerSkill inf;
		inf.Skill = dbcSkillLine.LookupEntry(SkillLine);
		inf.CurrentValue = Current;
		inf.MaximumValue = Max;
		inf.BonusValue = 0;
		m_skills.insert( make_pair( SkillLine, inf ) );
		_UpdateSkillFields();

		//Add to proficiency
		if((prof=(ItemProf *)GetProficiencyBySkill(SkillLine)))
		{
			packetSMSG_SET_PROFICICENCY pr;
			pr.ItemClass = prof->itemclass;
            if(prof->itemclass==4)
            {
                armor_proficiency|=prof->subclass;
                //SendSetProficiency(prof->itemclass,armor_proficiency);
				pr.Profinciency = armor_proficiency;
            }else
            {
                weapon_proficiency|=prof->subclass;
                //SendSetProficiency(prof->itemclass,weapon_proficiency);
				pr.Profinciency = weapon_proficiency;
            }
			m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof( packetSMSG_SET_PROFICICENCY ), &pr );
    	}

		// hackfix for poisons
		if(SkillLine==SKILL_POISONS && !HasSpell(2842))
			addSpell(2842);
	}
}

void Player::_UpdateSkillFields()
{
	uint32 f = PLAYER_SKILL_INFO_1_1;
	/* Set the valid skills */
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end();)
	{
		if(!itr->first)
		{
			SkillMap::iterator it2 = itr++;
			m_skills.erase(it2);
			continue;
		}

		ASSERT(f <= PLAYER_CHARACTER_POINTS1);
		if(itr->second.Skill->type == SKILL_TYPE_PROFESSION)
			SetUInt32Value(f++, itr->first | 0x10000);
		else
			SetUInt32Value(f++, itr->first);

		SetUInt32Value(f++, (itr->second.MaximumValue << 16) | itr->second.CurrentValue);
		SetUInt32Value(f++, itr->second.BonusValue);
		++itr;
	}

	/* Null out the rest of the fields */
	for(; f < PLAYER_CHARACTER_POINTS1; ++f)
	{
		if(m_uint32Values[f] != 0)
			SetUInt32Value(f, 0);
	}
}

bool Player::_HasSkillLine(uint32 SkillLine)
{
	return (m_skills.find(SkillLine) != m_skills.end());
}

void Player::_AdvanceSkillLine(uint32 SkillLine, uint32 Count /* = 1 */)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
	{
		/* Add it */
		_AddSkillLine(SkillLine, Count, getLevel() * 5);
	}
	else
	{
		/* Update it. */
		if(itr->second.CurrentValue >= itr->second.MaximumValue)
			return;

		itr->second.CurrentValue += Count;
		if(itr->second.CurrentValue >= itr->second.MaximumValue)
			itr->second.CurrentValue = itr->second.MaximumValue;
	}

	_UpdateSkillFields();
}

uint32 Player::_GetSkillLineMax(uint32 SkillLine)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	return (itr == m_skills.end()) ? 0 : itr->second.MaximumValue;
}

uint32 Player::_GetSkillLineCurrent(uint32 SkillLine, bool IncludeBonus /* = true */)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return 0;

	return (IncludeBonus ? itr->second.CurrentValue + itr->second.BonusValue : itr->second.CurrentValue);
}

void Player::_RemoveSkillLine(uint32 SkillLine)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	m_skills.erase(itr);
	_UpdateSkillFields();
}

void Player::_UpdateMaxSkillCounts()
{
	bool dirty = false;
	uint32 new_max = getLevel() * 5;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.Skill->type != SKILL_TYPE_WEAPON &&
			itr->second.Skill->id != SKILL_POISONS &&
			itr->second.Skill->id != SKILL_LOCKPICKING)
		{
			continue;
		}

        if(itr->second.MaximumValue != new_max)
		{
			dirty = true;
			itr->second.MaximumValue = new_max;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

void Player::_ModifySkillBonus(uint32 SkillLine, int32 Delta)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	itr->second.BonusValue += Delta;
	_UpdateSkillFields();
}

void Player::_ModifySkillBonusByType(uint32 SkillType, int32 Delta)
{
	bool dirty = false;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.Skill->type == SkillType)
		{
			itr->second.BonusValue += Delta;
			dirty=true;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

/** Maybe this formula needs to be checked?
 * - Burlex
 */

float PlayerSkill::GetSkillUpChance()
{
	float diff = float(MaximumValue - CurrentValue);
	return (diff * 100.0f / float(MaximumValue)) / 3.0f;
}

void Player::_RemoveLanguages()
{
	for(SkillMap::iterator itr = m_skills.begin(), it2; itr != m_skills.end();)
	{
		if(itr->second.Skill->type == SKILL_TYPE_LANGUAGE)
		{
			it2 = itr++;
			m_skills.erase(it2);
		}
		else
			++itr;
	}
}

void PlayerSkill::Reset(uint32 Id)
{
	MaximumValue = 0;
	CurrentValue = 0;
	BonusValue = 0;
	Skill = (Id == 0) ? NULL : dbcSkillLine.LookupEntry(Id);
}

void Player::_AddLanguages(bool All)
{
	/** This function should only be used at login, and after _RemoveLanguages is called.
	 * Otherwise weird stuff could happen :P
	 * - Burlex
	 */

	PlayerSkill sk;
	skilllineentry * en;
	uint32 spell_id;
	static uint32 skills[] = { SKILL_LANG_COMMON, SKILL_LANG_ORCISH, SKILL_LANG_DWARVEN, SKILL_LANG_DARNASSIAN, SKILL_LANG_TAURAHE, SKILL_LANG_THALASSIAN,
		SKILL_LANG_TROLL, SKILL_LANG_GUTTERSPEAK, SKILL_LANG_DRAENEI, 0 };
	
	if(All)
	{
		for(uint32 i = 0; skills[i] != 0; ++i)
		{
			if(!skills[i])
				break;

            sk.Reset(skills[i]);
			sk.MaximumValue = sk.CurrentValue = 300;
			m_skills.insert( make_pair(skills[i], sk) );
			if((spell_id = ::GetSpellForLanguage(skills[i])))
				addSpell(spell_id);
		}
	}
	else
	{
		for(list<CreateInfo_SkillStruct>::iterator itr = info->skills.begin(); itr != info->skills.end(); ++itr)
		{
			en = dbcSkillLine.LookupEntry(itr->skillid);
			if(en->type == SKILL_TYPE_LANGUAGE)
			{
				sk.Reset(itr->skillid);
				sk.MaximumValue = sk.CurrentValue = 300;
				m_skills.insert( make_pair(itr->skillid, sk) );
				if((spell_id = ::GetSpellForLanguage(itr->skillid)))
					addSpell(spell_id);
			}
		}
	}

	_UpdateSkillFields();
}

float Player::GetSkillUpChance(uint32 id)
{
	SkillMap::iterator itr = m_skills.find(id);
	if(itr == m_skills.end())
		return 0.0f;

	return itr->second.GetSkillUpChance();
}

void Player::_RemoveAllSkills()
{
	m_skills.clear();
	_UpdateSkillFields();
}

void Player::_AdvanceAllSkills(uint32 count)
{
	bool dirty=false;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.CurrentValue != itr->second.MaximumValue)
		{
			itr->second.CurrentValue += count;
			if(itr->second.CurrentValue >= itr->second.MaximumValue)
				itr->second.CurrentValue = itr->second.MaximumValue;
			dirty=true;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

void Player::_ModifySkillMaximum(uint32 SkillLine, uint32 NewMax)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	if(NewMax > itr->second.MaximumValue)
	{
		if(SkillLine == SKILL_RIDING)
			itr->second.CurrentValue = NewMax;

		itr->second.MaximumValue = NewMax;
		_UpdateSkillFields();
	}
}

void Player::RemoveSpellTargets(uint32 Type)
{
	if(m_spellIndexTypeTargets[Type] != 0)
	{
		Unit * pUnit = m_mapMgr ? m_mapMgr->GetUnit(m_spellIndexTypeTargets[Type]) : NULL;
		if(pUnit)
            pUnit->RemoveAurasByBuffIndexType(Type, GetGUID());

		m_spellIndexTypeTargets[Type] = 0;
	}
}

void Player::RemoveSpellIndexReferences(uint32 Type)
{
	m_spellIndexTypeTargets[Type] = 0;
}

void Player::SetSpellTargetType(uint32 Type, Unit* target)
{
	m_spellIndexTypeTargets[Type] = target->GetGUID();
}

void Player::RecalculateHonor()
{
	HonorHandler::RecalculateHonorFields(this);
}

//wooot, crapy code rulez.....NOT
void Player::EventTalentHearthOfWildChange(bool apply)
{
	if(!hearth_of_wild_pct)
		return;

	//druid hearth of the wild should add more features based on form
	int tval;
	if(apply)
		tval = hearth_of_wild_pct;
	else tval = -hearth_of_wild_pct;

	uint32 SS=GetShapeShift();

	//increase stamina if :
	if(SS==FORM_BEAR || SS==FORM_DIREBEAR)
	{
		TotalStatModPctPos[STAT_STAMINA] += tval; 
		CalcStat(STAT_STAMINA);	
		UpdateStats();
		UpdateChances();
	}
	//increase attackpower if :
	else if(SS==FORM_CAT)
	{
		SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER)+float(tval/200.0f));
		SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER)+float(tval/200.0f));
		UpdateStats();
		UpdateChances();
	}
}

/************************************************************************/
/* New Save System                                                      */
/************************************************************************/
#ifdef OPTIMIZED_PLAYER_SAVING

void Player::save_LevelXP()
{
	CharacterDatabase.Execute("UPDATE characters SET level = %u, xp = %u WHERE guid = %u", m_uint32Values[UNIT_FIELD_LEVEL], m_uint32Values[PLAYER_XP], m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_PositionHP()
{
	CharacterDatabase.Execute("UPDATE characters SET current_hp = %u, current_power = %u, positionX = '%f', positionY = '%f', positionZ = '%f', orientation = '%f', mapId = %u, instance_id = %u WHERE guid = %u",
		m_uint32Values[UNIT_FIELD_HEALTH], m_uint32Values[UNIT_FIELD_POWER1+GetPowerType()], m_position.x, m_position.y, m_position.z, m_position.o, m_mapId, m_instanceId, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Gold()
{
	CharacterDatabase.Execute("UPDATE characters SET gold = %u WHERE guid = %u", m_uint32Values[PLAYER_FIELD_COINAGE], m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_GuildData()
{
	if(myGuild)
	{
		string escaped_note = m_playerInfo->publicNote ? CharacterDatabase.EscapeString(m_playerInfo->publicNote) : "";
		string escaped_note2 = m_playerInfo->officerNote ?  CharacterDatabase.EscapeString(m_playerInfo->officerNote) : "";
		CharacterDatabase.Execute("UPDATE characters SET guildid=%u, guildRank=%u, publicNote='%s', officerNote='%s' WHERE guid = %u",
			GetGuildId(), GetGuildRank(), escaped_note.c_str(), escaped_note2.c_str(), m_uint32Values[OBJECT_FIELD_GUID]);
	}
	else
	{
		CharacterDatabase.Execute("UPDATE characters SET guildid = 0, guildRank = 0, publicNote = '', officerNote = '' WHERE guid = %u",
			m_uint32Values[OBJECT_FIELD_GUID]);
	}
}

void Player::save_ExploreData()
{
	char buffer[2048] = {0};
	int p = 0;
	for(uint32 i = 0; i < 64; ++i)
	{
		p += sprintf(&buffer[p], "%u,", m_uint32Values[PLAYER_EXPLORED_ZONES_1 + i]);
	}

	ASSERT(p < 2048);
	CharacterDatabase.Execute("UPDATE characters SET exploration_data = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Honor()
{
	CharacterDatabase.Execute("UPDATE characters SET honorPointsToAdd = %u, killsToday = %u, killsYesterday = %u, killsLifeTime = %u, honorToday = %u, honorYesterday = %u, honorPoints = %u, arenaPoints = %u WHERE guid = %u",
		m_honorPointsToAdd, m_killsToday, m_killsYesterday, m_killsLifetime, m_honorToday, m_honorYesterday, m_honorPoints, m_arenaPoints, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Skills()
{
	char buffer[6000] = {0};
	int p = 0;

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first && itr->second.Skill->type != SKILL_TYPE_LANGUAGE)
			p += sprintf(&buffer[p], "%u;%u;%u;", itr->first, itr->second.CurrentValue, itr->second.MaximumValue);
	}

	ASSERT(p < 6000);
	CharacterDatabase.Execute("UPDATE characters SET skills = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Reputation()
{
	char buffer[10000] = {0};
	int p = 0;

	ReputationMap::iterator iter = m_reputation.begin();
	for(; iter != m_reputation.end(); ++iter)
	{
		p += sprintf(&buffer[p], "%d,%d,%d,%d,",
			iter->first, iter->second->flag, iter->second->baseStanding, iter->second->standing);
	}

	ASSERT(p < 10000);
	CharacterDatabase.Execute("UPDATE characters SET reputation = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Actions()
{
	char buffer[2048] = {0};
	int p = 0;

	for(uint32 i = 0; i < 120; ++i)
	{
		p += sprintf(&buffer[p], "%u,%u,%u,", mActions[i].Action, mActions[i].Misc, mActions[i].Type);
	}

	ASSERT(p < 2048);
	CharacterDatabase.Execute("UPDATE characters SET actions = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_BindPosition()
{
	CharacterDatabase.Execute("UPDATE characters SET bindpositionX = '%f', bindpositionY = '%f', bindpositionZ = '%f', bindmapId = %u, bindzoneId = %u WHERE guid = %u",
		m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, m_bind_mapid, m_bind_zoneid, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_EntryPoint()
{
	CharacterDatabase.Execute("UPDATE characters SET entrypointmap = %u, entrypointx = '%f', entrypointy = '%f', entrypointz = '%f', entrypointo = '%f', entrypointinstance = %u WHERE guid = %u",
		m_bgEntryPointMap, m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO, m_bgEntryPointInstance, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Taxi()
{
	char buffer[1024] = {0};
	int p = 0;
	for(uint32 i = 0; i < 8; ++i)
		p += sprintf(&buffer[p], "%u ", m_taximask[i]);

	if(m_onTaxi) {
		CharacterDatabase.Execute("UPDATE characters SET taximask = '%s', taxi_path = %u, taxi_lastnode = %u, taxi_mountid = %u WHERE guid = %u",
			buffer, m_CurrentTaxiPath->GetID(), lastNode, m_uint32Values[UNIT_FIELD_MOUNTDISPLAYID], m_uint32Values[OBJECT_FIELD_GUID]);
	}
	else
	{
		CharacterDatabase.Execute("UPDATE characters SET taximask = '%s', taxi_path = 0, taxi_lastnode = 0, taxi_mountid = 0 WHERE guid = %u",
			buffer, m_uint32Values[OBJECT_FIELD_GUID]);
	}
}

void Player::save_Zone()
{
	CharacterDatabase.Execute("UPDATE characters SET zoneId = %u WHERE guid = %u", m_zoneId, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Spells()
{
	char buffer[10000] = {0};
	char buffer2[10000] = {0};
	uint32 p=0,q=0;

	SpellSet::iterator itr = mSpells.begin();
	for(; itr != mSpells.end(); ++itr)
	{
		p += sprintf(&buffer[p], "%u,", *itr);
	}

	for(itr = mDeletedSpells.begin(); itr != mDeletedSpells.end(); ++itr)
	{
		q += sprintf(&buffer2[q], "%u,", *itr);
	}

	CharacterDatabase.Execute("UPDATE characters SET spells = '%s', deleted_spells = '%s' WHERE guid = %u",
		buffer, buffer2, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_InstanceType()
{
	CharacterDatabase.Execute("UPDATE characters SET instancetype = %u WHERE guid = %u", iInstanceType, m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Misc()
{
	uint32 playedt = UNIXTIME - m_playedtime[2];
	m_playedtime[0] += playedt;
	m_playedtime[1] += playedt;
	m_playedtime[2] += playedt;

	uint32 player_flags = m_uint32Values[PLAYER_FLAGS];
	{
		// Remove un-needed and problematic player flags from being saved :p
		if(player_flags & PLAYER_FLAG_PARTY_LEADER)
			player_flags &= ~PLAYER_FLAG_PARTY_LEADER;
		if(player_flags & PLAYER_FLAG_AFK)
			player_flags &= ~PLAYER_FLAG_AFK;
		if(player_flags & PLAYER_FLAG_DND)
			player_flags &= ~PLAYER_FLAG_DND;
		if(player_flags & PLAYER_FLAG_GM)
			player_flags &= ~PLAYER_FLAG_GM;
		if(player_flags & PLAYER_FLAG_PVP_TOGGLE)
			player_flags &= ~PLAYER_FLAG_PVP_TOGGLE;
		if(player_flags & PLAYER_FLAG_FREE_FOR_ALL_PVP)
			player_flags &= ~PLAYER_FLAG_FREE_FOR_ALL_PVP;
	}

	CharacterDatabase.Execute("UPDATE characters SET totalstableslots = %u, first_login = 0, TalentResetTimes = %u, playedTime = '%u %u %u ', isResting = %u, restState = %u, restTime = %u, timestamp = %u, watched_faction_index = %u, ammo_id = %u, available_prof_points = %u, available_talent_points = %u, bytes = %u, bytes2 = %u, player_flags = %u, player_bytes = %u",
		GetStableSlotCount(), GetTalentResetTimes(), m_playedtime[0], m_playedtime[1], playedt, uint32(m_isResting), uint32(m_restState), m_restAmount, UNIXTIME,
		m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX], m_uint32Values[PLAYER_AMMO_ID], m_uint32Values[PLAYER_CHARACTER_POINTS2],
		m_uint32Values[PLAYER_CHARACTER_POINTS1], m_uint32Values[PLAYER_BYTES], m_uint32Values[PLAYER_BYTES_2], player_flags,
		m_uint32Values[PLAYER_FIELD_BYTES],
		m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_PVP()
{
	CharacterDatabase.Execute("UPDATE characters SET pvprank = %u, selected_pvp_title = %u, available_pvp_title = %u WHERE guid = %u",
		uint32(GetPVPRank()), m_uint32Values[PLAYER_CHOSEN_TITLE], m_uint32Values[PLAYER__FIELD_KNOWN_TITLES], m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Auras()
{
	char buffer[10000];
	uint32 p =0;

	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			Aura *aur=m_auras[x];
			bool skip = false;
			for(uint32 i = 0; i < 3; ++i)
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT)
				{
					skip = true;
					break;
				}
			}

			// skipped spells due to bugs
			switch(aur->m_spellProto->Id)
			{
			case 12043: // Presence of mind
			case 11129: // Combustion
			case 28682: // Combustion proc
			case 16188: // Natures Swiftness
			case 17116: // Natures Swiftness
			case 34936: // Backlash
			case 35076: // Blessing of A'dal
			case 23333:	// WSG
			case 23335:	// WSG
				skip = true;
				break;
			}

			//disabled proc spells until proper loading is fixed. Some spells tend to block or not remove when restored
			if(aur->GetSpellProto()->procFlags)
			{
				//				DEBUG_LOG("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
				skip = true;
			}
			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
				//				DEBUG_LOG("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
				skip = true;
			}
			//we are going to cast passive spells anyway on login so no need to save auras for them
			if(aur->IsPassive())
				skip = true;

			if(skip)continue;
			uint32 d=aur->GetTimeLeft();
			if(d>3000)
				p += sprintf(buffer, "%u,%u,", aur->GetSpellId(), d);
		}
	}

	CharacterDatabase.Execute("UPDATE characters SET auras = '%s' WHERE guid = %u", buffer, m_uint32Values[OBJECT_FIELD_GUID]);
}

#endif

#ifdef CLUSTERING


#endif


void Player::EventGroupFullUpdate()
{
	if(m_playerInfo->m_Group)
	{
		//m_playerInfo->m_Group->Update();
		m_playerInfo->m_Group->UpdateAllOutOfRangePlayersFor(this);
	}
}

void Player::EjectFromInstance()
{
	if(m_bgEntryPointX && m_bgEntryPointY && m_bgEntryPointZ && !IS_INSTANCE(m_bgEntryPointMap))
	{
		if(SafeTeleport(m_bgEntryPointMap, m_bgEntryPointInstance, m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO))
			return;
	}

	SafeTeleport(m_bind_mapid, 0, m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, 0);
}

bool Player::HasQuestSpell(uint32 spellid) //Only for Cast Quests
{
	if (quest_spells.size()>0 && quest_spells.find(spellid) != quest_spells.end())
		return true;
	return false;
}
void Player::RemoveQuestSpell(uint32 spellid) //Only for Cast Quests
{
	if (quest_spells.size()>0)
		quest_spells.erase(spellid);
}

bool Player::HasQuestMob(uint32 entry) //Only for Kill Quests
{
	if (quest_mobs.size()>0 && quest_mobs.find(entry) != quest_mobs.end())
		return true;
	return false;
}
void Player::RemoveQuestMob(uint32 entry) //Only for Kill Quests
{
	if (quest_mobs.size()>0)
		quest_mobs.erase(entry);
}

PlayerInfo::~PlayerInfo()
{
	if(m_Group)
		m_Group->RemovePlayer(this);
}

void Player::CopyAndSendDelayedPacket(WorldPacket * data)
{
	WorldPacket * data2 = new WorldPacket(*data);
	delayedPackets.add(data2);
}

void Player::SendMeetingStoneQueue(uint32 DungeonId, uint8 Status)
{
	WorldPacket data(SMSG_MEETINGSTONE_SETQUEUE, 5);
	data << DungeonId << Status;
	m_session->SendPacket(&data);
}

void Player::PartLFGChannel()
{
	Channel * pChannel = channelmgr.GetChannel("LookingForGroup", this);
	if( pChannel == NULL )
		return;

	/*for(list<Channel*>::iterator itr = m_channels.begin(); itr != m_channels.end(); ++itr)
	{
		if( (*itr) == pChannel )
		{
			pChannel->Part(this);
			return;
		}
	}*/
	if( m_channels.find( pChannel->m_channelId ) == m_channels.end() )
		return;

	pChannel->Part( this, false );
}

//if we charmed or simply summoned a pet, this function should get called
void Player::EventSummonPet( Pet *new_pet )
{
	if ( !new_pet )
		return ; //another wtf error

	SpellSet::iterator it,iter;
	for(iter= mSpells.begin();iter != mSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *spellInfo = dbcSpell.LookupEntry(SpellID);
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER )
		{
			this->RemoveAllAuras( SpellID, this->GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( this->GetGUID() );
			Spell *spell = new Spell(this, spellInfo ,true, NULL);	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET )
		{
			this->RemoveAllAuras( SpellID, this->GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( new_pet->GetGUID() );
			Spell *spell = new Spell(this, spellInfo ,true, NULL);	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
	}
	//there are talents that stop working after you gain pet
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_ON_PET)
			m_auras[x]->Remove();
	//pet should inherit some of the talents from caster
	//new_pet->InheritSMMods(); //not required yet. We cast full spell to have visual effect too
}

//if pet/charm died or whatever hapened we should call this function
//!! note function might get called multiple times :P
void Player::EventDismissPet()
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET)
			m_auras[x]->Remove();
}

#ifdef ENABLE_COMPRESSED_MOVEMENT

CMovementCompressorThread *MovementCompressor;

void Player::AppendMovementData(uint32 op, uint32 sz, const uint8* data)
{
	//printf("AppendMovementData(%u, %u, 0x%.8X)\n", op, sz, data);
	m_movementBufferLock.Acquire();
	m_movementBuffer << uint8(sz + 2);
	m_movementBuffer << uint16(op);
	m_movementBuffer.append( data, sz );
	m_movementBufferLock.Release();
}

bool CMovementCompressorThread::run()
{
	set<Player*>::iterator itr;
	while(running)
	{
		m_listLock.Acquire();
		for(itr = m_players.begin(); itr != m_players.end(); ++itr)
		{
			(*itr)->EventDumpCompressedMovement();
		}
		m_listLock.Release();
		Sleep(World::m_movementCompressInterval);
	}

	return true;
}

void CMovementCompressorThread::AddPlayer(Player * pPlayer)
{
	m_listLock.Acquire();
	m_players.insert(pPlayer);
	m_listLock.Release();
}

void CMovementCompressorThread::RemovePlayer(Player * pPlayer)
{
	m_listLock.Acquire();
	m_players.erase(pPlayer);
	m_listLock.Release();
}

void Player::EventDumpCompressedMovement()
{
	if( m_movementBuffer.size() == 0 )
		return;

	m_movementBufferLock.Acquire();
	uint32 size = (uint32)m_movementBuffer.size();
	uint32 destsize = size + size/10 + 16;
	int rate = World::m_movementCompressRate;
	if(size >= 40000 && rate < 6)
		rate = 6;
	if(size <= 100)
		rate = 0;			// don't bother compressing packet smaller than this, zlib doesnt really handle them well

	// set up stream
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree  = 0;
	stream.opaque = 0;

	if(deflateInit(&stream, rate) != Z_OK)
	{
		sLog.outError("deflateInit failed.");
		m_movementBufferLock.Release();
		return;
	}

	uint8 *buffer = new uint8[destsize];

	// set up stream pointers
	stream.next_out  = (Bytef*)buffer+4;
	stream.avail_out = destsize;
	stream.next_in   = (Bytef*)m_movementBuffer.contents();
	stream.avail_in  = size;

	// call the actual process
	if(deflate(&stream, Z_NO_FLUSH) != Z_OK ||
		stream.avail_in != 0)
	{
		sLog.outError("deflate failed.");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		sLog.outError("deflate failed: did not end stream");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		sLog.outError("deflateEnd failed.");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// fill in the full size of the compressed stream
	*(uint32*)&buffer[0] = size;

	// send it
	m_session->OutPacket(763, (uint16)stream.total_out + 4, buffer);
	//printf("Compressed move compressed from %u bytes to %u bytes.\n", m_movementBuffer.size(), stream.total_out + 4);

	// cleanup memory
	delete [] buffer;
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
}
#endif

void Player::AddShapeShiftSpell(uint32 id)
{
	SpellEntry * sp = dbcSpell.LookupEntry( id );
	mShapeShiftSpells.insert( id );

	if( sp->RequiredShapeShift && ((uint32)1 << (GetShapeShift()-1)) & sp->RequiredShapeShift )
	{
		Spell * spe = new Spell( this, sp, true, NULL );
		SpellCastTargets t(this->GetGUID());
		spe->prepare( &t );
	}
}

void Player::RemoveShapeShiftSpell(uint32 id)
{
	mShapeShiftSpells.erase( id );
	RemoveAura( id );
}

// COOLDOWNS
void Player::_Cooldown_Add(uint32 Type, uint32 Misc, uint32 Time, uint32 SpellId, uint32 ItemId)
{
	PlayerCooldownMap::iterator itr = m_cooldownMap[Type].find( Misc );
	if( itr != m_cooldownMap[Type].end( ) )
	{
		if( itr->second.ExpireTime < Time )
		{
			itr->second.ExpireTime = Time;
			itr->second.ItemId = ItemId;
			itr->second.SpellId = SpellId;
		}
	}
	else
	{
		PlayerCooldown cd;
		cd.ExpireTime = Time;
		cd.ItemId = ItemId;
		cd.SpellId = SpellId;

		m_cooldownMap[Type].insert( make_pair( Misc, cd ) );
	}

	DEBUG_LOG("added cooldown for type %u misc %u time %u item %u spell %u", Type, Misc, Time - getMSTime(), ItemId, SpellId);
}

void Player::Cooldown_Add(SpellEntry * pSpell, Item * pItemCaster)
{
	uint32 mstime = getMSTime();
	int32 cool_time;

	if( CooldownCheat )
		return;

	if( pSpell->CategoryRecoveryTime > 0 && pSpell->Category )
	{
		cool_time = pSpell->CategoryRecoveryTime;
		if( pSpell->SpellGroupType )
		{
			SM_FIValue(SM_FCooldownTime, &cool_time, pSpell->SpellGroupType);
			SM_PIValue(SM_PCooldownTime, &cool_time, pSpell->SpellGroupType);
		}

		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}
	
	if( pSpell->RecoveryTime > 0 )
	{
		cool_time = pSpell->RecoveryTime;
		if( pSpell->SpellGroupType )
		{
			SM_FIValue(SM_FCooldownTime, &cool_time, pSpell->SpellGroupType);
			SM_PIValue(SM_PCooldownTime, &cool_time, pSpell->SpellGroupType);
		}

		_Cooldown_Add( COOLDOWN_TYPE_SPELL, pSpell->Id, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}
}

void Player::Cooldown_AddStart(SpellEntry * pSpell)
{
	if( pSpell->StartRecoveryTime == 0 || CooldownCheat)
		return;

	uint32 mstime = getMSTime();
	int32 atime;
	if( m_floatValues[UNIT_MOD_CAST_SPEED] >= 1.0f )
		atime = pSpell->StartRecoveryTime;
	else
		atime = float2int32( float(pSpell->StartRecoveryTime) * m_floatValues[UNIT_MOD_CAST_SPEED] );

	if( atime <= 0 )
		return;

	if( pSpell->StartRecoveryCategory )		// if we have a different cool category to the actual spell category - only used by few spells
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->StartRecoveryCategory, mstime + atime, pSpell->Id, 0 );
	/*else if( pSpell->Category )				// cooldowns are grouped
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + pSpell->StartRecoveryTime, pSpell->Id, 0 );*/
	else									// no category, so it's a gcd
	{
		DEBUG_LOG("Global cooldown adding: %u ms", atime );
		m_globalCooldown = mstime + atime;
	}
}

void Player::Cooldown_OnCancel(SpellEntry *pSpell)
{
	if( pSpell->StartRecoveryTime == 0 || CooldownCheat)
		return;

	uint32 mstime = getMSTime();
	int32 atime = float2int32( float(pSpell->StartRecoveryTime) * m_floatValues[UNIT_MOD_CAST_SPEED] );
	if( atime <= 0 )
		return;

	if( pSpell->StartRecoveryCategory )		// if we have a different cool category to the actual spell category - only used by few spells
		m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase(pSpell->StartRecoveryCategory);
	else									// no category, so it's a gcd
		m_globalCooldown = mstime;
}

bool Player::Cooldown_CanCast(SpellEntry * pSpell)
{
	PlayerCooldownMap::iterator itr;
	uint32 mstime = getMSTime();

	if( CooldownCheat )
		return true;

	if( pSpell->Category )
	{
		itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].find( pSpell->Category );
		if( itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end( ) )
		{
			if( mstime < itr->second.ExpireTime )
				return false;
			else
				m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr );
		}		
	}

	itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].find( pSpell->Id );
	if( itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end( ) )
	{
		if( mstime < itr->second.ExpireTime )
			return false;
		else
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr );
	}

	if( pSpell->StartRecoveryTime && m_globalCooldown )			/* gcd doesn't affect spells without a cooldown it seems */
	{
		if( mstime < m_globalCooldown )
			return false;
		else
			m_globalCooldown = 0;
	}

	return true;
}

void Player::Cooldown_AddItem(ItemPrototype * pProto, uint32 x)
{
	if( pProto->Spells[x].CategoryCooldown <= 0 && pProto->Spells[x].Cooldown <= 0 )
		return;

	ItemSpell * isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->CategoryCooldown > 0)
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, isp->Category, isp->CategoryCooldown + mstime, isp->Id, pProto->ItemId );

	if( isp->Cooldown > 0 )
		_Cooldown_Add( COOLDOWN_TYPE_SPELL, isp->Id, isp->Cooldown + mstime, isp->Id, pProto->ItemId );
}

bool Player::Cooldown_CanCast(ItemPrototype * pProto, uint32 x)
{
	PlayerCooldownMap::iterator itr;
	ItemSpell * isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->Category )
	{
		itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].find( isp->Category );
		if( itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end( ) )
		{
			if( mstime < itr->second.ExpireTime )
				return false;
			else
				m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr );
		}	
	}

	itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].find( isp->Id );
	if( itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end( ) )
	{
		if( mstime < itr->second.ExpireTime )
			return false;
		else
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr );
	}

	return true;
}

#define COOLDOWN_SKIP_SAVE_IF_MS_LESS_THAN 10000

void Player::_SavePlayerCooldowns(QueryBuffer * buf)
{
	PlayerCooldownMap::iterator itr;
	PlayerCooldownMap::iterator itr2;
	uint32 i;
	uint32 seconds;
	uint32 mstime = getMSTime();

	// clear them (this should be replaced with an update queue later)
	if( buf != NULL )
		buf->AddQuery("DELETE FROM playercooldowns WHERE player_guid = %u", m_uint32Values[OBJECT_FIELD_GUID] );		// 0 is guid always
	else
		CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", m_uint32Values[OBJECT_FIELD_GUID] );		// 0 is guid always

	for( i = 0; i < NUM_COOLDOWN_TYPES; ++i )
	{
		itr = m_cooldownMap[i].begin( );
		for( ; itr != m_cooldownMap[i].end( ); )
		{
			itr2 = itr++;

			// expired ones - no point saving, nor keeping them around, wipe em
			if( mstime >= itr2->second.ExpireTime )
			{
				m_cooldownMap[i].erase( itr2 );
				continue;
			}
			
			// skip small cooldowns which will end up expiring by the time we log in anyway
			if( ( itr2->second.ExpireTime - mstime ) < COOLDOWN_SKIP_SAVE_IF_MS_LESS_THAN )
				continue;

			// work out the cooldown expire time in unix timestamp format
			// burlex's reason: 30 day overflow of 32bit integer, also
			// under windows we use GetTickCount() which is the system uptime, if we reboot
			// the server all these timestamps will appear to be messed up.
			
			seconds = (itr2->second.ExpireTime - mstime) / 1000;
			// this shouldnt ever be nonzero because of our check before, so no check needed
			
			if( buf != NULL )
			{
				buf->AddQuery( "INSERT INTO playercooldowns VALUES(%u, %u, %u, %u, %u, %u)", m_uint32Values[OBJECT_FIELD_GUID],
					i, itr2->first, seconds + (uint32)UNIXTIME, itr2->second.SpellId, itr2->second.ItemId );
			}
			else
			{
				CharacterDatabase.Execute( "INSERT INTO playercooldowns VALUES(%u, %u, %u, %u, %u, %u)", m_uint32Values[OBJECT_FIELD_GUID],
					i, itr2->first, seconds + (uint32)UNIXTIME, itr2->second.SpellId, itr2->second.ItemId );
			}
		}
	}
}

void Player::_LoadPlayerCooldowns(QueryResult * result)
{
	if( result == NULL )
		return;

	// we should only really call getMSTime() once to avoid user->system transitions, plus
	// the cost of calling a function for every cooldown the player has
	uint32 mstime = getMSTime();
	uint32 type;
	uint32 misc;
	uint32 rtime;
	uint32 realtime;
	uint32 itemid;
	uint32 spellid;
	PlayerCooldown cd;

	do 
	{
		type = result->Fetch()[0].GetUInt32();
		misc = result->Fetch()[1].GetUInt32();
		rtime = result->Fetch()[2].GetUInt32();
		spellid = result->Fetch()[3].GetUInt32();
		itemid = result->Fetch()[4].GetUInt32();

		if( type >= NUM_COOLDOWN_TYPES )
			continue;

		// remember the cooldowns were saved in unix timestamp format for the reasons outlined above,
		// so restore them back to mstime upon loading

		if( (uint32)UNIXTIME > rtime )
			continue;

		rtime -= (uint32)UNIXTIME;

		if( rtime < 10 )
			continue;

		realtime = mstime + ( ( rtime ) * 1000 );

		// apply it back into cooldown map
		cd.ExpireTime = realtime;
		cd.ItemId = itemid;
		cd.SpellId = spellid;
		m_cooldownMap[type].insert( make_pair( misc, cd ) );

	} while ( result->NextRow( ) );
}

void Player::_SpeedhackCheck()
{
	if( sWorld.antihack_speed && !GetTaxiState() && m_isMoving && GetMapMgr())
	{
		if( ( sWorld.no_antihack_on_gm && GetSession()->HasGMPermissions() ) )
			return; // do not check GMs speed been the config tells us not to.
		/*if( m_position == _lastHeartbeatPosition && m_isMoving )
		{
			// this means the client is probably lagging. don't update the timestamp, don't do anything until we start to receive
			// packets again (give the poor laggers a chance to catch up)
			return;
		}*/

		// simplified; just take the fastest speed. less chance of fuckups too
		float speed = ( flying_aura ) ? m_flySpeed : m_runSpeed;
		if( flying_aura )
		{
			if( m_runSpeed > m_flySpeed )
				speed = m_runSpeed;
		}

		if( m_swimSpeed > speed )
			speed = m_swimSpeed;

		if( speed != m_lastHeartbeatV )
		{
			if( m_isMoving )
				m_startMoveTime = m_lastMoveTime;
			else
				m_startMoveTime = 0;

			m_lastHeartbeatPosition = m_position;
			m_lastHeartbeatV = speed;
			return;
		}

		if( !m_heartbeatDisable && !m_uint32Values[UNIT_FIELD_CHARM] && m_TransporterGUID == 0 && !m_speedChangeInProgress )
		{
			// latency compensation a little
			if( World::m_speedHackLatencyMultiplier > 0.0f )
				speed += (float(m_session->GetLatency()) / 100.0f) * World::m_speedHackLatencyMultiplier; 

			float distance = m_position.Distance2D( m_lastHeartbeatPosition );
			uint32 time_diff = m_lastMoveTime - m_startMoveTime;
			uint32 move_time = float2int32( ( distance / ( speed * 0.001f ) ) );
			int32 difference = time_diff - move_time;
			DEBUG_LOG("speed: %f diff: %i dist: %f move: %u tdiff: %u", speed, difference, distance, move_time, time_diff );
			if( difference < World::m_speedHackThreshold )
			{
				if( m_speedhackChances != 0 )
				{
					SetMovement( MOVE_ROOT, 1 );
					BroadcastMessage( "Speedhack detected. Please contact an admin with the below information if you believe this is a false detection." );
					BroadcastMessage( "You will be disconnected in 10 seconds." );
					BroadcastMessage( MSG_COLOR_WHITE"speed: %f diff: %i dist: %f move: %u tdiff: %u\n", speed, difference, distance, move_time, time_diff );
					sCheatLog.writefromsession(GetSession(), "Speed hack detected! Distance: %i, Speed: %f, Move: %u, tdiff: %u", distance, speed, move_time, time_diff);
					if(m_bg)
						m_bg->RemovePlayer(this, false);

					//SafeTeleport(GetMapMgr(), m_lastHeartbeatPosition);
					sEventMgr.AddEvent( this, &Player::_Disconnect, EVENT_PLAYER_KICK, 10000, 1, 0 );
					m_speedhackChances = 0;
				}
			}

			// Quickie wall hack detection
			float slope = (GetPositionZ() - m_lastHeartbeatPosition.z) / distance;
			if(!flying_aura && slope > World::m_wallhackthreshold && distance > 1.2f)
			{
				sCheatLog.writefromsession(GetSession(), "Wall hack detected! 2D Distance: %f, Slope: %f, Threshold: %f", distance, slope, World::m_wallhackthreshold);
				BroadcastMessage("Wall hack detected. Please contact an admin with the following debug information if you believe this is a false positive." );
				BroadcastMessage("Distance: %f, Slope: %f", distance, slope);
				sEventMgr.AddEvent( this, &Player::_Disconnect, EVENT_PLAYER_KICK, 10000, 1, 0 );
				//SafeTeleport(GetMapMgr(), m_lastHeartbeatPosition);
			}

		}
	}
}

void Player::_Disconnect()
{
	m_session->Disconnect();
}

void Player::ResetSpeedHack()
{
	ResetHeartbeatCoords();
	m_heartbeatDisable = 0;
}

void Player::DelaySpeedHack(uint32 ms)
{
	uint32 t;
	m_heartbeatDisable = 1;

	if( event_GetTimeLeft( EVENT_PLAYER_RESET_HEARTBEAT, &t ) )
	{
		if( t > ms )		// dont override a slower reset
			return;

		// override it
		event_ModifyTimeAndTimeLeft( EVENT_PLAYER_RESET_HEARTBEAT, ms );
		return;
	}

	// add a new event
	sEventMgr.AddEvent( this, &Player::ResetSpeedHack, EVENT_PLAYER_RESET_HEARTBEAT, ms, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

/************************************************************************/
/* SOCIAL                                                               */
/************************************************************************/

void Player::Social_AddFriend(const char * name, const char * note)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	map<uint32, char*>::iterator itr;
	PlayerInfo * info;

	// lookup the player
	info = objmgr.GetPlayerInfoByName(name);
	if( info == NULL )
	{
		data << uint8(FRIEND_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}

	// team check
	if( info->team != m_playerInfo->team )
	{
		data << uint8(FRIEND_ENEMY) << uint64(info->guid);
		m_session->SendPacket(&data);
		return;
	}

	// are we ourselves?
	if( info == m_playerInfo )
	{
		data << uint8(FRIEND_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_friends.find(info->guid);
	if( itr != m_friends.end() )
	{
		data << uint8(FRIEND_ALREADY) << uint64(info->guid);
		m_session->SendPacket(&data);
		m_socialLock.Release();
		return;
	}

	if( info->m_loggedInPlayer != NULL )
	{
		data << uint8(FRIEND_ADDED_ONLINE);
		data << uint64(info->guid);
		if( note != NULL )
			data << note;
		else
			data << uint8(0);

		data << uint8(1);
		data << info->m_loggedInPlayer->GetZoneId();
		data << info->lastLevel;
		data << uint32(info->cl);

		info->m_loggedInPlayer->m_socialLock.Acquire();
		info->m_loggedInPlayer->m_hasFriendList.insert( GetLowGUID() );
		info->m_loggedInPlayer->m_socialLock.Release();
	}
	else
	{
		data << uint8(FRIEND_ADDED_OFFLINE);
		data << uint64(info->guid);
	}

	if( note != NULL )
		m_friends.insert( make_pair(info->guid, strdup(note)) );
	else
		m_friends.insert( make_pair(info->guid, (char*)NULL) );

	m_socialLock.Release();
	m_session->SendPacket(&data);

	// dump into the db
	CharacterDatabase.Execute("INSERT INTO social_friends VALUES(%u, %u, \"%s\")",
		GetLowGUID(), info->guid, note ? CharacterDatabase.EscapeString(string(note)).c_str() : "");
}

void Player::Social_RemoveFriend(uint32 guid)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	map<uint32, char*>::iterator itr;

	// are we ourselves?
	if( guid == GetLowGUID() )
	{
		data << uint8(FRIEND_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_friends.find(guid);
	if( itr != m_friends.end() )
	{
		if( itr->second != NULL )
			free(itr->second);

		m_friends.erase(itr);
	}

	data << uint8(FRIEND_REMOVED);
	data << uint64(guid);

	m_socialLock.Release();

	Player * pl = objmgr.GetPlayer( (uint32)guid );
	if( pl != NULL )
	{
		pl->m_socialLock.Acquire();
		pl->m_hasFriendList.erase( GetLowGUID() );
		pl->m_socialLock.Release();
	}

	m_session->SendPacket(&data);

	// remove from the db
	CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u AND friend_guid = %u", 
		GetLowGUID(), (uint32)guid);
}

void Player::Social_SetNote(uint32 guid, const char * note)
{
	map<uint32,char*>::iterator itr;

	m_socialLock.Acquire();
	itr = m_friends.find(guid);

	if( itr == m_friends.end() )
	{
		m_socialLock.Release();
		return;
	}

	if( itr->second != NULL )
		free(itr->second);

	if( note != NULL )
		itr->second = strdup( note );
	else
		itr->second = NULL;

	m_socialLock.Release();
	CharacterDatabase.Execute("UPDATE social_friends SET note = \"%s\" WHERE character_guid = %u AND friend_guid = %u",
		note ? CharacterDatabase.EscapeString(string(note)).c_str() : "", GetLowGUID(), guid);
}

void Player::Social_AddIgnore(const char * name)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;
	PlayerInfo * info;

	// lookup the player
	info = objmgr.GetPlayerInfoByName(name);
	if( info == NULL )
	{
		data << uint8(FRIEND_IGNORE_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}

	// are we ourselves?
	if( info == m_playerInfo )
	{
		data << uint8(FRIEND_IGNORE_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_ignores.find(info->guid);
	if( itr != m_ignores.end() )
	{
		data << uint8(FRIEND_IGNORE_ALREADY) << uint64(info->guid);
		m_session->SendPacket(&data);
		m_socialLock.Release();
		return;
	}

	data << uint8(FRIEND_IGNORE_ADDED);
	data << uint64(info->guid);

	m_ignores.insert( info->guid );

	m_socialLock.Release();
	m_session->SendPacket(&data);

	// dump into db
	CharacterDatabase.Execute("INSERT INTO social_ignores VALUES(%u, %u)", GetLowGUID(), info->guid);
}

void Player::Social_RemoveIgnore(uint32 guid)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;

	// are we ourselves?
	if( guid == GetLowGUID() )
	{
		data << uint8(FRIEND_IGNORE_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_ignores.find(guid);
	if( itr != m_ignores.end() )
	{
		m_ignores.erase(itr);
	}

	data << uint8(FRIEND_IGNORE_REMOVED);
	data << uint64(guid);

	m_socialLock.Release();

	m_session->SendPacket(&data);

	// remove from the db
	CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u AND ignore_guid = %u", 
		GetLowGUID(), (uint32)guid);
}

bool Player::Social_IsIgnoring(PlayerInfo * m_info)
{
	bool res;
	m_socialLock.Acquire();
	if( m_ignores.find( m_info->guid ) == m_ignores.end() )
		res = false;
	else
		res = true;

	m_socialLock.Release();
	return res;
}

bool Player::Social_IsIgnoring(uint32 guid)
{
	bool res;
	m_socialLock.Acquire();
	if( m_ignores.find( guid ) == m_ignores.end() )
		res = false;
	else
		res = true;

	m_socialLock.Release();
	return res;
}

void Player::Social_TellFriendsOnline()
{
	if( m_hasFriendList.empty() )
		return;

	WorldPacket data(SMSG_FRIEND_STATUS, 22);
	set<uint32>::iterator itr;
	Player * pl;
	data << uint8( FRIEND_ONLINE ) << GetGUID() << uint8( 1 );
	data << GetAreaID() << getLevel() << uint32(getClass());

	m_socialLock.Acquire();
	for( itr = m_hasFriendList.begin(); itr != m_hasFriendList.end(); ++itr )
	{
		pl = objmgr.GetPlayer(*itr);
		if( pl != NULL )
			pl->GetSession()->SendPacket(&data);
	}
	m_socialLock.Release();
}

void Player::Social_TellFriendsOffline()
{
	if( m_hasFriendList.empty() )
		return;

	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;
	Player * pl;
	data << uint8( FRIEND_OFFLINE ) << GetGUID() << uint8( 0 );

	m_socialLock.Acquire();
	for( itr = m_hasFriendList.begin(); itr != m_hasFriendList.end(); ++itr )
	{
		pl = objmgr.GetPlayer(*itr);
		if( pl != NULL )
			pl->GetSession()->SendPacket(&data);
	}
	m_socialLock.Release();
}

void Player::Social_SendFriendList(uint32 flag)
{
	WorldPacket data(SMSG_CONTACT_LIST, 500);
	map<uint32,char*>::iterator itr;
	set<uint32>::iterator itr2;
	Player * plr;

	m_socialLock.Acquire();

	data << flag;
	data << uint32( m_friends.size() + m_ignores.size() );
	for( itr = m_friends.begin(); itr != m_friends.end(); ++itr )
	{
		// guid
		data << uint64( itr->first );

		// friend/ignore flag.
		// 1 - friend
		// 2 - ignore
		// 3 - muted?
		data << uint32( 1 );

		// player note
		if( itr->second != NULL )
			data << itr->second;
		else
			data << uint8(0);

		// online/offline flag
		plr = objmgr.GetPlayer( itr->first );
		if( plr != NULL )
		{
			data << uint8( 1 );
			data << plr->GetZoneId();
			data << plr->getLevel();
			data << uint32( plr->getClass() );
		}
		else
			data << uint8( 0 );
	}

	for( itr2 = m_ignores.begin(); itr2 != m_ignores.end(); ++itr2 )
	{
		// guid
		data << uint64( (*itr2) );
		
		// ignore flag - 2
		data << uint32( 2 );

		// no note
		data << uint8( 0 );
	}

	m_socialLock.Release();
	m_session->SendPacket(&data);
}

void Player::VampiricSpell(uint32 dmg, Unit* pTarget)
{
	float fdmg = float(dmg);
	uint32 bonus;
	int32 perc;
	Group * pGroup = GetGroup();
	SubGroup * pSubGroup = (pGroup != NULL) ? pGroup->GetSubGroup(GetSubGroup()) : NULL;
	GroupMembersSet::iterator itr;

	if( ( !m_vampiricEmbrace && !m_vampiricTouch ) || getClass() != PRIEST )
		return;

	if( m_vampiricEmbrace > 0 && pTarget->m_hasVampiricEmbrace > 0 && pTarget->HasAurasOfNameHashWithCaster(SPELL_HASH_VAMPIRIC_EMBRACE, this) )
	{
		perc = 15;
		SM_FIValue(SM_FSPELL_VALUE, &perc, 4);

		bonus = float2int32(fdmg * (float(perc)/100.0f));
		if( bonus > 0 )
		{
			Heal(this, 15286, bonus);
			
			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo )
						Heal( (*itr)->m_loggedInPlayer, 15286, bonus );
				}
			}
		}
	}

	if( m_vampiricTouch > 0 && pTarget->m_hasVampiricTouch > 0 && pTarget->HasAurasOfNameHashWithCaster(SPELL_HASH_VAMPIRIC_TOUCH, this) )
	{
		perc = 5;
		//SM_FIValue(SM_FSPELL_VALUE, &perc, 4);

		bonus = float2int32(fdmg * (float(perc)/100.0f));
		if( bonus > 0 )
		{
			Energize(this, 34919, bonus, POWER_TYPE_MANA);

			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo && (*itr)->m_loggedInPlayer->GetPowerType() == POWER_TYPE_MANA )
						Energize((*itr)->m_loggedInPlayer, 34919, bonus, POWER_TYPE_MANA);
				}
			}
		}
	}
}

void Player::_FlyhackCheck()
{
#ifdef COLLISION
	if(!IsFlyHackEligible())
		return;

	// Get the current terrain height at this position
	float height = CollideInterface.GetHeight(GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ());
	if(height == NO_WMO_HEIGHT)
	{
		height = GetMapMgr()->GetLandHeight(GetPositionX(), GetPositionY());
		if(height == 0.0f) // At this point, we just can't get a valid height.
			return;
	}

	float diff = GetPositionZ() - height;
	if(diff < 0)
		diff = -diff;

	if(diff <= 8.0f) // Relatively small threshold maybe?
		return;

	if(m_flyHackChances--)
		return;

	// Something's afoot!
	EventTeleport(GetMapId(), GetPositionX(), GetPositionY(), height); // Return us to valid coordinates for the next logon.
	GetSession()->Disconnect();
#endif
}

bool Player::IsFlyHackEligible()
{
	if(!sWorld.antihack_flight) return false;

	if(GetSession()->HasGMPermissions())
		return false;

	if(!GetMapMgr() || flying_aura || IsStunned() || IsPacified() || IsFeared() || GetTaxiState() || m_TransporterGUID != 0) // Stunned, rooted, riding a flying machine, whatever
		return false;

	if(GetMapId() == 369) return false; // Deeprun Tram

	MovementInfo* moveInfo = GetSession()->GetMovementInfo();
	if(!moveInfo) return false;

	uint32 moveFlags = moveInfo->flags;

	if(moveFlags & MOVEFLAG_FALLING || moveFlags & MOVEFLAG_FALLING_FAR || moveFlags & MOVEFLAG_FREE_FALLING)
		return false; // Freeee falling.

	if(m_UnderwaterState) // We're swimming.
		return false;

	return true;
}

void Player::UpdateTalentInspectBuffer()
{
	/*memset(m_talentInspectBuffer, 0, TALENT_INSPECT_BYTES);

	uint32 talent_tab_pos = 0;
	uint32 talent_max_rank;
	uint32 talent_tab_id;
	uint32 talent_index;
	uint32 rank_index;
	uint32 rank_slot;
	uint32 rank_offset;
	uint32 i;

	for( i = 0; i < 3; ++i )
	{
		talent_tab_id = sWorld.InspectTalentTabPages[getClass()][i];

		for( uint32 j = 0; j < dbcTalent.GetNumRows(); ++j )
		{
			TalentEntry const* talent_info = dbcTalent.LookupRow( j );

			//DEBUG_LOG( "HandleInspectOpcode: i(%i) j(%i)", i, j );

			if( talent_info == NULL )
				continue;

			//DEBUG_LOG( "HandleInspectOpcode: talent_info->TalentTree(%i) talent_tab_id(%i)", talent_info->TalentTree, talent_tab_id );

			if( talent_info->TalentTree != talent_tab_id )
				continue;

			talent_max_rank = 0;
			for( uint32 k = 5; k > 0; --k )
			{
				//DEBUG_LOG( "HandleInspectOpcode: k(%i) RankID(%i) HasSpell(%i) TalentTree(%i) Tab(%i)", k, talent_info->RankID[k - 1], player->HasSpell( talent_info->RankID[k - 1] ), talent_info->TalentTree, talent_tab_id );
				if( talent_info->RankID[k - 1] != 0 && HasSpell( talent_info->RankID[k - 1] ) )
				{
					talent_max_rank = k;
					break;
				}
			}

			//DEBUG_LOG( "HandleInspectOpcode: RankID(%i) talent_max_rank(%i)", talent_info->RankID[talent_max_rank-1], talent_max_rank );

			if( talent_max_rank <= 0 )
				continue;

			talent_index = talent_tab_pos;

			std::map< uint32, uint32 >::iterator itr = sWorld.InspectTalentTabPos.find( talent_info->TalentID );

			if( itr != sWorld.InspectTalentTabPos.end() )
				talent_index += itr->second;
			//else
			//DEBUG_LOG( "HandleInspectOpcode: talent(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i)", talent_info->TalentID, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset );

			rank_index = ( uint32( ( talent_index + talent_max_rank - 1 ) / 7 ) ) * 8  + ( uint32( ( talent_index + talent_max_rank - 1 ) % 7 ) );
			rank_slot = rank_index / 8;
			rank_offset = rank_index % 8;

			if( rank_slot < TALENT_INSPECT_BYTES )
			{
				uint32 v = (uint32)m_talentInspectBuffer[rank_slot];
				v |= ( 1 << rank_offset );
				m_talentInspectBuffer[rank_offset] |= v;
			};

			DEBUG_LOG( "HandleInspectOpcode: talent(%i) talent_max_rank(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i)", talent_info->TalentID, talent_max_rank, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset );
		}

		std::map< uint32, uint32 >::iterator itr = sWorld.InspectTalentTabSize.find( talent_tab_id );

		if( itr != sWorld.InspectTalentTabSize.end() )
			talent_tab_pos += itr->second;

	}*/
}

void Player::GenerateLoot(Corpse *pCorpse)
{
	if( m_bg == NULL )
		return;

	// default gold
	pCorpse->ClearLoot();
	pCorpse->m_loot.gold = 500;
		
	if( m_bg != NULL )
		m_bg->HookGenerateLoot(this, pCorpse);
}
