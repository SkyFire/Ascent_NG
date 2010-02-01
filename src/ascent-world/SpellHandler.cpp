/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "StdAfx.h"

void WorldSession::HandleUseItemOpcode(WorldPacket& recvPacket)
{
	CHECK_INWORLD_RETURN;
	
	PlayerPointer p_User = GetPlayer();
	DEBUG_LOG("WORLD","Received use Item packet, data length = %i",recvPacket.size());
	int8 tmp1,slot;
	uint8 unk; // 3.0.2 added unk
	uint64 item_guid;
	uint8 cn;
	uint32 spellId = 0;
	uint32 glyphIndex;

	recvPacket >> tmp1 >> slot >> cn >> spellId >> item_guid >> glyphIndex >> unk;
	ItemPointer tmpItem = NULLITEM;
	tmpItem = p_User->GetItemInterface()->GetInventoryItem(tmp1,slot);
	if (!tmpItem)
		tmpItem = p_User->GetItemInterface()->GetInventoryItem(slot);
	if (!tmpItem)
		return;
	ItemPrototype *itemProto = tmpItem->GetProto();
	if(!itemProto)
		return;

	if(_player->getDeathState()==CORPSE)
		return;

	if(itemProto->Bonding == ITEM_BIND_ON_USE)
		tmpItem->SoulBind();

	if(sScriptMgr.CallScriptedItem(tmpItem,_player))
		return;

	if(itemProto->QuestId)
	{
		// Item Starter
		Quest *qst = QuestStorage.LookupEntry(itemProto->QuestId);
		if(!qst) 
			return;

		if( sQuestMgr.PlayerMeetsReqs(_player, qst, false) != QMGR_QUEST_AVAILABLE || qst->min_level > _player->getLevel() )
			return;

        WorldPacket data;
        sQuestMgr.BuildQuestDetails(&data, qst, tmpItem, 0, language, _player);
		SendPacket(&data);
	}
	
	SpellCastTargets targets(recvPacket, _player->GetGUID());
	uint32 x;
	for(x = 0; x < 5; x++)
	{
		if(itemProto->Spells[x].Trigger == USE)
		{
			if(itemProto->Spells[x].Id)
			{
				spellId = itemProto->Spells[x].Id;
				break;
			}
		}
	}
	if(!spellId)
		return;
	// check for spell id
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced( spellId );

	if(!spellInfo)
	{
		OUT_DEBUG("WORLD: unknown spell id %i\n", spellId);
		return;
	}

	if (spellInfo->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		if (p_User->CombatStatus.IsInCombat() || p_User->IsMounted())
		{
			_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULLITEM,INV_ERR_CANT_DO_IN_COMBAT);
			return;
		}
	
		if(p_User->GetStandState()!=STANDSTATE_SIT)
			p_User->SetStandState(STANDSTATE_SIT);
	}

	if(itemProto->RequiredLevel)
	{
		if(_player->getLevel() < itemProto->RequiredLevel)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULLITEM,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			return;
		}
	}

	if(itemProto->RequiredSkill)
	{
		if(!_player->_HasSkillLine(itemProto->RequiredSkill))
		{
			_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULLITEM,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			return;
		}

		if(itemProto->RequiredSkillRank)
		{
			if(_player->_GetSkillLineCurrent(itemProto->RequiredSkill, false) < itemProto->RequiredSkillRank)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULLITEM,INV_ERR_ITEM_RANK_NOT_ENOUGH);
				return;
			}
		}
	}
	
	if( itemProto->AllowableClass && !(_player->getClassMask() & itemProto->AllowableClass) || itemProto->AllowableRace && !(_player->getRaceMask() & itemProto->AllowableRace) )
	{
		_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULLITEM,INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM);
		return;
	}		

	if( !_player->Cooldown_CanCast( itemProto, x ) )
	{
		_player->SendCastResult(spellInfo->Id, SPELL_FAILED_NOT_READY, cn, 0);
		return;
	}

	if(_player->m_currentSpell)
	{
		_player->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
		return;
	}

	if( itemProto->ForcedPetId >= 0 )
	{
		if( itemProto->ForcedPetId == 0 )
		{
			if( _player->GetGUID() != targets.m_unitTarget )
			{
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0);
				return;
			}
		}
		else
		{
			if( !_player->GetSummon() || _player->GetSummon()->GetEntry() != (uint32)itemProto->ForcedPetId )
			{
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
				return;
			}
		}
	}

	if(!sHookInterface.OnCastSpell(_player, spellInfo))
	{
		_player->SendCastResult(spellInfo->Id, SPELL_FAILED_UNKNOWN, cn, 0);
		return;
	}

	SpellPointer spell(new Spell(_player, spellInfo, false, NULLAURA));
	spell->extra_cast_number=cn;
	spell->m_glyphIndex = glyphIndex;
	spell->i_caster = tmpItem;
	if( spell->prepare(&targets) == SPELL_CANCAST_OK )
		_player->Cooldown_AddItem( itemProto, x );
}

void WorldSession::HandleCastSpellOpcode(WorldPacket& recvPacket)
{
	CHECK_INWORLD_RETURN;
	if(_player->getDeathState()==CORPSE)
		return;

	uint32 spellId;
	uint8 cn, unk; // 3.0.2 unk

	recvPacket >> cn >> spellId  >> unk;
	if(!spellId)
	{
		OUT_DEBUG("WORLD: unknown spell id %i\n", spellId);
		return;
	}
	// check for spell id
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced(spellId);

	if(!spellInfo || !sHookInterface.OnCastSpell(_player, spellInfo))
	{
		OUT_DEBUG("WORLD: unknown spell id %i\n", spellId);
		return;
	}

	OUT_DEBUG("WORLD: got cast spell packet, spellId - %i (%s), data length = %i",
		spellId, spellInfo->Name, recvPacket.size());
	
	// Cheat Detection only if player and not from an item
	// this could fuck up things but meh it's needed ALOT of the newbs are using WPE now
	// WPE allows them to mod the outgoing packet and basicly choose what ever spell they want :(

	if( !GetPlayer()->HasSpell(spellId) || spellInfo->Attributes & ATTRIBUTES_PASSIVE )
	{
		// Some spells the player doesn't actually know, but are given to him by his current shapeshift.
		// These spells should be allowed to be cast.
		uint8 shapeshift = GetPlayer()->GetShapeShift();
		SpellShapeshiftForm * ssf = dbcSpellShapeshiftForm.LookupEntryForced(shapeshift);
		if(!ssf) return;

		bool ok = false;
		for(uint8 i = 0; i < 7; ++i)
			if( ssf->spells[i] == spellId)
				ok = true;

		if( !ok )
			return;
	}

	if (GetPlayer()->GetOnMeleeSpell() != spellId)
	{
		//autoshot 75
		if((spellInfo->Flags3 & FLAGS3_ACTIVATE_AUTO_SHOT) /*spellInfo->Attributes == 327698*/)	// auto shot..
		{
			//sLog.outString( "HandleSpellCast: Auto Shot-type spell cast (id %u, name %s)" , spellInfo->Id , spellInfo->Name );
			ItemPointer weapon = GetPlayer()->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
			if(!weapon) 
				return;
			uint32 spellid;
			switch(weapon->GetProto()->SubClass)
			{
			case 2:			 // bows
			case 3:			 // guns
            case 18:		 // crossbow
				spellid = SPELL_RANGED_GENERAL;
				break;
			case 16:			// thrown
				spellid = SPELL_RANGED_THROW;
				break;
			case 19:			// wands
				spellid = SPELL_RANGED_WAND;
				break;
			default:
				spellid = 0;
				break;
			}
		   
			if(!spellid) 
				spellid = spellInfo->Id;
			
			if(!_player->m_onAutoShot)
			{
				_player->m_AutoShotTarget = _player->GetSelection();
				uint32 duration = _player->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME);
				SpellCastTargets targets(recvPacket,GetPlayer()->GetGUID());
				if(!targets.m_unitTarget)
				{
					sLog.outString( "Cancelling auto-shot cast because targets.m_unitTarget is null!" );
					return;
				}
				SpellEntry *sp = dbcSpell.LookupEntry(spellid);
			
				_player->m_AutoShotSpell = sp;
				_player->m_AutoShotDuration = duration;
				//This will fix fast clicks
				if(_player->m_AutoShotAttackTimer < 500)
					_player->m_AutoShotAttackTimer = 500;
				_player->m_onAutoShot = true;
			}

			return;
		}

        if(_player->m_currentSpell)
        {
			if( _player->m_currentSpell->getState() == SPELL_STATE_CASTING )
			{
				// cancel the existing channel spell, cast this one
				_player->m_currentSpell->cancel();
			}
			else
			{
				// send the error message
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
				return;
			}
        }

		SpellCastTargets targets(recvPacket,GetPlayer()->GetGUID());

		// some anticheat stuff
		if( spellInfo->self_cast_only )
		{
			if( targets.m_unitTarget && targets.m_unitTarget != _player->GetGUID() )
			{
				// send the error message
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0);
				return;
			}
		}

		if( targets.m_unitTarget && GetPlayer()->GetMapMgr() && spellInfo->c_is_flags & SPELL_FLAG_IS_DAMAGING )
		{
			UnitPointer pUnit = GetPlayer()->GetMapMgr()->GetUnit( targets.m_unitTarget );
			if( pUnit && pUnit != GetPlayer() && !isAttackable( GetPlayer(), pUnit, false ) && !pUnit->IsInRangeOppFactSet(GetPlayer()) && !pUnit->CombatStatus.DidDamageTo(GetPlayer()->GetGUID()))
			{
				//GetPlayer()->BroadcastMessage("Faction exploit detected. You will be disconnected in 5 seconds.");
				//GetPlayer()->Kick(5000);
				// Just cancel the cast
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0);
				return;
			}
		}

		SpellPointer spell(new Spell(GetPlayer(), spellInfo, false, NULLAURA));
		spell->extra_cast_number=cn;
		spell->prepare(&targets);
	}
}

void WorldSession::HandleCancelCastOpcode(WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	if(GetPlayer()->m_currentSpell)
		GetPlayer()->m_currentSpell->cancel();
}

void WorldSession::HandleCancelAuraOpcode( WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	if(spellId == 33763 || spellId == 48450 || spellId == 48451) // Prevents Lifebloom exploit
		return;
	
	for(uint32 x = 0; x < MAX_AURAS+MAX_POSITIVE_AURAS; ++x)
	{
		if(_player->m_auras[x] && _player->m_auras[x]->IsPositive() && _player->m_auras[x]->GetSpellId() == spellId)
			_player->m_auras[x]->Remove();
	}
	DEBUG_LOG("Aura","Removing aura %u",spellId);
}

void WorldSession::HandleCancelChannellingOpcode( WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	PlayerPointer plyr = GetPlayer();
	if(!plyr)
		return;
	if(plyr->m_currentSpell)
	{		
		plyr->m_currentSpell->cancel();
	}
}

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& recv_data)
{
	_player->m_onAutoShot = false;
}

void WorldSession::HandleAddDynamicTargetOpcode(WorldPacket & recvPacket)
{

	DEBUG_LOG( "WORLD"," got CMSG_PET_CAST_SPELL." );
	uint64 guid;
	uint8 counter;
	uint32 spellid;
	uint8 flags;
	UnitPointer caster;
	SpellCastTargets targets;
	SpellEntry *sp;
	SpellPointer pSpell;
	list<AI_Spell*>::iterator itr;

	recvPacket >> guid >> counter >> spellid >> flags;
	sp = dbcSpell.LookupEntry(spellid);

	// Summoned Elemental's Freeze
    if (spellid == 33395)
	{
		caster = _player->m_Summon;
		if( caster && TO_PET(caster)->GetAISpellForSpellId(spellid) == NULL )
			return;
	}
	else
	{
		caster = _player->m_CurrentCharm;
		if( caster != NULL )
		{
			for(itr = caster->GetAIInterface()->m_spells.begin(); itr != caster->GetAIInterface()->m_spells.end(); ++itr)
			{
				if( (*itr)->spell->Id == spellid )
					break;
			}

			if( itr == caster->GetAIInterface()->m_spells.end() )
				return;
		}
	}

	if( caster == NULL || guid != caster->GetGUID() )
		return;
	
	targets.read(recvPacket, _player->GetGUID());

	pSpell = SpellPointer(new Spell(caster, sp, false, NULLAURA));
	pSpell->prepare(&targets);
}
