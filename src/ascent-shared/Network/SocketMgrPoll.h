/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
