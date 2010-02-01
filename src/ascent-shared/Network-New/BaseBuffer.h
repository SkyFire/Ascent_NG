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

#ifndef _NETLIB_BASE_BUFFER_H
#define _NETLIB_BASE_BUFFER_H

class BaseBuffer
{
public:
	/** Destructor
	 */
	virtual ~BaseBuffer() {}

	/** Allocate the buffer with room for size bytes
	 * @param size the number of bytes to allocate
	 */
	virtual void Allocate(size_t size) = 0;

	/** Write bytes to the buffer
	 * @param data pointer to the data to be written
	 * @param bytes number of bytes to be written
	 * @return true if was successful, otherwise false
	 */
	virtual bool Write(const void * data, size_t bytes) = 0;

	/** Read bytes from the buffer
	 * @param destination pointer to destination where bytes will be written
	 * @param bytes number of bytes to read
	 * @return true if there was enough data, false otherwise
	 */
	virtual bool Read(void * destination, size_t bytes) = 0;

	/** Returns a pointer at the "end" of the buffer, where new data can be written
	 */
	virtual void * GetBuffer() = 0;

	/** Returns a pointer at the "beginning" of the buffer, where data can be pulled from
	 */
	virtual void * GetBufferOffset() = 0;

	/** Returns the number of available bytes left.
	 */
	virtual size_t GetSpace() = 0;

	/** Increments the "writen" pointer forward len bytes
	 * @param len number of bytes to step
	 */
	virtual void IncrementWritten(size_t len) = 0;

	/** Returns the number of bytes currently stored in the buffer.
	 */
	virtual size_t GetSize() = 0;

	/** Removes len bytes from the front of the buffer
	 * @param len the number of bytes to "cut"
	 */
	virtual void Remove(size_t len) = 0;
};

#endif		// _NETLIB_BASE_BUFFER_H

