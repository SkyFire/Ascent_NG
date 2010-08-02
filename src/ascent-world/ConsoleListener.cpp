/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"

#include <Common.h>
#include <Network/Network.h>
#include <Config/ConfigEnv.h>
#include <svn_revision.h>

#include "BaseConsole.h"
#include "ConsoleCommands.h"
#define LOCAL_BUFFER_SIZE 2048
#define ENABLE_REMOTE_CONSOLE 1

enum STATES
{
	STATE_USER		= 1,
	STATE_PASSWORD	= 2,
	STATE_LOGGED	= 3,
	STATE_WAITING	= 4,
};

class ConsoleSocket : public Socket
{
	RemoteConsole * m_pConsole;
	char * m_pBuffer;
	uint32 m_pBufferLen;
	uint32 m_pBufferPos;
	uint32 m_state;
	string m_username;
	string m_password;
	uint32 m_requestNo;

public:
	ConsoleSocket(SOCKET iFd);
	~ConsoleSocket();

	void OnRead();
	void OnDisconnect();
	void OnConnect();
	void TryAuthenticate();
	void AuthCallback(bool result);
};

class ConsoleAuthMgr : public Singleton<ConsoleAuthMgr>
{
	Mutex authmgrlock;
	uint32 highrequestid;
	map<uint32, ConsoleSocket*> requestmap;
public:

	ConsoleAuthMgr()
	{
		highrequestid = 1;
	}

	uint32 GenerateRequestId()
	{
		uint32 n;
		authmgrlock.Acquire();
		n = highrequestid++;
		authmgrlock.Release();
        return n;
	}

	void SetRequest(uint32 id, ConsoleSocket * sock)
	{
		authmgrlock.Acquire();
		if( sock == NULL )
			requestmap.erase(id);
		else
			requestmap.insert(make_pair(id, sock));
		authmgrlock.Release();
	}

	ConsoleSocket * GetRequest(uint32 id)
	{
		ConsoleSocket* rtn;
		authmgrlock.Acquire();
		map<uint32,ConsoleSocket*>::iterator itr = requestmap.find(id);
		if(itr == requestmap.end())
			rtn = NULL;
		else
			rtn = itr->second;
		authmgrlock.Release();
		return rtn;
	}
};

ListenSocket<ConsoleSocket> * g_pListenSocket = NULL;
initialiseSingleton(ConsoleAuthMgr);

void ConsoleAuthCallback(uint32 request, uint32 result)
{
	ConsoleSocket * pSocket = ConsoleAuthMgr::getSingleton().GetRequest(request);
	if(pSocket == NULL || !pSocket->IsConnected())
		return;

    if(result)
		pSocket->AuthCallback(true);
	else
		pSocket->AuthCallback(false);
}

void CloseConsoleListener()
{
	if(g_pListenSocket != NULL)
		g_pListenSocket->Close();
}

bool StartConsoleListener( )
{
#ifndef ENABLE_REMOTE_CONSOLE
	return false;
#else
	string lhost = Config.MainConfig.GetStringDefault("RemoteConsole", "Host", "0.0.0.0");
	uint32 lport = Config.MainConfig.GetIntDefault("RemoteConsole", "Port", 8092);
	bool enabled = Config.MainConfig.GetBoolDefault("RemoteConsole", "Enabled", false);

	if( !enabled )
		return false;

	g_pListenSocket = new ListenSocket<ConsoleSocket>( lhost.c_str(), lport );
	if( g_pListenSocket == NULL )
		return false;

	if( !g_pListenSocket->IsOpen( ) )
	{
		g_pListenSocket->Close( );
		delete g_pListenSocket;
		g_pListenSocket = NULL;
		return false;
	}

	new ConsoleAuthMgr;
	return true;
#endif
}

ThreadContext * GetConsoleListener()
{
	return (ThreadContext*)g_pListenSocket;
}

ConsoleSocket::ConsoleSocket( SOCKET iFd ) : Socket(iFd, 10000, 1000)
{
	m_pBufferLen = LOCAL_BUFFER_SIZE;
	m_pBufferPos = 0;
	m_pBuffer = new char[LOCAL_BUFFER_SIZE];
	m_pConsole = new RemoteConsole(this);
	m_state = STATE_USER;
}

ConsoleSocket::~ConsoleSocket( )
{
	if( m_pBuffer != NULL )
		delete [] m_pBuffer;

	if( m_pConsole != NULL )
		delete m_pConsole;

	if(m_requestNo)
	{
		ConsoleAuthMgr::getSingleton().SetRequest(m_requestNo, NULL);
		m_requestNo=0;
	}
}

void TestConsoleLogin(string& username, string& password, uint32 requestid);

void ConsoleSocket::OnRead()
{
	uint32 readlen = (uint32)readBuffer.GetSize();
	uint32 len;
	char * p;
	if( ( readlen + m_pBufferPos ) >= m_pBufferLen )
	{
		Disconnect();
		return;
	}

	readBuffer.Read((uint8*)&m_pBuffer[m_pBufferPos], readlen);
	m_pBufferPos += readlen;

	// let's look for any newline bytes.
	p = strchr(m_pBuffer, '\n');
	while( p != NULL )
	{
		// windows is stupid. :P
		len = (uint32)((p+1) - m_pBuffer);
		if( *(p-1) == '\r' )
			*(p-1) = '\0';

		*p = '\0';

		// handle the command
		if( *m_pBuffer != '\0' )
		{
			switch(m_state)
			{
			case STATE_USER:
				m_username = string(m_pBuffer);
				m_pConsole->Write("password: ");
				m_state = STATE_PASSWORD;
				break;

			case STATE_PASSWORD:
				m_password = string(m_pBuffer);
				m_pConsole->Write("\r\nAttempting to authenticate. Please wait.\r\n");
				m_state = STATE_WAITING;

				m_requestNo = ConsoleAuthMgr::getSingleton().GenerateRequestId();
				ConsoleAuthMgr::getSingleton().SetRequest(m_requestNo, this);

                TestConsoleLogin(m_username, m_password, m_requestNo);
				break;

			case STATE_LOGGED:
				if( !strnicmp( m_pBuffer, "quit", 4 ) )
				{
					Disconnect();
					break;
				}

                HandleConsoleInput(m_pConsole, m_pBuffer);
				break;
			}
		}

		// move the bytes back
		if( len == m_pBufferPos )
		{
			m_pBuffer[0] = '\0';
			m_pBufferPos = 0;
		}
		else
		{
			memcpy(m_pBuffer, &m_pBuffer[len], m_pBufferPos - len);
			m_pBufferPos -= len;
		}

		p = strchr(m_pBuffer, '\n');
	}
}

void ConsoleSocket::OnConnect()
{
	m_pConsole->Write("Welcome to Ascent's Remote Administration Console.\r\n");
	m_pConsole->Write("Please authenticate to continue.\r\n\r\n");
	m_pConsole->Write("login: ");
}

void ConsoleSocket::OnDisconnect()
{
	if(m_requestNo)
	{
		ConsoleAuthMgr::getSingleton().SetRequest(m_requestNo, NULL);
		m_requestNo=0;
	}
}

void ConsoleSocket::AuthCallback(bool result)
{
	ConsoleAuthMgr::getSingleton().SetRequest(m_requestNo, NULL);
	m_requestNo=0;

	if( !result )
	{
		m_pConsole->Write("Authentication failed.\r\n\r\n");
		Disconnect();
	}
	else
	{
		m_pConsole->Write("Authentication passed.\r\n");
		m_pConsole->Write("You are now logged in under user `%s`.\r\n\r\n", m_username.c_str());
		m_state = STATE_LOGGED;
	}
}

RemoteConsole::RemoteConsole(ConsoleSocket* pSocket)
{
	m_pSocket = pSocket;
}

void RemoteConsole::Write(const char * Format, ...)
{
	char obuf[65536];
	va_list ap;

    va_start(ap, Format);
	vsnprintf(obuf, 65536, Format, ap);
	va_end(ap);

	if( *obuf == '\0' )
		return;

	m_pSocket->Send((const uint8*)obuf, (uint32)strlen(obuf));
}

void RemoteConsole::WriteNA(const char * Format)
{
	if( *Format == '\0' )
		return;

	m_pSocket->Send((const uint8*)Format, (uint32)strlen(Format));
}


struct ConsoleCommand
{
	bool(*CommandPointer)(BaseConsole*, int, const char*[]);
	const char * Name;					// 10 chars
	const char * ArgumentFormat;		// 30 chars
	const char * Description;			// 40 chars
										// = 70 chars
};

void HandleConsoleInput(BaseConsole * pConsole, const char * szInput)
{
	static ConsoleCommand Commands[] = {
		{ &HandleInfoCommand, "info", "none", "Gives server runtime information." },
		{ &HandleGMsCommand, "gms", "none", "Shows online GMs." },
		{ &HandleAnnounceCommand, "announce", "<announce string>", "Shows the message in all client chat boxes." },
		{ &HandleWAnnounceCommand, "wannounce", "<wannounce string>", "Shows the message in all client title areas." },
		{ &HandleKickCommand, "kick", "<plrname> <reason>", "Kicks player x for reason y." },
		{ &HandleQuitCommand, "shutdown", "[delay]", "Shuts down server with optional delay in seconds." },
		{ &HandleCancelCommand, "cancel", "none", "Cancels a pending shutdown." },
		{ &HandleUptimeCommand, "uptime", "none", "Shows server uptime." },
		{ &HandleBanAccountCommand, "banaccount", "<account> <timeperiod>", "Bans account x for time y." },
		{ &HandleUnbanAccountCommand, "unbanaccount", "<account>", "Unbans account x." },
		{ &HandleMOTDCommand, "motd", "<new motd>", "Sets MOTD" },
		{ &HandlePlayerInfoCommand, "playerinfo", "<plrname>", "Shows information about a player." },
		{ &HandleCreateAccountCommand, "createaccount", "<name> <pass> <email> <flags>", "Creates an account." },
		{ &HandleRehashCommand, "rehash", "none", "Reloads the config file" },
		{ &HandleBPStatsCommand, "bpstats", "none", "Shows buffer pool stats" },
		{ NULL, NULL, NULL, NULL },
	};

	uint32 i;
	char * p, *q;

	// let's tokenize into arguments.
	vector<const char*> tokens;

	q = (char*)szInput;
	p = strchr(q, ' ');
	while( p != NULL )
	{
		*p = 0;
		tokens.push_back(q);

		q = p+1;
		p = strchr(q, ' ');
	}

	if( q != NULL && *q != '\0' )
		tokens.push_back( q	);

	if( tokens.empty() )
		return;

	if( !stricmp(tokens[0], "help") || tokens[0][0] == '?' )
	{
		pConsole->Write("=========================================================================================\r\n");
		pConsole->Write("| %10s | %20s | %49s |\r\n", "Name", "Arguments", "Description");
		for(i = 0; Commands[i].Name != NULL; ++i)
		{
			pConsole->Write("| %10s | %20s | %49s |\r\n", Commands[i].Name, Commands[i].ArgumentFormat, Commands[i].Description);
		}
		pConsole->Write("=========================================================================================\r\n");		
	}
	else
	{
		for(i = 0; Commands[i].Name != NULL; ++i)
		{
			if( !stricmp( Commands[i].Name, tokens[0] ) )
			{
				if( !Commands[i].CommandPointer( pConsole, (int)tokens.size(), &tokens[0] ) )
				{
					pConsole->Write("Command '%s' failed. Try with syntax: '%s'.\r\n\r\n", Commands[i].Name, Commands[i].ArgumentFormat );
					return;
				}
				else
					return;
			}
		}

		pConsole->Write("No command named '%s' could be found. Please check your syntax and try again.\r\n\r\n", tokens[0]);
	}
}

void LocalConsole::Write(const char * Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	vprintf(Format, ap);
}

void LocalConsole::WriteNA(const char * Format)
{
	printf(Format);
}
