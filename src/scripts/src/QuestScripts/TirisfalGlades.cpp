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

class TheDormantShade : public QuestScript
{
public:
	void OnQuestComplete( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		Creature* creat = mTarget->GetMapMgr()->GetInterface()->SpawnCreature(1946, 2467.314f, 14.8471f, 23.5950f, 0, true, false, 0, 0);
		creat->Despawn(60000,0);
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have disturbed my rest. Now face my wrath!");
	}
};

class CalvinMontague : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CalvinMontague);
	CalvinMontague(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 68);
		_unit->SetStandState(STANDSTATE_STAND);
	}

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		if(_unit->GetHealthPct() < 10)
		{
			if(mAttacker->GetTypeId() == TYPEID_PLAYER)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				RegisterAIUpdateEvent(1000);
				QuestLogEntry *qle = (TO_PLAYER(mAttacker))->GetQuestLogForEntry(590);
				if(!qle)
					return;
				qle->SendQuestComplete();
			}
		}
	}

	void AIUpdate()
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Okay, okay! Enough fighting.");
		_unit->RemoveAllNegativeAuras();
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 68);
		_unit->SetStandState(STANDSTATE_SIT);
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(433), true);
		sEventMgr.AddEvent(TO_UNIT(_unit), &Unit::SetStandState, (uint8)STANDSTATE_STAND, EVENT_CREATURE_UPDATE, 18000, 0, 1);
		_unit->GetAIInterface()->WipeTargetList();         
		_unit->GetAIInterface()->WipeHateList();
        _unit->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, _unit, 0);
        _unit->GetAIInterface()->disable_melee = true;
        _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
	}
};

class ARoguesDeal  : public QuestScript 
{ 
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* Dashel = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 6784);

		if(Dashel == NULL)
			return;

		Dashel->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 28);
		Dashel->GetAIInterface()->disable_melee = false;
		Dashel->GetAIInterface()->SetAllowedToEnterCombat(true);
	}
};

class FieldsofGrief : public QuestScript 
{ 
public:

	void OnQuestComplete( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* Zealot = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 1931);

		if(Zealot == NULL)
			return;

		Zealot->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I. . . I. . .don't. . .feel. . .right. . .");
		sEAS.EventCastSpell(Zealot, Zealot, 3287, 3000);
		Zealot->GetAIInterface()->setMoveType(11);
		Zealot->GetAIInterface()->StopMovement(3000);


		sEAS.CreateCustomWaypointMap(Zealot);

		sEAS.WaypointCreate(Zealot, 2289.649658f, 237.001389f, 27.089531f, 1.323f, 0, 256, 0);
		sEAS.WaypointCreate(Zealot, 2291.700928f, 235.561844f, 27.089531f, 1.323f, 0, 256, 0);
		sEAS.WaypointCreate(Zealot, 2292.835693f, 240.636948f, 27.089531f, 1.323f, 0, 256, 0);
		sEAS.WaypointCreate(Zealot, 2288.737305f, 240.503952f, 27.088022f, 1.323f, 0, 256, 0);
		sEAS.WaypointCreate(Zealot, 2287.812744f, 236.320938f, 27.088022f, 1.323f, 0, 256, 0);
		sEAS.WaypointCreate(Zealot, 2292.643033f, 240.513489f, 27.088022f, 1.323f, 0, 256, 0);
		sEAS.WaypointCreate(Zealot, 2289.628418f, 239.908279f, 27.088022f, 1.323f, 0, 256, 0);
	}
};

class Zealot : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Zealot);
	Zealot(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if(!_unit->HasAura(3287))
			return;
		if(iWaypointId == 2)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "My mind. . .me flesh. . .I'm. . .rotting. . . .!");
		}

		if(iWaypointId == 7)
		{
			_unit->CastSpell(_unit, dbcSpell.LookupEntry(5), true);
		}
	}
};


void SetupTirisfalGlades(ScriptMgr * mgr)
{
	mgr->register_quest_script(410, CREATE_QUESTSCRIPT(TheDormantShade));
	mgr->register_creature_script(6784, &CalvinMontague::Create);
	mgr->register_quest_script(590, CREATE_QUESTSCRIPT(ARoguesDeal));
	/*mgr->register_quest_script(407, CREATE_QUESTSCRIPT(FieldsofGrief));*/
	mgr->register_creature_script(1931, &Zealot::Create);
}
