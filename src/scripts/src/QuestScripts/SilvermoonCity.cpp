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

class ThePathoftheAdept : public GameObjectAIScript
{
public:
	ThePathoftheAdept(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new ThePathoftheAdept(GO); }

	void OnActivate(Player* pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9692);
		if(qle == NULL)
			return;

		Creature* zelemar = sEAS.SpawnCreature(pPlayer, 17830, pPlayer->GetPositionX()+5, pPlayer->GetPositionY()+5, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		zelemar->Despawn(6*60*1000, 0);
	}
};


class PathoftheAdept : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature* lord = TO_CREATURE(pObject);
		if (lord == NULL)
			return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9692))
			Menu->AddItem( 0, "Take Insignia", 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature* lord = TO_CREATURE(pObject);
		if (lord == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				sEAS.AddItem(24226, plr);
				return;
			}break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};


class LordDawnstar : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LordDawnstar);
	LordDawnstar(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
		_unit->SetStandState(7);
		_unit->setDeathState(CORPSE);
		_unit->GetAIInterface()->m_canMove = false;
	}
};


void SetupSilvermoonCity(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(182024, &ThePathoftheAdept::Create);

	GossipScript *LordGossip = (GossipScript*) new PathoftheAdept();
	mgr->register_gossip_script(17832, LordGossip);

	mgr->register_creature_script(17832, &LordDawnstar::Create);
}
