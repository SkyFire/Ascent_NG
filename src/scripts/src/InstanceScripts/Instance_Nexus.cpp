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

#define ANOMALUS_CS		188527
#define TELESTRA_CS		188526
#define ORMOK_CS		188528			

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Anomalus ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CN_ANOMALUS				26763
#define CN_CHAOTIC_RIFT			26918
#define CN_CRAZED_MANA_WRAITH	26746

#define SPARK					47751 
#define SPARK_HC				57062
#define CHAOTIC_ENERGY_BURST	47688
#define CHAOTIC_RIFT_AURA		47687
#define SUMMON_MANA_WRAITH		47692

class AnomalusAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(AnomalusAI, AscentScriptBossAI);
	AnomalusAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
    {
		if( IsHeroic() )
			AddSpell(SPARK_HC, Target_RandomPlayer, 80, 0, 3);
		else
			AddSpell(SPARK, Target_RandomPlayer, 80, 0, 3);

		AddEmote( Event_OnDied, "Of course.", Text_Yell, 13187 );
    };
    
    void OnCombatStart(Unit* mTarget)
    {
		Emote("Chaos beckons.", Text_Yell, 13186);
		summon = 0;
		mRift = false;

		ParentClass::OnCombatStart(mTarget);
    };

    void AIUpdate()
    {
		if((GetHealthPercent() <= 76 && summon == 0) 
		|| (GetHealthPercent() <= 51 && summon == 2) 
		|| (GetHealthPercent() <= 25 && summon == 4))
			summon += 1;
		
		if(summon == 1 || summon == 3 || summon == 5) 
			Summon();

		if( mRift == true && ( pChaoticRift == NULL || !pChaoticRift->isAlive() ))
		{
			RemoveAura(47748);
			mRift = false;
		};

		ParentClass::AIUpdate();
    };

	void Summon()
	{
		Emote("Reality... unwoven.", Text_Yell, 13188);

		float x = _unit->GetPositionX() + 13.5f;
		_unit->CastSpell( _unit, 47748, true); // me immune

		pChaoticRift = _unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_CHAOTIC_RIFT, x, _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, true, NULL, NULL);
			

		mRift = true;
		summon += 1;
	};

	void OnDied(Unit* pKiller)
	{
		GameObject* pContainmentSphere = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 321.012451f, 12.578364f, -16.591925f, ANOMALUS_CS );
		if( pContainmentSphere != NULL )
			pContainmentSphere->SetState(1);
	
		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };

private:
	int				summon;
	bool			mRift;
	Creature* pChaoticRift;
};

class ChaoticRiftAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(ChaoticRiftAI, AscentScriptBossAI);
	ChaoticRiftAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell( SUMMON_MANA_WRAITH, Target_Self, 30, 0, dbcSpell.LookupEntryForced(SUMMON_MANA_WRAITH)->RecoveryTime );
		AddSpell( CHAOTIC_ENERGY_BURST, Target_RandomPlayer, 30, 0, dbcSpell.LookupEntryForced(CHAOTIC_ENERGY_BURST)->RecoveryTime );
	};

	void OnLoad()
	{
		ApplyAura( CHAOTIC_RIFT_AURA );
		Despawn( 40000, 0 );
		ParentClass::OnLoad();
	};

    void OnDied(Unit*  mKiller)
    {
		Despawn( 2000, 0 );
		ParentClass::OnDied(mKiller);
    };

	void OnCombatStop(Unit* pTarget)
	{
		Despawn( 2000, 0 );
		ParentClass::OnCombatStop(pTarget);
	};

	void Destroy()
    {
		delete this;
    };
};

class CraziedManaWrathAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(CraziedManaWrathAI, AscentScriptBossAI);
	CraziedManaWrathAI(Creature* pCreature) : AscentScriptBossAI(pCreature){};

	void OnCombatStop(Unit* pTarget)
	{
		Despawn();
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(Unit*  mKiller)
    {
		Despawn();
		ParentClass::OnDied(mKiller);
	};

	void Destroy()
    {
		delete this;
    };
};


////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Grand Magus Telestra //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

struct Spawns
{
	float x;
	float y;
	float z;
	float o;
};

static Spawns FormSpawns[] =	
{
	{ 494.726990f, 89.128799f, -15.941300f, 6.021390f },
	{ 495.006012f, 89.328102f, -16.124609f, 0.027486f },
	{ 504.798431f, 102.248375f, -16.124609f, 4.629921f }
};

#define CN_TELESTRA 26731
#define CN_TELESTRA_FROST 26930
#define CN_TELESTRA_FIRE 26928
#define CN_TELESTRA_ARCANE 26929

#define GRAVITY_WELL 47756

// Normal mode spells 
#define ICE_NOVA 47772
#define FIREBOMB 47773

// Heroic mode spells
#define ICE_NOVA_HC 56935
#define FIREBOMB_HC 56934

// Arcane spells
#define CRITTER 47731
#define TIME_STOP 47736

// Fire
#define FIRE_BLAST 47721
#define FIRE_BLAST_HC 56939
#define SCORCH 47723
#define SCORCH_HC 56938

// Frost 
#define BLIZZARD 47727
#define BLIZZARD_HC 56936
#define ICE_BARB 47729
#define ICE_BARB_HC 56937

class TelestraBossAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(TelestraBossAI, AscentScriptBossAI);
	TelestraBossAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		if(IsHeroic())
		{
			AddSpell(ICE_NOVA_HC, Target_Self, 25, 2.0, 15);
			AddSpell(FIREBOMB_HC, Target_RandomPlayer, 35, 1.5, 5);
			AddSpell(GRAVITY_WELL, Target_Self, 15, 0.5, 20);
		}
		else
		{
			AddSpell(ICE_NOVA, Target_Self, 25, 2.0, 15);
			AddSpell(FIREBOMB, Target_RandomPlayer, 35, 1.5, 5);
			AddSpell(GRAVITY_WELL, Target_Self, 15, 0.5, 20);
		};

		SetAIUpdateFreq(1000);

		AddEmote(Event_OnCombatStart, "You know what they say about curiosity. ", Text_Yell, 13319);
		AddEmote(Event_OnDied, "Damn the... luck.", Text_Yell, 13320);
		AddEmote(Event_OnTargetDied, "Death becomes you!", Text_Yell, 13324);

		pReturned = false;
	};

	void AIUpdate()
	{
		if(GetHealthPercent() <= 50 && GetPhase() == 1)
		{
			const char * Text = "";
			uint32 pSoundID = 0;
			uint32 Random = rand()%2;
			switch (Random)	
			{
				case 0:
					Text =  "I'll give you more than you can handle.";
					pSoundID = 13321;
					break;
				case 1:
					Text = "There's plenty of me to go around.";
					pSoundID = 13322;
					break;
			}
			
			Emote(Text, Text_Yell, pSoundID);

			SetPhase(2);
			SetCanMove(false);
			ApplyAura(60191);

			pFire = _unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_TELESTRA_FIRE, 494.726990f, 89.128799f, -15.941300f, 6.021390f, true, true, NULL, NULL );
			pFrost = _unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_TELESTRA_FROST, 495.006012f, 89.328102f, -16.124609f, 0.027486f, true, true, NULL, NULL );
			pArcane = _unit->GetMapMgr()->GetInterface()->SpawnCreature( CN_TELESTRA_ARCANE, 504.798431f, 102.248375f, -16.124609f, 4.629921f, true, true, NULL, NULL );
		};

		if( GetPhase() == 2 )
		{
			if( ( pFrost != NULL && pFrost->isAlive() ) || ( pFire != NULL && pFire->isAlive() ) || ( pArcane != NULL && pArcane->isAlive() ) )
				return;

			Emote("Now to finish the job!", Text_Yell, 13323);
			RemoveAura(60191);
			SetCanMove(true);
			SetPhase(3);
		};

		ParentClass::AIUpdate();
	};

	void OnDied(Unit* pKiller)
	{
		GameObject* pContainmentSphere = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 284.700287f, -25.500309f, -16.606443f, TELESTRA_CS );
		if( pContainmentSphere != NULL )
			pContainmentSphere->SetState(1);

		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };

private:
	bool	pReturned;
	Creature		*pFire, *pFrost, *pArcane;
};

class TelestraFireAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(TelestraFireAI, AscentScriptBossAI);
	TelestraFireAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		if( IsHeroic() )
		{
			AddSpell(FIRE_BLAST_HC, Target_RandomPlayer, 30, 0, 14);
			AddSpell(SCORCH_HC, Target_Current, 100, 1, 3);
		}
		else
		{
			AddSpell(FIRE_BLAST, Target_RandomPlayer, 30, 0, 14);
			AddSpell(SCORCH, Target_Current, 100, 1, 3);
		};
	};

	void OnLoad()
	{
		AggroNearestUnit();
		ParentClass::OnLoad();
	};

	void OnCombatStop(Unit* pTarget)
	{
		Despawn();
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(Unit* pKiller)
	{
		Despawn();
		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };
};

class TelestraFrostAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(TelestraFrostAI, AscentScriptBossAI);
	TelestraFrostAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		if( IsHeroic() )
		{
			AddSpell(BLIZZARD_HC, Target_RandomPlayerDestination, 20, 0, 20);
			AddSpell(ICE_BARB_HC, Target_RandomPlayer, 25, 0.5, 6);
		}
		else
		{
			AddSpell(BLIZZARD, Target_RandomPlayerDestination, 20, 0, 20);
			AddSpell(ICE_BARB, Target_RandomPlayer, 25, 0.5, 6);
		};
	};

	void OnLoad()
	{
		AggroNearestUnit();
		ParentClass::OnLoad();
	};

	void OnCombatStop(Unit* pTarget)
	{
		Despawn();
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(Unit* pKiller)
	{
		Despawn();
		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };
};

class TelestraArcaneAI : public AscentScriptBossAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(TelestraArcaneAI, AscentScriptBossAI);
	TelestraArcaneAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		AddSpell(TIME_STOP, Target_Self, 30, 1.5, 30);
	};

	void OnLoad()
	{
		AggroNearestUnit();
		ParentClass::OnLoad();
	};

	void OnCombatStop(Unit* pTarget)
	{
		Despawn();
		ParentClass::OnCombatStop(pTarget);
	};

	void OnDied(Unit* pKiller)
	{
		Despawn();
		ParentClass::OnDied(pKiller);
	};

	void Destroy()
    {
		delete this;
    };
};

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Ormorok the Tree-Shaper ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// creatures
#define CN_ORMOROK					26794
#define CN_CRYSTAL_SPIKE			27099

// spells
#define SPELL_REFLECTION			47981
#define FRENZY						48017

// normal mode spells
#define TRAMPLE						48016
#define CRYSTAL_SPIKES				47958

// heroic mode spells
#define TRAMPLE_H					57066
#define CRYSTAL_SPIKES_H			57082

// Crystal Spike spells
#define SPELL_CRYSTAL_SPIKE_VISUAL	50442
#define SPELL_CRYSTAL_SPIKE			47944
#define SPELL_CRYSTAL_SPIKE_H		57067

void SpellFunc_CrystalSpikes( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	if(pCreatureAI != NULL)
	{
		if( pTarget == NULL )
			return;

		for (int i = -2; i < 2; i++)
		{
			float x = pTarget->GetPositionX() + ( i * 3.0 );
			float y = pTarget->GetPositionY() + ( i * 3.0 );

			pCreatureAI->GetUnit()->GetMapMgr()->GetInterface()->SpawnCreature( CN_CRYSTAL_SPIKE, x, y, pTarget->GetPositionZ(), pTarget->GetOrientation(), true, true, NULL, NULL );
		};
		pCreatureAI->Emote( "Bleed!", Text_Yell, 13332 );
	};
};


class OrmorokAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(OrmorokAI, AscentScriptBossAI);
	OrmorokAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		if ( IsHeroic() )
			AddSpell(TRAMPLE_H, Target_Current, 30, 0, 9 );
		else
			AddSpell(TRAMPLE, Target_Current, 30, 0, 9 );

		AddSpell( SPELL_REFLECTION, Target_Self, 35, 2.0f, 15 );

		mCrystalSpikes = AddSpellFunc( &SpellFunc_CrystalSpikes, Target_RandomPlayerNotCurrent, 25, 0, 12 );

		AddEmote( Event_OnCombatStart, "Noo!", Text_Yell, 13328 );
		AddEmote( Event_OnDied, "Aaggh!", Text_Yell, 13330 );

		mEnraged = false;
	};

	void OnCombatStart(Unit* pTarget)
	{
		mEnraged = false;
		ParentClass::OnCombatStart(pTarget);
	};

	void AIUpdate()
	{
		if( GetHealthPercent() <= 25 && mEnraged == false )
		{
			ApplyAura( FRENZY );
			mEnraged = true;
		};

		ParentClass::AIUpdate();
	};

	void OnDied(Unit* pKiller)
	{
		GameObject* pContainmentSphere = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 282.852997f, 12.378636f, -16.594494f, ORMOK_CS );
		if( pContainmentSphere != NULL )
			pContainmentSphere->SetState(1);

		ParentClass::OnDied(pKiller);
	};

	void Destroy()
	{
		delete this;
	};

private:
	SpellDesc*	mCrystalSpikes;
	bool		mEnraged;
};

class CrystalSpikeAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(CrystalSpikeAI, AscentScriptBossAI);
	CrystalSpikeAI(Creature* pCreature) : AscentScriptBossAI(pCreature){};

	void OnLoad()
	{
		SetCanEnterCombat( false );
		SetCanMove( false );
		_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );

		Despawn( 4500, 0 );
		RegisterAIUpdateEvent( 500 );
		m_part = 0;

		ParentClass::OnLoad();
	};

	void AIUpdate()
	{
		m_part += 1;

		if( m_part == 1 )
		{
			_unit->CastSpell( _unit, SPELL_CRYSTAL_SPIKE_VISUAL, true );
		}
		else if( m_part == 5 )
		{
			Player* _plr = GetNearestPlayer();
			
			if ( IsHeroic() )
			{
				_unit->CastSpell( _plr, dbcSpell.LookupEntry(SPELL_CRYSTAL_SPIKE_H), true );
			}
			else
			{
				_unit->CastSpell( _plr, dbcSpell.LookupEntry(SPELL_CRYSTAL_SPIKE), true );
			};
		};
	};

	void Destroy()
	{
		delete this;
	};

private:
	int		m_part;
};

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Keristrasza //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#define CN_KERISTRASZA				26723

#define TAIL_SWEEP					50155
#define INTENSE_COLD				48094
#define CRYSTAL_CHAINS				50997
#define CRYSTALLIZE					48179
#define ENRAGE						8599

// normal mode
#define CRYSTALFIRE_BREATH			48096
//heroic
#define CRYSTALFIRE_BREATH_HC		57091

class KeristraszaAI : public AscentScriptBossAI
{
    ASCENTSCRIPT_FACTORY_FUNCTION(KeristraszaAI, AscentScriptBossAI);
	KeristraszaAI(Creature* pCreature) : AscentScriptBossAI(pCreature)
	{
		if ( IsHeroic() )
			AddSpell( CRYSTALFIRE_BREATH_HC, Target_Self, 30, 1, 14 );
		else
			AddSpell( CRYSTALFIRE_BREATH, Target_Self, 30, 1, 14 );

		AddSpell( CRYSTAL_CHAINS, Target_RandomPlayer, 30, 0, 12);
		AddSpell( TAIL_SWEEP, Target_Self, 40, 0, 8 );

		mCrystalize = AddSpell( CRYSTALLIZE, Target_Self, 25, 0, 22 );
		mCrystalize->AddEmote( "Stay. Enjoy your final moments.", Text_Yell, 13451 );

		AddEmote(Event_OnCombatStart, "Preserve? Why? There's no truth in it. No no no... only in the taking! I see that now!", Text_Yell, 13450);
		AddEmote(Event_OnTargetDied, "Now we've come to the truth!", Text_Yell, 13453);
		AddEmote(Event_OnDied, "Dragonqueen... Life-Binder... preserve... me.", Text_Yell, 13454);

		mGoCount = 3;
		mEnraged = false;
		SetCanEnterCombat(false);
	};

	void OnLoad()
	{
		ApplyAura( 47543 ); // frozen prison
		ParentClass::OnLoad();
	};

	void AIUpdate()
	{
		if( mEnraged == false && GetHealthPercent() <= 25 )
		{
			ApplyAura( ENRAGE );
			mEnraged = true;
		};
	};

	void UpdateGO()
	{
		--mGoCount;

		if( mGoCount == 0 ) // start encounter
		{
			SetCanEnterCombat( true );
			RemoveAura( 47543 );
			ApplyAura( INTENSE_COLD );
		};
	};

private:
	int			mGoCount;
	bool		mEnraged;
	SpellDesc*	mCrystalize;
};

class ContainmentSphereAI : public GameObjectAIScript
{
public:
	ContainmentSphereAI(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new ContainmentSphereAI(GO); }

	void OnActivate(Player* pPlayer)	
	{
		KeristraszaAI* Keristrasza = (KeristraszaAI*)_gameobject->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(302.235321f, -5.804970f, -15.564095f, CN_KERISTRASZA)->GetScript();
		if( Keristrasza != NULL )
		{
			Keristrasza->UpdateGO();
			_gameobject->SetState(0);
			_gameobject->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
		};
	};
};

void SetupNexus(ScriptMgr * mgr)
{
	// Anomalus Encounter
    mgr->register_creature_script(CN_ANOMALUS, &AnomalusAI::Create);
	mgr->register_creature_script(CN_CHAOTIC_RIFT, &ChaoticRiftAI::Create);
	mgr->register_creature_script(CN_CRAZED_MANA_WRAITH, &CraziedManaWrathAI::Create);

	// Grand Magus Telestra Encounter 
	mgr->register_creature_script(CN_TELESTRA, &TelestraBossAI::Create);
	mgr->register_creature_script(CN_TELESTRA_ARCANE, &TelestraArcaneAI::Create);
	mgr->register_creature_script(CN_TELESTRA_FROST, &TelestraFrostAI::Create);
	mgr->register_creature_script(CN_TELESTRA_FIRE, &TelestraFireAI::Create);

	// Ormorok the Tree-Shaper Encounter
	mgr->register_creature_script(CN_ORMOROK, &OrmorokAI::Create); 
	mgr->register_creature_script(CN_CRYSTAL_SPIKE, &CrystalSpikeAI::Create);

	// Keristrasza Encounter
	mgr->register_creature_script(CN_KERISTRASZA, &KeristraszaAI::Create); 

	mgr->register_gameobject_script(TELESTRA_CS, &ContainmentSphereAI::Create);
	mgr->register_gameobject_script(ANOMALUS_CS, &ContainmentSphereAI::Create);
	mgr->register_gameobject_script(ORMOK_CS, &ContainmentSphereAI::Create);
}
