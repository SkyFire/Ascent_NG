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

#include "dbcfile.h"
#include <stdio.h>
//#include "../Common.h"
DBCFile::DBCFile()
{
	data = NULL;
	stringTable = NULL;
}

DBCFile::~DBCFile()
{
	if(	data )
		delete[] data;
	if ( stringTable )
		delete[] stringTable;
}

bool DBCFile::open(const char*fn)
{
	if(data)
	{
		delete [] data;
		data=NULL;
	}
	FILE*pf=fopen(fn,"rb");
	if(!pf)return false;

	fread(header,4,1,pf); // Number of records
	assert(header[0]=='W' && header[1]=='D' && header[2]=='B' && header[3] == 'C');
	fread(&recordCount,4,1,pf); // Number of records
	fread(&fieldCount,4,1,pf); // Number of fields
	fread(&recordSize,4,1,pf); // Size of a record
	fread(&stringSize,4,1,pf); // String size

	data = new unsigned char[recordSize*recordCount];
	stringTable = new unsigned char[stringSize];
	//data = (unsigned char *)malloc (recordSize*recordCount); 
	//stringTable = (unsigned char *)malloc ( stringSize ) ;
	fread( data ,recordSize*recordCount,1,pf);
	fread( stringTable , stringSize,1,pf);

	fclose(pf);
	return true;
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

bool DBCFile::DumpBufferToFile(const char*fn)
{
  FILE * pFile;
  pFile = fopen ( fn , "wb" );
  if(!pFile)
	  return false;

  //write header stuff
  fwrite ((const void *)&header , 4 , 1 , pFile );
  fwrite ((const void *)&recordCount , 4 , 1 , pFile );
  fwrite ((const void *)&fieldCount , 4 , 1 , pFile );
  fwrite ((const void *)&recordSize , 4 , 1 , pFile );
  fwrite ((const void *)&stringSize , 4 , 1 , pFile );

  //now the main part is the data
  fwrite (data , recordSize*recordCount , 1 , pFile );
  fwrite (stringTable , stringSize , 1 , pFile );

  //and pull out
  fclose (pFile);
  return true;
}

/*
//Don't uncomment these since we cant mix realloc with new
//these could be reimplemented by using a vector, but do we really need them?!

int DBCFile::AddRecord() //simply add an empty record to the end of the data section
{
	recordCount++;
	if( data )
	{
		data = (unsigned char *)realloc( data, recordCount*recordSize );
		memset( data + (recordCount - 1) * recordSize, 0, recordSize);//make sure no random values get here
	}
	else 
	{
		//the dbc file is not yet opened
		printf(" Error : adding record to an unopened DBC file\n");
		recordCount = 0;
		return 0;
	}

	//seems like an error ocured
	if ( !data )
	{
		printf(" Error : Could not resize DBC data partition\n");
		recordCount = 0;
		return 0;
	}

	return (recordCount - 1);
}

int DBCFile::AddString(const char *new_string) //simply add an empty record to the end of the string section
{

	size_t new_str_len = strlen( new_string ) + 1;

	if( new_str_len == 0 )
		return 0; //we do not add 0 len strings

	if( stringTable )
	{
		stringTable = (unsigned char *)realloc( stringTable, stringSize + new_str_len );
	}
	else 
	{
		//the dbc file is not yet opened
		printf(" Error : adding string to an unopened DBC file\n");
		stringSize = 0;
		return 0;
	}

	//seems like an error occurred
	if ( !stringTable )
	{
		printf(" Error : Could not resize DBC string partition\n");
		stringSize = 0;
		return 0;
	}

	memcpy( stringTable + stringSize, new_string, new_str_len );

	int ret = stringSize;

	stringSize += (int)new_str_len;

	return ret;
}
*/
