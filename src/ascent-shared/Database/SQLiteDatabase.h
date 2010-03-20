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

#ifndef __SQLITEDATABASE_H
#define __SQLITEDATABASE_H

#include <sqlite/sqlite3.h>

struct SQLiteDatabaseConnection : public DatabaseConnection
{
	sqlite3 * handle;

	char ** tabledata;
	int rows;
	int cols;
};

class SERVER_DECL SQLiteDatabase : public Database
{
	friend class QueryThread;
	friend class AsyncQuery;
public:
	SQLiteDatabase();
	~SQLiteDatabase();

	bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize);

	void Shutdown();

	string EscapeString(string Escape);
	void EscapeLongString(const char * str, uint32 len, stringstream& out);
	string EscapeString(const char * esc, DatabaseConnection * con);
	
protected:

	bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self = false);

	void _BeginTransaction(DatabaseConnection * conn);
	void _EndTransaction(DatabaseConnection * conn);

	QueryResult * _StoreQueryResult(DatabaseConnection * con);

	string mConnectString;
};
/*
class SERVER_DECL PostgresQueryResult : public QueryResult
{
public:
	PostgresQueryResult(PGresult * res, uint32 FieldCount, uint32 RowCount);
	~PostgresQueryResult();

	bool NextRow();

protected:
	PGresult * mResult;
	uint32 mRow;
};
*/
#endif		// __POSTGRESDATABASE_H
