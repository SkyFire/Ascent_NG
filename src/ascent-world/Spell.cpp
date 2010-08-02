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

#define SPELL_CHANNEL_UPDATE_INTERVAL 1000

/// externals for spell system
extern pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS];
extern pSpellTarget SpellTargetHandler[TOTAL_SPELL_TARGET];

enum SpellTargetSpecification
{
    TARGET_SPECT_NONE       = 0,
    TARGET_SPEC_INVISIBLE   = 1,
    TARGET_SPEC_DEAD        = 2,
};

void SpellCastTargets::read( WorldPacket & data,uint64 caster )
{
	m_unitTarget = m_itemTarget = 0;
	m_srcX = m_srcY = m_srcZ = m_destX = m_destY = m_destZ = 0;
	m_strTarget = "";

	data >> m_targetMask;
	WoWGuid guid;

	if( m_targetMask == TARGET_FLAG_SELF )
	{
		m_unitTarget = caster;
	}

	if( m_targetMask & (TARGET_FLAG_OBJECT | TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 ) )
	{
		data >> guid;
		m_unitTarget = guid.GetOldGuid();
	}

	if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
	{
		data >> guid;
		m_itemTarget = guid.GetOldGuid();
	}

	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
	{
		data >> m_srcX >> m_srcY >> m_srcZ;

		if( !( m_targetMask & TARGET_FLAG_DEST_LOCATION ) )
		{
			m_destX = m_srcX;
			m_destY = m_srcY;
			m_destZ = m_srcZ;
		}
	}

	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
	{
		data >> m_destX >> m_destY >> m_destZ;
		if( !( m_targetMask & TARGET_FLAG_SOURCE_LOCATION ) )
		{
			m_srcX = m_destX;
			m_srcY = m_destY;
			m_srcZ = m_destZ;
		}
	}

	if( m_targetMask & TARGET_FLAG_STRING )
	{
		data >> m_strTarget;
	}
}

void SpellCastTargets::write( WorldPacket& data )
{
	data << m_targetMask;

	if( m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | TARGET_FLAG_OBJECT ) )
        FastGUIDPack( data, m_unitTarget );

    if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
        FastGUIDPack( data, m_itemTarget );

	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
		data << m_srcX << m_srcY << m_srcZ;

	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
		data << m_destX << m_destY << m_destZ;

	if( m_targetMask & TARGET_FLAG_STRING )
		data << m_strTarget;
}

void SpellCastTargets::write( StackPacket& data )
{
	data << m_targetMask;

	if( m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | TARGET_FLAG_OBJECT ) )
		FastGUIDPack( data, m_unitTarget );

	if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
		FastGUIDPack( data, m_itemTarget );

	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
		data << m_srcX << m_srcY << m_srcZ;

	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
		data << m_destX << m_destY << m_destZ;

	if( m_targetMask & TARGET_FLAG_STRING )
		data << m_strTarget;
}

Spell::Spell(Object* Caster, SpellEntry *info, bool triggered, Aura* aur)
{
	ASSERT( Caster != NULL && info != NULL );
  
	m_spellInfo = info;
	m_caster = Caster;
	duelSpell = false;

	switch( Caster->GetTypeId() )
	{
		case TYPEID_PLAYER:
        {
		    g_caster = NULL;
		    i_caster = NULL;
		    u_caster = static_cast< Unit* >( Caster );
		    p_caster = static_cast< Player* >( Caster );
			if( p_caster->GetDuelState() == DUEL_STATE_STARTED )
			    duelSpell = true;
        }break;

		case TYPEID_UNIT:
        {
		    g_caster = NULL;
		    i_caster = NULL;
		    p_caster = NULL;
		    u_caster = static_cast< Unit* >( Caster );
		    if( u_caster->IsPet() && static_cast< Pet* >( u_caster)->GetPetOwner() != NULL && static_cast< Pet* >( u_caster )->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED )
			    duelSpell = true;
        }break;

		case TYPEID_ITEM:
		case TYPEID_CONTAINER:
        {
		    g_caster = NULL;
		    u_caster = NULL;
		    p_caster = NULL;
		    i_caster = static_cast< Item* >( Caster );
			if( i_caster->GetOwner() && i_caster->GetOwner()->GetDuelState() == DUEL_STATE_STARTED )
				duelSpell = true;
        }break;
		
		case TYPEID_GAMEOBJECT:
        {
		    u_caster = NULL;
		    p_caster = NULL;
		    i_caster = NULL;
		    g_caster = static_cast< GameObject* >( Caster );
        }break;
        default:
            DEBUG_LOG("[DEBUG][SPELL] Incompatible object type, please report this to the dev's");
        break;
	}

	m_spellState = SPELL_STATE_NULL;

	m_castPositionX = m_castPositionY = m_castPositionZ = 0;
	//TriggerSpellId = 0;
	//TriggerSpellTarget = 0;
	m_triggeredSpell = triggered;
	m_AreaAura = false;
  
	m_triggeredByAura = aur;

	damageToHit = 0;
	castedItemId = 0;
	
	m_usesMana = false;
	m_Spell_Failed = false;
	bDurSet = false;
	bRadSet[0] = false;
	bRadSet[1] = false;
	bRadSet[2] = false;
	
	cancastresult = SPELL_CANCAST_OK;
	
	m_requiresCP = false;
	unitTarget = NULL;
	//ModeratedTargets.clear();
	itemTarget = NULL;
	gameObjTarget = NULL;
	playerTarget = NULL;
	corpseTarget = NULL;
	judgement = false;
	add_damage = 0;
	m_Delayed = false;
	pSpellId = 0;
	m_cancelled = false;
	ProcedOnSpell = 0;
	forced_basepoints[0] = forced_basepoints[1] = forced_basepoints[2] = 0;
	extra_cast_number = 0;
	m_reflectedParent = NULL;
	m_isCasting = false;
	m_hitTargetCount = 0;
	m_missTargetCount = 0;
	m_targetList.reserve(3);
	m_magnetTarget = NULL;
	m_projectileWait = false;
}

Spell::~Spell()
{
	if( u_caster != NULL && u_caster->GetCurrentSpell() == this )
		u_caster->SetCurrentSpell(NULL); 
}

//i might forget conditions here. Feel free to add them
bool Spell::IsStealthSpell()
{
	//check if aura name is some stealth aura
	if( m_spellInfo->EffectApplyAuraName[0] == 16 ||
		m_spellInfo->EffectApplyAuraName[1] == 16 ||
		m_spellInfo->EffectApplyAuraName[2] == 16 )
		return true;
	return false;
}

//i might forget conditions here. Feel free to add them
bool Spell::IsInvisibilitySpell()
{
	//check if aura name is some invisibility aura
	if( m_spellInfo->EffectApplyAuraName[0] == 18 ||
		m_spellInfo->EffectApplyAuraName[1] == 18 ||
		m_spellInfo->EffectApplyAuraName[2] == 18 )
		return true;
	return false;
}

void Spell::FillSpecifiedTargetsInArea( float srcx, float srcy, float srcz, uint32 ind, uint32 specification )
{
    FillSpecifiedTargetsInArea( ind, srcx, srcy, srcz, GetRadius(ind), specification );
}

// for the moment we do invisible targets
void Spell::FillSpecifiedTargetsInArea(uint32 i,float srcx,float srcy,float srcz, float range, uint32 specification)
{
	//TargetsList *tmpMap=&m_targetUnits[i];
    //IsStealth()
    float r = range * range;
	//uint8 did_hit_result;
    for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
    {
        // don't add objects that are not units and that are dead
        if( !( (*itr)->IsUnit() ) || ! static_cast< Unit* >( *itr )->isAlive())
            continue;
        
        //static_cast< Unit* >( *itr )->IsStealth()
        if( m_spellInfo->TargetCreatureType)
        {
            if((*itr)->GetTypeId()!= TYPEID_UNIT)
                continue;
            CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
            if(!inf || !(1<<(inf->Type-1) & m_spellInfo->TargetCreatureType))
                continue;
        }

        if(IsInrange(srcx,srcy,srcz,(*itr),r))
        {
            if( u_caster != NULL )
            {
                if( isAttackable( u_caster, static_cast< Unit* >( *itr ),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
                {
					_AddTarget(((Unit*)(*itr)), i);
                }

            }
            else //cast from GO
            {
                if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
                {
                    //trap, check not to attack owner and friendly
                    if(isAttackable(g_caster->m_summoner,(Unit*)(*itr),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
                        _AddTargetForced((*itr)->GetGUID(), i);
                }
                else
                    _AddTargetForced((*itr)->GetGUID(), i);
            }
            if( m_spellInfo->MaxTargets)
            {
                if( m_hitTargetCount >= m_spellInfo->MaxTargets )
                    return;
            }
        }
    }
}
void Spell::FillAllTargetsInArea(LocationVector & location,uint32 ind)
{
    FillAllTargetsInArea(ind,location.x,location.y,location.z,GetRadius(ind));
}

void Spell::FillAllTargetsInArea(float srcx,float srcy,float srcz,uint32 ind)
{
	FillAllTargetsInArea(ind,srcx,srcy,srcz,GetRadius(ind));
}

/// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllTargetsInArea(uint32 i,float srcx,float srcy,float srcz, float range)
{
	//TargetsList *tmpMap=&m_targetUnits[i];
	float r = range*range;
	//uint8 did_hit_result;
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || ! static_cast< Unit* >( *itr )->isAlive() || ( (*itr)->GetTypeId()==TYPEID_UNIT && ((Creature*)*itr)->IsTotem() ) )
			continue;

		if( m_spellInfo->TargetCreatureType )
		{
			if( (*itr)->GetTypeId()!= TYPEID_UNIT )
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
			if( !inf || !( 1 << (inf->Type-1) & m_spellInfo->TargetCreatureType ) )
				continue;
		}
		if( IsInrange( srcx, srcy, srcz, (*itr), r ) )
		{
			if( u_caster != NULL )
			{
				if( isAttackable( u_caster, static_cast< Unit* >( *itr ), !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				{
					_AddTarget(((Unit*)(*itr)), i);
				}
			}
			else //cast from GO
			{
				if( g_caster != NULL && g_caster->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && g_caster->m_summoner != NULL )
				{
					//trap, check not to attack owner and friendly
					if( isAttackable( g_caster->m_summoner, static_cast< Unit* >( *itr ), !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
						_AddTargetForced((*itr)->GetGUID(), i);
				}
				else
					_AddTargetForced((*itr)->GetGUID(), i);
			}			
			if( m_spellInfo->MaxTargets )
				if( m_hitTargetCount >= m_spellInfo->MaxTargets )
					return;
		}
	}	
}

// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllFriendlyInArea( uint32 i, float srcx, float srcy, float srcz, float range )
{
	//TargetsList *tmpMap=&m_targetUnits[i];
	float r = range * range;
	//uint8 did_hit_result;
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !((*itr)->IsUnit()) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;

		if( m_spellInfo->TargetCreatureType )
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
			if(!inf || !(1<<(inf->Type-1) & m_spellInfo->TargetCreatureType))
				continue;
		}

		if( IsInrange( srcx, srcy, srcz, (*itr), r ) )
		{
			if( u_caster != NULL )
			{
				if( isFriendly( u_caster, static_cast< Unit* >( *itr ) ) )
				{
					_AddTarget(((Unit*)(*itr)), i);
				}
			}
			else //cast from GO
			{
				if( g_caster != NULL && g_caster->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && g_caster->m_summoner != NULL )
				{
					//trap, check not to attack owner and friendly
					if( isFriendly( g_caster->m_summoner, static_cast< Unit* >( *itr ) ) )
						_AddTargetForced((*itr)->GetGUID(), i);
				}
				else
					_AddTargetForced((*itr)->GetGUID(), i);
			}			
			if( m_spellInfo->MaxTargets )
				if( m_hitTargetCount >= m_spellInfo->MaxTargets )
					return;
		}
	}	
}

uint64 Spell::GetSinglePossibleEnemy(uint32 i,float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = m_spellInfo->base_range_or_radius_sqr;
		if( m_spellInfo->SpellGroupType && u_caster)
		{
			SM_FFValue(u_caster->SM_FRadius,&r,m_spellInfo->SpellGroupType);
			SM_PFValue(u_caster->SM_PRadius,&r,m_spellInfo->SpellGroupType);
		}
	}
	float srcx = m_caster->GetPositionX(), srcy = m_caster->GetPositionY(), srcz = m_caster->GetPositionZ();
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;

		if( m_spellInfo->TargetCreatureType )
		{
			if( (*itr)->GetTypeId() != TYPEID_UNIT )
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
			if(!inf || !(1<<(inf->Type-1) & m_spellInfo->TargetCreatureType))
				continue;
		}	
		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( u_caster != NULL )
			{
				if(isAttackable(u_caster, static_cast< Unit* >( *itr ),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) && _DidHit(((Unit*)*itr))==SPELL_DID_HIT_SUCCESS)
					return (*itr)->GetGUID(); 			
			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if( isAttackable( g_caster->m_summoner, static_cast< Unit* >( *itr ),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
						return (*itr)->GetGUID();
				}
			}			
		}
	}
	return 0;
}

uint64 Spell::GetSinglePossibleFriend(uint32 i,float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = m_spellInfo->base_range_or_radius_sqr;
		if( m_spellInfo->SpellGroupType && u_caster)
		{
			SM_FFValue(u_caster->SM_FRadius,&r,m_spellInfo->SpellGroupType);
			SM_PFValue(u_caster->SM_PRadius,&r,m_spellInfo->SpellGroupType);
		}
	}
	float srcx=m_caster->GetPositionX(),srcy=m_caster->GetPositionY(),srcz=m_caster->GetPositionZ();
	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;
		if( m_spellInfo->TargetCreatureType )
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
				if(!inf || !(1<<(inf->Type-1) & m_spellInfo->TargetCreatureType))
					continue;
		}	
		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( u_caster != NULL )
			{
				if( isFriendly( u_caster, static_cast< Unit* >( *itr ) ) && _DidHit(((Unit*)*itr))==SPELL_DID_HIT_SUCCESS)
					return (*itr)->GetGUID(); 			
			}
			else //cast from GO
			{
				if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if( isFriendly( g_caster->m_summoner, static_cast< Unit* >( *itr ) ) )
						return (*itr)->GetGUID();
				}
			}			
		}
	}
	return 0;
}

uint8 Spell::_DidHit(const Unit *target)
{
	//note resistchance is vise versa, is full hit chance
	Unit* u_victim = ((Unit*)target);
	Player* p_victim = ( target->GetTypeId() == TYPEID_PLAYER ) ? ((Player*)target) : NULL;

	// 
	float baseresist[3] = { 4.0f, 5.0f, 6.0f };
	int32 lvldiff;
	float resistchance ;
	if( u_victim == NULL )
		return SPELL_DID_HIT_MISS;

	/************************************************************************/
	/* Can't resist non-unit                                                */
	/************************************************************************/
	if(!u_caster)
		return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Can't reduce your own spells                                         */
	/************************************************************************/
	if(u_caster == u_victim)
		return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Check if the unit is evading                                         */
	/************************************************************************/
	if(u_victim->GetTypeId()==TYPEID_UNIT && u_victim->GetAIInterface()->getAIState()==STATE_EVADE)
		return SPELL_DID_HIT_EVADE;

	/************************************************************************/
	/* Check if the target is immune to this spell school                   */
	/************************************************************************/
	if(u_victim->SchoolImmunityList[m_spellInfo->School])
		return SPELL_DID_HIT_IMMUNE;
	
	/*************************************************************************/
	/* Check if the target is immune to this mechanic                        */
	/*************************************************************************/
	if(u_victim->MechanicsDispels[m_spellInfo->MechanicsType])
	{
		return SPELL_DID_HIT_IMMUNE; // Moved here from Spell::CanCast
	}
	
	/**** HACK FIX: AoE Snare/Root spells (i.e. Frost Nova) ****/
	/* If you find any other AoE effects that also apply something that SHOULD be a mechanic, add it here. */
	if( u_victim->MechanicsDispels[MECHANIC_ROOTED] ||
		u_victim->MechanicsDispels[MECHANIC_ENSNARED]
		)
	{
	for( int i = 0 ; i < 3 ; ++i )
		{
			if( u_victim->MechanicsDispels[MECHANIC_ROOTED] && m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT )
				return SPELL_DID_HIT_IMMUNE;
			if( u_victim->MechanicsDispels[MECHANIC_ENSNARED] && m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED )
				return SPELL_DID_HIT_IMMUNE;
		}

	}

	/************************************************************************/
	/* Check if the target has a % resistance to this mechanic              */
	/************************************************************************/
		/* Never mind, it's already done below. Lucky I didn't go through with this, or players would get double resistance. */

	/************************************************************************/
	/* Check if the spell is a melee attack and if it was missed/parried    */
	/************************************************************************/
	uint32 melee_test_result;
	if( m_spellInfo->is_melee_spell || m_spellInfo->is_ranged_spell )
	{
		uint32 _type;
		if( GetType() == SPELL_DMG_TYPE_RANGED )
			_type = RANGED;
		else
		{
			if (m_spellInfo->Flags4 & 0x1000000)
				_type =  OFFHAND;
			else
				_type = MELEE;
		}

		melee_test_result = u_caster->GetSpellDidHitResult( u_victim, _type, m_spellInfo );
		if(melee_test_result != SPELL_DID_HIT_SUCCESS)
			return (uint8)melee_test_result;
	}

	/************************************************************************/
	/* Check if the spell is resisted.                                      */
	/************************************************************************/
	if( m_spellInfo->School==0  || m_spellInfo->is_ranged_spell ) // all ranged spells are physical too...
		return SPELL_DID_HIT_SUCCESS;

	bool pvp =(p_caster && p_victim);

	if(pvp)
		lvldiff = p_victim->getLevel() - p_caster->getLevel();
	else
		lvldiff = u_victim->getLevel() - u_caster->getLevel();
	if (lvldiff < 0)
	{
		resistchance = baseresist[0] +lvldiff;
	}
	else
	{
		if(lvldiff < 3)
		{ 
				resistchance = baseresist[lvldiff];
		}
		else
		{
			if(pvp)
				resistchance = baseresist[2] + (((float)lvldiff-2.0f)*7.0f);
			else
				resistchance = baseresist[2] + (((float)lvldiff-2.0f)*11.0f);
		}
	}
	//check mechanical resistance
	//i have no idea what is the best pace for this code
	if( m_spellInfo->MechanicsType<27)
	{
		if(p_victim)
			resistchance += p_victim->MechanicsResistancesPCT[m_spellInfo->MechanicsType];
		else 
			resistchance += u_victim->MechanicsResistancesPCT[m_spellInfo->MechanicsType];
	}
	//rating bonus
	if( p_caster != NULL )
	{
		resistchance -= p_caster->CalcRating( PLAYER_RATING_MODIFIER_SPELL_HIT );
		resistchance -= p_caster->GetHitFromSpell();
	}

	if(p_victim)
		resistchance += p_victim->m_resist_hit[2];

	if( m_spellInfo->c_is_flags & SPELL_FLAG_IS_DISPEL_SPELL && m_spellInfo->SpellGroupType && u_caster)
	{
		SM_FFValue(u_caster->SM_FRezist_dispell,&resistchance,m_spellInfo->SpellGroupType);
		SM_PFValue(u_caster->SM_PRezist_dispell,&resistchance,m_spellInfo->SpellGroupType);
	}

	if(m_spellInfo->SpellGroupType && u_caster)
	{
		float hitchance=0;
		SM_FFValue(u_caster->SM_FHitchance,&hitchance,m_spellInfo->SpellGroupType);
		resistchance -= hitchance;
	}

	if (m_spellInfo->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY)
		resistchance = 0.0f;

	if(resistchance >= 100.0f)
		return SPELL_DID_HIT_RESIST;

	if(resistchance<=1.0)//resist chance >=1
	{
		if( Rand(1.0f) )
			return SPELL_DID_HIT_RESIST;
	}
	else
	{
		if( Rand(resistchance) )
			return SPELL_DID_HIT_RESIST;
	}

	// spell reflect handler
	if( Reflect(u_victim) )
		return SPELL_DID_HIT_REFLECT;

	return SPELL_DID_HIT_SUCCESS;
}

//generate possible target list for a spell. Use as last resort since it is not acurate
//this function makes a rough estimation for possible target !
//!!!disabled parts that were not tested !!
void Spell::GenerateTargets(SpellCastTargets *store_buff)
{
	float r = m_spellInfo->base_range_or_radius_sqr;
	if( m_spellInfo->SpellGroupType && u_caster)
	{
		SM_FFValue(u_caster->SM_FRadius,&r,m_spellInfo->SpellGroupType);
		SM_PFValue(u_caster->SM_PRadius,&r,m_spellInfo->SpellGroupType);
	}
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = m_spellInfo->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = m_spellInfo->EffectImplicitTargetB[i];		
			switch(cur)
			{
				case EFF_TARGET_NONE:{
					//this is bad for us :(
					}break;
				case EFF_TARGET_SELF:{
						if(m_caster->IsUnit())
							store_buff->m_unitTarget = m_caster->GetGUID();
					}break;		
					// need more research
				case 4:{ // dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"
					}break;			
				case EFF_TARGET_PET:
					{// Target: Pet
						if(p_caster && p_caster->GetSummon())
							store_buff->m_unitTarget = p_caster->GetSummon()->GetGUID();
					}break;
				case EFF_TARGET_SINGLE_ENEMY:// Single Target Enemy
				case 77:					// grep: i think this fits
				case 8: // related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
				case EFF_TARGET_ALL_ENEMY_IN_AREA: // All Enemies in Area of Effect (TEST)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT: // All Enemies in Area of Effect instant (e.g. Flamestrike)
				case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
				case EFF_TARGET_IN_FRONT_OF_CASTER:
				case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:// All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
				case 31:// related to scripted effects
				case 53:// Target Area by Players CurrentSelection()
				case 54:// Targets in Front of the Caster
					{
						if( p_caster != NULL )
						{
							Unit *selected = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
							if(isAttackable(p_caster,selected,!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
								store_buff->m_unitTarget = p_caster->GetSelection();
						}
						else if( u_caster != NULL )
						{
							if(	u_caster->GetAIInterface()->GetNextTarget() &&
								isAttackable(u_caster,u_caster->GetAIInterface()->GetNextTarget(),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) &&
								u_caster->GetDistanceSq(u_caster->GetAIInterface()->GetNextTarget()) <= r)
							{
								store_buff->m_unitTarget = u_caster->GetAIInterface()->GetNextTarget()->GetGUID();
							}
							if(u_caster->GetAIInterface()->getAITargetsCount())
							{
								//try to get most hated creature
								TargetMap *m_aiTargets = u_caster->GetAIInterface()->GetAITargets();
								TargetMap::iterator itr;
								for(itr = m_aiTargets->begin(); itr != m_aiTargets->end();itr++)
								{
									if( /*m_caster->GetMapMgr()->GetUnit(itr->first->GetGUID()) &&*/ itr->first->GetMapMgr() == m_caster->GetMapMgr() && 
										itr->first->isAlive() &&
										m_caster->GetDistanceSq(itr->first) <= r &&
										isAttackable(u_caster,itr->first,!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED))
										)
									{
										store_buff->m_unitTarget=itr->first->GetGUID();
										break;
									}
								}
							}
						}
						//try to get a whatever target
						if(!store_buff->m_unitTarget)
						{
							store_buff->m_unitTarget=GetSinglePossibleEnemy(i);
						}
						//if we still couldn't get a target, check maybe we could use 
//						if(!store_buff->m_unitTarget)
//						{
//						}
					}break;
					// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
				case 17: // A single target at a xyz location or the target is a possition xyz
				case 18:// Land under caster.Maybe not correct
					{
						store_buff->m_srcX=m_caster->GetPositionX();
						store_buff->m_srcY=m_caster->GetPositionY();
						store_buff->m_srcZ=m_caster->GetPositionZ();
						store_buff->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
					}break;
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
					{// All Party Members around the Caster in given range NOT RAID!			
						Player* p = p_caster;
						if( p == NULL)
						{
							if( static_cast< Creature* >( u_caster )->IsTotem() )
								p = static_cast< Player* >( static_cast< Creature* >( u_caster )->GetTotemOwner() );
						}
						if( p == NULL )
							break;

						if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
						{
							store_buff->m_unitTarget = m_caster->GetGUID();
							break;
						}
						SubGroup * subgroup = p->GetGroup() ?
							p->GetGroup()->GetSubGroup(p->GetSubGroup()) : 0;

						if(subgroup)
						{
							p->GetGroup()->Lock();
							for(GroupMembersSet::iterator itr = subgroup->GetGroupMembersBegin(); itr != subgroup->GetGroupMembersEnd(); ++itr)
							{
								if(!(*itr)->m_loggedInPlayer || m_caster == (*itr)->m_loggedInPlayer) 
									continue;
								if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
								{
									store_buff->m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
									break;
								}
							}
							p->GetGroup()->Unlock();
						}
					}break;
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6 
				case 57:// Targeted Party Member
					{// Single Target Friend
						if( p_caster != NULL )
						{
							if(isFriendly(p_caster,p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection())))
								store_buff->m_unitTarget = p_caster->GetSelection();
							else store_buff->m_unitTarget = p_caster->GetGUID();
						}
						else if( u_caster != NULL )
						{
							if(u_caster->GetUInt64Value(UNIT_FIELD_CREATEDBY))
								store_buff->m_unitTarget = u_caster->GetUInt64Value(UNIT_FIELD_CREATEDBY);
							else store_buff->m_unitTarget = u_caster->GetGUID();
						}
						else store_buff->m_unitTarget=GetSinglePossibleFriend(i,r);			
					}break;
				case EFF_TARGET_GAMEOBJECT:
					{
						if(p_caster && p_caster->GetSelection())
							store_buff->m_unitTarget = p_caster->GetSelection();
					}break;
				case EFF_TARGET_DUEL: 
					{// Single Target Friend Used in Duel
						if(p_caster && p_caster->DuelingWith && p_caster->DuelingWith->isAlive() && IsInrange(p_caster,p_caster->DuelingWith,r))
							store_buff->m_unitTarget = p_caster->GetSelection();
					}break;
				case EFF_TARGET_GAMEOBJECT_ITEM:{// Gameobject/Item Target
						//shit
					}break;
				case 27:{ // target is owner of pet
					// please correct this if not correct does the caster variablen need a Pet caster variable?
						if(u_caster && u_caster->IsPet())
							store_buff->m_unitTarget = static_cast< Pet* >( u_caster )->GetPetOwner()->GetGUID();
					}break;
				case EFF_TARGET_MINION:
				case 73:
					{// Minion Target
						if(m_caster->GetUInt64Value(UNIT_FIELD_SUMMON) == 0)
							store_buff->m_unitTarget = m_caster->GetGUID();
						else store_buff->m_unitTarget = m_caster->GetUInt64Value(UNIT_FIELD_SUMMON);
					}break;
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
					{
						Player *p=NULL;
						if( p_caster != NULL )
								p = p_caster;
						else if( u_caster && u_caster->GetTypeId() == TYPEID_UNIT && static_cast< Creature* >( u_caster )->IsTotem() )
								p = static_cast< Player* >( static_cast< Creature* >( u_caster )->GetTotemOwner() );
						if( p_caster != NULL )
						{
							if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
							{
								store_buff->m_unitTarget = p->GetGUID();
								break;
							}
							SubGroup * pGroup = p_caster->GetGroup() ?
								p_caster->GetGroup()->GetSubGroup(p_caster->GetSubGroup()) : 0;

							if( pGroup )
							{
								p_caster->GetGroup()->Lock();
								for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin();
									itr != pGroup->GetGroupMembersEnd(); ++itr)
								{
									if(!(*itr)->m_loggedInPlayer || p == (*itr)->m_loggedInPlayer) 
										continue;
									if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
									{
										store_buff->m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
										break;
									}
								}
								p_caster->GetGroup()->Unlock();
							}
						}
					}break;
				case 38:{//Dummy Target
					//have no idea
					}break;
				case EFF_TARGET_SELF_FISHING://Fishing
				case 46://Unknown Summon Atal'ai Skeleton
				case 47:// Portal
				case 52:	// Lightwells, etc
					{
						store_buff->m_unitTarget = m_caster->GetGUID();
					}break;
				case 40://Activate Object target(probably based on focus)
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
					{
						if( p_caster != NULL )
						{
							uint32 slot = m_spellInfo->Effect[i] - SPELL_EFFECT_SUMMON_TOTEM_SLOT1;
							if(p_caster->m_TotemSlots[slot] != 0)
								store_buff->m_unitTarget = p_caster->m_TotemSlots[slot]->GetGUID();
						}
					}break;
				case 61:{ // targets with the same group/raid and the same class
					//shit again
				}break;
				case EFF_TARGET_ALL_FRIENDLY_IN_AREA:{

				}break;
					
			}//end switch
		}//end for
	if(store_buff->m_unitTarget)
		store_buff->m_targetMask |= TARGET_FLAG_UNIT;
	if(store_buff->m_srcX)
		store_buff->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
	if(store_buff->m_destX)
		store_buff->m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}//end function

uint8 Spell::prepare( SpellCastTargets * targets )
{
	uint8 ccr;

	chaindamage = 0;
	m_targets = *targets;

	if( !m_triggeredSpell && p_caster != NULL && p_caster->CastTimeCheat )
		m_castTime = 0;
	else
	{
		m_castTime = GetCastTime( dbcSpellCastTime.LookupEntry( m_spellInfo->CastingTimeIndex ) );

		if( m_castTime && m_spellInfo->SpellGroupType && u_caster != NULL )
		{
			SM_FIValue( u_caster->SM_FCastTime, (int32*)&m_castTime, m_spellInfo->SpellGroupType );
			SM_PIValue( u_caster->SM_PCastTime, (int32*)&m_castTime, m_spellInfo->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			int spell_flat_modifers=0;
			int spell_pct_modifers=0;
			SM_FIValue(u_caster->SM_FCastTime,&spell_flat_modifers,m_spellInfo->SpellGroupType);
			SM_FIValue(u_caster->SM_PCastTime,&spell_pct_modifers,m_spellInfo->SpellGroupType);
			if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
				printf("!!!!!spell casttime mod flat %d , spell casttime mod pct %d , spell casttime %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,m_castTime,m_spellInfo->SpellGroupType);
#endif
		}

		// handle MOD_CAST_TIME
		if( u_caster != NULL && m_castTime )
		{
			m_castTime = float2int32( m_castTime * u_caster->GetFloatValue( UNIT_MOD_CAST_SPEED ) );
		}
	}

	uint8 forced_cancast_failure = 0;
	if( p_caster != NULL )
	{
		if( p_caster->cannibalize )
		{
			sEventMgr.RemoveEvents( p_caster, EVENT_CANNIBALIZE );
			p_caster->SetUInt32Value( UNIT_NPC_EMOTESTATE, 0 );
			p_caster->cannibalize = false;
		}

		if( GetGameObjectTarget() )
		{
			if( p_caster->IsStealth() )
			{
				p_caster->RemoveAura( p_caster->m_stealth );
			}

			if( (GetSpellProto()->Effect[0] == SPELL_EFFECT_OPEN_LOCK ||
				GetSpellProto()->Effect[1] == SPELL_EFFECT_OPEN_LOCK ||
				GetSpellProto()->Effect[2] == SPELL_EFFECT_OPEN_LOCK) && 
				p_caster->m_bgFlagIneligible)
			{
				forced_cancast_failure = SPELL_FAILED_BAD_TARGETS;
			}
		}
	}

	//let us make sure cast_time is within decent range
	//this is a hax but there is no spell that has more then 10 minutes cast time

	if( m_castTime < 0 )
		m_castTime = 0;
	else if( m_castTime > 60 * 10 * 1000)
		m_castTime = 60 * 10 * 1000; //we should limit cast time to 10 minutes right ?

	m_timer = m_castTime;

	//if( p_caster != NULL )
	//   m_castTime -= 100;	  // session update time


	if( !m_triggeredSpell && p_caster != NULL && p_caster->CooldownCheat )
		p_caster->ClearCooldownForSpell( m_spellInfo->Id );

	if( m_triggeredSpell )
		cancastresult = SPELL_CANCAST_OK;
	else
		cancastresult = CanCast(false);

	//sLog.outString( "CanCast result: %u. Refer to SpellFailure.h to work out why." , cancastresult );

	ccr = cancastresult;

	if( forced_cancast_failure )
		cancastresult = forced_cancast_failure;

	if( cancastresult != SPELL_CANCAST_OK )
	{
		SendCastResult( cancastresult );

		if( m_triggeredByAura )
		{
			SendChannelUpdate( 0 );
			if( u_caster != NULL )
				u_caster->RemoveAura( m_triggeredByAura );
		}
		else
		{
			// HACK, real problem is the way spells are handled
			// when a spell is channeling and a new spell is casted
			// that is a channeling spell, but not triggert by a aura
			// the channel bar/spell is bugged
			if( u_caster->GetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT) > 0 && u_caster->GetCurrentSpell() )
			{
				u_caster->GetCurrentSpell()->cancel();
				SendChannelUpdate( 0 );
				cancel();
				return ccr;
			}
		}
		finish();
		return ccr;
	}
	else
	{
		SendSpellStart();

		// start cooldown handler
		if( p_caster != NULL && !p_caster->CastTimeCheat && !m_triggeredSpell )
		{
			AddStartCooldown();
		}

		if( i_caster == NULL )
		{
			if( p_caster != NULL && m_timer > 0 && !m_triggeredSpell )
				p_caster->delayAttackTimer( m_timer + 1000 );
			//p_caster->setAttackTimer(m_timer + 1000, false);
		}

		// aura state removal
		if( m_spellInfo->CasterAuraState )
			u_caster->RemoveFlag( UNIT_FIELD_AURASTATE, m_spellInfo->CasterAuraState );
	}

	m_spellState = SPELL_STATE_PREPARING;

	//instant cast(or triggered) and not channeling
	if( u_caster != NULL && ( m_castTime > 0 || m_spellInfo->ChannelInterruptFlags ) && !m_triggeredSpell )	
	{
		m_castPositionX = m_caster->GetPositionX();
		m_castPositionY = m_caster->GetPositionY();
		m_castPositionZ = m_caster->GetPositionZ();

		u_caster->castSpell( this );
	}
	else
		cast( false );


	return ccr;
}

void Spell::cancel()
{
	SendInterrupted(0);
	SendCastResult(SPELL_FAILED_INTERRUPTED);

	if(m_spellState == SPELL_STATE_CASTING)
	{
		if( u_caster != NULL )
			u_caster->RemoveAura(m_spellInfo->Id);
	
		if(m_timer > 0 || m_Delayed)
		{
			if(p_caster && p_caster->IsInWorld())
			{
				Unit *pTarget = p_caster->GetMapMgr()->GetUnit(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				if(!pTarget)
					pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
				  
				if(pTarget)
				{
					pTarget->RemoveAura(m_spellInfo->Id, m_caster->GetGUID());
				}
				if(m_AreaAura)//remove of blizz and shit like this
				{
					
					DynamicObject* dynObj=m_caster->GetMapMgr()->GetDynamicObject(m_caster->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
					if(dynObj)
					{
						dynObj->RemoveFromWorld(true);
						delete dynObj;
					}
				}

				if(p_caster->GetSummonedObject())
				{
					if(p_caster->GetSummonedObject()->IsInWorld())
						p_caster->GetSummonedObject()->RemoveFromWorld(true);
					// for now..
					ASSERT(p_caster->GetSummonedObject()->GetTypeId() == TYPEID_GAMEOBJECT);
					delete ((GameObject*)(p_caster->GetSummonedObject()));
					p_caster->SetSummonedObject(NULL);
				}
				if (m_timer > 0)
					p_caster->delayAttackTimer(-m_timer);
//				p_caster->setAttackTimer(1000, false);
			 }
		}
		SendChannelUpdate(0);
	}

	//m_spellState = SPELL_STATE_FINISHED;

	// prevent memory corruption. free it up later.
	// if this is true it means we are currently in the cast() function somewhere else down the stack
	// (recursive spells) and we don't wanna have this class delete'd when we return to it.
	// at the end of cast() it will get freed anyway.
	if( !m_isCasting )
		finish();
}

void Spell::AddCooldown()
{
	if( p_caster != NULL )
		p_caster->Cooldown_Add( m_spellInfo, i_caster );
}

void Spell::AddStartCooldown()
{
	if( p_caster != NULL )
		p_caster->Cooldown_AddStart( m_spellInfo );
}

void Spell::cast(bool check)
{
	if( duelSpell && (
		( p_caster != NULL && p_caster->GetDuelState() != DUEL_STATE_STARTED ) ||
		( u_caster != NULL && u_caster->IsPet() && static_cast< Pet* >( u_caster )->GetPetOwner() && static_cast< Pet* >( u_caster )->GetPetOwner()->GetDuelState() != DUEL_STATE_STARTED ) ) )
	{
		// Can't cast that!
		SendInterrupted( SPELL_FAILED_TARGET_FRIENDLY );
		finish();
		return;
	}

	DEBUG_LOG("Spell::cast %u, Unit: %u", m_spellInfo->Id, m_caster->GetLowGUID());

	if(check)
		cancastresult = CanCast(true);
	else 
		cancastresult = SPELL_CANCAST_OK;

	if(cancastresult == SPELL_CANCAST_OK)
	{
		if (p_caster && !m_triggeredSpell && p_caster->IsInWorld() && GET_TYPE_FROM_GUID(m_targets.m_unitTarget)==HIGHGUID_TYPE_UNIT)
		{
			sQuestMgr.OnPlayerCast(p_caster,m_spellInfo->Id,m_targets.m_unitTarget);
		}

		// trigger on next attack
		if( m_spellInfo->Attributes & ATTRIBUTE_ON_NEXT_ATTACK && !m_triggeredSpell )
		{
			// check power
			if(!HasPower())
			{
				SendInterrupted(SPELL_FAILED_NO_POWER);
				finish();
				return; 
			}

			// we're much better to remove this here, because otherwise spells that change powers etc,
			// don't get applied.

			if(u_caster && !m_triggeredSpell && !m_triggeredByAura)
				u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, m_spellInfo->Id);

			//not sure if it must be there...
			/*if( p_caster != NULL )
			{
				if(p_caster->m_onAutoShot)
				{
					p_caster->GetSession()->OutPacket(SMSG_CANCEL_AUTO_REPEAT);
					p_caster->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
					p_caster->m_onAutoShot = false;
				}
			}*/
			
			m_isCasting = false;
			SendCastResult(cancastresult);
			if( u_caster != NULL )  
				u_caster->SetOnMeleeSpell(m_spellInfo->Id);

			finish();
			return;
		}

		if( !m_projectileWait )
		{
			if(!TakePower() && !m_triggeredSpell) //not enough mana
			{
				//DEBUG_LOG("Spell::Not Enough Mana");
				SendInterrupted(SPELL_FAILED_NO_POWER);
				finish();
				return; 
			}
			
			for(uint32 i=0;i<3;i++)
			{
				if( m_spellInfo->Effect[i])
					 FillTargetMap(i);
			}

			SendCastResult(cancastresult);
			if(cancastresult != SPELL_CANCAST_OK)
			{
				finish();
				return;
			}

			m_isCasting = true;

			//sLog.outString( "CanCastResult: %u" , cancastresult );
			if(!m_triggeredSpell)
				AddCooldown();

			if( p_caster )
			{
				if( m_spellInfo->NameHash == SPELL_HASH_SLAM)
				{
					/* slam - reset attack timer */
					p_caster->setAttackTimer( 0, true );
					p_caster->setAttackTimer( 0, false );
				}
				if( p_caster->IsStealth() && !(m_spellInfo->AttributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH) )
				{
					/* talents procing - don't remove stealth either */
					if (!(m_spellInfo->Attributes & ATTRIBUTES_PASSIVE) && 
						!( pSpellId && dbcSpell.LookupEntry(pSpellId)->Attributes & ATTRIBUTES_PASSIVE ) )
					{
						p_caster->RemoveAura(p_caster->m_stealth);
						p_caster->m_stealth = 0;
					}
				}
			}

			SendSpellGo();

			//******************** SHOOT SPELLS ***********************
			//* Flags are now 1,4,19,22 (4718610) //0x480012

			if (m_spellInfo->Flags4 & 0x8000 && m_caster->IsPlayer() && m_caster->IsInWorld())
			{
                /// Part of this function contains a hack fix
                /// hack fix for shoot spells, should be some other resource for it
                //p_caster->SendSpellCoolDown(m_spellInfo->Id, m_spellInfo->RecoveryTime ? m_spellInfo->RecoveryTime : 2300);
				WorldPacket data(SMSG_SPELL_COOLDOWN, 14);
				data << m_spellInfo->Id;
				data << p_caster->GetNewGUID();
				data << uint32(m_spellInfo->RecoveryTime ? m_spellInfo->RecoveryTime : 2300);
				p_caster->GetSession()->SendPacket(&data);
			}
		
			if( m_spellInfo->ChannelInterruptFlags != 0 && !m_triggeredSpell)
			{
				/*
				Channeled spells are handled a little differently. The five second rule starts when the spell's channeling starts; i.e. when you pay the mana for it.
				The rule continues for at least five seconds, and longer if the spell is channeled for more than five seconds. For example,
				Mind Flay channels for 3 seconds and interrupts your regeneration for 5 seconds, while Tranquility channels for 10 seconds
				and interrupts your regeneration for the full 10 seconds.
				*/

				uint32 channelDuration = GetDuration();
				m_spellState = SPELL_STATE_CASTING;
				SendChannelStart(channelDuration);
				if( p_caster != NULL )
				{
					//Use channel interrupt flags here
					if(m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION || m_targets.m_targetMask == TARGET_FLAG_SOURCE_LOCATION)
						u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSelection());					
					else if(p_caster->GetSelection() == m_caster->GetGUID())
					{
						if(p_caster->GetSummon())
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSummon()->GetGUID());
						else if(m_targets.m_unitTarget)
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, m_targets.m_unitTarget);
						else
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSelection());
					}
					else
					{
						if(p_caster->GetSelection())
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSelection());
						else if(p_caster->GetSummon())
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, p_caster->GetSummon()->GetGUID());
						else if(m_targets.m_unitTarget)
							u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, m_targets.m_unitTarget);
						else
						{
							m_isCasting = false;
							cancel();
							return;
						}
					}
				}
				if(u_caster && u_caster->GetPowerType()==POWER_TYPE_MANA)
				{
					if(channelDuration <= 5000)
						u_caster->DelayPowerRegeneration(5000);
					else
						u_caster->DelayPowerRegeneration(channelDuration);
				}

				if( u_caster != NULL )
					u_caster->SetCurrentSpell(this);
			}
			else
			{
				// timed?
				if( m_spellInfo->speed > 0.0f && m_targets.m_unitTarget != 0 )
				{
					Object *pTmpTarget = _LookupObject(m_targets.m_unitTarget);
					if( pTmpTarget != NULL && pTmpTarget->IsUnit() )
					{
						float tmpDistance = m_caster->CalcDistance(pTmpTarget);
						float tmpTime = ( tmpDistance * 1000.0f ) / m_spellInfo->speed;
#ifdef DEBUG
						printf("Spell projectile: dist: %.5f, time: %.5f\n", tmpDistance, tmpTime);
#endif
						if( tmpTime > 100.0f )
						{
							m_projectileWait = true;
							sEventMgr.AddEvent(m_caster, &Object::EventSpellHit, this, EVENT_UNIT_SPELL_HIT,
								(uint32)float2int32(tmpTime), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_FIRE_ON_DELETE);

							if( u_caster->GetCurrentSpell() == this )
								u_caster->SetCurrentSpell(NULL);

							return;
						}
					}
				}
			}
		}

		// projectile wait or hit

		//uint32 spellid = m_spellInfo->Id;

        // if the spell is not reflected
		SpellTargetList::iterator itr = m_targetList.begin();
		uint32 x;
		for(; itr != m_targetList.end(); ++itr)
		{
			if( itr->HitResult != SPELL_DID_HIT_SUCCESS )
				continue;

			// set target pointers
			_SetTargets(itr->Guid);

			// call effect handlers
			for( x = 0; x < 3; ++x )
			{
				if( m_spellInfo->Effect[x] && itr->EffectMask & (1 << x) )
					HandleEffects(x);
			}

			// handle the rest of shit
			if( unitTarget != NULL )
			{
				// aura state
				if( m_spellInfo->TargetAuraState )
					unitTarget->RemoveFlag(UNIT_FIELD_AURASTATE, m_spellInfo->TargetAuraState);

				// proc!
				if( u_caster != NULL && u_caster->IsInWorld() )
				{
					if(!m_triggeredSpell)
					{
						u_caster->HandleProc(PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, unitTarget, m_spellInfo);
						u_caster->m_procCounter = 0; //this is required for to be able to count the depth of procs (though i have no idea where/why we use proc on proc)
					}
				}
			}
		}

		/* don't call HandleAddAura unless we actually have auras... - Burlex*/
		if( m_spellInfo->EffectApplyAuraName[0] != 0 || m_spellInfo->EffectApplyAuraName[1] != 0 ||
		   m_spellInfo->EffectApplyAuraName[2] != 0)
		{
			itr = m_targetList.begin();
			for(; itr != m_targetList.end(); ++itr)
			{
				if( itr->HitResult != SPELL_DID_HIT_SUCCESS )
					continue;

				HandleAddAura(itr->Guid);
			}
		}

		// we're much better to remove this here, because otherwise spells that change powers etc,
		// don't get applied.

		if(u_caster && !m_triggeredSpell && !m_triggeredByAura)
			u_caster->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, m_spellInfo->Id);

		m_isCasting = false;

		if(m_spellState != SPELL_STATE_CASTING)
			finish();
	}
	else
	{
		// cancast failed
		SendCastResult(cancastresult);
		finish();
	}
}

void Spell::AddTime(uint32 type)
{
	if(u_caster && u_caster->IsPlayer())
	{
		if( m_spellInfo->InterruptFlags & CAST_INTERRUPT_ON_DAMAGE_TAKEN)
		{
			cancel();
			return;
		}
		if( m_spellInfo->SpellGroupType && u_caster)
		{
			float ch=0;
			SM_FFValue(u_caster->SM_PNonInterrupt,&ch,m_spellInfo->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			float spell_pct_modifers=0;
			SM_FFValue(u_caster->SM_PNonInterrupt,&spell_pct_modifers,m_spellInfo->SpellGroupType);
			if(spell_pct_modifers!=0)
				printf("!!!!!spell interrupt chance mod pct %f , uninterrupt chance %f, spell group %u\n",spell_pct_modifers,ch,m_spellInfo->SpellGroupType);
#endif
			if(Rand(ch))
				return;
		}
		if( p_caster != NULL )
		{
			if(Rand(p_caster->SpellDelayResist[type]))
				return;
		}
		if(m_spellState==SPELL_STATE_PREPARING)
		{
			int32 delay = m_castTime/4;
			m_timer+=delay;
			if(m_timer>m_castTime)
			{
				delay -= (m_timer - m_castTime);
				m_timer=m_castTime;
				if(delay<0)
					delay = 1;
			}

			WorldPacket data(SMSG_SPELL_DELAYED, 13);
			data << u_caster->GetNewGUID();
			data << uint32(delay);
			u_caster->SendMessageToSet(&data, true);

			if(!p_caster)
			{
				if(m_caster->GetTypeId() == TYPEID_UNIT)
					u_caster->GetAIInterface()->AddStopTime(delay);
			}
			//in case cast is delayed, make sure we do not exit combat 
			else
			{
//				sEventMgr.ModifyEventTimeLeft(p_caster,EVENT_ATTACK_TIMEOUT,PLAYER_ATTACK_TIMEOUT_INTERVAL,true);
				// also add a new delay to offhand and main hand attacks to avoid cutting the cast short
				p_caster->delayAttackTimer(delay);
			}
		}
		else if( m_spellInfo->ChannelInterruptFlags != 48140)
		{
			int32 delay = GetDuration()/3;
			m_timer-=delay;
			if(m_timer<0)
				m_timer=0;
			else
				p_caster->delayAttackTimer(-delay);

			m_Delayed = true;
			if(m_timer>0)
				SendChannelUpdate(m_timer);

		}
	}
}

void Spell::update(uint32 difftime)
{
	// ffs stealth capping
	if( p_caster && GetGameObjectTarget() && p_caster->IsStealth() )
	{
		p_caster->RemoveAura( p_caster->m_stealth );
	}

	// skip cast if we're more than 2/3 of the way through
	if(
		(((float)m_castTime / 1.5f) > (float)m_timer ) && 
//		float(m_castTime)/float(m_timer) >= 2.0f		&&
		(
		m_castPositionX != m_caster->GetPositionX() ||
		m_castPositionY != m_caster->GetPositionY() ||
		m_castPositionZ != m_caster->GetPositionZ()
		)
		)
	{	
		if( u_caster != NULL )
		{
			if(u_caster->HasNoInterrupt() == 0 && m_spellInfo->EffectMechanic[1] != 14)
			{
				cancel();
				return;
			}
		}
	}

	if(m_cancelled)
	{
		cancel();
		return;
	}

	switch(m_spellState)
	{
	case SPELL_STATE_PREPARING:
		{
			//printf("spell::update m_timer %u, difftime %d, newtime %d\n", m_timer, difftime, m_timer-difftime);
			if((int32)difftime >= m_timer)
			{
				if( u_caster != NULL && u_caster->GetCurrentSpell() == this )
					u_caster->SetCurrentSpell(NULL);

				cast(true);
			}
			else 
			{
				m_timer -= difftime;
				if((int32)difftime >= m_timer)
				{
					if( u_caster != NULL && u_caster->GetCurrentSpell() == this )
						u_caster->SetCurrentSpell(NULL);

					m_timer = 0;
					cast(true);
				}
			}
			
			
		} break;
	case SPELL_STATE_CASTING:
		{
			if(m_timer > 0)
			{
				if((int32)difftime >= m_timer)
					m_timer = 0;
				else
					m_timer -= difftime;
			}
			if(m_timer == 0)
			{
				if( u_caster != NULL && u_caster->GetCurrentSpell() == this )
					u_caster->SetCurrentSpell(NULL);

				SendChannelUpdate(0);
				finish();
			}
		} break;
	}
}

void Spell::finish()
{
	m_spellState = SPELL_STATE_FINISHED;
	if( u_caster != NULL )
	{
		u_caster->m_canMove = true;
		// mana           channeled                                                     power type is mana
		if(m_usesMana && (m_spellInfo->ChannelInterruptFlags == 0 && !m_triggeredSpell) && u_caster->GetPowerType()==POWER_TYPE_MANA)
		{
			/*
			Five Second Rule
			After a character expends mana in casting a spell, the effective amount of mana gained per tick from spirit-based regeneration becomes a ratio of the normal 
			listed above, for a period of 5 seconds. During this period mana regeneration is said to be interrupted. This is commonly referred to as the five second rule. 
			By default, your interrupted mana regeneration ratio is 0%, meaning that spirit-based mana regeneration is suspended for 5 seconds after casting.
			Several effects can increase this ratio, including:
			*/

			u_caster->DelayPowerRegeneration(5000);
		}
	}
	/* Mana Regenerates while in combat but not for 5 seconds after each spell */
	/* Only if the spell uses mana, will it cause a regen delay.
	   is this correct? is there any spell that doesn't use mana that does cause a delay?
	   this is for creatures as effects like chill (when they have frost armor on) prevents regening of mana	*/
	
	//moved to spellhandler.cpp -> remove item when click on it! not when it finishes 

	//enable pvp when attacking another player with spells
	if( p_caster != NULL )
	{
		if (m_spellInfo->Attributes & ATTRIBUTES_STOP_ATTACK)
		{
			p_caster->EventAttackStop();
			p_caster->smsg_AttackStop( unitTarget );
		}

		if(m_requiresCP && !GetSpellFailed())
		{
			if(p_caster->m_spellcomboPoints)
			{
				p_caster->m_comboPoints = p_caster->m_spellcomboPoints;
				p_caster->UpdateComboPoints(); //this will make sure we do not use any wrong values here
			}
			else
			{
				p_caster->NullComboPoints();
			}
		}
		if(m_Delayed)
		{
			Unit *pTarget = NULL;
			if( p_caster->IsInWorld() )
			{
				pTarget = p_caster->GetMapMgr()->GetUnit(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				if(!pTarget)
					pTarget = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
			}
			   
			if(pTarget)
			{
				pTarget->RemoveAura(m_spellInfo->Id, m_caster->GetGUID());
			}
		}
	}

	if( m_spellInfo->Effect[0] == SPELL_EFFECT_SUMMON_OBJECT ||
		m_spellInfo->Effect[1] == SPELL_EFFECT_SUMMON_OBJECT ||
		m_spellInfo->Effect[2] == SPELL_EFFECT_SUMMON_OBJECT)
		if( p_caster != NULL )
			p_caster->SetSummonedObject(NULL);		
	/*
	We set current spell only if this spell has cast time or is channeling spell
	otherwise it's instant spell and we delete it right after completion
	*/
	
	if( u_caster != NULL )
	{
		if( u_caster->GetCurrentSpell() == this )
			u_caster->SetCurrentSpell(NULL);
	}

	if( p_caster != NULL && !GetSpellFailed() )
	{
		sHookInterface.OnPostSpellCast( p_caster, GetSpellProto(), unitTarget );
	}
	
	if( p_caster && ( cancastresult == SPELL_CANCAST_OK && !GetSpellFailed() ) )
		RemoveItems();
	delete this;
}

void Spell::SendCastResult(uint8 result)
{
	uint32 Extra = 0;
	if(result == SPELL_CANCAST_OK) return;

	SetSpellFailed();

	if(!m_caster->IsInWorld()) return;

	Player * plr = p_caster;

	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr) return;

	// reset cooldowns
	if( m_spellState == SPELL_STATE_PREPARING )
		plr->Cooldown_OnCancel(m_spellInfo);

	// for some reason, the result extra is not working for anything, including SPELL_FAILED_REQUIRES_SPELL_FOCUS
	switch( result )
	{
	case SPELL_FAILED_REQUIRES_SPELL_FOCUS:
		Extra = m_spellInfo->RequiresSpellFocus;
		break;

	case SPELL_FAILED_REQUIRES_AREA:
		Extra = m_spellInfo->RequiresAreaId;
		break;

	case SPELL_FAILED_TOTEMS:
		Extra = m_spellInfo->Totem[1] ? m_spellInfo->Totem[1] : m_spellInfo->Totem[0];
		break;

	//case SPELL_FAILED_TOTEM_CATEGORY: seems to be fully client sided.
	}

	//plr->SendCastResult(m_spellInfo->Id, result, extra_cast_number, Extra);
	if( Extra )
	{
		packetSMSG_CASTRESULT_EXTRA pe;
		pe.SpellId = m_spellInfo->Id;
		pe.ErrorMessage = result;
		pe.MultiCast = extra_cast_number;
		pe.Extra = Extra;
		plr->GetSession()->OutPacket( SMSG_CAST_FAILED, sizeof( packetSMSG_CASTRESULT_EXTRA ), &pe );
	}
	else
	{
		packetSMSG_CASTRESULT pe;
		pe.SpellId = m_spellInfo->Id;
		pe.ErrorMessage = result;
		pe.MultiCast = extra_cast_number;
		plr->GetSession()->OutPacket( SMSG_CAST_FAILED, sizeof( packetSMSG_CASTRESULT ), &pe );
	}
}

// uint16 0xFFFF
enum SpellStartFlags
{
    //0x01
    SPELL_START_FLAG_DEFAULT = 0x02, // atm set as default flag
    //0x04
    //0x08
    //0x10
    SPELL_START_FLAG_RANGED = 0x20,
    //0x40
    //0x80
    //0x100
    //0x200
    //0x400
    //0x800
    //0x1000
    //0x2000
    //0x4000
    //0x8000
};

void Spell::SendSpellStart()
{
	// no need to send this on passive spells
	if( !m_caster->IsInWorld() || m_spellInfo->Attributes & 64 || m_triggeredSpell )
		return;

	//WorldPacket data( 150 );
	uint8 buf[1000];			// should be more than enough
	StackPacket data(SMSG_SPELL_START, buf, 1000);

	uint16 cast_flags = 2;

	if( GetType() == SPELL_DMG_TYPE_RANGED )
		cast_flags |= 0x20;

    // hacky yeaaaa
    if( m_spellInfo->Id == 8326 ) // death
        cast_flags = 0x0F;

	if( i_caster != NULL )
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	else
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();

	data << m_spellInfo->Id;
	data << extra_cast_number;
	data << cast_flags;
	data << (uint32)m_castTime;
		
	m_targets.write( data );

	if( GetType() == SPELL_DMG_TYPE_RANGED )
	{
		ItemPrototype* ip = NULL;
        if( m_spellInfo->Id == SPELL_RANGED_THROW ) // throw
        {
			if( p_caster != NULL )
			{
				Item *itm = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( itm != NULL )
				{
	                ip = itm->GetProto();
					/* Throwing Weapon Patch by Supalosa
					p_caster->GetItemInterface()->RemoveItemAmt(it->GetEntry(),1);
					(Supalosa: Instead of removing one from the stack, remove one from durability)
					We don't need to check if the durability is 0, because you can't cast the Throw spell if the thrown weapon is broken, because it returns "Requires Throwing Weapon" or something.
					*/
	
					// burlex - added a check here anyway (wpe suckers :P)
					if( itm->GetDurability() > 0 )
					{
	                    itm->SetDurability( itm->GetDurability() - 1 );
						if( itm->GetDurability() == 0 )
							p_caster->ApplyItemMods( itm, EQUIPMENT_SLOT_RANGED, false, true );
					}
				}
				else
				{
					ip = ItemPrototypeStorage.LookupEntry( 2512 );	/*rough arrow*/
				}
            }
        }
        else if( m_spellInfo->Flags4 & FLAGS4_PLAYER_RANGED_SPELLS )
        {
			if( p_caster != NULL )
				ip = ItemPrototypeStorage.LookupEntry( p_caster->GetUInt32Value( PLAYER_AMMO_ID ) );
			else
				ip = ItemPrototypeStorage.LookupEntry( 2512 );	/*rough arrow*/
        }
		
		if( ip != NULL )
			data << ip->DisplayInfoID << ip->InventoryType;
	}

	m_caster->SendMessageToSet( &data, true );
}

/************************************************************************/
/* General Spell Go Flags, for documentation reasons                    */
/************************************************************************/
enum SpellGoFlags
{
    //0x01
    //0x02
    //0x04
    //0x08
    //0x10
    SPELL_GO_FLAGS_RANGED           = 0x20,
    //0x40
    //0x80
    SPELL_GO_FLAGS_ITEM_CASTER      = 0x100,
    //0x200
    SPELL_GO_FLAGS_EXTRA_MESSAGE    = 0x400, //TARGET MISSES AND OTHER MESSAGES LIKE "Resist"
    //0x800
    //0x1000
    //0x2000
    //0x4000
    //0x8000
};

void Spell::SendSpellGo()
{
    // no need to send this on passive spells
    if( !m_caster->IsInWorld() || m_spellInfo->Attributes & 64 )
        return;

	// Start Spell
	/*WorldPacket data( 200 );
	data.SetOpcode( SMSG_SPELL_GO );
	uint16 flags = 0;

	if ( GetType() == SPELL_DMG_TYPE_RANGED )
		flags |= 0x20; // 0x20 RANGED

	if( i_caster != NULL )
		flags |= SPELL_GO_FLAGS_ITEM_CASTER; // 0x100 ITEM CASTER

    // hacky..
    if( m_spellInfo->Id == 8326 ) // death
        flags = SPELL_GO_FLAGS_ITEM_CASTER | 0x0D;
    
	if( i_caster != NULL && u_caster != NULL ) // this is needed for correct cooldown on items
	{
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	} 
	else 
	{
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();
	}

	data << m_spellInfo->Id;
	data << flags;
	data << getMSTime();
	writeSpellGoTargets(&data);

	m_targets.write( data ); // this write is included the target flag

	// er why handle it being null inside if if you can't get into if if its null
	if( GetType() == SPELL_DMG_TYPE_RANGED )
	{
		ItemPrototype* ip = NULL;
		if( m_spellInfo->Id == SPELL_RANGED_THROW )
		{
			if( p_caster != NULL )
			{
				Item* it = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( it != NULL )
					ip = it->GetProto();
			}
			else
			{
				ip = ItemPrototypeStorage.LookupEntry(2512);	//rough arrow
			}
        }
		else
		{
			if( p_caster != NULL )
				ip = ItemPrototypeStorage.LookupEntry(p_caster->GetUInt32Value( PLAYER_AMMO_ID ) );
			else // HACK FIX
				ip = ItemPrototypeStorage.LookupEntry(2512);	//rough arrow
		}
		if( ip != NULL)
			data << ip->DisplayInfoID << ip->InventoryType;
	}
   
	m_caster->SendMessageToSet( &data, true );*/

	ItemPrototype* ip = NULL;
	uint16 flags = (m_triggeredSpell && !(m_spellInfo->Attributes & ATTRIBUTE_ON_NEXT_ATTACK)) ? 0x0105 : 0x0100;
	SpellTargetList::iterator itr;
	uint32 counter;

	if( i_caster != NULL )
		flags |= SPELL_GO_FLAGS_ITEM_CASTER; // 0x100 ITEM CASTER

	// hacky..
	if( m_spellInfo->Id == 8326 ) // death
		flags = SPELL_GO_FLAGS_ITEM_CASTER | 0x0D;

	if( GetType() == SPELL_DMG_TYPE_RANGED )
	{
		flags |= 0x20; // 0x20 RANGED
		if( m_spellInfo->Id == SPELL_RANGED_THROW )
		{
			if( p_caster != NULL )
			{
				Item* it = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( it != NULL )
					ip = it->GetProto();
			}
			else
			{
				ip = ItemPrototypeStorage.LookupEntry(2512);	//rough arrow
			}
		}
		else
		{
			if( p_caster != NULL )
				ip = ItemPrototypeStorage.LookupEntry(p_caster->GetUInt32Value( PLAYER_AMMO_ID ) );
			else // HACK FIX
				ip = ItemPrototypeStorage.LookupEntry(2512);	//rough arrow
		}
	}

	// stack-based solution
	// this has room for every type of spell target type, 
	// as well as 100 missed and 100 hit targets.
	// if you're doing that, you're NUTS.
	uint8 packet[1882];
	StackPacket data(SMSG_SPELL_GO, packet, 1882);
	
	if( i_caster != NULL && u_caster != NULL ) // this is needed for correct cooldown on items
		data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	else 
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();
	
	data << m_spellInfo->Id;
	data << flags;
	data << getMSTime();

	/************************************************************************/
	/* spell go targets                                                     */
	/************************************************************************/
	// Make sure we don't hit over 100 targets.
	// It's fine internally, but sending it to the client will REALLY cause it to freak.

	data << uint8(m_hitTargetCount);
	if( m_hitTargetCount > 0 )
	{
		counter = 0;
		for( itr = m_targetList.begin(); itr != m_targetList.end() && counter < 100; ++itr )
		{
			if( itr->HitResult == SPELL_DID_HIT_SUCCESS )
			{
				data << itr->Guid;
				++counter;
			}
		}
	}

	data << uint8(m_missTargetCount);
	if( m_missTargetCount > 0 )
	{
		counter = 0;
		for( itr = m_targetList.begin(); itr != m_targetList.end() && counter < 100; ++itr )
		{
			if( itr->HitResult != SPELL_DID_HIT_SUCCESS )
			{
				data << itr->Guid;
				data << itr->HitResult;
				if( itr->HitResult == SPELL_DID_HIT_REFLECT )
					data << uint8(SPELL_DID_HIT_SUCCESS);

				++counter;
			}
		}
	}

	m_targets.write( data ); // this write is included the target flag

	if( ip != NULL)
		data << ip->DisplayInfoID << ip->InventoryType;

	m_caster->SendMessageToSet( &data, true );
	// spell log execute is still send 2.08
	// as I see with this combination, need to test it more
	//if (flags != 0x120 && m_spellInfo->Attributes & 16) // not ranged and flag 5
	  //  SendLogExecute(0,m_targets.m_unitTarget);
}
void Spell::writeSpellGoTargets( WorldPacket * data )
{
	SpellTargetList::iterator itr;
	uint32 counter;

	// Make sure we don't hit over 100 targets.
	// It's fine internally, but sending it to the client will REALLY cause it to freak.

	*data << uint8(m_hitTargetCount);
	if( m_hitTargetCount > 0 )
	{
		counter = 0;
		for( itr = m_targetList.begin(); itr != m_targetList.end() && counter < 100; ++itr )
		{
			if( itr->HitResult == SPELL_DID_HIT_SUCCESS )
			{
				*data << itr->Guid;
				++counter;
			}
		}
	}

	*data << uint8(m_missTargetCount);
	if( m_missTargetCount > 0 )
	{
		counter = 0;
		for( itr = m_targetList.begin(); itr != m_targetList.end() && counter < 100; ++itr )
		{
			if( itr->HitResult != SPELL_DID_HIT_SUCCESS )
			{
				*data << itr->Guid;
				++counter;
			}
		}
	}
}

void Spell::SendLogExecute(uint32 damage, uint64 & targetGuid)
{
	WorldPacket data(SMSG_SPELLLOGEXECUTE, 37);
	data << m_caster->GetNewGUID();
	data << m_spellInfo->Id;
	data << uint32(1);
//	data << m_spellInfo->SpellVisual;
//	data << uint32(1);
	if (m_caster->GetGUID() != targetGuid)
		data << targetGuid;
	if (damage)
		data << damage;
	m_caster->SendMessageToSet(&data,true);
}

void Spell::SendInterrupted(uint8 result)
{
	SetSpellFailed();

	if(!m_caster->IsInWorld()) return;

	//WorldPacket data(SMSG_SPELL_FAILURE, 20);
	uint8 buf[50];
	StackPacket data(SMSG_SPELL_FAILURE, buf, 50);

	// send the failure to pet owner if we're a pet
	Player *plr = p_caster;
	if(!plr && m_caster->IsPet())
		plr = static_cast<Pet*>(m_caster)->GetPetOwner();
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;

	if(plr&&plr->IsPlayer())
	{
		data << m_caster->GetNewGUID();
		data << m_spellInfo->Id;
		data << uint8(result);
		plr->GetSession()->SendPacket(&data);
	}

	data.Initialize(SMSG_SPELL_FAILED_OTHER);
	data << m_caster->GetNewGUID();
	data << m_spellInfo->Id;
	m_caster->SendMessageToSet(&data, false);
}

void Spell::SendChannelUpdate(uint32 time)
{
	if(time == 0)
	{
		if(u_caster && u_caster->IsInWorld() && u_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
		{
			DynamicObject* dynObj=u_caster->GetMapMgr()->GetDynamicObject(u_caster->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
			if(dynObj)
			{
				dynObj->RemoveFromWorld(true);
				delete dynObj;
			}
		}

		u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,0);
		u_caster->SetUInt32Value(UNIT_CHANNEL_SPELL,0);
	}

	if (!p_caster)
		return;

	/*WorldPacket data(MSG_CHANNEL_UPDATE, 18);*/
	uint8 buf[20];
	StackPacket data(MSG_CHANNEL_UPDATE, buf, 20);

	data << p_caster->GetNewGUID();
	data << time;	
	p_caster->SendMessageToSet(&data, true);	
}

void Spell::SendChannelStart(uint32 duration)
{
	if (m_caster->GetTypeId() != TYPEID_GAMEOBJECT)
	{
		// Send Channel Start
		/*WorldPacket data(MSG_CHANNEL_START, 22);*/
		uint8 buf[30];
		StackPacket data(MSG_CHANNEL_START, buf, 30);
		data << m_caster->GetNewGUID();
		data << m_spellInfo->Id;
		data << duration;
		m_caster->SendMessageToSet(&data, true);
	}

	m_castTime = m_timer = duration;

	if( u_caster != NULL )
		u_caster->SetUInt32Value(UNIT_CHANNEL_SPELL,m_spellInfo->Id);

	/*
	Unit* target = objmgr.GetCreature( static_cast< Player* >( m_caster )->GetSelection());
	if(!target)
		target = objmgr.GetObject<Player>( static_cast< Player* >( m_caster )->GetSelection());
	if(!target)
		return;
 
	m_caster->SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT,target->GetGUIDLow());
	m_caster->SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT+1,target->GetGUIDHigh());
	//disabled it can be not only creature but GO as well
	//and GO is not selectable, so this method will not work
	//these fields must be filled @ place of call
	*/
}

void Spell::SendResurrectRequest(Player* target)
{
	WorldPacket data(SMSG_RESURRECT_REQUEST, 13);
	data << m_caster->GetGUID();
	data << uint32(0) << uint8(0);

	target->GetSession()->SendPacket(&data);
}

bool Spell::HasPower()
{
	int32 powerField;
	if( u_caster != NULL )
		if(u_caster->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER))
			return true;

	if(p_caster && p_caster->PowerCheat)
		return true;

	switch(m_spellInfo->powerType)
	{
	case POWER_TYPE_HEALTH:{
		powerField = UNIT_FIELD_HEALTH;
						   }break;
	case POWER_TYPE_MANA:{
		powerField = UNIT_FIELD_POWER1;
		m_usesMana=true;
						 }break;
	case POWER_TYPE_RAGE:{
		powerField = UNIT_FIELD_POWER2;
						 }break;
	case POWER_TYPE_FOCUS:{
		powerField = UNIT_FIELD_POWER3;
						  }break;
	case POWER_TYPE_ENERGY:{
		powerField = UNIT_FIELD_POWER4;
						   }break;
	default:{
		DEBUG_LOG("unknown power type");
		// we should'nt be here to return
		return false;
			}break;
	}
	

	//FIXME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER
	if( u_caster != NULL )
	{
		if( m_spellInfo->AttributesEx & ATTRIBUTESEX_DRAIN_WHOLE_MANA ) // Uses %100 mana
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
	}

	int32 currentPower = m_caster->GetUInt32Value(powerField);

	int32 cost;
	if( m_spellInfo->ManaCostPercentage)//Percentage spells cost % of !!!BASE!!! mana
	{
		if( m_spellInfo->powerType==POWER_TYPE_MANA)
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*m_spellInfo->ManaCostPercentage)/100;
		else
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_HEALTH)*m_spellInfo->ManaCostPercentage)/100;
	}
	else 
	{
		cost = m_spellInfo->manaCost;
	}

	if((int32)m_spellInfo->powerType==POWER_TYPE_HEALTH)
		cost -= m_spellInfo->baseLevel;//FIX for life tap	
	else if( u_caster != NULL )
	{
		if( m_spellInfo->powerType==POWER_TYPE_MANA)
			cost += u_caster->PowerCostMod[m_spellInfo->School];//this is not percent!
		else
			cost += u_caster->PowerCostMod[0];
		cost +=float2int32(cost*u_caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+m_spellInfo->School));
	}

	//apply modifiers
	if( m_spellInfo->SpellGroupType && u_caster)
	{
		SM_FIValue(u_caster->SM_FCost,&cost,m_spellInfo->SpellGroupType);
		SM_PIValue(u_caster->SM_PCost,&cost,m_spellInfo->SpellGroupType);
	}

	if (cost <=0)
		return true;

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		return true;
	}
	else
	{
		if(cost <= currentPower) // Unit has enough power (needed for creatures)
		{
			return true;
		}
		else
			return false;	 
	} 
}

bool Spell::TakePower()
{
	int32 powerField;
	if( u_caster != NULL )
	if(u_caster->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER))
		return true;

	if(p_caster && p_caster->PowerCheat)
		return true;

	switch(m_spellInfo->powerType)
	{
		case POWER_TYPE_HEALTH:{
			powerField = UNIT_FIELD_HEALTH;
							   }break;
		case POWER_TYPE_MANA:{
			powerField = UNIT_FIELD_POWER1;
			m_usesMana=true;
							 }break;
		case POWER_TYPE_RAGE:{
			powerField = UNIT_FIELD_POWER2;
							 }break;
		case POWER_TYPE_FOCUS:{
			powerField = UNIT_FIELD_POWER3;
							  }break;
		case POWER_TYPE_ENERGY:{
			powerField = UNIT_FIELD_POWER4;
							  }break;
		default:{
			DEBUG_LOG("unknown power type");
			// we should'nt be here to return
			return false;
				}break;
	}

	//FIXME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER
	if( u_caster != NULL )
	{
		if( m_spellInfo->AttributesEx & ATTRIBUTESEX_DRAIN_WHOLE_MANA ) // Uses %100 mana
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
	}
	   
	int32 currentPower = m_caster->GetUInt32Value(powerField);

	int32 cost;
	if( m_spellInfo->ManaCostPercentage)//Percentage spells cost % of !!!BASE!!! mana
	{
		if( m_spellInfo->powerType==POWER_TYPE_MANA)
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*m_spellInfo->ManaCostPercentage)/100;
		else
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_HEALTH)*m_spellInfo->ManaCostPercentage)/100;
	}
	else 
	{
		cost = m_spellInfo->manaCost;
	}
	
	if((int32)m_spellInfo->powerType==POWER_TYPE_HEALTH)
			cost -= m_spellInfo->baseLevel;//FIX for life tap	
	else if( u_caster != NULL )
	{
		if( m_spellInfo->powerType==POWER_TYPE_MANA)
			cost += u_caster->PowerCostMod[m_spellInfo->School];//this is not percent!
		else
			cost += u_caster->PowerCostMod[0];
		cost +=float2int32(cost*u_caster->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+m_spellInfo->School));
	}

	//apply modifiers
	if( m_spellInfo->SpellGroupType && u_caster)
	{
		  SM_FIValue(u_caster->SM_FCost,&cost,m_spellInfo->SpellGroupType);
		  SM_PIValue(u_caster->SM_PCost,&cost,m_spellInfo->SpellGroupType);
	}

	if (cost <=0)
		return true;

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		m_caster->DealDamage(u_caster, cost, 0, 0, 0,true);
		return true;
	}
	else
	{
		if(cost <= currentPower) // Unit has enough power (needed for creatures)
		{
			m_caster->SetUInt32Value(powerField, currentPower - cost);
			return true;
		}
		else
			return false;	 
	} 
}

Object * Spell::_LookupObject(const uint64& guid)
{
	if( guid == m_caster->GetGUID() )
		return m_caster;

	switch(GET_TYPE_FROM_GUID(guid))
	{
	case HIGHGUID_TYPE_ITEM:
		{
			if( p_caster != NULL )
				return p_caster->GetItemInterface()->GetItemByGUID( (uint64)guid );
		}break;

	case HIGHGUID_TYPE_CORPSE:
		{
			return objmgr.GetCorpse((uint32)guid);
		}break;

	default:
		{
			if( m_caster->IsInWorld() )
				return m_caster->GetMapMgr()->GetUnit(guid);
		}break;
	}

	return NULL;
}

void Spell::_SetTargets(const uint64& guid)
{
	if(guid == m_caster->GetGUID())
	{
		unitTarget = u_caster;
		gameObjTarget = g_caster;
		playerTarget = p_caster;
		itemTarget = i_caster;
	}
	else
	{
		if(!m_caster->IsInWorld())
		{
			unitTarget = NULL;
			playerTarget = NULL;
			itemTarget = NULL;
			gameObjTarget = NULL;
			corpseTarget = NULL;
		}
		else if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
		{
			if( p_caster != NULL )
			{
				Player * plr = p_caster->GetTradeTarget();
				if(plr)
					itemTarget = plr->getTradeItem((uint32)guid);
			}
		}
		else
		{
			unitTarget = NULL;
			switch(GET_TYPE_FROM_GUID(guid))
			{
			case HIGHGUID_TYPE_UNIT:
				unitTarget = m_caster->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_PET:
				unitTarget = m_caster->GetMapMgr()->GetPet(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_PLAYER:
				{
					unitTarget =  m_caster->GetMapMgr()->GetPlayer((uint32)guid);
					playerTarget = static_cast< Player* >(unitTarget);
				}break;
			case HIGHGUID_TYPE_ITEM:
				if( p_caster != NULL )
					itemTarget = p_caster->GetItemInterface()->GetItemByGUID(guid);

				break;
			case HIGHGUID_TYPE_GAMEOBJECT:
				gameObjTarget = m_caster->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_CORPSE:
				corpseTarget = objmgr.GetCorpse((uint32)guid);
				break;
			}
		}
	}
}

void Spell::HandleEffects(uint32 i)
{   
	damage = CalculateEffect(i,unitTarget);  
	DEBUG_LOG( "WORLD: Spell effect id = %u, damage = %d", m_spellInfo->Effect[i], damage); 
	
	if( m_spellInfo->Effect[i]<TOTAL_SPELL_EFFECTS)
		(*this.*SpellEffectsHandler[m_spellInfo->Effect[i]])(i);
	else
		DEBUG_LOG("SPELL: unknown effect %u spellid %u",m_spellInfo->Effect[i], m_spellInfo->Id);
}

void Spell::HandleAddAura(uint64 guid)
{
	Unit * Target = 0;
	if(guid == 0)
		return;

	if(u_caster && u_caster->GetGUID() == guid)
		Target = u_caster;
	else if(m_caster->IsInWorld())
		Target = m_caster->GetMapMgr()->GetUnit(guid);

	if(!Target)
		return;

	// Applying an aura to a flagged target will cause you to get flagged.
    // self casting doesnt flag himself.
	if(Target->IsPlayer() && p_caster && p_caster != static_cast< Player* >(Target))
	{
		if(static_cast< Player* >(Target)->IsPvPFlagged())
			p_caster->SetPvPFlag();
	}

	// remove any auras with same type
	if( m_spellInfo->buffType > 0)
		Target->RemoveAurasByBuffType(m_spellInfo->buffType, m_caster->GetGUID(),m_spellInfo->Id);

	uint32 spellid = 0;

	if( m_spellInfo->MechanicsType == 25 && m_spellInfo->Id != 25771) // Cast spell Forbearance
		spellid = 25771;
	else if( m_spellInfo->MechanicsType == 16 && m_spellInfo->Id != 11196) // Cast spell Recently Bandaged
		spellid = 11196;
	else if( m_spellInfo->MechanicsType == 19 && m_spellInfo->Id != 6788) // Cast spell Weakened Soul
		spellid = 6788;
	else if( m_spellInfo->Id == 45438) // Cast spell Hypothermia
		spellid = 41425;
	else if( m_spellInfo->Id == 30451) // Cast spell Arcane Blast
		spellid = 36032;
	else if( m_spellInfo->Id == 20572 || m_spellInfo->Id == 33702 || m_spellInfo->Id == 33697) // Cast spell Blood Fury
		spellid = 23230;
	else if( m_spellInfo->Id == 31884)
		spellid = 25771;

	if(spellid && p_caster)
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry( spellid );
		if(!spellInfo) return;
		Spell *spell = new Spell(p_caster, spellInfo ,true, NULL);
		SpellCastTargets targets(Target->GetGUID());
		spell->prepare(&targets);	
	}

	// avoid map corruption
	if(Target->GetInstanceID()!=m_caster->GetInstanceID())
		return;

	std::map<uint32,Aura*>::iterator itr=Target->tmpAura.find(m_spellInfo->Id);
	if(itr!=Target->tmpAura.end())
	{
		if(itr->second != NULL)
		{
			// did our effects kill the target?
			if( Target->isDead() && !(m_spellInfo->Flags4 & CAN_PERSIST_AND_CASTED_WHILE_DEAD))
			{
				// free pointer
				delete itr->second;
				Target->tmpAura.erase(itr);
				return;
			}

			if(itr->second->GetSpellProto()->procCharges>0)
			{
				Aura *aur=NULL;
				for(int i=0;i<itr->second->GetSpellProto()->procCharges-1;i++)
				{
					aur = new Aura(itr->second->GetSpellProto(),itr->second->GetDuration(),itr->second->GetCaster(),itr->second->GetTarget());
					Target->AddAura(aur, NULL);
					aur=NULL;
				}
				if(!(itr->second->GetSpellProto()->procFlags & PROC_REMOVEONUSE))
				{
					SpellCharge charge;
					charge.count=itr->second->GetSpellProto()->procCharges;
					charge.spellId=itr->second->GetSpellId();
					charge.ProcFlag=itr->second->GetSpellProto()->procFlags;
					charge.lastproc = 0;
					Target->m_chargeSpells.insert(make_pair(itr->second->GetSpellId(),charge));
				}
			}
			Target->AddAura(itr->second, NULL); // the real spell is added last so the modifier is removed last
			Target->tmpAura.erase(itr);
		}
	}
}

/*
void Spell::TriggerSpell()
{
	if(TriggerSpellId != 0)
	{
		// check for spell id
		SpellEntry *spellInfo = sSpellStore.LookupEntry(TriggerSpellId );

		if(!spellInfo)
		{
			sLog.outError("WORLD: unknown spell id %i\n", TriggerSpellId);
			return;
		}

		Spell *spell = new Spell(m_caster, spellInfo,false, NULL);
		WPAssert(spell);

		SpellCastTargets targets;
		if(TriggerSpellTarget)
			targets.m_unitTarget = TriggerSpellTarget;
		else
			targets.m_unitTarget = m_targets.m_unitTarget;

		spell->prepare(&targets);
	}
}*/
/*
void Spell::DetermineSkillUp()
{
	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);
	if (m_caster->GetTypeId() == TYPEID_PLAYER)
	{
		if ((skill) && static_cast< Player* >( m_caster )->HasSkillLine( skill->skilline ) )
		{
			uint32 amt = static_cast< Player* >( m_caster )->GetBaseSkillAmt(skill->skilline);
			uint32 max = static_cast< Player* >( m_caster )->GetSkillMax(skill->skilline);
			if (amt >= skill->grey) //grey
			{
			}
			else if ((amt >= (((skill->grey - skill->green) / 2) + skill->green)) && (amt < max))
			{
				if (rand()%100 < 33) //green
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);	

			}
			else if ((amt >= skill->green) && (amt < max))
			{
				if (rand()%100 < 66) //yellow
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);		

			}
			else 
			{
				if (amt < max) //brown
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);
			}
		}
	}
}
*/

bool Spell::IsAspect()
{
	return (
		(m_spellInfo->Id ==  2596) || (m_spellInfo->Id ==  5118) || (m_spellInfo->Id == 14320) || (m_spellInfo->Id == 13159) || (m_spellInfo->Id == 13161) || (m_spellInfo->Id == 20190) || 
		(m_spellInfo->Id == 20043) || (m_spellInfo->Id == 14322) || (m_spellInfo->Id == 14321) || (m_spellInfo->Id == 13163) || (m_spellInfo->Id == 14319) || (m_spellInfo->Id == 14318) || (m_spellInfo->Id == 13165)); 
}

bool Spell::IsSeal()
{
	return (
		(m_spellInfo->Id == 13903) || (m_spellInfo->Id == 17177) || (m_spellInfo->Id == 20154) || (m_spellInfo->Id == 20162) || (m_spellInfo->Id == 20164) || (m_spellInfo->Id == 20165) || 
		(m_spellInfo->Id == 20166) || (m_spellInfo->Id == 20287) || (m_spellInfo->Id == 20288) || (m_spellInfo->Id == 20289) || (m_spellInfo->Id == 20290) || (m_spellInfo->Id == 20291) || 
		(m_spellInfo->Id == 20292) || (m_spellInfo->Id == 20293) || (m_spellInfo->Id == 20305) || (m_spellInfo->Id == 20306) || (m_spellInfo->Id == 20307) || (m_spellInfo->Id == 20308) || 
		(m_spellInfo->Id == 20347) || (m_spellInfo->Id == 20348) || (m_spellInfo->Id == 20349) || (m_spellInfo->Id == 20356) || (m_spellInfo->Id == 20357) || (m_spellInfo->Id == 20375) || 
		(m_spellInfo->Id == 20915) || (m_spellInfo->Id == 20918) || (m_spellInfo->Id == 20919) || (m_spellInfo->Id == 20920) || (m_spellInfo->Id == 21082) || (m_spellInfo->Id == 21084)); 
}

uint8 Spell::CanCast(bool tolerate)
{
	uint32 i;
	if(objmgr.IsSpellDisabled(m_spellInfo->Id))
		return SPELL_FAILED_SPELL_UNAVAILABLE;

	if( u_caster && u_caster->GetCurrentSpell() != NULL && u_caster->GetCurrentSpell() != this && !m_triggeredSpell )
		return SPELL_FAILED_SPELL_IN_PROGRESS;

	if( p_caster != NULL )
	{
		// flying auras
		if( m_spellInfo->c_is_flags & SPELL_FLAG_IS_FLYING )
		{
			// can't fly in non-flying zones
			if( p_caster->GetAreaDBC() != NULL && !(p_caster->GetAreaDBC()->AreaFlags & AREA_FLYING_PERMITTED) )
			{
				return SPELL_FAILED_NOT_HERE;
			}
		}

		if( m_spellInfo->Id == 33076 )			// Prayer of mending
		{
			if( p_caster->GetGroup() != NULL && p_caster->GetGroup()->m_prayerOfMendingCount )
			{
				Player *tmp_plr_pom = p_caster->IsInWorld() ? p_caster->GetMapMgr()->GetPlayer(p_caster->GetGroup()->m_prayerOfMendingTarget) : NULL;
				if( tmp_plr_pom != NULL )		// remove from current target.
					tmp_plr_pom->RemoveAura(41635);

				p_caster->GetGroup()->m_prayerOfMendingTarget = 0;
				p_caster->GetGroup()->m_prayerOfMendingCount = 0;
			}
		}

		uint32 self_rez = p_caster->GetUInt32Value(PLAYER_SELF_RES_SPELL);
		// if theres any spells that should be cast while dead let me know
		if( !p_caster->isAlive() && self_rez != m_spellInfo->Id)
		{
			// spirit of redemption
			if( p_caster->m_canCastSpellsWhileDead )
			{
				// casting a spell on self
				if( m_targets.m_targetMask & TARGET_FLAG_SELF || m_targets.m_unitTarget == p_caster->GetGUID() ||
					!IsHealingSpell(m_spellInfo) )		// not a holy spell
				{
					return SPELL_FAILED_SPELL_UNAVAILABLE;
				}
			}
			else		// not SOR
				return SPELL_FAILED_CASTER_DEAD;
		}

#ifdef COLLISION
		if (m_spellInfo->MechanicsType == MECHANIC_MOUNTED)
		{
			if (CollideInterface.IsIndoor( p_caster->GetMapId(), p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ() ))
				return SPELL_FAILED_NO_MOUNTS_ALLOWED;
		}
		else if( m_spellInfo->Attributes & ATTRIBUTES_ONLY_OUTDOORS )
		{
			if( !CollideInterface.IsOutdoor( p_caster->GetMapId(),p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ() ) )
				return SPELL_FAILED_ONLY_OUTDOORS;
		}
#endif
		//are we in an arena and the spell cooldown is longer then 15mins?
		if ( p_caster->m_bg && ( p_caster->m_bg->GetType() >= BATTLEGROUND_ARENA_2V2 && p_caster->m_bg->GetType() <= BATTLEGROUND_ARENA_5V5 ) )
		{
			if( ( m_spellInfo->RecoveryTime >= 900000 || m_spellInfo->CategoryRecoveryTime >= 900000 ) )
				return SPELL_FAILED_NOT_IN_ARENA;

			// block resurrect spells
			/*if( m_spellInfo->Effect[0] == SPELL_EFFECT_RESURRECT || m_spellInfo->Effect[1] == SPELL_EFFECT_RESURRECT || m_spellInfo->Effect[2] == SPELL_EFFECT_RESURRECT ||
				m_spellInfo->Effect[0] == SPELL_EFFECT_RESURRECT_FLAT || m_spellInfo->Effect[1] == SPELL_EFFECT_RESURRECT_FLAT || m_spellInfo->Effect[2] == SPELL_EFFECT_RESURRECT_FLAT )
			{
				return SPELL_FAILED_NOT_IN_ARENA;
			}*/

			if( !p_caster->m_bg->HasStarted() )
			{
				// cannot cast spells in an arena if it hasn't started (blinking through gates, lalala)
				if( m_spellInfo->NameHash == SPELL_HASH_BLINK )
					return SPELL_FAILED_SPELL_UNAVAILABLE;
			}
		}

		// What a waste.
		if( p_caster->m_bg && GetGameObjectTarget())
		{
			if(p_caster->SchoolImmunityList[0]) // Physical is all that really matters.
				return SPELL_FAILED_SPELL_UNAVAILABLE;
		}

		// backstab/ambush
		if( m_spellInfo->NameHash == SPELL_HASH_BACKSTAB || m_spellInfo->NameHash == SPELL_HASH_AMBUSH )
		{
			if( m_spellInfo->NameHash == SPELL_HASH_AMBUSH && !p_caster->IsStealth() )
				return SPELL_FAILED_ONLY_STEALTHED;

			Item * pMainHand = p_caster->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( !pMainHand || pMainHand->GetProto()->Class != 2 || pMainHand->GetProto()->SubClass != 15 )
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND;
		}

		// check for cooldowns
		if(!tolerate && !p_caster->Cooldown_CanCast(m_spellInfo))
				return SPELL_FAILED_NOT_READY;

		if(p_caster->GetDuelState() == DUEL_STATE_REQUESTED)
		{
			for(i = 0; i < 3; ++i)
			{
				if( m_spellInfo->Effect[i] && m_spellInfo->Effect[i] != SPELL_EFFECT_APPLY_AURA && m_spellInfo->Effect[i] != SPELL_EFFECT_APPLY_PET_AURA
					&& m_spellInfo->Effect[i] != SPELL_EFFECT_APPLY_AREA_AURA)
				{
					return SPELL_FAILED_TARGET_DUELING;
				}
			}
		}

		// check for duel areas
		if(p_caster && m_spellInfo->Id == 7266 )
		{
			AreaTable* at = dbcArea.LookupEntry( p_caster->GetAreaID() );
			if(at->AreaFlags & AREA_CITY_AREA)
				return SPELL_FAILED_NO_DUELING;

			if( p_caster->m_bg )
				return SPELL_FAILED_NO_DUELING;
		}

		// check if spell is allowed while player is on a taxi
		if(p_caster->m_onTaxi)
		{
			if( m_spellInfo->Id != 33836) // exception for Area 52 Special
				return SPELL_FAILED_NOT_ON_TAXI;
		}

		// check if spell is allowed while player is on a transporter
		if(p_caster->m_CurrentTransporter)
		{
			// no mounts while on transporters
			if( m_spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOUNTED || m_spellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOUNTED || m_spellInfo->EffectApplyAuraName[2] == SPELL_AURA_MOUNTED)
				return SPELL_FAILED_NOT_ON_TRANSPORT;
		}

		// check if spell is allowed while not mounted
		if(!p_caster->IsMounted())
		{
			if( m_spellInfo->Id == 25860) // Reindeer Transformation
				return SPELL_FAILED_ONLY_MOUNTED;
		}
		else
		{
			if (!(m_spellInfo->Attributes & ATTRIBUTES_MOUNT_CASTABLE))
				return SPELL_FAILED_NOT_MOUNTED;
		}

		// check if spell is allowed while shapeshifted
		if(p_caster->GetShapeShift())
		{
			switch(p_caster->GetShapeShift())
			{
				case FORM_TREE:
				case FORM_BATTLESTANCE:
				case FORM_DEFENSIVESTANCE:
				case FORM_BERSERKERSTANCE:
				case FORM_SHADOW:
				case FORM_STEALTH:
				case FORM_MOONKIN:
				{
					break;
				}

				case FORM_SWIFT:
				case FORM_FLIGHT:
				{
					// check if item is allowed (only special items allowed in flight forms)
					if(i_caster && !(i_caster->GetProto()->Flags & ITEM_FLAG_SHAPESHIFT_OK))
						return SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED;

					break;
				}

				//case FORM_CAT: 
				//case FORM_TRAVEL:
				//case FORM_AQUA:
				//case FORM_BEAR:
				//case FORM_AMBIENT:
				//case FORM_GHOUL:
				//case FORM_DIREBEAR:
				//case FORM_CREATUREBEAR:
				//case FORM_GHOSTWOLF:
				//case FORM_SPIRITOFREDEMPTION:

				default:
				{
					// check if item is allowed (only special & equipped items allowed in other forms)
					if(i_caster && !(i_caster->GetProto()->Flags & ITEM_FLAG_SHAPESHIFT_OK))
						if(i_caster->GetProto()->InventoryType == INVTYPE_NON_EQUIP)
							return SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED;
				}
			}
		}

		// check if spell is allowed while we have a battleground flag
		if(p_caster->m_bgHasFlag)
		{
			switch(m_spellInfo->Id)
			{
				// stealth spells
				case 1784:
				case 1785:
				case 1786:
				case 1787:
				case 5215:
				case 6783:
				case 9913:
				case 1856:
				case 1857:
				case 26889:
				{
					return SPELL_FAILED_SPELL_UNAVAILABLE;
					break;
				}
			}
		}

		// item spell checks
		if(i_caster)
		{
			if( i_caster->GetProto()->ZoneNameID && i_caster->GetProto()->ZoneNameID != i_caster->GetZoneId() ) 
				return SPELL_FAILED_NOT_HERE;
			if( i_caster->GetProto()->MapID && i_caster->GetProto()->MapID != i_caster->GetMapId() )
				return SPELL_FAILED_NOT_HERE;

			if(i_caster->GetProto()->Spells[0].Charges != 0)
			{
				// check if the item has the required charges
				if(i_caster->GetUInt32Value(ITEM_FIELD_SPELL_CHARGES) == 0)
					return SPELL_FAILED_NO_CHARGES_REMAIN;

				// for items that combine to create a new item, check if we have the required quantity of the item
				if(i_caster->GetProto()->ItemId == m_spellInfo->Reagent[0])
					if(p_caster->GetItemInterface()->GetItemCount(m_spellInfo->Reagent[0]) < 1 + m_spellInfo->ReagentCount[0])
						return SPELL_FAILED_ITEM_GONE;
			}

			// heal checks are only applied to item casters
			if( m_spellInfo->c_is_flags & SPELL_FLAG_IS_HEALING_SPELL && p_caster->GetUInt32Value(UNIT_FIELD_HEALTH) == p_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH) )
				return SPELL_FAILED_ALREADY_AT_FULL_HEALTH;

			if( p_caster->GetPowerType() == POWER_TYPE_MANA && m_spellInfo->c_is_flags & SPELL_FLAG_IS_HEALING_MANA_SPELL && p_caster->GetUInt32Value(UNIT_FIELD_POWER1) == p_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1) )
				return SPELL_FAILED_ALREADY_AT_FULL_MANA;
		}

		// check if we have the required reagents
		for(i=0; i<8 ;i++)
		{
			if( m_spellInfo->Reagent[i] == 0 || m_spellInfo->ReagentCount[i] == 0)
				continue;

			if(p_caster->GetItemInterface()->GetItemCount(m_spellInfo->Reagent[i]) < m_spellInfo->ReagentCount[i])
				return SPELL_FAILED_ITEM_GONE;
		}

		// check if we have the required tools, totems, etc
		if( m_spellInfo->Totem[0] != 0)
		{
			if(!p_caster->GetItemInterface()->GetItemCount(m_spellInfo->Totem[0]))
				return SPELL_FAILED_TOTEMS;
		}
		if( m_spellInfo->Totem[1] != 0)
		{
			if(!p_caster->GetItemInterface()->GetItemCount(m_spellInfo->Totem[1]))
				return SPELL_FAILED_TOTEMS;
		}

		// stealth check
		if( m_spellInfo->NameHash == SPELL_HASH_STEALTH )
		{
			if( p_caster->CombatStatus.IsInCombat() )
				return SPELL_FAILED_TARGET_IN_COMBAT;
		}

		// check if we have the required gameobject focus
		if( m_spellInfo->RequiresSpellFocus)
		{
			float focusRange;
			// professions use rangeIndex 1, which is 0yds, so we will use 5yds, which is standard interaction range.
			if( m_spellInfo->rangeIndex == 1)
				focusRange = 5.0f;
			else
				focusRange = GetMaxRange(dbcSpellRange.LookupEntry(m_spellInfo->rangeIndex));

			bool found = false;

			for(std::set<Object*>::iterator itr = p_caster->GetInRangeSetBegin(); itr != p_caster->GetInRangeSetEnd(); itr++ )
			{
				if((*itr)->GetTypeId() != TYPEID_GAMEOBJECT)
					continue;

				if((*itr)->GetUInt32Value(GAMEOBJECT_TYPE_ID) != GAMEOBJECT_TYPE_SPELL_FOCUS)
					continue;

				// check if focus object is close enough
				if(!IsInrange(p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), (*itr), (focusRange * focusRange)))
					continue;

				GameObjectInfo *info = ((GameObject*)(*itr))->GetInfo();

				if(!info)
				{
					DEBUG_LOG("Warning: could not find info about game object %u",(*itr)->GetEntry());
					continue;
				}

				if(info->SpellFocus == m_spellInfo->RequiresSpellFocus)
				{
					found = true;
					break;
				}
			}

			if(!found)
				return SPELL_FAILED_REQUIRES_SPELL_FOCUS;
		}

		if (m_spellInfo->RequiresAreaId && m_spellInfo->RequiresAreaId != p_caster->GetMapMgr()->GetAreaID(p_caster->GetPositionX(),p_caster->GetPositionY()))
			return SPELL_FAILED_REQUIRES_AREA;

		// aurastate check
		if( m_spellInfo->CasterAuraState )
		{
			if( !p_caster->HasFlag( UNIT_FIELD_AURASTATE, m_spellInfo->CasterAuraState ) )
				return SPELL_FAILED_CASTER_AURASTATE;
		}
	}

	// Targetted Item Checks
	if(m_targets.m_itemTarget && p_caster)
	{
		Item *i_target = NULL;

		// check if the targeted item is in the trade box
		if( m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM )
		{
			switch( m_spellInfo->Effect[0] )
			{
				// only lockpicking and enchanting can target items in the trade box
				case SPELL_EFFECT_OPEN_LOCK:
				case SPELL_EFFECT_ENCHANT_ITEM:
				case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
				{
					// check for enchants that can only be done on your own items
					if( m_spellInfo->Flags3 & FLAGS3_ENCHANT_OWN_ONLY )
						return SPELL_FAILED_BAD_TARGETS;
					// get the player we are trading with
					Player* t_player = p_caster->GetTradeTarget();
					// get the targeted trade item
					if( t_player != NULL )
						i_target = t_player->getTradeItem((uint32)m_targets.m_itemTarget);
				}
			}
		}
		// targeted item is not in a trade box, so get our own item
		else
		{
			i_target = p_caster->GetItemInterface()->GetItemByGUID( m_targets.m_itemTarget );
		}

		// check to make sure we have a targeted item
		if( i_target == NULL )
			return SPELL_FAILED_BAD_TARGETS;

		ItemPrototype* proto = i_target->GetProto();

		// check to make sure we have it's prototype info
		if(!proto) return SPELL_FAILED_BAD_TARGETS;

		// check to make sure the targeted item is acceptable
		switch(m_spellInfo->Effect[0])
		{
			// Lock Picking Targeted Item Check
			case SPELL_EFFECT_OPEN_LOCK:
			{
				// this is currently being handled in SpellEffects
				break;
			}

			// Enchanting Targeted Item Check
			case SPELL_EFFECT_ENCHANT_ITEM:
			case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
			{
				// check for enchants that can only be done on your own items, make sure they are soulbound
				if( m_spellInfo->Flags3 & FLAGS3_ENCHANT_OWN_ONLY && i_target->GetOwner() != p_caster)
					return SPELL_FAILED_BAD_TARGETS;

				// check if we have the correct class, subclass, and inventory type of target item
				if( m_spellInfo->EquippedItemClass != (int32)proto->Class)
					return SPELL_FAILED_BAD_TARGETS;

				if( m_spellInfo->EquippedItemSubClass && !(m_spellInfo->EquippedItemSubClass & (1 << proto->SubClass)))
					return SPELL_FAILED_BAD_TARGETS;

				if( m_spellInfo->RequiredItemFlags && !(m_spellInfo->RequiredItemFlags & (1 << proto->InventoryType)))
					return SPELL_FAILED_BAD_TARGETS;

				if (m_spellInfo->Effect[0] == SPELL_EFFECT_ENCHANT_ITEM && 
					m_spellInfo->baseLevel && (m_spellInfo->baseLevel > proto->ItemLevel))
					return int8(SPELL_FAILED_BAD_TARGETS); // maybe there is different err code

				break;
			}

			// Disenchanting Targeted Item Check
			case SPELL_EFFECT_DISENCHANT:
			{
				// check if item can be disenchanted
				if(proto->DisenchantReqSkill < 1)
					return SPELL_FAILED_CANT_BE_DISENCHANTED;

				// check if we have high enough skill
				if((int32)p_caster->_GetSkillLineCurrent(SKILL_ENCHANTING) < proto->DisenchantReqSkill)
					return SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL;

				break;
			}

			// Feed Pet Targeted Item Check
			case SPELL_EFFECT_FEED_PET:
			{
				Pet *pPet = p_caster->GetSummon();

				// check if we have a pet
				if(!pPet)
					return SPELL_FAILED_NO_PET;

				// check if item is food
				if(!proto->FoodType)
					return SPELL_FAILED_BAD_TARGETS;

				// check if food type matches pets diet
				if(!(pPet->GetPetDiet() & (1 << (proto->FoodType - 1))))
					return SPELL_FAILED_WRONG_PET_FOOD;

				// check food level: food should be max 30 lvls below pets level
				if(pPet->getLevel() > proto->ItemLevel + 30)
					return SPELL_FAILED_FOOD_LOWLEVEL;

				break;
			}

			// Prospecting Targeted Item Check
			case SPELL_EFFECT_PROSPECTING:
			{
				// check if the item can be prospected
				if(!(proto->Flags & ITEM_FLAG_PROSPECTABLE))
					return SPELL_FAILED_CANT_BE_PROSPECTED;

				// check if we have at least 5 of the item
				if(p_caster->GetItemInterface()->GetItemCount(proto->ItemId) < 5)
					return SPELL_FAILED_ITEM_GONE;

				// check if we have high enough skill
				if(p_caster->_GetSkillLineCurrent(SKILL_JEWELCRAFTING) < proto->RequiredSkillRank)
					return SPELL_FAILED_LOW_CASTLEVEL;

				break;
			}
		}
	}

	// set up our max Range
	//float maxRange = GetMaxRange( dbcSpellRange.LookupEntry( m_spellInfo->rangeIndex ) );
	float maxRange = m_spellInfo->base_range_or_radius;

	if( m_spellInfo->SpellGroupType && u_caster != NULL )
	{
		SM_FFValue( u_caster->SM_FRange, &maxRange, m_spellInfo->SpellGroupType );
		SM_PFValue( u_caster->SM_PRange, &maxRange, m_spellInfo->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		float spell_flat_modifers=0;
		float spell_pct_modifers=0;
		SM_FFValue(u_caster->SM_FRange,&spell_flat_modifers,m_spellInfo->SpellGroupType);
		SM_FFValue(u_caster->SM_PRange,&spell_pct_modifers,m_spellInfo->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!spell range bonus mod flat %f , spell range bonus pct %f , spell range %f, spell group %u\n",spell_flat_modifers,spell_pct_modifers,maxRange,m_spellInfo->SpellGroupType);
#endif
	}

	// Targeted Location Checks (AoE spells)
	if( m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION )
	{
		if( !IsInrange( m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, m_caster, ( maxRange * maxRange ) ) )
			return SPELL_FAILED_OUT_OF_RANGE;
	}

	Unit *target = NULL;
	if( m_targets.m_targetMask == TARGET_FLAG_SELF )
		target = u_caster;

	// Targeted Unit Checks
	if(m_targets.m_unitTarget)
	{
		if( m_targets.m_unitTarget == m_caster->GetGUID() && m_caster->IsUnit() )
			target = TO_UNIT(target);
		else
			target = (m_caster->IsInWorld()) ? m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget) : NULL;

		if(target)
		{
			if( target != m_caster )
			{
				// Partha: +2.52yds to max range, this matches the range the client is calculating.
				// see extra/supalosa_range_research.txt for more info

				if( tolerate ) // add an extra 33% to range on final check (squared = 1.78x)
				{
					if( !IsInrange( m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, ( ( maxRange + 2.52f ) * ( maxRange + 2.52f ) * 1.78f ) ) )
						return SPELL_FAILED_OUT_OF_RANGE;
				}
				else
				{
					if( !IsInrange( m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, ( ( maxRange + 2.52f ) * ( maxRange + 2.52f ) ) ) )
						return SPELL_FAILED_OUT_OF_RANGE;
				}
			}

			if( p_caster != NULL )
			{
				if( m_spellInfo->Id == SPELL_RANGED_THROW)
				{
					Item * itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					if(!itm || itm->GetDurability() == 0)
						return SPELL_FAILED_NO_AMMO;
				}
#ifdef COLLISION
				if( target != m_caster )
				{
					if ( p_caster->GetMapId() == target->GetMapId() &&
						!CollideInterface.CheckLOS(p_caster->GetMapId(),p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), 
						target->GetPositionX(), target->GetPositionY(), target->GetPositionZ()) )
					{
						return SPELL_FAILED_LINE_OF_SIGHT;
					}
				}
#endif

				// check aurastate
				if( m_spellInfo->TargetAuraState )
				{
					if( !target->HasFlag( UNIT_FIELD_AURASTATE, m_spellInfo->TargetAuraState ) )
					{
						return SPELL_FAILED_TARGET_AURASTATE;
					}
				}

				if(target->IsPlayer())
				{
					// disallow spell casting in sanctuary zones
					// allow attacks in duels
					if( p_caster->DuelingWith != target && !isFriendly( p_caster, target ) )
					{
						AreaTable* atCaster = dbcArea.LookupEntry( p_caster->GetAreaID() );
						AreaTable* atTarget = dbcArea.LookupEntry( static_cast< Player* >( target )->GetAreaID() );
						if( atCaster->AreaFlags & 0x800 || atTarget->AreaFlags & 0x800 )
							return SPELL_FAILED_NOT_HERE;
					}
				}
				else
				{
					if (target->GetAIInterface()->GetIsSoulLinked() && u_caster && target->GetAIInterface()->getSoullinkedWith() != u_caster)
						return SPELL_FAILED_BAD_TARGETS;
				}
				
				// pet training
				if( m_spellInfo->EffectImplicitTargetA[0] == EFF_TARGET_PET &&
					m_spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL )
				{
					Pet *pPet = p_caster->GetSummon();
					// check if we have a pet
					if( pPet == NULL )
						return SPELL_FAILED_NO_PET;

					// other checks
					SpellEntry* trig = dbcSpell.LookupEntry( m_spellInfo->EffectTriggerSpell[0] );
					if( trig == NULL )
						return SPELL_FAILED_SPELL_UNAVAILABLE;

					uint32 status = pPet->CanLearnSpell( trig );
					if( status != 0 )
						return status;
				}

				if( m_spellInfo->EffectApplyAuraName[0]==2)//mind control
				{
					if( m_spellInfo->EffectBasePoints[0])//got level req;
					{
						if((int32)target->getLevel() > m_spellInfo->EffectBasePoints[0]+1 + int32(p_caster->getLevel() - m_spellInfo->spellLevel))
							return SPELL_FAILED_HIGHLEVEL;
						else if(target->GetTypeId() == TYPEID_UNIT) 
						{ 
							Creature * c = (Creature*)(target);
							if (c&&c->GetCreatureName()&&c->GetCreatureName()->Rank >ELITE_ELITE)
								return SPELL_FAILED_HIGHLEVEL;
						} 
					}
				}
			}

			// scripted spell stuff
			switch(m_spellInfo->Id)
			{
				case 603: //curse of doom, can't be casted on players
				case 30910:
				{
					if(target->IsPlayer())
						return SPELL_FAILED_TARGET_IS_PLAYER;
				}break;

				case 13907:
					if (!target || target->IsPlayer() || target->getClass()!=TARGET_TYPE_DEMON )
						return SPELL_FAILED_SPELL_UNAVAILABLE;
					break;

				// disable spell
				case 25997: // Eye for an Eye
				case 38554: //Absorb Eye of Grillok
				{
					// do not allow spell to be cast
					return SPELL_FAILED_SPELL_UNAVAILABLE;
				}break;

				case 36314: //The Seer's Presence
				{
					// this spell can be cast only on socrethar. Otherwife cool exploit
					if(target->IsPlayer() || !target->IsUnit())
						return SPELL_FAILED_BAD_TARGETS;
					// this spell should be used only for a specific quest on specific monster = "Socrethar"
					if(target->GetEntry()!=20132) //nasty fixed numbers :(
						return SPELL_FAILED_BAD_TARGETS;
				}break;

				case 982: //Revive Pet
				{
					Pet *pPet = p_caster->GetSummon();
					if(pPet && !pPet->isDead())
						return SPELL_FAILED_TARGET_NOT_DEAD;
				}break;
				case 38177:
				{
					if(target->GetEntry() != 21387)
						return SPELL_FAILED_BAD_TARGETS;
				}break;
			}

			// if the target is not the unit caster and not the masters pet
			if(target != u_caster && !m_caster->IsPet())
			{
				// Dummy spells check
				if( m_spellInfo->Id == 4130)// Banish Burning Exile
				{
					if(target->GetEntry()!= 2760) // target needs to be a Burning Exile
						return SPELL_FAILED_BAD_TARGETS;
				}
				if( m_spellInfo->Id == 4131)// Banish Cresting Exile
				{
					if(target->GetEntry()!= 2761) // target needs to be a Cresting Exile
						return SPELL_FAILED_BAD_TARGETS;
				}
				if( m_spellInfo->Id == 4132)// Banish Thundering Exile
				{
					if(target->GetEntry()!= 2762) // target needs to be a Thundering Exile
						return SPELL_FAILED_BAD_TARGETS;
				}
				if( m_spellInfo->Id == 38177) //Blackwhelp Net
				{
					// should only affect Wyrmcult Blackwhelps
					if(target->GetEntry()!= 21387)
						return SPELL_FAILED_BAD_TARGETS;
				}

				/***********************************************************
				* Inface checks, these are checked in 2 ways
				* 1e way is check for damage type, as 3 is always ranged
				* 2e way is trough the data in the extraspell db
				*
				**********************************************************/

				/* burlex: units are always facing the target! */
				if(p_caster && !(m_spellInfo->buffType & SPELL_TYPE_CURSE))
				{
					if( m_spellInfo->Spell_Dmg_Type == SPELL_DMG_TYPE_RANGED )
					{ // our spell is a ranged spell
						if(!p_caster->isInFront(target))
							return SPELL_FAILED_UNIT_NOT_INFRONT;
					}
					else
					{ // our spell is not a ranged spell
						if( GetType() == SPELL_DMG_TYPE_MAGIC && m_spellInfo->in_front_status == 1 )
						{
							// must be in front
							if(!u_caster->isInFront(target))
								return SPELL_FAILED_UNIT_NOT_INFRONT;
						}
						else if( m_spellInfo->in_front_status == 2)
						{
							// behind
							if(target->isInFront(u_caster))
								return SPELL_FAILED_NOT_BEHIND;
						}
					}
				}
			}

			// if target is already skinned, don't let it be skinned again
			if( m_spellInfo->Effect[0] == SPELL_EFFECT_SKINNING) // skinning
				if(target->IsUnit() && (((Creature*)target)->Skinned) )
					return SPELL_FAILED_TARGET_UNSKINNABLE;

			// pet's owner stuff
			/*if (m_spellInfo->EffectImplicitTargetA[0] == 27 || 
				m_spellInfo->EffectImplicitTargetA[1] == 27 || 
				m_spellInfo->EffectImplicitTargetA[2] == 27)
			{
				if (!target->IsPlayer())
					return SPELL_FAILED_TARGET_NOT_PLAYER; //if you are there something is very wrong
			}*/

			// target 39 is fishing, all fishing spells are handled
			if( m_spellInfo->EffectImplicitTargetA[0] == 39 )//|| 
			 //m_spellInfo->EffectImplicitTargetA[1] == 39 || 
			 //m_spellInfo->EffectImplicitTargetA[2] == 39)
			{
				uint32 entry = m_spellInfo->EffectMiscValue[0];
				if(entry == GO_FISHING_BOBBER)
				{
					//uint32 mapid = p_caster->GetMapId();
					float px=u_caster->GetPositionX();
					float py=u_caster->GetPositionY();
					//float pz=u_caster->GetPositionZ();
					float orient = m_caster->GetOrientation();
					float posx = 0,posy = 0,posz = 0;
					float co = cos(orient);
					float si = sin(orient);
					MapMgr * map = m_caster->GetMapMgr(); 

					float r;
					for(r=20; r>10; r--)
					{
						posx = px + r * co;
						posy = py + r * si;
						/*if(!(map->GetWaterType(posx,posy) & 1))//water 
							continue;*/
						posz = map->GetWaterHeight(posx,posy);
						if(posz > map->GetLandHeight(posx,posy))//water
							break;
					}
					if(r<=10)
						return SPELL_FAILED_NOT_FISHABLE;

					// if we are already fishing, dont cast it again
					if(p_caster->GetSummonedObject())
						if(p_caster->GetSummonedObject()->GetEntry() == GO_FISHING_BOBBER)
							return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}

			if( p_caster != NULL )
			{
				if( m_spellInfo->NameHash == SPELL_HASH_GOUGE )// Gouge
					if(!target->isInFront(p_caster))
						return SPELL_FAILED_NOT_INFRONT;

				if( m_spellInfo->Category==1131)//Hammer of wrath, requires target to have 20- % of hp
				{
					if(target->GetUInt32Value(UNIT_FIELD_HEALTH) == 0)
						return SPELL_FAILED_BAD_TARGETS;

					if(target->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/target->GetUInt32Value(UNIT_FIELD_HEALTH)<5)
						 return SPELL_FAILED_BAD_TARGETS;
				}
				else if( m_spellInfo->Category==672)//Conflagrate, requires immolation spell on victim
				{
					if(!target->HasAuraVisual(46))
						return SPELL_FAILED_BAD_TARGETS;
				}

				if(target->dispels[m_spellInfo->DispelType])
					return SPELL_FAILED_PREVENTED_BY_MECHANIC-1;			// hackfix - burlex
				
				// Removed by Supalosa and moved to 'completed cast'
				//if(target->MechanicsDispels[m_spellInfo->MechanicsType])
				//	return SPELL_FAILED_PREVENTED_BY_MECHANIC-1; // Why not just use 	SPELL_FAILED_DAMAGE_IMMUNE                                   = 144,
			}

			// if we're replacing a higher rank, deny it
			AuraCheckResponse acr = target->AuraCheck(m_spellInfo->NameHash, m_spellInfo->RankNumber,m_caster);
			if( acr.Error == AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT )
				return SPELL_FAILED_AURA_BOUNCED;

			//check if we are trying to stealth or turn invisible but it is not allowed right now
			if( IsStealthSpell() || IsInvisibilitySpell() )
			{
				//if we have Faerie Fire, we cannot stealth or turn invisible
				if( u_caster->HasNegativeAuraWithNameHash( SPELL_HASH_FAERIE_FIRE ) || u_caster->HasNegativeAuraWithNameHash( SPELL_HASH_FAERIE_FIRE__FERAL_ ) )
					return SPELL_FAILED_SPELL_UNAVAILABLE;
			}
			
			if( target->IsPlayer() )
			{
				switch(m_spellInfo->NameHash)
				{
				case SPELL_HASH_DIVINE_PROTECTION:
				case SPELL_HASH_DIVINE_SHIELD:
					{
						if( TO_PLAYER(target)->mForbearance )
							return SPELL_FAILED_DAMAGE_IMMUNE;
					}break;

				case SPELL_HASH_ICE_BLOCK:
					{
						if( TO_PLAYER(target)->mHypothermia )
							return SPELL_FAILED_DAMAGE_IMMUNE;

					}break;

				case SPELL_HASH_POWER_WORD__SHIELD:
					{
						if( TO_PLAYER(target)->mWeakenedSoul )
							return SPELL_FAILED_DAMAGE_IMMUNE;
					}break;
				}
			}

			if (m_spellInfo->MechanicsType == 16 && target->mRecentlyBandaged)
				return SPELL_FAILED_DAMAGE_IMMUNE;
		}
	}	

	// Special State Checks (for creatures & players)
	if( u_caster )
	{
		if( u_caster->SchoolCastPrevent[m_spellInfo->School] )
		{	
			uint32 now_ = getMSTime();
			if( now_ > u_caster->SchoolCastPrevent[m_spellInfo->School] )//this limit has expired,remove
				u_caster->SchoolCastPrevent[m_spellInfo->School] = 0;
			else 
			{
				// HACK FIX
				switch( m_spellInfo->NameHash )
				{
					case SPELL_HASH_ICE_BLOCK: //Ice Block
					case 0x9840A1A6: //Divine Shield
						break;

					case 0x3DFA70E5: //Will of the Forsaken
						{
							if( u_caster->m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP ) )
								break;
						}break;

					case 0xF60291F4: //Death Wish
					case 0xD77038F4: //Fear Ward
					case 0x19700707: //Berserker Rage
						{
							if( u_caster->m_special_state & UNIT_STATE_FEAR )
								break;
						}break;

					// {Insignia|Medallion} of the {Horde|Alliance}
					case 0xC7C45478: //Immune Movement Impairment and Loss of Control
					case 0xDD06F1BF: // insignia of the alliance/horde 2.4.3
					case 0x048c32f9: // insignia of the alliance/horde prior to 2.4.3
						break;

						{
							if( u_caster->m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP | UNIT_STATE_ROOT | UNIT_STATE_STUN | UNIT_STATE_CONFUSE | UNIT_STATE_SNARE ) )
								break;
						}
							break;

					case 0xCD4CDF55: // Barksin
					{ // This spell is usable while stunned, frozen, incapacitated, feared or asleep.  Lasts 12 sec.
						if( u_caster->m_special_state & ( UNIT_STATE_STUN | UNIT_STATE_FEAR | UNIT_STATE_SLEEP ) ) // Uh, what unit_state is Frozen? (freezing trap...)
							break;
					}

					default:
						return SPELL_FAILED_SILENCED;
				}
			}
		}

		if( u_caster->m_silenced && m_spellInfo->School != NORMAL_DAMAGE )// can only silence non-physical
			return SPELL_FAILED_SILENCED;

		if(target) /* -Supalosa- Shouldn't this be handled on Spell Apply? */
		{
			for( int i = 0; i < 3; i++ ) // if is going to cast a spell that breaks stun remove stun auras, looks a bit hacky but is the best way i can find
			{
				if( m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY )
				{
					target->RemoveAllAurasByMechanic( m_spellInfo->EffectMiscValue[i] , -1 , true );
					// Remove all debuffs of that mechanic type.
					// This is also done in SpellAuras.cpp - wtf?
				}
				/*
				if( m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY && (m_spellInfo->EffectMiscValue[i] == 12 || m_spellInfo->EffectMiscValue[i] == 17))
				{
					for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
						if(target->m_auras[x])
							if(target->m_auras[x]->GetSpellProto()->MechanicsType == m_spellInfo->EffectMiscValue[i])
								target->m_auras[x]->Remove();
				}
				*/
			}
		}

		if( u_caster->IsPacified() && m_spellInfo->School == NORMAL_DAMAGE ) // only affects physical damage
		{
			// HACK FIX
			switch( m_spellInfo->NameHash )
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case 0x9840A1A6: //Divine Shield
				case 0x3DFA70E5: //Will of the Forsaken
				{
					if( u_caster->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP))
						break;
				}break;

				default:
					return SPELL_FAILED_PACIFIED;
			}
		}

		if( u_caster->IsStunned() || u_caster->IsFeared())
		{
			// HACK FIX
			switch( m_spellInfo->NameHash )
			{
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case SPELL_HASH_DIVINE_SHIELD: //Divine Shield
				case SPELL_HASH_DIVINE_PROTECTION: //Divine Protection
				case 0xCD4CDF55: //Barkskin
					break;
				/* -Supalosa- For some reason, being charmed or sleep'd is counted as 'Stunned'. 
				Check it: http://www.wowhead.com/?spell=700 */

				case 0xC7C45478: /* Immune Movement Impairment and Loss of Control (PvP Trinkets) */
					break;

				case 0x3DFA70E5: /* Will of the Forsaken (Undead Racial) */
					break;

				case 0xDD06F1BF: // insignia of the alliance/horde 2.4.3
				case 0x048c32f9: // insignia of the alliance/horde prior to 2.4.3
					break;

				case SPELL_HASH_BLINK:
					break;

				default:
					return SPELL_FAILED_STUNNED;
			}
		}

		if(u_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) > 0)
		{
			SpellEntry * t_spellInfo = (u_caster->GetCurrentSpell() ? u_caster->GetCurrentSpell()->m_spellInfo : NULL);

			if(!t_spellInfo || !m_triggeredSpell)
				return SPELL_FAILED_SPELL_IN_PROGRESS;
			else if (t_spellInfo)
			{
				if(
					t_spellInfo->EffectTriggerSpell[0] != m_spellInfo->Id &&
					t_spellInfo->EffectTriggerSpell[1] != m_spellInfo->Id &&
					t_spellInfo->EffectTriggerSpell[2] != m_spellInfo->Id)
				{
					return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}
		}
	}

	// no problems found, so we must be ok
	return SPELL_CANCAST_OK;
}

void Spell::RemoveItems()
{
	// Item Charges & Used Item Removal
	if(i_caster)
	{
		// Stackable Item -> remove 1 from stack
		if(i_caster->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > 1)
		{
			i_caster->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT, -1);
			i_caster->m_isDirty = true;
		}
		// Expendable Item
		else if(i_caster->GetProto()->Spells[0].Charges < 0
		     || i_caster->GetProto()->Spells[1].Charges == -1) // hackfix for healthstones/mana gems/depleted items
		{
			// if item has charges remaining -> remove 1 charge
			if(((int32)i_caster->GetUInt32Value(ITEM_FIELD_SPELL_CHARGES)) < -1)
			{
				i_caster->ModUnsigned32Value(ITEM_FIELD_SPELL_CHARGES, 1);
				i_caster->m_isDirty = true;
			}
			// if item has no charges remaining -> delete item
			else
			{
				if(i_caster->GetOwner()) // wtf?
				{
					i_caster->GetOwner()->GetItemInterface()->SafeFullRemoveItemByGuid(i_caster->GetGUID());
					i_caster = NULL;
				}
			}
		}
		// Non-Expendable Item -> remove 1 charge
		else if(i_caster->GetProto()->Spells[0].Charges > 0)
		{
			i_caster->ModUnsigned32Value(ITEM_FIELD_SPELL_CHARGES, -1);
			i_caster->m_isDirty = true;
		}
	} 

	// Ammo Removal
	if( p_caster && (m_spellInfo->Flags3 == FLAGS3_REQ_RANGED_WEAPON || m_spellInfo->Flags4 & FLAGS4_PLAYER_RANGED_SPELLS))
	{
		p_caster->GetItemInterface()->RemoveItemAmt_ProtectPointer(p_caster->GetUInt32Value(PLAYER_AMMO_ID), 1, &i_caster);
	}

	// Reagent Removal
	for(uint32 i=0; i<8 ;i++)
	{
		if( p_caster && m_spellInfo->Reagent[i])
		{
			p_caster->GetItemInterface()->RemoveItemAmt_ProtectPointer(m_spellInfo->Reagent[i], m_spellInfo->ReagentCount[i], &i_caster);
		}
	}
}

int32 Spell::CalculateEffect(uint32 i,Unit *target)
{
	// TODO: Add ARMOR CHECKS; Add npc that have ranged weapons use them;

	// Range checks
 /*   if (m_spellInfo->Id == SPELL_RANGED_GUN) // this includes bow and gun
	{
		if(!u_caster || !unitTarget)
			return 0;

		return ::CalculateDamage( u_caster, unitTarget, RANGED, m_spellInfo->SpellGroupType );
	}
*/
	int32 value = 0;

	float basePointsPerLevel    = m_spellInfo->EffectRealPointsPerLevel[i];
	float randomPointsPerLevel  = m_spellInfo->EffectDicePerLevel[i];
	int32 basePoints = m_spellInfo->EffectBasePoints[i] + 1;
	int32 randomPoints = m_spellInfo->EffectDieSides[i];

	//added by Zack : some talents inherit their basepoints from the previously casted spell: see mage - Master of Elements
	if(forced_basepoints[i])
		basePoints = forced_basepoints[i];

	/* Random suffix value calculation */
	/*if(i_caster && (int32(i_caster->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID)) < 0))
	{
        ItemRandomSuffixEntry * si = dbcItemRandomSuffix.LookupEntry(abs(int32(i_caster->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID))));
		EnchantEntry * ent;
		uint32 j,k;

		for(j = 0; j < 3; ++j)
		{
			if(si->enchantments[j] != 0)
			{
				ent = dbcEnchant.LookupEntry(si->enchantments[j]);
				for(k = 0; k < 3; ++k)
				{
					if(ent->spell[k] == m_spellInfo->Id)
					{
						if(si->prefixes[k] == 0)
							goto exit;
						
						value = RANDOM_SUFFIX_MAGIC_CALCULATION(si->prefixes[j], i_caster->GetItemRandomSuffixFactor());
						
						if(value == 0)
							goto exit;

						return value;
					}
				}
			}
		}
	}
exit:*/

	if( u_caster != NULL )
	{
		int32 diff = -(int32)m_spellInfo->baseLevel;
		if (m_spellInfo->maxLevel && u_caster->getLevel()>m_spellInfo->maxLevel)
			diff +=m_spellInfo->maxLevel;
		else
			diff +=u_caster->getLevel();
		randomPoints += float2int32(diff * randomPointsPerLevel);
		basePoints += float2int32(diff * basePointsPerLevel );
	}

	if(randomPoints <= 1)
		value = basePoints;
	else
		value = basePoints + rand() % randomPoints;

	//scripted shit
	if( m_spellInfo->NameHash == SPELL_HASH_STEADY_SHOT )
	{	//A steady shot that causes ${$RAP*0.3+$m1} damage. 
		//	Actual Equation (http://www.wowwiki.com/Steady_Shot)
		//		* The tooltip is proven to be wrong and the following is the best player worked out formula so far with data taken from [1]
		//		* Formula: DamagePercentageBonus*RangedWeaponSpecialization*(150 + WeaponDamage/WeaponSpeed*2.8 + 0.2*RAP + [Dazed: 175]) 
		if(i==0 && u_caster)
		{
			if( p_caster != NULL )
			{
				Item *it;
				if(p_caster->GetItemInterface())
				{
					it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(it)
						value += float2int32(150 + float(it->GetProto()->Damage[0].Min)/float(it->GetProto()->Delay)*2.8f);
				}
			}
			if(target && target->HasNegativeAura(CREATURE_SPELL_TO_DAZE))
				value += 175;
			value += (uint32)(u_caster->GetRAP()*0.2);
		}
	}
    // HACK FIX
    else if( m_spellInfo->NameHash == SPELL_HASH_VICTORY_RUSH )
	{//causing ${$AP*$m1/100} damage
		if(i==0 && u_caster)
			value = (value*u_caster->GetAP())/100;
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_RAKE )//rake
	{
		//Rake the target for ${$AP/100+$m1} bleed damage and an additional ${$m2*3+$AP*0.06} damage over $d.
		if( u_caster != NULL ) 
		{
			float ap = (float)u_caster->GetAP();
			if(i==0)
				value+=(uint32)ceilf((ap*0.01f));	// / 100
			else if(i==1)
				value=(int32)ceilf((float(value * 3) + ceilf((ap*0.06f))) / 3.0f);
		}
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_GARROTE )
	{
		// WoWWiki says +( 0.18 * attack power / number of ticks )
		// Tooltip gives no specific reading, but says ", increased by your attack power.".
		if( u_caster != NULL )
		{
			float ap = (float)u_caster->GetAP();
			if( i == 0 )
			{
				value += (uint32) ceilf( ( ap * 0.18f ) / 6 );
			}
		}

	}
	else if( m_spellInfo->NameHash == SPELL_HASH_RUPTURE )
	{
		/* 
		1pt = Attack Power * 0.04 + x
		2pt = Attack Power * 0.10 + y
		3pt = Attack Power * 0.18 + z
		4pt = Attack Power * 0.21 + a
		5pt = Attack Power * 0.24 + b
		*/
		if( u_caster->IsPlayer() )
		{
			float ap = (float)u_caster->GetAP();
			float cp = (float)static_cast< Player* >(u_caster)->m_comboPoints;
			value += (uint32) ceilf( ( ap * ( 0.04f * cp ) ) / ( ( 6 + ( cp * 2 ) ) / 2 ) );
		}
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_RIP ) //rip
	{
		if( u_caster != NULL ) 
			value+=(uint32)(u_caster->GetAP()*0.04f);

	}
	else if( m_spellInfo->NameHash == SPELL_HASH_MONGOOSE_BITE ) //Mongoose Bite
	{
		// ${$AP*0.2+$m1} damage.
		if( u_caster != NULL )
			value+=u_caster->GetAP()/5;
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_SWIPE ) // Swipe
	{
		// ${$AP*0.06+$m1} damage.
		if( u_caster != NULL )
			value+=float2int32(u_caster->GetAP()*0.06f);
	}
	
	if( p_caster != NULL )
	{	
		if( m_spellInfo->NameHash == SPELL_HASH_ENVENOM && i == 0 )//Envenom
		{
			value *= p_caster->m_comboPoints;
			value += (uint32)(p_caster->GetAP()*(0.03f*p_caster->m_comboPoints));
			m_requiresCP=true;
		}
		int32 comboDamage = (int32)m_spellInfo->EffectPointsPerComboPoint[i];
	
		if(comboDamage)
		{
			
			if( m_spellInfo->NameHash == SPELL_HASH_EVISCERATE ) //Eviscerate
			value += (uint32)( p_caster->GetAP() * ( 0.03 * p_caster->m_comboPoints ) );

			value += ( comboDamage * p_caster->m_comboPoints );
			m_requiresCP = true;
			//this is ugly so i will explain the case maybe someone ha a better idea :
			// while casting a spell talent will trigger uppon the spell prepare faze
			// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
			if( p_caster != NULL )
				p_caster->m_spellcomboPoints = 0;
		}

		SpellOverrideMap::iterator itr = p_caster->mSpellOverrideMap.find(m_spellInfo->Id);
		if(itr != p_caster->mSpellOverrideMap.end())
		{
			ScriptOverrideList::iterator itrSO;
			for(itrSO = itr->second->begin(); itrSO != itr->second->end(); ++itrSO)
			{
				//DK:FIXME->yeni bir map olu�tur
                // Capt: WHAT THE FUCK DOES THIS MEAN....
				// Supa: WHAT THE FUCK DOES THIS MEAN?
				value += RandomUInt((*itrSO)->damage);
			}
		}
	 }

	// TODO: INHERIT ITEM MODS FROM REAL ITEM OWNER - BURLEX BUT DO IT PROPERLY

	if( u_caster != NULL )
	{
		int32 spell_flat_modifers=0;
		int32 spell_pct_modifers=0;
		int32 spell_pct_modifers2=0;//separated from the other for debugging purpuses

		SM_FIValue(u_caster->SM_FSPELL_VALUE,&spell_flat_modifers,m_spellInfo->SpellGroupType);
		SM_FIValue(u_caster->SM_PSPELL_VALUE,&spell_pct_modifers,m_spellInfo->SpellGroupType);

		SM_FIValue(u_caster->SM_FEffectBonus,&spell_flat_modifers,m_spellInfo->SpellGroupType);
		SM_FIValue(u_caster->SM_PEffectBonus,&spell_pct_modifers,m_spellInfo->SpellGroupType);

		value = value + value*(spell_pct_modifers+spell_pct_modifers2)/100 + spell_flat_modifers;

	}
	else if( i_caster != NULL && target)
	{	
		//we should inherit the modifiers from the conjured food caster
		Unit *item_creator = target->GetMapMgr()->GetUnit( i_caster->GetUInt64Value( ITEM_FIELD_CREATOR ) );
		if( item_creator != NULL )
		{
			int32 spell_flat_modifers=0;
			int32 spell_pct_modifers=0;
			int32 spell_pct_modifers2=0;//separated from the other for debugging purpuses

			SM_FIValue(item_creator->SM_FSPELL_VALUE,&spell_flat_modifers,m_spellInfo->SpellGroupType);
			SM_FIValue(item_creator->SM_PSPELL_VALUE,&spell_pct_modifers,m_spellInfo->SpellGroupType);

			SM_FIValue(item_creator->SM_FEffectBonus,&spell_flat_modifers,m_spellInfo->SpellGroupType);
			SM_FIValue(item_creator->SM_PEffectBonus,&spell_pct_modifers,m_spellInfo->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			if(spell_flat_modifers!=0 || spell_pct_modifers!=0 || spell_pct_modifers2!=0)
				printf("!!!!ITEMCASTER ! : spell value mod flat %d , spell value mod pct %d, spell value mod pct2 %d , spell dmg %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,spell_pct_modifers2,value,m_spellInfo->SpellGroupType);
#endif
			value = value + value*(spell_pct_modifers+spell_pct_modifers2)/100 + spell_flat_modifers;
		}
	}


	return value;
}

void Spell::HandleTeleport(uint32 id, Unit* Target)
{
	if(Target->GetTypeId()!=TYPEID_PLAYER)
		return;

	Player* pTarget = static_cast< Player* >( Target );
   
	float x,y,z;
	uint32 mapid;
	
    // predefined behavior 
	if (m_spellInfo->Id == 8690 || m_spellInfo->Id == 556 || m_spellInfo->Id == 39937)// 556 - Astral Recall ; 39937 - Ruby Slippers
	{
		x = pTarget->GetBindPositionX();
		y = pTarget->GetBindPositionY();
		z = pTarget->GetBindPositionZ();
		mapid = pTarget->GetBindMapId();
	}
	else // normal behavior
	{
		TeleportCoords* TC = TeleportCoordStorage.LookupEntry(id);
		if(!TC)
			return;
		 
		x=TC->x;
		y=TC->y;
		z=TC->z;
		mapid=TC->mapId;
	}

	pTarget->EventAttackStop();
	pTarget->SetSelection(0);
	  
	// We use a teleport event on this one. Reason being because of UpdateCellActivity,
	// the game object set of the updater thread WILL Get messed up if we teleport from a gameobject
	// caster.
	if(!sEventMgr.HasEvent(pTarget, EVENT_PLAYER_TELEPORT))
		sEventMgr.AddEvent(pTarget, &Player::EventTeleport, mapid, x, y, z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::CreateItem(uint32 itemId)
{
    if( !itemId )
        return;

	Player*			pUnit = static_cast< Player* >( m_caster );
	Item*			newItem;
	Item*			add;
	SlotResult		slotresult;
	ItemPrototype*	m_itemProto;

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemId );
	if( m_itemProto == NULL )
	    return;

	if (pUnit->GetItemInterface()->CanReceiveItem(m_itemProto, 1, NULL))
	{
		SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
		return;
	}

	add = pUnit->GetItemInterface()->FindItemLessMax(itemId, 1, false);
	if (!add)
	{
		slotresult = pUnit->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
		if(!slotresult.Result)
		{
			 SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			 return;
		}
		
		newItem = objmgr.CreateItem(itemId, pUnit);
		AddItemResult result = pUnit->GetItemInterface()->SafeAddItem(newItem, slotresult.ContainerSlot, slotresult.Slot);
		if(!result)
		{
			delete newItem;
			return;
		}

		newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
		newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, damage);

		/*WorldPacket data(45);
		p_caster->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, 1, itemId ,0,0xFF,1,0xFFFFFFFF);
		p_caster->SendMessageToSet(&data, true);*/
		p_caster->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,1);
		newItem->m_isDirty = true;

	} 
	else 
	{
		add->SetUInt32Value(ITEM_FIELD_STACK_COUNT,add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + damage);
		/*WorldPacket data(45);
		p_caster->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, 1, itemId ,0,0xFF,1,0xFFFFFFFF);
		p_caster->SendMessageToSet(&data, true);*/
		p_caster->GetSession()->SendItemPushResult(add,true,false,true,false,p_caster->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,1);
		add->m_isDirty = true;
	}
}

/*void Spell::_DamageRangeUpdate()
{
	if(unitTarget)
	{
		if(unitTarget->isAlive())
		{
			m_caster->SpellNonMeleeDamageLog(unitTarget,m_spellInfo->Id, damageToHit);
		}
		else
		{	if( u_caster != NULL )
			if(u_caster->GetCurrentSpell() != this)
			{
					if(u_caster->GetCurrentSpell() != NULL)
					{
						u_caster->GetCurrentSpell()->SendChannelUpdate(0);
						u_caster->GetCurrentSpell()->cancel();
					}
			}
			SendChannelUpdate(0);
			cancel();
		}
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
	}
	else if(gameObjTarget)
	{
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
		//Go Support
	}
	else
	{
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
	}
}*/

void Spell::SendHealSpellOnPlayer(Object* caster, Object* target, uint32 dmg,bool critical, uint32 spellid)
{
	if(!caster || !target || !target->IsPlayer())
		return;

	uint8 buf[100];
	StackPacket data(SMSG_SPELLHEALLOG, buf, 100);
	// Bur: I know it says obsolete, but I just logged this tonight and got this packet.
	
	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellid;
	data << uint32(dmg);	// amt healed
	data << uint8(critical);	 //this is critical message

	caster->SendMessageToSet(&data, true);
}

void Spell::SendHealManaSpellOnPlayer(Object * caster, Object * target, uint32 dmg, uint32 powertype, uint32 spellid)
{
	if(!caster || !target || !target->IsPlayer())
		return;

	uint8 buf[100];
	StackPacket data(SMSG_SPELLENERGIZELOG, buf, 100);

	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellid;
	data << powertype;
	data << dmg;

	caster->SendMessageToSet(&data, true);
}

void Spell::Heal(int32 amount)
{
	if(!unitTarget || !unitTarget->isAlive())
		return;
	
	if( p_caster != NULL )
		p_caster->last_heal_spell=m_spellInfo;

    //self healing shouldn't flag himself
	if(p_caster && playerTarget && p_caster != playerTarget)
	{
		// Healing a flagged target will flag you.
		if(playerTarget->IsPvPFlagged())
			p_caster->SetPvPFlag();
	}

	//Make it critical
	bool critical = false;
	int32 critchance = 0; 
	int32 bonus = 0;
	float healdoneaffectperc = 1.0f;
	if( u_caster != NULL )
	{
		//Downranking
		if(p_caster && p_caster->IsPlayer() && m_spellInfo->baseLevel > 0 && m_spellInfo->maxLevel > 0)
		{
			float downrank1 = 1.0f;
			if (m_spellInfo->baseLevel < 20)
				downrank1 = 1.0f - (20.0f - float (m_spellInfo->baseLevel) ) * 0.0375f;
			float downrank2 = ( float(m_spellInfo->maxLevel + 5.0f) / float(p_caster->getLevel()) );
			if (downrank2 >= 1 || downrank2 < 0)
				downrank2 = 1.0f;
			healdoneaffectperc *= downrank1 * downrank2;
		}

		//Spells Not affected by Bonus Healing
		if(m_spellInfo->NameHash == SPELL_HASH_SEAL_OF_LIGHT) //Seal of Light
			healdoneaffectperc = 0.0f;

		if(m_spellInfo->NameHash == SPELL_HASH_LESSER_HEROISM) //Lesser Heroism
			healdoneaffectperc = 0.0f;

		//Spells affected by Bonus Healing
		if(m_spellInfo->NameHash == SPELL_HASH_EARTH_SHIELD) //Earth Shield
			healdoneaffectperc = 0.3f;

		//Basic bonus
		bonus += u_caster->HealDoneMod[m_spellInfo->School];
		bonus += unitTarget->HealTakenMod[m_spellInfo->School];

		//Bonus from Intellect & Spirit
		if( p_caster != NULL )  
		{
			for(uint32 a = 0; a < 6; a++)
				bonus += float2int32(p_caster->SpellHealDoneByAttribute[a][m_spellInfo->School] * p_caster->GetUInt32Value(UNIT_FIELD_STAT0 + a));
		}

		//Spell Coefficient
		if(  m_spellInfo->Dspell_coef_override >= 0 ) //In case we have forced coefficients
			bonus = float2int32( float( bonus ) * m_spellInfo->Dspell_coef_override );
		else
		{
			//Bonus to DH part
			if( m_spellInfo->fixed_dddhcoef >= 0 )
				bonus = float2int32( float( bonus ) * m_spellInfo->fixed_dddhcoef );
		}

		critchance = float2int32(u_caster->spellcritperc + u_caster->SpellCritChanceSchool[m_spellInfo->School]);

		if(m_spellInfo->SpellGroupType)
		{
			int penalty_pct = 0;
			int penalty_flt = 0;
			SM_FIValue( u_caster->SM_FPenalty, &penalty_flt, m_spellInfo->SpellGroupType );
			bonus += penalty_flt;
			SM_FIValue( u_caster->SM_PPenalty, &penalty_pct, m_spellInfo->SpellGroupType );
			bonus += bonus * ( penalty_pct / 100 );
			SM_FIValue( u_caster->SM_CriticalChance,&critchance,m_spellInfo->SpellGroupType);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			int spell_flat_modifers=0;
			int spell_pct_modifers=0;
			SM_FIValue(u_caster->SM_FPenalty,&spell_flat_modifers,m_spellInfo->SpellGroupType);
			SM_FIValue(u_caster->SM_PPenalty,&spell_pct_modifers,m_spellInfo->SpellGroupType);
			if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
				printf("!!!!!HEAL : spell dmg bonus(p=24) mod flat %d , spell dmg bonus(p=24) pct %d , spell dmg bonus %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,bonus,m_spellInfo->SpellGroupType);
#endif
		}
		
		amount += float2int32( float( bonus ) * healdoneaffectperc ); //apply downranking on final value ?
		amount *= u_caster->HealDonePctMod[m_spellInfo->School];
		amount *= unitTarget->HealTakenPctMod[m_spellInfo->School];

		if (m_spellInfo->SpellGroupType)
			SM_FIValue(u_caster->SM_PDamageBonus,&amount,m_spellInfo->SpellGroupType);

		if(critical = Rand(critchance))
		{
			/*int32 critbonus = amount >> 1;
			if( m_spellInfo->SpellGroupType)
					SM_PIValue(static_cast<Unit*>(u_caster)->SM_PCriticalDamage, &critbonus, m_spellInfo->SpellGroupType);
			amount += critbonus;*/

			int32 critical_bonus = 100;
			if( m_spellInfo->SpellGroupType )
				SM_FIValue( u_caster->SM_PCriticalDamage, &critical_bonus, m_spellInfo->SpellGroupType );

			if( critical_bonus > 0 )
			{
				// the bonuses are halved by 50% (funky blizzard math :S)
				float b = ( ( float(critical_bonus) / 2.0f ) / 100.0f );
				amount += float2int32( float(amount) * b );
			}

			//Shady: does it correct> caster casts heal and proc ..._VICTIM ? 
			// Or mb i'm completely wrong? So if true  - just replace with old string. 
			//u_caster->HandleProc(PROC_ON_SPELL_CRIT_HIT_VICTIM, unitTarget, m_spellInfo, amount);
			//Replaced with following one:
			
		
			unitTarget->HandleProc(PROC_ON_SPELL_CRIT_HIT_VICTIM, u_caster, m_spellInfo, amount);
			u_caster->HandleProc(PROC_ON_SPELL_CRIT_HIT, unitTarget, m_spellInfo, amount);
		}
		
	}

	if(amount < 0) 
		amount = 0;

	if( p_caster != NULL )  
	{
		if( unitTarget->IsPlayer() )
		{
			SendHealSpellOnPlayer( p_caster, static_cast< Player* >( unitTarget ), amount, critical, pSpellId ? pSpellId : m_spellInfo->Id );
		}
		p_caster->m_bgScore.HealingDone += amount;
		if( p_caster->m_bg != NULL )
			p_caster->m_bg->UpdatePvPData();
	}
	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 maxHealth = unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if((curHealth + amount) >= maxHealth)
		unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, maxHealth);
	else
		unitTarget->ModUnsigned32Value(UNIT_FIELD_HEALTH, amount);

	if (p_caster)
	{
		p_caster->m_casted_amount[m_spellInfo->School]=amount;
		p_caster->HandleProc( PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, unitTarget, m_spellInfo );
	}

	// add threat
	if( u_caster != NULL )
	{
		uint32 base_threat=Spell::GetBaseThreat(amount);
		int count = 0;
		Unit *unit;
		std::vector<Unit*> target_threat;
		if(base_threat)
		{
			target_threat.reserve(u_caster->GetInRangeCount()); // this helps speed

			for(std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
			{
				if((*itr)->GetTypeId() != TYPEID_UNIT)
					continue;
				unit = static_cast<Unit*>((*itr));
				if(unit->GetAIInterface()->GetNextTarget() == unitTarget)
				{
					target_threat.push_back(unit);
					++count;
				}
			}
			if(count == 0)
				count = 1;  // division against 0 protection
			/* 
			When a tank hold multiple mobs, the threat of a heal on the tank will be split between all the mobs.
			The exact formula is not yet known, but it is more than the Threat/number of mobs.
			So if a tank holds 5 mobs and receives a heal, the threat on each mob will be less than Threat(heal)/5.
			Current speculation is Threat(heal)/(num of mobs *2)
			*/
			uint32 threat = base_threat / (count * 2);
				
			for(std::vector<Unit*>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
			{
				// for now we'll just use heal amount as threat.. we'll prolly need a formula though
				static_cast< Unit* >( *itr )->GetAIInterface()->HealReaction( u_caster, unitTarget, threat );
			}
		}

		if(unitTarget->IsInWorld() && u_caster->IsInWorld())
			u_caster->CombatStatus.WeHealed(unitTarget);
	}   
}

void Spell::DetermineSkillUp(uint32 skillid,uint32 targetlevel)
{
	if(!p_caster)return;
	if(p_caster->GetSkillUpChance(skillid)<0.01)return;//to preven getting higher skill than max
	int32 diff=p_caster->_GetSkillLineCurrent(skillid,false)/5-targetlevel;
	if(diff<0)diff=-diff;
	float chance;
	if(diff<=5)chance=95.0;
	else if(diff<=10)chance=66;
	else if(diff <=15)chance=33;
	else return;
	if(Rand(chance*sWorld.getRate(RATE_SKILLCHANCE)))
		p_caster->_AdvanceSkillLine(skillid, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

void Spell::DetermineSkillUp(uint32 skillid)
{
	//This code is wrong for creating items and disenchanting. 
	if(!p_caster)return;
	float chance = 0.0f;
	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);
	if( skill != NULL && static_cast< Player* >( m_caster )->_HasSkillLine( skill->skilline ) )
	{
		uint32 amt = static_cast< Player* >( m_caster )->_GetSkillLineCurrent( skill->skilline, false );
		uint32 max = static_cast< Player* >( m_caster )->_GetSkillLineMax( skill->skilline );
		if( amt >= max )
			return;
		if( amt >= skill->grey ) //grey
			chance = 0.0f;
		else if( ( amt >= ( ( ( skill->grey - skill->green) / 2 ) + skill->green ) ) ) //green
			chance = 33.0f;
		else if( amt >= skill->green ) //yellow
			chance = 66.0f;
		else //brown
			chance=100.0f;
	}
	if(Rand(chance*sWorld.getRate(RATE_SKILLCHANCE)))
		p_caster->_AdvanceSkillLine(skillid, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

bool Spell::Reflect(Unit *refunit)
{
	SpellEntry * refspell = NULL;

	if( m_reflectedParent != NULL )
		return false;

	// if the spell to reflect is a reflect spell, do nothing.
	for(int i=0; i<3; i++)
    {
		if( m_spellInfo->Effect[i] == 6 && (m_spellInfo->EffectApplyAuraName[i] == 74 || m_spellInfo->EffectApplyAuraName[i] == 28))
			return false;
    }
	for(std::list<struct ReflectSpellSchool*>::iterator i = refunit->m_reflectSpellSchool.begin();i != refunit->m_reflectSpellSchool.end();i++)
	{
		if((*i)->school == -1 || (*i)->school == (int32)m_spellInfo->School)
		{
			if(Rand((float)(*i)->chance))
			{
				//the god blessed special case : mage - Frost Warding = is an augmentation to frost warding
				if((*i)->require_aura_hash && u_caster && !u_caster->HasAurasWithNameHash((*i)->require_aura_hash))
                {
					continue;
                }
				refspell = m_spellInfo;
			}
		}
	}

	if(!refspell || m_caster == refunit) return false;

	Spell *spell = new Spell(refunit, refspell, true, NULL);
	SpellCastTargets targets;
	targets.m_unitTarget = m_caster->GetGUID();
	spell->m_reflectedParent = this;
	spell->prepare(&targets);
	return true;
}

void ApplyDiminishingReturnTimer(uint32 * Duration, Unit * Target, SpellEntry * spell)
{
	uint32 status = GetDiminishingGroup(spell->NameHash);
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && Target->GetTypeId() != TYPEID_PLAYER && !Target->IsPet())
		return;

	// TODO: check for spells that should do this
	float Dur = float(*Duration);

	switch(Target->m_diminishCount[Grp])
	{
	case 0: // Full effect
		if (Target->IsPlayer() && Dur > 10000)
		{
			Dur = 10000;
		}
		break;
		
	case 1: // Reduced by 50%
		Dur *= 0.5f;
		if (Target->IsPlayer() && Dur > 5000)
		{
			Dur = 5000;
		}
		break;

	case 2: // Reduced by 75%
		Dur *= 0.25f;
		if (Target->IsPlayer() && Dur > 2500)
		{
			Dur = 2500;
		}
		break;

	default:// Target immune to spell
		{
			*Duration = 0;
			return;
		}break;
	}

	// Convert back
	*Duration = FL2UINT(Dur);

	// Reset the diminishing return counter, and add to the aura count (we don't decrease the timer till we
	// have no auras of this type left.
	++Target->m_diminishAuraCount[Grp];
	++Target->m_diminishCount[Grp];
}

void UnapplyDiminishingReturnTimer(Unit * Target, SpellEntry * spell)
{
	uint32 status = GetDiminishingGroup(spell->NameHash);
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && Target->GetTypeId() != TYPEID_PLAYER && !Target->IsPet())
		return;

	Target->m_diminishAuraCount[Grp]--;

	// start timer decrease
	if(!Target->m_diminishAuraCount[Grp])
	{
		Target->m_diminishActive = true;
		Target->m_diminishTimer[Grp] = 15000;
	}
}

/// Calculate the Diminishing Group. This is based on a name hash.
/// this off course is very hacky, but as its made done in a proper way
/// I leave it here.
uint32 GetDiminishingGroup(uint32 NameHash)
{
	int32 grp = -1;
	bool pve = false;

	switch(NameHash)
	{
	case SPELL_HASH_SAP:					// Sap
	case SPELL_HASH_GOUGE:					// Gouge
		grp = 0;
		break;

	case SPELL_HASH_CHEAP_SHOT:				// Cheap Shot
		{
			grp = 1;
			pve = true;
		}break;

	case SPELL_HASH_KIDNEY_SHOT:			// Kidney Shot
		{
			grp = 2;
			pve = true;
		}break;

	case SPELL_HASH_BASH:					// Bash
		grp = 3;
		break;

	case SPELL_HASH_ENTANGLING_ROOTS:		// Entangling Roots
		grp = 4;
		break;

	case SPELL_HASH_HAMMER_OF_JUSTICE:		// Hammer of Justice
		{
			grp = 5;
			pve = true;
		}break;

	case SPELL_HASH_STUN:					// Stuns (all of them)
		grp = 6;
		break;

	case SPELL_HASH_CHARGE:					// Charge
	case SPELL_HASH_INTERCEPT :				// Intercept
	case SPELL_HASH_CONCUSSION_BLOW:		// Concussion Blow
		{
			grp = 7;
			pve = true;
		}break;

	case SPELL_HASH_FEAR:					// Fear
	case SPELL_HASH_SEDUCTION:				// Seduction
	case SPELL_HASH_HOWL_OF_TERROR:			// Howl of Terror
		grp = 8;
		break;

	case SPELL_HASH_FROST_NOVA:				// Frost Nova
		grp = 9;
		break;

	case SPELL_HASH_POLYMORPH:				// Polymorph
	case SPELL_HASH_POLYMORPH__CHICKEN:		// Chicken
	case SPELL_HASH_POLYMORPH__PIG:			// Pig
	case SPELL_HASH_POLYMORPH__TURTLE:		// Turtle
	case SPELL_HASH_POLYMORPH__SHEEP:		// Good ol' sheep
		{
			grp = 10;
			pve = true;   
		}break;

	case SPELL_HASH_PSYCHIC_SCREAM:			// Psychic Scream
		grp = 11;
		break;

	case SPELL_HASH_MIND_CONTROL:			// Mind Control
		grp = 12;
		break;

	//case SPELL_HASH_FROST_SHOCK:			// Frost Shock
		//grp = 13;
		//break;

	case SPELL_HASH_HIBERNATE:				// Hibernate
		grp = 14;
		break;

	case SPELL_HASH_CYCLONE:				// Cyclone
		{
			grp = 15;
			pve = true;
		}break;

	case SPELL_HASH_CELESTIAL_FOCUS:		// Celestial Focus
		{
			grp = 16;
			pve = true;
		}break;

	case SPELL_HASH_IMPACT:					// Impact
		{
			grp = 17;
			pve = true;
		}break;

	case SPELL_HASH_BLACKOUT:				// Blackout
		{
			grp = 18;
			pve = true;
		}break;

	case SPELL_HASH_BANISH:					// Banish
		grp = 19;
		break;

	case SPELL_HASH_FREEZING_TRAP_EFFECT:	// Freezing Trap Effect
		grp = 20;
		break;

	case SPELL_HASH_SCARE_BEAST:			// Scare Beast
		grp = 21;
		break;

	case SPELL_HASH_ENSLAVE_DEMON:			// Enslave Demon
		grp = 22;
		break;
	case SPELL_HASH_SLEEP:					// Sleep
	case SPELL_HASH_RECKLESS_CHARGE:		// Reckless Charge
		grp = 23;
		break;
	case SPELL_HASH_RIPOSTE:
		grp = 24;
		break;
	}
	uint32 ret;
	if( pve )
		ret = grp | (1 << 16);
	else
		ret = grp;

	return ret;
}

/*void Spell::SendCastSuccess(Object * target)
{
	Player * plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr||!plr->IsPlayer())
		return;

	WorldPacket data(SMSG_CLEAR_EXTRA_AURA_INFO, 13);
	data << ((target != 0) ? target->GetNewGUID() : uint8(0));
	data << m_spellInfo->Id;
	
	plr->GetSession()->SendPacket(&data);
}*/

/*void Spell::SendCastSuccess(const uint64& guid)
{
	Player * plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr || !plr->IsPlayer())
		return;
    
	// fuck bytebuffers
	unsigned char buffer[13];
	uint32 c = FastGUIDPack(guid, buffer, 0);
	*(uint32*)&buffer[c] = m_spellInfo->Id;                 c += 4;
	plr->GetSession()->OutPacket(SMSG_CLEAR_EXTRA_AURA_INFO, c, buffer);
}*/

void Spell::_AddTarget(const Unit *target, const uint32 effectid)
{
	SpellTargetList::iterator itr;
	SpellTarget tgt;

	// look for the target in the list already
	for( itr = m_targetList.begin(); itr != m_targetList.end(); ++itr )
	{
		if( itr->Guid == target->GetGUID() )
		{
			// add the effect to it, hit result is already determined
			itr->EffectMask |= (1 << effectid);
			return;
		}
	}

	// setup struct
	tgt.Guid = target->GetGUID();
	tgt.EffectMask = (1 << effectid);

	// work out hit result (always true if we are a GO)
	tgt.HitResult = (g_caster != NULL) ? SPELL_DID_HIT_SUCCESS : _DidHit(target);

	// add to the list
	m_targetList.push_back(tgt);

	// add counter
	if( tgt.HitResult == SPELL_DID_HIT_SUCCESS )
		++m_hitTargetCount;
	else
		++m_missTargetCount;
}

void Spell::_AddTargetForced(const uint64& guid, const uint32 effectid)
{
	SpellTargetList::iterator itr;
	SpellTarget tgt;

	// look for the target in the list already
	for( itr = m_targetList.begin(); itr != m_targetList.end(); ++itr )
	{
		if( itr->Guid == guid )
		{
			// add the effect to it, hit result is already determined
			itr->EffectMask |= (1 << effectid);
			return;
		}
	}

	// setup struct
	tgt.Guid = guid;
	tgt.EffectMask = (1 << effectid);
	tgt.HitResult = SPELL_DID_HIT_SUCCESS;

	// add to the list
	m_targetList.push_back(tgt);

	// add counter
	if( tgt.HitResult == SPELL_DID_HIT_SUCCESS )
		++m_hitTargetCount;
	else
		++m_missTargetCount;
}
