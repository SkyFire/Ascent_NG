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

bool isHostile(Object* objA, Object* objB)// B is hostile for A?
{
	bool hostile = false;

	// can't attack self.. this causes problems with buffs if we dont have it :p
	if(!objA || !objB || (objA == objB))
		return false;

	// can't attack corpses neither...
	if(objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE)
		return false;

	if( objA->m_faction == NULL || objB->m_faction == NULL || objA->m_factionDBC == NULL || objB->m_factionDBC == NULL )
		return true;

	uint32 faction = objB->m_faction->Mask;
	uint32 host = objA->m_faction->HostileMask;

	if(faction & host)
		hostile = true;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == objB->m_faction->Faction)
		{
			hostile = true;
			break;
		}
		if(objA->m_faction->FriendlyFactions[i] == objB->m_faction->Faction)
		{
			hostile = false;
			break;
		}
	}

	// PvP Flag System Checks
	// We check this after the normal isHostile test, that way if we're
	// on the opposite team we'll already know :p

	Player* player_objA = GetPlayerFromObject(objA);
	Player* player_objB = GetPlayerFromObject(objB);

	//BG or PVP?
	if( player_objA && player_objB )
	{
		if( player_objA->m_bg != NULL )	
		{
			if( player_objA->m_bgTeam != player_objB->m_bgTeam )
				return true;
		}
		if( hostile && player_objA->IsPvPFlagged()&& player_objB->IsPvPFlagged() )
			return true;
		else
			return false;
	}


	// Reputation System Checks
	if(player_objA && !player_objB)	   // PvE
	{
		if(objB->m_factionDBC->RepListId >= 0)
			hostile = player_objA->IsHostileBasedOnReputation( objB->m_factionDBC );
	}
	
	if(player_objB && !player_objA)	   // PvE
	{
		if(objA->m_factionDBC->RepListId >= 0)
			hostile = player_objB->IsHostileBasedOnReputation( objA->m_factionDBC );
	}

	if( objA->IsPlayer() && objB->IsPlayer() && TO_PLAYER(objA)->m_bg != NULL )
	{
		if( TO_PLAYER(objA)->m_bgTeam != TO_PLAYER(objB)->m_bgTeam )
			return true;
	}

	return hostile;
}

/// Where we check if we object A can attack object B. This is used in many feature's
/// Including the spell class and the player class.
bool isAttackable(Object* objA, Object* objB, bool CheckStealth)// A can attack B?
{
	// can't attack self.. this causes problems with buffs if we don't have it :p
	if( !objA || !objB || objA == objB )
		return false;   

	// can't attack corpses neither...
	if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
		return false;

	// We do need all factiondata for this
	if( objB->m_factionDBC == NULL || objA->m_factionDBC == NULL || objB->m_faction == NULL || objA->m_faction == NULL )
		return false;

	// Checks for untouchable, unattackable
	if( objA->IsUnit() && objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_NOT_SELECTABLE))
		return false;
	if( objB->IsUnit() && objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_NOT_SELECTABLE))
		return false;

	// we cannot attack sheathed units. Maybe checked in other places too ?
	// !! warning, this presumes that objA is attacking ObjB
	if( CheckStealth && objB->IsUnit() && TO_UNIT(objB)->InStealth() )
		return false;

	//Get players (or owners of pets/totems)
	Player* player_objA = GetPlayerFromObject(objA);
	Player* player_objB = GetPlayerFromObject(objB);
	if( player_objA && player_objB )
	{
		//Handle duels
		if( player_objA->DuelingWith == player_objB && player_objA->GetDuelState() == DUEL_STATE_STARTED )
			return true;

		//These area's are sanctuaries
		for(uint32 i = 0; i < NUM_SANCTUARIES ; ++i)
		{
			if( player_objA->GetAreaID() == SANCTUARY_ZONES[i] || player_objB->GetAreaID() == SANCTUARY_ZONES[i])
				return false;
		}

		// Players with feign death flags can't be attacked
		// But they can be attacked by another players. -- Dvlpr
		// WARNING: This presumes, that objA attacks objb!!!
		if( (objA->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || objB->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH)) && !objA->IsPlayer() )
			return false;

		// do not let people attack each other in sanctuary
		// We know they aren't dueling
		AreaTable *atA = NULL;
		AreaTable *atB = NULL;
		atA = dbcArea.LookupEntry( player_objA->GetAreaID() );
		atB = dbcArea.LookupEntry( player_objB->GetAreaID() );
		if ( atA && atB && (atA->AreaFlags & AREA_SANCTUARY || atB->AreaFlags & AREA_SANCTUARY) )
			return false;

		//Handle BG's
		if( player_objA->m_bg != NULL)
		{
			//Handle ffa_PVP
			if( player_objA->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP) && player_objA->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP))
			{
				if( player_objA->GetGroup() == player_objB->GetGroup() )
					return false;
				else
					return true;
			}
			//Handle Arenas
			if( player_objA->GetTeam() != player_objB->GetTeam() )
				return true;
		}

		if(player_objA->IsFFAPvPFlagged() && player_objB->IsFFAPvPFlagged())
		{
			if( player_objA->GetGroup() && player_objA->GetGroup() == player_objB->GetGroup() )
				return false;

			if( player_objA == player_objB ) // Totems...
				return false;

			return true;		// can hurt each other in FFA pvp
		}

		if( player_objA->GetAreaDBC() != NULL )
		{
			if( player_objA->GetAreaDBC()->AreaFlags & 0x800 )
				return false;
		}
	}	

	// same faction can't kill each other.
	if(objA->m_faction == objB->m_faction)  
		return false;

	//moved this from IsHostile(); 
	// by doing so we skip a hell of a lot redundant checks, which we already passed in this routine.
	uint32 faction = objB->m_faction->Mask;
	uint32 host = objA->m_faction->HostileMask;
	bool hostile = false;
	if(faction & host)
	{
		hostile = true;
		// check friend/enemy list
		for(uint32 i = 0; i < 4; i++)
		{
			if(objA->m_faction->EnemyFactions[i] == objB->m_faction->Faction)
				hostile = true;
			if(objA->m_faction->FriendlyFactions[i] == objB->m_faction->Faction)
				hostile = false;
		}
	}

	// Reputation System Checks
	if(player_objA && !player_objB)	   // PvE
	{
		if(objB->m_factionDBC->RepListId >= 0)
			hostile = player_objA->IsHostileBasedOnReputation( objB->m_factionDBC );
	}
	
	if(player_objB && !player_objA)	   // PvE
	{
		if(objA->m_factionDBC->RepListId >= 0)
			hostile = player_objB->IsHostileBasedOnReputation( objA->m_factionDBC );
	}

	// Neutral Creature Check
	if(player_objA)
	{
		if(objB->m_factionDBC->RepListId == -1 && objB->m_faction->HostileMask == 0 && objB->m_faction->FriendlyMask == 0)
			hostile =  true;
	}
	else if(player_objB)
	{
		if(objA->m_factionDBC->RepListId == -1 && objA->m_faction->HostileMask == 0 && objA->m_faction->FriendlyMask == 0)
			hostile =  true;
	}
	return hostile;
}
Player* GetPlayerFromObject(Object* obj)
{
	Player* player_obj = NULL;

	if( obj->IsPlayer() )
	{
		player_obj =  TO_PLAYER( obj );
	}
	else if( obj->IsPet() )
	{
		Pet* pet_obj = TO_PET(obj);
		if( pet_obj )
			player_obj =  pet_obj->GetPetOwner();
	}
	else if( obj->IsUnit() )
	{
		// If it's not a player nor a pet, it can still be a totem.
		Creature* creature_obj = TO_CREATURE(obj);
		if( creature_obj && creature_obj->IsTotem()) 
			player_obj =  TO_PLAYER(creature_obj->GetSummonOwner());
	}
	return player_obj;
}
bool isCombatSupport(Object* objA, Object* objB)// B combat supports A?
{
	if( !objA || !objB )
		return false;   

	// can't support corpses...
	if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
		return false;

	// We do need all factiondata for this
	if( objB->m_factionDBC == NULL || objA->m_factionDBC == NULL || objB->m_faction == NULL || objA->m_faction == NULL )
		return false;

	bool combatSupport = false;

	uint32 fSupport = objB->m_faction->FriendlyMask;
	uint32 myFaction = objA->m_faction->Mask;

	if(myFaction & fSupport)
		combatSupport = true;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objB->m_faction->EnemyFactions[i] == objA->m_faction->Faction)
		{
			combatSupport = false;
			break;
		}
		if(objB->m_faction->FriendlyFactions[i] == objA->m_faction->Faction)
		{
			combatSupport = true;
			break;
		}
	}
	return combatSupport;
}


bool isAlliance(Object* objA)// A is alliance?
{
	if(!objA || objA->m_factionDBC == NULL || objA->m_faction == NULL)
		return true;

	//Get stormwind faction frm dbc (11/72)
	FactionTemplateDBC * m_sw_faction = dbcFactionTemplate.LookupEntry(11);
	FactionDBC * m_sw_factionDBC = dbcFaction.LookupEntry(72);

	if(m_sw_faction == objA->m_faction || m_sw_factionDBC == objA->m_factionDBC)
		return true;

	//Is StormWind hostile to ObjectA?
	uint32 faction = m_sw_faction->Faction;
	uint32 hostilemask = objA->m_faction->HostileMask;

	if(faction & hostilemask)
		return false;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == faction)
			return false;
	}

	//Is ObjectA hostile to StormWind?
	faction = objA->m_faction->Faction;
	hostilemask = m_sw_faction->HostileMask;

	if(faction & hostilemask)
		return false;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == faction)
			return false;
	}

	//We're not hostile towards SW, so we are allied
	return true;
}


