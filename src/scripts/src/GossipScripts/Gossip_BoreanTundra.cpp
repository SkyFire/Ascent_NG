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

class TiareGossipScript : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, "Teleport me to Amber Ledge!", 1); 
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature* Tiare = TO_CREATURE((pObject));		
		plr->Gossip_Complete();
		Tiare->CastSpell(plr, dbcSpell.LookupEntry(50135), true);
	}			
	void Destroy()
    {
        delete this;
	}

};

class SurristraszGossipScript : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
        Menu->AddItem( 0, "I'd like passage to the Transitus Shield.", 0);
		Menu->AddItem( 0, "May I use a drake to fly elsewhere?", 1);
		Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature* pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pCreature==NULL)
			return;

        switch(IntId)
        {
        case 0:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 795 ); // Flight Path
				plr->TaxiStart( path, 25854, 0 ); // Dragon
            }break;
		case 1:
			{
				plr->GetSession()->SendTaxiList(pCreature);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupBoreanTundraGossip(ScriptMgr * mgr)
{
	GossipScript * TiareGossip = (GossipScript*) new TiareGossipScript;
	mgr->register_gossip_script(30051, TiareGossip);		// Tiare
	GossipScript * SurristraszGossip = (GossipScript*) new SurristraszGossipScript;
	mgr->register_gossip_script(24795, SurristraszGossip);		// Surristrasz
}