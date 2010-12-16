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
/* Instance_RagefireChasm.cpp Script									*/
/************************************************************************/

/* TRASH MOBS */
// Ragefire Shaman AI
#define CN_RAGEFIRE_SHAMAN			11319
class RagefireShamanAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(RagefireShamanAI, AscentScriptCreatureAI);
    RagefireShamanAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Healing Wave
		AddSpell(11986, Target_WoundedFriendly, 15, 3, 10);
		// Lightning Bolt
		AddSpell(9532, Target_Current, 20, 3, 0);
	}
};

// Ragefire Trogg AI by Soulshifter
#define CN_RAGEFIRE_TROGG			11318
class RagefireTroggAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(RagefireTroggAI, AscentScriptCreatureAI);
    RagefireTroggAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Strike
		AddSpell(11976, Target_Current, 40, 0, 0);
	}
};

// Searing Blade Warlock AI by Soulshifter
#define CN_SEARING_BLADE_WARLOCK	11324
class SearingBladeWarlockAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SearingBladeWarlockAI, AscentScriptCreatureAI);
    SearingBladeWarlockAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Shadow Bolt
		AddSpell(20791, Target_Current, 20, 3, 0);
		// Summon Voidwalker -- Doesnt work (Disabled for now)
		//CastSpellNowNoScheduling(AddSpell(12746, Target_Self, 0, 0, 0));
	}
};

#define CN_SEARING_BLADE_ENFORCER	11323
class SearingBladeEnforcerAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SearingBladeEnforcerAI, AscentScriptCreatureAI);
    SearingBladeEnforcerAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Shield Slam
		AddSpell(8242, Target_Current, 15, 0, 0);
	}
};

// Blade Cultist AI by Soulshifter
#define CN_BLADE_CULTIST			11322
class BladeCultistAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(BladeCultistAI, AscentScriptCreatureAI);
    BladeCultistAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Curse of Agony
		AddSpell(18266, Target_Current, 30, 0, 15);
	}
};

// Molten Elemental AI by Soulshifter
#define CN_MOLTEN_ELEMENTAL			11321
class MoltenElementalAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(MoltenElementalAI, AscentScriptCreatureAI);
    MoltenElementalAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Fire Shield
		AddSpell(134, Target_Self, 40, 1, 15);
    }
};

// Earthborer AI by Soulshifter
#define CN_EARTHBORER				11320
class EarthborerAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(EarthborerAI, AscentScriptCreatureAI);
    EarthborerAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Earthborer Acid
		AddSpell(18070, Target_Current, 15, 0, 0);
	}
};

/* GAMEOBJECTS */
class BloodFilledOrb : public GameObjectAIScript
{
public:
	BloodFilledOrb(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject*  GO) { return new BloodFilledOrb(GO); }

	void OnActivate(Player*  pPlayer)
	{
		// Make sure player has the quest and Zelemar isn't spawned yet
		if(!pPlayer->HasQuest(9692)) // The Path of the Adept
		{
			pPlayer->GetSession()->SendNotification("Request quest `The Path of the Adept`.");
			return;
		}
		Creature* Zelemar = NULL;
		Zelemar = _gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-370.133f, 162.519f, -21.1299f, 17830);
		if(Zelemar)
			return;

		// Spawn Zelemar the Wrathful
		Zelemar = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(17830, -370.133f, 162.519f, -21.1299f, -1.29154f, true, false, 0, 0);
		if(Zelemar)
		{
			Zelemar->m_noRespawn = true;
			Zelemar = NULL;
		}
	}
};	
	
/* BOSSES */

//Oggleflint
#define CN_OGGLEFLINT	11517
class OggleflintAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(OggleflintAI, AscentScriptCreatureAI);
	OggleflintAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		// Cleave
		AddSpell(40505, Target_Current, 10, 0, 1);
	}
};

//Taragaman the Hungerer
#define CN_TARAGAMAN	11520
class TaragamanAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(TaragamanAI, AscentScriptCreatureAI);
	TaragamanAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		// Fire Nova
		AddSpell(11970, Target_Self, 10, 2, 0);
		// Uppercut
		AddSpell(18072, Target_Current, 10, 0, 0);
	}
};

//Jergosh The Invoker
#define CN_JERGOSH			11518
class JergoshAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(JergoshAI, AscentScriptCreatureAI);
	JergoshAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		// Immolate
		AddSpell(20800, Target_Current, 15, 2, 0);
		// Curse of Weakness
		AddSpell(18267, Target_Current, 10, 0, 0);
	}
};

//Bazzalan
#define CN_BAZZALAN	11519
class BazzalanAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(BazzalanAI, AscentScriptCreatureAI);
	BazzalanAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		// Sinister Strike
		AddSpell(14873, Target_Current, 15, 0, 0);
		// Poison (Correct ID?)
		AddSpell(18197, Target_Current, 5, 0, 0);
	}
};

void SetupRagefireChasm(ScriptMgr * mgr)
{
	/* TRASH MOBS */
	mgr->register_creature_script(CN_SEARING_BLADE_WARLOCK, &SearingBladeWarlockAI::Create);
	mgr->register_creature_script(CN_EARTHBORER, &EarthborerAI::Create);
	mgr->register_creature_script(CN_MOLTEN_ELEMENTAL, &MoltenElementalAI::Create);
	mgr->register_creature_script(CN_RAGEFIRE_SHAMAN, &RagefireShamanAI::Create);
	mgr->register_creature_script(CN_RAGEFIRE_TROGG, &RagefireTroggAI::Create);
	mgr->register_creature_script(CN_BLADE_CULTIST, &BladeCultistAI::Create);
	mgr->register_creature_script(CN_SEARING_BLADE_ENFORCER, &SearingBladeEnforcerAI::Create);

	/* GAMEOBJECTS */
	mgr->register_gameobject_script(182024, &BloodFilledOrb::Create);

	/* BOSSES */
	mgr->register_creature_script(CN_OGGLEFLINT, &OggleflintAI::Create);
	mgr->register_creature_script(CN_TARAGAMAN, &TaragamanAI::Create);
	mgr->register_creature_script(CN_JERGOSH, &JergoshAI::Create);
	mgr->register_creature_script(CN_BAZZALAN, &BazzalanAI::Create);
}