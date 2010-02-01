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

#ifndef _ACHIEVEMENT_INTERFACE_H
#define _ACHIEVEMENT_INTERFACE_H

#include "StdAfx.h"

struct AchievementData
{
	uint32 id;
	uint32 counter[32];

	uint32 date;
	bool completed;

	uint32 num_criterias;
	uint32 completionTimeLast;
	
	bool m_isDirty;
};

enum AchievementFlags
{
	ACHIEVEMENT_FLAG_COUNTER			= 0x01,
	ACHIEVEMENT_FLAG_LEVELUP			= 0x04,
	ACHIEVEMENT_FLAG_AVERAGE			= 0x40,
	ACHIEVEMENT_FLAG_REALM_FIRST_OBTAIN	= 0x100,
	ACHIEVEMENT_FLAG_REALM_FIRST_KILL	= 0x200,
};

enum AchievementCriteriaTypes
{
	ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1,
	ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5,
	ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15,
	ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16,
	ACHIEVEMENT_CRITERIA_TYPE_DEATH= 17,
	ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON = 18,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19,
	ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20,
	ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER = 23,
	ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27,
	ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28,
	ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29,
	ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE = 30,
	ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32,
	ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34,
	ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL = 35,
	ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38,
	ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING = 39,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40,
	ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM= 42,
	ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43,
	ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK= 44,
	ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT= 45,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION= 46,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47,
	ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP= 48,
	ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49,
	ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT = 50,
	ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52,
	ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53,
	ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54,
	ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55,
	ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS = 60,
	ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS = 61,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING = 63,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER = 65,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL = 66,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67,
	ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68,
	ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2= 69,
	ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL= 70,
	ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72,
	ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE = 74,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS= 75,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76,
	ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL = 77,
	ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE = 78,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS= 80,
	ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION= 82,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID= 83,
	ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS= 84,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD = 85,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED = 86,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION = 87,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION = 88,
	ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS = 89,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM = 90,
	ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM = 91,
	ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED = 93,
	ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED = 94,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH = 95,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR = 99,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT = 101,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED = 102,
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED = 103,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED = 104,
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED = 105,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED = 106,
	ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED = 107,
	ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN = 108,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109,
	ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE= 112,
	ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113,
	ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS = 114,
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL = 115,
};


// Making this an enum since they're reversed teamids.
enum AchievementFactionReq
{
	ACHIEVEMENT_FACTION_HORDE		= 0,
	ACHIEVEMENT_FACTION_ALLIANCE	= 1,
};

enum AchievementCriteriaCondition
{
	ACHIEVEMENT_CRITERIA_CONDITION_NONE				= 0,
	ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH			= 1, // reset this achievement when you die
	ACHIEVEMENT_CRITERIA_CONDITION_MAP				= 3,
	ACHIEVEMENT_CRITERIA_CONDITION_ARENA_ZERO_LOSS	= 4,
};

enum AchievementCriteriaCompletionFlags
{
	ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG = 2,
};

enum AchievementCriteriaGroupFlags
{
	ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP = 2,
};

inline uint32 unixTimeToTimeBitfields(time_t secs)
{
	tm* lt = localtime(&secs);
	return (lt->tm_year - 100) << 24 | lt->tm_mon  << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min;
}

typedef std::set<AchievementCriteriaEntry*>							AchievementCriteriaSet;
typedef std::map<uint32, AchievementCriteriaSet*>					AchievementCriteriaMap;

class SERVER_DECL AchievementInterface
{
	PlayerPointer m_player;
	map<uint32,AchievementData*> m_achivementDataMap;
private:
	void GiveRewardsForAchievement(AchievementEntry * ae);
	void EventAchievementEarned(AchievementData * pData);
	void SendCriteriaUpdate(AchievementData * ad, uint32 idx);
	bool CanCompleteAchievement(AchievementData * ad);
	AchievementData* CreateAchievementDataEntryForAchievement(AchievementEntry * ae);

	// Gets AchievementData struct. If there is none, one will be created.
	AchievementData* GetAchievementDataByAchievementID(uint32 ID);

	WorldPacket* m_achievementInspectPacket;

public:
	AchievementInterface(PlayerPointer plr);
	~AchievementInterface();

	void LoadFromDB( QueryResult * pResult );
	void SaveToDB(QueryBuffer * buffer);

	
	WorldPacket* BuildAchievementEarned(AchievementData * pData);
	WorldPacket* BuildAchievementData(bool forInspect = false);

	bool HasAchievements() { return m_achivementDataMap.size() > 0; }

	//----------------------------------------------------------------
	void HandleAchievementCriteriaConditionDeath();

	// Handlers for misc events
	//-----------------------------------------------------------------
	void HandleAchievementCriteriaKillCreature(uint32 killedMonster);
	void HandleAchievementCriteriaWinBattleground(uint32 bgMapId, uint32 scoreMargin, uint32 time, BattlegroundPointer bg);
	void HandleAchievementCriteriaRequiresAchievement(uint32 achievementId);
	void HandleAchievementCriteriaLevelUp(uint32 level);
	void HandleAchievementCriteriaOwnItem(uint32 itemId, uint32 stack = 1);
	void HandleAchievementCriteriaLootItem(uint32 itemId, uint32 stack = 1);
	void HandleAchievementCriteriaQuestCount(uint32 questCount);
	void HandleAchievementCriteriaHonorableKillClass(uint32 classId);
	void HandleAchievementCriteriaHonorableKillRace(uint32 raceId);
	void HandleAchievementCriteriaHonorableKill();
	void HandleAchievementCriteriaTalentResetCount();
	void HandleAchievementCriteriaTalentResetCostTotal(uint32 cost);
	void HandleAchievementCriteriaBuyBankSlot(bool retroactive = false);
	void HandleAchievementCriteriaFlightPathsTaken();
	void HandleAchievementCriteriaExploreArea(uint32 areaId, uint32 explorationFlags);
	void HandleAchievementCriteriaDoEmote(uint32 emoteId, UnitPointer pTarget);
	void HandleAchievementCriteriaCompleteQuestsInZone(uint32 zoneId);
	void HandleAchievementCriteriaReachSkillLevel(uint32 skillId, uint32 skillLevel);
	void HandleAchievementCriteriaWinDuel();
	void HandleAchievementCriteriaLoseDuel();
	void HandleAchievementCriteriaKilledByCreature(uint32 creatureEntry);
	void HandleAchievementCriteriaKilledByPlayer();
	void HandleAchievementCriteriaDeath();
	void HandleAchievementCriteriaDeathAtMap(uint32 mapId);

	//-----------------------------------------------------------------
};
#endif
