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

#ifndef _ITEMINTERFACE_H
#define _ITEMINTERFACE_H

#define INVALID_BACKPACK_SLOT ((uint8)(0xFF)) //In 1.8 client marked wrong slot like this

struct SlotResult
{
	SlotResult() { ContainerSlot = -1, Slot = -1, Result = false; }
	uint8 ContainerSlot;
	uint8 Slot;
	bool Result;
};

class Item;
class Container;
class Player;
class UpdateData;
class ByteBuffer;

// sanity checking
enum AddItemResult
{
	ADD_ITEM_RESULT_ERROR			= 0,
	ADD_ITEM_RESULT_OK				= 1,
	ADD_ITEM_RESULT_DUPLICATED		= 2,
};

class SERVER_DECL ItemInterface
{
private:
	SlotResult result;
	Player* m_pOwner;
	Item* m_pItems[MAX_INVENTORY_SLOT];
	Item* m_pBuyBack[MAX_BUYBACK_SLOT];

	AddItemResult m_AddItem(Item* item, uint8 ContainerSlot, uint8 slot);

public:
	friend class ItemIterator;
	ItemInterface( Player* pPlayer );
	~ItemInterface();

	Player* GetOwner() { return m_pOwner; }
	bool IsBagSlot(uint8 slot);

	uint32 m_CreateForPlayer(ByteBuffer *data);
	void m_DestroyForPlayer();

	void mLoadItemsFromDatabase(QueryResult * result);
	void mSaveItemsToDatabase(bool first, QueryBuffer * buf);

	Item* GetInventoryItem(uint8 slot);
	Item* GetInventoryItem(uint8 ContainerSlot, uint8 slot);
	uint8 GetInventorySlotById(uint32 ID);
	uint8 GetInventorySlotByGuid(uint64 guid);
	uint8 GetBagSlotByGuid(uint64 guid);

	Item* SafeAddItem(uint32 ItemId, uint8 ContainerSlot, uint8 slot);
	AddItemResult SafeAddItem(Item* pItem, uint8 ContainerSlot, uint8 slot);
	Item* SafeRemoveAndRetreiveItemFromSlot(uint8 ContainerSlot, uint8 slot, bool destroy); //doesnt destroy item from memory
	Item* SafeRemoveAndRetreiveItemByGuid(uint64 guid, bool destroy);
	Item* SafeRemoveAndRetreiveItemByGuidRemoveStats(uint64 guid, bool destroy);
	bool SafeFullRemoveItemFromSlot(uint8 ContainerSlot, uint8 slot); //destroys item fully
	bool SafeFullRemoveItemByGuid(uint64 guid); //destroys item fully
	AddItemResult AddItemToFreeSlot(Item* item);
	AddItemResult AddItemToFreeBankSlot(Item* item);
	
	/** Finds a stack that didn't reach max capacity
	\param itemid The entry of the item to search for
	\param cnt The item count you wish to add to the stack
	\param IncBank Should this search the player's bank as well?
	\return An Item* to a stack of itemid which can contain cnt more items
	*/
	Item* FindItemLessMax(uint32 itemid, uint32 cnt, bool IncBank);
	uint32 GetItemCount(uint32 itemid, bool IncBank = false);
	uint32 RemoveItemAmt(uint32 id, uint32 amt);
	uint32 RemoveItemAmt_ProtectPointer(uint32 id, uint32 amt, Item* * pointer);
	void RemoveAllConjured();
	void BuyItem(ItemPrototype *item, uint32 total_amount, Creature* pVendor, ItemExtendedCostEntry *ec);

	uint32 CalculateFreeSlots(ItemPrototype *proto);
	void ReduceItemDurability();

	uint8 LastSearchItemBagSlot(){return result.ContainerSlot;}
	uint8 LastSearchItemSlot(){return result.Slot;}
	SlotResult *LastSearchResult(){return &result;}

	//Searching functions
	SlotResult FindFreeInventorySlot(ItemPrototype *proto);
	SlotResult FindFreeBankSlot(ItemPrototype *proto);
	SlotResult FindAmmoBag();
	uint8 FindFreeBackPackSlot();
	uint8 FindFreeKeyringSlot();
	uint8 FindSpecialBag(Item* item);


	uint8 CanEquipItemInSlot(uint8 DstInvSlot, uint8 slot, ItemPrototype* item, bool ignore_combat = false, bool skip_2h_check = false);
	uint8 CanReceiveItem(ItemPrototype * item, uint32 amount, ItemExtendedCostEntry *ec);
	uint8 CanAffordItem(ItemPrototype * item,uint32 amount, Creature* pVendor, ItemExtendedCostEntry *ec);
	uint8 GetItemSlotByType(uint32 type);
	Item* GetItemByGUID(uint64 itemGuid);


	void BuildInventoryChangeError(Item* SrcItem, Item* DstItem, uint8 Error);
	void SwapItemSlots(uint8 srcslot, uint8 dstslot);

	uint8 GetInternalBankSlotFromPlayer(uint8 islot); //converts inventory slots into 0-x numbers

	//buyback stuff
	ASCENT_INLINE Item* GetBuyBack(int32 slot) 
	{ 
		if(slot >= 0 && slot <= 12)
			return m_pBuyBack[slot];
		else 
			return NULL;
	}
	void AddBuyBackItem(Item* it, uint32 price);
	void RemoveBuyBackItem(uint32 index);
	void EmptyBuyBack();
	bool IsEquipped(uint32 itemid);

	void CheckAreaItems();

public:
	ASCENT_INLINE bool VerifyBagSlots(uint8 ContainerSlot, uint8 Slot)
	{
		if( ContainerSlot > 0 && (ContainerSlot < INVENTORY_SLOT_BAG_START || ContainerSlot >= INVENTORY_SLOT_BAG_END) )
			return false;

		if( ContainerSlot == ((uint8)-1) && (Slot >= INVENTORY_SLOT_ITEM_END  || Slot <= EQUIPMENT_SLOT_END) )
			return false;
			
		return true;
	}

	ASCENT_INLINE bool VerifyBagSlotsWithBank(uint8 ContainerSlot, uint8 Slot)
	{
		if( ContainerSlot > 0 && (ContainerSlot < INVENTORY_SLOT_BAG_START || ContainerSlot >= INVENTORY_SLOT_BAG_END) )
			return false;

		if( ContainerSlot == ((uint8)-1) && (Slot >= MAX_INVENTORY_SLOT || Slot <= EQUIPMENT_SLOT_END) )
			return false;

		return true;
	}

	ASCENT_INLINE bool VerifyBagSlotsWithInv(uint8 ContainerSlot, uint8 Slot)
	{
		if( ContainerSlot > 0 && (ContainerSlot < INVENTORY_SLOT_BAG_START || ContainerSlot >= INVENTORY_SLOT_BAG_END) )
			return false;

		if( ContainerSlot == ((uint8)-1) && Slot >= MAX_INVENTORY_SLOT )
			return false;

		return true;
	}
};

class ItemIterator
{
	bool m_atEnd;
	bool m_searchInProgress;
	uint8 m_slot;
	uint8 m_containerSlot;
	Container*  m_container;
	Item*  m_currentItem;
	ItemInterface* m_target;
public:
	ItemIterator(ItemInterface* target) : m_atEnd(false), m_searchInProgress(false), m_slot(0), m_containerSlot(0), m_container(NULL), m_target(target) {}
	~ItemIterator() { if(m_searchInProgress) { EndSearch(); } }

	void BeginSearch()
	{
		// iteminterface doesn't use mutexes, maybe it should :P
		ASSERT(!m_searchInProgress);
		m_atEnd = false;
		m_searchInProgress = true;
		m_container = NULL;
		m_currentItem = NULL;
		m_slot = 0;
		Increment();
	}

	void EndSearch()
	{
		// nothing here either
		ASSERT(m_searchInProgress);
		m_atEnd=true;
		m_searchInProgress=false;
	}

	Item* operator*() const
	{
		return m_currentItem;
	}

	Item* operator->() const
	{
		return m_currentItem;
	}

	ItemIterator* operator++()
	{
		Increment();
		return this;
	}

	void Increment()
	{
		if(!m_searchInProgress)
			BeginSearch();

		// are we currently inside a container?
		if(m_container != NULL)
		{
			// loop the container.
			for(; m_containerSlot < m_container->GetProto()->ContainerSlots; ++m_containerSlot)
			{
				m_currentItem = m_container->GetItem(m_containerSlot);
				if(m_currentItem != NULL)
				{
					// increment the counter so we don't get the same item again
					++m_containerSlot;

					// exit
					return;
				}
			}

			// unset this
			m_container = NULL;
		}

		for(; m_slot < MAX_INVENTORY_SLOT; ++m_slot)
		{
			if(m_target->m_pItems[m_slot])
			{
				if(m_target->m_pItems[m_slot]->IsContainer())
				{
					// we are a container :O lets look inside the box!
					m_container = TO_CONTAINER(m_target->m_pItems[m_slot]);
					m_containerSlot = 0;

					// clear the pointer up. so we can tell if we found an item or not
					m_currentItem = NULL;

					// increment m_slot so we don't search this container again
					++m_slot;

					// call increment() recursively. this will search the container.
					Increment();

					// jump out so we're not wasting cycles and skipping items
					return;
				}

				// we're not a container, just a regular item
				// set the pointer
				m_currentItem = m_target->m_pItems[m_slot];

				// increment the slot counter so we don't do the same item again
				++m_slot;

				// jump out
				return;
			}
		}

		// if we're here we've searched all items.
		m_atEnd = true;
		m_currentItem = NULL;
	}

	ASCENT_INLINE uint8 GetCurrentContainerSlot() { return m_containerSlot; }
	ASCENT_INLINE uint8 GetCurrentSlot() { return m_slot; }

	ASCENT_INLINE Item* Grab() { return m_currentItem; }
	ASCENT_INLINE bool End() { return m_atEnd; }
};

#endif
