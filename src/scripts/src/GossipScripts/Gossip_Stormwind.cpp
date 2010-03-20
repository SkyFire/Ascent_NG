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

// Archmage Malin
#define GOSSIP_ARCHMAGE_MALIN    "Can you send me to Theramore? I have an urgent message for Lady Jaina from Highlord Bolvar."

class ArchmageMalin_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 11469, plr);

		if(plr->GetQuestLogForEntry(11223))
        Menu->AddItem( 0, GOSSIP_ARCHMAGE_MALIN, 1);
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature* pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		
		switch(IntId)
        {
        case 1:
			{
				plr->Gossip_Complete();
				pCreature->CastSpell(plr, dbcSpell.LookupEntry(42711), true);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};


void SetupStormwindGossip(ScriptMgr * mgr)
{
	GossipScript * ArchmageMalinGossip = (GossipScript*) new ArchmageMalin_Gossip;

	mgr->register_gossip_script(2708, ArchmageMalinGossip); // Archmage Malin

}
