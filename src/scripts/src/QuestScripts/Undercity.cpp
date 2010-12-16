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

class Quest_JourneytoUndercity : public QuestScript // never extend std::tr1::enable_shared_from_this. EVER. :)
{ 
public:

  void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
  {
    Creature* creat = mTarget->GetMapMgr()->GetSqlIdCreature(19175); //Lady Sylvanas Windrunner - NCDB guid
	if(creat == NULL) // we still check for equality with == NULL. if we are assigning, we use = NULL cuz the pointers have changed
      return;

    sEAS.SpawnCreatureExtended(mTarget, 21628, 1295.222656f, 314.253998f, -57.320854f, 2.365611f, 35, 180000, true, true, true);
    sEAS.SpawnCreatureExtended(mTarget, 21628, 1293.403931f, 311.264465f, -57.320854f, 1.939140f, 35, 180000, true, true, true);
    sEAS.SpawnCreatureExtended(mTarget, 21628, 1286.532104f, 311.452423f, -57.320854f, 0.592182f, 35, 180000, true, true, true);
    sEAS.SpawnCreatureExtended(mTarget, 21628, 1284.536011f, 314.496338f, -57.320845f, 0.580401f, 35, 180000, true, true, true);

    creat->PlaySoundToSet(10896);
    creat->CastSpell(creat, dbcSpell.LookupEntry(36568), false);

    creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

    // Players can't interact with Sylvanas for 180000 ms.
	// Cast creat to an object because the EventSetUInt32Value method is in Object class.
	sEventMgr.AddEvent( TO_OBJECT(creat), &Object::EventSetUInt32Value, (uint32)UNIT_NPC_FLAGS, (uint32)2, EVENT_SCRIPT_UPDATE_EVENT, 180000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
  }
};

void SetupUndercity(ScriptMgr *mgr)
{
  mgr->register_quest_script(9180, CREATE_QUESTSCRIPT(Quest_JourneytoUndercity));
}
