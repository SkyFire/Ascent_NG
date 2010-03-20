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

// -----------------------------------------------------------------------------
// Quest 12532 - Flown the Coop!
// Quest 12702 - Chicken Party! (by bartus

bool ChickenNet( uint32 i, Spell* pSpell )
{
	if( !pSpell->u_caster->IsPlayer() )
        return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12532 );
	if( qle == NULL )
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12702 );
		if( qle == NULL )
			return true;
	}

	
	if( !pSpell->GetUnitTarget()->IsCreature() )
		return true;

	Creature* Chicken = TO_CREATURE( pSpell->GetUnitTarget() );

	if( Chicken != NULL && Chicken->GetEntry() == 28161 )
	{
		Chicken->Despawn( 5000, 360000 );
		pPlayer->CastSpell( pSpell->u_caster, dbcSpell.LookupEntry( 51037 ), true );
	}

   return true;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void SetupSholazarBasin( ScriptMgr * mgr )
{
   mgr->register_dummy_spell(51959, &ChickenNet);
}