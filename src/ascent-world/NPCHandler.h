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

#ifndef __NPCHANDLER_H
#define __NPCHANDLER_H

enum GOSSIP_SPECIAL
{
	GOSSIP_NO_SPECIAL		   = 0x00,
	GOSSIP_POI				  = 0x01,
	GOSSIP_SPIRIT_HEALER_ACTIVE = 0x02,
	GOSSIP_VENDOR			   = 0x03,
	GOSSIP_TRAINER			  = 0x04,
	GOSSIP_TABARD_VENDOR		= 0x05,
	GOSSIP_INNKEEPER			= 0x06,
	GOSSIP_PETITIONER		   = 0x07,
	GOSSIP_TAXI				 = 0x08,
	GOSSIP_TEXT				 = 0x09,
	GOSSIP_MENU				 = 0x0A,
	GOSSIP_BATTLEMASTER		 = 0x0B,
	GOSSIP_BANKEER			  = 0x0C,
};

enum TrainerServiceType
{
	TRAINER_SERVICE_AVAILABLE		=0x0,
	TRAINER_SERVICE_UNAVAILABLE	  =0x1,
	TRAINER_SERVICE_USED			 =0x2,
	TRAINER_SERVICE_NOT_SHOWN		=0x3,
	TRAINER_SERVICE_NEVER			=0x4,
	TRAINER_SERVICE_NO_PET		   =0x5
};
#define NUM_TRAINER_SERVICE_TYPES		0x6

enum TrainerType
{
	TRAINER_TYPE_GENERAL			 =0x0,
	TRAINER_TYPE_TALENTS			 =0x1,
	TRAINER_TYPE_TRADESKILLS		 =0x2,
	TRAINER_TYPE_PET				 =0x3
};
#define NUM_TRAINER_TYPES				0x4

#pragma pack(push,1)
struct GossipText_Text
{
	float Prob;
	char * Text[2];
	uint32 Lang;
	uint32 Emote[6];
};

struct GossipText
{
	uint32 ID;
	GossipText_Text Texts[8];
};
#pragma pack(pop)

struct GossipOptions
{
	uint32 ID;
	uint32 GossipID;
	uint16 Icon;
	std::string OptionText;
	uint32 NextTextID;
	uint32 Special;
	float PoiX;
	float PoiY;
	uint32 PoiIcon;
	uint32 PoiFlags;
	uint32 PoiData;
	std::string PoiName;
	uint32 BgMapId;
};

struct GossipNpc
{
	GossipNpc() { pOptions = NULL;  }
	uint32 ID;
	uint32 EntryId;
	uint32 TextID;
	uint32 OptionCount;
	GossipOptions *pOptions;
};

struct trainertype
{
	const char* partialname;
	uint32 type;
};

#define TRAINER_TYPE_MAX 16
extern trainertype trainer_types[TRAINER_TYPE_MAX];

#define TRAINER_STATUS_ALREADY_HAVE		0
#define TRAINER_STATUS_LEARNABLE		1
#define TRAINER_STATUS_NOT_LEARNABLE	2

#endif
