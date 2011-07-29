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
createFileSingleton(StructFactory);

MapScriptInterface::MapScriptInterface(MapMgr* mgr)
{
	mapMgr = mgr;
}

MapScriptInterface::~MapScriptInterface()
{
	mapMgr->ScriptInterface = 0;
}

uint32 MapScriptInterface::GetPlayerCountInRadius(float x, float y, float z /* = 0.0f */, float radius /* = 5.0f */)
{
	// use a cell radius of 2
	uint32 PlayerCount = 0;
	uint32 cellX = mapMgr->GetPosX(x);
	uint32 cellY = mapMgr->GetPosY(y);

	uint32 endX = cellX < _sizeX ? cellX + 1 : _sizeX;
	uint32 endY = cellY < _sizeY ? cellY + 1 : _sizeY;
	uint32 startX = cellX > 0 ? cellX - 1 : 0;
	uint32 startY = cellY > 0 ? cellY - 1 : 0;
	MapCell * pCell;
	ObjectSet::iterator iter, iter_end;

	for(uint32 cx = startX; cx < endX; ++cx)
	{
		for(uint32 cy = startY; cy < endY; ++cy)
		{
			pCell = mapMgr->GetCell(cx, cy);
			if(pCell == 0 || pCell->GetPlayerCount() == 0)
				continue;

			iter = pCell->Begin();
			iter_end = pCell->End();

			for(; iter != iter_end; ++iter)
			{
				if((*iter)->GetTypeId() == TYPEID_PLAYER &&
					(*iter)->CalcDistance(x, y, (z == 0.0f ? (*iter)->GetPositionZ() : z)) < radius)
				{
					++PlayerCount;
				}
			}
		}
	}

	return PlayerCount;
}

GameObject* MapScriptInterface::SpawnGameObject(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, uint32 Misc1, uint32 Misc2)
{
	GameObject* pGameObject = mapMgr->CreateGameObject(Entry);
	if(pGameObject == NULL || !pGameObject->CreateFromProto(Entry, mapMgr->GetMapId(), cX, cY, cZ, cO, 0.0f, 0.0f, 0.0f, 0.0f))
		return NULL;

	pGameObject->SetInstanceID(mapMgr->GetInstanceID());
	pGameObject->SetPhase(1);

	if(AddToWorld)
		pGameObject->PushToWorld(mapMgr);

	return pGameObject;
}

Creature* MapScriptInterface::SpawnCreature(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, bool tmplate, uint32 Misc1, uint32 Misc2)
{
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(Entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(Entry);
	if(proto == 0 || info == 0)
	{
		return NULL;
	}

	CreatureSpawn * sp = NULL;
	sp = new CreatureSpawn;
	sp->entry = Entry;
	sp->form = 0;
	sp->id = 0;
	sp->movetype = 0;
	sp->x = cX;
	sp->y = cY;
	sp->z = cZ;
	sp->o = cO;
	sp->emote_state =0;
	sp->flags = 0;
	sp->factionid = proto->Faction;
	sp->bytes=0;
	sp->bytes1=0;
	sp->bytes2=0;
	sp->displayid=0;
	sp->stand_state = 0;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;
	sp->MountedDisplayID = 0;
	sp->phase = 1;
	sp->vehicle = proto->vehicle_entry;

	Creature* p = NULL;
	p = mapMgr->CreateCreature(Entry);
	if(p == NULL)
	{
		delete sp;
		return NULL;
	}

	p->Load(sp, (uint32)NULL, NULL);
	p->spawnid = 0;
	p->m_spawn = NULL;
	delete sp;

	if( AddToWorld )
		p->PushToWorld(mapMgr);

	return p;
}

void MapScriptInterface::DeleteCreature(Creature* ptr)
{
	ptr->Destructor();
}

void MapScriptInterface::DeleteGameObject(GameObject* ptr)
{
	ptr->Destructor();
}

WayPoint * StructFactory::CreateWaypoint()
{
	return new WayPoint;
}
