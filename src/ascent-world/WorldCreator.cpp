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

//
// WorldCreator.cpp
//

#include "StdAfx.h"

initialiseSingleton( FormationMgr );
SERVER_DECL InstanceMgr sInstanceMgr;

InstanceMgr::InstanceMgr()
{
	memset(m_maps, 0, sizeof(Map*)* NUM_MAPS);
	memset(m_instances, 0, sizeof(InstanceMap*) * NUM_MAPS);
	
	for(uint32 i = 0; i < NUM_MAPS; ++i)
		m_singleMaps[i] = NULLMAPMGR;
}

void InstanceMgr::Load(TaskList * l)
{
	new FormationMgr;
	new WorldStateTemplateManager;

	sWorldStateTemplateManager.LoadFromDB();

	// Create all non-instance type maps.
	QueryResult *result = CharacterDatabase.Query( "SELECT MAX(id) FROM instances" );
	if( result )
	{
		m_InstanceHigh = result->Fetch()[0].GetUInt32()+1;
		delete result;
	}
	else
		m_InstanceHigh = 1;

	// load each map we have in the database.
	result = WorldDatabase.Query("SELECT DISTINCT Map FROM creature_spawns");
	if(result)
	{
		do 
		{
			if(WorldMapInfoStorage.LookupEntry(result->Fetch()[0].GetUInt32()) == NULL)
				continue;

			if( result->Fetch()[0].GetUInt32() >= NUM_MAPS )
			{
				Log.Warning("InstanceMgr", "One or more of your creature_spawns rows specifies an invalid map: %u", result->Fetch()[0].GetUInt32() );
				continue;
			}

			//_CreateMap(result->Fetch()[0].GetUInt32());
			l->AddTask(new Task(new NoSharedPtrCallbackP1<InstanceMgr,uint32>(this, &InstanceMgr::_CreateMap, result->Fetch()[0].GetUInt32())));
		} while(result->NextRow());
		delete result;
	}

	l->wait();

	// create maps for any we don't have yet.
	StorageContainerIterator<MapInfo> * itr = WorldMapInfoStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		if( itr->Get()->mapid >= NUM_MAPS )
		{
			Log.Warning("InstanceMgr", "One or more of your worldmap_info rows specifies an invalid map: %u", itr->Get()->mapid );
			itr->Inc();
			continue;
		}

#ifdef EXCLUDE_TEST_MAPS
		MapEntry *me = dbcMap.LookupEntry(itr->Get()->mapid);
		if (me && !me->multimap_id)
		{
			Log.Notice("InstanceMgr", "Skipped test map: %u (AscentConfig.h)", itr->Get()->mapid );
			itr->Inc();
			continue;
		}
#endif
		if(m_maps[itr->Get()->mapid] == NULL)
		{
			l->AddTask(new Task(new NoSharedPtrCallbackP1<InstanceMgr,uint32>(this, &InstanceMgr::_CreateMap, itr->Get()->mapid)));
		}

		if( itr->Get()->flags != 1 && itr->Get()->cooldown == 0 )
		{
			Log.Warning("InstanceMgr", "Your worldmap_info has no cooldown for map %u.", itr->Get()->mapid);
			itr->Get()->cooldown = TIME_MINUTE * 30;
		}
		//_CreateMap(itr->Get()->mapid);

		itr->Inc();
	}
	itr->Destruct();
	l->wait();

	// load saved instances
	_LoadInstances();
}

InstanceMgr::~InstanceMgr()
{
	delete WorldStateTemplateManager::getSingletonPtr();
}

void InstanceMgr::Shutdown()
{
	uint32 i;
	InstanceMap::iterator itr;
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_instances[i] != NULL)
		{
			for(itr = m_instances[i]->begin(); itr != m_instances[i]->end(); ++itr)
			{
				if(itr->second->m_mapMgr)
					itr->second->m_mapMgr->KillThread();

				delete itr->second;
			}

			delete m_instances[i];
			m_instances[i]=NULL;
		}

		if(m_singleMaps[i] != NULL)
		{
			MapMgrPointer ptr = m_singleMaps[i];
			ptr->KillThread();
			ptr->Destructor();
			ptr = NULLMAPMGR;
			m_singleMaps[i]=NULLMAPMGR;// and it dies :)
		}

		if(m_maps[i] != NULL)
		{
			delete m_maps[i];
			m_maps[i]=NULL;
		}
	}

	delete FormationMgr::getSingletonPtr();
}

uint32 InstanceMgr::PreTeleport(uint32 mapid, PlayerPointer plr, uint32 instanceid)
{
	// preteleport is where all the magic happens :P instance creation, etc.
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(mapid);
	InstanceMap * instancemap;
	Instance * in = NULL;

	//is the map vaild?
	if(inf == NULL || mapid>=NUM_MAPS)
		return INSTANCE_ABORT_NOT_FOUND;

	// main continent check.
	if(inf->type == INSTANCE_NULL)
	{
		// this will be useful when clustering comes into play.
		// we can check if the destination world server is online or not and then cancel them before they load.
		return (m_singleMaps[mapid] != NULL) ? INSTANCE_OK : INSTANCE_ABORT_NOT_FOUND;
	}

	// shouldn't happen
	if(inf->type==INSTANCE_PVP)
		return INSTANCE_ABORT_NOT_FOUND;

	//do we need addition raid checks?
	Group * pGroup = plr->GetGroup() ;
	if( !plr->triggerpass_cheat )
	{
		// players without groups cannot enter raid instances (no soloing them:P)
		if( pGroup == NULL && (inf->type == INSTANCE_RAID || inf->type == INSTANCE_MULTIMODE))
			return INSTANCE_ABORT_NOT_IN_RAID_GROUP;

		// check that heroic mode is available if the player has requested it.
		if(plr->iInstanceType && inf->type != INSTANCE_MULTIMODE)
			return INSTANCE_ABORT_HEROIC_MODE_NOT_AVAILABLE;
	}

	// if we are here, it means:
	// 1) we're a non-raid instance
	// 2) we're a raid instance, and the person is in a group.
	// so, first we have to check if they have an instance on this map already, if so, allow them to teleport to that.
	// next we check if there is a saved instance belonging to him.
	// otherwise, we can create them a new one.
	
	m_mapLock.Acquire();

	//find all instances for our map
	instancemap = m_instances[mapid];
	if(instancemap)
	{
		InstanceMap::iterator itr;
		//do we have a specific instance id we should enter (saved or active).
		//don't bother looking for saved instances; if he had one it's instanceid allready (as found by areatrigger).
		if(instanceid != 0)
		{
			itr = instancemap->find(instanceid);
			if(itr != instancemap->end()) 
			{
				in = itr->second;
				// If the map is active and has players
				if(in->m_mapMgr && in->m_mapMgr->HasPlayers() && !plr->triggerpass_cheat)
				{
					//check if combat is in progress
					if( in->m_mapMgr->IsCombatInProgress())
					{
						m_mapLock.Release();
						return INSTANCE_ABORT_ENCOUNTER;
					}

					// check if we are full
					if( in->m_mapMgr->GetPlayerCount() >= inf->playerlimit )
					{
						m_mapLock.Release();
						return INSTANCE_ABORT_FULL;
					}
				}

				//we have an instance,but can we enter it?
				uint8 owns =  PlayerOwnsInstance( in, plr );
				if( owns >= OWNER_CHECK_OK )
				{
					//wakeup call for saved instances
					if(owns == OWNER_CHECK_SAVED_OK && !in->m_mapMgr)
					{
						if(plr->GetGroup())
							in->m_creatorGroup = plr->GetGroupID();
						in->m_mapMgr = _CreateInstance(in);
					}
					m_mapLock.Release();
					return INSTANCE_OK;
				}
				else
					DEBUG_LOG("InstanceMgr","Check failed %s, return code %u",plr->GetName(), owns);
			}
			m_mapLock.Release();
			return INSTANCE_ABORT_NOT_FOUND;
		}
		else
		{
			// search all active instances and see if we have one here.
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;
				// check the player count and in combat status.
				if(in->m_mapMgr && in->m_mapMgr->HasPlayers() && !plr->triggerpass_cheat)
				{
					if( in->m_mapMgr->IsCombatInProgress())
					{
						m_mapLock.Release();
						return INSTANCE_ABORT_ENCOUNTER;
					}
					// check if we are full
					if( in->m_mapMgr->GetPlayerCount() >= inf->playerlimit )
					{
						m_mapLock.Release();
						return INSTANCE_ABORT_FULL;
					}
				}

				//we have an instance,but do we own it?
				uint8 owns = PlayerOwnsInstance(in, plr);
				if( owns >= OWNER_CHECK_OK )
				{
					// found our instance, allow him in.
					m_mapLock.Release();
					return INSTANCE_OK;
				}
				else
					DEBUG_LOG("InstanceMgr","Check failed %s, return code %u",plr->GetName(), owns);
			}
		}
	}
	else
	{
		if(instanceid != 0)
		{
			// wtf, how can we have an instance_id for a mapid which doesn't even exist?
			m_mapLock.Release();
			return INSTANCE_ABORT_NOT_FOUND;
		}
		// this mapid hasn't been added yet, so we gotta create the hashmap now.
		m_instances[mapid] = new InstanceMap;
		instancemap = m_instances[mapid];
	}

	// if we're here, it means we need to create a new instance.
	in = new Instance;
	in->m_creation = UNIXTIME;
	in->m_expiration = (inf->type == INSTANCE_NONRAID) ? 0 : UNIXTIME + inf->cooldown;		// expire time 0 is 10 minutes after last player leaves
	in->m_creatorGuid = plr->GetLowGUID();
	in->m_creatorGroup = pGroup ? pGroup->GetID() : 0;
	in->m_difficulty = plr->iInstanceType;
	in->m_instanceId = GenerateInstanceID();
	in->m_mapId = mapid;
	in->m_mapMgr = NULLMAPMGR;		// always start off without a map manager, it is created in _CreateInstance(in)

	//crash fix; GM's without group will start up raid instances as if they where nonraids
	//this to avoid exipring check, this is mainly for developers purpose; GM's should NOT invite any players here!
	if( plr->triggerpass_cheat && !plr->GetGroup() && inf->type == INSTANCE_RAID)
	{
		const char * message = "Started this instance for development purposes only, do not invite players!!";
		sEventMgr.AddEvent( plr, &Player::_Warn, message, EVENT_UNIT_SENDMESSAGE, 20000, 1, 0);
	}

	in->m_mapInfo = inf;
	in->m_isBattleground=false;
	plr->SetInstanceID(in->m_instanceId);
	DEBUG_LOG("InstanceMgr", "Prepared new instance %u for player %u and group %u on map %u. (%u)",in->m_instanceId, in->m_creatorGuid, in->m_creatorGroup, in->m_mapId, in->m_instanceId);


	// apply it in the instance map
	instancemap->insert( InstanceMap::value_type( in->m_instanceId, in ) );

	// create the actual instance (if we don't GetInstance() won't be able to access it).
	in->m_mapMgr = _CreateInstance(in);

	// instance created ok, i guess? return the ok for him to transport.
	m_mapLock.Release();
	return INSTANCE_OK;
}

MapMgrPointer InstanceMgr::GetMapMgr(uint32 mapId)
{
	return m_singleMaps[mapId];
}

MapMgrPointer InstanceMgr::GetInstance(ObjectPointer obj)
{
	PlayerPointer plr;
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(obj->GetMapId());

	// we can *never* teleport to maps without a mapinfo.
	if( inf == NULL || obj->GetMapId() >= NUM_MAPS )
		return NULLMAPMGR;

	if( obj->IsPlayer() )
	{
		// players can join instances based on their groups/solo status.
		plr = TO_PLAYER( obj );

		// single-instance maps never go into the instance set.
		if( inf->type == INSTANCE_NULL )
			return m_singleMaps[obj->GetMapId()];

		m_mapLock.Acquire();
		instancemap = m_instances[obj->GetMapId()];
		if(instancemap != NULL)
		{
			// check our instance_id, which we saved before in Player::_Relocate
			uint32 plr_instanceID = obj->GetInstanceID();
			itr = instancemap->find(plr_instanceID);
			if(itr != instancemap->end())
			{
				in = itr->second;
				if(in)
				{
					//we have an instance,but can we enter it?
					uint8 owns =  PlayerOwnsInstance( in, plr );
					if( owns >= OWNER_CHECK_OK )
					{
						//wakeup call for saved instances
						if(owns == OWNER_CHECK_SAVED_OK && !in->m_mapMgr)
						{	
							if(plr->GetGroup())
								in->m_creatorGroup = plr->GetGroupID();
							in->m_mapMgr = _CreateInstance(in);
						}
						m_mapLock.Release();
						return in->m_mapMgr;
					}
				}
			}

			
			// iterate over our instances, and see if any of them are owned/joinable by him.
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				// Is this our instance?
				uint8 owns = PlayerOwnsInstance(in, plr);
				if(owns >= OWNER_CHECK_OK )
				{
					//Has it been created yet?
					if(in->m_mapMgr == NULL)
					{
						// create the actual instance.
						in->m_mapMgr = _CreateInstance(in);
						m_mapLock.Release();
						return in->m_mapMgr;
					}
					else // instance has found and verfied; us it.
					{
						m_mapLock.Release();
						return in->m_mapMgr;
					}
				}
				else
					DEBUG_LOG("InstanceMgr","Check failed %s",plr->GetName());
			}
		}

		// if we're here, it means there are no instances on that map, or none of the instances on that map are joinable
		// by this player.
		m_mapLock.Release();
		return NULLMAPMGR;
	}
	else
	{
		// units are *always* limited to their set instance ids.
		if(inf->type == INSTANCE_NULL)
			return m_singleMaps[obj->GetMapId()];

		m_mapLock.Acquire();
		instancemap = m_instances[obj->GetMapId()];
		if(instancemap)
		{
			itr = instancemap->find(obj->GetInstanceID());
			if(itr != instancemap->end())
			{
				// we never create instances just for units.
				m_mapLock.Release();
				return itr->second->m_mapMgr;
			}
		}

		// instance is non-existant (shouldn't really happen for units...)
		m_mapLock.Release();
		return NULLMAPMGR;
	}
}

MapMgrPointer InstanceMgr::_CreateInstance(uint32 mapid, uint32 instanceid)
{
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(mapid);

	ASSERT(inf && inf->type == INSTANCE_NULL);
	ASSERT(mapid < NUM_MAPS && m_maps[mapid] != NULL);

	Log.Notice("InstanceMgr", "Creating continent %s.", m_maps[mapid]->GetName());

	MapMgrPointer ret(new MapMgr(m_maps[mapid], mapid, instanceid));
	ret->Init();

	// start its thread
	ThreadPool.ExecuteTask(ret.get());
    
	// assign pointer
	m_singleMaps[mapid] = ret;
	return ret;
}

MapMgrPointer InstanceMgr::_CreateInstance(Instance * in)
{
	Log.Notice("InstanceMgr", "Creating saved instance %u (%s)", in->m_instanceId, m_maps[in->m_mapId]->GetName());
	ASSERT(in->m_mapMgr==NULL);

	// we don't have to check for world map info here, since the instance wouldn't have been saved if it didn't have any.
	in->m_mapMgr = MapMgrPointer (new MapMgr(m_maps[in->m_mapId], in->m_mapId, in->m_instanceId));
	in->m_mapMgr->Init();
	in->m_mapMgr->pInstance = in;
	in->m_mapMgr->iInstanceMode = in->m_difficulty;
	in->m_mapMgr->InactiveMoveTime = 60+UNIXTIME;
	ThreadPool.ExecuteTask(in->m_mapMgr.get());
	return in->m_mapMgr;
}

Instance * InstanceMgr::GetSavedInstance(uint32 map_id, uint32 guid)
{
		InstanceMap::iterator itr;
		InstanceMap * instancemap;

		m_mapLock.Acquire();
		instancemap = m_instances[map_id];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				if(itr != instancemap->end())
				{
					itr->second->m_SavedLock.Acquire();
					if( itr->second->m_SavedPlayers.find(guid) != itr->second->m_SavedPlayers.end() )
					{
						itr->second->m_SavedLock.Release();
						m_mapLock.Release();
						return itr->second;
					}
					itr->second->m_SavedLock.Release();
					++itr;
				}
			}
		}
		m_mapLock.Release();
		return NULL;

}

void InstanceMgr::_CreateMap(uint32 mapid)
{
	if( mapid >= NUM_MAPS )
		return;

	MapInfo * inf;

	inf = WorldMapInfoStorage.LookupEntry(mapid);
	if(inf==NULL)
		return;
	if(m_maps[mapid]!=NULL)
		return;

	m_maps[mapid] = new Map(mapid, inf);
	if(inf->type == INSTANCE_NULL)
	{
		// we're a continent, create the instance.
		_CreateInstance(mapid, GenerateInstanceID());
	}
}

uint32 InstanceMgr::GenerateInstanceID()
{
	uint32 iid;
	m_mapLock.Acquire();
	iid = m_InstanceHigh++;
	m_mapLock.Release();
	return iid;
}

void BuildStats(MapMgrPointer mgr, char * m_file, Instance * inst, MapInfo * inf)
{
	char tmp[200];
	strcpy(tmp, "");
#define pushline strcat(m_file, tmp)

	snprintf(tmp, 200, "	<instance>\n");																												pushline;
	snprintf(tmp, 200, "		<map>%u</map>\n", mgr->GetMapId());																						pushline;
	snprintf(tmp, 200, "		<maptype>%u</maptype>\n", inf->type);																						pushline;
	snprintf(tmp, 200, "		<players>%u</players>\n", (unsigned int)mgr->GetPlayerCount());																			pushline;
	snprintf(tmp, 200, "		<maxplayers>%u</maxplayers>\n", inf->playerlimit);																		pushline;

	//<creationtime>
	if (inst)
	{
		tm *ttime = localtime( &inst->m_creation );
		snprintf(tmp, 200, "		<creationtime>%02u:%02u:%02u %02u/%02u/%u</creationtime>\n",ttime->tm_hour, ttime->tm_min, ttime->tm_sec, ttime->tm_mday, ttime->tm_mon, uint32( ttime->tm_year + 1900 ));
		pushline;
	}
	else
	{
		snprintf(tmp, 200, "		<creationtime>N/A</creationtime>\n");
		pushline;
	}

	//<expirytime>
	if (inst && inst->m_expiration)
	{
		tm *ttime = localtime( &inst->m_expiration );
		snprintf(tmp, 200, "		<expirytime>%02u:%02u:%02u %02u/%02u/%u</expirytime>\n",ttime->tm_hour, ttime->tm_min, ttime->tm_sec, ttime->tm_mday, ttime->tm_mon, uint32( ttime->tm_year + 1900 ));
		pushline;
	}
	else
	{
		snprintf(tmp, 200, "		<expirytime>N/A</expirytime>\n");
		pushline;

	}
	//<idletime>
	if (mgr->InactiveMoveTime)
	{
		tm *ttime = localtime( &mgr->InactiveMoveTime );
		snprintf(tmp, 200, "		<idletime>%02u:%02u:%02u %02u/%02u/%u</idletime>\n",ttime->tm_hour, ttime->tm_min, ttime->tm_sec, ttime->tm_mday, ttime->tm_mon, uint32( ttime->tm_year + 1900 ));
		pushline;
	}
	else
	{
		snprintf(tmp, 200, "		<idletime>N/A</idletime>\n");
		pushline;
	}

	snprintf(tmp, 200, "	</instance>\n");																											pushline;
#undef pushline
}

void InstanceMgr::BuildXMLStats(char * m_file)
{
	uint32 i;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	Instance * in;
	
	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_singleMaps[i] != NULL)
			BuildStats(m_singleMaps[i], m_file, NULL, m_singleMaps[i]->GetMapInfo());
		else
		{
			instancemap = m_instances[i];
			if(instancemap != NULL)
			{
				for(itr = instancemap->begin(); itr != instancemap->end();)
				{
					in = itr->second;
					++itr;

					if(in->m_mapMgr==NULL)
						continue;

					BuildStats(in->m_mapMgr, m_file, in, in->m_mapInfo);
				}
			}
		}
	}
	m_mapLock.Release();
}

void InstanceMgr::_LoadInstances()
{
	MapInfo * inf;
	Instance * in;
	QueryResult * result;

	// clear any instances that have expired.
	Log.Notice("InstanceMgr", "Deleting Expired Instances...");
	CharacterDatabase.WaitExecute("DELETE FROM instances WHERE expiration <= %u", UNIXTIME);
	
	// load saved instances
	result = CharacterDatabase.Query("SELECT * FROM instances");
	Log.Notice("InstanceMgr", "Loading %u saved instances." , result ? result->GetRowCount() : 0);

	if(result)
	{
		do 
		{
			inf = WorldMapInfoStorage.LookupEntry(result->Fetch()[1].GetUInt32());
			if(inf == NULL || result->Fetch()[1].GetUInt32() >= NUM_MAPS)
			{
				CharacterDatabase.Execute("DELETE FROM instances WHERE mapid = %u", result->Fetch()[1].GetUInt32());
				continue;
			}

			in = new Instance();
			in->m_mapInfo = inf;
			in->LoadFromDB(result->Fetch());

			if(m_instances[in->m_mapId] == NULL)
				m_instances[in->m_mapId] = new InstanceMap;

			m_instances[in->m_mapId]->insert( InstanceMap::value_type( in->m_instanceId, in ) );

		} while(result->NextRow());
		DEBUG_LOG("InstanceMgr", "Loading %u saved instances." , result->GetRowCount());
		delete result;
	}
	else
		DEBUG_LOG("InstanceMgr", "No saved instances found.");
}

void Instance::LoadFromDB(Field * fields)
{
	m_instanceId = fields[0].GetUInt32();
	m_mapId = fields[1].GetUInt32();
	m_creation = fields[2].GetUInt32();
	m_expiration = fields[3].GetUInt32();
	m_difficulty = fields[5].GetUInt32();
	m_creatorGroup = fields[6].GetUInt32();
	m_creatorGuid = fields[7].GetUInt32();
	m_mapMgr=NULLMAPMGR;
	m_isBattleground = false;

	// process saved npc's
	char * pnpcstr, *npcstr;
	char * m_npcstring = strdup(fields[4].GetString());

	npcstr = m_npcstring;
	pnpcstr = strchr(m_npcstring, ' ');
	while(pnpcstr)
	{
		*pnpcstr = 0;
		uint32 val = atol(npcstr);
		if (val)
			m_killedNpcs.insert( val );
		npcstr = pnpcstr+1;
		pnpcstr = strchr(npcstr, ' ');
	}
	free(m_npcstring);

	// process saved players
	char * pplayerstr, *playerstr;
	char * m_playerstring = strdup(fields[8].GetString());
	playerstr = m_playerstring;
	pplayerstr = strchr(m_playerstring, ' ');
	while(pplayerstr)
	{
		*pplayerstr = 0;
		uint32 val = atol(playerstr);
		if (val)
			m_SavedPlayers.insert( val );
		playerstr = pplayerstr+1;
		pplayerstr = strchr(playerstr, ' ');
	}
	free(m_playerstring);
}

void InstanceMgr::ResetSavedInstances(PlayerPointer plr)
{
	WorldPacket data(SMSG_INSTANCE_RESET, 4);
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	if(!plr->IsInWorld() || plr->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
		return;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_instances[i] != NULL)
		{
			instancemap = m_instances[i];
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				if  ( in->m_mapInfo->type == INSTANCE_NONRAID && 
					( plr->GetLowGUID() == in->m_creatorGuid || 
					( plr->GetGroup() && plr->GetGroupID() == in->m_creatorGroup )))
				{
					if(in->m_mapMgr && in->m_mapMgr->HasPlayers())
					{
						plr->GetSession()->SystemMessage("Can't reset instance %u (%s) when there are still players inside!", in->m_instanceId, in->m_mapMgr->GetMapInfo()->name);
						continue;
					}

					// <mapid> has been reset.
					data << uint32(in->m_mapId);
					plr->GetSession()->SendPacket(&data);

					// destroy the instance
					_DeleteInstance(in, true);
				}
			}
		}
	}
    m_mapLock.Release();	
}

void InstanceMgr::OnGroupDestruction(Group * pGroup)
{
	// this means a group has been deleted, so lets clear out all instances that they owned.
	// (instances don't transfer to the group leader, or anything)
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		instancemap = m_instances[i];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;
			}
		}
	}
	m_mapLock.Release();
}

bool InstanceMgr::_DeleteInstance(Instance * in, bool ForcePlayersOut)
{
	m_mapLock.Acquire();
	InstanceMap * instancemap;
	InstanceMap::iterator itr;

	if(in->m_mapMgr)
	{

		// "ForcePlayersOut" will teleport the players in this instance to their entry point/hearthstone.
		// otherwise, they will get a 60 second timeout telling them they are not in this instance's group.
		if(in->m_mapMgr->HasPlayers())
		{
			if(ForcePlayersOut)
				in->m_mapMgr->InstanceShutdown();
			else
			{
				in->m_mapMgr->BeginInstanceExpireCountdown();
				in->m_mapMgr->pInstance = NULL;
			}
		}
		else
			in->m_mapMgr->InstanceShutdown();
	}


	// remove the instance from the large map.
	instancemap = m_instances[in->m_mapId];
	if(instancemap)
	{
		itr = instancemap->find(in->m_instanceId);
		if(itr != instancemap->end())
			instancemap->erase(itr);
	}

	// cleanup corpses, database references
	in->DeleteFromDB();

	// delete the instance pointer.
	delete in;
	m_mapLock.Release();
	
	return true;
}

void Instance::DeleteFromDB()
{
	// delete from the database
	CharacterDatabase.Execute("DELETE FROM instances WHERE id = %u", m_instanceId);

	// cleanup all the corpses
	CharacterDatabase.Execute("DELETE FROM T1 USING corpses T1 LEFT JOIN instances As T2 ON T1.instanceid = T2.id WHERE  T2.id IS NULL");
}

void InstanceMgr::CheckForExpiredInstances()
{
	// checking for any expired instances.
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		instancemap = m_instances[i];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				// use a "soft" delete here.
				if(in->m_mapInfo->type != INSTANCE_NONRAID && HasInstanceExpired(in))
					_DeleteInstance(in, false);
			}

		}
	}
	m_mapLock.Release();
}

void InstanceMgr::BuildSavedInstancesForPlayer(PlayerPointer plr)
{
	WorldPacket data(4);
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	if(!plr->IsInWorld() || plr->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
	{
		m_mapLock.Acquire();
		for(i = 0; i < NUM_MAPS; ++i)
		{
			if(m_instances[i] != NULL)
			{
				instancemap = m_instances[i];
				for(itr = instancemap->begin(); itr != instancemap->end();)
				{
					in = itr->second;
					++itr;

					if( in->m_mapInfo->type == INSTANCE_NONRAID && PlayerOwnsInstance(in, plr))
					{
						m_mapLock.Release();

						data.SetOpcode(SMSG_UPDATE_LAST_INSTANCE);
						data << uint32(in->m_mapId);
						plr->GetSession()->SendPacket(&data);

						data.Initialize(SMSG_UPDATE_INSTANCE_OWNERSHIP);
						data << uint32(0x01);
						plr->GetSession()->SendPacket(&data);

						break; //next mapid
					}
				}
			}
		}
		m_mapLock.Release();
	}

	data.SetOpcode(SMSG_UPDATE_INSTANCE_OWNERSHIP);
	data << uint32(0x00);
	plr->GetSession()->SendPacket(&data);
}

void InstanceMgr::BuildSavedRaidInstancesForPlayer(PlayerPointer plr)
{
	WorldPacket data(SMSG_RAID_INSTANCE_INFO, 200);
	Instance * in;
	InstanceMap::iterator itr;
	uint32 i;
	uint32 counter = 0;

	data << counter;
	for(i = 0; i < NUM_MAPS; ++i)
	{
		in = GetSavedInstance(i, plr->GetLowGUID());
		if(in && in->m_mapInfo->type == INSTANCE_RAID)
		{
			data << in->m_mapId;
			data << uint32(in->m_expiration - UNIXTIME);
			data << in->m_instanceId;
			data << in->m_difficulty;
			++counter;
		}
	}

	*(uint32*)&data.contents()[0] = counter;
	plr->GetSession()->SendPacket(&data);
}

void Instance::SaveToDB()
{
	// don't save non-raid instances.
	if(m_mapInfo->type == INSTANCE_NONRAID || m_isBattleground)
		return;

	// don't save instance if nothing is killed yet
	if (m_killedNpcs.size()==0)
		return;

	// Add new players to existing m_SavedPlayers
	m_SavedLock.Acquire();
	PlayerStorageMap::iterator itr1 = m_mapMgr->m_PlayerStorage.begin();
	unordered_set<uint32>::iterator itr2, itr3;
	for(; itr1 != m_mapMgr->m_PlayerStorage.end(); itr1++)
	{
		itr2 = m_SavedPlayers.find(itr1->second->GetLowGUID());
		if( itr2 == m_SavedPlayers.end() )
			m_SavedPlayers.insert(itr1->second->GetLowGUID());
	}
	m_SavedLock.Release();

	std::stringstream ss;

	ss << "REPLACE INTO instances VALUES("
		<< m_instanceId << ","
		<< m_mapId << ","
		<< (uint32)m_creation << ","
		<< (uint32)m_expiration << ",'";

	for(itr3 = m_killedNpcs.begin(); itr3 != m_killedNpcs.end(); ++itr3)
		ss << (*itr3) << " ";

	ss	<< "',"
		<< m_difficulty << ","
		<< m_creatorGroup << ","
		<< m_creatorGuid << ",'";

	m_SavedLock.Acquire();
	for(itr2 = m_SavedPlayers.begin(); itr2 != m_SavedPlayers.end(); ++itr2)
		ss << (*itr2) << " ";
	m_SavedLock.Release();

	ss <<"')";

	CharacterDatabase.Execute(ss.str().c_str());
}

void InstanceMgr::PlayerLeftGroup(Group * pGroup, PlayerPointer pPlayer)
{
	// does this group own any instances? we have to kick the player out of those instances.
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	WorldPacket data(SMSG_RAID_GROUP_ONLY, 8);
	uint32 i;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		instancemap = m_instances[i];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				if(PlayerOwnsInstance(in,pPlayer))
				{
					// better make sure we're actually in that instance.. :P
					if(!pPlayer->raidgrouponlysent && pPlayer->GetInstanceID() == (int32)in->m_instanceId)
					{
						data << uint32(60000) << uint32(1);
						pPlayer->GetSession()->SendPacket(&data);
						pPlayer->raidgrouponlysent=true;
	
						sEventMgr.AddEvent(pPlayer, &Player::EjectFromInstance, EVENT_PLAYER_EJECT_FROM_INSTANCE, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

						m_mapLock.Release();
						return;
					}
				}
			}
		}
	}
	m_mapLock.Release();
}

MapMgrPointer InstanceMgr::CreateBattlegroundInstance(uint32 mapid)
{
	// shouldn't happen
	if( mapid >= NUM_MAPS )
		return NULLMAPMGR;

	if(!m_maps[mapid])
	{
		_CreateMap(mapid);
		if(!m_maps[mapid])
			return NULLMAPMGR;
	}

	MapMgrPointer ret = MapMgrPointer (new MapMgr(m_maps[mapid],mapid,GenerateInstanceID()));
	ret->Init();
	Instance * pInstance = new Instance();
	pInstance->m_creation = UNIXTIME;
	pInstance->m_creatorGroup = 0;
	pInstance->m_creatorGuid = 0;
	pInstance->m_difficulty = 0;
	pInstance->m_expiration = 0;
	pInstance->m_instanceId = ret->GetInstanceID();
	pInstance->m_isBattleground = true;
	pInstance->m_mapId = mapid;
	pInstance->m_mapInfo = WorldMapInfoStorage.LookupEntry( mapid );
	pInstance->m_mapMgr = ret;
	m_mapLock.Acquire();
	if( m_instances[mapid] == NULL )
		m_instances[mapid] = new InstanceMap;

	m_instances[mapid]->insert( make_pair( pInstance->m_instanceId, pInstance ) );
	m_mapLock.Release();
	ThreadPool.ExecuteTask(ret.get());
	return ret;
}

void InstanceMgr::DeleteBattlegroundInstance(uint32 mapid, uint32 instanceid)
{
	m_mapLock.Acquire();
	InstanceMap::iterator itr = m_instances[mapid]->find( instanceid );
	if( itr == m_instances[mapid]->end() )
	{
		printf("Could not delete battleground instance!\n");
		m_mapLock.Release();
		return;
	}

	m_instances[mapid]->erase( itr );
	m_mapLock.Release();
}

FormationMgr::FormationMgr()
{
	QueryResult * res = WorldDatabase.Query("SELECT * FROM creature_formations");
	if(res)
	{
		Formation *f ;
		do 
		{
			f = new Formation;
			f->fol = res->Fetch()[1].GetUInt32();
			f->ang = res->Fetch()[2].GetFloat();
			f->dist = res->Fetch()[3].GetFloat();
			m_formations[res->Fetch()[0].GetUInt32()] = f;
		} while(res->NextRow());
		delete res;
	}
}

FormationMgr::~FormationMgr()
{
	FormationMap::iterator itr;
	for(itr = m_formations.begin(); itr != m_formations.end(); ++itr)
		delete itr->second;
}
