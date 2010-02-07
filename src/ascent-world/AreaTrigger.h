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

#ifndef __AREATRIGGERHANDLER_H
#define __AREATRIGGERHANDLER_H


enum AreaTriggerType
{
	ATTYPE_NULL		 = 0,
	ATTYPE_INSTANCE	 = 1,
	ATTYPE_QUESTTRIGGER = 2,
	ATTYPE_INN		  = 3,
	ATTYPE_TELEPORT	 = 4,
	ATTYPE_SPELL		= 5,
	ATTYPE_BATTLEGROUND = 6,
};

#pragma pack(push,1)
typedef struct AreaTrigger
{
	uint32 AreaTriggerID;
	uint8 Type;
	uint32 Mapid;
	uint32 PendingScreen;
	char * Name;
	float x;
	float y;
	float z;
	float o;
	uint32 required_honor_rank;
	uint32 required_level;
}AreaTrigger;
#pragma pack(pop)



#endif
