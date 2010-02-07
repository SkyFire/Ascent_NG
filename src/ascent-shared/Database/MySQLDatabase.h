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

#ifndef __MYSQLDATABASE_H
#define __MYSQLDATABASE_H

#include <mysql/mysql.h>

struct MySQLDatabaseConnection : public DatabaseConnection
{
	
};

class SERVER_DECL MySQLDatabase : public Database
{
	friend class QueryThread;
	friend class AsyncQuery;

	

public:
	MySQLDatabase();
	~MySQLDatabase();

	bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize);

	void Shutdown();

	string EscapeString(string Escape);
	void EscapeLongString(const char * str, uint32 len, stringstream& out);
	string EscapeString(const char * esc, DatabaseConnection * con);

	bool SupportsReplaceInto() { return true; }
	bool SupportsTableLocking() { return true; }
	
protected:




	void _BeginTransaction(DatabaseConnection * conn);
	void _EndTransaction(DatabaseConnection * conn);
	
};

class SERVER_DECL MySQLQueryResult : public QueryResult
{
public:
	MySQLQueryResult(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount);
	~MySQLQueryResult();

	bool NextRow();

protected:
	
};

#endif		// __MYSQLDATABASE_H
