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

class SunkenTreasure : public QuestScript 
{ 
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 2768);
		if(creat == NULL)
			return;
		creat->m_escorter = mTarget;   
		creat->GetAIInterface()->setMoveType(11);
		creat->GetAIInterface()->StopMovement(3000);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Defens Me!");
		creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

		sEAS.CreateCustomWaypointMap(creat);
		sEAS.WaypointCreate(creat,-2078.054443f, -2091.207764f, 9.526212f, 4.770276f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2076.626465f, -2109.960449f, 14.320494f, 4.821321f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2072.851074f, -2123.574219f, 18.482662f, 5.623996f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2063.878906f, -2132.617920f, 21.430487f, 5.512474f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2051.495117f, -2145.205811f, 20.500065f, 5.481060f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2051.495117f, -2145.205811f, 20.500065f, 5.481060f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2063.878906f, -2132.617920f, 21.430487f, 5.512474f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2072.851074f, -2123.574219f, 18.482662f, 5.623996f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2076.626465f, -2109.960449f, 14.320494f, 4.821321f, 0, 256, 4049);
		sEAS.WaypointCreate(creat,-2078.054443f, -2091.207764f, 9.526212f, 4.770276f, 0, 256, 4049);
		sEAS.EnableWaypoints(creat);
	}
};

class Professor_Phizzlethorpe : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Professor_Phizzlethorpe);
	Professor_Phizzlethorpe(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 15)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, I found the fact that, it searched");
			_unit->Despawn(5000,1000);
			sEAS.DeleteWaypoints(_unit);
			if(_unit->m_escorter == NULL)
				return;
			Player* plr = _unit->m_escorter;
			_unit->m_escorter = NULL;
			plr->GetQuestLogForEntry(665)->SendQuestComplete();
		}
	}
};

void SetupArathiHighlands(ScriptMgr * mgr)
{
	mgr->register_creature_script(2768, &Professor_Phizzlethorpe::Create);
	/*mgr->register_quest_script(665, CREATE_QUESTSCRIPT(SunkenTreasure));*/
}
