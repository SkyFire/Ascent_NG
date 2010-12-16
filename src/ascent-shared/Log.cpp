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

#include "Common.h"
#include "Config/ConfigEnv.h"
#include "Log.h"
#include "NGLog.h"
#include <stdarg.h>

string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp)
{

	char p[MAX_PATH];
	p[0] = 0;
	time_t t = time(NULL);
	tm * a = gmtime(&t);
	strcat(p, Prefix);
	strcat(p, "/");
	strcat(p, Description);
	if(useTimeStamp)
	{
		char ftime[100];
		snprintf(ftime, 100, "-%-4d-%02d-%02d %02d-%02d-%02d", a->tm_year+1900, a->tm_mon+1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec);
		strcat(p, ftime);
	}

	strcat(p, ".log");
	return string(p);
}

createFileSingleton( oLog );
createFileSingleton(CLog);
initialiseSingleton( WorldLog );

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;
#ifndef WIN32
static const char* colorstrings[TBLUE+1] = {
"",
"\033[22;31m",
"\033[22;32m",
"\033[01;33m",
//"\033[22;37m",
"\033[0m",
"\033[01;37m",
"\033[22;34m",
};
#endif

void oLog::outTime()
{
#ifndef WIN32
	char buf[256];
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	if (tm)
	{
		strftime(buf, 256, "[%Y-%m-%d %T] ", tm);
		fprintf(m_file, buf);
	}
#endif
}

void oLog::outString( const char * str, ... )
{
	va_list ap;
	char buf[32768];

	if(m_fileLogLevel < 0 && m_screenLogLevel < 0)
		return;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);
	
	if(m_screenLogLevel >= 0)
	{
		printf(buf);
		putc('\n', stdout);
	}
	if(m_fileLogLevel >= 0 && m_file)
	{
		outTime();
		fprintf(m_file, buf);
		putc('\n', m_file);
	}
}

void oLog::outError( const char * err, ... )
{
	va_list ap;
	char buf[32768];

	if(m_fileLogLevel < 1 && m_screenLogLevel < 1)
		return;

	va_start(ap, err);
	vsnprintf(buf, 32768, err, ap);
	va_end(ap);

	if(m_screenLogLevel >= 1)
	{
#ifdef WIN32
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
		puts(colorstrings[TRED]);
#endif
		fprintf(stderr, buf);
		putc('\n', stderr);
#ifdef WIN32
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#else
		puts(colorstrings[TNORMAL]);
#endif
	}
	if(m_fileLogLevel >= 1 && m_file)
	{
		outTime();
		fprintf(m_file, buf);
		putc('\n', m_file);
	}
}

void oLog::outDetail( const char * str, ... )
{
	va_list ap;
	char buf[32768];

	if(m_fileLogLevel < 2 && m_screenLogLevel < 2)
		return;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	if(m_screenLogLevel >= 2)
	{
		printf(buf);
		putc('\n', stdout);
	}
	if(m_fileLogLevel >= 2 && m_file)
	{
		outTime();
		fprintf(m_file, buf);
		putc('\n', m_file);
	}
}

void oLog::outDebug( const char * str, ... )
{
	va_list ap;
	char buf[32768];

	if(m_fileLogLevel < 3 && m_screenLogLevel < 3)
		return;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	if(m_screenLogLevel >= 3)
	{
		printf(buf);
		putc('\n', stdout);
	}
	if(m_fileLogLevel >= 3 && m_file)
	{
		outTime();
		fprintf(m_file, buf);
		putc('\n', m_file);
	}
}

void oLog::Init(int32 fileLogLevel, int32 screenLogLevel)
{
	m_screenLogLevel = screenLogLevel;
	Log.log_level = screenLogLevel;
	m_fileLogLevel = fileLogLevel;
	m_file = NULL;

	if (m_fileLogLevel >= 0)
	{
		const char *filename = "file.log";
		m_file = fopen(filename, "w");
		if (m_file == NULL)
		{
			fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, filename, strerror(errno));
		}
	}

	// get error handle
#ifdef WIN32
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}
void oLog::SetScreenLoggingLevel(int32 level)
{
	m_screenLogLevel = level;
}

void oLog::SetFileLoggingLevel(int32 level)
{
	m_fileLogLevel = level;
}
void SessionLogWriter::write(const char* format, ...)
{
	if(!m_file)
		return;

	va_list ap;
	va_start(ap, format);
	char out[32768];

	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	sprintf(out, "[%-4d-%02d-%02d %02d:%02d:%02d] ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	size_t l = strlen(out);
	vsnprintf(&out[l], 32768 - l, format, ap);

	fprintf(m_file, "%s\n", out);
	va_end(ap);
}

WorldLog::WorldLog()
{
	bEnabled = false;
	bEnabledXml = false;
	onlyPlayer = NULL;
	m_file=NULL;
	m_xml=NULL;

	if (Config.MainConfig.GetBoolDefault("LogLevel", "World", false))
	{
		Log.Notice("WorldLog", "Enabling packetlog output to \"world.log\"");
		Enable();
	} else {
		Disable();
	}

	if (Config.MainConfig.GetBoolDefault("LogLevel", "WorldXml", false))
	{
		Log.Notice("WorldLog", "Enabling packetlog output to \"world.xml\"");
		EnableXml();
	} else {
		DisableXml();
	}

}

void WorldLog::Enable()
{
	if(bEnabled)
		return;

	bEnabled = true;
	if(m_file != NULL)
	{
		Disable();
		bEnabled=true;
	}
	m_file = fopen("world.log", "w");
}

void WorldLog::Disable()
{
	if(!bEnabled)
		return;

	bEnabled = false;
	if(!m_file)
		return;

	fflush(m_file);
	fclose(m_file);
	m_file=NULL;
}

void WorldLog::EnableXml()
{
	if(bEnabledXml)
		return;

	bEnabledXml = true;
	if(m_xml != NULL)
	{
		DisableXml();
		bEnabledXml=true;
	}
	m_xml = fopen("world.xml", "w");
	if (m_xml)
	{
		fprintf(m_xml, "<?xml version=\"1.0\" ?><log>");
	}
}

void WorldLog::DisableXml()
{
	if(!bEnabledXml)
		return;

	bEnabledXml = false;
	if(!m_xml)
		return;

	fprintf(m_xml, "</log>");
	fflush(m_xml);
	fclose(m_xml);
	m_xml=NULL;
}

WorldLog::~WorldLog()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}

	if (m_xml)
	{
		fprintf(m_xml, "</log>");
		fclose(m_xml);
		m_xml = NULL;
	}
}

void oLog::outColor(uint32 colorcode, const char * str, ...)
{
	if( !str ) return;
	va_list ap;
	va_start(ap, str);
#ifdef WIN32
	SetConsoleTextAttribute(stdout_handle, colorcode);
#else
	printf(colorstrings[colorcode]);
#endif
	vprintf( str, ap );
	fflush(stdout);
	va_end(ap);
}

void SessionLogWriter::Open()
{
	m_file = fopen(m_filename, "a");
}

void SessionLogWriter::Close()
{
	if(!m_file) return;
	fflush(m_file);
	fclose(m_file);
	m_file=NULL;
}

SessionLogWriter::SessionLogWriter(const char * filename, bool open)
{
	m_filename = strdup(filename);
	m_file=NULL;
	if(open)
		Open();
}

SessionLogWriter::~SessionLogWriter()
{
	if(m_file)
		Close();

	free(m_filename);
}


