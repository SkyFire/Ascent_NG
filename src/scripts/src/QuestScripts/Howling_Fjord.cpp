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

// War is Hell
bool WarIsHell(uint32 i, Spell* pSpell)
{
	if( !pSpell->u_caster->IsPlayer() )
		return true;

	Player* plr = TO_PLAYER(pSpell->u_caster);
	if( plr == NULL )
		return true;

	Creature* target = TO_CREATURE(plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 24008 ));
	if( target == NULL )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(11270);
  
	if( qle == NULL )
		return true;
 
	GameObject* obj = NULL;
 
	if( qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount( 0, qle->GetMobCount( 0 ) + 1 );
		qle->SendUpdateAddKill( 0 );
		
		obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1, 0, 0, 0, 0);
		sEAS.GameobjectDelete(obj, 1*30*1000);
	}
	target->Despawn( 2000, 60*1000 );
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
	return true;
}

// A Lesson in Fear
bool PlantForsakenBanner(uint32 i, Spell* pSpell)
{
	if(pSpell->u_caster->IsPlayer() == false)
		return true;

	Player* pPlayer = TO_PLAYER(pSpell->u_caster);
	if( pPlayer == NULL )
		return true;

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry(11282);
	if( pQuest == NULL )
		return true;
	
	Creature* target = TO_CREATURE(pSpell->GetUnitTarget());
	if (target == NULL)
		return true;

	float X = target->GetPositionX();
	float Y = target->GetPositionY();
	float Z = target->GetPositionZ();

	uint32 cit = target->GetEntry();
	
	switch(cit)
	{
	case 24161:
		{
			if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 0 ] )
			{
				uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
				pQuest->SetMobCount( 0, newcount );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
				target->Despawn(0, 3*60*1000);
			}	
		}
		break;
	case 24016:
		{
			if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 1 ] )
			{
				uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
				pQuest->SetMobCount( 0, newcount );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
				target->Despawn(0, 3*60*1000);
			}	
		}
		break;
	case 24162:
		{
			if( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[ 2 ] )
			{
				uint32 newcount = pQuest->GetMobCount( 0 ) + 1;
				pQuest->SetMobCount( 0, newcount );
				pQuest->SendUpdateAddKill( 0 );
				pQuest->UpdatePlayerFields();
				target->Despawn(0, 3*60*1000);
			}	
		}
		break;
  }
  return true;
}

void SetupHowlingFjord(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(42793, &WarIsHell); 
	//mgr->register_dummy_spell(43178, &PlantForsakenBanner);
}
