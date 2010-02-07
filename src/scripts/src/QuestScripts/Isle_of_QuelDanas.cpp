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



/*--------------------------------------------------------------------------------------------------------*/
// Crisis at the Sunwell

class ScryingOrb : public GameObjectAIScript
{
public:
	ScryingOrb(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new ScryingOrb(GO); }

	void OnActivate(Player* pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(11490);
  		if(qle)
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			
			GameObject* Orb = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( SSX, SSY, SSZ, 187578);
			if (Orb)
			{
				Orb->SetState(0);
  				qle->SetMobCount(0, 1);
  				qle->SendUpdateAddKill(0);
  				qle->UpdatePlayerFields();
			}
			return;
		}
		else
		{
			pPlayer->BroadcastMessage("Missing required quest : The Scryer's Scryer");
		}
	}
};



/*--------------------------------------------------------------------------------------------------------*/
// Erratic Behavior

bool ConvertingSentry(uint32 i, Spell* pSpell)
{
  Player* caster = pSpell->p_caster;
  if(caster == NULL)
    return true;

  Creature* target = TO_CREATURE(pSpell->GetUnitTarget());
  if(target == NULL)
    return true;

  // Erratic Sentry: 24972
  if(target->GetEntry() != 24972)
    return true;

  if(target->isAlive())
    return true;

  QuestLogEntry *qle = NULL;
  qle = caster->GetQuestLogForEntry(11525);
  if(qle == NULL)
  {
	qle = caster->GetQuestLogForEntry(11524);
	if(qle == NULL)
	{
       return true;
    }
  }

  if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
    return true;

  qle->SetMobCount(0, qle->GetMobCount(0)+1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  target->Despawn(500, 2*60*1000);

  return true;
}

bool OrbOfMurlocControl(uint32 i, Spell* pSpell)
{
	if(pSpell->m_caster->IsPlayer() == false)
		return true;

	Player* plr = TO_PLAYER(pSpell->u_caster);

	QuestLogEntry *pQuest = plr->GetQuestLogForEntry(11541);
	if(pQuest == NULL)
		return true;

	Unit* mTarget;
	Creature* cTarget;

	for(ObjectSet::iterator itr = pSpell->m_caster->GetInRangeSetBegin(); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsUnit())
			mTarget = TO_UNIT(*itr);
		else
			continue;

		if(pSpell->m_caster->CalcDistance(mTarget) > 5 )
			continue;

		cTarget = TO_CREATURE(mTarget);

		if(cTarget->GetEntry() == 25084)
		{
		  if(pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
		  {
			pQuest->SetMobCount(0, pQuest->GetMobCount(0) + 1);
			pQuest->SendUpdateAddKill(0);		
			Creature* FreedGreengill = sEAS.SpawnCreature(plr, 25085, cTarget->GetPositionX(), cTarget->GetPositionY(), cTarget->GetPositionZ(), cTarget->GetOrientation(), 0);
			FreedGreengill->Despawn(6*60*1000, 0);
			cTarget->Despawn(0, 6*60*1000);
			pQuest->UpdatePlayerFields();
			return true;
		  }  
		}
  }
  return true;
}

#define GO_FIRE 183816

struct Coords
{
	float x;
	float y;
	float z;
	float o;
};
static Coords BloodoathFire[] =
{
	{13329.4f, -6994.70f, 14.5219f, 1.38938f},
 	{13315.4f, -6990.72f, 14.7647f, 1.25979f}
};
static Coords SinlorenFire[] =
{
	{13214.3f, -7059.19f, 17.5717f, 1.58573f},
	{13204.2f, -7059.38f, 17.5717f, 1.57787f}
};
static Coords DawnchaserFire[] =
{
	{13284.1f, -7152.65f, 15.9774f, 1.44828f},
	{13273.0f, -7151.21f, 15.9774f, 1.39723f}
};

bool ShipBombing(uint32 i, Spell* pSpell)
{
	Player* plr = TO_PLAYER(pSpell->u_caster);
	if(!plr)
		return true;

	if(!pSpell->u_caster->IsPlayer())
		return true;

	QuestLogEntry *qle = NULL;
	qle = plr->GetQuestLogForEntry(11542);
	if(qle == NULL)
	{
		qle = plr->GetQuestLogForEntry(11543);
		if(qle == NULL)
		{
			return true;
		}
	}
	
	GameObject* Sinloren = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13200.232422, -7049.176270, 3.838517, 550000);
	GameObject* Bloodoath = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13319.419922, -6988.779785, 4.002993, 550000);
	GameObject* Dawnchaser = plr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(13274.51625, -7145.434570, 4.770292, 550000);
	
	GameObject* obj = NULL;

	if(Sinloren != NULL)
	{
		if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
		{
			if(plr->CalcDistance(plr, Sinloren) < 15)
			{
				qle->SetMobCount(0, qle->GetMobCount(0)+1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
				for(uint8 i = 0; i < 2; i++)
				{
					obj = sEAS.SpawnGameobject(plr, GO_FIRE, SinlorenFire[i].x, SinlorenFire[i].y, SinlorenFire[i].z, SinlorenFire[i].o, 1, 0, 0, 0, 0);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	if(Bloodoath != NULL)
	{
		if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
		{
			if(plr->CalcDistance(plr, Bloodoath) < 15)
			{
				qle->SetMobCount(1, qle->GetMobCount(1)+1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
				for(uint8 i = 0; i < 2; i++)
				{
					obj = sEAS.SpawnGameobject(plr, GO_FIRE, BloodoathFire[i].x, BloodoathFire[i].y, BloodoathFire[i].z, BloodoathFire[i].o, 1, 0, 0, 0, 0);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	if(Dawnchaser != NULL)
	{
		if(plr->CalcDistance(plr, Dawnchaser) < 15)
		{
			if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
			{
				qle->SetMobCount(2, qle->GetMobCount(2)+1);
				qle->SendUpdateAddKill(2);
				qle->UpdatePlayerFields();
				for(uint8 i = 0; i < 2; i++)
				{
					obj = sEAS.SpawnGameobject(plr, GO_FIRE, DawnchaserFire[i].x, DawnchaserFire[i].y, DawnchaserFire[i].z, DawnchaserFire[i].o, 1, 0, 0, 0, 0);
					sEAS.GameobjectDelete(obj, 2*60*1000);
				}
			}
		}
	}
	return true;
}

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Ayren Cloudbreaker Gossip

class SCRIPT_DECL AyrenCloudbreaker_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12252, plr);
		if (plr->GetQuestLogForEntry(11532) || plr->GetQuestLogForEntry(11533))
			Menu->AddItem( 0, "Speaking of action, I've been ordered to undertake an air strike.", 1);
		if (plr->GetQuestLogForEntry(11543) || plr->GetQuestLogForEntry(11542))
			Menu->AddItem( 0, "I need to intercept the Dawnblade reinforcements.", 2);
		Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 1:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 779 );
				plr->TaxiStart( path, 22840, 0 );
				plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		case 2:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 784 );
				plr->TaxiStart( path, 22840, 0 );
				plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Unrestrained Dragonhawk Gossip

class SCRIPT_DECL UnrestrainedDragonhawk_Gossip : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12371, plr);
		if (plr->GetQuestLogForEntry(11543) || plr->GetQuestLogForEntry(11542))
			Menu->AddItem( 0, "<Ride the dragonhawk to Sun's Reach>", 1);
		Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 1:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 788 );
				plr->TaxiStart( path, 22840, 0 );
				plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
			}
			break;
		}
	}
	void Destroy()
	{
		delete this;
	}
};

// The Battle for the Sun's Reach Armory
class TheBattleForTheSunReachArmory : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TheBattleForTheSunReachArmory);
    TheBattleForTheSunReachArmory(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if (mKiller->IsPlayer())
		{
			QuestLogEntry *qle = (TO_PLAYER(mKiller))->GetQuestLogForEntry(11537);
			if( qle == NULL )
			{
				qle = (TO_PLAYER(mKiller))->GetQuestLogForEntry(11538);
				if( qle == NULL )
					return;
			}

			if( qle->GetMobCount( 1 ) < qle->GetQuest()->required_mobcount[ 1 ] )
			{
				uint32 newcount = qle->GetMobCount( 1 ) + 1;
				qle->SetMobCount( 1, newcount );
				qle->SendUpdateAddKill( 1 );
				qle->UpdatePlayerFields();
				return;
			}
		}
	}
};

bool ImpaleEmissary(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster->IsPlayer() == false)
	return true;

	Player* pPlayer = TO_PLAYER(pSpell->u_caster);
	if( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(11537);
	if( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry(11538);
		if( pQuest == NULL )
			return true;
	}
	
	Creature* mEmissary = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 25003);
	if( mEmissary == NULL )
		return true;
		
	if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 0 ] )
	{
		uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
		pQuest->SetMobCount( 0, newcount );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		mEmissary->Despawn(0, 3*60*1000);
	}
	return true;
}

void SetupIsleOfQuelDanas(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(187578, &ScryingOrb::Create);

	mgr->register_dummy_spell(45109, &OrbOfMurlocControl);
	mgr->register_dummy_spell(44997, &ConvertingSentry);
	mgr->register_dummy_spell(45115, &ShipBombing);
	
	mgr->register_creature_script(24999, &TheBattleForTheSunReachArmory::Create);
	mgr->register_creature_script(25001, &TheBattleForTheSunReachArmory::Create);
	mgr->register_creature_script(25002, &TheBattleForTheSunReachArmory::Create);

	mgr->register_dummy_spell(45030, &ImpaleEmissary);

	//GOSSIP
	GossipScript * AyrenCloudbreakerGossip = (GossipScript*) new AyrenCloudbreaker_Gossip;
	mgr->register_gossip_script(25059, AyrenCloudbreakerGossip);
	GossipScript * UnrestrainedDragonhawkGossip = (GossipScript*) new UnrestrainedDragonhawk_Gossip;
	mgr->register_gossip_script(25236, UnrestrainedDragonhawkGossip);
}
