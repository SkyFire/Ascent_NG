/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef LISTEN_SOCKET_WIN32_H
#define LISTEN_SOCKET_WIN32_H

#ifdef CONFIG_USE_IOCP

#include "../Threading/ThreadPool.h"

template<class T>
class SERVER_DECL ListenSocket : public ThreadContext
{
public:
	ListenSocket(const char * ListenAddress, uint32 Port)
	{
		m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		SocketOps::ReuseAddr(m_socket);
		SocketOps::Blocking(m_socket);

		m_address.sin_family = AF_INET;
		m_address.sin_port = ntohs((u_short)Port);
		m_address.sin_addr.s_addr = htonl(INADDR_ANY);
		m_opened = false;

		if(strcmp(ListenAddress, "0.0.0.0"))
		{
			struct hostent * hostname = gethostbyname(ListenAddress);
			if(hostname != 0)
				memcpy(&m_address.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
		}

		// bind.. well attempt to.
		int ret = ::bind(m_socket, (const sockaddr*)&m_address, sizeof(m_address));
		if(ret != 0)
		{
			DEBUG_LOG("ListenSocket","Bind unsuccessful on port %u.", Port);
			return;
		}

		ret = listen(m_socket, 5);
		if(ret != 0) 
		{
			printf("Unable to listen on port %u.", Port);
			return;
		}

		m_opened = true;
		len = sizeof(sockaddr_in);
		m_cp = sSocketMgr.GetCompletionPort();
	}

	~ListenSocket()
	{
		Close();	
	}

	bool run()
	{
		while(m_opened)
		{
			//aSocket = accept(m_socket, (sockaddr*)&m_tempAddress, (socklen_t*)&len);
			aSocket = WSAAccept(m_socket, (sockaddr*)&m_tempAddress, (socklen_t*)&len, NULL, NULL);
			if(aSocket == INVALID_SOCKET)
				continue;		// shouldn't happen, we are blocking.

			socket = new T(aSocket);
			socket->SetCompletionPort(m_cp);
			socket->Accept(&m_tempAddress);
		}
		return false;
	}

	void Close()
	{
		// prevent a race condition here.
		bool mo = m_opened;
		m_opened = false;

		if(mo)
			SocketOps::CloseSocket(m_socket);
	}

	ASCENT_INLINE bool IsOpen() { return m_opened; }

private:
	SOCKET m_socket;
	struct sockaddr_in m_address;
	struct sockaddr_in m_tempAddress;
	bool m_opened;
	uint32 len;
	SOCKET aSocket;
	T * socket;
	HANDLE m_cp;
};

#endif
#endif
