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


#include "dbcfile.h"
#include "mpq.h"
#include "Stormlib.h"
#define __STORMLIB_SELF__

DBCFile::DBCFile(const std::string &filename) : filename(filename)
{
	data = NULL;
}

bool DBCFile::open()
{
	MPQFile f(filename.c_str());

	// Need some error checking, otherwise an unhandled exception error occurs
	// if people screw with the data path.
	if (f.isEof() == true)
		return false;

	unsigned char header[4];
	unsigned int na,nb,es,ss;

	f.read(header,4); // File Header

	if (header[0]!='W' || header[1]!='D' || header[2]!='B' || header[3] != 'C') {

		f.close();
		data = NULL;
		printf("Critical Error: An error occured while trying to read the DBCFile %s.", filename.c_str());
		return false;
	}

	//assert(header[0]=='W' && header[1]=='D' && header[2]=='B' && header[3] == 'C');

	f.read(&na,4); // Number of records
	f.read(&nb,4); // Number of fields
	f.read(&es,4); // Size of a record
	f.read(&ss,4); // String size
	
	recordSize = es;
	recordCount = na;
	fieldCount = nb;
	stringSize = ss;
	//assert(fieldCount*4 == recordSize);
	assert(fieldCount*4 >= recordSize);

	data = new unsigned char[recordSize*recordCount+stringSize];
	stringTable = data + recordSize*recordCount;
	f.read(data,recordSize*recordCount+stringSize);
	f.close();
	return true;
}

DBCFile::~DBCFile()
{
	delete [] data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
	assert(data);
	return Record(*this, data + id*recordSize);
}

DBCFile::Iterator DBCFile::begin()
{
	assert(data);
	return Iterator(*this, data);
}
DBCFile::Iterator DBCFile::end()
{
	assert(data);
	return Iterator(*this, stringTable);
}

