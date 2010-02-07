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
	virtual void Init();
	virtual void Destructor();

   // void Create();
	void Create (Player* owner, uint32 mapid, float x, float y, float z, float ang );

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

