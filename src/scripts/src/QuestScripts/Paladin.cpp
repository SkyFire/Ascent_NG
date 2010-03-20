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

bool SymbolOfLife(uint32 i, Spell* pSpell) // Alliance ress. quests
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player* plr = TO_PLAYER(pSpell->u_caster);
  Creature* target = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  
  if(target == NULL)
    return true;

  const uint32 targets[] = {17542, 6177, 6172}; 
  const uint32 quests[] =  {9600,  1783, 1786};
  bool questOk = false;
  bool targetOk = false;

  for(int i = 0; i<3; i++)
  {
    if(target->GetEntry() == targets[i])
    {
      targetOk = true;
      
      break;
    }
  }
  
  if(!targetOk)
    return true;
  
  QuestLogEntry *qle;
  
  for(int i = 0; i<3; i++)
  {
    if(plr->GetQuestLogForEntry(quests[i]) != NULL)
    {
      qle = plr->GetQuestLogForEntry(quests[i]);
      questOk = true;
      
      break;
    }
  }
  
  if(!questOk)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(10*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();
  
  return true;
}

bool FilledShimmeringVessel(uint32 i, Spell* pSpell) // Blood Elf ress. quest
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player* plr = TO_PLAYER(pSpell->u_caster);
  
  Creature* target = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  if(target == NULL)
    return true;

  if(target->GetEntry() != 17768)
    return true;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(9685);
  if(qle == NULL)
    return true;

  target->SetStandState(0);
  target->setDeathState(ALIVE);

  target->Despawn(30*1000, 1*60*1000);

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}

class PaladinDeadNPC : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(PaladinDeadNPC);
  PaladinDeadNPC(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnLoad()
  {
    _unit->SetStandState(7);
    _unit->setDeathState(CORPSE);
    _unit->GetAIInterface()->m_canMove = false;
  }
};

/*--------------------------------------------------------------------------------------------------------*/

class GildedBrazier : public GameObjectAIScript
{
public:
	GildedBrazier(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new GildedBrazier(GO); }

	void OnActivate(Player* pPlayer)	
	{
		if(pPlayer->GetQuestLogForEntry(9678))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
			
			GameObject* Brazier = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 181956);
			if (Brazier)
			{
				Brazier->SetState(0);
				pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(17716, SSX, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
			}
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : The First Trial");
		}
	}
};

class stillbladeQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(stillbladeQAI);
    	stillbladeQAI(Creature* pCreature) : CreatureAIScript(pCreature)  
    	{
    	
	}
	
	void OnDied(Unit* mKiller) 
	{
		float SSX = mKiller->GetPositionX();
		float SSY = mKiller->GetPositionY();
		float SSZ = mKiller->GetPositionZ();
		float SSO = mKiller->GetOrientation();
			
		GameObject* Brazier = mKiller->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 181956);
		if (Brazier)
		{
			Brazier->SetState(1);
		}
	}
};

bool DouseEternalFlame(uint32 i, Spell* pSpell)
{
	if (pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer())
		return true;

	Player* plr = TO_PLAYER(pSpell->u_caster);
	QuestLogEntry *qle = plr->GetQuestLogForEntry(9737);
	if(qle == NULL)
		return true;

	GameObject* Flame = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(3678, -3640, 139, 182068);
	if(Flame != NULL)
	{
		if(plr->CalcDistance(plr, Flame) < 30)
			if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
			{
				qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
			}
	}
	return true;
}

void SetupPaladin(ScriptMgr * mgr)
{
  mgr->register_dummy_spell(8593, &SymbolOfLife);
  mgr->register_dummy_spell(31225, &FilledShimmeringVessel);
  mgr->register_creature_script(17768, &PaladinDeadNPC::Create);
  mgr->register_creature_script(17542, &PaladinDeadNPC::Create);
  mgr->register_creature_script(6177, &PaladinDeadNPC::Create);
  mgr->register_creature_script(6172, &PaladinDeadNPC::Create);
  mgr->register_gameobject_script(181956, &GildedBrazier::Create);
  mgr->register_creature_script(17716, &stillbladeQAI::Create);
  mgr->register_dummy_spell(31497, &DouseEternalFlame);
}
