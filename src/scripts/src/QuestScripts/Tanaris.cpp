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

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \ Menu->SendTo(plr);



class SpiritScreeches : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature* spirit = TO_CREATURE(pObject);
		if (spirit == NULL)
			return;

		if(plr->GetQuestLogForEntry(3520))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2039, plr);
			Menu->AddItem( 0, "Goodbye", 1);

			if(AutoSend)
				Menu->SendTo(plr);
		}



	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature* spirit = TO_CREATURE(pObject);
		if (spirit == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
				{
					QuestLogEntry *en = plr->GetQuestLogForEntry(3520);
					if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
					{
						en->SetMobCount(0, en->GetMobCount(0) + 1);
						en->SendUpdateAddKill(0);
						en->UpdatePlayerFields();
					}
					if(!spirit)
						return;

					spirit->Despawn(1, 0);
					return;

				}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class ScreecherSpirit : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ScreecherSpirit);

	ScreecherSpirit(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		if(!_unit)
			return

		_unit->Despawn(60*1000, 0);
		Creature* cialo = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 5307);
		if(!cialo)
			return;

		if(!cialo->isAlive())
			cialo->Despawn(1, 6*60*1000);
	}
};

class StewardOfTime : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(10279) || plr->HasFinishedQuest(10279))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9978, plr);
			Menu->AddItem( 0, "Please take me to the master's lair", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = TO_CREATURE(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(plr, dbcSpell.LookupEntry(34891), true);
			break;
		}
	}

	void Destroy()
	{
		delete this;
	};
};

void SetupTanaris(ScriptMgr * mgr)
{
	GossipScript * Screeches = (GossipScript*) new SpiritScreeches();
	mgr->register_gossip_script(8612, Screeches);

	mgr->register_creature_script(8612, &ScreecherSpirit::Create);
	
	GossipScript * StewardOfTimeGossip = (GossipScript*) new StewardOfTime();
	mgr->register_gossip_script(20142, StewardOfTimeGossip);
}
