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

#ifndef _DAYWATCHERTHREAD_
#define _DAYWATCHERTHREAD_

enum DAYWATCHERSETTINGS
{
	WEEKLY		= 1,
	DAILY		= 2,
	MONTHLY		= 3,
	HOURLY		= 4,
	MINUTELY	= 5,
};

struct Events
{
	uint8 eventId;
	uint8 daynumber;
	uint8 monthnumber;
	uint8 activedays;
	time_t lastactivated;
	bool isactive;
	bool eventbyhour;
	uint8 starthour;
	uint8 endhour;
};

typedef std::vector<Events*>								EventsList;

// Multimap typedef's
typedef std::multimap<uint8, uint16>						CreatureEventSpawnMaps;
typedef std::multimap<uint8, uint16>						GameobjectEventSpawnMaps;

class SERVER_DECL DayWatcherThread : public Singleton<DayWatcherThread>, public ThreadContext
{
	bool m_threadRunning;
	bool m_busy;
	bool m_dirty;
	bool _loaded;
	bool runEvents;
	bool _firstrun[2];
	bool m_heroic_reset;

	static const time_t timeout = 120;		/* check every 2 minutes */
	time_t currenttime;
	tm local_currenttime;
	time_t last_arena_time;
	tm local_last_arena_time;
	time_t last_daily_reset_time;
	tm local_last_daily_reset_time;
	time_t last_eventid_time;
	tm local_last_eventid_time;

	uint32 arena_period;

public:
	DayWatcherThread();
	~DayWatcherThread();

	bool run();
	void terminate();
	void maintenence();
	void dupe_tm_pointer(tm * returnvalue, tm * mypointer);
	void load_settings();
	void update_settings();
	void set_tm_pointers();
	uint32 get_timeout_from_string(const char * string, uint32 def);
	bool has_timeout_expired(tm * now_time, tm * last_time, uint32 timeoutval);
	void update_arena();
	void update_daily();
	void Reset_Heroic_Instances();
	//ING Events
	CreatureEventSpawnMaps m_creatureEventSpawnMaps;
	GameobjectEventSpawnMaps m_gameobjectEventSpawnMaps;
	EventsList m_eventIdList;

	void update_event_settings(uint8 eventid, time_t activated);
	void LoadEventIdSettings();
	bool CheckHourlyEvent(tm * now_time, uint8 starthour, uint8 endhour);
	bool has_eventid_timeout_expired(tm * nowtime, int updatetime, uint8 timeoutval);
	bool has_eventid_expired(int activedays, time_t lastactivated);
	bool SpawnEventId(uint8 eventId, bool activate = true);
	uint8 eventToDespawn;
};
#define sDayWatcher DayWatcherThread::getSingleton()

#endif
