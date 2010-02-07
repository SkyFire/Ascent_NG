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

// Cassa Crimsonwing
#define GOSSIP_CASSA_CRIMSONWING    "Lady Jaina told me to speak to yo about using a gryphon to survey Alcaz Island."

class CassaCrimsonwing_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 11224, plr);
        if(plr->GetQuestLogForEntry(11142) != NULL){
			Menu->AddItem( 0, GOSSIP_CASSA_CRIMSONWING, 1);
		};
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
        case 1:
			{
				plr->GetQuestLogForEntry(11142)->SendQuestComplete();
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 724 ); // Flight Path
				plr->TaxiStart( path, 1147, 0 ); // Gryph
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

// Captain Garran Vimes
#define GOSSIP_CAPTAIN_GARRAN_VIMES    "What have you heard of the Shady Rest Inn?"

class CaptainGarranVimes_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1793, plr);
			
			Menu->AddItem( 0, GOSSIP_CAPTAIN_GARRAN_VIMES, 1);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		GossipMenu * Menu;
        switch(IntId)
        {
        case 1:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1794, plr);
				Menu->SendTo(plr);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupTheramoreGossip(ScriptMgr * mgr)
{
	GossipScript * CassaCrimsonwingGossip = (GossipScript*) new CassaCrimsonwing_Gossip;
	GossipScript * CaptainGarranVimesGossip = (GossipScript*) new CaptainGarranVimes_Gossip;
	
	mgr->register_gossip_script(23704, CassaCrimsonwingGossip); // Cassa Crimsonwing
	mgr->register_gossip_script(4944, CaptainGarranVimesGossip); // Captain Garran Vimes
}
