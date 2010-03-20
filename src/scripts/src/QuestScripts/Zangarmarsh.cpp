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

class AncientMarks : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 entry = pObject->GetEntry();
		const char* text = "";
		uint32 TextId = 0;

		if(entry == 17900)
		{
			text = "Grant me your mark, wise ancient.";
			TextId = 9176;
		} 
		else if(entry == 17901)
		{
			text = "Grant me your mark, mighty ancient.";
			TextId = 9177;
		}
		
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextId, plr);

		if(plr->HasFinishedQuest(9785) || plr->GetQuestLogForEntry(9785))
			Menu->AddItem(0, text, 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		if(IntId == 1)
		{
			QuestLogEntry *en = plr->GetQuestLogForEntry(9785);
			Creature* casta = (TO_CREATURE(pObject));
			switch (pObject->GetEntry())
			{
			case 17900:
				{
					if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
					{
						en->SetMobCount(0, 1);
						en->SendUpdateAddKill(0);
						en->UpdatePlayerFields();
					}
					
					if(plr->GetStandingRank(942) == 4)
						casta->CastSpell(plr, 31808, true);
					else if(plr->GetStandingRank(942) == 5)
						casta->CastSpell(plr, 31810, true);
					else if(plr->GetStandingRank(942) == 6)
						casta->CastSpell(plr, 31811, true);
					else if(plr->GetStandingRank(942) == 7)
						casta->CastSpell(plr, 31815, true);

				}break;
			case 17901:
				{
					if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
					{
						en->SetMobCount(1, 1);
						en->SendUpdateAddKill(1);
						en->UpdatePlayerFields();
					}

					Creature* casta = (TO_CREATURE(pObject));
					if(plr->GetStandingRank(942) == 4)
						casta->CastSpell(plr, 31807, true);
					else if(plr->GetStandingRank(942) == 5)
						casta->CastSpell(plr, 31814, true);
					else if(plr->GetStandingRank(942) == 6)
						casta->CastSpell(plr, 31813, true);
					else if(plr->GetStandingRank(942) == 7)
						casta->CastSpell(plr, 31812, true);

				}break;
			}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class ElderKuruti : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(!plr->GetItemInterface()->GetItemCount(24573, true))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9226, plr);
			Menu->AddItem( 0, "Offer treat", 1);
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		GossipMenu *Menu;
		switch(IntId)
		{
		case 1:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9227, plr);
				Menu->AddItem( 0, "Im a messenger for Draenei", 2);
				Menu->SendTo(plr);
				break;
		case 2:
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9229, plr);
				Menu->AddItem( 0, "Get message", 3);
				Menu->SendTo(plr);
				break;
		case 3:
			if(!plr->GetItemInterface()->GetItemCount(24573, true))
			{
				sEAS.AddItem(24573, plr);
				if(plr->GetItemInterface()->GetItemCount(24573, true))
					SendQuickMenu(9231);
			}
			else
				SendQuickMenu(9231);
			break;
		}
	}

	void Destroy()
	{
		delete this;
	};
};

void SetupZangarmarsh(ScriptMgr * mgr)
{
	GossipScript * AMark = (GossipScript*) new AncientMarks();
	mgr->register_gossip_script(17900, AMark);	// Ashyen Ancient of Lore
	mgr->register_gossip_script(17901, AMark);	// Keleth Ancient of War
	mgr->register_gossip_script(18197, CREATE_GOSSIPSCRIPT(ElderKuruti));
}
