/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef _R_MASTER_H
#define _R_MASTER_H

#include "Common.h"
#include "Config/ConfigEnv.h"
#include "Database/DatabaseEnv.h"
#include "ascent_getopt.h"

#ifndef _VERSION
# define _VERSION "3.3.x"
#endif

#if PLATFORM == PLATFORM_WIN32
# define _FULLVERSION _VERSION "-GIT (Win32)"
#else
# define _FULLVERSION _VERSION "-GIT (Unix)"
#endif

#ifdef _DEBUG
#define BUILDTYPE "Debug"
#else
#define BUILDTYPE "Release"
#endif

extern Database * Database_World;
extern Database * Database_Character;

#define WorldDatabase (*Database_World)
#define CharacterDatabase (*Database_Character)

class SERVER_DECL Master : public Singleton<Master>
{
public:
	Master();
	~Master();
	bool Run(int argc, char ** argv);
	void Rehash(bool load);

	static volatile bool m_stopEvent;
	uint32 m_startTime;
	std::string GetUptimeString()
	{
		char str[300];
		time_t pTime = (time_t)UNIXTIME - m_startTime;
		tm * tmv = gmtime(&pTime);

		snprintf(str, 300, "%u days, %u hours, %u minutes, %u seconds.", tmv->tm_yday, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);
		return string(str);
	}

	bool m_lfgForNonLfg;
	std::string GmClientChannel;

private:
	bool _StartDB();
	void _StopDB();

	void _HookSignals();
	void _UnhookSignals();

	static void _OnSignal(int s);
};

#define sRMaster Master::getSingleton()

#endif
