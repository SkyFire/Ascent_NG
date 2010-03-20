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
void replace(std::string &str, const char* find, const char* rep, uint32 limit = 0);

volatile long Sync_Add(volatile long* value);
volatile long Sync_Sub(volatile long* value);

SERVER_DECL bool Rand(float chance);
SERVER_DECL bool Rand(uint32 chance);
SERVER_DECL bool Rand(int32 chance);

bool VerifyName(const char * name, size_t nlen, bool limitnames = true);

#ifdef WIN32
uint64 c_GetTimerValue();

uint32 c_GetNanoSeconds(uint64 t1, uint64 t2);
#endif

#endif
