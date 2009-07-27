/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
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
	m_session = NULL;
	mClientBuild = 0;
	mClientSeed = 0;
	m_fullAccountName = NULL;
}

WorldSocket::~WorldSocket()
{
	if (m_fullAccountName != NULL)
		delete m_fullAccountName;
}

void WorldSocket::OnDisconnect()
{
	if(mRequestID != 0)
	{
		sLogonCommHandler.UnauthedSocketClose(mRequestID);
		mRequestID = 0;
	}
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
    _crypt.EncryptFourSend((uint8*)&Header);

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
		sLog.outDetail("Incomplete copy of AUTH_SESSION Received.");
		return;
	}

	// Send out a request for this account.
	mRequestID = sLogonCommHandler.ClientConnected(account, this);

	if(mRequestID == 0xFFFFFFFF)
	{
		Disconnect();
		return;
	}

	m_fullAccountName = new std::string(account);

	// Set the authentication packet 
	pAuthenticationPacket = recvPacket;
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
		return;
	}

	// Extract account information from the packet.
	string AccountName;
	uint32 AccountID;
	string GMFlags;
	uint8 AccountFlags;
	std::string lang = "enUS";

	recvData >> AccountID >> AccountName >> GMFlags >> AccountFlags;
	sLog.outDebug( " >> got information packet from logon: `%s` ID %u (request %u)", AccountName.c_str(), AccountID, mRequestID);
	//	sLog.outColor(TNORMAL, "\n");

	mRequestID = 0;
	// Pull the session key.
	uint8 K[40];
	recvData.read(K, 40);

	BigNumber BNK;
	BNK.SetBinary(K, 40);

	uint8 abuf[64], bbuf[64];

	memset(abuf, 0x36, 64);
	memset(bbuf, 0x5C, 64);

	static const uint8 Key[16] = { 0x38, 0xA7, 0x83, 0x15, 0xF8, 0x92, 0x25, 0x30, 0x71, 0x98, 0x67, 0xB1, 0x8C, 0x04, 0xE2, 0xAA };

	for (int i=0; i<16; i++)
	{
		abuf[i]^=Key[i];
		bbuf[i]^=Key[i];
	}

	uint8 buffer[104];

	Sha1Hash hasher;

	hasher.Initialize();

	memcpy(buffer, abuf, 64);
	memcpy(&buffer[64], K, 40);

	hasher.UpdateData(buffer, 104);
	hasher.Finalize();

	memcpy(buffer, bbuf, 64);
	memcpy(&buffer[64], hasher.GetDigest(), 20);

	hasher.Initialize();
	hasher.UpdateData(buffer, 84);
	hasher.Finalize();
	
	// Initialize crypto.
	_crypt.SetKey(hasher.GetDigest(), 20);
	_crypt.Init();

	if(recvData.rpos() != recvData.wpos())
		recvData.read((uint8*)lang.data(), 4);

	Session * session = sClientMgr.CreateSession(AccountID);
	if(session == NULL)
	{
		/* we are already logged in. send auth failed. (if anyone has a better error lemme know :P) */
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		return;
	}

	m_session = session;
	session->m_socket = this;
    Sha1Hash sha;

	uint8 digest[20];
	pAuthenticationPacket->read(digest, 20);

	uint32 t = 0;
	if (m_fullAccountName == NULL)
		sha.UpdateData(AccountName);
	else
	{
		sha.UpdateData(*m_fullAccountName);

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

	// Allocate session
	m_session->m_accountFlags = AccountFlags;
	m_session->m_GMPermissions = GMFlags;
	m_session->m_accountId = AccountID;
	m_session->m_latency = _latency;
	m_session->m_accountName = AccountName;

	Log.Notice("Auth", "%s from %s:%u [%ums]", AccountName.c_str(), GetRemoteIP().c_str(), GetRemotePort(), _latency);
	Authenticate();
}

void WorldSocket::Authenticate()
{
	ASSERT(pAuthenticationPacket);
	delete pAuthenticationPacket;
	pAuthenticationPacket = NULL;

	if(m_session->m_accountFlags & 16)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x02");
	else if(m_session->m_accountFlags & 8)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x01");
	else
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x00");
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
	for(;;)
	{
		// Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(GetReadBuffer().GetSize() < 6)
			{
				// No header in the packet, let's wait.
				return;
			}

			// Copy from packet buffer into header local var
			ClientPktHeader Header;
			GetReadBuffer().Read(&Header, 6);

			// Decrypt the header
            _crypt.DecryptSixRecv((uint8*)&Header);

			mRemaining = mSize = ntohs(Header.size) - 4;
			mOpcode = Header.cmd;
		}

		WorldPacket * Packet;

		if(mRemaining > 0)
		{
			if( GetReadBuffer().GetSize() < mRemaining )
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
			//Read(mRemaining, (uint8*)Packet->contents());
			GetReadBuffer().Read((uint8*)Packet->contents(), mRemaining);
		}

		/*sWorldLog.LogPacket(mSize, mOpcode, mSize ? Packet->contents() : NULL, 0);*/
		mRemaining = mSize = mOpcode = 0;

		// Check for packets that we handle
		switch(Packet->GetOpcode())
		{
		case CMSG_PING:
			{
				if(m_session == NULL || m_session->m_currentPlayer == NULL)
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
