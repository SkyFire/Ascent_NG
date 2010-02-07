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


#ifndef MODEL_H
#define MODEL_H

#include "vec3d.h"
#include "mpq.h"
#include "modelheaders.h"
//#include "quaternion.h"
#include <vector>

class Model;
class WMOInstance;

Vec3D fixCoordSystem(Vec3D v);




class Model
{ 
public:
	ModelHeader header;
	ModelAnimation *anims;
	int *globalSequences;
	
public:
	bool animGeometry,animTextures,animBones;
	bool animated;

	bool isAnimated(MPQFile &f);
	ModelVertex *origVertices;
	Vec3D *vertices, *normals;
	uint16 *indices;
	size_t nIndices;
	
	bool open();
	bool ConvertToVMAPModel(char * outfilename);

public:

	bool ok;
	bool ind;

	float rad;
	float trans;
	bool animcalc;
	int anim, animtime;

	Model(std::string &filename);
	~Model();
	
private:
	std::string filename;
	char outfilename;
};

class ModelInstance 
{
public:
	Model *model;

	int id;

	Vec3D pos, rot;
	unsigned int d1, scale;

	float frot,w,sc;

	int light;
	Vec3D ldir;
	Vec3D lcol;

	ModelInstance() {}
	ModelInstance(MPQFile &f,const char* ModelInstName,const char*MapName, FILE *pDirfile);

};

#endif
