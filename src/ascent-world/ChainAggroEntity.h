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

#ifndef _CHAIN_AGRO_ENTITY_H
#define _CHAIN_AGRO_ENTITY_H

// Class used to link multiple monsters together for agro.
class SERVER_DECL ChainAggroEntity
{
	friend class AIInterface;

private:
	set<CreaturePointer> m_agroEntitySet;
public:
	ChainAggroEntity(CreaturePointer pOwner);
	~ChainAggroEntity();

	static ChainAggroEntity* Create(CreaturePointer pCreature) { return new ChainAggroEntity(pCreature); }

	void AddAggroEntity(CreaturePointer pCreature);
	void RemoveAggroEntity(CreaturePointer pCreature);

	void Delete();

	void EventEnterCombat(UnitPointer pTarget);
};


#endif
