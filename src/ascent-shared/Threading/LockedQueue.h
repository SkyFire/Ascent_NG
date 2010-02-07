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

#ifndef _THREADING_LOCKED_QUEUE_H
#define _THREADING_LOCKED_QUEUE_H

#include "Mutex.h"
#include <deque>

template<class TYPE>
class LockedQueue
{
public:
	~LockedQueue()
	{

	}

	ASCENT_INLINE void add(const TYPE& element)
	{
		mutex.Acquire();
		queue.push_back(element);
		mutex.Release();
	}

	ASCENT_INLINE TYPE next()
	{
		mutex.Acquire();
		assert(queue.size() > 0);
		TYPE t = queue.front();
		queue.pop_front();
		mutex.Release();
		return t;
	}

	ASCENT_INLINE size_t size()
	{
		mutex.Acquire();
		size_t c = queue.size();
		mutex.Release();
		return c;
	}

	ASCENT_INLINE bool empty()
	{	// return true only if sequence is empty
		mutex.Acquire();
		bool isEmpty = queue.empty();
		mutex.Release();
		return isEmpty;
	}

	ASCENT_INLINE TYPE get_first_element()
	{
		mutex.Acquire();
		TYPE t; 
		if(queue.size() == 0)
			t = reinterpret_cast<TYPE>(0);
		else
			t = queue.front();
		mutex.Release();
		return t;			
	}

	ASCENT_INLINE void pop()
	{
		mutex.Acquire();
		ASSERT(queue.size() > 0);
		queue.pop_front();
		mutex.Release();
	}

protected:
	std::deque<TYPE> queue;
	Mutex mutex;
};

#endif
