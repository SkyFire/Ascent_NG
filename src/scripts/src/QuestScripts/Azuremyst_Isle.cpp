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

//#define BLIZZLIKE

class DraeneiFishingNet : public GossipScript
{
public:
	void GossipHello( Object* pObject, Player* pPlayer, bool AutoSend )
	{
		if ( pObject == NULL || pObject->GetTypeId() != TYPEID_ITEM || pPlayer == NULL )
			return;

		QuestLogEntry* QuestEntry = pPlayer->GetQuestLogForEntry( 9452 );
		if ( QuestEntry == NULL )
			return;

#ifndef BLIZZLIKE
		//if ( QuestEntry->GetMobCount( 0 ) >= QuestEntry->GetQuest()->required_mobcount[ 0 ] )
		//	return;
#endif
		if ( pPlayer->GetMapMgr() == NULL )
			return;

		// Meh, double object looking - we should find a way to remove this
		GameObject* School = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 181616 );
		if ( School == NULL || pPlayer->CalcDistance( School ) > 5.0f )
			return;

#ifdef BLIZZLIKE
		sEventMgr.AddEvent( School, &GameObject::Despawn, static_cast< uint32 >( 20000 ), EVENT_GAMEOBJECT_ITEM_SPAWN, 1000, 1, 0 );
#else
		School->Despawn( 20000 );
#endif
		pPlayer->CastSpell( pPlayer, dbcSpell.LookupEntry( TO_ITEM( pObject )->GetProto()->Spells[ 0 ].Id ), false );
		uint32 Chance = RandomUInt( 10 );
		if ( Chance <= 3 )
		{
			Creature* NewCreature = sEAS.SpawnCreature( pPlayer, 17102, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 180000 );
			if ( NewCreature != NULL )
			{
				NewCreature->GetAIInterface()->StopMovement( 500 );
				NewCreature->setAttackTimer( 1000, false );
				NewCreature->m_noRespawn = true;
			};

			return;
		};

		sEAS.AddItem( 23614, pPlayer );
		QuestEntry->SendUpdateAddKill( 1 );
		QuestEntry->UpdatePlayerFields();
		pPlayer->Gossip_Complete();
	};

	void Destroy()
	{
		delete this;
	};
};

typedef std::pair< uint64, Creature* > QuestDefinition;
typedef std::vector< QuestDefinition > QuestCreature;

class TotemofCoo : public QuestScript 
{ 
public:
	void OnQuestStart(Player* pPlayer, QuestLogEntry *pQuest)
	{
		if ( pPlayer == NULL || pPlayer->GetMapMgr() == NULL || pPlayer->GetMapMgr()->GetInterface() == NULL )
			return;

		Creature* pAkida = sEAS.SpawnCreature( pPlayer, 17379, -4183.043457f, -12511.419922f, 44.361786f, 6.05629f, 0 );
		if ( pAkida == NULL )
			return;

		pAkida->m_escorter = pPlayer;   
		pAkida->GetAIInterface()->setMoveType( RUN );
		pAkida->GetAIInterface()->StopMovement( 1000 );
		pAkida->GetAIInterface()->SetAllowedToEnterCombat( false );
		pAkida->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me I shall result you on a place!" );
		pAkida->SetUInt32Value( UNIT_NPC_FLAGS, 0 );
		pAkida->CastSpell( pAkida, 25035, true ); // Apparition Effect

		sEAS.CreateCustomWaypointMap( pAkida );
		sEAS.WaypointCreate( pAkida, -4174.025879f, -12512.800781f, 44.361458f, 2.827430f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -4078.135986f, -12535.500977f, 43.066765f, 5.949394f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -4040.495361f, -12565.537109f, 43.698250f, 5.592041f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -4009.526367f, -12598.929688f, 53.168480f, 5.434962f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3981.581543f, -12635.541602f, 63.896046f, 5.332861f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3953.170410f, -12680.391602f, 75.433006f, 5.218981f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3924.324951f, -12741.846680f, 95.187035f, 5.124734f, 0, 256, 16995 );
		sEAS.WaypointCreate( pAkida, -3920.791260f, -12746.218750f, 96.887978f, 3.271200f, 0, 256, 16995 );
		sEAS.EnableWaypoints( pAkida );
		mAkidas.push_back( std::make_pair( pPlayer->GetGUID(), pAkida ) );
	}

	void OnQuestComplete(Player* pPlayer, QuestLogEntry *pQuest)
	{
		uint64 PlayerGuid = pPlayer->GetGUID();
		for ( QuestCreature::iterator itr = mAkidas.begin(); itr != mAkidas.end(); ++itr )
		{
			if ( itr->first == PlayerGuid )
			{
				Creature* pAkida = itr->second;
				if ( pAkida != NULL )							// Can't happen, but whatever :)
				{
					pAkida->CastSpell( pAkida, 30428, true );	// Disparition Effect
					pAkida->Despawn( 5000, 0 );
				}

				mAkidas.erase( itr );
			}
		}
	}

	void OnQuestCancel(Player* pPlayer)
	{
		uint64 PlayerGuid = pPlayer->GetGUID();
		for ( QuestCreature::iterator itr = mAkidas.begin(); itr != mAkidas.end(); ++itr )
		{
			if ( itr->first == PlayerGuid )
			{
				Creature* pAkida = itr->second;
				if ( pAkida != NULL )							// Can't happen, but whatever :)
				{
					pAkida->CastSpell( pAkida, 30428, true );	// Disparition Effect
					pAkida->Despawn( 5000, 0 );
				}

				mAkidas.erase( itr );
			}
		}
	}

	QuestCreature	mAkidas;
};

class TotemofTikti : public QuestScript
{ 
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( !mTarget || !mTarget->GetMapMgr() || !mTarget->GetMapMgr()->GetInterface() )
			return;

		Coo = sEAS.SpawnCreature( mTarget, 17391, -3926.974365f, -12752.285156f, 97.672722f, 4.926801f, 0 );
		if( !Coo )
			return;

		Coo->CastSpell( Coo, 25035, true); // Apparition Effect

		Coo->m_escorter = mTarget;   
		Coo->GetAIInterface()->setMoveType( 11 );
		Coo->GetAIInterface()->StopMovement( 3000 );
		Coo->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me!" );
		Coo->SetUInt32Value( UNIT_NPC_FLAGS, 0 );

		sEAS.CreateCustomWaypointMap( Coo );
		sEAS.WaypointCreate( Coo, -3926.076660f, -12755.158203f, 99.080429f, 5.031188f, 0, 256, 16993 );
		sEAS.WaypointCreate( Coo, -3924.019043f, -12763.895508f, 101.547874f, 5.212689f, 0, 256, 16993 );
		sEAS.EnableWaypoints( Coo );


		Unit* Totem = TO_UNIT(Coo);
		Unit* Plr = TO_UNIT(mTarget);

		string msg = "Ritk kin'chikx azul azure summit...";
		sEventMgr.AddEvent(TO_UNIT(Coo), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, msg.c_str(), EVENT_UNIT_CHAT_MSG, 6000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		if( mTarget->CalcDistance( Coo, mTarget ) <= 10 )
			sEventMgr.AddEvent(TO_UNIT(Coo), &Unit::EventCastSpell, Plr, dbcSpell.LookupEntry(30424), EVENT_CREATURE_UPDATE, 8750, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

		string msg2 = "Coo xip fly... Jump ilos river. Find Tikti.";
		sEventMgr.AddEvent(TO_UNIT(Coo), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 9000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(TO_UNIT(Coo), &Unit::EventCastSpell, Totem, dbcSpell.LookupEntry(30473), EVENT_CREATURE_UPDATE, 12750, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
		Coo->Despawn( 15200, 0 );
	}

private:
	Creature*  Coo;
};

class TotemofYor : public QuestScript
{ 
public:

	void OnQuestStart( Player* mTarget, QuestLogEntry * qLogEntry )
	{
		if( !mTarget || !mTarget->GetMapMgr() || !mTarget->GetMapMgr()->GetInterface() )
			return;

		Tikti = sEAS.SpawnCreature( mTarget, 17392, -3875.430664f, -13125.011719f, 6.822148f, 2.020735f, 0 );
		if( !Tikti )
			return;

		mTarget->CastSpell( Tikti, 25035, true); // Apparition Effect

		Tikti->m_escorter = mTarget;   
		Tikti->GetAIInterface()->setMoveType( WALK );
		Tikti->GetAIInterface()->StopMovement( 3000 );
		Tikti->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Follow me!" );
		Tikti->SetUInt32Value( UNIT_NPC_FLAGS, 0 );

		sEAS.CreateCustomWaypointMap( Tikti );
		sEAS.WaypointCreate( Tikti, -3881.700928f, -13111.898438f, 5.814010f, 1.855801f, 0, 256, 16999 );
		sEAS.WaypointCreate( Tikti, -3886.341553f, -13098.914063f, 3.964841f, 1.855801f, 0, 256, 16999 );
		sEAS.EnableWaypoints( Tikti );

		Unit* Totem = TO_UNIT(Tikti);
		Unit* Plr = TO_UNIT(mTarget);
		string msg = "[Furbolg] Far you mixik tak come. Gaze upon the kitch'kal river. Follow. Ilog to Yor.";
		sEventMgr.AddEvent(TO_UNIT(Tikti), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, msg.c_str(), EVENT_UNIT_CHAT_MSG, 15000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		if( mTarget->CalcDistance( Tikti, mTarget ) <= 10 )
			sEventMgr.AddEvent(TO_UNIT(Tikti), &Unit::EventCastSpell, Plr, dbcSpell.LookupEntry(30430), EVENT_CREATURE_UPDATE, 18000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

		string msg2 = "[Furbolg] Go... Search yitix'kil bottom river. South!";
		sEventMgr.AddEvent(TO_UNIT(Tikti), &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 20000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		sEventMgr.AddEvent(TO_UNIT(Tikti), &Unit::EventCastSpell, Totem, dbcSpell.LookupEntry(30431), EVENT_GMSCRIPT_EVENT, 20000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

		sEventMgr.AddEvent(Tikti, &Creature::Despawn, uint32(0), uint32(0), EVENT_GMSCRIPT_EVENT, 24000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

private:
	Creature*  Tikti;
};

class TotemofVark : public QuestScript
{ 
public:

	void OnQuestStart(Player* mTarget, QuestLogEntry * qLogEntry)
	{
		if( !mTarget || !mTarget->GetMapMgr() || !mTarget->GetMapMgr()->GetInterface() )
			return;

		Yor = sEAS.SpawnCreature( mTarget, 17393, -4634.246582f, -13071.686523f, -14.755350f, 1.569997f, 0 );
		if( !Yor )
			return;
	
		Yor->CastSpell( Yor, 25035, true ); // Apparition Effect

		Yor->m_escorter = mTarget;   
		Yor->GetAIInterface()->StopMovement( 1000 );
		Yor->SetUInt32Value( UNIT_NPC_FLAGS, 0 );
		char msg[256];
		snprintf((char*)msg, 256, "Come, %s . Let us leave the water together, purified.", mTarget->GetName() );
		Yor->SendChatMessage( CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg );

		sEAS.CreateCustomWaypointMap( Yor );
		sEAS.WaypointCreate( Yor, -4650.081055f, -13016.692383f, 1.776296f, 2.021601f, 0, 256, 16393 );
		sEAS.WaypointCreate( Yor, -3886.341553f, -13098.914063f, 3.964841f, 1.855801f, 1000, 256, 16393 );
		sEAS.WaypointCreate( Yor, -4677.421387f, -12983.874023f, 0.833827f, 2.335760f, 0, 256, 16393 ); // Should look player
		sEAS.EnableWaypoints( Yor );

		//We have to set up these pointers first to resolve ambiguity in the event manager template
		Unit* Totem = TO_UNIT(Yor);
		Unit* Plr = TO_UNIT(mTarget);
		// Change to Stillpine form
		sEventMgr.AddEvent(Totem, &Unit::EventCastSpell, Totem, dbcSpell.LookupEntry(30446), EVENT_CREATURE_UPDATE, 15500, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		string msg2 = "[Furbolg] We go now, together. We will seek Vark.";
		sEventMgr.AddEvent(Totem, &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, msg2.c_str(), EVENT_UNIT_CHAT_MSG, 26500, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// Change to nightsaber form
		sEventMgr.AddEvent(Totem, &Unit::EventCastSpell, Totem, dbcSpell.LookupEntry(30448), EVENT_CREATURE_UPDATE, 30000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		string msg3 = "[Furbolg] Follow me to Vark.";
		sEventMgr.AddEvent(Totem, &Unit::SendChatMessage, (uint8)CHAT_MSG_MONSTER_SAY, (uint32)LANG_UNIVERSAL, msg3.c_str(), EVENT_UNIT_CHAT_MSG, 31000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		if( mTarget->CalcDistance( Totem, Plr ) <= 10 )
			sEventMgr.AddEvent(Totem, &Unit::EventCastSpell, Plr, dbcSpell.LookupEntry(30448), EVENT_CREATURE_UPDATE, 31000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	// NEED TO MAKE THE PATH TO THE TOTEM OF VARK
private:
	Creature* Yor;
};

// Chieftain Oomooroo

class ChieftainOomoorooQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ChieftainOomoorooQAI);
    ChieftainOomoorooQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit* mKiller)
	{
		if ( mKiller->IsPlayer() )
		{
			QuestLogEntry *pQuest = TO_PLAYER(mKiller)->GetQuestLogForEntry( 9573 );
			if ( pQuest != NULL && pQuest->GetMobCount( 1 ) < pQuest->GetQuest()->required_mobcount[1] )
			{
				pQuest->SetMobCount( 1, pQuest->GetMobCount( 1 ) + 1 );
				pQuest->SendUpdateAddKill( 1 );
				pQuest->UpdatePlayerFields();
			}
		}
	}
};

// Healing The Lake

bool HealingTheLake(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
		return true;

	Player* pPlayer = TO_PLAYER( pSpell->u_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 9294 );
	if ( pQuest == NULL )
		return true;

	if ( pQuest->GetMobCount( 0 ) < pQuest->GetQuest()->required_mobcount[0] )
	{
		pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
		pQuest->SendUpdateAddKill( 0 );
		pQuest->UpdatePlayerFields();
		
		return true;
	}
	
	return true;
}

void SetupAzuremystIsle( ScriptMgr * mgr )
{
	//mgr->register_item_gossip_script( 23654, CREATE_GOSSIPSCRIPT( DraeneiFishingNet ) );
	/*mgr->register_quest_script( 9539, CREATE_QUESTSCRIPT( TotemofCoo ) );
	mgr->register_quest_script( 9540, CREATE_QUESTSCRIPT( TotemofTikti ));
	mgr->register_quest_script( 9541, CREATE_QUESTSCRIPT( TotemofYor ) );
	mgr->register_quest_script( 9542, CREATE_QUESTSCRIPT( TotemofVark ) );*/
	mgr->register_creature_script(17189, &ChieftainOomoorooQAI::Create);
	mgr->register_dummy_spell(28700, &HealingTheLake);
}
