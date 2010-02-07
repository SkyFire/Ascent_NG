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


#ifndef SOCKETMGR_LINUX_H
#define SOCKETMGR_LINUX_H

#include "SocketDefines.h"
#ifdef CONFIG_USE_EPOLL

#define SOCKET_HOLDER_SIZE 30000    // You don't want this number to be too big, otherwise you're gonna be eating
                                    // memory. 65536 = 256KB, so thats no big issue for now, and I really can't
                                    // see anyone wanting to have more than 65536 concurrent connections.

#define THREAD_EVENT_SIZE 4096      // This is the number of socket events each thread can receieve at once.
                                    // This default value should be more than enough.

class Socket;
class SocketWorkerThread;
class ListenSocketBase;

class SocketMgr : public Singleton<SocketMgr>
{
    /// /dev/epoll instance handle
    int epoll_fd;

    // fd -> pointer binding.
    Socket * fds[SOCKET_HOLDER_SIZE];
	ListenSocketBase * listenfds[SOCKET_HOLDER_SIZE];

    /// socket counter
    int socket_count;

public:

    /// friend class of the worker thread -> it has to access our private resources
    friend class SocketWorkerThread;

    /// constructor > create epoll device handle + initialize event set
    SocketMgr()
    {
        epoll_fd = epoll_create(SOCKET_HOLDER_SIZE);
        if(epoll_fd == -1)
        {
            printf("Could not create epoll fd (/dev/epoll).");
            exit(-1);
        }

        // null out the pointer array
        memset(fds, 0, sizeof(void*) * SOCKET_HOLDER_SIZE);
		memset(listenfds, 0, sizeof(void*) * SOCKET_HOLDER_SIZE);
    }

    /// destructor > destroy epoll handle
    ~SocketMgr()
    {
        // close epoll handle
        close(epoll_fd);
    }

    /// add a new socket to the epoll set and to the fd mapping
    void AddSocket(Socket * s);
	void AddListenSocket(ListenSocketBase * s);

    /// remove a socket from epoll set/fd mapping
    void RemoveSocket(Socket * s);

    /// returns epoll fd
    inline int GetEpollFd() { return epoll_fd; }

    /// returns number of sockets in array
    inline int Count() { return socket_count; }

    /// closes all sockets
    void CloseAll();

    /// spawns worker threads
    void SpawnWorkerThreads();
};

class SocketWorkerThread : public ThreadContext
{
    /// epoll event struct
    struct epoll_event events[THREAD_EVENT_SIZE];
    bool running;
public:
    bool run();
    void OnShutdown()
    {
        running = false;
    }
};

#define sSocketMgr SocketMgr::getSingleton()

#endif

#endif
