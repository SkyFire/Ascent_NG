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
	MapScriptInterface(MapMgrPointer mgr);
	~MapScriptInterface();

	template<class T, uint32 TypeId> shared_ptr<T> GetObjectNearestCoords(uint32 Entry, float x, float y, float z = 0.0f, float ClosestDist = 999999.0f)
	{
		MapCell * pCell = mapMgr->GetCell(mapMgr->GetPosX(x), mapMgr->GetPosY(y));
		if(pCell == 0)
			return CAST(T, NULLPTR);

		ObjectPointer ClosestObject;
		float CurrentDist = 0;
        ObjectSet::const_iterator iter = pCell->Begin();
		for(; iter != pCell->End(); ++iter)
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

	ASCENT_INLINE GameObjectPointer GetGameObjectNearestCoords(float x, float y, float z = 0.0f, uint32 Entry = 0, float ClosestDistance = 999999.0f)
	{
		return GetObjectNearestCoords<GameObject, TYPEID_GAMEOBJECT>(Entry, x, y, z, ClosestDistance);
	}

	ASCENT_INLINE CreaturePointer GetCreatureNearestCoords(float x, float y, float z = 0.0f, uint32 Entry = 0, float ClosestDistance = 999999.0f)
	{
		return GetObjectNearestCoords<Creature, TYPEID_UNIT>(Entry, x, y, z, ClosestDistance);
	}

	ASCENT_INLINE PlayerPointer GetPlayerNearestCoords(float x, float y, float z = 0.0f, uint32 Entry = 0, float ClosestDistance = 999999.0f)
	{
		return GetObjectNearestCoords<Player, TYPEID_PLAYER>(Entry, x, y, z, ClosestDistance);
	}

	uint32 GetPlayerCountInRadius(float x, float y, float z = 0.0f, float radius = 5.0f);
	
	GameObjectPointer SpawnGameObject(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, uint32 Misc1, uint32 Misc2);
	CreaturePointer SpawnCreature(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, bool tmplate, uint32 Misc1, uint32 Misc2);
	WayPoint * CreateWaypoint();

	void DeleteGameObject(GameObjectPointer ptr);
	void DeleteCreature(CreaturePointer ptr);

private:
	MapMgrPointer mapMgr;
};

class SERVER_DECL StructFactory : public Singleton<StructFactory>
{
public:
	StructFactory() {}
	WayPoint * CreateWaypoint();
};

#define sStructFactory StructFactory::getSingleton()

#endif

