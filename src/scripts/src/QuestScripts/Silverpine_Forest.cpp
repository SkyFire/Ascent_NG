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

// Prove Your Worth
class ProveYourWorthQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ProveYourWorthQAI);
	ProveYourWorthQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if (mKiller->IsPlayer())
		{
			QuestLogEntry *en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(421);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

class EscortingErland : public QuestScript
{
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 3465);
		if(creat == NULL)
			return;
		creat->m_escorter = mTarget;
		creat->GetAIInterface()->setMoveType(11);
		creat->GetAIInterface()->StopMovement(3000);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I to the horror I fear wolfs, do not allow to approach them to me closely. Follow me");
		creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

		sEAS.CreateCustomWaypointMap(creat);
		sEAS.WaypointCreate(creat,1408.243286f, 1086.476929f, 53.684814f, 4.145067f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1400.292236f, 1070.908325f, 52.454960f, 2.442609f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1370.173218f, 1087.330811f, 52.136669f, 3.921230f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1344.565063f, 1063.532349f, 52.780121f, 3.053365f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1291.855225f, 1065.194336f, 54.129812f, 2.291528f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1278.805664f, 1080.154541f, 53.967297f, 1.062379f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1293.010742f, 1106.864624f, 50.572147f, 1.476284f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1298.289917f, 1148.772827f, 52.434540f, 1.625511f, 0, 256, 2684);
		sEAS.WaypointCreate(creat,1289.469727f, 1200.744995f, 52.651005f, 1.488066f, 0, 256, 2684);
		sEAS.EnableWaypoints( creat );
	}
};

class Deathstalker_Erland : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Deathstalker_Erland);
	Deathstalker_Erland(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(iWaypointId == 9)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, you helped me to overcome this obstacle");
			_unit->Despawn(5000,1000);
			sEAS.DeleteWaypoints(_unit);
			if(_unit->m_escorter == NULL)
				return;
			Player* plr = _unit->m_escorter;
			_unit->m_escorter = NULL;
			plr->GetQuestLogForEntry(435)->SendQuestComplete();
		}
	}
};

void SetupSilverpineForest(ScriptMgr * mgr)
{
	mgr->register_creature_script(1769, &ProveYourWorthQAI::Create);
	/*mgr->register_quest_script(435, CREATE_QUESTSCRIPT(EscortingErland));*/
	mgr->register_creature_script(1978, &Deathstalker_Erland::Create);
}
