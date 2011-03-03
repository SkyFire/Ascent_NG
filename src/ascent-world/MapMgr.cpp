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
#define MAP_MGR_UPDATE_PERIOD 100
#define MAPMGR_INACTIVE_MOVE_TIME 10
extern bool bServerShutdown;

#ifdef WIN32
#pragma warning(disable:4355)
#endif

MapMgr::MapMgr(Map *map, uint32 mapId, uint32 instanceid) : CellHandler<MapCell>(map), _mapId(mapId), eventHolder(instanceid)
{
	m_instanceID = instanceid;
	pMapInfo = WorldMapInfoStorage.LookupEntry(mapId);
	m_UpdateDistance = pMapInfo->update_distance * pMapInfo->update_distance;
	iInstanceMode = 0;

	// Set up storage arrays
	m_CreatureArraySize = map->CreatureSpawnCount;
	m_VehicleArraySize = 0;

	//m_CreatureStorage = new Creature*[m_CreatureArraySize];
	//m_CreatureStorage = (Creature**)malloc(sizeof(Creature*) * m_CreatureArraySize);
	//memset(m_CreatureStorage,0,sizeof(Creature*)*m_CreatureArraySize);

	m_GOHighGuid = 0;
	m_CreatureHighGuid = 0;
	m_VehicleHighGuid = 0;
	m_DynamicObjectHighGuid=0; 
	lastUnitUpdate = getMSTime();
	lastGameobjectUpdate = getMSTime();
	m_battleground = NULL;

	m_holder = &eventHolder;
	m_event_Instanceid = eventHolder.GetInstanceID();
	forced_expire = false;
	InactiveMoveTime = 0;
	mLoopCounter=0;
	pInstance = NULL;
	thread_kill_only = false;
	thread_running = false;

	// buffers
	m_updateBuffer.reserve(50000);
	m_createBuffer.reserve(20000);
	m_updateBuildBuffer.reserve(65536);

	// uint32 destsize = size + size/10 + 16;
	m_compressionBuffer.reserve(75000);

	m_PlayerStorage.clear();
	m_PetStorage.clear();
	m_DynamicObjectStorage.clear();

	_combatProgress.clear();
	_mapWideStaticObjects.clear();
	_updates.clear();
	_processQueue.clear();
	MapSessions.clear();

	activeGameObjects.clear();
	activeCreatures.clear();
	activeVehicles.clear();
	m_corpses.clear();
	_sqlids_vehicles.clear();
	_sqlids_creatures.clear();
	_sqlids_gameobjects.clear();
	_reusable_guids_creature.clear();
	_reusable_guids_vehicle.clear();

	if (sWorld.Collision && GetMapInfo()->collision)
		SetCollision(true);
	else
		SetCollision(false);

	mInstanceScript = NULL;
}

void MapMgr::Init()
{
	m_stateManager = new WorldStateManager(this);
	// Create script interface
	ScriptInterface = new MapScriptInterface( this );
}

// call me to break the circular reference, perform cleanup
void MapMgr::Destructor()
{
	delete this;
}

MapMgr::~MapMgr()
{
	SetThreadName("thread_proc");//free the name

	sEventMgr.RemoveEvents(this);
	sEventMgr.RemoveEventHolder(m_instanceID);

	delete ScriptInterface;
	delete m_stateManager;

	if ( mInstanceScript != NULL )
		mInstanceScript->Destroy();

	// Remove objects
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
						_cells[i][j]->RemoveObjects();
					}
				}
			}
		}
	}

	Object* pObject;
	for(set<Object* >::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
	{
		pObject = *itr;
		if(!pObject)
			continue;

		if(pObject->IsInWorld())
			pObject->RemoveFromWorld(false);
		
		switch(pObject->GetTypeFromGUID())
		{
		case HIGHGUID_TYPE_VEHICLE:
			TO_VEHICLE(pObject)->Destructor();
			break;
		case HIGHGUID_TYPE_UNIT:
			TO_CREATURE(pObject)->Destructor();
			break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			TO_GAMEOBJECT(pObject)->Destructor();
			break;
		default:
			pObject->Destructor();

		}
	}
	_mapWideStaticObjects.clear();

	Corpse* pCorpse;
	for(unordered_set<Corpse* >::iterator itr = m_corpses.begin(); itr != m_corpses.end(); ++itr)
	{
		pCorpse = *itr;
		if(!pCorpse)
			continue;

		if(pCorpse->IsInWorld())
			pCorpse->RemoveFromWorld(false);

		pCorpse->Destructor();
	}
	m_corpses.clear();

	//Clear our remaining containers
	m_PlayerStorage.clear();
	m_PetStorage.clear();
	m_DynamicObjectStorage.clear();
	m_CreatureStorage.clear();
	m_gameObjectStorage.clear();

	_combatProgress.clear();
	_updates.clear();
	_processQueue.clear();
	MapSessions.clear();
	pMapInfo = NULL;

	activeGameObjects.clear();
	activeCreatures.clear();
	activeVehicles.clear();
	_sqlids_vehicles.clear();
	_sqlids_creatures.clear();
	_sqlids_gameobjects.clear();
	_reusable_guids_creature.clear();
	_reusable_guids_vehicle.clear();

	m_battleground = NULL;

	Log.Notice("MapMgr", "Instance %u shut down. (%s)" , m_instanceID, GetBaseMap()->GetName());
}

void MapMgr::PushObject(Object* obj)
{
	/////////////
	// Assertions
	/////////////
	ASSERT(obj);
	
	// That object types are not map objects. TODO: add AI groups here?
	if(obj->GetTypeId() == TYPEID_ITEM || obj->GetTypeId() == TYPEID_CONTAINER)
	{
		// mark object as updatable and exit
		return;
	}

	if(obj->GetTypeId() == TYPEID_CORPSE)
	{
		m_corpses.insert( TO_CORPSE(obj) );
	}	
	
	Player* plObj = NULL;

	if(obj->GetTypeId() == TYPEID_PLAYER)
	{
		plObj = TO_PLAYER( obj );
		if(plObj == NULL)
		{
			DEBUG_LOG("MapMgr","Could not get a valid playerobject from object while trying to push to world");
			return;
		}
		WorldSession * plSession = NULL;
		plSession = plObj->GetSession();
		if(plSession == NULL)
		{
			DEBUG_LOG("MapMgr","Could not get a valid session for player while trying to push to world");
			return;
		}
	}
	
	obj->ClearInRangeSet();

	///////////////////////
	// Get cell coordinates
	///////////////////////

	ASSERT(obj->GetMapId() == _mapId);

	ASSERT(obj->GetPositionY() < _maxY && obj->GetPositionY() > _minY);
	ASSERT(_cells);

	float mx = obj->GetPositionX();
	float my = obj->GetPositionY();
	uint32 cx = GetPosX(mx);
	uint32 cy = GetPosY(my);

	if(	mx > _maxX || my > _maxY ||
		mx < _minX || my < _minY ||
		cx >= _sizeX || cy >= _sizeY)
	{
		if( plObj != NULL )
		{
			if(plObj->GetBindMapId() != GetMapId())
			{
				plObj->SafeTeleport(plObj->GetBindMapId(),0,plObj->GetBindPositionX(),plObj->GetBindPositionY(),plObj->GetBindPositionZ(),0);
				plObj->GetSession()->SystemMessage("Teleported you to your hearthstone location as you ended up on the wrong map.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plObj->GetBindPositionX(),plObj->GetBindPositionY(),plObj->GetBindPositionZ(),0);
				plObj->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				WorldPacket * data = plObj->BuildTeleportAckMsg(plObj->GetPosition());
				plObj->GetSession()->SendPacket(data);
				delete data;
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0,0,0,0);
		}

		cx = GetPosX(obj->GetPositionX());
		cy = GetPosY(obj->GetPositionY());
	}

	MapCell *objCell = GetCell(cx,cy);
	if (!objCell)
	{
		objCell = Create(cx,cy);
		objCell->Init(cx, cy, _mapId, this);
	}

	uint32 endX = (cx <= _sizeX) ? cx + 1 : (_sizeX-1);
	uint32 endY = (cy <= _sizeY) ? cy + 1 : (_sizeY-1);
	uint32 startX = cx > 0 ? cx - 1 : 0;
	uint32 startY = cy > 0 ? cy - 1 : 0;
	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter;

	uint32 count;

	if(plObj)
	{
		DEBUG_LOG("MapMgr","Creating player "I64FMT" for himself.", obj->GetGUID());
		count = plObj->BuildCreateUpdateBlockForPlayer(&m_createBuffer, plObj);
		plObj->PushCreationData(&m_createBuffer, count);
		m_createBuffer.clear();
	}

	//////////////////////
	// Build in-range data
	//////////////////////

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			cell = GetCell(posX, posY);
			if (cell)
			{
				UpdateInRangeSet(obj, plObj, cell);
			}
		}
	}

	//Add to the cell's object list
	objCell->AddObject(obj);

	obj->SetMapCell(objCell);
	 //Add to the mapmanager's object list
	if(plObj)
	{
		m_PlayerStorage[plObj->GetLowGUID()] = plObj;
		UpdateCellActivity(cx, cy, 2);
	}
	else
	{
		switch(obj->GetTypeFromGUID())
		{
		case HIGHGUID_TYPE_PET:
			m_PetStorage[obj->GetUIdFromGUID()] = TO_PET( obj );
			break;

		case HIGHGUID_TYPE_UNIT:
			{
				ASSERT((obj->GetUIdFromGUID()) <= m_CreatureHighGuid);
				m_CreatureStorage[obj->GetUIdFromGUID()] = TO_CREATURE(obj);
				if(TO_CREATURE(obj)->m_spawn != NULL)
				{
					_sqlids_creatures.insert(make_pair( TO_CREATURE(obj)-> GetSQL_id(), TO_CREATURE(obj) ) );
				}
			}break;

		case HIGHGUID_TYPE_VEHICLE:
			{
				ASSERT((obj->GetUIdFromGUID()) <= m_VehicleHighGuid);
				m_VehicleStorage[obj->GetUIdFromGUID()] = TO_VEHICLE(obj);
				if(TO_VEHICLE(obj)->m_spawn != NULL)
				{
					_sqlids_vehicles.insert(make_pair( TO_VEHICLE(obj)->m_spawn->id, TO_VEHICLE(obj) ) );
				}
			}break;
		case HIGHGUID_TYPE_GAMEOBJECT:
			{
				m_gameObjectStorage.insert(make_pair(obj->GetUIdFromGUID(), TO_GAMEOBJECT(obj)));
				if( TO_GAMEOBJECT(obj)->m_spawn != NULL)
				{
					_sqlids_gameobjects.insert(make_pair( TO_GAMEOBJECT(obj)->m_spawn->id, TO_GAMEOBJECT(obj) ) );
				}
			}break;

		case HIGHGUID_TYPE_DYNAMICOBJECT:
			m_DynamicObjectStorage[obj->GetLowGUID()] = TO_DYNAMICOBJECT(obj);
			break;
		}
	}

	// Handle activation of that object.
	if(objCell->IsActive() && obj->CanActivate())
		obj->Activate(this);

	// Add the session to our set if it is a player.
	if(plObj)
	{
		MapSessions.insert(plObj->GetSession());

		// Change the instance ID, this will cause it to be removed from the world thread (return value 1)
		plObj->GetSession()->SetInstance(GetInstanceID());

		/* Add the map wide objects */
		if(_mapWideStaticObjects.size())
		{
			for(set<Object* >::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
			{
				count = (*itr)->BuildCreateUpdateBlockForPlayer(&m_createBuffer, plObj);
				plObj->PushCreationData(&m_createBuffer, count);
				m_createBuffer.clear();
			}
		}
	}

	if(plObj && InactiveMoveTime && !forced_expire)
		InactiveMoveTime = 0;
}


void MapMgr::PushStaticObject(Object* obj)
{
	_mapWideStaticObjects.insert(obj);

	switch(obj->GetTypeFromGUID())
	{
		case HIGHGUID_TYPE_VEHICLE:
			m_VehicleStorage[obj->GetUIdFromGUID()] = TO_VEHICLE(obj);
			break;

		case HIGHGUID_TYPE_UNIT:
			m_CreatureStorage[obj->GetUIdFromGUID()] = TO_CREATURE(obj);
			break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			m_gameObjectStorage.insert(make_pair(obj->GetUIdFromGUID(), TO_GAMEOBJECT(obj)));
			break;

		default:
			// maybe add a warning, shouldnt be needed
			break;
	}
}

void MapMgr::RemoveObject(Object* obj, bool free_guid)
{
	/////////////
	// Assertions
	/////////////

	ASSERT(obj);
	ASSERT(obj->GetMapId() == _mapId);
	//ASSERT(obj->GetPositionX() > _minX && obj->GetPositionX() < _maxX);
	//ASSERT(obj->GetPositionY() > _minY && obj->GetPositionY() < _maxY);
	ASSERT(_cells);

	_updates.erase(obj);
	obj->ClearUpdateMask();
	Player* plObj = (obj->GetTypeId() == TYPEID_PLAYER) ? TO_PLAYER( obj ) : NULL;

	///////////////////////////////////////
	// Remove object from all needed places
	///////////////////////////////////////
 
	switch(obj->GetTypeFromGUID())
	{
	case HIGHGUID_TYPE_VEHICLE:
		{
			ASSERT(obj->GetUIdFromGUID() <= m_VehicleHighGuid);
			if(TO_VEHICLE(obj)->m_spawn != NULL)
				_sqlids_vehicles.erase(TO_VEHICLE(obj)->m_spawn->id);
			if(free_guid)
				_reusable_guids_vehicle.push_back(obj->GetUIdFromGUID());
			m_CreatureStorage.erase(obj->GetUIdFromGUID());
		}break;

	case HIGHGUID_TYPE_UNIT:
		{
			ASSERT(obj->GetUIdFromGUID() <= m_CreatureHighGuid);
			if(TO_CREATURE(obj)->m_spawn != NULL)
				_sqlids_creatures.erase(TO_CREATURE(obj)->GetSQL_id());
			if(free_guid)
				_reusable_guids_creature.push_back(obj->GetUIdFromGUID());
			m_CreatureStorage.erase(obj->GetUIdFromGUID());
		}break;

	case HIGHGUID_TYPE_PET:
		{
			// check iterator
			if( __pet_iterator != m_PetStorage.end() && __pet_iterator->second == TO_PET(obj) )
				++__pet_iterator;
			m_PetStorage.erase(obj->GetUIdFromGUID());
		}break;

	case HIGHGUID_TYPE_DYNAMICOBJECT:
		{
			m_DynamicObjectStorage.erase(obj->GetLowGUID());
		}break;

	case HIGHGUID_TYPE_GAMEOBJECT:
		{
			m_gameObjectStorage.erase(obj->GetUIdFromGUID());
			if(TO_GAMEOBJECT(obj)->m_spawn != NULL)
				_sqlids_gameobjects.erase(TO_GAMEOBJECT(obj)->m_spawn->id);
		}break;

	case HIGHGUID_TYPE_PLAYER:
		{
			// check iterator
			if( __player_iterator != m_PlayerStorage.end() && __player_iterator->second == TO_PLAYER(obj) )
				++__player_iterator;
		}break;
	}

	if(obj->Active)
		obj->Deactivate(this);

	// That object types are not map objects. TODO: add AI groups here?
	if(obj->GetTypeId() == TYPEID_ITEM || obj->GetTypeId() == TYPEID_CONTAINER || obj->GetTypeId()==TYPEID_UNUSED)
	{
		return;
	}

	if(obj->GetTypeId() == TYPEID_CORPSE)
	{
		m_corpses.erase( TO_CORPSE(obj) );
	}

	if(!obj->GetMapCell())
	{
		/* set the map cell correctly */
		if(obj->GetPositionX() >= _maxX || obj->GetPositionX() <= _minY ||
			obj->GetPositionY() >= _maxY || obj->GetPositionY() <= _minY)
		{
			// do nothing
		}
		else
		{
			obj->SetMapCell(this->GetCellByCoords(obj->GetPositionX(), obj->GetPositionY()));
		}		
	}

	if(obj->GetMapCell())
	{
		ASSERT(obj->GetMapCell());

		// Remove object from cell
		obj->GetMapCell()->RemoveObject(obj);

		// Unset object's cell
		obj->SetMapCell(NULL);
	}

	// Clear any updates pending
	if(obj->GetTypeId() == TYPEID_PLAYER)
	{
		_processQueue.erase(TO_PLAYER(obj));
		TO_PLAYER( obj )->ClearAllPendingUpdates();
	}

	// Remove object from all objects 'seeing' him
	for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin();
		iter != obj->GetInRangeSetEnd(); ++iter)
	{
		if( (*iter) )
		{
			if( (*iter)->GetTypeId() == TYPEID_PLAYER )
			{
				if( TO_PLAYER( *iter )->IsVisible( obj ) && TO_PLAYER( *iter )->m_TransporterGUID != obj->GetGUID())
					TO_PLAYER( *iter )->PushOutOfRange(obj->GetNewGUID());
			}
			(*iter)->RemoveInRangeObject(obj);
		}
	}

	// Clear object's in-range set
	obj->ClearInRangeSet();

	if(plObj)
	{
		// If it's a player and he's inside boundaries - update his nearby cells
		if(obj->GetPositionX() <= _maxX && obj->GetPositionX() >= _minX &&
			obj->GetPositionY() <= _maxY && obj->GetPositionY() >= _minY)
		{
			uint32 x = GetPosX(obj->GetPositionX());
			uint32 y = GetPosY(obj->GetPositionY());
			UpdateCellActivity(x, y, 2);
		}
		m_PlayerStorage.erase( TO_PLAYER( obj )->GetLowGUID() );

		// Remove the session from our set if it is a player.
		for(set<Object* >::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
		{
			plObj->PushOutOfRange((*itr)->GetNewGUID());
		}

		// Setting an instance ID here will trigger the session to be removed
		// by MapMgr::run(). :)
		plObj->GetSession()->SetInstance(0);

		// Add it to the global session set (if it's not being deleted).
		if(!plObj->GetSession()->bDeleted)
			sWorld.AddGlobalSession(plObj->GetSession());
	}

	if(!HasPlayers() && !InactiveMoveTime && !forced_expire && GetMapInfo()->type != INSTANCE_NULL)
	{
		InactiveMoveTime = UNIXTIME + (MAPMGR_INACTIVE_MOVE_TIME * 60);	   // 5 mins -> move to inactive
	}
}

void MapMgr::ChangeObjectLocation( Object* obj )
{
	// Items and containers are of no interest for us
	if( obj->GetTypeId() == TYPEID_ITEM || obj->GetTypeId() == TYPEID_CONTAINER || obj->GetMapMgr() != this )
	{
		return;
	}

	Player* plObj;

	if( obj->GetTypeId() == TYPEID_PLAYER )
	{
		plObj = TO_PLAYER( obj );
	}
	else
	{
		plObj = NULL;
	}

	Object* curObj = NULL;
	float fRange;

	///////////////////////////////////////
	// Update in-range data for old objects
	///////////////////////////////////////

	if(obj->HasInRangeObjects()) {
		for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin(), iter2;
			iter != obj->GetInRangeSetEnd();)
		{
			curObj = *iter;
			iter2 = iter++;
			if( curObj->IsPlayer() && obj->IsPlayer() && plObj->m_TransporterGUID && plObj->m_TransporterGUID == TO_PLAYER( curObj )->m_TransporterGUID )
				fRange = 0.0f; // unlimited distance for people on same boat
			else if( curObj->IsPlayer() && obj->IsPlayer() && plObj->m_CurrentVehicle && plObj->m_CurrentVehicle == TO_PLAYER( curObj )->m_CurrentVehicle )
				fRange = 0.0f; // unlimited distance for people on same vehicle
			else if( curObj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER || curObj->GetTypeFromGUID() ==  HIGHGUID_TYPE_VEHICLE)
				fRange = 0.0f; // unlimited distance for transporters (only up to 2 cells +/- anyway.)
//TODO Implement distance by protoid
//			else if (curObj->m_update_distance)
//				frange = curObj->m_update_distance;
			else
				fRange = m_UpdateDistance; // normal distance

			//If we have a update_distance, check if we are in range. 
			if( fRange > 0.0f && curObj->GetDistance2dSq(obj) > fRange )
			{
				if( plObj )
					plObj->RemoveIfVisible(curObj);

				if( curObj->IsPlayer() )
					TO_PLAYER( curObj )->RemoveIfVisible(obj);

				curObj->RemoveInRangeObject(obj);

				if( obj->GetMapMgr() != this )
				{
					/* Something removed us. */
					return;
				}
				obj->RemoveInRangeObject(iter2);
			}
		}
	}

	///////////////////////////
	// Get new cell coordinates
	///////////////////////////
	if(obj->GetMapMgr() != this)
	{
		/* Something removed us. */
		return;
	}

	if(obj->GetPositionX() >= _maxX || obj->GetPositionX() <= _minX ||
		obj->GetPositionY() >= _maxY || obj->GetPositionY() <= _minY)
	{
		if(obj->IsPlayer())
		{
			Player* plr = TO_PLAYER( obj );
			if(plr->GetBindMapId() != GetMapId())
			{
				plr->SafeTeleport(plr->GetBindMapId(),0,plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				WorldPacket * data = plr->BuildTeleportAckMsg(plr->GetPosition());
				plr->GetSession()->SendPacket(data);
				delete data;
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0,0,0,0);
		}
	}

	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());

	if(cellX >= _sizeX || cellY >= _sizeY)
	{
		return;
	}

	MapCell *objCell = GetCell(cellX, cellY);
	MapCell * pOldCell = obj->GetMapCell();
	if (!objCell)
	{
		objCell = Create(cellX,cellY);
		objCell->Init(cellX, cellY, _mapId, this);
	}

	// If object moved cell
	if (objCell != obj->GetMapCell())
	{
		// THIS IS A HACK!
		// Current code, if a creature on a long waypoint path moves from an active
		// cell into an inactive one, it will disable itself and will never return.
		// This is to prevent cpu leaks. I will think of a better solution very soon :P

		if(!objCell->IsActive() && !plObj && obj->Active)
			obj->Deactivate(this);

		if(obj->GetMapCell())
			obj->GetMapCell()->RemoveObject(obj);
	
		objCell->AddObject(obj);
		obj->SetMapCell(objCell);

		// if player we need to update cell activity
		// radius = 2 is used in order to update both
		// old and new cells
		if(obj->GetTypeId() == TYPEID_PLAYER)
		{
			// have to unlock/lock here to avoid a deadlock situation.
			UpdateCellActivity(cellX, cellY, 2);
			if( pOldCell != NULL )
			{
				// only do the second check if theres -/+ 2 difference
				if( abs( (int)cellX - (int)pOldCell->_x ) > 2 ||
					abs( (int)cellY - (int)pOldCell->_y ) > 2 )
				{
					UpdateCellActivity( pOldCell->_x, pOldCell->_y, 2 );
				}
			}
		}
	}


	//////////////////////////////////////
	// Update in-range set for new objects
	//////////////////////////////////////

	uint32 endX = cellX <= _sizeX ? cellX + 1 : (_sizeX-1);
	uint32 endY = cellY <= _sizeY ? cellY + 1 : (_sizeY-1);
	uint32 startX = cellX > 0 ? cellX - 1 : 0;
	uint32 startY = cellY > 0 ? cellY - 1 : 0;
	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter;

	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell)
				UpdateInRangeSet(obj, plObj, cell);
		}
	}

	if(obj->IsUnit())
	{
		Unit* pobj = TO_UNIT(obj);
		pobj->OnPositionChange();
	}
}

void MapMgr::UpdateInRangeSet( Object* obj, Player* plObj, MapCell* cell )
{
	if( cell == NULL )
		return;

	Object* curObj;
	Player* plObj2;
	int count;
	ObjectSet::iterator iter = cell->Begin();
	ObjectSet::iterator itr;
	float fRange;
	bool cansee, isvisible;

	while( iter != cell->End() )
	{
		curObj = *iter;
		++iter;

		if( curObj == NULL )
			continue;

		if( curObj->IsPlayer() && obj->IsPlayer() && plObj && plObj->m_TransporterGUID && plObj->m_TransporterGUID == TO_PLAYER( curObj )->m_TransporterGUID )
			fRange = 0.0f; // unlimited distance for people on same boat
		else if( curObj->IsPlayer() && obj->IsPlayer() && plObj && plObj->m_CurrentVehicle && plObj->m_CurrentVehicle == TO_PLAYER( curObj )->m_CurrentVehicle )
			fRange = 0.0f; // unlimited distance for people on same vehicle
		else if( curObj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER || curObj->GetTypeFromGUID() ==  HIGHGUID_TYPE_VEHICLE)
			fRange = 0.0f; // unlimited distance for transporters (only up to 2 cells +/- anyway.)
//TODO Implement distance by protoid
//			else if (curObj->m_update_distance)
//				frange = curObj->m_update_distance;
		else
			fRange = m_UpdateDistance; // normal distance

		//Add if we are not ourself and  range ==0 or distance is withing range.
		if ( curObj != obj && (fRange == 0.0f || curObj->GetDistance2dSq( obj ) <= fRange ))
		{
			if( !obj->IsInRangeSet( curObj ) )
			{
				// Object in range, add to set
				obj->AddInRangeObject( curObj );
				curObj->AddInRangeObject( obj );

				if( curObj->IsPlayer() )
				{
					plObj2 = TO_PLAYER( curObj );

					if( plObj2->CanSee( obj ) && !plObj2->IsVisible( obj ) )
					{
						count = obj->BuildCreateUpdateBlockForPlayer(&m_createBuffer, plObj2);
						plObj2->PushCreationData(&m_createBuffer, count);
						plObj2->AddVisibleObject(obj);
						m_createBuffer.clear();
					}
				}

				if( plObj != NULL )
				{
					if( plObj->CanSee( curObj ) && !plObj->IsVisible( curObj ) )
					{
						count = curObj->BuildCreateUpdateBlockForPlayer( &m_createBuffer, plObj );
						plObj->PushCreationData( &m_createBuffer, count );
						plObj->AddVisibleObject( curObj );
						m_createBuffer.clear();
					}
				}
			}
			else
			{
				// Check visiblility
				if( curObj->IsPlayer() )
				{
					plObj2 = TO_PLAYER( curObj );
					cansee = plObj2->CanSee(obj);
					isvisible = plObj2->GetVisibility(obj, &itr);
					if(!cansee && isvisible)
					{
						plObj2->PushOutOfRange(obj->GetNewGUID());
						plObj2->RemoveVisibleObject(itr);
					}
					else if(cansee && !isvisible)
					{
						count = obj->BuildCreateUpdateBlockForPlayer(&m_createBuffer, plObj2);
						plObj2->PushCreationData(&m_createBuffer, count);
						plObj2->AddVisibleObject(obj);
						m_createBuffer.clear();
					}
				}

				if( plObj )
				{
					cansee = plObj->CanSee( curObj );
					isvisible = plObj->GetVisibility( curObj, &itr );
					if(!cansee && isvisible)
					{
						plObj->PushOutOfRange( curObj->GetNewGUID() );
						plObj->RemoveVisibleObject( itr );
					}
					else if(cansee && !isvisible)
					{
						count = curObj->BuildCreateUpdateBlockForPlayer( &m_createBuffer, plObj );
						plObj->PushCreationData( &m_createBuffer, count );
						plObj->AddVisibleObject( curObj );
						m_createBuffer.clear();
					}
				}
			}
		}
	}
/*
#define IN_RANGE_LOOP_P1 \
	while(iter != cell->End()) \
	{ \
		curObj = *iter; \
		++iter; \
		if(curObj->IsPlayer() && obj->IsPlayer() && plObj && plObj->m_TransporterGUID && plObj->m_TransporterGUID == TO_PLAYER( curObj )->m_TransporterGUID) \
			fRange = 0.0f; \
		else if((curObj->GetGUIDHigh() == HIGHGUID_TRANSPORTER ||obj->GetGUIDHigh() == HIGHGUID_TRANSPORTER)) \
			fRange = 0.0f; \
		else if((curObj->GetGUIDHigh() == HIGHGUID_GAMEOBJECT && curObj->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == GAMEOBJECT_TYPE_TRANSPORT || obj->GetGUIDHigh() == HIGHGUID_GAMEOBJECT && obj->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == GAMEOBJECT_TYPE_TRANSPORT)) \
			fRange = 0.0f; \
		else \
			fRange = m_UpdateDistance; \
		if ( curObj != obj && (fRange == 0.0f || curObj->GetDistance2dSq(obj) < fRange ) ) \
		{ \
			if(!obj->IsInRangeSet(curObj)) \
			{ \
				if(curObj->NeedsInRangeSet()) \
				{ \
					curObj->AddInRangeObject(obj); \
				} else if(obj->IsPlayer()) \
				{ \
					curObj->AddInRangePlayer(obj); \
				} \
				if(curObj->IsPlayer()) \
				{ \
					plObj2 = TO_PLAYER( curObj ); \
					if (plObj2->CanSee(obj) && !plObj2->IsVisible(obj))  \
					{ \
						CHECK_BUF; \
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2); \
						plObj2->PushCreationData(*buf, count); \
						plObj2->AddVisibleObject(obj); \
						(*buf)->clear(); \
					} \
				} 

#define IN_RANGE_LOOP_P2 \
			} \
			else \
			{ \
				if(curObj->IsPlayer()) \
				{ \
					plObj2 = TO_PLAYER( curObj ); \
					cansee = plObj2->CanSee(obj); \
					isvisible = plObj2->GetVisibility(obj, &itr); \
					if(!cansee && isvisible) \
					{ \
						plObj2->RemoveVisibleObject(itr); \
						plObj2->PushOutOfRange(obj->GetNewGUID()); \
					} \
					else if(cansee && !isvisible) \
					{ \
						CHECK_BUF; \
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2); \
						plObj2->PushCreationData(*buf, count); \
						plObj2->AddVisibleObject(obj); \
						(*buf)->clear(); \
					} \
				} \

#define IN_RANGE_LOOP_P3 \
			} \
		} \
	} \


	if(plObj)
	{
		IN_RANGE_LOOP_P1

			obj->AddInRangeObject(curObj);
			if(plObj->CanSee(curObj) && !plObj->IsVisible(curObj))
			{
				CHECK_BUF;
				count = curObj->BuildCreateUpdateBlockForPlayer(*buf, plObj);
				plObj->PushCreationData(*buf, count);
				plObj->AddVisibleObject(curObj);
				(*buf)->clear();
			}

		IN_RANGE_LOOP_P2

			if(plObj)
			{
				cansee = plObj->CanSee(curObj);
				isvisible = plObj->GetVisibility(curObj, &itr);
				if(!cansee && isvisible)
				{
					plObj->PushOutOfRange(curObj->GetNewGUID());
					plObj->RemoveVisibleObject(itr);
				}
				else if(cansee && !isvisible)
				{
					CHECK_BUF;
					count = curObj->BuildCreateUpdateBlockForPlayer(*buf, plObj);
					plObj->PushCreationData(*buf, count);
					plObj->AddVisibleObject(curObj);
					(*buf)->clear();
				}
			}

		IN_RANGE_LOOP_P3
	} else if(obj->NeedsInRangeSet())
	{
		IN_RANGE_LOOP_P1
			obj->AddInRangeObject(curObj);
		IN_RANGE_LOOP_P2
		IN_RANGE_LOOP_P3
	}
	else
	{
		IN_RANGE_LOOP_P1
			if(curObj->IsPlayer())
				obj->AddInRangePlayer(obj);

		IN_RANGE_LOOP_P2
		IN_RANGE_LOOP_P3
	}

#undef IN_RANGE_LOOP_P1
#undef IN_RANGE_LOOP_P2
#undef IN_RANGE_LOOP_P3*/
}

void MapMgr::_UpdateObjects()
{
	if(!_updates.size() && !_processQueue.size())
	{
		return;
	}
	//m_updateMutex.Release();

	Object* pObj;
	Player* pOwner;
	//std::set<Object* >::iterator it_start, it_end, itr;
	unordered_set<Player*  >::iterator it_start, it_end, itr;
	Player* lplr;
	uint32 count = 0;
	
	//m_updateMutex.Acquire();
	UpdateQueue::iterator iter = _updates.begin();
	//m_updateMutex.Release();
	PUpdateQueue::iterator it, eit;

	for(; iter != _updates.end();)
	{
		pObj = *iter;
		++iter;
		if(!pObj) continue;

		if(pObj->GetTypeId() == TYPEID_ITEM || pObj->GetTypeId() == TYPEID_CONTAINER)
		{
			// our update is only sent to the owner here.
			pOwner = TO_ITEM(pObj)->GetOwner();
			if( pOwner != NULL )
			{
				count = TO_ITEM( pObj )->BuildValuesUpdateBlockForPlayer( &m_updateBuffer, pOwner );
				// send update to owner
				if( count )
				{
					pOwner->PushUpdateData( &m_updateBuffer, count );
					m_updateBuffer.clear();
				}
			}
		}
		else
		{
			if( pObj->IsInWorld() )
			{
				// players have to receive their own updates ;)
				if( pObj->GetTypeId() == TYPEID_PLAYER )
				{
					// need to be different! ;)
					count = pObj->BuildValuesUpdateBlockForPlayer( &m_updateBuffer, TO_PLAYER( pObj ) );
					if( count )
					{
						TO_PLAYER( pObj )->PushUpdateData( &m_updateBuffer, count );
						m_updateBuffer.clear();
					}
				}

				if( pObj->IsUnit() && pObj->HasUpdateField( UNIT_FIELD_HEALTH ) )
					TO_UNIT( pObj )->EventHealthChangeSinceLastUpdate();

				// build the update
				count = pObj->BuildValuesUpdateBlockForPlayer( &m_updateBuffer, TO_PLAYER(NULL) );

				if( count )
				{
					it_start = pObj->GetInRangePlayerSetBegin();
					it_end = pObj->GetInRangePlayerSetEnd();
					for(itr = it_start; itr != it_end;)
					{
						lplr = *itr;
						++itr;
						// Make sure that the target player can see us.
						if( lplr != NULL && lplr->GetTypeId() == TYPEID_PLAYER && lplr->IsVisible( pObj ) )
							lplr->PushUpdateData( &m_updateBuffer, count );
					}
					m_updateBuffer.clear();
				}
			}
		}
		pObj->ClearUpdateMask();
	}
	//_updates.clear();
	//m_updateMutex.Release();
	
	// generate pending a9packets and send to clients.
	Player* plyr;
	m_updateMutex.Acquire();
	for(it = _processQueue.begin(); it != _processQueue.end();)
	{
		plyr = *it;
		eit = it;
		++it;
		_processQueue.erase(eit);
		if(plyr->GetMapMgr() == this)
			plyr->ProcessPendingUpdates(&m_updateBuildBuffer, &m_compressionBuffer);
	}
	m_updateMutex.Release();
}
void MapMgr::LoadAllCells()
{
	// eek
	MapCell * cellInfo;
	CellSpawns * spawns;

	for( uint32 x = 0 ; x < _sizeX ; x ++ )
	{
		for( uint32 y = 0 ; y < _sizeY ; y ++ )
		{
			cellInfo = GetCell( x , y );
			
			if( !cellInfo )
			{
				// Cell doesn't exist, create it.
				// There is no spoon. Err... cell.
				cellInfo = Create( x , y );
				cellInfo->Init( x , y , _mapId , this );
				DEBUG_LOG("MapMgr","Created cell [%u,%u] on map %d (instance %d)." , x , y , _mapId , m_instanceID );
				cellInfo->SetActivity( true );
				_map->CellGoneActive( x , y );
				ASSERT( !cellInfo->IsLoaded() );

				spawns = _map->GetSpawnsList( x , y );
				if( spawns )
					cellInfo->LoadObjects( spawns );
			}
			else
			{
				// Cell exists, but is inactive
				if ( !cellInfo->IsActive() )
				{
					DEBUG_LOG("MapMgr","Activated cell [%u,%u] on map %d (instance %d).", x, y, _mapId, m_instanceID );
					_map->CellGoneActive( x , y );
					cellInfo->SetActivity( true );

					if (!cellInfo->IsLoaded())
					{
						//DEBUG_LOG("MapMgr","Loading objects for Cell [%d][%d] on map %d (instance %d)...", 
						//	posX, posY, this->_mapId, m_instanceID);
						spawns = _map->GetSpawnsList( x , y );
						if( spawns )
							cellInfo->LoadObjects( spawns );
					}
				}
			}
		}
	}
}

void MapMgr::UpdateCellActivity(uint32 x, uint32 y, int radius)
{
	CellSpawns * sp;
	uint32 endX = (x + radius) <= _sizeX ? x + radius : (_sizeX-1);
	uint32 endY = (y + radius) <= _sizeY ? y + radius : (_sizeY-1);
	uint32 startX = x - radius > 0 ? x - radius : 0;
	uint32 startY = y - radius > 0 ? y - radius : 0;
	uint32 posX, posY;

	MapCell *objCell;

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			objCell = GetCell(posX, posY);

			if (!objCell)
			{
				if (_CellActive(posX, posY))
				{
					objCell = Create(posX, posY);
					objCell->Init(posX, posY, _mapId, this);

//					DEBUG_LOG("MapMgr","Cell [%d,%d] on map %d (instance %d) is now active.", 
//						posX, posY, this->_mapId, m_instanceID);
					objCell->SetActivity(true);
					_map->CellGoneActive(posX, posY);

					ASSERT(!objCell->IsLoaded());

//					DEBUG_LOG("MapMgr","Loading objects for Cell [%d][%d] on map %d (instance %d)...", 
//						posX, posY, this->_mapId, m_instanceID);

					sp = _map->GetSpawnsList(posX, posY);
					if(sp) objCell->LoadObjects(sp);
				}
			}
			else
			{
				//Cell is now active
				if (_CellActive(posX, posY) && !objCell->IsActive())
				{
//					DEBUG_LOG("MapMgr","Cell [%d,%d] on map %d (instance %d) is now active.", 
//						posX, posY, this->_mapId, m_instanceID);
					_map->CellGoneActive(posX, posY);
					objCell->SetActivity(true);

					if (!objCell->IsLoaded())
					{
//						DEBUG_LOG("MapMgr","Loading objects for Cell [%d][%d] on map %d (instance %d)...", 
//							posX, posY, this->_mapId, m_instanceID);
						sp = _map->GetSpawnsList(posX, posY);
						if(sp) objCell->LoadObjects(sp);
					}
				}
				//Cell is no longer active
				else if (!_CellActive(posX, posY) && objCell->IsActive())
				{
//					DEBUG_LOG("MapMgr","Cell [%d,%d] on map %d (instance %d) is now idle.", 
//						posX, posY, this->_mapId, m_instanceID);
					_map->CellGoneIdle(posX, posY);
					objCell->SetActivity(false);
				}
			}
		}
	}

}

void MapMgr::AddForcedCell(MapCell * c)
{
	c->SetPermanentActivity(true);
	UpdateCellActivity(c->GetPositionX(), c->GetPositionY(), 1);
}

void MapMgr::RemoveForcedCell(MapCell * c)
{
	c->SetPermanentActivity(false);
	UpdateCellActivity(c->GetPositionX(), c->GetPositionY(), 1);
}

bool MapMgr::_CellActive(uint32 x, uint32 y)
{
	uint32 endX = ((x+1) <= _sizeX) ? x + 1 : (_sizeX-1);
	uint32 endY = ((y+1) <= _sizeY) ? y + 1 : (_sizeY-1);
	uint32 startX = x > 0 ? x - 1 : 0;
	uint32 startY = y > 0 ? y - 1 : 0;
	uint32 posX, posY;

	MapCell *objCell;

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			objCell = GetCell(posX, posY);

			if (objCell)
			{
				if (objCell->HasPlayers() || objCell->IsForcedActive() )
				{
					return true;
				}
			}
		}
	}

	return false;
}

void MapMgr::ObjectUpdated(Object* obj)
{
	// set our fields to dirty
	// stupid fucked up code in places.. i hate doing this but i've got to :<
	// - burlex
	_updates.insert(obj);
}

void MapMgr::PushToProcessed(Player* plr)
{
	_processQueue.insert(plr);
}


void MapMgr::ChangeFarsightLocation(Player* plr, Unit* farsight, bool apply)
{
	if(!apply)
	{
		// We're clearing.
		for(ObjectSet::iterator itr = plr->m_visibleFarsightObjects.begin(); itr != plr->m_visibleFarsightObjects.end();
			++itr)
		{
			// Send destroy
			plr->PushOutOfRange((*itr)->GetNewGUID());
		}
		plr->m_visibleFarsightObjects.clear();
	}
	else
	{
		plr->m_visibleFarsightObjects.clear();
		uint32 cellX = GetPosX(farsight->GetPositionX());
		uint32 cellY = GetPosY(farsight->GetPositionY());
		uint32 endX = (cellX <= _sizeX) ? cellX + 1 : (_sizeX-1);
		uint32 endY = (cellY <= _sizeY) ? cellY + 1 : (_sizeY-1);
		uint32 startX = cellX > 0 ? cellX - 1 : 0;
		uint32 startY = cellY > 0 ? cellY - 1 : 0;
		uint32 posX, posY;
		MapCell *cell;
		Object* obj;
		MapCell::ObjectSet::iterator iter, iend;
		uint32 count;
		for (posX = startX; posX <= endX; ++posX )
		{
			for (posY = startY; posY <= endY; ++posY )
			{
				cell = GetCell(posX, posY);
				if (cell)
				{
					iter = cell->Begin();
					iend = cell->End();
					for(; iter != iend; ++iter)
					{
						obj = (*iter);
						if(!plr->IsVisible(obj) && plr->CanSee(obj) && farsight->GetDistance2dSq(obj) <= m_UpdateDistance)
						{
							ByteBuffer buf;
							count = obj->BuildCreateUpdateBlockForPlayer(&buf, plr);
							plr->PushCreationData(&buf, count);
							plr->m_visibleFarsightObjects.insert(obj);
						}
					}
					
				}
			}
		}
	}
}

void MapMgr::ChangeFarsightLocation(Player* plr, float X, float Y, bool apply)
{
	if(!apply)
	{
		// We're clearing.
		for(ObjectSet::iterator itr = plr->m_visibleFarsightObjects.begin(); itr != plr->m_visibleFarsightObjects.end();
			++itr)
		{
			// Send destroy
			plr->PushOutOfRange((*itr)->GetNewGUID());
		}
		plr->m_visibleFarsightObjects.clear();
	}
	else
	{
		uint32 cellX = GetPosX(X);
		uint32 cellY = GetPosY(Y);
		uint32 endX = (cellX <= _sizeX) ? cellX + 1 : (_sizeX-1);
		uint32 endY = (cellY <= _sizeY) ? cellY + 1 : (_sizeY-1);
		uint32 startX = cellX > 0 ? cellX - 1 : 0;
		uint32 startY = cellY > 0 ? cellY - 1 : 0;
		uint32 posX, posY;
		MapCell *cell;
		Object* obj;
		MapCell::ObjectSet::iterator iter, iend;
		uint32 count;
		for (posX = startX; posX <= endX; ++posX )
		{
			for (posY = startY; posY <= endY; ++posY )
			{
				cell = GetCell(posX, posY);
				if (cell)
				{
					iter = cell->Begin();
					iend = cell->End();
					for(; iter != iend; ++iter)
					{
						obj = (*iter);
						if(!plr->IsVisible(obj) && plr->CanSee(obj) && obj->GetDistance2dSq(X, Y) <= m_UpdateDistance)
						{
							ByteBuffer buf;
							count = obj->BuildCreateUpdateBlockForPlayer(&buf, plr);
							plr->PushCreationData(&buf, count);
							plr->m_visibleFarsightObjects.insert(obj);
						}
					}
					
				}
			}
		}
	}
}

/* new stuff
*/

bool MapMgr::run()
{
	return Do();
}

bool MapMgr::Do()
{
#ifdef WIN32
	threadid=GetCurrentThreadId();
#endif
	thread_running = true;
	SetThreadName("Map mgr - M%u|I%u",this->_mapId ,this->m_instanceID);
	ObjectSet::iterator i;
	uint32 last_exec=getMSTime();

	// Create Instance script
	LoadInstanceScript();

	/* create static objects */
	for(GOSpawnList::iterator itr = _map->staticSpawns.GOSpawns.begin(); itr != _map->staticSpawns.GOSpawns.end(); ++itr)
	{
		GameObject* obj = CreateGameObject((*itr)->entry);
		if(obj == NULL)
			continue;
		obj->Load((*itr));
		_mapWideStaticObjects.insert(obj);
	}

	for(CreatureSpawnList::iterator itr = _map->staticSpawns.CreatureSpawns.begin(); itr != _map->staticSpawns.CreatureSpawns.end(); ++itr)
	{
		Creature* obj = NULL;
		obj = CreateCreature((*itr)->entry);
		if(obj == NULL)
			continue;

		obj->Load(*itr, iInstanceMode, pMapInfo);
		_mapWideStaticObjects.insert(obj);
	}

	/* add static objects */
	for(set<Object* >::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
		PushStaticObject(*itr);

	/* load corpses */
	objmgr.LoadCorpses(this);

	// initialize worldstates
	sWorldStateTemplateManager.ApplyMapTemplate(this);

	// Call script OnLoad virtual procedure
	CALL_INSTANCE_SCRIPT_EVENT( this, OnLoad )();

	if( GetMapInfo()->type == INSTANCE_NULL )
		sHookInterface.OnContinentCreate(this);

	// always declare local variables outside of the loop!
	// otherwise theres a lot of sub esp; going on.

	uint32 exec_time, exec_start;
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif
	while(m_threadRunning)
	{
		exec_start=getMSTime();
		//first push to world new objects
		m_objectinsertlock.Acquire();//<<<<<<<<<<<<<<<<
		if(m_objectinsertpool.size())
		{
			for(i=m_objectinsertpool.begin();i!=m_objectinsertpool.end();++i)

				(*i)->PushToWorld(this);

			m_objectinsertpool.clear();
		}
		m_objectinsertlock.Release();//>>>>>>>>>>>>>>>>
		//-------------------------------------------------------
				
		//Now update sessions of this map + objects
		_PerformObjectDuties();

		last_exec=getMSTime();
		exec_time=last_exec-exec_start;
		if(exec_time<MAP_MGR_UPDATE_PERIOD)
		{
			/*
				The common place I see this is waiting for a Win32 thread to exit. I used to come up with all sorts of goofy,
				elaborate event-based systems to do this myself until I discovered that thread handles are waitable. Just use
				WaitForSingleObject() on the thread handle and you're done. No risking race conditions with the thread exit code.
				I think pthreads has pthread_join() for this too.

				- http://www.virtualdub.org/blog/pivot/entry.php?id=62
			*/

#ifdef WIN32
			WaitForSingleObject(hThread, MAP_MGR_UPDATE_PERIOD-exec_time);
#else
			Sleep(MAP_MGR_UPDATE_PERIOD-exec_time);
#endif
		}

		//////////////////////////////////////////////////////////////////////////
		// Check if we have to die :P
		//////////////////////////////////////////////////////////////////////////
		if(InactiveMoveTime && UNIXTIME >= InactiveMoveTime)
			break;
	}

	// Clear the instance's reference to us.
	if(m_battleground)
	{
		BattlegroundManager.DeleteBattleground(m_battleground);
		sInstanceMgr.DeleteBattlegroundInstance( GetMapId(), GetInstanceID() );
	}

	if(pInstance)
	{
		// check for a non-raid instance, these expire after 10 minutes.
		if(GetMapInfo()->type == INSTANCE_NONRAID || pInstance->m_isBattleground)
		{
			pInstance->m_mapMgr = NULL;
			sInstanceMgr._DeleteInstance(pInstance, true);
		}
		else
		{
			// just null out the pointer
			pInstance->m_mapMgr=NULL;
		}
	}
	else if(GetMapInfo()->type == INSTANCE_NULL)
		sInstanceMgr.m_singleMaps[GetMapId()] = NULL;

	// Teleport any left-over players out.
	TeleportPlayers();	

	thread_running = false;
	if(thread_kill_only)
		return false;

	// delete ourselves
	Destructor();

	// already deleted, so the threadpool doesn't have to.
	return false;
}

void MapMgr::BeginInstanceExpireCountdown()
{
	WorldPacket data(SMSG_RAID_GROUP_ONLY, 8);

	// so players getting removed don't overwrite us
	forced_expire = true;
	
	// send our sexy packet
	data << uint32(60000) << uint32(1);

	// Update all players on map.
	__player_iterator = m_PlayerStorage.begin();
	Player* ptr;
	for(; __player_iterator != m_PlayerStorage.end();)
	{
		ptr = __player_iterator->second;;
		++__player_iterator;

		if(ptr->GetSession())
		{
			if(!ptr->raidgrouponlysent)
				ptr->GetSession()->SendPacket(&data);
		}
	}

	// set our expire time to 60 seconds.
	InactiveMoveTime = UNIXTIME + 60;
}

void MapMgr::AddObject(Object* obj)
{
	m_objectinsertlock.Acquire();//<<<<<<<<<<<<
	m_objectinsertpool.insert(obj);
	m_objectinsertlock.Release();//>>>>>>>>>>>>
}


Unit* MapMgr::GetUnit(const uint64 & guid)
{
	switch(GET_TYPE_FROM_GUID(guid))
	{
	case HIGHGUID_TYPE_UNIT:
		return GetCreature( GET_LOWGUID_PART(guid) );
		break;

	case HIGHGUID_TYPE_PLAYER:
		return GetPlayer( (uint32)guid );
		break;

	case HIGHGUID_TYPE_PET:
		return GetPet( GET_LOWGUID_PART(guid) );
		break;

	case HIGHGUID_TYPE_VEHICLE:
		return GetVehicle( GET_LOWGUID_PART(guid) );
 		break;
	}

	return NULL;
}

Object* MapMgr::_GetObject(const uint64 & guid)
{
	switch(GET_TYPE_FROM_GUID(guid))
	{
	case	HIGHGUID_TYPE_VEHICLE:
		return GetVehicle(GET_LOWGUID_PART(guid));
 		break;
	case	HIGHGUID_TYPE_GAMEOBJECT:
		return GetGameObject(GET_LOWGUID_PART(guid));
		break;
	case	HIGHGUID_TYPE_UNIT:
		return GetCreature(GET_LOWGUID_PART(guid));
		break;
	case	HIGHGUID_TYPE_DYNAMICOBJECT:
		return GetDynamicObject((uint32)guid);
		break;
	case	HIGHGUID_TYPE_TRANSPORTER:
		return objmgr.GetTransporter(GUID_LOPART(guid));
		break;
	case HIGHGUID_TYPE_CORPSE:
		return objmgr.GetCorpse(GUID_LOPART(guid));
		break;
	default:
		return GetUnit(guid);
		break;
	}
}

void MapMgr::_PerformObjectDuties()
{
	++mLoopCounter;
	uint32 mstime = getMSTime();
	uint32 difftime = mstime - lastUnitUpdate;
	if(difftime > 500)
		difftime = 500;

	// Update our objects.

	Creature* ptr;
	__creature_iterator = activeCreatures.begin();
	for(; __creature_iterator != activeCreatures.end();)
	{
		ptr = *__creature_iterator;
		++__creature_iterator;

		ptr->Update(difftime);
	}

	Pet* ptr2;
	__pet_iterator = m_PetStorage.begin();
	for(; __pet_iterator != m_PetStorage.end();)
	{
		ptr2 = __pet_iterator->second;
		++__pet_iterator;

		ptr2->Update(difftime);
	}		

	Vehicle* ptr3;
	__vehicle_iterator = activeVehicles.begin();
	for(; __vehicle_iterator != activeVehicles.end();)
	{
		ptr3 = *__vehicle_iterator;
		++__vehicle_iterator;
 
		ptr3->Update(difftime);
	}

	// Update any events.
	eventHolder.Update(difftime);

	// Update players.
	Player* ptr4;
	__player_iterator = m_PlayerStorage.begin();
	for(; __player_iterator != m_PlayerStorage.end();)
	{
		ptr4 = __player_iterator->second;;
		++__player_iterator;

		ptr4->Update( difftime );
	}

	lastUnitUpdate = mstime;

	// Update gameobjects (every 2nd tick only).
	if( mLoopCounter % 2 )
	{
		difftime = mstime - lastGameobjectUpdate;

		GameObject* ptr5;
		__gameobject_iterator = activeGameObjects.begin();
		for(; __gameobject_iterator != activeGameObjects.end(); )
		{
			ptr5 = *__gameobject_iterator;
			++__gameobject_iterator;

			ptr5->Update( difftime );
		}

		lastGameobjectUpdate = mstime;
	}	

	// Sessions are updated every loop.
	{
		int result = 0;
		WorldSession * MapSession;
		SessionSet::iterator itr = MapSessions.begin();
		SessionSet::iterator it2;

		for(; itr != MapSessions.end();)
		{
			MapSession = (*itr);
			it2 = itr;
			++itr;

			//we have teleported to another map, remove us here.
			if(MapSession->GetInstance() != m_instanceID)
			{
				MapSessions.erase(it2);
				continue;
			}

			// Don't update players not on our map.
			// If we abort in the handler, it means we will "lose" packets, or not process this.
			// .. and that could be diasterous to our client :P
			if( MapSession->GetPlayer()->GetMapMgr()== NULL || 
				MapSession->GetPlayer()->GetMapMgr() != this)
				continue;

			result = MapSession->Update(m_instanceID);
			if(result)//session or socket deleted?
			{
				if(result == 1)//socket don't exist anymore, delete from both world- and map-sessions.
					sWorld.DeleteGlobalSession(MapSession);
				MapSessions.erase(it2);
			}
		}
	}
	// Finally, A9 Building/Distribution
	_UpdateObjects();
}

void MapMgr::EventCorpseDespawn(uint64 guid)
{
	Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
	if(pCorpse == NULL)	// Already Deleted
		return;

	if(pCorpse->GetMapMgr() != this)
		return;

	pCorpse->Despawn();
	pCorpse->Destructor();
}

void MapMgr::TeleportPlayers()
{
	if(!bServerShutdown)
	{
		// Update all players on map.
		__player_iterator = m_PlayerStorage.begin();
		Player* ptr;
		for(; __player_iterator != m_PlayerStorage.end();)
		{
			ptr = __player_iterator->second;;
			++__player_iterator;

			if(ptr->GetSession())
				ptr->EjectFromInstance();
		}
	}
	else
	{
		// Update all players on map.
		__player_iterator = m_PlayerStorage.begin();
		Player* ptr;
		for(; __player_iterator != m_PlayerStorage.end();)
		{
			ptr = __player_iterator->second;;
			++__player_iterator;

			if(ptr->GetSession())
				ptr->GetSession()->LogoutPlayer(false);
			else
			{
				ptr->Destructor();
				m_PlayerStorage.erase(__player_iterator);
			}
		}
	}
}

void MapMgr::UnloadCell(uint32 x,uint32 y)
{
	MapCell * c = GetCell(x,y);
	if(c == NULL || c->HasPlayers() || _CellActive(x,y) || !c->IsUnloadPending()) return;

	DEBUG_LOG("MapMgr","Unloading Cell [%d][%d] on map %d (instance %d)...", 
		x,y,_mapId,m_instanceID);

	c->Unload();
}

void MapMgr::EventRespawnVehicle(Vehicle* v, MapCell * p)
{
	ObjectSet::iterator itr = p->_respawnObjects.find( v );
	if(itr != p->_respawnObjects.end())
	{
		v->m_respawnCell=NULL;
		p->_respawnObjects.erase(itr);
		v->OnRespawn(this);
	}
}

void MapMgr::EventRespawnCreature(Creature* c, MapCell * p)
{
	ObjectSet::iterator itr = p->_respawnObjects.find( c );
	if(itr != p->_respawnObjects.end())
	{
		c->m_respawnCell=NULL;
		p->_respawnObjects.erase(itr);
		c->OnRespawn(this);
	}
}

void MapMgr::EventRespawnGameObject(GameObject* o, MapCell * c)
{
	ObjectSet::iterator itr = c->_respawnObjects.find( o);
	if(itr != c->_respawnObjects.end())
	{
		o->m_respawnCell=NULL;
		c->_respawnObjects.erase(itr);
		o->Spawn(this);
	}
}

void MapMgr::SendMessageToCellPlayers(Object* obj, WorldPacket * packet, uint32 cell_radius /* = 2 */)
{
	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());
	uint32 endX = ((cellX+cell_radius) <= _sizeX) ? cellX + cell_radius : (_sizeX-1);
	uint32 endY = ((cellY+cell_radius) <= _sizeY) ? cellY + cell_radius : (_sizeY-1);
	uint32 startX = (cellX-cell_radius) > 0 ? cellX - cell_radius : 0;
	uint32 startY = (cellY-cell_radius) > 0 ? cellY - cell_radius : 0;

	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter, iend;
	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell && cell->HasPlayers() )
			{
				iter = cell->Begin();
				iend = cell->End();
				for(; iter != iend; ++iter)
				{
					if((*iter)->IsPlayer())
					{
						TO_PLAYER(*iter)->GetSession()->SendPacket(packet);
					}
				}
			}
		}
	}
}

void MapMgr::SendChatMessageToCellPlayers(Object* obj, WorldPacket * packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession * originator)
{
	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());
	uint32 endX = ((cellX+cell_radius) <= _sizeX) ? cellX + cell_radius : (_sizeX-1);
	uint32 endY = ((cellY+cell_radius) <= _sizeY) ? cellY + cell_radius : (_sizeY-1);
	uint32 startX = (cellX-cell_radius) > 0 ? cellX - cell_radius : 0;
	uint32 startY = (cellY-cell_radius) > 0 ? cellY - cell_radius : 0;

	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter, iend;
	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell && cell->HasPlayers() )
			{
				iter = cell->Begin();
				iend = cell->End();
				for(; iter != iend; ++iter)
				{
					if((*iter)->IsPlayer())
					{
						//TO_PLAYER(*iter)->GetSession()->SendPacket(packet);
						TO_PLAYER(*iter)->GetSession()->SendChatPacket(packet, langpos, lang, originator);
					}
				}
			}
		}
	}
}

Vehicle* MapMgr::GetSqlIdVehicle(uint32 sqlid)
{
	VehicleSqlIdMap::iterator itr = _sqlids_vehicles.find(sqlid);
	return (itr == _sqlids_vehicles.end()) ? NULL : itr->second;
}

Creature* MapMgr::GetSqlIdCreature(uint32 sqlid)
{
	CreatureSqlIdMap::iterator itr = _sqlids_creatures.find(sqlid);
	return (itr == _sqlids_creatures.end()) ? NULL : itr->second;
}

GameObject* MapMgr::GetSqlIdGameObject(uint32 sqlid)
{
	GameObjectSqlIdMap::iterator itr = _sqlids_gameobjects.find(sqlid);
	return (itr == _sqlids_gameobjects.end()) ? NULL : itr->second;
}

void MapMgr::HookOnAreaTrigger(Player* plr, uint32 id)
{
	switch (id)
	{
	case 4591:
		//Only opens when the first one steps in, if 669 if you find a way, put it in :P (else was used to increase the time the door stays opened when another one steps on it)
		GameObject* door = GetInterface()->GetGameObjectNearestCoords(803.827f, 6869.38f, -38.5434f, 184212);
		if (door && (door->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE) == 1))
		{
			door->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_STATE, 0);
			//sEventMgr.AddEvent(door, &GameObject::SetUInt32Value, GAMEOBJECT_STATE, 1, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
		}
		//else
		//{
			//sEventMgr.RemoveEvents(door);
			//sEventMgr.AddEvent(door, &GameObject::SetUInt32Value,GAMEOBJECT_STATE, 0, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
		//}
		break;
	}
}

Vehicle* MapMgr::CreateVehicle(uint32 entry)
{
	uint64 newguid = ( (uint64)HIGHGUID_TYPE_VEHICLE << 32 ) | ( (uint64)entry << 24 );
	Vehicle* v = NULL;
	if(_reusable_guids_vehicle.size())
	{
		uint32 guid = _reusable_guids_vehicle.front();
		_reusable_guids_vehicle.pop_front();

		newguid |= guid;
	}

	else
		newguid |= ++m_VehicleHighGuid;

	v = new Vehicle(newguid);
	v->Init();
	ASSERT( v->GetTypeFromGUID() == HIGHGUID_TYPE_VEHICLE );
	m_VehicleStorage.insert( make_pair< const uint32, Vehicle* >(v->GetUIdFromGUID(), v));
	return v;
}

Creature* MapMgr::CreateCreature(uint32 entry)
{
	uint64 newguid = ( (uint64)HIGHGUID_TYPE_UNIT << 32 ) | ( (uint64)entry << 24 );
	Creature* cr = NULL;
	if(_reusable_guids_creature.size())
	{
		uint32 guid = _reusable_guids_creature.front();
		_reusable_guids_creature.pop_front();

		newguid |= guid;
	}

	else
		newguid |= ++m_CreatureHighGuid;

	cr = new Creature(newguid);
	cr->Init();
	ASSERT( cr->GetTypeFromGUID() == HIGHGUID_TYPE_UNIT );
	return cr;
}

GameObject* MapMgr::CreateGameObject(uint32 entry)
{
	//Validate the entry
	GameObjectInfo *goi = NULL;
	goi = GameObjectNameStorage.LookupEntry( entry );
	if( goi == NULL )
	{
		Log.Warning("MapMgr", "Skipping CreateGameObject for entry %u due to incomplete database.", entry);
		return NULL;
	}

	uint64 new_guid = ( (uint64)HIGHGUID_TYPE_GAMEOBJECT << 32 ) | ( (uint64)entry << 24 );
	m_GOHighGuid &= 0x00FFFFFF;
	new_guid |= (uint64)(++m_GOHighGuid);

	GameObject* go = NULL;
	go = new GameObject(new_guid);
	go->Init();
	ASSERT( go->GetTypeFromGUID() == HIGHGUID_TYPE_GAMEOBJECT );
	return go;
}

DynamicObject* MapMgr::CreateDynamicObject()
{
	DynamicObject* dyn = NULL;
	dyn = new DynamicObject(HIGHGUID_TYPE_DYNAMICOBJECT,(++m_DynamicObjectHighGuid));
	dyn->Init();
	ASSERT( dyn->GetTypeFromGUID() == HIGHGUID_TYPE_DYNAMICOBJECT );
	return dyn;
}

void MapMgr::SendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, StackPacket *pData)
{
	// Update all players on map.

	Player* ptr = NULL;
	PlayerStorageMap::iterator itr1 = m_PlayerStorage.begin();
	for(itr1 = m_PlayerStorage.begin(); itr1 != m_PlayerStorage.end();)
	{
		ptr = itr1->second;
		itr1++;
		if(ptr->GetSession())
		{
			//Are we in the right zone?
			if( iZoneMask != ZONE_MASK_ALL && ptr->GetZoneId() != (uint32)iZoneMask )
				continue;

			//Are we the right faction?
			if( iFactionMask != FACTION_MASK_ALL && ptr->GetTeam() != (uint32)iFactionMask )
				continue;

			ptr->GetSession()->SendPacket(pData);
		}
	}
}

void MapMgr::EventSendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, WorldPacket *pData)
{
	SendPacketToPlayers(iZoneMask, iFactionMask, pData);
}

void MapMgr::SendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, WorldPacket *pData)
{
	// Update all players on map.

	Player * ptr = NULL;
	PlayerStorageMap::iterator itr1 = m_PlayerStorage.begin();
	for(itr1 = m_PlayerStorage.begin(); itr1 != m_PlayerStorage.end();)
	{
		ptr = itr1->second;
		itr1++;
		if(ptr->GetSession())
		{
			//Are we in the right zone?
			if( iZoneMask != ZONE_MASK_ALL && ptr->GetZoneId() != (uint32)iZoneMask )
				continue;

			//Are we the right faction?
			if( iFactionMask != FACTION_MASK_ALL && ptr->GetTeam() != (uint32)iFactionMask )
				continue;

			ptr->GetSession()->SendPacket(pData);
		}
	}
}


void MapMgr::RemoveAuraFromPlayers(int32 iFactionMask, uint32 uAuraId)
{
	// Update all players on map.
	Player* ptr;
	for(__player_iterator = m_PlayerStorage.begin(); __player_iterator != m_PlayerStorage.end();__player_iterator++)
	{
		ptr = __player_iterator->second;;

		if(ptr->GetSession())
		{
			if( iFactionMask != FACTION_MASK_ALL && ptr->GetTeam() != (uint32)iFactionMask )
				continue;

			ptr->RemoveAura(uAuraId);
		}
	}
}

void MapMgr::RemovePositiveAuraFromPlayers(int32 iFactionMask, uint32 uAuraId)
{
	// Update all players on map.
	Player* ptr;
	for(__player_iterator = m_PlayerStorage.begin(); __player_iterator != m_PlayerStorage.end();__player_iterator++)
	{
		ptr = __player_iterator->second;;

		if(ptr->GetSession())
		{
			if( iFactionMask != FACTION_MASK_ALL && ptr->GetTeam() != (uint32)iFactionMask )
				continue;
			ptr->RemovePositiveAura(uAuraId);
		}
	}
}

void MapMgr::CastSpellOnPlayers(int32 iFactionMask, uint32 uSpellId)
{
	SpellEntry * sp = dbcSpell.LookupEntryForced(uSpellId);
	if( !sp )
		return;

	// Update all players on map.
	Player* ptr;
	for(__player_iterator = m_PlayerStorage.begin(); __player_iterator != m_PlayerStorage.end();__player_iterator++)
	{
		ptr = __player_iterator->second;;

		if(ptr->GetSession())
		{
			if( iFactionMask != FACTION_MASK_ALL && ptr->GetTeam() != (uint32)iFactionMask )
				continue;

			ptr->CastSpell(TO_UNIT(__player_iterator->second), sp, true);
		}
	}
}

void MapMgr::SendPvPCaptureMessage(int32 iZoneMask, uint32 ZoneId, const char * Format, ...)
{
	va_list ap;
	va_start(ap,Format);

	WorldPacket data(SMSG_DEFENSE_MESSAGE, 208);
	char msgbuf[200];
	vsnprintf(msgbuf, 200, Format, ap);
	va_end(ap);

	data << ZoneId;
	data << uint32(strlen(msgbuf)+1);
	data << msgbuf;

	// Update all players on map.
	Player* ptr;
	for(__player_iterator = m_PlayerStorage.begin(); __player_iterator != m_PlayerStorage.end();__player_iterator++)
	{
		ptr = __player_iterator->second;;

		if(ptr->GetSession())
		{
			if( ( iZoneMask != ZONE_MASK_ALL && ptr->GetZoneId() != (uint32)iZoneMask) )
				continue;

			ptr->GetSession()->SendPacket(&data);
		}
	}
}

void MapMgr::LoadInstanceScript()
{
	mInstanceScript = sScriptMgr.CreateScriptClassForInstance( _mapId, this );
};

void MapMgr::CallScriptUpdate()
{
	ASSERT( mInstanceScript );
	mInstanceScript->UpdateEvent();
};
