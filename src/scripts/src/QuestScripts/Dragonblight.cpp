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

class WoodlandWalker : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WoodlandWalker);
	WoodlandWalker(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		pCreature->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 35 );
	}
};

class WoodlandWalkerGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		Creature*  pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
		if( pCreature == NULL )
			return;

		uint32 chance = RandomUInt(1);
		if( chance == 0 )
		{
			pCreature->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 14 );
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The Woodlands Walker is angered by your request and attacks!" );
		}
		else
		{
			sEAS.AddItem( 36786, plr );
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Breaking off a piece of its bark, the Woodlands Walker hands it to you before departing." );
		}

	}

	void Destroy()
	{
		delete this;
	};
};

class WrathGateQuestCinema : public QuestScript
{	
public:
  void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
  {
	if( mTarget == NULL )
		return;

	// send packet for movie
	uint32 id = 14;	
	mTarget->GetSession()->OutPacket(SMSG_TRIGGER_MOVIE, sizeof(uint32), &id);
  }
};

void SetupDragonblight(ScriptMgr * mgr)
{
	mgr->register_creature_script(26421, &WoodlandWalker::Create);

	GossipScript * WW = (GossipScript*) new WoodlandWalkerGossip();
	mgr->register_gossip_script(26421, WW);
	
	QuestScript *WrathGateCinema = (QuestScript*) new WrathGateQuestCinema();
	mgr->register_quest_script(12499, WrathGateCinema);
	mgr->register_quest_script(12500, WrathGateCinema);
}