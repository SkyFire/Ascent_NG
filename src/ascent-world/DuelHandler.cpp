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

void WorldSession::HandleDuelAccepted(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	if( _player->DuelingWith == NULL )
		return;

	if( _player->m_duelState != DUEL_STATE_FINISHED )
		return;
	
	if( _player->m_duelCountdownTimer > 0 )
		return;

	_player->m_duelStatus = DUEL_STATUS_INBOUNDS;
	_player->DuelingWith->m_duelStatus = DUEL_STATUS_INBOUNDS;

	_player->m_duelState = DUEL_STATE_STARTED;
	_player->DuelingWith->m_duelState = DUEL_STATE_STARTED;

	WorldPacket data( SMSG_DUEL_COUNTDOWN, 4 );
	data << uint32( 3000 );

	SendPacket( &data );
	_player->DuelingWith->m_session->SendPacket( &data );

	_player->m_duelCountdownTimer = 3000;

	sEventMgr.AddEvent(_player, &Player::DuelCountdown, EVENT_PLAYER_DUEL_COUNTDOWN, 1000, 3,0);
}

void WorldSession::HandleDuelCancelled(WorldPacket & recv_data)
{
	if( _player->DuelingWith ==  NULL )
		return;

	if( _player->m_duelState == DUEL_STATE_STARTED )
	{
		_player->DuelingWith->EndDuel( DUEL_WINNER_KNOCKOUT );
		return;
	}

	WorldPacket data( SMSG_DUEL_COMPLETE, 1 );
	data << uint8( 1 );

	SendPacket( &data );
	_player->DuelingWith->m_session->SendPacket( &data );

	GameObject* arbiter = _player->GetMapMgr() ? _player->GetMapMgr()->GetGameObject( GET_LOWGUID_PART(_player->GetUInt64Value( PLAYER_DUEL_ARBITER )) ) : NULL;
	if( arbiter != NULL )
	{
		arbiter->RemoveFromWorld( true );
		arbiter->Destructor();
 	}

	_player->DuelingWith->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
	_player->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );

	_player->DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
	_player->SetUInt32Value( PLAYER_DUEL_TEAM, 0);

	_player->DuelingWith->m_duelState = DUEL_STATE_FINISHED;
	_player->m_duelState = DUEL_STATE_FINISHED;

	_player->DuelingWith->m_duelCountdownTimer = 0;
	_player->m_duelCountdownTimer = 0;

	_player->DuelingWith->DuelingWith = NULL;
	_player->DuelingWith = NULL;

}
