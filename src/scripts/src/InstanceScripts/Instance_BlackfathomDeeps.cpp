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

/************************************************************************/
#define CN_Ghamoora 4887
class GhamooraAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(GhamooraAI, AscentScriptCreatureAI);
    GhamooraAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		// Trample
		AddSpell(5568, Target_Self, 12, 0, 0);
	}
};

#define CN_LadySarevess 4831
class LadySarevessAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(LadySarevessAI, AscentScriptCreatureAI);
    LadySarevessAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		// Forked Lightning
		AddSpell(8435, Target_Current, 10, 2, 0);
		// Frost Nova
		AddSpell(865, Target_Self, 15, 0, 25);
		// Slow
		AddSpell(246, Target_Current, 15, 0, 10);
		AddEmote(Event_OnCombatStart, "You should not be here! Slay them!", Text_Yell, 5799);
	}
};

#define CN_Gelihast 6243
class GelihastAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(GelihastAI, AscentScriptCreatureAI);
    GelihastAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		// Net
		AddSpell(6533, Target_RandomPlayer, 10, 0, 0);
	}
};

#define CN_LorgusJett 12902
class LorgusJettAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(LorgusJettAI, AscentScriptCreatureAI);
    LorgusJettAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		// Lightning Bolt
		AddSpell(12167, Target_Current, 20, 3, 0);
	}
};

#define CN_BaronAquanis 12876
class BaronAquanisAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(BaronAquanisAI, AscentScriptCreatureAI);
    BaronAquanisAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		// Frostbolt
		AddSpell(15043, Target_Current, 20, 3, 0);
		AggroNearestPlayer();
		SetDespawnWhenInactive(true);
	}
};

#define OBJ_FathomStone 177964
class FathomStone : public GameObjectAIScript
{
public:
	FathomStone(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new FathomStone(GO); }

	void OnActivate(Player*  pPlayer)
	{
		if(pPlayer->GetTeam() == 1 && !BaronAquanis) // Horde
		{
			// Spawn Baron Aquanis
			_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(CN_BaronAquanis, -782.021f, -63.5876f, -45.0935f, -2.44346f, true, false, 0, 0);
			BaronAquanis = true;

		}
	}

protected:
	bool BaronAquanis;
};

#define CN_TwilightLordKelris 12876
class KelrisAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(KelrisAI, AscentScriptCreatureAI);
    KelrisAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell, 5802);
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell, 5803);

		// Sleep
		AddSpell(8399, Target_RandomPlayer, 12, 1.3f, 0, 0, 0, false, "Sleep...", Text_Yell, 5804);
		// Mind Blast
		AddSpell(15587, Target_Current, 16, 1.5f, 0);
	}
};

#define CN_Akumai 12876
class AkumaiAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(AkumaiAI, AscentScriptCreatureAI);
    AkumaiAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature) 
	{
		AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell); // Should this really be here?
		AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell); // Should this really be here?

		// Frenzied Rage
		AddSpell(3490, Target_Self, 12, 0, 0, 0, 0);
		// Poison Cloud
		AddSpell(3815, Target_Self, 16, 0, 45);
	}
};
#define GS_Morridune 6729
class MorriduneGossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player*  plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7247, plr);
        
		if(plr->GetTeam() == 0)
			Menu->AddItem( 0, "Please Teleport me to Darnassus.", 1);
		Menu->AddItem( 0, "I wish to leave this horrible place", 2);

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player*  plr, uint32 Id, uint32 IntId, const char * Code)
    {
		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
			break;
			case 1:
				// port to Darnassus
				plr->SafeTeleport(1, 0, 9951.52f, 2280.32f, 1341.39f, 0);
			break;
			case 2:
				// Port to entrance
				plr->SafeTeleport(1, 0, 4247.74f, 745.879f, -24.2967f, 4.36996f);
			break;
		}
	}

    void Destroy()
    {
        delete this;
    }

};

void SetupBlackfathomDeeps(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_Ghamoora, &GhamooraAI::Create);
	mgr->register_creature_script(CN_LadySarevess, &LadySarevessAI::Create);
	mgr->register_creature_script(CN_Gelihast, &GelihastAI::Create);
	mgr->register_creature_script(CN_LorgusJett, &LorgusJettAI::Create);
	mgr->register_creature_script(CN_BaronAquanis, &BaronAquanisAI::Create);

	mgr->register_gameobject_script(OBJ_FathomStone, &FathomStone::Create);

	mgr->register_creature_script(CN_TwilightLordKelris, &KelrisAI::Create);
	mgr->register_creature_script(CN_Akumai, &AkumaiAI::Create);

	GossipScript * Morridune = (GossipScript*) new MorriduneGossip();
	mgr->register_gossip_script(GS_Morridune, Morridune);
}