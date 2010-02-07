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
#ifdef CONFIG_USE_EPOLL

namespace SocketOps
{
    // Create file descriptor for socket i/o operations.
    SOCKET CreateTCPFileDescriptor()
    {
        // create a socket for use with overlapped i/o.
        return socket(AF_INET, SOCK_STREAM, 0);
    }

    // Disable blocking send/recv calls.
    bool Nonblocking(SOCKET fd)
    {
        uint32 arg = 1;
        return (::ioctl(fd, FIONBIO, &arg) == 0);
    }

    // Disable blocking send/recv calls.
    bool Blocking(SOCKET fd)
    {
        uint32 arg = 0;
        return (ioctl(fd, FIONBIO, &arg) == 0);
    }

    // Disable nagle buffering algorithm
    bool DisableBuffering(SOCKET fd)
    {
        uint32 arg = 1;
        return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
    }

    // Enable nagle buffering algorithm
    bool EnableBuffering(SOCKET fd)
    {
        uint32 arg = 0;
        return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
    }

    // Set internal buffer size to socket.
    bool SetSendBufferSize(SOCKET fd, uint32 size)
    {
        return (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
    }

    // Set internal buffer size to socket.
    bool SetRecvBufferSize(SOCKET fd, uint32 size)
    {
        return (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
    }

    // Closes a socket fully.
    void CloseSocket(SOCKET fd)
    {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }

    // Sets reuseaddr
    void ReuseAddr(SOCKET fd)
    {
        uint32 option = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);
    }
}

#endif
