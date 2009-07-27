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

#ifndef _WORKER_OPCODES_H
#define _WORKER_OPCODES_H

enum WorkerServerOpcodes
{
	ISMSG_AUTH_REQUEST,
	ICMSG_AUTH_REPLY,
	ISMSG_AUTH_RESULT,
	ICMSG_REGISTER_WORKER,
	ISMSG_REGISTER_RESULT,
	ISMSG_CREATE_INSTANCE,
	ISMSG_DESTROY_INSTANCE,
	ISMSG_PLAYER_LOGIN,
	ISMSG_PLAYER_INFO,
	ICMSG_PLAYER_INFO,
	ISMSG_WOW_PACKET,
	ICMSG_WOW_PACKET,
	ICMSG_WHISPER,
    ICMSG_CHAT,
	ISMSG_WHISPER,
	ISMSG_CHAT,
	ISMSG_PACKED_PLAYER_INFO,
	ICMSG_PLAYER_LOGIN_RESULT,
	ICMSG_PLAYER_LOGOUT,
	ISMSG_DESTROY_PLAYER_INFO,
	ICMSG_TELEPORT_REQUEST,
	ISMSG_TELEPORT_RESULT,
	ICMSG_PLAYER_CHANGE_SERVER_INFO,
	ICMSG_ERROR_HANDLER,
	ISMSG_SESSION_REMOVED,
	ICMSG_SWITCH_SERVER,
	ICMSG_SAVE_ALL_PLAYERS,
	ISMSG_SAVE_ALL_PLAYERS,
	ICMSG_TRANSPORTER_MAP_CHANGE,
	ISMSG_TRANSPORTER_MAP_CHANGE,
	ICMSG_PLAYER_TELEPORT,
	ISMSG_PLAYER_TELEPORT,
	ICMSG_CREATE_PLAYER,
	ISMSG_CREATE_PLAYER,

	IMSG_NUM_TYPES,
};

#endif		// _WORKER_OPCODES_H


