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


	m_parentSpell=NULL;
	m_aliveDuration = 0;
	u_caster = NULL;
	m_spellProto = 0;
	p_caster = NULL;
	m_caster = NULL;
	g_caster = NULL;
}

DynamicObject::~DynamicObject()
{
	// remove aura from all targets
	DynamicObjectList::iterator jtr  = targets.begin();
	DynamicObjectList::iterator jend = targets.end();
	Unit* target;

	while(jtr != jend)
	{
		target = *jtr;
		++jtr;
		target->RemoveAura(m_spellProto->Id);
	}

	if(m_caster && m_caster->dynObj == this )
		m_caster->dynObj = NULL;

	m_parentSpell=NULL;
	m_aliveDuration = 0;
	u_caster = NULL;
	m_spellProto = 0;
	p_caster = NULL;
	m_caster = NULL;
	g_caster = NULL;

}

void DynamicObject::Init()
{
	Object::Init();
}

void DynamicObject::Destructor()
{
	delete this;
}

void DynamicObject::Create(Object* caster, Spell* pSpell, float x, float y, float z, uint32 duration, float radius)
{
	Object::_Create(caster->GetMapId(),x, y, z, 0);
	if(pSpell->g_caster)
		m_parentSpell = pSpell;

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
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
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
	caster->dynObj = TO_DYNAMICOBJECT(this);

	PushToWorld(caster->GetMapMgr());

	sEventMgr.AddEvent(TO_DYNAMICOBJECT(this), &DynamicObject::UpdateTargets, EVENT_DYNAMICOBJECT_UPDATE, 200, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void DynamicObject::AddInRangeObject( Object* pObj )
{
	if( pObj->IsUnit() )
	{
		bool attackable;
		if( p_caster != NULL)
			attackable = isAttackable( m_caster, pObj );
		else
			attackable = isAttackable( TO_DYNAMICOBJECT(this), pObj );
		
		if( attackable )
			m_inRangeOppFactions.insert( TO_UNIT( pObj ) );
	}
	Object::AddInRangeObject( pObj );
}

void DynamicObject::OnRemoveInRangeObject( Object* pObj )
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

	if(m_aliveDuration >= 200)
	{
		Unit* target;

		float radius = m_floatValues[DYNAMICOBJECT_RADIUS]*m_floatValues[DYNAMICOBJECT_RADIUS];

		Object::InRangeSet::iterator itr,itr2;
		for( itr = GetInRangeSetBegin(); itr != GetInRangeSetEnd(); ++itr)
		{
			itr2 = itr;

			if( !( (*itr2)->IsUnit() ) || ! TO_UNIT( *itr2 )->isAlive() || ((*itr2)->GetTypeId()==TYPEID_UNIT && TO_CREATURE(*itr2)->IsTotem() ) )
				continue;

			target = TO_UNIT( *itr2 );

			if( !isAttackable( m_caster, target, !(m_spellProto->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				continue;

			// skip units already hit, their range will be tested later
			if(targets.find(target) != targets.end())
				continue;

			if(GetDistanceSq(target) <= radius)
			{
				Aura* pAura(new Aura(m_spellProto, m_aliveDuration, m_caster, target));

				for(uint32 i = 0; i < 3; ++i)
				{
					if(m_spellProto->Effect[i] == 27)
					{
						pAura->AddMod(m_spellProto->EffectApplyAuraName[i],
							m_spellProto->EffectBasePoints[i]+1, m_spellProto->EffectMiscValue[i], i);
					}
				}
				target->AddAura(pAura);
				if( m_caster->IsPlayer() )
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

		m_aliveDuration -= 200;
	}
	else
	{
		m_aliveDuration = 0;
	}

	if(m_aliveDuration == 0)
	{
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jend = targets.end();
		Unit* target;

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

