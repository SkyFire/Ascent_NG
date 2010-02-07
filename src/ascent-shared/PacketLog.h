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

#ifndef WOWSERVER_PACKETLOG_H
#define WOWSERVER_PACKETLOG_H

#include "Common.h"
#include "Singleton.h"
#include "RealmPacket.h"
#include "WorldPacket.h"

class PacketLog : public Singleton< PacketLog > {
public:
    PacketLog();
    ~PacketLog();
    //utility functions
    int hextoint(char c);
    char makehexchar(int i);
    //general log functions
    void HexDump(const unsigned char* data, size_t length, const char* file);
    void HexDump(const char *data, size_t length, const char* file);
    void HexDumpStr(const char *msg, const char *data, size_t len, const char* file);
    //realm packet log
    void RealmHexDump(RealmPacket * data, uint32 socket, bool direction);
    //world packet log
    void WorldHexDump(WorldPacket * data, uint32 socket, bool direction);
};

#define sPacketLog PacketLog::getSingleton()

#endif