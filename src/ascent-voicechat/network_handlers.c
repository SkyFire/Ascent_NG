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

#include "common.h"
#include "network.h"
#include "log.h"
#include "network_handlers.h"
#include "configfile.h"

// before anyone yells at me about not cleaning up the socket() here, that is because
// 1: close() under posix, closesocket() under windows
// 2: the app is gonna terminate anyway afterwards so theres no point

int voicechat_init_clientsocket()
{
	int fd;
	int rv;
	struct sockaddr_in addr;
	network_socket * s;

	// convert the human-readable ip into byte form
	memset(&addr, 0, sizeof(struct sockaddr_in));
	rv = vc_inet_pton(AF_INET, g_serverConfig.udp_listen_host, &addr.sin_addr);

	if( rv <= 0 )
	{
		log_write(ERROR, "FATAL: UDP listen host '%s' was non-parsable.", g_serverConfig.udp_listen_host);
		return -1;
	}

	// create a socket
	fd = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( fd < 0 )
	{
		log_write(ERROR, "FATAL: socket() for udp socket returned an error. %d.", fd);
		return -1;
	}

	// assign the port
	addr.sin_family = AF_INET;
	addr.sin_port = htons( (unsigned short)g_serverConfig.udp_listen_port );

	// bind it
	rv = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if( rv < 0 )
	{
		log_write(ERROR, "FATAL: UDP socket was unable to bind to specified host/port.");
		return -1;
	}

	// allocate the network_socket structure
	s = (network_socket*)vc_malloc(sizeof(network_socket));

	// initialize structure
	network_init_socket(s, fd, 0);

	// set the handler
	s->event_handler = voicechat_client_socket_read_handler;
	s->write_handler = NULL;

	// add it to the network map (platform-specific)
	network_add_socket(s);
	
	// thats it.
	return 0;
}

int voicechat_init_serversocket()
{
	int fd;
	int rv;
	struct sockaddr_in addr;
	network_socket * s;

	// convert the human-readable ip into byte form
	memset(&addr, 0, sizeof(struct sockaddr_in));
	rv = vc_inet_pton(AF_INET, g_serverConfig.tcp_listen_host, &addr.sin_addr);

	if( rv <= 0 )
	{
		log_write(ERROR, "FATAL: TCP listen host '%s' was non-parsable.", g_serverConfig.tcp_listen_host);
		return -1;
	}

	// create a socket
	fd = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( fd < 0 )
	{
		log_write(ERROR, "FATAL: socket() for tcp socket returned an error. %d.", fd);
		return -1;
	}

	// assign the port
	addr.sin_family = AF_INET;
	addr.sin_port = htons( (unsigned short)g_serverConfig.tcp_listen_port );

	// bind it
	rv = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if( rv < 0 )
	{
		log_write(ERROR, "FATAL: TCP socket was unable to bind to specified host/port.");
		return -1;
	}

	// listen.
	if( listen(fd, 2) < 0 )
	{
		log_write(ERROR, "FATAL: TCP socket could not listen.");
		return -1;
	}

	// allocate the network_socket structure
	s = (network_socket*)vc_malloc(sizeof(network_socket));

	// initialize structure
	network_init_socket(s, fd, 0);

	// set the handlers
	s->event_handler = voicechat_ascent_listen_socket_read_handler;
	s->write_handler = default_tcp_write_handler;

	// add it to the network map (platform-specific)
	network_add_socket(s);

	// thats it.
	return 0;
}




