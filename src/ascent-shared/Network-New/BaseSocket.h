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

#ifndef _NETLIB_BASESOCKET_H
#define _NETLIB_BASESOCKET_H

class BaseBuffer;
class BaseSocket
{
public:
	/** Virtual destructor
	 */
	virtual ~BaseSocket() {}

	/** Returns the socket's file descriptor
	 */
	inline int GetFd() { return m_fd; }

	/** Sets the socket's file descriptor
	 * @param new_fd The new file descriptor
	 */
	inline void SetFd(int new_fd) { m_fd = new_fd; }

	/** Is this socket in a read state? Or a write state?
	 */
	virtual bool Writable() = 0;

	/** Virtual OnRead() callback
	 */
	virtual void OnRead(size_t len) = 0;

	/** Virtual OnWrite() callback
	 */
	virtual void OnWrite(size_t len) = 0;

	/** Virtual OnError() callback
	 */
	virtual void OnError(int errcode) = 0;

	/** This is a windows-only implementation
	 */
	virtual void OnAccept(void * pointer) = 0;

	/** Are we connected?
	*/
	inline bool IsConnected() { return m_connected; }

	/** If for some reason we need to access the buffers directly 
	 * (shouldn't happen) these will return them
	 */
	inline BaseBuffer * GetReadBuffer() { return m_readBuffer; }
	inline BaseBuffer * GetWriteBuffer() { return m_writeBuffer; }

	/** Write mutex (so we don't post a write event twice
	 */
	volatile long m_writeLock;

	/** Disconnects the socket
	 */
	virtual void Disconnect() = 0;

	/** Deletes the socket
	 */
	virtual void Delete() = 0;

protected:
	/** This socket's file descriptor
	 */
	int m_fd;

	/** deleted/disconnected markers
	 */
	bool m_deleted;
	bool m_connected;

	/** Read (inbound) buffer
	 */
	BaseBuffer * m_readBuffer;

	/** Write (outbound) buffer
	 */
	BaseBuffer * m_writeBuffer;
};

#endif		// _NETLIB_BASESOCKET_H
