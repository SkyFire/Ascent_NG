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

//MagisterTerrace Script 

//Bosses

//Selin Firehart Encounter

// Creature Entry's
#define TRASH_FelCrystal 24722
#define BOSS_SelinFireheart 24723

// Normal & Heroic Spells

/*
   Mana Rage
   Caster: Fel Crystal
   Details: Empowers the caster with fel energy, restoring their mana for 10 sec.
   Triggers: Increases the target's mana by 10%.
*/
#define FC_MANARAGE 44320
#define FC_MANARAGE_TRIGGER 44321

/*
   Fel Explosion
   Caster Selin Fireheart
   Details: Area of effect damage spell, cast continually until Selin is out of mana
*/
#define SF_FELEXPLOSION 44314
/*
   Drain Life
   Caster Selin Fireheart
   Details: Randomly targeted channeled spell, deals damage and heals Selin.
*/
#define SF_DRAINLIFE 44294

// Heroic Spells
/*
   Drain Mana (Heroic Mode Only)
   Caster Selin Fireheart
   Details: Randomly targeted channeled spell on a player with mana, drain Mana to the player and give it to Selin.
*/
#define SF_DRAINMANA 46153

// Fel Crystal Spawn Locations
static Coords FelCrystals[]={
	{225.969f, -20.0775f, -2.9731f, 0.942478f, TRASH_FelCrystal},
	{226.314f, 20.2183f, -2.98127f, 5.32325f, TRASH_FelCrystal},
	{247.888f, -14.6252f, 3.80777f, 2.33874f, TRASH_FelCrystal},
	{248.053f, 14.592f, 3.74882f, 3.94444f, TRASH_FelCrystal},
	{263.149f, 0.309245f, 1.32057f, 3.15905f, TRASH_FelCrystal}
};

class SelinFireheartAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SelinFireheartAI, AscentScriptCreatureAI);
	SelinFireheartAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(SF_DRAINLIFE, Target_RandomPlayer, 8, 0, 35);

		if(_unit->GetMapMgr()->iInstanceMode == MODE_HEROIC)
			AddSpell(SF_DRAINMANA, Target_RandomPlayer, 8, 0, 35);

		ManaRage = dbcSpell.LookupEntry(FC_MANARAGE);
		ManaRageTrigger = AddSpell(FC_MANARAGE_TRIGGER, Target_Self, 0, 0, 0);
		FelExplosion = AddSpell(SF_FELEXPLOSION, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit* pTarget)
	{
		/*
			Selin Fireheart starts with 0 mana and drains it from the felcrystals in the room
			- ToDo: Set it so mana regen is off
		*/
		_unit->SetUInt32Value(UNIT_FIELD_POWER1, 0);
		ParentClass::OnCombatStart(pTarget);
	}

	/*
		During the AIUpdate() Selin will spam FelExplosion until hes out of mana
		He will then attempt to gain mana from a FelCrystal thats in the room by running to them
	*/
	void AIUpdate()
	{
		// 10% of his mana according to wowhead is 3231 which is whats needed to cast FelExplosion
		if(GetManaPercent() < 10 || FelExplosion->mEnabled == false)
			Mana();
		else if(!IsCasting())// Mana is greater than 10%
			CastFelExplosion();

		ParentClass::AIUpdate();
	}

	void Mana()
	{
		/*
			Attempt to get a Fel Crystal and move to it if not in range.
			Once in range we get the FelCrystal to cast Mana Rage on Selin
		*/
		Unit* FelCrystal = NULL;
		PreventActions(false);

		FelCrystal = FindFelCrystal();

		if(!FelCrystal || !FelCrystal->isAlive())
		{
			PreventActions(true);
			FelCrystal = NULL;
			return;
		}

		// Not in range
		if(_unit->GetDistance2dSq(FelCrystal) > 100)
		{
			MoveTo(FelCrystal->GetPositionX(), FelCrystal->GetPositionY(), FelCrystal->GetPositionZ());
			FelCrystal = NULL;
			return;
		}

		_unit->GetAIInterface()->StopMovement(0);

		if(!FelCrystal->GetCurrentSpell())
			FelCrystal->CastSpell(_unit, ManaRage, false);

		// Mana Rage giving of mana doesnt work so we give 10%(3231) / AIUpdate() Event.
		CastSpellNowNoScheduling(ManaRageTrigger);
		uint32 mana = _unit->GetUInt32Value(UNIT_FIELD_POWER1)+3231;
		if(mana >= _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
			mana = _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1);

		_unit->SetUInt32Value(UNIT_FIELD_POWER1, mana);

		// Re-Enable FelExplosion
		if(GetManaPercent() >= 100)
			PreventActions(true);

		FelCrystal = NULL;
	}

	void PreventActions(bool Allow)
	{
		FelExplosion->mEnabled = Allow;
		SetAllowMelee(Allow);
		SetAllowRanged(Allow);
		SetAllowSpell(Allow);
		SetAllowTargeting(Allow);
	}

	Unit* FindFelCrystal()
	{
		/*
			Find a FelCrystal
		*/
		Unit* FC = NULL;
		for(int x=0; x<5; x++)
		{
			FC = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(FelCrystals[x].mX, FelCrystals[x].mY, FelCrystals[x].mZ, FelCrystals[x].mAddition);
			if(!FC || !FC->isAlive() || FC->GetInstanceID() != _unit->GetInstanceID())
				FC = NULL;
			else
				break;
		}
		return FC;
	}

	void CastFelExplosion()
	{
		CastSpellNowNoScheduling(FelExplosion);

		// No Idea why the mana isnt taken when the spell is cast so had to manually take it -_-
		_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_POWER1) - 3231);
	}

	void Destroy()
	{
		delete this;
	};
	
	SpellEntry* ManaRage;
	SpellDesc* ManaRageTrigger;
	SpellDesc* FelExplosion;
};

/*
	Vexallus
*/
#define BOSS_VEXALLUS 24744
#define CN_PURE_ENERGY 24745

#define VEXALLUS_CHAIN_LIGHTNING 44318
#define VEXALLUS_OVERLOAD 44353
#define VEXALLUS_ARCANE_SHOCK 44319
#define VEXALLUS_SUMMON_PURE_ENERGY 44322

class VexallusAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(VexallusAI, AscentScriptBossAI);
	VexallusAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddPhaseSpell( 1, AddSpell( VEXALLUS_CHAIN_LIGHTNING, Target_Current, 19, 0, 8, 0, 0));
		AddPhaseSpell( 1, AddSpell( VEXALLUS_ARCANE_SHOCK, Target_ClosestPlayer, 12, 0, 20, 0, 0, true, "Un...con...tainable.", Text_Yell, 12392));
		AddPhaseSpell( 2, AddSpell( VEXALLUS_OVERLOAD, Target_Self, 85, 0, 3, 0, 0));
		mPureEnergy = AddSpell( VEXALLUS_SUMMON_PURE_ENERGY, Target_Self, 85, 0, 3);
		
		AddEmote( Event_OnTargetDied, "Con...sume.", Text_Yell, 12393);

		mSummon = 0;
	}
	
	void OnCombatStart(Unit* pTarget)
	{
		Emote("Drain... life...", Text_Yell, 12389);
		SetPhase(1);
		ParentClass::OnCombatStart(pTarget);
	}
	
	void AIUpdate()
	{
		if((GetHealthPercent() <= 85  && mSummon == 0) || 
			( GetHealthPercent() <= 70 && mSummon == 1 ) ||
			( GetHealthPercent() <= 55 && mSummon == 2 ) ||
			( GetHealthPercent() <= 40 && mSummon == 3 ) || 
		( GetHealthPercent() <= 25 && mSummon == 4 ))
		{
			CastSpell(mPureEnergy);
			++mSummon;
			//SpawnCreature(CN_PURE_ENERGY, 231, -207, 6, 0, true);
		}

		if( GetHealthPercent() <= 10 && GetPhase() == 1 )
			SetPhase(2);
	
		
		ParentClass::AIUpdate();
	} 
	
	void Destroy()
	{
		delete this;
	};
	
	SpellDesc*	mPureEnergy;
	uint8		mSummon;
};

//Priestess Delrissa 
#define BOSS_Priestess_Delrissa 24560

#define PRIESTESS_DELRISSA_DISPEL_MAGIC 27609
#define PRIESTESS_DELRISSA_FLASH_HEAL 17843
#define PRIESTESS_DELRISSA_SHADOWWORD_PAIN 15654
#define PRIESTESS_DELRISSA_POWERWORD_SHIELD 44291
#define PRIESTESS_DELRISSA_RENEW 44174

class Priestess_DelrissaAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(Priestess_DelrissaAI, AscentScriptBossAI);
	Priestess_DelrissaAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(PRIESTESS_DELRISSA_DISPEL_MAGIC, Target_RandomFriendly, 35, 0, 5, 0, 30);
		AddSpell(PRIESTESS_DELRISSA_FLASH_HEAL, Target_RandomFriendly, 40, 1.5, 7, 0, 40);
		AddSpell(PRIESTESS_DELRISSA_SHADOWWORD_PAIN, Target_RandomPlayer, 45, 0, 18, 0, 30); 
		AddSpell(PRIESTESS_DELRISSA_POWERWORD_SHIELD, Target_RandomFriendly, 32, 0, 15, 0, 40);
		AddSpell(PRIESTESS_DELRISSA_RENEW, Target_RandomFriendly, 30, 0, 18, 0, 40);
		
		AddEmote(Event_OnDied, "Not what I had... planned...", Text_Yell, 12397);
		
		mClearHateList = AddTimer(15000);
		mKilledPlayers = 0;
	};

	void OnCombatStart(Unit* pTarget)
	{
		Emote("Annihilate them!", Text_Yell, 12395);
		//AggroRandomUnit();	// Want to aggro random unit ? Set it instead of calling premade
								// method that in this case recursively loops this procedure

		ParentClass::OnCombatStart(pTarget);
	};
	
	void OnTargetDied(Unit* pTarget)
	{
		if(!pTarget || !pTarget->IsPlayer())
			return;

		++mKilledPlayers;

		if(mKilledPlayers == 1)
			Emote("I call that a good start.", Text_Yell, 12405);
		else if(mKilledPlayers == 2)
			Emote("I could have sworn there were more of you.", Text_Yell, 12407);
		else if(mKilledPlayers == 3)
			Emote("Not really much of a group, anymore, is it?", Text_Yell, 12409);
		else if(mKilledPlayers == 4)
			Emote("One is such a lonely number.", Text_Yell, 12410);
		else if(mKilledPlayers == 5)
			Emote("It's been a kick, really.", Text_Yell, 12411);
		
		ParentClass::OnTargetDied(pTarget);
	};

	void OnCombatStop(Unit* pTarget)
	{
		Emote("It's been a kick, really.", Text_Yell, 12411);
		mKilledPlayers = 0;

		ParentClass::OnCombatStop(pTarget);
	};
	
	void AIUpdate()
	{
		if( IsTimerFinished(mClearHateList) )
		{
			ClearHateList();
			AggroRandomUnit();
			ResetTimer(mClearHateList, 15000);
		};

		ParentClass::AIUpdate();
	};

	void Destroy()
	{
		delete this;
	};
	
protected:
	uint8		mKilledPlayers;
	int32		mClearHateList;
};

//Kagani Nightstrike
#define CN_KaganiNightstrike 24557

#define KAGANI_NIGHTSTRIKE_Eviscerate 46189
#define KAGANI_NIGHTSTRIKE_KidneyShot 27615
#define KAGANI_NIGHTSTRIKE_Gouge	  12540 

class Kagani_NightstrikeAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(Kagani_NightstrikeAI, AscentScriptBossAI);
	Kagani_NightstrikeAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(KAGANI_NIGHTSTRIKE_KidneyShot, Target_Current, 80, 0, 25, 0, 30);
		AddSpell(KAGANI_NIGHTSTRIKE_Gouge, Target_ClosestPlayer, 20, 0, 18, 0, 30); 
		AddSpell(KAGANI_NIGHTSTRIKE_Eviscerate, Target_Current, 8, 0, 45, 0, 30);
	}
	
};

//Ellrys Duskhallow
#define CN_EllrysDuskhallow 14558

#define EllrysDuskhallow_Immolate 44267
#define EllrysDuskhallow_ShadowBolt 12471
#define EllrysDuskhallow_CurseofAgony 14875
#define EllrysDuskhallow_Fear 38595

class Ellrys_DuskhallowAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(Ellrys_DuskhallowAI, AscentScriptBossAI);
	Ellrys_DuskhallowAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(EllrysDuskhallow_Immolate, Target_Current, 75, 2, 15, 0, 30);
		AddSpell(EllrysDuskhallow_ShadowBolt, Target_RandomPlayer, 75, 3, 5, 4, 40);
		AddSpell(EllrysDuskhallow_CurseofAgony, Target_RandomPlayer, 75, 0, 4, 0, 30);
		AddSpell(EllrysDuskhallow_Fear, Target_RandomPlayer, 75, 1.5, 9, 0, 20);
	}
	
	void Destroy()
	{
		delete this;
	};
	
};

//Eramas Brightblaze 
#define CN_EramasBrightblaze 24554

#define ERAMAS_BRIGHTBLAZE_KNOCKDOWN 11428
#define ERAMAS_BRIGHTBLAZE_SNAP_KICK 46182

class Eramas_BrightblazeAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(Eramas_BrightblazeAI, AscentScriptBossAI);
	Eramas_BrightblazeAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(ERAMAS_BRIGHTBLAZE_KNOCKDOWN, Target_Current, 25, 0, 5, 0, 5);
		AddSpell(ERAMAS_BRIGHTBLAZE_SNAP_KICK, Target_SecondMostHated, 40, 0, 2, 0, 5);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Yazzai
#define CN_YAZZAI 24561

#define YAZZAI_POLYMORPH 13323
#define YAZZAI_ICE_BLOCK 27619
#define YAZZAI_BLIZZARD 44178
#define YAZZAI_CONE_OF_COLD 38384
#define YAZZAI_FROSTBOLT 15530

class YazzaiAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(YazzaiAI, AscentScriptBossAI);
	YazzaiAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(YAZZAI_POLYMORPH, Target_RandomPlayer, 30, 1.5, 16, 0, 30);
		AddSpell(YAZZAI_ICE_BLOCK, Target_Self, 20, 0, 300, 0, 1);
		AddSpell(YAZZAI_BLIZZARD, Target_RandomPlayer, 25, 0, 20, 0, 30);
		AddSpell(YAZZAI_CONE_OF_COLD, Target_Self, 10, 0, 19, 0, 1);
		AddSpell(YAZZAI_FROSTBOLT, Target_RandomPlayer, 80, 3, 14, 0, 40);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Warlord Salaris
#define CN_WARLORD_SALARIS 24559

#define WARLORD_SALARIS_INTERCEPT 27577
#define WARLORD_SALARIS_DISARM 27581
#define WARLORD_SALARIS_PIERCING_HOWL 23600
#define WARLORD_SALARIS_FRIGHTENING_SHOUT 19134
#define WARLORD_SALARIS_HAMSTRING 27584
//#define WARLORD_SALARIS_BATTLE_SHOUT 27578
#define WARLORD_SALARIS_MORTAL_STRIKE 44268

class Warlord_SalarisAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(Warlord_SalarisAI, AscentScriptBossAI);
	Warlord_SalarisAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		//AddSpell(uint32 pSpellId, TargetType pTargetType, float pChance, float pCastTime, int32 pCooldown, float pMinRange, float pMaxRange
		AddSpell(WARLORD_SALARIS_INTERCEPT, Target_RandomPlayer , 25, 0, 8, 8, 25 );
		AddSpell(WARLORD_SALARIS_DISARM, Target_Current, 100, 0, 60, 0, 5 );
		AddSpell(WARLORD_SALARIS_PIERCING_HOWL, Target_Self, 22, 0, 17, 0, 1);
		AddSpell(WARLORD_SALARIS_FRIGHTENING_SHOUT, Target_RandomPlayer, 30, 0, 9, 0, 10);
		AddSpell(WARLORD_SALARIS_HAMSTRING, Target_ClosestPlayer, 10, 0, 20, 0, 5);
		AddSpell(WARLORD_SALARIS_MORTAL_STRIKE, Target_Current, 100, 0, 6, 0, 5);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Geraxxas
#define CN_GARAXXAS 24555

#define GARAXXAS_AIMED_SHOT 44271
#define GARAXXAS_SHOOT 15620
#define GARAXXAS_CONCUSSIV_SHOT 27634
#define GARAXXAS_MULTI_SHOT 44285
#define GARAXXAS_WING_CLIP 44286


class GaraxxasAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(GaraxxasAI, AscentScriptBossAI);
	GaraxxasAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(GARAXXAS_AIMED_SHOT, Target_RandomPlayer, 90, 3, 6, 5, 35);
		AddSpell(GARAXXAS_SHOOT, Target_RandomPlayer, 90, 2.5, 5, 5, 30);
		AddSpell(GARAXXAS_CONCUSSIV_SHOT, Target_RandomPlayer, 40, 0, 8, 5, 35);
		AddSpell(GARAXXAS_MULTI_SHOT, Target_RandomPlayer, 25, 0, 12, 5, 30);
		AddSpell(GARAXXAS_WING_CLIP, Target_Current, 30, 0, 9, 0, 5);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Apoko
#define CN_APOKO 24553

#define APOKO_FROST_SHOCK 21401
#define APOKO_LESSER_HEALING_WAVE 44256
#define APOKO_PURGE 27626

class ApokoAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(ApokoAI, AscentScriptCreatureAI);
	ApokoAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		AddSpell(APOKO_FROST_SHOCK, Target_RandomPlayer, 40, 0, 8, 0, 20);
		AddSpell(APOKO_LESSER_HEALING_WAVE, Target_RandomFriendly, 50, 1.5, 10, 0, 40);
		AddSpell(APOKO_PURGE, Target_RandomUnit, 20, 0, 40, 0, 30);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Zelfan
#define CN_ZELFAN 24556

#define ZELFAN_GOBLIN_DRAGON_GUN 44272
#define ZELFAN_HIGH_EXPLOSIV_SHEEP 44276
#define ZELFAN_ROCKET_LAUNCH 44137

class ZelfanAI : public AscentScriptCreatureAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(ZelfanAI, AscentScriptCreatureAI);
	ZelfanAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
      AddSpell(ZELFAN_GOBLIN_DRAGON_GUN, Target_Current, 90, 0, 15, 0, 5);
	  AddSpell(ZELFAN_HIGH_EXPLOSIV_SHEEP, Target_Self, 90, 2, 80);
	  AddSpell(ZELFAN_ROCKET_LAUNCH, Target_RandomPlayer, 99, 3.5, 60, 0, 45);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Trash mobs

//Coilskar Witch
#define CN_COILSKAR_WITCH 24696

#define COILSKAR_WITCH_FORKED_LIGHTNING 46150
#define COILSKAR_WITCH_FROST_ARROW 44639
#define COILSKAR_WITCH_MANA_SHIELD 46151
#define COILSKAR_WITCH_SHOOT 35946

class CoilskarWitchAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(CoilskarWitchAI, AscentScriptBossAI);
	CoilskarWitchAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(COILSKAR_WITCH_FORKED_LIGHTNING, Target_Current, 60, 2, 12, 0, 30);
		AddSpell(COILSKAR_WITCH_FROST_ARROW, Target_RandomPlayer, 15, 0, 16, 0, 40);
		AddSpell(COILSKAR_WITCH_MANA_SHIELD, Target_Self, 6, 0, 40, 0, 0);
		AddSpell(COILSKAR_WITCH_SHOOT, Target_RandomPlayer, 75, 1.5, 4, 5, 30);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Sister of Torment
#define CN_SISTER_OF_TORMENT 24697

#define SISTER_OF_TORMENT_DEADLY_EMBRACE 44547
#define SISTER_OF_TORMENT_LASH_OF_PAIN 44640

class SisterOfTormentAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SisterOfTormentAI, AscentScriptBossAI);
	SisterOfTormentAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(SISTER_OF_TORMENT_LASH_OF_PAIN, Target_Current, 60, 0, 8, 0, 5);
		AddSpell(SISTER_OF_TORMENT_DEADLY_EMBRACE, Target_RandomPlayer, 20, 1.5, 16, 0, 20);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Sunblade Blood Knight
#define CN_SB_BLOOD_KNIGHT 24684

#define BLOOD_KNIGHT_HOLY_LIGHT 46029
#define BLOOD_KNIGHT_JUDGEMENT_OF_WRATH 44482 
#define BLOOD_KNIGHT_SEAL_OF_WRATH 46030

class SunbladeBloodKnightAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SunbladeBloodKnightAI, AscentScriptBossAI);
	SunbladeBloodKnightAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(BLOOD_KNIGHT_JUDGEMENT_OF_WRATH, Target_Current, 20, 0, 30, 0, 5);
		AddSpell(BLOOD_KNIGHT_SEAL_OF_WRATH, Target_Self, 99, 0, 30, 0, 0);
		AddSpell(BLOOD_KNIGHT_HOLY_LIGHT, Target_Self, 10, 2, 30, 0, 40);		
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Sunblade Imp
#define CN_SB_IMP 24815

#define IMP_FIREBOLT 44577

class SunbladeImpAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SunbladeImpAI, AscentScriptBossAI);
	SunbladeImpAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(IMP_FIREBOLT, Target_Current, 100, 2, (int32)2.5, 0, 30);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Sunblade Mage Guard 
#define CN_SB_MAGE_GUARD 24683

#define MAGE_GUARD_GLAVE_THROW 46028
#define MAGE_GUARD_MAGIC_DAMPENING_FIELD 44475

class SunbladeMageGuardAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SunbladeMageGuardAI, AscentScriptBossAI);
	SunbladeMageGuardAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(MAGE_GUARD_GLAVE_THROW, Target_Current, 60, 0, 25, 0, 5);
		AddSpell(MAGE_GUARD_MAGIC_DAMPENING_FIELD, Target_RandomPlayer, 20, 1, 35, 0, 20);
	}
	
	void Destroy()
	{
		delete this;
	};
};

//Sunblade Magister
#define CN_SB_MAGISTER 24685 

#define MAGISTER_ARCANE_NOVA 46036
#define MAGISTER_FROSTBOLT 46035

class SunbladeMagisterAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SunbladeMagisterAI, AscentScriptBossAI);
	SunbladeMagisterAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(MAGISTER_FROSTBOLT, Target_Current, 65, 2, 4, 0, 30);
		AddSpell(MAGISTER_ARCANE_NOVA, Target_Self, 12, 1.5, 40, 0, 0);
	}
	
	void Destroy()
	{
		delete this;
	};
};

void SetupMagistersTerrace(ScriptMgr* pScriptMgr) 
{
    //Bosses
	pScriptMgr->register_creature_script(BOSS_SelinFireheart, &SelinFireheartAI::Create);
	pScriptMgr->register_creature_script(BOSS_VEXALLUS, &VexallusAI::Create);
	pScriptMgr->register_creature_script(BOSS_Priestess_Delrissa, &Priestess_DelrissaAI::Create);
	//Priestess Delrissa Encounter Creature AI
	pScriptMgr->register_creature_script(CN_KaganiNightstrike, &Kagani_NightstrikeAI::Create);
	pScriptMgr->register_creature_script(CN_EllrysDuskhallow, &Ellrys_DuskhallowAI::Create);
	pScriptMgr->register_creature_script(CN_EramasBrightblaze, &Eramas_BrightblazeAI::Create);
	pScriptMgr->register_creature_script(CN_YAZZAI, &YazzaiAI::Create);
	pScriptMgr->register_creature_script(CN_WARLORD_SALARIS, &Warlord_SalarisAI::Create);
	pScriptMgr->register_creature_script(CN_GARAXXAS, &GaraxxasAI::Create);
	pScriptMgr->register_creature_script(CN_APOKO, &ApokoAI::Create);
	pScriptMgr->register_creature_script(CN_ZELFAN, &ZelfanAI::Create);
	//Trash Mobs
	pScriptMgr->register_creature_script(CN_COILSKAR_WITCH, &CoilskarWitchAI::Create);
	pScriptMgr->register_creature_script(CN_SISTER_OF_TORMENT, &SisterOfTormentAI::Create);
	pScriptMgr->register_creature_script(CN_SB_IMP, &SunbladeImpAI::Create);
	pScriptMgr->register_creature_script(CN_SB_MAGE_GUARD, &SunbladeMageGuardAI::Create);
	pScriptMgr->register_creature_script(CN_SB_MAGISTER, &SunbladeMagisterAI::Create);
}
