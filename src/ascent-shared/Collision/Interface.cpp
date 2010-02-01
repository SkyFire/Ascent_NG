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

#include "vmap/IVMapManager.h"
#include "vmap/VMapManager.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "LocationVector.h"

VMAP::VMapManager* vmgr;

void * collision_init()
{
	vmgr = new VMAP::VMapManager();
	return vmgr;
}

void collision_shutdown()
{
	vmgr->unloadMap(0);
	vmgr->unloadMap(1);
	vmgr->unloadMap(530);
	vmgr->unloadMap(571);
}

