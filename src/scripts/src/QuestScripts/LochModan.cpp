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

class ProtectingtheShipment : public QuestScript 
{ 
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();


		Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 1379);
		if(creat == NULL)
			return;
		creat->m_escorter = mTarget;   
		creat->GetAIInterface()->setMoveType(11);
		creat->GetAIInterface()->StopMovement(3000);
		creat->GetAIInterface()->SetAllowedToEnterCombat(false);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Okay let's do!");
		creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

		sEAS.CreateCustomWaypointMap(creat);
		sEAS.WaypointCreate(creat,-5753.780762f, -3433.290039f, 301.628387f, 4.834769f, 0, 256, 1417);
		sEAS.WaypointCreate(creat,-5744.062500f, -3476.653564f, 302.269287f, 5.580896f, 0, 256, 1417);
		sEAS.WaypointCreate(creat,-5674.703125f, -3543.583984f, 303.273682f, 4.775867f, 0, 256, 1417);
		sEAS.WaypointCreate(creat,-5670.187500f, -3595.618164f, 311.888153f, 4.791576f, 0, 256, 1417);
		sEAS.WaypointCreate(creat,-5664.515625f, -3687.601563f, 317.954590f, 4.131842f, 0, 256, 1417);
		sEAS.WaypointCreate(creat,-5705.745117f, -3755.254150f, 321.452118f, 4.457779f, 0, 256, 1417);
		sEAS.WaypointCreate(creat,-5711.766113f, -3778.145752f, 322.827942f, 4.473486f, 0, 256, 1417);
		sEAS.EnableWaypoints(creat);
	}
};

class Miran : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Miran);
	Miran(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 7)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, I'm outta here!");
			_unit->Despawn(5000,1000);
			sEAS.DeleteWaypoints(_unit);
			if(_unit->m_escorter == NULL)
				return;
			Player* plr = _unit->m_escorter;
			_unit->m_escorter = NULL;
			plr->GetQuestLogForEntry(309)->SendQuestComplete();
		}
	}
};

void SetupLochModan(ScriptMgr * mgr)
{
	mgr->register_creature_script(1379, &Miran::Create);
	/*mgr->register_quest_script(309, CREATE_QUESTSCRIPT(ProtectingtheShipment));*/
}
