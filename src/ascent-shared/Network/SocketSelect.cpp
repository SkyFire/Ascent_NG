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

#include "Network.h"
#ifdef CONFIG_USE_SELECT

void Socket::BurstPush()
{
	if(AcquireSendLock())
		sSocketMgr.WantWrite(GetFd());
}

void Socket::ReadCallback(uint32 len)
{
	int bytes = recv(m_fd, ((char*)m_readBuffer + m_readByteCount), m_readBufferSize - m_readByteCount, 0);
	if(bytes <= 0)
	{
		m_readMutex.Release();
		Disconnect();
		return;
	}    
	else if(bytes > 0)
	{
		m_readByteCount += bytes;
		// call virtual onread()
		OnRead();
	}

	m_readMutex.Release();
}

void Socket::WriteCallback()
{
	// We should already be locked at this point, so try to push everything out.
	int bytes_written = send(m_fd, (const char*)m_writeBuffer, m_writeByteCount, 0);
	if(bytes_written < 0)
	{
		// error.
		Disconnect();
		return;
	}

	if(bytes_written)
		RemoveWriteBufferBytes(bytes_written, false);
}

#endif
