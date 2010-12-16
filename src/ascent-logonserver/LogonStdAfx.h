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

#pragma once

#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include <fstream>

#include "Common.h"
#include <Network/Network.h>

#include "../ascent-shared/Log.h"
#include "../ascent-shared/Util.h"
#include "../ascent-shared/ByteBuffer.h"
#include "../ascent-shared/Config/ConfigEnv.h"
#include <zlib.h>

#include "../ascent-shared/Database/DatabaseEnv.h"
#include "../ascent-shared/Database/DBCStores.h"
#include "../ascent-shared/Database/dbcfile.h"

#include "../ascent-shared/Auth/BigNumber.h"
#include "../ascent-shared/Auth/Sha1.h"
#include "../ascent-shared/Auth/WowCrypt.h"
#include "../ascent-shared/CrashHandler.h"

#include "LogonOpcodes.h"
#include "../ascent-logonserver/Main.h"
#include "../ascent-world/NameTables.h"
#include "AccountCache.h"
#include "PeriodicFunctionCall_Thread.h"
#include "../ascent-logonserver/AutoPatcher.h"
#include "../ascent-logonserver/AuthSocket.h"
#include "../ascent-logonserver/AuthStructs.h"
#include "../ascent-logonserver/LogonOpcodes.h"
#include "../ascent-logonserver/LogonCommServer.h"
#include "../ascent-logonserver/LogonConsole.h"
#include "../ascent-shared/WorldPacket.h"

// database decl
extern Database* sLogonSQL;
