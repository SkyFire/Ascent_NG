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

#ifndef __NETWORK_HANDLERS_H
#define __NETWORK_HANDLERS_H

int voicechat_ascent_listen_socket_read_handler(network_socket *s, int act);
int voicechat_ascent_socket_read_handler(network_socket *s, int act);
int voicechat_client_socket_read_handler(network_socket *s, int act);

int voicechat_init_clientsocket();
int voicechat_init_serversocket();
int get_server_count();

#endif
