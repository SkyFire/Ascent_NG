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

#include "Common.h"
#include "PacketLog.h"
#include "Config/ConfigEnv.h"

#include <ctype.h>


createFileSingleton( PacketLog );

PacketLog::PacketLog()
{
    // clear realm logfile
    if (sConfig.GetBoolDefault("LogRealm", false))
    {
        FILE *pFile = fopen("realm.log", "w+");
        fclose(pFile);
    }
    // clear world logfile
    if (sConfig.GetBoolDefault("LogWorld", false))
    {
        FILE *pFile = fopen("world.log", "w+");
        fclose(pFile);
    }
}

PacketLog::~PacketLog()
{
}


char PacketLog::makehexchar(int i) {
    return (i<=9) ? '0'+i : 'A'+(i-10);
}

int PacketLog::hextoint(char c) {
    c = toupper(c);
    return (c > '9' ? c - 'A' + 10 : c - '0');
}


void PacketLog::HexDump(const unsigned char* data, size_t length, const char* file) 
{
    FILE *pFile;
    pFile = fopen(file, "a");

    const int char_offset = 16*3 + 2;
    const int line_size = 16*3 + 16 + 3;
    char line[line_size+1];

    fprintf(pFile,"OFFSET  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789ABCDEF\n");
    fprintf(pFile,"--------------------------------------------------------------------------\n");

    line[char_offset - 1] = ' ';
    line[char_offset - 2] = ' ';

    for (size_t i=0; i<length; ) {
        int bi=0;
        int ci=0;

        int start_i = i;

        for (int line_i=0; i < length && line_i < 16; i++, line_i++) {
            line[bi++] = makehexchar(*data>>4);
            line[bi++] = makehexchar(*data & 0x0f);
            line[bi++] = ' ';
            line[char_offset+(ci++)]=(isprint(*data) ? *data : '.');
            data++;
        }

        while (bi<16*3) {
            line[bi++]=' ';
        }

        line[char_offset+(ci++)]='\n';
        line[char_offset+ci]=0;

        fprintf(pFile,"%06X  %s", start_i, line);
    }
    fprintf(pFile, "\n\n");
    fclose(pFile);
}

void PacketLog::HexDump(const char *data, size_t length, const char* file) {
    HexDump((unsigned char *)data, length, file);
}

void PacketLog::HexDumpStr(const char *msg, const char *data, size_t len, const char* file)
{
    FILE *pFile;
    pFile = fopen(file, "a");
    fprintf(pFile,"%s\n", msg);
    fclose(pFile);

    HexDump(data, len, file);
}


void PacketLog::RealmHexDump(RealmPacket* data, uint32 socket, bool direction)
{
    if (!sConfig.GetBoolDefault("LogRealm", false))
        return;

    FILE *pFile;
    pFile = fopen("realm.log", "a");

    uint16 len = data->size() + 2;
    uint8 opcode = data->GetOpcode();
    if (direction)
        fprintf(pFile, "SERVER:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.2X\nDATA:\n", socket, len, opcode);
    else
        fprintf(pFile, "CLIENT:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.2X\nDATA:\n", socket, len, opcode);

    fclose(pFile);
    HexDump((char *)data->contents(), data->size(), "realm.log");

}
void PacketLog::WorldHexDump(WorldPacket* data, uint32 socket, bool direction)
{
    if (!sConfig.GetBoolDefault("LogWorld", false))
        return;

    FILE *pFile;
    pFile = fopen("world.log", "a");

    uint16 len = data->size();// + 4;
    uint16 opcode = data->GetOpcode();
    if (direction)
        fprintf(pFile, "SERVER:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.4X\nDATA:\n", socket, len, opcode);
    else
        fprintf(pFile, "CLIENT:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.4X\nDATA:\n", socket, len, opcode);

    fclose(pFile);
    HexDump((char *)data->contents(), data->size(), "world.log");

}