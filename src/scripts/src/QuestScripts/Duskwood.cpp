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
#include "../Common/Base.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Eliza
#define CN_ELIZA				314
#define ELIZA_FROST_NOVA		11831
#define ELIZA_FROSTBOLT			20819
#define ELIZA_SUMMON_GUARD		3107

class ElizaAI : public AscentScriptCreatureAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(ElizaAI, AscentScriptCreatureAI);
	ElizaAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		mElizaCombatTimer = INVALIDATE_TIMER;
		SetCanEnterCombat(false);
		AddSpell(ELIZA_FROST_NOVA, Target_Current, 10, 0, 1, 0, 10, true);
		AddSpell(ELIZA_FROSTBOLT, Target_Current, 20, 3, 1);
		mSummonGuard = AddSpell(ELIZA_SUMMON_GUARD, Target_Self, 0, 0, 0);

		Emote("Wait...you are not my husband. But he must have sent you. And you...look..delicious!", Text_Say);
		mElizaCombatTimer = AddTimer(4000);		

		RegisterAIUpdateEvent(1000);
	}
	void AIUpdate()
	{
		ParentClass::AIUpdate();
		if (IsTimerFinished(mElizaCombatTimer))
		{
			SetCanEnterCombat(true);
			AggroNearestUnit();
			RemoveTimer(mElizaCombatTimer);
		}
		if ( GetHealthPercent()>=10 && GetHealthPercent()<=98 && !IsCasting())
		{
			// Correct me if I'm wrong but I saw only 1 guard spawning
			mElizaGuard = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 1871);
			if ( mElizaGuard == NULL )
			{
					CastSpellNowNoScheduling(mSummonGuard);
			}
		}
	}
	
	int32		mElizaCombatTimer;
	SpellDesc*	mSummonGuard;
	Creature*	mElizaGuard;
};

class SummonElizaQuest : public QuestScript
{	
public:
	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		Creature* Eliza = sEAS.SpawnCreature(mTarget, 314, -10271.127f, 53.784f, 42.711f, 1.72f);
		if ( Eliza != NULL )
			Eliza->Despawn( 300000, 0 );	// Should it be that much ?
	};
};

void SetupDuskwood(ScriptMgr * mgr)
{
	QuestScript *SummonEliza = (QuestScript*) new SummonElizaQuest();
	mgr->register_quest_script(254, SummonEliza);
	mgr->register_creature_script(CN_ELIZA, &ElizaAI::Create);
}
