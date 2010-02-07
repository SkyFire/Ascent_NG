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


#ifndef _MAP_SCRIPT_INTERFACE_H
#define _MAP_SCRIPT_INTERFACE_H

/* * Class MapScriptInterface
   * Provides an interface to mapmgr for scripts, to obtain objects,
   * get players, etc.
*/

class GameObject;
class Object;
class Creature;
class Unit;
class Player;

class SERVER_DECL MapScriptInterface
{
public:
	MapScriptInterface(MapMgr* mgr);
	~MapScriptInterface();

	template<class T, uint32 TypeId> T* GetObjectNearestCoords(uint32 Entry, float x, float y, float z, float ClosestDist)
	{
		MapCell * pCell = mapMgr->GetCell(mapMgr->GetPosX(x), mapMgr->GetPosY(y));

		if(pCell == NULL)
			return CAST(T, NULL);

		Object* ClosestObject = NULL;
		float CurrentDist = 0;
		ObjectSet::const_iterator iter;
		for(iter = pCell->Begin(); iter != pCell->End(); ++iter)
		{
			CurrentDist = (*iter)->CalcDistance(x, y, (z != 0.0f ? z : (*iter)->GetPositionZ()));
			if(CurrentDist < ClosestDist && (*iter)->GetTypeId() == TypeId)
			{
				if((Entry && (*iter)->GetEntry() == Entry) || !Entry)
				{
					ClosestDist = CurrentDist;
					ClosestObject = (*iter);
				}
			}
		}

		return CAST(T, ClosestObject);
	}

	ASCENT_INLINE GameObject* GetGameObjectNearestCoords(float x, float y, float z = 0.0f, uint32 Entry = 0, float ClosestDistance = 999999.0f)
	{
		return GetObjectNearestCoords<GameObject, TYPEID_GAMEOBJECT>(Entry, x, y, z, ClosestDistance);
	}

	ASCENT_INLINE Creature* GetCreatureNearestCoords(float x, float y, float z = 0.0f, uint32 Entry = 0, float ClosestDistance = 999999.0f)
	{
		return GetObjectNearestCoords<Creature, TYPEID_UNIT>(Entry, x, y, z, ClosestDistance);
	}

	ASCENT_INLINE Player* GetPlayerNearestCoords(float x, float y, float z = 0.0f, uint32 Entry = 0, float ClosestDistance = 999999.0f)
	{
		// Don't bother checking empty maps.
		if(!mapMgr->GetPlayerCount())
			return NULL;
		return GetObjectNearestCoords<Player, TYPEID_PLAYER>(Entry, x, y, z, ClosestDistance);
	}

	uint32 GetPlayerCountInRadius(float x, float y, float z = 0.0f, float radius = 5.0f);
	
	GameObject* SpawnGameObject(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, uint32 Misc1, uint32 Misc2);
	Creature* SpawnCreature(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, bool tmplate, uint32 Misc1, uint32 Misc2);
	WayPoint * CreateWaypoint();

	void DeleteGameObject(GameObject* ptr);
	void DeleteCreature(Creature* ptr);

private:
	MapMgr* mapMgr;
};

class SERVER_DECL StructFactory : public Singleton<StructFactory>
{
public:
	StructFactory() {}
	WayPoint * CreateWaypoint();
};

#define sStructFactory StructFactory::getSingleton()

#endif

