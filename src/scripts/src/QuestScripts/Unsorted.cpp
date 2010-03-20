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

class Quest_Grimoire_Business : public QuestScript
{
public:
	void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		sEAS.SpawnCreature(mTarget, 22911, 3279.67f, 4640.77f, 216.526f, 1.3516f, 2*60*1000);
	}

};

class Quest_Maggocs_Treasure_Chest : public QuestScript
{
public:
	void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		mTarget->GetMapMgr()->GetInterface()->SpawnCreature(20600, 2001.76f, 5164.77f, 265.19f, 5.5148f, true, false, 0, 0);
	}

};

class Quest_Grulloc_Has_Two_Skulls : public QuestScript
{
public:
	void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		sEAS.SpawnCreature(mTarget, 20216, 2687.46f, 5541.14f, -1.93669f, 3.52847f, 2*60*1000);
	}

};

class Quest_Zuluhed_the_Whacked : public QuestScript
{
public:
	void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;
		sEAS.SpawnCreature(mTarget, 11980, -4177.39f, 376.289f, 117.78f, 2.7381f, 2*60*1000);
	}

};

#define QUEST_CLUCK		 3861
#define ITEM_CHICKEN_FEED   11109

class Chicken : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Chicken);
	Chicken(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 12);
		_unit->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
		RegisterAIUpdateEvent(120000);
	}

	void AIUpdate()
	{
		if(_unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
			OnLoad();
	}
};

class Kaliri : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Kaliri);
	Kaliri(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
		_unit->_setFaction();
	}
};

void SetupUnsorted(ScriptMgr * mgr)
{
	mgr->register_quest_script(10998, CREATE_QUESTSCRIPT(Quest_Grimoire_Business));
	mgr->register_quest_script(10996, CREATE_QUESTSCRIPT(Quest_Maggocs_Treasure_Chest));
	mgr->register_quest_script(10995, CREATE_QUESTSCRIPT(Quest_Grulloc_Has_Two_Skulls));
	mgr->register_quest_script(10866, CREATE_QUESTSCRIPT(Quest_Zuluhed_the_Whacked));
	mgr->register_creature_script(620, &Chicken::Create);
	mgr->register_creature_script(21468, &Kaliri::Create);
}
