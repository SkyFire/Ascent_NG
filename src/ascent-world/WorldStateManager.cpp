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
initialiseSingleton(WorldStateTemplateManager);

/** World State Manager Implementation Class **/

// creates world state structure
void WorldStateManager::CreateWorldState(uint32 uWorldStateId, uint32 uInitialValue, int32 iFactionMask /* = FACTION_MASK_ALL */, int32 iZoneMask /* = ZONE_MASK_ALL */)
{
	WorldState newState;
	WorldStateMap::iterator itr;

	// lockit!
	//m_lock.Acquire();

	// search away, for naughty people
	itr = m_states.find(uWorldStateId);
	
	// does it already exist? naughty boy.
	if( itr != m_states.end() )
	{
		itr->second.FactionMask = iFactionMask;
		itr->second.ZoneMask = iZoneMask;
		itr->second.Value = uInitialValue;
	}
	else
	{
		// set the worldstate, add it to the list.
		newState.FactionMask = iFactionMask;
		newState.ZoneMask = iZoneMask;
		newState.Value = uInitialValue;

		// add!
		m_states.insert( make_pair( uWorldStateId, newState) );
	}

	// release the lock
	//m_lock.Release();
}

// updates a world state with a new value
void WorldStateManager::UpdateWorldState(uint32 uWorldStateId, uint32 uValue)
{
	WorldStateMap::iterator itr;
	uint8 msgdata[10];
	StackPacket data(SMSG_UPDATE_WORLD_STATE, msgdata, 10);

	// lockit!
	//m_lock.Acquire();

	if( !m_states.size() )
		return;

	// we should be existant.
	itr = m_states.find(uWorldStateId);

	if( itr == m_states.end() )
	{
		// otherwise try to create it
		DEBUG_LOG("WorldState","Creating new world state %u with value %u for map %u.", uWorldStateId, uValue, m_mapMgr->GetMapId());
		CreateWorldState(uWorldStateId, uValue);
		itr = m_states.find(uWorldStateId);
		if( itr == m_states.end() )
		{
			//Creation of worldstate failed, abort !
			//m_lock.Release();
			Log.Error("WorldState","Creation of world state %u with value %u for map %u failed!", uWorldStateId, uValue, m_mapMgr->GetMapId());
			return;
		}
		//m_lock.Release();
	}

	// set the new value
	itr->second.Value = uValue;

	// build the new packet
	data << uWorldStateId << uValue;

	// send to the appropriate players
	m_mapMgr->SendPacketToPlayers(itr->second.ZoneMask, itr->second.FactionMask, &data);

	// release the lock
	//m_lock.Release();
}

void WorldStateManager::SendWorldStates(Player* pPlayer)
{
	// be threadsafe! wear a mutex!
	//m_lock.Acquire();

	if( !m_states.size() )
		return;

	WorldPacket data(SMSG_INIT_WORLD_STATES, (m_states.size() * 8) + 32);
	WorldStateMap::iterator itr;
	uint32 state_count = 0;

	// header
	data << m_mapMgr->GetMapId();
	//data << m_property1;
	//data << m_property2;
	data << pPlayer->GetZoneId();
	data << pPlayer->GetAreaID();

	// set this to zero, since the count can be variable
	data << (uint16)0;

	// add states to packet
	for(itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		if( itr->second.FactionMask != FACTION_MASK_ALL && itr->second.FactionMask != (int32)pPlayer->GetTeam() )
			continue;

		if( itr->second.ZoneMask != ZONE_MASK_ALL && itr->second.ZoneMask != (int32)pPlayer->GetZoneId() )
			continue;

		// add away
		data << itr->first << itr->second.Value;

		// increment
		++state_count;
	}

	// unlock
	//m_lock.Release();

	// append the count, and send away
	*(uint16*)(&data.contents()[12]) = state_count;
	pPlayer->GetSession()->SendPacket(&data);	
}

void WorldStateManager::ClearWorldStates(Player* pPlayer)
{
	// clears the clients view of world states for this map
	uint8 msgdata[10];
	StackPacket data(SMSG_INIT_WORLD_STATES, msgdata, 10);

	// map=0
	// data1=0
	// data2=0
	// valcount=0
	data << uint32(0) << uint16(0) << uint16(0) << uint16(0);

	// send
	pPlayer->GetSession()->SendPacket(&data);
}

const string WorldStateManager::GetPersistantSetting(const char *szKeyName, const char *szDefaultValue)
{
	QueryResult * pResult = CharacterDatabase.Query("SELECT setting_value FROM worldstate_save_data WHERE setting_id = \"%s\"", 
		CharacterDatabase.EscapeString(string(szKeyName)).c_str());

	if( pResult == NULL )
		return string(szDefaultValue);

	string ret = string(pResult->Fetch()[0].GetString());
	delete pResult;
	return ret;
}

void WorldStateManager::SetPersistantSetting(const char *szKeyName, const char *szValue)
{
	string pkey = string(szKeyName);
	string pval = string(szValue);

	pkey = CharacterDatabase.EscapeString(pkey);
	pval = CharacterDatabase.EscapeString(pval);

	CharacterDatabase.Execute("REPLACE INTO worldstate_save_data VALUES(\"%s\", \"%s\")", pkey.c_str(), pval.c_str());
}


//////////////////////////////////////////////////////////////////////////
// Template Manager
//////////////////////////////////////////////////////////////////////////

void WorldStateTemplateManager::LoadFromDB()
{
	QueryResult * pResult = WorldDatabase.Query("SELECT * FROM worldstate_template");
	if( pResult == NULL )
		return;

	Field *fields;
	WorldStateTemplate tmpl;
	int32 mapid;
	do 
	{
		fields = pResult->Fetch();
		mapid = fields[0].GetInt32();
		tmpl.iZoneMask = fields[1].GetInt32();
		tmpl.iFactionMask = fields[2].GetInt32();
		tmpl.uField = fields[3].GetUInt32();
		tmpl.uValue = fields[4].GetUInt32();

		if( mapid == -1 )
		{
			m_general.push_back(tmpl);
		}
		else
		{
			if( mapid >= NUM_MAPS )
			{
				Log.LargeErrorMessage(LARGERRORMESSAGE_WARNING, "Worldstate template for field %u on map %u (%s) contains out of range map.", 
					tmpl.uField, mapid, fields[5].GetString());

				continue;
			}

			m_templatesForMaps[mapid].push_back(tmpl);
		}
	} while (pResult->NextRow());
	delete pResult;
}

void WorldStateTemplateManager::ApplyMapTemplate(MapMgr* pmgr)
{
	WorldStateTemplateList::iterator itr = m_templatesForMaps[pmgr->GetMapId()].begin();
	WorldStateTemplateList::iterator itrend = m_templatesForMaps[pmgr->GetMapId()].end();
	for(; itr != itrend; ++itr)
		pmgr->GetStateManager().CreateWorldState(itr->uField, itr->uValue, itr->iFactionMask, itr->iZoneMask);

	itr = m_general.begin();
	itrend = m_general.end();
	for(; itr != itrend; ++itr)
		pmgr->GetStateManager().CreateWorldState(itr->uField, itr->uValue, itr->iFactionMask, itr->iZoneMask);
}
