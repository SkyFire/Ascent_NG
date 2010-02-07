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


//Krikthir The Gatewatcher BOSS
#define BOSS_KRIKTHIR 28684

#define KRIKTHIR_MINDFLAY 52586
#define KRIKTHIR_CURSEOFFATIGUE 52592
#define KRIKTHIR_ENRAGE 28747

#define KRIKTHIR_MINDFLAY_HC 59367
#define KRIKTHIR_CURSEOFFATIGUE_HC 59368

class KrikthirAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(KrikthirAI, AscentScriptCreatureAI);
	KrikthirAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		if( !IsHeroic() )
		{
			AddSpell(KRIKTHIR_CURSEOFFATIGUE, Target_Self, 100, 0, 10);
			AddSpell(KRIKTHIR_MINDFLAY, Target_RandomPlayer, 100, 0, 7, 0, 30);
		}
		else
		{
			AddSpell(KRIKTHIR_CURSEOFFATIGUE_HC, Target_Self, 100, 0, 10);
			AddSpell(KRIKTHIR_MINDFLAY_HC, Target_RandomPlayer, 100, 0, 7, 0, 30);
		};
	
		AddEmote(Event_OnCombatStart, "This kingdom belongs to the Scourge! Only the dead may enter.", Text_Yell, 14075);
		AddEmote(Event_OnTargetDied, "You were foolish to come.", Text_Yell, 14077);
		AddEmote(Event_OnTargetDied, "As Anub'Arak commands!", Text_Yell, 14078);
		AddEmote(Event_OnDied, "I should be grateful. But I long ago lost the capacity.", Text_Yell, 14087);
	
		mEnraged = false;
	};
	
	void AIUpdate()
	{
		if ( _unit->GetHealthPct() <= 10 && mEnraged == false )
		{
			ApplyAura(KRIKTHIR_ENRAGE);
			mEnraged = true;
		};

		ParentClass::AIUpdate();
	};
	
	void OnDied(Unit* pKiller)
	{
		GameObject* Doors = GetNearestGameObject(192395);
		if( Doors != NULL )
			Doors->Despawn(0);

		ParentClass::OnDied(pKiller);
	};

	void Destroy()
	{
		delete this;
	};
	
	bool mEnraged;
};

//boss Hadronox
#define BOSS_HADRONOX 28921

#define HADRONOX_WEBGRAB 53406
#define HADRONOX_PIERCEARMOR 53418 
#define HADRONOX_LEECHPOISON 53030
#define HADRONOX_ACIDCLOUD 53400

#define HADRONOX_WEBGRAB_HC 59421
#define HADRONOX_LEECHPOISON_HC 59417
#define HADRONOX_ACIDCLOUD_HC 59419

class HadronoxAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(HadronoxAI, AscentScriptCreatureAI);
	HadronoxAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		if( !IsHeroic() )
		{
			AddSpell(HADRONOX_WEBGRAB, Target_RandomPlayer, 22, 0, 14, 0, 0);
			AddSpell(HADRONOX_LEECHPOISON, Target_Self, 14, 0, 25, 0, 20);
			AddSpell(HADRONOX_ACIDCLOUD, Target_RandomPlayer, 18, 0, 20, 0, 60);
		}
		else
		{
			AddSpell(HADRONOX_WEBGRAB_HC, Target_RandomPlayer, 22, 0, 14, 0, 0);
			AddSpell(HADRONOX_LEECHPOISON_HC, Target_Self, 14, 0, 25, 0, 20);
			AddSpell(HADRONOX_ACIDCLOUD_HC, Target_RandomPlayer, 18, 0, 20, 0, 60);
		};
		
		AddSpell(HADRONOX_PIERCEARMOR, Target_ClosestPlayer, 20, 0, 5, 0, 0);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Watcher Gashra.
#define CN_GASHRA 28730

#define GASHRA_WEBWRAP 52086
#define GASHRA_INFECTEDBITE 52469
#define GASHRA_ENRAGE 52470

class GashraAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(GashraAI, AscentScriptCreatureAI);
	GashraAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(GASHRA_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
		AddSpell(GASHRA_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Watcher Narjil
#define CN_NARJIL 28729

#define NARJIL_WEBWRAP 52086
#define NARJIL_INFECTEDBITE 52469
#define NARJIL_BLINDINGWEBS 52524

class NarjilAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(NarjilAI, AscentScriptCreatureAI);
	NarjilAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(NARJIL_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
		AddSpell(NARJIL_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
		AddSpell(NARJIL_BLINDINGWEBS, Target_ClosestPlayer, 16, 0, 9, 0, 0);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Watcher Silthik
#define CN_SILTHIK 28731

#define SILTHIK_WEBWRAP 52086
#define SILTHIK_INFECTEDBITE 52469
#define SILTHIK_POISONSPRAY 52493

class SilthikAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(SilthikAI, AscentScriptCreatureAI);
	SilthikAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(NARJIL_WEBWRAP, Target_RandomPlayer, 22, 0, 35, 0, 0);
		AddSpell(NARJIL_INFECTEDBITE, Target_ClosestPlayer, 35, 0, 12, 0, 0);
		AddSpell(SILTHIK_POISONSPRAY, Target_RandomPlayer, 30, 0, 15, 0, 0);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Anub'ar Shadowcaster (anub shadowcaster)
#define CN_ANUB_SHADOWCASTER 28733

#define SHADOWCASTER_SHADOWBOLT 52534
#define SHADOWCASTER_SHADOW_NOVA 52535

class AnubShadowcasterAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(AnubShadowcasterAI, AscentScriptCreatureAI);
	AnubShadowcasterAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWCASTER_SHADOWBOLT, Target_RandomPlayer, 36, 0, 8);
		AddSpell(SHADOWCASTER_SHADOW_NOVA, Target_Self, 22, 0, 15);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Anub'ar Skirmisher (anub skirmisher)
#define CN_ANUB_SKIRMISHER 28734

#define SKIRMISHER_BACKSTAB 52540

class AnubSkirmisherAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(AnubSkirmisherAI, AscentScriptCreatureAI);
	AnubSkirmisherAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(SKIRMISHER_BACKSTAB, Target_ClosestPlayer, 36, 0, 8);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Anub'ar Venomancer (anub venomancer)
#define CN_ANUB_VENOMANCER 29217

#define VENOMANCER_POISONBOLT 53617

class AnubVenomancerAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(AnubVenomancerAI, AscentScriptCreatureAI);
	AnubVenomancerAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(VENOMANCER_POISONBOLT, Target_RandomPlayer, 25, 0, 6);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Anub'ar Warrior (anub warrior)
#define CN_ANUB_WARRIOR 28732

#define WARRIOR_CLAVE 49806
#define WARRIOR_STRIKE 52532

class AnubWarriorAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(AnubWarriorAI, AscentScriptCreatureAI);
	AnubWarriorAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(WARRIOR_CLAVE, Target_ClosestPlayer, 35, 0, 7);
		AddSpell(WARRIOR_STRIKE, Target_ClosestPlayer, 10, 0, 10);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

//Anub'ar Webspinner (anub webspinner)
#define CN_ANUB_WEBSPINNER 29335

#define WEBSPINNER_WEBSHOT 54290
#define WEBSPINNER_WEBWRAP 52086

class AnubWebspinnerAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(AnubWebspinnerAI, AscentScriptCreatureAI);
	AnubWebspinnerAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(WEBSPINNER_WEBSHOT, Target_RandomPlayer, 30, 0, 5);
		AddSpell(WEBSPINNER_WEBWRAP, Target_RandomPlayer, 20, 0, 12);
	};
	
	void Destroy()
	{
		delete this;
	};
};	

void SetupAzjolNerub(ScriptMgr * mgr) 
{
	//Bosses
	mgr->register_creature_script(BOSS_KRIKTHIR, &KrikthirAI::Create);
	mgr->register_creature_script(BOSS_HADRONOX, &HadronoxAI::Create);
	
	// Trash
	mgr->register_creature_script(CN_GASHRA, &GashraAI::Create);
	mgr->register_creature_script(CN_NARJIL, &NarjilAI::Create);
	mgr->register_creature_script(CN_SILTHIK, &SilthikAI::Create);
	mgr->register_creature_script(CN_ANUB_SHADOWCASTER, &AnubShadowcasterAI::Create);
	mgr->register_creature_script(CN_ANUB_SKIRMISHER, &AnubSkirmisherAI::Create);
	mgr->register_creature_script(CN_ANUB_VENOMANCER, &AnubVenomancerAI::Create);
	mgr->register_creature_script(CN_ANUB_WARRIOR, &AnubWarriorAI::Create);
	mgr->register_creature_script(CN_ANUB_WEBSPINNER, &AnubWebspinnerAI::Create);
}
	