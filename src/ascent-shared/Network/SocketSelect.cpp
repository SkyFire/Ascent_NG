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
