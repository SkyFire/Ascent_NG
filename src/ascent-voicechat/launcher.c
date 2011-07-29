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

#include "log.h"
#include "configfile.h"
#include "mutex.h"
#include "linkedlist.h"
#include "network.h"
#include "network_handlers.h"
#include "voice_channel.h"
#include <signal.h>

int running = 1;
void signal_handler(int s)
{
	running = 0;
}

void status_updater_thread(void* unused)
{
	float bandwidth_in = 0.0f;
	float bandwidth_out = 0.0f;
	int channels = 0;
	int servers = 0;

	while( running )
	{
		network_get_bandwidth_statistics( &bandwidth_in, &bandwidth_out );
		channels = voice_get_channel_count();
		servers = get_server_count();

		// remember these are in bytes, so convert to kb
		bandwidth_in /= 1000.0f;
		bandwidth_out /= 1000.0f;

		// dump to console
		printf("\r| %02u servers | %05u channels | in: %04.2f KB/s | out: %04.2f KB/s |",
			servers, channels, bandwidth_in, bandwidth_out );

		vc_sleep(1000);
	}
}

int main(int argc, char* argv[])
{
	configfile_init();

	if( configfile_parsecmdoverride(argc, argv) < 0 )
		return -1;

	log_open();

	// read config file
	if( configfile_load_config("ascent-voicechat.conf") < 0 )
	{
		// this needs to be cmd-line overrideable
		log_write(ERROR, "FATAL: Could not open a valid config file. Exiting.");
		return -1;
	}

	printf("Ascent Voicechat Server\n");
	printf("Starting...\n");

	if( network_init() < 0 )
	{
		log_write(ERROR, "FATAL: Network failed initialization.");
		return -1;
	}

	printf("Binding sockets...\n");
	if( voicechat_init_clientsocket() < 0 || voicechat_init_serversocket() < 0 )
	{
		log_write(ERROR, "FATAL: Could not bind sockets.");
		return -1;
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
#ifdef WIN32	
	signal(SIGBREAK, signal_handler);
#endif

	printf("I/O Loop running...\n");
	start_thread(status_updater_thread, NULL);
	while(running)
	{
		network_io_poll();
	}

	printf("Shutting down...\n");
	network_shutdown();
	log_close();

	return 0;
}

