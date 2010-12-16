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
/* Instance_HellfireRamparts.cpp Script		                            */
/************************************************************************/

// Bleeding Hollow ArcherAI

#define CN_BLEEDING_HOLLOW_ARCHER 17270

#define MULTI_SHOT 18651
#define AIMED_SHOT 30614

class BLEEDINGHOLLOWARCHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLEEDINGHOLLOWARCHERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BLEEDINGHOLLOWARCHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(MULTI_SHOT);
		spells[0].targettype = TARGET_ATTACKING;	// changed from VARIOUS to prevent crashes when caster kills itself by using this ability on low hp
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(AIMED_SHOT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 25;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bleeding Hollow DarkcasterAI

#define CN_BLEEDING_HOLLOW_DARKCASTER 17269

#define SCORCH 36807	// DBC: 15241 // not sure
#define RAIN_OF_FIRE 36808	// DBC: 20754 same here

class BLEEDINGHOLLOWDARKCASTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLEEDINGHOLLOWDARKCASTERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BLEEDINGHOLLOWDARKCASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SCORCH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 13;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[1].targettype = TARGET_DESTINATION;
		spells[1].instant = false;
		spells[1].cooldown = 30;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bleeding Hollow ScryerAI

#define CN_BLEEDING_HOLLOW_SCRYER 17478

#define FEAR 30615
#define SHADOW_BOLT_SCRYER 12471 // not sure to those two
#define FEL_INFUSION 30659 // not sure if it's using it

class BLEEDINGHOLLOWSCRYERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BLEEDINGHOLLOWSCRYERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BLEEDINGHOLLOWSCRYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(FEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(SHADOW_BOLT_SCRYER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        /*spells[2].info = dbcSpell.LookupEntry(FEL_INFUSION);
		spells[2].targettype = TARGET_SELF;	// why it targets enemy :O
		spells[2].instant = true;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bonechewer BeastmasterAI

#define CN_BONECHEWER_BEASTMASTER 17455

#define BATTLE_SHOUT 30635
#define UPPERCUT 10966		// not sure to any spell
// in a certain ammount of time he calls in about 2-4 more dogs.
class BONECHEWERBEASTMASTERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERBEASTMASTERAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    BONECHEWERBEASTMASTERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(BATTLE_SHOUT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 45;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(UPPERCUT);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].cooldown = 30;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

	void OnTargetDied(Unit* mTarget)
    {
    }

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bonechewer DestroyerAI

#define CN_BONECHEWER_DESTROYER 17271

#define MORTAL_STRIKE 15708
#define KNOCK_AWAY 10101
#define SWEEPING_STRIKES 18765	// 12723

class BONECHEWERDESTROYERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERDESTROYERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BONECHEWERDESTROYERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(KNOCK_AWAY);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 35;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(SWEEPING_STRIKES);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].cooldown = 40;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bonechewer HungererAI

#define CN_BONECHEWER_HUNGERER 17259

#define DEMORALIZING_SHOUT 16244
#define STRIKE 14516
#define DISARM 6713	// no idea if those are correct spells

class BONECHEWERHUNGERERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERHUNGERERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    BONECHEWERHUNGERERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(DEMORALIZING_SHOUT);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = true;
		spells[0].cooldown = 40;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(STRIKE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(DISARM);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 50;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bonechewer RavenerAI

#define CN_BONECHEWER_RAVENER 17264

#define KIDNEY_SHOT 30621
// Invisibility and Stealth Detection ? 18950
class BONECHEWERRAVENERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERRAVENERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BONECHEWERRAVENERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(KIDNEY_SHOT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (RandomUInt(15) == 1)
			_unit->GetAIInterface()->ClearHateList();

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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Bonechewer RipperAI

#define CN_BONECHEWER_RIPPER 17281

#define ENRAGE 18501

class BONECHEWERRIPPERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BONECHEWERRIPPERAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    BONECHEWERRIPPERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(ENRAGE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 20;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Fiendish HoundAI

#define CN_FIENDISH_HOUND 17540

#define DRAIN_LIFE 35748

class FIENDISHHOUNDAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FIENDISHHOUNDAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    FIENDISHHOUNDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(DRAIN_LIFE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Hellfire WatcherAI

#define CN_HELLFIRE_WATCHER 17309

#define SHADOW_WORD_PAIN 14032
#define HEAL 30643	// DBC: 12039, 30643
#define RENEW 37260 // DBC: 8362; no idea if those are correct spells // uses it also on enemy :O // DBC: 8362
// renew? other heal?

class HELLFIREWATCHERAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HELLFIREWATCHERAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    HELLFIREWATCHERAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(SHADOW_WORD_PAIN);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].cooldown = 45;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(RENEW);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;
		spells[2].cooldown = 60;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// Shattered Hand WarhoundAI

#define CN_SHATTERED_HAND_WARHOUND 17280

#define CARNIVOROUS_BITE 30639
//#define FURIOUS_HOWL 30636
// Invisibility and Stealth Detection 18950 ? && Increase Spell Dam 43 17280 ?
class SHATTEREDHANDWARHOUNDAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SHATTEREDHANDWARHOUNDAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    SHATTEREDHANDWARHOUNDAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(CARNIVOROUS_BITE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
/*
        spells[1].info = dbcSpell.LookupEntry(FURIOUS_HOWL);	// crashes server :O
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
*/
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

/*****************************/
/*                           */
/*         Boss AIs          */
/*                           */
/*****************************/

// Watchkeeper GargolmarAI

#define CN_WATCHKEEPER_GARGOLMAR 17306

#define SURGE 34645 // 25787 not sure
#define MORTAL_WOUND 30641 // 25646
#define	OVERPOWER 32154	// I am not sure about this spell and those down there
#define RETALIATION 22857 // DBC: 22857, 22858

class WATCHKEEPERGARGOLMARAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(WATCHKEEPERGARGOLMARAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    WATCHKEEPERGARGOLMARAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(MORTAL_WOUND);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 35;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(OVERPOWER);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].cooldown = 10;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SURGE);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].cooldown = 15;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].speech = "Back off, pup!";
		spells[2].soundid = 10330;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 40.0f;

        spells[3].info = dbcSpell.LookupEntry(RETALIATION);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].cooldown = 60;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

		CalledForHelp = false;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();

		CalledForHelp = false;
		int RandomSpeach = rand()%3;
		switch (RandomSpeach)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "What do we have here? ...");
			_unit->PlaySoundToSet(10331);
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Heh... this may hurt a little.");
			_unit->PlaySoundToSet(10332);
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm gonna enjoy this!");
			_unit->PlaySoundToSet(10333);
			break;
		}
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<4;i++)
			spells[i].casttime = 0;
		spells[2].casttime = (uint32)time(NULL) + 10;
	}

	void OnTargetDied(Unit* mTarget)
    {
		if (_unit->GetHealthPct() > 0)	// Hack to prevent double yelling (OnDied and OnTargetDied when creature is dying)
		{
			int RandomSpeach = rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Say farewell!");
				_unit->PlaySoundToSet(10334);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Much too easy!");
				_unit->PlaySoundToSet(10335);
				break;
			}
		}
    }

    void OnCombatStop(Unit* mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		CalledForHelp = false;
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		_unit->PlaySoundToSet(10336);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (_unit->GetHealthPct() <= 40 && !CalledForHelp)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Heal me! QUICKLY!");
			_unit->PlaySoundToSet(10329);
			CalledForHelp = true;
		}

		uint32 t = (uint32)time(NULL);
		if (_unit->GetHealthPct() <= 20 && t > spells[3].casttime && _unit->GetCurrentSpell() == NULL)
		{
			_unit->setAttackTimer(1500, false);

			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

			spells[3].casttime = t + 120;
			return;
		}

		if (t > spells[2].casttime)
		{
			Unit* target = NULL;
			target = FindTarget();
			if (target)
			{
				_unit->setAttackTimer(1500, false);

				_unit->CastSpell(target, spells[2].info, spells[2].instant);
			}

			spells[2].casttime = t + 10;
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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit*  RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	Unit* FindTarget()
	{
		Unit* target = NULL;
		float distance = 5.0f;

		Unit* pUnit;
		float dist;

		for (unordered_set<Object*>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = TO_UNIT((*itr));

			if (pUnit == _unit->GetAIInterface()->GetMostHated())
				continue;

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist < distance*distance && dist > 900.0f)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}

	void Destroy()
	{
		delete this;
	};

protected:

	int CalledForHelp;
	int nrspells;
};

//Omor the Unscarred
#define CN_OMOR_THE_UNSCARRED						17308
#define OMOR_THE_UNSCARRED_DEMONIC_SHIELD			31901
#define OMOR_THE_UNSCARRED_SUMMON_FIENDISH_HOUND	30707
#define OMOR_THE_UNSCARRED_SHADOW_WHIP				30638
#define OMOR_THE_UNSCARRED_SHADOW_BOLT				30686
#define OMOR_THE_UNSCARRED_SHADOW_BOLT2				39297
#define OMOR_THE_UNSCARRED_TREACHEROUS_AURA			30695
#define OMOR_THE_UNSCARRED_BANE_OF_TREACHERY		37566

class OmorTheUnscarredAI : public AscentScriptCreatureAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(OmorTheUnscarredAI, AscentScriptCreatureAI);
	OmorTheUnscarredAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		SpellDesc *pShield = AddSpell(OMOR_THE_UNSCARRED_DEMONIC_SHIELD, Target_Self, 30, 0, 25);
		pShield->mEnabled = false;
		SpellDesc *pSummon = AddSpell(OMOR_THE_UNSCARRED_SUMMON_FIENDISH_HOUND, Target_Self, 8, 1, 20);
		pSummon->AddEmote("Achor-she-ki! Feast my pet! Eat your fill!", Text_Yell, 10277);
		AddSpell(OMOR_THE_UNSCARRED_SHADOW_WHIP, Target_RandomPlayer, 10, 0, 30);
		if(_unit->GetMapMgr()->iInstanceMode != MODE_HEROIC)
		{
			AddSpell(OMOR_THE_UNSCARRED_SHADOW_BOLT, Target_RandomPlayer, 8, 3, 15, 10, 60, true);
			SpellDesc *pAura = AddSpell(OMOR_THE_UNSCARRED_TREACHEROUS_AURA, Target_RandomPlayer, 8, 2, 35, 0, 60, true);
			pAura->AddEmote("A-Kreesh!", Text_Yell, 10278);
		}
		else
		{
			AddSpell(OMOR_THE_UNSCARRED_SHADOW_BOLT2, Target_RandomPlayer, 8, 3, 15, 10, 60, true);
			SpellDesc *pAura = AddSpell(OMOR_THE_UNSCARRED_BANE_OF_TREACHERY, Target_RandomPlayer, 8, 2, 35, 0, 60, true);
			pAura->AddEmote("A-Kreesh!", Text_Yell, 10278);
		}

		AddEmote(Event_OnCombatStart, "I will not be defeated!", Text_Yell, 10279);
		AddEmote(Event_OnCombatStart, "You dare stand against me?", Text_Yell, 10280);	// corrections
		AddEmote(Event_OnCombatStart, "Your incidents will be your death!", Text_Yell, 10281);
		AddEmote(Event_OnTargetDied, "Die weakling!", Text_Yell, 10282);
		AddEmote(Event_OnDied, "It is... not over.", Text_Yell, 10284);
	}

	void OnCombatStart(Unit* pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SetCanMove(false);
	}

	void OnCombatStop(Unit* pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (IsAlive())
		{
			Emote("I am victorious!", Text_Yell, 10283);
		}
	}

	void AIUpdate()
	{
		SpellDesc *pShield = FindSpellById(OMOR_THE_UNSCARRED_DEMONIC_SHIELD);
		if (GetHealthPercent() <= 20 && pShield != NULL && !pShield->mEnabled)
		{
			pShield->mEnabled = true;
		}

		Unit* pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) > 10.0f)
			{
				pTarget = GetBestPlayerTarget(TargetFilter_Closest);
				if (pTarget != NULL)
				{
					if (GetRangeToUnit(pTarget) > 10.0f)
					{
						pTarget = NULL;
					}
					else
					{
						ClearHateList();
						_unit->GetAIInterface()->AttackReaction(pTarget, 500);
						_unit->GetAIInterface()->SetNextTarget(pTarget);
					}
				}
				else
					return;
			}

			if (pTarget == NULL)
			{
				SpellDesc *pWhip = FindSpellById(OMOR_THE_UNSCARRED_SHADOW_WHIP);	// used for now
				if (pWhip != NULL)
				{
					pWhip->mLastCastTime = 0;
					CastSpellNowNoScheduling(pWhip);
					return;
				}
			}
		}

		ParentClass::AIUpdate();
		SetCanMove(false);
	}
};

/*****************************/
/*                           */
/*    Vazruden the Herald	 */
/*			*Full Event*	 */
/*                           */
/*****************************/

// EVENT DEFINITIONS/VARIABLES
uint32 VAZRUDEN_THE_HERALD_EVENT_PHASE;
uint32 HERALDS_SENTRY_DEATHS;	

// Hellfire Sentry AI - mob

#define CN_HELLFIRE_SENTRY 17517

#define KIDNEY_SHOT_SENTRY 30621
// Not sure!
class HELLFIRESENTRYAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(HELLFIRESENTRYAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    HELLFIRESENTRYAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}
		
        spells[0].info = dbcSpell.LookupEntry(KIDNEY_SHOT_SENTRY);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 25;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

// VazrudenAI

#define CN_VAZRUDEN 17537

#define REVENGE 40392 // DBC: 19130; 37517, 40392, 19130 or other

class VAZRUDENAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VAZRUDENAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    VAZRUDENAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		PHASE_LIMITER = 2;
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(REVENGE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		PHASE_LIMITER = 2;
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)
    {
		CastTime();
		PHASE_LIMITER = 2;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		CastTime();
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if (_unit->GetHealthPct() <= 20 && PHASE_LIMITER == 2)
		{
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 3;
			PHASE_LIMITER = 3;
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
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

	uint32 PHASE_LIMITER;
	int nrspells;
};

// NazanAI
								// DOESN'T WORK YET!
#define WALK 0
#define RUN 256
#define FLY 768

//#define CN_NAZAN 17536
#define CN_NAZAN 17307 // 17307 = 17536 + 17537; VAZRUDEN THE HERALD = NAZAN (MOUNT) + VAZRUDEN
// Event: Phase1 spells
#define SUMMON_VAZRUDEN 30717
// Event: Phase2 spells
#define FIREBALL 30691	// DBC: 34653, 30691
#define LIQUID_FLAME 1
#define CONE_OF_FIRE 30926
// FACE HIGHEST THREAT 30700

static Coords fly[] = 
{
	{ 0, 0, 0, 0 },
	{ -1413.410034f, 1744.969971f,  80.900000f, 0.147398f },	// fly starting position
	{ -1413.410034f, 1744.969971f,  92.948196f, 0.147398f },	// fly !
	{ -1378.454712f, 1687.340332f, 110.200218f, 1.017074f },	// 1 full fly in circle wp
	{ -1352.973145f, 1726.131470f, 110.408745f, 1.297234f },	// 2
    { -1362.943970f, 1767.925415f, 110.101616f, 5.212438f },	// 3
	{ -1415.544189f, 1804.141357f, 110.075363f, 5.974271f },	// 4
	{ -1461.189575f, 1780.554932f, 110.854507f, 0.460774f },	// 5
	{ -1482.489380f, 1718.727783f, 110.248772f, 5.847037f },	// 6
	{ -1418.811646f, 1676.112427f, 110.405968f, 0.231439f },	// 7
	{ -1413.408203f, 1744.974121f,  92.000000f, 0.147398f }		// land place
};
//	{ -1413.410034, 1744.969971,  80.948196, 0.147398 },	// fly starting position

class NAZANAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NAZANAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    NAZANAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
		HERALDS_SENTRY_DEATHS = 0;
        m_entry = pCreature->GetEntry();
        m_useSpell = false;
        m_FireballCooldown = 5;
        m_ConeOfFireCooldown = 0;

		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 2000, RUN));	//RUN
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));	// FLY START!
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(4, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(5, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(6, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(7, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(8, 0, FLY));
        _unit->GetAIInterface()->addWayPoint(CreateWaypoint(9, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(10, 1000, FLY));	// SPECIAL WP! (FOR VAZRUDEN LANDING!)

        //infoFireball = dbcSpell.LookupEntry(FIREBALL);
        infoLiquidFlame = dbcSpell.LookupEntry(LIQUID_FLAME);
        //infoConeOfFire = dbcSpell.LookupEntry(CONE_OF_FIRE);
		infoSummonVazruden = dbcSpell.LookupEntry(SUMMON_VAZRUDEN);

		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

        spells[0].info = dbcSpell.LookupEntry(FIREBALL);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 10;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(CONE_OF_FIRE);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 20;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

        if(/*!infoFireball || */!infoLiquidFlame ||/* !infoConeOfFire || */!infoSummonVazruden)
            m_useSpell = false;

        _unit->GetAIInterface()->setOutOfCombatRange(200000);

		VAZRUDEN_LAND = false;
		NAZAN_LAND = false;
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
        _unit->GetAIInterface()->StopMovement(0);
        _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
		_unit->GetAIInterface()->setWaypointToMove(1);

    }
    
    void OnCombatStart(Unit* mTarget)
    {
		CastTime();
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void CastTime()
	{
		for(int i=0;i<nrspells;i++)
			spells[i].casttime = spells[i].cooldown;
	}

    void OnCombatStop(Unit* mTarget)	// not finished yet, as I must force Nazan to go into combat state
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
		{
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
			HERALDS_SENTRY_DEATHS = 0;
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(1);
			VAZRUDEN_LAND = false;
			NAZAN_LAND = false;
		}

		if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 2)
		{
			HERALDS_SENTRY_DEATHS = 0;
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
			VAZRUDEN_LAND = false;
		}

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit* mKiller)
    {
		VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
		HERALDS_SENTRY_DEATHS = 0;
		VAZRUDEN_LAND = false;
		NAZAN_LAND = false;
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		switch (VAZRUDEN_THE_HERALD_EVENT_PHASE)
		{
		case 1: 
			{
				PhaseOne();
			}break;
		case 2:
			{
				PhaseTwo();
			}break;
		case 3: 
			{
				PhaseThree();
			}break;
		default:
			{
				VAZRUDEN_THE_HERALD_EVENT_PHASE = 1;
			};
		};
		//float val = (float)RandomFloat(100.0f);
		//SpellCast(val);
    }

    void PhaseOne()
    {
    }

	void PhaseTwo()
	{
		m_useSpell = true;
		m_FireballCooldown--;
        if(!m_FireballCooldown && _unit->GetAIInterface()->GetNextTarget())
        {
            _unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), infoFireball, false);
            m_FireballCooldown = 5;
        }
	}

    void PhaseThree()
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
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
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

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

    void Fly()
    {
        _unit->Emote(EMOTE_ONESHOT_LIFTOFF);
        _unit->GetAIInterface()->m_moveFly = true;
    }

    void Land()
    {
        _unit->Emote(EMOTE_ONESHOT_LAND);
        _unit->GetAIInterface()->m_moveFly = false;
    }

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		if (HERALDS_SENTRY_DEATHS == 2 && VAZRUDEN_LAND == false) /*VAZRUDEN_THE_HERALD_EVENT_PHASE == 2*/
		{
			VAZRUDEN_THE_HERALD_EVENT_PHASE = 2;
			VAZRUDEN_LAND = true;
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
            _unit->GetAIInterface()->setWaypointToMove(10);
		}

		if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3 && NAZAN_LAND == false)
		{
			NAZAN_LAND = true;
            _unit->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
            if(_unit->GetCurrentSpell() != NULL)				// these 3 lines added from Onyxia script
                _unit->GetCurrentSpell()->cancel();
            _unit->GetAIInterface()->m_canMove = true;
            _unit->GetAIInterface()->SetAllowedToEnterCombat(false);
            //_unit->GetAIInterface()->StopMovement(0);	// commented? this crap "despawns" creature
            _unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
			_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			_unit->GetAIInterface()->setWaypointToMove(2);
			if (_unit->GetAIInterface()->GetNextTarget())
			{
				Unit* target = NULL;
				target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(target, spells[1].info, spells[1].instant);
			}
		}

		else
		{
			switch(iWaypointId)
			{
			case 1:
				{
					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
					{
						_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(0);
						WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
						data << _unit->GetNewGUID();
						data << uint32(0);
						_unit->SendMessageToSet(&data, false);
						Land();

					}

					else
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(2);
						Fly();
					}
				}break;

			case 2:
				{
					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(1);
					}

					else
					{
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(3);	
					}
				}break;

			case 3:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(4);
			    }break;

			case 4:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(5);
			    }break;

			case 5:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(6);
					/*
			        _unit->GetAIInterface()->m_canMove = false;
			        _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
			        _unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			        //_unit->m_pacified--;
			        _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
			        _unit->GetAIInterface()->setWaypointToMove(0);
			        WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
			        data << _unit->GetNewGUID();
			        data << uint32(0);
			        _unit->SendMessageToSet(&data, false);
			        m_currentWP = 3;*/
			    }break;

			case 6:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(7);
			    }break;

			case 7:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(8);
			    }break;

			case 8:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(9);
			    }break;

			case 9:
			    {
			        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
			        _unit->GetAIInterface()->setWaypointToMove(3);
			    }break;

			case 10:
			    {
					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 3)
					{
						/*_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAInterface()->setCurrentAgent(AGENT_NULL);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						//_unit->GetAIInterface()->setWaypointToMove(0);
						//_unit->GetAIInterface()->m_canMove = true;
						//_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						//_unit->GetAIInterface()->StopMovement(0);
						//_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
						//_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						Land();
						m_useSpell = true;
		                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
						_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(0);
		                WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
						data << _unit->GetNewGUID();
						data << uint32(0);
						_unit->SendMessageToSet(&data, false);
						Land();*/
					    //_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
						//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
						/*nit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);*/	// this shit messes all =/ // SCRIPTIDLE
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(9);
						/*_unit->m_pacified--;
						if(_unit->m_pacified > 0)
					    _unit->m_pacified--;*/
						/*WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
						data << _unit->GetNewGUID();
						data << uint32(0);
						_unit->SendMessageToSet(&data, false);*/ // Hover disabled for now
						//_unit->GetAIInterface()->m_moveFly = false;
						Land();
						/* working partially
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
						_unit->GetAIInterface()->setWaypointToMove(0);
						Land();
						*/
					}

					if (VAZRUDEN_THE_HERALD_EVENT_PHASE == 2)
					{
						m_useSpell = true;
						_unit->CastSpell(_unit, infoSummonVazruden, true);
						_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
						_unit->GetAIInterface()->setWaypointToMove(3);
					}

				}break;

			default:
				{
                _unit->GetAIInterface()->m_canMove = false;
                _unit->GetAIInterface()->SetAllowedToEnterCombat(true);
                _unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
                _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
                _unit->GetAIInterface()->setWaypointToMove(0);
                WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
                data << _unit->GetNewGUID();
                data << uint32(0);
                _unit->SendMessageToSet(&data, false);
				}break;
			}
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = fly[id].mX;
        wp->y = fly[id].mY;
        wp->z = fly[id].mZ;
        wp->o = fly[id].mO;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

	void Destroy()
	{
		delete this;
	};

protected:

    uint32 m_entry;
    bool m_useSpell;
	bool VAZRUDEN_LAND;
	bool NAZAN_LAND;
    uint32 m_FireballCooldown;
    uint32 m_ConeOfFireCooldown;
    uint32 m_fCastCount;
    uint32 m_currentWP;
    SpellEntry *infoConeOfFire, *infoLiquidFlame, *infoFireball, *infoSummonVazruden;
	int nrspells;
};

void SetupHellfireRamparts(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_BLEEDING_HOLLOW_ARCHER, &BLEEDINGHOLLOWARCHERAI::Create);
	mgr->register_creature_script(CN_BLEEDING_HOLLOW_DARKCASTER, &BLEEDINGHOLLOWDARKCASTERAI::Create);
	mgr->register_creature_script(CN_BLEEDING_HOLLOW_SCRYER, &BLEEDINGHOLLOWSCRYERAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BEASTMASTER, &BONECHEWERBEASTMASTERAI::Create);	
	mgr->register_creature_script(CN_BONECHEWER_DESTROYER, &BONECHEWERDESTROYERAI::Create);	
	
	mgr->register_creature_script(CN_BONECHEWER_HUNGERER, &BONECHEWERHUNGERERAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_RAVENER, &BONECHEWERRAVENERAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_RIPPER, &BONECHEWERRIPPERAI::Create);
	mgr->register_creature_script(CN_FIENDISH_HOUND, &FIENDISHHOUNDAI::Create);
	mgr->register_creature_script(CN_HELLFIRE_WATCHER, &HELLFIREWATCHERAI::Create);
	mgr->register_creature_script(CN_SHATTERED_HAND_WARHOUND, &SHATTEREDHANDWARHOUNDAI::Create);
	mgr->register_creature_script(CN_WATCHKEEPER_GARGOLMAR, &WATCHKEEPERGARGOLMARAI::Create);
	mgr->register_creature_script(CN_OMOR_THE_UNSCARRED, &OmorTheUnscarredAI::Create);
	/*mgr->register_creature_script(CN_HELLFIRE_SENTRY, &HELLFIRESENTRYAI::Create);
	mgr->register_creature_script(CN_VAZRUDEN, &VAZRUDENAI::Create);
	mgr->register_creature_script(CN_NAZAN, &NAZANAI::Create);*/
}
