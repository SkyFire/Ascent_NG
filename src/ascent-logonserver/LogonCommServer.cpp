/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "LogonStdAfx.h"
#pragma pack(push, 1)
typedef struct
{
	uint16 opcode;
	uint32 size;
}logonpacket;
#pragma pack(pop)

ASCENT_INLINE static void swap32(uint32* p) { *p = ((*p >> 24 & 0xff)) | ((*p >> 8) & 0xff00) | ((*p << 8) & 0xff0000) | (*p << 24); }

LogonCommServerSocket::LogonCommServerSocket(SOCKET fd) : Socket(fd, 65536, 524288)
{
	// do nothing
	last_ping = (uint32)time(NULL);
	next_server_ping = last_ping + 30;
	remaining = opcode = 0;
	removed = true;

	use_crypto = false;
	authenticated = 0;
}

LogonCommServerSocket::~LogonCommServerSocket()
{

}

void LogonCommServerSocket::OnConnect()
{
	if( !IsServerAllowed(GetRemoteAddress().s_addr) )
	{
		printf("Server connection from %s:%u DENIED, not an allowed IP.\n", GetRemoteIP().c_str(), GetRemotePort());
		Disconnect();
		return;
	}

	sInfoCore.AddServerSocket(this);
	removed = false;
}

void LogonCommServerSocket::OnDisconnect()
{
	// if we're registered -> de-register
	if(!removed)
	{
		set<uint32>::iterator itr = server_ids.begin();
		for(; itr != server_ids.end(); ++itr)
			sInfoCore.SetRealmOffline((*itr), this);

		sInfoCore.RemoveServerSocket(this);
	}
}

void LogonCommServerSocket::OnRead()
{
	while(true)
	{
		if(!remaining)
		{
			if(readBuffer.GetSize() < 6)
				return;	 // no header

			// read header
			readBuffer.Read(&opcode, 2);
			readBuffer.Read(&remaining, 4);

			if(use_crypto)
			{
				// decrypt the packet
				recvCrypto.Process((unsigned char*)&opcode, (unsigned char*)&opcode, 2);
				recvCrypto.Process((unsigned char*)&remaining, (unsigned char*)&remaining, 4);
			}

			/* reverse byte order */
			swap32(&remaining);
		}

		// do we have a full packet?
		if(readBuffer.GetSize() < remaining)
			return;

		// create the buffer
		WorldPacket buff(opcode, remaining);
		if(remaining)
		{
			buff.resize(remaining);
			readBuffer.Read((uint8*)buff.contents(), remaining);
		}

		if(use_crypto && remaining)
			recvCrypto.Process((unsigned char*)buff.contents(), (unsigned char*)buff.contents(), remaining);

		// handle the packet
		HandlePacket(buff);

		remaining = 0;
		opcode = 0;
	}
}

void LogonCommServerSocket::HandlePacket(WorldPacket & recvData)
{
	if(authenticated == 0 && recvData.GetOpcode() != RCMSG_AUTH_CHALLENGE)
	{
		// invalid
		Disconnect();
		return;
	}

	static logonpacket_handler Handlers[RMSG_COUNT] = {
		NULL,												// RMSG_NULL
		&LogonCommServerSocket::HandleRegister,				// RCMSG_REGISTER_REALM
		NULL,												// RSMSG_REALM_REGISTERED
		&LogonCommServerSocket::HandleSessionRequest,		// RCMSG_REQUEST_SESSION
		NULL,												// RSMSG_SESSION_RESULT
		&LogonCommServerSocket::HandlePing,					// RCMSG_PING
		NULL,												// RSMSG_PONG
		NULL,/*Deprecated*/									// RCMSG_SQL_EXECUTE
		NULL,/*Deprecated*/									// RCMSG_RELOAD_ACCOUNTS
		&LogonCommServerSocket::HandleAuthChallenge,		// RCMSG_AUTH_CHALLENGE
		NULL,												// RSMSG_AUTH_RESPONSE
		NULL,												// RSMSG_REQUEST_ACCOUNT_CHARACTER_MAPPING
		&LogonCommServerSocket::HandleMappingReply,			// RCMSG_ACCOUNT_CHARACTER_MAPPING_REPLY
		&LogonCommServerSocket::HandleUpdateMapping,		// RCMSG_UPDATE_CHARACTER_MAPPING_COUNT
		NULL,												// RSMSG_DISCONNECT_ACCOUNT
		&LogonCommServerSocket::HandleTestConsoleLogin,		// RCMSG_TEST_CONSOLE_LOGIN
		NULL,												// RSMSG_CONSOLE_LOGIN_RESULT
		&LogonCommServerSocket::HandleDatabaseModify,		// RCMSG_DATABASE_MODIFY
		NULL,												// RSMSG_SERVER_PING
		&LogonCommServerSocket::HandleServerPong,			// RCMSG_SERVER_PONG
	};

	if(recvData.GetOpcode() >= RMSG_COUNT || Handlers[recvData.GetOpcode()] == 0)
	{
		printf("Got unknwon packet from logoncomm: %u\n", recvData.GetOpcode());
		return;
	}

	(this->*(Handlers[recvData.GetOpcode()]))(recvData);
}

void LogonCommServerSocket::HandleRegister(WorldPacket & recvData)
{
	Realm * realm;
	string realmName;
	recvData >> realmName;

	realm = sInfoCore.GetRealmByName(realmName.c_str());

	// not existant -> create
	if( realm == NULL )
	{
		realm = new Realm();
		realm->Id = sInfoCore.GenerateRealmID();
		realm->Name = realmName;
		sInfoCore.AddRealm(realm);
	}

	// update data
	recvData >> realm->Address;
	recvData >> realm->Colour >> realm->Icon >> realm->TimeZone >> realm->Population;

	sLog.outString("Updating realm realm `%s` ID %u.", realm->Name.c_str(), realm->Id);

	// Send back response packet.
	WorldPacket data(RSMSG_REALM_REGISTERED, 4);
	data << uint32(0);	  // Error
	data << realm->Id;		  // Realm ID
	data << realm->Name;
	SendPacket(&data);
	server_ids.insert(realm->Id);

	/* request character mapping for this realm */
	data.Initialize(RSMSG_REQUEST_ACCOUNT_CHARACTER_MAPPING);
	data << realm->Id;
	SendPacket(&data);

	realm->ServerSocket = this;
}

void LogonCommServerSocket::HandleSessionRequest(WorldPacket & recvData)
{
	uint32 request_id;
	string account_name;
	recvData >> request_id;
	recvData >> account_name;

	// get sessionkey!
	uint32 error = 0;
	Account * acct = sAccountMgr.GetAccount(account_name);
	if(acct == NULL || acct->SessionKey == NULL || acct == 0)
		error = 1;		  // Unauthorized user.

	// build response packet
	WorldPacket data(RSMSG_SESSION_RESULT, 150);
	data << request_id;
	data << error;
	if(!error)
	{
		// Append account information.
		data << acct->AccountId;
		data << acct->Username;
		if(!acct->GMFlags)
			data << uint8(0);
		else
			data << acct->GMFlags;

		data << acct->AccountFlags;
		data.append(acct->SessionKey, 40);
		data.append(acct->Locale, 4);
		data << acct->Muted;
	}
	
	SendPacket(&data);
}

void LogonCommServerSocket::HandlePing(WorldPacket & recvData)
{
	WorldPacket data(RSMSG_PONG, 4);
	SendPacket(&data);
	last_ping = (uint32)time(NULL);
}

void LogonCommServerSocket::SendPacket(WorldPacket * data)
{
	bool rv;
	BurstBegin();

	logonpacket header;
	header.opcode = data->GetOpcode();
	//header.size   = ntohl((u_long)data->size());
	header.size = (uint32)data->size();
	swap32(&header.size);

	if(use_crypto)
		sendCrypto.Process((unsigned char*)&header, (unsigned char*)&header, 6);

	rv=BurstSend((uint8*)&header, 6);

	if(data->size() > 0 && rv)
	{
		if(use_crypto)
			sendCrypto.Process((unsigned char*)data->contents(), (unsigned char*)data->contents(), (uint32)data->size());

		rv=BurstSend(data->contents(), (uint32)data->size());
	}

	if(rv) BurstPush();
	BurstEnd();
}

void LogonCommServerSocket::HandleAuthChallenge(WorldPacket & recvData)
{
	unsigned char key[20];
	uint32 result = 1;
	recvData.read(key, 20);

	// check if we have the correct password
	if(memcmp(key, LogonServer::getSingleton().sql_hash, 20))
		result = 0;

	sLog.outString("Authentication request from %s, result %s.", GetRemoteIP().c_str(), result ? "OK" : "FAIL");

	printf("Key: ");
	for(int i = 0; i < 20; ++i)
		printf("%.2X", key[i]);
	printf("\n");

	recvCrypto.Setup(key, 20);
	sendCrypto.Setup(key, 20);	

	/* packets are encrypted from now on */
	use_crypto = true;

	/* send the response packet */
	WorldPacket data(RSMSG_AUTH_RESPONSE, 1);
	data << result;
	SendPacket(&data);

	/* set our general var */
	authenticated = result;
}

void LogonCommServerSocket::HandleMappingReply(WorldPacket & recvData)
{
	/* this packet is gzipped, whee! :D */
	uint32 real_size;
	recvData >> real_size;
	uLongf rsize = real_size;

	ByteBuffer buf(real_size);
	buf.resize(real_size);

	if(uncompress((uint8*)buf.contents(), &rsize, recvData.contents() + 4, (u_long)recvData.size() - 4) != Z_OK)
	{
		printf("Uncompress of mapping failed.\n");
		return;
	}

	uint32 account_id;
	uint8 number_of_characters;
	uint32 count;
	uint32 realm_id;
	buf >> realm_id;
	Realm * realm = sInfoCore.GetRealmById(realm_id);
	if( realm == NULL )
		return;

	realm->m_charMapLock.Acquire();
	HM_NAMESPACE::hash_map<uint32, uint8>::iterator itr;
	buf >> count;
	printf("Got mapping packet for realm %u, total of %u entries.\n", (unsigned int)realm_id, (unsigned int)count);
	for(uint32 i = 0; i < count; ++i)
	{
		buf >> account_id >> number_of_characters;
		itr = realm->CharacterMap.find(account_id);
		if(itr != realm->CharacterMap.end())
			itr->second = number_of_characters;
		else
			realm->CharacterMap.insert( make_pair( account_id, number_of_characters ) );
	}
	realm->m_charMapLock.Release();
}

void LogonCommServerSocket::HandleUpdateMapping(WorldPacket & recvData)
{
	uint32 realm_id;
	uint32 account_id;
	uint8 chars_to_add;
	recvData >> realm_id;

	Realm * realm = sInfoCore.GetRealmById(realm_id);
	if(!realm)
		return;
	
	realm->m_charMapLock.Acquire();
	recvData >> account_id >> chars_to_add;

	HM_NAMESPACE::hash_map<uint32, uint8>::iterator itr = realm->CharacterMap.find(account_id);
	if(itr != realm->CharacterMap.end())
		itr->second += chars_to_add;
	else
		realm->CharacterMap.insert( make_pair( account_id, chars_to_add ) );

	realm->m_charMapLock.Release();
}

void LogonCommServerSocket::HandleTestConsoleLogin(WorldPacket & recvData)
{
	WorldPacket data(RSMSG_CONSOLE_LOGIN_RESULT, 8);
	uint32 request;
	string accountname;
	uint8 key[20];

	recvData >> request;
	recvData >> accountname;
	recvData.read(key, 20);
	printf("testing console login: %s\n", accountname.c_str());

	data << request;

	Account * pAccount = sAccountMgr.GetAccount(accountname);
	if(pAccount == NULL)
	{
		data << uint32(0);
		SendPacket(&data);
		return;
	}

	if(pAccount->GMFlags == NULL || strchr(pAccount->GMFlags, 'z') == NULL)
	{
		data << uint32(0);
		SendPacket(&data);
		return;
	}

	if(memcmp(pAccount->SrpHash, key, 20) != 0)
	{
		data << uint32(0);
		SendPacket(&data);
		return;
	}

	data << uint32(1);
	SendPacket(&data);
}

void LogonCommServerSocket::HandleDatabaseModify(WorldPacket& recvData)
{
	uint32 method;
	recvData >> method;

	if( !IsServerAllowedMod(GetRemoteAddress().s_addr) )
	{
		printf("Database modify request %u denied for %s.\n", method, GetRemoteIP().c_str());
		return;
	}

	switch(method)
	{
	case 1:			// set account ban
		{
			string account;
			uint32 duration;
			string reason;
			recvData >> account >> duration >> reason;

			// remember we expect this in uppercase
			ASCENT_TOUPPER(account);

			Account * pAccount = sAccountMgr.GetAccount(account);
			if( pAccount == NULL )
				return;

			pAccount->Banned = duration;

			// update it in the sql (duh)
			sLogonSQL->Execute("UPDATE accounts SET banned = %u, banReason = \"%s\" WHERE login = \"%s\"", duration, sLogonSQL->EscapeString(reason).c_str(), 
				sLogonSQL->EscapeString(account).c_str());

		}break;

	case 2:		// set gm
		{
			string account;
			string gm;
			recvData >> account >> gm;

			// remember we expect this in uppercase
			ASCENT_TOUPPER(account);

			Account * pAccount = sAccountMgr.GetAccount(account);
			if( pAccount == NULL )
				return;

			pAccount->SetGMFlags( account.c_str() );

			// update it in the sql (duh)
			sLogonSQL->Execute("UPDATE accounts SET gm = \"%s\" WHERE login = \"%s\"", sLogonSQL->EscapeString(gm).c_str(), sLogonSQL->EscapeString(account).c_str());

		}break;

	case 3:		// set mute
		{
			string account;
			uint32 duration;
			recvData >> account >> duration;

			// remember we expect this in uppercase
			ASCENT_TOUPPER(account);

			Account * pAccount = sAccountMgr.GetAccount(account);
			if( pAccount == NULL )
				return;

			pAccount->Muted = duration;

			// update it in the sql (duh)
			sLogonSQL->Execute("UPDATE accounts SET muted = %u WHERE login = \"%s\"", duration, sLogonSQL->EscapeString(account).c_str());
		}break;

	case 4:		// ip ban add
		{
			string ip;
			uint32 duration;

			recvData >> ip >> duration;

			if( sIPBanner.Add( ip.c_str(), duration ) )
				sLogonSQL->Execute("INSERT INTO ipbans VALUES(\"%s\", %u)", sLogonSQL->EscapeString(ip).c_str(), duration);

		}break;

	case 5:		// ip ban reomve
		{
			string ip;
			recvData >> ip;

			if( sIPBanner.Remove( ip.c_str() ) )
				sLogonSQL->Execute("DELETE FROM ipbans WHERE ip = \"%s\")", sLogonSQL->EscapeString(ip).c_str());

		}break;

	}
}

void LogonCommServerSocket::SendPing()
{
	next_server_ping = (uint32)UNIXTIME + 20;
	WorldPacket data(RSMSG_SERVER_PING, 4);
	data << uint32(0);
	SendPacket(&data);
}

void LogonCommServerSocket::HandleServerPong(WorldPacket &recvData)
{
	// nothing
}
