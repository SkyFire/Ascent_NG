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

#ifndef __MUTEX_H
#define __MUTEX_H

#include "common.h"

#ifdef WIN32

typedef CRITICAL_SECTION mutex;
static void mutex_lock(mutex* mut) { EnterCriticalSection(mut); }
static void mutex_unlock(mutex* mut) { LeaveCriticalSection(mut); }
static void mutex_initialize(mutex* mut) { InitializeCriticalSection(mut); }
static void mutex_free(mutex* mut) { DeleteCriticalSection(mut); }

#else

#include <pthread.h>
typedef pthread_mutex_t mutex;
static void mutex_lock(mutex* mut) { pthread_mutex_lock(mut); }
static void mutex_unlock(mutex* mut) { pthread_mutex_unlock(mut); }
void mutex_initialize(mutex* mut);
static void mutex_free(mutex* mut) { pthread_mutex_destroy(mut); }

#endif

#endif
