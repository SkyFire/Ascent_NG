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

bool CrystalOfDeepShadows(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player* plr = TO_PLAYER(pSpell->u_caster);
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10833);
  
  if(qle == NULL)
    return true;

  qle->SetMobCount(0, 1);
  qle->SendUpdateAddKill(0);
  qle->UpdatePlayerFields();

  return true;
}
// Infiltrating Dragonmaw Fortress Quest
class InfiltratingDragonmawFortressQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(InfiltratingDragonmawFortressQAI);
    InfiltratingDragonmawFortressQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10836);
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

class KneepadsQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(KneepadsQAI);
    KneepadsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if(!mKiller)
			return;

		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = NULL;
			en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10703);
			if (en == NULL)
			{
				en = (TO_PLAYER(mKiller))->GetQuestLogForEntry(10702);
				if (en == NULL)
				{
					return;
				}
			}

			if(en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
			}
		}
		return;
	}
};

bool ToLegionHold(uint32 i, Aura* pAura, bool apply)
{
	if ( pAura == NULL || pAura->GetUnitCaster() == NULL || !pAura->GetUnitCaster()->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pAura->GetUnitCaster() );

	Creature* pJovaanCheck = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-3310.743896f, 2951.929199f, 171.132538f, 21633);
	if ( pJovaanCheck != NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 10563 );
	if ( pQuest == NULL )
	{
		pQuest = pPlayer->GetQuestLogForEntry( 10596 );
		if ( pQuest == NULL )
			return true;
	}

	if ( apply )
	{

		pPlayer->SetUInt32Value( UNIT_FIELD_DISPLAYID, 20366 );
		pPlayer->Root();
		Creature* pJovaan = sEAS.SpawnCreature( pPlayer, 21633, -3310.743896f, 2951.929199f, 171.132538f, 5.054039f, 0 );	// Spawn Jovaan
		if ( pJovaan != NULL )
		{
			pJovaan->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
			if ( pJovaan->GetAIInterface() != NULL )
			{
				pJovaan->GetAIInterface()->SetAllowedToEnterCombat( false );
			}
		}
		GameObject* pGameObject = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 184834);
		if ( pGameObject != NULL )
		{
			pGameObject->Despawn(60000);
			pPlayer->UpdateNearbyGameObjects();
		}
	}
	else
	{
		if ( pQuest->GetMobCount( 2 ) < pQuest->GetQuest()->required_mobcount[2] )
		{
			pQuest->SetMobCount( 2, pQuest->GetMobCount( 2 ) + 1 );
			pQuest->SendUpdateAddKill( 2 );
			pQuest->UpdatePlayerFields();
		}
	
		pPlayer->SetUInt32Value( UNIT_FIELD_DISPLAYID, pPlayer->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		pPlayer->UnRoot();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Deathbringer Jovaan
#define CN_DEATHBRINGER_JOVAAN	21633

//WP Coords Wait Times
struct WPWaitTimes{
	Coords mCoords; 
	uint32 WaitTime;
};
const WPWaitTimes DeathbringerJovaanWP[] =
{
	{ { }, 0},
	{ { -3310.743896f, 2951.929199f, 171.132538f, 5.054039f, Flag_Walk }, 0 }, 
	{ { -3308.501221f, 2940.098389f, 171.025772f, 5.061895f, Flag_Walk }, 0 },
	{ { -3306.261203f, 2933.843210f, 170.934145f, 5.474234f, Flag_Walk }, 44000 },
	{ { -3310.743896f, 2951.929199f, 171.132538f, 1.743588f, Flag_Walk }, 0 }
};

class DeathbringerJovaanAI : public AscentScriptCreatureAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(DeathbringerJovaanAI, AscentScriptCreatureAI);
	DeathbringerJovaanAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		mJovaanTimer = INVALIDATE_TIMER;
		mJovaanPhase = -1;

		for (int i = 1; i < 5; ++i)
		{
			AddWaypoint(CreateWaypoint(i, DeathbringerJovaanWP[i].WaitTime, DeathbringerJovaanWP[i].mCoords.mAddition, DeathbringerJovaanWP[i].mCoords));
		}	
	}
	
	void AIUpdate()
	{
		if (IsTimerFinished(mJovaanTimer))
		{
			switch( mJovaanPhase )
			{
				case 0:
					{
						AscentScriptCreatureAI *pRazuunAI = SpawnCreature(21502, -3300.47f, 2927.22f, 173.870f, 2.42924f, false);	// Spawn Razuun
						if ( pRazuunAI != NULL )
						{
							pRazuunAI->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
							pRazuunAI->SetCanEnterCombat(false);
							pRazuunAI->SetMoveType(Move_DontMoveWP);
							pRazuunAI->SetCanMove(false);
						}
						_unit->SetStandState(STANDSTATE_KNEEL);
						_unit->Emote(EMOTE_ONESHOT_TALK);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Everything is in readiness, warbringer.");
						mJovaanPhase = 1;
						ResetTimer(mJovaanTimer, 6000);
					}
					break;
				case 1:
					{
						_unit->Emote(EMOTE_ONESHOT_TALK);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Warbringer, that will require the use of all the hold's infernals. It may leave us vulnerable to a counterattack.");
						mJovaanPhase = 2;
						ResetTimer(mJovaanTimer, 11000);
					}
					break;
				case 2:
					{
						_unit->SetStandState(STANDSTATE_STAND);
						mJovaanPhase = 3;
						ResetTimer(mJovaanTimer, 1000);
					}
					break;
				case 3:
					{
						_unit->Emote(EMOTE_ONESHOT_SALUTE);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It shall be as you say, warbringer. One last question, if I may...");
						mJovaanPhase = 4;
						ResetTimer(mJovaanTimer, 10000);
					}
					break;
				case 4:
					{
						_unit->Emote(EMOTE_ONESHOT_QUESTION);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What's in the crate?");
						mJovaanPhase = 5;
						ResetTimer(mJovaanTimer, 10000);
					}
					break;
				case 5:
					{
						_unit->Emote(EMOTE_ONESHOT_SALUTE);
						mJovaanPhase = -1;
						RemoveTimer(mJovaanTimer);
					}
					break;
			}
		}
		ParentClass::AIUpdate();
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		switch (iWaypointId)
		{
			case 3:
				{	
					RegisterAIUpdateEvent(1000);
					_unit->Emote(EMOTE_ONESHOT_POINT);
					mJovaanPhase = 0;
					mJovaanTimer = AddTimer(1500);
				}
				break;
			case 4:
				{
					Despawn(0, 0);
				}
				break;
		}
	}

	int32	mJovaanTimer;
	int32	mJovaanPhase;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Warbringer Razuun
#define CN_WARBRINGER_RAZUUN	21502

class WarbringerRazuunAI : public AscentScriptCreatureAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(WarbringerRazuunAI, AscentScriptCreatureAI);
	WarbringerRazuunAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		RegisterAIUpdateEvent(1000);
		mRazuunTimer = AddTimer(800);
		mRazuunPhase = 0;
	}

	void AIUpdate()
	{
		if (IsTimerFinished(mRazuunTimer))
		{
			switch( mRazuunPhase )
			{
				case 0:
					{
						_unit->Emote(EMOTE_ONESHOT_TALK);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Doom Lord Kazzak will be pleased. You are to increase the pace of your attacks. Destroy the orcish and dwarven strongholds with all haste.");
						mRazuunPhase = 1;
						ResetTimer(mRazuunTimer, 9000);
					}
					break;
				case 1:
					{
						_unit->Emote(EMOTE_ONESHOT_TALK);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Don't worry about that. I've increased production at the Deathforge. You'll have all the infernals you need to carry out your orders. Don't fail, Jovaan.");
						mRazuunPhase = 2;
						ResetTimer(mRazuunTimer, 15000);
					}
					break;
				case 2:
					{
						_unit->Emote(EMOTE_ONESHOT_QUESTION);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Yes?");
						mRazuunPhase = 3;
						ResetTimer(mRazuunTimer, 8000);
					}
					break;
				case 3:
					{
						_unit->Emote(EMOTE_ONESHOT_QUESTION);
						_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Crate? I didn't send you a crate, Jovaan. Don't you have more important things to worry about? Go see to them!");
						mRazuunPhase = 4;
						ResetTimer(mRazuunTimer, 5000);
					}
					break;
				case 4:
					{
						mRazuunPhase = -1;
						RemoveTimer(mRazuunTimer);
						Despawn(0, 0);
					}
					break;
			}
		}
		ParentClass::AIUpdate();
	}

	int32	mRazuunTimer;
	int32	mRazuunPhase;
};


bool Carcass(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
	if(!pSpell->u_caster->IsPlayer())
		return true;

	Player*	pPlayer = TO_PLAYER(pSpell->u_caster);
	QuestLogEntry	*pQuest = pPlayer->GetQuestLogForEntry( 10804 );
	Creature*	NetherDrake = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 21648);
	GameObject* FlayerCarcass = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 185155);

	if ( FlayerCarcass == NULL )
	{
		FlayerCarcass = sEAS.SpawnGameobject(pPlayer, 185155, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, 1, 0, 0, 0, 0);
		FlayerCarcass->Despawn(60000);
	}
	if ( NetherDrake == NULL )
		return true;

	if ( NetherDrake->HasActiveAura(38502) )
		return true;

	if( pQuest!=NULL && pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		NetherDrake->CastSpell(NetherDrake, dbcSpell.LookupEntry( 38502 ), true);
		NetherDrake->GetAIInterface()->m_moveFly = true;
		NetherDrake->GetAIInterface()->MoveTo(pPlayer->GetPositionX(), pPlayer->GetPositionY()+2, pPlayer->GetPositionZ(), 0);
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 )+1);
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
	}
	return true;
}

bool EatenRecently(uint32 i, Aura* pAura, bool apply)
{
	if ( pAura == NULL || pAura->GetUnitCaster() == NULL || pAura->GetUnitCaster()->IsPlayer() )
		return true;
	Creature* NetherDrake = TO_CREATURE( pAura->GetUnitCaster() );
	
	if (NetherDrake == NULL)
		return true;

	if ( apply )
	{
		NetherDrake->GetAIInterface()->SetAllowedToEnterCombat(false);
		NetherDrake->Emote(EMOTE_ONESHOT_EAT);
	}
	else
	{
		NetherDrake->GetAIInterface()->SetAllowedToEnterCombat(true);
		NetherDrake->GetAIInterface()->m_moveFly = true;
		NetherDrake->GetAIInterface()->MoveTo(NetherDrake->GetSpawnX(), NetherDrake->GetSpawnY(), NetherDrake->GetSpawnZ(), NetherDrake->GetSpawnO());
	}
	return true;
}

class NeltharakusTale_Gossip : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
		if (plr->GetQuestLogForEntry(10814))
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10613, plr);
			Menu->AddItem( 0, "I am listening, dragon", 1);
            Menu->SendTo(plr);
		}
    }

    void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;
		
        switch(IntId)
        {
        case 1:
			{
				GossipMenu * Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10614, plr);
				Menu->AddItem( 0, "But you are dragons! How could orcs do this to you?", 2);
				Menu->SendTo(plr);
            }break;
        case 2:
			{
				GossipMenu * Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10615, plr);
				Menu->AddItem( 0, "Your mate?", 3);
				Menu->SendTo(plr);
            }break;
        case 3:
			{
				GossipMenu * Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10616, plr);
				Menu->AddItem( 0, "I have battled many beasts, dragon. I will help you.", 4);
				Menu->SendTo(plr);
            }break;
        case 4:
			{
				QuestLogEntry *pQuest = plr->GetQuestLogForEntry(10814);
				if ( pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
				{
					pQuest->SetMobCount(0, 1);
					pQuest->SendUpdateAddKill(0);
					pQuest->UpdatePlayerFields();
				}
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Warbringer Razuun
#define CN_ENSLAVED_NETHERWING_DRAKE	21722

class EnslavedNetherwingDrakeAI : public AscentScriptCreatureAI
{
public:
	ASCENTSCRIPT_FACTORY_FUNCTION(EnslavedNetherwingDrakeAI, AscentScriptCreatureAI);
	EnslavedNetherwingDrakeAI(Creature* pCreature) : AscentScriptCreatureAI(pCreature)
	{
		Coords WayPoint = { _unit->GetPositionX(), _unit->GetPositionY()+30, _unit->GetPositionZ()+100, _unit->GetOrientation(), Flag_Fly };
		SetCanMove(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH | UNIT_FLAG_NOT_ATTACKABLE_2);
		AddWaypoint(CreateWaypoint(1, 0, Flag_Fly, WayPoint));
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if (iWaypointId == 1)
		{
			Despawn(0, 3*60*1000);
		}
	}
};

bool ForceofNeltharakuSpell(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
	if(pSpell->u_caster->IsPlayer() == false)
		return true;

	Player*	pPlayer= TO_PLAYER(pSpell->u_caster);
	Unit*		pUnit	= TO_UNIT(pPlayer->GetMapMgr()->GetCreature(GET_LOWGUID_PART(pPlayer->GetSelection())));

	if(pUnit == NULL)
		return true;

	if(!pUnit->IsCreature())
		return true;

	Creature*		 pTarget	= TO_CREATURE(pUnit);
	QuestLogEntry	 *pQuest	= pPlayer->GetQuestLogForEntry(10854);
	if(pQuest == NULL)
		return true;

	if(pTarget->GetEntry() == 21722 && pPlayer->CalcDistance(pUnit)<30)
	{
		if ( pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pTarget->CastSpell(pPlayer, dbcSpell.LookupEntry(38775), true);
			pQuest->SetMobCount(0, pQuest->GetMobCount(0)+1);
			pQuest->SendUpdateAddKill(0);
			pQuest->UpdatePlayerFields();
			if ( pTarget->GetScript() != NULL )
			{
				AscentScriptCreatureAI *pDrakeAI = static_cast<AscentScriptCreatureAI*>(pTarget->GetScript());
				pDrakeAI->SetCanMove(true);
				pDrakeAI->SetWaypointToMove(0);
			}
		}
	}
	return true;
}

bool UnlockKarynakuChains(uint32 i, Spell* pSpell) // Becoming a Shadoweave Tailor
{
	if(pSpell->u_caster->IsPlayer() == false)
		return true;

	Player*	pPlayer= TO_PLAYER(pSpell->u_caster);
	QuestLogEntry	 *pQuest	= pPlayer->GetQuestLogForEntry(10872);
	if(pQuest == NULL)
		return true;

	if ( pQuest && pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
	{
		pQuest->SetMobCount(0, pQuest->GetMobCount(0)+1);
		pQuest->SendUpdateAddKill(0);
		pQuest->UpdatePlayerFields();
	}
	return true;
}

class KarynakuChains : public GameObjectAIScript
{
public:
	KarynakuChains(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject* GO) { return new KarynakuChains(GO); }

	void OnActivate(Player* pPlayer)
	{
		QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(10872);
		
		if(pQuest == NULL)
			return;

		pQuest->SetMobCount(0, pQuest->GetMobCount(0)+1);
		pQuest->SendUpdateAddKill(0);
		pQuest->UpdatePlayerFields();
	}
};

void SetupShadowmoon(ScriptMgr * mgr)
{
	mgr->register_creature_script(11980, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21718, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21719, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21720, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22253, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22274, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(22331, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(23188, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21717, &InfiltratingDragonmawFortressQAI::Create);
	mgr->register_creature_script(21878, &KneepadsQAI::Create);
	mgr->register_creature_script(21879, &KneepadsQAI::Create);
	mgr->register_creature_script(21864, &KneepadsQAI::Create);
	mgr->register_creature_script(CN_DEATHBRINGER_JOVAAN, &DeathbringerJovaanAI::Create);
	mgr->register_creature_script(CN_WARBRINGER_RAZUUN, &WarbringerRazuunAI::Create);
	mgr->register_creature_script(CN_ENSLAVED_NETHERWING_DRAKE, &EnslavedNetherwingDrakeAI::Create);

	mgr->register_gameobject_script(185156, &KarynakuChains::Create);

	mgr->register_dummy_aura(37097, &ToLegionHold);
	mgr->register_dummy_aura(38502, &EatenRecently);
	
	mgr->register_dummy_spell(39094, &CrystalOfDeepShadows);
	mgr->register_dummy_spell(38439, &Carcass);
	mgr->register_dummy_spell(38762, &ForceofNeltharakuSpell);

	GossipScript * NeltharakusTaleGossip = (GossipScript*) new NeltharakusTale_Gossip;
	mgr->register_gossip_script(21657, NeltharakusTaleGossip);
}
