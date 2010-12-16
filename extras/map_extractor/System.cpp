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


#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <vector>
#include "dbcfile.h"
#include "adt.h"
#include "mpq_libmpq.h"
#include <io.h>
#include <fcntl.h>
#include <string>
#include <map>
#include <set>
#include <Windows.h>
#include <mmsystem.h>
using namespace std;
#include "filestruct.h"

extern unsigned int iRes;
bool ConvertADT(uint32 x, uint32 y, FILE * out_file, char* name);
void reset();
void CleanCache();
typedef std::vector<MPQArchive*> ArchiveSet;
extern ArchiveSet gOpenArchives;

typedef struct{
	char name[64];
	unsigned int id;
}map_id;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
map_id * map_ids;

uint32 MapCount;

void SetProgressBar(int val, int max, const char* label)
{
    printf("\r");
    for(int i = 0; i < 78; ++i)
        printf(" ");

    printf("\r");
    printf ("[%s]",&label[6]);
    int total = strlen(label) + 12;
    int diff = 78 - total;
    for(int i = 0; i < diff; ++i)
        printf(" ");

    printf("|");
    int barPos = val * 8/*30*/ / max + 1;


    int p;
    for (p = 0; p < barPos; p++) putchar ('=');
    for (; p <8/*30*/; p++) putchar (' ');

    printf ("| %d%%\r", val * 100 / max);
    fflush(stdout);
}

void ClearProgressBar()
{
    for(int p = 0; p<70;p++)
        putchar(' ');
    printf("\r");
}

void SimpleProgressBar(int val, int max)
{
    printf("\r");
    printf("\xba");
    int barPos = val * 60 / max + 1;
    int p;
    for (p = 0; p < barPos; p++) putchar (0xb1);
    for (; p <60; p++) putchar (' ');

    printf ("\xba %d%%\r", val * 100 / max);
}

void ExtractMapsFromMpq()
{
    bool Available_Maps[64][64];
    uint32 Offsets[512][512];
    uint32 TotalTiles = 0;
    uint32 AvailableTiles = 0;
    char output_filename[50];
    map_id * map;
	char mpq_filename[128];
	printf("\nProcessing %u maps...\n\n", MapCount);

    for(uint32 i = 0; i < MapCount; ++i)
    {
        map = &map_ids[i];
        printf("Converting maps for mapid %u [%s]...\n", map->id, map->name);
        // Create the container file
        sprintf(output_filename, "maps\\Map_%u.bin", map->id);
        printf("  Creating output file %s.\n", output_filename);
        FILE * out_file = fopen(output_filename, "wb");
        if(!out_file)
        {
            printf("  Could not create output file!\n");
            return;
        }
		TotalTiles = 0;
		AvailableTiles = 0;
		memset(Available_Maps, 0, sizeof(bool)*64*64);
        printf("  Checking which tiles are extractable...\n");
        sprintf(mpq_filename, "World\\Maps\\%s\\%s.wdt", map->name, map->name);
		MPQFile mf(mpq_filename);
		if(!mf.isEof())
		{
			uint8 * data = new uint8[mf.getSize()];
			mf.read(data, mf.getSize());
			fileVer * version = (fileVer*)data;
			if(version->fcc != 'MVER' || version->ver != 18)
			{
				delete data;
				continue;
			}
			wdt_MPHD * mphd = (wdt_MPHD*)((uint8*) version+version->size+8);
			if(mphd->fcc != 'MPHD')
			{
				delete data;
				continue;
			}
			wdt_MAIN * main = (wdt_MAIN*)((uint8*) mphd+mphd->size+8);
			if(main->fcc != 'MAIN')
			{
				delete data;
				continue;
			}

			// First, check the number of present tiles.
			for(uint32 x = 0; x < 64; ++x)
			{
				for(uint32 y = 0; y < 64; ++y)
				{
					// check if the file exists
					if(!main->adt_list[x][y].exist)
					{
						// file does not exist
						Available_Maps[x][y] = false;
					}
					else
					{
						// file does exist
						Available_Maps[x][y] = true;
						++AvailableTiles;
					}
					++TotalTiles;
				}
			}
			delete data;
		}
		mf.close();

        // Calculate the estimated size.
        float Estimated_Size = 1048576.0f;
        uint32 TilesToExtract = 0;

        for(uint32 x = 0; x < 512; ++x)
        {
            for(uint32 y = 0; y < 512; ++y)
            {
                Offsets[x][y] = 0;
                if(Available_Maps[x/8][y/8] == true)
                    ++TilesToExtract;                
            }
        }
        // Write the offsets to file
        printf("  Writing empty index to the beginning of the file...\n");
        fwrite(Offsets, sizeof(Offsets), 1, out_file);

        Estimated_Size += 4124.0f * TilesToExtract;
        Estimated_Size /= 1024.0f;
        Estimated_Size /= 1024.0f;
        printf("  %u of %u tiles are available. Estimated file size will be %.4fMB.\n", AvailableTiles, TotalTiles, Estimated_Size);
        printf("  %u passes in total have to be performed, it may take a while.\n", TilesToExtract);
        printf("  Extracting data...\n");
        uint32 DoneTiles = 0;
        uint32 start_time = timeGetTime();
        reset();

		if(TilesToExtract)
		{
			// call the extraction function.
			for(uint32 x = 0; x < 512; ++x)
			{
				for(uint32 y = 0; y < 512; ++y)
				{
					// Check if the map is available.
					if(Available_Maps[x/8][y/8])
					{
						uint32 Offset = ftell(out_file);
						if(ConvertADT(x, y, out_file, map->name))
							Offsets[x][y] = Offset;

						++DoneTiles;
					}
				}
				SimpleProgressBar( DoneTiles, TilesToExtract );

				// Clean the cache every 8 cells.
				if(!(x % 8))
					CleanCache();
				if(DoneTiles >= TilesToExtract)
					break;
			}
			ClearProgressBar();
			// clean any leftover cells
			CleanCache();
		}

        printf("  Finished extracting in %ums. Appending header to start of file...\n", timeGetTime() - start_time);
        fseek(out_file, 0, SEEK_SET);
        fwrite(Offsets, sizeof(Offsets), 1, out_file);
        printf("  Closing output file.\n");
        fclose(out_file);

        printf("  Conversion of map %u completed\n\n", map->id);
    }

}

bool ExtractFile( char const* mpq_name, std::string const& filename ) 
{
    FILE *output = fopen(filename.c_str(), "wb");
    if(!output)
    {
        printf("Can't create the output file '%s'\n", filename.c_str());
        return false;
    }
    MPQFile m(mpq_name);
    if(!m.isEof())
        fwrite(m.getPointer(), 1, m.getSize(), output);

    fclose(output);
    return true;
}

int main(int argc, char * arg[])
{
	printf("Ascent map extractor for version 3.3.2 \n");
	printf("============================================================\n\n");

	FILE * tf;
	const char* localeNames[] = { "enUS", "enGB", "deDE", "frFR", "koKR", "zhCN", "zhTW", "esES", "ruRU", 0 };
	int maxPatches = 3;
	int locale = -1;
	char tmp[100];

	tf = fopen("Data/common.MPQ", "r");
	if (!tf)
	{
		printf("Could not find Data/common.MPQ\n");
		return 1;
	}
	fclose(tf);
	new MPQArchive("Data/common.MPQ");

	tf = fopen("Data/common-2.MPQ", "r");
	if (!tf)
	{
		printf("Could not find Data/common-2.MPQ\n");
		return 1;
	}
	fclose(tf);
	new MPQArchive("Data/common-2.MPQ");

	for( size_t i = 0; localeNames[i] != 0; i++ )
	{
		sprintf(tmp, "Data/%s/locale-%s.MPQ", localeNames[i], localeNames[i]);
		tf = fopen(tmp, "r");
		if (!tf)
			continue;
		fclose(tf);
		locale = i;
		new MPQArchive(tmp);
	}

	tf = fopen("Data/expansion.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/expansion.MPQ");
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/expansion-locale-%s.MPQ", localeNames[locale], localeNames[locale]);
			new MPQArchive(tmp);
		}
	}

	tf = fopen("Data/lichking.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/lichking.MPQ");
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/lichking-locale-%s.MPQ", localeNames[locale], localeNames[locale]);
			new MPQArchive(tmp);
		}
	}

	tf = fopen("Data/patch.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/patch.MPQ");
		for(int i = 2; i <= maxPatches; i++)
		{
			sprintf(tmp, "Data/patch-%d.MPQ", i);
			tf = fopen(tmp, "r");
			if (!tf)
				continue;
			fclose(tf);
			new MPQArchive(tmp);
		}
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/patch-%s.MPQ", localeNames[locale], localeNames[locale]);
			tf = fopen(tmp, "r");
			if (tf)
			{
				fclose(tf);
				new MPQArchive(tmp);
				for(int i = 2; i <= maxPatches; i++)
				{
					sprintf(tmp, "Data/%s/patch-%s-%d.MPQ", localeNames[locale], localeNames[locale], i);
					tf = fopen(tmp, "r");
					if (!tf)
						continue;
					fclose(tf);
					new MPQArchive(tmp);
				}
			}
		}
	}

	printf("\nExtracting DBC Files: Identifying files...\n");

	std::set<std::string> dbcFiles;
	int itc = 0;
	for(std::vector<MPQArchive*>::iterator it = gOpenArchives.begin(); it != gOpenArchives.end(); ++it)
	{
		std::vector<std::string> files;
		files = (*it)->GetFileList();
        for (vector<string>::iterator iter = files.begin(); iter != files.end(); ++iter)
            if (iter->rfind(".dbc") == iter->length() - strlen(".dbc"))
                    dbcFiles.insert(*iter);
		SimpleProgressBar(++itc, gOpenArchives.size());
	}
	CleanCache();
	ClearProgressBar();
	printf("Extracting...\n");

    CreateDirectory("dbc", NULL);

    // extract DBCs
    int count = 0;
    for (set<string>::iterator iter = dbcFiles.begin(); iter != dbcFiles.end(); ++iter)
    {
        string filename = "DBC\\";
        filename += (iter->c_str() + strlen("DBFilesClient\\"));

        if(ExtractFile(iter->c_str(), filename))
            ++count;
		SimpleProgressBar(count, dbcFiles.size());
    }
	CleanCache();
	ClearProgressBar();
    printf("Extracted %u DBC files\n\n", count);

	//map.dbc
	DBCFile * dbc= new DBCFile("DBFilesClient\\Map.dbc");
	dbc->open();

	MapCount=dbc->getRecordCount ();
	map_ids=new map_id[MapCount];
	for(unsigned int x = 0; x < MapCount; x++)
	{
		map_ids[x].id=dbc->getRecord (x).getUInt(0);
		strcpy(map_ids[x].name,dbc->getRecord(x).getString(1));
	}
	delete dbc;

	CreateDirectory("maps", NULL);
	ExtractMapsFromMpq();
	delete [] map_ids;

	return 0; // Exit The Program
}
