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

#ifndef _CHAIN_AGRO_ENTITY_H
#define _CHAIN_AGRO_ENTITY_H

// Class used to link multiple monsters together for agro.
class SERVER_DECL ChainAggroEntity
{
	friend class AIInterface;

private:
	set<Creature*> m_agroEntitySet;
public:
	ChainAggroEntity(Creature* pOwner);
	~ChainAggroEntity();

	static ChainAggroEntity* Create(Creature* pCreature) { return new ChainAggroEntity(pCreature); }

	void AddAggroEntity(Creature* pCreature);
	void RemoveAggroEntity(Creature* pCreature);

	void Delete();

	void EventEnterCombat(Unit* pTarget);
};


#endif
