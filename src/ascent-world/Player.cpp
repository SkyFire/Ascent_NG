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
UpdateMask Player::m_visibleUpdateMask;
#define COLLISION_MOUNT_CHECK_INTERVAL 1000
static const uint8 baseRunes[6] = {0,0,1,1,2,2};
static const uint32 DKNodesMask[12] = {4294967295,4093640703,830406655,0,33570816,1310944,3250593812,73752,896,67111952,0,0};//all old continents are available to DK's by default.

Player::Player( uint32 guid )
{
	m_runemask = 0x3F;
	m_bgRatedQueue = false;
	m_massSummonEnabled = false;
	m_objectTypeId = TYPEID_PLAYER;
	m_valuesCount = PLAYER_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(PLAYER_END)*sizeof(uint32));
	m_updateMask.SetCount(PLAYER_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_PLAYER|TYPE_UNIT|TYPE_OBJECT);
	SetUInt32Value( OBJECT_FIELD_GUID,guid);
	m_wowGuid.Init(GetGUID());
	m_deathRuneMasteryChance = 0;
}

void Player::Init()
{
	m_mailBox = new Mailbox( GetUInt32Value(OBJECT_FIELD_GUID) );
	m_ItemInterface		 = new ItemInterface(TO_PLAYER(this));
	m_achievementInterface = new AchievementInterface(TO_PLAYER(this));

	m_bgSlot = 0;
	mAngerManagement = false;

	m_feralAP = 0;
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
	Seal					= 0;
	m_session			   = 0;
	TrackingSpell		   = 0;
	m_status				= 0;
	offhand_dmg_mod		 = 0.5;
	m_walkSpeed			= 2.5f;
	m_runSpeed			  = PLAYER_NORMAL_RUN_SPEED;
	m_isMoving			  = false;
	m_isWaterWalking	  = 0;
	m_ShapeShifted		  = 0;
	m_curSelection		  = 0;
	m_lootGuid			  = 0;
	m_Summon				= NULL;
	hasqueuedpet			= false;
	m_hasInRangeGuards = 0;

	m_PetNumberMax		  = 0;
	m_lastShotTime		  = 0;

	m_H_regenTimer			= 0;
	m_P_regenTimer			= 0;
	m_onTaxi				= false;

	m_taxi_pos_x			= 0;
	m_taxi_pos_y			= 0;
	m_taxi_pos_z			= 0;
	m_taxi_ride_time		= 0;

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

	for(int32 i=0;i<NUM_MECHANIC;i++)
	{
		MechanicDurationPctMod[i] = 1.0f;
	}

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

	m_lootGuid			  = 0;
	m_banned				= false;

	//Bind possition
	m_bind_pos_x			= 0;
	m_bind_pos_y			= 0;
	m_bind_pos_z			= 0;
	m_bind_mapid			= 0;
	m_bind_zoneid		   = 0;

	// Rest
	m_timeLogoff		= 0;
	m_isResting			= 0;
	m_restState			= 0;
	m_restAmount		= 0;
	m_afk_reason		= "";
	m_playedtime[0]		= 0;
	m_playedtime[1]		= 0;
	m_playedtime[2]		= (uint32)UNIXTIME;

	m_AllowAreaTriggerPort  = true;

	// Battleground
	m_bgEntryPointMap	   = 0;
	m_bgEntryPointX		 = 0;	
	m_bgEntryPointY		 = 0;
	m_bgEntryPointZ		 = 0;
	m_bgEntryPointO		 = 0;
	for(uint32 i = 0; i < 3; ++i)
	{
		m_bgQueueType[i] = 0;
		m_bgQueueInstanceId[i] = 0;
		m_bgIsQueued[i] = false;
		m_bgQueueTime[i] = 0;
	}
	m_bg = NULL;

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
	m_UnderwaterTime		= 180000;
	m_UnderwaterMaxTime	 = 180000;
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
	{
		for(uint32 x = 0; x < 7; x++)
		{	
			SpellDmgDoneByAttribute[a][x] = 0;
			SpellHealDoneByAttribute[a][x] = 0;
		}
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

		CurrentGossipMenu	   = NULL;

		ResetHeartbeatCoords();

		m_AreaID				= 0;
		m_areaDBC				= NULL;
		m_actionsDirty		  = false;
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

		myCorpse				= NULL;

		blinked				 = false;
		m_speedhackChances	  = 3;
		m_explorationTimer	  = getMSTime();
		linkTarget			  = NULL;
		stack_cheat			 = false;
		triggerpass_cheat = false;
		m_pvpTimer			  = 0;
		m_globalCooldown = 0;
		m_lastHonorResetTime	= 0;
		memset(&mActions, 0, PLAYER_ACTION_BUTTON_SIZE);
		tutorialsDirty = true;
		m_TeleportState = 1;
		m_beingPushed = false;
		m_FlyingAura = 0;
		resend_speed = false;
		rename_pending = false;
		recustomize_pending = false;
		titanGrip = false;
		iInstanceType		   = 0;
		memset(reputationByListId, 0, sizeof(FactionReputation*) * 128);

		m_comboTarget = 0;
		m_comboPoints = 0;

		SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 0.0f);
		SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 0.0f);

		UpdateLastSpeeds();

		m_resist_critical[0]=m_resist_critical[1]=0;
		m_castFilterEnabled = false;

		m_resist_critical[0]=m_resist_critical[1]=0;
		m_castFilterEnabled = false;
		for (uint32 x =0;x<3;x++)
		{
			m_resist_hit[x]=0;
			m_skipCastCheck[x] = 0;
			m_castFilter[x] = 0;
		}
		for(int i = 0; i < 6; ++i)
		{
			m_runes[i] = baseRunes[i];
		}
		m_maxTalentPoints = 0;
		m_talentActiveSpec = 0;
		m_talentSpecsCount = 1;
		ok_to_remove = false;
		trigger_on_stun = 0;
		trigger_on_stun_chance = 100;
		m_modphyscritdmgPCT = 0;
		m_RootedCritChanceBonus = 0;

		for(int i = 0; i < 6; ++i)
			m_runes[i] = baseRunes[i];

		ok_to_remove = false;
		trigger_on_stun = 0;
		trigger_on_stun_chance = 100;
		m_modphyscritdmgPCT = 0;
		m_RootedCritChanceBonus = 0;

		m_ModInterrMRegenPCT = 0;
		m_ModInterrMRegen =0;
		m_rap_mod_pct = 0;
		m_modblockabsorbvalue = 0;
		m_modblockvaluefromspells = 0;
		m_summoner = m_summonInstanceId = m_summonMapId = 0;
		m_lastMoveType = 0;
		m_tempSummon = NULL;
		m_spellcomboPoints = 0;

		for(uint8 i = 0; i < 3 ; ++i)
			m_pendingBattleground[i] = NULL;

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
		m_vampiricEmbrace = 0;
		m_magnetAura = NULL;
		m_lastMoveTime = 0;
		m_lastMovementPacketTimestamp = 0;
		m_cheatEngineChances = 2;
		m_mageInvisibility = false;
		mWeakenedSoul = false;
		mForbearance = false;
		mExhaustion = false;
		mHypothermia = false;
		mSated = false;
		mAvengingWrath = true;
		m_flyhackCheckTimer = 0;
		m_bgFlagIneligible = 0;
		m_moltenFuryDamageIncreasePct = 0;
		m_insigniaTaken = false;
		mSpellsUniqueTargets.clear();

		m_wratings.clear();
		m_QuestGOInProgress.clear();
		m_removequests.clear();
		m_finishedQuests.clear();
		m_finishedDailyQuests.clear();
		quest_spells.clear();
		quest_mobs.clear();
		loginauras.clear();
		OnMeleeAuras.clear();
		m_Pets.clear();
		m_itemsets.clear();
		m_channels.clear();
		m_visibleObjects.clear();
		mSpells.clear();
		for(uint32 i = 0; i < 21; ++i)
			m_WeaponSubClassDamagePct[i] = 1.0f;

	Unit::Init();
}

void Player::OnLogin()
{

}


Player::~Player ( )
{
	sEventMgr.RemoveEvents(TO_PLAYER(this));

	int i;

	if(!ok_to_remove)
	{
		printf("Player deleted from non-logoutplayer!\n");
		OutputCrashLogLine("Player deleted from non-logoutplayer!");
#ifdef WIN32
		CStackWalker sw;
		sw.ShowCallstack();
#endif
	}

	objmgr.RemovePlayer(this);

	if(m_session)
	{
		m_session->SetPlayer(NULL);
	}

	Player* pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->mTradeTarget = 0;
	}

	pTarget = objmgr.GetPlayer(GetInviter());
	if(pTarget)
		pTarget->SetInviter(0);

	pTarget = NULL;

	if(m_Summon)
	{
		m_Summon->Dismiss(true);
		m_Summon->ClearPetOwner();
	}

	if (m_GM_SelectedGO)
		m_GM_SelectedGO = NULL;

	if (m_SummonedObject)
	{
		m_SummonedObject->Delete();
		m_SummonedObject = NULL;
	}

	if( m_mailBox )
		delete m_mailBox;

	mTradeTarget = 0;

	if(DuelingWith != NULL)
		DuelingWith->DuelingWith = NULL;
	DuelingWith = NULL;

	CleanupGossipMenu();
	ASSERT(!IsInWorld());

	// delete m_talenttree

	CleanupChannels();
	for(i = 0; i < 25; ++i)
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

	if(m_achievementInterface)
		delete m_achievementInterface;

	if(m_reputation.size())
	{
		for(ReputationMap::iterator itr = m_reputation.begin(); itr != m_reputation.end(); ++itr)
			delete itr->second;
	}
	m_objectTypeId = TYPEID_UNUSED;

	if(m_playerInfo)
		m_playerInfo->m_loggedInPlayer=NULL;

	if( !delayedPackets.empty() )
	{
		while( delayedPackets.size() )
		{
			WorldPacket * pck = delayedPackets.next();
			delete pck;
		}
	}

	//	SetSession(NULL);

	mSpellsUniqueTargets.clear();


	if(myCorpse)
		myCorpse = NULL;

	if(linkTarget)
		linkTarget = NULL;

	m_wratings.clear();
	m_QuestGOInProgress.clear();
	m_removequests.clear();
	m_finishedQuests.clear();
	quest_spells.clear();
	quest_mobs.clear();
	loginauras.clear();
	OnMeleeAuras.clear();

	for(std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin(); itr != m_Pets.end(); ++itr)
		delete itr->second;
	m_Pets.clear();
	
	m_itemsets.clear();
	m_channels.clear();
	mSpells.clear();

}

void Player::Destructor()
{
	delete this;
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

	case SKILL_LANG_WORGEN:
		return 69270;
		break;

	case SKILL_LANG_GOBLIN:
		return 69269;
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

	memset(m_taximask, 0, sizeof(uint32)*12);
	
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
	if(class_ == DEATHKNIGHT)
	{
		for(uint8 i=0;i<12;++i)
			m_taximask[i] |= DKNodesMask[i];
	}

	switch(race)
	{
		case RACE_TAUREN:	AddTaximaskNode(22);						break;
		case RACE_HUMAN:	AddTaximaskNode(2);							break;
		case RACE_DWARF:	AddTaximaskNode(6);							break;
		case RACE_GNOME:	AddTaximaskNode(6);							break;
		case RACE_ORC:		AddTaximaskNode(23);						break;
		case RACE_TROLL:	AddTaximaskNode(23);						break;
		case RACE_UNDEAD:	AddTaximaskNode(11);						break;
		case RACE_NIGHTELF:	{AddTaximaskNode(26); AddTaximaskNode(27);}	break;
		case RACE_BLOODELF:	AddTaximaskNode(82);						break;
		case RACE_DRAENEI:	AddTaximaskNode(94);						break;
	}
	// team dependant taxi node
	AddTaximaskNode(100-m_team);

	// Set Starting stats for char
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((race==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	//SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // this gets devided by 10
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_POWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 0 );
   
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );
	SetUInt32Value(UNIT_FIELD_MAXPOWER6, 8);

	if( sWorld.StartLevel >= uint8(class_ != DEATHKNIGHT ? 10: 55) )
	{
		//SetUInt32Value(PLAYER_CHARACTER_POINTS1, sWorld.StartLevel - uint32(class_ != DEATHKNIGHT ? 9: 55));
		SetUInt32Value(UNIT_FIELD_LEVEL,sWorld.StartLevel);
	}
	else
		SetUInt32Value(UNIT_FIELD_LEVEL,uint32(class_ != DEATHKNIGHT ? 1: 55));
	
	InitGlyphSlots();
	InitGlyphsForLevel();
	// lookup level information
	uint32 lvl = GetUInt32Value(UNIT_FIELD_LEVEL);
	lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), lvl);
	if(lvlinfo)
		ApplyLevelInfo(lvlinfo, lvl);
	
	//THIS IS NEEDED
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana );
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate );
	
	SetUInt32Value(UNIT_FIELD_BYTES_0, ( ( race ) | ( class_ << 8 ) | ( gender << 16 ) | ( powertype << 24 ) ) );
	//UNIT_FIELD_BYTES_1	(standstate) | (unk1) | (unk2) | (attackstate)
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
	//SetUInt32Value(PLAYER_CHARACTER_POINTS2,2);
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.GetMaxLevel(TO_PLAYER(this)));
  
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

	SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, 0xEEEEEEEE);

	m_StableSlotCount = 0;

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
		{
			if( sWorld.StartLevel > 1 )
			{
				_AddSkillLine(se->id, sWorld.StartLevel * 5, sWorld.StartLevel * 5 );
			}
			else
				_AddSkillLine(se->id, ss->currentval, ss->maxval);
		}
	}
	_UpdateMaxSkillCounts();
	
	_InitialReputation();

	// Add actionbars
	for(std::list<CreateInfo_ActionBarStruct>::iterator itr = info->actionbars.begin();itr!=info->actionbars.end();++itr)
	{
		setAction(itr->button, itr->action, itr->type, itr->misc);
	}
	if( GetSession()->HasGMPermissions() && sWorld.gm_force_robes )
	{
		// Force GM robes on GM's except 'az' status (if set to 1 in world.conf)
		if( strstr(GetSession()->GetPermissions(), "az") == NULL)
		{
			//We need to dupe this
			PlayerCreateInfo *GMinfo = NULL;
			GMinfo = new PlayerCreateInfo;
			memcpy(GMinfo,info, sizeof(info));

			GMinfo->items.clear();
			CreateInfo_ItemStruct itm;

			itm.protoid = 11508; //Feet
			itm.slot = 7;
			itm.amount = 1;
			GMinfo->items.push_back(itm);

			itm.protoid = 2586;//Chest
			itm.slot = 4;
			itm.amount = 1;
			GMinfo->items.push_back(itm);

			itm.protoid = 12064;//head
			itm.slot = 0;
			itm.amount = 1;
			GMinfo->items.push_back(itm);

			EquipInit(GMinfo);
		}
		else
			EquipInit(info);
	}
	else
		EquipInit(info);

	sHookInterface.OnCharacterCreate(TO_PLAYER(this));

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];
	return true;
}

void Player::EquipInit(PlayerCreateInfo *EquipInfo)
{

	for(std::list<CreateInfo_ItemStruct>::iterator is = EquipInfo->items.begin(); is!=EquipInfo->items.end(); is++)
	{
		if ( (*is).protoid != 0)
		{
			Item* item =objmgr.CreateItem((*is).protoid,TO_PLAYER(this));
			if(item != NULL)
			{
				item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,(*is).amount);
				if((*is).slot<INVENTORY_SLOT_BAG_END)
				{
					if( !GetItemInterface()->SafeAddItem(item, INVENTORY_SLOT_NOT_SET, (*is).slot) )
					{
						item->Destructor();
					}
				}
				else
				{
					if( !GetItemInterface()->AddItemToFreeSlot(item) )
					{
						item->Destructor();
					}
				}
			}
		}
	}
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

	if( m_onAutoShot )
	{
		if( m_AutoShotAttackTimer > p_time )
		{
			//OUT_DEBUG( "HUNTER AUTOSHOT 0) %i, %i", m_AutoShotAttackTimer, p_time );
			m_AutoShotAttackTimer -= p_time;
		}
		else
		{
			//OUT_DEBUG( "HUNTER AUTOSHOT 1) %i", p_time );
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

				DealDamage(TO_PLAYER(this), damage, 0, 0, 0);
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

			DealDamage(TO_PLAYER(this), damage, 0, 0, 0);
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

	if(m_CurrentVehicle)
	{
		// Update our position
		float vposx = m_CurrentVehicle->GetPositionX();
		float vposy = m_CurrentVehicle->GetPositionY();
		float vposz = m_CurrentVehicle->GetPositionZ();
		float vposo = m_CurrentVehicle->GetOrientation();
		SetPosition(vposx, vposy, vposz, vposo, false);
	}

	// Exploration
	if(mstime >= m_explorationTimer)
	{
		_EventExploration();
		m_explorationTimer = mstime + 3000;
	}

	if(m_pvpTimer)
	{
		if(!IsPvPFlagged())
		{
			StopPvPTimer();
			RemovePvPFlag();	// Reset Timer Status
		}

		if(p_time >= m_pvpTimer)
		{
			RemovePvPFlag();
			m_pvpTimer = 0;
		}
		else
			m_pvpTimer -= p_time;
	}

	if (GetMapMgr())
	{
		if( GetMapMgr()->IsCollisionEnabled() && mstime >= m_mountCheckTimer )
		{
			if( CollideInterface.IsIndoor( m_mapId, m_position.x, m_position.y, m_position.z ) )
			{
				//Mount expired?
				if(IsMounted())
				{
					// Qiraj battletanks work everywhere on map 531
					if (! (m_mapId == 531 && ( m_MountSpellId == 25953 || m_MountSpellId == 26054 || m_MountSpellId == 26055 || m_MountSpellId == 26056 )) )
						TO_UNIT(this)->Dismount();
				}
				// Now remove all auras that are only usable outdoors (e.g. Travel form)
				for(uint32 x=0;x<MAX_AURAS;x++)
				{
					if(m_auras[x] != NULL && m_auras[x]->m_spellProto && (m_auras[x]->m_spellProto->Attributes & ATTRIBUTES_ONLY_OUTDOORS))
					{
						RemoveAuraBySlot(x);
					}
				}
			}
			m_mountCheckTimer = mstime + COLLISION_MOUNT_CHECK_INTERVAL;
		}
	}

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

	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_TAXI_INTERPOLATE);

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

	Unit* pVictim = NULL;
	if(m_curSelection)
		pVictim = GetMapMgr()->GetUnit(m_curSelection);
	
	//Can't find victim, stop attacking
	if (!pVictim)
	{
		OUT_DEBUG("Player::Update:  No valid current selection to attack, stopping attack\n");
		setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		EventAttackStop();
		return;
	}

	if( !isAttackable( TO_PLAYER(this), pVictim, false ) )
	{
		setHRegenTimer(5000);
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

		if(InStealth())
		{
			RemoveAura( m_stealth );
			SetStealth(0);
		}

		if (!GetOnMeleeSpell() || offhand)
		{
			Strike( pVictim, ( offhand ? OFFHAND : MELEE ), NULL, 0, 0, 0, false, false, true);
				
		} 
		else 
		{ 
			SpellEntry *spellInfo = dbcSpell.LookupEntry(GetOnMeleeSpell());
			uint8 cn = m_meleespell_cn;
			SetOnMeleeSpell(0, 0);
			Spell* spell = NULL;
			spell = (new Spell(TO_PLAYER(this),spellInfo,true,NULL));
			SpellCastTargets targets;
			targets.m_unitTarget = GetSelection();
			spell->extra_cast_number = cn;
			spell->prepare(&targets);
		}
	}
}

void Player::_EventCharmAttack()
{
	if(!m_CurrentCharm)
		return;

	Unit* pVictim = NULL;
	if(!IsInWorld())
	{
		m_CurrentCharm=NULL;
		sEventMgr.RemoveEvents(TO_PLAYER(this),EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	if(m_curSelection == 0)
	{
		sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	pVictim= GetMapMgr()->GetUnit(m_curSelection);

	//Can't find victim, stop attacking
	if (!pVictim)
	{
		DEBUG_LOG( "WORLD"," "I64FMT" doesn't exist.",m_curSelection);
		OUT_DEBUG("Player::Update:  No valid current selection to attack, stopping attack\n");
		setHRegenTimer(5000); //prevent clicking off creature for a quick heal
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
			sEventMgr.ModifyEventTimeLeft(TO_PLAYER(this), EVENT_PLAYER_CHARM_ATTACK, 100);	
		}
		else if(!m_CurrentCharm->isInFront(pVictim))
		{
			if(m_AttackMsgTimer == 0)
			{
				m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
				m_AttackMsgTimer = 2000;		// 2 sec till next msg.
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(TO_PLAYER(this), EVENT_PLAYER_CHARM_ATTACK, 100);	
		}
		else
		{
			//if(pVictim->GetTypeId() == TYPEID_UNIT)
			//	pVictim->GetAIInterface()->StopMovement(5000);

			//pvp timeout reset
			/*if(pVictim->IsPlayer())
			{
				if( TO_PLAYER( pVictim )->DuelingWith == NULL)//Dueling doesn't trigger PVP
					TO_PLAYER( pVictim )->PvPTimeoutUpdate(false); //update targets timer

				if(DuelingWith == NULL)//Dueling doesn't trigger PVP
					PvPTimeoutUpdate(false); //update casters timer
			}*/

			if (!m_CurrentCharm->GetOnMeleeSpell())
			{
				m_CurrentCharm->Strike( pVictim, MELEE, NULL, 0, 0, 0, false, false, true );
			} 
			else 
			{ 
				SpellEntry *spellInfo = dbcSpell.LookupEntry(m_CurrentCharm->GetOnMeleeSpell());
				uint8 cn = m_meleespell_cn;
				m_CurrentCharm->SetOnMeleeSpell(0, 0);
				Spell* spell = NULL;
				spell = (new Spell(m_CurrentCharm,spellInfo,true,NULL));
				SpellCastTargets targets;
				targets.m_unitTarget = GetSelection();
				spell->extra_cast_number = cn;
				spell->prepare(&targets);
			}
		}
	}   
}

void Player::EventAttackStart()
{
	m_attacking = true;
	if( IsMounted() )
	{
		TO_UNIT(this)->Dismount();
		SetPlayerSpeed(RUN, m_runSpeed);
	}
}

void Player::EventAttackStop()
{
	if(m_CurrentCharm != NULL)
		sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_CHARM_ATTACK);

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

	uint16 AreaId = GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY(), GetPositionZ());
	
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
    sChatHandler.BlueSystemMessageToPlr(TO_PLAYER(this),areaname);*/

	uint32 offset = at->explorationFlag / 32;
	offset += PLAYER_EXPLORED_ZONES_1;

	uint32 val = (uint32)(1 << (at->explorationFlag % 32));
	uint32 currFields = GetUInt32Value(offset);

	if(AreaId != m_AreaID)
	{
		m_AreaID = AreaId;
		m_areaDBC = dbcArea.LookupEntryForced(m_AreaID);
		if (m_areaDBC && m_areaDBC->ZoneId != 0)
			m_areaDBC = dbcArea.LookupEntryForced(m_areaDBC->ZoneId);
		UpdatePvPArea();
		if(GetGroup())
            GetGroup()->UpdateOutOfRangePlayer(TO_PLAYER(this), 128, true, NULL);
	}

	if(at->ZoneId != 0)
	{
		if( m_zoneId != at->ZoneId )
			ZoneUpdate(at->ZoneId);
	}
	else if( m_zoneId != at->AreaId )
		ZoneUpdate(at->AreaId);

	bool rest_on = false;
	// Check for a restable area
    if(at->AreaFlags & AREA_CITY_AREA || at->AreaFlags & AREA_CITY)
	{
		// check faction
		if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == ALLIANCE) || (at->category == AREAC_HORDE_TERRITORY && GetTeam() == HORDE) )
		{
			rest_on = true;
		}
        else if(at->category != AREAC_ALLIANCE_TERRITORY && at->category != AREAC_HORDE_TERRITORY)
		{
			rest_on = true;
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
					rest_on = true;
				else if(at2->category != AREAC_ALLIANCE_TERRITORY && at2->category != AREAC_HORDE_TERRITORY)
				rest_on = true;
			}
		}
	}

	if (rest_on)
	{
		if(!m_isResting)
			ApplyPlayerRestState(true);
	}
	else
	{
		if(m_isResting)
		{
			if (GetMapMgr()->IsCollisionEnabled())
			{
				const LocationVector & loc = GetPosition();
				if(!CollideInterface.IsIndoor(GetMapId(), loc.x, loc.y, loc.z + 2.0f))
					ApplyPlayerRestState(false);
			}
			else
				ApplyPlayerRestState(false);
		}
	}


	if( !(currFields & val) && !GetTaxiState() && !m_TransporterGUID)//Unexplored Area		// bur: we dont want to explore new areas when on taxi
	{
		SetUInt32Value(offset, (uint32)(currFields | val));

		uint32 explore_xp = 0;
		if(getLevel() < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
			explore_xp = at->level * 10;
		WorldPacket data(SMSG_EXPLORATION_EXPERIENCE, 8);
		data << at->AreaId << explore_xp;
		m_session->SendPacket(&data);

		if(explore_xp)
			GiveXP(explore_xp, 0, false);
	}
	
	if( !GetTaxiState() && !m_TransporterGUID )
		GetAchievementInterface()->HandleAchievementCriteriaExploreArea( at->AreaId, GetUInt32Value(offset) );
}

void Player::EventDeath()
{
	if (m_state & UF_ATTACKING)
		EventAttackStop();

	if (m_onTaxi)
		sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_TAXI_DISMOUNT);

	if(!IS_INSTANCE(GetMapId()) && !sEventMgr.HasEvent(TO_PLAYER(this),EVENT_PLAYER_FORECED_RESURECT)) //Should never be true 
		sEventMgr.AddEvent(TO_PLAYER(this),&Player::EventRepopRequestedPlayer,EVENT_PLAYER_FORECED_RESURECT,PLAYER_FORCED_RESURECT_INTERVAL,1,0); //in case he forgets to release spirit (afk or something)
	
	//remove any summoned totems/summons/go's created by this player
	SummonExpireAll(false);
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

	//add reststate bonus
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

		if(level >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
			break;
	}

	if(level > GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		level = GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);

	if(levelup)
	{
		m_playedtime[0] = 0; //Reset the "Current level played time"

		LevelInfo * oldlevel = lvlinfo;
		lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), level);

		ApplyLevelInfo(lvlinfo, level);

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
	
		if( getClass() == WARLOCK && GetSummon() && GetSummon()->IsInWorld() && GetSummon()->isAlive())
		{
			GetSummon()->ModUnsigned32Value( UNIT_FIELD_LEVEL, 1 );
			GetSummon()->ApplyStatsForLevel();
		}
		InitGlyphsForLevel();

		_UpdateMaxSkillCounts();

		GetAchievementInterface()->HandleAchievementCriteriaLevelUp( getLevel() );
	}

	// Set the update bit
	SetUInt32Value(PLAYER_XP, newxp);
	
	HandleProc(PROC_ON_GAIN_EXPIERIENCE, TO_PLAYER(this), NULL);
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
		data << uint32(*sitr);				   // spell id
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

		data << uint32( itr2->first );						// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( 0 );								// spell category
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for spell)
		data << uint32( 0 );								// cooldown remaining in ms (for category)

		++itemCount;

		OUT_DEBUG("sending spell cooldown for spell %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
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

		data << uint32( itr2->second.SpellId );				// spell id
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

void Player::BuildPlayerTalentsInfo(WorldPacket *data, bool self)
{
	*data << uint32(availTalentPoints); // Unspent talents
	// TODO: probably shouldn't send both specs if target is not self
	*data << uint8(m_talentSpecsCount);
	*data << uint8(m_talentActiveSpec);
	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		PlayerSpec spec = m_specs[s];
		// Send Talents
		*data << uint8(spec.talents.size());
		std::map<uint32, uint8>::iterator itr;
		for(itr = spec.talents.begin(); itr != spec.talents.end(); itr++)
		{
			*data << uint32(itr->first);	// TalentId
			*data << uint8(itr->second);	// TalentRank
		}
		if(self)
		{
			// Send Glyph info
			*data << uint8(GLYPHS_COUNT);
			for(uint8 i = 0; i < GLYPHS_COUNT; i++)
			{
				*data << uint16(spec.glyphs[i]);
			}
		} else
		{
			*data << uint8(0);	// glyphs not sent when inspecting another player
		}
	}
}

void Player::BuildPetTalentsInfo(WorldPacket *data)
{
	Pet * pPet = NULL;
	pPet = GetSummon();
	if(pPet == NULL || !pPet->IsPet())
		return;

	*data << uint32(pPet->GetUnspentPetTalentPoints());
	*data << uint8(pPet->m_talents.size());

	for(PetTalentMap::iterator itr = pPet->m_talents.begin(); itr != pPet->m_talents.end(); ++itr)
	{
		*data << uint32(itr->first);
		*data << uint8(itr->second);
	}
}

void Player::smsg_TalentsInfo(bool pet)
{
	WorldPacket data(SMSG_TALENTS_INFO, 1000);
	data << uint8(pet?1:0);
	if(pet)
		BuildPetTalentsInfo(&data);
	else	// initialize sending all info
		BuildPlayerTalentsInfo(&data, true);

	GetSession()->SendPacket(&data);
}

void Player::_SavePet(QueryBuffer * buf)
{
	// Remove any existing info
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));
	else
		buf->AddQuery("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerpetactionbar WHERE ownerguid=%u", GetLowGUID());
	else
		buf->AddQuery("DELETE FROM playerpetactionbar WHERE ownerguid=%u", GetLowGUID());

	if(m_Summon&&m_Summon->IsInWorld()&&m_Summon->GetPetOwner()==TO_PLAYER(this))	// update PlayerPets array with current pet's info
	{
		PlayerPet*pPet = GetPlayerPet(m_Summon->m_PetNumber);
		if(!pPet || pPet->active == false)
			m_Summon->UpdatePetInfo(true);
		else m_Summon->UpdatePetInfo(false);

		if(!m_Summon->Summon)	   // is a pet
		{
			// save pet spells
			PetSpellMap::iterator itr = m_Summon->mSpells.begin();
			uint32 pn = m_Summon->m_PetNumber;
			if(buf == NULL)
				CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());
			else
				buf->AddQuery("DELETE FROM playerpetspells WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());

			for(; itr != m_Summon->mSpells.end(); ++itr)
			{
				if(buf == NULL)
					CharacterDatabase.Execute("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
				else
					buf->AddQuery("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
			}
			if(buf == NULL)
				CharacterDatabase.Execute("DELETE FROM playerpettalents WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());
			else
				buf->AddQuery("DELETE FROM playerpettalents WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());

			PetTalentMap::iterator itr2 = m_Summon->m_talents.begin();
			for(; itr2 != m_Summon->m_talents.end(); ++itr2)
			{
				if(buf == NULL)
					CharacterDatabase.Execute("INSERT INTO playerpettalents VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr2->first, itr2->second);
				else
					buf->AddQuery("INSERT INTO playerpettalents VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr2->first, itr2->second);
			}
		}
	}

	std::stringstream ss;

	for(std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin(); itr != m_Pets.end(); itr++)
	{
		ss.rdbuf()->str("");
		ss << "REPLACE INTO playerpets VALUES('"
			<< GetLowGUID() << "','"
			<< itr->second->number << "','"
			<< CharacterDatabase.EscapeString(itr->second->name) << "','"
			<< itr->second->entry << "','"
			<< itr->second->fields << "','"
			<< itr->second->xp << "','"
			<< (itr->second->active ?  1 : 0) + itr->second->stablestate * 10 << "','"
			<< itr->second->level << "','"
			<< itr->second->happiness << "','" //happiness/loyalty xp
			<< itr->second->happinessupdate << "','"
			<< (itr->second->summon ?  1 : 0) << "')";
			
		if(buf == NULL)
			CharacterDatabase.ExecuteNA(ss.str().c_str());
		else
			buf->AddQueryStr(ss.str());
		ss.rdbuf()->str("");
		ss << "REPLACE INTO playerpetactionbar VALUES('";
		// save action bar
		ss << GetLowGUID() << "','"
		<< itr->second->number << "','";
		for(uint8 i = 0; i < 10; ++i)
		{
			ss << itr->second->actionbarspell[i] << "','";
		}
		for(uint8 i = 0; i < 9; ++i)
		{
			ss << itr->second->actionbarspellstate[i] << "','";
		}
		ss << itr->second->actionbarspellstate[9] << "')";

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

		PlayerPet* pet = NULL;
		pet = new PlayerPet;
		pet->number  = fields[1].GetUInt32();
		pet->name	= fields[2].GetString();
		pet->entry   = fields[3].GetUInt32();
		pet->fields  = fields[4].GetString();
		pet->xp	  = fields[5].GetUInt32();
		pet->active  = fields[6].GetInt8()%10 > 0 ? true : false;
		pet->stablestate = fields[6].GetInt8() / 10;
		pet->level   = fields[7].GetUInt32();
		pet->happiness = fields[8].GetUInt32();
		pet->happinessupdate = fields[9].GetUInt32();
		pet->summon = (fields[10].GetUInt32()>0 ? true : false);

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

void Player::_LoadPetActionBar(QueryResult * result)
{
	if(!result)
		return;

	if(!m_Pets.size())
		return;

	do
	{
		Field *fields = result->Fetch();
		PlayerPet* pet = NULL;
		uint32 number  = fields[1].GetUInt32();
		pet = m_Pets[number];
		if(!pet)
			continue;

		for(uint8 i = 0; i < 10; ++i)
		{
			pet->actionbarspell[i] = fields[2+i].GetUInt32();
		}
		for(uint8 i = 0; i < 10; ++i)
		{
			pet->actionbarspellstate[i] = fields[12+i].GetUInt32();
		}
	}while(result->NextRow());
}

void Player::SpawnPet(uint32 pet_number)
{
	std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(pet_number);
	if(itr == m_Pets.end())
	{
		OUT_DEBUG("PET SYSTEM: "I64FMT" Tried to load invalid pet %d", GetGUID(), pet_number);
		return;
	}

	if( m_Summon != NULL )
	{
		m_Summon->Remove(true, true, true);
		m_Summon = NULL;
	}

	Pet* pPet = objmgr.CreatePet();
	pPet->SetInstanceID(GetInstanceID());
	pPet->LoadFromDB(TO_PLAYER(this), itr->second);
	if( IsPvPFlagged() )
		pPet->SetPvPFlag();
}

void Player::_LoadPetSpells(QueryResult * result)
{
	//std::stringstream query;
	//std::map<uint32, std::list<uint32>* >::iterator itr;
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
		SpellEntry * spell = dbcSpell.LookupEntry(spell_id);
		uint32 max = 1;
		switch(skill->type)
		{
			case SKILL_TYPE_PROFESSION:
				max=75*((spell->RankNumber)+1);
				availProfPoints -= 1;
				break;
			case SKILL_TYPE_SECONDARY:
				max=75*((spell->RankNumber)+1);
				break;
			case SKILL_TYPE_WEAPON:
				max=5*getLevel();
				break;
			case SKILL_TYPE_CLASS:
			case SKILL_TYPE_ARMOR:
				if(skill->id == SKILL_LOCKPICKING)
					max=5*getLevel();
				break;
		};

		_AddSkillLine(sk->skilline, 1, max);
		_UpdateMaxSkillCounts();
	}
}

//===================================================================================================================
//  Set Create Player Bits -- Sets bits required for creating a player in the updateMask.
//  Note:  Doesn't set Quest or Inventory bits
//  updateMask - the updatemask to hold the set bits
//===================================================================================================================
void Player::_SetCreateBits(UpdateMask *updateMask, Player* target) const
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


void Player::_SetUpdateBits(UpdateMask *updateMask, Player* target) const
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
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_ENTRY);
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
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER6);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER7);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER8);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER9);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXHEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER5);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER6);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER7);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER8);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER9);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_LEVEL);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FACTIONTEMPLATE);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_0);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS_2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_AURASTATE);
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
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HOVERHEIGHT);

	Player::m_visibleUpdateMask.SetBit(PLAYER_FLAGS);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_2);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_3);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILD_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_TEAM);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER+1);
	//Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDID);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDRANK);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_2);

	for(uint16 i = PLAYER_QUEST_LOG_1_1; i < PLAYER_QUEST_LOG_25_2; i+=4)
		Player::m_visibleUpdateMask.SetBit(i);

    for(uint16 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        uint32 offset = i * PLAYER_VISIBLE_ITEM_LENGTH;

        // item entry
        Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_ENTRYID + offset);
        // enchant
        Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + offset);
    }

	Player::m_visibleUpdateMask.SetBit(PLAYER_CHOSEN_TITLE);

	/*for(uint16 i = PLAYER_FIELD_ARENA_TEAM_INFO_1_1; i <= PLAYER_FIELD_ARENA_TEAM_INFO_1_17; ++i)
		Player::m_visibleUpdateMask.SetBit(i);*/

	/* fuck i hate const - burlex */
	/*if(target && target->GetGroup() == const_cast<Player* >(TO_PLAYER(this))->GetGroup() && const_cast<Player*  >(TO_PLAYER(this))->GetSubGroup() == target->GetSubGroup())
	{
	// quest fields are the same for party members
	for(uint32 i = PLAYER_QUEST_LOG_1_01; i < PLAYER_QUEST_LOG_25_2; ++i)
	Player::m_visibleUpdateMask.SetBit(i);
	}*/
}


void Player::DestroyForPlayer( Player* target ) const
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

	if(availProfPoints>2)
		availProfPoints=2;
 
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

	for(uint32 i = 0; i < 128; ++i)
		ss << m_uint32Values[PLAYER_EXPLORED_ZONES_1 + i] << ",";

	ss << "','0', "; //skip saving oldstyle skills, just fill with 0

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
		if(player_flags & PLAYER_FLAG_PVP_TIMER)
			player_flags &= ~PLAYER_FLAG_PVP_TIMER;
		if(player_flags & PLAYER_FLAG_UNKNOWN2)
			player_flags &= ~PLAYER_FLAG_UNKNOWN2;
	}

	ss << m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX] << ","
	<< m_uint32Values[PLAYER_CHOSEN_TITLE] << ","
	<< GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) << ","
	<< GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES1) << ","
	<< m_uint32Values[PLAYER_FIELD_COINAGE] << ","
	<< ammoTrash << ","
	<< availProfPoints << ","
	<< m_maxTalentPoints << ","
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
		
	for(uint32 i = 0; i < 12; i++ )
		ss << m_taximask[i] << " ";
	ss << "', "
	
	<< m_banned << ", '"
	<< CharacterDatabase.EscapeString(m_banreason) << "', "
	<< (uint32)UNIXTIME << ",";
	
	//online state
	if(GetSession()->_loggingOut || bNewCharacter)
	{
		ss << "0,";
	}
	else
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
	<< rename_pending        << ","
	<< m_arenaPoints         << ","
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

	// Dump deleted spell data to stringstream
	SpellSet::iterator spellItr;
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
	for(uint32 i = 0; i < PLAYER_ACTION_BUTTON_COUNT; ++i)
	{
		ss << uint32(mActions[i].Action) << ","
			<< uint32(mActions[i].Type) << ","
			<< uint32(mActions[i].Misc) << ",";
	}
	ss << "','";

	if(!bNewCharacter)
		SaveAuras(ss);

	//ss << LoadAuras;
	ss << "','";

	// Add player finished quests
	set<uint32>::iterator fq = m_finishedQuests.begin();
	for(; fq != m_finishedQuests.end(); ++fq)
	{
		ss << (*fq) << ",";
	}

	ss << "','";
	// Add player finished daily quests
	fq = m_finishedDailyQuests.begin();
	for(; fq != m_finishedDailyQuests.end(); ++fq)
	{
		ss << (*fq) << ",";
	}

	ss << "', ";
	ss << m_honorRolloverTime << ", ";
	ss << m_killsToday << ", " << m_killsYesterday << ", " << m_killsLifetime << ", ";
	ss << m_honorToday << ", " << m_honorYesterday << ", ";
	ss << m_honorPoints << ", ";
   	ss << iInstanceType << ", ";

	ss << uint32(m_talentActiveSpec) << ", ";
	ss << uint32(m_talentSpecsCount) << ", ";

	ss << "0, " << recustomize_pending << ")";	// force_reset_talents
	
	if(bNewCharacter)
		CharacterDatabase.WaitExecuteNA(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());

	//Save Other related player stuff

	sHookInterface.OnPlayerSaveToDB(TO_PLAYER(this), buf);

	// Skills
	_SaveSkillsToDB(buf);

	// Talents
	_SaveTalentsToDB(buf);

	// Spells
	_SaveSpellsToDB(buf);

	// Glyphs
	_SaveGlyphsToDB(buf);

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

	// Achievements
	GetAchievementInterface()->SaveToDB( buf );

	m_nextSave = getMSTime() + sWorld.getIntRate(INTRATE_SAVE);

	if(buf)
		CharacterDatabase.AddQueryBuffer(buf);
}
void Player::_SaveSkillsToDB(QueryBuffer * buf)
{
	// if we have nothing to save why save?
	if (m_skills.size() == 0)
		return;

	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerskills WHERE Player_Guid = %u", GetLowGUID() );
	else
		buf->AddQuery("DELETE FROM playerskills WHERE Player_Guid = %u", GetLowGUID() );

	std::stringstream ss;
	ss << "INSERT INTO playerskills (Player_Guid, skill_id, type, currentlvl, maxlvl ) VALUES ";
	uint32 iI = uint32(m_skills.size())-1;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end() ; ++itr)
	{
		if(itr->first)
		{
			ss	<< "(" << GetLowGUID() << ","
				<< itr->first << ","
				<< itr->second.Skill->type << ","
				<< itr->second.CurrentValue << ","
				<< itr->second.MaximumValue << ")";
			if (iI)
				ss << ",";
		}
		iI -= 1;
	}

	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

void Player::_LoadGlyphs(QueryResult * result)
{
	// init with 0s just in case
	for(uint8 s = 0; s < MAX_SPEC_COUNT; s++)
	{
		for(uint32 i=0; i < GLYPHS_COUNT; i++)
		{
			m_specs[s].glyphs[i] = 0;
		}
	}
	// Load info from DB
	if(result)
	{
		do 
		{
			Field *fields = result->Fetch();
			uint8 spec = fields[1].GetInt8();
			if(spec >= MAX_SPEC_COUNT)
			{
				sLog.outDebug("Out of range spec number [%d] for player with GUID [%d] in playerglyphs", 
					spec, fields[0].GetUInt32());
				continue;
			}
			for(uint32 i=0; i < GLYPHS_COUNT; i++)
			{
				m_specs[spec].glyphs[i] = fields[2 + i].GetUInt16();
			}
		} while(result->NextRow());
	}
}

void Player::_SaveGlyphsToDB(QueryBuffer * buf)
{
	bool empty = true;
	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		for(uint32 i=0; i < GLYPHS_COUNT; i++)
		{
			if(m_specs[s].glyphs[i] != 0)
			{
				empty = false;
				break;
			}
		}
	}
	if(empty)
		return;	// nothing to save

	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		std::stringstream ss;
		ss << "REPLACE INTO playerglyphs (guid, spec, glyph1, glyph2, glyph3, glyph4, glyph5, glyph6) VALUES "
			<< "(" << GetLowGUID() << ","
			<< uint32(s) << ",";
		for(uint32 i = 0; i < GLYPHS_COUNT; i++)
		{
			ss << uint32(m_specs[s].glyphs[i]);
			if(i != GLYPHS_COUNT - 1)
				ss << ",";
			else
				ss << ")";
		}

		if(buf == NULL)
			CharacterDatabase.Execute(ss.str().c_str());
		else
			buf->AddQueryStr(ss.str());
	}
}

void Player::_LoadSpells(QueryResult * result)
{
	if(result)
	{
		do 
		{
			Field *fields = result->Fetch();
			SpellEntry * spProto = dbcSpell.LookupEntryForced(fields[0].GetInt32());
			if(spProto)
				mSpells.insert(spProto->Id);
		} while(result->NextRow());
	}
}

void Player::_SaveSpellsToDB(QueryBuffer * buf)
{
	// delete old first
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerspells WHERE guid = %u", GetLowGUID() );
	else
		buf->AddQuery("DELETE FROM playerspells WHERE guid = %u", GetLowGUID() );

	// Dump spell data to stringstream
	std::stringstream ss;
	ss << "INSERT INTO playerspells (guid, spellid) VALUES ";
	SpellSet::iterator spellItr = mSpells.begin();
	bool first = true;
	for(; spellItr != mSpells.end(); ++spellItr)
	{
		SpellEntry * sp = dbcSpell.LookupEntry( *spellItr );
		if( !sp || sp->RequiredShapeShift == FORM_ZOMBIE )
			continue;
		if(!first)
			ss << ",";
		else
			first = false;
		ss << "("<< GetLowGUID() << "," << uint32(*spellItr) << ")";
	}
	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

void Player::_LoadTalents(QueryResult * result)
{
	// Load info from DB
	uint32 talentId;
	uint8 talentRank;
	if(result)
	{
		do 
		{
			Field *fields = result->Fetch();
			uint8 spec = fields[0].GetInt8();
			if(spec >= MAX_SPEC_COUNT)
			{
				sLog.outDebug("Out of range spec number [%d] for player with GUID [%d] in playertalents", 
					spec, GetLowGUID());
				continue;
			}
			talentId = fields[1].GetUInt32();
			talentRank = fields[2].GetUInt8();
			m_specs[spec].talents.insert(make_pair(talentId, talentRank));
		} while(result->NextRow());
	}
}

void Player::_SaveTalentsToDB(QueryBuffer * buf)
{
	// delete old talents first
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playertalents WHERE guid = %u", GetLowGUID() );
	else
		buf->AddQuery("DELETE FROM playertalents WHERE guid = %u", GetLowGUID() );

	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		if(s > MAX_SPEC_COUNT)
			break;
		std::map<uint32, uint8> *talents = &m_specs[s].talents;
		std::map<uint32, uint8>::iterator itr;
		for(itr = talents->begin(); itr != talents->end(); itr++)
		{
			std::stringstream ss;
			ss << "INSERT INTO playertalents (guid, spec, tid, rank) VALUES "
				<< "(" << GetLowGUID() << "," 
				<< uint32(s) << "," 
				<< itr->first << ","
				<< uint32(itr->second) << ")";
			if(buf == NULL)
				CharacterDatabase.Execute(ss.str().c_str());
			else
				buf->AddQueryStr(ss.str());
		}
	}
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
		if( disarmed )
			return false;

		if(GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND))
		{
			if((int32)GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (!((1 << GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->SubClass) 
							& m_spellInfo->EquippedItemSubClass))
							return false;
					}
				}
			}
		}
		else if(m_spellInfo->EquippedItemSubClass == 173555)
			return false;

		if (GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED))
		{
			if((int32)GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (!((1 << GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->SubClass) 
							& m_spellInfo->EquippedItemSubClass))
							return false;
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
	Destructor();
	//delete this;
}

bool Player::LoadFromDB(uint32 guid)
{
	AsyncQuery * q = NULL;
	q = new AsyncQuery( new SQLClassCallbackP0<Player>(TO_PLAYER(this), &Player::LoadFromDBProc) );
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

	//Achievements
	q->AddQuery("SELECT * from achievements WHERE player = %u", guid);

	//skills
	q->AddQuery("SELECT * FROM playerskills WHERE player_guid = %u AND type <> %u ORDER BY skill_id ASC, currentlvl DESC", guid,SKILL_TYPE_LANGUAGE ); //load skill, skip languages

	//pet action bar
	q->AddQuery("SELECT * FROM playerpetactionbar WHERE ownerguid=%u ORDER BY petnumber", guid);

	//Talents
	q->AddQuery("SELECT spec, tid, rank FROM playertalents WHERE guid = %u", guid);
	
	//Glyphs
	q->AddQuery("SELECT * FROM playerglyphs WHERE guid = %u", guid);

	//Spells
	q->AddQuery("SELECT spellid FROM playerspells WHERE guid = %u", guid);

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

	if(GetSession() == NULL || results.size() < 8)		// should have 8 query results for aplayer load.
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

	uint32 banned = fields[33].GetUInt32();
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
	uint8 plvl = get_next_field.GetUInt32();
	m_uint32Values[UNIT_FIELD_LEVEL] = plvl;

	// level dependant taxi node
	SetTaximaskNode(213,plvl>=68?false:true);	//Add 213 (Shattered Sun Staging Area) if lvl >=68

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
	while(Counter <128) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value(PLAYER_EXPLORED_ZONES_1 + Counter, atol(start));
		start = end +1;
		Counter++;
	}

	QueryResult *checkskills = results[12].result;
	if(checkskills)
	{
		_LoadSkills(results[12].result);
		field_index++;
		DEBUG_LOG("WorldSession","Skills loaded");
	}
	else 
	{
		// old format
		Counter = 0;
		start = (char*)get_next_field.GetString();//buff old system;
		const ItemProf * prof;
		if(!strchr(start, ' ') && !strchr(start,';'))
		{
			// no skills - reset to defaults 
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
					if (v1 == SKILL_RIDING)
						sk.CurrentValue = sk.MaximumValue;
					m_skills.insert(make_pair(v1, sk));
	
					prof = GetProficiencyBySkill(v1);
					if(prof)
					{
						if(prof->itemclass==4)
							armor_proficiency|=prof->subclass;
						else
							weapon_proficiency|=prof->subclass;
					}
				}
			}
			free(f);
			_UpdateMaxSkillCounts();
			DEBUG_LOG("Player","loaded old style skills for player %s", m_name.c_str());
		}
	}


	// set the rest of the shit
	m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX]  = get_next_field.GetUInt32();
	m_uint32Values[ PLAYER_CHOSEN_TITLE ]					= get_next_field.GetUInt32();
	SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, get_next_field.GetUInt64() );
	SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES1, get_next_field.GetUInt64() );
	m_uint32Values[PLAYER_FIELD_COINAGE]				= get_next_field.GetUInt32();
	ammoTrash					  = get_next_field.GetUInt32();
	availProfPoints			= get_next_field.GetUInt32();
	m_maxTalentPoints								= get_next_field.GetUInt16();
	load_health										 = get_next_field.GetUInt32();
	load_mana										   = get_next_field.GetUInt32();
	
	uint8 pvprank = get_next_field.GetUInt8();
	SetUInt32Value(PLAYER_BYTES, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_BYTES_2, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_BYTES_3, getGender() | (pvprank << 24));
	SetUInt32Value(PLAYER_FLAGS, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_FIELD_BYTES, get_next_field.GetUInt32());

	m_position.x										= get_next_field.GetFloat();
	m_position.y										= get_next_field.GetFloat();
	m_position.z										= get_next_field.GetFloat();
	m_position.o										= get_next_field.GetFloat();

	m_mapId											 = get_next_field.GetUInt32();
	m_zoneId											= get_next_field.GetUInt32();

	// Calculate the base stats now they're all loaded
	for(uint32 i = 0; i < 5; ++i)
		CalcStat(i);

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
	SetUInt32Value(UNIT_FIELD_POWER6, 8);
	SetUInt32Value(UNIT_FIELD_POWER7, 0);
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );
	if(getClass() == WARRIOR)
		SetShapeShift(FORM_BATTLESTANCE);

	// We're players!
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
	field_index++; //Player is Online!
	
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
	if(load_health && m_deathState == JUST_DIED)
	{
		m_deathState = CORPSE;
		load_health = 0;
	}
	SetUInt32Value(UNIT_FIELD_HEALTH, load_health);

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
		Transporter* t = objmgr.GetTransporter(GUID_LOPART(m_TransporterGUID));
		m_TransporterGUID = t ? t->GetGUID() : 0;
	}

	m_TransporterX = get_next_field.GetFloat();
	m_TransporterY = get_next_field.GetFloat();
	m_TransporterZ = get_next_field.GetFloat();

	start = (char*)get_next_field.GetString();//buff;
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SpellEntry * spProto = NULL;
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

		rep = NULL;
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
	while(Counter < PLAYER_ACTION_BUTTON_COUNT)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Action = (uint16)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Type = (uint8)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter++].Misc = (uint8)atol(start);
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
		if(la.id != 43869 && la.id != 43958)
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
	GetAchievementInterface()->HandleAchievementCriteriaQuestCount( uint32(m_finishedQuests.size()));

	DailyMutex.Acquire();
	start =  (char*)get_next_field.GetString();
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + uint32(m_finishedDailyQuests.size()), atol(start));
		m_finishedDailyQuests.insert(atol(start));
		start = end +1;
	}
	DailyMutex.Release();
	
	m_honorRolloverTime = get_next_field.GetUInt32();
	m_killsToday = get_next_field.GetUInt32();
	m_killsYesterday = get_next_field.GetUInt32();
	m_killsLifetime = get_next_field.GetUInt32();
	
	m_honorToday = get_next_field.GetUInt32();
	m_honorYesterday = get_next_field.GetUInt32();
	m_honorPoints = get_next_field.GetUInt32();

	RolloverHonor();
    iInstanceType = get_next_field.GetUInt32();

	HonorHandler::RecalculateHonorFields(TO_PLAYER(this));
	
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);
  
	_setFaction();
	InitGlyphSlots();
	InitGlyphsForLevel();
   
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
	case DEATHKNIGHT: 
		armor_proficiency|=(1<<10);//SIGILS
		break;
	case WARLOCK:
	case HUNTER:
		_LoadPet(results[5].result);
        _LoadPetSpells(results[6].result);
		_LoadPetActionBar(results[13].result);
	break;
	}

	if(m_session->CanUseCommand('c'))
		_AddLanguages(true);
	else
		_AddLanguages(false);

	OnlineTime	= (uint32)UNIXTIME;
	if(GetGuildId())
		SetUInt32Value(PLAYER_GUILD_TIMESTAMP, (uint32)UNIXTIME);

	m_talentActiveSpec = get_next_field.GetUInt32();
	m_talentSpecsCount = get_next_field.GetUInt32();
	if(m_talentSpecsCount > MAX_SPEC_COUNT)
		m_talentSpecsCount = MAX_SPEC_COUNT;
	if(m_talentActiveSpec >= m_talentSpecsCount )
		m_talentActiveSpec = 0;

	bool needTalentReset = get_next_field.GetBool();
	if( needTalentReset )
		Reset_Talents();

	recustomize_pending = get_next_field.GetBool();

#undef get_next_field

	// load properties
	_LoadTalents(results[14].result);
	_LoadGlyphs(results[15].result);
	_LoadSpells(results[16].result);
	_LoadTutorials(results[1].result);
	_LoadPlayerCooldowns(results[2].result);
	_LoadQuestLogEntry(results[3].result);
	m_ItemInterface->mLoadItemsFromDatabase(results[4].result);
	m_mailBox->Load(results[7].result);

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
	
	// Load achievements
	GetAchievementInterface()->LoadFromDB( results[11].result );

	// Set correct maximum level
	uint32 maxLevel = sWorld.GetMaxLevel(this);
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, maxLevel);

	UpdateTalentInspectBuffer();
	SetFlag(UNIT_FIELD_FLAGS_2,UNIT_FLAG2_REGENERATE_POWER); // enables automatic power regen
	m_session->FullLogin(this);
	if(m_session)
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
			
			entry = NULL;
			entry = new QuestLogEntry;
			entry->Init(quest, TO_PLAYER(this), slot);
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

	// If we join an invalid instance and get booted out, TO_PLAYER(this) will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld();
	
	// Add failed.
	if(m_mapMgr == NULL)
	{
		DEBUG_LOG("WorldSession","Adding player %s to map %u failed.",GetName(),GetMapId());
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::AddToWorld(MapMgr* pMapMgr)
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

	// If we join an invalid instance and get booted out, TO_PLAYER(this) will prevent our stats from doubling :P
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

	EventHealthChangeSinceLastUpdate(); // just in case
}

void Player::OnPrePushToWorld()
{
	SendInitialLogonPackets();
}

void Player::OnPushToWorld()
{
	// Process create packet
	if( m_mapMgr != NULL )
		ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);

	if(m_TeleportState == 2)   // Worldport Ack
		OnWorldPortAck();

	ResetSpeedHack();
	m_beingPushed = false;
	AddItemsToWorld();
	m_lockTransportVariables = false;

	// delay the unlock movement packet
	WorldPacket * data = NULL;
	data = new WorldPacket(SMSG_TIME_SYNC_REQ, 4);
	*data << uint32(0);
	delayedPackets.add(data);
	sWorld.mInWorldPlayerCount++;

	Unit::OnPushToWorld();

	// Update PVP Situation
	LoginPvPSetup();
   
	if(m_FirstLogin)
	{
		sHookInterface.OnFirstEnterWorld(TO_PLAYER(this));
		m_FirstLogin = false;
	}

	// force area update (needed for world states)
	ForceAreaUpdate();

	// send world states
	if( m_mapMgr != NULL )
		m_mapMgr->GetStateManager().SendWorldStates(TO_PLAYER(this));

	// execute some of zeh hooks
	sHookInterface.OnEnterWorld(TO_PLAYER(this));
	sHookInterface.OnZone(TO_PLAYER(this), m_zoneId, 0);
	CALL_INSTANCE_SCRIPT_EVENT( m_mapMgr, OnZoneChange )( TO_PLAYER(this), m_zoneId, 0 );
	CALL_INSTANCE_SCRIPT_EVENT( m_mapMgr, OnPlayerEnter )( TO_PLAYER(this) );

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
	sWeatherMgr.SendWeather(TO_PLAYER(this));

	if( load_health > 0 )
	{
		if( load_health > m_uint32Values[UNIT_FIELD_MAXHEALTH] )
			SetUInt32Value(UNIT_FIELD_HEALTH, m_uint32Values[UNIT_FIELD_MAXHEALTH] );
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
	//sEventMgr.AddEvent(TO_PLAYER(this), &Player::EventDumpCompressedMovement, EVENT_PLAYER_FLUSH_MOVEMENT, World::m_movementCompressInterval, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	MovementCompressor->AddPlayer(TO_PLAYER(this));
#endif

	if( m_mapMgr != NULL && m_mapMgr->m_battleground != NULL && m_bg != m_mapMgr->m_battleground )
	{
		m_bg = m_mapMgr->m_battleground;
		m_bg->PortPlayer( TO_PLAYER(this), true );
	}

	if( m_bg != NULL && m_mapMgr != NULL )
		m_bg->OnPlayerPushed( TO_PLAYER(this) );

	z_axisposition = 0.0f;
	m_changingMaps = false;

	SendPowerUpdate();
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
	EndDuel( 0 );

	if( m_CurrentCharm && m_CurrentCharm != m_CurrentVehicle )
		UnPossess();

	if( m_uint32Values[UNIT_FIELD_CHARMEDBY] != 0 && IsInWorld() )
	{
		Player* charmer = m_mapMgr->GetPlayer(m_uint32Values[UNIT_FIELD_CHARMEDBY]);
		if( charmer != NULL )
			charmer->UnPossess();
	}

	m_resurrectHealth = 0;
	m_resurrectMana = 0;
	resurrector = 0;

	// just in case
	m_uint32Values[UNIT_FIELD_FLAGS] &= ~(UNIT_FLAG_SKINNABLE);

	//Cancel any forced teleport pending.
	if(raidgrouponlysent)
	{
		event_RemoveEvents(EVENT_PLAYER_EJECT_FROM_INSTANCE);
		raidgrouponlysent = false;
	}

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];

	if(m_bg)
	{
		m_bg->RemovePlayer(TO_PLAYER(this), true);
		m_bg = NULL;
	}

	// Cancel trade if it's active.
	Player* pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->ResetTradeVariables();

		ResetTradeVariables();
	}
	//clear buyback
	GetItemInterface()->EmptyBuyBack();

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
			if(m_SummonedObject->GetTypeId() != TYPEID_PLAYER)
			{
				if(m_SummonedObject->IsInWorld())
				{
					m_SummonedObject->RemoveFromWorld(true);
				}
				m_SummonedObject->Destructor();
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
	MovementCompressor->RemovePlayer(TO_PLAYER(this));
	m_movementBufferLock.Acquire();
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
	//sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_FLUSH_MOVEMENT);
	
#endif

	if(GetTaxiState())
		event_RemoveEvents( EVENT_PLAYER_TAXI_INTERPOLATE );

	if( m_CurrentTransporter && !m_lockTransportVariables )
	{
		m_CurrentTransporter->RemovePlayer(TO_PLAYER(this));
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;
	}

	if( m_CurrentVehicle )
	{
		m_CurrentVehicle->RemovePassenger(TO_PLAYER(this));
		m_CurrentVehicle = NULL;
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

	//fast check to skip mod applying if the item dosen't meat the requirements.
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

	switch (setid)
	{
		// * Gladiator's Battlegear
	case 701:
	case 736:
	case 567:
		setid = 746;
		break;

		// * Gladiator's Dreadgear
	case 702:
	case 734:
	case 568:
		setid = 734;
		break;

		// * Gladiator's Earthshaker
	case 703:
	case 732:
	case 578:
		setid = 734;
		break;

		// * Gladiator's Felshroud
	case 704:
	case 735:
	case 615:
		setid = 735;
		break;

		// * Gladiator's Investiture
	case 705:
	case 278:
	case 687:
		setid = 728;
		break;

		// * Gladiator's Pursuit
	case 706:
	case 723:
	case 586:
		setid = 723;
		break;

		// * Gladiator's Raiment
	case 707:
	case 729:
	case 581:
		setid = 729;
		break;

		// * Gladiator's Redemption
	case 708:
	case 725:
	case 690:
		setid = 725;
		break;

		// * Gladiator's Refuge
	case 709:
	case 720:
	case 685:
		setid = 720;
		break;

		// * Gladiator's Regalia 
	case 710: 
	case 724: 
	case 579: 
		setid = 724; 
		break; 

		// * Gladiator's Sanctuary 
	case 711: 
	case 721: 
	case 584: 
		setid = 721; 
		break; 

		// * Gladiator's Thunderfist 
	case 712: 
	case 733: 
	case 580: 
		setid = 733; 
		break; 

		// * Gladiator's Vestments 
	case 713: 
	case 730: 
	case 577: 
		setid = 730; 
		break; 

		// * Gladiator's Vindication 
	case 714: 
	case 726: 
	case 583: 
		setid = 726; 
		break; 

		// * Gladiator's Wartide 
	case 715: 
	case 731: 
	case 686: 
		setid = 731; 
		break; 

		// * Gladiator's Wildhide 
	case 716: 
	case 722: 
	case 585: 
		setid = 722; 
		break; 
	} 

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
						Spell* spell = NULL;
						spell = (new Spell( TO_PLAYER(this), info, true, NULL ));
						SpellCastTargets targets;
						targets.m_unitTarget = GetGUID();
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
		if( apply )
		{
			BaseResistance[RESISTANCE_ARMOR]+= proto->Armor;
		}
		else
		{
			BaseResistance[RESISTANCE_ARMOR] -= proto->Armor;
		}
		CalcResistance(RESISTANCE_ARMOR);
	}

	// Resistances
	//TODO: FIXME: can there be negative resistances from items?
	if( proto->FireRes )
	{
		if( apply )
			FlatResistanceModifierPos[RESISTANCE_FIRE] += proto->FireRes;
		else
			FlatResistanceModifierPos[RESISTANCE_FIRE] -= proto->FireRes;
		CalcResistance(RESISTANCE_FIRE);
	}

	if( proto->NatureRes )

	{
		if( apply )
			FlatResistanceModifierPos[RESISTANCE_NATURE] += proto->NatureRes;
		else
			FlatResistanceModifierPos[RESISTANCE_NATURE] -= proto->NatureRes;
		CalcResistance(RESISTANCE_NATURE);
	}

	if( proto->FrostRes )
	{
		if( apply )
			FlatResistanceModifierPos[RESISTANCE_FROST] += proto->FrostRes;
		else
			FlatResistanceModifierPos[RESISTANCE_FROST] -= proto->FrostRes;
		CalcResistance(RESISTANCE_FROST);	
	}

	if( proto->ShadowRes )
	{
		if( apply )
			FlatResistanceModifierPos[RESISTANCE_SHADOW] += proto->ShadowRes;
		else
			FlatResistanceModifierPos[RESISTANCE_SHADOW] -= proto->ShadowRes;
		CalcResistance(RESISTANCE_SHADOW);	
	}

	if( proto->ArcaneRes )
	{
		if( apply )
			FlatResistanceModifierPos[RESISTANCE_ARCANE] += proto->ArcaneRes;
		else
			FlatResistanceModifierPos[RESISTANCE_ARCANE] -= proto->ArcaneRes;
		CalcResistance(RESISTANCE_ARCANE);
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
				BaseDamage[0] = apply ? proto->Damage[0].Min : 0;
				BaseDamage[1] = apply ? proto->Damage[0].Max : 0;
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

				Spell* spell = NULL;
				spell = (new Spell( TO_PLAYER(this), spells ,true, NULL ));
				SpellCastTargets targets;
				targets.m_unitTarget = this->GetGUID();
				spell->castedItemId = item->GetEntry();
				spell->prepare( &targets );

			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				ProcTriggerSpell ts;
				memset(&ts, 0, sizeof(ProcTriggerSpell));
				ts.origId = 0;
				ts.spellId = item->GetProto()->Spells[k].Id;
				ts.procChance = 5;
				ts.caster = this->GetGUID();
				ts.procFlags = PROC_ON_MELEE_ATTACK;
				if(slot == EQUIPMENT_SLOT_MAINHAND)
					ts.weapon_damage_type = 1; // Proc only on main hand attacks
				else if(slot == EQUIPMENT_SLOT_OFFHAND)
					ts.weapon_damage_type = 2; // Proc only on off hand attacks
				else
					ts.weapon_damage_type = 0; // Doesn't depend on weapon
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
	if( value < 0.1f )
		value = 0.1f;

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
	SetUInt32Value(UNIT_FIELD_HEALTH, 1 );

	//8326 --for all races but ne, 20584--ne
	SpellCastTargets tgt;
	tgt.m_unitTarget=this->GetGUID();
   
	if(getRace()==RACE_NIGHTELF)
	{
		SpellEntry *inf = dbcSpell.LookupEntry(20584);
		Spell* sp = NULL;
		sp = (new Spell(TO_PLAYER(this),inf,true,NULL));
		sp->prepare(&tgt);
	}
	else
	{
	
		SpellEntry *inf=dbcSpell.LookupEntry(8326);
		Spell* sp = NULL;
		sp = (new Spell(TO_PLAYER(this),inf,true,NULL));
		sp->prepare(&tgt);
	}

	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);
	
	SetFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE);

	SetMovement(MOVE_UNROOT, 1);
	SetMovement(MOVE_WATER_WALK, 1);
}

Corpse* Player::RepopRequestedPlayer()
{
	if( myCorpse != NULL )
	{
		GetSession()->SendNotification( NOTIFICATION_MESSAGE_NO_PERMISSION );
		return NULL;
	}

	if( m_CurrentTransporter != NULL )
	{
		m_CurrentTransporter->RemovePlayer( TO_PLAYER(this) );
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;

		ResurrectPlayer(NULL);
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		return NULL;
	}

	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_FORECED_RESURECT ); //in case somebody resurrected us before this event happened

	// Set death state to corpse, that way players will lose visibility
	setDeathState( CORPSE );
	
	// Update visibility, that way people wont see running corpses :P
	UpdateVisibility();

	// If we're in battleground, remove the skinnable flag.. has bad effects heheh
	RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE );

	bool corpse = (m_bg != NULL) ? m_bg->CreateCorpse( TO_PLAYER(this) ) : true;
	Corpse* ret = NULL;

	if( corpse )
		ret = CreateCorpse();
	
	BuildPlayerRepop();

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

	MapInfo * pPMapinfo = NULL;
	pPMapinfo = WorldMapInfoStorage.LookupEntry( GetMapId() );
	if( pPMapinfo != NULL )
	{
		if( pPMapinfo->type == INSTANCE_NULL || pPMapinfo->type == INSTANCE_PVP )
		{
			RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		}
		else
		{
			RepopAtGraveyard( pPMapinfo->repopx, pPMapinfo->repopy, pPMapinfo->repopz, pPMapinfo->repopmapid );
		}
	}
	else
	{
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
	}

	return ret;
}

void Player::ResurrectPlayer(Player* pResurrector)
{
	sEventMgr.RemoveEvents(TO_PLAYER(this),EVENT_PLAYER_FORECED_RESURECT); //in case somebody resurected us before this event happened
	if( m_resurrectHealth )
		SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)min( m_resurrectHealth, m_uint32Values[UNIT_FIELD_MAXHEALTH] ) );
	if( m_resurrectMana )
		SetUInt32Value( UNIT_FIELD_POWER1, (uint32)min( m_resurrectMana, m_uint32Values[UNIT_FIELD_MAXPOWER1] ) );

	m_resurrectHealth = m_resurrectMana = 0;

	SpawnCorpseBones();
	
	if(getRace()==RACE_NIGHTELF)
	{
		RemoveAura(20584);
	}else
		RemoveAura(8326);

	RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE);
	setDeathState(ALIVE);
	UpdateVisibility();
	SetMovement(MOVE_LAND_WALK, 1);

	if(IsInWorld() && pResurrector != NULL && pResurrector->IsInWorld())
	{
		//make sure corpse and resurrector are on the same map.
		if( GetMapId() == pResurrector->GetMapId() )
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
	}
	else
	{
		// update player counts in arenas
		if( m_bg != NULL && m_bg->IsArena() )
			TO_ARENA(m_bg)->UpdatePlayerCounts();			
	}
}

void Player::KillPlayer()
{
	setDeathState(JUST_DIED);

	// Battleground stuff
	if(m_bg)
		m_bg->HookOnPlayerDeath(TO_PLAYER(this));

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
	if(this->getClass() == DEATHKNIGHT)
		SetUInt32Value(UNIT_FIELD_POWER7, 0);

	// combo points reset upon death
	NullComboPoints();

	GetAchievementInterface()->HandleAchievementCriteriaConditionDeath();
	GetAchievementInterface()->HandleAchievementCriteriaDeath();

	sHookInterface.OnDeath(TO_PLAYER(this));
}

Corpse* Player::CreateCorpse()
{
	Corpse* pCorpse;
	uint32 _uf, _pb, _pb2, _cfb1, _cfb2;

	objmgr.DelinkPlayerCorpses(TO_PLAYER(this));
	pCorpse = objmgr.CreateCorpse();
	pCorpse->SetInstanceID(GetInstanceID());
	pCorpse->Create(TO_PLAYER(this), GetMapId(), GetPositionX(),
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
	Item* pItem;
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
	Corpse* pCorpse;
	pCorpse = objmgr.GetCorpseByOwner(GetLowGUID());
	myCorpse = NULL;
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
	Item* pItem;

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

	if(m_bg && m_bg->HookHandleRepop(TO_PLAYER(this)))
	{
		return;
	}
	else
	{
		itr = GraveyardStorage.MakeIterator();
		while(!itr->AtEnd())
		{
			GraveyardTeleport *pGrave = itr->Get();
			if(pGrave->MapId == mapid && pGrave->FactionID == GetTeam() ||
			   pGrave->MapId == mapid && pGrave->FactionID == 3)
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

	if(sHookInterface.OnRepop(TO_PLAYER(this)) && dest.x != 0 && dest.y != 0 && dest.z != 0)
	{
		SafeTeleport(mapid, 0, dest);
	}
}

void Player::JoinedChannel(uint32 channelId)
{
#ifndef CLUSTERING
	Channel * pChannel;
	pChannel = channelmgr.GetChannel(channelId);
	if(!pChannel)
		return;
#endif
	m_channels.insert(channelId);
}

void Player::LeftChannel(uint32 channelId)
{
#ifndef CLUSTERING
	Channel * pChannel;
	pChannel = channelmgr.GetChannel(channelId);
	if(!pChannel)
		return;
#endif
	m_channels.erase(channelId);
}

void Player::CleanupChannels()
{
#ifdef CLUSTERING
	std::set<uint32>::iterator i;
	std::vector<uint32> channels;
	uint32 cid;

	for(i = m_channels.begin(); i != m_channels.end();)
	{
		cid = *i;
		++i;
		channels.push_back(cid);
	}
	WorldPacket data(ICMSG_CHANNEL_UPDATE, (sizeof(std::vector<uint32>::size_type) * channels.size()) + 5);
	data << uint8(PART_ALL_CHANNELS); //part all channels
	data << GetLowGUID();
	data << channels;
	sClusterInterface.SendPacket(&data);
#else
	set<uint32>::iterator i;
	Channel * c;
	uint32 cid;
	for(i = m_channels.begin(); i != m_channels.end();)
	{
		cid = *i;
		++i;
		
		c = channelmgr.GetChannel(cid);
		if( c != NULL )
			c->Part(TO_PLAYER(this), false);
	}
#endif
}

void Player::SendInitialActions()
{
	uint8 buffer[PLAYER_ACTION_BUTTON_SIZE + 1];
	StackPacket data(SMSG_ACTION_BUTTONS, buffer, PLAYER_ACTION_BUTTON_SIZE + 1);
	data << uint8(1);	// some bool - 0 or 1. seems to work both ways
	data.Write((uint8*)&mActions, PLAYER_ACTION_BUTTON_SIZE);
	m_session->SendPacket(&data);
	//m_session->OutPacket(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE, &mActions);
}

void Player::setAction(uint8 button, uint16 action, uint8 type, uint8 misc)
{
	if( button > PLAYER_ACTION_BUTTON_COUNT - 1 )
		return;

	mActions[button].Action = action;
	mActions[button].Type = type;
	mActions[button].Misc = misc;
}

//Groupcheck
bool Player::IsGroupMember(Player* plyr)
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
	m_finishedQuests.insert(quest_id);
	GetAchievementInterface()->HandleAchievementCriteriaQuestCount( uint32(m_finishedQuests.size()));
}

bool Player::HasFinishedQuest(uint32 quest_id)
{
	return (m_finishedQuests.find(quest_id) != m_finishedQuests.end());
}

void Player::AddToFinishedDailyQuests(uint32 quest_id)
{
	if(m_finishedDailyQuests.size() >= 25)
		return;

	DailyMutex.Acquire();
	SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + uint32(m_finishedDailyQuests.size()), quest_id);
	m_finishedDailyQuests.insert(quest_id);
	DailyMutex.Release();
}

void Player::ResetDailyQuests()
{
	m_finishedDailyQuests.clear();
	for(uint32 i = 0; i < 25; i++)
		SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + i, 0);
}

bool Player::HasFinishedDailyQuest(uint32 quest_id)
{
	return (m_finishedDailyQuests.find(quest_id) != m_finishedDailyQuests.end());
}
void Player::_LoadSkills(QueryResult * result)
{	
	int32 proff_counter = 2;
	if(result)
	{
		const ItemProf * prof;
		do 
		{
			PlayerSkill sk;
			uint32 v1;
			Field *fields = result->Fetch();
			v1 = fields[1].GetUInt32();
			sk.Reset(v1);

			if ( sWorld.CheckProfessions && fields[2].GetUInt32() == SKILL_TYPE_PROFESSION )
			{
				proff_counter--;
				if( proff_counter < 0 )
				{
					proff_counter = 0;
					
					sCheatLog.writefromsession(GetSession(),"Bug_Primary_Professions Player %s [%d] \n", GetName(), GetLowGUID());

					const char * message = "Your character has more then 2 primary professions.\n You have 5 minutes remaining to unlearn and relog.";

					// Send warning after 2 minutes, as he might miss it if it's send inmedeately.
					sEventMgr.AddEvent( TO_PLAYER(this), &Player::_Warn, message, EVENT_UNIT_SENDMESSAGE, 60000, 1, 0);
					sEventMgr.AddEvent( TO_PLAYER(this), &Player::_Kick, EVENT_PLAYER_KICK, 360000, 1, 0 );
				}
			}
			sk.CurrentValue = fields[3].GetUInt32();
			sk.MaximumValue = fields[4].GetUInt32();
			m_skills.insert(make_pair(v1,sk));

			prof = GetProficiencyBySkill(v1);
			if(prof)
			{
				if(prof->itemclass==4)
					armor_proficiency|=prof->subclass;
				else
					weapon_proficiency|=prof->subclass;
			}
		} while(result->NextRow());
	}
	else // no result from db set up from create_info instead
	{
		for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
		{
			if(ss->skillid && ss->currentval && ss->maxval && !::GetSpellForLanguage(ss->skillid))
				_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);		
		}
	}
	//Update , GM's can still learn more
	//SetUInt32Value( PLAYER_CHARACTER_POINTS2, ( GetSession()->HasGMPermissions()? 2 : proff_counter ) );
	availProfPoints += GetSession()->HasGMPermissions() ? 2 : proff_counter;
	_UpdateMaxSkillCounts();
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
{//TO_PLAYER(TO_PLAYER(this))->getClass() == HUNTER ||
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
	uint32 pLevel = (getLevel() > 80) ? 80 : getLevel();

	float tmp = 0;
	float defence_contribution = 0;

	// defence contribution estimate
	defence_contribution = ( float( _GetSkillLineCurrent( SKILL_DEFENSE, true ) ) - ( float( pLevel ) * 5.0f ) ) * 0.04f;
	defence_contribution += CalcRating( PLAYER_RATING_MODIFIER_DEFENCE ) * 0.04f;

	// dodge // Use lvl 70 ratio for levels < 75 and 80 ratio for >= 75 for now
	tmp = baseDodge[pClass] + float( GetUInt32Value( UNIT_FIELD_AGILITY ) / dodgeRatio[(pLevel < 75) ? 69 : 79][pClass] ); 
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
	if(pClass == DEATHKNIGHT) // DK gets 1/4 of strength as parry rating
	{
		tmp += CalcPercentForRating(PLAYER_RATING_MODIFIER_PARRY, GetUInt32Value(UNIT_FIELD_STAT0) / 4);
	}
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
	Item* weap ;
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
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, uint32(( speed * m_meleeattackspeedmod ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) ) / 100.0f ) ));
	
	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( weap != NULL && weap->GetProto()->Class == ITEM_CLASS_WEAPON )
	{
		speed = weap->GetProto()->Delay;
		SetUInt32Value( UNIT_FIELD_BASEATTACKTIME + 1, uint32(( speed * m_meleeattackspeedmod ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) ) / 100.0f ) ));
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
	int32 stam = 0;
	int32 intl = 0;

	uint32 str = GetUInt32Value(UNIT_FIELD_STAT0);
	uint32 agi = GetUInt32Value(UNIT_FIELD_STAT1);
	uint32 lev = getLevel();

	// Attack power
	uint32 cl = getClass();   
	switch (cl)
	{
	case DRUID:
        //(Strength x 2) - 20           
        AP = str * 2 - 20;
        //Agility - 10
        RAP = agi - 10;
    
        if( GetShapeShift() == FORM_MOONKIN )
        {
            //(Strength x 2) + (Character Level x 1.5) - 20
            AP += float2int32( static_cast<float>(lev) * 1.5f );
        }
        if( GetShapeShift() == FORM_CAT )
		{
            //(Strength x 2) + Agility + (Character Level x 2) - 20
            AP += agi + (lev *2);
        }
        if( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR )
        {
            //(Strength x 2) + (Character Level x 3) - 20
            AP += (lev *3);
		}
		break;


	case ROGUE:
		//Strength + Agility + (Character Level x 2) - 20
		AP = str + agi + (lev *2) - 20;
		//Character Level + Agility - 10
		RAP = lev + agi - 10;

		break;


	case HUNTER:
		//Strength + Agility + (Character Level x 2) - 20
		 AP = str + agi + (lev *2) - 20;
		//(Character Level x 2) + Agility - 10
		RAP = (lev *2) + agi - 10;

		break;

	case SHAMAN:   
		//(Strength) + (Agility) + (Character Level x 2) - 20
		AP = str + agi + (lev *2) - 20;
		//Agility - 10
		RAP = agi - 10;
		
		break;

	case PALADIN:
		//(Strength x 2) + (Character Level x 3) - 20
		AP = (str *2) + (lev *3) - 20;
		//Agility - 10
		RAP = agi - 10;
	
		break;


	case WARRIOR:
	case DEATHKNIGHT:
		{
			//(Strength x 2) + (Character Level x 3) - 20
		AP = (str *2) + (lev *3) - 20;
		uint32 divi = 0; 
		uint32 sprank = 0; 
		if     ( HasSpell(49393) ) { sprank = 5; divi = 180;} // Death Knight: Bladed Armor 
		else if( HasSpell(49392) ) { sprank = 4; divi = 180;} // Increases your attack power by <spellrank> for every 180 armor value you have. 
		else if( HasSpell(49391) ) { sprank = 3; divi = 180;} 
		else if( HasSpell(49390) ) { sprank = 2; divi = 180;} 
		else if( HasSpell(48978) ) { sprank = 1; divi = 180;} 
		else if( HasSpell(61222) ) { sprank = 3; divi = 108;} // Warrior: Armored to the Teeth 
		else if( HasSpell(61221) ) { sprank = 2; divi = 108;} //Increases your attack power by <spellrank> for every 108 armor value you have. 
		else if( HasSpell(61216) ) { sprank = 1; divi = 108;} 
		if(divi && sprank ) 
			AP += sprank * (GetUInt32Value(UNIT_FIELD_RESISTANCES)/divi);
		}
		//Character Level + Agility - 10
		RAP = lev + agi - 10;
		
		break;

	default:    //mage,priest,warlock
		AP = agi - 10;
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
		stam = lvlinfo->Stat[STAT_STAMINA];
		intl = lvlinfo->Stat[STAT_INTELLECT];
	}

	int32 hp = GetUInt32Value( UNIT_FIELD_BASE_HEALTH );

	stam += GetUInt32Value( UNIT_FIELD_POSSTAT2 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT2 );

	int32 res = hp + std::max(20, stam) + std::max(0, stam - 20) * 10 + m_healthfromspell + m_healthfromitems;

	if (res < 1)
		res = 1;

	int32 oldmaxhp = GetUInt32Value( UNIT_FIELD_MAXHEALTH );

	if( res < hp ) res = hp;
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, res );

	if( ( int32 )GetUInt32Value(UNIT_FIELD_HEALTH ) > res )
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	else if( ( cl == DRUID) && ( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR ) )
	{
		res = float2int32( ( float )GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * ( float )GetUInt32Value( UNIT_FIELD_HEALTH ) / float( oldmaxhp ) );
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	}

	if( cl != WARRIOR && cl != ROGUE && cl != DEATHKNIGHT)
	{
		// MP
		int32 mana = GetUInt32Value( UNIT_FIELD_BASE_MANA );

		intl += GetUInt32Value( UNIT_FIELD_POSSTAT3 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT3 );

		res = mana + std::max(20, intl) + std::max(0, intl - 20) * 15 + m_manafromspell + m_manafromitems;
		if( res < mana )
			res = mana;

		SetUInt32Value(UNIT_FIELD_MAXPOWER1, res);

		if((int32)GetUInt32Value(UNIT_FIELD_POWER1)>res)
			SetUInt32Value(UNIT_FIELD_POWER1,res);
		//Manaregen
		const static float BaseRegen[80] = 
		{
			0.034965f, 0.034191f, 0.033465f, 0.032526f, 0.031661f, 0.031076f, 0.030523f, 0.029994f, 0.029307f, 0.028661f,
			0.027584f, 0.026215f, 0.025381f, 0.024300f, 0.023345f, 0.022748f, 0.021958f, 0.021386f, 0.020790f, 0.020121f, 
			0.019733f, 0.019155f, 0.018819f, 0.018316f, 0.017936f, 0.017576f, 0.017201f, 0.016919f, 0.016581f, 0.016233f,
			0.015994f, 0.015707f, 0.015464f, 0.015204f, 0.014956f, 0.014744f, 0.014495f, 0.014302f, 0.014094f, 0.013895f,
			0.013724f, 0.013522f, 0.013363f, 0.013175f, 0.012996f, 0.012853f, 0.012687f, 0.012539f, 0.012384f, 0.012233f,
			0.012113f, 0.011973f, 0.011859f, 0.011714f, 0.011575f, 0.011473f, 0.011342f, 0.011245f, 0.011110f, 0.010999f, 
			0.010700f, 0.010522f, 0.010290f, 0.010119f, 0.009968f, 0.009808f, 0.009651f, 0.009553f, 0.009445f, 0.009327f,
			0.008859f, 0.008415f, 0.007993f, 0.007592f, 0.007211f, 0.006849f, 0.006506f, 0.006179f, 0.005869f, 0.005575f
		};

		uint32 lvl = getLevel();
		if(lvl > 80) lvl = 80;

		float amt = ( 0.001f + sqrt((float)GetUInt32Value( UNIT_FIELD_INTELLECT )) * GetUInt32Value( UNIT_FIELD_SPIRIT ) * BaseRegen[lvl-1] ) * PctPowerRegenModifier[POWER_TYPE_MANA];
		SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, amt + m_ModInterrMRegen / 5.0f);
		SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, amt * m_ModInterrMRegenPCT / 100.0f + m_ModInterrMRegen / 5.0f);
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

		int32 blockable_damage = float2int32( float( shield->GetProto()->Block ) +( float( m_modblockvaluefromspells + GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) ) * block_multiplier ) + ( ( float( str ) / 2.0f ) ) );
		SetUInt32Value( PLAYER_SHIELD_BLOCK, blockable_damage );
	}
	else
	{
		SetUInt32Value( PLAYER_SHIELD_BLOCK, 0 );
	}

	UpdateChances();
	CalcDamage();
}

uint32 Player::SubtractRestXP(uint32 amount)
{
	if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you've reached max_level
		amount = 0;

	int32 restAmount = m_restAmount - (amount << 1);									// remember , we are dealing with xp without restbonus, so multiply by 2

	if( restAmount < 0)
		m_restAmount = 0;
	else
		m_restAmount = restAmount;

	DEBUG_LOG("REST","Subtracted %d rest XP to a total of %d", amount, m_restAmount);
	UpdateRestState();																	// Update clients interface with new values.
	return amount;
}

void Player::AddCalculatedRestXP(uint32 seconds)
{
	// When a player rests in a city or at an inn they will gain rest bonus at a very slow rate. 
	// Eight hours of rest will be needed for a player to gain one "bubble" of rest bonus. 
	// At any given time, players will be able to accumulate a maximum of 30 "bubbles" worth of rest bonus which 
	// translates into approximately 1.5 levels worth of rested play (before your character returns to normal rest state).
	// Thanks to the comforts of a warm bed and a hearty meal, players who rest or log out at an Inn will 
	// accumulate rest credit four times faster than players logged off outside of an Inn or City. 
	// Players who log out anywhere else in the world will earn rest credit four times slower.
	// http://www.worldofwarcraft.com/info/basics/resting.html


	// Define xp for a full bar ( = 20 bubbles)
	uint32 xp_to_lvl = uint32(lvlinfo->XPToNextLevel);

	// get RestXP multiplier from config.
	float bubblerate = sWorld.getRate(RATE_RESTXP);

	// One bubble (5% of xp_to_level) for every 8 hours logged out.
	// if multiplier RestXP (from Ascent.config) is f.e 2, you only need 4hrs/bubble.
	uint32 rested_xp = uint32(0.05f * xp_to_lvl * ( seconds / (3600 * ( 8 / bubblerate))));

	// if we are at a resting area rest_XP goes 4 times faster (making it 1 bubble every 2 hrs)
	if (m_isResting)
		rested_xp <<= 2;

	// Add result to accumulated rested XP
	m_restAmount += uint32(rested_xp);

	// and set limit to be max 1.5 * 20 bubbles * multiplier (1.5 * xp_to_level * multiplier)
	if (m_restAmount > xp_to_lvl + (uint32)((float)( xp_to_lvl >> 1 ) * bubblerate ))
		m_restAmount = xp_to_lvl + (uint32)((float)( xp_to_lvl >> 1 ) * bubblerate );

	DEBUG_LOG("REST","Add %d rest XP to a total of %d, RestState %d", rested_xp, m_restAmount,m_isResting);

	// Update clients interface with new values.
	UpdateRestState();
}

void Player::UpdateRestState()
{
	if(m_restAmount && GetUInt32Value(UNIT_FIELD_LEVEL) < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		m_restState = RESTSTATE_RESTED;
	else
		m_restState = RESTSTATE_NORMAL;

	// Update RestState 100%/200%
	SetUInt32Value(PLAYER_BYTES_2, ((GetUInt32Value(PLAYER_BYTES_2) & 0x00FFFFFF) | (m_restState << 24)));

	//update needle (weird, works at 1/2 rate)
	SetUInt32Value(PLAYER_REST_STATE_EXPERIENCE, m_restAmount >> 1);
}

void Player::ApplyPlayerRestState(bool apply)
{
	if(apply)
	{
		m_restState = RESTSTATE_RESTED;
		m_isResting = true;
		SetFlag(PLAYER_FLAGS, PLAYER_FLAG_RESTING);	//put zzz icon
	}
	else
	{
		m_isResting = false;
		RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_RESTING);	//remove zzz icon
	}
	UpdateRestState();
}

#define CORPSE_VIEW_DISTANCE 1600 // 40*40

bool Player::CanSee(Object* obj) // * Invisibility & Stealth Detection - Partha *
{
	if (obj == TO_PLAYER(this))
	   return true;

	uint32 object_type = obj->GetTypeId();

	// We can't see any objects in another phase
	// unless they're in ALL_PHASES
	if( !PhasedCanInteract(obj) )
		return false;

	/* I'm a GM, I can see EVERYTHING! :D */
	if( bGMTagOn )
		return true;

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
			Player* pObj = TO_PLAYER(obj);

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
			if(obj->IsPlayer() && TO_PLAYER(obj)->InStealth())
				return false;

			return true;
		}

		if(object_type == TYPEID_UNIT)
		{
			Unit* uObj = TO_UNIT(obj);

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
				Player* pObj = TO_PLAYER(obj);

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

				if(pObj->InStealth()) // Stealth Detection
				{
					if(GetGroup() && pObj->GetGroup() == GetGroup()) // can see stealthed group members
						return true;

					uint64 stalker = pObj->stalkedby;
					if(stalker)
					{
						Object* pStalker = GetMapMgr() ? GetMapMgr()->_GetObject(stalker) : NULL;
						if(pStalker == TO_PLAYER(this))
							return true;

						if(pStalker && pStalker->IsPlayer())
						{
							if(GetGroup() && GetGroup()->HasMember(TO_PLAYER(pStalker)))
								return true;
						}
					}
					
					// 2d distance SQUARED!
					float base_range = 49.0f;
					float modDistance = 0.0f;

					int32 hide_level = (getLevel() * 5 + GetStealthDetectBonus()) - pObj->GetStealthLevel();
					modDistance += hide_level * 0.2f;
				
					if(pObj->isInBack(TO_PLAYER(this)))
						base_range /= 2.0f;

					if( base_range + modDistance <= 0 )
						return false;

					base_range += modDistance;

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
				Unit* uObj = TO_UNIT(obj);
					
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
				GameObject* gObj = TO_GAMEOBJECT(obj);

				if(gObj->invisible) // Invisibility - Detection of GameObjects
				{
					uint64 owner = gObj->GetUInt64Value(OBJECT_FIELD_CREATED_BY);

					if(GetGUID() == owner) // the owner of an object can always see it
						return true;

					if(GetGroup())
					{
						Player * gplr = NULL;
						gplr = GetMapMgr()->GetPlayer((uint32)owner);
						if(gplr != NULL && GetGroup()->HasMember(gplr))
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
			m_CurrentTaxiPath->SendMoveForTime( TO_PLAYER(this), TO_PLAYER( pObj ), ntime - m_taxi_ride_time);
		/*else
			m_CurrentTaxiPath->SendMoveForTime( TO_PLAYER(this), TO_PLAYER( pObj ), m_taxi_ride_time - ntime);*/
	}

	if( pObj->IsCreature() && pObj->m_faction->FactionFlags & 0x1000 )
		m_hasInRangeGuards++;

	Unit::AddInRangeObject(pObj);

	//if the object is a unit send a move packet if they have a destination
	if(pObj->GetTypeId() == TYPEID_UNIT)
	{
		//add an event to send move update have to send guid as pointer was causing a crash :(
		//sEventMgr.AddEvent( TO_CREATURE( pObj )->GetAIInterface(), &AIInterface::SendCurrentMove, TO_PLAYER(this)->GetGUID(), EVENT_UNIT_SENDMOVE, 200, 1);
		TO_CREATURE( pObj )->GetAIInterface()->SendCurrentMove(TO_PLAYER(this));
	}

	//unit based objects, send aura data
	if (pObj->IsUnit())
	{
		Unit* pUnit=TO_UNIT(pObj);
		Aura* aur = NULL;
		
		if (GetSession() != NULL)
		{
			WorldPacket data(SMSG_AURA_UPDATE_ALL, 28 * MAX_AURAS);
			data << pUnit->GetNewGUID();
			for (uint32 i=0; i<MAX_AURAS; ++i)
			{
				aur = pUnit->m_auras[i];
				if (aur != NULL)
				{
					data << uint8( aur->m_auraSlot );
					data << uint32( aur->GetSpellId() );
					uint16 flags = aur->GetAuraFlags();
					if( aur->IsPositive() )
						flags |= AFLAG_POSITIVE;
					else
						flags |= AFLAG_NEGATIVE;
					data << flags;
					data << uint8(aur->procCharges > aur->stackSize ? aur->procCharges : aur->stackSize);
					if(!(aur->GetAuraFlags() & AFLAG_NOT_GUID))
					{
						FastGUIDPack(data, aur->GetCasterGUID());
					}

					if( aur->GetAuraFlags() & AFLAG_HAS_DURATION )
					{
						data << aur->GetDuration();
						data << aur->GetTimeLeft();
					}
				}
			}
			CopyAndSendDelayedPacket(&data);
		}
	}
}

void Player::OnRemoveInRangeObject(Object* pObj)
{
	if( pObj->IsCreature() && pObj->m_faction->FactionFlags & 0x1000 )
		m_hasInRangeGuards--;

	//if (/*!CanSee(pObj) && */IsVisible(pObj))
	//{
		//RemoveVisibleObject(pObj);
	//}
	if(m_tempSummon == pObj)
	{
		m_tempSummon->RemoveFromWorld(false, true);
		if(m_tempSummon)
			m_tempSummon->SafeDelete();

		m_tempSummon = NULL;
		SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	}

	m_visibleObjects.erase(pObj);
	Unit::OnRemoveInRangeObject(pObj);

	if( pObj == m_CurrentCharm)
	{
		Unit* p = m_CurrentCharm;

		if(pObj == m_CurrentVehicle)
		{
			m_CurrentVehicle->RemovePassenger(TO_PLAYER(this));
		}
		else
			this->UnPossess();
		if(m_currentSpell)
			m_currentSpell->cancel();	   // cancel the spell
		m_CurrentCharm=NULL;

		if( p->m_temp_summon&&p->GetTypeId() == TYPEID_UNIT )
			TO_CREATURE( p )->SafeDelete();
	}
 
	if(pObj == m_Summon)
	{
		if(m_Summon->IsSummonedPet())
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
			m_Summon = NULL;
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
		sEventMgr.AddEvent(TO_PLAYER(this), &Player::EndDuel, (uint8)DUEL_WINNER_RETREAT, EVENT_PLAYER_DUEL_COUNTDOWN, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::ClearInRangeSet()
{
	m_visibleObjects.clear();
	Unit::ClearInRangeSet();
}


void Player::EventReduceDrunk(bool full)
{
	uint8 drunk = ((GetUInt32Value(PLAYER_BYTES_3) >> 8) & 0xFF);
	if(full) drunk = 0;
	else drunk -= 10;
	SetUInt32Value(PLAYER_BYTES_3, ((GetUInt32Value(PLAYER_BYTES_3) & 0xFFFF00FF) | (drunk << 8)));
	if(drunk == 0) sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_REDUCEDRUNK);
}

void Player::LoadTaxiMask(const char* data)
{
	vector<string> tokens = StrSplit(data, " ");

	int index;
	vector<string>::iterator iter;

	for (iter = tokens.begin(), index = 0;
		(index < 12) && (iter != tokens.end()); ++iter, ++index)
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

			// Check the item_quest_association table for an entry related to this item
			QuestAssociationList *tempList = QuestMgr::getSingleton().GetQuestAssociationListForItemId( itemid );
			if( tempList != NULL )
			{
				QuestAssociationList::iterator it;
				for (it = tempList->begin(); it != tempList->end(); ++it)
					if ( ((*it)->qst == qst) && (GetItemInterface()->GetItemCount( itemid ) < (*it)->item_count) )
						return true;
			}

			// No item_quest association found, check the quest requirements
			if( !qst->count_required_item )
				continue;

			for( uint32 j = 0; j < 6; ++j )
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
	Object* lootObj;
	
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

	//lootsteal fix
	if( lootObj->GetTypeId() == TYPEID_UNIT )
	{
		Creature* LootOwner = TO_CREATURE( lootObj );
		uint32 GroupId = LootOwner->m_taggingGroup;
		if ( GroupId != 0 && ( m_Group == NULL || GroupId != m_Group->GetID() ) )
			return;
		loot_method = LootOwner->m_lootMethod;
	};

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
		
	WorldPacket data, data2(29);
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
					iter->roll = new LootRoll;
					iter->roll->Init(60000, (m_Group != NULL ? m_Group->MemberCount() : 1),  guid, x, iter->item.itemproto->ItemId, factor, uint32(ipid), GetMapMgr());
					
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

					data2 << uint32(iter->iItemsCount);
					data2 << uint32(60000); // countdown
					data2 << uint8(0x0F); //mask
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

void Player::SendDualTalentConfirm()
{
	uint32 talentCost = sWorld.dualTalentTrainCost;
	// Here we should send a confirmation box, but there is no documented opcode for it
	if( GetUInt32Value(PLAYER_FIELD_COINAGE) >= talentCost )
	{
		// Cast the learning spell
		CastSpell(this, 63624, true);
		ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)talentCost);
	}
	else
	{
		// Not enough money!
		sChatHandler.SystemMessage(m_session, "You must have at least %u copper to use this function.", talentCost);
	}
	/*WorldPacket data();
	data << GetGUID();
	data << sWorld.dualTalentTrainCost;
	GetSession()->SendPacket( &data );*/
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

	SpellEntry* spellinfo = NULL;
	if( autoshot )
		spellinfo = dbcSpell.LookupEntry( 75 );
	else
		spellinfo = dbcSpell.LookupEntry( spellid );

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
	uint32 rIndex = spellinfo->rangeIndex;
	SpellRange* range = dbcSpellRange.LookupEntry( rIndex );
	float minrange = GetMinRange( range );
	float dist = GetDistance2dSq( target ) - target->GetSize() - GetSize();
	float maxr = GetMaxRange( range );

	if( spellinfo->SpellGroupType )
	{
		SM_FFValue( this->SM[SMT_RANGE][0], &maxr, spellinfo->SpellGroupType );
		SM_PFValue( this->SM[SMT_RANGE][1], &maxr, spellinfo->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		float spell_pct_modifers=0;
		SM_FFValue(this->SM[SMT_RANGE][0],&spell_flat_modifers,spellinfo->SpellGroupType);
		SM_FFValue(this->SM[SMT_RANGE][1],&spell_pct_modifers,spellinfo->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!spell range bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u\n",spell_flat_modifers,spell_pct_modifers,maxr,spellinfo->SpellGroupType);
#endif
	}

	maxr += 4.0f; // Matches client range
	maxr *= maxr; // square me!
	minrange *= minrange;

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

	if (GetMapMgr() && GetMapMgr()->IsCollisionEnabled())
	{
		if( !CollideInterface.CheckLOS(m_mapId, GetPositionX(), GetPositionY(), GetPositionZ(), 
			target->GetPositionX(), target->GetPositionY(), target->GetPositionZ()) )
		{
			fail = SPELL_FAILED_LINE_OF_SIGHT;
		}
	}
	if( fail > 0 )// && fail != SPELL_FAILED_OUT_OF_RANGE)
	{
		//SendCastResult( autoshot ? 75 : spellid, fail, 0, 0 );
		packetSMSG_CASTRESULT cr;
		cr.SpellId = spellinfo->Id;
		cr.ErrorMessage = fail;
		cr.MultiCast = 0;
		m_session->OutPacket( SMSG_CAST_FAILED, sizeof(packetSMSG_CASTRESULT), &cr );
		if( fail != SPELL_FAILED_OUT_OF_RANGE )
		{
			uint32 spellid2 = spellinfo->Id;
			m_session->OutPacket( SMSG_CANCEL_AUTO_REPEAT, 4, &spellid2 );
		}
		//sLog.outString( "Result for CanShootWIthRangedWeapon: %u" , fail );
		//OUT_DEBUG( "Can't shoot with ranged weapon: %u (Timer: %u)" , fail , m_AutoShotAttackTimer );
		return fail;
	}

	return 0;
}

void Player::EventRepeatSpell()
{
	if( !m_curSelection || !IsInWorld() )
		return;

	if( m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP | UNIT_STATE_STUN | UNIT_STATE_CONFUSE ) || IsStunned() || IsFeared() )
		return;
	
	Unit* target = GetMapMgr()->GetUnit( m_curSelection );
	if( target == NULL )
	{
		m_AutoShotAttackTimer = 0; //avoid flooding client with error mesages
		m_onAutoShot = false;
		//OUT_DEBUG( "Can't cast Autoshot: Target changed! (Timer: %u)" , m_AutoShotAttackTimer );
		return;
	}

	m_AutoShotDuration = m_uint32Values[UNIT_FIELD_RANGEDATTACKTIME];

	if( m_isMoving )
	{
		//OUT_DEBUG( "HUNTER AUTOSHOT 2) %i, %i", m_AutoShotAttackTimer, m_AutoShotDuration );
		//m_AutoShotAttackTimer = m_AutoShotDuration;//avoid flooding client with error mesages
		//OUT_DEBUG( "Can't cast Autoshot: You're moving! (Timer: %u)" , m_AutoShotAttackTimer );
		m_AutoShotAttackTimer = 400; // shoot when we can
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
	
		Spell* sp = NULL;
		sp = (new Spell( TO_PLAYER(this), m_AutoShotSpell, true, NULL ));
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

void Player::EventTimedQuestExpire(Quest *qst, QuestLogEntry *qle, uint32 log_slot, uint32 interval)
{
	qle->SubtractTime(interval);
	DEBUG_LOG("QuestLogEntry","qle TimeLeft:%u[ms]",qle->GetTimeLeft());

	if(qle->GetTimeLeft() == 0)
	{
		WorldPacket fail;
		sQuestMgr.BuildQuestFailed(&fail, qst->id);
		GetSession()->SendPacket(&fail);
		sHookInterface.OnQuestCancelled(TO_PLAYER(this), qst);
		CALL_QUESTSCRIPT_EVENT(qle, OnQuestCancel)(TO_PLAYER(this));
//		qle->Finish();
	}
	else
		sEventMgr.AddEvent(TO_PLAYER(this), &Player::EventTimedQuestExpire, qst, qle, log_slot, interval, EVENT_TIMED_QUEST_EXPIRE, interval, 1, 0);
}
void Player::RemoveQuestsFromLine(int skill_line)
{
	for (int i = 0; i < 25; i++)
	{
		if (m_questlog[i])
		{
			Quest* qst = m_questlog[i]->GetQuest();
			if (qst && qst->required_tradeskill == skill_line)
			{
				m_questlog[i]->Finish();
	
				// Remove all items given by the questgiver at the beginning
				for(uint32 j = 0; j < 4; j++)
					if(qst->receive_items[j])
						GetItemInterface()->RemoveItemAmt(qst->receive_items[j], 1 );
				delete qst;
			}
		}
	}

	set<uint32>::iterator itr, itr2;
	for (itr = m_finishedQuests.begin(); itr != m_finishedQuests.end();)
	{
		itr2 = itr;
		++itr;
		Quest * qst = QuestStorage.LookupEntry((*itr2));
		if (qst && qst->required_tradeskill == skill_line)
			m_finishedQuests.erase(itr2);
	}

	UpdateNearbyGameObjects();
}

void Player::SendInitialLogonPackets()
{
	// Initial Packets... they seem to be re-sent on port.
	m_session->OutPacket(SMSG_QUEST_FORCE_REMOVE, 4, &m_timeLogoff);

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
	time_t t;
	struct tm *tm;

	uint32 DayOfTheWeek;		//	(0b111 = (any) day, 0 = Monday ect)
    uint32 DayOfTheMonth;		//	Day - 1 (0 is actual 1) its now the 20e here.
    uint32 CurrentMonth;		//	Month - 1 (0 is actual 1) same as above.
    uint32 CurrentYear;			//	2000 + this number results in a correct value for this crap.

	t = UNIXTIME;
	tm = localtime(&t);
	if (tm)
	{
		DayOfTheWeek = tm->tm_wday == 0 ? 6 : tm->tm_wday - 1;
		DayOfTheMonth = tm->tm_mday - 1;
		CurrentMonth = tm->tm_mon;
		CurrentYear = tm->tm_year - 100;
	}

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
	data << uint32(0);
	GetSession()->SendPacket( &data );

	DEBUG_LOG("WORLD","Sent initial logon packets for %s.", GetName());
}

void Player::Reset_Spells()
{
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	ASSERT(info);

	std::list<uint32> spelllist;
	bool profession = false;
	
	for(SpellSet::iterator itr = mSpells.begin(); itr!=mSpells.end(); itr++)
	{
		SpellEntry *sp = dbcSpell.LookupEntry((*itr));
		for( uint32 lp = 0; lp < 3; lp++ )
		{
			if( sp->Effect[lp] == SPELL_EFFECT_SKILL )
				profession = true;
		}

		if( !profession )
			spelllist.push_back((*itr));
			
		profession = false;
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

	profession = false;
	for(std::set<uint32>::iterator itr = mDeletedSpells.begin(); itr != mDeletedSpells.end(); itr++)
	{
		SpellEntry *sp = dbcSpell.LookupEntry((*itr));
		for( uint32 lp = 0; lp < 3; lp++ )
		{
			if( sp->Effect[lp] == SPELL_EFFECT_SKILL )
				profession = true;
		}

		if( !profession )
			mDeletedSpells.erase((*itr));
		
		profession = false;
	}
}

void Player::ResetTitansGrip()
{
	if(titanGrip || !GetItemInterface())
		return;

	Item* mainhand = GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND);
	Item* offhand = GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND);
	if(offhand && (offhand->GetProto()->InventoryType == INVTYPE_2HWEAPON ||
		mainhand && mainhand->GetProto()->InventoryType == INVTYPE_2HWEAPON))
	{
		// we need to de-equip this
		if(titanGrip != NULL)
		offhand = GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND, false);
		if( offhand == NULL )
			return;     // should never happen
		SlotResult result = GetItemInterface()->FindFreeInventorySlot(offhand->GetProto());
		if( !result.Result )
		{
			// no free slots for this item. try to send it by mail
			offhand->RemoveFromWorld();
			offhand->SetOwner( NULL );
			offhand->SaveToDB( INVENTORY_SLOT_NOT_SET, 0, true, NULL );
			sMailSystem.DeliverMessage(NORMAL, GetGUID(), GetGUID(), "Your offhand item", "", 0, 0, offhand->GetUInt32Value(OBJECT_FIELD_GUID), 1, true);
			offhand->Destructor();
		}
		else if( !GetItemInterface()->SafeAddItem(offhand, result.ContainerSlot, result.Slot) )
			if( !GetItemInterface()->AddItemToFreeSlot(offhand) )   // shouldn't happen either.
			{
				offhand->Destructor();
			}
	}
}

void Player::Reset_Talents()
{
	std::map<uint32, uint8> *talents = &m_specs[m_talentActiveSpec].talents;
	std::map<uint32, uint8>::iterator itr;
	for(itr = talents->begin(); itr != talents->end(); itr++)
	{
		TalentEntry *te = dbcTalent.LookupEntryForced(itr->first);
		if(!te)
			continue;
		RemoveTalent(te->RankID[itr->second]);
	}
	talents->clear();

	// The dual wield skill for shamans can only be added by talents.
	// so when reset, the dual wield skill should be removed too.
	// (see also void Spell::SpellEffectDualWield)
	if( getClass()==SHAMAN && _HasSkillLine( SKILL_DUAL_WIELD ) )
		_RemoveSkillLine( SKILL_DUAL_WIELD );

	//SetUInt32Value(PLAYER_CHARACTER_POINTS1, GetMaxTalentPoints()); 
	availTalentPoints = GetMaxTalentPoints();

	if( getClass() == WARRIOR )
	{	
		titanGrip = false;
		ResetTitansGrip();
	}
	smsg_TalentsInfo(false);
}

uint16 Player::GetMaxTalentPoints()
{
	// see if we have a custom value
	if(m_maxTalentPoints > 0)
		return m_maxTalentPoints;

	// otherwise use blizzlike value
	uint32 l = getLevel();
	if(l > 9)
	{
		return l - 9; 
	}
	else
	{
		return 0; 
	}
}

void Player::ApplySpec(uint8 spec, bool init)
{
	if(spec > m_talentSpecsCount || spec > MAX_SPEC_COUNT)
		return;
	std::map<uint32, uint8> *talents;
	std::map<uint32, uint8>::iterator itr;

	if(!init)	// unapply old spec
	{
		talents = &m_specs[m_talentActiveSpec].talents;
		for(itr = talents->begin(); itr != talents->end(); itr++)
		{
			TalentEntry * talentInfo = dbcTalent.LookupEntryForced(itr->first);
			if(!talentInfo || itr->second > 4)
				continue;
			RemoveTalent(talentInfo->RankID[itr->second]);
		}
		if( getClass() == WARRIOR )
		{	
			titanGrip = false;
			ResetTitansGrip();
		}

		//Dismiss any pets
		if(GetSummon())
		{
			if(GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				GetSummon()->Dismiss(false);				// warlock summon -> dismiss
			else
				GetSummon()->Remove(false, true, true);	// hunter pet -> just remove for later re-call
		}
	}
	
	// apply new spec
	talents = &m_specs[spec].talents;
	uint32 spentPoints = 0;
	for(itr = talents->begin(); itr != talents->end(); itr++)
	{
		TalentEntry * talentInfo = dbcTalent.LookupEntryForced(itr->first);
		if(!talentInfo || itr->second > 4)
			continue;
		ApplyTalent(talentInfo->RankID[itr->second]);
		spentPoints += itr->second + 1;
	}
	m_talentActiveSpec = spec;
	// update available Talent Points
	uint32 maxTalentPoints = GetMaxTalentPoints();
	uint32 newTalentPoints;
	if(spentPoints >= maxTalentPoints)
		newTalentPoints = 0;	// just in case
	else
		newTalentPoints = maxTalentPoints - spentPoints;
	//SetUInt32Value(PLAYER_CHARACTER_POINTS1, newTalentPoints);
	availTalentPoints = newTalentPoints;

	// Apply glyphs
	for(uint32 i = 0; i < GLYPHS_COUNT; i++)
	{
		UnapplyGlyph(i);
		SetGlyph(i, m_specs[m_talentActiveSpec].glyphs[i]);
	}
	smsg_TalentsInfo(false);
}

void Player::ApplyTalent(uint32 spellid)
{
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced( spellid );
	if(!spellInfo)
		return;	// not found

	if(!(spellInfo->Attributes & ATTRIBUTES_PASSIVE))
	{
		addSpell(spellid);	// in this case we need to learn the spell itself
	}

	if( (spellInfo->Attributes & ATTRIBUTES_PASSIVE || (spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL ||
		spellInfo->Effect[1] == SPELL_EFFECT_LEARN_SPELL ||
		spellInfo->Effect[2] == SPELL_EFFECT_LEARN_SPELL) 
		&& ( (spellInfo->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) == 0 || ( (spellInfo->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) && GetSummon() ) ) )
		)
	{
		if( spellInfo->RequiredShapeShift && !( (uint32)1 << (GetShapeShift()-1) & spellInfo->RequiredShapeShift ) )
		{
			// do nothing
		}
		else
		{
			Spell* sp = new Spell(this,spellInfo,true,NULL);
			SpellCastTargets tgt;
			tgt.m_unitTarget=GetGUID();
			sp->prepare(&tgt);
		}
	}
}

void Player::RemoveTalent(uint32 spellid)
{
	SpellEntry * sp = dbcSpell.LookupEntryForced(spellid);
	if(!sp)
		return; // not found

	for(uint32 k = 0; k < 3; ++k)
	{
		if(sp->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
		{
			SpellEntry * sp2 = dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[k]);
			if(!sp2) continue;
			removeSpellByHashName(sp2->NameHash);
		}
	}
	if(!(sp->Attributes & ATTRIBUTES_PASSIVE))
	{	// in this case we need to remove the spells we learned from this talent
		// TODO we need to store them somewhere so that player doesn't have to relearn all ranks again
		removeSpellByHashName(sp->NameHash);
	}

	for(uint32 x=0;x < MAX_AURAS + MAX_PASSIVE_AURAS; x++)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == sp->NameHash)
		{
			m_auras[x]->Remove();
		}
	}
}

void Player::LearnTalent(uint32 talent_id, uint32 requested_rank)
{
	unsigned int i;
	if (requested_rank > 4)
		return;

	TalentEntry * talentInfo = dbcTalent.LookupEntryForced(talent_id);
	if(!talentInfo)return;

	uint32 CurTalentPoints = availTalentPoints;
	std::map<uint32, uint8> *talents = &m_specs[m_talentActiveSpec].talents;
	uint8 currentRank = 0;
	std::map<uint32, uint8>::iterator itr = talents->find(talent_id);
	if(itr != talents->end())
		currentRank = itr->second + 1;

	if(currentRank >= requested_rank + 1)
		return; // player already knows requested or higher rank for this talent

	uint32 RequiredTalentPoints = requested_rank + 1 - currentRank;
	if(CurTalentPoints < RequiredTalentPoints )
		return; // player doesn't have enough points to get this rank for this talent

	// Check if it requires another talent
	if (talentInfo->DependsOn > 0)
	{
		TalentEntry *depTalentInfo = NULL;
		depTalentInfo = dbcTalent.LookupEntryForced(talentInfo->DependsOn);
		if (depTalentInfo)
		{
			itr = talents->find(talentInfo->DependsOn);
			if(itr == talents->end())
				return;	// player doesn't have the talent this one depends on
			if(talentInfo->DependsOnRank > itr->second)
				return;	// player doesn't have the talent rank this one depends on
		}
	}

	// Check that the requested talent belongs to a tree from player's class
	uint32 tTree = talentInfo->TalentTree;
	uint32 cl = getClass();

	for(i = 0; i < 3; ++i)
		if(tTree == TalentTreesPerClass[cl][i])
			break;

	if(i == 3)
	{
		// cheater!
		GetSession()->Disconnect();
		return;
	}

	// Find out how many points we have in this field
	uint32 spentPoints = 0;
	if (talentInfo->Row > 0)
	{
		for(itr = talents->begin(); itr != talents->end(); itr++)
		{
			TalentEntry *tmpTalent = dbcTalent.LookupEntryForced(itr->first);
			if (tmpTalent->TalentTree == tTree)
			{
				spentPoints += itr->second + 1;
			}
		}
	}

	uint32 spellid = talentInfo->RankID[requested_rank];
	if( spellid == 0 )
	{
		OUT_DEBUG("Talent: %u Rank: %u = 0", talent_id, requested_rank);
		return;
	}

	if(spentPoints < (talentInfo->Row * 5))			 // Min points spent
	{
		return;
	}

	(*talents)[talent_id] = requested_rank;
	//SetUInt32Value(PLAYER_CHARACTER_POINTS1, CurTalentPoints - RequiredTalentPoints);
	availTalentPoints = (CurTalentPoints - RequiredTalentPoints);
	// More cheat death hackage! :)
	if(spellid == 31330)
		m_cheatDeathRank = 3;
	else if(spellid == 31329)
		m_cheatDeathRank = 2;
	else if(spellid == 31328)
		m_cheatDeathRank = 1;	 

	if(requested_rank > 0 )	// remove old rank aura
	{
		uint32 respellid = talentInfo->RankID[currentRank - 1];
		if(respellid)
		{
			RemoveAura(respellid);
		}
	}

	ApplyTalent(spellid);
}

void Player::Reset_ToLevel1()
{
	RemoveAllAuras();

	SetUInt32Value(UNIT_FIELD_LEVEL, 1);
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	ASSERT(info);

	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // this gets devided by 10
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_POWER7, 0 );
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );
	SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );
	availTalentPoints = 0;
	availProfPoints = 2;
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

}

void Player::CalcResistance(uint32 type)
{
	int32 res;
	int32 pos;
	int32 neg;
	ASSERT(type < 7);
	pos = float2int32( BaseResistance[type] * (float)( BaseResistanceModPctPos[ type ] / 100.0f ) );
	neg = float2int32( BaseResistance[type] * (float)( BaseResistanceModPctNeg[ type ] / 100.0f ) );

	pos += FlatResistanceModifierPos[type];
	neg += FlatResistanceModifierNeg[type];
	res = BaseResistance[type] + pos - neg;
	if( type == 0 )
		res += m_uint32Values[UNIT_FIELD_STAT1] * 2;//fix armor from agi
	if( res < 0 )
		res = 0;
	pos += float2int32( res * (float)( ResistanceModPctPos[type] / 100.0f ) );
	neg += float2int32( res * (float)( ResistanceModPctNeg[type] / 100.0f ) );
	res = pos - neg + BaseResistance[type];
	if( type == 0 )
		res += m_uint32Values[UNIT_FIELD_STAT1] * 2;//fix armor from agi

	res = res < 0 ? 0 : res;

	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE+type,pos);
	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE+type,neg);
	SetUInt32Value(UNIT_FIELD_RESISTANCES+type,res);
}

void Player::UpdateNearbyGameObjects()
{
	for (Object::InRangeSet::iterator itr = GetInRangeSetBegin(); itr != GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			GameObject* go = TO_GAMEOBJECT(*itr);
			GameObjectInfo *info;

			info = go->GetInfo();
			if (!info)
			{
				Log.Error("Player","go->GetInfo returned NULL for go entry = %d, missing database info?", go->GetEntry());
				continue;
			} else if( info->InvolvedQuestIds != NULL )
			{
				uint32 v = 0;
				for(; v < info->InvolvedQuestCount; ++v)
				{
					if( GetQuestLogForEntry(info->InvolvedQuestIds[v]) != NULL )
					{
						go->SetUInt32Value(GAMEOBJECT_DYNAMIC, GO_DYNFLAG_QUEST);
						go->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 1);
						go->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
						break;
					}
				}
				if( v == info->InvolvedQuestCount )
				{
					go->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0);
					go->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 0);
					go->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
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
	float orientation = 0;
	uint32 cn = m_taxiMapChangeNode;

	m_taxiMapChangeNode = 0;

	if( IsMounted() )
		TO_UNIT(this)->Dismount();

	//also remove morph spells
	if(GetUInt32Value(UNIT_FIELD_DISPLAYID)!=GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
	{
		RemoveAllAurasOfType(SPELL_AURA_TRANSFORM);
		RemoveAllAurasOfType(SPELL_AURA_MOD_SHAPESHIFT);
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

	// temporary workaround for taximodes with changing map 
	if (   path->GetID() == 766 || path->GetID() == 767 || path->GetID() == 771 || path->GetID() == 772 
		|| path->GetID() == 775 || path->GetID() == 776 || path->GetID() == 796 || path->GetID() == 797 
		|| path->GetID() == 807) 
	{
		JumpToEndTaxiNode(path);
		return;
	}

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
	data << uint8(0);
	data << firstNode->x << firstNode->y << firstNode->z;
	data << m_taxi_ride_time;
	data << uint8( 0 );
	data << uint32( MONSTER_MOVE_FLAG_FLY );
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
  
	sEventMgr.AddEvent(TO_PLAYER(this), &Player::EventTaxiInterpolate, 
		EVENT_PLAYER_TAXI_INTERPOLATE, 900, 0,0);

	if( mapchangeid < 0 )
	{
		TaxiPathNode *pn = path->GetPathNode((uint32)path->GetNodeCount() - 1);
		sEventMgr.AddEvent(TO_PLAYER(this), &Player::EventDismount, path->GetPrice(), 
			pn->x, pn->y, pn->z, EVENT_PLAYER_TAXI_DISMOUNT, traveltime, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); 
	}
	else
	{
		sEventMgr.AddEvent(TO_PLAYER(this), &Player::EventTeleport, (uint32)mapchangeid, mapchangex, mapchangey, mapchangez, orientation, EVENT_PLAYER_TELEPORT, traveltime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
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
		skilllinespell* sp = dbcSkillLineSpell.LookupRow(i);
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
		pos += float2int32(( res * (int32)TotalStatModPctPos[type] ) / 100.0f);
		neg += float2int32(( res * (int32)TotalStatModPctNeg[type] ) / 100.0f);
		res = pos + int32(BaseStats[type]) - neg;
		if( res <= 0 )
			res = 1;
	}

	SetUInt32Value( UNIT_FIELD_POSSTAT0 + type, pos );
	SetUInt32Value( UNIT_FIELD_NEGSTAT0 + type, neg );
	SetUInt32Value( UNIT_FIELD_STAT0 + type, res > 0 ?res : 0 );
	if( type == 1 )
	   CalcResistance(RESISTANCE_ARMOR);
}

void Player::RegenerateMana(bool is_interrupted)
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER1);
	uint32 mm = GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if(cur >= mm)return;
	float amt = (is_interrupted) ? GetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER) : GetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER);
	amt /= 5;
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

	uint32 cur = GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(cur >= mh)
		return;

	gtFloat* HPRegenBase = dbcHPRegenBase.LookupEntry(getLevel()-1 + (getClass()-1)*100);
	gtFloat* HPRegen =  dbcHPRegen.LookupEntry(getLevel()-1 + (getClass()-1)*100);

	float amt = (m_uint32Values[UNIT_FIELD_STAT4]*HPRegen->val+HPRegenBase->val*100);
	amt *= sWorld.getRate(RATE_HEALTH);//Apply shit from conf file

	if(PctRegenModifier)
		amt+= (amt * PctRegenModifier) / 100;

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

void Player::LoosePower(uint32 powerField, int32 decayValue)
{
	if( m_interruptRegen > 0 )
		return;

	uint32 cur = GetUInt32Value(powerField);
    uint32 newpower = ((int)cur <= decayValue) ? 0 : cur-decayValue;
    if (newpower > 1000 )
	  newpower = 1000;

    SetUInt32Value(powerField,newpower);
}

void Player::LoseRage(int32 decayValue)
{
	//Rage is lost at a rate of 3 rage every 3 seconds. 
	//The Anger Management talent changes this to 2 rage every 3 seconds.
	LoosePower(UNIT_FIELD_POWER2, decayValue);
}

void Player::LooseRunic(int32 decayValue)
{
	LoosePower(UNIT_FIELD_POWER7, decayValue);
}

void Player::RegenerateEnergy()
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER4);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXPOWER4);
	if( cur >= mh )
		return;

	float amt = 2.0f * PctPowerRegenModifier[POWER_TYPE_ENERGY];

	cur += float2int32(amt);
	m_toRegen += (amt - cur);
	if( m_toRegen > 1 )
	{
		cur++;
		m_toRegen--;
	}

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
	//Send transfer pending only when switching between differnt mapids!
	WorldPacket data(41);
	if(sendpending && mapid != m_mapId && force_new_world)
	{
		data.SetOpcode(SMSG_TRANSFER_PENDING);
		data << mapid;
		GetSession()->SendPacket(&data);
	}

	//are we changing maps?
	if(m_mapId != mapid || force_new_world)
	{
		//Preteleport will try to find an instance (saved or active), or create a new one if none found.
		uint32 status = sInstanceMgr.PreTeleport(mapid, this, instance_id);
		if(status != INSTANCE_OK)
		{
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << mapid << status;
			GetSession()->SendPacket(&data);
			return;
		}

		//did we get a new instanceid?
		if(instance_id)
			m_instanceId=instance_id;

		//remove us from this map
		if(IsInWorld())
			RemoveFromWorld();

		//send new world
		data.Initialize(SMSG_NEW_WORLD);
		data << (uint32)mapid << v << v.o;
		GetSession()->SendPacket( &data );
		SetMapId(mapid);
		SetPlayerStatus(TRANSFER_PENDING);
	}
	else
	{

		// we are on same map allready, no further checks needed,
		// send teleport ack msg
		WorldPacket * data = BuildTeleportAckMsg(v);
		m_session->SendPacket(data);
		delete data;

		//reset transporter if we where on one.
		if( m_CurrentTransporter && !m_lockTransportVariables )
		{
			m_CurrentTransporter->RemovePlayer(TO_PLAYER(this));
			m_CurrentTransporter = NULL;
			m_TransporterGUID = 0;
		}
	}

	//update position
	m_sentTeleportPosition = v;
	SetPosition(v);
	ResetHeartbeatCoords();

	z_axisposition = 0.0f;
}
#endif

void Player::UpdateKnownCurrencies(uint32 ItemId, bool apply)
{
	if(CurrencyTypesEntry * ctEntry = dbcCurrencyTypes.LookupEntryForced(ItemId))
	{
		/*if(apply)
			SetFlag64(PLAYER_FIELD_KNOWN_CURRENCIES,(1 << (ctEntry->BitIndex-1)));
		else
			RemoveFlag64(PLAYER_FIELD_KNOWN_CURRENCIES,(1 << (ctEntry->BitIndex-1)));*/ //TODO: Fix? - CMB
	}
}

// Player::AddItemsToWorld
// Adds all items to world, applies any modifiers for them.

void Player::AddItemsToWorld()
{
	Item* pItem;
	for(uint32 i = 0; i < INVENTORY_CURRENCY_END; i++)
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
					Item* item = TO_CONTAINER(pItem)->GetItem(e);
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
	Item* pItem;
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
					Item* item = TO_CONTAINER(pItem)->GetItem(e);
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

uint32 Player::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player* target )
{
	int count = 0;
	if(target == TO_PLAYER(this))
	{
		// we need to send create objects for all items.
		count += GetItemInterface()->m_CreateForPlayer(data);
	}
	count += Unit::BuildCreateUpdateBlockForPlayer(data, target);
	return count;
}

void Player::_Warn(const char *message)
{
	sChatHandler.RedSystemMessage(GetSession(), message);
}

void Player::Kick(uint32 delay /* = 0 */)
{
	if(!delay)
	{
		m_KickDelay = 0;
		_Kick();
	} else {
		m_KickDelay = delay;
		sEventMgr.AddEvent(TO_PLAYER(this), &Player::_Kick, EVENT_PLAYER_KICK, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::_Kick()
{
	if(!m_KickDelay)
	{
		sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_KICK);
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
		sChatHandler.BlueSystemMessageToPlr(TO_PLAYER(this), "You will be removed from the server in %u seconds.", (uint32)(m_KickDelay/1000));
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
		case DEATHKNIGHT:
		{
			ClearCooldownsOnLine(770, guid);
			ClearCooldownsOnLine(771, guid);
			ClearCooldownsOnLine(772, guid);
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
		m_mapMgr->PushToProcessed(TO_PLAYER(this));
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
		m_mapMgr->PushToProcessed(TO_PLAYER(this));
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
		m_mapMgr->PushToProcessed(TO_PLAYER(this));
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
		m_mapMgr->PushToProcessed(TO_PLAYER(this));
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
		*(uint16*)&update_buffer[c] = 0;
		c += 2;
        *(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mCreationCount + 1) : mCreationCount);
		c += 4;

            // append any out of range updates
	    if(mOutOfRangeIdCount)
	    {
		    update_buffer[c]				= UPDATETYPE_OUT_OF_RANGE_OBJECTS;
			++c;
            *(uint32*)&update_buffer[c]	 = mOutOfRangeIdCount;
			c += 4;
		    memcpy(&update_buffer[c], mOutOfRangeIds.contents(), mOutOfRangeIds.size());
			c += mOutOfRangeIds.size();
		    mOutOfRangeIds.clear();
		    mOutOfRangeIdCount = 0;
	    }

        if(bCreationBuffer.size())
			memcpy(&update_buffer[c], bCreationBuffer.contents(), bCreationBuffer.size());
		c += bCreationBuffer.size();

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
		*(uint16*)&update_buffer[c] = 0;
		c += 2;
		*(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mUpdateCount + 1) : mUpdateCount);
		c += 4;
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
		OUT_DEBUG("deflateInit failed.");
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
		OUT_DEBUG("deflate failed.");
		delete [] buffer;
		return false;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		OUT_DEBUG("deflate failed: did not end stream");
		delete [] buffer;
		return false;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		OUT_DEBUG("deflateEnd failed.");
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
	MapInfo * pPMapinfo = NULL;
	pPMapinfo = WorldMapInfoStorage.LookupEntry(mapId);

	if(pPMapinfo != NULL)
	{
		m_bgEntryPointX = pPMapinfo->repopx;
		m_bgEntryPointY = pPMapinfo->repopy;
		m_bgEntryPointZ = pPMapinfo->repopz;
		m_bgEntryPointO = GetOrientation();
		m_bgEntryPointMap = pPMapinfo->repopmapid;
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
	m_AreaID = m_mapMgr->GetAreaID(m_position.x, m_position.y, m_position.z);
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
	sHookInterface.OnZone(TO_PLAYER(this), ZoneId, oldzone);
	CALL_INSTANCE_SCRIPT_EVENT( m_mapMgr, OnZoneChange )( TO_PLAYER(this), ZoneId, oldzone );


	AreaTable *at = dbcArea.LookupEntry(GetAreaID());
	if(at && at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY)
	{
		Unit* pUnit = (GetSelection() == 0) ? NULL : (m_mapMgr ? m_mapMgr->GetUnit(GetSelection()) : NULL);
		if(pUnit && DuelingWith != pUnit)
		{
			EventAttackStop();
			smsg_AttackStop(pUnit);
		}

		if(m_currentSpell)
		{
			Unit* target = m_currentSpell->GetUnitTarget();
			if(target && target != DuelingWith && target != TO_PLAYER(this))
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
#ifdef CLUSTERING
		std::set<uint32>::iterator i;
		std::vector<uint32> channels;
		uint32 cid;

		for(i = m_channels.begin(); i != m_channels.end();)
		{
			cid = *i;
			++i;
			channels.push_back(cid);
		}
		WorldPacket data(ICMSG_CHANNEL_UPDATE, 4 + (sizeof(std::vector<uint32>::size_type) * channels.size()) + 4 + 4 + 4);
		data << uint8(UPDATE_CHANNELS_ON_ZONE_CHANGE); //update channels on zone change
		data << GetLowGUID();
		data << channels;
		data << GetAreaID();
		data << GetZoneId();
		data << GetMapId();
		sClusterInterface.SendPacket(&data);
#else
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
					p->Part(TO_PLAYER(this), true);
				}

				p = channelmgr.GetCreateChannel(strbuf, TO_PLAYER(this), dbcid);
				p->AttemptJoin(TO_PLAYER(this), "");
			}
		}
#endif
		// flying auras
		if( m_FlyingAura != 0 && !(at->AreaFlags & AREA_FLYING_PERMITTED) )
		{
			// remove flying buff
			RemoveAura(m_FlyingAura);
		}
	}

	UpdatePvPArea();

	if( IsInWorld() )				// should be
		m_mapMgr->GetStateManager().SendWorldStates(TO_PLAYER(this));
}

void Player::SendTradeUpdate()
{
	Player* pTarget = GetTradeTarget();
	if(pTarget == NULL)
		return;

	WorldPacket data(SMSG_TRADE_STATUS_EXTENDED, 100);
	data << uint8(1);
	data << uint32(0x19);
	data << m_tradeSequence;
	data << m_tradeSequence++;
	data << mTradeGold << uint32(0);
		
	// Items
	for(uint32 Index = 0; Index < 7; ++Index)
	{
		data << uint8(Index);
		Item* pItem = mTradeItems[Index];
		if(pItem != 0)
		{
			ItemPrototype * pProto = pItem->GetProto();
			ASSERT(pProto != 0);

			data << pProto->ItemId;
			data << pProto->DisplayInfoID;
			data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);  // Amount		   OK
			
			// Enchantment stuff
			data << uint32(0);									  // unknown
			data << pItem->GetUInt64Value(ITEM_FIELD_GIFTCREATOR);  // gift creator	 OK
			data << pItem->GetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1);  // Item Enchantment OK
			data << uint32(0);									  // unknown
			data << uint32(0);									  // unknown
			data << uint32(0);									  // unknown
			data << pItem->GetUInt64Value(ITEM_FIELD_CREATOR);	  // item creator	 OK
			data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);  // Spell Charges	OK

			data << pItem->GetItemRandomSuffixFactor();
			data << pItem->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID);
			data << pProto->LockId;								 // lock ID		  OK
			data << pItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
			data << pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);
		}
		else
		{
			for(uint8 j = 0; j < 18; j++)
				data << uint32(0);
		}
	}

	pTarget->GetSession()->SendPacket(&data);
}

void Player::RequestDuel(Player* pTarget)
{
	// We Already Dueling or have already Requested a Duel

	if( DuelingWith != NULL )
		return;

	if( m_duelState != DUEL_STATE_FINISHED )
		return;

	SetDuelState( DUEL_STATE_REQUESTED );

	//Setup Duel
	pTarget->DuelingWith = TO_PLAYER(this);
	DuelingWith = pTarget;

	//Get Flags position
	float dist = CalcDistance(pTarget);
	dist = dist * 0.5f; //half way
	float x = (GetPositionX() + pTarget->GetPositionX()*dist)/(1+dist) + cos(GetOrientation()+(float(M_PI)/2))*2;
	float y = (GetPositionY() + pTarget->GetPositionY()*dist)/(1+dist) + sin(GetOrientation()+(float(M_PI)/2))*2;
	float z = (GetPositionZ() + pTarget->GetPositionZ()*dist)/(1+dist);

	//Create flag/arbiter
	GameObject* pGameObj = GetMapMgr()->CreateGameObject(21680);
	if( pGameObj == NULL || !pGameObj->CreateFromProto(21680,GetMapId(), x, y, z, GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f))
		return;
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

	if( int32(m_duelCountdownTimer) < 0 )
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

		sEventMgr.AddEvent(TO_PLAYER(this), &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
		sEventMgr.AddEvent( DuelingWith, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
	}
}

void Player::DuelBoundaryTest()
{
	//check if in bounds
	if(!IsInWorld())
		return;

	GameObject* pGameObject = GetMapMgr()->GetGameObject(GET_LOWGUID_PART(GetUInt64Value(PLAYER_DUEL_ARBITER)));
	if(!pGameObject)
	{
		EndDuel(DUEL_WINNER_RETREAT);
		return;
	}

	float Dist = GetDistance2dSq(pGameObject);

	if(Dist > 5625.0f)
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
	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_DUEL_COUNTDOWN );
	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_DUEL_BOUNDARY_CHECK );

	for( uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x )
	{
		if( m_auras[x] != NULL && m_auras[x]->WasCastInDuel() )
			RemoveAuraBySlot(x);
	}

	m_duelState = DUEL_STATE_FINISHED;

	if( DuelingWith == NULL )
		return;

	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );
	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_COUNTDOWN );

	// spells waiting to hit
	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_SPELL_DAMAGE_HIT);

	for( uint32 x = 0; x < MAX_AURAS; ++x )
	{
		if( DuelingWith->m_auras[x] != NULL && DuelingWith->m_auras[x]->WasCastInDuel() )
			DuelingWith->RemoveAuraBySlot(x);
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

	GameObject* arbiter = m_mapMgr ? GetMapMgr()->GetGameObject(GET_LOWGUID_PART(GetUInt64Value(PLAYER_DUEL_ARBITER))) : NULL;

	if( arbiter != NULL )
	{
		arbiter->RemoveFromWorld( true );
		arbiter->Destructor();
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
		this->GetSummon()->GetAIInterface()->SetUnitToFollow( TO_PLAYER(this) );
		this->GetSummon()->GetAIInterface()->HandleEvent( EVENT_FOLLOWOWNER, GetSummon(), 0 );
		this->GetSummon()->GetAIInterface()->WipeTargetList();
	}

	// removing auras that kills players after if low HP
	/*RemoveNegativeAuras(); NOT NEEDED. External targets can always gank both duelers with DoTs. :D
	DuelingWith->RemoveNegativeAuras();*/

	//Stop Players attacking so they don't kill the other player
	m_session->OutPacket( SMSG_CANCEL_COMBAT );
	DuelingWith->m_session->OutPacket( SMSG_CANCEL_COMBAT );

	smsg_AttackStop( DuelingWith );
	DuelingWith->smsg_AttackStop( TO_PLAYER(this) );

	DuelingWith->m_duelCountdownTimer = 0;
	m_duelCountdownTimer = 0;

	DuelingWith->DuelingWith = NULL;
	DuelingWith = NULL;
}

void Player::StopMirrorTimer(uint32 Type)
{
	m_session->OutPacket(SMSG_STOP_MIRROR_TIMER, 4, &Type);
}

void Player::EventTeleport(uint32 mapid, float x, float y, float z, float o=0.0f)
{
	SafeTeleport(mapid, 0, LocationVector(x, y, z, o));
}

void Player::ApplyLevelInfo(LevelInfo* Info, uint32 Level)
{
	// Apply level
	uint32 PreviousLevel = GetUInt32Value(UNIT_FIELD_LEVEL);
	SetUInt32Value(UNIT_FIELD_LEVEL, Level);

	// Set next level conditions
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, Info->XPToNextLevel);

	// Set stats
	for(uint32 i = 0; i < 5; ++i)
	{
		BaseStats[i] = Info->Stat[i];
		CalcStat(i);
	}

	CalculateBaseStats();

	// Set health / mana
	SetUInt32Value(UNIT_FIELD_HEALTH,GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	SetUInt32Value(UNIT_FIELD_POWER1,GetUInt32Value(UNIT_FIELD_MAXPOWER1));

	int32 Talents = Level - PreviousLevel;
	if(PreviousLevel < 9)
		Talents = Level - 9;
	if(Talents < 0)
		Reset_Talents();
	else if(Level >= 10)
		availTalentPoints += Talents;

	_UpdateMaxSkillCounts();
	UpdateStats();
	//UpdateChances();
	if (m_playerInfo)
		m_playerInfo->lastLevel = Level;

	GetAchievementInterface()->HandleAchievementCriteriaLevelUp( getLevel() );
	InitGlyphsForLevel();

	OUT_DEBUG("Player %s set parameters to level %u", GetName(), Level);
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
float Player::CalcPercentForRating( uint32 index, uint32 rating )
{
	uint32 relative_index = index - (PLAYER_FIELD_COMBAT_RATING_1);
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

	if(level < 34 && (index == PLAYER_RATING_MODIFIER_BLOCK || index == PLAYER_RATING_MODIFIER_PARRY || index == PLAYER_RATING_MODIFIER_DEFENCE))
		level = 34;

	CombatRatingDBC * pDBCEntry = dbcCombatRating.LookupEntryForced( relative_index * 100 + level - 1 );
	if( pDBCEntry == NULL )
		return (float) rating;
	else
		return (rating / pDBCEntry->val);
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O)
{
	return SafeTeleport(MapID, InstanceID, LocationVector(X, Y, Z, O));
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, LocationVector vec)
{
	//abort duel if other map or new distance becomes bigger then 1600
	if(DuelingWith && (MapID != GetMapId() || m_position.Distance2DSq(vec) >= 1600) )		// 40
		EndDuel(DUEL_WINNER_RETREAT);

	//retrieve our bind point in case vector is 0,0,0
	if( vec.x == 0 && vec.y == 0 && vec.z == 0 )
	{
		vec.x = GetBindPositionX();
		vec.y = GetBindPositionY();
		vec.z = GetBindPositionZ();
		MapID = GetBindMapId();
	}

	bool force_new_world = false;

	// Lookup map info
	MapInfo * mi = WorldMapInfoStorage.LookupEntry(MapID);
	if(!mi)
		return false;

	//are we changing instance or map?
	if(InstanceID && (uint32)m_instanceId != InstanceID)
	{
		force_new_world = true;
#ifndef CLUSTERING
		SetInstanceID(InstanceID);
#endif
	}
	else if(m_mapId != MapID)
	{
		force_new_world = true;
	}

	//We are going to another map
	if(force_new_world )
	{
		//Do we need TBC expansion?
		if(mi->flags & WMI_INSTANCE_XPACK_01 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_01) && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_02))
		{
			WorldPacket msg(SMSG_MOTD, 50);
			msg << uint32(3) << "You must have The Burning Crusade Expansion to access this content." << uint8(0);
			m_session->SendPacket(&msg);
			return false;
		}

		//Do we need WOTLK expansion?
		if(mi->flags & WMI_INSTANCE_XPACK_02 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_02))
		{
			WorldPacket msg(SMSG_MOTD, 50);
			msg << uint32(3) << "You must have Wrath of the Lich King Expansion to access this content." << uint8(0);
			m_session->SendPacket(&msg);
			return false;
		}
		
		// Dismount
		TO_UNIT(this)->Dismount();
	}

	//no flying outside new continents
	if((GetShapeShift() == FORM_FLIGHT || GetShapeShift() == FORM_SWIFT) && MapID != 530 && MapID != 571 )
		RemoveShapeShiftSpell(m_ShapeShifted);

	// make sure player does not drown when teleporting from under water
	if (m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
		m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;

#ifndef CLUSTERING
	//all set...relocate
	_Relocate(MapID, vec, true, force_new_world, InstanceID);
#else
	if(force_new_world)
	{
		WorldPacket data(SMSG_TRANSFER_PENDING, 4);
		data << uint32(MapID);
		GetSession()->SendPacket(&data);
		sClusterInterface.RequestTransfer(TO_PLAYER(this), MapID, InstanceID, vec);
		return true;
	}

	m_sentTeleportPosition = vec;
	SetPosition(vec);
	ResetHeartbeatCoords();

	WorldPacket * data = BuildTeleportAckMsg(vec);
	m_session->SendPacket(data);
	delete data;
#endif
	return true;
}

void Player::SafeTeleport(MapMgr* mgr, LocationVector vec)
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

	if(DuelingWith != NULL)
	{
		EndDuel(DUEL_WINNER_RETREAT);
	}
}

void Player::SetGuildRank(uint32 guildRank)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDRANK;
		sEventMgr.AddEvent(TO_OBJECT(this), &Object::EventSetUInt32Value, field, guildRank, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDRANK,guildRank);
	}
}

void Player::UpdatePvPArea()
{
	ForceAreaUpdate();

	if( m_areaDBC == NULL )
		return;

	AreaTable * zoneDBC = NULL;
	if(m_areaDBC->ZoneId)
		zoneDBC = dbcArea.LookupEntry(m_areaDBC->ZoneId);

	// This is where all the magic happens :P
    if((m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 1) ||
		zoneDBC && ((zoneDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 0) || (zoneDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 1)))
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
            if((m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 0) ||
				zoneDBC && ((zoneDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || (zoneDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 0)))
            {
                if(!IsPvPFlagged()) SetPvPFlag();
                    StopPvPTimer();
                return;
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
                if(!IsPvPFlagged())
					SetPvPFlag();
				StopPvPTimer();
            }

            if(sWorld.GetRealmType() == REALM_PVE)
            {
                if(HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
                {
                    if(!IsPvPFlagged())
						SetPvPFlag();
                }
				else if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE) && IsPvPFlagged() && !m_pvpTimer)
					ResetPvPTimer();
			}
		}
	}

	if(m_areaDBC->AreaFlags & AREA_PVP_ARENA)			/* ffa pvp arenas will come later */
	{
		if(!IsPvPFlagged()) SetPvPFlag();

		SetFFAPvPFlag();
	}
	else
	{
		RemoveFFAPvPFlag();
	}
}

void Player::BuildFlagUpdateForNonGroupSet(uint32 index, uint32 flag)
{
	Group* pGroup = NULL;
	Object* curObj;
	for (Object::InRangeSet::iterator iter = GetInRangeSetBegin(); iter != GetInRangeSetEnd();)
	{
		curObj = *iter;
		iter++;
		if(curObj->IsPlayer())
		{
			pGroup = TO_PLAYER( curObj )->GetGroup();
			if( pGroup == NULL || pGroup != GetGroup())
				BuildFieldUpdatePacket( TO_PLAYER( curObj ), index, flag );
		}
	}
}

void Player::LoginPvPSetup()
{
	// Make sure we know our area ID.
	_EventExploration();

	if(isAlive())
	{
		// Honorless target at 1st entering world. 
		CastSpell(TO_UNIT(this), PLAYER_HONORLESS_TARGET_SPELL, true);

		//initialise BG
		if(GetMapMgr() && GetMapMgr()->m_battleground != NULL && !GetMapMgr()->m_battleground->HasStarted())
			GetMapMgr()->m_battleground->OnAddPlayer(this);
	}
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
                    if(!(m_areaDBC->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == 1) || (m_areaDBC->category == AREAC_HORDE_TERRITORY && GetTeam() == 0))
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
		ForceAreaUpdate();
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
            if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
		        SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
            else
			    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE);
        }
    }
}

void Player::ResetPvPTimer()
{
	SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TIMER);
	m_pvpTimer = sWorld.getIntRate(INTRATE_PVPTIMER);
}

void Player::CalculateBaseStats()
{
	if(!lvlinfo) return;

	memcpy(BaseStats, lvlinfo->Stat, sizeof(uint32) * 5);

	LevelInfo * levelone = objmgr.GetLevelInfo(this->getRace(), this->getClass(),1);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, lvlinfo->HP);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, lvlinfo->BaseHP);
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, lvlinfo->XPToNextLevel);

	if(GetPowerType() == POWER_TYPE_MANA)
	{
		SetUInt32Value(UNIT_FIELD_MAXPOWER1, lvlinfo->Mana);
		SetUInt32Value(UNIT_FIELD_BASE_MANA, lvlinfo->BaseMana);
	}
}

void Player::CompleteLoading()
{	
    // cast passive initial spells	  -- grep note: these shouldnt require plyr to be in world
	SpellSet::iterator itr;
	SpellEntry *info;
	SpellCastTargets targets;
	targets.m_unitTarget = GetGUID();
	targets.m_targetMask = TARGET_FLAG_UNIT;


	for (itr = mSpells.begin(); itr != mSpells.end(); ++itr)
	{
		info = dbcSpell.LookupEntry(*itr);

		if(	info  && (info->Attributes & ATTRIBUTES_PASSIVE) && !( info->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )) 
		{
			if( info->RequiredShapeShift )
			{
				if( !( ((uint32)1 << (GetShapeShift()-1)) & info->RequiredShapeShift ) )
					continue;
			}

			Spell* spell = NULL;
			spell = (new Spell(TO_PLAYER(this),info,true,NULL));
			spell->prepare(&targets);
		}
	}

	ApplySpec(m_talentActiveSpec, true);

	if(!isDead())//only add aura's to the living (death aura set elsewhere)
	{
		std::list<LoginAura>::iterator i,i2;
		for(i =  loginauras.begin();i!=loginauras.end();)
		{
			i2 = i++;

			// this stuff REALLY needs to be fixed - Burlex
			SpellEntry * sp = dbcSpell.LookupEntry((*i2).id);

			//do not load auras that only exist while pet exist. We should recast these when pet is created anyway
			if ( sp->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
				continue;

			Aura* a = NULL;
			a = (new Aura(sp,(*i2).dur,TO_OBJECT(this),TO_UNIT(this)));
			for(uint32 x =0;x<3;x++)
			{
				if(sp->Effect[x]==SPELL_EFFECT_APPLY_AURA)
					a->AddMod(sp->EffectApplyAuraName[x],sp->EffectBasePoints[x]+1,sp->EffectMiscValue[x],x);
			}
			AddAura(a);
			loginauras.erase(i2);
		}
		loginauras.clear();

		// warrior has to have battle stance
		if( getClass() == WARRIOR && !HasAura(2457))
			CastSpell(TO_UNIT(this), dbcSpell.LookupEntry(2457), true);
	}

	// this needs to be after the cast of passive spells, because it will cast ghost form, after the remove making it in ghost alive, if no corpse.

	if(iActivePet)
		SpawnPet(iActivePet);	   // only spawn if >0

	// Banned
	if(IsBanned())
	{
		const char * message = ("This character is banned for  %s.\n You will be kicked in 30 secs.", GetBanReason().c_str());

		// Send warning after 30sec, as he might miss it if it's send inmedeately.
		sEventMgr.AddEvent( TO_PLAYER(this), &Player::_Warn, message, EVENT_UNIT_SENDMESSAGE, 30000, 1, 0);
		sEventMgr.AddEvent( TO_PLAYER(this), &Player::_Kick, EVENT_PLAYER_KICK, 60000, 1, 0 );
	}

	if(m_playerInfo->m_Group)
		sEventMgr.AddEvent(TO_PLAYER(this), &Player::EventGroupFullUpdate, EVENT_UNK, 100, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	if(raidgrouponlysent)
	{
		WorldPacket data2(SMSG_RAID_GROUP_ONLY, 8);
		data2 << uint32(0xFFFFFFFF) << uint32(0);
		GetSession()->SendPacket(&data2);
		raidgrouponlysent=false;
	}

	sInstanceMgr.BuildSavedInstancesForPlayer(TO_PLAYER(this));
}

void Player::OnWorldPortAck()
{
	//only rezz if player is porting to a instance portal
	MapInfo *pPMapinfo = NULL;
	pPMapinfo = WorldMapInfoStorage.LookupEntry(GetMapId());

	if(pPMapinfo != NULL)
	{
		if(isDead() && pPMapinfo->type != INSTANCE_NULL && pPMapinfo->type != INSTANCE_PVP)
			ResurrectPlayer(NULL);

		if(pPMapinfo->HasFlag(WMI_INSTANCE_WELCOME) && GetMapMgr())
		{
			std::string welcome_msg;
			welcome_msg = "Welcome to ";
			welcome_msg += pPMapinfo->name;
			welcome_msg += ". ";
			if(pPMapinfo->type != INSTANCE_NONRAID && m_mapMgr->pInstance)
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
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );
			}break;
		case RANGED_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );
			}break;
		case SPELL_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );
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
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;
		case CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;
		case HIT_AVOIDANCE_RATING:// this is guessed based on layout of other fields
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );//spell
			}break;
		case CRITICAL_AVOIDANCE_RATING:
			{
				// todo. what is it?
			}break;
		case EXPERTISE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_EXPERTISE, val );
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
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );
			}break;
		case ATTACK_POWER:
			{
				ModUnsigned32Value( UNIT_FIELD_ATTACK_POWER_MOD_POS, val );
				ModUnsigned32Value( UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS, val );
			}break;
		case RANGED_ATTACK_POWER:
			{
				ModUnsigned32Value( UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS, val );
			}break;
		case FERAL_ATTACK_POWER:
			{
				// todo
			}break;
		case SPELL_HEALING_DONE:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					HealDoneMod[school] += val;
				}
				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, val );
			}break;
		case SPELL_DAMAGE_DONE:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, val );
				}
			}break;
		case MANA_REGENERATION:
			{
				m_ModInterrMRegen += val;
			}break;
		case ARMOR_PENETRATION_RATING:
			{
				ModUnsigned32Value(PLAYER_RATING_MODIFIER_ARMOR_PENETRATION_RATING, val);
			}break;
		case SPELL_POWER:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, val );
					HealDoneMod[school] += val;
				}
				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, val );
			}break;
		}
}

bool Player::CanSignCharter(Charter * charter, Player* requester)
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
		if(m_auras[x] != NULL)
		{
			Aura* aur=m_auras[x];
			bool skip = false;
			for(uint32 i = 0; i < 3; ++i)
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AURA_128 || 
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
			case 642:
			case 1020:              //divine shield 
			case 11129:             // Combustion 
			case 12043:             // Presence of mind 
			case 16188:             // Natures Swiftness 
			case 17116:             // Natures Swiftness 
			case 23333:             // WSG 
			case 23335:             // WSG 
			case 28682:             // Combustion proc 
			case 31665:             // Master of Subtlety (buff) 
			case 32724:             // Gold Team 
			case 32725:             // Green Team 
			case 32727:             // Arena Preparation 
			case 32728:             // Arena Preparation 
			case 32071:             // Hellfire Superority 
			case 32049:             // Hellfire Superority 
			case 34936:             // Backlash 
			case 35076:             // Blessing of A'dal 
			case 35774:             // Gold Team 
			case 35775:             // Green Team 
			case 44521:             // Preparation? 
			case 44683:             // Team A 
			case 44684:             // Team B 
			case 45438:             // Ice Block 
			case 48418:             // Master Shapeshifter Damage (buff)
			case 48420:             // Master Shapeshifter Critical Strike (buff)
			case 48421:             // Master Shapeshifter Spell Damage (buff)
			case 48422:		        // Master Shapeshifter Healing (buff)
				skip = true;
				break;
			}
			if( aur->m_spellProto->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP) // To prevent food/drink bug
				skip = true;

			//disabled proc spells until proper loading is fixed. Some spells tend to block or not remove when restored
			if(aur->GetSpellProto()->procFlags)
			{
//				OUT_DEBUG("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
				skip = true;
			}
			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
//				OUT_DEBUG("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
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
				if( old_ss > 0 && old_ss != 28)
				{
					if(  ( ((uint32)1 << (old_ss-1)) & reqss ) &&		// we were in the form that required it
						!( ((uint32)1 << (ss-1) & reqss) ) )			// new form doesnt have the right form
					{
						RemoveAuraBySlot(x);
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
							RemoveAuraBySlot(x);
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
	Spell* spe = NULL;
	SpellCastTargets t(GetGUID());

	for( itr = mSpells.begin(); itr != mSpells.end(); ++itr )
	{
		sp = dbcSpell.LookupEntry( *itr );
		if( sp->apply_on_shapeshift_change || sp->Attributes & 64 )		// passive/talent
		{
			if( sp->RequiredShapeShift && ((uint32)1 << (ss-1)) & sp->RequiredShapeShift )
			{
				spe = (new Spell( TO_PLAYER(this), sp, true, NULL ));
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
			spe = (new Spell( TO_PLAYER(this), sp, true, NULL ));
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
		Item* it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
		
		if(!disarmed)
		{	
			if(it)
				speed = it->GetProto()->Delay;
		}
		
		float bonus=ap_bonus*speed;
		float tmp = GetDamageDonePctMod(SCHOOL_NORMAL);
	
		r = BaseDamage[0]+delta+bonus;
		r *= tmp;
		if( it )
			r *= m_WeaponSubClassDamagePct[it->GetProto()->SubClass];

		SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);
	
		r = BaseDamage[1]+delta+bonus;
		r *= tmp;
		if( it )
			r *= m_WeaponSubClassDamagePct[it->GetProto()->SubClass];

		SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);

		uint32 cr = 0;
		if( it )
		{
			if( m_wratings.size() )
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
		it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
		if(it)
		{
			if(!disarmed)
			{
				speed =it->GetProto()->Delay;
			}
			else speed  = 2000;
			
			bonus = ap_bonus * speed;

			r = (BaseOffhandDamage[0]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			r *= m_WeaponSubClassDamagePct[it->GetProto()->SubClass];
			SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE,r>0?r:0);

			r = (BaseOffhandDamage[1]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			r *= m_WeaponSubClassDamagePct[it->GetProto()->SubClass];
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
			if(it->GetProto()->SubClass != 19)//wands do not have bonuses from RAP & ammo
			{
//				ap_bonus = (GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER)+(int32)GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS))/14000.0;
				//modified by Zack : please try to use premade functions if possible to avoid forgetting stuff
				ap_bonus = GetRAP()/14000.0f;
				bonus = ap_bonus*it->GetProto()->Delay;
				
				/*if(GetUInt32Value(PLAYER_AMMO_ID))
				{
					ItemPrototype * xproto=ItemPrototypeStorage.LookupEntry(GetUInt32Value(PLAYER_AMMO_ID));
					if(xproto)
					{
						bonus+=((xproto->Damage[0].Min+xproto->Damage[0].Max)*it->GetProto()->Delay)/2000.0f;
					}
				}
			}else*/ 
			}
			bonus =0;
			
			r = BaseRangedDamage[0]+delta+bonus;
			r *= tmp;
			r *= m_WeaponSubClassDamagePct[it->GetProto()->SubClass];
			SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,r>0?r:0);

			r = BaseRangedDamage[1]+delta+bonus;
			r *= tmp;
			r *= m_WeaponSubClassDamagePct[it->GetProto()->SubClass];
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

	/*	//tmp = 1;
		tmp = 0;
		for(i = damagedone.begin();i != damagedone.end();i++)
		if(i->second.wclass==(uint32)-1)  //any weapon
			tmp += i->second.value/100.0f;
		
		//display only modifiers for any weapon

		// OMG?
		ModFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT ,tmp);*/
 
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
	Item* it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
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

	Player* p = objmgr.GetPlayer(guid);
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
		Unit* target = (m_mapMgr != NULL) ? m_mapMgr->GetUnit(m_comboTarget) : NULL;
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
	if(!SoulStone)
		return;
	uint32 sSoulStone = 0;
	switch(SoulStone)
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
	case 47882:
		{
			sSoulStone = 47883;
		}break;
	}
	this->RemoveAura(sSoulStone);
	this->SoulStone = this->SoulStoneReceiver = 0; //just incase
}

void Player::SoftDisconnect()
{/*
      sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_SOFT_DISCONNECT);
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

void Player::Possess(Unit* pTarget)
{
	if( m_Summon || m_CurrentCharm )
		return;

	ResetHeartbeatCoords();
	if( pTarget->IsPlayer() )
		TO_PLAYER(pTarget)->ResetHeartbeatCoords();

	m_CurrentCharm = pTarget;
	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->DisableAI();
		pTarget->GetAIInterface()->StopMovement(0);
		pTarget->m_redirectSpellPackets = TO_PLAYER(this);
	}

	m_noInterrupt++;
	SetUInt64Value(UNIT_FIELD_CHARM, pTarget->GetGUID());
	SetUInt64Value(PLAYER_FARSIGHT, pTarget->GetGUID());
	pTarget->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(this), pTarget, true);

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
	
	WorldPacket data(SMSG_PET_SPELLS, pTarget->GetAIInterface()->m_spells.size() * 4 + 22);
	data << pTarget->GetGUID();
	data << uint16(0x0000);//unk1
	data << uint32(0x00000101);//unk2
	data << uint32(0x00000000);//unk3

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
	
	data << uint8(0);
	m_session->SendPacket(&data);
}

void Player::UnPossess()
{
	if(m_Summon || !m_CurrentCharm)
		return;

	Unit* pTarget = m_CurrentCharm; 
	m_CurrentCharm = NULL;

	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->EnableAI();
		pTarget->m_redirectSpellPackets = NULL;
	}

	ResetHeartbeatCoords();
	DelaySpeedHack(5000);
	if( pTarget->GetTypeId() == TYPEID_PLAYER )
		TO_PLAYER(pTarget)->DelaySpeedHack(5000);

	m_noInterrupt--;
	SetUInt64Value(PLAYER_FARSIGHT, 0);
	pTarget->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(this), pTarget, false);
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

void Player::RemoveFromBattlegroundQueue(uint32 queueSlot, bool forced)
{
	if(queueSlot > 2) return;
	if( m_pendingBattleground[queueSlot] )
	{
		m_pendingBattleground[queueSlot]->RemovePendingPlayer(TO_PLAYER(this));
		m_pendingBattleground[queueSlot] = NULL;
	}
	m_bgIsQueued[queueSlot] = false;
	m_bgQueueType[queueSlot] = 0;
	m_bgQueueInstanceId[queueSlot] = 0;

	BattlegroundManager.SendBattlegroundQueueStatus(TO_PLAYER(this), queueSlot);
	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_BATTLEGROUND_QUEUE_UPDATE_SLOT_1 + queueSlot);

	if(forced) 
		sChatHandler.SystemMessage(m_session, "You were removed from the queue for the battleground for not joining after 2 minutes.");
}

#ifdef CLUSTERING
void Player::EventRemoveAndDelete()
{

}
#endif

void Player::_AddSkillLine(uint32 SkillLine, uint32 Curr_sk, uint32 Max_sk)
{
	skilllineentry * CheckedSkill = dbcSkillLine.LookupEntry(SkillLine);
	if (!CheckedSkill) //skill doesn't exist, exit here
		return;

	// force to be within limits
	Max_sk = Max_sk > 450 ? 450 : Max_sk;
	Curr_sk = Curr_sk > Max_sk ? Max_sk : Curr_sk < 1 ? 1 : Curr_sk ;

	ItemProf * prof;
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr != m_skills.end())
	{
		if( (Curr_sk > itr->second.CurrentValue && Max_sk >= itr->second.MaximumValue) || (Curr_sk == itr->second.CurrentValue && Max_sk > itr->second.MaximumValue) )
		{
			itr->second.CurrentValue = Curr_sk;
			itr->second.MaximumValue = Max_sk;
			_UpdateMaxSkillCounts();
		}
	}
	else
	{
		PlayerSkill inf;
		inf.Skill = CheckedSkill;
		inf.MaximumValue = Max_sk;
		inf.CurrentValue = ( inf.Skill->id != SKILL_RIDING ? Curr_sk : Max_sk );
		inf.BonusValue = 0;
		m_skills.insert( make_pair( SkillLine, inf ) );
		_UpdateSkillFields();
	}
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
		}
		else
		{
				weapon_proficiency|=prof->subclass;
				//SendSetProficiency(prof->itemclass,weapon_proficiency);
				pr.Profinciency = weapon_proficiency;
		}
		m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof( packetSMSG_SET_PROFICICENCY ), &pr );
	}

	// hackfix for runeforging
	if(SkillLine==SKILL_RUNEFORGING)
	{
		if(!HasSpell(53341))
			addSpell(53341); // Rune of Cinderglacier
		if(!HasSpell(53343))
			addSpell(53343); // Rune of Razorice
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

		ASSERT(f <= availTalentPoints);
		if(itr->second.Skill->type == SKILL_TYPE_PROFESSION)
			SetUInt32Value(f++, itr->first | 0x10000);
		else
			SetUInt32Value(f++, itr->first);

		SetUInt32Value(f++, (itr->second.MaximumValue << 16) | itr->second.CurrentValue);
		SetUInt32Value(f++, itr->second.BonusValue);

		GetAchievementInterface()->HandleAchievementCriteriaReachSkillLevel( itr->second.Skill->id, itr->second.CurrentValue );

		switch(itr->second.Skill->id)
		{
			case SKILL_HERBALISM:
			{
				uint32 skill_base = getRace() == RACE_TAUREN ? 90 : 75;
				if( itr->second.CurrentValue >= skill_base + 375 && !HasSpell( 55503 ) )
				{
					removeSpell(55502,false,false,0);//can't use name_hash
					addSpell( 55503 );												// Lifeblood Rank 6
				}
				else if( itr->second.CurrentValue >= skill_base + 300 && !HasSpell( 55502 ) )
				{
					removeSpell(55501,false,false,0);
					addSpell( 55502 );												// Lifeblood Rank 5
				}
				else if( itr->second.CurrentValue >= skill_base + 225 && !HasSpell( 55501 ) )
				{
					removeSpell(55500,false,false,0);
					addSpell( 55501 );												// Lifeblood Rank 4
				}
				else if( itr->second.CurrentValue >= skill_base + 150 && !HasSpell( 55500 ) )
				{
					removeSpell(55480,false,false,0);
					addSpell( 55500 );												// Lifeblood Rank 3
				}
				else if( itr->second.CurrentValue >= skill_base + 75 && !HasSpell( 55480 ) )
				{
					removeSpell(55428,false,false,0);
					addSpell( 55480 );												// Lifeblood Rank 2
				}
			}break;
			case SKILL_SKINNING:
			{
				if( itr->second.CurrentValue >= 450 && !HasSpell( 53666 ) )
				{
					removeSpell(53665,false,false,0);//can't use namehash here either
					addSpell( 53666 );												// Master of Anatomy Rank 6
				}
				else if( itr->second.CurrentValue >= 375 && !HasSpell( 53665 ) )
				{
					removeSpell(53664,false,false,0);
					addSpell( 53665 );												// Master of Anatomy Rank 5
				}
				else if( itr->second.CurrentValue >= 300 && !HasSpell( 53664 ) )
				{
					removeSpell(53663,false,false,0);
					addSpell( 53664 );												// Master of Anatomy Rank 4
				}
				else if( itr->second.CurrentValue >= 225 && !HasSpell( 53663 ) )
				{
					removeSpell(53662,false,false,0);
					addSpell( 53663 );												// Master of Anatomy Rank 3
				}
				else if( itr->second.CurrentValue >= 150 && !HasSpell( 53662 ) )
				{
					removeSpell(53125,false,false,0);
					addSpell( 53662 );												// Master of Anatomy Rank 2
				}
				else if( itr->second.CurrentValue >= 75 && !HasSpell( 53125 ) )
				{
					addSpell( 53125 );												// Master of Anatomy Rank 1
				}
			}break;
			case SKILL_MINING:
			{
				if( itr->second.CurrentValue >= 450 && !HasSpell( 53040 ) )
				{
					removeSpell(53124,false,false,0);//can't use namehash here either
					addSpell( 53040 );												// Toughness Rank 6
				}
				else if( itr->second.CurrentValue >= 375 && !HasSpell( 53124 ) )
				{
					removeSpell(53123,false,false,0);
					addSpell( 53124 );												// Toughness Rank 5
				}
				else if( itr->second.CurrentValue >= 300 && !HasSpell( 53123 ) )
				{
					removeSpell(53122,false,false,0);
					addSpell( 53123 );												// Toughness Rank 4
				}
				else if( itr->second.CurrentValue >= 225 && !HasSpell( 53122 ) )
				{
					removeSpell(53121,false,false,0);
					addSpell( 53122 );												// Toughness Rank 3
				}
				else if( itr->second.CurrentValue >= 150 && !HasSpell( 53121 ) )
				{
					removeSpell(53120,false,false,0);
					addSpell( 53121 );												// Toughness Rank 2
				}
				else if( itr->second.CurrentValue >= 75 && !HasSpell( 53120 ) )
				{
					addSpell( 53120 );												// Toughness Rank 1
				}
			}break;
		}
		++itr;
	}

	/* Null out the rest of the fields */
	/*for(; f < availTalentPoints; ++f)
	{
		if(m_uint32Values[f] != 0)
			SetUInt32Value(f, 0);
	}*/
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
		_UpdateMaxSkillCounts();
	}
	else
	{	
		uint32 curr_sk = itr->second.CurrentValue;
		itr->second.CurrentValue = min(curr_sk + Count,itr->second.MaximumValue);
		if (itr->second.CurrentValue != curr_sk)
			_UpdateSkillFields();
	}
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
	uint32 new_max;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{

		if( itr->second.Skill->id == SKILL_LOCKPICKING )
		{
			new_max = 5 * getLevel();
		}
		else
		{
			switch(itr->second.Skill->type)
			{
				case SKILL_TYPE_WEAPON:
				{
					new_max = 5 * getLevel();
				}break;

				case SKILL_TYPE_LANGUAGE:
				{
					new_max = 300;
				}break;

				case SKILL_TYPE_PROFESSION:
				case SKILL_TYPE_SECONDARY:
				{
					new_max = itr->second.MaximumValue;
					if(itr->second.Skill->id == SKILL_RIDING)
						itr->second.CurrentValue = new_max;
				}break;

				// default the rest to max = 1, so they won't mess up skill frame for player.
				default:
					new_max = 1;
			}
		}

		//Update new max, forced to be within limits
		itr->second.MaximumValue = new_max > 450 ? 450 : new_max < 1 ? 1 : new_max;

		//Check if current is below nem max, if so, set new current to new max
		itr->second.CurrentValue = itr->second.CurrentValue > new_max ? new_max : itr->second.CurrentValue;
	}
	//Always update client to prevent cached data messing up things later.
	_UpdateSkillFields();
}

void Player::_ModifySkillBonus(uint32 SkillLine, int32 Delta)
{
	if( m_skills.empty() )
		return;

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
		SKILL_LANG_TROLL, SKILL_LANG_GUTTERSPEAK, SKILL_LANG_DRAENEI, SKILL_LANG_WORGEN, SKILL_LANG_GOBLIN, 0 };
	
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
		//Max out riding skill
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
		Unit* pUnit = m_mapMgr ? m_mapMgr->GetUnit(m_spellIndexTypeTargets[Type]) : NULL;
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
	HonorHandler::RecalculateHonorFields(TO_PLAYER(this));
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
	for(uint32 i = 0; i < 12; ++i)
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
		if(m_auras[x]!=NULL)
		{
			Aura* aur=m_auras[x];
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
				//				OUT_DEBUG("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
				skip = true;
			}
			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
				//				OUT_DEBUG("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
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
		m_playerInfo->m_Group->UpdateAllOutOfRangePlayersFor(TO_PLAYER(this));
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
	WorldPacket * data2 = NULL;
	data2 = new WorldPacket(*data);
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
//TODO: LFG channel support for clustering
#ifndef CLUSTERING
	Channel * pChannel = channelmgr.GetChannel("LookingForGroup", TO_PLAYER(this));
	if( pChannel == NULL )
		return;

	if( m_channels.find( pChannel->m_channelId ) == m_channels.end() )
		return;

	pChannel->Part( TO_PLAYER(this), false );
#endif
}

//if we charmed or simply summoned a pet, this function should get called
void Player::EventSummonPet( Pet* new_pet )
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
			RemoveAllAuras( SpellID, GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( this->GetGUID() );
			Spell* spell = NULL;
			spell = (new Spell(TO_PLAYER(this), spellInfo ,true, NULL));	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET )
		{
			RemoveAllAuras( SpellID, GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( new_pet->GetGUID() );
			Spell* spell = NULL;
			spell = (new Spell(TO_PLAYER(this), spellInfo ,true, NULL));	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
	}
	//there are talents that stop working after you gain pet
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] != NULL && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_ON_PET)
			RemoveAuraBySlot(x);
	//pet should inherit some of the talents from caster
	//new_pet->InheritSMMods(); //not required yet. We cast full spell to have visual effect too
}

//if pet/charm died or whatever happened we should call this function
//!! note function might get called multiple times :P
void Player::EventDismissPet()
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x]!= NULL && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET)
			RemoveAuraBySlot(x);
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
	SetThreadName("Compr Movement");
	set<Player*  >::iterator itr;
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

void CMovementCompressorThread::AddPlayer(Player* pPlayer)
{
	m_listLock.Acquire();
	m_players.insert(pPlayer);
	m_listLock.Release();
}

void CMovementCompressorThread::RemovePlayer(Player* pPlayer)
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
		Spell* spe = NULL;
		spe = (new Spell( TO_PLAYER(this), sp, true, NULL ));
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

	DEBUG_LOG("Player","added cooldown for type %u misc %u time %u item %u spell %u", Type, Misc, Time - getMSTime(), ItemId, SpellId);
}

void Player::Cooldown_Add(SpellEntry * pSpell, Item* pItemCaster)
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
			SM_FIValue(SM[SMT_COOLDOWN_DECREASE][0], &cool_time, pSpell->SpellGroupType);
			SM_PIValue(SM[SMT_COOLDOWN_DECREASE][1], &cool_time, pSpell->SpellGroupType);
		}

		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}
	
	if( pSpell->RecoveryTime > 0 )
	{
		cool_time = pSpell->RecoveryTime;
		if( pSpell->SpellGroupType )
		{
			SM_FIValue(SM[SMT_COOLDOWN_DECREASE][0], &cool_time, pSpell->SpellGroupType);
			SM_PIValue(SM[SMT_COOLDOWN_DECREASE][1], &cool_time, pSpell->SpellGroupType);
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

	if( pSpell->SpellGroupType )
	{
		SM_FIValue(SM[SMT_GLOBAL_COOLDOWN][0], &atime, pSpell->SpellGroupType);
		SM_PIValue(SM[SMT_GLOBAL_COOLDOWN][1], &atime, pSpell->SpellGroupType);
	}

	if( atime <= 0 )
		return;

	if( pSpell->StartRecoveryCategory && pSpell->StartRecoveryCategory != 133 )		// if we have a different cool category to the actual spell category - only used by few spells
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->StartRecoveryCategory, mstime + atime, pSpell->Id, 0 );
	/*else if( pSpell->Category )				// cooldowns are grouped
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + pSpell->StartRecoveryTime, pSpell->Id, 0 );*/
	else									// no category, so it's a gcd
	{
		//OUT_DEBUG("Global cooldown adding: %u ms", atime );
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

	ItemSpell* isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->CategoryCooldown > 0)
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, isp->Category, isp->CategoryCooldown + mstime, isp->Id, pProto->ItemId );

	if( isp->Cooldown > 0 )
		_Cooldown_Add( COOLDOWN_TYPE_SPELL, isp->Id, isp->Cooldown + mstime, isp->Id, pProto->ItemId );
}

bool Player::Cooldown_CanCast(ItemPrototype * pProto, uint32 x)
{
	PlayerCooldownMap::iterator itr;
	ItemSpell* isp = &pProto->Spells[x];
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
	if( sWorld.antihack_speed && !m_TransporterGUID && !(m_special_state & UNIT_STATE_CONFUSE) && !m_uint32Values[UNIT_FIELD_CHARMEDBY] && !GetTaxiState() && m_isMoving && GetMapMgr())
	{
		if( ( sWorld.no_antihack_on_gm && GetSession()->HasGMPermissions() ) )
			return; // do not check GMs speed been the config tells us not to.
		if( m_position == m_lastHeartbeatPosition && m_isMoving )
		{
			// this means the client is probably lagging. don't update the timestamp, don't do anything until we start to receive
			// packets again (give the poor laggers a chance to catch up)
			return;
		}

		// simplified; just take the fastest speed. less chance of fuckups too
		float speed = ( m_FlyingAura ) ? m_flySpeed : m_runSpeed;
		if( m_FlyingAura )
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

		if( sWorld.antihack_cheatengine && !m_heartbeatDisable && !m_uint32Values[UNIT_FIELD_CHARM] && m_TransporterGUID == 0 && !m_speedChangeInProgress )
		{
			// latency compensation a little
			if( World::m_speedHackLatencyMultiplier > 0.0f )
				speed += (float(m_session->GetLatency()) / 100.0f) * World::m_speedHackLatencyMultiplier; 

			float distance = m_position.Distance2D( m_lastHeartbeatPosition );
			uint32 time_diff = m_lastMoveTime - m_startMoveTime;
			uint32 move_time = float2int32( ( distance / ( speed * 0.001f ) ) );
			int32 difference = time_diff - move_time;
			DEBUG_LOG("Player","SpeedhackCheck: speed=%f diff=%i dist=%f move=%u tdiff=%u", speed, difference, distance, move_time, time_diff );
			if( difference < World::m_speedHackThreshold )
			{
				if( m_speedhackChances == 1 )
				{
					SetMovement( MOVE_ROOT, 1 );
					BroadcastMessage( "Speedhack detected. Please contact an admin with the below information if you believe this is a false detection." );
					BroadcastMessage( "You will be disconnected in 10 seconds." );
					BroadcastMessage( MSG_COLOR_WHITE"speed: %f diff: %i dist: %f move: %u tdiff: %u\n", speed, difference, distance, move_time, time_diff );
					sCheatLog.writefromsession(GetSession(), "Speed hack detected! Distance: %i, Speed: %f, Move: %u, tdiff: %u", distance, speed, move_time, time_diff);
					if(m_bg)
						m_bg->RemovePlayer(TO_PLAYER(this), false);

					//SafeTeleport(GetMapMgr(), m_lastHeartbeatPosition);
					sEventMgr.AddEvent(TO_PLAYER(this), &Player::_Disconnect, EVENT_PLAYER_KICK, 10000, 1, 0 );
					m_speedhackChances = 0;
				}
				else if (m_cheatEngineChances > 0 )
					m_cheatEngineChances--;
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
	sEventMgr.AddEvent(TO_PLAYER(this), &Player::ResetSpeedHack, EVENT_PLAYER_RESET_HEARTBEAT, ms, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
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

	Player* pl = objmgr.GetPlayer( (uint32)guid );
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
	Player* pl;
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
	Player* pl;
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
	Player* plr;

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

void Player::VampiricSpell(uint32 dmg, Unit* pTarget, SpellEntry *spellinfo)
{
	float fdmg = float(dmg);
	uint32 bonus;
	Group * pGroup = GetGroup();
	SubGroup * pSubGroup = (pGroup != NULL) ? pGroup->GetSubGroup(GetSubGroup()) : NULL;
	GroupMembersSet::iterator itr;
	if( m_vampiricEmbrace > 0 && pTarget->HasAurasOfNameHashWithCaster(SPELL_HASH_VAMPIRIC_EMBRACE, TO_PLAYER(this)) )
	{
		int32 pct = 15;
		uint32 sgt[3] = {4,0,0};
		SM_PIValue(SM[SMT_MISC_EFFECT][1],&pct,sgt);
		bonus = float2int32(fdmg * pct/100.0f);
		if( bonus > 0 )
		{
			Heal(TO_PLAYER(this), 15286, bonus);

			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo )
						Heal( (*itr)->m_loggedInPlayer, 15286, bonus / 5 );
				}
			}
		}
	}

	if( pTarget->m_vampiricTouch > 0 && spellinfo->NameHash == SPELL_HASH_MIND_BLAST )
	{					
		if( GetGroup() )
		{
			uint32 TargetCount = 0;
			GetGroup()->Lock();
			for(uint32 x = 0; x < GetGroup()->GetSubGroupCount(); ++x)
			{
				if( TargetCount == 10 )
					break;

				for(GroupMembersSet::iterator itr = GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); ++itr)
				{
					if((*itr)->m_loggedInPlayer && TargetCount <= 10)
					{
						Player* p_target = (*itr)->m_loggedInPlayer;
						if( p_target->GetPowerType() != POWER_TYPE_MANA )
							continue;

						SpellEntry* Replinishment = dbcSpell.LookupEntryForced( 57669 );
						Spell* pSpell = NULL;
						pSpell = (new Spell(TO_PLAYER(this), Replinishment, true, NULL));
						SpellCastTargets tgt;
						tgt.m_unitTarget = p_target->GetGUID();
						pSpell->prepare(&tgt);
						TargetCount++;
					}
				}
			}
			GetGroup()->Unlock();
		}
		else
		{
			SpellEntry* Replinishment = dbcSpell.LookupEntryForced( 57669 );
			Spell* pSpell = NULL;
			pSpell = (new Spell(TO_PLAYER(this), Replinishment, true, NULL));
			SpellCastTargets tgt;
			tgt.m_unitTarget = GetGUID();
			pSpell->prepare(&tgt);
		}
	}
}

void Player::_FlyhackCheck()
{
	if (!GetMapMgr() || !GetMapMgr()->IsCollisionEnabled())
		return;

	if(!IsFlyHackEligible())
		return;

	// Get the current terrain height at this position
	float height = CollideInterface.GetHeight(GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ() + 2.0f);
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
	EventTeleport(GetMapId(), GetPositionX(), GetPositionY(), height, 0.0f); // Return us to valid coordinates for the next logon.
	GetSession()->Disconnect();
}

bool Player::IsFlyHackEligible()
{
	if(!sWorld.antihack_cheatengine) return false;

	if(GetSession()->HasGMPermissions())
		return false;

	if(!GetMapMgr() || m_FlyingAura || IsStunned() || IsPacified() || IsFeared() || GetTaxiState() || m_TransporterGUID != 0) // Stunned, rooted, riding a flying machine, whatever
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
	memset(m_talentInspectBuffer, 0, TALENT_INSPECT_BYTES);

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

			//OUT_DEBUG( "HandleInspectOpcode: i(%i) j(%i)", i, j );

			if( talent_info == NULL )
				continue;

			//OUT_DEBUG( "HandleInspectOpcode: talent_info->TalentTree(%i) talent_tab_id(%i)", talent_info->TalentTree, talent_tab_id );

			if( talent_info->TalentTree != talent_tab_id )
				continue;

			talent_max_rank = 0;
			for( uint32 k = 5; k > 0; --k )
			{
				//OUT_DEBUG( "HandleInspectOpcode: k(%i) RankID(%i) HasSpell(%i) TalentTree(%i) Tab(%i)", k, talent_info->RankID[k - 1], player->HasSpell( talent_info->RankID[k - 1] ), talent_info->TalentTree, talent_tab_id );
				if( talent_info->RankID[k - 1] != 0 && HasSpell( talent_info->RankID[k - 1] ) )
				{
					talent_max_rank = k;
					break;
				}
			}

			//OUT_DEBUG( "HandleInspectOpcode: RankID(%i) talent_max_rank(%i)", talent_info->RankID[talent_max_rank-1], talent_max_rank );

			if( talent_max_rank <= 0 )
				continue;

			talent_index = talent_tab_pos;

			std::map< uint32, uint32 >::iterator itr = sWorld.InspectTalentTabPos.find( talent_info->TalentID );

			if( itr != sWorld.InspectTalentTabPos.end() )
				talent_index += itr->second;
			//else
			//OUT_DEBUG( "HandleInspectOpcode: talent(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i)", talent_info->TalentID, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset );

			rank_index = ( uint32( ( talent_index + talent_max_rank - 1 ) / 7 ) ) * 8  + ( uint32( ( talent_index + talent_max_rank - 1 ) % 7 ) );
			rank_slot = rank_index / 8;
			rank_offset = rank_index % 8;

			if( rank_slot < TALENT_INSPECT_BYTES )
			{
				uint32 v = (uint32)m_talentInspectBuffer[rank_slot];
				v |= ( 1 << rank_offset );
				m_talentInspectBuffer[rank_offset] |= v;
			};

			DEBUG_LOG( "Player","HandleInspectOpcode: talent(%i) talent_max_rank(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i)", talent_info->TalentID, talent_max_rank, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset );
		}

		std::map< uint32, uint32 >::iterator itr = sWorld.InspectTalentTabSize.find( talent_tab_id );

		if( itr != sWorld.InspectTalentTabSize.end() )
			talent_tab_pos += itr->second;

	}
}

void Player::GenerateLoot(Corpse* pCorpse)
{
	if( m_bg == NULL )
		return;

	// default gold
	pCorpse->ClearLoot();
	pCorpse->m_loot.gold = 500;
		
	if( m_bg != NULL )
		m_bg->HookGenerateLoot(TO_PLAYER(this), pCorpse);
}

uint32 Player::GetMaxPersonalRating()
{
	uint32 maxrating = 0;
	int i;

	ASSERT(m_playerInfo != NULL);

	for (i=0; i<NUM_ARENA_TEAM_TYPES; i++)
	{
		if(m_playerInfo->arenaTeam[i] != NULL)
		{
			ArenaTeamMember *m = m_playerInfo->arenaTeam[i]->GetMemberByGuid(GetLowGUID());
			if (m)
			{
				if (m->PersonalRating > maxrating) maxrating = m->PersonalRating;
			}
			else
			{
				sLog.outString("%s: GetMemberByGuid returned NULL for player guid = %u\n", __FUNCTION__, m_playerInfo->guid);
			}
		}
	}

	return maxrating;
}

void Player::FullHPMP()
{
	if( isDead() )
		ResurrectPlayer(NULL);

    SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
    SetUInt32Value(UNIT_FIELD_POWER1, GetUInt32Value(UNIT_FIELD_MAXPOWER1));
    SetUInt32Value(UNIT_FIELD_POWER4, GetUInt32Value(UNIT_FIELD_MAXPOWER4));
}

void Player::SetKnownTitle( int32 title, bool set )
{	
	if( !( HasKnownTitle( title ) ^ set ) ||
			title < 1 || title >= TITLE_END)
		return;

	if(title == GetUInt32Value(PLAYER_CHOSEN_TITLE)) // if it's the chosen title, remove it
		SetUInt32Value(PLAYER_CHOSEN_TITLE, 0);

	uint32 field = PLAYER__FIELD_KNOWN_TITLES;
	uint32 title2 = title;
	if(title > 63)
	{
		field = PLAYER__FIELD_KNOWN_TITLES1;
		title2 = title - 64;
	}
	uint64 current = GetUInt64Value( field );
	if( set )
		SetUInt64Value( field, current | uint64(1) << title2 );
	else
		SetUInt64Value( field, current & ~uint64(1) << title2 );

	// PVP Ranks, Legacy Stuff :P
	if( title < PVPTITLE_GLADIATOR && title > PVPTITLE_NONE )
		SetPVPRank( GetTeam() ? title - 14 : title );
	
	WorldPacket data( SMSG_TITLE_EARNED, 8 );
	data << uint32( title ) << uint32( set ? 1 : 0 );
	m_session->SendPacket( &data );
}

void Player::CheckSpellUniqueTargets(SpellEntry *sp, uint64 guid)
{

	HM_NAMESPACE::hash_map<uint32, uint64>::iterator itr;
	itr = mSpellsUniqueTargets.find(sp->NameHash);

	if(itr != mSpellsUniqueTargets.end())
	{
		if (GetMapMgr())
		{
			Unit* unit = GetMapMgr()->GetUnit(itr->second);
			if (unit)
			{
				unit->RemoveAuraByNameHash(sp->NameHash);
			}
		}

		mSpellsUniqueTargets[sp->NameHash] = guid;
	}
	else
	{
		mSpellsUniqueTargets[sp->NameHash] = guid;
	}
}

bool Player::HasBattlegroundQueueSlot()
{
	if( m_bgIsQueued[0] &&  m_bgIsQueued[1] && m_bgIsQueued[2] )
		return false;

	return true;
}

uint32 Player::GetBGQueueSlot()
{
	for(uint32 i = 0; i < 3; ++i)
	{
		if( !m_bgIsQueued[i] )
			return i;
	}

	return 0; // Using 0 so if bad code comes up, we don't make an access violation :P
}

uint32 Player::GetBGQueueSlotByBGType(uint32 type)
{
	for(uint32 i = 0; i < 3; ++i)
	{
		if( m_bgQueueType[i] == type)
			return i;
	}
	return 0;
}

uint32 Player::HasBGQueueSlotOfType(uint32 type)
{
	for(uint32 i = 0; i < 3; ++i)
	{
		if( m_bgIsQueued[i] &&
			m_bgQueueType[i] == type)
			return i;
	}

	return 4;
}

void Player::RetroactiveCompleteQuests()
{
	set<uint32>::iterator itr = m_finishedQuests.begin();
	for(; itr != m_finishedQuests.end(); ++itr)
	{
		Quest * pQuest = QuestStorage.LookupEntry( *itr );
		if(!pQuest) continue;

		GetAchievementInterface()->HandleAchievementCriteriaCompleteQuestsInZone( pQuest->zone_id );
	}
}

// Update glyphs after level change
void Player::InitGlyphsForLevel()
{
	// Enable number of glyphs depending on level
	uint32 level = getLevel();
	uint32 glyph_mask = 0; 
	if(level == 80)
		glyph_mask = 6;
	else if(level >= 70)
		glyph_mask = 5;
	else if(level >= 50)
		glyph_mask = 4;
	else if(level >= 30)
		glyph_mask = 3;
	else if(level >= 15)
		glyph_mask = 2;
	SetUInt32Value(PLAYER_GLYPHS_ENABLED, (1 << glyph_mask) -1 );
}

void Player::InitGlyphSlots()
{
	for(uint32 i = 0; i < 6; ++i)
		SetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS_1 + i, 21 + i);
}

void Player::UnapplyGlyph(uint32 slot)
{
	if(slot > 5)
		return; // Glyph doesn't exist
	// Get info
	uint32 glyphId = GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot);
	if(glyphId == 0)
		return;
	GlyphPropertyEntry *glyph = dbcGlyphProperty.LookupEntry(glyphId);
	if(!glyph)
		return;
	SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot, 0);
	RemoveAllAuras(glyph->SpellID);
}

static const uint32 glyphType[6] = {0, 1, 1, 0, 1, 0};

uint8 Player::SetGlyph(uint32 slot, uint32 glyphId)
{
	if(slot < 0 || slot > 6)
		return SPELL_FAILED_INVALID_GLYPH;
	// Get info
	GlyphPropertyEntry *glyph = dbcGlyphProperty.LookupEntry(glyphId);
	if(!glyph)
		return SPELL_FAILED_INVALID_GLYPH;

	uint32 type = glyph->Type;
	for(uint32 x = 0; x < GLYPHS_COUNT; ++x)
	{
		if(m_specs[m_talentActiveSpec].glyphs[x] == glyphId && slot != x)
			return SPELL_FAILED_UNIQUE_GLYPH;
	}
	
	if( glyphType[slot] != glyph->Type || // Glyph type doesn't match
			(GetUInt32Value(PLAYER_GLYPHS_ENABLED) & (1 << slot)) == 0) // slot is not enabled
		return SPELL_FAILED_INVALID_GLYPH;

	UnapplyGlyph(slot);
	SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot, glyphId);
	m_specs[m_talentActiveSpec].glyphs[slot] = glyphId;
	CastSpell(TO_PLAYER(this), glyph->SpellID, true);	// Apply the glyph effect
	return 0;
}

void Player::ConvertRune(uint8 index, uint8 value)
{
	ASSERT(index < 6);
	m_runes[index] = value;
	m_runemask |= (1 << index);
	if(value >= RUNE_TYPE_RECHARGING)
		return;

	WorldPacket data(SMSG_CONVERT_RUNE, 2);
	data << (uint8)index;
	data << (uint8)value;
	if( GetSession() )
		GetSession()->SendPacket(&data);
}

bool Player::CanUseRunes(uint8 blood, uint8 frost, uint8 unholy)
{
	uint8 death = 0;
	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_BLOOD && blood )
			blood--;
		if( m_runes[ i ] == RUNE_TYPE_FROST && frost )
			frost--;
		if( m_runes[ i ] == RUNE_TYPE_UNHOLY && unholy )
			unholy--;

		if( m_runes[ i ] == RUNE_TYPE_DEATH )
			death++;
	}

	uint8 res = blood + frost + unholy;
	if( res == 0 )
		return true;

	if( death >= (blood + frost + unholy) )
		return true;

	return false;
}

void Player::ScheduleRuneRefresh(uint8 index, bool forceDeathRune)
{
	sEventMgr.RemoveEvents(TO_PLAYER(this), EVENT_PLAYER_RUNE_REGEN + index);
	sEventMgr.AddEvent(TO_PLAYER(this), &Player::ConvertRune, (uint8)index, (forceDeathRune ? uint8(RUNE_TYPE_DEATH) : baseRunes[index]), EVENT_PLAYER_RUNE_REGEN + index, 10000, 0, 0);
}

void Player::UseRunes(uint8 blood, uint8 frost, uint8 unholy, SpellEntry* pSpell)
{
	uint8 death = 0;
	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_BLOOD && blood )
		{
			blood--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;
			ScheduleRuneRefresh(i);
			continue;
		}
		if( m_runes[ i ] == RUNE_TYPE_FROST && frost )
		{
			frost--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;

			if( pSpell && pSpell->NameHash == SPELL_HASH_DEATH_STRIKE || pSpell->NameHash == SPELL_HASH_OBLITERATE && Rand(pSpell->procChance) )
				ScheduleRuneRefresh(i, true);
			else
				ScheduleRuneRefresh(i);
			continue;
		}
		if( m_runes[ i ] == RUNE_TYPE_UNHOLY && unholy )
		{
			unholy--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;
			if( pSpell && pSpell->NameHash == SPELL_HASH_DEATH_STRIKE || pSpell->NameHash == SPELL_HASH_OBLITERATE && Rand(pSpell->procChance) )
				ScheduleRuneRefresh(i, true);
			else
				ScheduleRuneRefresh(i);
			continue;
		}

		if( m_runes[ i ] == RUNE_TYPE_DEATH )
			death++;
	}

	uint8 res = blood + frost + unholy;

	if( res == 0 )
		return;

	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_DEATH && res )
		{
			res--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;
			ScheduleRuneRefresh(i);
		}
	}
}

uint8 Player::TheoreticalUseRunes(uint8 blood, uint8 frost, uint8 unholy)
{
	uint8 runemask = m_runemask;
	uint8 death = 0;
	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_BLOOD && blood )
		{
			blood--;
			runemask &= ~(1 << i);
		}
		if( m_runes[ i ] == RUNE_TYPE_FROST && frost )
		{
			frost--;
			runemask &= ~(1 << i);
		}
		if( m_runes[ i ] == RUNE_TYPE_UNHOLY && unholy )
		{
			unholy--;
			runemask &= ~(1 << i);
		}

		if( m_runes[ i ] == RUNE_TYPE_DEATH )
			death++;
	}

	uint8 res = blood + frost + unholy;

	if( res == 0 )
		return runemask;

	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_DEATH && res )
		{
			res--;
			runemask &= ~(1 << i);
		}
	}
	
	return runemask;
}

void Player::GroupUninvite(Player* targetPlayer, PlayerInfo *targetInfo)
{
	if ( targetPlayer == NULL && targetInfo == NULL )
	{
		GetSession()->SendPartyCommandResult(this, 0, "", ERR_PARTY_CANNOT_FIND);
		return;
	}

	if ( !InGroup() || targetInfo->m_Group != GetGroup() )
	{
		GetSession()->SendPartyCommandResult(this, 0, "", ERR_PARTY_IS_NOT_IN_YOUR_PARTY);
		return;
	}

	if ( !IsGroupLeader() || targetInfo->m_Group->HasFlag(GROUP_FLAG_BATTLEGROUND_GROUP) )	// bg group
	{
		if(this != targetPlayer)
		{
			GetSession()->SendPartyCommandResult(this, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
			return;
		}
	}

	if(m_bg)
		return;

	Group *group = GetGroup();

	if(group)
		group->RemovePlayer(targetInfo);
}

void Player::SetTaximaskNode(uint32 nodeidx, bool Unset)
{
	uint8  field   = uint8((nodeidx - 1) / 32);
	uint32 submask = 1<<((nodeidx-1)%32);
	if(Unset)
	{
		//We have this node allready? Remove it
		if ((GetTaximask(field)& submask) == submask)  
			SetTaximask(field,(GetTaximask(field)& ~submask));
	}
	else
	{
		//We don't have this node allready? Add it.
		if ((GetTaximask(field)& submask) != submask)  
			SetTaximask(field,(GetTaximask(field)|submask));
	}
}

uint16 Player::FindQuestSlot( uint32 questid )
{
	for ( uint16 i = 0; i < 25; ++i )
		if( (GetUInt32Value(PLAYER_QUEST_LOG_1_1 + i * 5)) == questid )
			return i;

	return 25;
}

void Player::EventCheckCurrencies()
{
	ItemIterator itr(m_ItemInterface);
	itr.BeginSearch();
	for(; !itr.End(); ++itr)
	{
		if(itr.Grab() == NULL)
			continue;

		if(itr->GetProto() == NULL || (itr->GetProto()->BagFamily & ITEM_TYPE_CURRENCY) == 0)
			continue;

		if(itr.GetCurrentSlot() >= INVENTORY_CURRENCY_1 && itr.GetCurrentSlot() <= INVENTORY_CURRENCY_32)
			continue;

		uint32 count = itr->GetCount();
		Item * i = m_ItemInterface->SafeRemoveAndRetreiveItemByGuid(itr->GetGUID(), false);

		uint8 slot = m_ItemInterface->GetInventorySlotById(itr->GetEntry());
		while(slot != (uint8)-1 && slot < INVENTORY_CURRENCY_1)
		{
			Item * itm = m_ItemInterface->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, slot, false);
			count += itm->GetCount();
			if(itm->IsInWorld())
				itm->RemoveFromWorld();
			itm->DeleteFromDB();
			delete itm;

			slot = m_ItemInterface->GetInventorySlotById(itr->GetEntry());
		}

		if(slot == INVENTORY_SLOT_NOT_SET)
		{
			// No currency item with this entry in currencies slots
			Item * itm = objmgr.CreateItem(itr->GetEntry(), this);
			itm->SetCount(count);
			if(m_ItemInterface->AddItemToFreeSlot(itm) == ADD_ITEM_RESULT_OK)
				itm->m_isDirty = true;
		}
		else
		{
			Item * itm = m_ItemInterface->GetInventoryItem(slot);
			count += itm->GetCount();
			itm->SetCount(count);
			itm->m_isDirty = true;
		}

		if(i->IsInWorld())
			i->RemoveFromWorld();
		i->DeleteFromDB();
		delete i;
	}
	itr.EndSearch();
}