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


#ifndef SOCKETMGR_H_WIN32
#define SOCKETMGR_H_WIN32

#ifdef CONFIG_USE_IOCP

class Socket;
class SERVER_DECL SocketMgr : public Singleton<SocketMgr>
{
public:
	SocketMgr();
	~SocketMgr();

	ASCENT_INLINE HANDLE GetCompletionPort() { return m_completionPort; }
	void SpawnWorkerThreads();
	void CloseAll();
	void AddSocket(Socket * s)
	{
		socketLock.Acquire();
		_sockets.insert(s);
		socketLock.Release();
	}

	void RemoveSocket(Socket * s)
	{
		socketLock.Acquire();
		_sockets.erase(s);
		socketLock.Release();
	}

	void ShutdownThreads();
	long threadcount;

private:
	HANDLE m_completionPort;
	set<Socket*> _sockets;
	Mutex socketLock;
};

#define sSocketMgr SocketMgr::getSingleton()

typedef void(*OperationHandler)(Socket * s, uint32 len);

class SocketWorkerThread : public ThreadContext
{
public:
	bool run();
};

void SERVER_DECL HandleReadComplete(Socket * s, uint32 len);
void SERVER_DECL HandleWriteComplete(Socket * s, uint32 len);
void SERVER_DECL HandleShutdown(Socket * s, uint32 len);

static OperationHandler ophandlers[NUM_SOCKET_IO_EVENTS] = {
	&HandleReadComplete,
	&HandleWriteComplete,
	&HandleShutdown };

#endif
#endif