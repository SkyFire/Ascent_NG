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

#ifndef __SPELLSTORE_H
#define __SPELLSTORE_H

#include "Common.h"
#include "DataStore.h"
#include "Timer.h"

#pragma pack(push,1)
struct AchievementCriteriaEntry;
struct AchievementEntry
{
    uint32 ID; // 0
    uint32 factionFlag; // 1 -1=all, 0=horde, 1=alliance
    uint32 mapID; // 2 -1=none
    char* name; // 4
    //uint32 name_flags; // 20
    //char *description[16]; // 21-36
    //uint32 desc_flags; // 37
    uint32 categoryId; // 38
    uint32 points; // 39 reward points
    //uint32 OrderInCategory; // 40
    uint32 flags; // 41
    //uint32 flags; // 42 not flags, some unknown value...
    //char *unk1[16]; // 43-58
    //uint32 unk_flags; // 59
    //uint32 count; // 60
    //uint32 refAchievement; // 61

	uint32 AssociatedCriteria[32]; // Custom stuff
	uint32 AssociatedCriteriaCount;
};
 
struct AchievementCriteriaEntry
{
    uint32  ID;                                             // 0
    uint32  referredAchievement;                            // 1
    uint32  requiredType;                                   // 2
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0
        // TODO: also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 3
            uint32  creatureCount;                          // 4
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1
        // TODO: there are further criterias instead just winning
        struct
        {
            uint32  bgMapID;                                // 3
            uint32  winCount;                               // 4
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5
        struct
        {
            uint32  unused;                                 // 3
            uint32  level;                                  // 4
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8
        struct
        {
            uint32  linkedAchievement;                      // 3
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9
        struct
        {
            uint32  unused;                                 // 3
            uint32  totalQuestCount;                        // 4
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfDays;                           // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 3
            uint32  questCount;                             // 4
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14
        struct
        {
            uint32  unused;                                 // 3
            uint32  questCount;                             // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15
        struct
        {
            uint32  mapID;                                  // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16
        struct
        {
            uint32  mapID;                                  // 3
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19
        struct
        {
            uint32  groupSize;                              // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20
        struct
        {
            uint32  creatureEntry;                          // 3
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24
        struct
        {
            uint32  unused;                                 // 3
            uint32  fallHeight;                             // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27
        struct
        {
            uint32  questID;                                // 3
            uint32  questCount;                             // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2= 69
        struct
        {
            uint32  spellID;                                // 3
            uint32  spellCount;                             // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29
        struct
        {
            uint32  spellID;                                // 3
            uint32  castCount;                              // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32  areaID;                                 // 3 Reference to AreaTable.dbc
            uint32  killCount;                              // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34
        struct
        {
            uint32  spellID;                                // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
            uint32  flag;                                   // 5 4=in a row
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38
        struct
        {
            uint32  teamtype;                               // 3 {2,3,5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING = 39
        struct
        {
            uint32  teamtype;                               // 3 {2,3,5}
            uint32  teamrating;                             // 4
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM = 42
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43
        struct
        {
            // TODO: This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 3
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK= 44
        struct
        {
            // TODO: This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 3
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT= 45
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfSlots;                          // 4
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION= 46
        struct
        {
            uint32  factionID;                              // 3
            uint32  reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfExaltedFactions;                // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49
        // TODO: where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 3
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT= 50
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_need_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52
        struct
        {
            uint32  classID;                                // 3
            uint32  count;                                  // 4
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53
        struct
        {
            uint32  raceID;                                 // 3
            uint32  count;                                  // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54
        // TODO: where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 3
        } do_emote;

        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
            uint32  flag;                                   // 5 =3 for battleground healing
            uint32  mapid;                                  // 6
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57
        struct
        {
            uint32  itemID;                                 // 3
        } equip_item;


        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68
        struct
        {
            uint32  goEntry;                                // 3
            uint32  useCount;                               // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL= 70
        // TODO: are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72
        struct
        {
            uint32  goEntry;                                // 3
            uint32  lootCount;                              // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS= 75
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skilline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76
        struct
        {
            uint32  unused;                                 // 3
            uint32  duelCount;                              // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96
        struct
        {
            uint32  powerType;                              // 3 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97
        struct
        {
            uint32  statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98
        struct
        {
            uint32  spellSchool;                            // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100
        struct
        {
            uint32  ratingType;                             // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109
        struct
        {
            uint32  lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32  lootTypeCount;                          // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } cast_spell2;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE= 112
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } honorable_kill;

        struct
        {
            uint32  field3;                                 // 3 main requirement
            uint32  field4;                                 // 4 main requirement count
            uint32  additionalRequirement1_type;            // 5 additional requirement 1 type
            uint32  additionalRequirement1_value;           // 6 additional requirement 1 value
            uint32  additionalRequirement2_type;            // 7 additional requirement 2 type
            uint32  additionalRequirement2_value;           // 8 additional requirement 1 value
        } raw;
    };
	char* name;
    //char*  name[16];                                      // 9-24
    //uint32 name_flags;                                    // 25
    uint32  completionFlag;                                 // 26
    uint32  groupFlag;                                      // 27
    uint32  timeLimit;                                      // 29 time limit in seconds
    //uint32 unk1;                                          // 30
};


struct AreaTriggerEntry
{
	uint32 AreaTriggerID;	// 1
	uint32 Mapid;			// 2
	float x;				// 3
	float y;				// 4
	float z;				// 5
	float o;				// 6
	float edge_x;			// 7
	float edge_y;			// 8
	float edge_z;			// 9
	float edge_o;			// 10
};

struct AreaPOIEntry
{
	uint32 Id;
	uint32 Icon[11];
	float X;
	float Y;
	float Z;
	uint32 MapId;
	uint32 ZoneId;
	uint32 WorldState;
};

struct BankSlotPrice
{
	uint32 Id;
	uint32 Price;
};

struct CharTitlesEntry
{
    uint32  ID;                                             // 0, title ids
    //uint32      unk1;                                     // 1 flags?
    char*         name;                                     // 2 title name
	//char*       name2[15];								// 3-17, unused
                                                            // 18 string flag, unused
    //char*       name2[16];                                // 19-34, unused
                                                            // 35 string flag, unused
    uint32  bit_index;                                      // 36 used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
};

struct ItemSetEntry
{
    uint32 id;						//1
    char* name;						//2
    //uint32 unused_shit[15];		//3 - 17
    uint32 flag;					//18 constant
    uint32 itemid[10];				//19 - 28
    //uint32 more_unused_shit[7];	//29 - 35
    uint32 SpellID[8];				//36 - 43
    uint32 itemscount[8];			//44 - 51
    uint32 RequiredSkillID;			//52
    uint32 RequiredSkillAmt;		//53
};

struct Lock
{
    uint32 Id;
    uint32 locktype[5]; // 0 - no lock, 1 - item needed for lock, 2 - min lockping skill needed
    //uint32 unk1[3];
    uint32 lockmisc[5]; // if type is 1 here is a item to unlock, else is unknow for now
    //uint32 unk2[3];
    uint32 minlockskill[5]; // min skill in lockpiking to unlock.
    //uint32 unk3[11];
};

struct emoteentry
{
    uint32 Id;
    //uint32 name;
    uint32 textid;
    uint32 textid2;
    uint32 textid3;
    uint32 textid4;
    //uint32 unk1;
    uint32 textid5;
    //uint32 unk2;
    uint32 textid6;
    //uint32 unk3;
    //uint32 unk4;
    //uint32 unk5;
    //uint32 unk6;
    //uint32 unk7;
    //uint32 unk8;
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
};

struct skilllinespell //SkillLineAbility.dbc
{
    uint32 Id;			//1
    uint32 skilline;	//2
    uint32 spell;		//3
    uint32    racemask;                                     // 4        m_raceMask
    uint32    classmask;                                    // 5        m_classMask
    //uint32    racemaskNot;                                // 6        m_excludeRace
    //uint32    classmaskNot;                               // 7        m_excludeClass
    uint32    req_skill_value;                              // 8        m_minSkillLineRank
    uint32 next;		//9
    uint32 minrank;		//10
    uint32 grey;		//11
    uint32 green;		//12
};

struct EnchantEntry
{
    uint32 Id;			//1
	//uint32 charges;	//2
    uint32 type[3];		//3-5
    int32 min[3];		//6-8 //for compat, in practice min==max
    int32 max[3];		//9-11
    uint32 spell[3];	//12-14
    char* Name;			//15
	//char* NameAlt[15]	//16-31
    //uint32 NameFlags;	//32
    uint32 visual;		//33
    uint32 EnchantGroups; //34
    uint32 GemEntry;	//35
    uint32 unk7;		//36 Gem Related
	//uint32 requiredSkill;	//37
    //uint32 requiredSkillValue;	//38
	//								//39
};

struct GemPropertyEntry{
    uint32 Entry;
    uint32 EnchantmentID;
    uint32 unk1;//bool
    uint32 unk2;//bool
    uint32 SocketMask;
};

struct GlyphPropertyEntry	//GlyphProperties.dbc
{
    uint32 Entry;
    uint32 SpellID;
    uint32 Type; // 0 = Major, 1 = Minor
    uint32 unk; // some flag
};

struct skilllineentry //SkillLine.dbc
{
    uint32 id;
    uint32 type;
    uint32 skillCostID;
    char* Name;
    //int32 NameAlt1;
    //uint32 NameAlt2;
    //uint32 NameAlt3;
    //uint32 NameAlt4;
    //uint32 NameAlt5;
    //uint32 NameAlt6;
    //uint32 NameAlt7;
    //uint32 NameAlt8;
    //uint32 NameAlt9;
    //uint32 NameAlt10;
    //uint32 NameAlt11;
    //uint32 NameAlt12;
    //uint32 NameAlt13;
    //uint32 NameAlt14;
    //uint32 NameAlt15;
    //uint32 NameFlags;
    //uint32 Description;
    //uint32 DescriptionAlt1;
    //uint32 DescriptionAlt2;
    //uint32 DescriptionAlt3;
    //uint32 DescriptionAlt4;
    //uint32 DescriptionAlt5;
    //uint32 DescriptionAlt6;
    //uint32 DescriptionAlt7;
    //uint32 DescriptionAlt8;
    //uint32 DescriptionAlt9;
    //uint32 DescriptionAlt10;
    //uint32 DescriptionAlt11;
    //uint32 DescriptionAlt12;
    //uint32 DescriptionAlt13;
    //uint32 DescriptionAlt14;
    //uint32 DescriptionAlt15;
    //uint32 DescriptionFlags;
    //uint32 unk2;

};

struct SummonPropertiesEntry
{
	uint32 Id;
	uint32 controltype;
	uint32 factionId;
	uint32 type;
	uint32 slot;
	uint32 unk2;
};

// Struct for the entry in Spell.dbc
struct SpellEntry
{
	uint32 Id;                              //0
	uint32 Category;                        //1
	uint32 DispelType;                      //2
	uint32 MechanicsType;                   //3
	uint32 Attributes;                      //4
	uint32 AttributesEx;                    //5
	uint32 Flags3;                          //6
	uint32 Flags4;                          //7
	uint32 Flags5;                          //8
	uint32 Flags6;                          //9
	uint32 Flags7;                          //10
	uint32 RequiredShapeShift;              //11 // Flags BitMask for shapeshift spells
	uint32 NotAllowedShapeShift;            //12 // Flags BitMask for which shapeshift forms this spell can NOT be used in.
	uint32 Targets;                         //13 - N / M
	uint32 TargetCreatureType;              //14
	uint32 RequiresSpellFocus;              //15
	uint32 FacingCasterFlags;				//16
	uint32 CasterAuraState;                 //17
	uint32 TargetAuraState;                 //18
	uint32 CasterAuraStateNot;              //19
	uint32 TargetAuraStateNot;              //20
	uint32 casterAuraSpell;                 //21
    uint32 targetAuraSpell;                 //22
    uint32 excludeCasterAuraSpell;          //23
    uint32 excludeTargetAuraSpell;          //24
	uint32 CastingTimeIndex;                //25
	uint32 RecoveryTime;                    //26
	uint32 CategoryRecoveryTime;            //27
	uint32 InterruptFlags;                  //28
	uint32 AuraInterruptFlags;              //29
	uint32 ChannelInterruptFlags;           //30
	uint32 procFlags;                       //31
	uint32 procChance;                      //32
	uint32 procCharges;                     //33
	uint32 maxLevel;                        //34
	uint32 baseLevel;                       //35
	uint32 spellLevel;                      //36
	uint32 DurationIndex;                   //37
	uint32 powerType;                       //38
	uint32 manaCost;                        //39
	uint32 manaCostPerlevel;                //40
	uint32 manaPerSecond;                   //41
	uint32 manaPerSecondPerLevel;           //42
	uint32 rangeIndex;                      //43
	float  speed;                           //44
	uint32 modalNextSpell;                  //45
	uint32 maxstack;                        //46
	uint32 Totem[2];                        //47 - 48
	uint32 Reagent[8];                      //49 - 56
	uint32 ReagentCount[8];                 //57 - 64
	int32 EquippedItemClass;                //65
	uint32 EquippedItemSubClass;            //66
	uint32 RequiredItemFlags;               //67
	uint32 Effect[3];                       //68 - 70
	uint32 EffectDieSides[3];               //71 - 73
	uint32 EffectBaseDice[3];               //74 - 76
	float  EffectDicePerLevel[3];           //77 - 79
	float  EffectRealPointsPerLevel[3];     //80 - 82
	int32  EffectBasePoints[3];             //83 - 85
	int32  EffectMechanic[3];               //86 - 88       Related to SpellMechanic.dbc
	uint32 EffectImplicitTargetA[3];        //89 - 91
	uint32 EffectImplicitTargetB[3];        //92 - 94
	uint32 EffectRadiusIndex[3];            //95 - 97
	uint32 EffectApplyAuraName[3];          //98 - 100
	uint32 EffectAmplitude[3];              //101 - 103
	float  EffectMultipleValue[3];          //104 - 106     This value is the $ value from description
	uint32 EffectChainTarget[3];            //107 - 109
	uint32 EffectItemType[3];				//110 - 112
	uint32 EffectMiscValue[3];              //113 - 115
	uint32 EffectMiscValueB[3];             //116 - 118  2.4.3
	uint32 EffectTriggerSpell[3];           //119 - 121
	float  EffectPointsPerComboPoint[3];    //122 - 124
	uint32 EffectSpellClassMask[3][3];      //125-133
	uint32 SpellVisual;                     //134
	// unknown								//135
	uint32 SpellIconID;                     //136
	uint32 activeIconID;                    //137
	uint32 spellPriority;                   //138
	char* Name;                             //139
	//char* NameAlt[15];                    //140-154 not used
	//uint32 NameFlags;                     //155 not used
	char * Rank;                            //156
	//char * RankAlt[15];                   //157-171 not used
	//uint32 RankFlags;                     //172 not used
	char * Description;						//173
	//char * DescriptionAlt[15];			//174-188 not used
	//uint32 DescriptionFlags;              //189 not used
	char * BuffDescription;					//190
	//char * BuffDescription[15];			//191-205 not used
	//uint32 buffdescflags;                 //206 not used
	uint32 ManaCostPercentage;              //207
	uint32 StartRecoveryCategory;           //208
	uint32 StartRecoveryTime;               //209
	uint32 MaxTargetLevel;                  //210
	uint32 SpellFamilyName;                 //211
	uint32 SpellGroupType[3];				//212-214
	uint32 MaxTargets;                      //215
	uint32 Spell_Dmg_Type;                  //216   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 PreventionType;                  //217   0,1,2 related to Spell_Dmg_Type I think
	int32 StanceBarOrder;                   //218   related to paladin aura's 
	float dmg_multiplier[3];                //219 - 221   if the name is correct I dono
	uint32 MinFactionID;                    //222   only one spellid:6994 has this value = 369 UNUSED
	uint32 MinReputation;                   //223   only one spellid:6994 has this value = 4 UNUSED
	uint32 RequiredAuraVision;              //224  3 spells 1 or 2   
	uint32 TotemCategory[2];				//225-226
	int32 AreaGroupId;						//227  //look up area by index in areagroup.dbc
	uint32 School;							//228
	uint32 runeCostID;                      //229
    //uint32 spellMissileID;                //230 not used
	uint32  PowerDisplayId;					//231 Related to PowerDisplay.dbc

    /// CUSTOM: these fields are used for the modifications made in the world.cpp
    uint32 DiminishStatus;                  //
    uint32 proc_interval;                   //!!! CUSTOM, <Fill description for variable>
	float ProcsPerMinute;                   //!!! CUSTOM, number of procs per minute
    uint32 buffIndexType;					//!!! CUSTOM, <Fill description for variable>
    uint32 c_is_flags;						//!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
    uint32 buffType;                        //!!! CUSTOM, these are related to creating a item through a spell
    uint32 RankNumber;                      //!!! CUSTOM, this protects players from having >1 rank of a spell
    uint32 NameHash;                        //!!! CUSTOM, related to custom spells, summon spell quest related spells
	float base_range_or_radius;				//!!! CUSTOM, needed for aoe spells most of the time
    float base_range_or_radius_sqr;         //!!! CUSTOM, needed for aoe spells most of the time
	uint32 talent_tree;						//!!! CUSTOM,
	bool is_melee_spell;					//!!! CUSTOM,
	bool is_ranged_spell;					//!!! CUSTOM,
	bool spell_can_crit;					//!!! CUSTOM,
	uint32 ThreatForSpell;

	// love me or hate me, all "In a cone in front of the caster" spells don't necessarily mean "in front"
	float cone_width;

	//Spell Coefficient
	uint32 spell_coef_flags;                //!!! CUSTOM, store flags for spell coefficient calculations
	float Dspell_coef_override;             //!!! CUSTOM, overrides any spell coefficient calculation and use this value in DD&DH
	float OTspell_coef_override;			//!!! CUSTOM, overrides any spell coefficient calculation and use this value in HOT&DOT
	float AP_coef_override;					//!!! CUSTOM, Additional coef from ap
	float RAP_coef_override;				//!!! CUSTOM, Additional coef from RAP

	bool self_cast_only;
	bool apply_on_shapeshift_change;
	bool always_apply;
	uint32 auraimmune_flag;									//!!! CUSTOM, this var keeps aura effects in binary format.

	bool Unique;											//!!! CUSTOM, Is this a unique effect? ex: Mortal Strike -50% healing.

	uint32 area_aura_update_interval;
	uint32 skilline;
	uint32 logsId;	// SpellId used to send log to client for this spell
	uint32 AdditionalAura;
	uint32 forced_creature_target;
	uint32 AreaAuraTarget;

	//poisons type...
	uint32 poison_type;

	//backattack
	bool AllowBackAttack;
};

struct SpellRuneCostEntry
{
    uint32  ID;
    uint32  bloodRuneCost;
    uint32  frostRuneCost;
    uint32  unholyRuneCost;
    uint32  runePowerGain;
};

struct ItemExtendedCostEntry
{
    uint32 costid;
    uint32 honor;
    uint32 arena;
	//uint32 unk;
    uint32 item[5];
    uint32 count[5];
	uint32 personalrating;
};

struct TalentEntry
{
    uint32  TalentID;
    uint32  TalentTree;
    uint32  Row;
    uint32  Col;
    uint32  RankID[5];
    //uint32  unk[4];
    uint32  DependsOn;
    //uint32  unk1[2];
    uint32  DependsOnRank;
    //uint32  unk2[4];
	uint32  DependsOnSpell;	//21
};

struct TalentTabEntry
{
    uint32	TalentTabID;	// 1
    //char*	Name;
	//unit32  unk3
	//unit32  unk4
	//unit32  unk5
	//unit32  unk6
	//unit32  unk7
	//unit32  unk8
	//unit32  unk9
	//unit32  unk10
	//unit32  unk11
	//unit32  unk12
	//unit32  unk13
	//unit32  unk14
	//unit32  unk15
	//unit32  unk16
	//unit32  unk17
	//unit32  unk18
	//unit32  unk19
	//unit32  unk20
	uint32  ClassMask;      // 20
	uint32  PetTalentMask;  // 21
	uint32	TabPage;	    // 22
	//char*	InternalName;	// 23
};

struct Trainerspell
{
    uint32 Id;
    uint32 skilline1;
    uint32 skilline2;
    uint32 skilline3;
    uint32 maxlvl;
    uint32 charclass;
};

struct SpellCastTime
{
    uint32 ID;
    uint32 CastTime;
    //uint32 unk1;
    //uint32 unk2;
};

struct SpellRadius
{
    uint32 ID;
    float Radius;
    //float unk1;
    float Radius2;
};

struct SpellRange
{
    uint32 ID;
    float minRange;
    float maxRange;
	uint32 type;
	char *longname;
	char *name;
};

struct SpellDuration
{
    uint32 ID;
    uint32 Duration1;
    uint32 Duration2;
    uint32 Duration3;
};

struct RandomProps
{
    uint32 ID;
    char *rpname;
    uint32 spells[3];
    //uint32 unk1;
    //uint32 unk2;
    //uint32 name2;
    //uint32 RankAlt1;
    //uint32 RankAlt2;
    //uint32 RankAlt3;
    //uint32 RankAlt4;
    //uint32 RankAlt5;
    //uint32 RankAlt6;
    //uint32 RankAlt7;
    //uint32 RankAlt8;
    //uint32 RankAlt9;
    //uint32 RankAlt10;
    //uint32 RankAlt11;
    //uint32 RankAlt12;
    //uint32 RankAlt13;
    //uint32 RankAlt14;
    //uint32 RankAlt15;
    //uint32 RankFlags;

};

struct WorldMapOverlayEntry
{
	uint32 AreaReference;
	uint32 AreaTableID;
};

struct AreaGroup
{
	uint32  AreaGroupId;		// 0
	uint32  AreaId[7];			// 1-7
};

struct AreaTable
{
    uint32 AreaId;
    uint32 mapId;
    uint32 ZoneId;
    uint32 explorationFlag;
    uint32 AreaFlags;
    //uint32 unk2;
    //uint32 unk3;
    //uint32 unk4;
    uint32 EXP;//not XP
    //uint32 unk5;
    uint32 level;
    char* name;
    //uint32 nameAlt1;
    //uint32 nameAlt2;
    //uint32 nameAlt3;
    //uint32 nameAlt4;
    //uint32 nameAlt5;
    //uint32 nameAlt6;
    //uint32 nameAlt7;
    //uint32 nameAlt8;
    //uint32 nameAlt9;
    //uint32 nameAlt10;
    //uint32 nameAlt11;
    //uint32 nameAlt12;
    //uint32 nameAlt13;
    //uint32 nameAlt14;
    //uint32 nameAlt15;
    //uint32 nameFlags;
    uint32 category;
    //uint32 unk7;
    //uint32 unk8;
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
    //uint32 unk12;
};

struct FactionTemplateDBC
{
    uint32 ID;
    uint32 Faction;
	uint32 FactionFlags;
    //uint32 FactionGroup; unused
    uint32 Mask;
    uint32 FriendlyMask;
    uint32 HostileMask;
    uint32 EnemyFactions[4];
    uint32 FriendlyFactions[4];
};

struct AuctionHouseDBC
{
    uint32 id;
    uint32 unk;
    uint32 fee;
    uint32 tax;
    //char* name;
    //char* nameAlt1;
    //char* nameAlt2;
    //char* nameAlt3;
    //char* nameAlt4;
    //char* nameAlt5;
    //char* nameAlt6;
    //char* nameAlt7;
    //char* nameAlt8;
    //char* nameAlt9;
    //char* nameAlt10;
    //char* nameAlt11;
    //char* nameAlt12;
    //char* nameAlt13;
    //char* nameAlt14;
    //char* nameAlt15;
    //char* nameFlags;
};

struct FactionDBC
{
    uint32 ID;
    int32 RepListId;
    uint32 baseRepMask[4];
    //uint32 unk1[4];
    int32 baseRepValue[4];
    //uint32 unk2[4];
    uint32 parentFaction;
    char* Name;
    //uint32 shit[16];
    //uint32 Description;
    //uint32 shit2[16];
};

struct DBCTaxiNode
{
    uint32 id;
    uint32 mapid;
    float x;
    float y;
    float z;
    //uint32 name;
    //uint32 namealt1;
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    uint32 horde_mount;
    uint32 alliance_mount;
};

struct DBCTaxiPath
{
    uint32 id;
    uint32 from;
    uint32 to;
    uint32 price;
};

struct DBCTaxiPathNode
{
    uint32 id;
    uint32 path;
    uint32 seq;
    uint32 mapid;
    float x;
    float y;
    float z;
    uint32 unk1;
    uint32 waittime;
    //uint32 unk2;
    //uint32 unk3;
};

struct CreatureSpellDataEntry
{
    uint32 id;
    uint32 Spells[3];
    uint32 PHSpell;
    uint32 Cooldowns[3];
    uint32 PH;
};

struct CharRaceEntry
{
	uint32 race_id;
	uint32 faction_id;
	uint32 male_displayid;
	uint32 female_displayid;
	uint32 team_id;
	uint32 cinematic_id;
	char* name1;
	uint32 expansion;
};

struct CharClassEntry
{
    uint32 class_id;
    //uint32 unk1;
    uint32 power_type;
    //uint32 unk2;
    char* name;
    //uint32 namealt1;
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    //char* string1[16];									// 22-37 unused
                                                            // 38 string flag, unused
    //char* string2[16];									// 39-54 unused
                                                            // 55 string flag, unused
                                                            // 56, unused
	uint32  spellfamily;                                    // 57
                                                            // 58, unused
    uint32  CinematicSequence;                              // 59 id from CinematicSequences.dbc
};

struct CreatureDisplayInfo
{
	uint32 ID;		//id
	//uint32 unk2;	//ModelData column2?
	//uint32 unk3;	//ExtraDisplayInfo column 18?
	//uint32 unk4;
	float Scale;
	//uint32 unk6;
	//char* name;	//FilenName? 
	//uint32 unk8;
	//uint32 unk9;
	//uint32 unk10;
	//uint32 unk11;
	//uint32 unk12;
	//uint32 unk13;
	//uint32 unk14;
	//uint32 unk15;
	//uint32 unk16;
};


struct CreatureFamilyEntry
{
	uint32 ID;                                              // 1
	float minsize;                                          // 2
	uint32 minlevel;                                        // 3
	float maxsize;                                          // 4
	uint32 maxlevel;                                        // 5
	uint32 skilline;                                        // 6
	uint32 tameable;		                                // 7 second skill line - 270 Generic
	uint32 petdietflags;                                    // 8
	int32 pettalenttype;                                    // 9 m_petTalentType
															// 10 m_categoryEnumID
	char* name;                                             // 11
	//char*   NameAlt[15];									// 12-26
															// 27 string flags, unused
															// 28 m_iconFile unused
};

struct MapEntry
{
	uint32 id;
	char* name_internal;
	uint32 map_type;
	uint32 is_pvp_zone;
	char* real_name;
	uint32 linked_zone;		// common zone for instance and continent map
	char* hordeIntro;		// text for PvP Zones
	char* allianceIntro;	// text for PvP Zones
	uint32 multimap_id;		// seems to be 0 for all test maps.
	char* normalReqText;	// normal mode requirement text
	char* heroicReqText;	// heroic mode requirement text
	int32 parent_map;		// map_id of parent map
	float start_x;			// enter x coordinate (if exist single entry)
	float start_y;			// enter y coordinate (if exist single entry)
	uint32 resetTimeRaid;
	uint32 resetTimeHeroic;
	uint32 addon;			// 0-original maps, 1-tbc addon, 2-wotlk addon
};

struct ItemRandomSuffixEntry
{
	uint32 id;
	char *name;
	uint32 enchantments[5];
	uint32 prefixes[5];
};

struct BarberShopStyleEntry
{
    uint32  id;												// 0
    uint32  type;											// 1 value 0 -> hair, value 2 -> facialhair
	char* name;												// 2 string hairstyle name
    //char*   name[15];										// 3-17 name of hair style
    //uint32  name_flags;									// 18
    //uint32  unk_name[16];									// 19-34, all empty
    //uint32  unk_flags;									// 35
    //float   unk3;											// 36 values 1 and 0,75
    uint32  race;											// 37 race
    uint32  gender;											// 38 0 male, 1 female
    uint32  hair_id;										// 39 Hair ID
};

struct gtFloat
{
	float val;
};

struct CombatRatingDBC
{
	float val;
};

struct ChatChannelDBC
{
	uint32 id;
	uint32 flags;
	const char* pattern;
};

struct DurabilityQualityEntry
{
    uint32 id;
    float quality_modifier;
};

struct DurabilityCostsEntry
{
    uint32 itemlevel;
    uint32 modifier[29];
};

struct SpellShapeshiftForm
{
    uint32 id;
	uint32 spells[7];
};

struct VehicleEntry
{
	uint32 m_ID; // 0
	uint32 m_flags; // 1
	float m_turnSpeed; // 2
	float m_pitchSpeed; // 3
	float m_pitchMin; // 4
	float m_pitchMax; // 5
	uint32 m_seatID[8]; // 6-13
	float m_mouseLookOffsetPitch; // 14
	float m_cameraFadeDistScalarMin; // 15
	float m_cameraFadeDistScalarMax; // 16
	float m_cameraPitchOffset; // 17
	//int m_powerType[3]; // removed in 3.1
	//int m_powerToken[3]; // removed in 3.1
	float m_facingLimitRight; // 18
	float m_facingLimitLeft; // 19
	float m_msslTrgtTurnLingering; // 20
	float m_msslTrgtPitchLingering; // 21
	float m_msslTrgtMouseLingering; // 22
	float m_msslTrgtEndOpacity; // 23
	float m_msslTrgtArcSpeed; // 24
	float m_msslTrgtArcRepeat; // 25
	float m_msslTrgtArcWidth; // 26
	float m_msslTrgtImpactRadius[2]; // 27-28
	char* m_msslTrgtArcTexture; // 29
	char* m_msslTrgtImpactTexture; // 30
	char* m_msslTrgtImpactModel[2]; // 31-32
	float m_cameraYawOffset; // 33
	uint32 m_uiLocomotionType; // 34
	float m_msslTrgtImpactTexRadius; // 35
	uint32 m_uiSeatIndicatorType; // 36
	// 37, new in 3.1
	// 38, new in 3.1
	// 39, new in 3.1
};
 
struct VehicleSeatEntry
{
	uint32  m_ID;                                           // 0
	uint32  m_flags;                                        // 1
	int32   m_attachmentID;                                 // 2
	float   m_attachmentOffsetX;                            // 3
	float   m_attachmentOffsetY;                            // 4
	float   m_attachmentOffsetZ;                            // 5
	float   m_enterPreDelay;                                // 6
	float   m_enterSpeed;                                   // 7
	float   m_enterGravity;                                 // 8
	float   m_enterMinDuration;                             // 9
	float   m_enterMaxDuration;                             // 10
	float   m_enterMinArcHeight;                            // 11
	float   m_enterMaxArcHeight;                            // 12
	int32   m_enterAnimStart;                               // 13
	int32   m_enterAnimLoop;                                // 14
	int32   m_rideAnimStart;                                // 15
	int32   m_rideAnimLoop;                                 // 16
	int32   m_rideUpperAnimStart;                           // 17
	int32   m_rideUpperAnimLoop;                            // 18
	float   m_exitPreDelay;                                 // 19
	float   m_exitSpeed;                                    // 20
	float   m_exitGravity;                                  // 21
	float   m_exitMinDuration;                              // 22
	float   m_exitMaxDuration;                              // 23
	float   m_exitMinArcHeight;                             // 24
	float   m_exitMaxArcHeight;                             // 25
	int32   m_exitAnimStart;                                // 26
	int32   m_exitAnimLoop;                                 // 27
	int32   m_exitAnimEnd;                                  // 28
	float   m_passengerYaw;                                 // 29
	float   m_passengerPitch;                               // 30
	float   m_passengerRoll;                                // 31
	int32   m_passengerAttachmentID;                        // 32
	int32   m_vehicleEnterAnim;                             // 33
	int32   m_vehicleExitAnim;                              // 34
	int32   m_vehicleRideAnimLoop;                          // 35
	int32   m_vehicleEnterAnimBone;                         // 36
	int32   m_vehicleExitAnimBone;                          // 37
	int32   m_vehicleRideAnimLoopBone;                      // 38
	float   m_vehicleEnterAnimDelay;                        // 39
	float   m_vehicleExitAnimDelay;                         // 40
	uint32  m_vehicleAbilityDisplay;                        // 41
	uint32  m_enterUISoundID;                               // 42
	uint32  m_exitUISoundID;                                // 43
	int32   m_uiSkin;                                       // 44
	uint32  m_flagsB;                                       // 45
															// 46-57 added in 3.1, floats mostly
};

#pragma pack(pop)

ASCENT_INLINE float GetScale(CreatureDisplayInfo *Scale)
{
	return Scale->Scale;
}

ASCENT_INLINE float GetRadius(SpellRadius *radius)
{
    return radius->Radius;
}
ASCENT_INLINE uint32 GetCastTime(SpellCastTime *time)
{
    return time->CastTime;
}
ASCENT_INLINE float GetMaxRange(SpellRange *range)
{
    return range->maxRange;
}
ASCENT_INLINE float GetMinRange(SpellRange *range)
{
    return range->minRange;
}
ASCENT_INLINE uint32 GetDuration(SpellDuration *dur)
{
    return dur->Duration1;
}

#define SAFE_DBC_CODE_RETURNS			/* undefine this to make out of range/nulls return null. */

template<class T>
class SERVER_DECL DBCStorage
{
	T * m_heapBlock;
	T * m_firstEntry;

	T ** m_entries;
	uint32 m_max;
	uint32 m_numrows;
	uint32 m_stringlength;
	char * m_stringData;

public:
	
	DBCStorage()
	{
		m_heapBlock = NULL;
		m_entries = NULL;
		m_firstEntry = NULL;
		m_max = 0;
		m_numrows = 0;
		m_stringlength=0;
		m_stringData = NULL;
	}

	~DBCStorage()
	{
		Cleanup();
	}

	void Cleanup()
	{
		if(m_heapBlock)
		{
			delete[] m_heapBlock;
			//free(m_heapBlock);
			m_heapBlock = NULL;
		}
		if(m_entries)
		{
			delete[] m_entries;
			//free(m_entries);
			m_entries = NULL;
		}
		if( m_stringData != NULL )
		{
			delete[] m_stringData;
			//free(m_stringData);
			m_stringData = NULL;
		}
	}

	bool Load(const char * filename, const char * format, bool load_indexed, bool load_strings)
	{
		uint32 rows;
		uint32 cols;
		uint32 useless_shit;
		uint32 string_length;
		uint32 header;
		uint32 i;
		long pos;

		FILE * f = fopen(filename, "rb");
		if(f == NULL)
			return false;

		/* read the number of rows, and allocate our block on the heap */
		fread(&header,4,1,f);
		fread(&rows, 4, 1, f);
		fread(&cols, 4, 1, f);
		fread(&useless_shit, 4, 1, f);
		fread(&string_length, 4, 1, f);
		pos = ftell(f);

#ifdef USING_BIG_ENDIAN
		swap32(&rows); swap32(&cols); swap32(&useless_shit); swap32(&string_length);
#endif

		if( load_strings )
		{
			fseek( f, 20 + ( rows * cols * 4 ), SEEK_SET );
			m_stringData = new char[string_length];
			//m_stringData = (char*)malloc(string_length);
			m_stringlength = string_length;
			fread( m_stringData, string_length, 1, f );
		}

		fseek(f, pos, SEEK_SET);

		m_heapBlock = new T[rows];
		//m_heapBlock = (T*)malloc(rows * sizeof(T));
		ASSERT(m_heapBlock);

		/* read the data for each row */
		for(i = 0; i < rows; ++i)
		{
			memset(&m_heapBlock[i], 0, sizeof(T));
			ReadEntry(f, &m_heapBlock[i], format, cols, filename);

			if(load_indexed)
			{
				/* all the time the first field in the dbc is our unique entry */
				if(*(uint32*)&m_heapBlock[i] > m_max)
					m_max = *(uint32*)&m_heapBlock[i];
			}
		}

		if(load_indexed)
		{
			m_entries = new T*[(m_max+1)];
			//m_entries = (T**)malloc(sizeof(T*) * (m_max+1));
			ASSERT(m_entries);

			memset(m_entries, 0, (sizeof(T*) * (m_max+1)));
			for(i = 0; i < rows; ++i)
			{
				if(m_firstEntry == NULL)
					m_firstEntry = &m_heapBlock[i];

				m_entries[*(uint32*)&m_heapBlock[i]] = &m_heapBlock[i];
			}
		}

		m_numrows = rows;

		fclose(f);
		return true;
	}

	void ReadEntry(FILE * f, T * dest, const char * format, uint32 cols, const char * filename)
	{
		const char * t = format;
		uint32 * dest_ptr = (uint32*)dest;
		uint32 c = 0;
		uint32 val;
		size_t len = strlen(format);
		if(len!= cols)
			printf("!!! possible invalid format in file %s (us: %u, them: %u)\n", filename, (uint32)len, cols);

		while(*t != 0)
		{
			if((++c) > cols)
			{
				++t;
				printf("!!! Read buffer overflow in DBC reading of file %s\n", filename);
				continue;
			}

			fread(&val, 4, 1, f);
			if(*t == 'x')
			{
				++t;
				continue;		// skip!
			}
#ifdef USING_BIG_ENDIAN
			swap32(&val);
#endif
			if(*t == 's')
			{
				char ** new_ptr = (char**)dest_ptr;
				static const char * null_str = "";
				char * ptr;
				if( val < m_stringlength )
					ptr = m_stringData + val;
				else
					ptr = (char*)null_str;

				*new_ptr = ptr;
				new_ptr++;
				dest_ptr = (uint32*)new_ptr;
			}
			else
			{
				*dest_ptr = val;
				dest_ptr++;
			}

			++t;
		}
	}

	ASCENT_INLINE uint32 GetNumRows()
	{
		return m_numrows;
	}

	T * LookupEntryForced(uint32 i)
	{
#if 0
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
			{
				printf("LookupEntryForced failed for entry %u\n", i);
				return NULL;
			}
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
#else
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
#endif
	}

	void SetRow(uint32 i, T * t)
	{
		if(i < m_max && m_entries)
			m_entries[i] = t;
	}

#ifdef SAFE_DBC_CODE_RETURNS
	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return m_firstEntry;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return &m_heapBlock[0];
			else
				return &m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return &m_heapBlock[0];
		else
			return &m_heapBlock[i];
	}

#else

	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return NULL;
		else
			return m_heapBlock[i];
	}

#endif
};

extern SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
extern SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
extern SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
extern SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
extern SERVER_DECL DBCStorage<Lock> dbcLock;
extern SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
extern SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
extern SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
extern SERVER_DECL DBCStorage<SpellShapeshiftForm> dbcSpellShapeshiftForm;
extern SERVER_DECL DBCStorage<SpellRuneCostEntry> dbcSpellRuneCost;
extern SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
extern SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
extern SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
extern SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
extern SERVER_DECL DBCStorage<AreaTable> dbcArea;
extern SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
extern SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
extern SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
extern SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
extern SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
extern SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
extern SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
extern SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
extern SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
extern SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
extern SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
extern SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
extern SERVER_DECL DBCStorage<CreatureDisplayInfo> dbcCreatureDisplayInfo;
extern SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
extern SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
extern SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
extern SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
extern SERVER_DECL DBCStorage<MapEntry> dbcMap;
extern SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
extern SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
extern SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
extern SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
extern SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
extern SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices; //uses same structure as Bank
extern SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyle;
extern SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
extern SERVER_DECL DBCStorage<AchievementEntry> dbcAchievement;
extern SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchivementCriteria;
extern SERVER_DECL DBCStorage<VehicleEntry> dbcVehicle;
extern SERVER_DECL DBCStorage<VehicleSeatEntry> dbcVehicleSeat;
extern SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
extern SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProps;
extern SERVER_DECL DBCStorage<AreaPOIEntry> dbcAreaPOI;

bool LoadDBCs();
bool LoadRSDBCs();

#endif
