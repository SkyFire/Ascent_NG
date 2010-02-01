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