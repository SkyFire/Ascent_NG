/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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

#ifndef WOWSERVER_CREATURE_H
#define WOWSERVER_CREATURE_H

class CreatureTemplate;
class GossipScript;

#define MAX_CREATURE_ITEMS 128
#define MAX_CREATURE_LOOT 8
#define MAX_PET_SPELL 4
#define VENDOR_ITEMS_UPDATE_TIME 3600000
#include "Map.h"

struct CreatureItem
{
	uint32 itemid;
	uint32 amount; //!!!!! stack amount.
	uint32 available_amount;
	uint32 max_amount;
	uint32 incrtime;
	ItemExtendedCostEntry *extended_cost;
};

SERVER_DECL bool Rand(float chance);
SERVER_DECL bool Rand(uint32 chance);
SERVER_DECL bool Rand(int32 chance);
#pragma pack(push,1)
struct CreatureInfo
{
	uint32 Id;
	char * Name;
	char * SubName;
	char * info_str;
	uint32 Flags1;
	uint32 Type;
	uint32 Family;
	uint32 Rank;
	uint32 Unknown1;
	uint32 SpellDataID;
	uint32 Male_DisplayID;
	uint32 Female_DisplayID;
	uint32 unkint1;
	uint32 unkint2;
	float unkfloat1;
	float unkfloat2;
	uint8  Civilian;
	uint8  Leader;

	std::string lowercase_name;
	GossipScript * gossip_script;
	uint32 GenerateModelId(uint32 * dest)
	{
		/* only M */
        if(Male_DisplayID == Female_DisplayID)
		{
			*dest = Male_DisplayID;
			return 0;
		}

		/* only M */
		if(Male_DisplayID && !Female_DisplayID)
		{
            *dest = Male_DisplayID;
			return 0;
		}

		/* only F */
		if(!Male_DisplayID && Female_DisplayID)
		{
			*dest = Female_DisplayID;
			return 1;
		}

		/* make a random one */
		if(Rand(50.0f))
		{
			*dest = Female_DisplayID;
			return 1;
		}
		else
		{
			*dest = Male_DisplayID;
			return 0;
		}
	}
};

struct CreatureProto
{
	uint32 Id;
	uint32 MinLevel;
	uint32 MaxLevel;
	uint32 Faction;
	uint32 MinHealth;
	uint32 MaxHealth;
	uint32 Mana;
	float  Scale;
	uint32	NPCFLags;
	uint32 AttackTime;
	uint32 AttackType;
	float MinDamage;
	float MaxDamage;
	uint32 RangedAttackTime;
	float RangedMinDamage;
	float RangedMaxDamage;
	uint32 MountedDisplayID;
	uint32 Item1SlotDisplay;
	uint32 Item1Info1;
	uint32 Item1Info2;
	uint32 Item2SlotDisplay;
	uint32 Item2Info1;
	uint32 Item2Info2;
	uint32 Item3SlotDisplay;
	uint32 Item3Info1;
	uint32 Item3Info2;
	uint32 RespawnTime;
	uint32 Resistances[7];
	float CombatReach;
	float BoundingRadius;
	char * aura_string;
	uint32 boss;
	uint32 money;
	uint32 invisibility_type;
	uint32 death_state;
	float	walk_speed;//base movement
	float	run_speed;//most of the time mobs use this
	float fly_speed;
	uint32 extra_a9_flags;

	/* AI Stuff */
	bool m_canRangedAttack;
	bool m_canFlee;
	float m_fleeHealth;
	uint32 m_fleeDuration;
	bool m_canCallForHelp;
	float m_callForHelpHealth;

	set<uint32> start_auras;
	list<AI_Spell*> spells;
};
#pragma pack(pop)

struct Formation{
	uint32 fol;
	float ang;
	float dist;
};
 
   
enum UNIT_TYPE
{
	NOUNITTYPE	  = 0,
	BEAST		   = 1,
	DRAGONSKIN	  = 2,
	DEMON		   = 3,
	ELEMENTAL	   = 4,
	GIANT		   = 5,
	UNDEAD		  = 6,
	HUMANOID		= 7,
	CRITTER		 = 8,
	MECHANICAL	  = 9,
	UNIT_TYPE_MISC  = 10,
};

enum FAMILY
{
	FAMILY_WOLF = 1,
	FAMILY_CAT,
	FAMILY_SPIDER,
	FAMILY_BEAR,
	FAMILY_BOAR,
	FAMILY_CROCILISK,
	FAMILY_CARRION_BIRD,
	FAMILY_CRAB,
	FAMILY_GORILLA,
	FAMILY_RAPTOR = 11,
	FAMILY_TALLSTRIDER ,
	FAMILY_FELHUNTER = 15,
	FAMILY_VOIDWALKER,
	FAMILY_SUCCUBUS,
	FAMILY_DOOMGUARD = 19,
	FAMILY_SCORPID,
	FAMILY_TURTLE, 
	FAMILY_IMP = 23,
	FAMILY_BAT,
	FAMILY_HYENA,
	FAMILY_OWL,
	FAMILY_WIND_SERPENT,
	FAMILY_REMOTE_CONTROL,
	FAMILY_FELGUARD,
	FAMILY_DRAGONHAWK,
	FAMILY_RAVAGER,
	FAMILY_WARP_STALKER,
	FAMILY_SPOREBAT,
	FAMILY_NETHER_RAY,
	FAMILY_SERPENT
};

enum ELITE
{
	ELITE_NORMAL = 0,
	ELITE_ELITE,
	ELITE_RAREELITE,
	ELITE_WORLDBOSS,
	ELITE_RARE
};
enum TIME_REMOVE_CORPSE
{
	TIME_CREATURE_REMOVE_CORPSE = 180000,
	TIME_CREATURE_REMOVE_RARECORPSE = 180000*3,
	TIME_CREATURE_REMOVE_BOSSCORPSE = 180000*5,
};

struct PetSpellCooldown
{
	uint32 spellId;
	int32 cooldown;
};

class CreatureAIScript;
class GossipScript;
class AuctionHouse;
struct Trainer;
#define CALL_SCRIPT_EVENT(obj, func) if(obj->GetTypeId() == TYPEID_UNIT && static_cast<Creature*>(obj)->GetScript() != NULL) static_cast<Creature*>(obj)->GetScript()->func

///////////////////
/// Creature object

class SERVER_DECL Creature : public Unit
{
public:

	Creature(uint64 guid);
	virtual ~Creature();
    bool Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info);
	void Load(CreatureProto * proto_, float x, float y, float z, float o);

	void AddToWorld();
	void AddToWorld(MapMgr * pMapMgr);
	void RemoveFromWorld(bool addrespawnevent, bool free_guid);

	/// Creation
	void Create ( const char* creature_name, uint32 mapid, float x, float y, float z, float ang);	
	void CreateWayPoint ( uint32 WayPointID, uint32 mapid, float x, float y, float z, float ang);
	

	/// Updates
	virtual void Update( uint32 time );

	ASCENT_INLINE CreatureInfo* GetCreatureInfo() { return creature_info; }

	/// Creature inventory
	ASCENT_INLINE uint32 GetItemIdBySlot(uint32 slot) { return m_SellItems->at(slot).itemid; }
	ASCENT_INLINE uint32 GetItemAmountBySlot(uint32 slot) { return m_SellItems->at(slot).amount; }

	ASCENT_INLINE bool HasItems() { return ((m_SellItems != NULL) ? true : false); }

	int32 GetSlotByItemId(uint32 itemid)
	{
		uint32 slot = 0;
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
				return slot;
			else
				++slot;			
		}
		return -1;
	}

	uint32 GetItemAmountByItemId(uint32 itemid)
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
				return ((itr->amount < 1) ? 1 : itr->amount);
		}
		return 0;
	}

	ASCENT_INLINE void GetSellItemBySlot(uint32 slot, CreatureItem &ci)
	{
		ci = m_SellItems->at(slot);
	}

	void GetSellItemByItemId(uint32 itemid, CreatureItem &ci)
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
			{
				ci = (*itr);   
				return;
			}
		}
		ci.amount = 0;
		ci.max_amount=0;
		ci.available_amount =0;
		ci.incrtime=0;
		ci.itemid = 0;
	}

	ASCENT_INLINE std::vector<CreatureItem>::iterator GetSellItemBegin() { return m_SellItems->begin(); }
	ASCENT_INLINE std::vector<CreatureItem>::iterator GetSellItemEnd()   { return m_SellItems->end(); }
	ASCENT_INLINE size_t GetSellItemCount() { return m_SellItems->size(); }
	void RemoveVendorItem(uint32 itemid)
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
			{
				m_SellItems->erase(itr);
				return;
			}
		}
	}
	void AddVendorItem(uint32 itemid, uint32 amount);
	void ModAvItemAmount(uint32 itemid, uint32 value);
	void UpdateItemAmount(uint32 itemid);
	/// Quests
	void _LoadQuests();
	bool HasQuests() { return m_quests != NULL; };
	bool HasQuest(uint32 id, uint32 type)
	{
		if(!m_quests) return false;
		for(std::list<QuestRelation*>::iterator itr = m_quests->begin(); itr != m_quests->end(); ++itr)
		{
			if((*itr)->qst->id == id && (*itr)->type & type)
				return true;
		}
		return false;
	}
	void AddQuest(QuestRelation *Q);
	void DeleteQuest(QuestRelation *Q);
	Quest *FindQuest(uint32 quest_id, uint8 quest_relation);
	uint16 GetQuestRelation(uint32 quest_id);
	uint32 NumOfQuests();
	list<QuestRelation *>::iterator QuestsBegin() { return m_quests->begin(); };
	list<QuestRelation *>::iterator QuestsEnd() { return m_quests->end(); };
	void SetQuestList(std::list<QuestRelation *>* qst_lst) { m_quests = qst_lst; };

	ASCENT_INLINE bool isQuestGiver() { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER ); };
	
	int32 FlatResistanceMod[7];
	int32 BaseResistanceModPct[7];
	int32 ResistanceModPct[7];
	
	int32 FlatStatMod[5];
	int32 StatModPct[5];
	int32 TotalStatModPct[5];
	
	int32 ModDamageDone[7];
	float ModDamageDonePct[7];
	void CalcResistance(uint32 type);
	void CalcStat(uint32 type);
	bool m_canRegenerateHP;
	void RegenerateHealth();
	void RegenerateMana();
	int BaseAttackType;

	bool CanSee(Unit* obj) // * Invisibility & Stealth Detection - Partha *
	{
		if(!obj)
			return false;

		if(obj->m_invisible) // Invisibility - Detection of Players and Units
		{
			if(obj->getDeathState() == CORPSE) // can't see dead players' spirits
				return false;

			if(m_invisDetect[obj->m_invisFlag] < 1) // can't see invisible without proper detection
				return false;
		}

		if(obj->IsStealth()) // Stealth Detection (  I Hate Rogues :P  )
		{
			if(isInFront(obj)) // stealthed player is in front of creature
			{
				// Detection Range = 5yds + (Detection Skill - Stealth Skill)/5
				if(getLevel() < 60)
					detectRange = 5.0f + getLevel() + 0.2f * (float)(GetStealthDetectBonus() - obj->GetStealthLevel());
				else
					detectRange = 65.0f + 0.2f * (float)(GetStealthDetectBonus() - obj->GetStealthLevel());

				if(detectRange < 1.0f) detectRange = 1.0f; // Minimum Detection Range = 1yd
			}
			else // stealthed player is behind creature
			{
				if(GetStealthDetectBonus() > 1000) return true; // immune to stealth
				else detectRange = 0.0f;
			}	

			detectRange += GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of creature
			detectRange += obj->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of stealthed player

			if(GetDistance2dSq(obj) > detectRange * detectRange)
				return false;
		}

		return true;
	}

	//Make this unit face another unit
	bool setInFront(Unit* target);

	bool Skinned;

	/// Misc
	ASCENT_INLINE void setEmoteState(uint8 emote) { m_emoteState = emote; };

	virtual void setDeathState(DeathState s);

	uint32 GetOldEmote() { return m_oldEmote; }

	// Serialization
	void SaveToDB();
	void SaveToFile(std::stringstream & name);
	//bool LoadFromDB(uint32 guid);
	//bool LoadFromDB(CreatureTemplate *t);
	void LoadAIAgents(CreatureTemplate * t);
	void LoadAIAgents();
	void DeleteFromDB();

	void OnJustDied();
	void OnRemoveCorpse();
	void OnRespawn(MapMgr * m);
	void SafeDelete();
	//void Despawn();
	void SummonExpire(); // this is used for guardians. They are non respawnable creatures linked to a player


	// In Range
	void AddInRangeObject(Object* pObj);
	void OnRemoveInRangeObject(Object* pObj);
	void ClearInRangeSet();

	// Demon
	void EnslaveExpire();

	// Pet
	void UpdatePet();
	uint32 GetEnslaveCount() { return m_enslaveCount; }
	void SetEnslaveCount(uint32 count) { m_enslaveCount = count; }
	uint32 GetEnslaveSpell() { return m_enslaveSpell; }
	void SetEnslaveSpell(uint32 spellId) { m_enslaveSpell = spellId; }
	bool RemoveEnslave();

	ASCENT_INLINE Player *GetTotemOwner() { return totemOwner; }
	ASCENT_INLINE void SetTotemOwner(Player *owner) { totemOwner = owner; }
	ASCENT_INLINE uint32 GetTotemSlot() { return totemSlot; }
	ASCENT_INLINE void SetTotemSlot(uint32 slot) { totemSlot = slot; }

	ASCENT_INLINE bool IsPickPocketed() { return m_PickPocketed; }
	ASCENT_INLINE void SetPickPocketed(bool val = true) { m_PickPocketed = val; }

	ASCENT_INLINE CreatureAIScript * GetScript() { return _myScriptClass; }
	void LoadScript();

	void CallScriptUpdate();

	uint32 m_TaxiNode;
	CreatureInfo *creature_info;
	ASCENT_INLINE CreatureInfo *GetCreatureName()
	{
		return creature_info; 
	}
	ASCENT_INLINE void SetCreatureName(CreatureInfo *ci) { creature_info = ci; }
	ASCENT_INLINE Trainer* GetTrainer() { return mTrainer; }
	void RegenerateFocus();

	CreatureFamilyEntry * myFamily;
	ASCENT_INLINE bool IsTotem() { return totemOwner != 0 && totemSlot != -1; }
	void TotemExpire();
	void FormationLinkUp(uint32 SqlId);
	void ChannelLinkUpGO(uint32 SqlId);
	void ChannelLinkUpCreature(uint32 SqlId);
	bool haslinkupevent;
	WayPoint * CreateWaypointStruct();
	uint32 original_emotestate;
	CreatureProto * proto;
	ASCENT_INLINE CreatureProto *GetProto() { return proto; }
	CreatureSpawn * m_spawn;
	void OnPushToWorld();
	void Despawn(uint32 delay, uint32 respawntime);
	void TriggerScriptEvent(string func);

	AuctionHouse * auctionHouse;
	bool has_waypoint_text;
	bool has_combat_text;

	void DeleteMe();
	bool CanAddToWorld();

	WayPointMap * m_custom_waypoint_map;
	Player * m_escorter;
	void DestroyCustomWaypointMap();
	bool IsInLimboState() { return m_limbostate; }
	uint32 GetLineByFamily(CreatureFamilyEntry * family){return family->skilline ? family->skilline : 0;};
	void RemoveLimboState(Unit * healer);
	void SetGuardWaypoints();
	bool m_corpseEvent;
	MapCell * m_respawnCell;
	bool m_noRespawn;
	LocationVector * m_transportPosition;
	uint32 m_transportGuid;
protected:
	CreatureAIScript *_myScriptClass;
	bool m_limbostate;
	Trainer* mTrainer;

	void _LoadGoods();
	void _LoadGoods(std::list<CreatureItem*>* lst);
	void _LoadMovement();

	/// Vendor data
	std::vector<CreatureItem>* m_SellItems;

	/// Taxi data
	uint32 mTaxiNode;

	/// Quest data
	std::list<QuestRelation *>* m_quests;
   
	/// Pet
	uint32 m_enslaveCount;
	uint32 m_enslaveSpell;

	Player * totemOwner;
	int32 totemSlot;

	bool m_PickPocketed;
	uint32 _fields[UNIT_END];

public:
	// loooooot
	void GenerateLoot();

	// tagging
	uint32 m_taggingGroup;
	uint32 m_taggingPlayer;
	int8 m_lootMethod;

	// updates the loot state, whether it is tagged or lootable, or no longer has items
	void UpdateLootAnimation();

	// clears tag, clears "tagged" visual grey
	void ClearTag();

	// tags the object by a certain player.
	void Tag(Player *plr);

	// used by bgs
	bool m_noDeleteAfterDespawn;
};

#endif
