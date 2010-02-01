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

#ifndef _ITEMINTERFACE_H
#define _ITEMINTERFACE_H

#define INVALID_BACKPACK_SLOT ((int8)(0xFF)) //In 1.8 client marked wrong slot like this

struct SlotResult
{
	SlotResult() { ContainerSlot = -1, Slot = -1, Result = false; }
	int8 ContainerSlot;
	int8 Slot;
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
	PlayerPointer m_pOwner;
	ItemPointer m_pItems[MAX_INVENTORY_SLOT];
	ItemPointer m_pBuyBack[MAX_BUYBACK_SLOT];

	AddItemResult m_AddItem(ItemPointer item, int8 ContainerSlot, int8 slot);

public:
	friend class ItemIterator;
	ItemInterface( PlayerPointer pPlayer );
	~ItemInterface();

	PlayerPointer GetOwner() { return m_pOwner; }
	bool IsBagSlot(int8 slot);

	uint32 m_CreateForPlayer(ByteBuffer *data);
	void m_DestroyForPlayer();

	void mLoadItemsFromDatabase(QueryResult * result);
	void mSaveItemsToDatabase(bool first, QueryBuffer * buf);

	ItemPointer GetInventoryItem(int8 slot);
	ItemPointer GetInventoryItem(int8 ContainerSlot, int8 slot);
	int8 GetInventorySlotById(uint32 ID);
	int8 GetInventorySlotByGuid(uint64 guid);
	int8 GetBagSlotByGuid(uint64 guid);

	ItemPointer SafeAddItem(uint32 ItemId, int8 ContainerSlot, int8 slot);
	AddItemResult SafeAddItem(ItemPointer pItem, int8 ContainerSlot, int8 slot);
	ItemPointer SafeRemoveAndRetreiveItemFromSlot(int8 ContainerSlot, int8 slot, bool destroy); //doesnt destroy item from memory
	ItemPointer SafeRemoveAndRetreiveItemByGuid(uint64 guid, bool destroy);
	ItemPointer SafeRemoveAndRetreiveItemByGuidRemoveStats(uint64 guid, bool destroy);
	bool SafeFullRemoveItemFromSlot(int8 ContainerSlot, int8 slot); //destroys item fully
	bool SafeFullRemoveItemByGuid(uint64 guid); //destroys item fully
	AddItemResult AddItemToFreeSlot(ItemPointer item);
	AddItemResult AddItemToFreeBankSlot(ItemPointer item);
	
	/** Finds a stack that didn't reach max capacity
	\param itemid The entry of the item to search for
	\param cnt The item count you wish to add to the stack
	\param IncBank Should this search the player's bank as well?
	\return An ItemPointer to a stack of itemid which can contain cnt more items
	*/
	ItemPointer FindItemLessMax(uint32 itemid, uint32 cnt, bool IncBank);
	uint32 GetItemCount(uint32 itemid, bool IncBank = false);
	uint32 RemoveItemAmt(uint32 id, uint32 amt);
	uint32 RemoveItemAmt_ProtectPointer(uint32 id, uint32 amt, ItemPointer * pointer);
	void RemoveAllConjured();
	void BuyItem(ItemPrototype *item, uint32 total_amount, CreaturePointer pVendor, ItemExtendedCostEntry *ec);

	uint32 CalculateFreeSlots(ItemPrototype *proto);
	void ReduceItemDurability();

	uint8 LastSearchItemBagSlot(){return result.ContainerSlot;}
	uint8 LastSearchItemSlot(){return result.Slot;}
	SlotResult *LastSearchResult(){return &result;}

	//Searching functions
	SlotResult FindFreeInventorySlot(ItemPrototype *proto);
	SlotResult FindFreeBankSlot(ItemPrototype *proto);
	SlotResult FindAmmoBag();
	int8 FindFreeBackPackSlot();
	int8 FindFreeKeyringSlot();
	int8 FindSpecialBag(ItemPointer item);


	int8 CanEquipItemInSlot(int8 DstInvSlot, int8 slot, ItemPrototype* item, bool ignore_combat = false, bool skip_2h_check = false);
	int8 CanReceiveItem(ItemPrototype * item, uint32 amount, ItemExtendedCostEntry *ec);
	int8 CanAffordItem(ItemPrototype * item,uint32 amount, CreaturePointer pVendor, ItemExtendedCostEntry *ec);
	int8 GetItemSlotByType(uint32 type);
	ItemPointer GetItemByGUID(uint64 itemGuid);


	void BuildInventoryChangeError(ItemPointer SrcItem, ItemPointer DstItem, uint8 Error);
	void SwapItemSlots(int8 srcslot, int8 dstslot);

	int8 GetInternalBankSlotFromPlayer(int8 islot); //converts inventory slots into 0-x numbers

	//buyback stuff
	ASCENT_INLINE ItemPointer GetBuyBack(int32 slot) 
	{ 
		if(slot >= 0 && slot <= 12)
			return m_pBuyBack[slot];
		else 
			return NULLITEM;
	}
	void AddBuyBackItem(ItemPointer it, uint32 price);
	void RemoveBuyBackItem(uint32 index);
	void EmptyBuyBack();
	bool IsEquipped(uint32 itemid);

	void CheckAreaItems();

public:
	ASCENT_INLINE bool VerifyBagSlots(int8 ContainerSlot, int8 Slot)
	{
		if( ContainerSlot < -1 || Slot < 0 )
			return false;

		if( ContainerSlot > 0 && (ContainerSlot < INVENTORY_SLOT_BAG_START || ContainerSlot >= INVENTORY_SLOT_BAG_END) )
			return false;

		if( ContainerSlot == -1 && (Slot >= INVENTORY_SLOT_ITEM_END  || Slot <= EQUIPMENT_SLOT_END) )
			return false;
			
		return true;
	}

	ASCENT_INLINE bool VerifyBagSlotsWithBank(int8 ContainerSlot, int8 Slot)
	{
		if( ContainerSlot < -1 || Slot < 0 )
			return false;

		if( ContainerSlot > 0 && (ContainerSlot < INVENTORY_SLOT_BAG_START || ContainerSlot >= INVENTORY_SLOT_BAG_END) )
			return false;

		if( ContainerSlot == -1 && (Slot >= MAX_INVENTORY_SLOT || Slot <= EQUIPMENT_SLOT_END) )
			return false;

		return true;
	}

	ASCENT_INLINE bool VerifyBagSlotsWithInv(int8 ContainerSlot, int8 Slot)
	{
		if( ContainerSlot < -1 || Slot < 0 )
			return false;

		if( ContainerSlot > 0 && (ContainerSlot < INVENTORY_SLOT_BAG_START || ContainerSlot >= INVENTORY_SLOT_BAG_END) )
			return false;

		if( ContainerSlot == -1 && Slot >= MAX_INVENTORY_SLOT )
			return false;

		return true;
	}
};

class ItemIterator
{
	bool m_atEnd;
	bool m_searchInProgress;
	uint32 m_slot;
	uint32 m_containerSlot;
	ContainerPointer m_container;
	ItemPointer m_currentItem;
	ItemInterface* m_target;
public:
	ItemIterator(ItemInterface* target) : m_atEnd(false),m_searchInProgress(false),m_slot(0),m_containerSlot(0),m_container(NULLCONTAINER),m_target(target) {}
	~ItemIterator() { if(m_searchInProgress) { EndSearch(); } }

	void BeginSearch()
	{
		// iteminterface doesn't use mutexes, maybe it should :P
		ASSERT(!m_searchInProgress);
		m_atEnd=false;
		m_searchInProgress=true;
		m_container=NULLCONTAINER;
		m_currentItem=NULLITEM;
		m_slot=0;
		Increment();
	}

	void EndSearch()
	{
		// nothing here either
		ASSERT(m_searchInProgress);
		m_atEnd=true;
		m_searchInProgress=false;
	}

	ItemPointer operator*() const
	{
		return m_currentItem;
	}

	ItemPointer operator->() const
	{
		return m_currentItem;
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
			m_container=NULLCONTAINER;
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
					m_currentItem = NULLITEM;

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
		m_atEnd=true;
		m_currentItem=NULLITEM;
	}

	ASCENT_INLINE ItemPointer Grab() { return m_currentItem; }
	ASCENT_INLINE bool End() { return m_atEnd; }
};

#endif
