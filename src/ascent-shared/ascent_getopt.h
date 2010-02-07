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

#ifndef _ASCENT_GETOPT_H
#define _ASCENT_GETOPT_H

/* getopt() wrapper */
#define ascent_no_argument            0
#define ascent_required_argument      1
#define ascent_optional_argument      2
struct ascent_option
{
	const char *name;
	int has_arg;
	int *flag;
	int val;
};
extern char ascent_optarg[514];
int ascent_getopt_long_only (int ___argc, char *const *___argv, const char *__shortopts, const struct ascent_option *__longopts, int *__longind);

#endif
