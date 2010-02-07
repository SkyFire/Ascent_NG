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


#ifndef ADT_H
#define ADT_H

#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)
#define UNITSIZE (CHUNKSIZE / 8.0f)

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
class Liquid;
typedef struct {
float x;
float y;
float z;
}svec;

typedef struct {
double x;
double y;
double z;
}vec;

typedef struct{
	vec v[3];

}triangle;	

typedef struct{
float v9[16*8+1][16*8+1];
float v8[16*8][16*8];
}Cell;

typedef struct{
double v9[9][9];
double v8[8][8];
uint16 area_id;
//Liquid *lq;
float waterlevel;
uint8 flag;
}chunk;


class WMO;
class WMOManager;
void fixname(std::string &name);


typedef struct
{
chunk ch[16][16];
}mcell;
class MPQFile;
void LoadMapChunk(MPQFile &,chunk*);
bool LoadWMO(char* filename);
#endif

