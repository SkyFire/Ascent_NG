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
	setVehicleEntry(0);
	setPassengerCount(0);
	setMaxPassengers(0);
	m_isVehicle = true;
	setInitializedStatus(false);
	setCreatedFromSpell(false);
	setMountSpell(0);
}

Vehicle::~Vehicle()
{	
	setPassengerCount(0);

	for(vector<Unit*>::iterator Itr = passengers.begin(); Itr != passengers.end(); ++Itr)
		RemovePassenger( (*Itr) );

	if( IsInWorld() )
		RemoveFromWorld(false, true);
}

void Vehicle::Destructor()
{
	delete this;
}

void Vehicle::Init()
{
	Creature::Init();
}

void Vehicle::InitSeats(uint32 vehicleEntry, Player* pRider)
{
	setMaxPassengers(0);
	setVehicleEntry(vehicleEntry);
	VehicleEntry * ve = dbcVehicle.LookupEntry( vehicleEntry );
	if(!ve)
	{
		OUT_DEBUG("Attempted to create non-existant vehicle %u.\n", vehicleEntry);
		return;
	}

	for( uint32 i = 0; i < 8; ++i )
	{
		if( ve->m_seatID[i] )
		{
			vehicleSeats.push_back( dbcVehicleSeat.LookupEntry( ve->m_seatID[i] ) );
			++maxPassengers;
		}
	}

	for( uint8 i = 0; i < getMaxPassengerCount(); ++i )
		passengers.push_back(NULL);

	setInitializedStatus(true);

	if( pRider != NULL)
		AddPassenger( pRider );
}

void Vehicle::Load(CreatureProto * proto_, float x, float y, float z, float o /* = 0.0f */)
{
	proto = proto_;
	if(!proto)
		return;

	if(proto->vehicle_entry != -1)
		setVehicleEntry(proto->vehicle_entry);
	else
	{
		setVehicleEntry(124);
		OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.\n", proto->Id);
	}

	InitSeats(vehicleEntry);

	Creature::Load(proto_, x, y, z, o);
}

bool Vehicle::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;

	if(proto->vehicle_entry != -1)
	{
		setVehicleEntry(proto->vehicle_entry);
	}
	else
	{
		setVehicleEntry(124);
		OUT_DEBUG("Attempted to create vehicle %u with invalid vehicle_entry, defaulting to 124, check your creature_proto table.\n", proto->Id);
	}

	InitSeats(vehicleEntry);

	return Creature::Load(spawn, mode, info);
}

void Vehicle::SendSpells(uint32 entry, Player* plr)
{
	list<uint32> avail_spells;
	for(list<AI_Spell*>::iterator itr = GetAIInterface()->m_spells.begin(); itr != GetAIInterface()->m_spells.end(); ++itr)
	{
		if((*itr)->agent == AGENT_SPELL)
			avail_spells.push_back((*itr)->spell->Id);
	}
	list<uint32>::iterator itr = avail_spells.begin();
	
	WorldPacket data(SMSG_PET_SPELLS, GetAIInterface()->m_spells.size() * 4 + 22);
	data << GetGUID();
	data << uint16(0x0000);//pet family
	data << uint32(0x00000000);//unk
	data << uint32(0x00000101);//bar type

	// Send the actionbar
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

	// Send the rest of the spells.
	data << uint8(avail_spells.size());
	for(itr = avail_spells.begin(); itr != avail_spells.end(); ++itr)
		data << uint16(*itr) << uint16(DEFAULT_SPELL_STATE);
	
	data << uint8(0);

	plr->GetSession()->SendPacket(&data);
}

void Vehicle::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::Despawn, (uint32)0, respawntime, EVENT_VEHICLE_RESPAWN, delay, 1,0);
		return;
	}

	if(!IsInWorld())
		return;

	if(respawntime)
	{
		/* get the cell with our SPAWN location. if we've moved cell this might break :P */
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(!pCell)
			pCell = m_mapCell;
	
		ASSERT(pCell);
		pCell->_respawnObjects.insert(TO_OBJECT(this));
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnVehicle, TO_VEHICLE(this), pCell, EVENT_VEHICLE_RESPAWN, respawntime, 1, 0);
		Unit::RemoveFromWorld(false);
		m_position = m_spawnLocation;
		m_respawnCell=pCell;
	}
	else
	{
		Unit::RemoveFromWorld(true);
		SafeDelete();
	}
}

void Vehicle::Update(uint32 time)
{
	Creature::Update(time);
}

void Vehicle::SafeDelete()
{
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(TO_VEHICLE(this), &Vehicle::DeleteMe, EVENT_VEHICLE_SAFE_DELETE, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Vehicle::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	Destructor();
}

void Vehicle::AddPassenger(Unit* pPassenger)
{
	if(!getMaxPassengerCount()) //how is this happening?
	{
		sLog.outColor(TRED, "Vehicle was not correctly initialised, retrying");
		sLog.outColor(TNORMAL, "\n");
		InitSeats(vehicleEntry);
	}

	// Find an available seat
	for(uint8 i = 0; i < getMaxPassengerCount(); ++i)
	{
		if( !passengers[i] ) // Found a slot
		{
			AddToSlot(pPassenger, i );
			break;
		}
	}
}

uint8 Vehicle::getPassengerSlot(Unit* pPassenger)
{
	for(vector<Unit*>::iterator Itr = passengers.begin(); Itr != passengers.end(); ++Itr)
	{
		if( (*Itr) == pPassenger )
		{
			return (*Itr)->m_inVehicleSeatId;
			break;
		}
	}
	return 0xFF;
}

VehicleSeatEntry* Vehicle::getVehicleSeat(uint8 slotId)
{
	return vehicleSeats.at(slotId);
}

void Vehicle::RemovePassenger(Unit* pPassenger)
{
	uint8 slot = pPassenger->m_inVehicleSeatId;
	if(slot >= 8)
		return;
	pPassenger->m_CurrentVehicle = NULL;
	pPassenger->m_inVehicleSeatId = 0xFF;

	pPassenger->RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION));
	if( pPassenger->IsPlayer() )
		pPassenger->RemoveAura(TO_PLAYER(pPassenger)->m_MountSpellId);

	if( getMountSpell() )
		pPassenger->RemoveAura( getMountSpell() );

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
		Player* plr = TO_PLAYER(pPassenger);
		if(plr == getControllingUnit())
		{
			plr->m_CurrentCharm = NULL;
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << (uint8)0;
			plr->GetSession()->SendPacket(&data);
		}
		RemoveFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_PLAYER_CONTROLLED_CREATURE | UNIT_FLAG_PLAYER_CONTROLLED));

		plr->SetPlayerStatus(TRANSFER_PENDING);
		plr->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());
		plr->SetPosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());
		data.Initialize(MSG_MOVE_TELEPORT_ACK);
		data << plr->GetNewGUID();
		data << plr->m_teleportAckCounter;
		plr->m_teleportAckCounter++;
		data << uint32(MOVEFLAG_FLYING);
		data << uint16(0x40);
		data << getMSTime();
		data << GetPositionX();
		data << GetPositionY();
		data << GetPositionZ();
		data << GetOrientation();
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);

		plr->SetUInt64Value( PLAYER_FARSIGHT, 0 );	

		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(vehicleSeats[slot]->m_exitUISoundID);
		data << plr->GetPosition();
		plr->GetSession()->SendPacket(&data);

		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		data << uint32(0);
		plr->GetSession()->SendPacket(&data);
	}
		
	if(slot == 0)
	{
		m_redirectSpellPackets = NULL;
		CombatStatus.Vanished();
		pPassenger->SetUInt64Value( UNIT_FIELD_CHARM, 0 );
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetCharmTempVal());
		SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
		/* update target faction set */
		_setFaction();
		UpdateOppFactionSet();

		GetAIInterface()->WipeHateList();
		GetAIInterface()->WipeTargetList();
		EnableAI();

		setControllingUnit(NULL);
		
		for(vector<Unit*>::iterator Itr = passengers.begin(); Itr != passengers.end(); ++Itr)
			RemovePassenger( (*Itr) );
	}

	passengers[slot] = NULL;
}

bool Vehicle::HasPassenger(Unit* pPassenger)
{
	for(vector<Unit*>::iterator Itr = passengers.begin(); Itr != passengers.end(); ++Itr)	
	{
		if( (*Itr) == pPassenger )
			return true;
	}

	return false;
}

void Vehicle::AddToSlot(Unit* pPassenger, uint8 slot)
{
	assert( slot < getMaxPassengerCount() );
	passengers[ slot ] = pPassenger;
	pPassenger->m_inVehicleSeatId = slot;
	LocationVector v;
	v.x = vehicleSeats[slot]->m_attachmentOffsetX;
	v.y = vehicleSeats[slot]->m_attachmentOffsetY;
	v.z = vehicleSeats[slot]->m_attachmentOffsetZ;
	v.o = 0;

	if( getMountSpell() )
		pPassenger->CastSpell( pPassenger, getMountSpell(), true );

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SELF_RES);
	// This is where the real magic happens
	if( pPassenger->IsPlayer() )
	{
		Player* pPlayer = TO_PLAYER(pPassenger);

		if(pPlayer->m_CurrentCharm)
			return;

		//Dismount
		if(pPlayer->m_MountSpellId)
			pPlayer->RemoveAura(pPlayer->m_MountSpellId);
	
		//Remove morph spells
		if(pPlayer->GetUInt32Value(UNIT_FIELD_DISPLAYID)!= pPlayer->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
		{
			pPlayer->RemoveAllAurasOfType(SPELL_AURA_TRANSFORM);
			pPlayer->RemoveAllAurasOfType(SPELL_AURA_MOD_SHAPESHIFT);
		}

		//Dismiss any pets
		if(pPlayer->GetSummon())
		{
		if(pPlayer->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
			pPlayer->GetSummon()->Dismiss(false);				// warlock summon -> dismiss
		else
			pPlayer->GetSummon()->Remove(false, true, true);	// hunter pet -> just remove for later re-call
		}

		pPlayer->m_CurrentVehicle = TO_VEHICLE(this);

		pPlayer->SetFlag(UNIT_FIELD_FLAGS, (UNIT_FLAG_UNKNOWN_5 | UNIT_FLAG_PREPARATION));

		pPlayer->SetUInt64Value(PLAYER_FARSIGHT, GetGUID());

		pPlayer->SetPlayerStatus(TRANSFER_PENDING);
		pPlayer->m_sentTeleportPosition.ChangeCoords(GetPositionX(), GetPositionY(), GetPositionZ());

		DisableAI();

		WorldPacket data(64);

		if(slot == 0)
		{
			setControllingUnit(pPlayer);
			m_redirectSpellPackets = pPlayer;

			SetSpeed(RUN, m_runSpeed);
			SetSpeed(FLY, m_flySpeed);

			// send "switch mover" packet 
			data.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data << GetNewGUID() << uint8(1);
			pPlayer->GetSession()->SendPacket(&data);

			pPlayer->m_CurrentCharm = TO_VEHICLE(this);
			pPlayer->SetUInt64Value(UNIT_FIELD_CHARM, GetGUID());
			SetCharmTempVal(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			SetUInt64Value(UNIT_FIELD_CHARMEDBY, pPlayer->GetGUID());
			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

			/* update target faction set */
			_setFaction();
			UpdateOppFactionSet();

			SendSpells(GetEntry(), pPlayer);
		}

		data.Initialize(SMSG_PET_DISMISS_SOUND);
		data << uint32(vehicleSeats[slot]->m_enterUISoundID);
		data << pPlayer->GetPosition();
		pPlayer->GetSession()->SendPacket(&data);
	}
	WorldPacket data(SMSG_MONSTER_MOVE_TRANSPORT, 64);

	data << pPassenger->GetNewGUID();
	data << GetNewGUID();
	data << int8(slot);
	data << uint8(0);
	data << pPassenger->GetPositionX() - GetPositionX();
	data << pPassenger->GetPositionY() - GetPositionY();
	data << pPassenger->GetPositionZ() - GetPositionZ();
	data << uint32(getMSTime());
	data << uint8(4);
	data << v.o;
	data << uint32(0x800000);
	data << uint32(0); 
	data << uint32(v.x);
	data << uint32(v.y);
	data << uint32(v.z);

	SendMessageToSet(&data,true);
}

/* This function changes a vehicles position server side to
keep us in sync with the client, so that the vehicle doesn't
get dismissed because the server thinks its gone out of range
of its passengers*/
void Vehicle::MoveVehicle(float x, float y, float z, float o)
{
	SetPosition(x, y, z, o, false);
	
	for(vector<Unit*>::iterator Itr = passengers.begin(); Itr != passengers.end(); ++Itr)
		(*Itr)->SetPosition(x, y, z, o, false);
}

void Vehicle::setDeathState(DeathState s)
{
	Creature::setDeathState(s);

	for(vector<Unit*>::iterator Itr = passengers.begin(); Itr != passengers.end(); ++Itr)
		RemovePassenger( (*Itr) );

	if( s == JUST_DIED && isCreatedFromSpell() )
		SafeDelete();
}

/* To change a vehicles speed we must send a Force Speed
change packet to the client. SpeedType takes values from
the enum PlayerSpeedType located in Player.h
RUN	            = 1,
RUNBACK         = 2,
SWIM	        = 3,
SWIMBACK        = 4,
WALK	        = 5,
FLY	            = 6,
value is the new speed to set*/
void Vehicle::SetSpeed(uint8 SpeedType, float value)
{
	if( value < 0.1f )
		value = 0.1f;

	uint8 buf[200];
	StackPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, buf, 200);

	if( SpeedType != SWIMBACK )
	{
		data << GetNewGUID();
		data << uint32(0);
		if( SpeedType == RUN )
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << m_position.x;
		data << m_position.y;
		data << m_position.z;
		data << m_position.o;
		data << uint32(0);
		data << value;
	}
	
	switch(SpeedType)
	{
	case RUN:
		{
			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
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
			m_backSwimSpeed = value;
		}break;
	case FLY:
		{
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
		}break;
	default:return;
	}
	
	SendMessageToSet(&data , true);

}

/* This function handles the packet sent from the client when we
leave a vehicle, it removes us server side from our current
vehicle*/
void WorldSession::HandleVehicleDismiss(WorldPacket & recv_data)
{
	if (GetPlayer() == NULL || !GetPlayer()->m_CurrentVehicle)
		return;

	GetPlayer()->m_CurrentVehicle->RemovePassenger(GetPlayer());
}

/* This function handles the packet from the client which is
sent when we click on an npc with the flag UNIT_FLAG_SPELLCLICK
and checks if there is room for us then adds us as a passenger
to that vehicle*/
void WorldSession::HandleSpellClick( WorldPacket & recv_data )
{
	if (GetPlayer() == NULL || GetPlayer()->m_CurrentVehicle)
		return;

    CHECK_PACKET_SIZE(recv_data, 8);

    uint64 guid;
    recv_data >> guid;

	Vehicle* pVehicle = GetPlayer()->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));
	Unit* pPlayer = TO_UNIT(GetPlayer());

	if(!pVehicle)
		return;
	
	if(!pVehicle->getMaxPassengerCount())
		return;

	// just in case.
	if( sEventMgr.HasEvent( pVehicle, EVENT_VEHICLE_SAFE_DELETE ) )
		return;

	if(pVehicle->HasPassenger(pPlayer))
		pVehicle->RemovePassenger(pPlayer);

	pVehicle->AddPassenger(pPlayer);
}
