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

#ifndef __LOGONCONSOLE_H
#define __LOGONCONSOLE_H

#include "Common.h"

class LogonConsoleThread : public ThreadContext
{
public:
	bool kill;
	LogonConsoleThread();
	~LogonConsoleThread();
	bool run();
};

class LogonConsole :  public Singleton < LogonConsole >
{
	friend class LogonConsoleThread;

public:						// Public methods:
	void Kill();

protected:					// Protected methods:
	LogonConsoleThread *_thread;

	// Process one command
	void ProcessCmd(char *cmd);

	// quit | exit
	void TranslateQuit(char *str);
	void ProcessQuit(int delay);
	void CancelShutdown(char *str);

	// help | ?
	void TranslateHelp(char *str);
	void ProcessHelp(char *command);

	void ReloadAccts(char *str);
	void TranslateRehash(char* str);
};

#define sLogonConsole LogonConsole::getSingleton()

#endif
