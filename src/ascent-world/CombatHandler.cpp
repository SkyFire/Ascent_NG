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

void WorldSession::HandleAttackSwingOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	if(!guid)
	{
		// does this mean cancel combat?
		HandleAttackStopOpcode(recv_data);
		return;
	}

	// AttackSwing
	DEBUG_LOG( "WORLD"," Recvd CMSG_ATTACKSWING Message" );

	if(GetPlayer()->IsPacified() || GetPlayer()->IsStunned() || GetPlayer()->IsFeared())
		return;

//	printf("Got ATTACK SWING: %08X %08X\n", GUID_HIPART(guid), GUID_LOPART(guid));
	Unit* pEnemy = _player->GetMapMgr()->GetUnit(guid);
	//printf("Pointer: %08X\n", pEnemy);

	if(!pEnemy)
	{
		OUT_DEBUG("WORLD: "I64FMT" does not exist.", guid);
		return;
	}

	if(pEnemy->isDead() || _player->isDead())		// haxors :(
		return;

	// Faction "Hack" by Deathshit
	if( !isAttackable( GetPlayer(), pEnemy, false ) && !pEnemy->IsInRangeOppFactSet(_player) && !pEnemy->CombatStatus.DidDamageTo(GetPlayer()->GetGUID()))
	{
		GetPlayer()->BroadcastMessage("Faction exploit detected. You will be disconnected in 5 seconds.");
		GetPlayer()->Kick(5000);
		return;
	}

	GetPlayer()->smsg_AttackStart(pEnemy);
	GetPlayer()->EventAttackStart();

	// Set PVP Flag.
	/*if(pEnemy->IsPlayer() && isHostile(_player, pEnemy))
	{
		// don't in duel.. this should be done in dealdamage anyway :S
		if( TO_PLAYER( pEnemy )->GetTeam() != _player->GetTeam() )
			_player->SetPvPFlag();
	}*/
}

void WorldSession::HandleAttackStopOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	uint64 guid = GetPlayer()->GetSelection();
	Unit* pEnemy = NULL;

	if(guid)
	{
		pEnemy = _player->GetMapMgr()->GetUnit(guid);
		if(pEnemy)
		{
			GetPlayer()->EventAttackStop();
			GetPlayer()->smsg_AttackStop(pEnemy);

			/*WorldPacket data(SMSG_ATTACKSTOP, 20);
			data << _player->GetNewGUID();
			data << uint8(0);
			data << uint32(0);
			SendPacket(&data);*/
		}
	}
}

