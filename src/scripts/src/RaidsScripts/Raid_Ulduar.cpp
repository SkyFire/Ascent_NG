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
#include "Raid_Ulduar.h"


// Ulduar Teleporter
void UlduarTeleporter::GossipHello(Object*  pObject, Player* Plr, bool AutoSend)
{
	GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 0, Plr);

    Menu->AddItem(0, "Teleport to the Expedition Base Camp.", 0);
    Menu->AddItem(0, "Teleport to the Formation Grounds.", 1);
    Menu->AddItem(0, "Teleport to the Colossal Forge.", 2);

    if(AutoSend)
		Menu->SendTo(Plr);
};

void UlduarTeleporter::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char *Code)
{
	switch(IntId)
	{
	case 0:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), -735.864075f, -93.616364f, 429.841797f, 0.079723f);
		break;
	case 1:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 130.710297f, -35.272095f, 409.804901f, 6.276515f);
		break;
	case 2:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 539.894897f, -11.009894f, 409.804749f, 0.021830f);
		break;
	};
};

void SetupUlduar(ScriptMgr* mgr)
{
	GossipScript * UlduarTele = (GossipScript*) new UlduarTeleporter();
	mgr->register_go_gossip_script(194569, UlduarTele);
};