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
