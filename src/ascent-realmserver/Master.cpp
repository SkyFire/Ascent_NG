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


#include "RStdAfx.h"
#include "../ascent-shared/svn_revision.h"
#include <Console/CConsole.h>

#define BANNER "AscentNG r%u/%s-%s-%s :: Realm Server\n"

#ifndef WIN32
#include <sched.h>
#endif

#include "svn_revision.h"

#include <signal.h>

createFileSingleton( Master );
bool crashed = false;

volatile bool Master::m_stopEvent = false;

// Database defines.
Database* Database_Character;
Database* Database_World;

void Master::_OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		Rehash(true);
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		Master::m_stopEvent = true;
		break;
	}

	signal(s, _OnSignal);
}

Master::Master()
{

}

Master::~Master()
{
}

struct Addr
{
	unsigned short sa_family;
	/* sa_data */
	unsigned short Port;
	unsigned long IP; // inet_addr
	unsigned long unusedA;
	unsigned long unusedB;
};

#define DEF_VALUE_NOT_SET 0xDEADBEEF

#ifdef WIN32
static const char* default_config_file = "ascent-cluster.conf";
static const char* default_realm_config_file = "ascent-realms.conf";
#else
static const char* default_config_file = CONFDIR "/ascent-cluster.conf";
static const char* default_realm_config_file = CONFDIR "/ascent-realms.conf";
#endif

volatile bool bServerShutdown = false;

void Master::Rehash(bool load)
{
	if(load)
	{
		Config.ClusterConfig.SetSource( default_config_file );
	}

	if(!ChannelMgr::getSingletonPtr())
		new ChannelMgr;

	channelmgr.seperatechannels = Config.ClusterConfig.GetBoolDefault("Server", "SeperateChatChannels", false);
	m_lfgForNonLfg = Config.ClusterConfig.GetBoolDefault("Server", "EnableLFGJoin", false);
	if(!Config.ClusterConfig.GetString("GMClient", "GmClientChannel", &GmClientChannel))
	{
		GmClientChannel = "";
	}

	if( load )
		Channel::LoadConfSettings();
}

bool Master::Run(int argc, char ** argv)
{
	char * config_file = (char*)default_config_file;
	char * realm_config_file = (char*)default_realm_config_file;

	int file_log_level = DEF_VALUE_NOT_SET;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;
	int do_cheater_check = 0;
	int do_database_clean = 0;
	time_t curTime;

	struct ascent_option longopts[] =
	{
		{ "checkconf",			ascent_no_argument,			&do_check_conf,			1		},
		{ "screenloglevel",		ascent_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		ascent_required_argument,		&file_log_level,		-1		},
		{ "version",			ascent_no_argument,			&do_version,			1		},
		{ "conf",				ascent_required_argument,		NULL,					'c'		},
		{ "realmconf",			ascent_required_argument,		NULL,					'r'		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = ascent_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 'c':
			config_file = new char[strlen(ascent_optarg)];
			strcpy(config_file, ascent_optarg);
			break;

		case 'r':
			realm_config_file = new char[strlen(ascent_optarg)];
			strcpy(realm_config_file, ascent_optarg);
			break;

		case 0:
			break;
		default:
			sLog.m_fileLogLevel = -1;
			sLog.m_screenLogLevel = 3;
			printf("Usage: %s [--checkconf] [--conf <filename>] [--realmconf <filename>] [--version]\n", argv[0]);
			return true;
		}
	}
	/* set new log levels if used as argument*/
	if( screen_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetScreenLoggingLevel(screen_log_level);

	if( file_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetFileLoggingLevel(file_log_level);

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	/* Print Banner */
	Log.Notice("Server", "==============================================================");
	Log.Notice("Server", "| Ascent Cluster System - Realm Server                     |");
	Log.Notice("Server", "| Version 1.0, Revision %04u                                 |", BUILD_REVISION);
	Log.Notice("Server", "==============================================================");
	Log.Line();

	if( do_check_conf )
	{
		Log.Notice( "Config", "Checking config file: %s", config_file );
		if( Config.ClusterConfig.SetSource(config_file, true ) )
			Log.Success( "Config", "Passed without errors." );
		else
			Log.Warning( "Config", "Encountered one or more errors." );

		Log.Notice( "Config", "Checking config file: %s\n", realm_config_file );
		if( Config.RealmConfig.SetSource( realm_config_file, true ) )
			Log.Success( "Config", "Passed without errors.\n" );
		else
			Log.Warning( "Config", "Encountered one or more errors.\n" );

		/* test for die variables */
		string die;
		if( Config.ClusterConfig.GetString( "die", "msg", &die) || Config.ClusterConfig.GetString("die2", "msg", &die ) )
			Log.Warning( "Config", "Die directive received: %s", die.c_str() );

		return true;
	}

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time.\n" );
	Log.Line();

	//use these log_level until we are fully started up.
	sLog.Init(-1, 3);

#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
		Log.LargeErrorMessage( LARGERRORMESSAGE_WARNING, "You are running AscentNG as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
#endif

	ThreadPool.Startup();
	uint32 LoadingTime = getMSTime();

	_HookSignals();

	Log.Line();

	Log.Notice( "Config", "Loading Config Files..." );
	if( Config.ClusterConfig.SetSource( config_file ) )
		Log.Success( "Config", ">> %s", config_file );
	else
	{
		Log.Error( "Config", ">> %s", config_file );
		return false;
	}

	string die;
	if( Config.ClusterConfig.GetString( "die", "msg", &die) || Config.ClusterConfig.GetString( "die2", "msg", &die ) )
	{
		Log.Warning( "Config", "Die directive received: %s", die.c_str() );
		return false;
	}	

	if(Config.RealmConfig.SetSource(realm_config_file))
		Log.Success( "Config", ">> %s", realm_config_file );
	else
	{
		Log.Error( "Config", ">> %s", realm_config_file );
		return false;
	}

	Rehash(true);

	if( !_StartDB() )
	{
		Database::CleanupLibs();
		ThreadPool.Shutdown();
		_UnhookSignals();
		return false;
	}
	Log.Success("Database", "Connections established...");

	new ClusterMgr;
	new ClientMgr;

	Log.Line();

	ThreadPool.ShowStats();
	Log.Line();

	if( !LoadRSDBCs() )
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "One or more of the DBC files are missing.", "These are absolutely necessary for the server to function.", "The server will not start without them.", NULL);
		return false;
	}

	Log.Success("Storage", "DBC Files Loaded...");
	Storage_Load();

	sLocalizationMgr.Reload(true);
	Log.Success("Locale", "Localization Manager Loaded...");

	Log.Line();

	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	/* connect to LS */
	new LogonCommHandler;
	sLogonCommHandler.Startup();

	Log.Success("Network", "Network Subsystem Started.");

	Log.Notice("Network", "Opening Client Port...");
	ListenSocket<WorldSocket> * wsl = new ListenSocket<WorldSocket>("0.0.0.0", 8129);
	bool lsc = wsl->IsOpen();

	Log.Notice("Network", "Opening Server Port...");
	ListenSocket<WSSocket> * isl = new ListenSocket<WSSocket>("0.0.0.0", 11010);
	bool ssc = isl->IsOpen();

	if(!lsc || !ssc)
	{
		Log.Error("Network", "Could not open one of the sockets.");
		return 1;
	}

	ThreadPool.ExecuteTask( isl );
	ThreadPool.ExecuteTask( wsl );

	ConsoleThread * console = new ConsoleThread();
	ThreadPool.ExecuteTask(console);

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();

	sSocketMgr.SpawnWorkerThreads();

	LoadingTime = getMSTime() - LoadingTime;
	Log.Success("Server","Ready for connections. Startup time: %ums\n", LoadingTime );

	m_startTime = uint32(UNIXTIME);

	//Update sLog to obey config setting
	sLog.Init(Config.ClusterConfig.GetIntDefault("LogLevel", "File", -1),Config.ClusterConfig.GetIntDefault("LogLevel", "Screen", 1));

	/* write pid file */
	FILE * fPid = fopen( "ascent-realmserver.pid", "w" );
	if( fPid )
	{
		uint32 pid;
#ifdef WIN32
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf( fPid, "%u", (unsigned int)pid );
		fclose( fPid );
	}
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif

	uint32 loopcounter = 0;
	uint32 start = 0;
	uint32 diff = 0;
	uint32 last_time = 0;
	uint32 etime = 0;
	//ThreadPool.Gobble();

	/* voicechat */
#ifdef VOICE_CHAT
	new VoiceChatHandler();
	sVoiceChatHandler.Startup();
#endif

	while(!m_stopEvent)
	{
		start = now();
		diff = start - last_time;
		if(! ((++loopcounter) % 10000) )		// 5mins
		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();//Checks if THREAD_RESERVE is met
		}

		/* since time() is an expensive system call, we only update it once per server loop */
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&curTime);
		}

#ifdef VOICE_CHAT
		sVoiceChatHandler.Update();
#endif

		sLogonCommHandler.UpdateSockets();
		//wsl->Update();
		//isl->Update();
		sClientMgr.Update();
		sClusterMgr.Update();
		sSocketGarbageCollector.Update();

		/* UPDATE */
		last_time = now();
		etime = last_time - start;
		if( 50 > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, 50 - etime );
#else
			Sleep( 50 - etime );
#endif
		}
	}
	// begin server shutdown
	Log.Notice( "Shutdown", "Initiated at %s", ConvertTimeStampToDataTime( (uint32)UNIXTIME).c_str() );
	bServerShutdown = true;

	_UnhookSignals();

	Log.Notice("ChannelMgr", "~ChannelMgr()");
	delete ChannelMgr::getSingletonPtr();

	delete LogonCommHandler::getSingletonPtr();
	Log.Success("~LogonComm", "LogonCommHandler shut down");

	sSocketMgr.CloseAll();
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif
	Log.Success("~Network", "Network Subsystem shut down.");

	Log.Notice( "~Network", "Deleting network subsystem..." );
	delete SocketGarbageCollector::getSingletonPtr();
	delete SocketMgr::getSingletonPtr();

	Log.Notice("~Network", "Closing Client Port...");
	delete wsl;

	Log.Notice("~Network", "Closing Server Port...");
	delete isl;

	//sLocalizationMgr.Shutdown();
	Log.Success("~Locale", "Localization Manager shut down.");

	Storage_Cleanup();
	Log.Success("~Storage", "DBC Files Unloaded...");

	delete ClusterMgr::getSingletonPtr();
	delete ClientMgr::getSingletonPtr();

	CharacterDatabase.EndThreads();
	WorldDatabase.EndThreads();
	Database::CleanupLibs();
	Log.Notice( "Database", "Closing Connections..." );
	_StopDB();
	Log.Success("~Database", "Shut down.");

	Log.Notice("~ThreadPool", "Ending %u active threads...", ThreadPool.GetActiveThreadCount());
	ThreadPool.Shutdown();

	/* Shut down console system */
	console->terminate();
	delete console;

	// remove pid
	remove( "ascent-realmserver.pid" );

	Log.Notice( "Shutdown", "Shutdown complete." );

#ifdef WIN32
	WSACleanup();
#endif

	return true;
}

bool Master::_StartDB()
{
	string hostname, username, password, database;
	int port = 0;
	// Configure Main Database

	bool result = Config.ClusterConfig.GetString( "WorldDatabase", "Username", &username );
	Config.ClusterConfig.GetString( "WorldDatabase", "Password", &password );
	result = !result ? result : Config.ClusterConfig.GetString( "WorldDatabase", "Hostname", &hostname );
	result = !result ? result : Config.ClusterConfig.GetString( "WorldDatabase", "Name", &database );
	result = !result ? result : Config.ClusterConfig.GetInt( "WorldDatabase", "Port", &port );
	Database_World = Database::Create();

	if(result == false)
	{
		OUT_DEBUG( "SQL: One or more parameters were missing from WorldDatabase directive." );
		return false;
	}

	// Initialize it
	if( !WorldDatabase.Initialize(hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.ClusterConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		OUT_DEBUG( "SQL: Main database initialization failed. Exiting." );
		return false;
	}

	result = Config.ClusterConfig.GetString( "CharacterDatabase", "Username", &username );
	Config.ClusterConfig.GetString( "CharacterDatabase", "Password", &password );
	result = !result ? result : Config.ClusterConfig.GetString( "CharacterDatabase", "Hostname", &hostname );
	result = !result ? result : Config.ClusterConfig.GetString( "CharacterDatabase", "Name", &database );
	result = !result ? result : Config.ClusterConfig.GetInt( "CharacterDatabase", "Port", &port );
	Database_Character = Database::Create();

	if(result == false)
	{
		OUT_DEBUG( "SQL: One or more parameters were missing from Database directive." );
		return false;
	}

	// Initialize it
	if( !CharacterDatabase.Initialize( hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.ClusterConfig.GetIntDefault( "CharacterDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		OUT_DEBUG( "sql: Main database initialization failed. Exiting." );
		return false;
	}

	return true;
}

void Master::_StopDB()
{
	delete Database_World;
	delete Database_Character;
}

#ifndef WIN32
// Unix crash handler :oOoOoOoOoOo
volatile bool m_crashed = false;
void segfault_handler(int c)
{
	if( m_crashed )
	{
		abort();
		return;		// not reached
	}

	m_crashed = true;

	printf ("Segfault handler entered...\n");
	try
	{
		if( World::getSingletonPtr() != 0 )
		{
			sLog.outString( "Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			sLog.outString( "All pending database operations cleared.\n" );
			sWorld.SaveAllPlayers();
			sLog.outString( "Data saved." );
		}
	}
	catch(...)
	{
		sLog.outString( "Threw an exception while attempting to save all data." );
	}

	printf("Writing coredump...\n");
	abort();
}
#endif


void Master::_HookSignals()
{
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
#ifdef _WIN32
	signal( SIGBREAK, _OnSignal );
#else
	signal( SIGHUP, _OnSignal );
	signal(SIGUSR1, _OnSignal);

	// crash handler
	signal(SIGSEGV, segfault_handler);
	signal(SIGFPE, segfault_handler);
	signal(SIGILL, segfault_handler);
	signal(SIGBUS, segfault_handler);
#endif
}

void Master::_UnhookSignals()
{
	signal( SIGINT, 0 );
	signal( SIGTERM, 0 );
	signal( SIGABRT, 0 );
#ifdef _WIN32
	signal( SIGBREAK, 0 );
#else
	signal( SIGHUP, 0 );
#endif

}

#ifdef WIN32

Mutex m_crashedMutex;

// Crash Handler
void OnCrash( bool Terminate )
{
	sLog.outString( "Advanced crash handler initialized." );

	if( !m_crashedMutex.AttemptAcquire() )
		TerminateThread( GetCurrentThread(), 0 );

	try
	{
		if( ClusterMgr::getSingletonPtr() != 0 )
		{
			sLog.outString( "Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			sLog.outString( "All pending database operations cleared.\n" );
			//sClusterMgr.SaveAllPlayers();
			//sLog.outString( "Data saved." );
		}
	}
	catch(...)
	{
		sLog.outString( "Threw an exception while attempting to save all data." );
	}

	sLog.outString( "Closing." );

	// Terminate Entire Application
	if( Terminate )
	{
		HANDLE pH = OpenProcess( PROCESS_TERMINATE, TRUE, GetCurrentProcessId() );
		TerminateProcess( pH, 1 );
		CloseHandle( pH );
	}
}

#endif