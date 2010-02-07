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

class Lady_Jaina : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(558))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7012, plr);
			Menu->AddItem(0, "I know this is rather silly but i have a young ward who is a bit shy and would like your autograph.", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature*  pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: // Give Item
			{
				plr->CastSpell(plr, dbcSpell.LookupEntry(23122),true);
				plr->Gossip_Complete();
				break;
			}
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class Cairne : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(plr->GetQuestLogForEntry(925))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7013, plr);
			Menu->AddItem(0, "Give me hoofprint.", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu *Menu;
		Creature*  pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pObject==NULL)
			return;

		switch(IntId)
		{
		case 0: // Return to start
			GossipHello(pCreature, plr, true);
			break;
		case 1: // Give Item
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7014, plr);
				Menu->SendTo(plr);
				plr->CastSpell(plr, dbcSpell.LookupEntry(23123),true);
				break;
			}
			break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

void SetupQuestGossip(ScriptMgr * mgr)
{
	GossipScript * LJ = (GossipScript*) new Lady_Jaina();
	GossipScript * CB = (GossipScript*) new Cairne();

	mgr->register_gossip_script(4968, LJ);
	mgr->register_gossip_script(3057, CB);

}
