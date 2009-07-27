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

#ifndef _LOOTMGR_H
#define _LOOTMGR_H

struct ItemPrototype;
class MapMgr;
class Player;
class LootRoll : public EventableObject, public std::tr1::enable_shared_from_this<LootRoll>
{
public:
	LootRoll();
	~LootRoll();
	void Init(uint32 timer, uint32 groupcount, uint64 guid, uint32 slotid, uint32 itemid, uint32 itemunk1, uint32 itemunk2, MapMgrPointer mgr);
	void PlayerRolled(PlayerPointer player, uint8 choice);
	void Finalize();

	int32 event_GetInstanceID();

private:
	Mutex mLootLock;
	std::map<uint32, uint32> m_NeedRolls;
	std::map<uint32, uint32> m_GreedRolls;
	set<uint32> m_passRolls;
	uint32 _groupcount;
	uint32 _slotid;
	uint32 _itemid;
	uint32 _itemunk1;
	uint32 _itemunk2;
	uint32 _remaining;
	uint64 _guid;
	MapMgrPointer _mgr;
};

typedef vector<pair<RandomProps*, float> > RandomPropertyVector;
typedef vector<pair<ItemRandomSuffixEntry*, float> > RandomSuffixVector;

typedef struct _LootItem
{
	ItemPrototype * itemproto;
	uint32 displayid;
}_LootItem;

typedef std::set<uint32> LooterSet;

typedef struct __LootItem
{
	_LootItem item;
	uint32 iItemsCount;
	RandomProps * iRandomProperty;
	ItemRandomSuffixEntry * iRandomSuffix;
	LootRollPointer roll;
	bool passed;
	LooterSet has_looted;
	uint32 ffa_loot;
}__LootItem;


typedef struct StoreLootItem
{
	_LootItem item;
	float chance;
	float chance2;
	uint32 mincount;
	uint32 maxcount;
	uint32 ffa_loot;
}StoreLootItem;


typedef struct StoreLootList
{
	uint32 count;
	StoreLootItem *items;
}StoreLootList;

struct Loot
{
	std::vector<__LootItem> items;
	uint32 gold;
	LooterSet looters;
	bool HasItems(PlayerPointer Looter);
	bool HasLoot(PlayerPointer Looter);
};

struct tempy
{
	uint32 itemid;
	float chance;
	float chance_2;
	uint32 mincount;
	uint32 maxcount;
	uint32 ffa_loot;
};


//////////////////////////////////////////////////////////////////////////////////////////


typedef HM_NAMESPACE::hash_map<uint32, StoreLootList > LootStore;  

enum PARTY_LOOT
{
	PARTY_LOOT_FFA		= 0,
	PARTY_LOOT_RR		= 1,
	PARTY_LOOT_MASTER	= 2,
	PARTY_LOOT_GROUP	= 3,
	PARTY_LOOT_NBG		= 4,
};
enum PARTY_ROLL
{
	NEED	= 1,
	GREED	= 2,
};

class SERVER_DECL LootMgr : public Singleton < LootMgr >
{
public:
	LootMgr();
	~LootMgr();

	void AddLoot(Loot * loot, uint32 itemid, uint32 mincount, uint32 maxcount, uint32 ffa_loot);
	void FillCreatureLoot(Loot * loot,uint32 loot_id, bool heroic);
	void FillGOLoot(Loot * loot,uint32 loot_id, bool heroic);
	void FillItemLoot(Loot *loot, uint32 loot_id);
	void FillFishingLoot(Loot * loot,uint32 loot_id);
	void FillSkinningLoot(Loot * loot,uint32 loot_id);
	void FillPickpocketingLoot(Loot *loot, uint32 loot_id);
	void FillDisenchantingLoot(Loot *loot, uint32 loot_id);
	void FillProspectingLoot(Loot *loot, uint32 loot_id);
	void FillMillingLoot(Loot *loot, uint32 loot_id);

	bool CanGODrop(uint32 LootId,uint32 itemid);
	bool IsPickpocketable(uint32 creatureId);
	bool IsSkinnable(uint32 creatureId);
	bool IsFishable(uint32 zoneid);

	void LoadLoot();
	void LoadCreatureLoot();
	void LoadLootProp();
	
	LootStore	CreatureLoot;
	LootStore	FishingLoot;
	LootStore	SkinningLoot;
	LootStore	GOLoot;
	LootStore	ItemLoot;
	LootStore	ProspectingLoot;
	LootStore	DisenchantingLoot;
	LootStore	PickpocketingLoot;
	LootStore	MillingLoot;
	std::map<uint32, std::set<uint32> > quest_loot_go;

	RandomProps * GetRandomProperties(ItemPrototype * proto);
	ItemRandomSuffixEntry * GetRandomSuffix(ItemPrototype * proto);

	bool is_loading;
 
	void FillObjectLootMap(map<uint32, vector<uint32> > *dest);

private:
	void LoadLootTables(const char * szTableName,LootStore * LootTable);
	void PushLoot(StoreLootList *list,Loot * loot, bool heroic, bool disenchant);
	
	map<uint32, RandomPropertyVector> _randomprops;
	map<uint32, RandomSuffixVector> _randomsuffix;
};

#define lootmgr LootMgr::getSingleton()

#endif
