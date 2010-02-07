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

enum ENCOUNTER_CREATURES
{
	CN_SARTHARION		= 28860,
	CN_FLAME_TSUNAMI	= 30616,
	CN_LAVA_BLAZE		= 30643,
	CN_CYCLON			= 30648,
};

enum SARTHARION_DRAKES
{
	CN_DRAKE_TENEBRON	= 30452,
	CN_DRAKE_VESPERON	= 30449,
	CN_DRAKE_SHADRON	= 30451,
};

enum ENCOUNTER_SPELLS
{
	// Sartharion
	SARTHARION_CLEAVE				= 56909,
	SARTHARION_ENRAGE				= 61632,
	SARTHARION_AURA					= 61254,
	// Normal mode spells
	SARTHARION_FLAME_BREATH_NM		= 56908,
	SARTHARION_TAIL_LASH_NM			= 56910,
	// Heroic mode spells
	SARTHARION_FLAME_BREATH_HC		= 58956,
	SARTHARION_TAIL_LASH_HC			= 58957,
	// Tsunami spells
	TSUNAMI							= 57492,
	TSUNAMI_VISUAL					= 57494,
	// Cyclon spells
	CYCLON_AURA						= 57562,
	CYCLON_SPELL					= 57560,
	// TODO: add drake spells
};

struct Coord 
{
	float positionX;
	float positionY;
	float positionZ;
	float positionO;
};

static Coord TSUNAMI_SPAWN[] =
{
	// Right 
	{ 3283.215820f, 511.234100f, 59.288776f, 3.148659f },
	{ 3286.661133f, 533.317261f, 59.366989f, 3.156505f },
	{ 3283.311035f, 556.839611f, 59.397129f, 3.105450f },
	// Left 
	{ 3211.564697f, 505.982727f, 59.556610f, 0.000000f },
	{ 3214.280029f, 531.491089f, 59.168331f, 0.000000f },
	{ 3211.609131f, 560.359375f, 59.420803f, 0.000000f },
};

static Coord TSUNAMI_MOVE[] =
{
	// Left  if right 
	{ 3211.564697f, 505.982727f, 59.556610f, 3.148659f },
	{ 3214.280029f, 531.491089f, 59.168331f, 3.156505f },
	{ 3211.609131f, 560.359375f, 59.420803f, 3.105450f },
	// Right 1 if left 1
	{ 3283.215820f, 511.234100f, 59.288776f, 3.148659f },
	{ 3286.661133f, 533.317261f, 59.366989f, 3.156505f },
	{ 3283.311035f, 556.839611f, 59.397129f, 3.105450f }
};

static Coord LAVA_SPAWNS[]	=
{
	{ 3234.500488f, 516.904297f, 58.713600f, 0.955027f },
	{ 3255.424805f, 534.890137f, 58.886429f, 4.215994f },
	{ 3250.638672f, 554.778381f, 58.550964f, 2.790497f },
	{ 3252.041748f, 515.223572f, 58.552681f, 3.670144f },
	{ 3237.217285f, 507.188324f, 58.793762f, 0.956592f },
	{ 3245.539307f, 517.599243f, 58.707531f, 1.400339f },
	{ 3236.014648f, 529.310181f, 58.736656f, 4.736738f },
	{ 3244.688721f, 546.702637f, 58.394291f, 1.489113f }
};

class SartharionAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(SartharionAI, AscentScriptBossAI);
	SartharionAI(Creature *pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(SARTHARION_CLEAVE, Target_Current, 24, 0, 8 );

		if( IsHeroic() )
		{
			mFlame = AddSpell( SARTHARION_FLAME_BREATH_HC, Target_Self, 18, 2, 16); 
			mFlame->AddEmote("Burn, you miserable wretches!", Text_Yell, 14098);
			AddSpell( SARTHARION_TAIL_LASH_HC, Target_Self, 40, 0, 12);
		}
		else
		{
			mFlame = AddSpell( SARTHARION_FLAME_BREATH_NM, Target_Self, 18, 2, 16); 
			mFlame->AddEmote("Burn, you miserable wretches!", Text_Yell, 14098);
			AddSpell( SARTHARION_TAIL_LASH_NM, Target_Self, 40, 0, 12);
		};
			

		AddEmote(Event_OnTargetDied, "You will make a fine meal for the hatchlings.", Text_Yell, 14094);
		AddEmote(Event_OnTargetDied, "You are at a grave disadvantage!", Text_Yell, 14096);
		AddEmote(Event_OnTargetDied, "This is why we call you lesser beings.", Text_Yell, 14097);
			
		AddEmote(Event_OnCombatStart, "It is my charge to watch over these eggs. I will see you burn before any harm comes to them!", Text_Yell, 14093);
	};

	void OnCombatStart(Unit* pTarget)
	{
		i_drakeCount = CheckDrakes();
		if( i_drakeCount >= 1 ) //HardMode!
			mDrakeTimer = AddTimer(30000);

		mTsunamiTimer = AddTimer(28000);
		mAddTimer = AddTimer(12000);

		ParentClass::OnCombatStart(pTarget);
	};

	void AIUpdate()
	{
		if( i_drakeCount >= 1)
		{
			if( IsTimerFinished(mDrakeTimer) )
			{
				if( b_tenebron == true )
					CallTenebron();
				else if( b_shadron == true )
					CallShadron();
				else if( b_vesperon == true )
					CallVesperon();

				ResetTimer( mDrakeTimer, 45000 );
			};
		};

		if(IsTimerFinished(mTsunamiTimer))
		{
			_unit->SendChatMessage(CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, "The lava surrounding Sartharion churns!");

			uint32 RandomSpeach = rand()%4;
			switch (RandomSpeach)		
			{
			case 0:
				Emote( "Such flammable little insects....", Text_Yell, 14100);	
				break;
			case 1:
				Emote( "Your charred bones will litter the floor!", Text_Yell, 14101);
				break;
			case 2:
				Emote( "How much heat can you take?", Text_Yell, 14102);
				break;
			case 3:
				Emote( "All will be reduced to ash!", Text_Yell, 14103);
				break;				
			};
			
			uint32 RndSide = rand()%2;
			Creature* Tsunami = NULL;

			for(int i = 0; i < 3; ++i)
			{
				switch(RndSide)
				{
				case 0:
					Tsunami = this->GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_TSUNAMI, TSUNAMI_SPAWN[i].positionX, TSUNAMI_SPAWN[i].positionY, TSUNAMI_SPAWN[i].positionZ, TSUNAMI_SPAWN[i].positionO, true, true, NULL, NULL);  
					
					if( Tsunami != NULL )
						Tsunami->GetAIInterface()->MoveTo( TSUNAMI_MOVE[i].positionX, TSUNAMI_MOVE[i].positionY, TSUNAMI_MOVE[i].positionZ, TSUNAMI_MOVE[i].positionO );
					break;
				case 1:
					Tsunami = this->GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_TSUNAMI, TSUNAMI_SPAWN[i + 3].positionX, TSUNAMI_SPAWN[i + 3].positionY, TSUNAMI_SPAWN[i + 3].positionZ, TSUNAMI_SPAWN[i + 3].positionO, true, true, NULL, NULL);  
					
					if( Tsunami != NULL )
						Tsunami->GetAIInterface()->MoveTo( TSUNAMI_MOVE[i + 3].positionX, TSUNAMI_MOVE[i + 3].positionY, TSUNAMI_MOVE[i + 3].positionZ, TSUNAMI_MOVE[i + 3].positionO );
				};
			};

			ResetTimer(mTsunamiTimer, 32000);
		};

		if( IsTimerFinished(mAddTimer) )
		{			
			for(uint32 i = 0; i < 2; ++i)
			{
				uint32 j = rand()%8;
				SpawnCreature(CN_LAVA_BLAZE, LAVA_SPAWNS[j].positionX, LAVA_SPAWNS[j].positionY, LAVA_SPAWNS[j].positionZ, LAVA_SPAWNS[j].positionO, true);
			};

			ResetTimer(mAddTimer, 15000);
		};
			
		if( IsTimerFinished(mAddTimer) && GetHealthPercent() <= 10 ) // enrage phase
		{
			for(uint32 i = 0; i < 6; ++i)
			{
				uint32 j = rand()%8;
				SpawnCreature(CN_LAVA_BLAZE, LAVA_SPAWNS[j].positionX, LAVA_SPAWNS[j].positionY, LAVA_SPAWNS[j].positionZ, LAVA_SPAWNS[j].positionO, true);
			};

			RemoveTimer(mAddTimer);
		};

		ParentClass::AIUpdate();		
	};

	int CheckDrakes()
	{
		i_drakeCount = 0;
		
		Tenebron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3248.687256f, 533.557983f, 58.961288f, CN_DRAKE_TENEBRON);
		if(Tenebron != NULL && Tenebron->isAlive()) 
		{
				b_tenebron = true;
				i_drakeCount++;
		};

		Vesperon = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3248.687256f, 533.557983f, 58.961288f, CN_DRAKE_VESPERON);
		if(Vesperon != NULL && Vesperon->isAlive())
		{
				b_vesperon = true;
				i_drakeCount++;
		};

		Shadron = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( 3248.687256f, 533.557983f, 58.961288f, CN_DRAKE_SHADRON);
		if(Shadron != NULL && Shadron->isAlive())
		{
				b_shadron = true;
				i_drakeCount++;
		};
		
		return i_drakeCount;
	};

	void CallTenebron()
	{
		if(Tenebron != NULL && Tenebron->isAlive())
		{
			Emote( "Tenebron! The eggs are yours to protect as well!", Text_Yell, 14106);
			Tenebron->GetAIInterface()->MoveTo( 3254.606689f, 531.867859f, 66.898163f, 4.215994f);
			Tenebron->CastSpell( Tenebron, 61248, true);
		};
		b_tenebron = false;
	};

	void CallShadron()
	{
		if(Shadron != NULL && Shadron->isAlive())
		{
			Emote( "Vesperon! The clutch is in danger! Assist me!", Text_Yell, 14104);
			Shadron->GetAIInterface()->MoveTo( 3254.606689f, 531.867859f, 66.898163f, 4.215994f);
			Shadron->CastSpell( Shadron, 58105, true);
		};
		b_shadron = false;
	};

	void CallVesperon()
	{
		if(Vesperon != NULL && Vesperon->isAlive())
		{
			Emote( "Shadron! Come to me, all is at risk!", Text_Yell, 14105);
			Vesperon->GetAIInterface()->MoveTo( 3254.606689f, 531.867859f, 66.898163f, 4.215994f);
			Vesperon->CastSpell(Vesperon, 61251, true);
		};
		b_vesperon = false;
	};

	void OnDied(Unit* pKiller)
	{
		Emote( "Such is the price... of failure...", Text_Yell, 14107);
		
		RemoveAIUpdateEvent();
		ParentClass::OnDied(pKiller);
	};

	void Destroy()
	{
		delete this;
	};

private:
	bool b_tenebron, b_vesperon, b_shadron;
	int32 mTsunamiTimer, mAddTimer, mDrakeTimer;
	int	i_drakeCount;
	
	Creature *Tenebron, *Vesperon, *Shadron;
	SpellDesc* mFlame;
};	

class TsunamiAI : public AscentScriptBossAI
{
	ASCENTSCRIPT_FACTORY_FUNCTION(TsunamiAI, AscentScriptBossAI);
	TsunamiAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{};
	
	void OnLoad()
	{
		RegisterAIUpdateEvent(1000);
		_unit->Despawn( 11500, 0 );
		SetFlyMode(true);
		SetCanEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		
		
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TSUNAMI), true);
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TSUNAMI_VISUAL), true);

		ParentClass::OnLoad();
	};

	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TSUNAMI), true);
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(TSUNAMI_VISUAL), true);

		ParentClass::AIUpdate();
	};

	void Destroy()
	{
		delete this;
	};
};	

class CyclonAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(CyclonAI, AscentScriptBossAI);
	CyclonAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{};

	void OnLoad()
	{
		RegisterAIUpdateEvent(1000);
		SetCanMove(false);
		SetCanEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLON_SPELL), true); 
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLON_AURA), true);

		ParentClass::OnLoad();
	};

	void AIUpdate()
	{
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLON_SPELL), true); 
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(CYCLON_AURA), true);

		ParentClass::AIUpdate();
	};

	void Destroy()
	{
		delete this;
	};
};

class LavaBlazeAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(LavaBlazeAI, AscentScriptBossAI);
	LavaBlazeAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{};

	void OnLoad()
	{
		AggroNearestPlayer(1);
		ParentClass::OnLoad();
	};

	void OnCombatStop(Unit* pTarget)
	{
		Despawn( 1000, 0 );
	};

	void OnDied(Unit* pKiller)
	{
		Despawn( 1000, 0 );
	};

	void Destroy()
	{
		delete this;
	};
};

void SetupTheObsidianSanctum(ScriptMgr * mgr)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	///////// Mobs
	//////////////////////////////////////////////////////////////////////////////////////////
	///////// Bosses
	mgr->register_creature_script(CN_SARTHARION, &SartharionAI::Create);
	mgr->register_creature_script(CN_FLAME_TSUNAMI, &TsunamiAI::Create);
	mgr->register_creature_script(CN_CYCLON, &CyclonAI::Create);
	mgr->register_creature_script(CN_LAVA_BLAZE, &LavaBlazeAI::Create);
};