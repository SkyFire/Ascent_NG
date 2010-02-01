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

enum LFGTypes 
{
	LFG_NONE = 0,
	LFG_INSTANCE = 1,
	LFG_RAID = 2,
	LFG_QUEST = 3,
	LFG_ZONE = 4,
	LFG_HEROIC_DUNGEON = 5, // from client
};

#define MAX_DUNGEONS 244+1 // check max entrys +1 on lfgdungeons.dbc
#define MAX_LFG_QUEUE_ID 3
#define LFG_MATCH_TIMEOUT 30		// in seconds

class LfgMatch;
class LfgMgr : public Singleton < LfgMgr >, EventableObject
{
public:	
	
	typedef list<PlayerPointer  > LfgPlayerList;

	LfgMgr();
	~LfgMgr();
	
	bool AttemptLfgJoin(PlayerPointer pl, uint32 LfgDungeonId);
	void SetPlayerInLFGqueue(PlayerPointer pl,uint32 LfgDungeonId);
	void SetPlayerInLfmList(PlayerPointer pl, uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueue(PlayerPointer pl,uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueues(PlayerPointer pl);
	void RemovePlayerFromLfmList(PlayerPointer pl, uint32 LfmDungeonId);
	void UpdateLfgQueue(uint32 LfgDungeonId);
	void SendLfgList(PlayerPointer plr, uint32 Dungeon);
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
	set<PlayerPointer  > PendingPlayers;
	set<PlayerPointer  > AcceptedPlayers;
	Mutex lock;
	uint32 DungeonId;
    Group * pGroup;

	LfgMatch(uint32 did) : DungeonId(did),pGroup(NULL) { }
};

extern uint32 LfgDungeonTypes[MAX_DUNGEONS];

#define sLfgMgr LfgMgr::getSingleton()
