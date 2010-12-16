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

#ifndef SOCKETMGR_H_SELECT
#define SOCKETMGR_H_SELECT

#ifdef CONFIG_USE_SELECT

class Socket;
class SERVER_DECL SocketMgr : public Singleton<SocketMgr>
{
//#ifdef WIN32
	/* Because windows uses global fd's (not starting from 0 for every app) we have to make this
	 * a lot larger :( 
	 */
//	Socket * fds[FD_SETSIZE * 1500];
//#else
	/* Unix sockets start from 0 for every process.
	 */
//	Socket * fds[FD_SETSIZE];
//#endif

	/** FD->Pointer Map
	 */
	map<int, Socket*> fds;

	/** How many sockets are we holding? 
	 */
	int socket_count;

	/** Our FD sets 
	 */
	FD_SET m_allSet;
	FD_SET m_readableSet;
	FD_SET m_writableSet;
	FD_SET m_exceptionSet;
	Mutex m_setLock;
	
public:

	SocketMgr();
	~SocketMgr();

	/// returns number of sockets in array
	inline int Count() { return socket_count; }

	/** socket removal/adding
	 */
	void AddSocket(Socket * s);
	void AddListenSocket(Socket * s);
	void RemoveSocket(Socket * s);
	void WantWrite(int fd);

	void SpawnWorkerThreads();
	void CloseAll();
	void ShutdownThreads();

	void thread_function();
};

#define sSocketMgr SocketMgr::getSingleton()

class SocketWorkerThread : public ThreadBase
{
public:
	void run();
};

#endif
#endif