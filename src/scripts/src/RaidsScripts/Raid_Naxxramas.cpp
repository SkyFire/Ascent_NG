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
#include "Raid_Naxxramas.h"

/////////////////////////////////////////////////////////////////////////////////
////// The Arachnid Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Carrion Spinner
CarrionSpinnerAI::CarrionSpinnerAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( CARRION_SPINNER_POISON_BOLT_HEROIC, Target_Self, 15, 0, 15 );
	else
		AddSpell( CARRION_SPINNER_POISON_BOLT_NORMAL, Target_Self, 15, 0, 15 );

	// Does it work ?
	AddSpell( CARRION_SPINNER_WEB_WRAP, Target_RandomPlayer, 8, 0, 10, 0, 40 );
};

void CarrionSpinnerAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Dread Creeper
DreadCreeperAI::DreadCreeperAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( DREAD_CREEPER_VEIL_OF_SHADOW_HEROIC, Target_Self, 15, 0, 10 );
	else
		AddSpell( DREAD_CREEPER_VEIL_OF_SHADOW_NORMAL, Target_Self, 15, 0, 10 );
};

void DreadCreeperAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Cultist
NaxxramasCultistAI::NaxxramasCultistAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( NAXXRAMAS_CULTIST_KNOCKBACK_HEROIC, Target_Destination, 10, 0, 10, 0, 8 );
	else
		AddSpell( NAXXRAMAS_CULTIST_KNOCKBACK_NORMAL, Target_Destination, 10, 0, 10, 0, 8 );
};

void NaxxramasCultistAI::Destroy()
{
	delete this;
};
//Necro Stalker AI
/////////////////////////////////////////////////////////////////////////////////
////// Venom Stalker
VenomStalkerAI::VenomStalkerAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	// Do those really work ?
	if ( IsHeroic() )
		AddSpell( VENOM_STALKER_POISON_CHARGE_HEROIC, Target_RandomPlayer, 10, 0, 10, 0, 40 );
	else
		AddSpell( VENOM_STALKER_POISON_CHARGE_NORMAL, Target_RandomPlayer, 10, 0, 10, 0, 40 );
};

void VenomStalkerAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Tomb Horror
TombHorrorAI::TombHorrorAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( TOMB_HORROR_CRYPT_SCARAB_SWARM_HEROIC, Target_Self, 7, 3, 20 );
		AddSpell( TOMB_HORROR_CRYPT_SCARABS_HEROIC, Target_RandomPlayer, 8, 2, 10, 0, 40 );
	}
	else
	{
		AddSpell( TOMB_HORROR_CRYPT_SCARAB_SWARM_HEROIC, Target_Self, 7, 3, 20 );
		AddSpell( TOMB_HORROR_CRYPT_SCARABS_NORMAL, Target_RandomPlayer, 10, 1.5, 10, 0, 40 );
	};

	AddSpell( TOMB_HORROR_SPIKE_VOLLEY, Target_Self, 10, 0.5f, 15 );
};

void TombHorrorAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Acolyte
NaxxramasAcolyteAI::NaxxramasAcolyteAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( NAXXRAMAS_ACOLYTE_SHADOW_BOLT_VOLLEY_HEROIC, Target_RandomPlayerDestination, 10, 3, 5, 0, 30 );
		AddSpell( NAXXRAMAS_ACOLYTE_ARCANE_EXPLOSION_HEROIC, Target_Self, 10, 2, 15 );
	}
	else
	{
		AddSpell( NAXXRAMAS_ACOLYTE_SHADOW_BOLT_VOLLEY_NORMAL, Target_RandomPlayerDestination, 10, 3, 5, 0, 30 );
		AddSpell( NAXXRAMAS_ACOLYTE_ARCANE_EXPLOSION_NORMAL, Target_Self, 10, 2, 15 );
	};
};

void NaxxramasAcolyteAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Vigilant Shade
VigilantShadeAI::VigilantShadeAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	// Does it really work ?
	if ( IsHeroic() )
		AddSpell( NAXXRAMAS_ACOLYTE_SHADOW_BOLT_VOLLEY_HEROIC, Target_RandomPlayerDestination, 10, 0, 15, 0, 30 );
	else
		AddSpell( NAXXRAMAS_ACOLYTE_SHADOW_BOLT_VOLLEY_NORMAL, Target_RandomPlayerDestination, 10, 0, 15, 0, 30 );

	ApplyAura( VIGILANT_SHADE_INVISIBILITY );
};

void VigilantShadeAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	RemoveAura( VIGILANT_SHADE_INVISIBILITY );
};

void VigilantShadeAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	ApplyAura( VIGILANT_SHADE_INVISIBILITY );
};

void VigilantShadeAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Crypt Reaver
CryptReaverAI::CryptReaverAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( CRYPT_REAVER_CLEAVE, Target_Current, 10, 0, 5, 0, 8 );
	AddSpell( CRYPT_REAVER_FRENZY, Target_Self, 7, 0, 40 );
};

void CryptReaverAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Web Wrap
WebWrapAI::WebWrapAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	mPlayerGuid = 0;
};

void WebWrapAI::OnCombatStart( Unit* pTarget )
{
	SetAllowMelee( false );
	SetCanMove( false );
	StopMovement();
};

void WebWrapAI::OnCombatStop( Unit* pTarget )
{
	SetAllowMelee( true );
	SetCanMove( true );
};

void WebWrapAI::OnDied( Unit* pKiller )
{
	// Slower, but safer
	if ( mPlayerGuid != 0 )
	{
		Player* PlayerPtr = objmgr.GetPlayer( mPlayerGuid );
		if ( PlayerPtr != NULL && PlayerPtr->HasAura( MAEXXNA_WEB_WRAP ) )
		{
			PlayerPtr->RemoveAura( MAEXXNA_WEB_WRAP );
			PlayerPtr->UnRoot();
		};

		RemoveAIUpdateEvent();
		mPlayerGuid = 0;
	};
};

void WebWrapAI::AIUpdate()
{
	if ( mPlayerGuid != 0 )
	{
		Player* PlayerPtr = objmgr.GetPlayer( mPlayerGuid );
		if ( PlayerPtr == NULL || !PlayerPtr->isAlive() || !PlayerPtr->HasAura( MAEXXNA_WEB_WRAP ) )
		{
			mPlayerGuid = 0;
			RemoveAIUpdateEvent();
			Despawn();
		}
	};
};

void WebWrapAI::Destroy()
{
	if ( mPlayerGuid != 0 )
	{
		Player* PlayerPtr = objmgr.GetPlayer( mPlayerGuid );
		if ( PlayerPtr != NULL && PlayerPtr->HasAura( MAEXXNA_WEB_WRAP ) )
		{
			PlayerPtr->RemoveAura( MAEXXNA_WEB_WRAP );
			PlayerPtr->UnRoot();
		};

		mPlayerGuid = 0;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Maexxna Spiderling
MaexxnaSpiderlingAI::MaexxnaSpiderlingAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( MAEXXNA_SPIDERLING_NECROTIC_POISON_HEROIC, Target_Current, 10, 0, 20, 0, 8 );
	else
		AddSpell( MAEXXNA_SPIDERLING_NECROTIC_POISON_NORMAL, Target_Current, 10, 0, 20, 0, 8 );
};

void MaexxnaSpiderlingAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Maexxna
MaexxnaAI::MaexxnaAI( Creature* pCreature ) : AscentScriptBossAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( MAEXXNA_POISON_SHOCK_HEROIC, Target_Self, 100, 0, 10 );		// Target_Current with range ?
		AddSpell( MAEXXNA_NECROTIC_POISON_HEROIC, Target_Current, 10, 0, 15, 0, 8 );
	}
	else
	{
		AddSpell( MAEXXNA_POISON_SHOCK_NORMAL, Target_Self, 100, 0, 10 );		// Target_Current with range ?
		AddSpell( MAEXXNA_NECROTIC_POISON_NORMAL, Target_Current, 10, 0, 15, 0, 8 );
	};

	mWebWrapProc = AddSpellFunc( &SpellFunc_MaexxnaWebWrap, Target_RandomPlayerNotCurrent, 0, 0, 0 );
	mAddsSummonTimer = mWebWrapTimer = mWebSprayTimer = INVALIDATE_TIMER;
	mHasEnraged = false;
	mLeftWall = true;
};

void MaexxnaAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mAddsSummonTimer = AddTimer( 30000 );
	mWebSprayTimer = AddTimer( 40000 );
	mWebWrapTimer = AddTimer( 20000 );
	mHasEnraged = false;
	if ( RandomUInt( 1 ) == 1 )
		mLeftWall = !mLeftWall;
};

void MaexxnaAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	RemoveTimer( mWebWrapTimer );
};

void MaexxnaAI::AIUpdate()
{
	if ( IsTimerFinished( mAddsSummonTimer ) )
	{
		AscentScriptCreatureAI* Spiderling = NULL;
		for ( int32 i = 0; i < 8; ++i )
		{
			Spiderling = SpawnCreature( CN_MAEXXNA_SPIDERLING );
			if ( Spiderling != NULL )
			{
				Spiderling->GetUnit()->m_noRespawn = true;
				Spiderling->SetDespawnWhenInactive( true );
				Spiderling->AggroRandomPlayer( 1000 );
			};
		};

		ResetTimer( mAddsSummonTimer, 40000 );
	};

	if ( !IsCasting() )
	{
		if ( !mHasEnraged && GetHealthPercent() <= 30 )
		{
			if ( IsHeroic() )
				ApplyAura( MAEXXNA_FRENZY_HEROIC );
			else
				ApplyAura( MAEXXNA_FRENZY_NORMAL );

			mHasEnraged = true;
		}
		else if ( IsTimerFinished( mWebSprayTimer ) )
		{
			if ( IsHeroic() )
				ApplyAura( MAEXXNA_WEB_SPRAY_HEROIC );
			else
				ApplyAura( MAEXXNA_WEB_SPRAY_NORMAL );

			ResetTimer( mWebSprayTimer, 40000 );
		}
		else if ( IsTimerFinished( mWebWrapTimer ) )
		{
			if ( IsHeroic() )
				CastSpellNowNoScheduling( mWebWrapProc );

			CastSpellNowNoScheduling( mWebWrapProc );
			ResetTimer( mWebWrapTimer, 40000 );
		};
	};

	ParentClass::AIUpdate();
};

void MaexxnaAI::Destroy()
{
	delete this;
};

void SpellFunc_MaexxnaWebWrap( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	MaexxnaAI* Maexxna = ( pCreatureAI != NULL ) ? static_cast< MaexxnaAI* >( pCreatureAI ) : NULL;
	if ( Maexxna != NULL )
	{
		// Is target really added everytime and isn't this check redundant ?
		if ( pTarget == NULL || !pTarget->IsPlayer() || pTarget->HasAura( MAEXXNA_WEB_WRAP ) || Maexxna->GetUnit() == NULL || Maexxna->GetUnit()->GetMapMgr() == NULL )
			return;

		uint32 Id = RandomUInt( 2 );
		if ( !Maexxna->mLeftWall )
			Id += 3;

		WebWrapAI* WebWrap = static_cast< WebWrapAI* >( Maexxna->SpawnCreature( CN_WEB_WRAP, WebWrapPos[ Id ].mX, WebWrapPos[ Id ].mY, WebWrapPos[ Id ].mZ, pTarget->GetOrientation() ) );
		if ( WebWrap == NULL )
			return;

		WebWrap->GetUnit()->m_noRespawn = true;
		WebWrap->RegisterAIUpdateEvent( 5000 );
		WebWrap->mPlayerGuid = TO_PLAYER( pTarget )->GetGUID();

		if ( pTarget->GetCurrentSpell() != NULL )
			pTarget->GetCurrentSpell()->cancel();

		// Somewhy root does not apply at all
		TO_PLAYER( pTarget )->SafeTeleport( Maexxna->GetUnit()->GetMapId(), Maexxna->GetUnit()->GetInstanceID(), LocationVector( WebWrapPos[ Id ].mX, WebWrapPos[ Id ].mY, WebWrapPos[ Id ].mZ ) );
		pTarget->CastSpell( pTarget, MAEXXNA_WEB_WRAP, true );
	};
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Worshipper
NaxxramasWorshipperAI::NaxxramasWorshipperAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( NAXXRAMAS_WORSHIPPER_FIREBALL_HEROIC, Target_Current, 10, 2.5, 0, 0, 45 );
	else
		AddSpell( NAXXRAMAS_WORSHIPPER_FIREBALL_NORMAL, Target_Current, 10, 2.5, 0, 0, 45 );

	mGrandWidow = NULL;
	mPossessed = false;
};

void NaxxramasWorshipperAI::OnCastSpell( uint32 pSpellId )
{
	/*if ( pSpellId == NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE && IsAlive() )
	{
		_unit->DealDamage( _unit, _unit->GetUInt32Value( UNIT_FIELD_HEALTH ), 0, 0, NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE );
		uint8 Buffer[ 100 ];
		StackPacket DataPacket( SMSG_SPELLINSTAKILLLOG, Buffer, 100 );
		DataPacket << _unit->GetGUID() << _unit->GetGUID() << NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE;
		_unit->SendMessageToSet( &DataPacket, true );
	};*/
};

void NaxxramasWorshipperAI::OnDied( Unit* pKiller )
{
	ParentClass::OnDied( pKiller );
	if ( mGrandWidow != NULL )//&& !IsHeroic() )
	{
		if ( GetRange( static_cast< AscentScriptCreatureAI* >( mGrandWidow ) ) <= 15.0f )
		{
			for ( set< NaxxramasWorshipperAI* >::iterator Iter = mGrandWidow->mWorshippers.begin(); Iter != mGrandWidow->mWorshippers.end(); ++Iter )
			{
				if ( !( *Iter )->IsAlive() )
					continue;

				// Must check if it does not crash if creature is already casting
				( *Iter )->ApplyAura( NAXXRAMAS_WORSHIPPER_MIND_EXHAUSTION );
			};

			// Should be applied on Grand Widow, but is on the enemies - to script ?
			//ApplyAura( NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE );
			// I don't like the way we apply it
			Aura* WidowEmbrace( new Aura( dbcSpell.LookupEntry( NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE ), 30000, _unit, mGrandWidow->GetUnit() ) );
			if ( WidowEmbrace != NULL )
				_unit->AddAura( WidowEmbrace );

			// Not sure about new Frenzy Timer
			mGrandWidow->ResetTimer( mGrandWidow->mFrenzyTimer, 60000 + RandomUInt( 20 ) * 1000 );
			if ( mGrandWidow->GetUnit()->HasAura( GRAND_WIDOW_FAERLINA_FRENZY_NORMAL ) )
				mGrandWidow->GetUnit()->RemoveAura( GRAND_WIDOW_FAERLINA_FRENZY_NORMAL );	// Really needed ?
			else if ( mGrandWidow->GetUnit()->HasAura( GRAND_WIDOW_FAERLINA_FRENZY_HEROIC ) )
				mGrandWidow->GetUnit()->RemoveAura( GRAND_WIDOW_FAERLINA_FRENZY_HEROIC );	// Really needed ?
			else
			{
				mGrandWidow->ResetTimer( mGrandWidow->mPoisonVolleyBoltTimer, 30000 );
				return;
			};

			mGrandWidow->ResetTimer( mGrandWidow->mPoisonVolleyBoltTimer, 60000 );
		};
	};
};

void NaxxramasWorshipperAI::AIUpdate()
{
	/*if ( !mPossessed && _unit->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE ) )
	{
		Player* PlayerPtr = objmgr.GetPlayer( _unit->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) );
		if ( PlayerPtr != NULL )
		{
			AI_Spell* NewSpell = new AI_Spell;
			NewSpell->entryId = _unit->GetEntry();
			NewSpell->minrange = 0;
			NewSpell->maxrange = 15;
			NewSpell->spell = dbcSpell.LookupEntry( NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE );
			NewSpell->spelltargetType = TTYPE_SOURCE;
			NewSpell->cooldown = NewSpell->cooldowntime = 0;
			NewSpell->autocast_type = 0;
			NewSpell->first_use = true;
			NewSpell->custom_* = false;
			NewSpell->floatMisc1 = NewSpell->Misc2 = 0;
			NewSpell->procChance = NewSpell->procCount = NewSpell->procCounter = 0;
			NewSpell->spellType = STYPE_DEBUFF;
			NewSpell->agent = AGENT_SPELL;
			_unit->GetAIInterface()->m_spells.push_back( NewSpell );

			list< uint32 > Spells;
			Spells.push_back( NAXXRAMAS_WORSHIPPER_WIDOW_EMBRACE );
			list< uint32 >::iterator Iter = Spells.begin();
			
			WorldPacket DataPacket( SMSG_PET_SPELLS, _unit->GetAIInterface()->m_spells.size() * 4 + 20 );
			DataPacket << _unit->GetGUID();
			DataPacket << static_cast< uint32 >( 0x00000000 );	//unk1
			DataPacket << static_cast< uint32 >( 0x00000101 );	//unk2
			DataPacket << static_cast< uint32 >( 0x00000000 );	//unk3
			DataPacket << static_cast< uint32 >( PET_SPELL_ATTACK );

			for ( uint32 i = 1; i < 10; ++i )
			{
				if ( Iter != Spells.end() )
				{
					DataPacket << static_cast< uint16 >( *Iter ) << static_cast< uint16 >( DEFAULT_SPELL_STATE );
					++Iter;
				}
				else
					DataPacket << static_cast< uint16 >( 0 ) << static_cast< uint8 >( 0 ) << static_cast< uint8 >( i + 5 );
			};

			// Send the rest of the spells.
			DataPacket << static_cast< uint8 >( Spells.size() );
			for ( Iter = Spells.begin(); Iter != Spells.end(); ++Iter )
			{
				DataPacket << static_cast< uint16 >( *Iter ) << static_cast< uint16 >( DEFAULT_SPELL_STATE );
			};
			
			DataPacket << static_cast< uint64 >( 0 );
			PlayerPtr->GetSession()->SendPacket( &DataPacket );
			mPossessed = true;
		};
	}
	else if ( mPossessed && !_unit->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE ) )
		mPossessed = false;

	if ( mPossessed )
		return;*/

	ParentClass::AIUpdate();
};

void NaxxramasWorshipperAI::Destroy()
{
	if ( mGrandWidow != NULL )
	{
		set< NaxxramasWorshipperAI* >::iterator Iter = mGrandWidow->mWorshippers.find( this );
		if ( Iter != mGrandWidow->mWorshippers.end() )
			mGrandWidow->mWorshippers.erase( Iter );

		mGrandWidow = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Naxxramas Follower
NaxxramasFollowerAI::NaxxramasFollowerAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	mCharge = AddSpell( NAXXRAMAS_FOLLOWER_BERSERKER_CHARGE_HEROIC, Target_Current, 0, 0, 0 );
	AddSpellFunc( &SpellFunc_NaxxramasFollowerCharge, Target_RandomPlayer, 8, 0, 20, 0, 40 );
	AddSpell( NAXXRAMAS_FOLLOWER_SILENCE_HEROIC, Target_Self, 10, 0, 15 );
	
	mGrandWidow = NULL;
};

void NaxxramasFollowerAI::Destroy()
{
	if ( mGrandWidow != NULL )
	{
		set< NaxxramasFollowerAI* >::iterator Iter = mGrandWidow->mFollowers.find( this );
		if ( Iter != mGrandWidow->mFollowers.end() )
			mGrandWidow->mFollowers.erase( Iter );

		mGrandWidow = NULL;
	};

	delete this;
};

void SpellFunc_NaxxramasFollowerCharge( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	NaxxramasFollowerAI *NaxxramasFollower = ( pCreatureAI != NULL ) ? static_cast< NaxxramasFollowerAI* >( pCreatureAI ) : NULL;
	if ( NaxxramasFollower != NULL )
	{
		Unit* CurrentTarget = NaxxramasFollower->GetUnit()->GetAIInterface()->GetNextTarget();
		if ( CurrentTarget != NULL && CurrentTarget != pTarget )
		{
			NaxxramasFollower->GetUnit()->GetAIInterface()->AttackReaction( pTarget, 500 );
			NaxxramasFollower->GetUnit()->GetAIInterface()->SetNextTarget( pTarget );
			//NaxxramasFollower->GetUnit()->GetAIInterface()->RemoveThreatByPtr( CurrentTarget );
		};

		NaxxramasFollower->CastSpell( NaxxramasFollower->mCharge );
	};
};

/////////////////////////////////////////////////////////////////////////////////
////// Grand Widow Faerlina
GrandWidowFaerlinaAI::GrandWidowFaerlinaAI( Creature* pCreature ) : AscentScriptBossAI( pCreature )
{
	AscentScriptCreatureAI* AddAI = NULL;
	for ( uint32 i = 0; i < 4; ++i )
	{
		AddAI = SpawnCreature( CN_NAXXRAMAS_WORSHIPPER, 3353.364502f + Worshippers[ i ].mX, -3620.322998f, 260.996857f, 4.725017f );
		if ( AddAI != NULL )
		{
			AddAI->GetUnit()->m_noRespawn = true;
			static_cast< NaxxramasWorshipperAI* >( AddAI )->mGrandWidow = this;
			mWorshippers.insert( static_cast< NaxxramasWorshipperAI* >( AddAI ) );
		};
	};

	if ( IsHeroic() )
	{
		for ( uint32 i = 0; i < 2; ++i )
		{
			AddAI = SpawnCreature( CN_NAXXRAMAS_FOLLOWER, 3353.364502f + Followers[ i ].mX, -3620.322998f, 260.996857f, 4.725017f );
			if ( AddAI != NULL )
			{
				AddAI->GetUnit()->m_noRespawn = true;
				static_cast< NaxxramasFollowerAI* >( AddAI )->mGrandWidow = this;
				mFollowers.insert( static_cast< NaxxramasFollowerAI* >( AddAI ) );
			};
		};

		mPoisonVolleyBolt = AddSpell( GRAND_WIDOW_FAERLINA_POISON_VOLLEY_BOLT_HEROIC, Target_Self, 0, 0, 0 );
		mFrenzy = AddSpell( GRAND_WIDOW_FAERLINA_FRENZY_HEROIC, Target_Self, 0, 0, 0 );
		AddSpell( GRAND_WIDOW_RAIN_OF_FIRE_HEROIC, Target_RandomPlayerDestination, 7, 0, 10, 0, 40 );
	}
	else
	{
		mPoisonVolleyBolt = AddSpell( GRAND_WIDOW_FAERLINA_POISON_VOLLEY_BOLT_NORMAL, Target_Self, 0, 0, 0 );
		mFrenzy = AddSpell( GRAND_WIDOW_FAERLINA_FRENZY_NORMAL, Target_Self, 0, 0, 0 );
		AddSpell( GRAND_WIDOW_RAIN_OF_FIRE_NORMAL, Target_RandomPlayerDestination, 7, 0, 10, 0, 40 );
	};

	Emote( "Your old lives, your mortal desires, mean nothing. You are acolytes of the master now, and you will serve the cause without question! The greatest glory is to die in the master's service!", Text_Yell, 8799 );
	AddEmote( Event_OnCombatStart, "Slay them in the master's name!", Text_Yell, 8794 );
	AddEmote( Event_OnTargetDied, "You have failed!", Text_Yell, 8800 );
	AddEmote( Event_OnTargetDied, "Pathetic wretch!", Text_Yell, 8801 );
	AddEmote( Event_OnDied, "The master... will avenge me!", Text_Yell, 8798 );
	mFrenzy->AddEmote( "You cannot hide from me!", Text_Yell, 8795 );
	mFrenzy->AddEmote( "Kneel before me, worm!", Text_Yell, 8796 );
	mFrenzy->AddEmote( "Run while you still can!", Text_Yell, 8797 );
	mPoisonVolleyBoltTimer = mFrenzyTimer = INVALIDATE_TIMER;

	GameObject* WebGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 3318.65f, -3695.85f, 259.094f, 181235 );
	if ( WebGate != NULL )
		WebGate->SetState( 0 );
};

void GrandWidowFaerlinaAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mPoisonVolleyBoltTimer = AddTimer( 15000 );
	mFrenzyTimer = AddTimer( 60000 + RandomUInt( 20 ) * 1000 );

	GameObject* WebGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 3318.65f, -3695.85f, 259.094f, 181235 );
	if ( WebGate != NULL )
		WebGate->SetState( 1 );

	for ( set< NaxxramasWorshipperAI* >::iterator Iter = mWorshippers.begin(); Iter != mWorshippers.end(); ++Iter )
	{
		( *Iter )->AggroNearestPlayer( 200 );
	};

	for ( set< NaxxramasFollowerAI* >::iterator Iter = mFollowers.begin(); Iter != mFollowers.end(); ++Iter )
	{
		( *Iter )->AggroNearestPlayer( 200 );
	};
};

void GrandWidowFaerlinaAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	mPoisonVolleyBoltTimer = mFrenzyTimer = INVALIDATE_TIMER;

	GameObject* WebGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 3318.65f, -3695.85f, 259.094f, 181235 );
	if ( WebGate != NULL )
		WebGate->SetState( 0 );

	for ( set< NaxxramasWorshipperAI* >::iterator Iter = mWorshippers.begin(); Iter != mWorshippers.end(); ++Iter )
	{
		( *Iter )->mGrandWidow = NULL;
		if ( IsAlive() )
			( *Iter )->Despawn();
	};

	mWorshippers.clear();

	for ( set< NaxxramasFollowerAI* >::iterator Iter = mFollowers.begin(); Iter != mFollowers.end(); ++Iter )
	{
		( *Iter )->mGrandWidow = NULL;
		if ( IsAlive() )
			( *Iter )->Despawn();
	};

	mFollowers.clear();

	if ( IsAlive() )
	{
		AscentScriptCreatureAI* AddAI = NULL;
		for ( uint32 i = 0; i < 4; ++i )
		{
			AddAI = SpawnCreature( CN_NAXXRAMAS_WORSHIPPER, 3353.364502f + Worshippers[ i ].mX, -3620.322998f, 260.996857f, 4.725017f );
			if ( AddAI != NULL )
			{
				AddAI->GetUnit()->m_noRespawn = true;
				static_cast< NaxxramasWorshipperAI* >( AddAI )->mGrandWidow = this;
				mWorshippers.insert( static_cast< NaxxramasWorshipperAI* >( AddAI ) );
			};
		};

		if ( IsHeroic() )
		{
			for ( uint32 i = 0; i < 2; ++i )
			{
				AddAI = SpawnCreature( CN_NAXXRAMAS_FOLLOWER, 3353.364502f + Followers[ i ].mX, -3620.322998f, 260.996857f, 4.725017f );
				if ( AddAI != NULL )
				{
					AddAI->GetUnit()->m_noRespawn = true;
					static_cast< NaxxramasFollowerAI* >( AddAI )->mGrandWidow = this;
					mFollowers.insert( static_cast< NaxxramasFollowerAI* >( AddAI ) );
				};
			};
		};
	};
};

void GrandWidowFaerlinaAI::AIUpdate()
{
	if ( !IsCasting() )
	{
		if ( IsTimerFinished( mPoisonVolleyBoltTimer ) )
		{
			CastSpellNowNoScheduling( mPoisonVolleyBolt );
			ResetTimer( mPoisonVolleyBoltTimer, 15000 );
		}
		else if ( IsTimerFinished( mFrenzyTimer ) )
		{
			CastSpellNowNoScheduling( mFrenzy );
			ResetTimer( mFrenzyTimer, 60000 + RandomUInt( 20 ) * 1000 );
		};
	};

	ParentClass::AIUpdate();
};

void GrandWidowFaerlinaAI::Destroy()
{
	for ( set< NaxxramasWorshipperAI* >::iterator Iter = mWorshippers.begin(); Iter != mWorshippers.end(); ++Iter )
	{
		( *Iter )->mGrandWidow = NULL;
		( *Iter )->Despawn();
	};

	mWorshippers.clear();

	for ( set< NaxxramasFollowerAI* >::iterator Iter = mFollowers.begin(); Iter != mFollowers.end(); ++Iter )
	{
		( *Iter )->mGrandWidow = NULL;
		( *Iter )->Despawn();
	};

	mFollowers.clear();

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Crypt Guard
CryptGuardAI::CryptGuardAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( CRYPT_GUARD_ACID_SPLIT_HEROIC, Target_RandomPlayer, 8, 0, 15, 0, 40 );
	else
		AddSpell( CRYPT_GUARD_ACID_SPLIT_NORMAL, Target_RandomPlayer, 8, 0, 15, 0, 40 );

	AddSpell( CRYPT_GUARD_CLEAVE, Target_Current, 10, 0, 10, 0, 8 );
	mAnubRekhanAI = NULL;
	mEnraged = false;
};

void CryptGuardAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mEnraged = false;
};

void CryptGuardAI::AIUpdate()
{
	if ( !mEnraged && GetHealthPercent() <= 50 && !IsCasting() )
	{
		ApplyAura( CRYPT_GUARD_FRENZY );
		mEnraged = true;
	};

	ParentClass::AIUpdate();
};

void CryptGuardAI::Destroy()
{
	if ( mAnubRekhanAI != NULL )
	{
		set< CryptGuardAI* >::iterator Iter = mAnubRekhanAI->mCryptGuards.find( this );
		if ( Iter != mAnubRekhanAI->mCryptGuards.end() )
			mAnubRekhanAI->mCryptGuards.erase( this );

		mAnubRekhanAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Corpse Scarab
CorpseScarabAI::CorpseScarabAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	mAnubRekhanAI = NULL;
};

void CorpseScarabAI::Destroy()
{
	if ( mAnubRekhanAI != NULL )
	{
		set< CorpseScarabAI* >::iterator Iter = mAnubRekhanAI->mScarabs.find( this );
		if ( Iter != mAnubRekhanAI->mScarabs.end() )
			mAnubRekhanAI->mScarabs.erase( this );

		mAnubRekhanAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Anub'Rekhan
AnubRekhanAI::AnubRekhanAI( Creature* pCreature ) : AscentScriptBossAI( pCreature )
{
	if ( IsHeroic() )
	{
		CryptGuardAI* CryptAI = NULL;
		for ( uint32 i = 0; i < 2; ++i )
		{
			CryptAI = static_cast< CryptGuardAI* >( SpawnCreature( CN_CRYPT_GUARD, CryptGuards[ i ].mX, CryptGuards[ i ].mY, CryptGuards[ i ].mZ, CryptGuards[ i ].mO ) );
			if ( CryptAI != NULL )
			{
				CryptAI->GetUnit()->m_noRespawn = true;
				CryptAI->mAnubRekhanAI = this;
				mCryptGuards.insert( CryptAI );
			};
		};

		AddSpell( ANUBREKHAN_IMPALE_HEROIC, Target_RandomPlayerDestination, 7, 1, 10, 0, 45 );
		mLocustSwarm = AddSpell( ANUBREKHAN_LOCUST_SWARM_HEROIC, Target_Self, 0, 3, 0 );
	}
	else
	{
		AddSpell( ANUBREKHAN_IMPALE_NORMAL, Target_RandomPlayerDestination, 7, 0, 10, 0, 45 );
		mLocustSwarm = AddSpell( ANUBREKHAN_LOCUST_SWARM_NORMAL, Target_Self, 0, 3, 0 );
	};

	AddSpellFunc( &SpellFunc_AnubRekhanCorpseScarabsPlayer, Target_Self, 8, 0, 20 );
	AddSpellFunc( &SpellFunc_AnubRekhanCorpseScarabsCryptGuard, Target_Self, 8, 0, 20 );
	Emote( "Ahh... welcome to my parlor.", Text_Yell, 8788 );
	AddEmote( Event_OnCombatStart, "Just a little taste...", Text_Yell, 8785 );
	AddEmote( Event_OnCombatStart, "There is no way out.", Text_Yell, 8786 );
	AddEmote( Event_OnCombatStart, "Yes, run! It makes the blood pump faster!", Text_Yell, 8787 );
	AddEmote( Event_OnTargetDied, "Shhh... it will all be over soon.", Text_Yell, 8789 );
	AddEmote( Event_OnTaunt, "I hear little hearts beating. Yesss... beating faster now. Soon the beating will stop.", Text_Yell, 8790 );
	AddEmote( Event_OnTaunt, "Where to go? What to do? So many choices that all end in pain, end in death.", Text_Yell, 8791 );
	AddEmote( Event_OnTaunt, "Which one shall I eat first? So difficult to choose. They all smell so delicious...", Text_Yell, 8792 );
	AddEmote( Event_OnTaunt, "Closer now... tasty morsels. I've been too long without food. Without blood to drink.", Text_Yell, 8793 );
	SetEnrageInfo( AddSpell( ANUBREKHAN_BERSERK, Target_Self, 0, 0, 0 ), 600000 );
	mLocustSwarmTimer = mCryptSpawnTimer = INVALIDATE_TIMER;
};

void AnubRekhanAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mLocustSwarmTimer = AddTimer( 70000 + RandomUInt( 50 ) * 1000 );

	if ( IsHeroic() )
	{
		for ( set< CryptGuardAI* >::iterator Iter = mCryptGuards.begin(); Iter != mCryptGuards.end(); ++Iter )
		{
			( *Iter )->AggroRandomPlayer( 200 );
		};
	}
	else
		mCryptSpawnTimer = AddTimer( 20000 );
};

void AnubRekhanAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	mLocustSwarmTimer = mCryptSpawnTimer = INVALIDATE_TIMER;

	for ( set< CryptGuardAI* >::iterator Iter = mCryptGuards.begin(); Iter != mCryptGuards.end(); ++Iter )
	{
		( *Iter )->mAnubRekhanAI = NULL;
		if ( IsAlive() )
			( *Iter )->Despawn();
	};

	mCryptGuards.clear();

	for ( set< CorpseScarabAI* >::iterator Iter = mScarabs.begin(); Iter != mScarabs.end(); ++Iter )
	{
		( *Iter )->mAnubRekhanAI = NULL;
		if ( IsAlive() )
			( *Iter )->Despawn();
	};

	mScarabs.clear();

	if ( IsHeroic() && IsAlive() )
	{
		CryptGuardAI* CryptAI = NULL;
		for ( uint32 i = 0; i < 2; ++i )
		{
			CryptAI = static_cast< CryptGuardAI* >( SpawnCreature( CN_CRYPT_GUARD, CryptGuards[ i ].mX, CryptGuards[ i ].mY, CryptGuards[ i ].mZ, CryptGuards[ i ].mO ) );
			if ( CryptAI != NULL )
			{
				CryptAI->GetUnit()->m_noRespawn = true;
				CryptAI->mAnubRekhanAI = this;
				mCryptGuards.insert( CryptAI );
			};
		};
	};
};

void AnubRekhanAI::AIUpdate()
{
	if ( !IsCasting() )
	{
		if ( mCryptSpawnTimer != INVALIDATE_TIMER && IsTimerFinished( mCryptSpawnTimer ) )
		{
			RemoveTimer( mCryptSpawnTimer );
			CryptGuardAI* CryptAI = static_cast< CryptGuardAI* >( SpawnCreature( CN_CRYPT_GUARD, CryptGuards[ 2 ].mX, CryptGuards[ 2 ].mY, CryptGuards[ 2 ].mZ, CryptGuards[ 2 ].mO ) );
			if ( CryptAI != NULL )
			{
				CryptAI->GetUnit()->m_noRespawn = true;
				CryptAI->mAnubRekhanAI = this;
				mCryptGuards.insert( CryptAI );
				CryptAI->AggroRandomPlayer( 200 );
			};
		};

		if ( IsTimerFinished( mLocustSwarmTimer ) )
		{
			CryptGuardAI* CryptAI = static_cast< CryptGuardAI* >( SpawnCreature( CN_CRYPT_GUARD, CryptGuards[ 2 ].mX, CryptGuards[ 2 ].mY, CryptGuards[ 2 ].mZ, CryptGuards[ 2 ].mO ) );
			if ( CryptAI != NULL )
			{
				CryptAI->GetUnit()->m_noRespawn = true;
				CryptAI->mAnubRekhanAI = this;
				mCryptGuards.insert( CryptAI );
				CryptAI->AggroRandomPlayer( 200 );
			};
	
			CastSpellNowNoScheduling( mLocustSwarm );
			mLocustSwarmTimer = AddTimer( 70000 + RandomUInt( 50 ) * 1000 );
		};
	};

	ParentClass::AIUpdate();
};

void AnubRekhanAI::Destroy()
{
	for ( set< CryptGuardAI* >::iterator Iter = mCryptGuards.begin(); Iter != mCryptGuards.end(); ++Iter )
	{
		( *Iter )->mAnubRekhanAI = NULL;
		( *Iter )->Despawn();
	};

	mCryptGuards.clear();

	for ( set< CorpseScarabAI* >::iterator Iter = mScarabs.begin(); Iter != mScarabs.end(); ++Iter )
	{
		( *Iter )->mAnubRekhanAI = NULL;
		( *Iter )->Despawn();
	};

	mScarabs.clear();

	delete this;
};

void SpellFunc_AnubRekhanCorpseScarabsPlayer( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	AnubRekhanAI *AnubRekhan = ( pCreatureAI != NULL ) ? static_cast< AnubRekhanAI* >( pCreatureAI ) : NULL;
	if ( AnubRekhan != NULL )
	{
		std::vector< pair< Player*, Coords > > PlayerCorpses;
		Player* PlayerPtr = NULL;
		Object* ObjectPtr = NULL;
		for( unordered_set< Player* >::iterator Iter = AnubRekhan->GetUnit()->GetInRangePlayerSetBegin(); Iter != AnubRekhan->GetUnit()->GetInRangePlayerSetEnd(); ++Iter ) 
		{ 
			if ( ( *Iter ) == NULL )
				continue;

			PlayerPtr = *Iter;
			set< uint32 >::iterator PlayerIter = AnubRekhan->mUsedCorpseGuids.find( PlayerPtr->GetGUID() );
			if ( PlayerIter != AnubRekhan->mUsedCorpseGuids.end() )
			{
				if ( PlayerPtr->isAlive() )
					AnubRekhan->mUsedCorpseGuids.erase( PlayerIter );

				continue;
			};

			if ( PlayerPtr->isAlive() )
				continue;

			if ( PlayerPtr->getDeathState() == JUST_DIED )
				ObjectPtr = PlayerPtr;
			else if ( PlayerPtr->getDeathState() == CORPSE )
			{
				if ( PlayerPtr->getMyCorpse() == NULL || PlayerPtr->getMyCorpse()->GetCorpseState() != CORPSE_STATE_BODY )
					continue;

				ObjectPtr = PlayerPtr->getMyCorpse();
			}
			else
				continue;

			if ( AnubRekhan->GetUnit()->CalcDistance( ObjectPtr ) > 60.0f )
				continue;

			Coords ObjectCoords;
			ObjectCoords.mX = ObjectPtr->GetPositionX();
			ObjectCoords.mY = ObjectPtr->GetPositionY();
			ObjectCoords.mZ = ObjectPtr->GetPositionZ();
			ObjectCoords.mO = ObjectPtr->GetOrientation();
			PlayerCorpses.push_back( make_pair( PlayerPtr, ObjectCoords ) );
		};

		if ( PlayerCorpses.size() > 0 )
		{
			uint32 Id = RandomUInt( uint32(PlayerCorpses.size()) - 1 );
			PlayerPtr = PlayerCorpses[ Id ].first;
			AnubRekhan->mUsedCorpseGuids.insert( PlayerPtr->GetGUID() );
			AnubRekhan->ApplyAura( ANUBREKHAN_SUMMON_CORPSE_SCARABS_5 );

			CorpseScarabAI* ScarabAI = NULL;
			for ( uint32 i = 0; i < 5; ++i )
			{
				ScarabAI = static_cast< CorpseScarabAI* >( AnubRekhan->SpawnCreature( CN_CORPSE_SCARAB, PlayerCorpses[ Id ].second.mX, PlayerCorpses[ Id ].second.mY, PlayerCorpses[ Id ].second.mZ, PlayerCorpses[ Id ].second.mO ) );
				if ( ScarabAI != NULL )
				{
					ScarabAI->GetUnit()->m_noRespawn = true;
					ScarabAI->mAnubRekhanAI = AnubRekhan;
					AnubRekhan->mScarabs.insert( ScarabAI );
					ScarabAI->AggroRandomPlayer( 200 );
				};
			};
		};
	};
};

void SpellFunc_AnubRekhanCorpseScarabsCryptGuard( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	AnubRekhanAI *AnubRekhan = ( pCreatureAI != NULL ) ? static_cast< AnubRekhanAI* >( pCreatureAI ) : NULL;
	if ( AnubRekhan != NULL )
	{
		std::vector< Creature* > CryptCorpses;
		Creature* CreaturePtr = NULL;
		for( unordered_set< Object* >::iterator Iter = AnubRekhan->GetUnit()->GetInRangeSetBegin(); Iter != AnubRekhan->GetUnit()->GetInRangeSetEnd(); ++Iter )
		{
			if ( ( *Iter ) == NULL || !( *Iter )->IsCreature() )
				continue;

			CreaturePtr = TO_CREATURE( *Iter );
			if ( CreaturePtr->GetEntry() != CN_CRYPT_GUARD )
				continue;
			
			if ( CreaturePtr->isAlive() || !CreaturePtr->IsInWorld() )
				continue;

			if ( AnubRekhan->GetUnit()->CalcDistance( CreaturePtr ) > 60.0f )
				continue;

			CryptCorpses.push_back( CreaturePtr );
		};

		if ( CryptCorpses.size() > 0 )
		{
			uint32 Id = RandomUInt( uint32(CryptCorpses.size()) - 1 );
			CreaturePtr = CryptCorpses[ Id ];

			AnubRekhan->ApplyAura( ANUBREKHAN_SUMMON_CORPSE_SCARABS_10 );
			float X, Y, Z, O;
			X = CreaturePtr->GetPositionX();
			Y = CreaturePtr->GetPositionY();
			Z = CreaturePtr->GetPositionZ();
			O = CreaturePtr->GetOrientation();

			CorpseScarabAI* ScarabAI = NULL;
			for ( uint32 i = 0; i < 10; ++i )
			{
				ScarabAI = static_cast< CorpseScarabAI* >( AnubRekhan->SpawnCreature( CN_CORPSE_SCARAB, X, Y, Z, O ) );
				if ( ScarabAI != NULL )
				{
					ScarabAI->GetUnit()->m_noRespawn = true;
					ScarabAI->mAnubRekhanAI = AnubRekhan;
					AnubRekhan->mScarabs.insert( ScarabAI );
					ScarabAI->AggroRandomPlayer( 200 );
				};
			};
		};
	};
};

/////////////////////////////////////////////////////////////////////////////////
////// The Plague Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Infectious Ghoul
InfectiousGhoulAI::InfectiousGhoulAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( INFECTIOUS_GHOUL_FLESH_ROT, Target_Current, 10, 0, 15, 0, 8 );
	if ( IsHeroic() )
		AddSpell( INFECTIOUS_GHOUL_REND_HEROIC, Target_Current, 7, 0, 15, 0, 8 );
	else
		AddSpell( INFECTIOUS_GHOUL_REND_NORMAL, Target_Current, 7, 0, 15, 0, 8 );

	mEnraged = false;
};

void InfectiousGhoulAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mEnraged = false;
};

void InfectiousGhoulAI::AIUpdate()
{
	if ( !mEnraged && !IsCasting() && GetHealthPercent() <= 50 )
	{
		if ( IsHeroic() )
			ApplyAura( INFECTIOUS_GHOUL_FRENZY_HEROIC );
		else
			ApplyAura( INFECTIOUS_GHOUL_FRENZY_NORMAL );

		mEnraged = true;
	};

	ParentClass::AIUpdate();
};

void InfectiousGhoulAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Stoneskin Gargoyle
StoneskinGargoyleAI::StoneskinGargoyleAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( STONESKIN_GARGOYLE_ACID_VOLLEY_HEROIC, Target_Self, 10, 0, 5 );
		mStoneskin = AddSpell( STONESKIN_GARGOYLE_STONESKIN_HEROIC, Target_Self, 0, 7, 0 );
	}
	else
	{
		AddSpell( STONESKIN_GARGOYLE_ACID_VOLLEY_NORMAL, Target_Self, 10, 0, 5 );
		mStoneskin = AddSpell( STONESKIN_GARGOYLE_STONESKIN_NORMAL, Target_Self, 0, 7, 0 );
	};
};

bool StoneskinGargoyleAI::HasStoneskin()
{
	return ( _unit->HasAura( STONESKIN_GARGOYLE_STONESKIN_NORMAL ) || _unit->HasAura( STONESKIN_GARGOYLE_STONESKIN_HEROIC ) );
};

void StoneskinGargoyleAI::AIUpdate()
{
	bool HasAura = HasStoneskin();
	if ( IsCasting() || HasAura )
		return;
	else if ( _unit->GetUInt32Value( UNIT_NPC_EMOTESTATE ) == EMOTE_STATE_SUBMERGED )
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 0 );

	if ( !IsCasting() && GetHealthPercent() <= 30 )
	{
		CastSpellNowNoScheduling( mStoneskin );
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED );
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
		StopMovement();
		return;
	};

	ParentClass::AIUpdate();
};

void StoneskinGargoyleAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Frenzied Bat
FrenziedBatAI::FrenziedBatAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( FRENZIED_BAT_FRENZIED_DIVE, Target_Self, 10, 0, 15 );
};

void FrenziedBatAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plague Beast
PlagueBeastAI::PlagueBeastAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( PLAGUE_BEAST_PLAGUE_SPLASH_HEROIC, Target_RandomPlayerDestination, 8, 0, 15, 0, 50 );
	else
		AddSpell( PLAGUE_BEAST_PLAGUE_SPLASH_NORMAL, Target_RandomPlayerDestination, 8, 0, 15, 0, 50 );

	AddSpell( PLAGUE_BEAST_TRAMPLE, Target_Current, 10, 0, 10, 0, 8 );
	ApplyAura( PLAGUE_BEAST_MUTATED_SPORES );
};

void PlagueBeastAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	ApplyAura( PLAGUE_BEAST_MUTATED_SPORES );
};

void PlagueBeastAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	ApplyAura( PLAGUE_BEAST_MUTATED_SPORES );
};

void PlagueBeastAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Eye Stalker
EyeStalkerAI::EyeStalkerAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( EYE_STALKER_MIND_FLAY_HEROIC, Target_Current, 100, 6, 8, 0, 35 );
	else
		AddSpell( EYE_STALKER_MIND_FLAY_NORMAL, Target_Current, 100, 6, 8, 0, 45 );
};

void EyeStalkerAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void EyeStalkerAI::AIUpdate()
{
	Unit* CurrentTarget = _unit->GetAIInterface()->GetNextTarget();
	if ( !IsCasting() && CurrentTarget != NULL )
	{
		float MaxRange = 45.0f;
		if ( IsHeroic() )
			MaxRange = 35.0f;

		if ( GetRangeToUnit( CurrentTarget ) > MaxRange )
		{
			Unit* NewTarget = GetBestUnitTarget( TargetFilter_Closest );
			if ( NewTarget != NULL && GetRangeToUnit( NewTarget ) <= MaxRange )
			{
				_unit->GetAIInterface()->SetNextTarget( NewTarget );
				_unit->GetAIInterface()->AttackReaction( NewTarget, 200 );
			};

			_unit->GetAIInterface()->RemoveThreatByPtr( CurrentTarget );
		};
	};

	ParentClass::AIUpdate();

	// Meh, reset it in case something went wrong
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void EyeStalkerAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Noth the Plaguebringer
NothThePlaguebringerAI::NothThePlaguebringerAI( Creature* pCreature ) : AscentScriptBossAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddPhaseSpell( 1, AddSpell( NOTH_THE_PLAGUEBRINGER_CURSE_OF_THE_PLAGUE_HEROIC, Target_Self, 10, 0, 20 ) );
		mCriple = AddSpellFunc( &SpellFunc_NothCriple, Target_Self, 0, 0, 0 );
		mBlink = AddSpellFunc( &SpellFunc_NothBlink, Target_RandomPlayer, 0, 0, 0 );
	}
	else
	{
		AddPhaseSpell( 1, AddSpell( NOTH_THE_PLAGUEBRINGER_CURSE_OF_THE_PLAGUE_NORMAL, Target_Self, 10, 0, 20 ) );
		mCriple = NULL;
		mBlink = NULL;
	};

	mToBalconySwitch = AddSpellFunc( &SpellFunc_NothToBalconyPhaseSwitch, Target_Self, 0, 0, 0 );
	mFromBalconySwitch = AddSpellFunc( &SpellFunc_NothFromBalconyPhaseSwitch, Target_RandomPlayer, 0, 0, 0 );
	mBlinkTimer = mSkeletonTimer = mPhaseSwitchTimer = INVALIDATE_TIMER;
	mPhaseCounter = 0;

	if ( _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2740.689209f, -3489.697266f, 262.117767f, 181200 );
		if ( Gate != NULL )
			Gate->SetState( 0 );

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2683.670654f, -3556.429688f, 261.823334f, 181201 );
		if ( Gate != NULL )
			Gate->SetState( 0 );
	};
	
	AddEmote( Event_OnCombatStart, "Glory to the master!", Text_Yell, 8845 );
	AddEmote( Event_OnCombatStart, "Your life is forfeit.", Text_Yell, 8846 );
	AddEmote( Event_OnCombatStart, "Die, trespasser!", Text_Yell, 8847 );
	AddEmote( Event_OnDied, "I will serve the master... in death.", Text_Yell, 8848 );
	AddEmote( Event_OnTargetDied, "My task is done!", Text_Yell, 8849 );
	AddEmote( Event_OnTargetDied, "Breathe no more.", Text_Yell, 8850 );
};

void NothThePlaguebringerAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	if ( IsHeroic() )
		mBlinkTimer = AddTimer( 28000 + RandomUInt( 12 ) * 1000 );

	mPhaseSwitchTimer = AddTimer( 110000 );
	mSkeletonTimer = AddTimer( 8000 );
	mPhaseCounter = 0;

	if ( _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2740.689209f, -3489.697266f, 262.117767f, 181200 );
		if ( Gate != NULL )
			Gate->SetState( 1 );

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2683.670654f, -3556.429688f, 261.823334f, 181201 );
		if ( Gate != NULL )
			Gate->SetState( 1 );
	};
};

void NothThePlaguebringerAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2740.689209f, -3489.697266f, 262.117767f, 181200 );
		if ( Gate != NULL )
			Gate->SetState( 0 );

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2683.670654f, -3556.429688f, 261.823334f, 181201 );
		if ( Gate != NULL )
			Gate->SetState( 0 );
	};

	for ( set< PlaguedWarriorAI* >::iterator Iter = mWarriors.begin(); Iter != mWarriors.end(); ++Iter )
	{
		( *Iter )->mNothAI = NULL;
		( *Iter )->Despawn();
	};

	mWarriors.clear();

	for ( set< PlaguedChampionAI* >::iterator Iter = mChampions.begin(); Iter != mChampions.end(); ++Iter )
	{
		( *Iter )->mNothAI = NULL;
		( *Iter )->Despawn();
	};

	mChampions.clear();

	for ( set< PlaguedGuardianAI* >::iterator Iter = mGuardians.begin(); Iter != mGuardians.end(); ++Iter )
	{
		( *Iter )->mNothAI = NULL;
		( *Iter )->Despawn();
	};

	mGuardians.clear();
};

void NothThePlaguebringerAI::AIUpdate()
{
	if ( GetPhase() == 1 )
	{
		if ( !IsCasting() )
		{
			if ( mPhaseCounter < 3 && IsTimerFinished( mPhaseSwitchTimer ) )
			{
				SetPhase( 2, mToBalconySwitch );
				ResetTimer( mPhaseSwitchTimer, 70000 );
				ResetTimer( mSkeletonTimer, 0 );
				++mPhaseCounter;
				return;
			};

			if ( mPhaseCounter == 3 )
			{
				ApplyAura( NOTH_THE_PLAGUEBRINGER_BERSERK );
				++mPhaseCounter;
			};

			if ( IsHeroic() && IsTimerFinished( mBlinkTimer ) )
			{
				CastSpellNowNoScheduling( mCriple );
				ResetTimer( mBlinkTimer, 28000 + RandomUInt( 12 ) * 1000 );
			};
		};

		if ( IsTimerFinished( mSkeletonTimer ) )
		{
			uint32 SkelLimit = 2;
			if ( IsHeroic() )
				SkelLimit = 3;

			bool PosTaken[ 3 ];
			for ( uint32 i = 0; i < 3; ++i )
			{
				PosTaken[ i ] = false;
			};

			uint32 Id = 0;
			PlaguedWarriorAI* WarriorAI = NULL;
			for ( uint32 i = 0; i < SkelLimit; ++i )
			{
				Id = RandomUInt( 2 );
				if ( PosTaken[ Id ] )
				{
					for ( uint32 j = 0; j < 3; ++j )
					{
						if ( !PosTaken[ j ] )
						{
							Id = j;
							break;
						};
					};
				};

				WarriorAI = static_cast< PlaguedWarriorAI* >( SpawnCreature( CN_PLAGUED_WARRIOR, SkelPosPhase1[ Id ].mX, SkelPosPhase1[ Id ].mY, SkelPosPhase1[ Id ].mZ, SkelPosPhase1[ Id ].mO ) );
				if ( WarriorAI != NULL )
				{
					WarriorAI->GetUnit()->m_noRespawn = true;
					WarriorAI->AggroNearestPlayer( 200 );
					mWarriors.insert( WarriorAI );
				};

				PosTaken[ Id ] = true;
			};

			Emote( "Rise, my soldiers! Rise and fight once more!", Text_Yell, 8851 );
			ResetTimer( mSkeletonTimer, 30000 );
			PosTaken[ Id ] = true;
		};
	}
	else
	{
		if ( !IsCasting() && IsTimerFinished( mPhaseSwitchTimer ) )
		{
			SetPhase( 1, mFromBalconySwitch );
			ResetTimer( mPhaseSwitchTimer, 70000 );
			ResetTimer( mSkeletonTimer, 8000 );
			if ( IsHeroic() )
				ResetTimer( mBlinkTimer, 28000 + RandomUInt( 12 ) * 1000 );

			return;
		};

		if ( IsTimerFinished( mSkeletonTimer ) )
		{
			uint32 SpawnLimit = 2;
			if ( IsHeroic() )
				++SpawnLimit;

			uint32 Id = 0;
			uint32 Champions = 0;
			if ( mPhaseCounter <= SpawnLimit + 1 )
				Champions = SpawnLimit - mPhaseCounter + 1;

			bool PosTaken[ 4 ];
			for ( uint32 i = 0; i < 4; ++i )
			{
				PosTaken[ i ] = false;
			};

			PlaguedChampionAI* ChampionAI = NULL;
			for ( uint32 i = 0; i < Champions; ++i )
			{
				Id = RandomUInt( 3 );
				if ( PosTaken[ Id ] )
				{
					for ( uint32 j = 0; j < 4; ++j )
					{
						if ( !PosTaken[ Id ] )
						{
							Id = j;
							break;
						};
					};
				};

				ChampionAI = static_cast< PlaguedChampionAI* >( SpawnCreature( CN_PLAGUED_CHAMPION, SkelPosPhase2[ Id ].mX, SkelPosPhase2[ Id ].mY, SkelPosPhase2[ Id ].mZ, SkelPosPhase2[ Id ].mO ) );
				if ( ChampionAI != NULL )
				{
					ChampionAI->GetUnit()->m_noRespawn = true;
					ChampionAI->AggroNearestPlayer( 200 );
					mChampions.insert( ChampionAI );
				};

				PosTaken[ Id ] = true;
			};

			PlaguedGuardianAI* GuardianAI = NULL;
			for ( uint32 i = 0; i < SpawnLimit - Champions; ++i )
			{
				Id = RandomUInt( 3 );
				if ( PosTaken[ Id ] )
				{
					for ( uint32 j = 0; j < 4; ++j )
					{
						if ( !PosTaken[ Id ] )
						{
							Id = j;
							break;
						};
					};
				};

				GuardianAI = static_cast< PlaguedGuardianAI* >( SpawnCreature( CN_PLAGUED_GUARDIAN, SkelPosPhase2[ Id ].mX, SkelPosPhase2[ Id ].mY, SkelPosPhase2[ Id ].mZ, SkelPosPhase2[ Id ].mO ) );
				if ( GuardianAI != NULL )
				{
					GuardianAI->GetUnit()->m_noRespawn = true;
					GuardianAI->AggroNearestPlayer( 200 );
					mGuardians.insert( GuardianAI );
				};
			};

			ResetTimer( mSkeletonTimer, 35000 );
			PosTaken[ Id ] = true;
		};
	};

	ParentClass::AIUpdate();
	if ( GetPhase() == 2 )
	{
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
		StopMovement();
	};
};

void NothThePlaguebringerAI::Destroy()
{
	for ( set< PlaguedWarriorAI* >::iterator Iter = mWarriors.begin(); Iter != mWarriors.end(); ++Iter )
	{
		( *Iter )->mNothAI = NULL;
		( *Iter )->Despawn();
	};

	mWarriors.clear();

	for ( set< PlaguedChampionAI* >::iterator Iter = mChampions.begin(); Iter != mChampions.end(); ++Iter )
	{
		( *Iter )->mNothAI = NULL;
		( *Iter )->Despawn();
	};

	mChampions.clear();

	for ( set< PlaguedGuardianAI* >::iterator Iter = mGuardians.begin(); Iter != mGuardians.end(); ++Iter )
	{
		( *Iter )->mNothAI = NULL;
		( *Iter )->Despawn();
	};

	mGuardians.clear();

	delete this;
};

void SpellFunc_NothToBalconyPhaseSwitch( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	NothThePlaguebringerAI *Noth = ( pCreatureAI != NULL ) ? static_cast< NothThePlaguebringerAI* >( pCreatureAI ) : NULL;
	if ( Noth != NULL )
	{
		// Are these coords correct ? Or maybe it should be just disappear / appear thing ? And is this spell correct ? I doubt it ...
		Noth->ApplyAura( NOTH_THE_PLAGUEBRINGER_BLINK_HEROIC );
		Noth->GetUnit()->SetPosition( 2631.051025f, -3529.595703f, 274.037811f, 0.109163f );
		Noth->SetBehavior( Behavior_Spell );
		Noth->SetCanMove( false );
		Noth->StopMovement();
	};
};

void SpellFunc_NothFromBalconyPhaseSwitch( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	NothThePlaguebringerAI *Noth = ( pCreatureAI != NULL ) ? static_cast< NothThePlaguebringerAI* >( pCreatureAI ) : NULL;
	if ( Noth != NULL )
	{
		Noth->SetBehavior( Behavior_Default );
		Noth->SetCanMove( true );
		Noth->ApplyAura( NOTH_THE_PLAGUEBRINGER_BLINK_HEROIC );
		Noth->GetUnit()->SetPosition( 2684.620850f, -3502.447266f, 261.314880f, 0.098174f );

		if ( pTarget != NULL )
			Noth->GetUnit()->GetAIInterface()->AttackReaction( pTarget, 200 );

		Noth->GetUnit()->GetAIInterface()->SetNextTarget( pTarget );
	};
};

void SpellFunc_NothCriple( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	NothThePlaguebringerAI *Noth = ( pCreatureAI != NULL ) ? static_cast< NothThePlaguebringerAI* >( pCreatureAI ) : NULL;
	if ( Noth != NULL )
	{
		// Dunno if target count that should be affected is correct
		Noth->ApplyAura( NOTH_THE_PLAGUEBRINGER_CRIPLE_HEROIC );
		Noth->CastSpell( Noth->mBlink );
	};
};

void SpellFunc_NothBlink( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	NothThePlaguebringerAI *Noth = ( pCreatureAI != NULL ) ? static_cast< NothThePlaguebringerAI* >( pCreatureAI ) : NULL;
	if ( Noth != NULL )
	{
		Noth->ApplyAura( NOTH_THE_PLAGUEBRINGER_BLINK_HEROIC );
		float Angle = Noth->GetUnit()->GetOrientation();
		float NewX = Noth->GetUnit()->GetPositionX() + 20.0f * cosf( Angle );
		float NewY = Noth->GetUnit()->GetPositionY() + 20.0f * sinf( Angle );
		Noth->GetUnit()->SetPosition( NewX, NewY, Noth->GetUnit()->GetPositionZ(), Angle );
		Noth->ClearHateList();
		if ( pTarget != NULL )
			Noth->GetUnit()->GetAIInterface()->AttackReaction( pTarget, 500 );

		Noth->GetUnit()->GetAIInterface()->SetNextTarget( pTarget );
	};
};

/////////////////////////////////////////////////////////////////////////////////
////// Plagued Warrior
PlaguedWarriorAI::PlaguedWarriorAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( PLAGUED_WARRIOR_STRIKE, Target_Current, 10, 0, 5, 0, 8 );
	AddSpell( PLAGUED_WARRIOR_CLEAVE, Target_Current, 10, 0, 10, 0, 8 );
};

void PlaguedWarriorAI::Destroy()
{
	if ( mNothAI != NULL )
	{
		set< PlaguedWarriorAI* >::iterator Iter = mNothAI->mWarriors.find( this );
		if ( Iter != mNothAI->mWarriors.end() )
			mNothAI->mWarriors.erase( Iter );
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plagued Champion
PlaguedChampionAI::PlaguedChampionAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( PLAGUED_CHAMPION_MORTAL_STRIKE_HEROIC, Target_Current, 8, 0, 10, 0, 8 );
		AddSpell( PLAGUED_CHAMPION_SHADOW_SHOCK_HEROIC, Target_Self, 10, 0, 10 );
	}
	else
	{
		AddSpell( PLAGUED_CHAMPION_MORTAL_STRIKE_NORMAL, Target_Current, 8, 0, 10, 0, 8 );
		AddSpell( PLAGUED_CHAMPION_SHADOW_SHOCK_NORMAL, Target_Self, 10, 0, 10 );
	};
};

void PlaguedChampionAI::Destroy()
{
	if ( mNothAI != NULL )
	{
		set< PlaguedChampionAI* >::iterator Iter = mNothAI->mChampions.find( this );
		if ( Iter != mNothAI->mChampions.end() )
			mNothAI->mChampions.erase( Iter );
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plagued Guardian
PlaguedGuardianAI::PlaguedGuardianAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( PLAGUED_GUARDIAN_ARCANE_EXPLOSION_HEROIC, Target_Self, 10, 1.5, 10 );
	else
		AddSpell( PLAGUED_GUARDIAN_ARCANE_EXPLOSION_NORMAL, Target_Self, 10, 1.5, 10 );
};

void PlaguedGuardianAI::Destroy()
{
	if ( mNothAI != NULL )
	{
		set< PlaguedGuardianAI* >::iterator Iter = mNothAI->mGuardians.find( this );
		if ( Iter != mNothAI->mGuardians.end() )
			mNothAI->mGuardians.erase( Iter );
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Heigan the Unclean
HeiganTheUncleanAI::HeiganTheUncleanAI( Creature* pCreature ) : AscentScriptBossAI( pCreature )
{
	AddPhaseSpell( 1, AddSpell( HEIGAN_THE_UNCLEAN_SPELL_DISRUPTION, Target_Self, 100, 0, 5 ) );
	if ( IsHeroic() )
		AddPhaseSpell( 1, AddSpell( HEIGAN_THE_UNCLEAN_DECREPIT_FEVER_HEROIC, Target_Self, 100, 0, 20 ) );
	else
		AddPhaseSpell( 1, AddSpell( HEIGAN_THE_UNCLEAN_DECREPIT_FEVER_NORMAL, Target_Self, 100, 0, 20 ) );

	AddPhaseSpell( 2, AddSpell( HEIGAN_THE_UNCLEAN_PLAGUE_CLOUD_DAMAGE, Target_Self, 100, 0, 2 ) );

	AddEmote( Event_OnCombatStart, "You are mine now!", Text_Yell, 8825 );
	AddEmote( Event_OnCombatStart, "I see you...", Text_Yell, 8826 );
	AddEmote( Event_OnCombatStart, "You... are next!", Text_Yell, 8827 );
	AddEmote( Event_OnDied, "Arghhaahhhhh!", Text_Yell, 8828 ); 
	AddEmote( Event_OnTargetDied, "Close your eyes. Sleep.", Text_Yell, 8829 );
	AddEmote( Event_OnTaunt, "The races of the world will perish... it is only a matter of time.", Text_Yell, 8830 );
	AddEmote( Event_OnTaunt, "I see endless suffering... I see torment... I see rage... I see everything.", Text_Yell, 8831 );
	AddEmote( Event_OnTaunt, "Soon... the world will tremble.", Text_Yell, 8832 );
	AddEmote( Event_OnTaunt, "Hungry worms will feast on your rotting flesh.", Text_Yell, 8834 );
	mPhaseSwitchTimer = mEruptionTimer = INVALIDATE_TIMER;
	mEruptionPhase = 0;
	mClockWiseEruption = true;

	if ( _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2790.709961f, -3708.669922f, 276.584991f, 181202 );
		if ( Gate != NULL )
			Gate->SetState( 0 );

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2771.718506f, -3739.965820f, 273.616211f, 181203 );
		if ( Gate != NULL )
			Gate->SetState( 0 );
	};
};

uint32 HeiganTheUncleanAI::CalculateTriggerArea( float pPosX, float pPosY )
{
	if ( pPosY < -3.0f * pPosX + 4679.5f )
		return 0;
	else if ( pPosY < -1.5f * pPosX + 474.65f )
		return 1;
	else if ( pPosY > -0.5f * pPosX - 2319.5f )
		return 2;

	return 3;
};

void HeiganTheUncleanAI::CallEruptionEvent( int32 pTimerId, int32 pNewTime )
{
	if ( pTimerId < 0 )
		return;

	for ( set< pair< uint32, PlagueFissureGO* > >::iterator Iter = mFissures.begin(); Iter != mFissures.end(); ++Iter )
	{
		if ( ( *Iter ).first == mEruptionPhase )
			continue;

		( *Iter ).second->SetState( 0 );
	};

	for ( set< pair< uint32, FissureTriggerAI* > >::iterator Iter = mFissureTriggers.begin(); Iter != mFissureTriggers.end(); ++Iter )
	{
		if ( ( *Iter ).first == mEruptionPhase )
			continue;

		( *Iter ).second->GetUnit()->CastSpell( ( *Iter ).second->GetUnit(), FISSURE_TRIGGER_ERUPTION, true );
	};

	ResetTimer( pTimerId, pNewTime );
	if ( mEruptionPhase == 3 )
		mClockWiseEruption = false;
	else if ( mEruptionPhase == 0 )
		mClockWiseEruption = true;

	if ( mClockWiseEruption )
		++mEruptionPhase;
	else
		--mEruptionPhase;
};

void HeiganTheUncleanAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mPhaseSwitchTimer = AddTimer( 90000 );
	mEruptionTimer = AddTimer( 8000 );
	mEruptionPhase = 0;
	mClockWiseEruption = true;

	/*if ( _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2790.709961f, -3708.669922f, 276.584991f, 181202 );
		if ( Gate != NULL )
			Gate->SetState( 1 );

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2771.718506f, -3739.965820f, 273.616211f, 181203 );
		if ( Gate != NULL )
			Gate->SetState( 1 );

		if ( mFissures.size() == 0 )
		{
			GameObject* Fissure = NULL;
			PlagueFissureGO* FissureGO = NULL;
			FissureTriggerAI* TriggerAI = NULL;
			for ( unordered_set< Object* >::iterator Iter = _unit->GetInRangeSetBegin(); Iter != _unit->GetInRangeSetEnd(); ++Iter )
			{
				if ( ( *Iter ) == NULL || ( *Iter )->GetTypeId() != TYPEID_GAMEOBJECT )
					continue;

				Fissure = TO_GAMEOBJECT( *Iter );
				if ( Fissure->GetInfo() == NULL )
					continue;

				if ( Fissure->GetUInt32Value( GAMEOBJECT_DISPLAYID ) != 6785 && Fissure->GetUInt32Value( GAMEOBJECT_DISPLAYID ) != 1287 )
					continue;

				if ( Fissure->GetScript() == NULL )
					continue;

				uint32 AreaId = CalculateTriggerArea( Fissure->GetPositionX(), Fissure->GetPositionY() );
				TriggerAI = static_cast< FissureTriggerAI* >( SpawnCreature( CN_FISSURE_TRIGGER, Fissure->GetPositionX(), Fissure->GetPositionY(), Fissure->GetPositionZ(), Fissure->GetOrientation(), true ) );
				if ( TriggerAI != NULL )
				{
					TriggerAI->GetUnit()->m_noRespawn = true;
					TriggerAI->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
					TriggerAI->SetCanEnterCombat( false );
					TriggerAI->mHeiganAI = this;
					mFissureTriggers.insert( make_pair( AreaId, TriggerAI ) );
				};

				FissureGO = static_cast< PlagueFissureGO* >( Fissure->GetScript() );
				mFissures.insert( make_pair( AreaId, FissureGO ) );
				FissureGO->mHeiganAI = this;
				FissureGO->SetState( 1 );
			};
		};
	};*/
};

void HeiganTheUncleanAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	SetTargetToChannel( NULL, 0 );
	if ( _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		GameObject* Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2790.709961f, -3708.669922f, 276.584991f, 181202 );
		if ( Gate != NULL )
			Gate->SetState( 0 );

		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords( 2771.718506f, -3739.965820f, 273.616211f, 181203 );
		if ( Gate != NULL )
			Gate->SetState( 0 );
	};

	for ( set< pair< uint32, FissureTriggerAI* > >::iterator Iter = mFissureTriggers.begin(); Iter != mFissureTriggers.end(); ++Iter )
	{
		( *Iter ).second->mHeiganAI = NULL;
		( *Iter ).second->Despawn();
	};

	mFissureTriggers.clear();
	mFissures.clear();
};

void HeiganTheUncleanAI::AIUpdate()
{
	if ( GetPhase() == 1 )
	{
		// Disabled until eruption is fixed - somehow.
		/*if ( !IsCasting() && IsTimerFinished( mPhaseSwitchTimer ) )
		{
			ApplyAura( HEIGAN_THE_UNCLEAN_TELEPORT );
			Emote( "The end is uppon you!", Text_Yell, 8833 );
			_unit->SetPosition( 2794.235596f, -3707.067627f, 276.545746f, 2.407245f );
			SetTargetToChannel( _unit, HEIGAN_THE_UNCLEAN_PLAGUE_CLOUD_CHANNEL );
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
			StopMovement();
			SetPhase( 2 );
			ResetTimer( mPhaseSwitchTimer, 45000 );
			ResetTimer( mEruptionTimer, 3000 );
			return;
		};

		if ( IsTimerFinished( mEruptionTimer ) )
			CallEruptionEvent( mEruptionTimer, 8000 );*/
	}
	else
	{
		if ( !IsCasting() && IsTimerFinished( mPhaseSwitchTimer ) )
		{
			SetTargetToChannel( NULL, 0 );
			SetBehavior( Behavior_Default );
			SetCanMove( true );
			SetPhase( 1 );
			ResetTimer( mPhaseSwitchTimer, 90000 );
			return;
		};

		if ( IsTimerFinished( mEruptionTimer ) )
			CallEruptionEvent( mEruptionTimer, 3000 );
	};

	ParentClass::AIUpdate();
	if ( GetPhase() == 2 )
	{
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
		StopMovement();
	};
};

void HeiganTheUncleanAI::Destroy()
{
	for ( set< pair< uint32, FissureTriggerAI* > >::iterator Iter = mFissureTriggers.begin(); Iter != mFissureTriggers.end(); ++Iter )
	{
		( *Iter ).second->mHeiganAI = NULL;
		( *Iter ).second->Despawn();
	};

	mFissureTriggers.clear();
	mFissures.clear();

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Plague Fissure
GameObjectAIScript* PlagueFissureGO::Create( GameObject* pGameObject )
{
	return new PlagueFissureGO( pGameObject );
};

PlagueFissureGO::PlagueFissureGO( GameObject* pGameObject ) : GameObjectAIScript( pGameObject )
{
	mHeiganAI = NULL;
};

void PlagueFissureGO::SetState( uint32 pState )
{
	_gameobject->SetState( pState );
};

void PlagueFissureGO::Destroy()
{
	if ( mHeiganAI != NULL )
	{
		for ( set< pair< uint32, PlagueFissureGO* > >::iterator Iter = mHeiganAI->mFissures.begin(); Iter != mHeiganAI->mFissures.end(); ++Iter )
		{
			if ( ( *Iter ).second == this )
			{
				mHeiganAI->mFissures.erase( Iter );
				break;
			};
		};			

		mHeiganAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Fissure Trigger
FissureTriggerAI::FissureTriggerAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	mHeiganAI = NULL;
};

void FissureTriggerAI::Destroy()
{
	if ( mHeiganAI != NULL )
	{
		for ( set< pair< uint32, FissureTriggerAI* > >::iterator Iter = mHeiganAI->mFissureTriggers.begin(); Iter != mHeiganAI->mFissureTriggers.end(); ++Iter )
		{
			if ( ( *Iter ).second == this )
			{
				mHeiganAI->mFissureTriggers.erase( Iter );
				break;
			};
		};			

		mHeiganAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Loatheb
LoathebAI::LoathebAI( Creature* pCreature ) : AscentScriptBossAI( pCreature )
{
	AddSpell( LOATHEB_NECROTIC_AURA, Target_Self, 100, 0, 20 );
	if ( IsHeroic() )
		AddSpell( LOATHEB_DEATHBLOOM_HEROIC, Target_Self, 100, 0, 30 );
	else
		AddSpell( LOATHEB_DEATHBLOOM_NORMAL, Target_Self, 100, 0, 30 );

	mSporeTimer = mDoomTimer = mDeathbloomTimer = INVALIDATE_TIMER;
	mDoomStaticTimer = 120000;
	mDeathbloomDamagePhase = false;
};
	
void LoathebAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mDoomStaticTimer = 120000;
	mSporeTimer = AddTimer( 30000 );
	mDoomTimer = AddTimer( mDoomStaticTimer );
	mDeathbloomTimer = AddTimer( 30000 );
	mDeathbloomDamagePhase = false;
};

void LoathebAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	for ( set< SporeAI* >::iterator Iter = mSpores.begin(); Iter != mSpores.end(); ++Iter )
	{
		( *Iter )->mLoathebAI = NULL;
		( *Iter )->Despawn();
	};

	mSpores.clear();
};
	
void LoathebAI::AIUpdate()
{
	if ( IsTimerFinished( mSporeTimer ) )
	{
		bool PosTaken[ 4 ];
		for ( uint32 i = 0; i < 4; ++i )
		{
			PosTaken[ i ] = false;
		};

		uint32 Id = 0;
		SporeAI* Spore = NULL;
		for ( uint32 i = 0; i < 3; ++i )
		{
			Id = RandomUInt( 3 );
			if ( PosTaken[ Id ] )
			{
				for ( uint32 j = 0; j < 4; ++j )
				{
					if ( !PosTaken[ j ] )
					{
						Id = j;
						break;
					};
				};
			};

			Spore = static_cast< SporeAI* >( SpawnCreature( CN_SPORE, Spores[ Id ].mX, Spores[ Id ].mY, Spores[ Id ].mZ, Spores[ Id ].mO, true ) );
			if ( Spore != NULL )
			{
				Spore->GetUnit()->m_noRespawn = true;
				Spore->AggroRandomPlayer( 200 );
				mSpores.insert( Spore );
			};
		};

		ResetTimer( mSporeTimer, 30000 );
		PosTaken[ Id ] = true;
	}

	if ( !IsCasting() )
	{
		if ( IsTimerFinished( mDoomTimer ) )
		{
			if ( IsHeroic() )
				ApplyAura( LOATHEB_INEVITABLE_DOOM_HEROIC );
			else
				ApplyAura( LOATHEB_INEVITABLE_DOOM_NORMAL );

			if ( mDoomStaticTimer > 36000 )
			{
				if ( mDoomStaticTimer <= 36000 )
					mDoomStaticTimer = 15000;
				else
					mDoomStaticTimer -= 21000;
			};

			ResetTimer( mDoomTimer, mDoomStaticTimer );
		}
		else if ( IsTimerFinished( mDeathbloomTimer ) )
		{
			if ( mDeathbloomDamagePhase )
			{
				Player* PlayerPtr = NULL;
				for ( unordered_set< Player* >::iterator Iter = _unit->GetInRangePlayerSetBegin(); Iter != _unit->GetInRangePlayerSetEnd(); ++Iter )
				{
					if ( ( *Iter ) == NULL )
						continue;

					PlayerPtr = *Iter;
					if ( !PlayerPtr->isAlive() )
						continue;

					if ( !PlayerPtr->HasAura( LOATHEB_DEATHBLOOM_NORMAL ) && !PlayerPtr->HasAura( LOATHEB_DEATHBLOOM_HEROIC ) )
						continue;

					if ( IsHeroic() )
						PlayerPtr->CastSpell( PlayerPtr, LOATHEB_DEATHBLOOM_DAMAGE_HEROIC, true );
					else
						PlayerPtr->CastSpell( PlayerPtr, LOATHEB_DEATHBLOOM_DAMAGE_NORMAL, true );
				};

				ResetTimer( mDeathbloomTimer, 25000 );
			}
			else
			{
				if ( IsHeroic() )
					ApplyAura( LOATHEB_DEATHBLOOM_HEROIC );
				else
					ApplyAura( LOATHEB_DEATHBLOOM_NORMAL );

				ResetTimer( mDeathbloomTimer, 5000 );
			};

			mDeathbloomDamagePhase = !mDeathbloomDamagePhase;
		};
	};

	ParentClass::AIUpdate();
};

void LoathebAI::Destroy()
{
	for ( set< SporeAI* >::iterator Iter = mSpores.begin(); Iter != mSpores.end(); ++Iter )
	{
		( *Iter )->mLoathebAI = NULL;
		( *Iter )->Despawn();
	};

	mSpores.clear();

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Spore
SporeAI::SporeAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	Despawn( 90000 );

	mLoathebAI = NULL;
};

void SporeAI::OnDied( Unit* pKiller )
{
	ParentClass::OnDied( pKiller );
	ApplyAura( SPORE_FUNGAL_CREEP );
};

void SporeAI::Destroy()
{
	if ( mLoathebAI != NULL )
	{
		set< SporeAI* >::iterator Iter = mLoathebAI->mSpores.find( this );
		if ( Iter != mLoathebAI->mSpores.end() )
			mLoathebAI->mSpores.erase( Iter );

		mLoathebAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// The Military Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight
DeathKnightAI::DeathKnightAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( DEATH_KNIGHT_DEATH_COIL_HEROIC, Target_RandomPlayer, 9, 0, 15, 0, 45 );
	else
		AddSpell( DEATH_KNIGHT_DEATH_COIL_NORMAL, Target_RandomPlayer, 9, 0, 15, 0, 45 );

	AddSpell( DEATH_KNIGHT_DEATH_COIL_HEAL, Target_WoundedFriendly, 6, 0, 15, 0, 45 );
	AddSpell( DEATH_KNIGHT_HYSTERIA, Target_RandomFriendly, 7, 0, 30, 0, 45 );
};

void DeathKnightAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	ApplyAura( DEATH_KNIGHT_BLOOD_PRESENCE );
};

void DeathKnightAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight Captain
DeathKnightCaptainAI::DeathKnightCaptainAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( DEATH_KNIGHT_CAPTAIN_PLAGUE_STRIKE_HEROIC, Target_Current, 8, 0, 20, 0, 8 );
	else
		AddSpell( DEATH_KNIGHT_CAPTAIN_PLAGUE_STRIKE_NORMAL, Target_Current, 8, 0, 20, 0, 8 );

	AddSpell( DEATH_KNIGHT_CAPTAIN_RAISE_DEAD, Target_RandomPlayerDestination, 6, 1.5, 20 );
	AddSpell( DEATH_KNIGHT_CAPTAIN_WHIRLWIND, Target_Self, 10, 0, 10 );
};

void DeathKnightCaptainAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	ApplyAura( DEATH_KNIGHT_CAPTAIN_UNHOLY_PRESENCE );
};

void DeathKnightCaptainAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Skeletal Construct - wiki says he's in Naxx, but WoWHead claims him to be in Icecrown only

/////////////////////////////////////////////////////////////////////////////////
////// Ghost of Naxxramas
GhostOfNaxxramasAI::GhostOfNaxxramasAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
};

void GhostOfNaxxramasAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Shade of Naxxramas
ShadeOfNaxxramasAI::ShadeOfNaxxramasAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( SHADE_OF_NAXXRAMAS_SHADOW_BOLT_VOLLEY_HEROIC, Target_Self, 10, 0, 10 );
	else
		AddSpell( SHADE_OF_NAXXRAMAS_SHADOW_BOLT_VOLLEY_NORMAL, Target_Self, 10, 0, 10 );

	AddSpell( SHADE_OF_NAXXRAMAS_PORTAL_OF_SHADOWS, Target_Self, 8, 0, 60 );
};

void ShadeOfNaxxramasAI::OnDied( Unit* pKiller )
{
	AscentScriptCreatureAI* Ghost = SpawnCreature( CN_GHOST_OF_NAXXRAMAS, true );
	if ( Ghost != NULL )
	{
		Ghost->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
		Ghost->SetDespawnWhenInactive( true );
		Ghost->AggroNearestPlayer( 200 );
	};

	for ( set< PortalOfShadowsAI* >::iterator Iter = mPortals.begin(); Iter != mPortals.end(); ++Iter )
	{
		( *Iter )->mShadeAI = NULL;
		( *Iter )->Despawn();
	};

	mPortals.clear();
};

void ShadeOfNaxxramasAI::Destroy()
{
	for ( set< PortalOfShadowsAI* >::iterator Iter = mPortals.begin(); Iter != mPortals.end(); ++Iter )
	{
		( *Iter )->mShadeAI = NULL;
		( *Iter )->Despawn();
	};

	mPortals.clear();

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Portal of Shadows - timer value is a wild guess
PortalOfShadowsAI::PortalOfShadowsAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	RegisterAIUpdateEvent( 1000 );
	mSpawnTimer = AddTimer( 15000 );
	mShadeAI = NULL;

	// We do not consider using a spell that summons these portals by anyone else than Shade of Naxxramas.
	// I must figure out why it's often not added if only one Shade is on the battlefield.
	// I don't like this method anyway.
	if ( _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY ) != 0 && _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		//mShadeAI = static_cast< ShadeOfNaxxramasAI* >( GetNearestCreature( CN_SHADE_OF_NAXXRAMAS ) );
		Unit* UnitPtr = ForceCreatureFind( CN_SHADE_OF_NAXXRAMAS );
		if ( UnitPtr != NULL )
		{
			mShadeAI = static_cast< ShadeOfNaxxramasAI* >( TO_CREATURE( UnitPtr )->GetScript() );
			if ( mShadeAI != NULL )
				mShadeAI->mPortals.insert( this );
		};
	};
};

void PortalOfShadowsAI::OnCombatStart( Unit* pTarget )
{
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void PortalOfShadowsAI::OnCombatStop( Unit* pTarget )
{
	CancelAllSpells();
	CancelAllTimers();
	SetCanMove( true );
	SetBehavior( Behavior_Default );
};

void PortalOfShadowsAI::AIUpdate()
{
	if ( mShadeAI != NULL && mShadeAI->GetUnit()->GetAIInterface()->GetNextTarget() != NULL )
	{
		if ( IsTimerFinished( mSpawnTimer ) )
		{
			AscentScriptCreatureAI* Ghost = SpawnCreature( CN_GHOST_OF_NAXXRAMAS, true );
			if ( Ghost != NULL )
			{
				Ghost->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 );
				Ghost->SetDespawnWhenInactive( true );
				Ghost->AggroNearestPlayer( 200 );
			};

			ResetTimer( mSpawnTimer, 15000 );
		};
	}
	else
	{
		RemoveAIUpdateEvent();
		Despawn();
		return;
	};

	ParentClass::AIUpdate();
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void PortalOfShadowsAI::Destroy()
{
	if ( mShadeAI != NULL )
	{
		set< PortalOfShadowsAI* >::iterator Iter = mShadeAI->mPortals.find( this );
		if ( Iter != mShadeAI->mPortals.end() )
			mShadeAI->mPortals.erase( Iter );

		mShadeAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Necro Knight
NecroKnightAI::NecroKnightAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( NECRO_KNIGHT_ARCANE_EXPLOSION, Target_Self, 8, 1.5, 5 );
	AddSpell( NECRO_KNIGHT_BLAST_WAVE, Target_Self, 7, 0, 5 );
	AddSpell( NECRO_KNIGHT_CONE_OF_COLD, Target_Current, 7, 0, 5, 0, 10 );
	AddSpell( NECRO_KNIGHT_FLAMESTRIKE, Target_RandomPlayerDestination, 8, 2, 10, 0, 30 );
	AddSpell( NECRO_KNIGHT_FROST_NOVA, Target_Self, 8, 0, 10 );
	AddSpellFunc( &SpellFunc_NecroKnightBlink, Target_RandomPlayerNotCurrent, 5, 0, 20, 0, 30 );
};

void NecroKnightAI::Destroy()
{
	delete this;
};

void SpellFunc_NecroKnightBlink( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	NecroKnightAI *NecroKnight = ( pCreatureAI != NULL ) ? static_cast< NecroKnightAI* >( pCreatureAI ) : NULL;
	if ( NecroKnight != NULL && pTarget != NULL )
	{
		NecroKnight->ApplyAura( NECRO_KNIGHT_BLINK );
		NecroKnight->GetUnit()->SetPosition( pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), NecroKnight->GetUnit()->GetOrientation() );
		NecroKnight->GetUnit()->GetAIInterface()->AttackReaction( pTarget, 500 );
		NecroKnight->GetUnit()->GetAIInterface()->SetNextTarget( pTarget );
	};
};

/////////////////////////////////////////////////////////////////////////////////
////// Skeletal Smith
SkeletalSmithAI::SkeletalSmithAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( SKELETAL_SMITH_CRUSH_ARMOR, Target_Current, 10, 0, 10, 0, 8);
	AddSpell( SKELETAL_SMITH_DISARM, Target_Current, 10, 0, 15, 0, 8 );
	AddSpell( SKELETAL_SMITH_THUNDERCLAP, Target_Self, 8, 0, 15 );
	//AddSpell( SKELETAL_SMITH_SUDDER_ARMOR, Target_Current, 8, 0, 10, 0, 8 );
};

void SkeletalSmithAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Death Knight Cavalier
DeathKnightCavalierAI::DeathKnightCavalierAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( DEATH_KNIGHT_CAVALIER_BONE_ARMOR_HEROIC, Target_Self, 7, 0, 30 );
		AddSpell( DEATH_KNIGHT_CAVALIER_ICY_TOUCH_HEROIC, Target_RandomPlayer, 10, 0, 15, 0, 20 );	// Target_Current ?
		AddSpell( DEATH_KNIGHT_CAVALIER_STRANGULATE_HEROIC, Target_RandomPlayer, 7, 1, 15, 0, 50 );
	}
	else
	{
		AddSpell( DEATH_KNIGHT_CAVALIER_BONE_ARMOR_NORMAL, Target_Self, 7, 0, 30 );
		AddSpell( DEATH_KNIGHT_CAVALIER_ICY_TOUCH_NORMAL, Target_RandomPlayer, 10, 0, 15, 0, 20 );	// Target_Current ?
		AddSpell( DEATH_KNIGHT_CAVALIER_STRANGULATE_NORMAL, Target_RandomPlayer, 7, 1, 15, 0, 50 );
	};

	AddSpell( DEATH_KNIGHT_CAVALIER_AURA_OF_AGONY, Target_RandomPlayer, 10, 0, 5, 0, 30 );
	AddSpell( DEATH_KNIGHT_CAVALIER_CLEAVE, Target_Current, 10, 0, 10, 0, 8 );
	AddSpell( DEATH_KNIGHT_CAVALIER_DEATH_COIL, Target_RandomPlayer, 7, 0, 10, 0, 30 );
	_unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 25278 );
	mChargerAI = NULL;
	mIsMounted = true;
};

void DeathKnightCavalierAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( mChargerAI != NULL )
	{
		if ( IsAlive() && _unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 )
			_unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 25278 );

		mChargerAI->mDeathKnightAI = NULL;
		mChargerAI->Despawn();
		mChargerAI = NULL;
	};

	mIsMounted = true;
};

void DeathKnightCavalierAI::AIUpdate()
{
	if ( mIsMounted && _unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 )
		_unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 25278 );
	if ( mIsMounted && RandomUInt( 99 ) < 2 )
	{
		_unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0 );
		ApplyAura( DEATH_KNIGHT_CAVALIER_DISMOUNT_DEATHCHARGER );
		mIsMounted = false;
	};

	ParentClass::AIUpdate();
};

void DeathKnightCavalierAI::Destroy()
{
	if ( mChargerAI != NULL )
	{
		mChargerAI->mDeathKnightAI = NULL;
		mChargerAI->Despawn();
		mChargerAI = NULL;
	};

	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Deathcharger Steed
DeathchargerSteedAI::DeathchargerSteedAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	mDeathKnightAI = NULL;
	mCharge = AddSpell( DEATHCHARGER_STEED_CHARGE, Target_Current, 0, 0, 0, 5, 45 );
	//AddSpellFunc( &SpellFunc_DeathchargerSteedCharge, Target_RandomPlayer, 8, 0, 15, 5, 45 );

	// We do not consider using a spell that summons this unit by anyone else than Death Knight Cavalier.
	// I don't like this method anyway.
	if ( _unit->GetUInt64Value( UNIT_FIELD_SUMMONEDBY ) != 0 && _unit->GetMapMgr() != NULL && _unit->GetMapMgr()->GetInterface() != NULL )
	{
		mDeathKnightAI = static_cast< DeathKnightCavalierAI* >( GetNearestCreature( CN_DEATH_KNIGHT_CAVALIER ) );
		if ( mDeathKnightAI != NULL && mDeathKnightAI->mChargerAI == NULL )
			mDeathKnightAI->mChargerAI = this;
	};
};

void DeathchargerSteedAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( mDeathKnightAI != NULL )
	{
		if ( mDeathKnightAI->IsInCombat() )
			AggroNearestUnit( 200 );
		else
		{
			mDeathKnightAI->mChargerAI = NULL;
			mDeathKnightAI = NULL;
			Despawn();
		};
	};
};

void DeathchargerSteedAI::Destroy()
{
	if ( mDeathKnightAI != NULL )
	{
		mDeathKnightAI->mChargerAI = NULL;
		mDeathKnightAI = NULL;
	};

	delete this;
};

void SpellFunc_DeathchargerSteedCharge( SpellDesc* pThis, AscentScriptCreatureAI* pCreatureAI, Unit* pTarget, TargetType pType )
{
	DeathchargerSteedAI *Deathcharger = ( pCreatureAI != NULL ) ? static_cast< DeathchargerSteedAI* >( pCreatureAI ) : NULL;
	if ( Deathcharger != NULL )
	{
		Unit* CurrentTarget = Deathcharger->GetUnit()->GetAIInterface()->GetNextTarget();
		if ( CurrentTarget != NULL && CurrentTarget != pTarget )
		{
			Deathcharger->GetUnit()->GetAIInterface()->AttackReaction( pTarget, 500 );
			Deathcharger->GetUnit()->GetAIInterface()->SetNextTarget( pTarget );
			//Deathcharger->GetUnit()->GetAIInterface()->RemoveThreatByPtr( CurrentTarget );
		};

		Deathcharger->CastSpellNowNoScheduling( Deathcharger->mCharge );
	};
};

/////////////////////////////////////////////////////////////////////////////////
////// Dark Touched Warrior
DarkTouchedWarriorAI::DarkTouchedWarriorAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( DARK_TOUCHED_WARRIOR_WHIRLWIND, Target_Self, 10, 0, 15 );

	mResetHateTimer = INVALIDATE_TIMER;
};

void DarkTouchedWarriorAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mResetHateTimer = AddTimer( 8000 + RandomUInt( 7 ) * 1000 );
};

void DarkTouchedWarriorAI::AIUpdate()
{
	if ( !IsCasting() && IsTimerFinished( mResetHateTimer ) )
	{
		ClearHateList();
		mResetHateTimer = AddTimer( 8000 + RandomUInt( 7 ) * 1000 );
	};

	ParentClass::AIUpdate();
};

void DarkTouchedWarriorAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Risen Squire
RisenSquireAI::RisenSquireAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( RISEN_SQUIRE_PIERCE_ARMOR, Target_Current, 10, 0, 15, 0, 8 );
};

void RisenSquireAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Unholy Axe
UnholyAxeAI::UnholyAxeAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( UNHOLY_AXE_MORTAL_STRIKE_HEROIC, Target_Current, 10, 0, 10, 0, 8 );
		AddSpell( UNHOLY_AXE_WHIRLWIND_HEROIC, Target_Self, 8, 2, 15 );
	}
	else
	{
		AddSpell( UNHOLY_AXE_MORTAL_STRIKE_NORMAL, Target_Current, 10, 0, 10, 0, 8 );
		AddSpell( UNHOLY_AXE_WHIRLWIND_NORMAL, Target_Self, 8, 2, 15 );
	};
};

void UnholyAxeAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Unholy Sword
UnholySwordAI::UnholySwordAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( UNHOLY_SWORD_CLEAVE_HEROIC, Target_Current, 10, 0, 15, 0, 8 );
	else
		AddSpell( UNHOLY_SWORD_CLEAVE_NORMAL, Target_Current, 10, 0, 15, 0, 8 );
};

void UnholySwordAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Unholy Staff
UnholyStaffAI::UnholyStaffAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( UNHOLY_STAFF_ARCANE_EXPLOSION_HEROIC, Target_Self, 8, 0.5, 15 );
	else
		AddSpell( UNHOLY_STAFF_ARCANE_EXPLOSION_NORMAL, Target_Self, 8, 0.5, 15 );

	AddSpell( UNHOLY_STAFF_FROST_NOVA, Target_Self, 10, 0, 15 );
	AddSpell( UNHOLY_STAFF_POLYMORPH, Target_RandomPlayer, 8, 0, 20, 0, 45 );	// NotCurrent ?
};

void UnholyStaffAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// The Construct Quarter

/////////////////////////////////////////////////////////////////////////////////
////// Patchwork Golem
PatchworkGolemAI::PatchworkGolemAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( PATCHWORK_GOLEM_CLEAVE, Target_Current, 10, 0, 10, 0, 8 );
	if ( IsHeroic() )
	{
		AddSpell( PATCHWORK_GOLEM_EXECUTE_HEROIC, Target_Current, 8, 0, 10, 0, 8 );
		AddSpell( PATCHWORK_GOLEM_WAR_STOMP_HEROIC, Target_Self, 8, 0, 15 );
		ApplyAura( PATCHWORK_GOLEM_DISEASE_CLOUD_HEROIC );
	}
	else
	{
		AddSpell( PATCHWORK_GOLEM_EXECUTE_NORMAL, Target_Current, 8, 0, 10, 0, 8 );
		AddSpell( PATCHWORK_GOLEM_WAR_STOMP_NORMAL, Target_Self, 8, 0, 15 );
		ApplyAura( PATCHWORK_GOLEM_DISEASE_CLOUD_NORMAL );
	};
};

void PatchworkGolemAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	if ( IsHeroic() )
		ApplyAura( PATCHWORK_GOLEM_DISEASE_CLOUD_HEROIC );
	else
		ApplyAura( PATCHWORK_GOLEM_DISEASE_CLOUD_NORMAL );
};

void PatchworkGolemAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( IsAlive() )
	{
		if ( IsHeroic() )
			ApplyAura( PATCHWORK_GOLEM_DISEASE_CLOUD_HEROIC );
		else
			ApplyAura( PATCHWORK_GOLEM_DISEASE_CLOUD_NORMAL );
	};
};

void PatchworkGolemAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Bile Retcher
BileRetcherAI::BileRetcherAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( BILE_RETCHER_BILE_VOMIT_NORMAL, Target_Destination, 10, 0, 10, 0, 20 );
	else
		AddSpell( BILE_RETCHER_BILE_VOMIT_HEROIC, Target_Destination, 10, 0, 10, 0, 20 );

	AddSpell( BILE_RETCHER_BILE_RETCHER_SLAM, Target_Destination, 8, 0, 10, 0, 10 );
};

void BileRetcherAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Sewage Slime
SewageSlimeAI::SewageSlimeAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	ApplyAura( SEWAGE_SLIME_DISEASE_CLOUD );
};

void SewageSlimeAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	ApplyAura( SEWAGE_SLIME_DISEASE_CLOUD );
};

void SewageSlimeAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	ApplyAura( SEWAGE_SLIME_DISEASE_CLOUD );
};

void SewageSlimeAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Embalming Slime
EmbalmingSlimeAI::EmbalmingSlimeAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	ApplyAura( EMBALMING_SLIME_EMBALMING_CLOUD );
};

void EmbalmingSlimeAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	ApplyAura( EMBALMING_SLIME_EMBALMING_CLOUD );
};

void EmbalmingSlimeAI::OnCombatStop( Unit* pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	ApplyAura( EMBALMING_SLIME_EMBALMING_CLOUD );
};

void EmbalmingSlimeAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Mad Scientist
MadScientistAI::MadScientistAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
	{
		AddSpell( MAD_SCIENTIST_GREAT_HEAL_HEROIC, Target_WoundedFriendly, 8, 2, 15, 0, 40 );
		AddSpell( MAD_SCIENTIST_MANA_BURN_HEROIC, Target_RandomPlayer, 10, 2, 10, 0, 40 );
	}
	else
	{
		AddSpell( MAD_SCIENTIST_GREAT_HEAL_NORMAL, Target_WoundedFriendly, 8, 2, 15, 0, 40 );
		AddSpell( MAD_SCIENTIST_MANA_BURN_NORMAL, Target_RandomPlayer, 10, 2, 10, 0, 40 );
	};
};

void MadScientistAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Living Monstrosity
LivingMonstrosityAI::LivingMonstrosityAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( LIVING_MONSTROSITY_FEAR, Target_Self, 8, 1, 20 );
	AddSpell( LIVING_MONSTROSITY_LIGHTNING_TOTEM, Target_Self, 8, 0.5, 25 );
	if ( IsHeroic() )
		AddSpell( LIVING_MONSTROSITY_CHAIN_LIGHTNING_HEROIC, Target_RandomPlayer, 10, 1.5, 10, 0, 45 );
	else
		AddSpell( LIVING_MONSTROSITY_CHAIN_LIGHTNING_NORMAL, Target_RandomPlayer, 10, 1.5, 10, 0, 45 );
};

void LivingMonstrosityAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Lightning Totem
LightningTotemAI::LightningTotemAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( LIGHTNING_TOTEM_SHOCK_HEROIC, Target_Self, 100, 0.5, 2 );
	else
		AddSpell( LIGHTNING_TOTEM_SHOCK_NORMAL, Target_Self, 100, 0.5, 2 );

	_unit->m_noRespawn = true;
	Despawn( 60000 );
};

void LightningTotemAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void LightningTotemAI::AIUpdate()
{
	ParentClass::AIUpdate();

	// Meh, reset it in case something went wrong
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void LightningTotemAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Stitched Colossus
StitchedColossusAI::StitchedColossusAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	if ( IsHeroic() )
		AddSpell( STITCHED_COLOSSUS_MASSIVE_STOMP_HEROIC, Target_Self, 8, 0, 15 );
	else
		AddSpell( STITCHED_COLOSSUS_MASSIVE_STOMP_NORMAL, Target_Self, 8, 0, 15 );
};

void StitchedColossusAI::OnCombatStart( Unit* pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mEnraged = false;
};

void StitchedColossusAI::AIUpdate()
{
	if ( !mEnraged && RandomUInt( 99 ) == 0 )
	{
		ApplyAura( STITCHED_COLOSSUS_UNSTOPPABLE_ENRAGE );
		mEnraged = true;
	};

	ParentClass::AIUpdate();
};

void StitchedColossusAI::Destroy()
{
	delete this;
};

/////////////////////////////////////////////////////////////////////////////////
////// Marauding Geist
MaraudingGeistAI::MaraudingGeistAI( Creature* pCreature ) : AscentScriptCreatureAI( pCreature )
{
	AddSpell( MARAUDING_GEIST_FRENZIED_LEAP, Target_RandomPlayer, 8, 0, 10 );
};

void MaraudingGeistAI::Destroy()
{
	delete this;
};

void SetupNaxxramas( ScriptMgr* pScriptMgr )
{
	/////////////////////////////////////////////////////////////////////////////////
	////// The Arachnid Quarter
	// ---- Trash ----
    pScriptMgr->register_creature_script( CN_CARRION_SPINNER, &CarrionSpinnerAI::Create );
	pScriptMgr->register_creature_script( CN_DREAD_CREEPER, &DreadCreeperAI::Create );
    pScriptMgr->register_creature_script( CN_NAXXRAMAS_CULTIST, &NaxxramasCultistAI::Create );
	pScriptMgr->register_creature_script( CN_VENOM_STALKER, &VenomStalkerAI::Create );
	pScriptMgr->register_creature_script( CN_TOMB_HORROR, &TombHorrorAI::Create );
    pScriptMgr->register_creature_script( CN_NAXXRAMAS_ACOLYTE, &NaxxramasAcolyteAI::Create );
	pScriptMgr->register_creature_script( CN_VIGILANT_SHADE, &VigilantShadeAI::Create );
	pScriptMgr->register_creature_script( CN_CRYPT_REAVER, &CryptReaverAI::Create );
	pScriptMgr->register_creature_script( CN_WEB_WRAP, &WebWrapAI::Create );
	pScriptMgr->register_creature_script( CN_MAEXXNA_SPIDERLING, &MaexxnaSpiderlingAI::Create );
	pScriptMgr->register_creature_script( CN_NAXXRAMAS_WORSHIPPER, &NaxxramasWorshipperAI::Create );
	pScriptMgr->register_creature_script( CN_NAXXRAMAS_FOLLOWER, &NaxxramasFollowerAI::Create );
	pScriptMgr->register_creature_script( CN_NAXXRAMAS_WORSHIPPER, &NaxxramasWorshipperAI::Create );
	pScriptMgr->register_creature_script( CN_CRYPT_GUARD, &CryptGuardAI::Create );
	pScriptMgr->register_creature_script( CN_CORPSE_SCARAB, &CorpseScarabAI::Create );
	// ---- Bosses ----
	pScriptMgr->register_creature_script( CN_MAEXXNA, &MaexxnaAI::Create );
	pScriptMgr->register_creature_script( CN_GRAND_WIDOW_FAERLINA, &GrandWidowFaerlinaAI::Create );
	pScriptMgr->register_creature_script( CN_ANUBREKHAN, &AnubRekhanAI::Create );

	/////////////////////////////////////////////////////////////////////////////////
	////// The Plague Quarter
	// ---- Trash ----
	pScriptMgr->register_creature_script( CN_INFECTIOUS_GHOUL, &InfectiousGhoulAI::Create );
	pScriptMgr->register_creature_script( CN_STONESKIN_GARGOYLE, &StoneskinGargoyleAI::Create );
	pScriptMgr->register_creature_script( CN_FRENZIED_BAT, &FrenziedBatAI::Create );
	pScriptMgr->register_creature_script( CN_PLAGUE_BEAST, &PlagueBeastAI::Create );
	pScriptMgr->register_creature_script( CN_EYE_STALKER, &EyeStalkerAI::Create );
	pScriptMgr->register_creature_script( CN_PLAGUED_WARRIOR, &PlaguedWarriorAI::Create );
	pScriptMgr->register_creature_script( CN_PLAGUED_CHAMPION, &PlaguedChampionAI::Create );
	pScriptMgr->register_creature_script( CN_PLAGUED_GUARDIAN, &PlaguedGuardianAI::Create );
	pScriptMgr->register_creature_script( CN_FISSURE_TRIGGER, &FissureTriggerAI::Create );
	pScriptMgr->register_creature_script( CN_SPORE, &SporeAI::Create );
	for ( uint32 Id = 181510; Id < 181553; ++Id )
	{
		pScriptMgr->register_gameobject_script( Id, &PlagueFissureGO::Create );
	};

	for ( uint32 Id = 181676; Id < 181679; ++Id )
	{
		pScriptMgr->register_gameobject_script( Id, &PlagueFissureGO::Create );
	};
	pScriptMgr->register_gameobject_script( 181695, &PlagueFissureGO::Create );
	// ---- Bosses ----
	pScriptMgr->register_creature_script( CN_NOTH_THE_PLAGUEBRINGER, &NothThePlaguebringerAI::Create );
	pScriptMgr->register_creature_script( CN_HEIGAN_THE_UNCLEAN, &HeiganTheUncleanAI::Create );
	pScriptMgr->register_creature_script( CN_LOATHEB, &LoathebAI::Create );

	/////////////////////////////////////////////////////////////////////////////////
	////// The Military Quarter
	// ---- Trash ----
	pScriptMgr->register_creature_script( CN_DEATH_KNIGHT, &DeathKnightAI::Create );
	pScriptMgr->register_creature_script( CN_DEATH_KNIGHT_CAPTAIN, &DeathKnightCaptainAI::Create );
	pScriptMgr->register_creature_script( CN_SHADE_OF_NAXXRAMAS, &ShadeOfNaxxramasAI::Create );
	pScriptMgr->register_creature_script( CN_PORTAL_OF_SHADOWS, &PortalOfShadowsAI::Create );
	pScriptMgr->register_creature_script( CN_NECRO_KNIGHT, &NecroKnightAI::Create );
	pScriptMgr->register_creature_script( CN_SKELETAL_SMITH, &SkeletalSmithAI::Create );
	pScriptMgr->register_creature_script( CN_DEATH_KNIGHT_CAVALIER, &DeathKnightCavalierAI::Create );
	pScriptMgr->register_creature_script( CN_DEATHCHARGER_STEED, &DeathchargerSteedAI::Create );
	pScriptMgr->register_creature_script( CN_DARK_TOUCHED_WARRIOR, &DarkTouchedWarriorAI::Create );
	pScriptMgr->register_creature_script( CN_RISEN_SQUIRE, &RisenSquireAI::Create );
	pScriptMgr->register_creature_script( CN_UNHOLY_AXE, &UnholyAxeAI::Create );
	pScriptMgr->register_creature_script( CN_UNHOLY_SWORD, &UnholySwordAI::Create );
	pScriptMgr->register_creature_script( CN_UNHOLY_STAFF, &UnholyStaffAI::Create );
	pScriptMgr->register_creature_script( CN_DEATH_KNIGHT_UNDERSTUDY, &DeathKnightUnderstudyAI::Create );
	// ---- Bosses ----
	pScriptMgr->register_creature_script( CN_INSTRUCTOR_RAZUVIOUS, &InstructorRazuviousAI::Create );

	/////////////////////////////////////////////////////////////////////////////////
	////// The Construct Quarter
	// ---- Trash ----
	pScriptMgr->register_creature_script( CN_PATCHWORK_GOLEM, &PatchworkGolemAI::Create );
	pScriptMgr->register_creature_script( CN_BILE_RETCHER, &BileRetcherAI::Create );
	pScriptMgr->register_creature_script( CN_SEWAGE_SLIME, &SewageSlimeAI::Create );
	pScriptMgr->register_creature_script( CN_EMBALMING_SLIME, &EmbalmingSlimeAI::Create );
	pScriptMgr->register_creature_script( CN_MAD_SCIENTIST, &MadScientistAI::Create );
	pScriptMgr->register_creature_script( CN_LIVING_MONSTROSITY, &LivingMonstrosityAI::Create );
	pScriptMgr->register_creature_script( CN_LIGHTNING_TOTEM, &LightningTotemAI::Create );
	pScriptMgr->register_creature_script( CN_STITCHED_COLOSSUS, &StitchedColossusAI::Create );
	pScriptMgr->register_creature_script( CN_MARAUDING_GEIST, &MaraudingGeistAI::Create );

	// ---- Abomination Wing ----
	pScriptMgr->register_creature_script(STICKED_SPEWER, &StickedSpewerAI::Create);
	pScriptMgr->register_creature_script(CN_SURGICAL_ASSISTANT, &SurgicalAssistantAI::Create);
	pScriptMgr->register_creature_script(CN_SLUDGE_BELCHER, &StickedSpewerAI::Create);
	// BOSS'S
	pScriptMgr->register_creature_script(CN_PATCHWREK, &PatchwerkAI::Create);
	pScriptMgr->register_creature_script(CN_GROBBULUS, &GrobbulusAI::Create);
	pScriptMgr->register_creature_script(CN_GLUTH, &GluthAI::Create);
	
	// ---- Deathknight Wing ----
	pScriptMgr->register_creature_script(CN_BONY_CONSTRUCT, &BonyConstructAI::Create);
	pScriptMgr->register_creature_script(CN_DEATH_LORD, &DeathLordAI::Create);
	pScriptMgr->register_creature_script(16163, &DeathLordAI::Create);
	pScriptMgr->register_creature_script(CN_BONY_CONSTRUCT, &BonyConstructAI::Create);
	// BOSS'S
	//pScriptMgr->register_creature_script(CN_INSTRUCTOR_RAZUVIOUS, &RazuviousAI::Create);
	// The Four Horsemen:
	pScriptMgr->register_creature_script(CN_THANE_KORTHAZZ, &KorthazzAI::Create);
	pScriptMgr->register_creature_script(CN_LADY_BLAUMEUX, &BlaumeuxAI::Create);
	pScriptMgr->register_creature_script(CN_SIR_ZELIEK, &ZeliekAI::Create);
	//pScriptMgr->register_creature_script(CN_Baron_Rivendare_4H, &RivendareAI::Create);

	// ---- Frostwyrm Lair ---- > Sapphiron Encounter:
	pScriptMgr->register_creature_script(CN_FROST_BREATH_TRIGGER, &FrostBreathTriggerAI::Create);
	pScriptMgr->register_creature_script(CN_FROST_BREATH_TRIGGER2, &FrostBreathTrigger2AI::Create);
	pScriptMgr->register_creature_script(CN_FROST_BREATH_TRIGGER3, &FrostBreathTrigger3AI::Create);
	pScriptMgr->register_creature_script(CN_CHILL_TRIGGER, &ChillTriggerAI::Create);
	pScriptMgr->register_creature_script(CN_SAPPHIRON, &SapphironAI::Create);

	// ---- Frostwyrm Lair ---- > Kel'thuzad Encounter:
	pScriptMgr->register_creature_script(CN_THE_LICH_KING ,&TheLichKingAI::Create);
	pScriptMgr->register_creature_script(CN_SOLDIER_OF_THE_FROZEN_WASTES ,&SoldierOfTheFrozenWastesAI::Create);
	pScriptMgr->register_creature_script(CN_UNSTOPPABLE_ABOMINATION ,&UnstoppableAbominationAI::Create);
	pScriptMgr->register_creature_script(CN_SOUL_WEAVER ,&SoulWeaverAI::Create);
	pScriptMgr->register_creature_script(CN_GUARDIAN_OF_ICECROWN ,&GuardianOfIcecrownAI::Create);
	pScriptMgr->register_creature_script(CN_KELTHUZAD, &KelthuzadAI::Create);
	
	// ---- Go Scripts ---- >
	//pScriptMgr->register_gameobject_script(181229, &FrostPortal::Create);
};
