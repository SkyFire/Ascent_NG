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

#ifndef _CONTAINER_H
#define _CONTAINER_H

class SERVER_DECL Container : public Item
{
public:
	friend class WorldSession;
	Container(uint32 high, uint32 low);
	~Container();
	virtual void Init();
	virtual void Destructor();

	void Create( uint32 itemid, Player* owner );
	void LoadFromDB( Field*fields);

	bool AddItem(int8 slot, Item* item);
	bool AddItemToFreeSlot(Item* pItem, uint32 * r_slot);
	Item* GetItem(int8 slot)
	{
		if(slot >= 0 && (uint8)slot < GetProto()->ContainerSlots)
			return m_Slot[slot];
		else
			return NULL;
	}
	
	int8 FindFreeSlot();
	bool HasItems();
	
	void SwapItems(int8 SrcSlot,int8 DstSlot);
	Item* SafeRemoveAndRetreiveItemFromSlot(int8 slot, bool destroy); //doesnt destroy item from memory
	bool SafeFullRemoveItemFromSlot(int8 slot); //destroys item fully
   
	void SaveBagToDB(int8 slot, bool first, QueryBuffer * buf);

protected:
	Item* m_Slot[72];
	uint32 __fields[CONTAINER_END];
};

#endif
