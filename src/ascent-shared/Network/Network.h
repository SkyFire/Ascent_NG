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

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../Log.h"
#include "../NGLog.h"
#include "CircularBuffer.h"
#include "SocketDefines.h"
#include "SocketOps.h"
#include "Socket.h"

#ifdef CONFIG_USE_IOCP
#include "SocketMgrWin32.h"
#include "ListenSocketWin32.h"
#endif

#ifdef CONFIG_USE_EPOLL
#include "SocketMgrLinux.h"
#include "ListenSocketLinux.h"
#endif

#ifdef CONFIG_USE_KQUEUE
#include "SocketMgrFreeBSD.h"
#include "ListenSocketFreeBSD.h"
#endif

#endif
