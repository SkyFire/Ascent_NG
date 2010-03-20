/*
 * Scripts for Ascent MMORPG Server
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

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/EasyFunctions.h"


bool BaronVardusAllowSpawn = true;

class WantedBaronVardus : public QuestScript 
{ 
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		if(BaronVardusAllowSpawn == true)
		{
			uint32 rand = RandomUInt(3);
			switch(rand)
			{
			case 0: mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 692.64f, -904.74f, 157.79f, 0, true, false, 0, 0)->Despawn(1800000, 0);
			case 1: mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 939.0f, -852.46f, 114.644f, 0, true, false, 0, 0)->Despawn(1800000, 0);
			case 2: mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 1184.07f, -553.43f, 71.3346f, 0, true, false, 0, 0)->Despawn(1800000, 0);
			case 3: mTarget->GetMapMgr()->GetInterface()->SpawnCreature(2306, 1001.20f, -793.93f, 108.65f, 0, true, false, 0, 0)->Despawn(1800000, 0);
			}
			BaronVardusAllowSpawn = false;
		}
	}
};

class Baron_Vardus : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Baron_Vardus);
	Baron_Vardus(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnDied(Unit* mKiller) 
	{
		BaronVardusAllowSpawn = true;
	}
};


void SetupHillsbradFoothills(ScriptMgr * mgr)
{
	mgr->register_creature_script(2306, &Baron_Vardus::Create);
	mgr->register_quest_script(566, CREATE_QUESTSCRIPT(WantedBaronVardus));
}
