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
#include "Item.h"
#include "Container.h"
#include "ItemPrototype.h"
#include "Player.h"
//
//-------------------------------------------------------------------//
ItemInterface::ItemInterface( Player* pPlayer )
{
	m_pOwner = pPlayer;

	for(uint8 i = 0; i < MAX_INVENTORY_SLOT; ++i )
	{
		m_pItems[i] = NULL;
	}
	for(uint8 i = 0; i < MAX_BUYBACK_SLOT; ++i )
	{
		m_pBuyBack[i] = NULL;
	}
}

//-------------------------------------------------------------------//
ItemInterface::~ItemInterface()
{
	for(uint32 i = 0; i < MAX_INVENTORY_SLOT; i++)
	{
		if( m_pItems[i] != NULL && m_pItems[i]->GetOwner() == m_pOwner )
			m_pItems[i]->Destructor();
	}

	for (uint32 i = 0; i < MAX_BUYBACK_SLOT; i++)
	{
		if (m_pBuyBack[i] != NULL)
			delete m_pBuyBack[i];
	}

	m_pOwner = NULL;
}

//-------------------------------------------------------------------// 100%
uint32 ItemInterface::m_CreateForPlayer(ByteBuffer *data)
{
	ASSERT(m_pOwner != NULL);
	uint32 count = 0;

	for(int i = 0; i < MAX_INVENTORY_SLOT; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->IsContainer())
			{
				count += (TO_CONTAINER(m_pItems[i]))->BuildCreateUpdateBlockForPlayer(data, m_pOwner);

				if(m_pItems[i]->GetProto())
				{
					for(uint32 e=0; e < m_pItems[i]->GetProto()->ContainerSlots; e++)
					{
						Item* pItem = (TO_CONTAINER(m_pItems[i]))->GetItem(e);
						if(pItem)
						{
							if(pItem->IsContainer())
							{
								count += (TO_CONTAINER(pItem))->BuildCreateUpdateBlockForPlayer( data, m_pOwner );
							}
							else
							{
								count += pItem->BuildCreateUpdateBlockForPlayer( data, m_pOwner );
							}
						}
					}
				}
			}
			else
			{
				count += m_pItems[i]->BuildCreateUpdateBlockForPlayer(data, m_pOwner);
			}
		}
	}
	return count;
}

//-------------------------------------------------------------------// 100%
void ItemInterface::m_DestroyForPlayer()
{
	ASSERT(m_pOwner != NULL);

	for(int i = 0; i < MAX_INVENTORY_SLOT; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->IsContainer())
			{
				if(m_pItems[i]->GetProto())
				{
					for(uint32 e=0; e < m_pItems[i]->GetProto()->ContainerSlots; e++)
					{
						Item* pItem = (TO_CONTAINER(m_pItems[i]))->GetItem(e);
						if(pItem)
						{
							if(pItem->IsContainer())
							{
								(TO_CONTAINER(pItem))->DestroyForPlayer( m_pOwner );
							}
							else
							{
								pItem->DestroyForPlayer( m_pOwner );
							}
						}
					}
				}
				(TO_CONTAINER(m_pItems[i]))->DestroyForPlayer( m_pOwner );
			}
			else
			{
				m_pItems[i]->DestroyForPlayer( m_pOwner );
			}
		}
	}
}

//-------------------------------------------------------------------//
//Description: Creates and adds a item that can be manipulated after		  
//-------------------------------------------------------------------//
Item* ItemInterface::SafeAddItem(uint32 ItemId, uint8 ContainerSlot, uint8 slot)
{
	ItemPrototype *pProto = ItemPrototypeStorage.LookupEntry(ItemId);
	if(!pProto) { return NULL; }

	if(pProto->InventoryType == INVTYPE_BAG)
	{
		Container* pItem(new Container(HIGHGUID_TYPE_CONTAINER,objmgr.GenerateLowGuid(HIGHGUID_TYPE_CONTAINER)));
		pItem->Create( ItemId, m_pOwner);
		if(m_AddItem(pItem, ContainerSlot, slot))
		{
			return TO_ITEM(pItem);
		}
		else
		{
			pItem->Destructor();
		}
	}
	else
	{
		Item* pItem(new Item(HIGHGUID_TYPE_ITEM,objmgr.GenerateLowGuid(HIGHGUID_TYPE_ITEM)));
		pItem->Create( ItemId, m_pOwner);
		if(m_AddItem(pItem, ContainerSlot, slot))
		{
			return pItem;
		}
		else
		{
			pItem->Destructor();
		}
	}
	return NULL;
}

//-------------------------------------------------------------------//
//Description: Creates and adds a item that can be manipulated after		  
//-------------------------------------------------------------------//
AddItemResult ItemInterface::SafeAddItem( Item* pItem, uint8 ContainerSlot, uint8 slot)
{
	return m_AddItem( pItem, ContainerSlot, slot );
}

//-------------------------------------------------------------------//
//Description: adds items to player inventory, this includes all types of slots.		  
//-------------------------------------------------------------------//
AddItemResult ItemInterface::m_AddItem( Item* item, uint8 ContainerSlot, uint8 slot)
{
	ASSERT( slot < MAX_INVENTORY_SLOT || slot == INVENTORY_SLOT_NOT_SET );
	ASSERT( ContainerSlot < MAX_INVENTORY_SLOT || ContainerSlot == INVENTORY_SLOT_NOT_SET );
	if( item == NULL || !item->GetProto() )
		return ADD_ITEM_RESULT_ERROR;

	item->m_isDirty = true;

	// doublechecking
	uint32 i, j, k;
	Item* tempitem;
	for(i = 0; i < MAX_INVENTORY_SLOT; ++i)
	{
		tempitem = m_pItems[i];
		if( tempitem != NULL )
		{
			if( tempitem == item )
			{
#ifdef WIN32
				//OutputCrashLogLine("item duplication, callstack:");
				//printf("item duplication, callstack: ");
				//CStackWalker ws;
				//ws.ShowCallstack();
#endif
				return ADD_ITEM_RESULT_DUPLICATED;
			}

			if( tempitem->IsContainer() )
			{
				k = tempitem->GetProto()->ContainerSlots;
				for(j = 0; j < k; ++j)
				{
					if( TO_CONTAINER(tempitem)->GetItem( j ) == item )
					{
#ifdef WIN32
						//OutputCrashLogLine("item duplication in container, callstack:");
						//printf("item duplication in container, callstack: ");
						//CStackWalker ws;
						//ws.ShowCallstack();
#endif
						return ADD_ITEM_RESULT_DUPLICATED;
					}
				}
			}
		}
	}

	if(item->GetProto())
	{
		//case 1, item is from backpack container
		if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
		{
			//ASSERT(m_pItems[slot] == NULL);
			if(GetInventoryItem(slot) != NULL /*|| (slot == EQUIPMENT_SLOT_OFFHAND && !m_pOwner->HasSkillLine(118))*/)
			{
				//sLog.outError("bugged inventory: %u %u", m_pOwner->GetName(), item->GetGUID());
				SlotResult result = this->FindFreeInventorySlot(item->GetProto());
				
				// send message to player
				sChatHandler.BlueSystemMessageToPlr(m_pOwner, "A duplicated item, `%s` was found in your inventory. We've attempted to add it to a free slot in your inventory, if there is none this will fail. It will be attempted again the next time you log on.",
					item->GetProto()->Name1);
				if(result.Result == true)
				{
					// Found a new slot for that item.
					slot = result.Slot;
					ContainerSlot = result.ContainerSlot;
				}
				else
				{
					// don't leak memory!
					/*if(item->IsContainer())
						delete TO_CONTAINER(item);
					else
						delete item;*/

					return ADD_ITEM_RESULT_ERROR;
				}
			}

			if(!GetInventoryItem(slot)) //slot is free, add item.
			{
				item->SetOwner( m_pOwner );
				item->SetUInt64Value(ITEM_FIELD_CONTAINED, m_pOwner->GetGUID());
				m_pItems[(int)slot] = item;

				if (item->GetProto()->Bonding == ITEM_BIND_ON_PICKUP) 
					item->SoulBind();

				if (item->GetProto()->BagFamily & ITEM_TYPE_CURRENCY)
					m_pOwner->UpdateKnownCurrencies(item->GetEntry(), true);

				if( m_pOwner->IsInWorld() && !item->IsInWorld())
				{
					//item->AddToWorld();
					item->PushToWorld(m_pOwner->GetMapMgr());
					ByteBuffer buf(2500);
					uint32 count = item->BuildCreateUpdateBlockForPlayer( &buf, m_pOwner );
					m_pOwner->PushCreationData(&buf, count);
				}
				m_pOwner->SetUInt64Value(PLAYER_FIELD_INV_SLOT_HEAD + (slot*2), item->GetGUID());
			}
			else
			{
				return ADD_ITEM_RESULT_ERROR;
			}
		}
		else //case 2: item is from a bag container
		{
			if( GetInventoryItem(ContainerSlot) && GetInventoryItem(ContainerSlot)->IsContainer() &&
				slot < (int32)GetInventoryItem(ContainerSlot)->GetProto()->ContainerSlots) //container exists
			{
				bool result = TO_CONTAINER(m_pItems[(int)ContainerSlot])->AddItem(slot, item);
				if( !result )
				{
					return ADD_ITEM_RESULT_ERROR;
				}
			}
			else
			{
				return ADD_ITEM_RESULT_ERROR;
			}
		}
	}
	else
	{
		return ADD_ITEM_RESULT_ERROR;
	}

	if ( slot < EQUIPMENT_SLOT_END && ContainerSlot == INVENTORY_SLOT_NOT_SET )
	{
		int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * PLAYER_VISIBLE_ITEM_LENGTH);
		if( VisibleBase > PLAYER_VISIBLE_ITEM_19_ENTRYID )
		{
			printf("Slot warning: slot: %d\n", slot);
			OutputCrashLogLine("Slot warning: slot: %d\n", slot);
		}
		else
		{
			m_pOwner->SetUInt32Value( VisibleBase, item->GetUInt32Value( OBJECT_FIELD_ENTRY ) );
			m_pOwner->SetUInt32Value( VisibleBase + 1, item->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_1 ) );
		}
	}

	if( m_pOwner->IsInWorld() && slot < INVENTORY_SLOT_BAG_END && ContainerSlot == INVENTORY_SLOT_NOT_SET )
	{
		m_pOwner->ApplyItemMods( item, slot, true );
	}

	if( ContainerSlot == INVENTORY_SLOT_NOT_SET && slot == EQUIPMENT_SLOT_OFFHAND && item->GetProto()->Class == ITEM_CLASS_WEAPON )
		m_pOwner->SetDuelWield(true);

	if( this->m_pOwner )
		m_pOwner->GetAchievementInterface()->HandleAchievementCriteriaOwnItem( item->GetUInt32Value(OBJECT_FIELD_ENTRY), item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1 );

	return ADD_ITEM_RESULT_OK;
}

//-------------------------------------------------------------------//
//Description: Checks if the slot is a Bag slot
//-------------------------------------------------------------------//
bool ItemInterface::IsBagSlot(uint8 slot)
{
	if((slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END) || (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END))
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------//
//Description: removes the item safely and returns it back for usage
//-------------------------------------------------------------------//
Item* ItemInterface::SafeRemoveAndRetreiveItemFromSlot(uint8 ContainerSlot, uint8 slot, bool destroy)
{
	ASSERT(slot < MAX_INVENTORY_SLOT || slot == INVENTORY_SLOT_NOT_SET);
	ASSERT(ContainerSlot < MAX_INVENTORY_SLOT || ContainerSlot == INVENTORY_SLOT_NOT_SET);
	Item* pItem = NULL;

	if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
	{
		pItem = GetInventoryItem(ContainerSlot,slot);

		if (pItem == NULL) { return NULL; }

		if(pItem->GetProto()->ContainerSlots > 0 && pItem->GetTypeId() == TYPEID_CONTAINER && TO_CONTAINER(pItem)->HasItems())
		{
			/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
			pItem->DeleteFromDB();
			return NULL;
		}

		m_pItems[(int)slot] = NULL;
		if(pItem->GetOwner() == m_pOwner)
		{
			pItem->m_isDirty = true;

			m_pOwner->SetUInt64Value(PLAYER_FIELD_INV_SLOT_HEAD  + (slot*2), 0 );

			if ( slot < EQUIPMENT_SLOT_END )
			{
				m_pOwner->ApplyItemMods( pItem, slot, false );
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 1, 0 );
			}
			else if ( slot < INVENTORY_SLOT_BAG_END )
				m_pOwner->ApplyItemMods( pItem, slot, false );

			if(slot == EQUIPMENT_SLOT_OFFHAND)
				m_pOwner->SetDuelWield(false);

			if(slot >= INVENTORY_CURRENCY_START && slot < INVENTORY_CURRENCY_END)
				m_pOwner->UpdateKnownCurrencies(pItem->GetEntry(), false);

			if(destroy)
			{
				if (pItem->IsInWorld())
				{
					pItem->RemoveFromWorld();
				}
				pItem->DeleteFromDB();
			}
		}
		else
			pItem = NULL;
	}
	else
	{
		Item* pContainer = GetInventoryItem(ContainerSlot);
		if(pContainer && pContainer->IsContainer())
		{
			pItem = TO_CONTAINER(pContainer)->SafeRemoveAndRetreiveItemFromSlot(slot, destroy);
			if (pItem == NULL) { return NULL; }
		}
	}

	return pItem;
}

//-------------------------------------------------------------------//
//Description: removes the item safely by guid and returns it back for usage, supports full inventory
//-------------------------------------------------------------------//
Item* ItemInterface::SafeRemoveAndRetreiveItemByGuid(uint64 guid, bool destroy)
{
	uint8 i = 0;

	for(i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
		else
		{
			if(item && item->IsContainer() && item->GetProto())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2 && item2->GetGUID() == guid)
					{
						return TO_CONTAINER(item)->SafeRemoveAndRetreiveItemFromSlot(j, destroy);
					}
				}
			}
		}
	}

	for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
		else
		{
			if(item && item->IsContainer() && item->GetProto())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2 && item2->GetGUID() == guid)
					{
						return TO_CONTAINER(item)->SafeRemoveAndRetreiveItemFromSlot(j, destroy);
					}
				}
			}
		}
	}

	return NULL;
}

Item* ItemInterface::SafeRemoveAndRetreiveItemByGuidRemoveStats(uint64 guid, bool destroy)
{
	int8 i = 0;

	for(i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			m_pOwner->ApplyItemMods(item, i, false, false);
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			m_pOwner->ApplyItemMods(item, i, false, false);
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
		else
		{
			if(item && item->IsContainer() && item->GetProto())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2 && item2->GetGUID() == guid)
					{
						return TO_CONTAINER(item)->SafeRemoveAndRetreiveItemFromSlot(j, destroy);
					}
				}
			}
		}
	}

	for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
	}

	for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, i, destroy);
		}
		else
		{
			if(item && item->IsContainer() && item->GetProto())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2 && item2->GetGUID() == guid)
					{
						return TO_CONTAINER(item)->SafeRemoveAndRetreiveItemFromSlot(j, destroy);
					}
				}
			}
		}
	}

	return NULL;
}

//-------------------------------------------------------------------//
//Description: completely removes item from player
//Result: true if item removal was succefull
//-------------------------------------------------------------------//
bool ItemInterface::SafeFullRemoveItemFromSlot(uint8 ContainerSlot, uint8 slot)
{
	ASSERT(slot < MAX_INVENTORY_SLOT || slot == INVENTORY_SLOT_NOT_SET);
	ASSERT(ContainerSlot < MAX_INVENTORY_SLOT || ContainerSlot == INVENTORY_SLOT_NOT_SET);

	if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
	{
		Item* pItem = GetInventoryItem(slot);

		if (pItem == NULL) { return false; }

		if(pItem->GetProto()->ContainerSlots > 0 && pItem->GetTypeId() == TYPEID_CONTAINER && TO_CONTAINER(pItem)->HasItems())
		{
			/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
			pItem->DeleteFromDB();
			return false;
		}

		m_pItems[(int)slot] = NULL;
		// hacky crashfix
		if( pItem->GetOwner() == m_pOwner )
		{
			pItem->m_isDirty = true;

			m_pOwner->SetUInt64Value(PLAYER_FIELD_INV_SLOT_HEAD  + (slot*2), 0 );

			if ( slot < EQUIPMENT_SLOT_END )
			{
				m_pOwner->ApplyItemMods(pItem, slot, false );
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 1, 0 );
			}
			else if( slot < INVENTORY_SLOT_BAG_END )
				m_pOwner->ApplyItemMods(pItem, slot, false ); //watch containers that give attackspeed and stuff ;)

			if(slot == EQUIPMENT_SLOT_OFFHAND)
				m_pOwner->SetDuelWield(false);

			if(slot >= INVENTORY_CURRENCY_START && slot < INVENTORY_CURRENCY_END)
				m_pOwner->UpdateKnownCurrencies(pItem->GetEntry(), false);

			if (pItem->IsInWorld())
			{
				pItem->RemoveFromWorld();
			}

			pItem->DeleteFromDB();
			pItem->Destructor();
		}
	}
	else
	{
		Item* pContainer = GetInventoryItem(ContainerSlot);
		if(pContainer && pContainer->IsContainer())
		{
			TO_CONTAINER(pContainer)->SafeFullRemoveItemFromSlot(slot);
		}
	}
	return true;
}

//-------------------------------------------------------------------//
//Description: removes the item safely by guid, supports full inventory
//-------------------------------------------------------------------//
bool ItemInterface::SafeFullRemoveItemByGuid(uint64 guid)
{
	uint8 i = 0;

	for(i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
		else
		{
			if(item && item->IsContainer() && item->GetProto())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2 && item2->GetGUID() == guid)
					{
						return TO_CONTAINER(item)->SafeFullRemoveItemFromSlot(j);
					}
				}
			}
		}
	}

	for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
	}

	for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->GetGUID() == guid)
		{
			return this->SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
		}
		else
		{
			if(item && item->IsContainer() && item->GetProto())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2 && item2->GetGUID() == guid)
					{
						return TO_CONTAINER(item)->SafeFullRemoveItemFromSlot(j);
					}
				}
			}
		}
	}
	return false;
}

//-------------------------------------------------------------------//
//Description: Gets a item from Inventory
//-------------------------------------------------------------------//
Item* ItemInterface::GetInventoryItem(uint8 slot)
{
	if(slot > MAX_INVENTORY_SLOT)
		return NULL;

	return m_pItems[(int)slot];
}

//-------------------------------------------------------------------//
//Description: Gets a Item from inventory or container
//-------------------------------------------------------------------//
Item* ItemInterface::GetInventoryItem(uint8 ContainerSlot, uint8 slot)
{
	if(ContainerSlot == INVENTORY_SLOT_NOT_SET)
	{
		if(slot > MAX_INVENTORY_SLOT)
			return NULL;

		return m_pItems[(int)slot];
	}
	else
	{
		if(IsBagSlot(ContainerSlot))
		{
			if(m_pItems[(int)ContainerSlot])
			{
				return TO_CONTAINER(m_pItems[(int)ContainerSlot])->GetItem(slot);
			}
		}
	}
	return NULL;
}

//-------------------------------------------------------------------//
//Description: checks for stacks that didnt reached max capacity
//-------------------------------------------------------------------//
Item* ItemInterface::FindItemLessMax(uint32 itemid, uint32 cnt, bool IncBank)
{
	uint32 i = 0;
	for(i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if((item->GetEntry() == itemid && item->wrapped_item_id==0) && (item->GetProto()->MaxCount >= (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + cnt)))
			{
				return item; 
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->IsContainer())
		{
			  for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2)
					{
						if((item2->GetProto()->ItemId == itemid && item2->wrapped_item_id==0) && (item2->GetProto()->MaxCount >= (item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + cnt)))
						{
							return item2;
						}
					}
				}
			
		}
	}

	if(IncBank)
	{
		for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++)
		{
			Item* item = GetInventoryItem(i);
			if (item)
			{
				if((item->GetEntry() == itemid && item->wrapped_item_id==0) && (item->GetProto()->MaxCount >= (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + cnt)))
				{
					return item; 
				}
			}
		}

		for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
		{
			Item* item = GetInventoryItem(i);
			if(item && item->IsContainer())
			{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2)
					{
						if((item2->GetProto()->ItemId == itemid && item->wrapped_item_id==0) && (item2->GetProto()->MaxCount >= (item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + cnt)))
						{
							return item2;
						}
					}
				}
			}
		}
	}

	return NULL;
}

//-------------------------------------------------------------------//
//Description: finds item ammount on inventory, banks not included
//-------------------------------------------------------------------//
uint32 ItemInterface::GetItemCount(uint32 itemid, bool IncBank)
{
	uint32 cnt = 0;
	uint32 i = 0;
	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item)
		{
			if(item->GetEntry() == itemid && item->wrapped_item_id==0)
			{
				cnt += item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1; 
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->IsContainer())
		{
				for (uint32 j =0; j < item->GetProto()->ContainerSlots;j++)
				{
					Item* item2 = TO_CONTAINER(item)->GetItem(j);
					if (item2)
					{
						if (item2->GetEntry() == itemid && item->wrapped_item_id==0)
						{
							cnt += item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1; 
						}
					}
				}
			
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item)
		{
			if(item->GetProto()->ItemId == itemid && item->wrapped_item_id==0)
			{
				cnt += item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1;
			}
		}
	}

	for(i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++)
	{
		Item* item = GetInventoryItem(i);

		if (item)
		{
			if(item->GetProto()->ItemId == itemid && item->wrapped_item_id==0)
			{
				cnt += item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1;
			}
		}
	}

	if(IncBank)
	{
		for(i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; i++)
		{
			Item* item = GetInventoryItem(i);
			if (item)
			{
				if(item->GetProto()->ItemId == itemid && item->wrapped_item_id==0)
				{
					cnt += item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1;
				}
			}
		}

		for(i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++)
		{
			Item* item = GetInventoryItem(i);
			if(item)
			{
				if(item->IsContainer())
				{
					for (uint32 j =0; j < item->GetProto()->ContainerSlots; j++)
					{
						Item* item2 = TO_CONTAINER(item)->GetItem(j);
						if (item2)
						{
							if(item2->GetProto()->ItemId == itemid && item->wrapped_item_id==0)
							{
								cnt += item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1;
							}
						}
					}
				}
			}
		}
	}
	return cnt;
}

//-------------------------------------------------------------------//
//Description: Removes a ammount of items from inventory
//-------------------------------------------------------------------//
uint32 ItemInterface::RemoveItemAmt(uint32 id, uint32 amt)
{
	//this code returns shit return value is fucked
	if (GetItemCount(id) < amt)
	{
		return 0;
	}
	uint32 i;

	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetEntry() == id && item->wrapped_item_id==0)
			{
				if(item->GetProto()->ContainerSlots > 0 && item->IsContainer() && TO_CONTAINER(item)->HasItems())
				{
					/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
					item->DeleteFromDB();
					continue;
				}

				if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
				{
					item->SetCount(item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
				}
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->IsContainer())
		{
			for (uint32 j =0; j < item->GetProto()->ContainerSlots;j++)
			{
				Item* item2 = TO_CONTAINER(item)->GetItem(j);
				if (item2)
				{
					if (item2->GetProto()->ItemId == id && item->wrapped_item_id==0)
					{
						if (item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
						{
							item2->SetCount(item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
							item2->m_isDirty = true;
							return amt;
						}
						else if (item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
						{
							bool result = SafeFullRemoveItemFromSlot(i, j);
							if(result)
							{
								return amt;
							}
							else
							{
								return 0;
							}
						}
						else
						{
							amt -= item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
							SafeFullRemoveItemFromSlot(i, j);
					  
						}
					}
				}
			}
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id==0)
			{
				if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
				{
					item->SetCount(item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
				}
			}
		}
	}

	for(i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id==0)
			{
				if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
				{
					item->SetCount(item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
				}
			}
		}
	}
	return 0;
}

uint32 ItemInterface::RemoveItemAmt_ProtectPointer(uint32 id, uint32 amt, Item** pointer)
{
	//this code returns shit return value is fucked
	if (GetItemCount(id) < amt)
	{
		return 0;
	}
	uint32 i;

	for(i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetEntry() == id && item->wrapped_item_id==0)
			{
				if(item->GetProto()->ContainerSlots > 0 && item->IsContainer() && TO_CONTAINER(item)->HasItems())
				{
					/* sounds weird? no. this will trigger a callstack display due to my other debug code. */
					item->DeleteFromDB();
					continue;
				}

				if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
				{
					item->SetCount(item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;

					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);

					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
				}
			}
		}
	}

	for(i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(item && item->IsContainer())
		{
			for (uint32 j =0; j < item->GetProto()->ContainerSlots;j++)
			{
				Item* item2 = TO_CONTAINER(item)->GetItem(j);
				if (item2)
				{
					if (item2->GetProto()->ItemId == id && item->wrapped_item_id==0)
					{
						if (item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
						{
							item2->SetCount(item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
							item2->m_isDirty = true;
							return amt;
						}
						else if (item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
						{
							bool result = SafeFullRemoveItemFromSlot(i, j);
							if( pointer != NULL && *pointer != NULL && *pointer == item2 )
								*pointer = NULL;

							if(result)
							{
								return amt;
							}
							else
							{
								return 0;
							}
						}
						else
						{
							amt -= item2->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
							SafeFullRemoveItemFromSlot(i, j);

							if( pointer != NULL && *pointer != NULL && *pointer == item2 )
								*pointer = NULL;
						}
					}
				}
			}
		}
	}

	for(i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id==0)
			{
				if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
				{
					item->SetCount(item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;

					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);

					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
				}
			}
		}
	}

	for(i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if (item)
		{
			if(item->GetProto()->ItemId == id && item->wrapped_item_id==0)
			{
				if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > amt)
				{
					item->SetCount(item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) - amt);
					item->m_isDirty = true;
					return amt;
				}
				else if (item->GetUInt32Value(ITEM_FIELD_STACK_COUNT)== amt)
				{
					bool result = SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);
					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;

					if(result)
					{
						return amt;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					amt -= item->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, i);

					if( pointer != NULL && *pointer != NULL && *pointer == item )
						*pointer = NULL;
				}
			}
		}
	}
	return 0;
}

void ItemInterface::RemoveAllConjured()
{
	for(uint32 x = INVENTORY_SLOT_BAG_START; x < INVENTORY_SLOT_ITEM_END; ++x)
	{
		if (m_pItems[x]!= NULL)
		{
			if(IsBagSlot(x) && m_pItems[x]->IsContainer())
			{
				Container* bag = TO_CONTAINER(m_pItems[x]);
 
				for(uint32 i = 0; i < bag->GetProto()->ContainerSlots; i++)
				{
					if (bag->GetItem(i) != NULL && bag->GetItem(i)->GetProto() && (bag->GetItem(i)->GetProto()->Flags)& 2)
						bag->SafeFullRemoveItemFromSlot(i);
				}
			}
			else
			{
				if(m_pItems[x]->GetProto() && (m_pItems[x]->GetProto()->Flags)& 2 )
					SafeFullRemoveItemFromSlot(INVENTORY_SLOT_NOT_SET, x);
			}
		}
	}
}


//-------------------------------------------------------------------//
//Description: Gets slot number by itemid, banks not included
//-------------------------------------------------------------------//
uint8 ItemInterface::GetInventorySlotById(uint32 ID)
{
	for(uint32 i=0;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetProto()->ItemId == ID)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetProto()->ItemId == ID)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_CURRENCY_START; i<INVENTORY_CURRENCY_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetProto()->ItemId == ID)
			{
				return i;
			}
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

//-------------------------------------------------------------------//
//Description: Gets slot number by item guid, banks not included
//-------------------------------------------------------------------//
uint8 ItemInterface::GetInventorySlotByGuid(uint64 guid)
{
	for(uint32 i=EQUIPMENT_SLOT_START ;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_CURRENCY_START; i<INVENTORY_CURRENCY_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	return ITEM_NO_SLOT_AVAILABLE; //was changed from 0 cuz 0 is the slot for head
}

uint8 ItemInterface::GetBagSlotByGuid(uint64 guid)
{
	for(uint32 i=EQUIPMENT_SLOT_START ;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_CURRENCY_START; i<INVENTORY_CURRENCY_END; i++)
	{
		if(m_pItems[i])
		{
			if(m_pItems[i]->GetGUID() == guid)
			{
				return i;
			}
		}
	}

	for(uint32 i=INVENTORY_SLOT_BAG_START; i<INVENTORY_SLOT_BAG_END; ++i)
	{
		if(m_pItems[i]&&m_pItems[i]->GetTypeId()==TYPEID_CONTAINER)
		{
			for(uint32 j = 0; j < m_pItems[i]->GetProto()->ContainerSlots; ++j)
			{
				Item* inneritem = TO_CONTAINER(m_pItems[i])->GetItem(j);
				if(inneritem && inneritem->GetGUID()==guid)
					return i;
			}
		}
	}

	return ITEM_NO_SLOT_AVAILABLE; //was changed from 0 cuz 0 is the slot for head
}


//-------------------------------------------------------------------//
//Description: Adds a Item to a free slot
//-------------------------------------------------------------------//
AddItemResult ItemInterface::AddItemToFreeSlot(Item* item)
{
	if( item == NULL )
		return ADD_ITEM_RESULT_ERROR;

	if( item->GetProto() == NULL )
		return ADD_ITEM_RESULT_ERROR;

	uint32 i = 0;
	bool result2;
	AddItemResult result3;

	//detect special bag item
	if( item->GetProto()->BagFamily )
	{
		if( item->GetProto()->BagFamily & ITEM_TYPE_KEYRING )
		{
			for(i=INVENTORY_KEYRING_START; i<INVENTORY_KEYRING_END; i++ )
			{
				if(m_pItems[i] == NULL)
				{
					result3 = SafeAddItem( item, INVENTORY_SLOT_NOT_SET, i );
					if( result3 )
					{
						result.ContainerSlot = INVENTORY_SLOT_NOT_SET;
						result.Slot = i;
						result.Result = true;
						return ADD_ITEM_RESULT_OK;
					}
				}
			}
		}
		else if( item->GetProto()->BagFamily & ITEM_TYPE_CURRENCY )
		{
			for( i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++ )
			{
				if(m_pItems[i] == NULL)
				{
					result3 = SafeAddItem( item, INVENTORY_SLOT_NOT_SET, i );
					if( result3 )
					{
						result.ContainerSlot = INVENTORY_SLOT_NOT_SET;
						result.Slot = i;
						result.Result = true;
						return ADD_ITEM_RESULT_OK;
					}
				}
			}
		}
		else
		{
			for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
			{
				if(m_pItems[i])
				{
					if (m_pItems[i]->GetProto()->BagFamily & item->GetProto()->BagFamily)
					{
						if(m_pItems[i]->IsContainer())
						{
							uint32 r_slot;
							result2 = TO_CONTAINER(m_pItems[i])->AddItemToFreeSlot(item, &r_slot);
							if (result2) {
								result.ContainerSlot = i;
								result.Slot = r_slot;
								result.Result = true;
								return ADD_ITEM_RESULT_OK;
							}
						}
					}
				}
			}
		}
	}

	//INVENTORY
	for(i=INVENTORY_SLOT_ITEM_START;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i] == NULL)
		{
			result3 = SafeAddItem(item, INVENTORY_SLOT_NOT_SET, i);
			if(result3) {
				result.ContainerSlot = INVENTORY_SLOT_NOT_SET;
				result.Slot = i;
				result.Result = true;
				return ADD_ITEM_RESULT_OK;
			}
		}
	}

	//INVENTORY BAGS
	for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
	{
		if(m_pItems[i] != NULL && m_pItems[i]->GetProto()->BagFamily == 0 && m_pItems[i]->IsContainer()) //special bags ignored
		{
			for (uint32 j =0; j < m_pItems[i]->GetProto()->ContainerSlots;j++)
			{
				Item* item2 = TO_CONTAINER(m_pItems[i])->GetItem(j);
				if (item2 == NULL)
				{
					result3 = SafeAddItem(item, i, j);
					if(result3) {
						result.ContainerSlot = i;
						result.Slot = j;
						result.Result = true;
						return ADD_ITEM_RESULT_OK;
					}
				}
			}
		}
	}
	return ADD_ITEM_RESULT_ERROR;
}

//-------------------------------------------------------------------//
//Description: Calculates inventory free slots, bag inventory slots not included
//-------------------------------------------------------------------//
uint32 ItemInterface::CalculateFreeSlots(ItemPrototype *proto)
{
	uint32 count = 0;
	uint32 i;

	if(proto)
	{
		if(proto->BagFamily)
		{
			if(proto->BagFamily & ITEM_TYPE_KEYRING)
			{
				for(uint32 i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++)
				{
					if(m_pItems[i] == NULL)
					{
						count++;
					}
				}
			}
			else if(proto->BagFamily & ITEM_TYPE_CURRENCY)
			{
				for(uint32 i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++)
				{
					if(m_pItems[i] == NULL)
					{
						count++;
					}
				}
			}
			else
			{
				for(uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END;i++)
				{
					if(m_pItems[i] && m_pItems[i]->IsContainer())
					{
						if (m_pItems[i]->GetProto()->BagFamily & proto->BagFamily)
						{
							uint8 slot = TO_CONTAINER(m_pItems[i])->FindFreeSlot();
							if(slot != ITEM_NO_SLOT_AVAILABLE) 
							{
								count++;
							}
						}
					}
				}
			}
		}
	}

	for(i=INVENTORY_SLOT_ITEM_START;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i] == NULL)
		{
			count++;
		}
	}

	for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
	{
		if(m_pItems[i] != NULL )
		{
			if(m_pItems[i]->IsContainer() && !m_pItems[i]->GetProto()->BagFamily)
			{

				for (uint32 j =0; j < m_pItems[i]->GetProto()->ContainerSlots;j++)
				{
					Item* item2 = TO_CONTAINER(m_pItems[i])->GetItem(j);
					if (item2 == NULL)
					{
						count++;
					}
				}
			}
		}
	}
	return count;
}

//-------------------------------------------------------------------//
//Description: finds a free slot on the backpack
//-------------------------------------------------------------------//
uint8 ItemInterface::FindFreeBackPackSlot()
{
	//search for backpack slots
	for(uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
	{
		Item* item = GetInventoryItem(i);
		if(!item) 
		{ 
			return i; 
		}
	}

	return ITEM_NO_SLOT_AVAILABLE; //no slots available
}

//-------------------------------------------------------------------//
//Description: converts bank bags slot ids into player bank byte slots(0-5)
//-------------------------------------------------------------------//
uint8 ItemInterface::GetInternalBankSlotFromPlayer(uint8 islot)
{
	switch(islot)
	{
	case BANK_SLOT_BAG_1:
		{
			return 1;
		}
	case BANK_SLOT_BAG_2:
		{
			return 2;
		}
	case BANK_SLOT_BAG_3:
		{
			return 3;
		}
	case BANK_SLOT_BAG_4:
		{
			return 4;
		}
	case BANK_SLOT_BAG_5:
		{
			return 5;
		}
	case BANK_SLOT_BAG_6:
		{
			return 6;
		}
	case BANK_SLOT_BAG_7:
		{
			return 7;
		}
	default:
		return 8;
	}
}

//-------------------------------------------------------------------//
//Description: checks if the item can be equiped on a specific slot
//-------------------------------------------------------------------//
uint8 ItemInterface::CanEquipItemInSlot(uint8 DstInvSlot, uint8 slot, ItemPrototype* proto, bool ignore_combat /* = false */, bool skip_2h_check /* = false */)
{
	uint32 type=proto->InventoryType;
	
	if(slot >= INVENTORY_SLOT_BAG_START && slot < INVENTORY_SLOT_BAG_END && DstInvSlot == ITEM_NO_SLOT_AVAILABLE)
		if(proto->ContainerSlots == 0)
			return INV_ERR_ITEMS_CANT_BE_SWAPPED;

	if((slot < INVENTORY_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET) || (slot >= BANK_SLOT_BAG_START && slot < BANK_SLOT_BAG_END && DstInvSlot == INVENTORY_SLOT_NOT_SET))
	{
		if (!ignore_combat && m_pOwner->CombatStatus.IsInCombat() && (slot < EQUIPMENT_SLOT_MAINHAND || slot > EQUIPMENT_SLOT_RANGED))
			return INV_ERR_CANT_DO_IN_COMBAT;

		if(IsEquipped(proto->ItemId))
		{
			if( proto->Unique ) //how did we end up here? We shouldn't have 2 of these...
				return INV_ERR_CANT_CARRY_MORE_OF_THIS;

			if( proto->Flags & ITEM_FLAG_UNIQUE_EQUIP)
				return INV_ERR_CANT_CARRY_MORE_OF_THIS;
		}
		
		if( proto->AllowableRace && !(proto->AllowableRace & m_pOwner->getRaceMask()))
			return INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM;

		// Check to see if we have the correct class
		if( proto->AllowableClass && !(proto->AllowableClass & m_pOwner->getClassMask()))
			return INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM2;

		// Check to see if we have the reqs for that reputation
		if(proto->RequiredFaction)
		{
			Standing current_standing = Player::GetReputationRankFromStanding(m_pOwner->GetStanding(proto->RequiredFaction));
			if(current_standing < (Standing)proto->RequiredFactionStanding)	   // Not enough rep rankage..
				return INV_ERR_ITEM_REPUTATION_NOT_ENOUGH;
		}

		// Check to see if we have the correct level.
		if(proto->RequiredLevel>m_pOwner->GetUInt32Value(UNIT_FIELD_LEVEL))
			return INV_ERR_YOU_MUST_REACH_LEVEL_N;

		if(proto->Class == ITEM_CLASS_ARMOR)
		{

			if(!(m_pOwner->GetArmorProficiency()&(((uint32)(1))<<proto->SubClass)))
				return INV_ERR_NO_REQUIRED_PROFICIENCY;

		}
		else if(proto->Class == ITEM_CLASS_WEAPON)
		{
			if(!(m_pOwner->GetWeaponProficiency()&(((uint32)(1))<<proto->SubClass)))
				return INV_ERR_NO_REQUIRED_PROFICIENCY;
		}

		if(proto->RequiredSkill)
			if (proto->RequiredSkillRank > m_pOwner->_GetSkillLineCurrent(proto->RequiredSkill,true))
				return INV_ERR_SKILL_ISNT_HIGH_ENOUGH;

		if(proto->RequiredSkillSubRank)
			if (!m_pOwner->HasSpell(proto->RequiredSkillSubRank))
				return INV_ERR_NO_REQUIRED_PROFICIENCY;

				// You are dead !
		if(m_pOwner->getDeathState() != ALIVE)
			return INV_ERR_YOU_ARE_DEAD;
	}

	switch(slot)
	{
	case EQUIPMENT_SLOT_HEAD:
		{
			if(type == INVTYPE_HEAD)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_NECK:
		{
			if(type == INVTYPE_NECK)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_SHOULDERS:
		{
			if(type == INVTYPE_SHOULDERS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_BODY:
		{
			if(type == INVTYPE_BODY)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_CHEST:
		{
			if(type == INVTYPE_CHEST || type == INVTYPE_ROBE)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_WAIST:
		{
			if(type == INVTYPE_WAIST)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_LEGS:
		{
			if(type == INVTYPE_LEGS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_FEET:
		{
			if(type == INVTYPE_FEET)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_WRISTS:
		{
			if(type == INVTYPE_WRISTS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_HANDS:
		{
			if(type == INVTYPE_HANDS)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_FINGER1:
	case EQUIPMENT_SLOT_FINGER2:
		{
			if(type == INVTYPE_FINGER)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_TRINKET1:
	case EQUIPMENT_SLOT_TRINKET2:
		{
			if(type == INVTYPE_TRINKET)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_BACK:
		{
			if(type == INVTYPE_CLOAK)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_MAINHAND:
		{
			if(type == INVTYPE_WEAPON || type == INVTYPE_WEAPONMAINHAND ||
				(type == INVTYPE_2HWEAPON && (!GetInventoryItem(EQUIPMENT_SLOT_OFFHAND) || skip_2h_check || m_pOwner->titanGrip)))
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_OFFHAND:
		{
			if( m_pOwner->titanGrip && (
				proto->SubClass == ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD ||
				proto->SubClass == ITEM_SUBCLASS_WEAPON_TWOHAND_AXE ||
				proto->SubClass == ITEM_SUBCLASS_WEAPON_TWOHAND_MACE|| 
				type == INVTYPE_SHIELD) )
				return 0;	// Titan's Grip
			
			if(type == INVTYPE_WEAPON || type == INVTYPE_WEAPONOFFHAND)
			{
				Item* mainweapon = GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if(mainweapon) //item exists
				{
					if(mainweapon->GetProto())
					{
						if(mainweapon->GetProto()->InventoryType != INVTYPE_2HWEAPON)
						{
							if(m_pOwner->_HasSkillLine(SKILL_DUAL_WIELD))
								return 0;
							else
								return INV_ERR_CANT_DUAL_WIELD;
						}
						else
						{
							if(!skip_2h_check)
								return INV_ERR_CANT_EQUIP_WITH_TWOHANDED;
							else
								return 0;
						}
					}
				}
				else
				{
					if(m_pOwner->_HasSkillLine(SKILL_DUAL_WIELD))
						return 0;
					else
						return INV_ERR_CANT_DUAL_WIELD;
				}
			}
			else if(type == INVTYPE_SHIELD || type == INVTYPE_HOLDABLE)
			{
				Item* mainweapon = GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if(mainweapon) //item exists
				{
					if(mainweapon->GetProto())
					{
						if(mainweapon->GetProto()->InventoryType != INVTYPE_2HWEAPON)
						{
							return 0;
						}
						else
						{
							if(!skip_2h_check)
								return INV_ERR_CANT_EQUIP_WITH_TWOHANDED;
							else
								return 0;
						}
					}
				}
				else
				{
					return 0;
				}
			}
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;
		}
	case EQUIPMENT_SLOT_RANGED:
		{
			if(type == INVTYPE_RANGED || type == INVTYPE_THROWN || type == INVTYPE_RANGEDRIGHT || type == INVTYPE_RELIC)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT;//6;
		}
	case EQUIPMENT_SLOT_TABARD:
		{
			if(type == INVTYPE_TABARD)
				return 0;
			else
				return INV_ERR_ITEM_DOESNT_GO_TO_SLOT; // 6;
		}
	case BANK_SLOT_BAG_1:
	case BANK_SLOT_BAG_2:
	case BANK_SLOT_BAG_3:
	case BANK_SLOT_BAG_4:
	case BANK_SLOT_BAG_5:
	case BANK_SLOT_BAG_6:
	case BANK_SLOT_BAG_7:
		{
			int32 bytes,slots;
			uint8 islot;

			if(!GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot))
			{
				//check if player got that slot.
				bytes = m_pOwner->GetUInt32Value(PLAYER_BYTES_2);
				slots =(uint8) (bytes >> 16);
				islot = GetInternalBankSlotFromPlayer(slot);
				if(slots < islot)
				{
					return INV_ERR_MUST_PURCHASE_THAT_BAG_SLOT;
				}

				//in case bank slot exists, check if player can put the item there
				if(type == INVTYPE_BAG)
				{
					return 0;
				}
				else
				{
					return INV_ERR_NOT_A_BAG;
				}
			}
			else
			{
				if(GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
				{
					if((IsBagSlot(slot) && DstInvSlot == INVENTORY_SLOT_NOT_SET))
					{
						if(proto->InventoryType == INVTYPE_BAG )
						{
							return 0;
						}
					}

					if(proto->BagFamily & GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
					{
						return 0;
					}
					else
					{
						return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
					}
				}
				else
				{
					return 0;
				}
			}
		}
	case INVENTORY_SLOT_BAG_1:	
	case INVENTORY_SLOT_BAG_2:
	case INVENTORY_SLOT_BAG_3:
	case INVENTORY_SLOT_BAG_4:
		{
			//this chunk of code will limit you to equip only 1 Ammo Bag. Later i found out that this is not blizzlike so i will remove it when it's blizzlike
			//we are trying to equip an Ammo Bag
			if(proto->Class==ITEM_CLASS_QUIVER)
			{
				//check if we already have an AB equiped
				FindAmmoBag();
				//we do have amo bag but we are not swaping them then we send error
				if(result.Slot!=ITEM_NO_SLOT_AVAILABLE && result.Slot != slot)
				{
					return INV_ERR_CAN_EQUIP_ONLY1_AMMOPOUCH;
				}
			}
			if(GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot))
			{
				if(GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
				{
					if((IsBagSlot(slot) && DstInvSlot == INVENTORY_SLOT_NOT_SET))
					{
						if(proto->InventoryType == INVTYPE_BAG )
						{
							return 0;
						}
					}
					
					if(proto->BagFamily & GetInventoryItem(INVENTORY_SLOT_NOT_SET,slot)->GetProto()->BagFamily)
					{
						return 0;
					}
					else
					{
						return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if(type == INVTYPE_BAG)
				{
					return 0;
				}
				else
				{
					return INV_ERR_NOT_A_BAG;
				}
			}
		}
	case INVENTORY_SLOT_ITEM_1:
	case INVENTORY_SLOT_ITEM_2:
	case INVENTORY_SLOT_ITEM_3:
	case INVENTORY_SLOT_ITEM_4:
	case INVENTORY_SLOT_ITEM_5:
	case INVENTORY_SLOT_ITEM_6:
	case INVENTORY_SLOT_ITEM_7:
	case INVENTORY_SLOT_ITEM_8:
	case INVENTORY_SLOT_ITEM_9:
	case INVENTORY_SLOT_ITEM_10:
	case INVENTORY_SLOT_ITEM_11:
	case INVENTORY_SLOT_ITEM_12:
	case INVENTORY_SLOT_ITEM_13:
	case INVENTORY_SLOT_ITEM_14:
	case INVENTORY_SLOT_ITEM_15:
	case INVENTORY_SLOT_ITEM_16:
		{
			return 0;
		}
	case INVENTORY_KEYRING_1:
	case INVENTORY_KEYRING_2:
	case INVENTORY_KEYRING_3:
	case INVENTORY_KEYRING_4:
	case INVENTORY_KEYRING_5:
	case INVENTORY_KEYRING_6:
	case INVENTORY_KEYRING_7:
	case INVENTORY_KEYRING_8:
	case INVENTORY_KEYRING_9:
	case INVENTORY_KEYRING_10:
	case INVENTORY_KEYRING_11:
	case INVENTORY_KEYRING_12:
	case INVENTORY_KEYRING_13:
	case INVENTORY_KEYRING_14:
	case INVENTORY_KEYRING_15:
	case INVENTORY_KEYRING_16:
	case INVENTORY_KEYRING_17:
	case INVENTORY_KEYRING_18:
	case INVENTORY_KEYRING_19:
	case INVENTORY_KEYRING_20:
	case INVENTORY_KEYRING_21:
	case INVENTORY_KEYRING_22:
	case INVENTORY_KEYRING_23:
	case INVENTORY_KEYRING_24:
	case INVENTORY_KEYRING_25:
	case INVENTORY_KEYRING_26:
	case INVENTORY_KEYRING_27:
	case INVENTORY_KEYRING_28:
	case INVENTORY_KEYRING_29:
	case INVENTORY_KEYRING_30:
	case INVENTORY_KEYRING_31:
	case INVENTORY_KEYRING_32:
		{
			if(proto->BagFamily & ITEM_TYPE_KEYRING )
			{
					return 0;
			}
			else
			{
					return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
			}
		}
	case INVENTORY_CURRENCY_1:
	case INVENTORY_CURRENCY_2:
	case INVENTORY_CURRENCY_3:
	case INVENTORY_CURRENCY_4:
	case INVENTORY_CURRENCY_5:
	case INVENTORY_CURRENCY_6:
	case INVENTORY_CURRENCY_7:
	case INVENTORY_CURRENCY_8:
	case INVENTORY_CURRENCY_9:
	case INVENTORY_CURRENCY_10:
	case INVENTORY_CURRENCY_11:
	case INVENTORY_CURRENCY_12:
	case INVENTORY_CURRENCY_13:
	case INVENTORY_CURRENCY_14:
	case INVENTORY_CURRENCY_15:
	case INVENTORY_CURRENCY_16:
	case INVENTORY_CURRENCY_17:
	case INVENTORY_CURRENCY_18:
	case INVENTORY_CURRENCY_19:
	case INVENTORY_CURRENCY_20:
	case INVENTORY_CURRENCY_21:
	case INVENTORY_CURRENCY_22:
	case INVENTORY_CURRENCY_23:
	case INVENTORY_CURRENCY_24:
	case INVENTORY_CURRENCY_25:
	case INVENTORY_CURRENCY_26:
	case INVENTORY_CURRENCY_27:
	case INVENTORY_CURRENCY_28:
	case INVENTORY_CURRENCY_29:
	case INVENTORY_CURRENCY_30:
	case INVENTORY_CURRENCY_31:
	case INVENTORY_CURRENCY_32:
		if( proto->BagFamily & ITEM_TYPE_CURRENCY )
			return 0;
		else
			return INV_ERR_ITEM_DOESNT_GO_INTO_BAG;
	default:
		return 0;
	}
}

//-------------------------------------------------------------------//
//Description: Checks if player can receive the item
//-------------------------------------------------------------------//
uint8 ItemInterface::CanReceiveItem(ItemPrototype * item, uint32 amount, ItemExtendedCostEntry *ec)
{
	if(!item)
		return 0;

	if(item->Unique)
	{
		uint32 count = GetItemCount(item->ItemId, true);
		if(count == item->Unique || ((count + amount) > item->Unique))
		{
			return INV_ERR_CANT_CARRY_MORE_OF_THIS;
		}
	}
	return 0;
}

void ItemInterface::BuyItem(ItemPrototype *item, uint32 total_amount, Creature* pVendor, ItemExtendedCostEntry *ec)
{
	if(item->BuyPrice)
	{
		uint32 itemprice = GetBuyPriceForItem(item, total_amount, m_pOwner, pVendor);
		if(itemprice>m_pOwner->GetUInt32Value(PLAYER_FIELD_COINAGE))
			m_pOwner->SetUInt32Value(PLAYER_FIELD_COINAGE,0);
		else
			m_pOwner->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)itemprice);
	}
	if( ec != NULL )
	{
		for(int i = 0;i<5;i++)
		{
			if(ec->item[i])
				m_pOwner->GetItemInterface()->RemoveItemAmt( ec->item[i], total_amount * ec->count[i] );
		}

		int32 val = (int32)(ec->honor * total_amount);

		/*if( m_pOwner->GetUInt32Value(PLAYER_FIELD_HONOR_CURRENCY) >= (uint32)val )
		{
			m_pOwner->ModUnsigned32Value( PLAYER_FIELD_HONOR_CURRENCY, -val );
			m_pOwner->m_honorPoints -= val;
		}

		val = (int32)(ec->arena * total_amount);
		if( m_pOwner->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY ) >= (uint32)val )
		{
			m_pOwner->ModUnsigned32Value( PLAYER_FIELD_ARENA_CURRENCY, -val );
			m_pOwner->m_arenaPoints -= val;
		}*/ /* TODO: Fix - CMB */
	}
}

enum CanAffordItem
{
	CAN_AFFORD_ITEM_ERROR_OK						= 0,
	CAN_AFFORD_ITEM_ERROR_CURRENTLY_SOLD_OUT		= 1,
	CAN_AFFORD_ITEM_ERROR_DONT_HAVE_ENOUGH_MONEY	= 2,
	CAN_AFFORD_ITEM_ERROR_NOT_FOUND					= 3,
	CAN_AFFORD_ITEM_ERROR_DOESNT_LIKE_YOU			= 4,
	CAN_AFFORD_ITEM_ERROR_TOO_FAR_AWAY				= 5,
	CAN_AFFORD_ITEM_ERROR_NO_MESSAGE				= 6,
	CAN_AFFORD_ITEM_ERROR_CANT_CARRY_ANY_MORE		= 8,
	CAN_AFFORD_ITEM_ERROR_NOT_REQUIRED_RANK			= 11,
	CAN_AFFORD_ITEM_ERROR_REPUTATION				= 12,
};

uint8 ItemInterface::CanAffordItem(ItemPrototype * item, uint32 amount, Creature* pVendor, ItemExtendedCostEntry *ec)
{
	if( ec != NULL )
	{
		for(int i = 0;i<5;i++)
		{
			if(ec->item[i])
			{
				if(m_pOwner->GetItemInterface()->GetItemCount(ec->item[i], false) < (ec->count[i]*amount))
					return CAN_AFFORD_ITEM_ERROR_DONT_HAVE_ENOUGH_MONEY;
			}
		}

		/*if(m_pOwner->GetUInt32Value(PLAYER_FIELD_HONOR_CURRENCY) < (ec->honor*amount))
			return CAN_AFFORD_ITEM_ERROR_DONT_HAVE_ENOUGH_MONEY;
		if(m_pOwner->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY ) < (ec->arena*amount))
			return CAN_AFFORD_ITEM_ERROR_DONT_HAVE_ENOUGH_MONEY;*/
 		if(m_pOwner->GetMaxPersonalRating() < ec->personalrating)
			return CAN_AFFORD_ITEM_ERROR_NOT_REQUIRED_RANK;
	}

	if(item->BuyPrice)
	{
		int32 price = GetBuyPriceForItem(item, amount, m_pOwner, pVendor) * amount;
		if((int32)m_pOwner->GetUInt32Value(PLAYER_FIELD_COINAGE) < price)
		{
			return CAN_AFFORD_ITEM_ERROR_DONT_HAVE_ENOUGH_MONEY;
		}
	}
	if(item->RequiredFaction)
	{
		FactionDBC *factdbc = dbcFaction.LookupEntry(item->RequiredFaction);
		if(!factdbc || factdbc->RepListId < 0)
			return 0;
		
		if( Player::GetReputationRankFromStanding( m_pOwner->GetStanding( item->RequiredFaction )) < (int32)item->RequiredFactionStanding )
		{
			return CAN_AFFORD_ITEM_ERROR_REPUTATION;
		}
	}
	return 0;
}

//-------------------------------------------------------------------//
//Description: Gets the Item slot by item type
//-------------------------------------------------------------------//
uint8 ItemInterface::GetItemSlotByType(uint32 type)
{
	switch(type)
	{
	case INVTYPE_NON_EQUIP:
		return ITEM_NO_SLOT_AVAILABLE; 
	case INVTYPE_HEAD:
		{
			return EQUIPMENT_SLOT_HEAD;
		}
	case INVTYPE_NECK:
		{
			return EQUIPMENT_SLOT_NECK;
		}
	case INVTYPE_SHOULDERS:
		{
			return EQUIPMENT_SLOT_SHOULDERS;
		}
	case INVTYPE_BODY:
		{
			return EQUIPMENT_SLOT_BODY;
		}
	case INVTYPE_CHEST:
		{
			return EQUIPMENT_SLOT_CHEST;
		}
	case INVTYPE_ROBE: // ???
		{
			return EQUIPMENT_SLOT_CHEST;
		}
	case INVTYPE_WAIST:
		{
			return EQUIPMENT_SLOT_WAIST;
		}
	case INVTYPE_LEGS:
		{
			return EQUIPMENT_SLOT_LEGS;
		}
	case INVTYPE_FEET:
		{
			return EQUIPMENT_SLOT_FEET;
		}
	case INVTYPE_WRISTS:
		{
			return EQUIPMENT_SLOT_WRISTS;
		}
	case INVTYPE_HANDS:
		{
			return EQUIPMENT_SLOT_HANDS;
		}
	case INVTYPE_FINGER:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_FINGER1))
				return EQUIPMENT_SLOT_FINGER1;
			else if (!GetInventoryItem(EQUIPMENT_SLOT_FINGER2))
				return EQUIPMENT_SLOT_FINGER2;
			else
				return EQUIPMENT_SLOT_FINGER1; //auto equips always in finger 1
		}
	case INVTYPE_TRINKET:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_TRINKET1))
				return EQUIPMENT_SLOT_TRINKET1;
			else if (!GetInventoryItem(EQUIPMENT_SLOT_TRINKET2))
				return EQUIPMENT_SLOT_TRINKET2;
			else
				return EQUIPMENT_SLOT_TRINKET1; //auto equips always on trinket 1
		}
	case INVTYPE_CLOAK:
		{
			return EQUIPMENT_SLOT_BACK;
		}
	case INVTYPE_WEAPON:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_MAINHAND) )
				return EQUIPMENT_SLOT_MAINHAND;
			else if(!GetInventoryItem(EQUIPMENT_SLOT_OFFHAND))
				return EQUIPMENT_SLOT_OFFHAND;
			else
				return EQUIPMENT_SLOT_MAINHAND;
		}
	case INVTYPE_SHIELD:
		{
			return EQUIPMENT_SLOT_OFFHAND;
		}
	case INVTYPE_RANGED:
		{
			return EQUIPMENT_SLOT_RANGED;
		}
	case INVTYPE_2HWEAPON:
		{
			if (!GetInventoryItem(EQUIPMENT_SLOT_MAINHAND) || !m_pOwner->titanGrip)
				return EQUIPMENT_SLOT_MAINHAND;
			else if(!GetInventoryItem(EQUIPMENT_SLOT_OFFHAND))
				return EQUIPMENT_SLOT_OFFHAND;
			else
				return EQUIPMENT_SLOT_MAINHAND;
		}
	case INVTYPE_TABARD:
		{
			return EQUIPMENT_SLOT_TABARD;
		}
	case INVTYPE_WEAPONMAINHAND:
		{
			return EQUIPMENT_SLOT_MAINHAND;
		}
	case INVTYPE_WEAPONOFFHAND:
		{
			return EQUIPMENT_SLOT_OFFHAND;
		}
	case INVTYPE_HOLDABLE:
		{
			return EQUIPMENT_SLOT_OFFHAND;
		}
	case INVTYPE_THROWN:
		return EQUIPMENT_SLOT_RANGED; // ?
	case INVTYPE_RANGEDRIGHT:
		return EQUIPMENT_SLOT_RANGED; // ?
	case INVTYPE_RELIC:
		return EQUIPMENT_SLOT_RANGED;
	case INVTYPE_BAG:
		{
			for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
			{
				if (!GetInventoryItem(i))
					return i;
			}
			return ITEM_NO_SLOT_AVAILABLE; //bags are not suposed to be auto-equiped when slots are not free
		}
	default:
		return ITEM_NO_SLOT_AVAILABLE;
	}
}

//-------------------------------------------------------------------//
//Description: Gets a Item by guid
//-------------------------------------------------------------------//
Item* ItemInterface::GetItemByGUID(uint64 Guid)
{
	uint32 i ;

	//EQUIPMENT
	for(i=EQUIPMENT_SLOT_START;i<EQUIPMENT_SLOT_END;i++)
	{
		if(m_pItems[i] != 0)
		{
			if( m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;//not a containerslot. In 1.8 client marked wrong slot like this
				result.Slot = i;
				return m_pItems[i];
			}
		}
	}

	//INVENTORY BAGS
	for(i=INVENTORY_SLOT_BAG_START;i<INVENTORY_SLOT_BAG_END;i++)
	{
		if(m_pItems[i] != NULL && m_pItems[i]->IsContainer())
		{
			if(m_pItems[i]->GetGUID()==Guid) 
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = i;
				return m_pItems[i]; 
			}

			for (uint32 j =0; j < m_pItems[i]->GetProto()->ContainerSlots;j++)
			{
				Item* item2 = TO_CONTAINER(m_pItems[i])->GetItem(j);
				if (item2)
				{
					if (item2->GetGUID() == Guid)
					{
						result.ContainerSlot = i;
						result.Slot = j;
						return item2;
					}
				}
			}
		}
	}

	//INVENTORY
	for(i=INVENTORY_SLOT_ITEM_START;i<INVENTORY_SLOT_ITEM_END;i++)
	{
		if(m_pItems[i] != 0)
		{
			if(m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = i;
				return m_pItems[i];
			}
		}
	}

	//Keyring
	for(i=INVENTORY_KEYRING_START;i<INVENTORY_KEYRING_END;i++)
	{
		if(m_pItems[i] != 0)
		{
			if(m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = i;
				return m_pItems[i];
			}
		}
	}

	for(i=INVENTORY_CURRENCY_START;i<INVENTORY_CURRENCY_END;i++)
	{
		if(m_pItems[i] != 0)
		{
			if(m_pItems[i]->GetGUID() == Guid)
			{
				result.ContainerSlot = INVALID_BACKPACK_SLOT;
				result.Slot = i;
				return m_pItems[i];
			}
		}
	}
	return NULL;
}

//-------------------------------------------------------------------//
//Description: Inventory Error report
//-------------------------------------------------------------------//
void ItemInterface::BuildInventoryChangeError(Item* SrcItem, Item* DstItem, uint8 Error)
{
	WorldPacket data(22);

	data.Initialize( SMSG_INVENTORY_CHANGE_FAILURE );
	data << Error;

	if(Error == 1) 
	{
		if(SrcItem)
		{
			data << SrcItem->GetProto()->RequiredLevel;
		}
	}

	data << (SrcItem ? SrcItem->GetGUID() : uint64(0));
	data << (DstItem ? DstItem->GetGUID() : uint64(0));
	data << uint8(0);
	if(Error == INV_ERR_YOU_MUST_REACH_LEVEL_N)
	{
		uint32 level = 0;
		if(SrcItem)
		{
			level = SrcItem->GetProto()->RequiredLevel;
			data << uint32(level);
		}
	}

	m_pOwner->GetSession()->SendPacket( &data );
}

void ItemInterface::EmptyBuyBack()
{
	 for (uint32 j = 0;j < 12;j++)
	 {
		 if (m_pBuyBack[j] != NULL)
		 {
			 m_pBuyBack[j]->DestroyForPlayer(m_pOwner);
			 m_pBuyBack[j]->DeleteFromDB();

			 if(m_pBuyBack[j]->IsContainer())
			 {
				if (TO_CONTAINER(m_pBuyBack[j])->IsInWorld())
					TO_CONTAINER(m_pBuyBack[j])->RemoveFromWorld();
				
				m_pBuyBack[j]->Destructor();
			 }
			 else
			 {
				if (m_pBuyBack[j]->IsInWorld())
					m_pBuyBack[j]->RemoveFromWorld();

				m_pBuyBack[j]->Destructor();
			 }

			 m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*j),0);
			 m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + j,0);
			 m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j,0);
			 m_pBuyBack[j] = NULL;
		 }
		 else
			 break;
	 }
}

void ItemInterface::AddBuyBackItem(Item* it,uint32 price)
{
	int i;
	if ((m_pBuyBack[11] != NULL) && (m_pOwner->GetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + 22) != 0))
	{
		if(m_pBuyBack[0] != NULL)
		{		   
			 m_pBuyBack[0]->DestroyForPlayer(m_pOwner);
			 m_pBuyBack[0]->DeleteFromDB();

			 if(m_pBuyBack[0]->IsContainer())
			 {
				if (TO_CONTAINER(m_pBuyBack[0])->IsInWorld())
					TO_CONTAINER(m_pBuyBack[0])->RemoveFromWorld();
				
				m_pBuyBack[0]->Destructor();
			 }
			 else
			 {
				if (m_pBuyBack[0]->IsInWorld())
					m_pBuyBack[0]->RemoveFromWorld();

				m_pBuyBack[0]->Destructor();
			 }
		}

		for (int j = 0;j < 11;j++)
		{
			//SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*j),buyback[j+1]->GetGUID());
			m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*j),m_pOwner->GetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ((j+1)*2) ) );
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + j,m_pOwner->GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + j+1));
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j,m_pOwner->GetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j+1));
			m_pBuyBack[j] = m_pBuyBack[j+1];
		}
		m_pBuyBack[11] = it;

		m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (2*(11)),m_pBuyBack[11]->GetGUID());
		m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + 11,price);
		m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + 11,(uint32)UNIXTIME);
		return;
	}

	for(i=0; i < 24;i+=2)
	{
		if((m_pOwner->GetUInt32Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + i) == 0) || (m_pBuyBack[i/2] == NULL))
		{
			OUT_DEBUG("setting buybackslot %u\n",i/2);
			m_pBuyBack[i >> 1] = it;

			m_pOwner->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + i,m_pBuyBack[i >> 1]->GetGUID());
			//SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + i,it->GetGUID());
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + (i >> 1),price);
			m_pOwner->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + (i >> 1),(uint32)UNIXTIME);
			return;
		}
	}
}

void ItemInterface::RemoveBuyBackItem(uint32 index)
{
	int32 j = 0;
	for( j = index;j < 11; j++ )
	{
		if (m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( j * 2 ) ) != 0 )
		{
			m_pOwner->SetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( 2 * j ), m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( ( j + 1 ) * 2 ) ) );
			m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j+1));
			m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j + 1 ) );
			
			if( m_pBuyBack[j+1] != NULL && ( m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( ( j + 1 ) * 2 ) ) != 0 ) )
			{
				m_pBuyBack[j] = m_pBuyBack[j+1];
			}
			else
			{
				if( m_pBuyBack[j] != NULL )
				{
					m_pBuyBack[j] = NULL;
				}

				OUT_DEBUG( "nulling %u\n", j );
			}
		}
		else
			return;
	}
	j = 11;
	m_pOwner->SetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( 2 * j ), m_pOwner->GetUInt64Value( PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + ( ( j + 1 ) * 2 ) ) );
	m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_PRICE_1 + j + 1 ) );
	m_pOwner->SetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j, m_pOwner->GetUInt32Value( PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + j + 1 ) );
	if( m_pBuyBack[11] )
	{
		m_pBuyBack[11] = NULL;
	}
	
}

//-------------------------------------------------------------------//
//Description: swap inventory slots
//-------------------------------------------------------------------//
void ItemInterface::SwapItemSlots(uint8 srcslot, uint8 dstslot)
{
	// srcslot and dstslot are int... NULL might not be an int depending on arch where it is compiled
	if( srcslot >= MAX_INVENTORY_SLOT )
		return;

	if( dstslot >= MAX_INVENTORY_SLOT )
		return;

	Item* SrcItem = GetInventoryItem( srcslot );
	Item* DstItem = GetInventoryItem( dstslot );

	DEBUG_LOG( "ItemInterface","SwapItemSlots(%u, %u);" , srcslot , dstslot );

	// Force GM robes on all GM's execpt 'az' status, if set in world config
	if(m_pOwner->GetSession()->HasGMPermissions() && sWorld.gm_force_robes )
	{
		if( strchr(m_pOwner->GetSession()->GetPermissions(),'az')==NULL)
		{
			//don't allow anything else then these items
			if( dstslot < INVENTORY_SLOT_BAG_END )
			{
				if( !(SrcItem->GetUInt32Value(OBJECT_FIELD_ENTRY) == 12064 || 
					SrcItem->GetUInt32Value(OBJECT_FIELD_ENTRY) == 2586 ||
					SrcItem->GetUInt32Value(OBJECT_FIELD_ENTRY) == 11508 ))
				{
					AddItemToFreeSlot(SrcItem);
					return;
				}
			}
		}
	}

	if( SrcItem != NULL && DstItem != NULL && SrcItem->GetEntry()==DstItem->GetEntry() && SrcItem->GetProto()->MaxCount > 1 && SrcItem->wrapped_item_id == 0 && DstItem->wrapped_item_id == 0 )
	{
		uint32 total = SrcItem->GetUInt32Value( ITEM_FIELD_STACK_COUNT ) + DstItem->GetUInt32Value( ITEM_FIELD_STACK_COUNT );
		if( total <= DstItem->GetProto()->MaxCount )
		{
			DstItem->ModUnsigned32Value( ITEM_FIELD_STACK_COUNT, SrcItem->GetUInt32Value( ITEM_FIELD_STACK_COUNT ) );
			SafeFullRemoveItemFromSlot( INVENTORY_SLOT_NOT_SET, srcslot );
			DstItem->m_isDirty = true;
			return;
		}
		else
		{
			if( DstItem->GetUInt32Value( ITEM_FIELD_STACK_COUNT ) == DstItem->GetProto()->MaxCount )
			{

			}
			else
			{
				int32 delta=DstItem->GetProto()->MaxCount-DstItem->GetUInt32Value( ITEM_FIELD_STACK_COUNT );
				DstItem->SetUInt32Value( ITEM_FIELD_STACK_COUNT, DstItem->GetProto()->MaxCount );
				SrcItem->ModUnsigned32Value( ITEM_FIELD_STACK_COUNT, -delta );
				SrcItem->m_isDirty = true;
				DstItem->m_isDirty = true;
				return;
			}
		}
	}

	//src item was equiped previously
	if( srcslot < INVENTORY_SLOT_BAG_END ) 
	{
		if( m_pItems[(int)srcslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)srcslot], srcslot, false );
	}

	//dst item was equiped previously
	if( dstslot < INVENTORY_SLOT_BAG_END ) 
	{
		if( m_pItems[(int)dstslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)dstslot], dstslot, false );
	}

	//OUT_DEBUG( "Putting items into slots..." );



	m_pItems[(int)dstslot] = SrcItem;

	// Moving a bag with items to a empty bagslot
	if ( (DstItem == NULL) && (SrcItem->IsContainer()) )
	{
		Item* tSrcItem = NULL;

		for ( uint32 Slot = 0; Slot < SrcItem->GetProto()->ContainerSlots; Slot++ )
		{
			tSrcItem = (TO_CONTAINER(m_pItems[(int)srcslot]))->GetItem(Slot);

			m_pOwner->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(srcslot, Slot, false);

			if ( tSrcItem != NULL )
			{
				m_pOwner->GetItemInterface()->SafeAddItem(tSrcItem, dstslot, Slot);
			}
		}
	}

	m_pItems[(int)srcslot] = DstItem;

	// swapping 2 bags filled with items
	if ( DstItem && SrcItem->IsContainer() && DstItem->IsContainer() )
	{
		Item* tDstItem = NULL;
		Item* tSrcItem = NULL;
		uint32 TotalSlots = 0;

		// Determine the max amount of slots to swap
		if ( SrcItem->GetProto()->ContainerSlots > DstItem->GetProto()->ContainerSlots )
			TotalSlots = SrcItem->GetProto()->ContainerSlots;
		else
			TotalSlots = DstItem->GetProto()->ContainerSlots;

		// swap items in the bags
		for( uint32 Slot = 0; Slot < TotalSlots; Slot++ )
		{
			tSrcItem = (TO_CONTAINER(m_pItems[(int)srcslot]))->GetItem(Slot);
			tDstItem = (TO_CONTAINER(m_pItems[(int)dstslot]))->GetItem(Slot);

			if( tSrcItem != NULL )
				m_pOwner->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(srcslot, Slot, false);
			if( tDstItem != NULL )
				m_pOwner->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(dstslot, Slot, false);

			if( tSrcItem != NULL )
				(TO_CONTAINER(DstItem))->AddItem(Slot, tSrcItem);
			if( tDstItem != NULL )
				(TO_CONTAINER(SrcItem))->AddItem(Slot, tDstItem);
		}
	}
	
	if( DstItem != NULL )
		DstItem->m_isDirty = true;
	if( SrcItem != NULL )
		SrcItem->m_isDirty = true;

	if( m_pItems[(int)dstslot] != NULL )
	{
		//OUT_DEBUG( "(SrcItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now %u" , dstslot * 2 , m_pItems[(int)dstslot]->GetGUID() );
		m_pOwner->SetUInt64Value( PLAYER_FIELD_INV_SLOT_HEAD + (dstslot*2),  m_pItems[(int)dstslot]->GetGUID() );
	}
	else
	{
		//OUT_DEBUG( "(SrcItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now 0" , dstslot * 2 );
		m_pOwner->SetUInt64Value( PLAYER_FIELD_INV_SLOT_HEAD + (dstslot*2), 0 );
	}

	if( m_pItems[(int)srcslot] != NULL )
	{
		//OUT_DEBUG( "(DstItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now %u" , dstslot * 2 , m_pItems[(int)srcslot]->GetGUID() );
		m_pOwner->SetUInt64Value( PLAYER_FIELD_INV_SLOT_HEAD + (srcslot*2), m_pItems[(int)srcslot]->GetGUID() );
	}
	else
	{
		//OUT_DEBUG( "(DstItem) PLAYER_FIELD_INV_SLOT_HEAD + %u is now 0" , dstslot * 2 );
		m_pOwner->SetUInt64Value( PLAYER_FIELD_INV_SLOT_HEAD + (srcslot*2), 0 );
	}

	if( srcslot < INVENTORY_SLOT_BAG_END )	// source item is equiped
	{
		if( m_pItems[(int)srcslot] ) // dstitem goes into here.
		{
			// Bags aren't considered "visible".
			if( srcslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (srcslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, m_pItems[(int)srcslot]->GetEntry() );
				m_pOwner->SetUInt32Value( VisibleBase + 1, m_pItems[(int)srcslot]->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_1 ) );
			}

			// handle bind on equip
			if( m_pItems[(int)srcslot]->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
				m_pItems[(int)srcslot]->SoulBind();

			// 2.4.3: Casting a spell is cancelled when you equip
			if( GetOwner() && GetOwner()->GetCurrentSpell() )
			{
				GetOwner()->GetCurrentSpell()->cancel();
			}
		} 
		else 
		{
			// Bags aren't considered "visible".
			if( srcslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (srcslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 1, 0 );
			}
		}
	}  

	if( dstslot < INVENTORY_SLOT_BAG_END )   // source item is inside inventory
	{
		if( m_pItems[(int)dstslot] != NULL ) // srcitem goes into here.
		{	
			// Bags aren't considered "visible".
			if( dstslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (dstslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, m_pItems[(int)dstslot]->GetEntry() );
				m_pOwner->SetUInt32Value( VisibleBase + 1, m_pItems[(int)dstslot]->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_1 ) );
			}

			// handle bind on equip
			if( m_pItems[(int)dstslot]->GetProto()->Bonding == ITEM_BIND_ON_EQUIP )
				m_pItems[(int)dstslot]->SoulBind();

		}
		else
		{

			// bags aren't considered visible
			if( dstslot < EQUIPMENT_SLOT_END )
			{
				int VisibleBase = PLAYER_VISIBLE_ITEM_1_ENTRYID + (dstslot * PLAYER_VISIBLE_ITEM_LENGTH);
				m_pOwner->SetUInt32Value( VisibleBase, 0 );
				m_pOwner->SetUInt32Value( VisibleBase + 1, 0 );
			}
		}
	}

	// handle dual wield
	if( dstslot == EQUIPMENT_SLOT_OFFHAND || srcslot == EQUIPMENT_SLOT_OFFHAND )
	{
		if( m_pItems[EQUIPMENT_SLOT_OFFHAND] != NULL && m_pItems[EQUIPMENT_SLOT_OFFHAND]->GetProto()->Class == ITEM_CLASS_WEAPON )
			m_pOwner->SetDuelWield( true );
		else
			m_pOwner->SetDuelWield( false );
	}

	//src item is equiped now
	if( srcslot < INVENTORY_SLOT_BAG_END ) 
 	{
		if( m_pItems[(int)srcslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)srcslot], srcslot, true );
		else if( srcslot == EQUIPMENT_SLOT_MAINHAND || srcslot == EQUIPMENT_SLOT_OFFHAND )
			m_pOwner->CalcDamage();
	}

	//dst item is equiped now
	if( dstslot < INVENTORY_SLOT_BAG_END ) 
	{
		if( m_pItems[(int)dstslot] != NULL )		
			m_pOwner->ApplyItemMods( m_pItems[(int)dstslot], dstslot, true );
		else if( dstslot == EQUIPMENT_SLOT_MAINHAND || dstslot == EQUIPMENT_SLOT_OFFHAND )
			m_pOwner->CalcDamage();
	}

}

//-------------------------------------------------------------------//
//Description: Item Loading
//-------------------------------------------------------------------//
void ItemInterface::mLoadItemsFromDatabase(QueryResult * result)
{
	uint8 containerslot, slot;
	Item* item;
	ItemPrototype* proto;

	if( result )
	{
		do
		{
			Field* fields = result->Fetch();

			containerslot = fields[13].GetInt8();
			slot = fields[14].GetInt8();
			proto = ItemPrototypeStorage.LookupEntry(fields[2].GetUInt32());

			if( proto != NULL )
			{
				if( proto->InventoryType == INVTYPE_BAG )
				{
					item = new Container( HIGHGUID_TYPE_CONTAINER, fields[1].GetUInt32() );
					item->Init();
					TO_CONTAINER( item )->LoadFromDB( fields );

				}
				else
				{
					item = new Item( HIGHGUID_TYPE_ITEM, fields[1].GetUInt32() );
					item->Init();
					item->LoadFromDB( fields, m_pOwner, false);
				}
				
				if(item != NULL && proto->BagFamily & ITEM_TYPE_CURRENCY)
				{
					if(containerslot != ((uint8)-1) || (containerslot == ((uint8)-1) && slot < INVENTORY_CURRENCY_1))
						sEventMgr.AddEvent(m_pOwner, &Player::EventCheckCurrencies, EVENT_CHECK_CURRENCIES, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}

				if( SafeAddItem( item, containerslot, slot ) )
				    item->m_isDirty = false;
				else
				{
					item->Destructor();
				}
			}
		}
		while( result->NextRow() );
	}
}

//-------------------------------------------------------------------//
//Description: Item saving
//-------------------------------------------------------------------//
void ItemInterface::mSaveItemsToDatabase(bool first, QueryBuffer * buf)
{
	uint32 x;

	for( x = EQUIPMENT_SLOT_START; x < INVENTORY_CURRENCY_END; ++x )
	{
		if( GetInventoryItem( x ) != NULL )
		{
			if( GetInventoryItem( x )->GetProto() != NULL )
			{
				if( IsBagSlot( x ) && GetInventoryItem( x )->IsContainer() )
				{
					TO_CONTAINER( GetInventoryItem( x ) )->SaveBagToDB( x, first, buf );
				}
				else
				{
					GetInventoryItem( x )->SaveToDB( INVENTORY_SLOT_NOT_SET, x, first, buf );
				}
			}
		}
	}
}

AddItemResult ItemInterface::AddItemToFreeBankSlot(Item* item)
{
	//special items first
	for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL )
		{
			if( m_pItems[i]->GetProto()->BagFamily & item->GetProto()->BagFamily )
			{
				if( m_pItems[i]->IsContainer() )
				{
					bool result = TO_CONTAINER( m_pItems[i] )->AddItemToFreeSlot( item, NULL );
					if( result )
						return ADD_ITEM_RESULT_OK;
				}
			}
		}
	}

	for( uint32 i= BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++ )
	{
		if( m_pItems[i] == NULL )
		{
			return SafeAddItem( item, INVENTORY_SLOT_NOT_SET, i );
		}
	}

	for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL && m_pItems[i]->GetProto()->BagFamily == 0 && m_pItems[i]->IsContainer() ) //special bags ignored
		{
			for( uint32 j =0; j < m_pItems[i]->GetProto()->ContainerSlots; j++ )
			{
				Item* item2 = TO_CONTAINER( m_pItems[i] )->GetItem(j);
				if( item2 == NULL )
				{
					return SafeAddItem( item, i, j );
				}
			}
		}
	}
	return ADD_ITEM_RESULT_ERROR;
}

uint8 ItemInterface::FindSpecialBag(Item* item)
{
	for( uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
	{
		if( m_pItems[i] != NULL )
		{
			if( m_pItems[i]->GetProto()->BagFamily & item->GetProto()->BagFamily )
			{
				return i;
			}
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

uint8 ItemInterface::FindFreeKeyringSlot()
{
	for( uint32 i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++ )
	{
		if( m_pItems[i] == NULL )
		{
			return i;
		}
	}
	return ITEM_NO_SLOT_AVAILABLE;
}

SlotResult ItemInterface::FindFreeInventorySlot(ItemPrototype *proto)
{
	//special item
	//special slots will be ignored of item is not set
	if( proto != NULL )
	{
		//OUT_DEBUG( "ItemInterface::FindFreeInventorySlot called for item %s" , proto->Name1 );
		if( proto->BagFamily)
		{
			if( proto->BagFamily & ITEM_TYPE_KEYRING )
			{
				for(uint32 i = INVENTORY_KEYRING_START; i < INVENTORY_KEYRING_END; i++ )
				{
					if( m_pItems[i] == NULL )
					{
						result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
						result.Slot = i;
						result.Result = true;
						return result;
					}
				}
			}
			else if( proto->BagFamily & ITEM_TYPE_CURRENCY )
			{
				for(uint8 i = INVENTORY_CURRENCY_START; i < INVENTORY_CURRENCY_END; i++ )
				{
					if( m_pItems[i] == NULL )
					{
						result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
						result.Slot = i;
						result.Result = true;
						return result;
					}
				}
			}
			else
			{
				for( uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
				{
					if( m_pItems[i] != NULL && m_pItems[i]->IsContainer() )
					{
						if( m_pItems[i]->GetProto()->BagFamily & proto->BagFamily )
						{
							uint8 slot = TO_CONTAINER( m_pItems[i] )->FindFreeSlot();
							if( slot != ITEM_NO_SLOT_AVAILABLE ) 
							{
								result.ContainerSlot = i;
								result.Slot = slot;
								result.Result = true;
								return result;
							}
						}
					}
				}
			}
		}
	}

	//backpack
	for( uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++ )
	{
		Item* item = GetInventoryItem( i );
		if( item == NULL ) 
		{
			result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
			result.Slot = i;
			result.Result = true;
			return result;
		}
	}

	//bags
	for( uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
	{
		Item* item = GetInventoryItem(i);
		if( item != NULL )
		{
			if( item->IsContainer() && !item->GetProto()->BagFamily )
			{
				uint8 slot = TO_CONTAINER( m_pItems[i] )->FindFreeSlot();
				if( slot != ITEM_NO_SLOT_AVAILABLE ) 
				{ 
					result.ContainerSlot = i;
					result.Slot = slot;
					result.Result = true;
					return result;
				}
			}
		}
	}

	result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
	result.Slot = ITEM_NO_SLOT_AVAILABLE;
	result.Result = false;

	return result;
}

SlotResult ItemInterface::FindFreeBankSlot(ItemPrototype *proto)
{
	//special item
	//special slots will be ignored of item is not set
	if( proto != NULL )
	{
		if( proto->BagFamily )
		{
			for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
			{
				if( m_pItems[i] != NULL && m_pItems[i]->IsContainer() )
				{
					if( m_pItems[i]->GetProto()->BagFamily & proto->BagFamily )
					{
						int32 slot = TO_CONTAINER( m_pItems[i] )->FindFreeSlot();
						if( slot != ITEM_NO_SLOT_AVAILABLE ) 
						{
							result.ContainerSlot = i;
							result.Slot = slot;
							result.Result = true;
							return result;
						}
					}
				}
			}
		}
	}

	//backpack
	for( uint32 i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; i++ )
	{
		Item* item = GetInventoryItem( i );
		if( item == NULL ) 
		{
			result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
			result.Slot = i;
			result.Result = true;
			return result;
		}
	}

	//bags
	for( uint32 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; i++ )
	{
		Item* item = GetInventoryItem(i);
		if( item != NULL )
		{
			if( item->IsContainer() && !item->GetProto()->BagFamily )
			{
				int32 slot = TO_CONTAINER( m_pItems[i] )->FindFreeSlot();
				if( slot != ITEM_NO_SLOT_AVAILABLE) 
				{ 
					result.ContainerSlot = i;
					result.Slot = slot;
					result.Result = true;
					return result;
				}
			}
		}
	}

	result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
	result.Slot = ITEM_NO_SLOT_AVAILABLE;
	result.Result = false;

	return result;
}

SlotResult ItemInterface::FindAmmoBag()
{
	for( uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++ )
		if( m_pItems[i] != NULL && m_pItems[i]->IsAmmoBag())
		{
			result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
			result.Slot = i;
			result.Result = true;
			return result;
		}

	result.ContainerSlot = ITEM_NO_SLOT_AVAILABLE;
	result.Slot = ITEM_NO_SLOT_AVAILABLE;
	result.Result = false;

	return result;
}

void ItemInterface::ReduceItemDurability()
{
	uint32 f = RandomUInt(100);
	if( f <= 10 ) //10% chance to loose 1 dur from a random valid item.
	{
		int32 slot = RandomUInt( EQUIPMENT_SLOT_END );
		Item* pItem = GetInventoryItem( INVENTORY_SLOT_NOT_SET, slot );
		if( pItem != NULL )
		{
			if( pItem->GetUInt32Value( ITEM_FIELD_DURABILITY) && pItem->GetUInt32Value( ITEM_FIELD_MAXDURABILITY ) )
			{
				pItem->SetUInt32Value( ITEM_FIELD_DURABILITY, ( pItem->GetUInt32Value( ITEM_FIELD_DURABILITY ) - 1 ) );
				pItem->m_isDirty = true;
				//check final durabiity
				if( !pItem->GetUInt32Value( ITEM_FIELD_DURABILITY ) ) //no dur left
				{
					this->GetOwner()->ApplyItemMods( pItem, slot, false, true );
				}
			}
		}
	}
}

bool ItemInterface::IsEquipped(uint32 itemid)
{
	for( uint32 x = EQUIPMENT_SLOT_START; x < EQUIPMENT_SLOT_END; ++x )
	{
		if( m_pItems[x] != NULL )
			if( m_pItems[x]->GetProto()->ItemId == itemid )
				return true;
	}
	return false;
}

void ItemInterface::CheckAreaItems()
{
	for( uint32 x = EQUIPMENT_SLOT_START; x < INVENTORY_SLOT_ITEM_END; ++x )
	{
		if( m_pItems[x] != NULL )
		{
			if( IsBagSlot(x) && m_pItems[x]->IsContainer() )
			{
				Container* bag = TO_CONTAINER(m_pItems[x]);
 
				for( uint32 i = 0; i < bag->GetProto()->ContainerSlots; i++ )
				{
					if( bag->GetItem(i) != NULL && bag->GetItem(i)->GetProto() && bag->GetItem(i)->GetProto()->MapID && bag->GetItem(i)->GetProto()->MapID != GetOwner()->GetMapId() )
						bag->SafeFullRemoveItemFromSlot( i );
				}
			}
			else
			{
				if( m_pItems[x]->GetProto() != NULL && m_pItems[x]->GetProto()->MapID && m_pItems[x]->GetProto()->MapID != GetOwner()->GetMapId() )
					SafeFullRemoveItemFromSlot( INVENTORY_SLOT_NOT_SET, x );
			}
		}
	}
}
