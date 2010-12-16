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
#include "../Common/EasyFunctions.h"
#include "../Common/Base.h"

// The Bladespire Threat Quest
class BladespireQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BladespireQAI);
	BladespireQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10503);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

class IntotheSoulgrinder : public QuestScript
{   
public:
	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		Creature* qg = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 22941);
		if(qg == NULL)
			return;
		qg->GetMapMgr()->GetInterface()->SpawnCreature(23053, 2794.978271f, 5842.185547f, 35.911819f, 0, true, false, 0, 0);
	}
};

class MagnetoAura : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(MagnetoAura);
	MagnetoAura(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->CastSpell(_unit, 37136, true);
	}
};

class powerconv : public GameObjectAIScript
{
public:
	powerconv(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new powerconv(GO); }

	void OnActivate(Player* pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10584);
		if(qle == NULL)
			return;

		Creature* magneto = sEAS.SpawnCreature(pPlayer, 21729, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, 0);
		if (magneto != NULL)
		{
			magneto->Despawn(5*60*1000, 0);
		}

		_gameobject->Despawn(300000);
	}
};

class NetherEgg : public GameObjectAIScript
{
public:
	NetherEgg(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new NetherEgg(GO); }

	void OnActivate(Player* pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(10609);
		if(qle == NULL)
			return;

		Creature* whelp = sEAS.SpawnCreature(pPlayer, 20021, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, 0);
		if (whelp != NULL)
		{
			whelp->Despawn(5*60*1000, 0);
		}

		_gameobject->Despawn(300000);
	}
};
		
		

class FunnyDragon : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FunnyDragon);
	FunnyDragon(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		RegisterAIUpdateEvent(5000);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
		_unit->GetAIInterface()->m_canMove = false;
		i = 1;
	}

	void AIUpdate()
	{
		switch(i)
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Muahahahahaha! You fool! you've released me from my banishment in the interstices between space and time!");
			break;
		case 2:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "All of Draenor shall quake beneath my feet! i Will destroy this world and reshape it in my immage!");
			break;
		case 3:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Where shall i Begin? i cannot bother myself with a worm such as yourself. Theres a World to be Conquered!");
			break;
		case 4:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "No doubt the fools that banished me are long dead. i shall take the wing and survey my new demense, Pray to whatever gods you hold dear that we do not meet again.");
			_unit->Despawn(5000, 0);
			break;
		}

		++i;
	}

	uint32 i;
};

class LegionObelisk : public GameObjectAIScript
{
public:
	LegionObelisk(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new LegionObelisk(GO); }

	void OnActivate(Player* pPlayer)
	{
		if( pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetInterface() == NULL )
				return;

		GameObject* obelisk1 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2898.92, 4759.29, 277.408, 185198);
		GameObject* obelisk2 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2942.3, 4752.28, 285.553, 185197);
		GameObject* obelisk3 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2834.39, 4856.67, 277.632, 185196);
		GameObject* obelisk4 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2923.37, 4840.36, 278.45, 185195);
		GameObject* obelisk5 = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(2965.75, 4835.25, 277.949, 185193);
		
		if( obelisk1 && obelisk2 && obelisk3 && obelisk4 && obelisk5 )
		{
			if(obelisk1->GetState() == 0 && obelisk2->GetState() == 0 && obelisk3->GetState() == 0 && obelisk4->GetState() == 0 && obelisk5->GetState() == 0)
			{
				sEAS.SpawnCreature(pPlayer, 19963, 2943.59, 4779.05, 284.49, 1.89, 60*5*1000 );
			}
		}

		if (obelisk1 != NULL)
			sEventMgr.AddEvent(TO_OBJECT(obelisk1), &Object::SetByte, (uint32)GAMEOBJECT_BYTES_1, (uint32)GAMEOBJECT_BYTES_STATE, (uint8)1, EVENT_UNK, 10000, 0, 1);
		if (obelisk2 != NULL)
			sEventMgr.AddEvent(TO_OBJECT(obelisk2), &Object::SetByte, (uint32)GAMEOBJECT_BYTES_1, (uint32)GAMEOBJECT_BYTES_STATE, (uint8)1, EVENT_UNK, 10000, 0, 1);
		if (obelisk3 != NULL)
			sEventMgr.AddEvent(TO_OBJECT(obelisk3), &Object::SetByte, (uint32)GAMEOBJECT_BYTES_1, (uint32)GAMEOBJECT_BYTES_STATE, (uint8)1, EVENT_UNK, 10000, 0, 1);
		if (obelisk4 != NULL)
			sEventMgr.AddEvent(TO_OBJECT(obelisk4), &Object::SetByte, (uint32)GAMEOBJECT_BYTES_1, (uint32)GAMEOBJECT_BYTES_STATE, (uint8)1, EVENT_UNK, 10000, 0, 1);
		if (obelisk5 != NULL)
			sEventMgr.AddEvent(TO_OBJECT(obelisk5), &Object::SetByte, (uint32)GAMEOBJECT_BYTES_1, (uint32)GAMEOBJECT_BYTES_STATE, (uint8)1, EVENT_UNK, 10000, 0, 1);
	}

};

class BloodmaulQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BloodmaulQAI);
    BloodmaulQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if ( mKiller == NULL || !mKiller->IsPlayer() )
			return;

		Player* pPlayer = TO_PLAYER(mKiller);
		QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10502 );
		if ( pQuest == NULL )
		{
			pQuest = pPlayer->GetQuestLogForEntry( 10505 );
			if ( pQuest == NULL )
			{
				return;
			}
		}

		if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
		{
			uint32 NewCount = pQuest->GetMobCount( 0 ) + 1;
			pQuest->SetMobCount( 0, NewCount );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
		}
	}
};

class Thuk_the_DefiantAI : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(Thuk_the_DefiantAI);
	
	Thuk_the_DefiantAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
	}
	void OnLoad()
	{ 	
		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
	}
    void OnDied(Unit* mKiller)
    {
       RemoveAIUpdateEvent();
    }
	void OnTargetDied(Unit* mTarget)
    {		
		_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
		_unit->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
		_unit->_setFaction();
    }
};

class Stasis_Chamber_Alpha : public GameObjectAIScript
{
public:
	Stasis_Chamber_Alpha(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) {
		return new Stasis_Chamber_Alpha(GO);
	}

	void OnActivate(Player* pPlayer)	
	{
		if(pPlayer->GetQuestLogForEntry(10974))
		{
			Creature*  pCreature = NULL;
			pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(3989.094482f, 6071.562500f, 266.416656f, 22920);			
			if (pCreature != NULL)
			{
				pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
				pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
				pCreature->GetAIInterface()->SetNextTarget(pPlayer);
				pCreature->GetAIInterface()->AttackReaction(pPlayer, 1);
				pCreature->_setFaction();
			}
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : Stasis Chambers of Bash'ir");
		}
	}
};

// Protecting Our Own
bool ProtectingOurOwn(uint32 i, Spell* pSpell)
{
   if(pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer())
      return true;

   Player* plr = TO_PLAYER(pSpell->u_caster);
   QuestLogEntry *qle = plr->GetQuestLogForEntry(10488);
   
   if(qle == NULL)
      return true;

   if ( qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[0] )
   {
      uint32 NewCount = qle->GetMobCount( 0 ) + 1;
      qle->SetMobCount( 0, NewCount );
      qle->SendUpdateAddKill( 0 );
      qle->UpdatePlayerFields();
   }

   return true;
}

//////////////////////////////////////////////////////////////////////////
/////// Bladespire Brute/Shaman/Cook
#define CN_BLADESPIRE_OGRE_1				19995
#define CN_BLADESPIRE_OGRE_2				19998
#define CN_BLADESPIRE_OGRE_3				20756

class BladespireOgreAI : public AscentScriptCreatureAI
{
public:
   ASCENTSCRIPT_FACTORY_FUNCTION(BladespireOgreAI, AscentScriptCreatureAI);
   BladespireOgreAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
   {

   }
};

//////////////////////////////////////////////////////////////////////////
/////// Bloodmaul Brutebane Stout Trigger
#define CN_BLOODMAUL_BRUTEBANE_STOUT_TRIGGER    21241

class BrutebaneStoutTriggerAI : public AscentScriptCreatureAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(BrutebaneStoutTriggerAI, AscentScriptCreatureAI);
	BrutebaneStoutTriggerAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
	  _unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);	  
      _unit->_setFaction();

	  SetCanMove(false);

	  plr = _unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() );
	  Ogre = GetNearestCreature(CN_BLADESPIRE_OGRE_1);
	  if(Ogre == NULL)
      {
         Ogre = GetNearestCreature(CN_BLADESPIRE_OGRE_2);
         if(Ogre == NULL)
         {
            Ogre = GetNearestCreature(CN_BLADESPIRE_OGRE_3);
			if(Ogre == NULL){
				return;
			}
         }
      }
	  Ogre->MoveTo(_unit);
	  RegisterAIUpdateEvent(1000);
   }

   void AIUpdate()
   {
	   if( Ogre == NULL )
		   return;
      if( GetRange( Ogre ) <= 5 )
      {
		 Ogre->SetDisplayWeaponIds (28562,0);
		 Ogre->GetUnit()->SetUInt32Value( UNIT_NPC_EMOTESTATE, 92 );
         Ogre->GetUnit()->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 35 );		 
		 Ogre->GetUnit()->SetStandState( STANDSTATE_SIT );
         Ogre->GetUnit()->_setFaction();
         NdGo = GetNearestGameObject(184315);
		 if( NdGo == NULL )
			 return;
		 NdGo->Despawn(0);
         Ogre->Despawn(60*1000, 3*60*1000);
		 if( plr == NULL )
			 return;
         QuestLogEntry * qle = plr->GetQuestLogForEntry( 10512 );

         if( qle!=NULL && qle->GetMobCount( 0 ) < qle->GetQuest()->required_mobcount[0] )
         {
            qle->SetMobCount( 0, qle->GetMobCount( 0 )+1);
            qle->SendUpdateAddKill( 0 );
            qle->UpdatePlayerFields();
         }
		 Despawn(0, 0);
      }	  
      ParentClass::AIUpdate();
   }

   Player*					plr;
   GameObject*				Keg;
   GameObject*				NdGo;
   AscentScriptCreatureAI*	Ogre;
};

void SetupBladeEdgeMountains(ScriptMgr * mgr)
{
	mgr->register_creature_script(19995, &BladespireQAI::Create);
	mgr->register_creature_script(21296, &BladespireQAI::Create);
	mgr->register_creature_script(20765, &BladespireQAI::Create);
	mgr->register_creature_script(20766, &BladespireQAI::Create);
	mgr->register_creature_script(19998, &BladespireQAI::Create);
	mgr->register_creature_script(21731, &MagnetoAura::Create);
	mgr->register_creature_script(21823, &FunnyDragon::Create);
	mgr->register_creature_script(19957, &BloodmaulQAI::Create);
	mgr->register_creature_script(19991, &BloodmaulQAI::Create);
	mgr->register_creature_script(21238, &BloodmaulQAI::Create);
	mgr->register_creature_script(19952, &BloodmaulQAI::Create);
	mgr->register_creature_script(21294, &BloodmaulQAI::Create);
	mgr->register_creature_script(19956, &BloodmaulQAI::Create);
	mgr->register_creature_script(19993, &BloodmaulQAI::Create);
	mgr->register_creature_script(19992, &BloodmaulQAI::Create);
	mgr->register_creature_script(19948, &BloodmaulQAI::Create);
	mgr->register_creature_script(22384, &BloodmaulQAI::Create);
	mgr->register_creature_script(22160, &BloodmaulQAI::Create);
	mgr->register_creature_script(19994, &BloodmaulQAI::Create);
	mgr->register_creature_script(22920, &Thuk_the_DefiantAI::Create);

	mgr->register_creature_script(CN_BLOODMAUL_BRUTEBANE_STOUT_TRIGGER, &BrutebaneStoutTriggerAI::Create);
	mgr->register_creature_script(CN_BLADESPIRE_OGRE_1, &BladespireOgreAI::Create);
	mgr->register_creature_script(CN_BLADESPIRE_OGRE_2, &BladespireOgreAI::Create);
	mgr->register_creature_script(CN_BLADESPIRE_OGRE_3, &BladespireOgreAI::Create);

	mgr->register_quest_script(11000, CREATE_QUESTSCRIPT(IntotheSoulgrinder));

	mgr->register_gameobject_script(184867, &NetherEgg::Create);
	mgr->register_gameobject_script(184906, &powerconv::Create);
	mgr->register_gameobject_script( 185198, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185197, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185196, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185195, &LegionObelisk::Create);
	mgr->register_gameobject_script( 185193, &LegionObelisk::Create);
	mgr->register_gameobject_script(185512, &Stasis_Chamber_Alpha::Create);

	mgr->register_dummy_spell(32578, &ProtectingOurOwn);
}
