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

#include "StdAfx.h"

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	PlayerInfo *pn = objmgr.GetPlayerInfo( (uint32)guid );
	WoWGuid pguid(guid);

	if(!pn)
		return;

	DEBUG_LOG("WorldSession","Received CMSG_NAME_QUERY for: %s", pn->name );

	uint8 databuffer[5000];
	StackPacket data(SMSG_NAME_QUERY_RESPONSE, databuffer, 5000);
	data << pguid;
	data << uint8(0);
	data << pn->name;
	data << uint8(0);	   // this probably is "different realm" or something flag.
	data << uint8(pn->race);
	data << uint8(pn->gender);
	data << uint8(pn->cl);
	data << uint8(0);
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void WorldSession::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	uint32 t = (uint32)UNIXTIME;
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCreatureQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	//WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
	uint8 databuffer[10000];
	StackPacket data(SMSG_CREATURE_QUERY_RESPONSE, databuffer, 10000);
	uint32 entry;
	uint64 guid;
	CreatureInfo *ci;

	recv_data >> entry;
	recv_data >> guid;

	if(entry == 300000)
	{
		data << (uint32)entry;
		data << "WayPoint";
		data << uint8(0) << uint8(0) << uint8(0);
		data << "Level is WayPoint ID";
		for(uint32 i = 0; i < 8;i++)
		{
			data << uint32(0);
		}
		data << uint8(0);  
	}
	else
	{
		ci = CreatureNameStorage.LookupEntry(entry);
		if(ci == NULL)
			return;

		DEBUG_LOG("WORLD","HandleCreatureQueryOpcode CMSG_CREATURE_QUERY '%s'", ci->Name);
		data << (uint32)entry;
		data << ci->Name;
		data << uint8(0) << uint8(0) << uint8(0);
		data << ci->SubName;

		data << ci->info_str; //!!! this is a string in 2.3.0 Example: stormwind guard has : "Direction"
		data << ci->Flags1;  
		data << ci->Type;
		data << ci->Family;
		data << ci->Rank;
		data << ci->SpellDataID;
		data << ci->Male_DisplayID;
		data << ci->Female_DisplayID;
		data << ci->Male_DisplayID2;
		data << ci->Female_DisplayID2;
		data << ci->Unknown1;
		data << ci->unkfloat1;
		data << ci->unkfloat2;
		data << ci->Leader;
		data << uint32(0);	// unk
		data << uint32(0);	// unk
		data << uint32(0);	// unk
		data << uint32(0);	// unk
		data << uint32(0);	// unk
	}

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	//WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 300);
	uint8 databuffer[10000];
	StackPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, databuffer, 10000);

	uint32 entryID;
	uint64 guid;
	GameObjectInfo *goinfo;
	

	recv_data >> entryID;
	recv_data >> guid;

	DEBUG_LOG("WORLD","HandleGameObjectQueryOpcode CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == NULL)
		return;
    
	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	data << goinfo->Name;

	data << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0);   // new string in 1.12
	data << goinfo->SpellFocus;
	data << goinfo->sound1;
	data << goinfo->sound2;
	data << goinfo->sound3;
	data << goinfo->sound4;
	data << goinfo->sound5;
	data << goinfo->sound6;
	data << goinfo->sound7;
	data << goinfo->sound8;
	data << goinfo->sound9;
	data << goinfo->Unknown1;
	data << goinfo->Unknown2;
	data << goinfo->Unknown3;
	data << goinfo->Unknown4;
	data << goinfo->Unknown5;
	data << goinfo->Unknown6;
	data << goinfo->Unknown7;
	data << goinfo->Unknown8;
	data << goinfo->Unknown9;
	data << goinfo->Unknown10;
	data << goinfo->Unknown11;
	data << goinfo->Unknown12;
	data << goinfo->Unknown13;
	data << goinfo->Unknown14;

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_CORPSE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCorpseQueryOpcode(WorldPacket &recv_data)
{
	DEBUG_LOG("WORLD","HandleCorpseQueryOpcode Received MSG_CORPSE_QUERY");

	Corpse* pCorpse;
	//WorldPacket data(MSG_CORPSE_QUERY, 21);
	uint8 databuffer[100];
	StackPacket data(MSG_CORPSE_QUERY, databuffer, 100);
	MapInfo *pPMapinfo = NULL;

	if(_player->isDead())
		_player->BuildPlayerRepop();

	pCorpse = objmgr.GetCorpseByOwner(_player->GetLowGUID());
	if(pCorpse)
	{
		MapInfo *pPMapinfo = NULL;
		pPMapinfo = WorldMapInfoStorage.LookupEntry(pCorpse->GetMapId());
		if(pPMapinfo == NULL)
		{
			if(pPMapinfo != NULL && !(pPMapinfo->type == INSTANCE_NULL || pPMapinfo->type == INSTANCE_PVP))
			{
				data << uint8(0x01); //show ?
				data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
				data << pCorpse->GetPositionX();
				data << pCorpse->GetPositionY();
				data << pCorpse->GetPositionZ();
				data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
				SendPacket(&data);
			}
			else
			{
				data << uint8(0x01); //show ?
				data << pPMapinfo->repopmapid; // mapid (that tombstones shown on)
				data << pPMapinfo->repopx;
				data << pPMapinfo->repopy;
				data << pPMapinfo->repopz;
				data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
				data << uint32(0); // 3.2.2
				SendPacket(&data);
			}
		}
		else
		{

			data.Initialize(MSG_CORPSE_QUERY);
			data << uint8(0x01); //show ?
			data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
			data << pCorpse->GetPositionX();
			data << pCorpse->GetPositionY();
			data << pCorpse->GetPositionZ();
			data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
			data << uint32(0); // 3.2.2
			SendPacket(&data);

		}
	}
}

void WorldSession::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	uint8 buffer[10000];
	StackPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE,buffer, 10000);
	recv_data >> pageid;

	while(pageid)
	{
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(page == NULL)
			return;

		data.Clear();
		data << pageid;
		data.Write((uint8*)page->text, strlen(page->text) + 1);

		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleItemNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint8 databuffer[1000];
	StackPacket reply(SMSG_ITEM_NAME_QUERY_RESPONSE, databuffer, 1000);

	uint32 itemid;
	recv_data >> itemid;
	reply << itemid;
	ItemPrototype *proto=ItemPrototypeStorage.LookupEntry(itemid);
	if(!proto)
		reply << "Unknown Item";
	else
	{
		reply << proto->Name1;
	}
	SendPacket(&reply);	
}

void WorldSession::HandleInrangeQuestgiverQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	//WorldPacket data(SMSG_QUESTGIVER_STATUS_MULTIPLE, 1000);
	uint8 databuffer[10000];
	StackPacket data(SMSG_QUESTGIVER_STATUS_MULTIPLE, databuffer, 10000);
	Object::InRangeSet::iterator itr;
	Creature* pCreature;
	uint32 count = 0;
	data << count;

	// 32 count
	// <foreach count>
	//    64 guid
	//    8 status

	for( itr = _player->m_objectsInRange.begin(); itr != _player->m_objectsInRange.end(); ++itr )
	{
		pCreature = TO_CREATURE(*itr);
		if( pCreature->GetTypeId() != TYPEID_UNIT )
			continue;

		if( pCreature->isQuestGiver() )
		{
			data << pCreature->GetGUID();
			data << sQuestMgr.CalcStatus( pCreature, _player );
			++count;
		}
	}

	*(uint32*)(data.GetBufferPointer()) = count;
	SendPacket(&data);
}
