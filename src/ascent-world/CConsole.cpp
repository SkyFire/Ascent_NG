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

/*
LocalConsole g_localConsole;

#ifndef WIN32
    #include <termios.h>
#endif

void ConsoleThread::terminate()
{
	m_threadRunning = false;
#ifdef WIN32
	/* write the return keydown/keyup event * /
	DWORD dwTmp;
	INPUT_RECORD ir[2];
	ir[0].EventType = KEY_EVENT;
	ir[0].Event.KeyEvent.bKeyDown = TRUE;
	ir[0].Event.KeyEvent.dwControlKeyState = 288;
	ir[0].Event.KeyEvent.uChar.AsciiChar = 13;
	ir[0].Event.KeyEvent.wRepeatCount = 1;
	ir[0].Event.KeyEvent.wVirtualKeyCode = 13;
	ir[0].Event.KeyEvent.wVirtualScanCode = 28;
	ir[1].EventType = KEY_EVENT;
	ir[1].Event.KeyEvent.bKeyDown = FALSE;
	ir[1].Event.KeyEvent.dwControlKeyState = 288;
	ir[1].Event.KeyEvent.uChar.AsciiChar = 13;
	ir[1].Event.KeyEvent.wRepeatCount = 1;
	ir[1].Event.KeyEvent.wVirtualKeyCode = 13;
	ir[1].Event.KeyEvent.wVirtualScanCode = 28;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, 2, & dwTmp );
#endif
	printf( "Waiting for console thread to terminate....\n" );
	while( m_isRunning )
	{
		Sleep( 100 );
	}
	printf( "Console shut down.\n" );
}

bool ConsoleThread::run()
{
	SetThreadName("Console Interpreter");
	size_t i = 0;
	size_t len;
	char cmd[300];
#ifndef WIN32
	fd_set fds;
	struct timeval tv;
#endif

	m_isRunning = true;
	while( m_threadRunning )
	{
#ifdef WIN32

		// Read in single line from "stdin"
		memset( cmd, 0, sizeof( cmd ) ); 
		if( fgets( cmd, 300, stdin ) == NULL )
			continue;

		if( !m_threadRunning )
			break;

#else
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO( &fds );
		FD_SET( STDIN_FILENO, &fds );
		if( select( 1, &fds, NULL, NULL, &tv ) <= 0 )
		{
			if(m_threadRunning)	// timeout
				continue;
			else
				break;
		}

		// Read in single line from "stdin"
		memset( cmd, 0, sizeof( cmd ) ); 
		if( fgets( cmd, 300, stdin ) == NULL )
			continue;
#endif

		len = strlen(cmd);
		for( i = 0; i < len; ++i )
		{
			if(cmd[i] == '\n' || cmd[i] == '\r')
				cmd[i] = '\0';
		}

		HandleConsoleInput(&g_localConsole, cmd);
	}
	m_isRunning = false;
	return false;
}
*/