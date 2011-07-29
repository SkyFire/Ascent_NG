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
#include "Log.h"
#include "BufferPool.h"

size_t BufferPool::BufferBucket::buffer_sizes[BUFFER_BUCKET_COUNT] = {
	20,			// 20 bytes
	50,			// 50 bytes
	100,		// 100 bytes
	200,		// 200 bytes
	500,		// 500 bytes
	1000,		// 1 kbyte
	5000,		// 5 kbyte
	10000,		// 10 kbyte (shouldn't be used much)
};

BufferPool g_bufferPool;
