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

#ifndef WOWSERVER_CORPSE_H
#define WOWSERVER_CORPSE_H

enum CORPSE_STATE
{
	CORPSE_STATE_BODY = 0,
	CORPSE_STATE_BONES = 1,
};

struct CorpseData
{
	uint32 LowGuid;
	uint32 mapid;
	uint64 owner;
	uint32 instancemapid;
	float x;
	float y;
	float z;
	void DeleteFromDB();
};

enum CORPSE_DYNAMIC_FLAGS
{
	CORPSE_DYN_FLAG_LOOTABLE			= 1,
};

#define CORPSE_RECLAIM_TIME 30
#define CORPSE_RECLAIM_TIME_MS CORPSE_RECLAIM_TIME * 1000
#define CORPSE_MINIMUM_RECLAIM_RADIUS 39 
#define CORPSE_MINIMUM_RECLAIM_RADIUS_SQ CORPSE_MINIMUM_RECLAIM_RADIUS * CORPSE_MINIMUM_RECLAIM_RADIUS

class SERVER_DECL Corpse : public Object
{
public:
	Corpse( uint32 high, uint32 low );
	~Corpse();

   // void Create();
	void Create (Player *owner, uint32 mapid, float x, float y, float z, float ang );

	void SaveToDB();
	void DeleteFromDB();
	ASCENT_INLINE void SetCorpseState(uint32 state) { m_state = state; }
	ASCENT_INLINE uint32 GetCorpseState() { return m_state; }
	void Despawn();
	
	ASCENT_INLINE void SetLoadedFromDB(bool value) { _loadedfromdb = value; }
	ASCENT_INLINE bool GetLoadedFromDB(void) { return _loadedfromdb; }

	void SpawnBones();
	void Delink();

	void ResetDeathClock(){ m_time = time( NULL ); }
	time_t GetDeathClock(){ return m_time; }

private:
	uint32 m_state;
	time_t m_time;
	uint32 _fields[CORPSE_END];
	bool _loadedfromdb;
};

#endif

