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

#ifndef _FASTQUEUE_H
#define _FASTQUEUE_H

/** dummy lock to use a non-locked queue.
 */
class DummyLock
{
public:
	ASCENT_INLINE void Acquire() { }
	ASCENT_INLINE void Release() { }
};

/** linked-list style queue
 */
template<class T, class LOCK>
class FastQueue
{
	struct node
	{
		T element;
		node * next;
	};

	node * last;
	node * first;
	LOCK m_lock;

public:

	FastQueue()
	{
		last = 0;
		first = 0;
	}

	~FastQueue()
	{
		Clear();
	}

	void Clear()
	{
		// clear any elements
		while(last != 0)
			Pop();
	}

	void Push(T elem)
	{
		m_lock.Acquire();
		node * n = new node;
		if(last)
			last->next = n;
		else
			first = n;

		last = n;
		n->next = 0;
		n->element = elem;
		m_lock.Release();
	}

	T Pop()
	{
		m_lock.Acquire();
		if(first == 0)
		{
			m_lock.Release();
			return reinterpret_cast<T>(0);
		}
        
		T ret = first->element;
		node * td = first;
		first = td->next;
		if(!first)
			last = 0;

		delete td;
		m_lock.Release();
		return ret;
	}

	T front()
	{
		m_lock.Acquire();
		if(first == 0)
		{
			m_lock.Release();
			return reinterpret_cast<T>(0);
		}

		T ret = first->element;
		m_lock.Release();
		return ret;
	}

	void pop_front()
	{
		m_lock.Acquire();
		if(first == 0)
		{
			m_lock.Release();
			return;
		}

		node * td = first;
		first = td->next;
		if(!first)
			last = 0;

		delete td;
		m_lock.Release();
	}

	bool HasItems()
	{
		bool ret;
		m_lock.Acquire();
		ret = (first != 0);
		m_lock.Release();
		return ret;
	}
};

#endif
