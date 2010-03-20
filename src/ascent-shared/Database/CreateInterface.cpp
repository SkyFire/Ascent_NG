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

#include "DatabaseEnv.h"
#include "../CrashHandler.h"
#include "../NGLog.h"

#if defined(ENABLE_DATABASE_MYSQL)
#include "MySQLDatabase.h"
#endif

#if defined(ENABLE_DATABASE_POSTGRES)
#include "PostgresDatabase.h"
#endif

#if defined(ENABLE_DATABASE_SQLITE)
#include "SQLiteDatabase.h"
#endif

void Database::CleanupLibs()
{
#if defined(ENABLE_DATABASE_MYSQL)
		mysql_library_end();
#endif
}

Database * Database::CreateDatabaseInterface(uint32 uType)
{
	switch(uType)
	{
#if defined(ENABLE_DATABASE_MYSQL)
	case 1:		// MYSQL
		return new MySQLDatabase();
		break;
#endif
	
#if defined(ENABLE_DATABASE_POSTGRES)
	case 2:		// POSTGRES
		return new PostgresDatabase();
		break;
#endif

#if defined(ENABLE_DATABASE_SQLITE)
	case 3:		// SQLITE
		return new SQLiteDatabase();
		break;
#endif
	}

	Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "You have attempted to connect to a database that is unsupported or nonexistant.\nCheck your config and try again.");
	abort();
	return NULL;
}
