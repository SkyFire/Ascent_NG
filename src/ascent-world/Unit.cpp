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

Unit::Unit()
{
	m_attackTimer = 0;
	m_attackTimer_1 = 0;
	m_duelWield = false;

	m_fearmodifiers = 0;
	m_state = 0;
	m_special_state = 0;
	m_deathState = ALIVE;
	m_currentSpell = NULL;
	m_meleespell = 0;
	m_addDmgOnce = 0;
	m_TotemSlots[0] = NULL;
	m_TotemSlots[1] = NULL;
	m_TotemSlots[2] = NULL;
	m_TotemSlots[3] = NULL;
	m_ObjectSlots[0] = 0;
	m_ObjectSlots[1] = 0;
	m_ObjectSlots[2] = 0;
	m_ObjectSlots[3] = 0;
	m_silenced = 0;
	disarmed   = false;

	// Pet
	m_isPet = false;
	
	//DK:modifiers
	PctRegenModifier = 0;
	for( uint32 x = 0; x < 4; x++ )
	{
		PctPowerRegenModifier[x] = 1;
	}
	m_speedModifier = 0;
	m_slowdown = 0;
	m_mountedspeedModifier=0;
	m_maxSpeed = 0;
	for(uint32 x=0;x<27;x++)
	{
		MechanicsDispels[x]=0;
		MechanicsResistancesPCT[x]=0;
		ModDamageTakenByMechPCT[x]=0;
	}

	//SM
	SM_CriticalChance=0;
	SM_FDur=0;//flat
	SM_PDur=0;//pct
	SM_FRadius=0;
	SM_FRange=0;
	SM_PCastTime=0;
	SM_FCastTime=0;
	SM_PCriticalDamage=0;
	SM_FDOT=0;
	SM_PDOT=0;
	SM_FEffectBonus=0;
	SM_PEffectBonus=0;
	SM_FDamageBonus=0;
	SM_PDamageBonus=0;
	SM_PSPELL_VALUE=0;
	SM_FSPELL_VALUE=0;
	SM_FHitchance=0;
	SM_PRange=0;//pct
	SM_PRadius=0;
	SM_PAPBonus=0;
	SM_PCost=0;
	SM_FCost=0;
	SM_FAdditionalTargets=0;
	SM_PJumpReduce=0;
	SM_FSpeedMod=0;
	SM_PNonInterrupt=0;
	SM_FPenalty=0;
	SM_PPenalty=0;
	SM_FCooldownTime = 0;
	SM_PCooldownTime = 0;
	SM_FChanceOfSuccess = 0;
	SM_FRezist_dispell = 0;
	SM_PRezist_dispell = 0;

	m_pacified = 0;
	m_interruptRegen = 0;
	m_resistChance = 0;
	m_powerRegenPCT = 0;
	RAPvModifier=0;
	APvModifier=0;
	stalkedby=0;

	m_extraattacks = 0;
	m_stunned = 0;
	m_manashieldamt=0;
	m_rooted = 0;
	m_triggerSpell = 0;
	m_triggerDamage = 0;
	m_canMove = 0;
	m_noInterrupt = 0;
	m_modlanguage = -1;
	
	critterPet = NULL;
	summonPet = NULL;

	m_useAI = false;
	for(uint32 x=0;x<10;x++)
	{
		dispels[x]=0;
		CreatureAttackPowerMod[x] = 0;
		CreatureRangedAttackPowerMod[x] = 0;
	}
	//REMIND:Update these if you make any changes
	CreatureAttackPowerMod[UNIT_TYPE_MISC] = 0;
	CreatureRangedAttackPowerMod[UNIT_TYPE_MISC] = 0;
	CreatureAttackPowerMod[11] = 0;
	CreatureRangedAttackPowerMod[11] = 0;

	m_invisible = false;
	m_invisFlag = INVIS_FLAG_NORMAL;

	for(int i = 0; i < INVIS_FLAG_TOTAL; i++)
	{
		m_invisDetect[i] = 0;
	}

	m_stealthLevel = 0;
	m_stealthDetectBonus = 0;
	m_stealth = 0;
	m_can_stealth = true;

	for(uint32 x=0;x<5;x++)
		BaseStats[x]=0;

	m_H_regenTimer = 2000;
	m_P_regenTimer = 2000;

	//	if(GetTypeId() == TYPEID_PLAYER) //only player for now
	//		CalculateActualArmor();

	m_aiInterface = new AIInterface();
	m_aiInterface->Init(this, AITYPE_AGRO, MOVEMENTTYPE_NONE);

	m_emoteState = 0;
	m_oldEmote = 0;	
	
	BaseDamage[0]=0;
	BaseOffhandDamage[0]=0;
	BaseRangedDamage[0]=0;
	BaseDamage[1]=0;
	BaseOffhandDamage[1]=0;
	BaseRangedDamage[1]=0;

	m_CombatUpdateTimer = 0;
	for(uint32 x=0;x<7;x++)
	{
		SchoolImmunityList[x] = 0;
		BaseResistance[x] = 0;
		HealDoneMod[x] = 0;
		HealDonePctMod[x] = 1.0f;
		HealTakenMod[x] = 0;
		HealTakenPctMod[x] = 1.0f;
		DamageTakenMod[x] = 0;
		DamageDoneModPCT[x]= 0;
		SchoolCastPrevent[x]=0;
		DamageTakenPctMod[x] = 1;
		SpellCritChanceSchool[x] = 0;
		PowerCostMod[x] = 0;
		PowerCostPctMod[x] = 0; // armor penetration & spell penetration
		AttackerCritChanceMod[x]=0;
		CritMeleeDamageTakenPctMod[x]=0;
		CritRangedDamageTakenPctMod[x]=0;
	}
	DamageTakenPctModOnHP35 = 1;
	RangedDamageTaken = 0;

	for(int i = 0; i < 5; i++)
	{
		m_detectRangeGUID[i] = 0;
		m_detectRangeMOD[i] = 0;
	}

	trackStealth = false;

	m_threatModifyer = 0;
	m_generatedThreatModifyer = 0;
	memset(m_auras, 0, (MAX_AURAS+MAX_PASSIVE_AURAS)*sizeof(Aura*));
	
	// diminishing return stuff
	memset(m_diminishAuraCount, 0, 23);
	memset(m_diminishCount, 0, 23*2);
	memset(m_diminishTimer, 0, 23*2);
	memset(m_auraStackCount, 0, MAX_AURAS);
	m_diminishActive = false;
	dynObj = 0;
	pLastSpell = 0;
	m_flyspeedModifier = 0;
	bInvincible = false;
	m_redirectSpellPackets = 0;
	can_parry = false;
	bProcInUse = false;
	spellcritperc = 0;

	polySpell = 0;
	RangedDamageTaken = 0;
	m_procCounter = 0;
	m_extrastriketargets = 0;
	m_damgeShieldsInUse = false;
//	fearSpell = 0;
	m_extraAttackCounter = false;
	CombatStatus.SetUnit(this);
	m_temp_summon=false;
	m_chargeSpellsInUse=false;
	m_interruptedRegenTime = 0;
	m_hasVampiricEmbrace = m_hasVampiricTouch = 0;
	mAngerManagement = false;
	mRecentlyBandaged = false;
}

Unit::~Unit()
{  
	RemoveAllAuras();

	if(SM_CriticalChance != 0) delete [] SM_CriticalChance ;
	if(SM_FDur != 0) delete [] SM_FDur ;//flat
	if(SM_PDur != 0) delete [] SM_PDur ;//pct
	if(SM_FRadius != 0) delete [] SM_FRadius ;
	if(SM_FRange != 0) delete [] SM_FRange ;
	if(SM_PCastTime != 0) delete [] SM_PCastTime ;
	if(SM_FCastTime != 0) delete [] SM_FCastTime ;
	if(SM_PCriticalDamage != 0) delete [] SM_PCriticalDamage ;
	if(SM_FDOT != 0) delete [] SM_FDOT ;
	if(SM_PDOT != 0) delete [] SM_PDOT ;
	if(SM_PEffectBonus != 0) delete [] SM_PEffectBonus ;
    if(SM_FEffectBonus != 0) delete [] SM_FEffectBonus ;
	if(SM_FDamageBonus != 0) delete [] SM_FDamageBonus ;
	if(SM_PDamageBonus != 0) delete [] SM_PDamageBonus ;
	if(SM_PSPELL_VALUE != 0) delete [] SM_PSPELL_VALUE ;
	if(SM_FSPELL_VALUE != 0) delete [] SM_FSPELL_VALUE ;
	if(SM_FHitchance != 0) delete [] SM_FHitchance ;
	if(SM_PRange != 0) delete [] SM_PRange ;//pct
	if(SM_PRadius != 0) delete [] SM_PRadius ;
	if(SM_PAPBonus != 0) delete [] SM_PAPBonus ;
	if(SM_PCost != 0) delete [] SM_PCost ;
	if(SM_FCost != 0) delete [] SM_FCost ;
	if(SM_FAdditionalTargets != 0) delete [] SM_FAdditionalTargets ;
	if(SM_PJumpReduce != 0) delete [] SM_PJumpReduce ;
	if(SM_FSpeedMod != 0) delete [] SM_FSpeedMod ;
	if(SM_PNonInterrupt != 0) delete [] SM_PNonInterrupt ;
	if(SM_FPenalty != 0) delete [] SM_FPenalty ;
	if(SM_PPenalty != 0) delete [] SM_PPenalty ;
	if(SM_FCooldownTime != 0) delete [] SM_FCooldownTime ;
	if(SM_PCooldownTime != 0) delete [] SM_PCooldownTime ;
	if(SM_FChanceOfSuccess != 0) delete [] SM_FChanceOfSuccess ;
	if(SM_FRezist_dispell != 0) delete [] SM_FRezist_dispell ;
	if(SM_PRezist_dispell != 0) delete [] SM_PRezist_dispell ;

	delete m_aiInterface;

	/*for(int i = 0; i < 4; i++)
		if(m_ObjectSlots[i])
			delete m_ObjectSlots[i];*/

	if(m_currentSpell)
		m_currentSpell->cancel();

	// clear tmpAura pointers
	for(map<uint32, Aura*>::iterator itr = tmpAura.begin(); itr != tmpAura.end(); ++itr)
		delete itr->second;
}

void Unit::Update( uint32 p_time )
{
	_UpdateSpells( p_time );

	if(!isDead())
	{
		CombatStatus.UpdateTargets();

		//-----------------------POWER & HP REGENERATION-----------------
/* Please dont do temp fixes. Better report to me. Thx. Shady */
        if( p_time >= m_H_regenTimer )
		    RegenerateHealth();
	    else
		    m_H_regenTimer -= p_time;

		if( p_time >= m_P_regenTimer )
		{
			RegeneratePower( false );
			m_interruptedRegenTime=0;
		}
		else
		{
			m_P_regenTimer -= p_time;
			if (m_interruptedRegenTime)
			{
				if(p_time>=m_interruptedRegenTime)
					RegeneratePower( true );
				else
					m_interruptedRegenTime -= p_time;
			}
		}


		if(m_aiInterface != NULL && m_useAI)
			m_aiInterface->Update(p_time);

		if(m_diminishActive)
		{
			uint32 count = 0;
			for(uint32 x = 0; x < 16; ++x)
			{
				// diminishing return stuff
				if(m_diminishTimer[x] && !m_diminishAuraCount[x])
				{
					if(p_time >= m_diminishTimer[x])
					{
						// resetting after 15 sec
						m_diminishTimer[x] = 0;
						m_diminishCount[x] = 0;
					}
					else
					{
						// reducing, still.
						m_diminishTimer[x] -= p_time;
						++count;
					}
				}
			}
			if(!count)
				m_diminishActive = false;
		}

/*		//if health changed since last time. Would be perfect if it would work for creatures too :)
		if(m_updateMask.GetBit(UNIT_FIELD_HEALTH))
			EventHealthChangeSinceLastUpdate();*/
	}
}

bool Unit::canReachWithAttack(Unit *pVictim)
{
//	float targetreach = pVictim->GetFloatValue(UNIT_FIELD_COMBATREACH);
	float selfreach = m_floatValues[UNIT_FIELD_COMBATREACH];
	float targetradius = pVictim->m_floatValues[UNIT_FIELD_BOUNDINGRADIUS];
	float selfradius = m_floatValues[UNIT_FIELD_BOUNDINGRADIUS];
	float targetscale = pVictim->m_floatValues[OBJECT_FIELD_SCALE_X];
	float selfscale = m_floatValues[OBJECT_FIELD_SCALE_X];

	if( GetMapId() != pVictim->GetMapId() )
		return false;

	float distance = sqrt(GetDistanceSq(pVictim));
	float attackreach = (((targetradius*targetscale) + selfreach) + (((selfradius*selfradius)*selfscale)+1.50f));

	//formula adjustment for player side.
	if( IsPlayer() )
	{
		if( attackreach <= 8 && attackreach >= 5 && targetradius >= 1.80f)
			attackreach = 11; //giant type units

		if( attackreach > 11)
			attackreach = 11; //distance limited to max 11 yards attack range //max attack distance

		if( attackreach < 5 )
			attackreach = 5; //normal units with too small reach.

		//range can not be less than 5 yards - this is normal combat range, SCALE IS NOT SIZE
		if( pVictim->IsPlayer() && TO_PLAYER(pVictim)->m_isMoving )
		{
			// this only applies to PvP.
			uint32 lat = static_cast< Player* >( this )->GetSession() ? static_cast< Player* >( this )->GetSession()->GetLatency() : 0;

			// if we're over 500 get fucked anyway.. your gonna lag! and this stops cheaters too
			lat = ( lat > 500) ? 500 : lat;

			// calculate the added distance
			attackreach += ( m_runSpeed * 0.001f ) * (float( lat ) * 1.5f);
		}
	}
	return ( distance <= attackreach );
}

void Unit::GiveGroupXP(Unit *pVictim, Player *PlayerInGroup)
{
	if(!PlayerInGroup) 
		return;
	if(!pVictim) 
		return;
	if(!PlayerInGroup->InGroup()) 
		return;
	Group *pGroup = PlayerInGroup->GetGroup();
	uint32 xp;
	if(!pGroup) 
		return;

	//Get Highest Level Player, Calc Xp and give it to each group member
	Player *pHighLvlPlayer = NULL;
	Player *pGroupGuy = NULL;
	  int active_player_count=0;
	Player *active_player_list[MAX_GROUP_SIZE_RAID];//since group is small we can afford to do this ratehr then recheck again the whole active player set
	int total_level=0;
	float xp_mod = 1.0f;

/*	if(pGroup->GetGroupType() == GROUP_TYPE_RAID)
	{   //needs to change
		//Calc XP
		xp = CalculateXpToGive(pVictim, PlayerInGroup);
		xp /= pGroup->MemberCount();

		GroupMembersSet::iterator itr;
		for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++)
		{
			for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
			{
				if((*itr)->getLevel() < sWorld.LevelCap)
					(*itr)->GiveXP(xp, pVictim->GetGUID(), true);
			}
		}
	}
	else if(pGroup->GetGroupType() == GROUP_TYPE_PARTY) */
	//change on 2007 04 22 by Zack
	//we only take into count players that are near us, on same map
	GroupMembersSet::iterator itr;
	pGroup->Lock();
	for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++) {
		for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
		{
			pGroupGuy = (*itr)->m_loggedInPlayer;
			if( pGroupGuy && 
				pGroupGuy->isAlive() && 
//				PlayerInGroup->GetInstanceID()==pGroupGuy->GetInstanceID() &&
				pVictim->GetMapMgr() == pGroupGuy->GetMapMgr() && 
				pGroupGuy->GetDistanceSq(pVictim)<100*100
				)
			{
				active_player_list[active_player_count]=pGroupGuy;
				active_player_count++;
				total_level += pGroupGuy->getLevel();
				if(pHighLvlPlayer)
				{
					if(pGroupGuy->getLevel() > pHighLvlPlayer->getLevel())
						pHighLvlPlayer = pGroupGuy;
				}
				else 
					pHighLvlPlayer = pGroupGuy;
			}
		}
	}
	pGroup->Unlock();
	if(active_player_count<1) //killer is always close to the victim. This should never execute
	{
		if(PlayerInGroup == 0)
		{
			PlayerInfo * pleaderinfo = pGroup->GetLeader();
			if(!pleaderinfo->m_loggedInPlayer)
				return;

			PlayerInGroup = pleaderinfo->m_loggedInPlayer;
		}

		xp = CalculateXpToGive(pVictim, PlayerInGroup);
		PlayerInGroup->GiveXP(xp, pVictim->GetGUID(), true);
	}
	else
	{
		if( pGroup->GetGroupType() == GROUP_TYPE_PARTY)
		{
			if(active_player_count==3)
				xp_mod=1.1666f;
			else if(active_player_count==4)
				xp_mod=1.3f;
			else if(active_player_count==5)
				xp_mod=1.4f;
			else xp_mod=1;//in case we have only 2 members ;)
		}
		else if(pGroup->GetGroupType() == GROUP_TYPE_RAID)
			xp_mod=0.5f;

		if(pHighLvlPlayer == 0)
		{
			PlayerInfo * pleaderinfo = pGroup->GetLeader();
			if(!pleaderinfo->m_loggedInPlayer)
				return;

			pHighLvlPlayer = pleaderinfo->m_loggedInPlayer;
		}

		xp = CalculateXpToGive(pVictim, pHighLvlPlayer);
		//i'm not sure about this formula is correct or not. Maybe some brackets are wrong placed ?
		for(int i=0;i<active_player_count;i++)
			active_player_list[i]->GiveXP( float2int32(((xp*active_player_list[i]->getLevel()) / total_level)*xp_mod), pVictim->GetGUID(), true );
	}
		/* old code start before 2007 04 22
		GroupMembersSet::iterator itr;
		for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++)
		{
			for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
			{
				pGroupGuy = (*itr);
				if(pGroupGuy)
				{
					if(pHighLvlPlayer)
					{
						if(pGroupGuy->getLevel() > pHighLvlPlayer->getLevel())
						{
							pHighLvlPlayer = pGroupGuy;
						}
					}
					else
						pHighLvlPlayer = pGroupGuy;
				}
			}
		}

		//Calc XP
		xp = CalculateXpToGive(pVictim, pHighLvlPlayer);
		uint32 giveXP = 0;

		for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++)
		{
			for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
			{
				pGroupGuy = (*itr);
				giveXP = xp * pGroupGuy->getLevel() / (pHighLvlPlayer->getLevel() + pGroupGuy->getLevel());
				if(pGroupGuy->getLevel() < sWorld.LevelCap)
					pGroupGuy->GiveXP(giveXP, pVictim->GetGUID(), true);
			}
		}
	}*/
}

void Unit::HandleProc( uint32 flag, Unit* victim, SpellEntry* CastingSpell, uint32 dmg, uint32 abs )
{
	++m_procCounter;
	bool can_delete = !bProcInUse; //if this is a nested proc then we should have this set to TRUE by the father proc
	bProcInUse = true; //locking the proc list

	std::list< struct ProcTriggerSpell >::iterator itr,itr2;
	for( itr = m_procSpells.begin(); itr != m_procSpells.end(); )  // Proc Trigger Spells for Victim
	{
		itr2 = itr;
		++itr;
		if( itr2->deleted )
		{
			if( can_delete )
			{
				/*if( m_objectTypeId == TYPEID_PLAYER )
					printf("trying to delete procspell %u (%u) player %u\n", itr2->spellId, itr2->origId, GetLowGUID());
				else
					printf("trying to delete procspell %u (%u) unit %u\n", itr2->spellId, itr2->origId, GetEntry());*/

				m_procSpells.erase( itr2 );
			}

			continue;
		}

		uint32 origId = itr2->origId;
		if( CastingSpell != NULL )
		{
			//this is to avoid spell proc on spellcast loop. We use dummy that is same for both spells
			//if( CastingSpell->Id == itr2->spellId )
			if( CastingSpell->Id == itr2->origId || CastingSpell->Id == itr2->spellId )
			{
				//printf("WOULD CRASH HERE ON PROC: CastingId: %u, OrigId: %u, SpellId: %u\n", CastingSpell->Id, itr2->origId, itr2->spellId);
				continue;
			}
		}
		SpellEntry* ospinfo = dbcSpell.LookupEntry( origId );//no need to check if exists or not since we were not able to register this trigger if it would not exist :P
		
		//this requires some specific spell check,not yet implemented
		if( itr2->procFlags & flag )
		{
			uint32 spellId = itr2->spellId;
			if( itr2->procFlags & PROC_ON_CAST_SPECIFIC_SPELL )
			{

				if( CastingSpell == NULL )
					continue;
				
				//this is wrong, dummy is too common to be based on this, we should use spellgroup or something
				SpellEntry* sp = dbcSpell.LookupEntry( spellId );
				if( sp->dummy != CastingSpell->dummy )
				{
					if( !ospinfo->School )
						continue;
					if( ospinfo->School != CastingSpell->School )
						continue;
					if( CastingSpell->EffectImplicitTargetA[0] == 1 || 
						CastingSpell->EffectImplicitTargetA[1] == 1 || 
						CastingSpell->EffectImplicitTargetA[2] == 1) //Prevents school based procs affecting caster when self buffing
						continue;
				}
				else
					if( sp->dummy == 1 )
						continue;
			}

			uint32 proc_Chance = itr2->procChance;
			SpellEntry* spe  = dbcSpell.LookupEntry( spellId );

			// feral = no procs (need a better way to do this)
			/*if( this->IsPlayer() && static_cast<Player*>(this)->GetShapeShift() )
			{
				if( spe->NameHash == SPELL_HASH_LIGHTNING_SPEED ) // mongoose
					continue;
				else if( spe->NameHash == SPELL_HASH_HOLY_STRENGTH ) //crusader
					continue;
			}*/
			//Custom procchance modifications based on equipped weapon speed.
			if( this->IsPlayer() && spe != NULL && spe->procs_per_minute > 0.0f )
			{
				Item * mh = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
				Item * of = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
				
				if( mh != NULL && of != NULL )
				{
					float mhs = float( mh->GetProto()->Delay );
					float ohs = float( of->GetProto()->Delay );
					proc_Chance = float2int32( ( mhs + ohs ) * 0.001f * spe->procs_per_minute / 0.6f );
				}
				else if( mh != NULL )
				{
					float mhs = float( mh->GetProto()->Delay );
					proc_Chance = float2int32( mhs * 0.001f * spe->procs_per_minute / 0.6f );
				}
				else
					proc_Chance = 0;

				if( static_cast< Player* >( this )->IsInFeralForm() )
				{
					if( static_cast< Player* >( this )->GetShapeShift() == FORM_CAT )
					{
						proc_Chance = float2int32( spe->procs_per_minute / 0.6f );
					}
					else if( static_cast< Player* >( this )->GetShapeShift() == FORM_BEAR || static_cast< Player* >( this )->GetShapeShift() == FORM_DIREBEAR )
					{
						proc_Chance = float2int32( spe->procs_per_minute / 0.24f );
					}
				}
			}

			SM_FIValue( SM_FChanceOfSuccess, (int32*)&proc_Chance, ospinfo->SpellGroupType );
			if( spellId && Rand( proc_Chance ) )
			{
				SpellCastTargets targets;
				if( itr2->procFlags & PROC_TARGET_SELF )
					targets.m_unitTarget = GetGUID();
				else 
					targets.m_unitTarget = victim->GetGUID();
				/* hmm whats a reasonable value here */
				if( m_procCounter > 40 )
				{
					/* something has proceed over 10 times in a loop :/ dump the spellids to the crashlog, as the crashdump will most likely be useless. */
					// BURLEX FIX ME!
					//OutputCrashLogLine("HandleProc %u SpellId %u (%s) %u", flag, spellId, sSpellStore.LookupString(sSpellStore.LookupEntry(spellId)->Name), m_procCounter);
					return;
				}

				//check if we can trigger due to time limitation
				if( ospinfo->proc_interval )
				{
					uint32 now_in_ms=getMSTime();
					if( itr2->LastTrigger + ospinfo->proc_interval > now_in_ms )
						continue; //we can't trigger it yet.
					itr2->LastTrigger = now_in_ms; // consider it triggered
				}
				//since we did not allow to remove auras like these with interrupt flag we have to remove them manually.
				if( itr2->procFlags & PROC_REMOVEONUSE )
					RemoveAura( origId );
				int dmg_overwrite = 0;

				//these are player talents. Fuckem they pull the emu speed down 
				if( IsPlayer() )
				{
					uint32 talentlevel = 0;
					switch( origId )
					{
						//mace specialization
						case 12284:	{talentlevel = 1;}break;
						case 12701:	{talentlevel = 2;}break;
						case 12702:	{talentlevel = 3;}break;
						case 12703:	{talentlevel = 4;}break;
						case 12704:	{talentlevel = 5;}break;

						//Unbridled Wrath
						case 12999:	{talentlevel = 1;}break;
						case 13000:	{talentlevel = 2;}break;
						case 13001:	{talentlevel = 3;}break;
						case 13002:	{talentlevel = 4;}break;
					}

					switch( spellId )
					{
						case 14189: //Seal Fate
						case 16953: //Blood Frenzy
						{
							if( !this->IsPlayer() || 
								!CastingSpell || 
								CastingSpell->Id == 14189 ||
								CastingSpell->Id == 16953 )
								continue;
							if( CastingSpell->Effect[0] != 80 &&
								CastingSpell->Effect[1] != 80 &&
								CastingSpell->Effect[2] != 80 &&
								CastingSpell->NameHash != SPELL_HASH_MANGLE__CAT_ )
								continue;
						}break;
						case 17106: //druid intencity
						{
							if( CastingSpell->Id != 5229 )//enrage
								continue;
						}break;
						case 31616:
						{
							//yep, another special case: Nature's grace
							if( GetHealthPct() > 30 )
								continue;
						}break;
						case 37309:
						{
							if( !this->IsPlayer() )
								continue;
							if( static_cast< Player* >( this )->GetShapeShift() != FORM_BEAR ||
								static_cast< Player* >( this )->GetShapeShift() != FORM_DIREBEAR )
								continue;
						}break;
						case 37310:
						{
							if( !this->IsPlayer() || static_cast< Player* >( this )->GetShapeShift() != FORM_CAT )
								continue;
						}break;
                        case 34754: //holy concentration
                        {

							if( CastingSpell->NameHash != SPELL_HASH_FLASH_HEAL &&
								CastingSpell->NameHash != SPELL_HASH_BINDING_HEAL &&
								CastingSpell->NameHash != SPELL_HASH_GREATER_HEAL )
								continue;
						}break;
						case 5530:
						{
							//warrior/rogue mace specialization can trigger only when using maces
							Item* it;
							if( static_cast< Player* >( this )->GetItemInterface() )
							{
								it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( it != NULL && it->GetProto() )
								{
									uint32 reqskill = GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
									if( reqskill != SKILL_MACES && reqskill != SKILL_2H_MACES )
										continue;
								}
								else
									continue; //no weapon no joy
							}
							else
								continue; //no weapon no joy
							//let's recalc chance to cast since we have a full 100 all time on this one
							//how lame to get talentpointlevel for this spell :(
							//float chance=it->GetProto()->Delay*100*talentlevel/60000;
						}break;
						case 4350:
						{
							//sword specialization
							if( static_cast< Player* >( this )->GetItemInterface())
							{
								Item* it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( it != NULL && it->GetProto() )
								{
									uint32 reqskill=GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
									if( reqskill != SKILL_SWORDS && reqskill != SKILL_2H_SWORDS )
										continue;
								}
								else
									continue; //no weapon no joy
							}
							else
								continue; //no weapon no joy
						}break;
						case 12721:
						{
							//deep wound requires a melee weapon
							Item* it;
							if( static_cast< Player* >( this )->GetItemInterface())
							{
								it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( it != NULL && it->GetProto() )
								{
									//class 2 means weapons ;)
									if( it->GetProto()->Class != 2 )
										continue;
								}
								else continue; //no weapon no joy
							}
							else continue; //no weapon no joy
						}break;
						//warrior - Blood Frenzy
						case 30069:
						case 30070:
						{
							if( CastingSpell == NULL )
								continue;
								if( CastingSpell->NameHash != SPELL_HASH_REND && 
									CastingSpell->NameHash != SPELL_HASH_DEEP_WOUNDS )
									continue;
						}break;
						//warrior - Unbridled Wrath
						case 12964:
						{
							//let's recalc chance to cast since we have a full 100 all time on this one
							Item* it;
							if( static_cast< Player* >( this )->GetItemInterface() )
							{
								it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( !( it != NULL && it->GetProto() ) )
									continue; //no weapon no joy
							}
							else
								continue; //no weapon no joy
							//float chance=float(it->GetProto()->Delay)*float(talentlevel)/600.0f;
							float chance = float( it->GetProto()->Delay ) * float(talentlevel ) / 300.0f; //zack this had a very low proc rate. Kinda liek a waisted talent
							uint32 myroll = RandomUInt( 100 );
							if( myroll > chance )
								continue;
						}break;
/*						//disabled by zack until finished : this needs to get trigered on trap trigger and not trap cast
						// hunter - Entrapment
						case 19185:
						{
							if( CastingSpell == NULL )
								continue;
								//only trigger effect for specified spells
								if( CastingSpell->NameHash!=SPELL_HASH_BACKSTAB && //immolation trap
									CastingSpell->NameHash!=SPELL_HASH_SINISTER_STRIKE && //frost trap
									CastingSpell->NameHash!=SPELL_HASH_SHIV && //shiv
									CastingSpell->NameHash!=SPELL_HASH_GOUGE ) //gouge
									continue;
						}break;*/
						// Mage ignite talent only for fire dmg
						case 12654:
						{
							if( CastingSpell == NULL )
								continue;
							if( CastingSpell->School != SCHOOL_FIRE )
								continue;
						}break;
						//rogue - blade twisting
						case 31125:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_BACKSTAB && //backstab
									CastingSpell->NameHash != SPELL_HASH_SINISTER_STRIKE && //sinister strike
									CastingSpell->NameHash != SPELL_HASH_SHIV && //shiv
									CastingSpell->NameHash != SPELL_HASH_GOUGE ) //gouge
									continue;
							}break;
						//warlock - Improved Shadow Bolt
						case 17794:
						case 17798:
						case 17797:
						case 17799:
						case 17800:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash!=SPELL_HASH_SHADOW_BOLT)//shadow bolt								
									continue;
							}break;
						// warlock - Improved Drain Soul
						case 18371:
							{
								//null check was made before like 2 times already :P
								dmg_overwrite = ( ospinfo->EffectBasePoints[2] + 1 ) * GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) / 100;
							}break;
						// warlock - Unstable Affliction
						case 43523:
							{
								//null check was made before like 2 times already :P
								dmg_overwrite = ( ospinfo->EffectBasePoints[0] + 1 ) * 9;
							}break;
						//warlock soul link
						case 25228:
							{
								//we need a pet for this, else we do not trigger it at all
								if( !IsPlayer() )
									continue;
								Unit* new_caster;
								if( static_cast< Player* >( this )->GetSummon() )
									new_caster = static_cast< Player* >( this )->GetSummon();
								else if( GetUInt64Value( UNIT_FIELD_CHARM ) )
									new_caster = GetMapMgr()->GetUnit( GetUInt64Value( UNIT_FIELD_CHARM ) );
								else
									new_caster = NULL;
								if( new_caster != NULL && new_caster->isAlive() )
								{
									SpellEntry* spellInfo = dbcSpell.LookupEntry( 25228 ); //we already modified this spell on server loading so it must exist
									Spell* spell = new Spell( new_caster, spellInfo, true, NULL );
									spell->forced_basepoints[0] = dmg;
									SpellCastTargets targets;
									targets.m_unitTarget = GetGUID();
									spell->prepare( &targets );
								}
								continue;
							}break;
						//warlock - Nighfall
						case 17941:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_CORRUPTION && //Corruption
									CastingSpell->NameHash != SPELL_HASH_DRAIN_LIFE )//Drain Life								
									continue;
							}break;
/*                        //warlock - Demonic Knowledge
                        case 39576:
                            {
                                if( CastingSpell == NULL )
                                    continue;
                                if( CastingSpell->Effect[0] != 56 )
                                    continue;
                                Pet* ps = static_cast< Player* >( this )->GetSummon();
                                if( ps == NULL)
                                    return;//no pet
                                int32 val;
                                SpellEntry *parentproc= dbcSpell.LookupEntry( origId );
                                val = parentproc->EffectBasePoints[0] + 1;
                                val = val * (ps->GetUInt32Value( UNIT_FIELD_STAT2 ) + ps->GetUInt32Value( UNIT_FIELD_STAT3 ) );
                                SpellEntry *spellInfo = dbcSpell.LookupEntry( 39576 );
                                Spell *spell = new Spell( this, spellInfo ,true, NULL );
                                spell->forced_basepoints[0] = ( val / 100 );
                                SpellCastTargets targets;
                                targets.m_unitTarget = GetGUID();
                                spell->prepare( &targets );
                                continue;
                            }break;*/
						//mage - Arcane Blast proc
						case 36032:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_ARCANE_BLAST ) //Arcane Blast
									continue;
							}break;
						//warlock - Shadow Embrace
						case 32386:
						case 32388:
						case 32389:
						case 32390:
						case 32391:
							{
								if( CastingSpell == NULL )
									continue;
								else
								{
									if( CastingSpell->NameHash != SPELL_HASH_FIRE_SHIELD_II && // Corruption
										CastingSpell->NameHash != SPELL_HASH_CURSE_OF_AGONY && //CoA
										CastingSpell->NameHash != SPELL_HASH_SIPHON_LIFE && //Siphon Life
										CastingSpell->NameHash != SPELL_HASH_SEED_OF_CORRUPTION ) //SoC
										continue;
								}
							}break;
						//warlock - Aftermath
						case 18118:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								skilllinespell* skillability = objmgr.GetSpellSkill(CastingSpell->Id);
								if( !skillability )
									continue;
								if( skillability->skilline != SKILL_DESTRUCTION )
									continue;
							}break;
						//warlock - Nether Protection
						case 30300:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
								if( CastingSpell->School!=SCHOOL_FIRE &&
									CastingSpell->School!=SCHOOL_SHADOW)
									continue;
							}break;
						//warlock - Soul Leech
						//this whole spell should get rewriten. Uses bad formulas, bad trigger method, spell is rewriten ...
						case 30294:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								uint32 amount;
								switch( CastingSpell->NameHash )
								{
									case SPELL_HASH_SHADOW_BOLT: //Shadow Bolt
									case SPELL_HASH_SOUL_FIRE: //Soul Fire
									case SPELL_HASH_INCINERATE: //Incinerate
									case SPELL_HASH_SEARING_PAIN: //Searing Pain
									case SPELL_HASH_CONFLAGRATE: //Conflagrate
									{
										amount = CastingSpell->EffectBasePoints[0]+1;
									}break;
									case SPELL_HASH_SHADOWBURN: //Shadowburn
									{
										amount = CastingSpell->EffectBasePoints[1]+1;
									}break;
									default:
										amount=0;
								}
								if(!amount)
									continue;
								SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId );
								if(!spellInfo)
									continue;
								Spell *spell = new Spell(this, spellInfo ,true, NULL);
								spell->SetUnitTarget(this);
								spell->Heal(amount*(ospinfo->EffectBasePoints[0]+1)/100);
								delete spell;
								continue;
							}break;
						//warlock - pyroclasm
						case 18093:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_RAIN_OF_FIRE && //Rain of Fire
									CastingSpell->NameHash != SPELL_HASH_HELLFIRE_EFFECT && //Hellfire
									CastingSpell->NameHash != SPELL_HASH_SOUL_FIRE ) //Soul Fire
									continue;
							}break;
						//mage - Improved Scorch
						case 22959:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_SCORCH ) //Scorch
									continue;
							}break;
						//mage - Combustion
						case 28682:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING)
									|| CastingSpell->School != SCHOOL_FIRE )
									continue;
								if( flag & PROC_ON_SPELL_CRIT_HIT )
								{
									itr2->procCharges++;
									if( itr2->procCharges >= 3 ) //whatch that number cause it depends on original stack count !
									{
										RemoveAllAuraByNameHash( SPELL_HASH_COMBUSTION );
										RemoveAllAuraByNameHash( SPELL_HASH_COMBUSTION_PROC );
									}
									continue;
								}
							}break;
						//priest - Misery
						case 33200:
						case 33199:
						case 33198:
						case 33197:
						case 33196:
							{
								if( CastingSpell == NULL )
									continue;
								else
								{
									if( CastingSpell->NameHash != SPELL_HASH_MIND_FLAY && // Mind Flay
										CastingSpell->NameHash != SPELL_HASH_SHADOW_WORD__PAIN && //SW:P
										CastingSpell->NameHash != SPELL_HASH_VAMPIRIC_TOUCH ) //SoC
										continue;
								}
							}break;
						//priest - Shadow Weaving
						case 15258:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( CastingSpell->School != SCHOOL_SHADOW || !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) ) //we need damaging spells for this, so we suppose all shadow spells casted on target are dmging spells = Wrong
									continue;
							}break;
						//priest - Inspiration
						case 15363:
						case 14893:
						case 15357:
							{
								if( !CastingSpell || !( CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING ) )
									continue;
							}
							break;
						//priest - Blessed Recovery
                        case 27813:
                        case 27817:
                        case 27818:
                            {
                                if(!IsPlayer() || !dmg)
                                    continue;
                                SpellEntry *parentproc= dbcSpell.LookupEntry(origId);
                                SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId);
								if (!parentproc || !spellInfo)
									continue;
								int32 val = parentproc->EffectBasePoints[0] + 1;
                                Spell *spell = new Spell(this, spellInfo ,true, NULL);
                                spell->forced_basepoints[0] = (val*dmg)/300; //per tick
                                SpellCastTargets targets;
                                targets.m_unitTarget = GetGUID();
                                spell->prepare(&targets);
                                continue;
                            }break;
						//shaman - Healing Way
						case 29203:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_HEALING_WAVE ) //healing wave
									continue;
							}break;
						//shaman - Elemental Devastation
						case 29177:
						case 29178:
						case 30165:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING)) //healing wave
									continue;
							}break;
						//shaman - windfurry weapon
						case 8232:
						case 8235:
						case 10486:
						case 16362:
						case 25505:
							{
								if(!IsPlayer())
									continue;
								//!! The wierd thing is that we need the spell thet trigegred this enchant spell in order to output logs ..we are using oldspell info too 
								//we have to recalc the value of this spell
								SpellEntry *spellInfo = dbcSpell.LookupEntry(origId);
								uint32 AP_owerride=GetAP() + spellInfo->EffectBasePoints[0]+1;
								uint32 dmg = static_cast< Player* >( this )->GetMainMeleeDamage(AP_owerride);
								SpellEntry *sp_for_the_logs = dbcSpell.LookupEntry(spellId);
								Strike( victim, MELEE, sp_for_the_logs, dmg, 0, 0, true, false );
								Strike( victim, MELEE, sp_for_the_logs, dmg, 0, 0, true, false );
								//nothing else to be done for this trigger
								continue;
							}break;
						//rogue - Ruthlessness
						case 14157:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//we need a finishing move for this 
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) || victim==this)
									continue;

								if(IsPlayer())
								{
									static_cast<Player*>(this)->m_spellcomboPoints++;
									static_cast<Player*>(this)->UpdateComboPoints();
								}
							}break;
						//rogue - Relentless Strikes
						case 14181:
							{
								if( CastingSpell == NULL || !IsPlayer() )
									continue;//this should not ocur unless we made a fuckup somewhere

								if( !(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) )
									continue;

								int32 procChance = 20 * TO_PLAYER(this)->m_comboPoints;
								if(!Rand(procChance))
									continue;

								dmg_overwrite = 25;

							}break;
						//rogue - Find Weakness
						case 31234:
						case 31235:
						case 31236:
						case 31237:
						case 31238:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE))
									continue;
							}break;
						//rogue - Initiative
						case 13977:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//we need a Ambush, Garrote, or Cheap Shot
								if( CastingSpell == NULL )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_CHEAP_SHOT && //Cheap Shot
									CastingSpell->NameHash != SPELL_HASH_AMBUSH && //Ambush
									CastingSpell->NameHash != SPELL_HASH_GARROTE )  //Garrote
									continue;
							}break;
						//Priest - Shadowguard
						case 28377:
						case 28378:
						case 28379:
						case 28380:
						case 28381:
						case 28382:
						case 28385:
							{
								if( CastingSpell && ( this == victim || !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) ) ) //no self casts allowed or beneficial spells
									continue;//we can proc on ranged weapons too
							}break;
						//Priest - blackout
						case 15269:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( CastingSpell->School != SCHOOL_SHADOW || !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;
						//warrior - improved berserker rage 
						case 23690:
						case 23691:
							{
								if( !CastingSpell || CastingSpell->NameHash != SPELL_HASH_BERSERKER_RAGE )
									continue;
							}break;
						//mage - Arcane Concentration 
						case 12536:
							{
								//requires damageing spell
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;
						//mage - Improved Blizzard
						case 12484:
						case 12485:
						case 12486:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_BLIZZARD || victim == this ) //Blizzard
									continue;
							}break;
						//mage - Master of Elements
						case 29077:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->School!=SCHOOL_FIRE && CastingSpell->School!=SCHOOL_FROST) //fire and frost criticals
									continue;
								dmg_overwrite = CastingSpell->manaCost * ( ospinfo->EffectBasePoints[0] + 1 ) / 100;
							}break;
						//Hunter - Thrill of the Hunt
						case 34720:
							{
								if( CastingSpell == NULL )
									continue;
								dmg_overwrite = CastingSpell->manaCost * 40 / 100;
							}break;
						//priest - Reflective Shield 
						case 33619:
							{
								//requires Power Word: Shield active
								int power_word_id = HasAurasWithNameHash( SPELL_HASH_POWER_WORD__SHIELD );
								if( !power_word_id )
									continue;//this should not ocur unless we made a fuckup somewhere
								//make a direct strike then exit rest of handler
								int tdmg = abs * ( ospinfo->EffectBasePoints[0] + 1 ) / 100;
								//somehow we should make this not caused any threat (tobedone)
								SpellNonMeleeDamageLog( victim, power_word_id, tdmg, false, true );
								continue;
							}break;
						//rogue - improved sprint
						case 30918:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_SPRINT || victim != this ) //sprint
									continue;
							}break;
						//rogue - combat potency
						case 35542:
						case 35545:
						case 35546:
						case 35547:
						case 35548:
							{
								if( !IsPlayer() || !dmg )
									continue;
								//this needs offhand weapon
								Item* it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
								if( it == NULL || it->GetProto()->InventoryType != INVTYPE_WEAPON )
									continue;
							}break;
						//paladin - Seal of Blood
						case 31893:
							{
								//we loose health depending on father of trigger spell when trigering this effect
								int32 healthtoloose = ospinfo->EffectBasePoints[1] * GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) / 100;
								if( healthtoloose > (int32)GetUInt32Value( UNIT_FIELD_HEALTH ) )
									SetUInt32Value( UNIT_FIELD_HEALTH, 1 );
								else
									ModUnsigned32Value( UNIT_FIELD_HEALTH, -healthtoloose );
							}break;
						//paladin - Improved Lay on Hands
						case 20233:
						case 20236:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_LAY_ON_HANDS )
									continue;
							}break;
						//paladin - Spiritual Attunement
						case 31786:
							{
								if( CastingSpell == NULL )
									continue;
								//trigger only on heal spell cast by NOT us
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING ) || this == victim )
									continue; 
							}break;
						//paladin - Light's Grace
						case 31834:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( CastingSpell->NameHash != SPELL_HASH_HOLY_LIGHT )
									continue; 
							}break;
						//paladin - Eye for an Eye
						case 25997:
							{
								if( victim == this )
									continue; //not self casted crits
								//requires damageing spell
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
								dmg_overwrite = ( dmg *  (ospinfo->EffectBasePoints[0] + 1 )) / 100 ; //only half dmg
								int32 half_health = this->GetUInt32Value(UNIT_FIELD_HEALTH) >> 1;
								if( dmg_overwrite > half_health )
									dmg_overwrite = half_health ;
							}break;
						case 45057: // Trinket: Evasive Maneuvers
							{
								if(GetHealthPct() > 35)
									continue;
							}
						//paladin - Blessed Life
						case 31828:
							{
								//we should test is damage is from enviroment or not :S
								ModUnsigned32Value(UNIT_FIELD_HEALTH,dmg/2);
								continue; //there is no visual for this ?
							}break;
							//paladin - sanctified judgement
							case 31930:
							{
								//!! not working since we use post even hook and seal disapears before event
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if(	CastingSpell->NameHash != SPELL_HASH_JUDGEMENT )
									continue;
								if( !IsPlayer() )
									continue; //great, we can only make this for players 
								Player* c = static_cast< Player* >( this );
								//printf("is there a seal on the player ? %u \n",c->Seal);
								if( !c->Seal )
									continue; //how the hack did we manage to cast judgement without a seal ?
								SpellEntry *spellInfo = dbcSpell.LookupEntry( c->Seal ); //null pointer check was already made
								if( !spellInfo )
									continue;	//now this is getting freeky, how the hell did we manage to create this bug ?
								dmg_overwrite = spellInfo->manaCost / 2 ; //only half dmg
								//printf("is there a seal on the player ? %u \n",dmg_overwrite);
							}break;
						//Energized
						case 43751:
							{
								if( CastingSpell == NULL )
									continue;
								if(	CastingSpell->NameHash != SPELL_HASH_LIGHTNING_BOLT )
									continue;
							}break;
						//Spell Haste Trinket
						case 33370:
							{
								if( CastingSpell == NULL )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;
						//shaman - Lightning Overload
						case 39805:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//trigger on lightning and chain lightning. Spell should be identical , well maybe next time :P
								if(	CastingSpell->NameHash == SPELL_HASH_LIGHTNING_BOLT || CastingSpell->NameHash == SPELL_HASH_CHAIN_LIGHTNING )
								{
									spellId = CastingSpell->Id;
									dmg_overwrite = (CastingSpell->EffectBasePoints[0] + 1) / 2; //only half dmg
								}
								else continue;
							}break;
						//item - Band of the Eternal Sage
						case 35084:
							{
								if( CastingSpell == NULL )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) ) //requires offensive spell. ! might not cover all spells
									continue;
							}break;
						// druid - Celestial Focus
						case 16922:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_STARFIRE )
									continue;
							}break;
						case 37565: //setbonus
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_FLASH_HEAL)
									continue;
							}break;
						//SETBONUSES
						case 37379:
							{
								if (!CastingSpell || CastingSpell->School != SCHOOL_SHADOW || !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
							}break;
						case 37378:
							{
								if (!CastingSpell || CastingSpell->School != SCHOOL_FIRE || !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
							}break;
						case 39950:
							{
								if (!CastingSpell ||  !(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING))
									continue;
							}break;
						case 37234:
						case 37214:
						case 37601:
							{
								if (!CastingSpell ||  !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
							}break;
						case 37237:
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_LIGHTNING_BOLT)
									continue;
							}break;
						case 37193:
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_HOLY_SHIELD)
									continue;
							}break;
						case 37196:
						case 43838:
							{
								if (!CastingSpell)
									continue;
								if (CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_THE_CRUSADER &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_JUSTICE &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_LIGHT &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_WISDOM &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_RIGHTEOUSNESS &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_BLOOD &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_VENGEANCE &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_COMMAND)
									continue;
							}break;
						case 43837:
							{
								if (!CastingSpell || (CastingSpell->NameHash != SPELL_HASH_FLASH_OF_LIGHT && CastingSpell->NameHash != SPELL_HASH_HOLY_LIGHT))
									continue;
							}break;
						case 37529:
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_OVERPOWER)
									continue;
							}break;
						case 37517:
							{
								if (!CastingSpell || CastingSpell->Id == 37517 || CastingSpell->NameHash != SPELL_HASH_REVENGE)
									continue; 
							}break;
						//SETBONUSES END
							//http://www.wowhead.com/?item=32493 Ashtongue Talisman of Shadows
						case 40480:
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_CORRUPTION )
									continue; 
							}break;

							//http://www.wowhead.com/?item=32496  Memento of Tyrande
						case 37656: //dont say damaging spell but EACH time spell is casted there is a chance (so can be healing spell)
							{
								if( CastingSpell == NULL )
									continue;
							}break;

							//http://www.wowhead.com/?item=32488 Ashtongue Talisman of Insight
						case 40483:
							{
								if( CastingSpell == NULL )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;

							//http://www.wowhead.com/?item=32487 Ashtongue Talisman of Swiftness
						case 40487:
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_STEADY_SHOT)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32485 Ashtongue Talisman of Valor
						case 40459:
							{
								if( CastingSpell == NULL || (CastingSpell->NameHash != SPELL_HASH_MORTAL_STRIKE || CastingSpell->NameHash != SPELL_HASH_BLOODTHIRST || CastingSpell->NameHash != SPELL_HASH_SHIELD_SLAM))
									continue; 
							}break;

						//item - Band of the Eternal Restorer 
						case 35087:
							{
								if( CastingSpell == NULL )
									continue;
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING)) //requires healing spell.
									continue;
							}break;

							//http://www.wowhead.com/?item=32486 Ashtongue Talisman of Equilibrium
						case 40452: //Mangle has a 40% chance to grant 140 Strength for 8 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_MANGLE__BEAR_ || CastingSpell->NameHash != SPELL_HASH_MANGLE__CAT_)
									continue; 
							}break;
						case 40445: //Starfire has a 25% chance to grant up to 150 spell damage for 8 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_STARFIRE)
									continue; 
							}break;
						case 40446: //Rejuvenation has a 25% chance to grant up to 210 healing for 8 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_REJUVENATION)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32490 Ashtongue Talisman of Acumen
						case 40441: //Each time your Shadow Word: Pain deals damage, it has a 10% chance to grant you 220 spell damage for 10 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_SHADOW_WORD__PAIN)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32490 Ashtongue Talisman of Acumen
						case 40440: //Each time your Renew heals, it has a 10% chance to grant you 220 healing for 5 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_RENEW)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32492 Ashtongue Talisman of Lethality
						case 40461:
							{
								if( CastingSpell == NULL )
									continue;
								//we need a finishing move for this 
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) || victim==this)
									continue;
							}break;
						case 37445: //using a mana gem grants you 225 spell damage for 15 sec
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_REPLENISH_MANA)
									continue; 
							}break;

						case 38395:
							{

								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_IMMOLATE &&
									CastingSpell->NameHash != SPELL_HASH_CORRUPTION)
									continue;
							}break;
					}
				}
				if(spellId==22858 && isInBack(victim)) //retatliation needs target to be not in front. Can be casted by creatures too
					continue;

				SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId );
				if( victim == this && spellInfo->c_is_flags & SPELL_FLAG_CANNOT_PROC_ON_SELF )
					continue;

				Spell *spell = new Spell(this, spellInfo ,true, NULL);
				spell->forced_basepoints[0] = dmg_overwrite;
				spell->ProcedOnSpell = CastingSpell;
				//Spell *spell = new Spell(this,spellInfo,false,0,true,false);
				if(spellId==974||spellId==32593||spellId==32594) // Earth Shield handler
				{
					spell->pSpellId=itr2->spellId;
					spell->SpellEffectDummy(0);
					delete spell;
					continue;
				}
				spell->pSpellId=origId;
				spell->prepare(&targets);
			}//not always we have a spell to cast
		}
	}

	m_chargeSpellsInUse=true;
	std::map<uint32,struct SpellCharge>::iterator iter,iter2;
	iter=m_chargeSpells.begin();
	while(iter!= m_chargeSpells.end())
	{
		iter2=iter++;
		if(iter2->second.count)
		{
			if((iter2->second.ProcFlag & flag))
			{
				//Fixes for spells that dont lose charges when dmg is absorbd
				if(iter2->second.ProcFlag==680&&dmg==0) continue;
				if(CastingSpell)
				{

					SpellCastTime *sd = dbcSpellCastTime.LookupEntry(CastingSpell->CastingTimeIndex);
					if(!sd) continue; // this shouldnt happen though :P
					//if we did not proc these then we should not remove them
					if( CastingSpell->Id == iter2->second.spellId)
						continue;
					switch(iter2->second.spellId)
					{
					case 12043:
						{
							//Presence of Mind and Nature's Swiftness should only get removed
							//when a non-instant and bellow 10 sec. Also must be nature :>
//							if(!sd->CastTime||sd->CastTime>10000) continue;
							if(sd->CastTime==0)
								continue;
						}break;
					case 17116: //Shaman - Nature's Swiftness
					case 16188:
						{
//							if( CastingSpell->School!=SCHOOL_NATURE||(!sd->CastTime||sd->CastTime>10000)) continue;
							if( CastingSpell->School!=SCHOOL_NATURE||(sd->CastTime==0)) continue;
						}break;
					case 16166:
						{
							if(!(CastingSpell->School==SCHOOL_FIRE||CastingSpell->School==SCHOOL_FROST||CastingSpell->School==SCHOOL_NATURE))
								continue;
						}break;
					case 14177: //cold blood will get removed on offensive spell
						{
							if(victim==this || isFriendly(this, victim))
								continue;

							// This is part 1 of mutilate. Since Mutilate is TWO spells, we skip this. :P
							// Instead, the second Strike will remove Cold Blood while they both reap the benefits.
							if(CastingSpell->Id == 34419)
								continue;
						}break;
					}
				}
				if(iter2->second.lastproc!=0)
				{
					if(iter2->second.procdiff>3000)
					{
						//--(iter2->second.count);
						RemoveAura(iter2->second.spellId);
					}
				}
				else
				{
					//--(iter2->second.count);		// done in Aura::Remove
					this->RemoveAura(iter2->second.spellId);
				}
			}
		}
		if(!iter2->second.count)
		{
			m_chargeSpells.erase(iter2);
		}
	}

	for(;!m_chargeSpellRemoveQueue.empty();)
	{
		iter = m_chargeSpells.find(m_chargeSpellRemoveQueue.front());
		if(iter != m_chargeSpells.end())
		{
			if(iter->second.count>1)
				--iter->second.count;
			else
				m_chargeSpells.erase(iter);
		}
		m_chargeSpellRemoveQueue.pop_front();
	}

	m_chargeSpellsInUse=false;
	if(can_delete) //are we the upper level of nested procs ? If yes then we can remove the lock
		bProcInUse = false;
}

//damage shield is a triggered spell by owner to atacker
void Unit::HandleProcDmgShield(uint32 flag, Unit* attacker)
{
	//make sure we do not loop dmg procs
	if(this==attacker || !attacker)
		return;
	if(m_damgeShieldsInUse)
		return;
	m_damgeShieldsInUse = true;
	//charges are already removed in handleproc
	WorldPacket data(24);
	std::list<DamageProc>::iterator i;
	std::list<DamageProc>::iterator i2;
	for(i = m_damageShields.begin();i != m_damageShields.end();)     // Deal Damage to Attacker
	{
		i2 = i++; //we should not proc on proc.. not get here again.. not needed.Better safe then sorry.
		if(	(flag & (*i2).m_flags) )
		{
			if(PROC_MISC & (*i2).m_flags)
			{
				data.Initialize(SMSG_SPELLDAMAGESHIELD);
				data << this->GetGUID();
				data << attacker->GetGUID();
				data << (*i2).m_spellId;
				data << (*i2).m_damage;
				data << g_spellSchoolConversionTable[(*i2).m_school];				
				SendMessageToSet(&data,true);
				this->DealDamage(attacker,(*i2).m_damage,0,0,(*i2).m_spellId);
			}
			else
			{
				SpellEntry	*ability=dbcSpell.LookupEntry((*i2).m_spellId);
				this->Strike( attacker, RANGED, ability, 0, 0, (*i2).m_damage, true, false );
			}
		}
	}
	m_damgeShieldsInUse = false;
}

/*
void Unit::HandleProcSpellOnSpell(Unit* Victim,uint32 damage,bool critical)
{
}
*/
bool Unit::isCasting()
{
	return (m_currentSpell != NULL);
}

bool Unit::IsInInstance()
{
	MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(this->GetMapId());
	if(pMapinfo)
		return (pMapinfo->type != INSTANCE_NULL);

	return false;
}

void Unit::RegenerateHealth()
{
	m_H_regenTimer = 2000;//set next regen time 

	if(!isAlive())
		return;

	// player regen
	if(this->IsPlayer())
	{
		// These only NOT in combat
		if(!CombatStatus.IsInCombat())
		{
			static_cast< Player* >( this )->RegenerateHealth(false);
		}
		else
			static_cast< Player* >( this )->RegenerateHealth(true);
	}
	else
	{
		// Only regen health out of combat
		if(!CombatStatus.IsInCombat())
			static_cast<Creature*>(this)->RegenerateHealth();
	}
}

void Unit::RegeneratePower(bool isinterrupted)
{
    // This is only 2000 IF the power is not rage
	if (isinterrupted)
		m_interruptedRegenTime =2000;
	else
		m_P_regenTimer = 2000;//set next regen time 

	if(!isAlive())
		return;

	// player regen
	if(this->IsPlayer())
	{
		uint32 powertype = GetPowerType();
		switch(powertype)
		{
		case POWER_TYPE_MANA:
			static_cast< Player* >( this )->RegenerateMana(isinterrupted);
			break;
		case POWER_TYPE_ENERGY:
			static_cast< Player* >( this )->RegenerateEnergy();
			break;

		case POWER_TYPE_RAGE:
			{
				// These only NOT in combat
				if(!CombatStatus.IsInCombat())
				{
					m_P_regenTimer = 3000;
					static_cast< Player* >( this )->LooseRage(30);
				}
				else
				{
					if (static_cast< Player* >( this )->mAngerManagement)
					{
						m_P_regenTimer = 3000;
						static_cast< Player* >( this )->LooseRage(-10);
					}
				}

			}break;
		}
		
		/*

		There is a problem here for druids.
		Druids when shapeshifted into bear have 2 power with different regen timers
		a) Mana (which regenerates while shapeshifted
		b) Rage

		Mana has a regen timer of 2 seconds
		Rage has a regen timer of 3 seconds

		I think the only viable way of fixing this is to have 2 different timers
		to check each individual power.

		Atm, mana is being regen at 3 seconds while shapeshifted...

		*/


		// druids regen mana when shapeshifted
		if(getClass() == DRUID && powertype != POWER_TYPE_MANA)
			static_cast< Player* >( this )->RegenerateMana(isinterrupted);
	}
	else
	{
		uint32 powertype = GetPowerType();
		switch(powertype)
		{
		case POWER_TYPE_MANA:
			static_cast<Creature*>(this)->RegenerateMana();
			break;
		case POWER_TYPE_FOCUS:
			static_cast<Creature*>(this)->RegenerateFocus();
			break;
		}
	}
}

void Unit::CalculateResistanceReduction(Unit *pVictim,dealdamage * dmg, SpellEntry* ability)
{
	float AverageResistance = 0.0f;
	float ArmorReduce;

	if((*dmg).school_type == 0)//physical
	{		
		if(this->IsPlayer())
			ArmorReduce = PowerCostPctMod[0];
		else
			ArmorReduce = 0.0f;

		if(ArmorReduce >= pVictim->GetResistance(0))		// fully penetrated :O
			return;

//		double Reduction = double(pVictim->GetResistance(0)) / double(pVictim->GetResistance(0)+400+(85*getLevel()));
		//dmg reduction formula from xinef
		double Reduction = 0;
		if(getLevel() < 60) Reduction = double(pVictim->GetResistance(0) - ArmorReduce) / double(pVictim->GetResistance(0)+400+(85*getLevel()));
		else if(getLevel() > 59 && getLevel() < 70) Reduction = double(pVictim->GetResistance(0) - ArmorReduce) / double(pVictim->GetResistance(0)-22167.5+(467.5*getLevel()));
		//
		else Reduction = double(pVictim->GetResistance(0) - ArmorReduce) / double(pVictim->GetResistance(0)+10557.5);
		if(Reduction > 0.75f) Reduction = 0.75f;
		else if(Reduction < 0) Reduction = 0;
		if(Reduction) dmg[0].full_damage = (uint32)(dmg[0].full_damage*(1-Reduction));	  // no multiply by 0
	}
	else
	{
		// applying resistance to other type of damage 
		int32 RResist = float2int32( float(pVictim->GetResistance( (*dmg).school_type ) + ((pVictim->getLevel() > getLevel()) ? (pVictim->getLevel() - this->getLevel()) * 5 : 0)) - PowerCostPctMod[(*dmg).school_type] ); 
		if (RResist<0)
			RResist = 0;
		AverageResistance = (float)(RResist) / (float)(getLevel() * 5) * 0.75f;
		  if(AverageResistance > 0.75f)
			AverageResistance = 0.75f;

		  // NOT WOWWIKILIKE but i think it's actual to add some fullresist chance frome resistances
		  if (!ability || !(ability->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
		  {
			  float Resistchance=(float)pVictim->GetResistance( (*dmg).school_type)/(float)pVictim->getLevel();
			  Resistchance*=Resistchance;
			  if(Rand(Resistchance))
				  AverageResistance=1.0f;
		  }

		if(AverageResistance>0)
			(*dmg).resisted_damage = (uint32)(((*dmg).full_damage)*AverageResistance);
		else 
			(*dmg).resisted_damage=0; 
	}
}

uint32 Unit::GetSpellDidHitResult( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability )
{
	Item * it = NULL;

	float hitchance          = 0.0f;
	float dodge				 = 0.0f;
	float parry				 = 0.0f;
	float block				 = 0.0f;

	float hitmodifier		 = 0;
	int32 self_skill;
	int32 victim_skill;
	uint32 SubClassSkill	 = SKILL_UNARMED;

	bool backAttack			 = isInBack( pVictim );
	uint32 vskill            = 0;

	//==========================================================================================
	//==============================Victim Skill Base Calculation===============================
	//==========================================================================================
	if( pVictim->IsPlayer() )
	{
		vskill = static_cast< Player* >( pVictim )->_GetSkillLineCurrent( SKILL_DEFENSE );
		if( weapon_damage_type != RANGED && !backAttack )
		{
			//--------------------------------block chance----------------------------------------------
			block = pVictim->GetFloatValue(PLAYER_BLOCK_PERCENTAGE); //shield check already done in Update chances
			//--------------------------------dodge chance----------------------------------------------
			if( pVictim->m_stunned <= 0 ) 
			{
				dodge = pVictim->GetFloatValue( PLAYER_DODGE_PERCENTAGE );
			}
			//--------------------------------parry chance----------------------------------------------
			if( pVictim->can_parry && !pVictim->disarmed )
			{
				if( static_cast< Player* >( pVictim )->HasSpell( 3127 ) || static_cast< Player* >( pVictim )->HasSpell( 18848 ) )
				{
					parry = pVictim->GetFloatValue( PLAYER_PARRY_PERCENTAGE );
				}
			}
		}
		victim_skill = float2int32( vskill + static_cast< Player* >( pVictim )->CalcRating( PLAYER_RATING_MODIFIER_DEFENCE ) );
	}
	//--------------------------------mob defensive chances-------------------------------------
	else
	{
		if( weapon_damage_type != RANGED && !backAttack )
			dodge = pVictim->GetUInt32Value(UNIT_FIELD_STAT1) / 14.5f; // what is this value?
		victim_skill = pVictim->getLevel() * 5;
		if(pVictim->m_objectTypeId == TYPEID_UNIT) 
		{ 
			Creature * c = (Creature*)(pVictim);
			if(c&&c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
			{
				victim_skill = std::max(victim_skill,((int32)this->getLevel()+3)*5); //used max to avoid situation when lowlvl hits boss.
			}
		} 
	}
	//==========================================================================================
	//==============================Attacker Skill Base Calculation=============================
	//==========================================================================================
	if(this->IsPlayer())
	{	  
		self_skill = 0;
		Player* pr = static_cast< Player* >( this );
		hitmodifier = pr->GetHitFromMeleeSpell();  

		switch( weapon_damage_type )
		{
		case MELEE:   // melee main hand weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL ) );
			break;
		case OFFHAND: // melee offhand weapon (dualwield)
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL ) );
			break;
		case RANGED:  // ranged weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_RANGED_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_RANGED_SKILL ) );
			break;
		}

		// erm. some spells don't use ranged weapon skill but are still a ranged spell and use melee stats instead
		// i.e. hammer of wrath
		if( ability && ability->NameHash == SPELL_HASH_HAMMER_OF_WRATH )
		{
			it = pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL ) );
		}

		if(it && it->GetProto())
			SubClassSkill = GetSkillByProto(it->GetProto()->Class,it->GetProto()->SubClass);
		else
			SubClassSkill = SKILL_UNARMED;

		if(SubClassSkill==SKILL_FIST_WEAPONS) 
			SubClassSkill = SKILL_UNARMED;

		//chances in feral form don't depend on weapon skill
		if(static_cast< Player* >( this )->IsInFeralForm()) 
		{
			uint8 form = static_cast< Player* >( this )->GetShapeShift();
			if(form == FORM_CAT || form == FORM_BEAR || form == FORM_DIREBEAR)
			{
				SubClassSkill = SKILL_FERAL_COMBAT;
				// Adjust skill for Level * 5 for Feral Combat
				self_skill += pr->getLevel() * 5;
			}
		}

		self_skill += pr->_GetSkillLineCurrent(SubClassSkill);
	}
	else
	{
		self_skill = this->getLevel() * 5;
		if(m_objectTypeId == TYPEID_UNIT) 
		{ 
			Creature * c = (Creature*)(this);
			if(c&&c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
				self_skill = std::max(self_skill,((int32)pVictim->getLevel()+3)*5);//used max to avoid situation when lowlvl hits boss.
		} 
	}
	//==========================================================================================
	//==============================Special Chances Base Calculation============================
	//==========================================================================================
	//<THE SHIT> to avoid Linux bug. 
	float diffVcapped = (float)self_skill;
	if(int32(pVictim->getLevel()*5)>victim_skill)
		diffVcapped -=(float)victim_skill;
	else
		diffVcapped -=(float)(pVictim->getLevel()*5);

	float diffAcapped = (float)victim_skill;
	if(int32(this->getLevel()*5)>self_skill)
		diffAcapped -=(float)self_skill;
	else
		diffAcapped -=(float)(this->getLevel()*5);
	//<SHIT END>

	//--------------------------------by victim state-------------------------------------------
	if(pVictim->IsPlayer()&&pVictim->GetStandState()) //every not standing state is >0
	{
		hitchance = 100.0f;
	}
	//--------------------------------by damage type and by weapon type-------------------------
	if( weapon_damage_type == RANGED ) 
	{
		dodge=0.0f;
		parry=0.0f;
	}
	else if(this->IsPlayer())
	{
		it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
		if( it != NULL && it->GetProto()->InventoryType == INVTYPE_WEAPON && !ability )//dualwield to-hit penalty
		{
			hitmodifier -= 19.0f;
		}
		else
		{
			it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			if( it != NULL && it->GetProto()->InventoryType == INVTYPE_2HWEAPON )//2 handed weapon to-hit penalty
				hitmodifier -= 4.0f;
		}
	}

	//--------------------------------by skill difference---------------------------------------
	float vsk = (float)self_skill - (float)victim_skill;
	dodge = std::max( 0.0f, dodge - vsk * 0.04f );
	if( parry )
		parry = std::max( 0.0f, parry - vsk * 0.04f );
	if( block )
		block = std::max( 0.0f, block - vsk * 0.04f );

	if( vsk > 0 )
		hitchance = std::max( hitchance, 95.0f + vsk * 0.02f + hitmodifier );
	else
	{
		if( pVictim->IsPlayer() )
			hitchance = std::max( hitchance, 95.0f + vsk * 0.1f + hitmodifier ); //wowwiki multiplier - 0.04 but i think 0.1 more balanced
		else
			hitchance = std::max( hitchance, 100.0f + vsk * 0.6f + hitmodifier ); //not wowwiki but more balanced
	}

	if( ability && ability->SpellGroupType )
		SM_FFValue( SM_FHitchance, &hitchance, ability->SpellGroupType );
	
	// overpower nana
	if( ability != NULL && ability->Attributes & ATTRIBUTES_CANT_BE_DPB )
	{
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
	}

	//==========================================================================================
	//==============================One Roll Processing=========================================
	//==========================================================================================
	//--------------------------------cummulative chances generation----------------------------
	float chances[4];
	chances[0]=std::max(0.0f,100.0f-hitchance);
	chances[1]=chances[0]+dodge;
	chances[2]=chances[1]+parry;
	chances[3]=chances[2]+block;

	//--------------------------------roll------------------------------------------------------
	float Roll = RandomFloat(100.0f);
	uint32 r = 0;
	while (r<4&&Roll>chances[r])
	{
		r++;
	}

	uint32 roll_results[5] = { SPELL_DID_HIT_MISS,SPELL_DID_HIT_DODGE,SPELL_DID_HIT_PARRY,SPELL_DID_HIT_BLOCK,SPELL_DID_HIT_SUCCESS };
	return roll_results[r];
}

void Unit::Strike( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 add_damage, int32 pct_dmg_mod, uint32 exclusive_damage, bool disable_proc, bool skip_hit_check )
{
//==========================================================================================
//==============================Unacceptable Cases Processing===============================
//==========================================================================================
	if(!pVictim->isAlive() || !isAlive()  || IsStunned() || IsPacified() || IsFeared())
		return;
	if(!isInFront(pVictim))
		if(IsPlayer())
		{
			static_cast< Player* >( this )->GetSession()->OutPacket(SMSG_ATTACKSWING_BADFACING);
			return;
		}
//==========================================================================================
//==============================Variables Initialization====================================
//==========================================================================================
	dealdamage dmg			  = {0,0,0};
	
	Item * it = NULL;
	
	float hitchance          = 0.0f;
	float dodge				 = 0.0f;
	float parry				 = 0.0f;
	float glanc              = 0.0f;
	float block				 = 0.0f;
	float crit				 = 0.0f;
	float crush              = 0.0f;

	uint32 targetEvent		 = 0;
	uint32 hit_status		 = 0;

	uint32 blocked_damage	 = 0;
	int32  realdamage		 = 0;

	uint32 vstate			 = 1;
	uint32 aproc			 = 0;
	uint32 vproc			 = 0;
	   
	float hitmodifier		 = 0;
	int32 self_skill;
	int32 victim_skill;
	uint32 SubClassSkill	 = SKILL_UNARMED;

	bool backAttack			 = !pVictim->isInFront( this );
	uint32 vskill            = 0;
	bool disable_dR			 = false;
	
	if(ability)
		dmg.school_type = ability->School;
	else
	{
		if (GetTypeId() == TYPEID_UNIT)
			dmg.school_type = static_cast< Creature* >( this )->BaseAttackType;
		else
			dmg.school_type = SCHOOL_NORMAL;
	}
//==========================================================================================
//==============================Victim Skill Base Calculation===============================
//==========================================================================================
	if(pVictim->IsPlayer())
	{
		vskill = static_cast< Player* >( pVictim )->_GetSkillLineCurrent( SKILL_DEFENSE );
		if( weapon_damage_type != RANGED && !backAttack )
		{
//--------------------------------block chance----------------------------------------------
				block = pVictim->GetFloatValue(PLAYER_BLOCK_PERCENTAGE); //shield check already done in Update chances
//--------------------------------dodge chance----------------------------------------------
			if(pVictim->m_stunned<=0) 
			{
				dodge = pVictim->GetFloatValue( PLAYER_DODGE_PERCENTAGE );
			}
//--------------------------------parry chance----------------------------------------------
			if(pVictim->can_parry && !disarmed)
			{
				parry = pVictim->GetFloatValue( PLAYER_PARRY_PERCENTAGE );
			}
		}
		victim_skill = float2int32( vskill + static_cast< Player* >( pVictim )->CalcRating( 1 ) );
	}
//--------------------------------mob defensive chances-------------------------------------
	else
	{
		if( weapon_damage_type != RANGED && !backAttack )
			dodge = pVictim->GetUInt32Value( UNIT_FIELD_STAT1 ) / 14.5f; // what is this value? (Agility)
		victim_skill = pVictim->getLevel() * 5;
		if( pVictim->m_objectTypeId == TYPEID_UNIT ) 
		{ 
			Creature* c = static_cast< Creature* >( pVictim );
			if( c != NULL && c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS )
			{
				victim_skill = std::max( victim_skill, ( (int32)getLevel() + 3 ) * 5 ); //used max to avoid situation when lowlvl hits boss.
			}
		} 
	}
//==========================================================================================
//==============================Attacker Skill Base Calculation=============================
//==========================================================================================
	if( this->IsPlayer() )
	{	  
		self_skill = 0;
		Player* pr = static_cast< Player* >( this );
		hitmodifier = pr->GetHitFromMeleeSpell();  
		
		switch( weapon_damage_type )
		{
		case MELEE:   // melee main hand weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL ) );
			if (it && it->GetProto())
				dmg.school_type = it->GetProto()->Damage[0].Type;
			break;
		case OFFHAND: // melee offhand weapon (dualwield)
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL ) );
			hit_status |= HITSTATUS_DUALWIELD;//animation
			if (it && it->GetProto())
				dmg.school_type = it->GetProto()->Damage[0].Type;
			break;
		case RANGED:  // ranged weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_RANGED_SKILL ) );
			if (it && it->GetProto())
				dmg.school_type = it->GetProto()->Damage[0].Type;
			break;
		}

		if(it && it->GetProto())
		{
			SubClassSkill = GetSkillByProto(it->GetProto()->Class,it->GetProto()->SubClass);
			if(SubClassSkill==SKILL_FIST_WEAPONS) 
				SubClassSkill = SKILL_UNARMED;
		}
		else
			SubClassSkill = SKILL_UNARMED;


		//chances in feral form don't depend on weapon skill
		if(pr->IsInFeralForm()) 
		{
			uint8 form = pr->GetShapeShift();
			if(form == FORM_CAT || form == FORM_BEAR || form == FORM_DIREBEAR)
			{
				SubClassSkill = SKILL_FERAL_COMBAT;
				self_skill += pr->getLevel() * 5;
			}
		}

		self_skill += pr->_GetSkillLineCurrent(SubClassSkill);
		crit = GetFloatValue(PLAYER_CRIT_PERCENTAGE);
	}
	else
	{
		self_skill = this->getLevel() * 5;
		if(m_objectTypeId == TYPEID_UNIT) 
		{ 
			Creature * c = (Creature*)(this);
			if(c&&c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
				self_skill = std::max(self_skill,((int32)pVictim->getLevel()+3)*5);//used max to avoid situation when lowlvl hits boss.
		} 
		crit = 5.0f; //will be modified later
	}
//==========================================================================================
//==============================Special Chances Base Calculation============================
//==========================================================================================
//<THE SHIT> to avoid Linux bug. 
float diffVcapped = (float)self_skill;
if(int32(pVictim->getLevel()*5)>victim_skill)
	diffVcapped -=(float)victim_skill;
else
	diffVcapped -=(float)(pVictim->getLevel()*5);

float diffAcapped = (float)victim_skill;
if(int32(this->getLevel()*5)>self_skill)
	diffAcapped -=(float)self_skill;
else
	diffAcapped -=(float)(this->getLevel()*5);
//<SHIT END>

//--------------------------------crushing blow chance--------------------------------------
	if(pVictim->IsPlayer()&&!this->IsPlayer()&&!ability && !dmg.school_type)
	{
		if(diffVcapped>=15.0f)
			crush = -15.0f+2.0f*diffVcapped; 
		else 
			crush = 0.0f;
	}
//--------------------------------glancing blow chance--------------------------------------
	if(this->IsPlayer()&&!pVictim->IsPlayer()&&!ability)
	{
		glanc = 10.0f + diffAcapped;
		if(glanc<0)
			glanc = 0.0f;
	}
//==========================================================================================
//==============================Advanced Chances Modifications==============================
//==========================================================================================
//--------------------------------by talents------------------------------------------------
	if(pVictim->IsPlayer())
	{
		if( weapon_damage_type != RANGED )
		{
			crit += static_cast< Player* >(pVictim)->res_M_crit_get();
			hitmodifier += static_cast< Player* >(pVictim)->m_resist_hit[0];
		}
		else 
		{
			crit += static_cast< Player* >(pVictim)->res_R_crit_get(); //this could be ability but in that case we overwrite the value
			hitmodifier += static_cast< Player* >(pVictim)->m_resist_hit[1];
		}
	}
	crit += (float)(pVictim->AttackerCritChanceMod[0]);
//--------------------------------by skill difference---------------------------------------

	float vsk = (float)self_skill - (float)victim_skill;
	dodge = std::max( 0.0f, dodge - vsk * 0.04f );
	if( parry )
		parry = std::max( 0.0f, parry - vsk * 0.04f );
	if( block )
		block = std::max( 0.0f, block - vsk * 0.04f );

	crit += pVictim->IsPlayer() ? vsk * 0.04f : min( vsk * 0.2f, 0.0f ); 

	if(vsk>0)
			hitchance = std::max(hitchance,95.0f+vsk*0.02f+hitmodifier);
	else
	{
		if(pVictim->IsPlayer())
			hitchance = std::max(hitchance,95.0f+vsk*0.04f+hitmodifier);
		else
			hitchance = std::max(hitchance,100.0f+vsk*0.6f+hitmodifier); //not wowwiki but more balanced
	}

	if(ability && ability->SpellGroupType)
	{
		SM_FFValue(SM_CriticalChance,&crit,ability->SpellGroupType);
		SM_FFValue(SM_FHitchance,&hitchance,ability->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		SM_FFValue(SM_CriticalChance,&spell_flat_modifers,ability->SpellGroupType);
		if(spell_flat_modifers!=0)
			printf("!!!!spell critchance mod flat %f ,spell group %u\n",spell_flat_modifers,ability->SpellGroupType);
#endif
	}
//--------------------------------by ratings------------------------------------------------
	crit -= pVictim->IsPlayer() ? static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) : 0.0f;
	if(crit<0) crit=0.0f;
	if (this->IsPlayer())
	{
		hitmodifier += (weapon_damage_type == RANGED) ? static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_RANGED_HIT ) : static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
		dodge -=static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_EXPERTISE );
		if(dodge<0) dodge=0.0f;
		parry -=static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_EXPERTISE );
		if(parry<0) parry=0.0f;
	}
	

//--------------------------------by damage type and by weapon type-------------------------
	if( weapon_damage_type == RANGED ) 
	{
		dodge=0.0f;
		parry=0.0f;
		glanc=0.0f;
	}
	else
		if(this->IsPlayer())
		{
			it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			if( it != NULL && it->GetProto()->InventoryType == INVTYPE_WEAPON && !ability )//dualwield to-hit penalty
				hitmodifier -= 19.0f;
			else
			{
				it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
				if( it != NULL && it->GetProto()->InventoryType == INVTYPE_2HWEAPON )//2 handed weapon to-hit penalty
					hitmodifier -= 4.0f;
			}
		}

	//Hackfix for Surprise Attacks
	if(  this->IsPlayer() && ability && static_cast< Player* >( this )->m_finishingmovesdodge && ability->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE)
			dodge = 0.0f;

	if( skip_hit_check )
	{
		hitchance = 100.0f;
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
	}

	if( ability != NULL && ability->Attributes & ATTRIBUTES_CANT_BE_DPB )
	{
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
	}

//--------------------------------by victim state-------------------------------------------
	if(pVictim->IsPlayer()&&pVictim->GetStandState()) //every not standing state is >0
	{
		hitchance = 100.0f;
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
		crush = 0.0f;
		crit = 100.0f;
	}
//==========================================================================================
//==============================One Roll Processing=========================================
//==========================================================================================
//--------------------------------cummulative chances generation----------------------------
	float chances[7];
	chances[0]=std::max(0.0f,100.0f-hitchance);
	chances[1]=chances[0]+dodge;
	chances[2]=chances[1]+parry;
	chances[3]=chances[2]+glanc;
	chances[4]=chances[3]+block;
	chances[5]=chances[4]+crit;
	chances[6]=chances[5]+crush;
//--------------------------------roll------------------------------------------------------
	float Roll = RandomFloat(100.0f);
	uint32 r = 0;
	while (r<7&&Roll>chances[r])
	{
		r++;
	}
//--------------------------------postroll processing---------------------------------------
	uint32 abs = 0;

	switch(r)
	{ 
//--------------------------------miss------------------------------------------------------
	case 0:
		hit_status |= HITSTATUS_MISS;
		// dirty ai agro fix
		if(pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetAIInterface()->GetNextTarget() == NULL)
			pVictim->GetAIInterface()->AttackReaction(this, 1, 0);
		break;
//--------------------------------dodge-----------------------------------------------------
	case 1:
		// dirty ai agro fix
		if(pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetAIInterface()->GetNextTarget() == NULL)
			pVictim->GetAIInterface()->AttackReaction(this, 1, 0);

		CALL_SCRIPT_EVENT(pVictim, OnTargetDodged)(this);
		CALL_SCRIPT_EVENT(this, OnDodged)(this);
		targetEvent = 1;
		vstate = DODGE;
		vproc |= PROC_ON_DODGE_VICTIM;
		pVictim->Emote(EMOTE_ONESHOT_PARRYUNARMED);			// Animation
		if( pVictim->IsPlayer() )
		{
			//allmighty warrior overpower
			if( this->IsPlayer() && static_cast< Player* >( this )->getClass() == WARRIOR )
			{
				static_cast< Player* >( this )->AddComboPoints( pVictim->GetGUID(), 1 );
				static_cast< Player* >( this )->UpdateComboPoints();
				if( !sEventMgr.HasEvent( static_cast< Player* >( this ), EVENT_COMBO_POINT_CLEAR_FOR_TARGET ) )
					sEventMgr.AddEvent( static_cast< Player* >( this ), &Player::NullComboPoints, (uint32)EVENT_COMBO_POINT_CLEAR_FOR_TARGET, (uint32)5000, (uint32)1, (uint32)0 );
				else
					sEventMgr.ModifyEventTimeLeft( static_cast< Player* >( this ), EVENT_COMBO_POINT_CLEAR_FOR_TARGET, 5000 ,0 );
			}
			pVictim->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_DODGE_BLOCK);	//SB@L: Enables spells requiring dodge
			if(!sEventMgr.HasEvent(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE))
				sEventMgr.AddEvent(pVictim,&Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_DODGE_BLOCK,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000,1,0);
			else sEventMgr.ModifyEventTimeLeft(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000,0);
		}
		break;
//--------------------------------parry-----------------------------------------------------
	case 2:
		// dirty ai agro fix
		if(pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetAIInterface()->GetNextTarget() == NULL)
			pVictim->GetAIInterface()->AttackReaction(this, 1, 0);

		CALL_SCRIPT_EVENT(pVictim, OnTargetParried)(this);
		CALL_SCRIPT_EVENT(this, OnParried)(this);
		targetEvent = 3;
		vstate = PARRY;
		pVictim->Emote(EMOTE_ONESHOT_PARRYUNARMED);			// Animation
		if(pVictim->IsPlayer())
		{
			pVictim->SetFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_PARRY );	//SB@L: Enables spells requiring parry
			if(!sEventMgr.HasEvent( pVictim, EVENT_PARRY_FLAG_EXPIRE ) )
				sEventMgr.AddEvent( pVictim, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_PARRY,EVENT_PARRY_FLAG_EXPIRE, 5000, 1, 0 );
			else 
				sEventMgr.ModifyEventTimeLeft( pVictim, EVENT_PARRY_FLAG_EXPIRE, 5000 );
			if( static_cast< Player* >( pVictim )->getClass() == 1 || static_cast< Player* >( pVictim )->getClass() == 4 )//warriors for 'revenge' and rogues for 'riposte'
			{
				//pVictim->SetFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_DODGE_BLOCK );	//SB@L: Enables spells requiring dodge
				if(!sEventMgr.HasEvent( pVictim, EVENT_DODGE_BLOCK_FLAG_EXPIRE ) )
					sEventMgr.AddEvent( pVictim, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_DODGE_BLOCK, EVENT_DODGE_BLOCK_FLAG_EXPIRE, 5000, 1, 0 );
				else 
					sEventMgr.ModifyEventTimeLeft( pVictim, EVENT_DODGE_BLOCK_FLAG_EXPIRE, 5000 );
			}
		}
		break;
//--------------------------------not miss,dodge or parry-----------------------------------
	default:
		hit_status |= HITSTATUS_HITANIMATION;//hit animation on victim
		if( pVictim->SchoolImmunityList[0] )
			vstate = IMMUNE;		
		else
		{
//--------------------------------state proc initialization---------------------------------
			vproc |= PROC_ON_ANY_DAMAGE_VICTIM;			
			if( weapon_damage_type != RANGED )
			{
				aproc |= PROC_ON_MELEE_ATTACK;
				vproc |= PROC_ON_MELEE_ATTACK_VICTIM;
			}
			else
			{
				aproc |= PROC_ON_RANGED_ATTACK;
				vproc |= PROC_ON_RANGED_ATTACK_VICTIM;
				if(ability && ability->Id==3018 && IsPlayer() && getClass()==HUNTER)
					aproc |= PROC_ON_AUTO_SHOT_HIT;
			}
//--------------------------------base damage calculation-----------------------------------
			if(exclusive_damage)
				dmg.full_damage = exclusive_damage;
			else
			{
				if( weapon_damage_type == MELEE && ability )
					dmg.full_damage = CalculateDamage( this, pVictim, MELEE, ability->SpellGroupType, ability );
				else			
					dmg.full_damage = CalculateDamage( this, pVictim, weapon_damage_type, 0, ability );
			}

			if(ability && ability->SpellGroupType)
			{	
				SM_FIValue(((Unit*)this)->SM_FDamageBonus,&dmg.full_damage,ability->SpellGroupType);
				SM_PIValue(((Unit*)this)->SM_PDamageBonus,&dmg.full_damage,ability->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
				int spell_flat_modifers=0;
				int spell_pct_modifers=0;
				SM_FIValue(((Unit*)this)->SM_FDamageBonus,&spell_flat_modifers,ability->SpellGroupType);
				SM_FIValue(((Unit*)this)->SM_PDamageBonus,&spell_pct_modifers,ability->SpellGroupType);
				if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
					printf("!!!!!spell dmg bonus mod flat %d , spell dmg bonus pct %d , spell dmg bonus %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,dmg.full_damage,ability->SpellGroupType);
#endif
			}
			dmg.full_damage += pVictim->DamageTakenMod[dmg.school_type];
			if( weapon_damage_type == RANGED )
			{
				dmg.full_damage += pVictim->RangedDamageTaken;
			}
			
			if( ability && ability->MechanicsType == MECHANIC_BLEEDING )
				disable_dR = true; 
			
			//float summaryPCTmod = (pVictim->DamageTakenPctMod[dmg.school_type] / 100.0f) + (GetDamageDonePctMod( dmg.school_type ) / 100.0f) + 1;

			if( pct_dmg_mod > 0 )
				dmg.full_damage = float2int32( dmg.full_damage *  ( float( pct_dmg_mod) / 100.0f ) );

			dmg.full_damage += add_damage;

			//a bit dirty fix
			/*if( ability != NULL && ability->NameHash == SPELL_HASH_SHRED )
			{
				summaryPCTmod *= 1 + pVictim->ModDamageTakenByMechPCT[MECHANIC_BLEEDING];
			}*/

			//dmg.full_damage = (dmg.full_damage < 0) ? 0 : float2int32(dmg.full_damage*summaryPCTmod);

			// burlex: fixed this crap properly
			float inital_dmg = float(dmg.full_damage);
			float dd_mod = GetDamageDonePctMod( dmg.school_type );
			if( pVictim->DamageTakenPctMod[dmg.school_type] != 1.0f )
				dmg.full_damage += float2int32( ( inital_dmg * pVictim->DamageTakenPctMod[ dmg.school_type ] ) - inital_dmg );

			if( dd_mod > 1.0f )
				dmg.full_damage += float2int32( ( inital_dmg * dd_mod) - inital_dmg );

			if( ability != NULL && ability->NameHash == SPELL_HASH_SHRED )
				dmg.full_damage += float2int32( ( inital_dmg * (1 + pVictim->ModDamageTakenByMechPCT[MECHANIC_BLEEDING]) ) - inital_dmg );

			//pet happiness state dmg modifier
			if( IsPet() && !static_cast<Pet*>(this)->IsSummon() )
				dmg.full_damage = ( dmg.full_damage <= 0 ) ? 0 : float2int32( dmg.full_damage * static_cast< Pet* >( this )->GetHappinessDmgMod() );

			if(dmg.full_damage < 0)
				dmg.full_damage = 0;
//--------------------------------check for special hits------------------------------------
			switch(r)
			{
//--------------------------------glancing blow---------------------------------------------
			case 3:
				{
					float low_dmg_mod = 1.5f - (0.05f * diffAcapped);
					if(this->getClass() == MAGE || this->getClass() == PRIEST || this->getClass() == WARLOCK) //casters = additional penalty.
					{
						low_dmg_mod -= 0.7f;
					}
					if(low_dmg_mod<0.01)
						low_dmg_mod = 0.01f;
					if(low_dmg_mod>0.91)
						low_dmg_mod = 0.91f;
					float high_dmg_mod = 1.2f - (0.03f * diffAcapped);
					if(this->getClass() == MAGE || this->getClass() == PRIEST || this->getClass() == WARLOCK) //casters = additional penalty.
					{
						high_dmg_mod -= 0.3f;
					}
					if(high_dmg_mod>0.99)
						high_dmg_mod = 0.99f;
					if(high_dmg_mod<0.2)
						high_dmg_mod = 0.2f;

					float damage_reduction = (high_dmg_mod + low_dmg_mod) / 2.0f;
					if(damage_reduction > 0)
					{
							dmg.full_damage = float2int32(damage_reduction * float(dmg.full_damage));
					}
					hit_status |= HITSTATUS_GLANCING;
				}
				break;
//--------------------------------block-----------------------------------------------------
			case 4:
				{
					Item* shield = static_cast< Player* >( pVictim )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
					if( shield != NULL )
					{
						targetEvent = 2;
						pVictim->Emote(EMOTE_ONESHOT_PARRYSHIELD);// Animation

						if( shield->GetProto()->InventoryType == INVTYPE_SHIELD )
						{
							float block_multiplier = ( 100.0f + float( static_cast< Player* >( pVictim )->m_modblockabsorbvalue ) ) / 100.0f;
							if( block_multiplier < 1.0f )block_multiplier = 1.0f;

							blocked_damage = float2int32( float( shield->GetProto()->Block ) + ( float( static_cast< Player* >( pVictim )->m_modblockvaluefromspells + pVictim->GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) ) * block_multiplier ) + ( ( float( pVictim->GetUInt32Value( UNIT_FIELD_STAT0 ) ) / 20.0f ) - 1.0f ) );
						}
						else
						{
							blocked_damage = 0;
						}

						if(dmg.full_damage <= (int32)blocked_damage)
						{
							CALL_SCRIPT_EVENT(pVictim, OnTargetBlocked)(this, blocked_damage);
							CALL_SCRIPT_EVENT(this, OnBlocked)(pVictim, blocked_damage);
							vstate = BLOCK;
							vproc |= PROC_ON_BLOCK_VICTIM;
						}
						if( pVictim->IsPlayer() )//not necessary now but we'll have blocking mobs in future
						{            
							pVictim->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_DODGE_BLOCK);	//SB@L: Enables spells requiring dodge
							if(!sEventMgr.HasEvent(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE))
								sEventMgr.AddEvent(pVictim,&Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_DODGE_BLOCK,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000,1,0);
							else 
								sEventMgr.ModifyEventTimeLeft(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000);
						}
					}
				}
				break;
//--------------------------------critical hit----------------------------------------------
			case 5:
				{
					hit_status |= HITSTATUS_CRICTICAL;
					float dmg_bonus_pct = 100.0f;
					if(ability && ability->SpellGroupType)
						SM_FFValue(SM_PCriticalDamage,&dmg_bonus_pct,ability->SpellGroupType);
					
					// resilience
					if( pVictim->IsPlayer() )
						dmg_bonus_pct -= 2.0f * static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE );

					if( IsPlayer() )
					{
						// m_modphyscritdmgPCT
						if( weapon_damage_type != RANGED )
						{
							dmg_bonus_pct += (float)TO_PLAYER(this)->m_modphyscritdmgPCT;
						}

						// IncreaseCriticalByTypePct
						if( !pVictim->IsPlayer() )
						{
							CreatureInfo *pCreatureName = TO_CREATURE(pVictim)->GetCreatureName();
							if( pCreatureName != NULL )
								dmg_bonus_pct += TO_PLAYER(this)->IncreaseCricticalByTypePCT[pCreatureName->Type];
						}
					}

					// SpellAuraReduceCritRangedAttackDmg
					if( weapon_damage_type == RANGED )
						dmg_bonus_pct -= CritRangedDamageTakenPctMod[dmg.school_type];
					else
						dmg_bonus_pct -= CritMeleeDamageTakenPctMod[dmg.school_type];

					// actual crit damage?
					if( dmg_bonus_pct > 0 )
						dmg.full_damage += float2int32( float(dmg.full_damage) * (dmg_bonus_pct / 100.0f));

					//e mote
					pVictim->Emote(EMOTE_ONESHOT_WOUNDCRITICAL);
					vproc |= PROC_ON_CRIT_HIT_VICTIM;
					aproc |= PROC_ON_CRIT_ATTACK;
					if( weapon_damage_type == RANGED )
					{
						vproc |= PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
						aproc |= PROC_ON_RANGED_CRIT_ATTACK;
					}

					if(this->IsPlayer())
					{
						this->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_CRITICAL);	//SB@L: Enables spells requiring critical strike
						if(!sEventMgr.HasEvent(this,EVENT_CRIT_FLAG_EXPIRE))
							sEventMgr.AddEvent((Unit*)this,&Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_CRITICAL,EVENT_CRIT_FLAG_EXPIRE,5000,1,0);
						else sEventMgr.ModifyEventTimeLeft(this,EVENT_CRIT_FLAG_EXPIRE,5000);
					}

					CALL_SCRIPT_EVENT(pVictim, OnTargetCritHit)(this, float(dmg.full_damage));
					CALL_SCRIPT_EVENT(this, OnCritHit)(pVictim, float(dmg.full_damage));
				}
				break;
//--------------------------------crushing blow---------------------------------------------
			case 6:
				hit_status |= HITSTATUS_CRUSHINGBLOW;
				dmg.full_damage = (dmg.full_damage * 3) >> 1;
				break;
//--------------------------------regular hit-----------------------------------------------
			default:
				break;	
			}
//==========================================================================================
//==============================Post Roll Damage Processing=================================
//==========================================================================================
//--------------------------absorption------------------------------------------------------
			uint32 dm = dmg.full_damage;
			abs = pVictim->AbsorbDamage(dmg.school_type,(uint32*)&dm);
		
			if(dmg.full_damage > (int32)blocked_damage)
			{
				uint32 sh = pVictim->ManaShieldAbsorb(dmg.full_damage);
//--------------------------armor reducing--------------------------------------------------
				if(sh)
				{
					dmg.full_damage -= sh;
					if(dmg.full_damage && !disable_dR)
						CalculateResistanceReduction(pVictim,&dmg, ability);
					dmg.full_damage += sh;
					abs+=sh;
				}
				else if(!disable_dR)
					CalculateResistanceReduction(pVictim,&dmg, ability);	
			}

			if(abs)
				vproc |= PROC_ON_ABSORB;

			if (dmg.school_type == SCHOOL_NORMAL)
			{
				abs+=dmg.resisted_damage;
				dmg.resisted_damage=0;
			}

			realdamage = dmg.full_damage-abs-dmg.resisted_damage-blocked_damage;
			if(realdamage < 0)
			{
				realdamage = 0;
				vstate = IMMUNE;
				hit_status |= HITSTATUS_ABSORBED;
			}
		}
		break;
	}

//==========================================================================================
//==============================Post Roll Special Cases Processing==========================
//==========================================================================================
//--------------------------special states processing---------------------------------------
	if(pVictim->GetTypeId() == TYPEID_UNIT)
	{
		if(pVictim->GetAIInterface() && (pVictim->GetAIInterface()->getAIState()== STATE_EVADE ||
										(pVictim->GetAIInterface()->GetIsSoulLinked() && pVictim->GetAIInterface()->getSoullinkedWith() != this)))
		{
			vstate = EVADE;
			realdamage = 0;
			dmg.full_damage = 0;
			dmg.resisted_damage = 0;
		}
	}
	if(pVictim->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >(pVictim)->GodModeCheat == true)
	{
		dmg.resisted_damage = dmg.full_damage; //godmode
	}
//--------------------------dirty fixes-----------------------------------------------------
	//vstate=1-wound,2-dodge,3-parry,4-interrupt,5-block,6-evade,7-immune,8-deflect	
	// the above code was remade it for reasons : damage shield needs moslty same flags as handleproc + dual wield should proc too ?
	if( !disable_proc )
    {
		this->HandleProc(aproc,pVictim, ability,realdamage,abs); //maybe using dmg.resisted_damage is better sometimes but then if using godmode dmg is resisted instead of absorbed....bad
		m_procCounter = 0;

		pVictim->HandleProc(vproc,this, ability,realdamage,abs);
		pVictim->m_procCounter = 0;

		if(realdamage > 0)
		{
			pVictim->HandleProcDmgShield(vproc,this);
			HandleProcDmgShield(aproc,pVictim);
		}
	}
//--------------------------spells triggering-----------------------------------------------
	if(realdamage > 0 && ability == 0)
	{
		if( IsPlayer() && static_cast< Player* >( this )->m_onStrikeSpells.size() )
		{
			SpellCastTargets targets;
			targets.m_unitTarget = pVictim->GetGUID();
			targets.m_targetMask = 0x2;
			Spell* cspell;

			// Loop on hit spells, and strike with those.
			for( map< SpellEntry*, pair< uint32, uint32 > >::iterator itr = static_cast< Player* >( this )->m_onStrikeSpells.begin();
				itr != static_cast< Player* >( this )->m_onStrikeSpells.end(); ++itr )
			{
				if( itr->second.first )
				{
					// We have a *periodic* delayed spell.
					uint32 t = getMSTime();
					if( t > itr->second.second )  // Time expired
					{
						// Set new time
						itr->second.second = t + itr->second.first;
					}

					// Cast.
					cspell = new Spell(this, itr->first, true, NULL);
					cspell->prepare(&targets);
				}
				else
				{
					cspell = new Spell(this, itr->first, true, NULL);
					cspell->prepare(&targets);
				}			
			}
		}

		if( IsPlayer() && static_cast< Player* >( this )->m_onStrikeSpellDmg.size() )
		{
			map< uint32, OnHitSpell >::iterator it2 = static_cast< Player* >( this )->m_onStrikeSpellDmg.begin();
			map< uint32, OnHitSpell >::iterator itr;
			uint32 min_dmg, max_dmg, range, dmg;
			for(; it2 != static_cast< Player* >( this )->m_onStrikeSpellDmg.end(); )
			{
				itr = it2;
				++it2;

				min_dmg = itr->second.mindmg;
				max_dmg = itr->second.maxdmg;
				range = min_dmg - max_dmg;
				dmg = min_dmg;
				if(range) range += RandomUInt(range);

				SpellNonMeleeDamageLog(pVictim, itr->second.spellid, dmg, true);
			}
		}

		// refresh judgements
		// TODO: find the opcode to refresh the aura or just remove it and re add it
		// rather than fuck with duration
		// DONE: Remove + readded it :P
		for( uint32 x = MAX_POSITIVE_AURAS; x <= MAX_AURAS; x++ )
		{
			if( pVictim->m_auras[x] != NULL && pVictim->m_auras[x]->GetUnitCaster() != NULL && pVictim->m_auras[x]->GetUnitCaster()->GetGUID() == GetGUID() && pVictim->m_auras[x]->GetSpellProto()->buffIndexType == SPELL_TYPE_INDEX_JUDGEMENT )
			{
				Aura * aur = pVictim->m_auras[x];
				SpellEntry * spinfo = aur->GetSpellProto();
				aur->Remove();
				Spell * sp = new Spell( this , spinfo , true , NULL );
				SpellCastTargets tgt;
				tgt.m_unitTarget = pVictim->GetGUID();
				sp->prepare( &tgt );
				/*pVictim->m_auras[x]->SetDuration( 20000 ); // 20 seconds?
				sEventMgr.ModifyEventTimeLeft( pVictim->m_auras[x], EVENT_AURA_REMOVE, 20000 );
			
				// We have to tell the target that the aura has been refreshed.
				if( pVictim->IsPlayer() )
				{
					WorldPacket data( 5 );
					data.SetOpcode( SMSG_UPDATE_AURA_DURATION );
					data << (uint8)pVictim->m_auras[x]->GetAuraSlot() << 20000;
					static_cast< Player* >( pVictim )->GetSession()->SendPacket( &data );
				}
				*/
				// However, there is also an opcode that tells the caster that the aura has been refreshed.
				// This isn't implemented anywhere else in the source, so I can't work on that part :P
				// (The 'cooldown' meter on the target frame that shows how long the aura has until expired does not get reset)=
				// I would say break; here, but apparently in Ascent, one paladin can have multiple judgements on the target. No idea if this is blizzlike or not.
			}
		}

	}
	
//==========================================================================================
//==============================Data Sending================================================
//==========================================================================================
	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 70);
	//0x4--dualwield,0x10 miss,0x20 absorbed,0x80 crit,0x4000 -glancing,0x8000-crushing
	//only for melee!

	if( !ability )
	{
		if( dmg.full_damage > 0 )
		{
			if( dmg.full_damage == (int32)abs )
				hit_status |= HITSTATUS_ABSORBED;
			else if (dmg.full_damage <= (int32)dmg.resisted_damage)
			{
				hit_status |= HITSTATUS_RESIST;
				dmg.resisted_damage = dmg.full_damage;
			}
		}

		if( dmg.full_damage < 0 )
			dmg.full_damage = 0;

		if( realdamage < 0 )
			realdamage = 0;

		data << (uint32)hit_status;   
		data << GetNewGUID();
		data << pVictim->GetNewGUID();
		
		data << (uint32)realdamage;		 // Realdamage;
		data << (uint8)1;				   // Damage type counter / swing type

		data << (uint32)g_spellSchoolConversionTable[dmg.school_type];				  // Damage school
		data << (float)dmg.full_damage;	 // Damage float
		data << (uint32)dmg.full_damage;	// Damage amount
		data << (uint32)abs;// Damage absorbed
		data << (uint32)dmg.resisted_damage;				  // Damage resisted

		data << (uint32)vstate;			 // new victim state
		data << (uint32)0x03e8;					// can be 0,1000 or -1
		data << (uint32)0;				  // unknown
		data << (uint32)blocked_damage;	 // Damage amount blocked
		//data << (uint32) 0;

		SendMessageToSet(&data, this->IsPlayer());
	}
	else
	{
		if( realdamage > 0 )//FIXME: add log for miss,block etc for ability and ranged
		{
			// here we send "dmg.resisted_damage" for "AbsorbedDamage", "0" for "ResistedDamage", and "false" for "PhysicalDamage" even though "School" is "SCHOOL_NORMAL"   o_O
			SendSpellNonMeleeDamageLog( this, pVictim, ability->Id, realdamage, dmg.school_type, dmg.resisted_damage, 0, false, blocked_damage, ( ( hit_status & HITSTATUS_CRICTICAL ) != 0 ), true );
		}
		//FIXME: add log for miss,block etc for ability and ranged
		//example how it works
		//SendSpellLog(this,pVictim,ability->Id,SPELL_LOG_MISS);
	}

	if(ability && realdamage==0)
	{	
		SendSpellLog(this,pVictim,ability->Id,SPELL_LOG_RESIST);
	}
//==========================================================================================
//==============================Damage Dealing==============================================
//==========================================================================================

	if(this->IsPlayer() && ability)
		static_cast< Player* >( this )->m_casted_amount[dmg.school_type]=(uint32)(realdamage+abs);
	if(realdamage)
	{
		DealDamage(pVictim, realdamage, 0, targetEvent, 0);
		//pVictim->HandleProcDmgShield(PROC_ON_MELEE_ATTACK_VICTIM,this);
//		HandleProcDmgShield(PROC_ON_MELEE_ATTACK_VICTIM,pVictim);

		if(pVictim->GetCurrentSpell())
			pVictim->GetCurrentSpell()->AddTime(0);
	}
	else
	{		
		// have to set attack target here otherwise it wont be set
		// because dealdamage is not called.
		//setAttackTarget(pVictim);
	}
//==========================================================================================
//==============================Post Damage Dealing Processing==============================
//==========================================================================================
//--------------------------durability processing-------------------------------------------
	if(pVictim->IsPlayer())
	{
		static_cast< Player* >( pVictim )->GetItemInterface()->ReduceItemDurability();
		if( !this->IsPlayer() )
		{
			Player *pr = static_cast< Player* >( pVictim );
			if( Rand( pr->GetSkillUpChance( SKILL_DEFENSE ) * sWorld.getRate( RATE_SKILLCHANCE ) ) )
			{
				pr->_AdvanceSkillLine( SKILL_DEFENSE, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
				pr->UpdateChances();
			}
		}
		else
		{
			 static_cast< Player* >( this )->GetItemInterface()->ReduceItemDurability();
		}
	}
	else
	{
		if(this->IsPlayer())//not pvp
		{
			static_cast< Player* >( this )->GetItemInterface()->ReduceItemDurability();
			Player* pr = static_cast< Player* >( this );
			if( Rand( pr->GetSkillUpChance( SubClassSkill) * sWorld.getRate( RATE_SKILLCHANCE ) ) )
			{
				pr->_AdvanceSkillLine( SubClassSkill, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
				//pr->UpdateChances();
			}
		}
	}
	//--------------------------rage processing-------------------------------------------------
	//http://www.wowwiki.com/Formulas:Rage_generation

	if( dmg.full_damage && IsPlayer() && GetPowerType() == POWER_TYPE_RAGE && !ability)
	{
		float val;
		float level = (float)getLevel();

		// Conversion Value
		float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;

		// Hit Factor
		float f = ( weapon_damage_type == OFFHAND ) ? 1.75f : 3.5f;

		if( hit_status & HITSTATUS_CRICTICAL )
			f *= 2.0f;

		float s = 1.0f;

		// Weapon speed (normal)
		Item* weapon = ( static_cast< Player* >( this )->GetItemInterface())->GetInventoryItem( INVENTORY_SLOT_NOT_SET, ( weapon_damage_type == OFFHAND ? EQUIPMENT_SLOT_OFFHAND : EQUIPMENT_SLOT_MAINHAND ) );
		if( weapon == NULL )
		{
			if( weapon_damage_type == OFFHAND )
				s = GetUInt32Value( UNIT_FIELD_BASEATTACKTIME_01 ) / 1000.0f;
			else
				s = GetUInt32Value( UNIT_FIELD_BASEATTACKTIME ) / 1000.0f;
		}
		else
		{
			uint32 entry = weapon->GetEntry();
			ItemPrototype* pProto = ItemPrototypeStorage.LookupEntry( entry );
			if( pProto != NULL )
			{
				s = pProto->Delay / 1000.0f;
			}
		}

		val = ( 7.5f * dmg.full_damage / c + f * s ) / 2.0f;;
		val *= ( 1 + ( static_cast< Player* >( this )->rageFromDamageDealt / 100.0f ) );
		val *= 10;

		//float r = ( 7.5f * dmg.full_damage / c + f * s ) / 2.0f;
		//float p = ( 1 + ( static_cast< Player* >( this )->rageFromDamageDealt / 100.0f ) );
		//DEBUG_LOG( "Rd(%i) d(%i) c(%f) f(%f) s(%f) p(%f) r(%f) rage = %f", realdamage, dmg.full_damage, c, f, s, p, r, val );

		ModUnsigned32Value( UNIT_FIELD_POWER2, (int32)val );
		if( GetUInt32Value( UNIT_FIELD_POWER2 ) > 1000 )
			ModUnsigned32Value( UNIT_FIELD_POWER2, 1000 - GetUInt32Value( UNIT_FIELD_POWER2 ) );

	}

	// I am receiving damage!
	if( dmg.full_damage && pVictim->IsPlayer() && pVictim->GetPowerType() == POWER_TYPE_RAGE && pVictim->CombatStatus.IsInCombat() )
	{
		float val;
		float level = (float)getLevel();

		// Conversion Value
		float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;

		val = 2.5f * dmg.full_damage / c;
		val *= 10;

		//DEBUG_LOG( "Rd(%i) d(%i) c(%f) rage = %f", realdamage, dmg.full_damage, c, val );

		pVictim->ModUnsigned32Value( UNIT_FIELD_POWER2, (int32)val );
		if( pVictim->GetUInt32Value( UNIT_FIELD_POWER2) > 1000 )
			pVictim->ModUnsigned32Value( UNIT_FIELD_POWER2, 1000 - pVictim->GetUInt32Value( UNIT_FIELD_POWER2 ) );

	}
		
	RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);
//--------------------------extra strikes processing----------------------------------------
	if(!m_extraAttackCounter)
	{
		int32 extra_attacks = m_extraattacks;
		m_extraAttackCounter = true;
		m_extraattacks = 0;

		while(extra_attacks > 0)
		{
			extra_attacks--;
			Strike( pVictim, weapon_damage_type, NULL, 0, 0, 0, false, false);
		}

		m_extraAttackCounter = false;
	}

	if(m_extrastriketargets)
	{
		int32 m_extra = m_extrastriketargets;
		int32 m_temp = m_extrastriketargets;
		m_extrastriketargets = 0;

		for(set<Object*>::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end() && m_extra; ++itr)
		{
			if(m_extra == 0)
				break;
			if (!(*itr) || (*itr) == pVictim || !(*itr)->IsUnit())
				continue;


			if(CalcDistance(*itr) < 10.0f && isAttackable(this, (*itr)) && (*itr)->isInFront(this) && !((Unit*)(*itr))->IsPacified())
			{
				Strike( static_cast< Unit* >( *itr ), weapon_damage_type, ability, add_damage, pct_dmg_mod, exclusive_damage, false ,false );
				--m_extra;
			}
		}
		m_extrastriketargets = m_temp;
	}
}	


void Unit::smsg_AttackStop(Unit* pVictim)
{
	if(!pVictim)
		return;

	WorldPacket data(SMSG_ATTACKSTOP, 24);
	if(m_objectTypeId==TYPEID_PLAYER)
	{
		data << pVictim->GetNewGUID();
		data << uint8(0);
		data << uint32(0);
		static_cast< Player* >( this )->GetSession()->SendPacket( &data );
		data.clear();
	}

	data << GetNewGUID();
	data << pVictim->GetNewGUID();
	data << uint32(0);
	SendMessageToSet(&data, true );

	// try to remove attack target
	CombatStatus.RemoveAttackTarget(pVictim);
}

void Unit::smsg_AttackStop(uint64 victimGuid)
{
	WorldPacket data(20);
	data.Initialize( SMSG_ATTACKSTOP );
	data << GetNewGUID();
	FastGUIDPack(data, victimGuid);
	data << uint32( 0 );
	SendMessageToSet(&data, IsPlayer());
}

void Unit::smsg_AttackStart(Unit* pVictim)
{
	if(GetTypeId() != TYPEID_PLAYER) 
		return;

	Player* pThis = static_cast< Player* >( this );

	// Prevent user from ignoring attack speed and stopping and start combat really really fast
	/*if(!isAttackReady())
		setAttackTimer(uint32(0));
	else if(!canReachWithAttack(pVictim))
	{
		setAttackTimer(uint32(500));
		//pThis->GetSession()->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
	}
	else if(!isInFront(pVictim))
	{
		setAttackTimer(uint32(500));
		//pThis->GetSession()->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
	}*/

	// Send out ATTACKSTART
	WorldPacket data(SMSG_ATTACKSTART, 16);
	data << GetGUID();
	data << pVictim->GetGUID();
	SendMessageToSet(&data, true);
	DEBUG_LOG( "WORLD: Sent SMSG_ATTACKSTART" );

	// FLAGS changed so other players see attack animation
	//	addUnitFlag(UNIT_FLAG_COMBAT);
	//	setUpdateMaskBit(UNIT_FIELD_FLAGS );
	if(pThis->cannibalize)
	{
		sEventMgr.RemoveEvents(pThis, EVENT_CANNIBALIZE);
		pThis->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
		pThis->cannibalize = false;
	}
}

void Unit::AddAura(Aura *aur, Aura *pParentAura)
{
	/*if( aur->GetSpellProto()->School && SchoolImmunityList[aur->GetSpellProto()->School] )
	{
		delete aur;
		return;
	}*/

	uint32 x;
	uint8 databuffer[100];
	StackPacket data(SMSG_SET_EXTRA_AURA_INFO, databuffer, 100);

    if( !aur->IsPassive() )
	{
		//uint32 aurName = aur->GetSpellProto()->Name;
		//uint32 aurRank = aur->GetSpellProto()->Rank;
		uint32 maxStack = aur->GetSpellProto()->maxstack;
		if( aur->GetSpellProto()->procCharges > 0 )
			maxStack=aur->GetSpellProto()->procCharges;
		if( IsPlayer() && static_cast< Player* >( this )->stack_cheat )
			maxStack = 999;

		SpellEntry * info = aur->GetSpellProto();
		//uint32 flag3 = aur->GetSpellProto()->Flags3;

		AuraCheckResponse acr;
		bool deleteAur = false;

		int32 aura_update_id = -1;
		uint32 aura_update_dur = 0;

		//check if we already have this aura by this caster -> update duration
		// Nasty check for Blood Fury debuff (spell system based on namehashes is bs anyways)
		if( !info->always_apply )
		{
			uint32 f = 0;
			for( x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x )
			{
				if( m_auras[x] != NULL && m_auras[x] != pParentAura && !m_auras[x]->IsPassive() )
				{
					if(	m_auras[x]->GetSpellProto()->Id != aur->GetSpellId() && 
						( aur->pSpellId != m_auras[x]->GetSpellProto()->Id ) //if this is a proc spell then it should not remove it's mother : test with combustion later
						)
					{
						// Check for auras by specific type.
						// Check for auras with the same name and a different rank.
						
						if(info->buffType > 0 && m_auras[x]->GetSpellProto()->buffType & info->buffType && maxStack == 0)
							deleteAur = HasAurasOfBuffType(info->buffType, aur->m_casterGuid,0);
						else
						{
							acr = AuraCheck(info->NameHash, info->RankNumber, m_auras[x],aur->GetCaster());
							if(acr.Error == AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT)
								deleteAur = true;
							else if(acr.Error == AURA_CHECK_RESULT_LOWER_BUFF_PRESENT)
							{
								// remove the lower aura
								Aura *tmpAura = m_auras[x];
								m_auras[x]->Remove();
								//m_auras[x] = NULL;
								ASSERT(m_auras[x] != tmpAura);

								// no more checks on bad ptr
								continue;
							}
						}					   
					}
					else if( (m_auras[x]->GetSpellId() == aur->GetSpellId() || m_auras[x]->GetSpellProto()->NameHash == aur->GetSpellProto()->NameHash))
					{
						// only allow the aura to stack if its not unique, but allow the same caster to stack it.
						if( m_auras[x]->m_casterGuid != aur->m_casterGuid )
						{
							if( aur->GetSpellProto()->Unique  )
							{
								// Then we shouldn't even add this aura.
								if( aur->GetSpellId() == m_auras[x]->GetSpellId() && aur->GetSpellProto()->Rank < m_auras[x]->GetSpellProto()->Rank )
									return;

								deleteAur = true;
								break;
							}

							// else try to stack
						}

						f++;
						//if(maxStack > 1)
						{
							//update duration,the same aura (update the whole stack whenever we cast a new one)
							m_auras[x]->SetDuration(aur->GetDuration());
							sEventMgr.ModifyEventTimeLeft(m_auras[x], EVENT_AURA_REMOVE, aur->GetDuration());

							if( aura_update_id == -1 )
							{
								aura_update_id = (int32)m_auras[x]->m_visualSlot;
								aura_update_dur = m_auras[x]->GetDuration();
							}
						}
						if(maxStack <= f)
						{
							deleteAur = true;
							break;
						}
					}
				}
			}
		}

		if( aura_update_id >= 0)
		{
			if(this->IsPlayer())
			{
				data.Initialize(SMSG_UPDATE_AURA_DURATION);
				data << uint8(aura_update_id) << aura_update_dur;
				TO_PLAYER(this)->GetSession()->SendPacket(&data);
			}

			data.Initialize(SMSG_SET_EXTRA_AURA_INFO_NEED_UPDATE);
			data << GetNewGUID() << uint8(aura_update_id) << aur->GetSpellId() << aura_update_dur << aura_update_dur;
			SendMessageToSet(&data,false);
		}

		if(deleteAur)
		{
			sEventMgr.RemoveEvents(aur);
			delete aur;
			return;
		}
	}
			
	////////////////////////////////////////////////////////
	if(aur->GetSpellProto()->SpellGroupType && m_objectTypeId == TYPEID_PLAYER)
	{
		int32 speedmod=0;
		SM_FIValue(SM_FSpeedMod,&speedmod,aur->GetSpellProto()->SpellGroupType);
		if(speedmod)
		{
			m_speedModifier += speedmod;
			UpdateSpeed();
		}
	}
	////////////////////////////////////////////////////////

	/*if( aur->m_auraSlot != 0xffffffff )
	{
		ASSERT(m_auras[aur->m_auraSlot] == NULL);
		m_auras[aur->m_auraSlot] = NULL;
	}*/
	
	aur->m_auraSlot=255;
	aur->ApplyModifiers(true);
	
	if(!aur->IsPassive())
	{	
		aur->AddAuraVisual();
		if(aur->m_auraSlot==255)
		{
			//add to invisible slot
			for(x=MAX_AURAS;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
			{
				if(!m_auras[x])
				{
					m_auras[x]=aur;
					aur->m_auraSlot=x;
					break;
				}
			}
			if(aur->m_auraSlot == 255)
			{
				DEBUG_LOG("Aura error in active aura. ");
				// for next loop.. lets kill the fucker
				aur->Remove();
				return;
			} 

			// add visual
			AddAuraVisual(aur->GetSpellId(), 1, aur->IsPositive());
		}
		else
		{
			ASSERT(m_auras[aur->m_auraSlot] == NULL);
			m_auras[aur->m_auraSlot]=aur;
		}

		// aura cast log
		data.Initialize(SMSG_AURACASTLOG);
		data << aur->m_casterGuid;
		data << GetGUID();
		data << aur->GetSpellId();
		data << uint64(0);
		SendMessageToSet(&data, true);
	}
	else
	{
		for(x=MAX_AURAS;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		{
			if(!m_auras[x])
			{
				m_auras[x]=aur;
				aur->m_auraSlot=x;
				break;
			}
		}

		if(aur->m_auraSlot==255)
		{
			DEBUG_LOG("Aura error in passive aura. removing. SpellId: %u", aur->GetSpellProto()->Id);
			// for next loop.. lets kill the fucker
			aur->Remove();
			return;
		}
	}

	// We add 500ms here to allow for the last tick in DoT spells. This is a dirty hack, but at least it doesn't crash like my other method.
	// - Burlex
	if(aur->GetDuration() > 0)
	{
		uint32 addTime = 500;
		for(uint32 spx = 0; spx < 3; spx++)
			if( aur->GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_STUN ||
				aur->GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_FEAR ||
				aur->GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_ROOT ||
				aur->GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_CHARM )
				addTime = 50;

		sEventMgr.AddEvent(aur, &Aura::Remove, EVENT_AURA_REMOVE, aur->GetDuration() + addTime, 1,
			EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
	}

	aur->RelocateEvents();

	// Reaction from enemy AI
	if(!aur->IsPositive() /*&& aur->IsCombatStateAffecting() && */)
	{
		Unit * pCaster = aur->GetUnitCaster();
		if(pCaster && isAlive())
		{
			pCaster->CombatStatus.OnDamageDealt(this, 1);

			if(m_objectTypeId == TYPEID_UNIT)
				m_aiInterface->AttackReaction(pCaster, 1, aur->GetSpellId());
		}
	}
}

bool Unit::RemoveAura(Aura *aur)
{
	aur->Remove();
	return true;
}

bool Unit::RemoveAura(uint32 spellId)
{//this can be speed up, if we know passive \pos neg
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemovePositiveAura(uint32 spellId)
{
	for(uint32 x=0;x<=MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveNegativeAura(uint32 spellId)
{
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuras(uint32 * SpellIds)
{
	if(!SpellIds || *SpellIds == 0)
		return false;

	uint32 x=0,y;
	bool res = false;
	for(;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			for(y=0;SpellIds[y] != 0;++y)
			{
				if(m_auras[x]->GetSpellId()==SpellIds[y])
				{
					m_auras[x]->Remove();
					res = true;
				}
			}
		}
	}
	return res;
}

bool Unit::RemoveAura(uint32 spellId, uint64 guid)
{   
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId && m_auras[x]->m_casterGuid == guid)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuraByNameHash(uint32 namehash)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuraPosByNameHash(uint32 namehash)
{
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuraNegByNameHash(uint32 namehash)
{
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAllAuras(uint32 spellId, uint64 guid)
{   
	bool res = false;
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				if (!guid || m_auras[x]->m_casterGuid == guid)
				{
					m_auras[x]->Remove();
					res = true;
				}
			}
		}
	}
	return res;
}

bool Unit::RemoveAllAuraByNameHash(uint32 namehash)
{
	bool res = false;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->Remove();
				res=true;
			}
		}
	}
	return res;
}

bool Unit::RemoveAllPosAuraByNameHash(uint32 namehash)
{
	bool res = false;
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->Remove();
				res=true;
			}
		}
	}
	return res;
}

bool Unit::RemoveAllNegAuraByNameHash(uint32 namehash)
{
	bool res = false;
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->Remove();
				res=true;
			}
		}
	}
	return res;
}

void Unit::RemoveNegativeAuras()
{
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
            if(m_auras[x]->GetSpellProto()->Flags4 & CAN_PERSIST_AND_CASTED_WHILE_DEAD)
                continue;
            else
            {
			    m_auras[x]->Remove();
            }
		}
	}
}

void Unit::RemoveAllAuras()
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			m_auras[x]->Remove();
		}
	}
}

//ex:to remove morph spells
void Unit::RemoveAllAuraType(uint32 auratype)
{
    for(uint32 x=0;x<MAX_AURAS;x++)
    {
		if(m_auras[x])
		{
			SpellEntry *proto=m_auras[x]->GetSpellProto();
			if(proto->EffectApplyAuraName[0]==auratype || proto->EffectApplyAuraName[1]==auratype || proto->EffectApplyAuraName[2]==auratype)
				RemoveAura(m_auras[x]->GetSpellId());//remove all morph auras containig to this spell (like wolf motph also gives speed)
		}
    }
}

bool Unit::SetAurDuration(uint32 spellId,Unit* caster,uint32 duration)
{
	Aura*aur=FindAura(spellId,caster->GetGUID());
	if(!aur)
		return false;
	aur->SetDuration(duration);
	sEventMgr.ModifyEventTimeLeft(aur, EVENT_AURA_REMOVE, duration);
			
	if(this->IsPlayer())
	{
		WorldPacket data(5);
		data.SetOpcode(SMSG_UPDATE_AURA_DURATION);
		data << (uint8)(aur)->GetAuraSlot() << duration;
		static_cast< Player* >( this )->GetSession()->SendPacket( &data );
	}

	WorldPacket data(SMSG_SET_EXTRA_AURA_INFO_NEED_UPDATE,21);
	data << GetNewGUID() << aur->m_visualSlot << uint32(spellId) << uint32(duration) << uint32(duration);
	SendMessageToSet(&data,false);
			
	return true;
}

bool Unit::SetAurDuration(uint32 spellId,uint32 duration)
{
	Aura*aur=FindAura(spellId);

	if(!aur) 
		return false;
	

	aur->SetDuration(duration);
	sEventMgr.ModifyEventTimeLeft(aur, EVENT_AURA_REMOVE, duration);
		
	if(this->IsPlayer())
	{
		WorldPacket data(5);
		data.SetOpcode(SMSG_UPDATE_AURA_DURATION);
		data << (uint8)(aur)->GetAuraSlot() << duration;
		static_cast< Player* >( this )->GetSession()->SendPacket( &data );
	}
	WorldPacket data(SMSG_SET_EXTRA_AURA_INFO_NEED_UPDATE,21);
	data << GetNewGUID() << aur->m_visualSlot << uint32(spellId) << uint32(duration) << uint32(duration);
	SendMessageToSet(&data,false);


	return true;
}


Aura* Unit::FindAuraPosByNameHash(uint32 namehash)
{
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

Aura* Unit::FindAura(uint32 spellId)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

Aura* Unit::FindAura(uint32 spellId, uint64 guid)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId() == spellId && m_auras[x]->m_casterGuid == guid)
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

void Unit::_UpdateSpells( uint32 time )
{
	if(m_currentSpell != NULL)
		m_currentSpell->update(time);
}

void Unit::castSpell( Spell * pSpell )
{
	m_currentSpell = pSpell;
	pLastSpell = pSpell->m_spellInfo;
}

int32 Unit::GetSpellDmgBonus(Unit *pVictim, SpellEntry *spellInfo,int32 base_dmg, bool isdot)
{
	int32 plus_damage = 0;
	Unit* caster = this;
	uint32 school = spellInfo->School;
	int durmod = 0;

	if( spellInfo->c_is_flags & SPELL_FLAG_IS_NOT_USING_DMG_BONUS )
		return 0;

	if( caster->IsPlayer() )
	{
		for(uint32 a = 0; a < 6; a++)
			plus_damage += float2int32(static_cast< Player* >(caster)->SpellDmgDoneByAttribute[a][school] * float(caster->GetUInt32Value(UNIT_FIELD_STAT0 + a)));
	}
//------------------------------by school---------------------------------------------------
	plus_damage += caster->GetDamageDoneMod(school);
	plus_damage += pVictim->DamageTakenMod[school];
//------------------------------by victim type----------------------------------------------
	if(((Creature*)pVictim)->GetCreatureName() && caster->IsPlayer()&& !pVictim->IsPlayer())
		plus_damage += static_cast< Player* >(caster)->IncreaseDamageByType[((Creature*)pVictim)->GetCreatureName()->Type];
//==========================================================================================
//==============================+Spell Damage Bonus Modifications===========================
//==========================================================================================
//------------------------------by cast duration--------------------------------------------
	float dmgdoneaffectperc = 1.0f;
	if( spellInfo->Dspell_coef_override >= 0 && !isdot )
		plus_damage = float2int32( float( plus_damage ) * spellInfo->Dspell_coef_override );
	else if( spellInfo->OTspell_coef_override >= 0 && isdot )
		plus_damage = float2int32( float( plus_damage ) * spellInfo->OTspell_coef_override );
	else
	{
		//Bonus to DD part
		if( spellInfo->fixed_dddhcoef >= 0 && !isdot )
			plus_damage = float2int32( float( plus_damage ) * spellInfo->fixed_dddhcoef );
		//Bonus to DoT part
		else if( spellInfo->fixed_hotdotcoef >= 0 && isdot )
		{
			plus_damage = float2int32( float( plus_damage ) * spellInfo->fixed_hotdotcoef );
			if( caster->IsPlayer() )
			{
				SM_FIValue(caster->SM_FDur, &durmod, spellInfo->SpellGroupType);
				plus_damage += float2int32( float( plus_damage * durmod ) / 15000.0f );
			}
		}
		//In case we dont fit in previous cases do old thing
		else
		{
			plus_damage = float2int32( float( plus_damage ) * spellInfo->casttime_coef );
			float td = float( GetDuration( dbcSpellDuration.LookupEntry( spellInfo->DurationIndex ) ));
			if( spellInfo->NameHash == SPELL_HASH_MOONFIRE || spellInfo->NameHash == SPELL_HASH_IMMOLATE || spellInfo->NameHash == SPELL_HASH_ICE_LANCE || spellInfo->NameHash == SPELL_HASH_PYROBLAST )
				plus_damage = float2int32( float( plus_damage ) * float( 1.0f - ( ( td / 15000.0f ) / ( ( td / 15000.0f ) + dmgdoneaffectperc ) ) ) );
		}
	}

	//------------------------------by downranking----------------------------------------------
	//DOT-DD (Moonfire-Immolate-IceLance-Pyroblast)(Hack Fix)

	float downrank1;
	float downrank2;
	if(spellInfo->baseLevel > 0 && spellInfo->maxLevel > 0)
	{
		downrank1 = 1.0f;
		if(spellInfo->baseLevel < 20)
		    downrank1 = 1.0f - (20.0f - float (spellInfo->baseLevel) ) * 0.0375f;
		downrank2 = ( float(spellInfo->maxLevel + 5.0f) / float(static_cast< Player* >(caster)->getLevel()) );
		if(downrank2 >= 1 || downrank2 < 0)
		        downrank2 = 1.0f;
		dmgdoneaffectperc *= downrank1 * downrank2;
	}
//==========================================================================================
//==============================Bonus Adding To Main Damage=================================
//==========================================================================================
	int32 bonus_damage = float2int32(plus_damage * dmgdoneaffectperc);

	//bonus_damage +=pVictim->DamageTakenMod[school]; Bad copy-past i guess :P
	int penalty_pct=0;
	int dmg_bonus_pct=0;

	if(spellInfo->SpellGroupType)
	{
		SM_FIValue(caster->SM_FPenalty, &bonus_damage, spellInfo->SpellGroupType);
		SM_FIValue(caster->SM_PPenalty, &penalty_pct, spellInfo->SpellGroupType);
		bonus_damage += bonus_damage*penalty_pct/100;
		SM_FIValue(caster->SM_FDamageBonus, &bonus_damage, spellInfo->SpellGroupType);
		
		// Molten Fury - Should be done in SpellAuraOverrideClassScripts, but heh xD	
		if( pVictim->GetHealthPct() < 20 && IsPlayer())
			dmg_bonus_pct += (int) TO_PLAYER(this)->m_moltenFuryDamageIncreasePct;

		SM_FIValue(caster->SM_PDamageBonus,&dmg_bonus_pct,spellInfo->SpellGroupType);
		bonus_damage += base_dmg*dmg_bonus_pct/100;
	}

//------------------------------by school----------------------------------------------
	float summaryPCTmod = caster->GetDamageDonePctMod(school)-1; //value is initialized with 1
	summaryPCTmod += pVictim->DamageTakenPctMod[school]-1;//value is initialized with 1
	summaryPCTmod += caster->DamageDoneModPCT[school];	// BURLEX FIXME
	summaryPCTmod += pVictim->ModDamageTakenByMechPCT[spellInfo->MechanicsType];
	
	int32 res = (int32)((base_dmg+bonus_damage)*summaryPCTmod + bonus_damage); // 1.x*(base_dmg+bonus_damage) == 1.0*base_dmg + 1.0*bonus_damage + 0.x*(base_dmg+bonus_damage) -> we add the returned value to base damage so we do not add it here (function returns bonus only)
	//ASSERT(res < 60000);
	return res;
}

void Unit::InterruptSpell()
{
	if(m_currentSpell)
		m_currentSpell->cancel();
}

void Unit::DeMorph()
{
	// hope it solves it :)
	uint32 displayid = this->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID);
	this->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayid);
}

void Unit::Emote(EmoteType emote)
{
	if(emote == 0)
		return;

	WorldPacket data(SMSG_EMOTE, 12);
	data << uint32(emote);
	data << this->GetGUID();
	SendMessageToSet (&data, true);
}

void Unit::SendChatMessageToPlayer(uint8 type, uint32 lang, const char *msg, Player *plr)
{
	size_t UnitNameLength = 0, MessageLength = 0;
	CreatureInfo *ci = (m_objectTypeId == TYPEID_UNIT) ? ((Creature*)this)->creature_info : NULL;

	if(ci == NULL || plr == NULL)
		return;

	UnitNameLength = strlen((char*)ci->Name) + 1;
	MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << ci->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	plr->GetSession()->SendPacket(&data);
}

void Unit::SendChatMessageAlternateEntry(uint32 entry, uint8 type, uint32 lang, const char * msg)
{
	size_t UnitNameLength = 0, MessageLength = 0;
	CreatureInfo *ci;

	ci = CreatureNameStorage.LookupEntry(entry);
	if(!ci)
		return;

	UnitNameLength = strlen((char*)ci->Name) + 1;
	MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << ci->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	SendMessageToSet(&data, true);
}

void Unit::SendChatMessage(uint8 type, uint32 lang, const char *msg)
{
	size_t UnitNameLength = 0, MessageLength = 0;
	CreatureInfo *ci = (m_objectTypeId == TYPEID_UNIT) ? ((Creature*)this)->creature_info : NULL;

	if(ci == NULL)
		return;

	UnitNameLength = strlen((char*)ci->Name) + 1;
	MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << ci->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	SendMessageToSet(&data, true);
}

void Unit::WipeHateList()
{ 
	GetAIInterface()->WipeHateList(); 
}
void Unit::ClearHateList()
{
	GetAIInterface()->ClearHateList();
}

void Unit::WipeTargetList() 
{ 
	GetAIInterface()->WipeTargetList(); 
}

void Unit::AddInRangeObject(Object* pObj)
{
	if((pObj->GetTypeId() == TYPEID_UNIT) || (pObj->GetTypeId() == TYPEID_PLAYER))
	{
		if( isHostile( this, (Unit*)pObj ) )
			m_oppFactsInRange.insert(pObj);
	}

	Object::AddInRangeObject(pObj);
}//427

void Unit::OnRemoveInRangeObject(Object* pObj)
{
	m_oppFactsInRange.erase(pObj);

	if(pObj->GetTypeId() == TYPEID_UNIT || pObj->GetTypeId() == TYPEID_PLAYER)
	{
		/*if(m_useAI)*/

		Unit *pUnit = static_cast<Unit*>(pObj);
		GetAIInterface()->CheckTarget(pUnit);

		if(GetUInt64Value(UNIT_FIELD_CHARM) == pObj->GetGUID())
			if(m_currentSpell)
				m_currentSpell->cancel();

        Object::OnRemoveInRangeObject(pObj);
        if(critterPet == pObj)
		{
			critterPet->SafeDelete();
			critterPet = 0;
		}
	}
    else
    {
        Object::OnRemoveInRangeObject(pObj);
    }
}

void Unit::ClearInRangeSet()
{
	Object::ClearInRangeSet();
}

//Events
void Unit::EventAddEmote(EmoteType emote, uint32 time)
{
	m_oldEmote = GetUInt32Value(UNIT_NPC_EMOTESTATE);
	SetUInt32Value(UNIT_NPC_EMOTESTATE,emote);
	sEventMgr.AddEvent(this, &Creature::EmoteExpire, EVENT_UNIT_EMOTE, time, 1,0);
}

void Unit::EmoteExpire()
{
	SetUInt32Value(UNIT_NPC_EMOTESTATE, m_oldEmote);
	sEventMgr.RemoveEvents(this, EVENT_UNIT_EMOTE);
}


uint32 Unit::GetResistance(uint32 type)
{
	return GetUInt32Value(UNIT_FIELD_RESISTANCES+type);
}

void Unit::MoveToWaypoint(uint32 wp_id)
{
	if(this->m_useAI && this->GetAIInterface() != NULL)
	{
		AIInterface *ai = GetAIInterface();
		WayPoint *wp = ai->getWayPoint(wp_id);
		if(!wp)
		{
			sLog.outString("WARNING: Invalid WP specified in MoveToWaypoint.");
			return;
		}

		ai->m_currentWaypoint = wp_id;
		if(wp->flags!=0)
			ai->m_moveRun = true;
		ai->MoveTo(wp->x, wp->y, wp->z, 0);
	}
}

int32 Unit::GetDamageDoneMod(uint32 school)
{
	if( this->IsPlayer() )
	   return (int32)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school ) - (int32)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + school );
	else
	   return static_cast< Creature* >( this )->ModDamageDone[school];
}
	
float Unit::GetDamageDonePctMod(uint32 school)
{
   if(this->IsPlayer())
	   return m_floatValues[PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+school];
	else
	   return ((Creature*)this)->ModDamageDonePct[school];
}

void Unit::CalcDamage()
{
	if( IsPlayer() )
		static_cast< Player* >( this )->CalcDamage();
	else
	{
	float r;
	float delta;
	float mult;
	
	float ap_bonus = float(GetAP())/14000.0f;

		float bonus = ap_bonus*GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
	
		delta = float(((Creature*)this)->ModDamageDone[0]);
		mult = float(((Creature*)this)->ModDamageDonePct[0]);
		r = BaseDamage[0]*mult+delta+bonus;
		// give some diversity to pet damage instead of having a 77-78 damage range (as an example)
		SetFloatValue(UNIT_FIELD_MINDAMAGE,r > 0 ? ( IsPet() ? r * 0.9f : r ) : 0 );
		r = BaseDamage[1]*mult+delta+bonus;
		SetFloatValue(UNIT_FIELD_MAXDAMAGE, r > 0 ? ( IsPet() ? r * 1.1f : r ) : 0 );
	
	//	SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,BaseRangedDamage[0]*mult+delta);
	//	SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,BaseRangedDamage[1]*mult+delta);
  
	}	
}

//returns absorbed dmg
uint32 Unit::ManaShieldAbsorb(uint32 dmg)
{
	if(!m_manashieldamt)
		return 0;
	//mana shield group->16. the only

	uint32 mana = GetUInt32Value(UNIT_FIELD_POWER1);
	int32 effectbonus = SM_PEffectBonus ? SM_PEffectBonus[16] : 0;

	int32 potential = (mana*50)/((100+effectbonus));
	if(potential>m_manashieldamt)
		potential = m_manashieldamt;

	if((int32)dmg<potential)
		potential = dmg;

	uint32 cost = (potential*(100+effectbonus))/50;

	SetUInt32Value(UNIT_FIELD_POWER1,mana-cost);
	m_manashieldamt -= potential;
	if(!m_manashieldamt)
		RemoveAura(m_manaShieldId);
	return potential;	
}

// grep: Remove any AA spells that aren't owned by this player.
//		 Otherwise, if he logs out and in and joins another group,
//		 he'll apply it to them.
/*
void Unit::RemoveAllAreaAuras()
{
	AuraList::iterator itr,it1;
	for(itr = m_auras.begin();itr!=m_auras.end();)
	{
		it1 = itr++;
		if(((*it1)->m_spellProto->Effect[0] == SPELL_EFFECT_APPLY_AREA_AURA ||
			(*it1)->m_spellProto->Effect[1] == SPELL_EFFECT_APPLY_AREA_AURA ||
			(*it1)->m_spellProto->Effect[2] == SPELL_EFFECT_APPLY_AREA_AURA) && (*it1)->GetCaster() != this)
		{
			(*it1)->Remove();
			m_auras.erase(it1);
		} 
	}
}
*/
uint32 Unit::AbsorbDamage( uint32 School, uint32* dmg )
{
	if( dmg == NULL )
		return 0;

	SchoolAbsorb::iterator i, j;
	uint32 abs = 0;
	for( i = Absorbs[School].begin(); i != Absorbs[School].end(); )
	{
		if( (int32)(*dmg) >= (*i)->amt)//remove this absorb
		{
			(*dmg) -= (*i)->amt;
			abs += (*i)->amt;
			j = i++;

			if( i != Absorbs[School].end() )
			{
				while( (*i)->spellid == (*j)->spellid )
				{
					++i;
					if( i == Absorbs[School].end() )
						break;
				}
			}
			
			this->RemoveAura((*j)->spellid); //,(*j)->caster);
			if(!*dmg)//absorbed all dmg
				break;		
		}
		else //absorb full dmg
		{
			abs += *dmg;
			(*i)->amt -= *dmg;
			*dmg=0;
			break;
		}
	}
	
	return abs;
}

bool Unit::setDetectRangeMod(uint64 guid, int32 amount)
{
	int next_free_slot = -1;
	for(int i = 0; i < 5; i++)
	{
		if(m_detectRangeGUID[i] == 0 && next_free_slot == -1)
		{
			next_free_slot = i;
		}
		if(m_detectRangeGUID[i] == guid)
		{
			m_detectRangeMOD[i] = amount;
			return true;
		}
	}
	if(next_free_slot != -1)
	{
		m_detectRangeGUID[next_free_slot] = guid;
		m_detectRangeMOD[next_free_slot] = amount;
		return true;
	}
	return false;
}

void Unit::unsetDetectRangeMod(uint64 guid)
{
	for(int i = 0; i < 5; i++)
	{
		if(m_detectRangeGUID[i] == guid)
		{
			m_detectRangeGUID[i] = 0;
			m_detectRangeMOD[i] = 0;
		}
	}
}

int32 Unit::getDetectRangeMod(uint64 guid)
{
	for(int i = 0; i < 5; i++)
	{
		if(m_detectRangeGUID[i] == guid)
		{
			return m_detectRangeMOD[i];
		}
	}
	return 0;
}

void Unit::SetStandState(uint8 standstate)
{
	SetByte( UNIT_FIELD_BYTES_1, 0, standstate );
	if( standstate == STANDSTATE_STAND )//standup
		RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_STAND_UP);

	if( m_objectTypeId == TYPEID_PLAYER )
		static_cast< Player* >( this )->GetSession()->OutPacket( SMSG_STANDSTATE_UPDATE, 1, &standstate );
}

void Unit::RemoveAurasByInterruptFlag(uint32 flag)
{
	Aura * a;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		a = m_auras[x];
		if(a == NULL)
			continue;
		
		//some spells do not get removed all the time only at specific intervals
		if((a->m_spellProto->AuraInterruptFlags & flag) && !(a->m_spellProto->procFlags & PROC_REMOVEONUSE))
		{
			a->Remove();
			//m_auras[x] = NULL;
		}
	}
}

bool Unit::HasAuraVisual(uint32 visualid)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	if(m_auras[x] && m_auras[x]->GetSpellProto()->SpellVisual==visualid)
	{
		return true;
	}

	return false;
}

bool Unit::HasAura(uint32 spellid)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] && m_auras[x]->GetSpellId()== spellid)
		{
			return true;
		}

		return false;
}


void Unit::DropAurasOnDeath()
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
    {
        if(m_auras[x] != NULL && !m_auras[x]->IsPassive())
        {
            if(m_auras[x] && m_auras[x]->GetSpellProto()->Flags4 & CAN_PERSIST_AND_CASTED_WHILE_DEAD)
                continue;
            else
	        {
		        m_auras[x]->Remove();
	        }
        }
    }
}

void Unit::UpdateSpeed()
{
	if(GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) == 0)
	{
		if(IsPlayer())
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_speedModifier)/100.0f);
		else
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_speedModifier)/100.0f);
	}
	else
	{
		if(IsPlayer())
		{
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_mountedspeedModifier)/100.0f);
			m_runSpeed += (m_speedModifier<0) ? (m_base_runSpeed*((float)m_speedModifier)/100.0f) : 0;
		}
		else
		{
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_mountedspeedModifier)/100.0f);
			m_runSpeed += (m_speedModifier<0) ? (m_base_runSpeed*((float)m_speedModifier)/100.0f) : 0;
		}
	}

	

	m_flySpeed = PLAYER_NORMAL_FLIGHT_SPEED*(1.0f + ((float)m_flyspeedModifier)/100.0f);

	// Limit speed due to effects such as http://www.wowhead.com/?spell=31896 [Judgement of Justice]
	if( m_maxSpeed && m_runSpeed > m_maxSpeed )
	{
		m_runSpeed = m_maxSpeed;
	}

	if(IsPlayer())
	{
		if(((Player*)this)->m_changingMaps)
			static_cast< Player* >( this )->resend_speed = true;
		else
		{
			static_cast< Player* >( this )->SetPlayerSpeed(RUN, m_runSpeed);
			static_cast< Player* >( this )->SetPlayerSpeed(FLY, m_flySpeed);
		}
	}
}

bool Unit::HasActiveAura(uint32 spellid)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	if(m_auras[x] && m_auras[x]->GetSpellId()==spellid)
	{
		return true;
	}

	return false;
}

bool Unit::HasActiveAura(uint32 spellid,uint64 guid)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	if(m_auras[x] && m_auras[x]->GetSpellId()==spellid && m_auras[x]->m_casterGuid==guid)
	{
		return true;
	}

	return false;
}

void Unit::EventSummonPetExpire()
{
	if(summonPet)
	{
		if(summonPet->GetEntry() == 7915)//Goblin Bomb
		{
			SpellEntry *spInfo = dbcSpell.LookupEntry(13259);
			if(!spInfo)
				return;

			Spell*sp=new Spell(summonPet,spInfo,true,NULL);
			SpellCastTargets tgt;
			tgt.m_unitTarget=summonPet->GetGUID();
			sp->prepare(&tgt);
		}
		else
		{
			summonPet->RemoveFromWorld(false, true);
			delete summonPet;
			summonPet = NULL;
		}
	}
	sEventMgr.RemoveEvents(this, EVENT_SUMMON_PET_EXPIRE);
}

void Unit::CastSpell(Unit* Target, SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	Spell *newSpell = new Spell(this, Sp, triggered, 0);
	SpellCastTargets targets(0);
	if(Target)
	{
		targets.m_unitTarget |= TARGET_FLAG_UNIT;
		targets.m_unitTarget = Target->GetGUID();
	}
	else
	{
		newSpell->GenerateTargets(&targets);
	}
	newSpell->prepare(&targets);
}

void Unit::CastSpell(Unit* Target, uint32 SpellID, bool triggered)
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(Target, ent, triggered);
}

void Unit::CastSpell(uint64 targetGuid, SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	SpellCastTargets targets(targetGuid);
	Spell *newSpell = new Spell(this, Sp, triggered, 0);
	newSpell->prepare(&targets);
}

void Unit::CastSpell(uint64 targetGuid, uint32 SpellID, bool triggered)
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(targetGuid, ent, triggered);
}
void Unit::CastSpellAoF(float x,float y,float z,SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	SpellCastTargets targets;
	targets.m_destX = x;
	targets.m_destY = y;
	targets.m_destZ = z;
	targets.m_targetMask=TARGET_FLAG_DEST_LOCATION;
	Spell *newSpell = new Spell(this, Sp, triggered, 0);
	newSpell->prepare(&targets);
}

void Unit::PlaySpellVisual(uint64 target, uint32 spellVisual)
{
	WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 12);
	data << target << spellVisual;
	SendMessageToSet(&data, true);
}

void Unit::Root()
{
	this->m_special_state |= UNIT_STATE_ROOT;

	if(m_objectTypeId == TYPEID_PLAYER)
	{
		static_cast< Player* >( this )->SetMovement(MOVE_ROOT, 1);
	}
	else
	{
		m_aiInterface->m_canMove = false;
		m_aiInterface->StopMovement(1);
	}
}

void Unit::Unroot()
{
	this->m_special_state &= ~UNIT_STATE_ROOT;

	if(m_objectTypeId == TYPEID_PLAYER)
	{
		static_cast< Player* >( this )->SetMovement(MOVE_UNROOT, 5);
	}
	else
	{
		m_aiInterface->m_canMove = true;
	}
}

void Unit::RemoveAurasByBuffType(uint32 buff_type, const uint64 &guid, uint32 skip)
{
	uint64 sguid = buff_type >= SPELL_TYPE_BLESSING ? guid : 0;

	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->buffType & buff_type && m_auras[x]->GetSpellId()!=skip)
			if(!sguid || (sguid && m_auras[x]->m_casterGuid == sguid))
				m_auras[x]->Remove();
	}
}

void Unit::RemoveAurasByBuffIndexType(uint32 buff_index_type, const uint64 &guid)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->buffIndexType == buff_index_type)
			if(!guid || (guid && m_auras[x]->m_casterGuid == guid))
				m_auras[x]->Remove();
	}
}

bool Unit::HasAurasOfBuffType(uint32 buff_type, const uint64 &guid,uint32 skip)
{
	uint64 sguid = (buff_type == SPELL_TYPE_BLESSING || buff_type == SPELL_TYPE_WARRIOR_SHOUT) ? guid : 0;

	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->buffType & buff_type && m_auras[x]->GetSpellId()!=skip)
			if(!sguid || (m_auras[x]->m_casterGuid == sguid))
				return true;
	}

	return false;
}

AuraCheckResponse Unit::AuraCheck(uint32 name_hash, uint32 rank, Object *caster)
{
	AuraCheckResponse resp;

	// no error for now
	resp.Error = AURA_CHECK_RESULT_NONE;
	resp.Misc  = 0;

	// look for spells with same namehash
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
//		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash && m_auras[x]->GetCaster()==caster)
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash)
		{
			// we've got an aura with the same name as the one we're trying to apply
			resp.Misc = m_auras[x]->GetSpellProto()->Id;

			// compare the rank to our applying spell
			if(m_auras[x]->GetSpellProto()->RankNumber > rank)
				resp.Error = AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT;
			else
				resp.Error = AURA_CHECK_RESULT_LOWER_BUFF_PRESENT;

			// we found something, save some loops and exit
			break;
		}
	}

	// return it back to our caller
	return resp;
}

AuraCheckResponse Unit::AuraCheck(uint32 name_hash, uint32 rank, Aura* aur, Object *caster)
{
	AuraCheckResponse resp;

	// no error for now
	resp.Error = AURA_CHECK_RESULT_NONE;
	resp.Misc  = 0;

	// look for spells with same namehash
//	if(aur->GetSpellProto()->NameHash == name_hash && aur->GetCaster()==caster)
	if(aur->GetSpellProto()->NameHash == name_hash)
	{
		// we've got an aura with the same name as the one we're trying to apply
		resp.Misc = aur->GetSpellProto()->Id;

		// compare the rank to our applying spell
		if(aur->GetSpellProto()->RankNumber > rank)
			resp.Error = AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT;
		else
			resp.Error = AURA_CHECK_RESULT_LOWER_BUFF_PRESENT;
	}

	// return it back to our caller
	return resp;
}

void Unit::OnPushToWorld()
{
	for(uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
	{
		if(m_auras[x] != 0)
			m_auras[x]->RelocateEvents();
	}
}

void Unit::RemoveFromWorld(bool free_guid)
{
	CombatStatus.OnRemoveFromWorld();
	if(critterPet != 0)
	{
		critterPet->RemoveFromWorld(false, true);
		delete critterPet;
		critterPet = 0;
	}

	if(dynObj != 0)
		dynObj->Remove();

	for(uint32 i = 0; i < 4; ++i)
	{
		if(m_ObjectSlots[i] != 0)
		{
			GameObject * obj = m_mapMgr->GetGameObject(m_ObjectSlots[i]);
			if(obj)
				obj->ExpireAndDelete();

			m_ObjectSlots[i] = 0;
		}
	}

	Object::RemoveFromWorld(free_guid);


	for(uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
	{
		if(m_auras[x] != 0)
		{
			/*if( m_auras[x]->m_deleted )
			{
				m_auras[x] = NULL;
				continue;
			}*/

			m_auras[x]->RelocateEvents();
		}
	}
	m_aiInterface->WipeReferences();

	if(m_currentSpell != NULL)
	{
		m_currentSpell->cancel();
		m_currentSpell = NULL;
	}
}

void Unit::RemoveAurasByInterruptFlagButSkip(uint32 flag, uint32 skip)
{
	Aura * a;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		a = m_auras[x];
		if(a == 0)
			continue;

		//some spells do not get removed all the time only at specific intervals
		if((a->m_spellProto->AuraInterruptFlags & flag) && (a->m_spellProto->Id != skip) && a->m_spellProto->proc_interval==0)
		{
			//the black sheeps of sociaty
			if(a->m_spellProto->AuraInterruptFlags & AURA_INTERRUPT_ON_CAST_SPELL)
			{
				switch(a->GetSpellProto()->Id)
				{
					//priest - surge of light
					case 33151:
						{
							//our luck. it got trigered on smite..we do not remove it just yet
							if( m_currentSpell && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_SMITE )
								continue;

							//this spell gets removed only when casting smite
						    SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SMITE )
								continue;
						}break;
					case 34936:		// Backlash
						{
							SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SHADOW_BOLT && spi->NameHash != SPELL_HASH_INCINERATE )
								continue;
						}break;

					case 17941: //Shadow Trance
						{
							SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SHADOW_BOLT )
								continue;
						}break;
					case 18708: //Fel Domination
						{
							SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SUMMON_IMP &&
								spi->NameHash != SPELL_HASH_SUMMON_VOIDWALKER &&
								spi->NameHash != SPELL_HASH_SUMMON_SUCCUBUS &&
								spi->NameHash != SPELL_HASH_SUMMON_FELHUNTER &&
								spi->NameHash != SPELL_HASH_SUMMON_FELGUARD )
								continue;
						}break;
				}
			}
			a->Remove();
		}
	}
}

int Unit::HasAurasWithNameHash(uint32 name_hash)
{
	for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash)
			return m_auras[x]->m_spellProto->Id;
	}

	return 0;
}

bool Unit::HasNegativeAuraWithNameHash(uint32 name_hash)
{
	for(uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash)
			return true;
	}

	return false;
}

bool Unit::HasNegativeAura(uint32 spell_id)
{
	for(uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->Id == spell_id)
			return true;
	}

	return false;
}

bool Unit::IsPoisoned()
{
	for(uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_POISON)
			return true;
	}

	return false;
}

uint32 Unit::AddAuraVisual(uint32 spellid, uint32 count, bool positive)
{
	int32 free = -1;
	uint32 start = positive ? 0 : MAX_POSITIVE_AURAS;
	uint32 end_  = positive ? MAX_POSITIVE_AURAS : MAX_AURAS;

	for(uint32 x = start; x < end_; ++x)
	{
		if(free == -1 && m_uint32Values[UNIT_FIELD_AURA+x] == 0)
			free = x;

		if(m_uint32Values[UNIT_FIELD_AURA+x] == spellid)
		{
			// Increase count of this aura.
			ModAuraStackCount(x, count);
			return free;
		}
	}

	if(free == -1) return 0xFF;

	uint8 flagslot = (free / 4);
	uint32 value = GetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot));

	/*uint8 aurapos = (free & 7) << 2;
	uint32 setflag = AFLAG_SET;
	if(positive)
		setflag = 0xD;

	uint32 value1 = (uint32)setflag << aurapos;
	value |= value1;*/
	uint8 aurapos = (free%4)*8;
	value &= ~(0xFF<<aurapos);
	if(positive)
		value |= (0x1F<<aurapos);
	else
		value |= (0x09<<aurapos);

	SetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot), value);
	SetUInt32Value(UNIT_FIELD_AURA + free, spellid);
	ModAuraStackCount(free, 1);
	SetAuraSlotLevel(free, positive);
	
	return free;
}

void Unit::SetAuraSlotLevel(uint32 slot, bool positive)
{
	uint32 index = slot / 4;
	uint32 val = m_uint32Values[UNIT_FIELD_AURALEVELS + index];
	uint32 bit = (slot % 4) * 8;
	val &= ~(0xFF << bit);
	if(positive)
		val |= (0x46 << bit);
	else
		val |= (0x19 << bit);
	
	SetUInt32Value(UNIT_FIELD_AURALEVELS + index, val);
}

void Unit::RemoveAuraVisual(uint32 spellid, uint32 count)
{
	for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_uint32Values[UNIT_FIELD_AURA+x] == spellid)
		{
			// Decrease count of this aura.
			int test = ModAuraStackCount(x, -(int32)count);
			if(test == 0)
			{
				// Aura has been removed completely.
				uint8 flagslot = (x/4);
				uint32 value = GetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot));
				/*uint8 aurapos = (x & 7) << 2;
				uint32 value1 = ~( (uint32)0xF << aurapos );
				value &= value1;*/
				uint8 aurapos = (x%4)*8;
				value &= ~(0xFF<<aurapos);
				SetUInt32Value(UNIT_FIELD_AURAFLAGS + flagslot,value);
				SetUInt32Value(UNIT_FIELD_AURA + x, 0);
				SetAuraSlotLevel(x, false);
			}			
		}
	}
}

uint32 Unit::ModAuraStackCount(uint32 slot, int32 count)
{
	uint32 index = (slot >> 2);
	uint32 byte  = (slot % 4);
	uint32 val   = m_uint32Values[UNIT_FIELD_AURAAPPLICATIONS+index];

	// shouldn't happen
	uint32 ac;

	if(count < 0 && m_auraStackCount[slot] < abs(count))
	{
		m_auraStackCount[slot] = ac = 0;
	}
	else
	{
		m_auraStackCount[slot] += count;
		ac = (m_auraStackCount[slot] > 0) ? m_auraStackCount[slot] - 1 : 0;
	}

	val &= ~(0xFF << byte * 8);
	val |= (ac << byte * 8);

	SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS+index, val);
	return m_auraStackCount[slot];
}

void Unit::RemoveAurasOfSchool(uint32 School, bool Positive, bool Immune)
{
	for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_auras[x]  && m_auras[x]->GetSpellProto()->School == School && (!m_auras[x]->IsPositive() || Positive) && (!Immune && m_auras[x]->GetSpellProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
			m_auras[x]->Remove();
	}
}

void Unit::EnableFlight()
{
	if(m_objectTypeId != TYPEID_PLAYER || ((Player*)this)->m_changingMaps)
	{
		WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 13);
		data << GetNewGUID();
		data << uint32(2);
		SendMessageToSet(&data, true);

		if( IsPlayer() )
		{
			static_cast< Player* >( this )->m_setflycheat = true;
		}
	}
	else
	{
		WorldPacket * data = new WorldPacket(SMSG_MOVE_SET_CAN_FLY, 13);
		*data << GetNewGUID();
		*data << uint32(2);
		SendMessageToSet(data, false);
		static_cast< Player* >( this )->z_axisposition = 0.0f;
		static_cast< Player* >( this )->delayedPackets.add( data );
		static_cast< Player* >( this )->m_setflycheat = true;
	}
}

void Unit::DisableFlight()
{
	if(m_objectTypeId != TYPEID_PLAYER || ((Player*)this)->m_changingMaps)
	{
		WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 13);
		data << GetNewGUID();
		data << uint32(5);
		SendMessageToSet(&data, true);

		if( IsPlayer() )
			static_cast< Player* >( this )->m_setflycheat = false;
	}
	else
	{
		WorldPacket * data = new WorldPacket( SMSG_MOVE_UNSET_CAN_FLY, 13 );
		*data << GetNewGUID();
		*data << uint32(5);
		SendMessageToSet(data, false);
		static_cast< Player* >( this )->z_axisposition = 0.0f;
		static_cast< Player* >( this )->delayedPackets.add( data );
		static_cast< Player* >( this )->m_setflycheat = false;
	}
}

bool Unit::IsDazed()
{
	for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->MechanicsType == MECHANIC_ENSNARED)
			return true;
			for(uint32 y=0;y<3;y++)
			if(m_auras[x]->GetSpellProto()->EffectMechanic[y]==MECHANIC_ENSNARED)
				return true;
		}
	}

	return false;

}

void Unit::UpdateVisibility()
{
	ByteBuffer buf(2500);
	InRangeSet::iterator itr, it3;
	uint32 count;
	bool can_see;
	bool is_visible;
	Player *pl;
	Object * pObj;
	Player * plr;

	if( m_objectTypeId == TYPEID_PLAYER )
	{
		plr = static_cast< Player* >( this );
		for( Object::InRangeSet::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end();)
		{
			pObj = (*itr);
			++itr;

			can_see = plr->CanSee(pObj);
			is_visible = plr->GetVisibility(pObj, &it3);
			if(can_see)
			{
				if(!is_visible)
				{
					buf.clear();
					count = pObj->BuildCreateUpdateBlockForPlayer( &buf, plr );
					plr->PushCreationData(&buf, count);
					plr->AddVisibleObject(pObj);
				}
			}
			else
			{
				if(is_visible)
				{
					pObj->DestroyForPlayer(plr);
					plr->RemoveVisibleObject(it3);
				}
			}

			if( pObj->GetTypeId() == TYPEID_PLAYER )
			{
				pl = static_cast< Player* >( pObj );
				can_see = pl->CanSee( plr );
				is_visible = pl->GetVisibility( plr, &it3 );
				if( can_see )
				{
					if(!is_visible)
					{
						buf.clear();
						count = plr->BuildCreateUpdateBlockForPlayer( &buf, pl );
						pl->PushCreationData(&buf, count);
						pl->AddVisibleObject(plr);
					}
				}
				else
				{
					if(is_visible)
					{
						plr->DestroyForPlayer(pl);
						pl->RemoveVisibleObject(it3);
					}
				}
			}
		}
	}
	else			// For units we can save a lot of work
	{
		for(set<Player*>::iterator it2 = GetInRangePlayerSetBegin(); it2 != GetInRangePlayerSetEnd(); ++it2)
		{
			can_see = (*it2)->CanSee(this);
			is_visible = (*it2)->GetVisibility(this, &itr);
			if(!can_see)
			{
				if(is_visible)
				{
					DestroyForPlayer(*it2);
					(*it2)->RemoveVisibleObject(itr);
				}
			}
			else
			{
				if(!is_visible)
				{
					buf.clear();
					count = BuildCreateUpdateBlockForPlayer(&buf, *it2);
					(*it2)->PushCreationData(&buf, count);
					(*it2)->AddVisibleObject(this);
				}
			}
		}
	}
}

void Unit::RemoveSoloAura(uint32 type)
{
	switch(type)
	{
		case 1:// Polymorph
		{
			if(!polySpell) return;
			if(HasActiveAura(polySpell))
				RemoveAura(polySpell);
		}break;
/*		case 2:// Sap
		{
			if(!sapSpell) return;
			if(HasActiveAura(sapSpell))
				RemoveAura(sapSpell);
		}break;
		case 3:// Fear (Warlock)
		{
			if(!fearSpell) return;
			if(HasActiveAura(fearSpell))
				RemoveAura(fearSpell);
		}break;*/
		default:
			{
			DEBUG_LOG("Warning: we are trying to remove a soloauratype that has no handle");
			}break;
	}
}

void Unit::EventHealthChangeSinceLastUpdate()
{
	int pct = GetHealthPct();
	if(pct<35)
	{
		uint32 toset=AURASTATE_FLAG_HEALTH35;
		if(pct<20)
			toset |= AURASTATE_FLAG_HEALTH20;
		else
			RemoveFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_HEALTH20);
		SetFlag(UNIT_FIELD_AURASTATE,toset);
	}
	else
		RemoveFlag(UNIT_FIELD_AURASTATE , AURASTATE_FLAG_HEALTH35 | AURASTATE_FLAG_HEALTH20);
}

int32 Unit::GetAP()
{
    int32 baseap = GetUInt32Value(UNIT_FIELD_ATTACK_POWER) + GetUInt32Value(UNIT_FIELD_ATTACK_POWER_MODS);
    float totalap = float(baseap)*(GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER)+1);
	if(totalap>=0)
		return float2int32(totalap);
	return	0;
}

int32 Unit::GetRAP()
{
    int32 baseap = GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER) + GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS);
    float totalap = float(baseap)*(GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER)+1);
	if(totalap>=0)
		return float2int32(totalap);
	return	0;
}

bool Unit::GetSpeedDecrease()
{
	int32 before=m_speedModifier;
	m_speedModifier -= m_slowdown;
	m_slowdown = 0;
	map< uint32, int32 >::iterator itr = speedReductionMap.begin();
	for(; itr != speedReductionMap.end(); ++itr)
		m_slowdown = (int32)min( m_slowdown, itr->second );

	if(m_slowdown<-100)
		m_slowdown = 100; //do not walk backwards !

	m_speedModifier += m_slowdown;
	//save bandwidth :P
	if(m_speedModifier!=before)
		return true;
	return false;
}

void Unit::EventCastSpell(Unit * Target, SpellEntry * Sp)
{
	Spell * pSpell = new Spell(Target, Sp, true, NULL);
	SpellCastTargets targets(Target->GetGUID());
	pSpell->prepare(&targets);
}

void Unit::SetFacing(float newo)
{
	SetOrientation(newo);
	/*WorldPacket data(40);
	data.SetOpcode(MSG_MOVE_SET_FACING);
	data << GetNewGUID();
	data << (uint32)0; //flags
	data << (uint32)0; //time
	data << GetPositionX() << GetPositionY() << GetPositionZ() << newo;
	data << (uint32)0; //unk
	SendMessageToSet( &data, false );*/

	/*WorldPacket data(SMSG_MONSTER_MOVE, 60);
	data << GetNewGUID();
	data << m_position << getMSTime();
	data << uint8(4) << newo;
	data << uint32(0x00000000);		// flags
	data << uint32(0);				// time
	data << m_position;				// position
	SendMessageToSet(&data, true);*/
	m_aiInterface->SendMoveToPacket(m_position.x,m_position.y,m_position.z,m_position.o,1,0x100); // MoveFlags = 0x100 (run)
}

//guardians are temporary spawn that will inherit master faction and will folow them. Apart from that they have their own mind
Unit* Unit::create_guardian(uint32 guardian_entry,uint32 duration,float angle, uint32 lvl)
{
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(guardian_entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(guardian_entry);
	if(!proto || !info)
	{
		DEBUG_LOG("Warning : Missing summon creature template %u !",guardian_entry);
		return NULL;
	}
	float m_fallowAngle=angle;
	float x = GetPositionX()+(3*(cosf(m_fallowAngle+GetOrientation())));
	float y = GetPositionY()+(3*(sinf(m_fallowAngle+GetOrientation())));
	float z = GetPositionZ();
	Creature * p = GetMapMgr()->CreateCreature(guardian_entry);
	p->SetInstanceID(GetMapMgr()->GetInstanceID());
	p->Load(proto, x, y, z, 0.0f);

	if (lvl != 0)
	{
		/* MANA */
		p->SetPowerType(POWER_TYPE_MANA);
		p->SetUInt32Value(UNIT_FIELD_MAXPOWER1,p->GetUInt32Value(UNIT_FIELD_MAXPOWER1)+28+10*lvl);
		p->SetUInt32Value(UNIT_FIELD_POWER1,p->GetUInt32Value(UNIT_FIELD_POWER1)+28+10*lvl);
		/* HEALTH */
		p->SetUInt32Value(UNIT_FIELD_MAXHEALTH,p->GetUInt32Value(UNIT_FIELD_MAXHEALTH)+28+30*lvl);
		p->SetUInt32Value(UNIT_FIELD_HEALTH,p->GetUInt32Value(UNIT_FIELD_HEALTH)+28+30*lvl);
		/* LEVEL */
		p->ModUnsigned32Value(UNIT_FIELD_LEVEL, lvl);
	}

	p->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, GetGUID());
    p->SetUInt64Value(UNIT_FIELD_CREATEDBY, GetGUID());
    p->SetZoneId(GetZoneId());
	p->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	p->_setFaction();

	p->GetAIInterface()->Init(p,AITYPE_PET,MOVEMENTTYPE_NONE,this);
	p->GetAIInterface()->SetUnitToFollow(this);
	p->GetAIInterface()->SetUnitToFollowAngle(m_fallowAngle);
	p->GetAIInterface()->SetFollowDistance(3.0f);

	p->PushToWorld(GetMapMgr());

	sEventMgr.AddEvent(p, &Creature::SummonExpire, EVENT_SUMMON_EXPIRE, duration, 1,0);

	return p;//lol, will compiler make the pointer conversion ?

}

float Unit::get_chance_to_daze(Unit *target)
{
//	if(GetTypeId()!=TYPEID_UNIT)
//		return 0.0f;
	float attack_skill = float( getLevel() ) * 5.0f;
	float defense_skill;
	if( target->IsPlayer() )
		defense_skill = float( static_cast< Player* >( target )->_GetSkillLineCurrent( SKILL_DEFENSE, false ) );
	else defense_skill = float( target->getLevel() * 5 );
	if( !defense_skill )
		defense_skill = 1;
	float chance_to_daze = attack_skill * 20 / defense_skill;//if level is equal then we get a 20% chance to daze
	chance_to_daze = chance_to_daze * std::min(target->getLevel() / 30.0f, 1.0f );//for targets below level 30 the chance decreses
	if( chance_to_daze > 40 )
		return 40.0f;
	else
		return chance_to_daze;
}

#define COMBAT_TIMEOUT_IN_SECONDS 5
#define COMBAT_TIMEOUT_RANGE 506.25			// 22.5

void CombatStatusHandler::ClearMyHealers()
{
	// this is where we check all our healers
	HealedSet::iterator i;
	Player * pt;
	for(i = m_healers.begin(); i != m_healers.end(); ++i)
	{
		pt = m_Unit->GetMapMgr()->GetPlayer(*i);
		if(pt != NULL)
			pt->CombatStatus.RemoveHealed(m_Unit);
	}

	m_healers.clear();
}

void CombatStatusHandler::WeHealed(Unit * pHealTarget)
{
	if(pHealTarget->GetTypeId() != TYPEID_PLAYER || m_Unit->GetTypeId() != TYPEID_PLAYER || pHealTarget == m_Unit)
		return;

	if(pHealTarget->CombatStatus.IsInCombat())
	{
		m_healed.insert(pHealTarget->GetLowGUID());
		pHealTarget->CombatStatus.m_healers.insert(m_Unit->GetLowGUID());
	}

	UpdateFlag();
}

void CombatStatusHandler::RemoveHealed(Unit * pHealTarget)
{
	m_healed.erase(pHealTarget->GetLowGUID());
	UpdateFlag();
}

void CombatStatusHandler::UpdateFlag()
{
	bool n_status = InternalIsInCombat();
	if(n_status != m_lastStatus)
	{
		m_lastStatus = n_status;
		if(n_status)
		{
			//printf(I64FMT" is now in combat.\n", m_Unit->GetGUID());
			m_Unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
			if(!m_Unit->hasStateFlag(UF_ATTACKING)) m_Unit->addStateFlag(UF_ATTACKING);
		}
		else
		{
			//printf(I64FMT" is no longer in combat.\n", m_Unit->GetGUID());
			m_Unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
			if(m_Unit->hasStateFlag(UF_ATTACKING)) m_Unit->clearStateFlag(UF_ATTACKING);

			// remove any of our healers from combat too, if they are able to be.
			ClearMyHealers();
		}
	}
}

bool CombatStatusHandler::InternalIsInCombat()
{
	if(m_healed.size() > 0)
		return true;

	if(m_attackTargets.size() > 0)
		return true;

	if(m_attackers.size() > 0)
		return true;

	return false;
}

bool CombatStatusHandler::IsAttacking(Unit * pTarget)
{
	// dead targets - no combat
	if( m_Unit->isDead() || pTarget->isDead() )
		return false;

	// check the target for any of our DoT's.
	for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
	{
		if(pTarget->m_auras[i] != NULL)
		{
			if(m_Unit->GetGUID() == pTarget->m_auras[i]->m_casterGuid && pTarget->m_auras[i]->IsCombatStateAffecting())
				return true;
		}
	}

	// place any additional checks here
	return false;
}

void CombatStatusHandler::ForceRemoveAttacker(const uint64& guid)
{
	// called on aura remove, etc.
	//printf("ForceRemoveAttacker "I64FMT"\n", guid);
	AttackerMap::iterator itr = m_attackers.find(guid);
	if(itr == m_attackers.end())
		return;

	m_attackers.erase(itr);
	UpdateFlag();
}

void CombatStatusHandler::RemoveAttackTarget(Unit * pTarget)
{
	// called on aura remove, etc.
	//printf("Trying to remove attack target "I64FMT" from "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());
	AttackTMap::iterator itr = m_attackTargets.find(pTarget->GetGUID());
	if(itr == m_attackTargets.end())
		return;

	if(!IsAttacking(pTarget))
	{
		if( pTarget->isDead() )
		{
			// remove naow.
			m_attackTargets.erase(itr);
			pTarget->CombatStatus.m_attackers.erase(m_Unit->GetGUID());
			UpdateFlag();
		}
		else
		{
			uint32 new_t = (uint32)UNIXTIME + COMBAT_TIMEOUT_IN_SECONDS;
			//printf("Setting attack target "I64FMT" on "I64FMT" to time out after 5 seconds.\n", pTarget->GetGUID(), m_Unit->GetGUID());
			if( itr->second < new_t )
				itr->second = new_t;
		}
	}
}

void CombatStatusHandler::OnDamageDealt(Unit * pTarget, uint32 damage)
{
	// we added an aura, or dealt some damage to a target. they need to have us as an attacker, and they need to be our attack target if not.
	//printf("OnDamageDealt to "I64FMT" from "I64FMT" timeout %u\n", pTarget->GetGUID(), m_Unit->GetGUID(), timeout);
	if(pTarget == m_Unit)
		return;

	if(!pTarget->isAlive())
		return;

	if( pTarget->GetDistanceSq(m_Unit) > COMBAT_TIMEOUT_RANGE )
		return;		// don't reset the combat timer when out of range.

	AttackTMap::iterator itr = m_attackTargets.find(pTarget->GetGUID());
	uint32 new_t = (uint32)UNIXTIME + COMBAT_TIMEOUT_IN_SECONDS;
	if(itr != m_attackTargets.end())
	{
		if( itr->second < new_t )
			itr->second = new_t;
	}
	else
	{
		m_attackTargets.insert(make_pair( pTarget->GetGUID(), new_t ));
		pTarget->CombatStatus.m_attackers.insert(m_Unit->GetGUID());

		UpdateFlag();
		pTarget->CombatStatus.UpdateFlag();
	}

	map<uint64,uint32>::iterator ditr = pTarget->CombatStatus.DamageMap.find(m_Unit->GetGUID());
	if(ditr == pTarget->CombatStatus.DamageMap.end())
	{
		pTarget->CombatStatus.DamageMap.insert( make_pair( m_Unit->GetGUID(), damage ));
		return;
	}
	ditr->second += damage;
}

void CombatStatusHandler::UpdateTargets()
{
	uint32 mytm = (uint32)UNIXTIME;
	AttackTMap::iterator itr = m_attackTargets.begin();
	AttackTMap::iterator it2 = m_attackTargets.begin();
	Unit *pUnit;

	for(; itr != m_attackTargets.end();)
	{
		it2 = itr++;
		if( it2->second <= mytm )
		{
			//printf("Timeout for attack target "I64FMT" on "I64FMT" expired.\n", it2->first, m_Unit->GetGUID());
			pUnit = m_Unit->GetMapMgr()->GetUnit(it2->first);
			if( pUnit == NULL || pUnit->isDead() )
				m_attackTargets.erase(it2);
			else
			{
				if( !IsAttacking(pUnit) )
				{
					pUnit->CombatStatus.m_attackers.erase( m_Unit->GetGUID() );
					pUnit->CombatStatus.UpdateFlag();
					
					m_attackTargets.erase(it2);
				}
			}
		}
	}

	UpdateFlag();
}

Unit* CombatStatusHandler::GetKiller()
{
	// No killer
	if(DamageMap.size() == 0)  
		return NULL;

	map<uint64,uint32>::iterator itr = DamageMap.begin();
	uint64 killer_guid = 0;
	uint32 mDamage = 0;
	for(; itr != DamageMap.end(); itr++)
	{
		if(itr->second > mDamage)
		{
			killer_guid = itr->first;
			mDamage = itr->second;
		}
	}

	if( killer_guid == 0 )
		return NULL;

	return (m_Unit->IsInWorld()) ? m_Unit->GetMapMgr()->GetUnit(killer_guid) : NULL;
}

void CombatStatusHandler::ClearAttackers()
{
	// this is a FORCED function, only use when the reference will be destroyed.
	AttackTMap::iterator itr = m_attackTargets.begin();
	Unit * pt;
	for(; itr != m_attackTargets.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetUnit(itr->first);
		if(pt)
		{
			pt->CombatStatus.m_attackers.erase(m_Unit->GetGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	AttackerMap::iterator it2;
	for(it2 = m_attackers.begin(); it2 != m_attackers.end(); ++it2)
	{
		pt = m_Unit->GetMapMgr()->GetUnit(*it2);
		if(pt)
		{
			pt->CombatStatus.m_attackTargets.erase(m_Unit->GetGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	m_attackers.clear();
	m_attackTargets.clear();
	UpdateFlag();
}

void CombatStatusHandler::ClearHealers()
{
	HealedSet::iterator itr = m_healed.begin();
	Player * pt;
	for(; itr != m_healed.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetPlayer(*itr);
		if(pt)
		{
			pt->CombatStatus.m_healers.erase(m_Unit->GetLowGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	for(itr = m_healers.begin(); itr != m_healers.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetPlayer(*itr);
		if(pt)
		{
			pt->CombatStatus.m_healed.erase(m_Unit->GetLowGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	m_healed.clear();
	m_healers.clear();
	UpdateFlag();
}

void CombatStatusHandler::OnRemoveFromWorld()
{
	ClearAttackers();
	ClearHealers();
	DamageMap.clear();
}

void Unit::Heal(Unit *target, uint32 SpellId, uint32 amount)
{//Static heal
	if(!target || !SpellId || !amount)
		return;

	uint32 ch=target->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh=target->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(mh!=ch)
	{
		ch += amount;
		if(ch > mh)
		{
			target->SetUInt32Value(UNIT_FIELD_HEALTH, mh);
			amount += mh-ch;
		}
		else 
			target->SetUInt32Value(UNIT_FIELD_HEALTH, ch);

		Spell::SendHealSpellOnPlayer(this, target, amount, false, SpellId);
	}
}
void Unit::Energize(Unit* target,uint32 SpellId, uint32 amount,uint32 type)
{//Static energize
	if(!target || !SpellId || !amount)
		return;
	uint32 cm=target->GetUInt32Value(UNIT_FIELD_POWER1+type);
	uint32 mm=target->GetUInt32Value(UNIT_FIELD_MAXPOWER1+type);
	if(mm!=cm)
	{
		cm += amount;
		if(cm > mm)
		{
			target->SetUInt32Value(UNIT_FIELD_POWER1+type, mm);
			amount += mm-cm;
		}
		else 
			target->SetUInt32Value(UNIT_FIELD_POWER1+type, cm);

		WorldPacket datamr(SMSG_SPELLENERGIZELOG, 30);
		datamr << target->GetNewGUID();
		datamr << this->GetNewGUID();
		datamr << uint32(SpellId);
		datamr << uint32(0);
		datamr << uint32(amount);
		this->SendMessageToSet(&datamr,true);
	}
}

void Unit::InheritSMMods(Unit *inherit_from)
{
	if(inherit_from->SM_CriticalChance)
	{
		if(SM_CriticalChance==0)
			SM_CriticalChance = new int32[SPELL_GROUPS];
		memcpy(SM_CriticalChance,inherit_from->SM_CriticalChance,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FDur)
	{
		if(SM_FDur==0)
			SM_FDur = new int32[SPELL_GROUPS];
		memcpy(SM_FDur,inherit_from->SM_FDur,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PDur)
	{
		if(SM_PDur==0)
			SM_PDur = new int32[SPELL_GROUPS];
		memcpy(SM_PDur,inherit_from->SM_PDur,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PRadius)
	{
		if(SM_PRadius==0)
			SM_PRadius = new int32[SPELL_GROUPS];
		memcpy(SM_PRadius,inherit_from->SM_PRadius,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FRadius)
	{
		if(SM_FRadius==0)
			SM_FRadius = new int32[SPELL_GROUPS];
		memcpy(SM_FRadius,inherit_from->SM_FRadius,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FRange)
	{
		if(SM_FRange==0)
			SM_FRange = new int32[SPELL_GROUPS];
		memcpy(SM_FRange,inherit_from->SM_FRange,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PCastTime)
	{
		if(SM_PCastTime==0)
			SM_PCastTime = new int32[SPELL_GROUPS];
		memcpy(SM_PCastTime,inherit_from->SM_PCastTime,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FCastTime)
	{
		if(SM_FCastTime==0)
			SM_FCastTime = new int32[SPELL_GROUPS];
		memcpy(SM_FCastTime,inherit_from->SM_FCastTime,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PCriticalDamage)
	{
		if(SM_PCriticalDamage==0)
			SM_PCriticalDamage = new int32[SPELL_GROUPS];
		memcpy(SM_PCriticalDamage,inherit_from->SM_PCriticalDamage,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FDOT)
	{
		if(SM_FDOT==0)
			SM_FDOT = new int32[SPELL_GROUPS];
		memcpy(SM_FDOT,inherit_from->SM_FDOT,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PDOT)
	{
		if(SM_PDOT==0)
			SM_PDOT = new int32[SPELL_GROUPS];
		memcpy(SM_PDOT,inherit_from->SM_PDOT,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FEffectBonus)
	{
		if(SM_FEffectBonus==0)
			SM_FEffectBonus = new int32[SPELL_GROUPS];
		memcpy(SM_FEffectBonus,inherit_from->SM_FEffectBonus,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PEffectBonus)
	{
		if(SM_PEffectBonus==0)
			SM_PEffectBonus = new int32[SPELL_GROUPS];
		memcpy(SM_PEffectBonus,inherit_from->SM_PEffectBonus,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FDamageBonus)
	{
		if(SM_FDamageBonus==0)
			SM_FDamageBonus = new int32[SPELL_GROUPS];
		memcpy(SM_FDamageBonus,inherit_from->SM_FDamageBonus,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PDamageBonus)
	{
		if(SM_PDamageBonus==0)
			SM_PDamageBonus = new int32[SPELL_GROUPS];
		memcpy(SM_PDamageBonus,inherit_from->SM_PDamageBonus,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PSPELL_VALUE)
	{
		if(SM_PSPELL_VALUE==0)
			SM_PSPELL_VALUE = new int32[SPELL_GROUPS];
		memcpy(SM_PSPELL_VALUE,inherit_from->SM_PSPELL_VALUE,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FSPELL_VALUE)
	{
		if(SM_FSPELL_VALUE==0)
			SM_FSPELL_VALUE = new int32[SPELL_GROUPS];
		memcpy(SM_FSPELL_VALUE,inherit_from->SM_FSPELL_VALUE,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FHitchance)
	{
		if(SM_FHitchance==0)
			SM_FHitchance = new int32[SPELL_GROUPS];
		memcpy(SM_FHitchance,inherit_from->SM_FHitchance,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PRange)
	{
		if(SM_PRange==0)
			SM_PRange = new int32[SPELL_GROUPS];
		memcpy(SM_PRange,inherit_from->SM_PRange,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PRadius)
	{
		if(SM_PRadius==0)
			SM_PRadius = new int32[SPELL_GROUPS];
		memcpy(SM_PRadius,inherit_from->SM_PRadius,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PAPBonus)
	{
		if(SM_PAPBonus==0)
			SM_PAPBonus = new int32[SPELL_GROUPS];
		memcpy(SM_PAPBonus,inherit_from->SM_PAPBonus,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PCost)
	{
		if(SM_PCost==0)
			SM_PCost = new int32[SPELL_GROUPS];
		memcpy(SM_PCost,inherit_from->SM_PCost,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FCost)
	{
		if(SM_FCost==0)
			SM_FCost = new int32[SPELL_GROUPS];
		memcpy(SM_FCost,inherit_from->SM_FCost,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FAdditionalTargets)
	{
		if(SM_FAdditionalTargets==0)
			SM_FAdditionalTargets = new int32[SPELL_GROUPS];
		memcpy(SM_FAdditionalTargets,inherit_from->SM_FAdditionalTargets,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PJumpReduce)
	{
		if(SM_PJumpReduce==0)
			SM_PJumpReduce = new int32[SPELL_GROUPS];
		memcpy(SM_PJumpReduce,inherit_from->SM_PJumpReduce,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FSpeedMod)
	{
		if(SM_FSpeedMod==0)
			SM_FSpeedMod = new int32[SPELL_GROUPS];
		memcpy(SM_FSpeedMod,inherit_from->SM_FSpeedMod,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PNonInterrupt)
	{
		if(SM_PNonInterrupt==0)
			SM_PNonInterrupt = new int32[SPELL_GROUPS];
		memcpy(SM_PNonInterrupt,inherit_from->SM_PNonInterrupt,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FPenalty)
	{
		if(SM_FPenalty==0)
			SM_FPenalty = new int32[SPELL_GROUPS];
		memcpy(SM_FPenalty,inherit_from->SM_FPenalty,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FCooldownTime)
	{
		if(SM_FCooldownTime==0)
			SM_FCooldownTime = new int32[SPELL_GROUPS];
		memcpy(SM_FCooldownTime,inherit_from->SM_FCooldownTime,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PCooldownTime)
	{
		if(SM_PCooldownTime==0)
			SM_PCooldownTime = new int32[SPELL_GROUPS];
		memcpy(SM_PCooldownTime,inherit_from->SM_PCooldownTime,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FChanceOfSuccess)
	{
		if(SM_FChanceOfSuccess==0)
			SM_FChanceOfSuccess = new int32[SPELL_GROUPS];
		memcpy(SM_FChanceOfSuccess,inherit_from->SM_FChanceOfSuccess,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_FRezist_dispell)
	{
		if(SM_FRezist_dispell==0)
			SM_FRezist_dispell = new int32[SPELL_GROUPS];
		memcpy(SM_FRezist_dispell,inherit_from->SM_FRezist_dispell,sizeof(int)*SPELL_GROUPS);
	}
	if(inherit_from->SM_PRezist_dispell)
	{
		if(SM_PRezist_dispell==0)
			SM_PRezist_dispell = new int32[SPELL_GROUPS];
		memcpy(SM_PRezist_dispell,inherit_from->SM_PRezist_dispell,sizeof(int)*SPELL_GROUPS);
	}
}

void Unit::CancelSpell(Spell * ptr)
{
	if(ptr)
		ptr->cancel();
	else if(m_currentSpell)
		m_currentSpell->cancel();
}

void Unit::EventStrikeWithAbility(uint64 guid, SpellEntry * sp, uint32 damage)
{
	Unit * victim = m_mapMgr ? m_mapMgr->GetUnit(guid) : NULL;
	if(victim)
		Strike( victim, RANGED, sp, 0, 0, 0, false, true );
}

void Unit::DispelAll(bool positive)
{
	for(uint32 i = 0; i < MAX_AURAS; ++i)
	{
		if(m_auras[i]!=NULL)
			if((m_auras[i]->IsPositive()&&positive)||!m_auras[i]->IsPositive())
				m_auras[i]->Remove();
	}
}

/* bool Unit::RemoveAllAurasByMechanic (renamed from MechanicImmunityMassDispel)
- Removes all auras on this unit that are of a specific mechanic.
- Useful for things like.. Apply Aura: Immune Mechanic, where existing (de)buffs are *always supposed* to be removed.
- I'm not sure if this goes here under unit.
* Arguments:
	- uint32 MechanicType
		*

* Returns;
	- False if no buffs were dispelled, true if more than 0 were dispelled.
*/
bool Unit::RemoveAllAurasByMechanic( uint32 MechanicType , uint32 MaxDispel = -1 , bool HostileOnly = true )
{
	//sLog.outString( "Unit::MechanicImmunityMassDispel called, mechanic: %u" , MechanicType );
	uint32 DispelCount = 0;
	for(uint32 x = ( HostileOnly ? MAX_POSITIVE_AURAS : 0 ) ; x < MAX_AURAS ; x++ ) // If HostileOnly = 1, then we use aura slots 40-56 (hostile). Otherwise, we use 0-56 (all)
		{
			if( DispelCount >= MaxDispel && MaxDispel > 0 )
			return true;

			if( m_auras[x] )
			{
					if( m_auras[x]->GetSpellProto()->MechanicsType == MechanicType ) // Remove all mechanics of type MechanicType (my english goen boom)
					{
						//sLog.outString( "Removed aura. [AuraSlot %u, SpellId %u]" , x , m_auras[x]->GetSpellId() );
						// TODO: Stop moving if fear was removed.
						m_auras[x]->Remove(); // EZ-Remove
						DispelCount ++;
					}
					else if( MechanicType == MECHANIC_ENSNARED ) // if got immunity for slow, remove some that are not in the mechanics
					{
						for( int i=0 ; i<3 ; i++ )
						{
							// SNARE + ROOT
							if( m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED || m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT )
							{
								m_auras[x]->Remove();
								break;
							}
						}
					}
			}
		}
	return ( DispelCount == 0 );
}

void Unit::setAttackTimer(int32 time, bool offhand)
{
	if(!time)
		time = offhand ? m_uint32Values[UNIT_FIELD_BASEATTACKTIME_01] : m_uint32Values[UNIT_FIELD_BASEATTACKTIME];

	time = std::max(1000,float2int32(float(time)*GetFloatValue(UNIT_MOD_CAST_SPEED)));
	if(time>300000)		// just in case.. shouldn't happen though
		time=offhand ? m_uint32Values[UNIT_FIELD_BASEATTACKTIME_01] : m_uint32Values[UNIT_FIELD_BASEATTACKTIME];

	if(offhand)
		m_attackTimer_1 = getMSTime() + time;
	else
		m_attackTimer = getMSTime() + time;
}

bool Unit::isAttackReady(bool offhand)
{
	if(offhand)
		return (getMSTime() >= m_attackTimer_1) ? true : false;
	else
		return (getMSTime() >= m_attackTimer) ? true : false;
}

void Unit::ReplaceAIInterface(AIInterface *new_interface) 
{ 
	delete m_aiInterface;	//be carefull when you do this. Might screw unit states !
	m_aiInterface = new_interface; 
}

bool Unit::HasAurasOfNameHashWithCaster(uint32 namehash, Unit * caster)
{
	for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
		if( m_auras[i] && m_auras[i]->GetSpellProto()->NameHash == namehash && m_auras[i]->GetCasterGUID() == caster->GetGUID() )
			return true;

	return false;
}

void Creature::UpdateLootAnimation()
{
	if( m_loot.HasItems() )
	{
		// update players with lootable flags
		for(set<Player*>::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
		{
			if( ( (*itr)->GetLowGUID() == m_taggingPlayer ) ||
				( (*itr)->GetGroup() != NULL && (*itr)->GetGroup()->GetID() == m_taggingGroup ) )
			{
				// only have to do the sparkly animation
				// TODO: do this by loot type for groups
				// switch(m_lootMethod)
				BuildFieldUpdatePacket(*itr, UNIT_DYNAMIC_FLAGS, m_uint32Values[UNIT_DYNAMIC_FLAGS] | U_DYN_FLAG_LOOTABLE);
			}
		}
	}
	else
	{
		// we are still alive, probably updating tapped state
		for(set<Player*>::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
		{
			if( !m_taggingPlayer )
			{
				BuildFieldUpdatePacket(*itr, UNIT_DYNAMIC_FLAGS, m_uint32Values[UNIT_DYNAMIC_FLAGS]);
			}
			else if( ( (*itr)->GetLowGUID() == m_taggingPlayer ) ||
				( (*itr)->GetGroup() != NULL && (*itr)->GetGroup()->GetID() == m_taggingGroup ) )
			{
				// tagger.
				BuildFieldUpdatePacket(*itr, UNIT_DYNAMIC_FLAGS, m_uint32Values[UNIT_DYNAMIC_FLAGS] | U_DYN_FLAG_TAPPED_BY_PLAYER);
			}
			else
			{
				BuildFieldUpdatePacket(*itr, UNIT_DYNAMIC_FLAGS, m_uint32Values[UNIT_DYNAMIC_FLAGS] | U_DYN_FLAG_TAGGED_BY_OTHER);
			}
		}
	}
}

void Creature::ClearTag()
{
	if( isAlive() )
	{
		ClearLoot();
		m_taggingGroup = m_taggingPlayer = 0;
		m_lootMethod = -1;

		// if we are alive, means that we left combat
		if( IsInWorld() )
			UpdateLootAnimation();
	}
	// dead, don't clear tag
}

void Object::ClearLoot()
{
	// better cancel any rolls just in case.
	for(vector<__LootItem>::iterator itr = m_loot.items.begin(); itr != m_loot.items.end(); ++itr)
	{
		if( itr->roll != NULL )
		{
			sEventMgr.RemoveEvents(itr->roll);
			delete itr->roll;
		}
	}

	m_loot.gold = 0;
	m_loot.items.clear();
	m_loot.looters.clear();
}


void Creature::Tag(Player *plr)
{
	// Tagging
	if( m_taggingPlayer != 0 )
		return;

	if(GetCreatureName() && GetCreatureName()->Type == CRITTER || IsPet())
		return;

	m_taggingPlayer = plr->GetLowGUID();
	m_taggingGroup = plr->m_playerInfo->m_Group ? plr->m_playerInfo->m_Group->GetID() : 0;

	/* set loot method */
	if( plr->GetGroup() != NULL )
		m_lootMethod = plr->GetGroup()->GetMethod();

	// update tag visual
	UpdateLootAnimation();
}
