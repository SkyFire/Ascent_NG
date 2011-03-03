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

#ifndef AUTHSOCKET_H
#define AUTHSOCKET_H

#include "AccountCache.h"
#include "AuthStructs.h"

class LogonCommServerSocket;
struct Patch;
class PatchJob;

class AuthSocket : public Socket
{
	friend class LogonCommServerSocket;
public:

	///////////////////////////////////////////////////
	// Netcore shit
	//////////////////////////
	AuthSocket(SOCKET fd);
	~AuthSocket();

	void OnRead();

	///////////////////////////////////////////////////
	// Client Packet Handlers
	//////////////////////////

	void HandleChallenge();
	void HandleProof();
	void HandleRealmlist();
	void HandleReconnectChallenge();
	void HandleReconnectProof();
	void HandleTransferAccept();
	void HandleTransferResume();
	void HandleTransferCancel();

	///////////////////////////////////////////////////
	// Server Packet Builders
	//////////////////////////

	void SendChallengeError(uint8 Error);
	void SendProofError(uint8 Error, uint8 * M2);
	ASCENT_INLINE sAuthLogonChallenge_C * GetChallenge() { return &m_challenge; }
	ASCENT_INLINE void SendPacket(const uint8* data, const uint16 len) { Send(data, len); }
	void OnDisconnect();
	ASCENT_INLINE time_t GetLastRecv() { return last_recv; }
	bool removedFromSet;
	ASCENT_INLINE uint32 GetAccountID() { return m_account ? m_account->AccountId : 0; }

protected:

	sAuthLogonChallenge_C m_challenge;
	Account * m_account;
	bool m_authenticated;
	std::string AccountName;

	//////////////////////////////////////////////////
	// Authentication BigNumbers
	/////////////////////////
	BigNumber N, s, g, v;
	BigNumber b, B;
	BigNumber K;
	BigNumber rs;

	//////////////////////////////////////////////////
	// Session Key
	/////////////////////////

	BigNumber m_sessionkey;
	time_t last_recv;

	//////////////////////////////////////////////////////////////////////////
	// Patching stuff
	//////////////////////////////////////////////////////////////////////////
public:
	Patch * m_patch;
	PatchJob * m_patchJob;
};

#endif
