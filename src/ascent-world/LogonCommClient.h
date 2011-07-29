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

#ifndef __LOGON_COMM_CLIENT_H
#define __LOGON_COMM_CLIENT_H

#include "../ascent-logonserver/LogonOpcodes.h"
#include <RC4Engine.h>

class LogonCommClientSocket : public Socket
{
	uint32 remaining;
	uint16 opcode;
	RC4Engine _sendCrypto;
	RC4Engine _recvCrypto;
public:
	LogonCommClientSocket(SOCKET fd);
	~LogonCommClientSocket();
	
	void OnRead();
	void SendPacket(WorldPacket * data, bool no_crypto);
	void HandlePacket(WorldPacket & recvData);
	void SendPing();
	void SendChallenge();
	void HandleAuthResponse(WorldPacket & recvData);

	void HandleRegister(WorldPacket & recvData);
	void HandlePong(WorldPacket & recvData);
	void HandleServerPing(WorldPacket &recvData);
	void HandleSessionInfo(WorldPacket & recvData);
	void HandleRequestAccountMapping(WorldPacket & recvData);
	void UpdateAccountCount(uint32 account_id, uint8 add);
	void HandleDisconnectAccount(WorldPacket & recvData);
	void HandleConsoleAuthResult(WorldPacket & recvData);

	void OnDisconnect();
	void CompressAndSend(ByteBuffer & uncompressed);
	uint32 last_ping;
	uint32 last_pong;

	uint32 pingtime;
	uint32 latency;
	uint32 _id;
	uint32 authenticated;
	bool use_crypto;
	set<uint32> realm_ids;
};

typedef void (LogonCommClientSocket::*logonpacket_handler)(WorldPacket&);

#endif

