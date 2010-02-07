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

//---------------------------------------------------------------------------------------------//
//-----------------------------------Zul'aman boss scripts-------------------------------------//
//---------------------------------Not blizzlike. Not tested-----------------------------------//
//-------------------------------Some crash/mess fixes - M4ksiu--------------------------------//
//---------------------------------------------------------------------------------------------//

// NalorakkAI
#define CN_NALORAKK					23576

// Troll Form spells
#define NALORAKK_MANGLE				44955
#define NALORAKK_SURGE				25787	// 42402 - correct spell hits creature casting spell
// Bear Form spells
#define NALORAKK_LACERATING_SLASH	42395
#define NALORAKK_REND_FLESH			42397
#define NALORAKK_DEAFENING_ROAR		42398
// Common spells
#define NALORAKK_BRUTAL_SWIPE		42384
// Other spells
#define NALORAKK_BERSERK			41924

class NalorakkAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(NalorakkAI, AscentScriptBossAI);
	NalorakkAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddPhaseSpell(1, AddSpell(NALORAKK_BRUTAL_SWIPE, Target_Current, 2, 0, 35));
		AddPhaseSpell(1, AddSpell(NALORAKK_MANGLE, Target_Current, 12, 0, 20));
		AddPhaseSpell(1, AddSpell(NALORAKK_SURGE, Target_RandomPlayer, 8, 0, 20, 0.0f, 45.0f, true, "I bring da pain!", Text_Yell, 12071));

		AddPhaseSpell(2, AddSpell(NALORAKK_LACERATING_SLASH, Target_Current, 12, 0, 20));
		AddPhaseSpell(2, AddSpell(NALORAKK_REND_FLESH, Target_Current, 12, 0, 12));
		AddPhaseSpell(2, AddSpell(NALORAKK_DEAFENING_ROAR, Target_RandomPlayer, 11, 0, 12));

		SetEnrageInfo(AddSpell(NALORAKK_BERSERK, Target_Self, 0, 0, 600, 0, 0, false, "You had your chance, now it be too late!", Text_Yell, 12074), 600000);

		AddEmote(Event_OnCombatStart, "You be dead soon enough!", Text_Yell, 12070);
		AddEmote(Event_OnTargetDied, "Da Amani gonna rule again!", Text_Yell, 12076);
		AddEmote(Event_OnTargetDied, "Now whatchoo got to say?", Text_Yell, 12075);
		AddEmote(Event_OnDied, "I... be waitin' on da udda side....", Text_Yell, 12077);

		// Bear Form
		Morph = AddSpell(42377, Target_Self, 0, 0, 0, 0, 0, false, "You call on da beast, you gonna get more dan you bargain for!", Text_Yell, 12072);
	}

	void OnCombatStart(Unit* pTarget)
	{
		// 45 Seconds until switch to Bear Form
		MorphTimer = AddTimer(45000);
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit* pTarget)
	{
		// On leaving combat he changes back to a troll
		SetDisplayId(21631);
		ParentClass::OnCombatStop(pTarget);
	}

	void OnDied(Unit* pKiller)
	{
		// On death he changes back to a troll
		SetDisplayId(21631);
		ParentClass::OnDied(pKiller);
	}

	void AIUpdate()
	{
		// Bear Form
		if(IsTimerFinished(MorphTimer) && GetPhase() == 1)
		{
			SetPhase(2, Morph);
			// Morph into a bear since the spell doesnt work
			SetDisplayId(21635);
			// 20 Seconds until switch to Troll Form
			ResetTimer(MorphTimer, 20000);
		}
		
		// Troll Form
		else if(IsTimerFinished(MorphTimer) && GetPhase() == 2)
		{
			// Remove Bear Form
			RemoveAura(42377);
			// Transform back into a Troll
			SetDisplayId(21631);
			SetPhase(1);
			// 45 Seconds until switch to Bear Form
			ResetTimer(MorphTimer, 45000);

			Emote("Make way for Nalorakk!", Text_Yell, 12073);
		}
		ParentClass::AIUpdate();
	}

	SpellDesc* Morph;
	int32 MorphTimer;
};

//Akil'zon <Eagle Avatar>
#define CN_AKILZON					23574

#define AKILZON_STATIC_DISRUPTION	44008 //INSTANT
#define AKILZON_CALL_LIGHTING		43661 //INSTANT 
#define AKILZON_GUST_OF_WIND		43621 //INSTANT
#define AKILZON_ELECTRICAL_STORM	43648
#define AKILZON_SOARING_EAGLE		24858 //SUMMONS  Akil'zon <Eagle Avatar>

class AKILZONAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(AKILZONAI, AscentScriptBossAI);
    AKILZONAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
    {
		AddSpell(AKILZON_STATIC_DISRUPTION, Target_Self, 2, 0, 60);
		AddSpell(AKILZON_CALL_LIGHTING, Target_Current, 2, 0, 0);
		AddSpell(AKILZON_GUST_OF_WIND, Target_Current, 0, 0, 0);
		AddSpell(AKILZON_ELECTRICAL_STORM, Target_Self, 1, 0, 0);

		AddEmote(Event_OnCombatStart, "I be da predator! You da prey!", Text_Yell, 12013);
		AddEmote(Event_OnTargetDied, "Stop your cryin'!", Text_Yell, 12018);
		AddEmote(Event_OnTargetDied, "Ya got nothin'!", Text_Yell, 12017);
		AddEmote(Event_OnDied, "You can't... kill... me spirit!", Text_Yell, 12019);
    }

	void OnCombatStart(Unit* pTarget)
	{
		// 2 Minute timer till Soaring Eagles are spawned
		timesummon = AddTimer(120000);
		ParentClass::OnCombatStart(pTarget);
	}

    void AIUpdate()
	{	
		if(IsTimerFinished(timesummon))
		{
			AscentScriptCreatureAI *Eagle = NULL;
			// Spawn 3 Soaring Eagles
			for(int x=0; x<3; x++)
			{
				Eagle = SpawnCreature(AKILZON_SOARING_EAGLE, (_unit->GetPositionX()+RandomFloat(10)-10), (_unit->GetPositionY()+RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(), true);
				if(Eagle)
				{
					Eagle->AggroNearestUnit();
					Eagle->SetDespawnWhenInactive(true);
				}
			}
			Eagle = NULL;
			Emote("Feed, me bruddahs!", Text_Yell, 12019);
			// Restart the timer
			ResetTimer(timesummon, 120000);
		}
		ParentClass::AIUpdate();
	}

	int32 timesummon;
};


//Halazzi <Lynx Avatar>

#define CN_HALAZZI						 23577
#define HALAZZI_CORRUPTED_LIGHTING_TOTEM 24224 
#define HALAZZI_ENRAGE					 44779

//Phase 1
#define HALAZZI_SABER_LASH				 43267 //43267 //43268 ///40810 //40816
//Phase 2
#define HALAZZI_FLAME_SHOCK				 43303
#define HALAZZI_EARTH_SHOCK				 43305 //INSTANT , VARIOUS
//Phase 3
//Halazzi now drops his totems more frequently as well as doing more damage. 




class HALAZZIAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(HALAZZIAI, AscentScriptBossAI);
    HALAZZIAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
    {
		AddPhaseSpell(1, AddSpell(HALAZZI_SABER_LASH, Target_Destination, 0.5, 0, 0, 0, 0, false, "Me gonna carve ya now!", Text_Yell, 12023));

		AddPhaseSpell(2, AddSpell(HALAZZI_FLAME_SHOCK, Target_Current, 12, 0, 0));
		AddPhaseSpell(2, AddSpell(HALAZZI_EARTH_SHOCK, Target_Current, 12, 0, 0));

		AddPhaseSpell(3, AddSpell(HALAZZI_SABER_LASH, Target_Destination, 0.5, 0, 0, 0, 0, false, "You gonna leave in pieces!", Text_Yell, 12024));
		AddPhaseSpell(3, AddSpell(HALAZZI_FLAME_SHOCK, Target_Current, 18, 0, 0));
		AddPhaseSpell(3, AddSpell(HALAZZI_EARTH_SHOCK, Target_Current, 18, 0, 0));
		AddPhaseSpell(3, AddSpell(HALAZZI_ENRAGE, Target_Self, 100, 0, 60));

		// Transfigure: 4k aoe damage
		Transfigure = AddSpell(44054, Target_Self, 0, 0, 0, 0, 0, false, "I fight wit' untamed spirit...", Text_Yell, 12021);

		AddEmote(Event_OnCombatStart, "Get on your knees and bow to da fang and claw!", Text_Yell, 12020);
		AddEmote(Event_OnTargetDied, "You cant fight da power...", Text_Yell, 12026);
		AddEmote(Event_OnTargetDied, "You all gonna fail...", Text_Yell, 12027);
		AddEmote(Event_OnDied, "Chaga... choka'jinn.", Text_Yell, 12028);
	}
	
	void OnCombatStart(Unit* pTarget)
	{
		TotemTimer = AddTimer(5000); // Just to make the Timer ID
		SplitCount = 0;
		MaxHealth = _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
		Lynx = NULL;
		
		ParentClass::OnCombatStart(pTarget);
	}

	void OnCombatStop(Unit* pTarget)
	{
		Merge();
		ParentClass::OnCombatStop(pTarget);
	}

	void OnDied(Unit* mKiller)
	{
		if(GetPhase() == 2) // Halazzi Can NOT die during phase 2 unless a player can do 48k damage
		{
			if (mKiller->IsPlayer())
			{
				_unit->Tag(TO_PLAYER(mKiller));
			}
			else if (mKiller->IsPet() && TO_PET(mKiller)->GetPetOwner())
			{
				_unit->Tag(TO_PET(mKiller)->GetPetOwner());
			}
		}
		ParentClass::OnDied(mKiller);
	}

    void AIUpdate()
	{
		// Every 25% Halazzi calls on the lynx
		if(!Lynx && GetHealthPercent() <= 75 && SplitCount == 0)
			Split();
		else if(!Lynx && GetHealthPercent() <= 50 && SplitCount == 1)
			Split();
		else if(!Lynx && GetHealthPercent() <= 25 && SplitCount == 2)
			Split();

		// Lynx OR Halazzi is at 20% HP Merge them together again
		if(Lynx && (Lynx->GetHealthPct() <= 20 || GetHealthPercent() <= 20))
			Merge();
		
		// At <25% Phase 3 begins
		if(GetHealthPercent() < 25 && GetPhase() == 1)
		{
			ResetTimer(TotemTimer, 30000);
			SetPhase(3);
		}

		if(GetPhase() == 2 || GetPhase() == 3)
		{
			if(IsTimerFinished(TotemTimer))
			{
				AscentScriptCreatureAI *Totem = NULL;
				Totem = SpawnCreature(HALAZZI_CORRUPTED_LIGHTING_TOTEM, (_unit->GetPositionX()+RandomFloat(3)-3), (_unit->GetPositionY()+RandomFloat(3)-3), _unit->GetPositionZ(), 0, true);
				if(Totem)
				{
					Totem->Despawn(60000); // Despawn in 60 seconds
					Totem->AggroNearestPlayer();
					Totem = NULL;
				}
				switch(GetPhase())
				{
					case 2: ResetTimer(TotemTimer, 60000); break;
					case 3: ResetTimer(TotemTimer, 30000);  break; // Spawn them faster then phase 2
				}
			}
		}

		ParentClass::AIUpdate();
	}

	void Split()
	{
		CurrentHealth = GetUnit()->GetUInt32Value(UNIT_FIELD_HEALTH);
		SetDisplayId(24144);
		GetUnit()->SetUInt32Value(UNIT_FIELD_HEALTH, 240000);
		GetUnit()->SetUInt32Value(UNIT_FIELD_MAXHEALTH, 240000);

		Lynx = GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature(24143, GetUnit()->GetPositionX(), GetUnit()->GetPositionY(), GetUnit()->GetPositionZ(), GetUnit()->GetOrientation(), true, false, 0, 0);
		if(Lynx)
		{
			Lynx->GetAIInterface()->AttackReaction(GetUnit()->GetAIInterface()->GetNextTarget(), 1);
			Lynx->m_noRespawn = true;
		}

		SetPhase(2, Transfigure);
	}

	void Merge()
	{
		if(Lynx)
		{
			Lynx->Despawn(0, 0);
			Emote("Spirit, come back to me!", Text_Yell, 12022);
		}
			
		if(CurrentHealth)
			GetUnit()->SetUInt32Value(UNIT_FIELD_HEALTH, CurrentHealth);
		if(MaxHealth)
			GetUnit()->SetUInt32Value(UNIT_FIELD_MAXHEALTH, MaxHealth);
		SetDisplayId(21632);
		
		// Null variables
		CurrentHealth = NULL;
		Lynx = NULL;
		SplitCount++;
		SetPhase(1);
	}

	Creature* Lynx;
	SpellDesc* Transfigure;
	int32 TotemTimer;
	int32 CurrentHealth;
	int32 MaxHealth;
	int SplitCount;
};

class LynxSpiritAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(LynxSpiritAI, AscentScriptCreatureAI);
    LynxSpiritAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		// Lynx Flurry
		AddSpell(43290, Target_Self, 15, 0, 8);
		// Shred Armor
		AddSpell(43243, Target_Current, 20, 0, 0);
	}
};


//Jan'alai <Dragonhawk Avatar>
#define CN_JANALAI				23578

//Phase 1
#define JANALAI_FLAME_BREATH	43582 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
#define JANALAI_FLAME_BUFFET	43299 
//#define TELEPORT ????
//Phase2 
//#define JANALAI_FLAME_BREATH	23461 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
//#define JANALAI_FLAME_BUFFET	43299 
//phase3 
//#define JANALAI_FLAME_BREATH	23461 //43582,43294,43215,43140,39263,38974,38267,37638 in dbc, need test
//#define JANALAI_FLAME_BUFFET	43299 
#define	  JANALAI_ENRAGE		44779




class JANALAIAI : public AscentScriptCreatureAI
{
public:
    ASCENTSCRIPT_FACTORY_FUNCTION(JANALAIAI, AscentScriptCreatureAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    JANALAIAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(JANALAI_FLAME_BREATH);
		spells[0].targettype = TARGET_DESTINATION;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(JANALAI_FLAME_BUFFET); 
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;
		
        spells[2].info = dbcSpell.LookupEntry(JANALAI_FLAME_BREATH);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = true;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 1000;

		spells[3].info = dbcSpell.LookupEntry(JANALAI_FLAME_BUFFET);
		spells[3].targettype = TARGET_ATTACKING; 
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000;
		


    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Spirits Of the wind, be your doom!");
		_unit->PlaySoundToSet(12031);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
			if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It all be over now, mon!");
				_unit->PlaySoundToSet(12036);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you!");
				_unit->PlaySoundToSet(12041);
				break;
			}
		}
    }

    void OnCombatStop(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Zul'jin... got a surprise for you...");
		_unit->PlaySoundToSet(12038);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 50 && phase == 1) 
		{
			spmin = 2;
			nrspells = 4;
			phase = 2;
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I burn ya now!");
			_unit->PlaySoundToSet(12032);
		}
		if (_unit->GetHealthPct() <= 25 && phase == 2) 
		{
		spmin = 2;
		nrspells = 4;
		phase = 3;
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(JANALAI_ENRAGE), true);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Where ma hatcha? Get to work on dem eggs!");
		_unit->PlaySoundToSet(12033);
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=spmin;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
	int spmin;
	int phase;
};

//HEX_LORD_MALACRASS

#define CN_HEX_LORD_MALACRASS 24239
//summons
#define CN_GAZAKROTH		  24244
#define CN_THURG			  24241 
#define CN_LORD_RAADAN		  24243 
#define CN_DARKHEART		  24246 
#define CN_ALYSON_ANTILLE	  24240
#define CN_SLITHER			  24242 
#define CN_FENSTALKER		  24245 
#define CN_KORAGG			  24247 

//spells
#define HEX_LORD_MALACRASS_SIPHON_SOUL	43501 //http://www.wowhead.com/?spell=43501
#define HEX_LORD_MALACRASS_SPIRIT_BOLTS 43382 //43383 

class HEXLORDMALACRASSAI : public AscentScriptCreatureAI
{
public:
    ASCENTSCRIPT_FACTORY_FUNCTION(HEXLORDMALACRASSAI,AscentScriptCreatureAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    HEXLORDMALACRASSAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SIPHON_SOUL);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 1.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HEX_LORD_MALACRASS_SPIRIT_BOLTS);
		spells[1].targettype = TARGET_DESTINATION; 
		spells[1].instant = true;
		spells[1].perctrigger = 3.0f;
		spells[1].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Da shadow gonna fall on you...");
		_unit->PlaySoundToSet(12041);
		timer = 5;
		mobs = 0;
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {
				if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Azzaga choogo zinn!");
				_unit->PlaySoundToSet(12044);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dis a nightmare ya don' wake up from!");
				_unit->PlaySoundToSet(12043);
				break;
			}
		}
	
    }

    void OnCombatStop(Unit* mTarget)
    {
		nrspells = 2;
		timer = 0;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dis not da end for me..");
		_unit->PlaySoundToSet(12051);
       RemoveAIUpdateEvent();

	   GameObject*  pDoor = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(123.256f, 914.422f, 34.1441f, 186306);
        if(pDoor == 0)
            return;

       pDoor->SetState(0); // Open the door
    }

    void AIUpdate()
	{
		if (!timer && !mobs)
		{
			for(int j=0;j<4;j++)
			{
				int RandomSpeach;
				RandomUInt(1000);
				RandomSpeach=rand()%8;
				switch (RandomSpeach)
				{
					case 0:
						summon = CN_GAZAKROTH;
						break;
					case 1:
						summon = CN_DARKHEART;
						break;
					case 2:
						summon = CN_FENSTALKER;
						break;
					case 3:
						summon = CN_SLITHER;
						break;
					case 4:
						summon = CN_ALYSON_ANTILLE;
						break;
					case 5:
						summon = CN_THURG;
						break;
					case 6:
						summon = CN_KORAGG;
						break;
					case 7:
						summon = CN_LORD_RAADAN;
						break;
				}
				Creature* cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(summon, 
				(_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			mobs = 1;
		}
		if (timer)
		{
			timer--;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
	int timer;
	uint32 summon;
	bool mobs;
};

//ZULJIN

#define CN_ZULJIN 23863

//Phase 1 troll
#define ZULJIN_GRIEVOUS_THROW	  43093 
#define ZULJIN_WHIRLWIND		  17207 // 44949 - work,43442,41400, 41399,41195, 41194,41098,  // from dbc, untested
#define ZULJIN_MODEL_TROLL		  21899 
//Phase2 bear
#define ZULJIN_CREEPING_PARALYSIS 43095 
#define ZULJIN_OVERPOWER		  43456
#define ZULJIN_MODEL_BEAR		  21736
//phase3  eagle
#define ZULJIN_ENERGY_STORM		  43983 //not Work
#define ZULJIN_MODEL_EAGLE		  22257
#define ZULJIN_FEATHER_VORTEX	  24136 //summons npc 4 begin phase
//phase4  lynx
#define ZULJIN_CLAW_RAGE		  43150 
#define ZULJIN_LYNX_RUSH		  43153
#define ZULJIN_MODEL_LYNX		  21907
//phase5  dragonhawk
#define ZULJIN_FLAME_WHIRL		  43213 //43208, 43213
#define ZULJIN_FLAME_BREATH		  43215 
#define ZULJIN_FLAME_SHOCK		  43303
#define ZULJIN_MODEL_DRAGONHAWK   21901




class ZULJINAI : public AscentScriptCreatureAI
{
public:
    ASCENTSCRIPT_FACTORY_FUNCTION(ZULJINAI, AscentScriptCreatureAI);
	SP_AI_Spell spells[10];
	bool m_spellcheck[10];

    ZULJINAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(ZULJIN_GRIEVOUS_THROW);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(ZULJIN_WHIRLWIND); 
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = false;
		spells[1].perctrigger = 25.0f;
		spells[1].attackstoptimer = 1000;
		
        spells[2].info = dbcSpell.LookupEntry(ZULJIN_CREEPING_PARALYSIS); 
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 5.0f;
		spells[2].attackstoptimer = 1000;
		
        spells[3].info = dbcSpell.LookupEntry(ZULJIN_OVERPOWER);
		spells[3].targettype = TARGET_VARIOUS; 
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(ZULJIN_ENERGY_STORM); 
		spells[4].targettype = TARGET_VARIOUS;  
		spells[4].instant = true;
		spells[4].perctrigger = 10.0f;
		spells[4].attackstoptimer = 1000;
		
		spells[5].info = dbcSpell.LookupEntry(ZULJIN_CLAW_RAGE);
		spells[5].targettype = TARGET_ATTACKING; 
		spells[5].instant = true;
		spells[5].perctrigger = 10.0f;
		spells[5].attackstoptimer = 1000;
		
		spells[6].info = dbcSpell.LookupEntry(ZULJIN_LYNX_RUSH);
		spells[6].targettype = TARGET_ATTACKING; 
		spells[6].instant = true;
		spells[6].perctrigger = 10.0f;
		spells[6].attackstoptimer = 1000;
		
        spells[7].info = dbcSpell.LookupEntry(ZULJIN_FLAME_WHIRL); 
		spells[7].targettype = TARGET_VARIOUS;
		spells[7].instant = true;
		spells[7].perctrigger = 3.0f;
		spells[7].attackstoptimer = 1000;

		spells[8].info = dbcSpell.LookupEntry(ZULJIN_FLAME_BREATH); 
		spells[8].targettype = TARGET_ATTACKING; 
		spells[8].instant = true;
		spells[8].perctrigger = 5.0f;
		spells[8].attackstoptimer = 1000;

		spells[9].info = dbcSpell.LookupEntry(ZULJIN_FLAME_SHOCK);
		spells[9].targettype = TARGET_ATTACKING; 
		spells[9].instant = true;
		spells[9].perctrigger = 5.0f;
		spells[9].attackstoptimer = 1000;			

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nobody badduh dan me.");
		_unit->PlaySoundToSet(12091);
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }


	void OnTargetDied(Unit* mTarget)
    {
					if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%3;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Di Amani Di chuga");
				_unit->PlaySoundToSet(12098);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lot more gonna fall like you!");
				_unit->PlaySoundToSet(12099);
				break;
			}
		}
    }

    void OnCombatStop(Unit* mTarget)
    {
		nrspells = 2;
		spmin = 0;
		phase =1;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_TROLL);  //change to Troll
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Mebbe me fall...but da Amani empire...never gonna die...");
		_unit->PlaySoundToSet(12100);
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 80 && phase == 1) 
		{
			spmin = 2;
			nrspells = 4;
			phase = 2;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_BEAR);  //change to Bear
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Got me some new tricks...like me bruddah bear!");
			_unit->PlaySoundToSet(12092);
		}
		if (_unit->GetHealthPct() <= 60 && phase == 2) 
		{
			spmin = 4;
			nrspells = 5;
			phase = 3;
			for(int j=0;j<4;j++)
			{
				Creature* cre = NULL;
				cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(ZULJIN_FEATHER_VORTEX, 
				(_unit->GetPositionX() + RandomFloat(10)-10), (_unit->GetPositionY() + RandomFloat(10)-10),
				_unit->GetPositionZ(), _unit->GetOrientation(),
				true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 0); 
				cre->GetAIInterface()->setOutOfCombatRange(50000);
			}
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_EAGLE);  //change to Eagle
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Dere be no hidin' from da eagle!");
			_unit->PlaySoundToSet(12093);
		}
		if (_unit->GetHealthPct() <= 40 && phase == 3) 
		{
			spmin = 5;
			nrspells = 7;
			phase = 4;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_LYNX);  //change to Lynx
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Let me introduce to you my new bruddahs: fang and claw!");
			_unit->PlaySoundToSet(12094);
		}
		if (_unit->GetHealthPct() <= 20 && phase == 4) 
		{
			spmin = 7;
			nrspells = 10;
			phase = 5;
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_DRAGONHAWK);  //change to Dragonhawk
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ya don' have to look to da sky to see da dragonhawk!");
			_unit->PlaySoundToSet(12095);
		}
		if (_unit->GetHealthPct() <= 1 && phase == 5) 
		{
			_unit->PlaySoundToSet(12100);
			_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , ZULJIN_MODEL_TROLL);  //change to Troll
			phase = 6;
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=spmin;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
	int spmin;
	int phase;
};


//Mobs

//SOARING_EAGLE Summon Akil'zon 

#define CN_SOARING_EAGLE 24858

#define EAGLE_SWOOP		 44732 //INSTANT


class SOARINGEAGLEAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(SOARINGEAGLEAI, AscentScriptCreatureAI);
    SOARINGEAGLEAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		AddSpell(EAGLE_SWOOP, Target_Destination, 5, 0, 0);
		GetUnit()->m_noRespawn = true;
    }
};

//Corrupted Lightning Totem
#define CLT_LIGHTING 43301 //43526 //43301  //43661 //

class CLTOTEMAI : public AscentScriptCreatureAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(CLTOTEMAI, AscentScriptCreatureAI);

    CLTOTEMAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
		AddSpell(CLT_LIGHTING, Target_Current, 20, 0.5f, 0);
		
		SetCanMove(false);
		SetAllowMelee(false);
		SetAllowRanged(false);
		_unit->m_noRespawn = true;
    }
};

//Gazakroth

#define CN_GAZAKROTH		24244

#define GAZAKROTH_FIREBOLT	39023 


class GAZAKROTHAI : public AscentScriptCreatureAI
{
public:
    ASCENTSCRIPT_FACTORY_FUNCTION(GAZAKROTHAI, AscentScriptCreatureAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    GAZAKROTHAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(GAZAKROTH_FIREBOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
};

//Lord Raadan
#define LR_FLAME_BREATH 43215 
#define LR_THUNDERCLAP  30633 //38537, 36706 // 36214  //44033


class LORDRAADANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LORDRAADANAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    LORDRAADANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(LR_FLAME_BREATH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;
		
       /* spells[1].info = dbcSpell.LookupEntry(LR_THUNDERCLAP); all types spells to crash server
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//SLITHER

#define S_POISON_BOLT 38459 


class SLITHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SLITHERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SLITHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(S_POISON_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//THURG

#define TH_CLEAVE 43273


class THURGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(THURGAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    THURGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(TH_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//DARKHEART

#define DH_FEAR 39415
#define DH_CLEAVE 43273

class DARKHEARTAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DARKHEARTAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    DARKHEARTAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 2;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(DH_CLEAVE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000; 
		
        /*spells[1].info = dbcSpell.LookupEntry(DH_FEAR);   crash
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 1.0f;
		spells[1].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//FENSTALKER

#define FEN_TOXIC_SLIME 37615  //40818

class FENSTALKERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FENSTALKERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FENSTALKERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(FEN_TOXIC_SLIME);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//ALYSON_ANTILLE
#define ALYS_HEAL_GRONN 36678

class ALYSONANTILLEAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ALYSONANTILLEAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ALYSONANTILLEAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(ALYS_HEAL_GRONN);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = false;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//KORAGG
#define KOR_FIREBALL 40877  

class KORAGGAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KORAGGAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    KORAGGAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(KOR_FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 2.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

//FEATHER_VORTEX
#define CN_FEATHER_VORTEX				24136
#define FEATHER_VORTEX_CYCLONE_STRIKE	42495 

class FEATHERVORTEXAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FEATHERVORTEXAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FEATHERVORTEXAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
			nrspells = 1;
			for(int i=0;i<nrspells;i++)
			{
				m_spellcheck[i] = false;
			}
		
        spells[0].info = dbcSpell.LookupEntry(FEATHER_VORTEX_CYCLONE_STRIKE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }


	void OnTargetDied(Unit* mTarget)
    {

    }

    void OnCombatStop(Unit* mTarget)
    {
		_unit->Despawn(3000,0);
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->Despawn(30000,0);
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit* target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}
                  	m_spellcheck[i] = false;
					return;
				}
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void Destroy()
	{
		delete this;
	};

protected:

	int nrspells;
};

void SetupZulAman(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_NALORAKK, &NalorakkAI::Create);
	/*
	mgr->register_creature_script(CN_JANALAI, &JANALAIAI::Create);
	mgr->register_creature_script(CN_HEX_LORD_MALACRASS, &HEXLORDMALACRASSAI::Create);
	mgr->register_creature_script(CN_ZULJIN, &ZULJINAI::Create);
	//MOBS
	mgr->register_creature_script(CN_AKILZON, &SOARINGEAGLEAI::Create);
	mgr->register_creature_script(CN_HALAZZI, &CLTOTEMAI::Create);
	mgr->register_creature_script(CN_GAZAKROTH, &GAZAKROTHAI::Create);
	mgr->register_creature_script(CN_LORD_RAADAN, &LORDRAADANAI::Create);
	mgr->register_creature_script(CN_SLITHER, &SLITHERAI::Create);
	mgr->register_creature_script(CN_THURG, &THURGAI::Create);
	mgr->register_creature_script(CN_DARKHEART, &DARKHEARTAI::Create);
	mgr->register_creature_script(CN_FENSTALKER, &FENSTALKERAI::Create);
	mgr->register_creature_script(CN_ALYSON_ANTILLE, &ALYSONANTILLEAI::Create);
	mgr->register_creature_script(CN_KORAGG, &KORAGGAI::Create);
	mgr->register_creature_script(CN_FEATHER_VORTEX, &FEATHERVORTEXAI::Create);*/

	mgr->register_creature_script(CN_AKILZON, &AKILZONAI::Create);
	mgr->register_creature_script(AKILZON_SOARING_EAGLE, &SOARINGEAGLEAI::Create);
	mgr->register_creature_script(CN_HALAZZI, &HALAZZIAI::Create);
	// Lynx Spirit
	mgr->register_creature_script(24143, &LynxSpiritAI::Create);
	mgr->register_creature_script(HALAZZI_CORRUPTED_LIGHTING_TOTEM, &CLTOTEMAI::Create);
}
