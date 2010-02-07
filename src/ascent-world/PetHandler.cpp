/*
 * Ascent MMORPG Server
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

void WorldSession::HandlePetAction(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	//WorldPacket data;
	uint64 petGuid = 0;
	uint16 misc = 0;
	uint16 action = 0;

	uint64 targetguid = 0;
	recv_data >> petGuid >> misc >> action;
	//recv_data.hexlike();

	//printf("Pet_Action: 0x%.4X 0x%.4X\n", misc, action);

	if(GET_TYPE_FROM_GUID(petGuid) == HIGHGUID_TYPE_UNIT)
	{
		Creature* pCharm = GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(petGuid));
		if(!pCharm) 
			return;

		// must be a mind controled creature..
		if(action == PET_ACTION_ACTION)
		{
			recv_data >> targetguid;
			switch(misc)
			{
			case PET_ACTION_ATTACK:
				{
					if(!sEventMgr.HasEvent(_player, EVENT_PLAYER_CHARM_ATTACK))
					{
						uint32 timer = pCharm->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
						if(!timer) timer = 2000;

						sEventMgr.AddEvent(_player, &Player::_EventCharmAttack, EVENT_PLAYER_CHARM_ATTACK, timer, 0,0);
						_player->_EventCharmAttack();
					}
				}break;
			}
		}
		return;
	}
	Pet* pPet = _player->GetMapMgr()->GetPet(GET_LOWGUID_PART(petGuid));
	if(!pPet || !pPet->isAlive())
		return;

	Unit* pTarget = NULL;

	if(action == PET_ACTION_SPELL || action == PET_ACTION_SPELL_1 || action == PET_ACTION_SPELL_2 || (action == PET_ACTION_ACTION && misc == PET_ACTION_ATTACK )) // >> target
	{
		recv_data >> targetguid;
		pTarget = _player->GetMapMgr()->GetUnit(targetguid);
		if(!pTarget) pTarget = pPet;	// target self
	}

	switch(action)
	{
	case PET_ACTION_ACTION:
		{
			pPet->SetPetAction(misc);	   // set current action


			// Action time? Stand up !
			if( misc!=PET_ACTION_STAY && pPet->GetStandState() == STANDSTATE_SIT ) 
						pPet->SetStandState(STANDSTATE_STAND);

			switch(misc)
			{
			case PET_ACTION_ATTACK:
				{
					
					// make sure the target is attackable
					if(pTarget == pPet || !isAttackable(pPet, pTarget))
					{
						WorldPacket data(SMSG_SPELL_FAILURE, 20);
						data << _player->GetNewGUID() << uint32(0) << uint32(0) << uint8(SPELL_FAILED_BAD_TARGETS);
						SendPacket(&data);
						return;
					}

					// Clear the threat
					pPet->GetAIInterface()->WipeTargetList();
					pPet->GetAIInterface()->WipeHateList();

					// Attack target with melee if the owner if we dont have spells - other wise cast. All done by AIInterface.
					if(pPet->GetAIInterface()->getUnitToFollow() == NULL)
						pPet->GetAIInterface()->SetUnitToFollow(_player);

					// EVENT_PET_ATTACK
					pPet->GetAIInterface()->SetAIState(STATE_ATTACKING);
					pPet->GetAIInterface()->AttackReaction(pTarget, 1, 0);
				}break;
			case PET_ACTION_FOLLOW:
				{
					// Clear the threat
					pPet->GetAIInterface()->WipeTargetList();
					pPet->GetAIInterface()->WipeHateList();

					// Follow the owner... run to him...
					pPet->GetAIInterface()->SetUnitToFollow(_player);
					pPet->GetAIInterface()->HandleEvent(EVENT_FOLLOWOWNER, pPet, 0);
				}break;
			case PET_ACTION_STAY:
				{
					// Clear the threat
					pPet->GetAIInterface()->WipeTargetList();
					pPet->GetAIInterface()->WipeHateList();

					// Stop following the owner, and sit.
					pPet->GetAIInterface()->SetUnitToFollow(NULL);
					pPet->SetStandState(STANDSTATE_SIT);
				}break;
			case PET_ACTION_DISMISS:
				{
					// Bye byte...
					pPet->Dismiss();
				}break;
			}
		}break;

	case PET_ACTION_SPELL_2:
	case PET_ACTION_SPELL_1:
	case PET_ACTION_SPELL:
		{
			// misc == spellid
			SpellEntry *entry = dbcSpell.LookupEntry(misc);
			if(!entry) 
				return;

			AI_Spell*sp = pPet->GetAISpellForSpellId(entry->Id);
			if(sp)
			{
				// Check the cooldown
				if(sp->cooldowntime && getMSTime() < sp->cooldowntime)
				{
					//SendNotification("That spell is still cooling down.");
					WorldPacket data(SMSG_SPELL_FAILURE, 20);
					data << pPet->GetNewGUID();
					data << uint8(0); //extra_cast_number
					data << sp->spell->Id;
					data << uint8(SPELL_FAILED_NOT_READY);
					SendPacket(&data);
				}
				else
				{
					if(sp->spellType != STYPE_BUFF)
					{
						// make sure the target is attackable
						if(pTarget == pPet || !isAttackable(pPet, pTarget))
						{
							WorldPacket data(SMSG_SPELL_FAILURE, 20);
							data << _player->GetNewGUID() << uint32(0) << sp->spell->Id << uint8(SPELL_FAILED_BAD_TARGETS);
							SendPacket(&data);
							return;
						}
					}

					if(sp->autocast_type != AUTOCAST_EVENT_ATTACK)
					{
						if(sp->autocast_type == AUTOCAST_EVENT_OWNER_ATTACKED)
							sEventMgr.AddEvent(TO_UNIT( pPet ), &Unit::EventCastSpell, TO_UNIT( _player ), sp->spell, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
						else
							sEventMgr.AddEvent(TO_UNIT( _player ), &Unit::EventCastSpell, TO_UNIT( pPet ), sp->spell, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					}
					else
					{
						// Clear the threat
						pPet->GetAIInterface()->WipeTargetList();
						pPet->GetAIInterface()->WipeHateList();

						pPet->GetAIInterface()->AttackReaction(pTarget, 1, 0);
						pPet->GetAIInterface()->SetNextSpell(sp);
					}
				}
			}
		}break;
	case PET_ACTION_STATE:
		{
			pPet->SetPetState(misc);
		}break;
	default:
		{
			DEBUG_LOG("Pet","WARNING: Unknown pet action received. Action = %.4X, Misc = %.4X\n", action, misc);
		}break;
	}

	/* Send pet action sound - WHEE THEY TALK */
	WorldPacket actionp(SMSG_PET_ACTION_SOUND, 12);
	actionp << pPet->GetGUID() << uint32(1);
	SendPacket(&actionp);
}

void WorldSession::HandlePetInfo(WorldPacket & recv_data)
{
	//nothing
	DEBUG_LOG("WorldSession","HandlePetInfo is called");
}

void WorldSession::HandlePetNameQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;
	uint32 reqNumber = 0;
	uint64 petGuid = 0;

	recv_data >> reqNumber >> petGuid;
	Pet* pPet = _player->GetMapMgr()->GetPet(GET_LOWGUID_PART(petGuid));
	if(!pPet) return;

	WorldPacket data(8 + pPet->GetName().size());
	data.SetOpcode(SMSG_PET_NAME_QUERY_RESPONSE);
	data << reqNumber;
	data << pPet->GetName();
	data << pPet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);		// stops packet flood
	data << uint8(0);		// unk
	SendPacket(&data);
}

void WorldSession::HandleStablePet(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	// remove pet from world and association with player
	Pet* pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) 
		return;
	
	PlayerPet *pet = _player->GetPlayerPet(_player->GetUnstabledPetNumber());
	if(!pet) 
		return;
	pet->stablestate = STABLE_STATE_PASSIVE;
	
	if(pPet) pPet->Remove(false, true, true);	// no safedelete needed

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x8);  // success
	SendPacket(&data);
}

void WorldSession::HandleUnstablePet(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;
	
	uint64 npcguid = 0;
	uint32 petnumber = 0;

	recv_data >> npcguid >> petnumber;
	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		OUT_DEBUG("PET SYSTEM: Player "I64FMT" tried to unstable non-existant pet %d", _player->GetGUID(), petnumber);
		return;
	}
	_player->SpawnPet(petnumber);
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(STABLERESULT_UNSTABLE_SUCCESS); // success?
	SendPacket(&data);

	Pet* pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) return;

	uint32 pLevel = (_player->getLevel() - 5);
	if(pPet->getLevel() < pLevel)
		pPet->LevelUpTo(pLevel);
}
void WorldSession::HandleStableSwapPet(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	uint64 npcguid = 0;
	uint32 petnumber = 0;
	recv_data >> npcguid >> petnumber;

	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		OUT_DEBUG("PET SYSTEM: Player "I64FMT" tried to unstable non-existant pet %d", _player->GetGUID(), petnumber);
		return;
	}
	Pet* pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) return;

	//stable current pet
	PlayerPet *pet2 = _player->GetPlayerPet(_player->GetUnstabledPetNumber());

	if(!pet2) 
		return;

	if(pPet)
		pPet->Remove(false, true, true);	// no safedelete needed
	pet2->stablestate = STABLE_STATE_PASSIVE;

	//unstable selected pet
	_player->SpawnPet(petnumber);
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data;
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(STABLERESULT_UNSTABLE_SUCCESS);
	SendPacket(&data);

	Pet* pPet2 = _player->GetSummon();
	if(pPet2 && pPet2->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) return;

	uint32 pLevel = (_player->getLevel() - 5);
	if(pPet2->getLevel() < pLevel)
		pPet2->LevelUpTo(pLevel);
}

void WorldSession::HandleStabledPetList(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;
	WorldPacket data(10 + (_player->m_Pets.size() * 25));
	data.SetOpcode(MSG_LIST_STABLED_PETS);

	uint64 npcguid = 0;
	recv_data >> npcguid;
	data << npcguid;

	data << uint8(_player->m_Pets.size());
	data << uint8(_player->m_StableSlotCount);
	uint8 i=0;
	bool HasActive = false;
	for(std::map<uint32, PlayerPet*>::iterator itr = _player->m_Pets.begin(); itr != _player->m_Pets.end(); ++itr)
	{
		data << uint32(itr->first); // pet no
		data << uint32(itr->second->entry); // entryid
		data << uint32(itr->second->level); // level
		data << itr->second->name;		  // name
		if(!HasActive && itr->second->stablestate == STABLE_STATE_ACTIVE)
		{
			data << uint8(STABLE_STATE_ACTIVE);
			HasActive = true;
			continue;
		}
		data << uint8(STABLE_STATE_PASSIVE + i);
		itr->second->stablestate = STABLE_STATE_PASSIVE; //if it ain't active it must be passive ;)
		i++;
		if(i>3)//we only have 4 stable slots
			break;
	}

	SendPacket(&data);
}

void WorldSession::HandleBuyStableSlot(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || _player->GetStableSlotCount() == MAX_STABLE_SLOTS)
		return;
	uint8 scount = _player->GetStableSlotCount();
	BankSlotPrice* bsp = dbcStableSlotPrices.LookupEntry(scount+1);
	int32 cost = (bsp != NULL) ? bsp->Price : 99999999;
	if(cost > (int32)_player->GetUInt32Value(PLAYER_FIELD_COINAGE))
	{
		WorldPacket data(1);
		data.SetOpcode(SMSG_STABLE_RESULT);
		data << uint8(STABLERESULT_FAIL_CANT_AFFORD);
		SendPacket(&data);
		return;
	}

	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -cost);
	
	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(STABLERESULT_BUY_SLOT_SUCCESS);
	SendPacket(&data);
	if(_player->GetStableSlotCount() > MAX_STABLE_SLOTS)
		_player->m_StableSlotCount = MAX_STABLE_SLOTS;
	else
		_player->m_StableSlotCount++;
#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Misc();
#endif
}


void WorldSession::HandlePetSetActionOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	uint32 unk1;
	uint32 unk2;
	uint32 slot;
	uint16 spell;
	uint16 state;
	recv_data >> unk1 >> unk2 >> slot >> spell >> state;
	if(!_player->GetSummon())
		return;

	Pet* pet = _player->GetSummon();
	SpellEntry * spe = dbcSpell.LookupEntryForced( spell );
	if( spe == NULL )
		return;

	// do we have the spell? if not don't set it (exploit fix)
	PetSpellMap::iterator itr = pet->GetSpells()->find( spe );
	if( itr == pet->GetSpells()->end( ) )
		return;

	pet->ActionBar[slot] = spell;
	pet->SetSpellState(spell, state);
}

void WorldSession::HandlePetRename(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;
	uint64 guid;
	string name;
	recv_data >> guid >> name;

	if(!_player->GetSummon() || _player->GetSummon()->GetGUID() != guid)
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	Pet* pet = _player->GetSummon();
	pet->Rename(name);

	// Disable pet rename.
	pet->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x2 << 16));
}

void WorldSession::HandlePetAbandon(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;
	Pet* pet = _player->GetSummon();
	if(!pet) return;

	pet->Dismiss(false);
}
void WorldSession::HandlePetUnlearn(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	uint64 guid;
	recv_data >> guid;

	Pet* pPet = _player->GetSummon();
	if( pPet == NULL || pPet->GetGUID() != guid )
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	int32 cost = pPet->GetUntrainCost();
	if( cost > ( int32 )_player->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
	{
		WorldPacket data(SMSG_BUY_FAILED, 12);
		data << uint64( _player->GetGUID() );
		data << uint32( 0 );
		data << uint8( 2 );		//not enough money
		return;	
	}
	_player->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
	pPet->WipeSpells();
	_player->smsg_TalentsInfo(true);
}

void WorldSession::HandleTotemDestroyed(WorldPacket & recv_data)
{
	// This code can handle destroying totem when it was right clicked in client. 
	// But now this packet for some reason is also sent when server destroys the totem which leads to a bug.
	// So leave it commented for now
	/*if( !_player->IsInWorld() )
		return;
	uint8 slot;
	recv_data >> slot;
	slot++;
	if(slot == 0 || slot > 4 || _player->m_SummonSlots[slot] == NULL)
		return;
	_player->SummonExpireSlot(slot);*/
}

void WorldSession::HandlePetLearnTalent( WorldPacket & recvPacket )  
{  
	CHECK_INWORLD_RETURN;

	uint64 guid = 0;
	uint32 talentid, rank = 0;

	recvPacket >> guid >> talentid >> rank;

	// get a pointer to our current pet
	Pet * pPet = _player->GetSummon();
	if( pPet == NULL )
		return;

	// check the guid to add the talent to is the same as our pets
	// we don't want any cheating now do we
	if( pPet->GetGUID() != guid )
		return;

	// check we have talent points to spend
	uint8 talentPoints = pPet->GetByte( UNIT_FIELD_BYTES_1, 1 );
	if(!talentPoints)
		return;
	else
	{
		// deduct a point now from our stored value
		// we will set this later in case one of our checks fails
		talentPoints--;
	}

	// find our talent
	TalentEntry *talentEntry = dbcTalent.LookupEntry( talentid );
	if( talentEntry == NULL )
		return;

	PetTalentMap::iterator itr;
	// check if we require another talent first to be able to learn this one
	if( talentEntry->DependsOn )
	{
		// find the talent that we require
		// to be able to add this one
		TalentEntry *requiredTalent = dbcTalent.LookupEntryForced( talentEntry->DependsOn );
		if( requiredTalent == NULL )
			return;

		// get the rank of the talent that we require
		// to be able to add this one
		uint32 requiredRank = talentEntry->DependsOnRank;

		// do we have the required talent in our map
		itr = pPet->m_talents.find(talentEntry->DependsOn);
		if (itr != pPet->m_talents.end())
		{
			// is the rank less that the rank we require?
			if(itr->second < requiredRank)
				return; // if so abort

			// if we didn't hit the if case then we have the talent
			// do nothing and continue adding the talent
		}
		else
			return; // we don't have the talent in our map, abort!
	}

	itr = pPet->m_talents.find(talentid);
	// do we have a lower rank of this talent?
	if (itr != pPet->m_talents.end())
	{
		// Remove the lower rank spell from our pet
		pPet->RemoveSpell( talentEntry->RankID[ rank - 1 ] );
		// Replace the rank in our map with the new one, we'll add the spell later
		itr->second = rank;
	}
	else
		pPet->m_talents.insert( make_pair( talentid, rank ) );

	// find spell
	SpellEntry* sp = dbcSpell.LookupEntry( talentEntry->RankID[ rank ] );
	if( sp )
	{
		// set the new talent points, remember we deducted a point earlier ;)
		DEBUG_LOG("Pet","Setting available talent points to %u", talentPoints);
		//pPet->SetUInt32Value(UNIT_FIELD_BYTES_1, 1 | (talentPoints << 24));
		pPet->SetByte( UNIT_FIELD_BYTES_1, 1, talentPoints );

		// add the talent spell to our pet
		pPet->AddSpell( sp, true );

		// send the packet to the client saying we've learned it :D
		OutPacket( SMSG_PET_LEARNED_SPELL, 4, &sp->Id );

		// finally re-send our actionbar/spells to the player xD
		pPet->SendSpellsToOwner();
	}
	_player->smsg_TalentsInfo(true);
}