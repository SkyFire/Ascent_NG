/*
 * Scripts for Ascent MMORPG Server
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




#ifndef EASYFUNC_H
#define EASYFUNC_H

// Uncomment if you want debug texts
// #define ENABLE_DEBUG_TEXT

#include "StdAfx.h"

#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#pragma warning(disable:4267) // warning C4267: '=' : conversion from 'size_t' to 'uint32', possible loss of data

#define CREATE_QUESTSCRIPT(cl) (QuestScript*)new cl
#define CREATE_GOSSIPSCRIPT(cl) (GossipScript*)new cl
#define CREATE_GAMEOBJECT_SCRIPT(cl) &cl::Create
#define CREATE_CREATURESCRIPT(cl) &cl::Create


class SCRIPT_DECL EasyFunctions
{
public:
  static EasyFunctions GetInstance()
  {
    static EasyFunctions easy_singleton;
    return easy_singleton;
  }

  void PrintMessage(const char *text, ...)
  {
#ifdef ENABLE_DEBUG_TEXT
    va_list arg;
    va_start(arg, text);

    printf("[EAS] ");
    vprintf(text, arg);
    putc('\n', stdout);

    va_end(arg);
#endif
  }
  
  void WaypointCreate(Creature* pCreature , float x, float y, float z, float o, uint32 waittime, uint32 flags, uint32 modelid)
  {
    PrintMessage("Function call: WaypointCreate()");
    if(pCreature == NULL)
      return;

    if(!pCreature->m_custom_waypoint_map)
		pCreature->m_custom_waypoint_map = new WayPointMap;

    if(!modelid)
      modelid = pCreature->GetUInt32Value(UNIT_FIELD_DISPLAYID);

    WayPoint * wp = new WayPoint;
	wp->id = pCreature->m_custom_waypoint_map->size() ? pCreature->m_custom_waypoint_map->size() + 1 : 1;
    wp->x = x;
    wp->y = y;
    wp->z = z;
    wp->o = o;
    wp->flags = flags;
    wp->backwardskinid = modelid;
    wp->forwardskinid = modelid;
    wp->backwardemoteid = wp->forwardemoteid = 0;
    wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
    wp->waittime = waittime;
	
	if(wp->id <= 0)
		return; //not valid id

	if(pCreature->m_custom_waypoint_map->size() <= wp->id)
		pCreature->m_custom_waypoint_map->resize(wp->id+1);

	if((*pCreature->m_custom_waypoint_map)[wp->id] == NULL)
	{
		(*pCreature->m_custom_waypoint_map)[wp->id] = wp;
	}
	
  }

  void EnableWaypoints(Creature* creat)
  {
	  if(!creat)
		  return;
	  if(!creat->m_custom_waypoint_map)
		  return;
	  
	  creat->GetAIInterface()->SetWaypointMap(creat->m_custom_waypoint_map);
  }
  
  void DeleteWaypoints(Creature* creat)
  {
    PrintMessage("Function call: DeleteWaypoints()");
    if(creat == NULL)
      return;

    if(creat->m_custom_waypoint_map == NULL)
      return;

    WayPointMap::iterator i = creat->m_custom_waypoint_map->begin();
    
    for(; i != creat->m_custom_waypoint_map->end(); ++i)
    {
      if((*i) != NULL)
        delete (*i);
    }

    creat->m_custom_waypoint_map->clear();
  }

  Creature* SpawnCreature(Player* pThis, uint32 entry, float posX, float posY, float posZ, float posO, uint32 duration = 0)
  {
    PrintMessage("Function call: SpawnCreature()");
    if(pThis == NULL)
		return NULL;
      
    CreatureProto *p = CreatureProtoStorage.LookupEntry(entry);
    
    if(p == NULL)
      return NULL;

    Creature* pCreature = pThis->GetMapMgr()->CreateCreature(entry);
    pCreature->m_spawn = 0;
    pCreature->Load(p, posX, posY, posZ);
    pCreature->SetMapId(pThis->GetMapId());
    pCreature->SetOrientation(posO);
    pCreature->Despawn(duration, 0);
    pCreature->SetInstanceID(pThis->GetInstanceID());
    pCreature->PushToWorld(pThis->GetMapMgr());

    return pCreature;
  }
  
  void DeleteSpawned(Creature* creat)
  {
    PrintMessage("Function call: DeleteSpawned()");
    if(creat == NULL)
      return;

    creat->Despawn(0, 0);
    creat->SafeDelete();
  }

  Creature* SpawnCreatureExtended(Player* pThis, uint32 entry, float posX, 
                                         float posY, float posZ, float posO, uint32 faction, 
                                         uint32 duration, bool dis_comb = false, bool dis_mel = false, 
                                         bool dis_target = false)
  {
    PrintMessage("Function call: SpawnCreatureExtended()");

    if(pThis == NULL)
      return NULL;

    CreatureProto *p = CreatureProtoStorage.LookupEntry(entry);
	if(p == NULL)
		return NULL;

    Creature* pCreature  = pThis->GetMapMgr()->CreateCreature(entry);
    pCreature->m_spawn = 0;
    pCreature->Load(p, posX, posY, posZ);
    pCreature->SetMapId(pThis->GetMapId());
    pCreature->SetOrientation(posO);
    pCreature->GetAIInterface()->disable_combat = dis_comb;
    pCreature->GetAIInterface()->disable_melee = dis_mel;
    pCreature->GetAIInterface()->disable_targeting = dis_target;
    pCreature->SetInstanceID(pThis->GetInstanceID());
    pCreature->PushToWorld(pThis->GetMapMgr());
    pCreature->Despawn(duration, 0);
    pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, faction);
    pCreature->_setFaction();
    
    return pCreature;
  }
  
  void GameobjectDelete(GameObject* pC, uint32 duration)
  {
    PrintMessage("Function call: GameobjectDelete()");
    if(pC == NULL)
      return;

	//sEventMgr.AddEvent(pC, &GameObject::ExpireAndDelete, EVENT_GMSCRIPT_EVENT, duration, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT, 0); 
  }

  GameObject* SpawnGameobject(Player* plr, uint32 entry_id, float x, float y, float z, float o, float scale, float orientation1, float orientation2, float orientation3, float orientation4 )
  {
    PrintMessage("Function call: SpawnGameobject()");
	if(plr == NULL)
		return NULL;

    GameObjectInfo *goi = GameObjectNameStorage.LookupEntry(entry_id);
	if(goi == NULL)
		return NULL;

    GameObject* pC = plr->GetMapMgr()->CreateGameObject(entry_id);
    //pC->spawnid=0;
    pC->m_spawn=0;
    pC->CreateFromProto(entry_id, plr->GetMapId(), (float)x, (float)y, (float)z, (float)o, float(orientation1), float(orientation2), float(orientation3), float(orientation4));
    pC->SetFloatValue(OBJECT_FIELD_SCALE_X, (float)scale);
    pC->SetMapId(plr->GetMapId());
    pC->SetInstanceID(plr->GetInstanceID());
    pC->PushToWorld(plr->GetMapMgr());

    return pC;
  }
  
  void MoveToPlayer(Player* plr, Creature* creat)
  {
    PrintMessage("Function call: MoveToPlayer()");
    if(plr == NULL)
      return;
      
    if(creat == NULL)
      return;
      
    creat->GetAIInterface()->MoveTo(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation());
  }

  void CreateCustomWaypointMap(Creature* creat)
  {
    PrintMessage("Function call: CreateCustomWaypointMap()");
    if(creat == NULL)
      return;

	creat->DestroyCustomWaypointMap();
    
    creat->m_custom_waypoint_map = new WayPointMap;
    creat->GetAIInterface()->SetWaypointMap(creat->m_custom_waypoint_map);
  }
  
	bool AddItem( uint32 pEntry, Player* pPlayer, uint32 pCount = 1 )
	{
		PrintMessage( "Function call: AddItem( %u )", pEntry );
		if ( pPlayer == NULL || pEntry == 0 || pCount == 0 )
			return false;

		Item* ItemStack = pPlayer->GetItemInterface()->FindItemLessMax( pEntry, pCount, false );
		if ( ItemStack == NULL )
		{
			ItemPrototype* ItemProto = ItemPrototypeStorage.LookupEntry( pEntry );
			if ( ItemProto == NULL )
				return false;

			SlotResult Result = pPlayer->GetItemInterface()->FindFreeInventorySlot( ItemProto );
			if ( !Result.Result )
			{
				pPlayer->GetItemInterface()->BuildInventoryChangeError( NULL, NULL, INV_ERR_INVENTORY_FULL );
				return false;
			}
			else
			{
				Item* NewItem = objmgr.CreateItem( pEntry, pPlayer );
				if ( NewItem == NULL )
					return false;

				if ( ItemProto->MaxCount < pCount )
					pCount = ItemProto->MaxCount;

				NewItem->SetUInt32Value( ITEM_FIELD_STACK_COUNT, pCount );
				if ( pPlayer->GetItemInterface()->SafeAddItem( NewItem, Result.ContainerSlot, Result.Slot ) == ADD_ITEM_RESULT_ERROR )
				{
					NewItem->Destructor();
					NewItem = NULL;
					return false;
				};

				pPlayer->GetSession()->SendItemPushResult( NewItem, false, true, true, true, Result.ContainerSlot, Result.Slot, pCount );
				return true;
			};
		}
		else
		{
			ItemStack->SetCount( ItemStack->GetUInt32Value( ITEM_FIELD_STACK_COUNT ) + pCount );
			ItemStack->m_isDirty = true;
			pPlayer->GetSession()->SendItemPushResult( ItemStack, false, true, true, false, pPlayer->GetItemInterface()->GetBagSlotByGuid( ItemStack->GetGUID() ), 0xFFFFFFFF, pCount );
			return true;
		};

		return false;
	};
  
  void EventCreatureDelete(Creature* creat, uint32 time)  // Creature and time in ms
  {
	  sEventMgr.AddEvent(creat, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, time, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
  }

  void EventCastSpell(Unit* caster, Unit* target, uint32 spellid, uint32 time)
  {
	  sEventMgr.AddEvent(TO_UNIT(caster), &Unit::EventCastSpell, TO_UNIT(target), dbcSpell.LookupEntry(spellid), EVENT_UNK, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
  }

  void EventPlaySound(Creature* creat, uint32 id, uint32 time)
  {
	  sEventMgr.AddEvent(TO_OBJECT(creat), &Object::PlaySoundToSet, id, EVENT_UNK, time, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
  }

  void EventCreatureSay(Creature* creat, string say,  uint32 time)
  {
	  sEventMgr.AddEvent(TO_UNIT(creat), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, say.c_str(), EVENT_UNIT_CHAT_MSG, time, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
  }

  void EventCreatureYell(Creature* creat, string say,  uint32 time)
  {
	  sEventMgr.AddEvent(TO_UNIT(creat), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_YELL, (uint32)LANG_UNIVERSAL, say.c_str(), EVENT_UNIT_CHAT_MSG, time, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
  }

  Creature* getNpcQuester(Player* plr, uint32 npcid)
  {
	  if( plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL )
		  return NULL;
	  return plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), npcid);
  }

  GameObject* getGOQuester(Player* plr, uint32 goid)
  {
	  if( plr == NULL || plr->GetMapMgr() == NULL || plr->GetMapMgr()->GetInterface() == NULL )
		  return NULL;
	  return plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), goid);
  }
};

#define sEAS EasyFunctions::GetInstance()

#endif