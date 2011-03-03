/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
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

#ifndef _NETLIB_SELECTENGINE_H
#define _NETLIB_SELECTENGINE_H

#ifdef NETLIB_SELECT

class BaseSocket;
class SelectEngine : public SocketEngine
{
public:
	SelectEngine();
	~SelectEngine();

	/** Adds a socket to the engine.
	 */
	void AddSocket(BaseSocket * s);

	/** Removes a socket from the engine. It should not receive any more events.
	 */
	void RemoveSocket(BaseSocket * s);

	/** This is called when a socket has data to write for the first time.
	 */
	void WantWrite(BaseSocket * s);

	/** Spawn however many worker threads this engine requires
	 */
	void SpawnThreads();

	/** Shutdown the socket engine, disconnect any associated sockets and 
	 * deletes itself and the socket deleter.
	 */
	void Shutdown();

	/** Called by SocketWorkerThread, this is the network loop.
	 */
	void MessageLoop();

protected:

	/** Thread running or not
	 */
	bool m_running;

	/** Protection for map
	 */
	Mutex m_socketLock;

	/** Map of fd->socket
	 */
	map<int, BaseSocket*> m_sockets;
};

inline void CreateSocketEngine() { new SelectEngine; }

#endif		// NETLIB_SELECT
#endif		// _NETLIB_SELECTENGINE_H
