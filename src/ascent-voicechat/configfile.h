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

#ifndef __CONFIGFILE_H
#define __CONFIGFILE_H

#include "common.h"

typedef struct
{
	int tcp_listen_port;
	char * tcp_listen_host;

	int udp_listen_port;
	char * udp_listen_host;

	int log_loglevel;
	char * log_logfile;

	int daemonize;
	// max channels maybe?
} server_config;

// global accessable server config struct
extern server_config g_serverConfig;

// load/parse/reload the config file
int configfile_load_config(const char * filename);
int configfile_init();
int configfile_parsecmdoverride(int argc, char* argv[]);

#endif
