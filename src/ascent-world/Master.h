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

#ifndef _MASTER_H
#define _MASTER_H

#include "Common.h"
//#include "Singleton.h"
#include "Config/ConfigEnv.h"
#include "Database/DatabaseEnv.h"
#include "MainServerDefines.h"

#ifndef _VERSION
# define _VERSION "3.3.5a(build 12340)"
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

#define DEFAULT_LOOP_TIME 0 /* 0 millisecs - instant */
#define DEFAULT_LOG_LEVEL 0
#define DEFAULT_PLAYER_LIMIT 100
#define DEFAULT_WORLDSERVER_PORT 8129
#define DEFAULT_REALMSERVER_PORT 3724
#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_REGEN_RATE 0.15
#define DEFAULT_XP_RATE 1
#define DEFAULT_DROP_RATE 1
#define DEFAULT_REST_XP_RATE 1
#define DEFAULT_QUEST_XP_RATE 1
#define DEFAULT_SAVE_RATE 300000	// 5mins

class SERVER_DECL Master : public Singleton<Master>
{
public:
	Master();
	~Master();
	bool Run(int argc, char ** argv);

	static volatile bool m_stopEvent;

private:
	bool _StartDB();
	void _StopDB();

	void _HookSignals();
	void _UnhookSignals();

	static void _OnSignal(int s);
};

#define sMaster Master::getSingleton()

#endif
