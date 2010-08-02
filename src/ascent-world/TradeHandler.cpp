/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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

void WorldSession::HandleInitiateTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;
	Player * pTarget = _player->GetMapMgr()->GetPlayer((uint32)guid);
	uint32 TradeStatus = TRADE_STATUS_PROPOSED;
	WorldPacket data(SMSG_TRADE_STATUS, 12);

	if(pTarget == 0)
	{
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
		OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
		return;
	}

	// Handle possible error outcomes
	if(pTarget->CalcDistance(_player) > 10.0f)		// This needs to be checked
		TradeStatus = TRADE_STATUS_TOO_FAR_AWAY;
	else if(pTarget->isDead())
		TradeStatus = TRADE_STATUS_DEAD;
	else if(pTarget->mTradeTarget != 0)
		TradeStatus = TRADE_STATUS_ALREADY_TRADING;
	else if(pTarget->GetTeam() != _player->GetTeam() && GetPermissionCount() == 0)
		TradeStatus = TRADE_STATUS_WRONG_FACTION;

	data << TradeStatus;

	if(TradeStatus == TRADE_STATUS_PROPOSED)
	{
		_player->ResetTradeVariables();
		pTarget->ResetTradeVariables();

		pTarget->mTradeTarget = _player->GetLowGUID();
		_player->mTradeTarget = pTarget->GetLowGUID();

		pTarget->mTradeStatus = TradeStatus;
		_player->mTradeStatus = TradeStatus;

		data << _player->GetGUID();
	}

	pTarget->m_session->SendPacket(&data);
}

void WorldSession::HandleBeginTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 TradeStatus = TRADE_STATUS_INITIATED;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
		OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
		return;
	}
	// We're too far from target now?
	if( _player->CalcDistance( objmgr.GetPlayer(_player->mTradeTarget) ) > 10.0f )
	TradeStatus = TRADE_STATUS_TOO_FAR_AWAY;

	WorldPacket data(SMSG_TRADE_STATUS, 8);
	data << TradeStatus << uint32(0x19);
	plr->m_session->SendPacket(&data);
	SendPacket(&data);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;
}

void WorldSession::HandleBusyTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 TradeStatus = TRADE_STATUS_PLAYER_BUSY;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
		OutPacket(TRADE_STATUS_PLAYER_NOT_FOUND, 4, &TradeStatus);
		return;
	}

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
}

void WorldSession::HandleIgnoreTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 TradeStatus = TRADE_STATUS_PLAYER_IGNORED;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
		OutPacket(TRADE_STATUS_PLAYER_NOT_FOUND, 4, &TradeStatus);
		return;
	}

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
}

void WorldSession::HandleCancelTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	if(_player->mTradeTarget == 0 || _player->mTradeStatus == TRADE_STATUS_COMPLETE)
		return;

    uint32 TradeStatus = TRADE_STATUS_CANCELLED;
    OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	Player * plr = _player->GetTradeTarget();
    if(plr)
    {
        if(plr->m_session && plr->m_session->GetSocket())
		plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	
	    plr->mTradeTarget = 0;
		plr->ResetTradeVariables();
    }
	
	_player->mTradeTarget = 0;
	_player->ResetTradeVariables();
}

void WorldSession::HandleUnacceptTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	Player * plr = _player->GetTradeTarget();
	_player->ResetTradeVariables();

	if(_player->mTradeTarget == 0 || plr == 0)
		return;

	uint32 TradeStatus = TRADE_STATUS_UNACCEPTED;
	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
	plr->ResetTradeVariables();
}

void WorldSession::HandleSetTradeItem(WorldPacket & recv_data)
{
	if(_player->mTradeTarget == 0)
		return;

	CHECK_PACKET_SIZE(recv_data, 3);

	uint8 TradeSlot = recv_data.contents()[0];
	int8 SourceBag = recv_data.contents()[1];
	uint8 SourceSlot = recv_data.contents()[2];
	Player * pTarget = _player->GetMapMgr()->GetPlayer( _player->mTradeTarget );

	Item * pItem = _player->GetItemInterface()->GetInventoryItem(SourceBag, SourceSlot);
	if( pTarget == NULL || pItem == 0 || TradeSlot > 6 || ( TradeSlot < 6 && pItem->IsSoulbound() ) )
		return;

	// cheatproof
	if( SourceBag == INVENTORY_SLOT_NOT_SET && SourceSlot < INVENTORY_SLOT_BAG_END && SourceSlot >= INVENTORY_SLOT_BAG_START )
		return;

	if( pItem->GetProto()->ContainerSlots > 0 && ((Container*)pItem)->HasItems() )
		return;

	if(TradeSlot < 6 && pItem->IsSoulbound())
	{
		// client is obviously cheating
		Disconnect();
		return;
	}

	for(uint32 i = 0; i < 8; ++i)
	{
		// duping little shits
		if(_player->mTradeItems[i] == pItem || pTarget->mTradeItems[i] == pItem)
		{
			// cheating client
			Disconnect();
			return;
		}
	}

	_player->mTradeItems[TradeSlot] = pItem;
	_player->SendTradeUpdate();
}

void WorldSession::HandleSetTradeGold(WorldPacket & recv_data)
{
	if(_player->mTradeTarget == 0)
		return;

	uint32 Gold;
	recv_data >> Gold;

	if(_player->mTradeGold != Gold)
	{
		_player->mTradeGold = (Gold > _player->GetUInt32Value(PLAYER_FIELD_COINAGE) ? _player->GetUInt32Value(PLAYER_FIELD_COINAGE) : Gold);
		_player->SendTradeUpdate();
	}
}

void WorldSession::HandleClearTradeItem(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);
	if(_player->mTradeTarget == 0)
		return;

	uint8 TradeSlot = recv_data.contents()[0];
	if(TradeSlot > 6)
		return;

	_player->mTradeItems[TradeSlot] = 0;
	_player->SendTradeUpdate();
}

void WorldSession::HandleAcceptTrade(WorldPacket & recv_data)
{
	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || !plr)
		return;

	uint32 TradeStatus = TRADE_STATUS_ACCEPTED;
	
	// Tell the other player we're green.
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	_player->mTradeStatus = TradeStatus;

	if(plr->mTradeStatus == TRADE_STATUS_ACCEPTED)
	{
		// Ready!
		uint32 ItemCount = 0;
		uint32 TargetItemCount = 0;
		Player * pTarget = plr;

		// Calculate Item Count
		for(uint32 Index = 0; Index < 7; ++Index)
		{
			if(_player->mTradeItems[Index] != 0)	++ItemCount;
			if(pTarget->mTradeItems[Index] != 0)	++TargetItemCount;
		}

		if( (_player->m_ItemInterface->CalculateFreeSlots(NULL) + ItemCount) < TargetItemCount ||
			(pTarget->m_ItemInterface->CalculateFreeSlots(NULL) + TargetItemCount) < ItemCount )
		{
			// Not enough slots on one end.
			TradeStatus = TRADE_STATUS_CANCELLED;
		}
		else
		{
			TradeStatus = TRADE_STATUS_COMPLETE;
			uint64 Guid;
			Item * pItem;
			
			// Remove all items from the players inventory
			for(uint32 Index = 0; Index < 6; ++Index)
			{
				Guid = _player->mTradeItems[Index] ? _player->mTradeItems[Index]->GetGUID() : 0;
				if(Guid != 0)
				{
					if( _player->mTradeItems[Index]->IsSoulbound() || ( _player->mTradeItems[Index]->GetProto()->ContainerSlots > 0 && ((Container*)_player->mTradeItems[Index])->HasItems() ) )
					{
						_player->mTradeItems[Index] = NULL;
					}
					else
					{
						if(GetPermissionCount()>0)
						{
							sGMLog.writefromsession(this, "traded item %s to %s", _player->mTradeItems[Index]->GetProto()->Name1, pTarget->GetName());
						}
						pItem = _player->m_ItemInterface->SafeRemoveAndRetreiveItemByGuidRemoveStats(Guid, true);
					}
				}

				Guid = pTarget->mTradeItems[Index] ? pTarget->mTradeItems[Index]->GetGUID() : 0;
				if(Guid != 0)
				{
					if( pTarget->mTradeItems[Index]->IsSoulbound() || ( pTarget->mTradeItems[Index]->GetProto()->ContainerSlots > 0 && ((Container*)pTarget->mTradeItems[Index])->HasItems() ) )
					{
						pTarget->mTradeItems[Index] = NULL;
					}
					else
					{
						pTarget->m_ItemInterface->SafeRemoveAndRetreiveItemByGuidRemoveStats(Guid, true);
					}
				}
			}

			// Dump all items back into the opposite players inventory
			for(uint32 Index = 0; Index < 6; ++Index)
			{
				pItem = _player->mTradeItems[Index];
				if(pItem != 0)
				{
					pItem->SetOwner(pTarget);
					if( !pTarget->m_ItemInterface->AddItemToFreeSlot(pItem) )
						delete pItem;
				}

				pItem = pTarget->mTradeItems[Index];
				if(pItem != 0)
				{
					pItem->SetOwner(_player);
					if( !_player->m_ItemInterface->AddItemToFreeSlot(pItem) )
						delete pItem;
				}
			}

			// Trade Gold
			if(pTarget->mTradeGold)
			{
				_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, pTarget->mTradeGold);
				pTarget->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)pTarget->mTradeGold);
			}

			if(_player->mTradeGold)
			{
				pTarget->ModUnsigned32Value(PLAYER_FIELD_COINAGE, _player->mTradeGold);
				_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)_player->mTradeGold);
			}

			// Close Window
			TradeStatus = TRADE_STATUS_COMPLETE;
			OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

			_player->mTradeStatus = TRADE_STATUS_COMPLETE;
			plr->mTradeStatus = TRADE_STATUS_COMPLETE;

			// Reset Trade Vars
			_player->ResetTradeVariables();
			pTarget->ResetTradeVariables();
			
			plr->mTradeTarget = 0;
			_player->mTradeTarget = 0;
		}
	}
}
