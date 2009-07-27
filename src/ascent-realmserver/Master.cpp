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

#include "RStdAfx.h"
#include "../ascent-shared/svn_revision.h"

Database * Database_Character;
Database * Database_World;
bool running;

void _OnSignal(int s)
{
	switch (s)
	{

	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		running = false;
		break;
	}

	signal(s, _OnSignal);

}

#ifdef WIN32
        static const char* default_config_file = "ascent-world.conf";
        static const char* default_realm_config_file = "ascent-realms.conf";
#else
        static const char* default_config_file = CONFDIR "/ascent-world.conf";
        static const char* default_realm_config_file = CONFDIR "/ascent-realms.conf";
#endif

int main(int argc, char *argv[])
{

	//init signals
	signal(SIGINT, _OnSignal);
	signal(SIGTERM, _OnSignal);
	signal(SIGABRT, _OnSignal);
#ifdef _WIN32
	signal(SIGBREAK, _OnSignal);
#else
	signal(SIGHUP, _OnSignal);
	signal(SIGUSR1, _OnSignal);
#endif
	Config.MainConfig.SetSource(default_config_file);
	Config.RealmConfig.SetSource(default_realm_config_file);

	running = true;
	sLog.Init(-1, -1);
	sLog.outString("TexT");
	/* Initialize global timestamp */
	UNIXTIME = time(NULL);

    /* Print Banner */
	Log.Notice("Server", "==============================================================");
	Log.Notice("Server", "| Codename Ascent Cluster - Realm Server                     |");
	Log.Notice("Server", "| Version 0.1, Revision %04u                                 |", BUILD_REVISION);
	Log.Notice("Server", "==============================================================");
	Log.Line();

	Database_Character = Database::CreateDatabaseInterface(1);
	Database_World = Database::CreateDatabaseInterface(1);
	Log.Success("Database", "Interface Created.");

	newSingleton(ClusterMgr);
	newSingleton(ClientMgr);
	ThreadPool.Startup();
	Log.Line();
	Log.Notice("Database", "Connecting to database...");

	string hostname, username, password, database;
	int port = 0;
	
	bool result = Config.MainConfig.GetString( "WorldDatabase", "Username", &username );
	Config.MainConfig.GetString( "WorldDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "WorldDatabase", "Port", &port );

	if(!result)
	{
		Log.Error("Database", "One or more parameters were missing from WorldDatabase directive.");
		exit(-1);
	}

	if(!Database_World->Initialize(hostname.c_str(), port, username.c_str(), password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 3 ), 1000))
	{
		Log.Error("Database", "One or more errors occured while connecting to databases.");
		exit(-1);
	}

	result = Config.MainConfig.GetString( "CharacterDatabase", "Username", &username );
	Config.MainConfig.GetString( "CharacterDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "CharacterDatabase", "Port", &port );

	if(!result)
	{
		Log.Error("Database", "One or more parameters were missing from CharacterDatabase directive.");
		exit(-1);
	}

	if(!Database_Character->Initialize(hostname.c_str(), port, username.c_str(), password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 3 ), 1000))
	{
		Log.Error("Database", "One or more errors occured while connecting to databases.");
		exit(-1);
	}


	Log.Success("Database", "Connections established.");

	Log.Line();

	Log.Success("Storage", "DBC Files Loaded...");
	Storage_Load();

	Log.Line();

	newSingleton(SocketMgr);
	newSingleton(SocketGarbageCollector);
	sSocketMgr.SpawnWorkerThreads(Config.MainConfig.GetIntDefault("Network", "SocketWorkers", 0));

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
	/* connect to LS */
	newSingleton(LogonCommHandler);
	sLogonCommHandler.Startup();

	/* main loop */
#ifdef WIN32
	ThreadPool.ExecuteTask(wsl);
	ThreadPool.ExecuteTask(isl);
	while (running)
#else
	while (running && wsl->IsOpen() && isl->IsOpen())
#endif
	{
        sLogonCommHandler.UpdateSockets();
		//wsl->Update();
		//isl->Update();
		sClientMgr.Update();
		sClusterMgr.Update();
		Sleep(50);
	}

	// send a query to wake it up if its inactive
	Log.Notice("Database", "Clearing all pending queries...");

	// kill the database thread first so we don't lose any queries/data
	Database_Character->EndThreads();
	Database_World->EndThreads();
	Database_Character->Shutdown();
	Database_World->Shutdown();
	delete Database_Character;
	delete Database_World;

	Log.Notice("Network", "Shutting down network subsystem.");
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif
	sSocketMgr.CloseAll();

	Log.Notice("Network", "Closing inter-communication threads");
	isl->Close();
	wsl->Close();

	Log.Notice("ThreadPool", "Shutting down thread pool");
	ThreadPool.Shutdown();

	delSingleton(LogonCommHandler);
}

#ifdef WIN32
void OnCrash(bool Terminate)
{
	try
	{
		Database_World->EndThreads();
		Database_Character->EndThreads();
	}
	catch (...)
	{
		Log.Error("Database", "Threw exception waiting for queries to finish");
	}

	try
	{
		WorldPacket data(ISMSG_SAVE_ALL_PLAYERS);
		sClusterMgr.DistributePacketToAll(&data);
	}
	catch (...)
	{
		Log.Error("Network", "Threw exception telling world servers to save players");
	}
}
#endif
