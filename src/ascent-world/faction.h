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

#ifndef __FACTION_H
#define __FACTION_H

#include "Unit.h"

SERVER_DECL bool isHostile(ObjectPointer objA, ObjectPointer objB); // B is hostile for A?
SERVER_DECL bool isAttackable(ObjectPointer objA, ObjectPointer objB, bool CheckStealth = true); // A can attack B?
SERVER_DECL bool isCombatSupport(ObjectPointer objA, ObjectPointer objB); // B combat supports A?;
SERVER_DECL bool isAlliance(ObjectPointer objA); // A is alliance?

ASCENT_INLINE bool isFriendly(ObjectPointer objA, ObjectPointer objB)// B is friendly to A if its not hostile
{
	return !isHostile(objA, objB);
}

ASCENT_INLINE bool isSameFaction(ObjectPointer objA, ObjectPointer objB)
{
	// shouldn't be necessary but still
	if( objA->m_faction == NULL || objB->m_faction == NULL )
		return false;

	return (objB->m_faction->Faction == objA->m_faction->Faction);
}
ASCENT_INLINE  PlayerPointer GetPlayerFromObject(ObjectPointer obj); 
#endif
