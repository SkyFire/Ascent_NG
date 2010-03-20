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

class Zenn_Foulhoof : public QuestScript
{ 
public:

  void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
  {
    if(!mTarget)
		return;

    Creature* creat = mTarget->GetMapMgr()->GetSqlIdCreature(43727);
    if(creat == NULL)
      return;

    creat->SetUInt32Value(UNIT_FIELD_DISPLAYID,901);
	creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Ribbit! No! This cannot...ribbit...be! You have duped me with...ribbit..your foul trickery! Ribbit!");

	sEventMgr.AddEvent(TO_OBJECT(creat), &Object::EventSetUInt32Value, (uint32)UNIT_FIELD_DISPLAYID, (uint32)10035, EVENT_UNK, 50000, 0, 1);
  }
};

void SetupTeldrassil(ScriptMgr *mgr)
{	
  QuestScript *Zenn_FoulhoofQuest = (QuestScript*) new Zenn_Foulhoof();
  mgr->register_quest_script(489, Zenn_FoulhoofQuest);
}