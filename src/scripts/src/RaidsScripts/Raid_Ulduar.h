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
#include "../Common/Base.h"

// Ulduar Teleporter
class SCRIPT_DECL UlduarTeleporter : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player*  Plr, bool AutoSend);
	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);

	void GossipEnd(Object*  pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	};
	void Destroy()
	{
		delete this;
	};
};

void SetupUlduar(ScriptMgr* mgr);
