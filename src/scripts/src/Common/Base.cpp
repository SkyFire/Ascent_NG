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

#include "Base.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class TargetType
TargetType::TargetType( uint32 pTargetGen, TargetFilter pTargetFilter, uint32 pMinTargetNumber, uint32 pMaxTargetNumber )
{
	mTargetGenerator = pTargetGen;
	mTargetFilter = pTargetFilter;
	mTargetNumber[ 0 ] = pMinTargetNumber;	// Unused array for now
	mTargetNumber[ 1 ] = pMaxTargetNumber;
};

TargetType::~TargetType()
{
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class SpellDesc
SpellDesc::SpellDesc(SpellEntry* pInfo, SpellFunc pFnc, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId)
{
	mInfo = pInfo;
	mSpellFunc = pFnc;
	mTargetType = pTargetType;
	mChance = max(min(pChance, 100.0f), 0.0f);
	mCastTime = pCastTime;
	mCooldown = pCooldown;
	mMinRange = pMinRange;
	mMaxRange = pMaxRange;
	mStrictRange = pStrictRange;
	mEnabled = true;
	mPredefinedTarget = NULL;
	mLastCastTime = 0;
	AddEmote(pText, pTextType, pSoundId);
}

SpellDesc::~SpellDesc()
{
	DeleteArray(mEmotes);
}

EmoteDesc* SpellDesc::AddEmote(const char* pText, TextType pType, uint32 pSoundId)
{
	EmoteDesc* NewEmote = NULL;
	if( pText || pSoundId )
	{
		NewEmote = new EmoteDesc(pText, pType, pSoundId);
		mEmotes.push_back(NewEmote);
	}
	return NewEmote;
}

void SpellDesc::TriggerCooldown(uint32 pCurrentTime)
{
	uint32 CurrentTime = ( pCurrentTime == 0 ) ? (uint32)time(NULL) : pCurrentTime;
	mLastCastTime = CurrentTime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class AscentScriptCreatureAI
AscentScriptCreatureAI::AscentScriptCreatureAI(Creature* pCreature) : CreatureAIScript(pCreature)
{
	mRunToTargetCache = NULL;
	mRunToTargetSpellCache = NULL;
	mTimerIdCounter = 0;
	mAIUpdateFrequency = DEFAULT_UPDATE_FREQUENCY;
	mBaseAttackTime = _unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
	mDespawnWhenInactive = false;
}

AscentScriptCreatureAI::~AscentScriptCreatureAI()
{
	DeleteArray(mOnDiedEmotes);
	DeleteArray(mOnTargetDiedEmotes);
	DeleteArray(mOnCombatStartEmotes);
	DeleteArray(mOnTauntEmotes);
	DeleteArray(mSpells);
}

bool AscentScriptCreatureAI::GetCanMove()
{
	return _unit->GetAIInterface()->m_canMove;
};

void AscentScriptCreatureAI::SetCanMove( bool pCanMove )
{
	if ( pCanMove )
		_unit->UnRoot();
	else 
		_unit->Root();
};

void AscentScriptCreatureAI::MoveTo( AscentScriptCreatureAI* pCreature, RangeStatusPair pRangeStatus )
{
	MoveTo( pCreature->_unit, pRangeStatus );
};

void AscentScriptCreatureAI::MoveTo( Unit* pUnit, RangeStatusPair pRangeStatus )
{
	if ( pRangeStatus.first == RangeStatus_TooClose )
		_unit->GetAIInterface()->_CalcDestinationAndMove( pUnit, pRangeStatus.second );
	else if ( pRangeStatus.first == RangeStatus_TooFar )
		MoveTo( pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ() );
};

void AscentScriptCreatureAI::MoveTo( float pX, float pY, float pZ, bool pRun )
{
	if ( pRun )
		_unit->GetAIInterface()->m_moveRun = true;

	_unit->GetAIInterface()->MoveTo( pX, pY, pZ, 0 );
};

void AscentScriptCreatureAI::MoveToSpawnOrigin()
{
	MoveTo(_unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ());
}

void AscentScriptCreatureAI::StopMovement()
{
	_unit->GetAIInterface()->StopMovement(0);
}

void AscentScriptCreatureAI::SetFlyMode(bool pValue)
{
	if( pValue && !_unit->GetAIInterface()->m_moveFly )
	{
		WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);
		_unit->GetAIInterface()->m_moveFly = true;
	}
	else if( !pValue && _unit->GetAIInterface()->m_moveFly )
	{
		WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
		data << _unit->GetNewGUID();
		data << uint32(0);
		_unit->SendMessageToSet(&data, false);
		_unit->GetAIInterface()->m_moveFly = false;
	}
}

bool AscentScriptCreatureAI::GetCanEnterCombat()
{
	return _unit->GetAIInterface()->GetAllowedToEnterCombat();
}

void AscentScriptCreatureAI::SetCanEnterCombat(bool pCanEnterCombat)
{
	_unit->SetUInt64Value(UNIT_FIELD_FLAGS, ( pCanEnterCombat ) ? 0 : UNIT_FLAG_NOT_ATTACKABLE_9);
	_unit->GetAIInterface()->SetAllowedToEnterCombat(pCanEnterCombat);
}

bool AscentScriptCreatureAI::IsInCombat()
{
	return _unit->CombatStatus.IsInCombat();
}

void AscentScriptCreatureAI::DelayNextAttack(int32 pMilliseconds)
{
	_unit->setAttackTimer(pMilliseconds, false);
}

void AscentScriptCreatureAI::SetDespawnWhenInactive(bool pValue)
{
	mDespawnWhenInactive = pValue;
}

void AscentScriptCreatureAI::SetBehavior(BehaviorType pBehavior)
{
	switch( pBehavior )
	{
		case Behavior_Default:		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL); break;
		case Behavior_Melee:		_unit->GetAIInterface()->setCurrentAgent(AGENT_MELEE); break;
		case Behavior_Ranged:		_unit->GetAIInterface()->setCurrentAgent(AGENT_RANGED); break;
		case Behavior_Spell:		_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL); break;
		case Behavior_Flee:			_unit->GetAIInterface()->setCurrentAgent(AGENT_FLEE); break;
		case Behavior_CallForHelp:	_unit->GetAIInterface()->setCurrentAgent(AGENT_CALLFORHELP); break;
		default:					sLog.outDebug("AscentScriptCreatureAI::SetBehavior() : Invalid behavior type!\n"); break;
	}
}

BehaviorType AscentScriptCreatureAI::GetBehavior()
{
	switch( _unit->GetAIInterface()->getCurrentAgent() )
	{
		case AGENT_NULL:		return Behavior_Default;
		case AGENT_MELEE:		return Behavior_Melee;
		case AGENT_RANGED:		return Behavior_Ranged;
		case AGENT_FLEE:		return Behavior_Flee;
		case AGENT_SPELL:		return Behavior_Spell;
		case AGENT_CALLFORHELP:	return Behavior_CallForHelp;
		default:				sLog.outDebug("AscentScriptCreatureAI::SetBehavior() : Invalid behavior type!\n"); return Behavior_Default;
	}
}

void AscentScriptCreatureAI::SetAllowMelee(bool pAllow)
{
	_unit->GetAIInterface()->disable_melee = !pAllow;
}

bool AscentScriptCreatureAI::GetAllowMelee()
{
	return !_unit->GetAIInterface()->disable_melee;
}

void AscentScriptCreatureAI::SetAllowRanged(bool pAllow)
{
	_unit->GetAIInterface()->disable_ranged = !pAllow;
}

bool AscentScriptCreatureAI::GetAllowRanged()
{
	return !_unit->GetAIInterface()->disable_ranged;
}

void AscentScriptCreatureAI::SetAllowSpell(bool pAllow)
{
	_unit->GetAIInterface()->disable_spell = !pAllow;
}

bool AscentScriptCreatureAI::GetAllowSpell()
{
	return !_unit->GetAIInterface()->disable_spell;
}

void AscentScriptCreatureAI::SetAllowTargeting(bool pAllow)
{
	_unit->GetAIInterface()->disable_targeting = !pAllow;
}

bool AscentScriptCreatureAI::GetAllowTargeting()
{
	return !_unit->GetAIInterface()->disable_targeting;
}

void AscentScriptCreatureAI::AggroNearestUnit(int pInitialThreat)
{
	Unit* NearestRandomTarget = GetBestUnitTarget(TargetFilter_Closest);
	if( NearestRandomTarget )
	{
		_unit->GetAIInterface()->AttackReaction(NearestRandomTarget, pInitialThreat);
		OnCombatStart(NearestRandomTarget);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void AscentScriptCreatureAI::AggroRandomUnit(int pInitialThreat)
{
	Unit* RandomTarget = GetBestUnitTarget();
	if( RandomTarget )
	{
		_unit->GetAIInterface()->AttackReaction(RandomTarget, pInitialThreat);
		OnCombatStart(RandomTarget);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void AscentScriptCreatureAI::AggroNearestPlayer(int pInitialThreat)
{
	Unit* NearestRandomPlayer = GetBestPlayerTarget(TargetFilter_Closest);
	if( NearestRandomPlayer )
	{
		_unit->GetAIInterface()->AttackReaction(NearestRandomPlayer, pInitialThreat);
		OnCombatStart(NearestRandomPlayer);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void AscentScriptCreatureAI::AggroRandomPlayer(int pInitialThreat)
{
	Unit* RandomPlayer = GetBestPlayerTarget();
	if( RandomPlayer )
	{
		_unit->GetAIInterface()->AttackReaction(RandomPlayer, pInitialThreat);
		OnCombatStart(RandomPlayer);	//Patch, for some reason, OnCombatStart isn't called in this case
	}
}

void AscentScriptCreatureAI::ClearHateList()
{
	_unit->GetAIInterface()->ClearHateList();	//without leaving combat
}

void AscentScriptCreatureAI::WipeHateList()
{
	_unit->GetAIInterface()->WipeHateList();
}

int32 AscentScriptCreatureAI::GetHealthPercent()
{
	return _unit->GetHealthPct();
}

int32 AscentScriptCreatureAI::GetManaPercent()
{
	return _unit->GetManaPct();
}

void AscentScriptCreatureAI::Regenerate()
{
	_unit->RegenerateHealth( false );
	_unit->RegeneratePower(false);
}

bool AscentScriptCreatureAI::IsAlive()
{
	return _unit->isAlive();
}

void AscentScriptCreatureAI::SetScale(float pScale)
{
	_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, pScale);
}

float AscentScriptCreatureAI::GetScale()
{
	return _unit->GetFloatValue(OBJECT_FIELD_SCALE_X);
}

void AscentScriptCreatureAI::SetDisplayId(uint32 pDisplayId)
{
	_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID, pDisplayId);
}

void AscentScriptCreatureAI::SetWieldWeapon(bool pValue)
{
	if( pValue && _unit->GetUInt32Value(UNIT_FIELD_BYTES_2) != 1 )
	{
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
	}
	else if( !pValue && _unit->GetUInt32Value(UNIT_FIELD_BYTES_2) != 0 )
	{
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
	}
}

void AscentScriptCreatureAI::SetDisplayWeapon(bool pMainHand, bool pOffHand)
{ 
	SetDisplayWeaponIds(pMainHand ? _unit->GetProtoItemDisplayId(0) : 0, pOffHand ? _unit->GetProtoItemDisplayId(1) : 0);
}

//change SetDisplayweaponIds to take 2 parameters ? pitem1id,pitem2id
void AscentScriptCreatureAI::SetDisplayWeaponIds(uint32 pItem1Id, uint32 pItem2Id)
{
	//Main Hand
	_unit->SetWeaponDisplayId(0, pItem1Id);

	//Off Hand
	_unit->SetWeaponDisplayId(1, pItem2Id);
}

float AscentScriptCreatureAI::GetRange(AscentScriptCreatureAI* pCreature)
{
	return ( _unit != pCreature->_unit ) ? _unit->CalcDistance(pCreature->_unit) : 0;
}

float AscentScriptCreatureAI::GetRangeToUnit(Unit* pUnit)
{
	return _unit->CalcDistance(pUnit);
}

bool AscentScriptCreatureAI::IsHeroic()
{
	if ( _unit->GetMapMgr() == NULL )
		return false;
	if ( _unit->GetMapMgr()->iInstanceMode != MODE_HEROIC )
		return false;

	return true;
};

Player* AscentScriptCreatureAI::GetNearestPlayer()
{
	return _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ());
}

GameObject* AscentScriptCreatureAI::GetNearestGameObject(uint32 pGameObjectId)
{
	return _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), pGameObjectId);
}

AscentScriptCreatureAI* AscentScriptCreatureAI::GetNearestCreature(uint32 pCreatureId)
{
	Creature* NearestCreature = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), pCreatureId);
	return ( NearestCreature ) ? static_cast<AscentScriptCreatureAI*>(NearestCreature->GetScript()) : NULL;
}

AscentScriptCreatureAI* AscentScriptCreatureAI::SpawnCreature(uint32 pCreatureId, bool pForceSameFaction)
{
	return SpawnCreature(pCreatureId, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), pForceSameFaction);
}

AscentScriptCreatureAI* AscentScriptCreatureAI::SpawnCreature(uint32 pCreatureId, float pX, float pY, float pZ, float pO, bool pForceSameFaction)
{
	Creature* NewCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(pCreatureId, pX, pY, pZ, pO, true, false, 0, 0);
	AscentScriptCreatureAI* CreatureScriptAI = ( NewCreature ) ? static_cast<AscentScriptCreatureAI*>(NewCreature->GetScript()) : NULL;
	if( pForceSameFaction && NewCreature )
	{
		uint32 FactionTemplate = _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE);
		NewCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, FactionTemplate);
	}
	return CreatureScriptAI;
}

Unit*	AscentScriptCreatureAI::ForceCreatureFind( uint32 pCreatureId )
{
	return ForceCreatureFind( pCreatureId, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
};

Unit* AscentScriptCreatureAI::ForceCreatureFind( uint32 pCreatureId, float pX, float pY, float pZ )
{
	Unit* UnitPtr = NULL; 
	MapMgr* Mgr = _unit->GetMapMgr();
	if ( Mgr == NULL )
		return UnitPtr;

	UnitPtr = Mgr->GetInterface()->GetCreatureNearestCoords( pX, pY, pZ, pCreatureId );
	if ( UnitPtr == NULL )
	{
		UnitArray Array;
		for ( uint32 i = 0; i != Mgr->m_CreatureHighGuid; ++i )
		{
			UnitPtr = Mgr->m_CreatureStorage[ i ];
			if ( UnitPtr != NULL )
			{ 
				if ( UnitPtr->GetEntry() == pCreatureId && UnitPtr != _unit )
					Array.push_back( UnitPtr );
			};
		};

		if ( Array.size() == 1 )
			return Array[ 0 ];

		UnitPtr = NULL;
		float Distance, NearestDistance = 99999;
		for ( UnitArray::iterator UnitIter = Array.begin(); UnitIter != Array.end(); ++UnitIter )
		{
			Distance = _unit->CalcDistance( pX, pY, pZ, ( *UnitIter )->GetPositionX(), ( *UnitIter )->GetPositionY(), ( *UnitIter )->GetPositionZ() );
			if ( Distance < NearestDistance )
			{
				NearestDistance = Distance;
				UnitPtr = ( *UnitIter );
			};
		};
	};

	return UnitPtr;
};

void AscentScriptCreatureAI::Despawn( uint32 pDelay, uint32 pRespawnTime )
{
	_unit->Despawn( pDelay, pRespawnTime );
};

SpellDesc* AscentScriptCreatureAI::AddSpell(uint32 pSpellId, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId)
{
	//Cannot add twice same spell id	- M4ksiu: Disabled, until I rewrite SetPhase(...) function to not disable same spells that are in different phases
	//SpellDesc* NewSpell = FindSpellById(pSpellId);
	//if( NewSpell ) return NewSpell;
	SpellDesc* NewSpell = NULL;

	//Find spell info from spell id
	SpellEntry* Info = dbcSpell.LookupEntry(pSpellId);

#ifdef USE_DBC_SPELL_INFO
	float CastTime = ( Info->CastingTimeIndex ) ? GetCastTime(dbcSpellCastTime.LookupEntry(Info->CastingTimeIndex)) : pCastTime;
	int32 Cooldown = Info->RecoveryTime;
	float MinRange = ( Info->rangeIndex ) ? GetMinRange(dbcSpellRange.LookupEntry(Info->rangeIndex)) : pMinRange;
	float MaxRange = ( Info->rangeIndex ) ? GetMaxRange(dbcSpellRange.LookupEntry(Info->rangeIndex)) : pMaxRange;
	sLog.mOutColor(TYELLOW, "AscentScriptCreatureAI::AddSpell(%u) : casttime=%.1f cooldown=%d minrange=%.1f maxrange=%.1f\n", pSpellId, CastTime, Cooldown, MinRange, MaxRange);
#else
	float CastTime = pCastTime;
	int32 Cooldown = pCooldown;
	float MinRange = pMinRange;
	float MaxRange = pMaxRange;
#endif

	//Create new spell
	NewSpell = new SpellDesc(Info, NULL, pTargetType, pChance, CastTime, Cooldown, MinRange, MaxRange, pStrictRange, pText, pTextType, pSoundId);
	mSpells.push_back(NewSpell);
	return NewSpell;
}

SpellDesc* AscentScriptCreatureAI::AddSpellFunc(SpellFunc pFnc, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange, bool pStrictRange, char* pText, TextType pTextType, uint32 pSoundId)
{
	if( !pFnc ) return NULL;

	//Create new spell
	SpellDesc* NewSpell = new SpellDesc(NULL, pFnc, pTargetType, pChance, pCastTime, pCooldown, pMinRange, pMaxRange, pStrictRange, pText, pTextType, pSoundId);
	mSpells.push_back(NewSpell);
	return NewSpell;
}

void AscentScriptCreatureAI::CastSpell(SpellDesc* pSpell)
{
	if( !IsSpellScheduled(pSpell) ) mQueuedSpells.push_back(pSpell);
}

void AscentScriptCreatureAI::CastSpellNowNoScheduling(SpellDesc* pSpell)
{
	if( CastSpellInternal(pSpell) )
	{
		DelayNextAttack(CalcSpellAttackTime(pSpell));
	}
}

SpellDesc* AscentScriptCreatureAI::FindSpellById(uint32 pSpellId)
{
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		if( (*SpellIter)->mInfo && (*SpellIter)->mInfo->Id == pSpellId ) return (*SpellIter);
	}
	return NULL;
}

SpellDesc* AscentScriptCreatureAI::FindSpellByFunc(SpellFunc pFnc)
{
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		if( (*SpellIter)->mSpellFunc == pFnc ) return (*SpellIter);
	}
	return NULL;
}

bool AscentScriptCreatureAI::IsCasting()
{
	return _unit->isCasting();
}

void AscentScriptCreatureAI::ApplyAura(uint32 pSpellId)
{
	_unit->CastSpell(_unit, dbcSpell.LookupEntry(pSpellId), true);
}

void AscentScriptCreatureAI::RemoveAura(uint32 pSpellId)
{
	_unit->RemoveAura(pSpellId);
}

void AscentScriptCreatureAI::RemoveAuraOnPlayers(uint32 pSpellId)
{
	for( unordered_set<Player*>::iterator PlayerIter = _unit->GetInRangePlayerSetBegin(); PlayerIter != _unit->GetInRangePlayerSetEnd(); ++PlayerIter ) 
	{// need testing
		(*PlayerIter)->RemoveAura(pSpellId);
	}
}

void AscentScriptCreatureAI::RemoveAllAuras()
{
	_unit->RemoveAllAuras();
}

void AscentScriptCreatureAI::TriggerCooldownOnAllSpells()
{
	uint32 CurrentTime = (uint32)time(NULL);
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		(*SpellIter)->TriggerCooldown(CurrentTime);
	}
}

void AscentScriptCreatureAI::CancelAllCooldowns()
{
	for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
	{
		(*SpellIter)->mLastCastTime = 0;
	}
}

EmoteDesc* AscentScriptCreatureAI::AddEmote(EventType pEventType, const char* pText, TextType pType, uint32 pSoundId)
{
	EmoteDesc* NewEmote = NULL;
	if( pText || pSoundId )
	{
		NewEmote = new EmoteDesc(pText, pType, pSoundId);
		switch( pEventType )
		{
			case Event_OnCombatStart:	mOnCombatStartEmotes.push_back(NewEmote); break;
			case Event_OnTargetDied:	mOnTargetDiedEmotes.push_back(NewEmote); break;
			case Event_OnDied:			mOnDiedEmotes.push_back(NewEmote); break;
			case Event_OnTaunt:			mOnTauntEmotes.push_back(NewEmote); break;
			default:					sLog.outDebug("AscentScriptCreatureAI::AddEmote() : Invalid event type!\n"); break;
		}
	}
	return NewEmote;
}

void AscentScriptCreatureAI::RemoveEmote(EventType pEventType, EmoteDesc* pEmote)
{
	switch( pEventType )
	{
		case Event_OnCombatStart:	DeleteItem(mOnCombatStartEmotes, pEmote); break;
		case Event_OnTargetDied:	DeleteItem(mOnTargetDiedEmotes, pEmote); break;
		case Event_OnDied:			DeleteItem(mOnDiedEmotes, pEmote); break;
		case Event_OnTaunt:			DeleteItem(mOnTauntEmotes, pEmote); break;
		default:					sLog.outDebug("AscentScriptCreatureAI::RemoveEmote() : Invalid event type!\n"); break;
	}
}

void AscentScriptCreatureAI::RemoveAllEmotes(EventType pEventType)
{
	switch( pEventType )
	{
		case Event_OnCombatStart:	DeleteArray(mOnCombatStartEmotes); break;
		case Event_OnTargetDied:	DeleteArray(mOnTargetDiedEmotes); break;
		case Event_OnDied:			DeleteArray(mOnDiedEmotes); break;
		case Event_OnTaunt:			DeleteArray(mOnTauntEmotes); break;
		default:					sLog.outDebug("AscentScriptCreatureAI::RemoveAllEmotes() : Invalid event type!\n"); break;
	}
}

void AscentScriptCreatureAI::Emote(EmoteDesc* pEmote)
{
	if( pEmote ) Emote(pEmote->mText.c_str(), pEmote->mType, pEmote->mSoundId);
}

void AscentScriptCreatureAI::Emote(const char* pText, TextType pType, uint32 pSoundId)
{
	if( pText && strlen(pText) > 0 )
	{
		switch( pType )
		{
			case Text_Say:		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, pText); break;
			case Text_Yell:		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, pText); break;
			case Text_Emote:	_unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, pText); break;
			default:			sLog.outDebug("AscentScriptCreatureAI::Emote() : Invalid text type!\n"); break;
		}
	}
	if( pSoundId > 0 ) _unit->PlaySoundToSet(pSoundId);
}

uint32 AscentScriptCreatureAI::AddTimer(int32 pDurationMillisec)
{
	int32 Index = mTimerIdCounter++;
	mTimers.push_back(std::make_pair(Index, pDurationMillisec));
	return Index;
}

int32 AscentScriptCreatureAI::GetTimer(int32 pTimerId)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId )
		{
			return TimerIter->second;
		}
	}

	return 0;
}

void AscentScriptCreatureAI::RemoveTimer(int32& pTimerId)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId )
		{
			mTimers.erase(TimerIter);
			pTimerId = INVALIDATE_TIMER;
			break;
		}
	}
}

void AscentScriptCreatureAI::ResetTimer(int32 pTimerId, int32 pDurationMillisec)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId )
		{
			TimerIter->second = pDurationMillisec;
			break;
		}
	}
}

bool AscentScriptCreatureAI::IsTimerFinished(int32 pTimerId)
{
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if( TimerIter->first == pTimerId ) return ( TimerIter->second <= 0 ) ? true : false;
	}
	return false;
}

void AscentScriptCreatureAI::CancelAllTimers()
{
	mTimers.clear();
	mTimerIdCounter = 0;
}

void AscentScriptCreatureAI::SetTargetToChannel(Unit* pTarget, uint32 pSpellId)
{
	if (pTarget == NULL)
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
	else
		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, pTarget->GetGUID());

	_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, pSpellId);
}

Unit* AscentScriptCreatureAI::GetTargetToChannel()
{

	return _unit->GetMapMgr()->GetUnit(_unit->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
}

void AscentScriptCreatureAI::SetAIUpdateFreq(uint32 pUpdateFreq)
{
	if( mAIUpdateFrequency != pUpdateFreq )
	{
		mAIUpdateFrequency = pUpdateFreq;
		ModifyAIUpdateEvent(mAIUpdateFrequency);
	}
}

uint32 AscentScriptCreatureAI::GetAIUpdateFreq()
{
	return mAIUpdateFrequency;
}

void AscentScriptCreatureAI::AddLootToTable(LootTable* pTable, uint32 pItemID, uint32 pChance, uint32 pMinCount, uint32 pMaxCount, uint32 pFFA)
{
	LootDesc loot;
	loot.mItemID = pItemID;
	loot.mChance = pChance;
	loot.mMinCount = pMinCount;
	loot.mMaxCount = pMaxCount;
	loot.mFFA = pFFA;
	pTable->push_back(loot);
}

void AscentScriptCreatureAI::ClearLoot(Unit* pTarget)
{
	pTarget->ClearLoot();
}

void AscentScriptCreatureAI::AddLootFromTable(Unit* pTarget, LootTable* pTable, uint32 pCount)
{
	uint32 total = 0;
	for (LootTable::iterator it = pTable->begin(); it != pTable->end(); ++it) total += (*it).mChance;
	for (uint32 count = 0; count < pCount; ++count)
	{
		uint32 result = RandomUInt(total);
		uint32 sum = 0;
		for (LootTable::iterator it = pTable->begin(); it != pTable->end(); ++it)
		{
			sum += (*it).mChance;
			if (result <= sum)
			{
				LootMgr::getSingleton().AddLoot(pTarget->GetLoot(), (*it).mItemID, (*it).mMinCount, (*it).mMaxCount, (*it).mFFA);
				break;
			}
		}
	}
}

void AscentScriptCreatureAI::SetGoldLoot(Unit* pTarget, uint32 pMinGold, uint32 pMaxGold)
{
	pTarget->GetLoot()->gold = RandomUInt(pMaxGold - pMinGold) + pMinGold;
}

void AscentScriptCreatureAI::AddLoot(Unit* pTarget, uint32 pItemID, uint32 pMinCount, uint32 pMaxCount, uint32 pFFA)
{
	LootMgr::getSingleton().AddLoot(pTarget->GetLoot(), pItemID, pMinCount, pMaxCount, pFFA);
}

void AscentScriptCreatureAI::AddRareLoot(Unit* pTarget, uint32 pItemID, float pPercentChance)
{
	float result = RandomFloat(100.0f);
	if (result <= pPercentChance)
		LootMgr::getSingleton().AddLoot(pTarget->GetLoot(), pItemID, 1, 1, 0);
}

WayPoint* AscentScriptCreatureAI::CreateWaypoint(int pId, uint32 pWaittime, uint32 pMoveFlag, Coords pCoords)
{
	WayPoint * wp = _unit->CreateWaypointStruct();
	wp->id = pId;
	wp->x = pCoords.mX;
	wp->y = pCoords.mY;
	wp->z = pCoords.mZ;
	wp->o = pCoords.mO;
	wp->waittime = pWaittime;
	wp->flags = pMoveFlag;
	wp->forwardemoteoneshot = 0;
	wp->forwardemoteid = 0;
	wp->backwardemoteoneshot = 0;
	wp->backwardemoteid = 0;
	wp->forwardskinid = 0;
	wp->backwardskinid = 0;
	return wp;
}

void AscentScriptCreatureAI::AddWaypoint(WayPoint* pWayPoint)
{
	_unit->GetAIInterface()->addWayPoint(pWayPoint);
}

void AscentScriptCreatureAI::ForceWaypointMove(uint32 pWaypointId)
{
	if (GetCanEnterCombat())
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	if (!GetCanMove())
		SetCanMove(true);

	StopMovement();
	_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
	SetMoveType(Move_WantedWP);
	SetWaypointToMove(pWaypointId);
}

void AscentScriptCreatureAI::SetWaypointToMove(uint32 pWaypointId)
{
	_unit->GetAIInterface()->setWaypointToMove(pWaypointId);
}

void AscentScriptCreatureAI::StopWaypointMovement()
{
	SetBehavior(Behavior_Default);
	_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
	SetMoveType(Move_DontMoveWP);
	SetWaypointToMove(0);
}

void AscentScriptCreatureAI::SetMoveType(MoveType pMoveType)
{
	switch( pMoveType )
	{
		case Move_None:				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_NONE); break;
		case Move_RandomWP:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_RANDOMWP); break;
		case Move_CircleWP:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_CIRCLEWP); break;
		case Move_WantedWP:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP); break;
		case Move_DontMoveWP:		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP); break;
		case Move_Quest:			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_QUEST); break;
		case Move_ForwardThenStop:	_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP); break;
		default:					sLog.outDebug("AscentScriptCreatureAI::SetMoveType() : Invalid move type!\n"); break;
	}
}

MoveType AscentScriptCreatureAI::GetMoveType()
{
	switch( _unit->GetAIInterface()->getMoveType() )
	{
		case MOVEMENTTYPE_NONE:				return Move_None;
		case MOVEMENTTYPE_RANDOMWP:			return Move_RandomWP;
		case MOVEMENTTYPE_CIRCLEWP:			return Move_CircleWP;
		case MOVEMENTTYPE_WANTEDWP:			return Move_WantedWP;
		case MOVEMENTTYPE_DONTMOVEWP:		return Move_DontMoveWP;
		case MOVEMENTTYPE_QUEST:			return Move_Quest;
		case MOVEMENTTYPE_FORWARDTHANSTOP:	return Move_ForwardThenStop;
		default:							sLog.outDebug("AscentScriptCreatureAI::GetMoveType() : Invalid move type!\n"); return Move_None;
	}
}

uint32 AscentScriptCreatureAI::GetCurrentWaypoint()
{
	return _unit->GetAIInterface()->getCurrentWaypoint();
}

size_t AscentScriptCreatureAI::GetWaypointCount()
{
	return _unit->GetAIInterface()->GetWayPointsCount();
}

bool AscentScriptCreatureAI::HasWaypoints()
{
	return _unit->GetAIInterface()->hasWaypoints();
}

void AscentScriptCreatureAI::OnCombatStart(Unit* pTarget)
{
	RandomEmote(mOnCombatStartEmotes);
	SetBehavior(Behavior_Melee);
    RegisterAIUpdateEvent(mAIUpdateFrequency);
}

void AscentScriptCreatureAI::OnCombatStop(Unit* pTarget)
{
	CancelAllSpells();
	CancelAllTimers();
	RemoveAllAuras();
	SetCanMove(true);
	SetBehavior(Behavior_Default);
	//_unit->GetAIInterface()->SetAIState(STATE_IDLE);				// Fix for stuck mobs that don't regen
	RemoveAIUpdateEvent();
	if( mDespawnWhenInactive ) Despawn(DEFAULT_DESPAWN_TIMER);
}

void AscentScriptCreatureAI::OnTargetDied(Unit* pTarget)
{
	if( GetHealthPercent() > 0 )	//Prevent double yelling (OnDied and OnTargetDied)
	{
		RandomEmote(mOnTargetDiedEmotes);
	}
}

void AscentScriptCreatureAI::OnDied(Unit* pKiller)
{
	RandomEmote(mOnDiedEmotes);
	CancelAllSpells();
	CancelAllTimers();
	RemoveAllAuras();
	RemoveAIUpdateEvent();
	if( mDespawnWhenInactive ) Despawn(DEFAULT_DESPAWN_TIMER);
}

void AscentScriptCreatureAI::AIUpdate()
{
	SpellDesc*	Spell;
	uint32		CurrentTime = (uint32)time(NULL);

	//Elapse timers
	for( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		TimerIter->second -= mAIUpdateFrequency;
	}

	//Check if we have a spell scheduled to be cast
	for( SpellDescList::iterator SpellIter = mScheduledSpells.begin(); SpellIter != mScheduledSpells.end(); )
	{
		Spell = (*SpellIter);
		if( CastSpellInternal(Spell, CurrentTime) )	//Can fail if we are already casting a spell, or if the spell is on cooldown
		{
			if( !mScheduledSpells.empty() )//temporary crashfix, we should use mutax here, but it needs more investigation
				mScheduledSpells.erase(SpellIter);

			break;
		}
		else
			++SpellIter;
	}

	//Do not schedule spell if we are *currently* casting a non-instant cast spell
	if( !IsCasting() && !mRunToTargetCache )
	{
		//Check if have queued spells that needs to be scheduled before we go back to random casting
		if( !mQueuedSpells.empty() )
		{
			Spell = mQueuedSpells.front();
			mScheduledSpells.push_back(Spell);
			mQueuedSpells.pop_front();

			//Stop melee attack for a short while for scheduled spell cast
			if( Spell->mCastTime >= 0 )
			{
				DelayNextAttack(mAIUpdateFrequency);
				if( Spell->mCastTime > 0 )
				{
					SetCanMove(false);
					SetBehavior(Behavior_Spell);
				}
			}
			return;	//Scheduling one spell at a time, exit now
		}

		//Try our chance at casting a spell (Will actually be cast on next ai update, so we just
		//schedule it. This is needed to avoid next dealt melee damage while we cast the spell.)
		float ChanceRoll = RandomFloat(100), ChanceTotal = 0;
		for( SpellDescArray::iterator SpellIter = mSpells.begin(); SpellIter != mSpells.end(); ++SpellIter )
		{
			Spell = (*SpellIter);
			if( Spell->mEnabled == false ) continue;
			if( Spell->mChance == 0 ) continue;

			//Check if spell won the roll
			if( (Spell->mChance == 100 || (ChanceRoll >= ChanceTotal && ChanceRoll < ChanceTotal + Spell->mChance)) &&
				(Spell->mLastCastTime + Spell->mCooldown <= CurrentTime) &&
				!IsSpellScheduled(Spell) )
			{
				mScheduledSpells.push_back(Spell);

				//Stop melee attack for a short while for scheduled spell cast
				if( Spell->mCastTime >= 0 )
				{
					DelayNextAttack(mAIUpdateFrequency + CalcSpellAttackTime(Spell));
					if( Spell->mCastTime > 0 )
					{
						SetCanMove(false);
						SetBehavior(Behavior_Spell);
					}
				}
				return;	//Scheduling one spell at a time, exit now
			}
			else if( Spell->mChance != 100 ) ChanceTotal += Spell->mChance;	//Only add spells that aren't 100% chance of casting
		}

		//Go back to default behavior since we didn't decide anything
		SetCanMove(true);
		SetBehavior(Behavior_Melee);

		//Random taunts
		if( ChanceRoll >= 95 ) RandomEmote(mOnTauntEmotes);
	}
}

void AscentScriptCreatureAI::Destroy()
{
	delete this;
};

bool AscentScriptCreatureAI::IsSpellScheduled(SpellDesc* pSpell)
{
	return ( std::find(mScheduledSpells.begin(), mScheduledSpells.end(), pSpell) == mScheduledSpells.end() ) ? false : true;
}

void AscentScriptCreatureAI::CancelAllSpells()
{
	mQueuedSpells.clear();
	mScheduledSpells.clear();
	PopRunToTargetCache();
}

bool AscentScriptCreatureAI::CastSpellInternal(SpellDesc* pSpell, uint32 pCurrentTime)
{
	if( pSpell == NULL) return false;
	//Do not cast if we are already casting
	if( IsCasting() ) return false;

	//We do not cast in special states such as : stunned, feared, silenced, charmed, asleep, confused and if they are not ignored
	if ( ( ~pSpell->mTargetType.mTargetFilter & TargetFilter_IgnoreSpecialStates ) && _unit->m_special_state & ( UNIT_STATE_STUN | UNIT_STATE_FEAR | UNIT_STATE_SLEEP | UNIT_STATE_SILENCE | UNIT_STATE_CHARM | UNIT_STATE_CONFUSE ) )
		return false;

	//Do not cast if we are in cooldown
	uint32 CurrentTime = ( pCurrentTime == 0 ) ? (uint32)time(NULL) : pCurrentTime;
	if( pSpell->mLastCastTime + pSpell->mCooldown > CurrentTime ) return false;

	//Check range before casting
	Unit* Target = GetTargetForSpell( pSpell );
	if( Target )
	{
		RangeStatusPair Status;
		if( pSpell->mTargetType.mTargetFilter & TargetFilter_InRangeOnly || ( Status = GetSpellRangeStatusToUnit( Target, pSpell ) ).first == RangeStatus_Ok )
		{
			//Safe-delay if we got special state flag before
			DelayNextAttack( CalcSpellAttackTime( pSpell ) );

			//If we were running to a target, stop because we're in range now
			PopRunToTargetCache();

			//Do emote associated with this spell
			RandomEmote(pSpell->mEmotes);

			//Cast spell now
			if( pSpell->mInfo ) CastSpellOnTarget(Target, pSpell->mTargetType, pSpell->mInfo, ( pSpell->mCastTime == 0 ) ? true : false);
			else if( pSpell->mSpellFunc ) pSpell->mSpellFunc(pSpell, this, Target, pSpell->mTargetType);
			else sLog.outDebug("AscentScriptCreatureAI::CastSpellInternal() : Invalid spell!\n");

			//Store cast time for cooldown
			pSpell->mLastCastTime = CurrentTime;
			return true;
		}
		else if( !pSpell->mStrictRange ) //Target is out of range, run to it
		{
			PushRunToTargetCache( Target, pSpell, Status );
			return false;
		}
	}

	//If we get here, its because the RunToTarget changed type, so its no longer valid, clear it
	PopRunToTargetCache();
	DelayNextAttack(0);		//Cancel attack delay
	return true;			//No targets possible? Consider spell casted nonetheless
}

void AscentScriptCreatureAI::CastSpellOnTarget( Unit* pTarget, TargetType pType, SpellEntry* pEntry, bool pInstant )
{
	switch( pType.mTargetGenerator )
	{
		case TargetGen_Self:
		case TargetGen_Current:
		case TargetGen_Predefined:
		case TargetGen_RandomUnit:
		case TargetGen_RandomPlayer:
			_unit->CastSpell( pTarget, pEntry, pInstant );
			break;

		case TargetGen_RandomUnitApplyAura:
		case TargetGen_RandomPlayerApplyAura:
			pTarget->CastSpell( pTarget, pEntry, pInstant );
			break;

		case TargetGen_Destination:
		case TargetGen_RandomUnitDestination:
		case TargetGen_RandomPlayerDestination:
			_unit->CastSpellAoF( pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pEntry, pInstant );
			break;

		default:
			sLog.outDebug("AscentScriptCreatureAI::CastSpellOnTarget() : Invalid target type!\n");
			break;
	};
};

int32 AscentScriptCreatureAI::CalcSpellAttackTime(SpellDesc* pSpell)
{
#ifdef USE_DBC_SPELL_INFO
	return mBaseAttackTime + int32(pSpell->mCastTime);
#else
	return mBaseAttackTime + int32(pSpell->mCastTime * 1000);
#endif
}

RangeStatusPair AscentScriptCreatureAI::GetSpellRangeStatusToUnit( Unit* pTarget, SpellDesc* pSpell )
{
	if ( pSpell->mTargetType.mTargetGenerator != TargetGen_Self && pTarget != _unit && (pSpell->mMinRange > 0 || pSpell->mMaxRange > 0) )
	{
		float Range = GetRangeToUnit(pTarget);
		if ( pSpell->mMinRange > 0 && (Range < pSpell->mMinRange) )
			return make_pair( RangeStatus_TooClose, pSpell->mMinRange );
		if ( pSpell->mMaxRange > 0 && (Range > pSpell->mMaxRange) )
			return make_pair( RangeStatus_TooFar, pSpell->mMaxRange );
	};

	return make_pair( RangeStatus_Ok, 0.0f );
};

Unit* AscentScriptCreatureAI::GetTargetForSpell( SpellDesc* pSpell )
{
	//Check if run-to-target cache and return it if its valid
	if ( mRunToTargetCache && mRunToTargetSpellCache == pSpell && IsValidUnitTarget( mRunToTargetCache, TargetFilter_None ) )
		return mRunToTargetCache;

	//Find a suitable target for the described situation :)
	switch( pSpell->mTargetType.mTargetGenerator )
	{
	case TargetGen_Self:
		if ( !IsAlive() )
			return NULL;
		if ( ( pSpell->mTargetType.mTargetFilter & TargetFilter_Wounded ) && _unit->GetHealthPct() >= 99 )
			return NULL;

		return _unit;
	case TargetGen_SecondMostHated:
		return _unit->GetAIInterface()->GetSecondHated();
	case TargetGen_Current:
	case TargetGen_Destination:
		return _unit->GetAIInterface()->GetNextTarget();
	case TargetGen_Predefined:
		return pSpell->mPredefinedTarget;
	case TargetGen_RandomPlayer:
	case TargetGen_RandomPlayerApplyAura:
	case TargetGen_RandomPlayerDestination:
		return GetBestPlayerTarget( pSpell->mTargetType.mTargetFilter, pSpell->mMinRange, pSpell->mMaxRange );
	case TargetGen_RandomUnit:
	case TargetGen_RandomUnitApplyAura:
	case TargetGen_RandomUnitDestination:
		return GetBestUnitTarget( pSpell->mTargetType.mTargetFilter, pSpell->mMinRange, pSpell->mMaxRange );
	default:
		sLog.outDebug("AscentScriptCreatureAI::GetTargetForSpell() : Invalid target type!\n");
		return NULL;
	};
};

Unit* AscentScriptCreatureAI::GetBestPlayerTarget( TargetFilter pTargetFilter, float pMinRange, float pMaxRange )
{
	//Build potential target list
	UnitArray TargetArray;
	for ( unordered_set< Player* >::iterator PlayerIter = _unit->GetInRangePlayerSetBegin(); PlayerIter != _unit->GetInRangePlayerSetEnd(); ++PlayerIter ) 
	{ 
		if ( IsValidUnitTarget( *PlayerIter, pTargetFilter, pMinRange, pMaxRange ) )
			TargetArray.push_back( TO_UNIT( *PlayerIter ) );
	};

	return ChooseBestTargetInArray( TargetArray, pTargetFilter );
};

Unit* AscentScriptCreatureAI::GetBestUnitTarget( TargetFilter pTargetFilter, float pMinRange, float pMaxRange )
{
	//Build potential target list
	UnitArray TargetArray;
	if ( pTargetFilter & TargetFilter_Friendly )
	{
		for ( unordered_set< Object* >::iterator ObjectIter = _unit->GetInRangeSetBegin(); ObjectIter != _unit->GetInRangeSetEnd(); ++ObjectIter )
		{
			if ( IsValidUnitTarget( *ObjectIter, pTargetFilter, pMinRange, pMaxRange ) )
				TargetArray.push_back( TO_UNIT( *ObjectIter ) );
		};

		if ( IsValidUnitTarget( _unit, pTargetFilter ) )
			TargetArray.push_back( _unit );	//Also add self as possible friendly target
	}
	else
	{
		for ( unordered_set< Object* >::iterator ObjectIter = _unit->GetInRangeOppFactsSetBegin(); ObjectIter != _unit->GetInRangeOppFactsSetEnd(); ++ObjectIter )
		{
			if ( IsValidUnitTarget( *ObjectIter, pTargetFilter, pMinRange, pMaxRange ) )
				TargetArray.push_back( TO_UNIT( *ObjectIter ) );
		};
	};

	return ChooseBestTargetInArray( TargetArray, pTargetFilter );
};

Unit* AscentScriptCreatureAI::ChooseBestTargetInArray( UnitArray& pTargetArray, TargetFilter pTargetFilter )
{
	//If only one possible target, return it
	if ( pTargetArray.size() == 1 )
		return pTargetArray[ 0 ];

	//Find closest unit if requested
	if ( pTargetFilter & TargetFilter_Closest )
		return GetNearestTargetInArray( pTargetArray );

	//Find second most hated if requested
	if ( pTargetFilter & TargetFilter_SecondMostHated )
		return GetSecondMostHatedTargetInArray( pTargetArray );

	//Choose random unit in array
	return ( pTargetArray.size() > 1 ) ? pTargetArray[ RandomUInt( ( uint32 )pTargetArray.size() - 1 ) ] : NULL;
};

Unit* AscentScriptCreatureAI::GetNearestTargetInArray(UnitArray& pTargetArray)
{
	Unit* NearestUnit = NULL;
	float Distance, NearestDistance = 99999;
	for ( UnitArray::iterator UnitIter = pTargetArray.begin(); UnitIter != pTargetArray.end(); ++UnitIter )
	{
		Distance = GetRangeToUnit( TO_UNIT( *UnitIter ) );
		if ( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestUnit = ( *UnitIter );
		};
	};

	return NearestUnit;
};

Unit* AscentScriptCreatureAI::GetSecondMostHatedTargetInArray( UnitArray& pTargetArray )
{
	Unit*	TargetUnit = NULL;
	Unit*	MostHatedUnit = NULL;
	Unit*	CurrentTarget = TO_UNIT( _unit->GetAIInterface()->GetNextTarget() );
	uint32	Threat = 0, HighestThreat = 0;
	for ( UnitArray::iterator UnitIter = pTargetArray.begin(); UnitIter != pTargetArray.end(); ++UnitIter )
	{
		TargetUnit = TO_UNIT( *UnitIter );
		if ( TargetUnit != CurrentTarget )
		{
			Threat = _unit->GetAIInterface()->getThreatByPtr( TargetUnit );
			if ( Threat > HighestThreat )
			{
				MostHatedUnit = TargetUnit;
				HighestThreat = Threat;
			};
		};
	};

	return MostHatedUnit;
};

bool AscentScriptCreatureAI::IsValidUnitTarget( Object* pObject, TargetFilter pFilter, float pMinRange, float pMaxRange )
{
	//Make sure its a valid unit
	if ( !pObject->IsUnit() )
		return false;
	if ( pObject->GetInstanceID() != _unit->GetInstanceID() )
		return false;

	Unit* UnitTarget = TO_UNIT( pObject );
	//Skip dead ( if required ), feign death or invisible targets
	if ( pFilter & TargetFilter_Corpse )
	{
		if ( UnitTarget->isAlive() || !UnitTarget->IsCreature() || TO_CREATURE( UnitTarget )->creature_info->Rank == ELITE_WORLDBOSS )
			return false;
	}
	else if ( !UnitTarget->isAlive() )
		return false;

	if ( UnitTarget->IsPlayer() && TO_PLAYER( UnitTarget )->m_isGmInvisible )
		return false;
	if ( UnitTarget->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH ) )
		return false;

	//Check if we apply target filtering
	if ( pFilter != TargetFilter_None )
	{
		//Skip units not on threat list
		if ( ( pFilter & TargetFilter_Aggroed ) && _unit->GetAIInterface()->getThreatByPtr( UnitTarget ) == 0 )
			return false;

		//Skip current attacking target if requested
		if ( ( pFilter & TargetFilter_NotCurrent ) && UnitTarget == _unit->GetAIInterface()->GetNextTarget() )
			return false;

		//Keep only wounded targets if requested
		if ( ( pFilter & TargetFilter_Wounded ) && UnitTarget->GetHealthPct() >= 99 )
			return false;

		//Skip targets not in melee range if requested
		if ( ( pFilter & TargetFilter_InMeleeRange ) && GetRangeToUnit( UnitTarget ) > _unit->GetAIInterface()->_CalcCombatRange( UnitTarget, false ) )
			return false;

		//Skip targets not in strict range if requested
		if ( ( pFilter & TargetFilter_InRangeOnly ) && ( pMinRange > 0 || pMaxRange > 0 ) )
		{
			float Range = GetRangeToUnit( UnitTarget );
			if ( pMinRange > 0 && Range < pMinRange )
				return false;
			if ( pMaxRange > 0 && Range > pMaxRange )
				return false;
		};

		//Skip targets not in Line Of Sight if requested
		if ( ( ~pFilter & TargetFilter_IgnoreLineOfSight ) && !_unit->IsInLineOfSight( UnitTarget ) )
			return false;

		//Handle hostile/friendly
		if ( ( ~pFilter & TargetFilter_Corpse ) && ( pFilter & TargetFilter_Friendly ) ) 
		{
			if ( !UnitTarget->CombatStatus.IsInCombat() )
				return false; //Skip not-in-combat targets if friendly
			if ( isHostile( _unit, UnitTarget ) || _unit->GetAIInterface()->getThreatByPtr( UnitTarget ) > 0 )
				return false;
		};
	};

	return true; //This is a valid unit target
};

void AscentScriptCreatureAI::PushRunToTargetCache( Unit* pTarget, SpellDesc* pSpell, RangeStatusPair pRangeStatus )
{
	if ( mRunToTargetCache != pTarget )
	{
		mRunToTargetCache = pTarget;
		mRunToTargetSpellCache = pSpell;
		SetCanMove( true );
		SetAllowMelee( false );
		SetAllowRanged( false );
		SetAllowSpell( false );
	};

	if ( mRunToTargetCache )
		MoveTo( mRunToTargetCache, pRangeStatus );
};

void AscentScriptCreatureAI::PopRunToTargetCache()
{
	if ( mRunToTargetCache )
	{
		mRunToTargetCache = NULL;
		mRunToTargetSpellCache = NULL;
		SetAllowMelee( true );
		SetAllowRanged( true );
		SetAllowSpell( true );
		StopMovement();
	};
};

void AscentScriptCreatureAI::RandomEmote( EmoteArray& pEmoteArray )
{
	int32 ArraySize = ( int32 )pEmoteArray.size();
	if ( ArraySize > 0 )
	{
		uint32 Roll = ( ArraySize > 1 ) ? RandomUInt( ArraySize - 1 ) : 0;
		Emote( pEmoteArray[ Roll ]->mText.c_str(), pEmoteArray[ Roll ]->mType, pEmoteArray[ Roll ]->mSoundId );
	};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class AscentScriptBossAI
AscentScriptBossAI::AscentScriptBossAI(Creature *pCreature) : AscentScriptCreatureAI(pCreature)
{
	mPhaseIndex = -1;
	mEnrageSpell = NULL;
	mEnrageTimerDuration = -1;
	mEnrageTimer = INVALIDATE_TIMER;
}

AscentScriptBossAI::~AscentScriptBossAI()
{
	mPhaseSpells.clear();
}

SpellDesc* AscentScriptBossAI::AddPhaseSpell(int32 pPhase, SpellDesc* pSpell)
{
	mPhaseSpells.push_back(std::make_pair(pPhase, pSpell));
	return pSpell;
}

int32 AscentScriptBossAI::GetPhase()
{
	return mPhaseIndex;
}

void AscentScriptBossAI::SetPhase(int32 pPhase, SpellDesc* pPhaseChangeSpell)
{
	if( mPhaseIndex != pPhase )
	{
		//Cancel all spells
		CancelAllSpells();

		//Enable spells related to that phase
		for( PhaseSpellArray::iterator SpellIter = mPhaseSpells.begin(); SpellIter != mPhaseSpells.end(); ++SpellIter )
		{
			if( SpellIter->first == pPhase ) SpellIter->second->mEnabled = true;
			else SpellIter->second->mEnabled = false;
		}

		//Remember phase index
		mPhaseIndex = pPhase;

		//Cast phase change spell now if available
		if( pPhaseChangeSpell ) CastSpellNowNoScheduling(pPhaseChangeSpell);
	}
}

void AscentScriptBossAI::SetEnrageInfo(SpellDesc* pSpell, int32 pTriggerMilliseconds)
{
	mEnrageSpell = pSpell;
	mEnrageTimerDuration = pTriggerMilliseconds;
}

void AscentScriptBossAI::OnCombatStart(Unit* pTarget)
{
	SetPhase(1);
	if( mEnrageSpell && mEnrageTimerDuration > 0 )
	{
		mEnrageTimer = AddTimer(mEnrageTimerDuration);
	}
	TriggerCooldownOnAllSpells();
	AscentScriptCreatureAI::OnCombatStart(pTarget);
}

void AscentScriptBossAI::OnCombatStop(Unit* pTarget)
{
	SetPhase(1);
	RemoveTimer(mEnrageTimer);
	AscentScriptCreatureAI::OnCombatStop(pTarget);
}

void AscentScriptBossAI::AIUpdate()
{
	if( mEnrageSpell && mEnrageTimerDuration > 0 && IsTimerFinished(mEnrageTimer) )
	{
		CastSpell(mEnrageSpell);
		RemoveTimer(mEnrageTimer);
	}
	AscentScriptCreatureAI::AIUpdate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Premade Spell Functions
#define SPELLFUNC_VANISH 24699

void SpellFunc_ClearHateList(SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	pCreatureAI->ClearHateList();
}

void SpellFunc_Disappear(SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	pCreatureAI->ClearHateList();
	pCreatureAI->SetCanMove(false);
	pCreatureAI->SetCanEnterCombat(false);
	pCreatureAI->ApplyAura(SPELLFUNC_VANISH);
}

void SpellFunc_Reappear(SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType)
{
	pCreatureAI->SetCanMove(true);
	pCreatureAI->SetCanEnterCombat(true);
	pCreatureAI->RemoveAura(SPELLFUNC_VANISH);
}
