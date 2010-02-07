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

#ifndef SOCKETMGR_H_POLL
#define SOCKETMGR_H_POLL

#ifdef CONFIG_USE_POLL

#include <poll.h>

#define SOCKET_HOLDER_SIZE 65536    // You don't want this number to be too big, otherwise you're gonna be eating
                                    // memory. 65536 = 256KB, so thats no big issue for now, and I really can't
                                    // see anyone wanting to have more than 65536 concurrent connections.

class Socket;
class SERVER_DECL SocketMgr : public Singleton<SocketMgr>
{
	/* Unix sockets start from 0 for every process.
	 */
	Socket * fds[SOCKET_HOLDER_SIZE];

	/** How many sockets are we holding? 
	 */
	int socket_count;

	/** Highest fd - used in poll() call.
	 */
	int highest_fd;

	/** Our poll set.
	 */
	struct pollfd poll_events[SOCKET_HOLDER_SIZE];

public:

	SocketMgr();
	~SocketMgr();

	/// returns number of sockets in array
	inline int Count() { return socket_count; }

	/** socket removal/adding
	 */
	void AddSocket(Socket * s);
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
