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

#pragma pack(push, 1)
struct ClientPktHeader
{
	uint16 size;
	uint32 cmd;
};

struct ServerPktHeader
{
	uint16 size;
	uint16 cmd;
};
#pragma pack(pop)

WorldSocket::WorldSocket(SOCKET fd) : Socket(fd, WORLDSOCKET_SENDBUF_SIZE, WORLDSOCKET_RECVBUF_SIZE)
{
	mSize = mOpcode = mRemaining = 0;
	_latency = 0;
	mSeed = rand() % 0xFFFFFFF0 + 10;
	pAuthenticationPacket = NULL;
	mRequestID = 0;
	Authed = false;
}

WorldSocket::~WorldSocket()
{

}

void WorldSocket::OnDisconnect()
{
	if(mRequestID != 0)
	{
		sLogonCommHandler.UnauthedSocketClose(mRequestID);
		mRequestID = 0;
	}
/*

	if( m_session )
	{
		sClientMgr.RemoveSession(m_session);
	}*/

}

void WorldSocket::OutPacket(uint16 opcode, size_t len, const void* data)
{
	bool rv;
	if(opcode == 0 || !IsConnected())
		return;

	BurstBegin();

	// Encrypt the packet
	// First, create the header.
	ServerPktHeader Header;
	Header.cmd = opcode;
	Header.size = uint16(ntohs((u_short)len + 2));
    _crypt.EncryptSend((uint8*)&Header, sizeof (ServerPktHeader));

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&Header, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(len > 0 && rv)
	{
		rv = BurstSend((const uint8*)data, (uint32)len);
	}

	if(rv) BurstPush();
	BurstEnd();
}

void WorldSocket::OnConnect()
{
	OutPacket(SMSG_AUTH_CHALLENGE, 4, &mSeed);
	_latency = getMSTime();
}

void WorldSocket::_HandleAuthSession(WorldPacket* recvPacket)
{
	std::string account;
	uint32 unk2, unk3;
	_latency = getMSTime() - _latency;

	try
	{
		*recvPacket >> mClientBuild;
		*recvPacket >> unk2;
		*recvPacket >> account;
		*recvPacket >> unk3;
		*recvPacket >> mClientSeed;
	}
	catch(ByteBuffer::error &)
	{
		printf("Incomplete copy of AUTH_SESSION Received.");
		return;
	}
	// Send out a request for this account.
	mRequestID = sLogonCommHandler.ClientConnected(account, this);

	if(mRequestID == 0xFFFFFFFF)
	{
		Disconnect();
		return;
	}

	// Set the authentication packet 
	pAuthenticationPacket = recvPacket;
	m_fullAccountName = new string(account);
}

void WorldSocket::InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid)
{
	if(requestid != mRequestID)
		return;

	uint32 error;
	recvData >> error;

	if(error != 0)
	{
		// something happened wrong @ the logon server
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		printf("Information callback returns failure.\n");
		return;
	}

	// Extract account information from the packet.
	string AccountName;
	uint32 AccountID;
	string GMFlags;
	uint8 AccountFlags;

	recvData >> AccountID >> AccountName >> GMFlags >> AccountFlags;
	printf( " >> got information packet from logon: `%s` ID %u (request %u)", AccountName.c_str(), AccountID, mRequestID);
	//	sLog.outColor(TNORMAL, "\n");

	mRequestID = 0;
	//Pull the session key.

	recvData.read(K, 40);

	_crypt.Init(K);

	BigNumber BNK;
	BNK.SetBinary(K, 40);

	//checking if player is already connected
	//disconnect current player and login this one(blizzlike)

	string lang = "enUS";
	if(recvData.rpos() != recvData.wpos())
		recvData.read((uint8*)lang.data(), 4);

	Session * session = sClientMgr.CreateSession(AccountID);
	if(session == NULL)
	{
		/* we are already logged in. send auth failed. (if anyone has a better error lemme know :P) */
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		printf("Duplicate client error.\n");
		return;
	}

	m_session = session;
	session->m_socket = this;
    Sha1Hash sha;

	uint8 digest[20];
	pAuthenticationPacket->read(digest, 20);

	uint32 t = 0;
	if( m_fullAccountName == NULL )				// should never happen !
		sha.UpdateData(AccountName);
	else
	{
		sha.UpdateData(*m_fullAccountName);

		// this is unused now. we may as well free up the memory.
		delete m_fullAccountName;
		m_fullAccountName = NULL;
	}

	sha.UpdateData((uint8 *)&t, 4);
	sha.UpdateData((uint8 *)&mClientSeed, 4);
	sha.UpdateData((uint8 *)&mSeed, 4);
	sha.UpdateBigNumbers(&BNK, NULL);
	sha.Finalize();

	if (memcmp(sha.GetDigest(), digest, 20))
	{
		// AUTH_UNKNOWN_ACCOUNT = 21
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
		return;
	}

	//_crypt.Init(digest);

	// Allocate session
	m_session->m_accountFlags = AccountFlags;
	m_session->m_GMPermissions = GMFlags;
	m_session->m_accountId = AccountID;
	m_session->m_latency = _latency;
	m_session->m_accountName = AccountName;
	m_session->m_ClientBuild = mClientBuild;
	m_session->language = sLocalizationMgr.GetLanguageId(lang);

	Log.Notice("Auth", "%s from %s:%u [%ums]", AccountName.c_str(), GetRemoteIP().c_str(), GetRemotePort(), _latency);
	Authenticate();
}

void WorldSocket::Authenticate()
{
	if(m_session->m_accountFlags & 16)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x02");
	else if(m_session->m_accountFlags & 8)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x01");
	else
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x00");

	SendAddonInfoPacket(pAuthenticationPacket, (uint32)pAuthenticationPacket->rpos(), m_session);

	delete pAuthenticationPacket;
	pAuthenticationPacket = 0;
}

void WorldSocket::_HandlePing(WorldPacket* recvPacket)
{
	uint32 ping;
	if(recvPacket->size() < 4)
	{
		sLog.outString("Socket closed due to incomplete ping packet.");
		Disconnect();
		return;
	}

	*recvPacket >> ping;
	*recvPacket >> _latency;
	OutPacket(SMSG_PONG, 4, &ping);
}

void WorldSocket::OnRead()
{
	DEBUG_LOG("WorldSocket","Received Packet");
	for(;;)
	{
		// Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(readBuffer.GetSize() < 6)
			{
				// No header in the packet, let's wait.
				return;
			}

			// Copy from packet buffer into header local var
			ClientPktHeader Header;
			readBuffer.Read(&Header, 6);

			// Decrypt the header
			_crypt.DecryptRecv((uint8*)&Header, sizeof (ClientPktHeader));

			mRemaining = mSize = ntohs(Header.size) - 4;
			mOpcode = Header.cmd;
		}

		WorldPacket * Packet;

		if(mRemaining > 0)
		{
			if( readBuffer.GetSize() < mRemaining )
			{
				// We have a fragmented packet. Wait for the complete one before proceeding.
				return;
			}
		}

		Packet = new WorldPacket(mOpcode, mSize);
		Packet->resize(mSize);

		if(mRemaining > 0)
		{
			// Copy from packet buffer into our actual buffer.
			readBuffer.Read((uint8*)Packet->contents(), mRemaining);
		}

		/*sWorldLog.LogPacket(mSize, mOpcode, mSize ? Packet->contents() : NULL, 0);*/
		mRemaining = mSize = mOpcode = 0;

		// Check for packets that we handle
		DEBUG_LOG("WorldSocket","Received Opcode: %u", Packet->GetOpcode());
		switch(Packet->GetOpcode())
		{
		case CMSG_PING:
			{
				if(!m_session->m_currentPlayer)
				{
					_HandlePing(Packet);
					delete Packet;
				}
				else
					m_session->m_readQueue.Push(Packet);				
			}break;
		case CMSG_AUTH_SESSION:
			{
				_HandleAuthSession(Packet);
			}break;
		default:
			{
				if(m_session) m_session->m_readQueue.Push(Packet);
				else delete Packet;
			}break;
		}
	}
}

// hacky key
static uint8 PublicKey[265] = { 0x02, 0x01, 0x01, 0xC3, 0x5B, 0x50, 0x84, 0xB9, 0x3E, 0x32, 0x42, 0x8C, 0xD0, 0xC7, 0x48, 0xFA, 0x0E, 0x5D, 0x54, 0x5A, 0xA3, 0x0E, 0x14, 0xBA, 0x9E, 0x0D, 0xB9, 0x5D, 0x8B, 0xEE, 0xB6, 0x84, 0x93, 0x45, 0x75, 0xFF, 0x31, 0xFE, 0x2F, 0x64, 0x3F, 0x3D, 0x6D, 0x07, 0xD9, 0x44, 0x9B, 0x40, 0x85, 0x59, 0x34, 0x4E, 0x10, 0xE1, 0xE7, 0x43, 0x69, 0xEF, 0x7C, 0x16, 0xFC, 0xB4, 0xED, 0x1B, 0x95, 0x28, 0xA8, 0x23, 0x76, 0x51, 0x31, 0x57, 0x30, 0x2B, 0x79, 0x08, 0x50, 0x10, 0x1C, 0x4A, 0x1A, 0x2C, 0xC8, 0x8B, 0x8F, 0x05, 0x2D, 0x22, 0x3D, 0xDB, 0x5A, 0x24, 0x7A, 0x0F, 0x13, 0x50, 0x37, 0x8F, 0x5A, 0xCC, 0x9E, 0x04, 0x44, 0x0E, 0x87, 0x01, 0xD4, 0xA3, 0x15, 0x94, 0x16, 0x34, 0xC6, 0xC2, 0xC3, 0xFB, 0x49, 0xFE, 0xE1, 0xF9, 0xDA, 0x8C, 0x50, 0x3C, 0xBE, 0x2C, 0xBB, 0x57, 0xED, 0x46, 0xB9, 0xAD, 0x8B, 0xC6, 0xDF, 0x0E, 0xD6, 0x0F, 0xBE, 0x80, 0xB3, 0x8B, 0x1E, 0x77, 0xCF, 0xAD, 0x22, 0xCF, 0xB7, 0x4B, 0xCF, 0xFB, 0xF0, 0x6B, 0x11, 0x45, 0x2D, 0x7A, 0x81, 0x18, 0xF2, 0x92, 0x7E, 0x98, 0x56, 0x5D, 0x5E, 0x69, 0x72, 0x0A, 0x0D, 0x03, 0x0A, 0x85, 0xA2, 0x85, 0x9C, 0xCB, 0xFB, 0x56, 0x6E, 0x8F, 0x44, 0xBB, 0x8F, 0x02, 0x22, 0x68, 0x63, 0x97, 0xBC, 0x85, 0xBA, 0xA8, 0xF7, 0xB5, 0x40, 0x68, 0x3C, 0x77, 0x86, 0x6F, 0x4B, 0xD7, 0x88, 0xCA, 0x8A, 0xD7, 0xCE, 0x36, 0xF0, 0x45, 0x6E, 0xD5, 0x64, 0x79, 0x0F, 0x17, 0xFC, 0x64, 0xDD, 0x10, 0x6F, 0xF3, 0xF5, 0xE0, 0xA6, 0xC3, 0xFB, 0x1B, 0x8C, 0x29, 0xEF, 0x8E, 0xE5, 0x34, 0xCB, 0xD1, 0x2A, 0xCE, 0x79, 0xC3, 0x9A, 0x0D, 0x36, 0xEA, 0x01, 0xE0, 0xAA, 0x91, 0x20, 0x54, 0xF0, 0x72, 0xD8, 0x1E, 0xC7, 0x89, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x00};

void WorldSocket::SendAddonInfoPacket(WorldPacket *source, uint32 pos, Session *m_session)
{
	WorldPacket returnpacket;
	returnpacket.Initialize(SMSG_ADDON_INFO);	// SMSG_ADDON_INFO

	uint32 realsize;
	uLongf rsize;
	try
	{
		*source >> realsize;
	}
	catch (ByteBuffer::error &)
	{
		DEBUG_LOG("AddonMgr","Warning, Incomplete auth session sent.");
		return;
	}	
	rsize = realsize;
	size_t position = source->rpos();

	ByteBuffer unpacked;
	unpacked.resize(realsize);

	if((source->size() - position) < 4 || realsize == 0)
	{
		// we shouldn't get here.. but just in case this will stop any crash here.
		DEBUG_LOG("AddonMgr","Warning, Incomplete auth session sent.");
		return;
	}
	int32 result;
	result = uncompress((uint8*)unpacked.contents(), &rsize, (uint8*)(*source).contents() + position, (uLong)((*source).size() - position));

	if(result != Z_OK)
	{
		DEBUG_LOG("AddonMgr","Decompression of addon section of CMSG_AUTH_SESSION failed.");
		return;
	}

	DEBUG_LOG("AddonMgr","Decompression of addon section of CMSG_AUTH_SESSION succeeded.");

	uint32 addons; // Added in 3.0.8
	uint8 Enable; // based on the parsed files from retool
	uint32 crc;
	uint32 unknown;

	unpacked >> addons;
	size_t p = unpacked.rpos(); 
	if( p + (addons*10) > unpacked.size() - p - 4) 
	{ 
		// stupid fucker, trying to make us waste CPU cycles 
		Disconnect(); 
		return; 
	}

	std::string name;
	for (uint32 i = 0; i < addons; ++i)
	{
		unpacked >> name;
		unpacked >> Enable;
		unpacked >> crc;
		unpacked >> unknown;

		// Hacky fix, Yea I know its a hacky fix I will make a proper handler one's I got the crc crap
		if (crc != 0x4C1C776D) // CRC of public key version 2.0.1
			returnpacket.append(PublicKey,264); // part of the hacky fix
		else
			returnpacket << uint8(0x02) << uint8(0x01) << uint8(0x00) << uint32(0) << uint8(0);

	}

	uint32 unk308;

	unpacked >> unk308;
	m_session->SendPacket(&returnpacket);
}
