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

#include "log.h"
#include "mutex.h"

static mutex l_writeMutex;
static char * l_logFilePath = NULL;
static FILE * l_logFile = NULL;
static int l_logLevel = 2;

void _log_openfile(const char * filename)
{
	mutex_lock(&l_writeMutex);
	if( l_logFilePath != NULL )
	{
		assert( l_logFile == NULL );
		l_logFile = fopen(l_logFilePath, "w");
	}
	mutex_unlock(&l_writeMutex);
}

void _log_closefile()
{
	mutex_lock(&l_writeMutex);
	if( l_logFile != NULL )
	{
		fflush(l_logFile);
		fclose(l_logFile);
		l_logFile = NULL;
	}
	mutex_unlock(&l_writeMutex);
}

void log_open()
{
	mutex_initialize(&l_writeMutex);
	if( l_logFilePath == NULL )
	{
		// log open? close it
		_log_closefile();
	}
	else
	{
		// not open? open it
		_log_closefile();
		_log_openfile(l_logFilePath);
	}
}

void log_close()
{
	_log_closefile();
}

void log_setloglevel(int new_level)
{
	mutex_lock(&l_writeMutex);
	l_logLevel = new_level;
	mutex_unlock(&l_writeMutex);
}

void log_setlogfile(const char * filename)
{
	_log_closefile();
	_log_openfile(filename);
}

void log_write(int level, const char * format, ...)
{
	va_list args;

	if( l_logLevel < level )
		return;
	
	va_start(args, format);
	mutex_lock(&l_writeMutex);
	vprintf(format, args);
	printf("\n");
	if( l_logFile != NULL )
	{
		vfprintf(l_logFile, format, args);
		fprintf(l_logFile, "\n");
	}

	va_end(args);
	mutex_unlock(&l_writeMutex);
}
