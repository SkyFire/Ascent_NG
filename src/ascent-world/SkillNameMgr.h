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

