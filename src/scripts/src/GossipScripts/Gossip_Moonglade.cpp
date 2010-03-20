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

// Trenton Lighthammer
#define GOSSIP_SILVA_FILNAVETH    "I'd like to fly to Rut'theran Village."

class SilvaFilnaveth_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        if(plr->getClass() == DRUID && plr->getRace() == RACE_NIGHTELF){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4914, plr);
			Menu->AddItem( 0, GOSSIP_SILVA_FILNAVETH, 1);
		}else if(plr->getClass() == DRUID && plr->getRace() == RACE_TAUREN){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4915, plr);
		}else{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4913, plr);
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
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 315 ); // Flight Path
				plr->TaxiStart( path, 479, 0 ); // Hippogryph
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

// Bunthen Plainswind
#define GOSSIP_BUNTHEN_PLAINSWIND    "I'd like to fly to Thunder Bluff."

class BunthenPlainswind_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
		if(plr->getClass() == DRUID && plr->getRace() == RACE_TAUREN){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4918, plr);
			Menu->AddItem( 0, GOSSIP_BUNTHEN_PLAINSWIND, 1);
		}else if(plr->getClass() == DRUID && plr->getRace() == RACE_NIGHTELF){
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4917, plr);
		}else{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4916, plr);
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
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 316 ); // Flight Path
				plr->TaxiStart( path, 295, 0 ); // Wyvern
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupMoongladeGossip(ScriptMgr * mgr)
{
	GossipScript * SilvaFilnavethGossip = (GossipScript*) new SilvaFilnaveth_Gossip;
	GossipScript * BunthenPlainswindGossip = (GossipScript*) new BunthenPlainswind_Gossip;

	mgr->register_gossip_script(11800, SilvaFilnavethGossip); // Silva Fil'naveth
	mgr->register_gossip_script(11798, BunthenPlainswindGossip); // Bunthen Plainswind

}
