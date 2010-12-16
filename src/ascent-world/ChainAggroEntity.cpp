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

ChainAggroEntity::ChainAggroEntity(Creature* pOwner)
{
	AddAggroEntity(pOwner);
}

ChainAggroEntity::~ChainAggroEntity()
{

}

void ChainAggroEntity::AddAggroEntity(Creature* pCreature)
{
	pCreature->GetAIInterface()->m_ChainAgroSet = this;
	m_agroEntitySet.insert(pCreature);
}

void ChainAggroEntity::RemoveAggroEntity(Creature* pCreature)
{
	pCreature->GetAIInterface()->m_ChainAgroSet = NULL;
	m_agroEntitySet.erase(pCreature);

	// Oh noes, I am with the worms!
	if(m_agroEntitySet.size() == 0)
	{
		delete this;
	}
}

void ChainAggroEntity::Delete()
{
	set<Creature*>::iterator itr = m_agroEntitySet.begin();
	for(; itr != m_agroEntitySet.end(); ++itr)
	{
		(*itr)->GetAIInterface()->m_ChainAgroSet = NULL;
	}
	delete this;
}

void ChainAggroEntity::EventEnterCombat(Unit* pTarget)
{
	set<Creature*>::iterator itr = m_agroEntitySet.begin();
	for(; itr != m_agroEntitySet.end(); ++itr)
	{
		if( (*itr)->isDead() )
			continue;

		(*itr)->GetAIInterface()->AttackReaction( pTarget, 1 );
	}
}
