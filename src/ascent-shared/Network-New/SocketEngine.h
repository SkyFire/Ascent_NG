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

#ifndef _NETLIB_SOCKETENGINE_H
#define _NETLIB_SOCKETENGINE_H

class SocketEngine : public Singleton<SocketEngine>
{
public:
	virtual ~SocketEngine() {}

	/** Adds a socket to the engine.
	 */
	virtual void AddSocket(BaseSocket * s) = 0;

	/** Removes a socket from the engine. It should not receive any more events.
	 */
	virtual void RemoveSocket(BaseSocket * s) = 0;

	/** This is called when a socket has data to write for the first time.
	 */
	virtual void WantWrite(BaseSocket * s) = 0;

	/** Spawn however many worker threads this engine requires
	 */
	virtual void SpawnThreads() = 0;

	/** Shutdown the socket engine, disconnect any associated sockets and 
	 * deletes itself and the socket deleter.
	 */
	virtual void Shutdown() = 0;

	/** Called by SocketWorkerThread, this is the network loop.
	 */
	virtual void MessageLoop() = 0;
};

class SocketEngineThread : public ThreadBase
{
	SocketEngine * se;
public:
	SocketEngineThread(SocketEngine * s) : se(s) {}
	void run()
	{
		se->MessageLoop();
	}
};

class SocketDeleter : public Singleton<SocketDeleter>
{
	typedef map<BaseSocket*, time_t> SocketDeleteMap;
	SocketDeleteMap _map;
	Mutex _lock;
public:
	/** Call this every loop of your program to delete old sockets
	 */
	void Update()
	{
		time_t ct = time(NULL);
        _lock.Acquire();
		SocketDeleteMap::iterator it, it2;
		for(it = _map.begin(); it != _map.end();)
		{
			it2 = it++;
            if(it2->second <= ct)
			{
				delete it2->first;
				_map.erase(it2);
			}
		}

		_lock.Release();
	}

	/** Add a socket for deletion in time.
	 */
	void Add(BaseSocket * s)
	{
		_lock.Acquire();
		_map[s] = time(NULL) + 15;
		_lock.Release();
	}

	/** Delete all sockets in this socket deleter, regardless of time
	 */
	void Kill()
	{
		SocketDeleteMap::iterator itr = _map.begin();
		for(; itr != _map.end(); ++itr)
			delete itr->first;

		_map.clear();
	}
};

#define sSocketEngine SocketEngine::getSingleton()
#define sSocketDeleter SocketDeleter::getSingleton()

#endif		// _NETLIB_SOCKETENGINE_H
