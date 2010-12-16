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

class ScourgeGryphonOne : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		TaxiPath * path = sTaxiMgr.GetTaxiPath( 1053 );
		plr->TaxiStart( path, 26308, 0 );
	}

	void Destroy()
	{
		delete this;
	};
};

class ScourgeGryphonTwo : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		TaxiPath * path = sTaxiMgr.GetTaxiPath( 1054 );
		plr->TaxiStart( path, 26308, 0 );
	}

	void Destroy()
	{
		delete this;
	};
};

#define CN_INITIATE_1				29519
#define CN_INITIATE_2				29565
#define CN_INITIATE_3				29567
#define CN_INITIATE_4				29520

class AcherusSoulPrison : GameObjectAIScript
{
public:
    AcherusSoulPrison(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
    static GameObjectAIScript *Create(GameObject* GO) 
	{ 
	return new AcherusSoulPrison(GO); 
	}

    void OnActivate(Player* pPlayer)
    {
        QuestLogEntry * en = pPlayer->GetQuestLogForEntry(12848);
        if(!en)
        return;
            
            float SSX = pPlayer->GetPositionX();
            float SSY = pPlayer->GetPositionY();
            float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
            
            Creature* pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, SSO);
		     
			if(!pCreature || !pCreature->isAlive())
            return;
			
			if (pCreature->GetEntry() == CN_INITIATE_1 || pCreature->GetEntry() == CN_INITIATE_2 || pCreature->GetEntry() == CN_INITIATE_3 || pCreature->GetEntry() == CN_INITIATE_4)
			{
			pPlayer->SendChatMessage(CHAT_MSG_SAY, LANG_UNIVERSAL, "I give you the key to your salvation");
            pCreature->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
            pCreature->GetAIInterface()->SetNextTarget(pPlayer);
            pCreature->GetAIInterface()->AttackReaction(pPlayer, 1, 0);
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have committed a big mistake, demon");
			
			if(en->GetMobCount(0) != 0)
			return;

			en->SetMobCount(0, 1);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
			}
		
    }
};

bool PreparationForBattle(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
	return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 12842 );
	if ( pQuest != NULL )
	{
		if ( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
			pQuest->SendQuestComplete();
		}
	}
	return true;
};

void SetupDeathKnight(ScriptMgr * mgr)
{
	GossipScript * SGO = (GossipScript*) new ScourgeGryphonOne();
	mgr->register_gossip_script(29488, SGO);
	GossipScript * SGT = (GossipScript*) new ScourgeGryphonTwo();
	mgr->register_gossip_script(29501, SGT);
	
	mgr->register_gameobject_script(191588, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191577, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191580, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191581, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191582, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191583, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191584, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191585, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191586, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191587, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191589, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191590, &AcherusSoulPrison::Create);
	
	mgr->register_dummy_spell(53341, &PreparationForBattle);
	mgr->register_dummy_spell(53343, &PreparationForBattle);
}