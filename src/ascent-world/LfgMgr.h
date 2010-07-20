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

enum LFGTypes 
{
	LFG_NONE                 = 0,
	LFG_DUNGEON              = 1,
	LFG_RAID                 = 2,
	LFG_QUEST                = 3,
	LFG_ZONE                 = 4,
	LFG_HEROIC_DUNGEON       = 5,
	LFG_ANY_DUNGEON          = 6,
	LFG_ANY_HEROIC_DUNGEON   = 7,
	LFG_DAILY_DUNGEON        = 8,
	LFG_DAILY_HEROIC_DUNGEON = 9,
};

#define MAX_DUNGEONS 294+1 // check max entries +1 on lfgdungeons.dbc
#define MAX_LFG_QUEUE_ID 3
#define LFG_MATCH_TIMEOUT 30		// in seconds

class LfgMatch;
class LfgMgr : public Singleton < LfgMgr >, EventableObject
{
public:	
	
	typedef list<Player*  > LfgPlayerList;

	LfgMgr();
	~LfgMgr();
	
	bool AttemptLfgJoin(Player* pl, uint32 LfgDungeonId);
	void SetPlayerInLFGqueue(Player* pl,uint32 LfgDungeonId);
	void SetPlayerInLfmList(Player* pl, uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueue(Player* pl,uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueues(Player* pl);
	void RemovePlayerFromLfmList(Player* pl, uint32 LfmDungeonId);
	void UpdateLfgQueue(uint32 LfgDungeonId);
	void SendLfgList(Player* plr, uint32 Dungeon);
	void EventMatchTimeout(LfgMatch * pMatch);

	int32 event_GetInstanceId() { return -1; }
	
protected:
	
	LfgPlayerList m_lookingForGroup[MAX_DUNGEONS];
	LfgPlayerList m_lookingForMore[MAX_DUNGEONS];
	Mutex m_lock;
	
	
};

class LfgMatch
{
public:
	set<Player*  > PendingPlayers;
	set<Player*  > AcceptedPlayers;
	Mutex lock;
	uint32 DungeonId;
    Group * pGroup;

	LfgMatch(uint32 did) : DungeonId(did),pGroup(NULL) { }
};

extern uint32 LfgDungeonTypes[MAX_DUNGEONS];

#define sLfgMgr LfgMgr::getSingleton()
