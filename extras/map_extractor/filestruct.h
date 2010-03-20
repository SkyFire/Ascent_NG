#ifndef FILESTRUCT_H_
#define FILESTRUCT_H_

struct fileVer
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
    uint32 ver;
};

struct wdt_MPHD{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };

    uint32 size;

    uint32 data1;
    uint32 data2;
    uint32 data3;
    uint32 data4;
    uint32 data5;
    uint32 data6;
    uint32 data7;
    uint32 data8;
};

struct wdt_MAIN{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };

    uint32 size;

    struct adtData{
        uint32 exist;
        uint32 data1;
    } adt_list[64][64];
};

struct adt_MCIN
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;

    struct adt_CELLS{
        uint32 offsMCNK;
        uint32 size;
        uint32 flags;
        uint32 asyncId;
    } cells[16][16];
};

struct adt_MHDR
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;

    uint32 pad;
    uint32 offsMCIN;           // MCIN
    uint32 offsTex;	           // MTEX
    uint32 offsModels;	       // MMDX	
    uint32 offsModelsIds;	   // MMID	
    uint32 offsMapObejcts;	   // MWMO	
    uint32 offsMapObejctsIds;  // MWID		
    uint32 offsDoodsDef;       // MDDF	
    uint32 offsObjectsDef;     // MODF
    uint32 offsMFBO;           // MFBO
    uint32 offsMH2O;           // MH2O
    uint32 data1;
    uint32 data2;
    uint32 data3;
    uint32 data4;
    uint32 data5;
};

struct adt_MCVT
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
    float height_map[145];
};

struct adt_MCLQ
{
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
    uint32 size;
    float height1;
    float height2;
    struct liquid_data{
        uint32 light;
        float  height;
    } liquid[9][9];
    
    // 1<<0 - ochen
    // 1<<1 - lava/slime
    // 1<<2 - water
    // 1<<6 - all water
    // 1<<7 - dark water
    // == 0x0F - not show liquid
    uint8 flags[8][8];
    uint8 data[84];
};

struct MapChunkHeader {
    union{
        uint32 fcc;
        char   fcc_txt[4];
    };
	uint32 size;
	uint32 flags;
	uint32 ix;
	uint32 iy;
	uint32 nLayers;
	uint32 nDoodadRefs;
	uint32 ofsHeight;
	uint32 ofsNormal;
	uint32 ofsLayer;
	uint32 ofsRefs;
	uint32 ofsAlpha;
	uint32 sizeAlpha;
	uint32 ofsShadow;
	uint32 sizeShadow;
	uint32 areaid;
	uint32 nMapObjRefs;
	uint32 holes;
	uint16 s1;
	uint16 s2;
	uint32 d1;
	uint32 d2;
	uint32 d3;
	uint32 predTex;
	uint32 nEffectDoodad;
	uint32 ofsSndEmitters;
	uint32 nSndEmitters;
	uint32 ofsLiquid;
	uint32 sizeLiquid;
	float  zpos;
	float  xpos;
	float  ypos;
	uint32 textureId;
	uint32 props;
	uint32 effectId;
};

#endif
