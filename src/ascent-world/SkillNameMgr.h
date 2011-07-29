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

#ifndef SKILL_NAME_MGR_H
#define SKILL_NAME_MGR_H

#include "StdAfx.h"

class SkillNameMgr
{
public:
	char **SkillNames;
	uint32 maxskill;

	SkillNameMgr()
	{
		DBCFile SkillDBC;
		
		if( !SkillDBC.open( "DBC/SkillLine.dbc" ) )
		{
			Log.Error( "SkillNameMgr", "Cannot find file ./DBC/SkillLine.dbc" );
			return;
		}

		//This will become the size of the skill name lookup table
		maxskill = SkillDBC.getRecord(SkillDBC.getRecordCount()-1).getUInt(0);

		//SkillNames = (char **) malloc(maxskill * sizeof(char *));
		SkillNames = new char * [maxskill+1]; //(+1, arrays count from 0.. not 1.)
		memset(SkillNames,0,(maxskill+1) * sizeof(char *));

		for(uint32 i = 0; i < SkillDBC.getRecordCount(); ++i)
		{
			unsigned int SkillID = SkillDBC.getRecord(i).getUInt(0);
			const char *SkillName = SkillDBC.getRecord(i).getString(3);

			SkillNames[SkillID] = new char [strlen(SkillName)+1];
			//When the DBCFile gets cleaned up, so does the record data, so make a copy of it..
			memcpy(SkillNames[SkillID],SkillName,strlen(SkillName)+1);
		}
	}
	~SkillNameMgr()
	{
		for(uint32 i = 0;i<=maxskill;i++)
		{
			if(SkillNames[i] != 0)
				delete[] SkillNames[i];
		}
		delete[] SkillNames;
	}
};
#endif

