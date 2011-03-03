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

void MapMgr::DespawnEvent(uint8 eventToRemove)
{
	if(_cells)
	{
		for (uint32 i = 0; i < _sizeX; i++)
		{
			if(_cells[i] != 0)
			{
				for (uint32 j = 0; j < _sizeY; j++)
				{
					if(_cells[i][j] != 0)
					{
						_cells[i][j]->_unloadpending=false;
						_cells[i][j]->RemoveEventIdObjects(eventToRemove);
						_cells[i][j]->ModifyEventIdSetting(false, eventToRemove);
					}
				}
			}
		}
	}
}

void MapMgr::SpawnEvent(uint8 eventId)
{
	CellSpawns * sp;
	if(_cells)
	{
		for (uint32 i = 0; i < _sizeX; i++)
		{
			if(_cells[i] != 0)
			{
				for (uint32 j = 0; j < _sizeY; j++)
				{
					if(_cells[i][j] != 0)
					{
						sp = _map->GetSpawnsList(i, j);
						_cells[i][j]->LoadEventIdObjects(sp, eventId);
						_cells[i][j]->ModifyEventIdSetting(true, eventId);
					}
				}
			}
		}
	}
}

void DayWatcherThread::LoadEventIdSettings()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM events");
	if (!result)
		return;

	QueryResult * result2;
	Field * fields;
	Events * events;
	do 
	{
		fields = result->Fetch();
		events = new Events;
		events->eventId = fields[0].GetUInt8();
		events->daynumber = fields[1].GetUInt8();
		events->monthnumber = fields[2].GetUInt8();
		events->activedays = fields[4].GetUInt8();
		result2 = CharacterDatabase.Query("SELECT lastactivated FROM events_settings WHERE eventid = %u", events->eventId);
		if(result2)
		{
			events->lastactivated = result2->Fetch()[0].GetUInt64();
			events->isactive = true;
			delete result2;
		}
		events->starthour = fields[5].GetUInt8();
		events->endhour = fields[6].GetUInt8();
		m_eventIdList.push_back(events);
	} while(result->NextRow());

	m_creatureEventSpawnMaps.clear();
	m_gameobjectEventSpawnMaps.clear();
	uint8 eventid;

	result = WorldDatabase.Query("SELECT eventid FROM events");
	if(result)
	{
		do
		{
			eventid = result->Fetch()[0].GetUInt8();
			result2 = WorldDatabase.Query("SELECT DISTINCT map FROM creature_spawns WHERE eventid = '%u'", eventid);
			if(result2)
			{
				do
				{
					m_creatureEventSpawnMaps.insert(pair<uint8, uint16>(eventid, result2->Fetch()[0].GetUInt16()));
				}while(result2->NextRow());
				delete result2;
			}
			result2 = WorldDatabase.Query("SELECT DISTINCT map FROM gameobject_spawns WHERE eventid = '%u'", eventid);
			if(result2)
			{
				do
				{
					m_gameobjectEventSpawnMaps.insert(pair<uint8, uint16>(eventid, result2->Fetch()[0].GetUInt16()));
				}while(result2->NextRow());
				delete result2;
			}
		}while(result->NextRow());
		delete result;
	}
	_loaded = true;
}

void MapCell::LoadEventIdObjects(CellSpawns * sp, uint8 eventId)
{
	Instance * pInstance = _mapmgr->pInstance;

	if(sp->CreatureSpawns.size())//got creatures
	{
		Vehicle* v;
		Creature* c;
		for(CreatureSpawnList::iterator i=sp->CreatureSpawns.begin();i!=sp->CreatureSpawns.end();i++)
		{
			if(pInstance)
			{
				if(pInstance->m_killedNpcs.find((*i)->id) != pInstance->m_killedNpcs.end())
					continue;
			}
			if((*i)->eventid && (*i)->eventid == eventId)
			{
				if(!((*i)->eventinfo->eventchangesflag & EVENTID_FLAG_SPAWN))
					continue;
				if((*i)->vehicle != 0)
				{
					v=_mapmgr->CreateVehicle((*i)->entry);

					v->SetMapId(_mapmgr->GetMapId());
					v->SetInstanceID(_mapmgr->GetInstanceID());
					v->m_loadedFromDB = true;

					if(v->Load(*i, _mapmgr->iInstanceMode, _mapmgr->GetMapInfo()))
					{
						if(!v->CanAddToWorld())
						{
							v->Destructor();
							continue;
						}

						v->PushToWorld(_mapmgr);
					}
					else
					{
						v->Destructor();
					}
				}
				else
				{
					c=_mapmgr->CreateCreature((*i)->entry);

					c->SetMapId(_mapmgr->GetMapId());
					c->SetInstanceID(_mapmgr->GetInstanceID());
					c->m_loadedFromDB = true;

					if(c->Load(*i, _mapmgr->iInstanceMode, _mapmgr->GetMapInfo()))
					{
						if(!c->CanAddToWorld())
						{
							c->Destructor();
							continue;
						}

						c->PushToWorld(_mapmgr);
					}
					else
					{
						c->Destructor();
					}
				}
			}
		}
	}

	if(sp->GOSpawns.size())//got GOs
	{
		GameObject* go;
		for(GOSpawnList::iterator i=sp->GOSpawns.begin();i!=sp->GOSpawns.end();i++)
		{
			if((*i)->eventid && (*i)->eventid == eventId)
			{
				if(!((*i)->eventinfo->eventchangesflag & EVENTID_FLAG_SPAWN))
					continue;
				
				go = _mapmgr->CreateGameObject((*i)->entry);
				if(go == NULL)
					continue;

				if(go->Load(*i))
				{
					go->m_loadedFromDB = true;
					go->PushToWorld(_mapmgr);
					CALL_GO_SCRIPT_EVENT(go, OnSpawn)();
				}
				else
				{
					go->Destructor();
				}
			}
		}
	}
}

void MapCell::ModifyEventIdSetting(bool active, uint8 eventId)
{
	ObjectSet::iterator itr;
	Object* pObject; //do this outside the loop!
	for(itr = _objects.begin(); itr != _objects.end();)
	{
		pObject = (*itr);
		itr++;

		if(!pObject)
			continue;

		switch(pObject->GetTypeId())
		{
		case TYPEID_UNIT: 
			{
				if( pObject->IsPet() )
					continue;

				if(!TO_CREATURE(pObject)->m_spawn->eventinfo)
					continue;

				if(TO_CREATURE(pObject)->m_spawn->eventid != eventId)
					continue;

				if(TO_CREATURE(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_MODELID)
					TO_CREATURE(pObject)->SetUInt32Value(UNIT_FIELD_DISPLAYID, active ? TO_CREATURE(pObject)->m_spawn->eventinfo->eventdisplayid : TO_CREATURE(pObject)->m_spawn->displayid);

				if(TO_CREATURE(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_PHASE)
					TO_CREATURE(pObject)->SetPhase(active ? TO_CREATURE(pObject)->m_spawn->eventinfo->eventphase : TO_CREATURE(pObject)->m_spawn->phase);

				if(TO_CREATURE(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_EQUIP)
				{
					if(TO_CREATURE(pObject)->m_spawn->eventinfo->eventitem1)
						TO_CREATURE(pObject)->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, active ? TO_CREATURE(pObject)->m_spawn->eventinfo->eventitem1 : TO_CREATURE(pObject)->proto->Item1);
					if(TO_CREATURE(pObject)->m_spawn->eventinfo->eventitem2)
						TO_CREATURE(pObject)->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID +1, active ? TO_CREATURE(pObject)->m_spawn->eventinfo->eventitem2 : TO_CREATURE(pObject)->proto->Item2);
					if(TO_CREATURE(pObject)->m_spawn->eventinfo->eventitem3)
						TO_CREATURE(pObject)->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID +2, active ? TO_CREATURE(pObject)->m_spawn->eventinfo->eventitem3 : TO_CREATURE(pObject)->proto->Item3);
				}
			}break;
		case TYPEID_GAMEOBJECT:
			{
				if(TO_GAMEOBJECT(pObject)->m_spawn->eventid != eventId)
					continue;

				if(!TO_GAMEOBJECT(pObject)->m_spawn->eventinfo)
					continue;

				if(TO_GAMEOBJECT(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_MODELID)
				{
					if(active)
					{
						TO_GAMEOBJECT(pObject)->SetUInt32Value(GAMEOBJECT_DISPLAYID, TO_GAMEOBJECT(pObject)->m_spawn->eventinfo->eventdisplayid);
					}
					else
					{
						GameObjectInfo * pInfo = GameObjectNameStorage.LookupEntry(pObject->GetEntry());
						TO_GAMEOBJECT(pObject)->SetUInt32Value(GAMEOBJECT_DISPLAYID, pInfo->DisplayID);
					}
				}

				if(TO_GAMEOBJECT(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_PHASE)
					TO_CREATURE(pObject)->SetPhase(active ? TO_CREATURE(pObject)->m_spawn->eventinfo->eventphase : TO_CREATURE(pObject)->m_spawn->phase);
			}break;
		default:
			{
				continue;
			}break;
		}
	}
}

void MapCell::RemoveEventIdObjects(uint8 eventToRemove)
{
	if(!eventToRemove)
		return;

	ObjectSet::iterator itr;

	/* delete objects in pending respawn state */
	Object* pObject;
	for(itr = _respawnObjects.begin(); itr != _respawnObjects.end(); ++itr)
	{
		pObject = *itr;
		if(!pObject)
			continue;
		
		switch(pObject->GetTypeId())
		{
		case TYPEID_UNIT: 
			{
				if(!(TO_CREATURE(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_SPAWN))
					continue;
				if(TO_CREATURE(pObject)->m_spawn->eventid == eventToRemove)
				{
					if( pObject->IsVehicle())
					{
						_mapmgr->_reusable_guids_vehicle.push_back( pObject->GetUIdFromGUID() );
						TO_VEHICLE(pObject)->m_respawnCell=NULL;
						TO_VEHICLE(pObject)->Destructor();
						_respawnObjects.erase(pObject);
					}
					else if( !pObject->IsPet() )
					{
						_mapmgr->_reusable_guids_creature.push_back( pObject->GetUIdFromGUID() );
						TO_CREATURE(pObject)->m_respawnCell=NULL;
						TO_CREATURE(pObject)->Destructor();
						_respawnObjects.erase(pObject);
					}
				}
			}break;

		case TYPEID_GAMEOBJECT:
			{
				if(!(TO_GAMEOBJECT(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_SPAWN))
					continue;

				if(TO_GAMEOBJECT(pObject)->m_spawn->eventid == eventToRemove)
				{
					TO_GAMEOBJECT(pObject)->m_respawnCell=NULL;
					TO_GAMEOBJECT(pObject)->Destructor();
					_respawnObjects.erase(pObject);
				}break;
			}
		}
	}

	for(itr = _objects.begin(); itr != _objects.end();)
	{
		pObject = (*itr);
		itr++;

		if(!pObject)
			continue;

		switch(pObject->GetTypeId())
		{
		case TYPEID_UNIT: 
			{
				if( pObject->IsPet() )
					continue;

				if(TO_CREATURE(pObject)->m_spawn->eventid != eventToRemove)
					continue;

				if(!(TO_CREATURE(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_SPAWN))
					continue;
			}break;
		case TYPEID_GAMEOBJECT:
			{
				if(TO_GAMEOBJECT(pObject)->m_spawn->eventid != eventToRemove)
					continue;

				if(!(TO_GAMEOBJECT(pObject)->m_spawn->eventinfo->eventchangesflag & EVENTID_FLAG_SPAWN))
					continue;
			}break;
		default:
			{
				continue;
			}break;
		}

		if( _unloadpending )
		{			
			if(!pObject->m_loadedFromDB)
				continue;
		}

		if( pObject->Active )
			pObject->Deactivate( _mapmgr );

		if( pObject->IsInWorld() )
			pObject->RemoveFromWorld( true );

		pObject->Destructor();

	}
}

bool DayWatcherThread::has_eventid_timeout_expired(tm * now_time, int updatetime, uint8 timeoutval)
{
	switch(timeoutval)
	{
		case MONTHLY:
			return (now_time->tm_mon == updatetime);

		case DAILY:
			return (now_time->tm_mday == updatetime);
	}
	return false;
}

bool DayWatcherThread::CheckHourlyEvent(tm * now_time, uint8 starthour, uint8 endhour)
{
	if(starthour < endhour)
		return( now_time->tm_hour >= starthour || (now_time->tm_hour > 0 &&  now_time->tm_hour <= endhour) );
	else
		return( now_time->tm_hour >= endhour);
}

bool DayWatcherThread::has_eventid_expired(int activedays, time_t lastactivated)
{
	return ( UNIXTIME >= lastactivated + (TIME_DAY * activedays) );
}

void DayWatcherThread::update_event_settings(uint8 eventid, time_t activated)
{
	if(activated)
		CharacterDatabase.Execute("REPLACE INTO events_settings VALUES('%u','%u')", eventid, activated);
	else
		CharacterDatabase.Execute("DELETE FROM events_settings where eventid = %u", eventid);
}

bool DayWatcherThread::SpawnEventId(uint8 eventId, bool activate)
{
	MapMgr* mgr;
	CreatureEventSpawnMaps::iterator itr = m_creatureEventSpawnMaps.find(eventId);
	if(itr != m_creatureEventSpawnMaps.end())
	{
		do
		{
			mgr = sInstanceMgr.GetMapMgr(itr->second);
			if(mgr)
				if(activate)
				{
					if(!sEventMgr.HasEvent(mgr, EVENT_SPAWN_ING_EVENT))
					{
						sEventMgr.AddEvent(mgr, &MapMgr::SpawnEvent, eventId, EVENT_SPAWN_ING_EVENT, 10000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					}
				}
				else
				{
					if(!sEventMgr.HasEvent(mgr, EVENT_DESPAWN_ING_EVENT))
					{
						sEventMgr.AddEvent(mgr, &MapMgr::DespawnEvent, eventId, EVENT_DESPAWN_ING_EVENT, 10000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					}
				}
			else
			{
				runEvents = true;
				return false;
			}
			itr++;
		}while(itr != m_creatureEventSpawnMaps.upper_bound(eventId));
	}
	GameobjectEventSpawnMaps::iterator itr2 = m_gameobjectEventSpawnMaps.find(eventId);
	if(itr2 != m_gameobjectEventSpawnMaps.end())
	{
		do
		{
			mgr = sInstanceMgr.GetMapMgr(itr2->second);
			if(mgr)
				if(activate)
				{
					if(!sEventMgr.HasEvent(mgr, EVENT_SPAWN_ING_EVENT))
					{
						sEventMgr.AddEvent(mgr, &MapMgr::SpawnEvent, eventId, EVENT_SPAWN_ING_EVENT, 10000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					}
				}
				else
				{
					if(!sEventMgr.HasEvent(mgr, EVENT_DESPAWN_ING_EVENT))
					{
						sEventMgr.AddEvent(mgr, &MapMgr::DespawnEvent, eventId, EVENT_DESPAWN_ING_EVENT, 10000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					}
				}
			else
			{
				runEvents = true;
				return false;
			}
			itr2++;
		}while(itr2 != m_gameobjectEventSpawnMaps.upper_bound(eventId));
	}
	return true;
}
