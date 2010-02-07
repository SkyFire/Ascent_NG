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


#ifndef WDTFILE_H
#define WDTFILE_H

#define __STORMLIB_SELF__ 

#include "mpq.h"
#include "adtfile.h"
#include "wmo.h"
#include <string>
#include "stdlib.h"

class WDTFile
{
public:
	WDTFile(char* file_name, char* file_name1);
	~WDTFile(void);
	bool init(char *map_id);
    
	string* gWmoInstansName;
	int gnWMO, nMaps;
	
	ADTFile* GetMap(int x, int z);

private:
	MPQFile WDT;
	bool maps[64][64];
	string filename;
};

#endif
