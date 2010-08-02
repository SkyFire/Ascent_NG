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

#ifndef _ITEMPROTOTYPE_H
#define _ITEMPROTOTYPE_H
class Spell;

#define GEM_META_SOCKET 1
#define GEM_RED_SOCKET 2
#define GEM_YELLOW_SOCKET 4
#define GEM_BLUE_SOCKET 8

#define MAX_INVENTORY_SLOT 118
#define MAX_BUYBACK_SLOT 12

#define ITEM_NO_SLOT_AVAILABLE -1 //works for all kind of slots now
#define INVENTORY_SLOT_NOT_SET -1

#define EQUIPMENT_SLOT_START		 0
#define EQUIPMENT_SLOT_HEAD		  0
#define EQUIPMENT_SLOT_NECK		  1
#define EQUIPMENT_SLOT_SHOULDERS	 2
#define EQUIPMENT_SLOT_BODY		  3
#define EQUIPMENT_SLOT_CHEST		 4
#define EQUIPMENT_SLOT_WAIST		 5
#define EQUIPMENT_SLOT_LEGS		  6
#define EQUIPMENT_SLOT_FEET		  7
#define EQUIPMENT_SLOT_WRISTS		8
#define EQUIPMENT_SLOT_HANDS		 9
#define EQUIPMENT_SLOT_FINGER1	   10
#define EQUIPMENT_SLOT_FINGER2	   11
#define EQUIPMENT_SLOT_TRINKET1	  12
#define EQUIPMENT_SLOT_TRINKET2	  13
#define EQUIPMENT_SLOT_BACK		  14
#define EQUIPMENT_SLOT_MAINHAND	  15
#define EQUIPMENT_SLOT_OFFHAND	   16
#define EQUIPMENT_SLOT_RANGED		17
#define EQUIPMENT_SLOT_TABARD		18
#define EQUIPMENT_SLOT_END		   19

#define INVENTORY_SLOT_BAG_START	 19
#define INVENTORY_SLOT_BAG_1		 19
#define INVENTORY_SLOT_BAG_2		 20
#define INVENTORY_SLOT_BAG_3		 21
#define INVENTORY_SLOT_BAG_4		 22
#define INVENTORY_SLOT_BAG_END	   23

#define INVENTORY_SLOT_ITEM_START	23
#define INVENTORY_SLOT_ITEM_1		23
#define INVENTORY_SLOT_ITEM_2		24
#define INVENTORY_SLOT_ITEM_3		25
#define INVENTORY_SLOT_ITEM_4		26
#define INVENTORY_SLOT_ITEM_5		27
#define INVENTORY_SLOT_ITEM_6		28
#define INVENTORY_SLOT_ITEM_7		29
#define INVENTORY_SLOT_ITEM_8		30
#define INVENTORY_SLOT_ITEM_9		31
#define INVENTORY_SLOT_ITEM_10	   32
#define INVENTORY_SLOT_ITEM_11	   33
#define INVENTORY_SLOT_ITEM_12	   34
#define INVENTORY_SLOT_ITEM_13	   35
#define INVENTORY_SLOT_ITEM_14	   36
#define INVENTORY_SLOT_ITEM_15	   37
#define INVENTORY_SLOT_ITEM_16	   38
#define INVENTORY_SLOT_ITEM_END	  39

#define BANK_SLOT_ITEM_START		 39
#define BANK_SLOT_ITEM_1			 39
#define BANK_SLOT_ITEM_2			 40
#define BANK_SLOT_ITEM_3			 41
#define BANK_SLOT_ITEM_4			 42
#define BANK_SLOT_ITEM_5			 43
#define BANK_SLOT_ITEM_6			 44
#define BANK_SLOT_ITEM_7			 45
#define BANK_SLOT_ITEM_8			 46
#define BANK_SLOT_ITEM_9			 47
#define BANK_SLOT_ITEM_10			48
#define BANK_SLOT_ITEM_11			49
#define BANK_SLOT_ITEM_12			50
#define BANK_SLOT_ITEM_13			51
#define BANK_SLOT_ITEM_14			52
#define BANK_SLOT_ITEM_15			53
#define BANK_SLOT_ITEM_16			54
#define BANK_SLOT_ITEM_17			55
#define BANK_SLOT_ITEM_18			56
#define BANK_SLOT_ITEM_19			57
#define BANK_SLOT_ITEM_20			58
#define BANK_SLOT_ITEM_21			59
#define BANK_SLOT_ITEM_22			60
#define BANK_SLOT_ITEM_23			61
#define BANK_SLOT_ITEM_24			62
#define BANK_SLOT_ITEM_25			63
#define BANK_SLOT_ITEM_26			64
#define BANK_SLOT_ITEM_27			65
#define BANK_SLOT_ITEM_28			66
#define BANK_SLOT_ITEM_END		   67

#define BANK_SLOT_BAG_START		  67
#define BANK_SLOT_BAG_1			  67
#define BANK_SLOT_BAG_2			  68
#define BANK_SLOT_BAG_3			  69
#define BANK_SLOT_BAG_4			  70
#define BANK_SLOT_BAG_5			  71
#define BANK_SLOT_BAG_6			  72
#define BANK_SLOT_BAG_7			  73
#define BANK_SLOT_BAG_END			74

#define INVENTORY_KEYRING_START	  86
#define INVENTORY_KEYRING_1		  86
#define INVENTORY_KEYRING_2		  87
#define INVENTORY_KEYRING_3		  88
#define INVENTORY_KEYRING_4		  89
#define INVENTORY_KEYRING_5		  90
#define INVENTORY_KEYRING_6		  91
#define INVENTORY_KEYRING_7		  92
#define INVENTORY_KEYRING_8		  93
#define INVENTORY_KEYRING_9		  94
#define INVENTORY_KEYRING_10		 95
#define INVENTORY_KEYRING_11		 96
#define INVENTORY_KEYRING_12		 97
#define INVENTORY_KEYRING_13		 98
#define INVENTORY_KEYRING_14		 99
#define INVENTORY_KEYRING_15		 100
#define INVENTORY_KEYRING_16		 101
#define INVENTORY_KEYRING_17		 102
#define INVENTORY_KEYRING_18		 103
#define INVENTORY_KEYRING_19		 104
#define INVENTORY_KEYRING_20		 105
#define INVENTORY_KEYRING_21		 106
#define INVENTORY_KEYRING_22		 107
#define INVENTORY_KEYRING_23		 108
#define INVENTORY_KEYRING_24		 109
#define INVENTORY_KEYRING_25		 110
#define INVENTORY_KEYRING_26		 111
#define INVENTORY_KEYRING_27		 112
#define INVENTORY_KEYRING_28		 113
#define INVENTORY_KEYRING_29		 114
#define INVENTORY_KEYRING_30		 115
#define INVENTORY_KEYRING_31		 116
#define INVENTORY_KEYRING_32		 117
#define INVENTORY_KEYRING_END		118

enum INV_ERR
{
	INV_ERR_OK,
	INV_ERR_YOU_MUST_REACH_LEVEL_N,
	INV_ERR_SKILL_ISNT_HIGH_ENOUGH,
	INV_ERR_ITEM_DOESNT_GO_TO_SLOT,
	INV_ERR_BAG_FULL,
	INV_ERR_NONEMPTY_BAG_OVER_OTHER_BAG,
	INV_ERR_CANT_TRADE_EQUIP_BAGS,
	INV_ERR_ONLY_AMMO_CAN_GO_HERE,
	INV_ERR_NO_REQUIRED_PROFICIENCY,
	INV_ERR_NO_EQUIPMENT_SLOT_AVAILABLE,
	INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,
	INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM2,
	INV_ERR_NO_EQUIPMENT_SLOT_AVAILABLE2,
	INV_ERR_CANT_EQUIP_WITH_TWOHANDED,
	INV_ERR_CANT_DUAL_WIELD,
	INV_ERR_ITEM_DOESNT_GO_INTO_BAG,
	INV_ERR_ITEM_DOESNT_GO_INTO_BAG2,
	INV_ERR_CANT_CARRY_MORE_OF_THIS,
	INV_ERR_NO_EQUIPMENT_SLOT_AVAILABLE3,
	INV_ERR_ITEM_CANT_STACK,
	INV_ERR_ITEM_CANT_BE_EQUIPPED,
	INV_ERR_ITEMS_CANT_BE_SWAPPED,
	INV_ERR_SLOT_IS_EMPTY,
	INV_ERR_ITEM_NOT_FOUND,
	INV_ERR_CANT_DROP_SOULBOUND,
	INV_ERR_OUT_OF_RANGE,
	INV_ERR_TRIED_TO_SPLIT_MORE_THAN_COUNT,
	INV_ERR_COULDNT_SPLIT_ITEMS,
	INV_ERR_MISSING_REAGENT,
	INV_ERR_NOT_ENOUGH_MONEY,
	INV_ERR_NOT_A_BAG,
	INV_ERR_CAN_ONLY_DO_WITH_EMPTY_BAGS,
	INV_ERR_DONT_OWN_THAT_ITEM,
	INV_ERR_CAN_EQUIP_ONLY1_QUIVER,
	INV_ERR_MUST_PURCHASE_THAT_BAG_SLOT,
	INV_ERR_TOO_FAR_AWAY_FROM_BANK,
	INV_ERR_ITEM_LOCKED,
	INV_ERR_YOU_ARE_STUNNED,
	INV_ERR_YOU_ARE_DEAD,
	INV_ERR_CANT_DO_RIGHT_NOW,
	INV_ERR_BAG_FULL2,
	INV_ERR_CAN_EQUIP_ONLY1_QUIVER2,
	INV_ERR_CAN_EQUIP_ONLY1_AMMOPOUCH,
	INV_ERR_STACKABLE_CANT_BE_WRAPPED,
	INV_ERR_EQUIPPED_CANT_BE_WRAPPED,
	INV_ERR_WRAPPED_CANT_BE_WRAPPED,
	INV_ERR_BOUND_CANT_BE_WRAPPED,
	INV_ERR_UNIQUE_CANT_BE_WRAPPED,
	INV_ERR_BAGS_CANT_BE_WRAPPED,
	INV_ERR_ALREADY_LOOTED,
	INV_ERR_INVENTORY_FULL,
	INV_ERR_BANK_FULL,
	INV_ERR_ITEM_IS_CURRENTLY_SOLD_OUT,
	INV_ERR_BAG_FULL3,
	INV_ERR_ITEM_NOT_FOUND2,
	INV_ERR_ITEM_CANT_STACK2,
	INV_ERR_BAG_FULL4,
	INV_ERR_ITEM_SOLD_OUT,
	INV_ERR_OBJECT_IS_BUSY,
	INV_ERR_NONE,
	INV_ERR_CANT_DO_IN_COMBAT,
	INV_ERR_CANT_DO_WHILE_DISARMED,
	INV_ERR_BAG_FULL6,
	INV_ERR_ITEM_RANK_NOT_ENOUGH,
	INV_ERR_ITEM_REPUTATION_NOT_ENOUGH,
	INV_ERR_MORE_THAN1_SPECIAL_BAG,
};

enum ITEM_STAT_TYPE
{
	POWER	  = 0,
	HEALTH	 = 1,
	UNKNOWN	= 2,
	AGILITY	= 3,
	STRENGTH   = 4,
	INTELLECT  = 5,
	SPIRIT	 = 6,
	STAMINA	= 7,
	WEAPON_SKILL_RATING = 11,
	DEFENSE_RATING = 12,
	DODGE_RATING   = 13,
	PARRY_RATING   = 14,
	SHIELD_BLOCK_RATING = 15,
	MELEE_HIT_RATING	= 16,
	RANGED_HIT_RATING   = 17,
	SPELL_HIT_RATING	= 18,
	MELEE_CRITICAL_STRIKE_RATING = 19,
	RANGED_CRITICAL_STRIKE_RATING = 20,
	SPELL_CRITICAL_STRIKE_RATING  = 21,
	MELEE_HIT_AVOIDANCE_RATING	= 22,
	RANGED_HIT_AVOIDANCE_RATING   = 23,
	SPELL_HIT_AVOIDANCE_RATING	= 24,
	MELEE_CRITICAL_AVOIDANCE_RATING = 25,
	RANGED_CRITICAL_AVOIDANCE_RATING = 26,
	SPELL_CRITICAL_AVOIDANCE_RATING  = 27,
	MELEE_HASTE_RATING			   = 28,
	RANGED_HASTE_RATING			  = 29,
	SPELL_HASTE_RATING			   = 30,
	HIT_RATING					   = 31,
	CRITICAL_STRIKE_RATING		   = 32,
	HIT_AVOIDANCE_RATING			 = 33,
	EXPERTISE_RATING				= 34,
	RESILIENCE_RATING				= 35,
	HASTE_RATING					 = 36,
	EXPERTISE_RATING_2				 = 37,
};

enum ITEM_DAMAGE_TYPE
{
	NORMAL_DAMAGE	= 0,
	HOLY_DAMAGE		= 1,
	FIRE_DAMAGE		= 2,
	NATURE_DAMAGE	= 3,
	FROST_DAMAGE	= 4,
	SHADOW_DAMAGE	= 5,
	ARCANE_DAMAGE	= 6,
};

enum ITEM_SPELLTRIGGER_TYPE
{
	USE				= 0,
	ON_EQUIP		= 1,
	CHANCE_ON_HIT	= 2,
	SOULSTONE		= 4,
	LEARNING		= 6,
};

enum ITEM_BONDING_TYPE
{
	ITEM_BIND_NONE		=   0, 
	ITEM_BIND_ON_PICKUP =   1,
	ITEM_BIND_ON_EQUIP  =   2,
	ITEM_BIND_ON_USE	=   3,
	ITEM_BIND_QUEST	    =   4,
	ITEM_BIND_QUEST2    =   5,
};

enum INVENTORY_TYPES
{
	INVTYPE_NON_EQUIP       = 0x0,
	INVTYPE_HEAD            = 0x1,
	INVTYPE_NECK            = 0x2,
	INVTYPE_SHOULDERS       = 0x3,
	INVTYPE_BODY            = 0x4,
	INVTYPE_CHEST           = 0x5,
	INVTYPE_WAIST           = 0x6,
	INVTYPE_LEGS            = 0x7,
	INVTYPE_FEET            = 0x8,
	INVTYPE_WRISTS          = 0x9,
	INVTYPE_HANDS           = 0xa,
	INVTYPE_FINGER          = 0xb,
	INVTYPE_TRINKET         = 0xc,
	INVTYPE_WEAPON          = 0xd,
	INVTYPE_SHIELD          = 0xe,
	INVTYPE_RANGED          = 0xf,
	INVTYPE_CLOAK           = 0x10,
	INVTYPE_2HWEAPON        = 0x11,
	INVTYPE_BAG             = 0x12,
	INVTYPE_TABARD          = 0x13,
	INVTYPE_ROBE            = 0x14,
	INVTYPE_WEAPONMAINHAND  = 0x15,
	INVTYPE_WEAPONOFFHAND   = 0x16,
	INVTYPE_HOLDABLE        = 0x17,
	INVTYPE_AMMO            = 0x18,
	INVTYPE_THROWN          = 0x19,
	INVTYPE_RANGEDRIGHT     = 0x1a,
	INVTYPE_QUIVER          = 0x1b,
	INVTYPE_RELIC           = 0x1c,
	NUM_INVENTORY_TYPES     = 0x1d,
};

enum ITEM_CLASS
{
	ITEM_CLASS_CONSUMABLE	= 0,
	ITEM_CLASS_CONTAINER	= 1,
	ITEM_CLASS_WEAPON		= 2,
	ITEM_CLASS_JEWELRY		= 3,
	ITEM_CLASS_ARMOR		= 4,
	ITEM_CLASS_REAGENT		= 5,
	ITEM_CLASS_PROJECTILE	= 6,
	ITEM_CLASS_TRADEGOODS	= 7,
	ITEM_CLASS_GENERIC		= 8,
	ITEM_CLASS_RECIPE		= 9,
	ITEM_CLASS_MONEY		= 10,
	ITEM_CLASS_QUIVER		= 11,
	ITEM_CLASS_QUEST		= 12,
	ITEM_CLASS_KEY			= 13,
	ITEM_CLASS_PERMANENT	= 14,
	ITEM_CLASS_MISCELLANEOUS= 15
};

enum Item_Subclass
{
   // Weapon
   ITEM_SUBCLASS_WEAPON_AXE					= 0,
   ITEM_SUBCLASS_WEAPON_TWOHAND_AXE			= 1,
   ITEM_SUBCLASS_WEAPON_BOW					= 2,
   ITEM_SUBCLASS_WEAPON_GUN					= 3,
   ITEM_SUBCLASS_WEAPON_MACE				= 4,
   ITEM_SUBCLASS_WEAPON_TWOHAND_MACE		= 5,
   ITEM_SUBCLASS_WEAPON_POLEARM				= 6,
   ITEM_SUBCLASS_WEAPON_SWORD				= 7,
   ITEM_SUBCLASS_WEAPON_TWOHAND_SWORD		= 8,
   ITEM_SUBCLASS_WEAPON_STAFF				= 10,
   ITEM_SUBCLASS_WEAPON_FIST_WEAPON			= 13,
   ITEM_SUBCLASS_WEAPON_MISC_WEAPON			= 14,
   ITEM_SUBCLASS_WEAPON_DAGGER				= 15,
   ITEM_SUBCLASS_WEAPON_THROWN				= 16,
   ITEM_SUBCLASS_WEAPON_CROSSBOW			= 18,
   ITEM_SUBCLASS_WEAPON_WAND				= 19,
   ITEM_SUBCLASS_WEAPON_FISHING_POLE		= 20,
   
   // Armor
   ITEM_SUBCLASS_ARMOR_MISC					= 0,
   ITEM_SUBCLASS_ARMOR_CLOTH				= 1,
   ITEM_SUBCLASS_ARMOR_LEATHER				= 2,
   ITEM_SUBCLASS_ARMOR_MAIL					= 3,
   ITEM_SUBCLASS_ARMOR_PLATE_MAIL			= 4,
   ITEM_SUBCLASS_ARMOR_SHIELD				= 6,

   // Projectile
   ITEM_SUBCLASS_PROJECTILE_ARROW			= 2,
   ITEM_SUBCLASS_PROJECTILE_BULLET			= 3,
   
   // Trade goods
   ITEM_SUBCLASS_PROJECTILE_TRADE_GOODS		= 0,
   ITEM_SUBCLASS_PROJECTILE_PARTS			= 1,
   ITEM_SUBCLASS_PROJECTILE_EXPLOSIVES      = 2,
   ITEM_SUBCLASS_PROJECTILE_DEVICES			= 3,
   
   // Recipe
   ITEM_SUBCLASS_RECIPE_BOOK				= 0,
   ITEM_SUBCLASS_RECIPE_LEATHERWORKING		= 1,
   ITEM_SUBCLASS_RECIPE_TAILORING			= 2,
   ITEM_SUBCLASS_RECIPE_ENGINEERING			= 3,
   ITEM_SUBCLASS_RECIPE_BLACKSMITHING		= 4,
   ITEM_SUBCLASS_RECIPE_COOKING				= 5,
   ITEM_SUBCLASS_RECIPE_ALCHEMY				= 6,
   ITEM_SUBCLASS_RECIPE_FIRST_AID			= 7,
   ITEM_SUBCLASS_RECIPE_ENCHANTING			= 8,
   ITEM_SUBCLASS_RECIPE_FISNING				= 9,

   // Quiver
   ITEM_SUBCLASS_QUIVER_AMMO_POUCH			= 3,
   ITEM_SUBCLASS_QUIVER_QUIVER				= 2,

   // Misc
   ITEM_SUBCLASS_MISC_JUNK					= 0,
};

enum ITEM_QUALITY
{
	ITEM_QUALITY_POOR_GREY             = 0,
	ITEM_QUALITY_NORMAL_WHITE          = 1,
	ITEM_QUALITY_UNCOMMON_GREEN        = 2,
	ITEM_QUALITY_RARE_BLUE             = 3,
	ITEM_QUALITY_EPIC_PURPLE           = 4,
	ITEM_QUALITY_LEGENDARY_ORANGE      = 5,
	ITEM_QUALITY_ARTIFACT_LIGHT_YELLOW = 6,
};

enum ITEM_FLAG
{
	ITEM_FLAG_SOULBOUND    = 0x1,      // not used in proto
	ITEM_FLAG_CONJURED     = 0x2,
	ITEM_FLAG_LOOTABLE     = 0x4,
	ITEM_FLAG_UNKNOWN_04   = 0x8,      // not used in proto
	ITEM_FLAG_UNKNOWN_05   = 0x10,     // many equipable items and bags
	ITEM_FLAG_UNKNOWN_06   = 0x20,     // totems
	ITEM_FLAG_UNKNOWN_07   = 0x40,     // many consumables
	ITEM_FLAG_UNKNOWN_08   = 0x80,     // only 1 wand uses this
	ITEM_FLAG_UNKNOWN_09   = 0x100,    // some wands & relics
	ITEM_FLAG_WRAP_GIFT    = 0x200,
	ITEM_FLAG_CREATE_ITEM  = 0x400,
	ITEM_FLAG_QUEST        = 0x800,
	ITEM_FLAG_UNKNOWN_13   = 0x1000,   // not used in proto
	ITEM_FLAG_SIGNABLE     = 0x2000,
	ITEM_FLAG_READABLE     = 0x4000,
	ITEM_FLAG_UNKNOWN_16   = 0x8000,
	ITEM_FLAG_EVENT_REQ    = 0x10000,
	ITEM_FLAG_UNKNOWN_18   = 0x20000,
	ITEM_FLAG_PROSPECTABLE = 0x40000,
	ITEM_FLAG_UNIQUE_EQUIP = 0x80000,
	ITEM_FLAG_UNKNOWN_21   = 0x100000, // not used in proto
	ITEM_FLAG_UNKNOWN_22   = 0x200000, // player created health/mana/poisons
	ITEM_FLAG_THROWN       = 0x400000,
	ITEM_FLAG_SHAPESHIFT_OK= 0x800000,
};

enum SPECIAL_ITEM_TYPE
{
	ITEM_TYPE_BOWAMMO	    = 1,	// Arrows
	ITEM_TYPE_GUNAMMO	    = 2,	// Bullets
	ITEM_TYPE_SOULSHARD     = 3,	// Soul Shards
	ITEM_TYPE_LEATHERWORK   = 4,	// Leatherworking Supplies
	ITEM_TYPE_HERBALISM     = 6,	// Herbs
	ITEM_TYPE_ENCHANTMENT   = 7,	// Enchanting Supplies
	ITEM_TYPE_ENGINEERING   = 8,	// Engineering Supplies
	ITEM_TYPE_KEYRING	    = 9,	// Keys
	ITEM_TYPE_GEMS   	    = 10,	// Gems
	ITEM_TYPE_MINING	    = 11,	// Mining Supplies
	ITEM_TYPE_SBEQUIPMENT   = 12,	// Soulbound Equipment
	ITEM_TYPE_VANITYPETS    = 13	// Vanity Pets
};

enum SOCKET_GEM_COLOR
{
	META_SOCKET = 1,
	RED_SOCKET = 2,
	YELLOW_SOCKET = 4,
	BLUE_SOCKET = 8
};

#pragma pack(push,1)
struct SocketInfo {
	uint32 SocketColor;
	uint32 Unk;
};

struct ItemSpell
{
	uint32 Id;
	uint32 Trigger;
	int32 Charges;
	int32 Cooldown;
	uint32 Category;
	int32 CategoryCooldown;
};

struct ItemDamage
{
	float Min;
	float Max;
	uint32 Type;
};

struct ItemStat
{
	uint32 Type;
	int32 Value;
};

struct ItemPrototype
{
	uint32 ItemId;
	uint32 Class;
	uint32 SubClass;
	uint32 unknown_bc;
	char * Name1;
	char * Name2;
	char * Name3;
	char * Name4;
	uint32 DisplayInfoID;
	uint32 Quality;
	uint32 Flags;
	uint32 BuyPrice;
	uint32 SellPrice;
	uint32 InventoryType;
	uint32 AllowableClass;
	uint32 AllowableRace;
	uint32 ItemLevel;
	uint32 RequiredLevel;
	uint32 RequiredSkill;
	uint32 RequiredSkillRank;
	uint32 RequiredSkillSubRank;
	uint32 RequiredPlayerRank1;
	uint32 RequiredPlayerRank2;
	uint32 RequiredFaction;
	uint32 RequiredFactionStanding;
	uint32 Unique;
	uint32 MaxCount;
	uint32 ContainerSlots;
	ItemStat Stats[10];
	ItemDamage Damage[5];
	uint32 Armor;
	uint32 HolyRes;
	uint32 FireRes;
	uint32 NatureRes;
	uint32 FrostRes;
	uint32 ShadowRes;
	uint32 ArcaneRes;
	uint32 Delay;
	uint32 AmmoType;
	float  Range;
	ItemSpell Spells[5];
	uint32 Bonding;
	char * Description;
	uint32 PageId;
	uint32 PageLanguage;
	uint32 PageMaterial;
	uint32 QuestId;
	uint32 LockId;
	uint32 LockMaterial;
	uint32 Field108;
	uint32 RandomPropId;
	uint32 RandomSuffixId;
	uint32 Block;
	uint32 ItemSet;
	uint32 MaxDurability;
	uint32 ZoneNameID;
	uint32 MapID;
	uint32 BagFamily;
	uint32 TotemCategory;
	SocketInfo Sockets[3];
	uint32 SocketBonus;
	uint32 GemProperties;
	int32 DisenchantReqSkill;
	uint32 ArmorDamageModifier;

	string lowercase_name;	// used in auctions
	uint32 FoodType;		//pet food type
	GossipScript * gossip_script;
	int32 ForcedPetId;

	string ConstructItemLink(uint32 random_prop, uint32 random_suffix, uint32 stack);
	bool ValidateItemLink(const char *szLink);
};

typedef struct {
	uint32 setid;
	uint32 itemscount;
	//Spell* spell[8];
}ItemSet;
#pragma pack(pop)

#endif
