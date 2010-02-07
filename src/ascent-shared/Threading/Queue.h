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

#ifndef FQUEUE_H
#define FQUEUE_H

#include "Condition.h"
#include "Mutex.h"
 
template<class T> 
class FQueue 
{
public:
	ASCENT_INLINE FQueue() : cond(&lock) {first=last=NULL;size=0;}
	volatile unsigned int size;

	uint32 get_size()
	{
		uint32 ret;
		cond.BeginSynchronized();
		ret = size;
		cond.EndSynchronized();
		return ret;
	}

	void push(T &item)
	{
		h*p=new h;
		p->value=item;
		p->pNext=NULL;
		
		//lock.Acquire();
		cond.BeginSynchronized();
		if(last)//have some items
		{
			last->pNext=p;
			last=p;
			size++;
		}
		else//first item
		{
			last=first=p;
			size=1;
			cond.Signal();
		}
		//lock.Release();
		cond.EndSynchronized();
	}

	T pop_nowait()
	{
		//lock.Acquire();
		cond.BeginSynchronized();
		if(size==0)
		{
			cond.EndSynchronized();
			return NULL;
		}

		h*tmp=first;
		if(tmp == NULL)
		{
			cond.EndSynchronized();
			return NULL;
		}

		if(--size)//more than 1 item
		{
			first=(h*)first->pNext;
		}
		else//last item
		{
			first=last=NULL;
		}
		//lock.Release();
		cond.EndSynchronized();

		T returnVal = tmp->value;
		delete tmp;

		return returnVal;
	}

	T pop()
	{
		//lock.Acquire();
		cond.BeginSynchronized();
		if(size==0)
		cond.Wait();

		h*tmp=first;
		if(tmp == NULL)
		{
			cond.EndSynchronized();
			return NULL;
		}

		if(--size)//more than 1 item
		{
			first=(h*)first->pNext;
		}
		else//last item
		{
			first=last=NULL;
		}
		//lock.Release();
		cond.EndSynchronized();

		T returnVal = tmp->value;
		delete tmp;
		
		return returnVal;
	}	

	ASCENT_INLINE Condition& GetCond() { return cond; }
	
private:
	struct h
	{
		T value;
		void *pNext;
	};

	h*first;
	h*last;
	
	Mutex lock;
	Condition cond;

};

#endif 


