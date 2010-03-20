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

//	Wyrmrest Temple Flighter
// Top to Bottom & Top to Middle
#define CN_TOP 26949
#define GOSSIP_TOP_TO_BOTTOM "Yes, please, I would like to return to the ground level of the temple."
#define GOSSIP_TOP_TO_MIDDLE "I would like to go Lord Afrasastrasz in the middle of the temple."
#define NPCTEXT_TOP 12714
// Middle to Top & Middle to Bottom
#define CN_MIDDLE 27575
#define GOSSIP_MIDDLE_TO_TOP "My lord, I need to get to the top of the temple."
#define GOSSIP_MIDDLE_TO_BOTTOM "Can I get a ride back to ground level, Lord Afrasastrasz?"
#define NPCTEXT_MIDDLE 12887
// Bottom to Top & Bottom to Middle
#define CN_BOTTOM 26443
#define GOSSIP_BOTTOM_TO_TOP "Steward, please allow me to ride one of the drakes to the queen's chamber at the top of the temple."
#define GOSSIP_BOTTOM_TO_MIDDLE "Can you spare a drake to take me to Lord Afrasastrasz in the middle of the temple?"
#define NPCTEXT_BOTTOM 12713

class SCRIPT_DECL WyrmrestTemple_FlightGossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		
		switch(pObject->GetEntry())
        {
		case CN_TOP:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), NPCTEXT_TOP, plr);
				Menu->AddItem( 0, GOSSIP_TOP_TO_BOTTOM, 1);
				Menu->AddItem( 0, GOSSIP_TOP_TO_MIDDLE, 2);
            }break;
		case CN_MIDDLE:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), NPCTEXT_MIDDLE, plr);
				Menu->AddItem( 0, GOSSIP_MIDDLE_TO_TOP, 3);
				Menu->AddItem( 0, GOSSIP_MIDDLE_TO_BOTTOM, 4);
            }break;
		case CN_BOTTOM:
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), NPCTEXT_BOTTOM, plr);
				Menu->AddItem( 0, GOSSIP_BOTTOM_TO_TOP, 5);
				Menu->AddItem( 0, GOSSIP_BOTTOM_TO_MIDDLE, 6);
            }break;
		}

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
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 879 ); // Flight Path
				plr->TaxiStart( path, 6371, 0 ); // Drake
            }break;
		case 2:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 880 ); // Flight Path
				plr->TaxiStart( path, 6371, 0 ); // Drake
            }break;
		case 3:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 881 ); // Flight Path
				plr->TaxiStart( path, 6371, 0 ); // Drake
            }break;
		case 4:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 882 ); // Flight Path
				plr->TaxiStart( path, 6371, 0 ); // Drake
            }break;
		case 5:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 878 ); // Flight Path
				plr->TaxiStart( path, 6371, 0 ); // Drake
            }break;
		case 6:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 883 ); // Flight Path
				plr->TaxiStart( path, 6371, 0 ); // Drake
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupWyrmrestTempleGossip(ScriptMgr * mgr)
{
	GossipScript * WyrmrestTempleFlightGossip = (GossipScript*) new WyrmrestTemple_FlightGossip;

	mgr->register_gossip_script(CN_TOP, WyrmrestTempleFlightGossip);	// Torastrasza <Majordomo to the Ruling Council>
	mgr->register_gossip_script(CN_MIDDLE, WyrmrestTempleFlightGossip);	// Lord Afrasastrasz <Commander of Wyrmrest Temple Defenses>
	mgr->register_gossip_script(CN_BOTTOM, WyrmrestTempleFlightGossip);	// Tariolstrasz <Steward of Wyrmrest Temple>
}
