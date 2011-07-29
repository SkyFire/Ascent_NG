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

Vehicle::Vehicle(uint64 guid) : Creature(guid)
{
	m_vehicleEntry = 0;
	m_passengerCount = 0;
	m_maxPassengers = 0;
	m_isVehicle = true;
	Initialised = false;
	m_CreatedFromSpell = false;
	m_mountSpell = 0;
	for(uint8 i = 0; i < 8; ++i)
	{
		m_vehicleSeats[i] = NULL;
		m_passengers[i] = NULL;
	}
}

Vehicle::~Vehicle()
{
	RemoveAllPassengers();

	m_passengerCount = 0;
	if( IsInWorld() )
		RemoveFromWorld(false, true);
}

void Vehicle::InitSeats(uint32 vehicleEntry, Player* pRider)
{
	m_maxPassengers = 0;

	SetVehicleEntry(vehicleEntry);

	VehicleEntry * ve = dbcVehicle.LookupEntry( vehicleEntry );
	if(!ve)
	{
		Log.Warning("Vehicles", "Attempted to create vehicle %u with invalid vehicle_entry, check your creature_proto table.\n", proto->Id);
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( ve->m_seatID[i] )
		{
			m_vehicleSeats[i] = dbcVehicleSeat.LookupEntry( ve->m_seatID[i] );
			if(m_vehicleSeats && m_vehicleSeats[i]->IsUsable())
				++m_maxPassengers;
		}
	}

	Initialised = true;

	if( pRider != NULL )
		AddPassenger( pRider );
}

void Vehicle::Load(CreatureProto * proto_, float x, float y, float z, float o /* = 0.0f */)
{
	proto = proto_;
	if(!proto)
		return;

	InitSeats(proto->vehicle_entry);

	Creature::Load(proto_, x, y, z, o);
}

bool Vehicle::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;

	InitSeats(proto->vehicle_entry);

	return Creature::Load(spawn, mode, info);
}

//-----------------------------------------------------------------
//This function builds and send the packets required to add spells
//to a vehicle that can be cast by its controlling unit
//-----------------------------------------------------------------
void Vehicle::SendSpells(uint32 entry, Player* plr)
{

	//Vehicles spells are set in the AI_Agents table
	//in the world database these are cached in
	//AIInterface so we will set up an temporary
	//list and an iterator, then iterate over the
	//the AI spell list adding any spells that can
	//be cast by the controlling unit on the vehicle
	//to our temporary list. These are spells of
	//type 4 from enum AI_Agent::AGENT_SPELL
	list<uint32> avail_spells;
	for(list<AI_Spell*>::iterator itr = GetAIInterface()->m_spells.begin(); itr != GetAIInterface()->m_spells.end(); ++itr)
	{
		if((*itr)->agent == AGENT_SPELL)
			avail_spells.push_back((*itr)->spell->Id);
	}
	//set the iterator to our temporary list
	//and position it at the start
	list<uint32>::iterator itr = avail_spells.begin();

	//Now we can build the packet to send these spells
	//to the vehicle, this uses the pet spell packet
	WorldPacket data(SMSG_PET_SPELLS, 8+4+4+1+1+2+4*10+1+1);
	data << uint64(GetGUID());
	data << uint32(GetCreatureInfo()->Family);
	data << uint32(0x00000000);//unk
	data << uint16(0x0101);	//unk2

	//iterate over the temporary list
	//and add the spells to the packet
	for(uint8 i = 1; i < 10; ++i)
	{
		if(itr != avail_spells.end())
		{
			data << uint16((*itr)) << uint8(0) << uint8(i+8);
			++itr;
		}
		else
			data << uint16(0) << uint8(0) << uint8(i+8);
	}

	//count of spells to follow, these are spells
	//which if we were sending to a pet would have
	//been added to the players spell book, so
	//obviously we don't want to do that for a
	//vehicle
	data << uint8(0);			
	data << uint8(0);			//count of spell cool down times to follow

	plr->GetSession()->SendPacket(&data);

	data.clear();
	data.SetOpcode(SMSG_PET_GUIDS);
	data << uint32(1); // count
	data << uint64(GetGUID());
	plr->GetSession()->SendPacket(&data);
}

void Vehicle::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(this, &Vehicle::Despawn, (uint32)0, respawntime, EVENT_VEHICLE_RESPAWN, delay, 1,0);
		return;
	}

	if(!IsInWorld())
		return;

	if(respawntime)
	{
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(!pCell)
			pCell = m_mapCell;

		if(!pCell)
		{
			ASSERT(pCell);
			return;
		}

		pCell->_respawnObjects.insert(static_cast<Object*>(this));

		sEventMgr.RemoveEvents(this);

		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnVehicle, this, pCell, EVENT_VEHICLE_RESPAWN, respawntime, 1, 0);

		Unit::RemoveFromWorld(false);

		m_position = m_spawnLocation;

		m_respawnCell=pCell;
	}
	else
	{
		Creature::RemoveFromWorld(false, true);
	}
}

void Vehicle::Update(uint32 time)
{
	Creature::Update(time);
}

void Vehicle::SafeDelete()
{
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(this, &Vehicle::DeleteMe, EVENT_VEHICLE_SAFE_DELETE, 1000, 1, EVENT_FLAG_DELETES_OBJECT);
}

void Vehicle::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	delete this;
}

void Vehicle::AddPassenger(Unit* pPassenger)
{
	if(!m_maxPassengers)
	{
		sLog.outError("Vehicle was not correctly initialized, retrying");
		InitSeats(m_vehicleEntry);
	}

	for(uint8 i = 0; i < 8; ++i)
	{
		if( !m_passengers[i] && m_vehicleSeats[i] && m_vehicleSeats[i]->IsUsable())
		{
			_AddToSlot(pPassenger, i );
			break;
		}
	}
}

uint8 Vehicle::GetPassengerSlot(Unit* pPassenger)
{
	for(uint8 i = 0; i < 8; ++i)
	{
		if( m_passengers[i] == pPassenger ) // Found a slot
		{
			return i;
			break;
		}
	}
	return 0xFF;
}

void Vehicle::RemovePassenger(Unit* pPassenger)
{
	//vehicle
	uint8 slot = pPassenger->m_inVehicleSeatId;
	pPassenger->m_CurrentVehicle = NULL;
	pPassenger->m_inVehicleSeatId = 0xFF;

	if(slot >= 8)
		return;

	//Remove the flag which was stopping units
	//from attacking us
	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

	//If the vehicle was created from a mount
	//spell, remove the aura
	if( pPassenger->IsPlayer() )
		pPassenger->RemoveAura(static_cast<Player*>(pPassenger)->m_MountSpellId);

	//If a spell was cast on us when we boarded
	//the vehicle, we must make sure it is removed
	if( m_mountSpell )
		pPassenger->RemoveAura( m_mountSpell );

	WorldPacket data(MSG_MOVE_HEARTBEAT, 48);
	data << pPassenger->GetNewGUID();
	data << uint32(MOVEFLAG_FLYING);
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	pPassenger->SendMessageToSet(&data, false);

	if(pPassenger->IsPlayer())
	{
		Player* plr = static_cast<Player*>(pPassenger);

		if(plr == GetControllingUnit())
		{
			plr->m_CurrentCharm = NULL;
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << (uint8)0;
			plr->GetSession()->SendPacket(&data);
		}

		RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_PLAYER_CONTROLLED_CREATURE | UNIT_FLAG_PLAYER_CONTROLLED));


		plr->SetPlayerStatus(TRANSFER_PENDING);

		//Set the new coordinates for the player
		//server side, so that we don't go out of sync
		plr->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
		plr->SetPosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());

		//Build a mighty packet which tells the
		//client we have left the vehicle and
		//we have teleported off it.
		SendMsgMoveTeleportAck(data, plr);

		//Clear the far sight field, this
		//will set our vision back to the
		//normal player radius
		plr->SetUInt64Value( PLAYER_FARSIGHT, 0 );	

		//Play the sound to the player
		//when they exit the vehicle
		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(m_vehicleSeats[slot]->m_exitUISoundID);
		data << plr->GetPosition();
		plr->GetSession()->SendPacket(&data);

		//Send an empty spells packet to
		//clear the action bar and give the
		//player their normal bar back
		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);
	}

	//Handle any work that needs to be done
	//if the controlling unit left, slot 0
	//denotes the passenger that controls,
	//moves, and casts spells
	if(slot == 0)
	{
		//Clear the redirection of spell cast
		//results, and send them to the vehicle
		//again, now that it is vacated the player
		//doesn't want to get these
		m_redirectSpellPackets = NULL;

		//Remove any attackers for now
		//so that there aren't any faction
		//conflicts, we will set the correct
		//faction later
		CombatStatus.Vanished();

		//Reset the current charm field
		pPassenger->SetUInt64Value( UNIT_FIELD_CHARM, 0 );
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

		//Set the faction back to the vehicles
		//original faction
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetCharmTempVal());
		
		//Now that we have set the correct
		//faction we can update it and allow
		//units to attack.
		_setFaction();
		UpdateOppFactionSet();

		//Clear out any attackers and targets
		//that we had before the controlling
		//unit left and give the vehicle its
		//brain back!
		GetAIInterface()->WipeHateList();
		GetAIInterface()->WipeTargetList();
		EnableAI();

		SetControllingUnit(NULL);

		RemoveAllPassengers();
	}

	m_passengers[slot] = NULL;
}

//-----------------------------------------------------------------
// This function checks if the vehicle has the passenger aboard
// that is passed as a parameter
//-----------------------------------------------------------------
bool Vehicle::HasPassenger(Unit* pPassenger)
{
	//Iterate over all possible slots in the vehicle
	for(uint8 i = 0; i < 8; ++i)
	{
		//If we find a slot that matches the pointer
		//to the passenger we were passed then return
		//true
		if( m_passengers[i] == pPassenger )
			return true;
	}
	//If we hit this then it means the passenger
	//is not in this vehicle
	return false;
}

void Vehicle::_AddToSlot(Unit* pPassenger, uint8 slot)
{
	if(slot >= 8 || slot > m_maxPassengers)
		return;
	if(!m_vehicleSeats[slot] || !GetAIInterface())
		return;

	if(m_passengers[slot] != NULL)
	{
		RemovePassenger(m_passengers[slot]);
	}

	if(pPassenger->m_CurrentVehicle != NULL)
		pPassenger->m_CurrentVehicle->RemovePassenger(pPassenger);

	m_passengers[ slot ] = pPassenger;

	pPassenger->m_inVehicleSeatId = slot;

	if( m_mountSpell )
		pPassenger->CastSpell( pPassenger, m_mountSpell, true );

	pPassenger->m_CurrentVehicle = this;

	if(slot == 0)
	{
		SetControllingUnit(pPassenger);
		CombatStatus.Vanished();
	}

	pPassenger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

	if( pPassenger->IsPlayer() )
	{
		Player* pPlayer = static_cast<Player*>(pPassenger);
		if(pPlayer->m_MountSpellId)
			pPlayer->RemoveAura(pPlayer->m_MountSpellId);

		if(pPlayer->GetSummon())
		{
			if(pPlayer->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				pPlayer->GetSummon()->Dismiss(false);
			else
				pPlayer->GetSummon()->Remove(false, true, true);
		}
		if(slot == 0)
		{
			DisableAI();
			m_redirectSpellPackets = pPlayer;
			pPlayer->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());

			SetSpeed(RUN, m_runSpeed);
			SetSpeed(FLY, m_flySpeed);

			WorldPacket data2(SMSG_CLIENT_CONTROL_UPDATE, 10);
			data2 << GetNewGUID() << uint8(1);
			pPlayer->GetSession()->SendPacket(&data2);

			pPlayer->m_CurrentCharm = this;
			pPlayer->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());

			SetCharmTempVal(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt64Value(UNIT_FIELD_CHARMEDBY, pPlayer->GetGUID());

			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

			_setFaction();
			UpdateOppFactionSet();

			SendSpells(GetEntry(), pPlayer);
		}
	}
	SendMonsterMoveTransport(pPassenger, slot);
}

void Vehicle::MoveVehicle(float x, float y, float z, float o) 
{
	SetPosition(x, y, z, o, false);

	float passengerX;
	float passengerY;
	float passengerZ;

	for (uint32 i = 0; i < 8; ++i)
	{
		if (m_passengers[i] != NULL)
		{
			passengerX = x + m_vehicleSeats[i]->m_attachmentOffsetX;
			passengerY = y + m_vehicleSeats[i]->m_attachmentOffsetY;
			passengerZ = z + m_vehicleSeats[i]->m_attachmentOffsetZ;

			m_passengers[i]->SetPosition(passengerX,passengerY,passengerZ,o,false);
		}
	}
}

void Vehicle::setDeathState(DeathState s)
{
	RemoveAllPassengers();

	if( s == JUST_DIED && m_CreatedFromSpell)
		SafeDelete();
	else
		Creature::setDeathState(s);
}

void Vehicle::SetSpeed(PlayerSpeedType SpeedType, float value)
{
	if( value < 0.1f )
		value = 0.1f;

	uint8 buf[200];
	StackPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, buf, 200);

	data << GetNewGUID();
	data << uint32(0);

	switch(SpeedType)
	{
	case RUN:
		{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			data << uint8(1);
			m_runSpeed = value;
		}break;
	case RUNBACK:
		{
			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
		}break;
	case SWIM:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
		}break;
	case SWIMBACK:
		{
			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			data << uint8(0);
			data << uint32(getMSTime());
			data << m_position.x;
			data << m_position.y;
			data << m_position.z;
			data << m_position.o;
			data << uint32(0);
			m_backSwimSpeed = value;
		}break;
	case FLY:
		{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
		}break;
	default:
		return;
		break;
	}

	data << value;

	SendMessageToSet(&data , true);
}

void Vehicle::RemoveAllPassengers()
{
	for(uint32 i = 0; i < 8; ++i)
	{
		if(m_passengers[i] != NULL)
			RemovePassenger(m_passengers[i]);
	}
}

void Vehicle::SendMonsterMoveTransport( Unit* pPassenger, uint8 slot )
{
	WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 60);
	data << pPassenger->GetNewGUID();
	data << GetNewGUID();
	data << int8(slot);
	data << uint8(0);                                       // 3.1
	data << GetPositionX() - pPassenger->GetPositionX();
	data << GetPositionY() - pPassenger->GetPositionY();
	data << GetPositionZ() - pPassenger->GetPositionZ();
	data << uint32(getMSTime());

	data << uint8(4);                                       // unknown
	data << float(0);                                       // facing angle

	data << uint32(0x800000);

	data << uint32(0);                                      // Time in between points
	data << uint32(1);										// number of way points
	data << m_vehicleSeats[slot]->m_attachmentOffsetX;
	data << m_vehicleSeats[slot]->m_attachmentOffsetY;
	data << m_vehicleSeats[slot]->m_attachmentOffsetZ;
	SendMessageToSet(&data, true);
}

void Vehicle::SendMsgMoveTeleportAck( WorldPacket &data, Player* plr )
{
	data.Initialize(MSG_MOVE_TELEPORT_ACK);
	data << plr->GetNewGUID();
	data << plr->m_teleportAckCounter++;
	data << uint32(MOVEFLAG_FLYING);
	data << uint16(0x40);
	data << getMSTime();
	data << GetPositionX();
	data << GetPositionY();
	data << GetPositionZ();
	data << GetOrientation();
	data << uint32(0);
	plr->GetSession()->SendPacket(&data);
}

void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	if(GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle == NULL)
		return;

	GetPlayer()->m_CurrentVehicle->RemovePassenger(GetPlayer());
}

void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{

	if(GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle || GetPlayer()->GetMapMgr() == NULL)
		return;

	CHECK_PACKET_SIZE(recv_data, 8);

	uint64 guid = 0;
	recv_data >> guid;

	Vehicle* pVehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));
	if(!pVehicle)
		return;

	if(pVehicle->HasPassenger(_player))
		pVehicle->RemovePassenger(_player);
	pVehicle->AddPassenger(_player);
}