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

#include "../ascent-shared/Singleton.h"

extern bool mrunning;
class AuthSocket;
extern set<AuthSocket*> _authSockets;
extern Mutex _authSocketLock;
extern bool m_encryptedPasswords;

struct AllowedIP
{
	unsigned int IP;
	unsigned char Bytes;
};

bool IsServerAllowed(unsigned int IP);
bool IsServerAllowedMod(unsigned int IP);

class LogonServer;
class LogonServer : public Singleton< LogonServer >
{
public:
	void CheckForDeadSockets();
	void Run(int argc, char ** argv);
	void Stop();
	uint32 max_build;
	uint32 min_build;
	uint8 sql_hash[20];

private:
	bool m_stopEvent;
};
