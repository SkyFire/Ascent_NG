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

#include "RStdAfx.h"
#include "../ascent-shared/AuthCodes.h"

void Session::HandleCharacterEnum(WorldPacket & pck)
{
	struct player_item
	{
		uint32 displayid;
		uint8 invtype;
	};

	player_item items[20];
	uint32 slot;
	uint32 i;
	ItemPrototype * proto;

	uint32 start_time = getMSTime();

	// loading characters
	QueryResult* result = CharacterDatabase.Query("SELECT guid, level, race, class, gender, bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending, player_flags, guild_data.guildid FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE acct=%u ORDER BY guid ASC LIMIT 10", GetAccountId());
	QueryResult * res;
	CreatureInfo *info = NULL;
	uint8 num = 0;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data((result ? result->GetRowCount() * 200 : 1));	

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
	data.SetOpcode(SMSG_CHAR_ENUM);

	data << num;
	if( result )
	{
		uint64 guid;
		uint8 Class;
		uint32 banned;
		uint32 bytes2;
		Field *fields;
		do
		{
			fields = result->Fetch();
			guid = fields[0].GetUInt32();
			bytes2 = fields[6].GetUInt32();
			Class = fields[3].GetUInt8();			

			/* build character enum, w0000t :p */
			data << fields[0].GetUInt64();		// guid
			data << fields[7].GetString();		// name
			data << fields[2].GetUInt8();		// race
			data << fields[3].GetUInt8();		// class
			data << fields[4].GetUInt8();		// gender
			data << fields[5].GetUInt32();		// PLAYER_BYTES
			data << uint8(bytes2 & 0xFF);		// facial hair
			data << fields[1].GetUInt8();		// Level
			data << fields[12].GetUInt32();		// zoneid
			data << fields[11].GetUInt32();		// Mapid
			data << fields[10].GetFloat();		// X
			data << fields[9].GetFloat();		// Y
			data << fields[8].GetFloat();		// Z
			data << fields[17].GetUInt32();		// GuildID

			/*
			if( fields[1].GetUInt8() > m_highestLevel )
				m_highestLevel = fields[1].GetUInt8();

			if( Class == DEATHKNIGHT )
				m_hasDeathKnight = true;*/

			banned = fields[13].GetUInt32();
			if(banned && (banned<10 || banned > (uint32)UNIXTIME))
				data << uint32(0x01A04040);
			else
			{
				if(fields[16].GetUInt32() != 0)
					data << uint32(0x00A04342);
				else if(fields[15].GetUInt32() != 0)
					data << (uint32)8704; // Dead (displaying as Ghost)
				else
					data << uint32(1);		// alive
			}

			data << uint32(0);					//Added in 3.0.2
			data << fields[14].GetUInt8();		// Rest State

			if(Class == 9 /*WARLOCK*/ || Class == 3 /*HUNTER*/)
			{
				res = CharacterDatabase.Query("SELECT entry FROM playerpets WHERE ownerguid=%u AND active=1", guid);

				if(res)
				{
					info = CreatureNameStorage.LookupEntry(res->Fetch()[0].GetUInt32());
					delete res;
				}
			}

			if(info)  //PET INFO uint32 displayid,	uint32 level,		 uint32 familyid
				data << uint32(info->Male_DisplayID) << uint32(10) << uint32(info->Family);
			else
				data << uint32(0) << uint32(0) << uint32(0);

			res = CharacterDatabase.Query("SELECT slot, entry FROM playeritems WHERE ownerguid=%u and containerslot=-1 and slot < 19 and slot >= 0", guid);

			memset(items, 0, sizeof(player_item) * 20);
			if(res)
			{
				do 
				{
					proto = ItemPrototypeStorage.LookupEntry(res->Fetch()[1].GetUInt32());
					if(proto)
					{
						slot = res->Fetch()[0].GetUInt32();
						items[slot].displayid = proto->DisplayInfoID;
						items[slot].invtype = proto->InventoryType;
					}
				} while(res->NextRow());
				delete res;
			}

			for( i = 0; i < 20; ++i )
				data << items[i].displayid << items[i].invtype << uint32(0);

			num++;
		}
		while( result->NextRow() );

		delete result;
	}

	data.put<uint8>(0, num);

	DEBUG_LOG("Character Enum", "Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );
}


void Session::HandlePlayerLogin(WorldPacket & pck)
{
	WorldPacket data(SMSG_CHARACTER_LOGIN_FAILED, 30);
	LocationVector LoginCoord;
	Instance * dest;
	ASSERT(!m_currentPlayer);
	uint64 guid;
	pck >> guid;

	if(sClientMgr.GetRPlayer((uint32)guid) != NULL)
	{
		data << uint8(CHAR_LOGIN_DUPLICATE_CHARACTER);
		SendPacket(&data);
		return;
	}

	m_currentPlayer = sClientMgr.CreateRPlayer((uint32)guid);
	RPlayerInfo * p = m_currentPlayer;

	/* Load player data */
	QueryResult * result = CharacterDatabase.Query("SELECT acct, name, level, guild_data.guildid, positionX, positionY, zoneId, mapId, race, class, gender, instance_id, entrypointmap, entrypointx, entrypointy, entrypointz, entrypointo, difficulty FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE guid = %u", guid);
	if(result)
	{
		Field * f = result->Fetch();
		p->AccountId = f[0].GetUInt32();
		p->Name = f[1].GetString();
		p->Level = f[2].GetUInt32();
		p->GuildId = f[3].GetUInt32();
		p->PositionX = f[4].GetFloat();
		p->PositionY = f[5].GetFloat();
		p->ZoneId = f[6].GetUInt32();
		p->MapId = f[7].GetUInt32();
		p->Race = f[8].GetUInt8();
		p->Class = f[9].GetUInt8();
		p->Gender = f[10].GetUInt8();
		p->Latency = m_latency;
		p->GMPermissions = m_GMPermissions;
		p->Account_Flags = m_accountFlags;
		p->InstanceId = f[11].GetUInt32();
		p->RecoveryMapId = f[12].GetUInt32();
		p->RecoveryPosition.ChangeCoords(f[13].GetFloat(), f[14].GetFloat(), f[15].GetFloat(), f[16].GetFloat());
		p->iInstanceType = f[17].GetUInt32();
		p->ClientBuild = m_ClientBuild;
		p->session = this;

		uint8 team = 0;
		// work out the side
		static uint8 teams[12] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0 };
		team = teams[p->Race];

		p->Team = team;

		delete result;
	}
	else
	{
		data << uint8(CHAR_LOGIN_NO_CHARACTER);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	if(IS_MAIN_MAP(m_currentPlayer->MapId))
	{
		/* we're on a continent, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);		
	}
	else
	{
		/* we're in an instanced map, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByInstanceId(m_currentPlayer->InstanceId);

		if(dest && dest->MapId != m_currentPlayer->MapId)
		{
			/* our instance has been deleted or no longer valid, world server probably crashed */
			MapInfo * info = WorldMapInfoStorage.LookupEntry(dest->MapId);
			if(info)
			{
				Log.Error("HandlePlayerLogin", "Instance has been deleted or no longer valid, attempting repop on map %u", info->repopmapid);
				m_currentPlayer->MapId = info->repopmapid;
				LoginCoord.x = info->repopx;
				LoginCoord.y = info->repopy;
				LoginCoord.z = info->repopz;
				LoginCoord.o = 0.0f;

				/* obtain instance */
				dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);
				if(dest)
				{
					data.SetOpcode(SMSG_NEW_WORLD);
					data << m_currentPlayer->MapId << LoginCoord << float(0);
					SendPacket(&data);
					data.clear();
				}
				else
				{
					dest = NULL;
					Log.Error("HandlePlayerLogin", "Repop failed, could not find instance, for map %u", m_currentPlayer->MapId);
				}

			}
			else
			{
				dest = NULL;
				Log.Error("HandlePlayerLogin", "Repop failed, no map info, for map %u", m_currentPlayer->MapId);
			}
		}

		if(!dest)
		{
			Log.Error("HandlePlayerLogin", "Could not find instance, attempting recovery");
			/* our instance has been deleted or no longer valid */
			m_currentPlayer->MapId = m_currentPlayer->RecoveryMapId;
			LoginCoord = m_currentPlayer->RecoveryPosition;

			/* obtain instance */
			dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);
			if(dest)
			{
				data.SetOpcode(SMSG_NEW_WORLD);
                data << m_currentPlayer->MapId << m_currentPlayer->RecoveryPosition << float(0);
				SendPacket(&data);
				data.clear();
			}
		}
	}

	if(!dest || !dest->Server)		// Shouldn't happen
	{
		Log.Error("CharacterHandler", "World server is down!");
		/* world server is down */
		data << uint8(CHAR_LOGIN_NO_WORLD);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	/* log the player into that WS */
	data.SetOpcode(ISMSG_PLAYER_LOGIN);

	/* append info */
	data << uint32(guid) << uint32(dest->MapId) << uint32(dest->InstanceId);

	/* append the account information */
	data << uint32(m_accountId) << uint32(m_accountFlags) << uint32(m_sessionId)
		<< m_GMPermissions << m_accountName << m_ClientBuild;

	dest->Server->SendPacket(&data);
	m_nextServer = dest->Server;
}

void Session::HandleRealmSplitQuery(WorldPacket & pck)
{
	uint32 v;
	pck >> v;

	WorldPacket data(SMSG_REALM_SPLIT, 16);
	data << v << uint32(0);
	data << "01/01/01";
	SendPacket(&data);
}
