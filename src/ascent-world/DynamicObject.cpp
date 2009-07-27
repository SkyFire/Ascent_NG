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

DynamicObject::DynamicObject(uint32 high, uint32 low)
{
	m_objectTypeId = TYPEID_DYNAMICOBJECT;
	m_valuesCount = DYNAMICOBJECT_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(DYNAMICOBJECT_END)*sizeof(uint32));
	m_updateMask.SetCount(DYNAMICOBJECT_END);
	m_uint32Values[OBJECT_FIELD_TYPE] = TYPE_DYNAMICOBJECT|TYPE_OBJECT;
	m_uint32Values[OBJECT_FIELD_GUID] = low;
	m_uint32Values[OBJECT_FIELD_GUID+1] = high;
	m_wowGuid.Init(GetGUID());
	m_floatValues[OBJECT_FIELD_SCALE_X] = 1;


	m_parentSpell=NULLSPELL;
	m_aliveDuration = 0;
	u_caster = NULLUNIT;
	m_spellProto = 0;
	p_caster = NULLPLR;
	m_caster = NULLOBJ;
	g_caster = NULLGOB;
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::Init()
{
	Object::Init();
}

void DynamicObject::Destructor()
{
	// remove aura from all targets
	DynamicObjectList::iterator jtr  = targets.begin();
	DynamicObjectList::iterator jend = targets.end();
	UnitPointer target;

	while(jtr != jend)
	{
		target = *jtr;
		++jtr;
		target->RemoveAura(m_spellProto->Id);
	}

	if(m_caster && m_caster->dynObj == dyn_shared_from_this() )
		m_caster->dynObj = NULLDYN;

	m_parentSpell=NULLSPELL;
	m_aliveDuration = 0;
	u_caster = NULLUNIT;
	m_spellProto = 0;
	p_caster = NULLPLR;
	m_caster = NULLOBJ;
	g_caster = NULLGOB;

	Object::Destructor();
}

void DynamicObject::Create(ObjectPointer caster, SpellPointer pSpell, float x, float y, float z, uint32 duration, float radius)
{
	Object::_Create(caster->GetMapId(),x, y, z, 0);
	if(pSpell->g_caster)
	{
		m_parentSpell = pSpell;
	}
	m_caster = caster;
	switch(caster->GetTypeId())
	{
	case TYPEID_GAMEOBJECT:
		{
			p_caster = pSpell->p_caster;
			u_caster = pSpell->u_caster;
			if(!u_caster && p_caster)
				u_caster = TO_UNIT(p_caster);
			g_caster = TO_GAMEOBJECT(caster);
		}break;
	case TYPEID_UNIT:
		{
			u_caster = TO_UNIT(caster);
		}break;
	case TYPEID_PLAYER:
		{
			p_caster = TO_PLAYER(caster);
			u_caster = TO_UNIT(caster);
		}break;
	}

	m_spellProto = pSpell->m_spellInfo;
	SetUInt64Value(DYNAMICOBJECT_CASTER, caster->GetGUID());

	m_uint32Values[OBJECT_FIELD_ENTRY] = m_spellProto->Id;
	m_uint32Values[DYNAMICOBJECT_BYTES] = 0x01eeeeee;
	m_uint32Values[DYNAMICOBJECT_SPELLID] = m_spellProto->Id;

	m_floatValues[DYNAMICOBJECT_RADIUS] = radius;
	m_floatValues[DYNAMICOBJECT_POS_X]  = x;
	m_floatValues[DYNAMICOBJECT_POS_Y]  = y;
	m_floatValues[DYNAMICOBJECT_POS_Z]  = z;
	m_uint32Values[DYNAMICOBJECT_CASTTIME] = (uint32)UNIXTIME;

	m_aliveDuration = duration;
	m_faction = caster->m_faction;
	m_factionDBC = caster->m_factionDBC;
	if(caster->dynObj != 0)
	{
		// expire next update
		caster->dynObj->m_aliveDuration = 1;
		caster->dynObj->UpdateTargets();
	}
	caster->dynObj = TO_DYNAMICOBJECT(shared_from_this());
	if(pSpell->g_caster)
	{
	   PushToWorld(pSpell->g_caster->GetMapMgr());
	}else 
		PushToWorld(caster->GetMapMgr());
	
  
	sEventMgr.AddEvent(TO_DYNAMICOBJECT(shared_from_this()), &DynamicObject::UpdateTargets, EVENT_DYNAMICOBJECT_UPDATE, 100, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void DynamicObject::AddInRangeObject( ObjectPointer pObj )
{
	if( pObj->IsUnit() )
	{
		bool attackable;
		if( p_caster != NULL)
			attackable = isAttackable( m_caster, pObj );
		else
			attackable = isAttackable( dyn_shared_from_this(), pObj );
		
		if( attackable )
			m_inRangeOppFactions.insert( TO_UNIT( pObj ) );
	}
	Object::AddInRangeObject( pObj );
}

void DynamicObject::OnRemoveInRangeObject( ObjectPointer pObj )
{
	if( pObj->IsUnit() )
	{
		m_inRangeOppFactions.erase( TO_UNIT( pObj ) );
		targets.erase( TO_UNIT(pObj) );
	}

	Object::OnRemoveInRangeObject( pObj );
}

void DynamicObject::UpdateTargets()
{
	if(m_aliveDuration == 0)
		return;

	if(m_aliveDuration >= 100)
	{
		UnitPointer target;
		float radius = m_floatValues[DYNAMICOBJECT_RADIUS]*m_floatValues[DYNAMICOBJECT_RADIUS];

		Object::InRangeSet::iterator itr,itr2;
		for( itr = GetInRangeSetBegin(); itr != GetInRangeSetEnd(); ++itr)
		{
			itr2 = itr;

			if( !( (*itr2)->IsUnit() ) || ! TO_UNIT( *itr2 )->isAlive() || ((*itr2)->GetTypeId()==TYPEID_UNIT && TO_CREATURE(*itr2)->IsTotem() ) )
				continue;

			target = TO_UNIT( *itr2 );

			if( !isAttackable( p_caster, target, !(m_spellProto->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				continue;

			// skip units already hit, their range will be tested later
			if(targets.find(target) != targets.end())
				continue;

			if(GetDistanceSq(target) <= radius)
			{
				AuraPointer pAura(new Aura(m_spellProto, m_aliveDuration, u_caster, target));
				for(uint32 i = 0; i < 3; ++i)
				{
					if(m_spellProto->Effect[i] == 27)
					{
						pAura->AddMod(m_spellProto->EffectApplyAuraName[i],
							m_spellProto->EffectBasePoints[i]+1, m_spellProto->EffectMiscValue[i], i);
					}
				}
				target->AddAura(pAura);
				if(p_caster)
				{
					p_caster->HandleProc(PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL,target, m_spellProto);
					p_caster->m_procCounter = 0;
				}

				// add to target list
				targets.insert(target);
			}
		}

		// loop the targets, check the range of all of them
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jtr2;
		DynamicObjectList::iterator jend = targets.end();
		
		while(jtr != jend)
		{
			target = *jtr;
			jtr2 = jtr;
			++jtr;

			if(GetDistanceSq(target) > radius)
			{
				targets.erase(jtr2);
				target->RemoveAura(m_spellProto->Id);
			}
		}

		m_aliveDuration -= 100;
	}
	else
	{
		m_aliveDuration = 0;
	}

	if(m_aliveDuration == 0)
	{
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jend = targets.end();
		UnitPointer target;

		while(jtr != jend)
		{
			target = *jtr;
			++jtr;
			target->RemoveAura(m_spellProto->Id);
		}

		Remove();
	}
}

void DynamicObject::Remove()
{
	if(IsInWorld())
		RemoveFromWorld(true);
	Destructor();
}

