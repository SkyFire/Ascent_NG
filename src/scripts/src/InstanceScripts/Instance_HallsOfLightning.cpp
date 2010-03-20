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

//////////////////////////////////////////
// TRASH MOBS
//////////////////////////////////////////

//////////////////////////////////////////
// BOSSES
//////////////////////////////////////////

#define GENERAL_BJARNGRIM_ENTRY 28586
#define STANCE_CHANGE_INTERVAL 20000//change stance each 20s
#define PHASES_COUNT 3

class GENERAL_BJARNGRIM_AI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GENERAL_BJARNGRIM_AI);

    GENERAL_BJARNGRIM_AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		heroic = ( _unit->GetMapMgr()->iInstanceMode == MODE_HEROIC );
		stances_timer = 0;
		phase = 0;
		spells.clear();
		/* SPELLS INIT */
		//all
		ScriptSpell* Ironform = new ScriptSpell;
		Ironform->normal_spellid = 52022;
		Ironform->heroic_spellid = 52022;
		Ironform->chance = 60;
		Ironform->timer = 8000;
		Ironform->time = 0;
		Ironform->phase = PHASES_COUNT+1;
		Ironform->target = SPELL_TARGET_SELF;
		spells.push_back(Ironform);

		ScriptSpell* KnockAway = new ScriptSpell;
		KnockAway->normal_spellid = 52022;
		KnockAway->heroic_spellid = 52022;
		KnockAway->chance = 50;
		KnockAway->timer = 4000;
		KnockAway->time = 0;
		KnockAway->phase = PHASES_COUNT+1;
		KnockAway->target = SPELL_TARGET_CURRENT_ENEMY;
		spells.push_back(KnockAway);

		ScriptSpell* Slam = new ScriptSpell;
		Slam->normal_spellid = 52026;
		Slam->heroic_spellid = 52026;
		Slam->chance = 95;
		Slam->timer = 5000;
		Slam->time = 0;
		Slam->phase = PHASES_COUNT+1;
		Slam->target = SPELL_TARGET_CURRENT_ENEMY;
		spells.push_back(Slam);

		//defensive stance
		ScriptSpell* SpellReflection = new ScriptSpell;
		SpellReflection->normal_spellid = 36096;
		SpellReflection->heroic_spellid = 36096;
		SpellReflection->chance = 50;
		SpellReflection->timer = 4000;
		SpellReflection->time = 0;
		SpellReflection->phase = 1;
		SpellReflection->target = SPELL_TARGET_SELF;
		spells.push_back(SpellReflection);

		ScriptSpell* Intercept = new ScriptSpell;
		Intercept->normal_spellid = 58769;
		Intercept->heroic_spellid = 58769;
		Intercept->chance = 40;
		Intercept->timer = 5000;
		Intercept->time = 0;
		Intercept->phase = 1;
		Intercept->target = SPELL_TARGET_RANDOM_PLAYER;
		spells.push_back(Intercept);

		ScriptSpell* Pummel = new ScriptSpell;
		Pummel->normal_spellid = 12555;
		Pummel->heroic_spellid = 12555;
		Pummel->chance = 30;
		Pummel->timer = 3000;
		Pummel->time = 0;
		Pummel->phase = 1;
		Pummel->target = SPELL_TARGET_RANDOM_PLAYER;
		spells.push_back(Pummel);

		//Berserker Stance
		ScriptSpell* Cleave = new ScriptSpell;
		Cleave->normal_spellid = 12555;
		Cleave->heroic_spellid = 12555;
		Cleave->chance = 70;
		Cleave->timer = 4000;
		Cleave->time = 0;
		Cleave->phase = 2;
		Cleave->target = SPELL_TARGET_CURRENT_ENEMY;
		spells.push_back(Cleave);

		//Battle Stance
		ScriptSpell* MortalStrike = new ScriptSpell;
		MortalStrike->normal_spellid = 16856;
		MortalStrike->heroic_spellid = 16856;
		MortalStrike->chance = 70;
		MortalStrike->timer = 4000;
		MortalStrike->time = 0;
		MortalStrike->phase = 3;
		MortalStrike->target = SPELL_TARGET_CURRENT_ENEMY;
		spells.push_back(MortalStrike);

		ScriptSpell* Whirlwind = new ScriptSpell;
		Whirlwind->normal_spellid = 52028;
		Whirlwind->heroic_spellid = 52027;
		Whirlwind->chance = 80;
		Whirlwind->timer = 9000;
		Whirlwind->time = 0;
		Whirlwind->phase = 3;
		Whirlwind->target = SPELL_TARGET_CURRENT_ENEMY;
		spells.push_back(Whirlwind);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I am the greatest of my father's sons! Your end has come!");
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		phase = 0;
		ChangeStance();
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
		_unit->RemoveAllAuras();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "How can it be...? Flesh is not... stronger!");
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		//BUAHAHAHAH
		if(Rand(1))
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So ends your curse!");
		else
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flesh... is... weak!");
	}

    void AIUpdate()
	{
		if( spells.size() > 0)
		{
			for( uint8 i = 0; i<spells.size(); i++ )
			{
				if( spells[i]->time < getMSTime() && (spells[i]->phase == phase || spells[i]->phase > PHASES_COUNT))
				{
					if( Rand( spells[i]->chance ) )
					{
						CastScriptSpell( spells[i] );
						spells[i]->time = getMSTime() + spells[i]->timer;
					}
				}
			}
		}
		if( stances_timer < getMSTime() )
		{
			ChangeStance();
		}
    }

	Player* GetRandomPlayerTarget()
	{
		//this SUCKS ASS!!
		vector< uint32 > possible_targets;
		for( unordered_set<Player*>::iterator iter = _unit->GetInRangePlayerSetBegin(); iter != _unit->GetInRangePlayerSetEnd(); ++iter ) 
		{ 
			if( (*iter) && (*iter)->isAlive() )
				possible_targets.push_back( (uint32)(*iter)->GetGUID() );
		}
		if( possible_targets.size() > 0 )
		{
			uint32 random_player = possible_targets[ Rand( uint32(possible_targets.size() - 1) ) ];
			return _unit->GetMapMgr()->GetPlayer( random_player );
		}
		return NULL;
	}

	void CastScriptSpell( ScriptSpell* spell )
	{
		_unit->Root();
		uint32 spellid = heroic ? spell->heroic_spellid : spell->normal_spellid;
		Unit* spelltarget = NULL;
		switch( spell->target )
		{
		case SPELL_TARGET_SELF:
			{
				spelltarget = _unit;
			}break;
		case SPELL_TARGET_GENERATE:
			{
				spelltarget = NULL;
			}break;
		case SPELL_TARGET_CURRENT_ENEMY:
			{
				spelltarget = _unit->GetAIInterface()->GetNextTarget();
			}break;
		case SPELL_TARGET_RANDOM_PLAYER:
			{
				spelltarget = GetRandomPlayerTarget();
			}break;
		}
		_unit->CastSpell( spelltarget , spellid, false );
		_unit->UnRoot();
	}
	void ChangeStance()
	{
		_unit->RemoveAllAuras();
		phase++;
		uint32 stance_id = 0;
		uint32 aura_id = 0;
		if( phase > 3 ) phase=1;
		switch(phase)
		{
		case 1://Defensive Stance
			{
				stance_id = 53790;
				aura_id = 41105;
			}break;
		case 2://Berserker Stance
			{
				stance_id = 53791;
				aura_id = 41107;
			}break;
		case 3://Berserker Stance
			{
				stance_id = 53792;
				aura_id = 41106;
			}break;
		}
		_unit->CastSpell( _unit, stance_id, true );
		_unit->CastSpell( _unit, aura_id, true );
		stances_timer = getMSTime() + STANCE_CHANGE_INTERVAL;
	}

	void Destroy()
	{
		for ( uint32 i = 0; i < spells.size(); ++i )
		{
			if ( spells[ i ] != NULL )
				delete spells[ i ];
		};

		spells.clear();

		delete this;
	};

protected:
	bool heroic;
	vector< ScriptSpell* > spells;
	uint32 stances_timer;
	uint32 phase;
};

#define VOLKHAN_ENTRY 28587
#define MOLTEN_GOLEM_ENTRY 28695
#define GOLEM_CREATION_HP 20
#define GOLEMS_COUNT 3
#define FREEZED_TO_SHATTER 1
#define SHATTER_CHANCE 50

class VOLKHAN_AI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VOLKHAN_AI);

    VOLKHAN_AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		heroic = ( _unit->GetMapMgr()->iInstanceMode == MODE_HEROIC );
		last_creation_hp = 0;
		freezed = 0;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		last_creation_hp = 100;
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It is you who have destroyed my children? You... shall... pay!");
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		if( (int)( last_creation_hp - GOLEM_CREATION_HP ) >= _unit->GetHealthPct() )
		{
			_unit->GetAIInterface()->MoveTo(1325.313f, -92.931f, 56.717f, 2.274f);
			_unit->CastSpell( _unit, 52238, true );
			last_creation_hp = _unit->GetHealthPct();
			switch(rand()%2)
			{
			case 0:
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nothing is wasted in the process. You will see...." );
				break;
			case 1:
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Life from the lifelessness... death for you." );
				break;
			}
		}
	}

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The master was right... to be concerned." );
		RemoveAIUpdateEvent();
    }

	//this is one big HAX
	void OnReachWP(uint32 i, bool usl)
	{
		if( i == 1337 )
		{
			freezed++;
		}
	}

	void OnTargetDied(Unit* mTarget)
	{
		//BUAHAHAHAH
		switch(rand()%3)
		{
		case 0:
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The armies of iron will conquer all!" );
			break;
		case 1:
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ha, pathetic!" );
			break;
		case 2:
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have cost me too much work!" );
			break;
		}
	}

    void AIUpdate()
	{
		if( freezed >= FREEZED_TO_SHATTER )
		{
			if( Rand( SHATTER_CHANCE ) )
			{//cast Shattering Stomp
				uint32 spellid = heroic ? 59529 : 52237;
				_unit->CastSpell( _unit, spellid, false );
				freezed = 0;//they are all destroyed
				switch(rand()%2)
				{
				case 0:
					_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will crush you beneath my boots!" );
					break;
				case 1:
					_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "All my work... undone!" );
					break;
				}
			}
		}
    }

	Player* GetRandomPlayerTarget()
	{
		//this SUCKS ASS!!
		vector< uint32 > possible_targets;
		for( unordered_set<Player*>::iterator iter = _unit->GetInRangePlayerSetBegin(); iter != _unit->GetInRangePlayerSetEnd(); ++iter ) 
		{ 
			if( (*iter) && (*iter)->isAlive() )
				possible_targets.push_back( (uint32)(*iter)->GetGUID() );
		}
		if( possible_targets.size() > 0 )
		{
			uint32 random_player = possible_targets[ Rand( uint32(possible_targets.size() - 1) ) ];
			return _unit->GetMapMgr()->GetPlayer( random_player );
		}
		return NULL;
	}

	void CastScriptSpell( ScriptSpell* spell )
	{
		_unit->Root();
		uint32 spellid = heroic ? spell->heroic_spellid : spell->normal_spellid;
		Unit* spelltarget = NULL;
		switch( spell->target )
		{
		case SPELL_TARGET_SELF:
			{
				spelltarget = _unit;
			}break;
		case SPELL_TARGET_GENERATE:
			{
				spelltarget = NULL;
			}break;
		case SPELL_TARGET_CURRENT_ENEMY:
			{
				spelltarget = _unit->GetAIInterface()->GetNextTarget();
			}break;
		case SPELL_TARGET_RANDOM_PLAYER:
			{
				spelltarget = GetRandomPlayerTarget();
			}break;
		}
		_unit->CastSpell( spelltarget , spellid, false );
		_unit->UnRoot();
	}

	void Destroy()
	{
		for ( uint32 i = 0; i < spells.size(); ++i )
		{
			if ( spells[ i ] != NULL )
				delete spells[ i ];
		};

		spells.clear();

		delete this;
	};

protected:
	bool heroic;
	uint32 last_creation_hp;
	uint32 freezed;
	vector< ScriptSpell* > spells;
};

class MOLTEN_GOLEM_AI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MOLTEN_GOLEM_AI);

    MOLTEN_GOLEM_AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		heroic = ( _unit->GetMapMgr()->iInstanceMode == MODE_HEROIC );
		spells.clear();
		/* SPELLS INIT */
		ScriptSpell* ImmolationStrike = new ScriptSpell;
		ImmolationStrike->normal_spellid = 52433;
		ImmolationStrike->heroic_spellid = 59530;
		ImmolationStrike->chance = 80;
		ImmolationStrike->timer = 7000;
		ImmolationStrike->time = 0;
		ImmolationStrike->target = SPELL_TARGET_RANDOM_PLAYER;
		spells.push_back(ImmolationStrike);

		ScriptSpell* BlastWave = new ScriptSpell;
		BlastWave->normal_spellid = 23113;
		BlastWave->heroic_spellid = 23113;
		BlastWave->chance = 50;
		BlastWave->timer = 5000;
		BlastWave->time = 0;
		BlastWave->target = SPELL_TARGET_CURRENT_ENEMY;
		spells.push_back(BlastWave);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		RemoveAIUpdateEvent();
    }

	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		if( _unit->GetHealthPct() < 3 )
		{//freeze, some kinda of animation here maybe?
			_unit->Root();
			_unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			for( uint8 i=0; i<7; i++ )
				_unit->SchoolImmunityList[i] = 1;
			_unit->GetAIInterface()->disable_combat = true;
			RemoveAIUpdateEvent();
			Unit* Volkhan = _unit->GetMapMgr()->GetUnit( _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY ) );
			if( Volkhan )
				TO_CREATURE( Volkhan )->GetScript()->OnReachWP( 1337, true );
		}
	}

    void AIUpdate()
	{
		if( spells.size() > 0 )
		{
			for( uint8 i = 0; i<spells.size(); i++ )
			{
				if( spells[i]->time < getMSTime() )
				{
					if( Rand( spells[i]->chance ) )
					{
						CastScriptSpell( spells[i] );
						spells[i]->time = getMSTime() + spells[i]->timer;
					}
				}
			}
		}
    }

	Player* GetRandomPlayerTarget()
	{
		//this SUCKS ASS!!
		vector< uint32 > possible_targets;
		for( unordered_set<Player*>::iterator iter = _unit->GetInRangePlayerSetBegin(); iter != _unit->GetInRangePlayerSetEnd(); ++iter ) 
		{ 
			if( (*iter) && (*iter)->isAlive() )
				possible_targets.push_back( (uint32)(*iter)->GetGUID() );
		}
		if( possible_targets.size() > 0 )
		{
			uint32 random_player = possible_targets[ Rand( uint32(possible_targets.size() - 1) ) ];
			return _unit->GetMapMgr()->GetPlayer( random_player );
		}
		return NULL;
	}

	void CastScriptSpell( ScriptSpell* spell )
	{
		_unit->Root();
		uint32 spellid = heroic ? spell->heroic_spellid : spell->normal_spellid;
		Unit* spelltarget = NULL;
		switch( spell->target )
		{
		case SPELL_TARGET_SELF:
			{
				spelltarget = _unit;
			}break;
		case SPELL_TARGET_GENERATE:
			{
				spelltarget = NULL;
			}break;
		case SPELL_TARGET_CURRENT_ENEMY:
			{
				spelltarget = _unit->GetAIInterface()->GetNextTarget();
			}break;
		case SPELL_TARGET_RANDOM_PLAYER:
			{
				spelltarget = GetRandomPlayerTarget();
			}break;
		}
		_unit->CastSpell( spelltarget , spellid, false );
		_unit->UnRoot();
	}

	void Destroy()
	{
		for ( uint32 i = 0; i < spells.size(); ++i )
		{
			if ( spells[ i ] != NULL )
				delete spells[ i ];
		};

		spells.clear();

		delete this;
	};

protected:
	bool heroic;
	vector< ScriptSpell* > spells;
};

bool Temper(uint32 i, Spell* pSpell)
{
	if( !pSpell->u_caster )
		return false;

	Unit* _unit = pSpell->u_caster;

	//summon several Molten Golem's
	for( uint8 i=0; i<GOLEMS_COUNT; i++ )
	{
		CreatureProto *cp = CreatureProtoStorage.LookupEntry(MOLTEN_GOLEM_ENTRY);
		CreatureInfo *ci = CreatureNameStorage.LookupEntry(MOLTEN_GOLEM_ENTRY);
		Creature* c = NULL;
		if (cp && ci)
		{
			c = _unit->GetMapMgr()->CreateCreature( MOLTEN_GOLEM_ENTRY );
			if (c)
			{
				c->Load(cp, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0.0f);
				c->PushToWorld(_unit->GetMapMgr());
				c->SetUInt64Value( UNIT_FIELD_SUMMONEDBY, _unit->GetGUID() );
			}
		}
	}
	return true;
}

bool ShatteringStomp(uint32 i, Spell* pSpell)
{
	if( !pSpell->u_caster )
		return false;

	Unit* _unit = pSpell->u_caster;
	Creature* golem = NULL;
	for(unordered_set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
	{
		if( (*itr) && (*itr)->IsCreature() )
		{
			golem = TO_CREATURE((*itr));
			if( golem->isAlive() && golem->creature_info && golem->creature_info->Id == MOLTEN_GOLEM_ENTRY )
			{
				uint32 spellid = pSpell->m_spellInfo->Id == 59529 ? 59527 : 52429;
				Creature* Golem = TO_CREATURE( (*itr) );
				Golem->CastSpell( Golem, spellid, true );
				for( uint8 i=0; i<7; i++ )
					Golem->SchoolImmunityList[i] = 0;
				_unit->DealDamage( Golem, Golem->GetHealth(),0,0,0);
			}
		}
	}
	return true;
}
#define IONAR_ENTRY 28546
#define DISPARSE_HP 20
#define SPARKS_COUNT 5
#define SPARK_PHASE_DURATION 10000//10s

class IONAR_AI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(IONAR_AI);

    IONAR_AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		heroic = ( _unit->GetMapMgr()->iInstanceMode == MODE_HEROIC );
		spells.clear();
		for( uint8 i=0; i<SPARKS_COUNT; i++)
			sparks[i] = 0;
		last_creation_hp = 0;
		spark_timer = 0;
		/* SPELLS INIT */
		ScriptSpell* StaticOverload = new ScriptSpell;
		StaticOverload->normal_spellid = 52658;
		StaticOverload->heroic_spellid = 59795;
		StaticOverload->chance = 100;
		StaticOverload->timer = 11000;
		StaticOverload->time = 0;
		StaticOverload->target = SPELL_TARGET_RANDOM_PLAYER;
		spells.push_back(StaticOverload);

		ScriptSpell* BallLightning = new ScriptSpell;
		BallLightning->normal_spellid = 52780;
		BallLightning->heroic_spellid = 59800;
		BallLightning->chance = 50;
		BallLightning->timer = 5000;
		BallLightning->time = 0;
		BallLightning->target = SPELL_TARGET_RANDOM_PLAYER_POSITION;
		spells.push_back(BallLightning);
    }
    
	void OnDamageTaken(Unit* mAttacker, float fAmount)
	{
		if( (int)( last_creation_hp - DISPARSE_HP ) >= _unit->GetHealthPct() )
		{
			switch(rand()%2)
			{
			case 0:
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The slightest spark shall be your undoing." );
				break;
			case 1:
				_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No one is safe!" );
				break;
			}
			spark_timer = getMSTime() + SPARK_PHASE_DURATION;
			last_creation_hp = _unit->GetHealthPct();
			_unit->CastSpell(_unit, 52770, true);//disperse, dummy
			_unit->Root();
			_unit->GetAIInterface()->disable_combat = false;
			_unit->m_invisible = false;
			_unit->UpdateVisibility();
			//disparse
			for( uint8 i=0; i<SPARKS_COUNT; i++)
				if( sparks[i] != 0 )
				{
					Unit* Spark = _unit->GetMapMgr()->GetUnit( sparks[i] );
					if( Spark )
					{
						uint32 spellid = heroic ? 59833 : 52667;
						Spark->CastSpell(Spark, spellid, true);
						Spark->UnRoot();
						_unit->GetAIInterface()->disable_combat = false;
						Player* p_target = GetRandomPlayerTarget();
						if( p_target )
						{
							Spark->GetAIInterface()->MoveTo(p_target->GetPositionX(),p_target->GetPositionY(), p_target->GetPositionZ(), p_target->GetOrientation());
						}
					}
				}			
		}
	}

    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You wish to confront the master? You must weather the storm!");
		spark_timer = 0;
		last_creation_hp = 100;
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		for( uint8 i=0; i<SPARKS_COUNT; i++)
		{
			CreatureProto *cp = CreatureProtoStorage.LookupEntry(28926);
			CreatureInfo *ci = CreatureNameStorage.LookupEntry(28926);
			Creature* c = NULL;
			if (cp && ci)
			{
				c = _unit->GetMapMgr()->CreateCreature( 28926 );
				if (c)
				{
					c->Load(cp,_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation());
					c->PushToWorld(_unit->GetMapMgr());
					sparks[i] = c->GetGUID();
					c->Root();
					c->GetAIInterface()->disable_combat = true;
					for( uint8 i=0; i<7; i++ )
						c->SchoolImmunityList[i] = 1;
				}
			}
		}
    }
    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
		for( uint8 i=0; i<SPARKS_COUNT; i++)
			if( sparks[i] != 0 )
			{
				Unit *Spark = _unit->GetMapMgr()->GetUnit( sparks[i] );
				if(Spark != NULL)
					Spark->SummonExpireAll(true);
			}
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Master... you have guests.");
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		//BUAHAHAHAH
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Shocking, isn't it?");
	}

    void AIUpdate()
	{
		if( spark_timer < getMSTime() )
		{
			spark_timer = 0;
			_unit->UnRoot();
			_unit->GetAIInterface()->disable_combat = false;
			_unit->m_invisible = false;
			_unit->UpdateVisibility();
			for( uint8 i=0; i<SPARKS_COUNT; i++)
				if( sparks[i] != 0 )
				{
					Unit* Spark = _unit->GetMapMgr()->GetUnit( sparks[i] );
					if( Spark )
					{
						Spark->RemoveAllAuras();
						Spark->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation());
						Spark->Root();
						Spark->GetAIInterface()->disable_combat = true;
					}
				}			
		}
		if( spark_timer == 0 && spells.size() > 0 )
		{
			for( uint8 i = 0; i<spells.size(); i++ )
			{
				if( spells[i]->time < getMSTime() )
				{
					if( Rand( spells[i]->chance ) )
					{
						CastScriptSpell( spells[i] );
						spells[i]->time = getMSTime() + spells[i]->timer;
					}
				}
			}
		}
    }

	Player* GetRandomPlayerTarget()
	{
		//this SUCKS ASS!!
		vector< uint32 > possible_targets;
		for( unordered_set<Player*>::iterator iter = _unit->GetInRangePlayerSetBegin(); iter != _unit->GetInRangePlayerSetEnd(); ++iter ) 
		{ 
			if( (*iter) && (*iter)->isAlive() )
				possible_targets.push_back( (uint32)(*iter)->GetGUID() );
		}
		if( possible_targets.size() > 0 )
		{
			uint32 random_player = possible_targets[ Rand( uint32(possible_targets.size() - 1) ) ];
			return _unit->GetMapMgr()->GetPlayer( random_player );
		}
		return NULL;
	}

	void CastScriptSpell( ScriptSpell* spell )
	{
		_unit->Root();
		uint32 spellid = heroic ? spell->heroic_spellid : spell->normal_spellid;
		Unit* spelltarget = NULL;
		switch( spell->target )
		{
		case SPELL_TARGET_SELF:
			{
				spelltarget = _unit;
			}break;
		case SPELL_TARGET_GENERATE:
			{
				spelltarget = NULL;
			}break;
		case SPELL_TARGET_CURRENT_ENEMY:
			{
				spelltarget = _unit->GetAIInterface()->GetNextTarget();
			}break;
		case SPELL_TARGET_RANDOM_PLAYER:
			{
				spelltarget = GetRandomPlayerTarget();
			}break;
		case SPELL_TARGET_RANDOM_PLAYER_POSITION:
			{
				Player* target = GetRandomPlayerTarget();
				if( target )
				{
					Spell* sp(new Spell(_unit,dbcSpell.LookupEntry( spellid ),true,NULL));
					SpellCastTargets targets;
					targets.m_destX = target->GetPositionX();
					targets.m_destY = target->GetPositionY();
					targets.m_destZ = target->GetPositionZ();
					targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
					sp->prepare(&targets);
					_unit->UnRoot();
					return;
				}
			}break;
		}
		_unit->CastSpell( spelltarget , spellid, false );
		_unit->UnRoot();
	}

	void Destroy()
	{
		for ( uint32 i = 0; i < spells.size(); ++i )
		{
			if ( spells[ i ] != NULL )
				delete spells[ i ];
		};

		spells.clear();

		delete this;
	};

protected:
	bool heroic;
	vector< ScriptSpell* > spells;
	uint32 last_creation_hp;
	uint64 sparks[SPARKS_COUNT];
	uint32 spark_timer;
};

//Loken
#define LOKEN_ENTRY 28923

class LOKEN_AI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LOKEN_AI);

    LOKEN_AI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		heroic = ( _unit->GetMapMgr()->iInstanceMode == MODE_HEROIC );
		spells.clear();
		/* SPELLS INIT */
		ScriptSpell* ArcLightning = new ScriptSpell;
		ArcLightning->normal_spellid = 52921;
		ArcLightning->heroic_spellid = 52921;
		ArcLightning->chance = 50;
		ArcLightning->timer = 3000;
		ArcLightning->time = 0;
		ArcLightning->target = SPELL_TARGET_RANDOM_PLAYER;
		spells.push_back(ArcLightning);

		ScriptSpell* LightningNova = new ScriptSpell;
		LightningNova->normal_spellid = 52960;
		LightningNova->heroic_spellid = 59835;
		LightningNova->chance = 100;
		LightningNova->timer = 10000;
		LightningNova->time = 0;
		LightningNova->target = SPELL_TARGET_SELF;
		spells.push_back(LightningNova);
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What hope is there for you? None!");
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
		_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My death... heralds the end of this world.");
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(Unit* mTarget)
	{
		switch(rand()%3)
		{
		case 0:
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Only mortal..." );
			break;
		case 1:
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I... am... FOREVER!" );
			break;
		case 2:
			_unit->SendChatMessage( CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What little time you had, you wasted!" );
			break;
		}
	}

    void AIUpdate()
	{
		if( spells.size() > 0 )
		{
			for( uint8 i = 0; i<spells.size(); i++ )
			{
				if( spells[i]->time < getMSTime() )
				{
					if( Rand( spells[i]->chance ) )
					{
						CastScriptSpell( spells[i] );
						spells[i]->time = getMSTime() + spells[i]->timer;
					}
				}
			}
		}
    }

	Player* GetRandomPlayerTarget()
	{
		//this SUCKS ASS!!
		vector< uint32 > possible_targets;
		for( unordered_set<Player*>::iterator iter = _unit->GetInRangePlayerSetBegin(); iter != _unit->GetInRangePlayerSetEnd(); ++iter ) 
		{ 
			if( (*iter) && (*iter)->isAlive() )
				possible_targets.push_back( (uint32)(*iter)->GetGUID() );
		}
		if( possible_targets.size() > 0 )
		{
			uint32 random_player = possible_targets[ Rand( uint32(possible_targets.size() - 1) ) ];
			return _unit->GetMapMgr()->GetPlayer( random_player );
		}
		return NULL;
	}

	void CastScriptSpell( ScriptSpell* spell )
	{
		_unit->Root();
		uint32 spellid = heroic ? spell->heroic_spellid : spell->normal_spellid;
		Unit* spelltarget = NULL;
		switch( spell->target )
		{
		case SPELL_TARGET_SELF:
			{
				spelltarget = _unit;
			}break;
		case SPELL_TARGET_GENERATE:
			{
				spelltarget = NULL;
			}break;
		case SPELL_TARGET_CURRENT_ENEMY:
			{
				spelltarget = _unit->GetAIInterface()->GetNextTarget();
			}break;
		case SPELL_TARGET_RANDOM_PLAYER:
			{
				spelltarget = GetRandomPlayerTarget();
			}break;
		}
		_unit->CastSpell( spelltarget , spellid, false );
		_unit->UnRoot();
	}

	void Destroy()
	{
		for ( uint32 i = 0; i < spells.size(); ++i )
		{
			if ( spells[ i ] != NULL )
				delete spells[ i ];
		};

		spells.clear();

		delete this;
	};

protected:
	bool heroic;
	vector< ScriptSpell* > spells;
};

void SetupHallsofLightning(ScriptMgr * mgr)
{
	//////////////////////////////////////////
	// TRASH MOBS
	//////////////////////////////////////////

	//////////////////////////////////////////
	// BOSSES
	//////////////////////////////////////////
	mgr->register_creature_script(GENERAL_BJARNGRIM_ENTRY, &GENERAL_BJARNGRIM_AI::Create);

	//Volkhan stuff
	mgr->register_creature_script(VOLKHAN_ENTRY, &VOLKHAN_AI::Create);
	mgr->register_creature_script(MOLTEN_GOLEM_ENTRY, &MOLTEN_GOLEM_AI::Create);
	mgr->register_dummy_spell(52238, &Temper);
	//Shattering Stomp
	SpellEntry* sp = dbcSpell.LookupEntryForced( 59529 );
	if( sp )
		sp->Effect[1] = SPELL_EFFECT_DUMMY;
	sp = dbcSpell.LookupEntryForced( 52237 );
	if( sp )
		sp->Effect[1] = SPELL_EFFECT_DUMMY;
	mgr->register_dummy_spell(59529, &ShatteringStomp);
	mgr->register_dummy_spell(52237, &ShatteringStomp);
	//Ionar
	mgr->register_creature_script(IONAR_ENTRY, &IONAR_AI::Create);
	//Loken
	mgr->register_creature_script(LOKEN_ENTRY, &LOKEN_AI::Create);
}