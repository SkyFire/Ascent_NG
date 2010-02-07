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

MySQLDatabase::~MySQLDatabase()
{
	for(int32 i = 0; i < mConnectionCount; ++i)
		delete Connections[i];

	delete [] Connections;
}

MySQLDatabase::MySQLDatabase() : Database()
{

}

void MySQLDatabase::_BeginTransaction(DatabaseConnection * conn)
{
	_SendQuery( conn, "START TRANSACTION", false );
}

void MySQLDatabase::_EndTransaction(DatabaseConnection * conn)
{
	_SendQuery( conn, "COMMIT", false );
}

bool MySQLDatabase::Initialize(const char* Hostname, unsigned int port, const char* Username, const char* Password, const char* DatabaseName, uint32 ConnectionCount, uint32 BufferSize)
{


	Database::_Initialize();
	return true;
}

string MySQLDatabase::EscapeString(string Escape)
{

}





void MySQLDatabase::Shutdown()
{
	// TODO
}



MySQLQueryResult::MySQLQueryResult(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount) : QueryResult(FieldCount, RowCount), mResult( res )
{
	mCurrentRow = new Field[FieldCount];
}

MySQLQueryResult::~MySQLQueryResult()
{

}

bool MySQLQueryResult::NextRow()
{

}



#endif
