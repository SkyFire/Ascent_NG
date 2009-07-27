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

#ifndef WOWSERVER_LOG_H
#define WOWSERVER_LOG_H

#include "Common.h"
#include "Singleton.h"

class WorldPacket;
class WorldSession;

#ifdef WIN32

#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY

#else

#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6

#endif
std::string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp);

// log define
#ifdef _DEBUG
#define OUT_DEBUG sLog.outDebug
#define DEBUG_LOG Log.Debug
#else
#define OUT_DEBUG
#define DEBUG_LOG
#endif

class SERVER_DECL oLog : public Singleton< oLog > {
public:
  void outString( const char * str, ... );
  void outError( const char * err, ... );
  void outDetail( const char * str, ... );
  void outDebug( const char * str, ... );
  void outTime( );

  void fLogText(const char *text);
  void SetLogging(bool enabled);
  
  void Init(int32 fileLogLevel, int32 screenLogLevel);
  void SetFileLoggingLevel(int32 level);
  void SetScreenLoggingLevel(int32 level);
  void outColor(uint32 colorcode, const char * str, ...);
  
#ifdef WIN32
  HANDLE stdout_handle, stderr_handle;
#endif

  int32 m_fileLogLevel;
  int32 m_screenLogLevel;
  
  FILE *m_file;
};

class SessionLogWriter
{
	FILE * m_file;
	char * m_filename;
public:
	SessionLogWriter(const char * filename, bool open);
	~SessionLogWriter();

	void write(const char* format, ...);
	void writefromsession(WorldSession* session, const char* format, ...);
	ASCENT_INLINE bool IsOpen() { return (m_file != NULL); }
	void Open();
	void Close();
};

extern SessionLogWriter * Anticheat_Log;
extern SessionLogWriter * GMCommand_Log;
extern SessionLogWriter * Player_Log;

#define sLog oLog::getSingleton()
#define sCheatLog (*Anticheat_Log)
#define sGMLog (*GMCommand_Log)
#define sPlrLog (*Player_Log)

class WorldLog : public Singleton<WorldLog>
{
public:
	WorldLog();
	~WorldLog();

	void LogPacket(uint32 len, uint16 opcode, const uint8* data, uint8 direction);
	void Enable();
	void Disable();
	void EnableXml();
	void DisableXml();
private:
	FILE * m_file;
	FILE * m_xml;
	Mutex mutex;
	bool bEnabled;
	bool bEnabledXml;
	char *onlyPlayer;
};

#define sWorldLog WorldLog::getSingleton()

#endif

