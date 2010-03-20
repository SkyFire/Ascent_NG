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

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
	Menu->SendTo(plr);


class Prisoner12 : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;
		GossipMenu *Menu;
		Creature* Prisoner12 = TO_CREATURE(pObject);
		if (Prisoner12 == NULL)
			return;
		
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
		if(plr->GetQuestLogForEntry(9164))
			Menu->AddItem( 0, "Release Him.", 1);
	 
		if(AutoSend)
			Menu->SendTo(plr);
	}
 
	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature* Prisoner12 = TO_CREATURE(pObject);
		if (Prisoner12 == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;
 
			case 1:
			{
			QuestLogEntry *en = plr->GetQuestLogForEntry(9164);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				en->SetMobCount(0, en->GetMobCount(0) + 1);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();

				if(!Prisoner12)
					return;

				Prisoner12->Despawn(5000, 6*60*1000);
				Prisoner12->SetStandState(STANDSTATE_STAND);
				Prisoner12->SetUInt32Value(UNIT_NPC_EMOTESTATE, 7);
				return;
			}break;
			}
		}
	}
 
	void Destroy()
	{
		delete this;
	}
};


class Prisoner22 : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature* Prisoner22 = TO_CREATURE(pObject);
		if (Prisoner22 == NULL)
			return;
			
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
		if(plr->GetQuestLogForEntry(9164))
			Menu->AddItem( 0, "Release Him.", 1);
	 
		if(AutoSend)
			Menu->SendTo(plr);
	}
 
	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature* Prisoner22 = TO_CREATURE(pObject);
		if (Prisoner22 == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;
 
			case 1:
			{
			QuestLogEntry *en = plr->GetQuestLogForEntry(9164);
			if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
			{
				en->SetMobCount(1, en->GetMobCount(1) + 1);
				en->SendUpdateAddKill(1);
				en->UpdatePlayerFields();

				if(!Prisoner22)
					return;

				Prisoner22->Despawn(5000, 6*60*1000);
				Prisoner22->SetStandState(STANDSTATE_STAND);
				Prisoner22->SetUInt32Value(UNIT_NPC_EMOTESTATE, 7);
				return;
			}break;
			}
		}
	}
 
	void Destroy()
	{
		delete this;
	}
};


class Prisoner32 : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature* Prisoner32 = TO_CREATURE(pObject);
		if (Prisoner32 == NULL)
			return;

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
		if(plr->GetQuestLogForEntry(9164))
			Menu->AddItem( 0, "Release Him.", 1);
	 
		if(AutoSend)
			Menu->SendTo(plr);
	}
 
	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature* Prisoner32 = TO_CREATURE(pObject);
		if (Prisoner32 == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;
 
			case 1:
			{
			QuestLogEntry *en = plr->GetQuestLogForEntry(9164);
			if(en && en->GetMobCount(2) < en->GetQuest()->required_mobcount[2])
			{
				en->SetMobCount(2, en->GetMobCount(2) + 1);
				en->SendUpdateAddKill(2);
				en->UpdatePlayerFields();

				if(!Prisoner32)
					return;

				Prisoner32->Despawn(5000, 6*60*1000);
				Prisoner32->SetStandState(STANDSTATE_STAND);
				Prisoner32->SetUInt32Value(UNIT_NPC_EMOTESTATE, 7);
				return;
			}break;
			}
		}
	}
 
	void Destroy()
	{
		delete this;
	}
};

class PrisonersatDeatholme : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(PrisonersatDeatholme);
  PrisonersatDeatholme(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnLoad()
  {
	_unit->SetStandState(STANDSTATE_DEAD);
	_unit->GetAIInterface()->m_canMove = false;
  }
};

class VanquishingAquantion : public GameObjectAIScript
{
public:
	VanquishingAquantion(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new VanquishingAquantion(GO); }

	void OnActivate(Player* pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9174);
		if(qle == NULL)
			return;

		Creature* naga = sEAS.SpawnCreature(pPlayer, 16292, 7938, -7632, 114, 3.05, 0);
		naga->Despawn(6*60*1000, 0);
	}
};

void SetupGhostlands(ScriptMgr * mgr)
{
	GossipScript * Prisoner12Gossip = (GossipScript*) new Prisoner12();
	mgr->register_gossip_script(16208, Prisoner12Gossip);

	GossipScript * Prisoner22Gossip = (GossipScript*) new Prisoner22();
	mgr->register_gossip_script(16206, Prisoner22Gossip);

	GossipScript * Prisoner32Gossip = (GossipScript*) new Prisoner32();
	mgr->register_gossip_script(16209, Prisoner32Gossip);

	mgr->register_creature_script(16208, &PrisonersatDeatholme::Create);
	mgr->register_creature_script(16206, &PrisonersatDeatholme::Create);
	mgr->register_creature_script(16209, &PrisonersatDeatholme::Create);

	mgr->register_gameobject_script(181157, &VanquishingAquantion::Create);
}
