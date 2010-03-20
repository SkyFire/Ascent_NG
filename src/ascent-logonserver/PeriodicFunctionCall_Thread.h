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

#include "../ascent-shared/CallBack.h"

template<class Type>
class PeriodicFunctionCaller : public ThreadContext
{
public:
	template<class T>
		PeriodicFunctionCaller(T * callback, void (T::*method)(), uint32 Interval)
	{
		cb = new CallbackP0<T>(callback, method);
		interval = Interval;
		running = true;
	}

	~PeriodicFunctionCaller()
	{
		delete cb;
	}

	bool run()
	{
#ifndef WIN32
		uint32 start = getMSTime();
		uint32 end;
		uint32 etime;
		// initial rest
		Sleep(interval);

		while(running && mrunning)
		{
			start = getMSTime();
			
			// execute the callback
			cb->execute();

			end = getMSTime();
			etime = end - start;
			if(etime < interval)
				Sleep(interval - etime);
		}
#else
		thread_active=true;
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		for(;;)
		{
			WaitForSingleObject(hEvent, interval);

			if(!running)
				break;	/* we got killed */

			/* times up */
			ResetEvent(hEvent);
			cb->execute();
		}
		thread_active=false;
#endif
		return false;
	}

	void kill()
	{
		running = false;
#ifdef WIN32
		/* push the event */
		SetEvent(hEvent);
		printf("Waiting for PFC thread to exit...");
		/* wait for the thread to exit */
		while(thread_active)
		{
			Sleep(100);
		}
		printf(" done.\n");
#endif
	}

private:
	CallbackBase * cb;
	uint32 interval;
	bool running;
#ifdef WIN32
	bool thread_active;
	HANDLE hEvent;
#endif
};

#define SpawnPeriodicCallThread(otype, ptr, method, interval) \
	launch_thread(new PeriodicFunctionCaller<otype>(ptr, method, interval));
