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

/*********************************************************************************/
/* Instance_ShadowfangKeep.cpp Script											*/
/*********************************************************************************/

// Commander Springvale AI
#define CN_SPRINGVALE 4278
class SpringvaleAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(SpringvaleAI, AscentScriptCreatureAI);
    SpringvaleAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Holy Light
		AddSpell(1026, Target_Self, 10, 2.5f, 0);
		// Devotion Aura (Not the right Spell ID)
		DevoAura = AddSpell(10290, Target_Self, 0, 0, 0);
		// Divine Protection (Not the right Spell ID)
		DivineProt = AddSpell(498, Target_Self, 0, 0, 0);
		// Hammer of Justice
		AddSpell(5588, Target_Current, 12, 0, 60);
    }
    
    void OnCombatStart(Unit* pTarget)
    {
		if(!GetUnit()->HasAura(DevoAura->mInfo->Id))
			CastSpellNowNoScheduling(DevoAura);

		ParentClass::OnCombatStart(pTarget);
    }

    void AIUpdate()
    {
		if(GetHealthPercent() <= 20 && DivineProt->mEnabled == true)
        {
			CastSpellNowNoScheduling(DivineProt);
			DivineProt->mEnabled = false;
		}
		ParentClass::AIUpdate();
    }

	SpellDesc* DevoAura;
	SpellDesc* DivineProt;
};

// Odo the Blindwatcher AI
#define CN_BLINDWATCHER 4279
class BlindWatcherAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(BlindWatcherAI, AscentScriptBossAI);
    BlindWatcherAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
    {
		// Howling Rage 1
		HowlingRage1 = AddSpell(7481, Target_Self, 0, 5, 0);
		// Howling Rage 2
		HowlingRage2 = AddSpell(7483, Target_Self, 0, 1.5f, 0);
		// Howling Rage 3
		HowlingRage3 = AddSpell(7484, Target_Self, 0, 1.5f, 0);
    }

    void AIUpdate()
    {
        if(GetHealthPercent() <= 75 && GetPhase() == 1)
        {
			SetPhase(2, HowlingRage1);
		}
		else if(GetHealthPercent() <= 45 && GetPhase() == 2)
        {
			if(GetUnit()->HasAura(7481))
				RemoveAura(7481);
			SetPhase(3, HowlingRage2);
		}
		else if(GetHealthPercent() <= 20 && GetPhase() == 3)
        {
			if(GetUnit()->HasAura(7483))
				RemoveAura(7483);
			SetPhase(4, HowlingRage3);
		}
		ParentClass::AIUpdate();
    }

	SpellDesc* HowlingRage1;
	SpellDesc* HowlingRage2;
	SpellDesc* HowlingRage3;
};

// Fenrus the Devourer AI
#define CN_FENRUS 4274
static Coords VWSpawns[] = {
	{}, // Spawn Locations for the 4 voidwalkers
	{-154.274368f, 2177.196533f, 128.448517f, 5.760980f},
	{-142.647537f, 2181.019775f, 128.448410f, 4.178475f},
	{-139.146774f, 2168.201904f, 128.448364f, 2.650803f},
	{-150.860092f, 2165.156250f, 128.448502f, 0.999966f},
};
class FenrusAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(FenrusAI, AscentScriptCreatureAI);
	FenrusAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(7125, Target_Current, 12, 1.5f, 60);
	}

	void OnDied(Unit*  pKiller)
	{
		GetUnit()->SendChatMessageAlternateEntry(4275, CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who dares interfere with the Sons of Arugal?");
		GetUnit()->PlaySoundToSet(5791);

		AscentScriptCreatureAI* voidwalker = NULL;
		// Spawn 4 x Arugal's Voidwalkers
		for(int x=1; x<5; x++)
		{
			voidwalker = SpawnCreature(4627, VWSpawns[x].mX, VWSpawns[x].mY, VWSpawns[x].mZ, VWSpawns[x].mO);
			if(voidwalker)
			{
				voidwalker->AggroNearestPlayer();
				voidwalker = NULL;
			}
		}

		ParentClass::OnDied(pKiller);
	}
};

//Arugals Voidwalkers
#define CN_VOIDWALKER 4627
class VoidWalkerAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(VoidWalkerAI, AscentScriptCreatureAI);
	VoidWalkerAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(7154, Target_WoundedFriendly, 5, 0, 7);
	}

	void OnDied(Unit*  pKiller)
	{
		GameObject*  pDoor = GetUnit()->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-129.034f, 2166.16f, 129.187f, 18972);
		if(pDoor)
			pDoor->SetState(0);

		ParentClass::OnDied(pKiller);
	}
};

// Archmage Arugal AI
#define CN_ARUGAL 4275
class ArugalAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(ArugalAI, AscentScriptCreatureAI);
    ArugalAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Void Bolt
		AddSpell(7588, Target_Current, 25, 3, 0);
		// Thunder Shock
		AddSpell(7803, Target_Self, 10, 0, 0);
		// Arugal's Curse
		AddSpell(7621, Target_RandomPlayer, 5, 0, 0, 0, 0, false, "Release your rage!", Text_Yell, 5797);
		// Arugal spawn-in spell (Teleport)
		AddSpell(10418, Target_Self, 10, 2, 0);

		AddEmote(Event_OnCombatStart, "You, too, shall serve!", Text_Yell, 5793);
		AddEmote(Event_OnTargetDied, "Another falls!", Text_Yell, 5795);
	}
};

//Wolf Master Nandos AI
#define CN_NENDOS 3927
class NandosAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(NandosAI, AscentScriptCreatureAI);
    NandosAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature){}

    void OnDied(Unit*  pKiller)
    {
		GameObject*  pDoor = GetUnit()->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-118.11f, 2161.86f, 155.678f, 18971);
		if(pDoor)		
			pDoor->SetState(0);

		ParentClass::OnDied(pKiller);
    }
};
//Deathstalker Adamant
#define CN_ADAMANT 3849

//Rethilgore AI
#define CN_RETHILGORE 3914
class RETHILGOREAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(RETHILGOREAI, AscentScriptCreatureAI);
    RETHILGOREAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature){}
    void OnDied(Unit*  pKiller)
    {
		_unit->SendChatMessageAlternateEntry(3849, CHAT_MSG_MONSTER_SAY, LANG_GUTTERSPEAK, "About time someone killed the wretch.");
		_unit->SendChatMessageAlternateEntry(3850, CHAT_MSG_MONSTER_SAY, LANG_COMMON, "For once I agree with you... scum.");      // dont know the allys text yet
		ParentClass::OnDied(pKiller);
    }
};

void SetupShadowfangKeep(ScriptMgr * mgr)
{
	//creature scripts
	mgr->register_creature_script(CN_NENDOS, &NandosAI::Create);
	mgr->register_creature_script(CN_VOIDWALKER, &VoidWalkerAI::Create);
	mgr->register_creature_script(CN_RETHILGORE, &RETHILGOREAI::Create);
	mgr->register_creature_script(CN_SPRINGVALE, &SpringvaleAI::Create);
	mgr->register_creature_script(CN_BLINDWATCHER, &BlindWatcherAI::Create);
	mgr->register_creature_script(CN_FENRUS, &FenrusAI::Create);
	mgr->register_creature_script(CN_ARUGAL, &ArugalAI::Create);
}
