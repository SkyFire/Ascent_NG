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


#ifdef WIN32
#include <windows.h>
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include <set>

#include "adt.h"
#include "mpq_libmpq.h"
#include "filestruct.h"

//#include <windows.h>
unsigned int iRes = 256;
extern uint16*areas;

#define SMALL_MAPFILES
#define PACK_MAPS
#define MAP_RESOLUTION 256

#define TilesCount 64
#define TileSize 533.33333f
#define _minY (-TilesCount*TileSize/2)
#define _minX (-TilesCount*TileSize/2)

#define _maxY (TilesCount*TileSize/2)
#define _maxX (TilesCount*TileSize/2)

#define CellsPerTile 8
#define _cellSize (TileSize/CellsPerTile)
#define _sizeX (TilesCount*CellsPerTile)
#define _sizeY (TilesCount*CellsPerTile)

//#pragma pack(push, 1)

typedef struct
{
    uint16    AreaID[16/CellsPerTile][16/CellsPerTile];
    uint8    LiquidType[16/CellsPerTile][16/CellsPerTile];
    float    LiquidLevel[16/CellsPerTile][16/CellsPerTile];
    float    Z[MAP_RESOLUTION/CellsPerTile][MAP_RESOLUTION/CellsPerTile];
}MapCellInfo; 


///This is temp. method.
typedef struct
{
    uint16    AreaID[16][16];
    uint8    LiquidType[16][16];
    float    LiquidLevel[16][16];
    float    Z[MAP_RESOLUTION][MAP_RESOLUTION];
}MapTileInfo; 

typedef struct
{
    float z[256][256];
}zstore;
//#pragma pack(pop)
vec wmoc;

Cell * cell;
uint32 wmo_count;
mcell *mcells;
zstore * cz;

mcell * mcell_cache[64][64];
zstore * cell_cache[64][64];

void reset()
{
    for(uint32 i = 0; i < 64; ++i)
    {
        for(uint32 j = 0; j < 64; ++j)
        {
            mcell_cache[i][j] = 0;
            cell_cache[i][j] = 0;
        }
    }
}
void CleanCache()
{
    for(uint32 i = 0; i < 64; ++i)
    {
        for(uint32 j = 0; j < 64; ++j)
        {
            if(cell_cache[i][j] != 0)
            {
                delete cell_cache[i][j];
                cell_cache[i][j] = 0;
            }

            if(mcell_cache[i][j] != 0)
            {
                delete mcell_cache[i][j];
                mcell_cache[i][j] = 0;
            }

        }
    }
}

bool LoadADT(char* filename)
{
	MPQFile mf(filename);

	if(mf.isEof())
		return false;

	mcells = new mcell;

	wmoc.x = 65*TILESIZE;
	wmoc.z = 65*TILESIZE;

	wmo_count = 0;
	bool found = false;

	uint8 * data = new uint8[mf.getSize()];
	mf.read(data, mf.getSize());
	mf.close ();

	fileVer * ver = (fileVer*)data;
	if(ver->fcc != 'MVER' || ver->ver != 18)
	{
		delete data;
		return false;
	}
	adt_MHDR * mhdr = (adt_MHDR*)(data+8+ver->size);
	if(mhdr->fcc != 'MHDR')
	{
		delete data;
		return false;
	}
	if(mhdr->size != sizeof(adt_MHDR)-8)
	{
		delete data;
		return false;
	}
	if(!mhdr->offsMCIN)
	{
		delete data;
		return false;
	}
	adt_MCIN * mcin = (adt_MCIN*)((uint8 *)&mhdr->pad + mhdr->offsMCIN);
	if(mcin->fcc != 'MCIN')
	{
		delete data;
		return false;
	}

	for (int j=0; j<16; j++) 
		for (int i=0; i<16; i++) 
		{
			LoadMapChunk ((MapChunkHeader*) ((uint8*)mcin + mcin->cells[j][i].offsMCNK - 84) , &(mcells->ch [i][j]));
		}

	delete data;
	return true;
}

inline
void LoadMapChunk(MapChunkHeader * header, chunk*_chunk)
{
	if(header->fcc != 'MCNK')
		return;

	_chunk->area_id = header->areaid ;
	_chunk->flag = 0;

	float xbase = header->xpos;
	float ybase = header->ypos;
	float zbase = header->zpos;
	zbase = TILESIZE*32-zbase;
	xbase = TILESIZE*32-xbase;
	if(wmoc.x >xbase)wmoc.x = xbase;
	if(wmoc.z >zbase)wmoc.z = zbase;
	int chunkflags = header->flags;
	float zmin = 999999999.0f;
	float zmax = -999999999.0f;
	//must be there, bl!zz uses some crazy format

	// Get Map Height
	if(header->ofsHeight)
	{
		adt_MCVT * mcvt = (adt_MCVT*) ((uint8*)header+header->ofsHeight);
		if(mcvt->fcc == 'MCVT')
		{
			int x = 0;
			for (int j=0; j<17; j++)
				for (int i=0; i<((j%2)?8:9); i++) 
					{
						float z = mcvt->height_map[x++]+ybase;
						if (j%2)
							_chunk->v8[i][j/2] = z;
						else
							_chunk->v9[i][j/2] = z;

						if(z > zmax) zmax = z;
					}
		}
	}
	
	// Water / Liquid
	if(header->ofsLiquid)
	{
		adt_MCLQ * mclq = (adt_MCLQ*) ((uint8*)header+header->ofsLiquid);
		if(mclq->fcc == 'MCLQ')
		{
			_chunk->waterlevel = mclq->height1;
			if(!_chunk->flag)
				_chunk->flag = chunkflags;
		}
	}
}



double solve (vec *v,vec *p)
{
double a = v[0].y *(v[1].z - v[2].z) + v[1].y *(v[2].z - v[0].z) + v[2].y *(v[0].z - v[1].z);
double b = v[0].z *(v[1].x - v[2].x) + v[1].z *(v[2].x - v[0].x) + v[2].z *(v[0].x - v[1].x);
double c = v[0].x *(v[1].y - v[2].y) + v[1].x *(v[2].y - v[0].y) + v[2].x *(v[0].y - v[1].y);
double d = v[0].x *(v[1].y*v[2].z - v[2].y*v[1].z) + v[1].x* (v[2].y*v[0].z - v[0].y*v[2].z) + v[2].x* (v[0].y*v[1].z - v[1].y*v[0].z);
//-d

//plane equation ax+by+cz+d=0
return ((a*p->x+c*p->z-d)/b);
}


inline
double GetZ(double x,double z)
{

	vec v[3];
	vec p;
	
	bool inWMO=false;
	
	if(!inWMO)
	{
		//find out quadrant
		int xc=(int)(x/UNITSIZE);
		int zc=(int)(z/UNITSIZE);
		if(xc>127)xc=127;
		if(zc>127)zc=127;

		double lx=x-xc*UNITSIZE;
		double lz=z-zc*UNITSIZE;
		p.x=lx;
		p.z=lz;
	

		v[0].x=UNITSIZE/2;
		v[0].y =cell->v8[xc][zc];
		v[0].z=UNITSIZE/2;

	
		if(lx>lz)
		{
				v[1].x=UNITSIZE;
				v[1].y =cell->v9[xc+1][zc];
				v[1].z=0;
		}else
		{
				v[1].x=0.0;
				v[1].y =cell->v9[xc][zc+1];
				v[1].z=UNITSIZE;
		}

		if(lz>UNITSIZE-lx)
		{
				v[2].x=UNITSIZE;
				v[2].y =cell->v9[xc+1][zc+1];
				v[2].z=UNITSIZE;

		}else
		{
				v[2].x=0;
				v[2].y=cell->v9[xc][zc];
				v[2].z=0;
		}

		return -solve(v,&p);
	}
	
}

/*inline
void TransformWaterData()
{
    cell= new Cell;

    for(int x=0;x<128;x++)
        for(int y=0;y<128;y++)
            cell->v9[x][y] = mcells->ch[x/8][y/8].waterlevel[x%8][y%8];

    //and the last 1
    cell->v9[128][128] = mcells->ch[15][15].waterlevel[8][8];
}*/

inline
void TransformData()
{
	cell= new Cell;

	for(int x=0;x<128;x++)
	{
		for(int y=0;y<128;y++)
		{
            cell->v8[x][y] = (float)mcells->ch[x/8][y/8].v8[x%8][y%8];
            cell->v9[x][y] = (float)mcells->ch[x/8][y/8].v9[x%8][y%8];
		}

		//extra 1 point on bounds
		cell->v9[x][128] = (float)mcells->ch[x/8][15].v9[x%8][8];
		//x==y
		cell->v9[128][x] = (float)mcells->ch[15][x/8].v9[8][x%8];

	}


	//and the last 1
	cell->v9[128][128] = (float)mcells->ch[15][15].v9[8][8];
}

bool ConvertADT(uint32 x, uint32 y, FILE * out_file, char* name)
{
    // Figure out what rock we're under :P
    uint32 tile_x = x / CellsPerTile;
    uint32 tile_y = y / CellsPerTile;

    // For some odd reason, this stuff is reversed.. who knows why..
    char mpq_filename[256];
    sprintf(mpq_filename, "World\\Maps\\%s\\%s_%u_%u.adt", name, name, tile_y, tile_x);
    
    // See if we have it cached first.
    if(mcell_cache[tile_y][tile_x] == 0 && cell_cache[tile_y][tile_x] == 0)
    {
        if(!LoadADT(mpq_filename))
            return false;

        TransformData();
        mcell_cache[tile_y][tile_x] = mcells;

        // Cache z information.
        cz = new zstore;
        for(uint32 cx = 0; cx < MAP_RESOLUTION; ++cx)
        {
            for(uint32 cy = 0; cy < MAP_RESOLUTION; ++cy)
            {
                float cfz=(float)GetZ(
                    (((double)(cy))*TILESIZE)/((double)(iRes-1)),
                    (((double)(cx))*TILESIZE)/((double)(iRes-1)));
                cz->z[cx][cy] = cfz;
            }
        }

        cell_cache[tile_y][tile_x] = cz;
        delete cell;
    }
    else
    {
        mcells = mcell_cache[tile_y][tile_x];
        cz = cell_cache[tile_y][tile_x];
    }

    // This is our output data.
    MapCellInfo out;

    // Write out the area/water information.
    for(uint32 xc=(x%CellsPerTile)*16/CellsPerTile;xc<(x%CellsPerTile)*16/CellsPerTile+16/CellsPerTile;xc++)
    {
        for(uint32 yc=(y%CellsPerTile)*16/CellsPerTile;yc<(y%CellsPerTile)*16/CellsPerTile+16/CellsPerTile;yc++)
        {
            uint32 lx=xc%(16/CellsPerTile);
            uint32 ly=yc%(16/CellsPerTile);
            out.AreaID[lx][ly] = mcells->ch[yc][xc].area_id;
            out.LiquidType[lx][ly] = mcells->ch[yc][xc].flag;
            out.LiquidLevel[ly][ly] = mcells->ch[yc][xc].waterlevel;
        }
    }


    // Convert it into antrix cell format.
    for(uint32 cx=(x%CellsPerTile)*MAP_RESOLUTION/CellsPerTile;cx<(x%CellsPerTile)*MAP_RESOLUTION/CellsPerTile+MAP_RESOLUTION/CellsPerTile;cx++)
    {
        for(uint32 cy=(y%CellsPerTile)*MAP_RESOLUTION/CellsPerTile;cy<(y%CellsPerTile)*MAP_RESOLUTION/CellsPerTile+MAP_RESOLUTION/CellsPerTile;cy++)
        {
            uint32 lx=cx%(MAP_RESOLUTION/CellsPerTile);
            uint32 ly=cy%(MAP_RESOLUTION/CellsPerTile);
            out.Z[lx][ly] = cz->z[cx][cy];
        }
    }

    // Write out to file.
    fwrite(&out, sizeof(out), 1, out_file);
	return true;
}

