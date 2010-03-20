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

#ifndef SOCKET_DEFINES_H
#define SOCKET_DEFINES_H

#include "../Common.h"
using namespace std;

/* Implementation Selection */
#ifdef WIN32		// Easy
#define CONFIG_USE_IOCP
//#define CONFIG_USE_SELECT
#else

// unix defines
#define SOCKET int
#define SD_BOTH SHUT_RDWR

#if UNIX_FLAVOUR == UNIX_FLAVOUR_LINUX

// select: epoll
#include <sys/epoll.h>
#define CONFIG_USE_EPOLL

#elif UNIX_FLAVOUR == UNIX_FLAVOUR_BSD

// select: kqueue
#include <sys/event.h>
#define CONFIG_USE_KQUEUE

#elif UNIX_FLAVOUR == UNIX_FLAVOUR_OSX
// select: kqueue
#include <sys/event.h>
#define CONFIG_USE_KQUEUE


#endif

#endif

/* IOCP Defines */

#ifdef CONFIG_USE_IOCP
enum SocketIOEvent
{
	SOCKET_IO_EVENT_READ_COMPLETE   = 0,
	SOCKET_IO_EVENT_WRITE_END		= 1,
	SOCKET_IO_THREAD_SHUTDOWN		= 2,
	NUM_SOCKET_IO_EVENTS			= 3,
};

class OverlappedStruct
{
public:
	OVERLAPPED m_overlap;
	SocketIOEvent m_event;
	volatile long m_inUse;
	OverlappedStruct(SocketIOEvent ev) : m_event(ev)
	{
		memset(&m_overlap, 0, sizeof(OVERLAPPED));
		m_inUse = 0;
	};

	OverlappedStruct()
	{
		memset(&m_overlap, 0, sizeof(OVERLAPPED));
		m_inUse = 0;
	}

	__forceinline void Reset(SocketIOEvent ev)
	{
		memset(&m_overlap, 0, sizeof(OVERLAPPED));
		m_event = ev;
	}

	void Mark()
	{
		long val = InterlockedCompareExchange(&m_inUse, 1, 0);
		if(val != 0)
			printf("!!!! Network: Detected double use of read/write event! Previous event was %u.\n", m_event);
	}

	void Unmark()
	{
		InterlockedExchange(&m_inUse, 0);
	}
};

#endif

#endif
