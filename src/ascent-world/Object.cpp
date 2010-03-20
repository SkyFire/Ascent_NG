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
#include "Unit.h"
using namespace std;

//#define DEG2RAD (M_PI/180.0)
#ifdef M_PI
#undef M_PI
#endif

#define M_PI		3.14159265358979323846f
#define M_H_PI		1.57079632679489661923f
#define M_Q_PI		0.785398163397448309615f

Object::Object() : m_position(0,0,0,0), m_spawnLocation(0,0,0,0)
{
	m_phaseAura = NULL;
	m_phaseMode = 1;
	m_mapId = 0;
	m_zoneId = 0;

	m_uint32Values = 0;
	m_objectUpdated = false;

	m_valuesCount = 0;

	//official Values
	m_walkSpeed = 2.5f;
	m_runSpeed = 8.0f;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;

	m_flySpeed = 7.0f;
	m_backFlySpeed = 4.5f;

	m_backWalkSpeed = 4.5f;	// this should really be named m_backRunSpeed
	m_swimSpeed = 4.722222f;
	m_backSwimSpeed = 2.5f;
	m_turnRate = 3.141593f;

	m_mapMgr = NULL;
	m_mapCell = 0;

	mSemaphoreTeleport = false;

	dynObj = NULL;

	m_faction = NULL;
	m_factionDBC = NULL;

	m_instanceId = 0;
	Active = false;
	m_inQueue = false;
	m_extensions = NULL;
	m_loadedFromDB = false;
	m_loot.gold = 0;
	m_looted = false;
	m_isVehicle = false;

	m_objectsInRange.clear();
	m_inRangePlayers.clear();
	m_oppFactsInRange.clear();
}

Object::~Object( )
{
	if(m_phaseAura)
	{
		m_phaseAura->Remove();
		m_phaseAura = NULL;
	}

	if(m_objectTypeId != TYPEID_ITEM)
		ASSERT(!m_inQueue);

	if (IsInWorld() && m_objectTypeId != TYPEID_ITEM && m_objectTypeId != TYPEID_CONTAINER)
	{
		RemoveFromWorld(false);
	}

	ClearInRangeSet();

	// for linux
	m_instanceId = -1;
	m_objectTypeId=TYPEID_UNUSED;

	if( m_extensions != NULL )
		delete m_extensions;

	sEventMgr.RemoveEvents(this);
}

void Object::Init()
{
}

void Object::Destructor()
{
	delete this;
}

void Object::SetPhase(int32 phase)
{
	m_phaseMode = phase;
}

void Object::EnablePhase(int32 phaseMode)
{
	m_phaseMode |= phaseMode;
}

void Object::DisablePhase(int32 phaseMode)
{
	m_phaseMode &= ~phaseMode;
}

void Object::_Create( uint32 mapid, float x, float y, float z, float ang )
{
	m_mapId = mapid;
	m_position.ChangeCoords(x, y, z, ang);
	m_spawnLocation.ChangeCoords(x, y, z, ang);
	m_lastMapUpdatePosition.ChangeCoords(x,y,z,ang);
}

uint32 Object::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player* target)
{
	uint16 flags = 0;
	uint32 flags2 = 0;

	uint8 updatetype = UPDATETYPE_CREATE_OBJECT;
	if(m_objectTypeId == TYPEID_CORPSE)
	{
		if(m_uint32Values[CORPSE_FIELD_DISPLAY_ID] == 0)
			return 0;
	}

	// any other case
	switch(m_objectTypeId)
	{
		// items + containers: 0x8
	case TYPEID_ITEM:
	case TYPEID_CONTAINER:
		{
			flags = 0x0010;
		}break;
		
		// player/unit: 0x0070 (except self)
	case TYPEID_UNIT:
		{
			flags = 0x0070;
		}break;

	case TYPEID_PLAYER:
		{
			flags = 0x0070;
		}break;

		// gameobjects and dynamicobjects
	case TYPEID_GAMEOBJECT:
		{
			flags = 0x0350;//200|100|40|10

			switch(GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID))
			{
			case GAMEOBJECT_TYPE_MO_TRANSPORT:  
				{
					if(GetTypeFromGUID() != HIGHGUID_TYPE_TRANSPORTER)
						return 0;   // bad transporter
					else
						flags |= 0x0002;
				}break;

			case GAMEOBJECT_TYPE_TRANSPORT:
				{
					/* deeprun tram, etc */
					flags |= 0x0002;
				}break;

			case GAMEOBJECT_TYPE_DUEL_ARBITER:
				{
					// duel flags have to stay as updatetype 3, otherwise
					// it won't animate
					updatetype = UPDATETYPE_CREATE_YOURSELF;
				}break;
			}
		}break;

	case TYPEID_DYNAMICOBJECT:
		{
			flags = 0x0150;
		}break;

	case TYPEID_CORPSE:
		{
			flags = 0x0148;
		}break;
	}

	if(GetTypeFromGUID() == HIGHGUID_TYPE_VEHICLE)
		flags |= 0x00E0;

	if(target == this)
	{
		// player creating self
		flags |= 0x0001;
		updatetype = UPDATETYPE_CREATE_YOURSELF;
	}

	// build our actual update
	*data << updatetype;

	// we shouldn't be here, under any circumstances, unless we have a wowguid..
	ASSERT(m_wowGuid.GetNewGuidLen());
	*data << m_wowGuid;
	
	*data << m_objectTypeId;

	_BuildMovementUpdate(data, flags, flags2, target);

	// we have dirty data, or are creating for ourself.
	UpdateMask updateMask;
	updateMask.SetCount( m_valuesCount );
	_SetCreateBits( &updateMask, target );

	// this will cache automatically if needed
	_BuildValuesUpdate( data, &updateMask, target );

	// update count: 1 ;)
	return 1;
}

//That is dirty fix it actually creates update of 1 field with
//the given value ignoring existing changes in fields and so on
//usefull if we want update this field for certain players
//NOTE: it does not change fields. This is also very fast method
WorldPacket *Object::BuildFieldUpdatePacket( uint32 index,uint32 value)
{
   // uint64 guidfields = GetGUID();
   // uint8 guidmask = 0;
	WorldPacket * packet=new WorldPacket(1500);
	packet->SetOpcode( SMSG_UPDATE_OBJECT );
	
	*packet << (uint32)1;//number of update/create blocks

	*packet << (uint8) UPDATETYPE_VALUES;		// update type == update
	*packet << GetNewGUID();

	uint32 mBlocks = index/32+1;
	*packet << (uint8)mBlocks;
	
	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
	*packet <<(uint32)0;

	*packet <<(((uint32)(1))<<(index%32));
	*packet << value;
	
	return packet;
}

void Object::BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value)
{
	ByteBuffer buf(500);

	BuildFieldUpdatePacket(&buf, Index, Value);
	Target->PushUpdateData(&buf, 1);
}

void Object::BuildFieldUpdatePacket(ByteBuffer * buf, uint32 Index, uint32 Value)
{
	*buf << uint8(UPDATETYPE_VALUES);
	*buf << GetNewGUID();

	uint32 mBlocks = Index/32+1;
	*buf << (uint8)mBlocks;

	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
		*buf <<(uint32)0;

	*buf <<(((uint32)(1))<<(Index%32));
	*buf << Value;
}

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer *data, Player* target)
{
	UpdateMask updateMask;
	updateMask.SetCount( m_valuesCount );
	_SetUpdateBits( &updateMask, target );
	for(uint32 x = 0; x < m_valuesCount; ++x)
	{
		if(updateMask.GetBit(x))
		{
			*data << (uint8) UPDATETYPE_VALUES;		// update type == update
			ASSERT(m_wowGuid.GetNewGuidLen());
			*data << m_wowGuid;

			_BuildValuesUpdate( data, &updateMask, target );
			return 1;
		}
	}

	return 0;
}

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer * buf, UpdateMask * mask )
{
	// returns: update count
	*buf << (uint8) UPDATETYPE_VALUES;		// update type == update

	ASSERT(m_wowGuid.GetNewGuidLen());
	*buf << m_wowGuid;

	_BuildValuesUpdate( buf, mask, NULL );

	// 1 update.
	return 1;
}

void Object::DestroyForPlayer(Player* target) const
{
	if(!target || target->GetSession() == 0) return;

	WorldPacket data(SMSG_DESTROY_OBJECT, 8);
	data << GetGUID();
	data << uint8(0);

	target->GetSession()->SendPacket( &data );
}


///////////////////////////////////////////////////////////////
/// Build the Movement Data portion of the update packet
/// Fills the data with this object's movement/speed info
/// TODO: rewrite this stuff, document unknown fields and flags
uint32 TimeStamp();

void Object::_BuildMovementUpdate(ByteBuffer * data, uint16 flags, uint32 flags2, Player* target )
{
	ByteBuffer *splinebuf = (m_objectTypeId == TYPEID_UNIT) ? target->GetAndRemoveSplinePacket(GetGUID()) : 0;
	uint16 flag16 = 0;	// some other flag
	*data << (uint16)flags;

	Player* pThis = NULL;
	MovementInfo* moveinfo;
	if(m_objectTypeId == TYPEID_PLAYER)
	{
		pThis = TO_PLAYER(this);
		if(pThis->GetSession())
			moveinfo = pThis->GetSession()->GetMovementInfo();
		if(target == pThis)
		{
			// Updating our last speeds.
			pThis->UpdateLastSpeeds();
		}
	}

	if (flags & 0x20)
	{
		if(pThis && pThis->m_TransporterGUID != 0)
			flags2 |= 0x200;
		else if(m_objectTypeId==TYPEID_UNIT && TO_CREATURE(this)->m_transportGuid != 0 && TO_CREATURE(this)->m_transportPosition != NULL)
			flags2 |= 0x200;
		else if (IsUnit() && TO_UNIT(this)->m_CurrentVehicle != NULL)
			flags2 |= 0x200;

		if(splinebuf)
		{
			flags2 |= 0x08000001;	   //1=move forward
			if(GetTypeId() == TYPEID_UNIT)
			{
				if( TO_UNIT(this)->GetAIInterface()->m_moveRun == false)
					flags2 |= 0x100;	//100=walk
			}			
		}

		if(GetTypeId() == TYPEID_UNIT)
		{
			//		 Don't know what this is, but I've only seen it applied to spirit healers.
			//		 maybe some sort of invisibility flag? :/

			switch(GetEntry())
			{
			case 6491:  // Spirit Healer
			case 13116: // Alliance Spirit Guide
			case 13117: // Horde Spirit Guide
				{
					flags2 |= 0x10000000;
				}break;
			}
		
			if( TO_UNIT(this)->GetAIInterface()->IsFlying())
//				flags2 |= 0x800; //in 2.3 this is some state that i was not able to decode yet
				flags2 |= 0x400; //Zack : Teribus the Cursed had flag 400 instead of 800 and he is flying all the time 
			if( TO_CREATURE(this)->proto && TO_CREATURE(this)->proto->extra_a9_flags)
			{
				if(!(flags2 & 0x0200))
					flags2 |= TO_CREATURE(this)->proto->extra_a9_flags;
			}
/*			if(GetGUIDHigh() == HIGHGUID_WAYPOINT)
			{
				if(GetUInt32Value(UNIT_FIELD_STAT0) == 768)		// flying waypoint
					flags2 |= 0x800;
			}*/
		}

		*data << (uint32)flags2;

		*data << (uint16)flag16;

		*data << getMSTime(); // this appears to be time in ms but can be any thing

		// this stuff:
		//   0x01 -> Enable Swimming?
		//   0x04 -> ??
		//   0x10 -> disables movement compensation and causes players to jump around all the place
		//   0x40 -> disables movement compensation and causes players to jump around all the place

		*data << (float)m_position.x;
		*data << (float)m_position.y;
		*data << (float)m_position.z;
		*data << (float)m_position.o;

		if ( flags2 & 0x0200 )	//BYTE1(flags2) & 2
		{
			if (IsUnit() && TO_UNIT(this)->m_CurrentVehicle != NULL)
			{
				Unit* pUnit = TO_UNIT(this);
				Vehicle* vehicle = TO_UNIT(this)->m_CurrentVehicle;

				if (pUnit->m_inVehicleSeatId != 0xFF  && vehicle->getVehicleSeat(pUnit->m_inVehicleSeatId) != NULL )
				{
					*data << pUnit->m_CurrentVehicle->GetNewGUID();
					*data << vehicle->getVehicleSeat(pUnit->m_inVehicleSeatId)->m_attachmentOffsetX;
					*data << vehicle->getVehicleSeat(pUnit->m_inVehicleSeatId)->m_attachmentOffsetY;
					*data << vehicle->getVehicleSeat(pUnit->m_inVehicleSeatId)->m_attachmentOffsetZ;
					*data << float(0.0f);
					*data << uint32(0);
					*data << pUnit->m_inVehicleSeatId;
				}
				else
				{
					*data << uint8(0);
					*data << float(0);
					*data << float(0);
					*data << float(0);
					*data << float(0);
					*data << uint32(0);
					*data << uint8(0);
				}
			}
			else if(pThis)
			{
				WoWGuid wowguid(pThis->m_TransporterGUID);
				*data << wowguid;
				*data << pThis->m_TransporterX << pThis->m_TransporterY << pThis->m_TransporterZ << pThis->m_TransporterO;
				*data << pThis->m_TransporterUnk << uint8(0);
			}
			else if(m_objectTypeId==TYPEID_UNIT && TO_CREATURE(this)->m_transportPosition != NULL)
			{
				*data << TO_CREATURE(this)->m_transportNewGuid;

				*data << uint32(HIGHGUID_TYPE_TRANSPORTER);
				*data << TO_CREATURE(this)->m_transportPosition->x << TO_CREATURE(this)->m_transportPosition->y << 
					TO_CREATURE(this)->m_transportPosition->z << TO_CREATURE(this)->m_transportPosition->o;
				*data << uint32(0);
				*data << uint8(0);
			}
		}

		if(flags2 & 0x2200000 || flag16 & 0x20) //flying/swimming, && unk sth to do with vehicles?
		{
			if(pThis && moveinfo)
			{
				*data << moveinfo->pitch;
			}
			*data << (float)0; //pitch
		}
		if(pThis && moveinfo)
		{
			*data << moveinfo->unklast;
		}
		else
		*data << (uint32)0; //last fall time

		if(flags2 & 0x1000) // BYTE1(flags2) & 0x10
		{
			if(pThis && moveinfo)
			{
				*data << moveinfo->FallTime;
				*data << moveinfo->jump_sinAngle;
				*data << moveinfo->jump_cosAngle;
				*data << moveinfo->jump_xySpeed;
			}
			*data << (float)0;
			*data << (float)1.0; //sinAngle
			*data << (float)0;	 //cosAngle
			*data << (float)0;	 //xySpeed
		}
		if( flags2 & 0x4000000 )
		{
			*data << (float)0; //unknown float
		}

		*data << m_walkSpeed;	 // walk speed
		*data << m_runSpeed;	  // run speed
		*data << m_backWalkSpeed; // backwards run speed
		*data << m_swimSpeed;	 // swim speed
		*data << m_backSwimSpeed; // backwards swim speed
		*data << m_flySpeed;		// fly speed
		*data << m_backFlySpeed;	// back fly speed
		*data << m_turnRate;	  // turn rate
		*data << float(7);			//pitch rate

		if(splinebuf)	// client expects that flags2 & 0x8000000 != 0 in this case
		{
			data->append(*splinebuf);
			delete splinebuf;
		}
	}
	else if(flags & 0x100)
    {
			*data << uint8(0);                              // unk PGUID!
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
			*data << (float)m_position.o;
			*data << float(0);
	}
	else if(flags & 0x40)
	{
		if(flags & 0x2 && (GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == GAMEOBJECT_TYPE_MO_TRANSPORT))
		{
			*data << float(0);
			*data << float(0);
			*data << float(0);
		}
		else
		{
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
		}
			*data << (float)m_position.o;
	}

	if(flags & 8)
	{
		*data << GetUInt32Value(OBJECT_FIELD_GUID);
	}
	
	if(flags & 0x0010)
		*data << GetUInt32Value(OBJECT_FIELD_GUID+1);

	if(flags & 0x0004)
	{
		*data << uint8(0);// unknown NewGUID
	}

	if(flags & 2)
	{
        *data << (uint32)getMSTime();
	}

	if (flags & 0x80) //if ((_BYTE)flags_ < 0)
	{
			*data << TO_VEHICLE(this)->getVehicleEntry() << float(0.0f);
	}

	// 0x200
    if(flags & 0x0200)
    {
		uint64 rotation = 0;
		if(IsGameObject())
			rotation = TO_GAMEOBJECT(this)->m_rotation;
        *data << uint64(rotation); //blizz 64bit rotation
    }
}

//=======================================================================================
//  Creates an update block with the values of this object as
//  determined by the updateMask.
//=======================================================================================
void Object::_BuildValuesUpdate(ByteBuffer * data, UpdateMask *updateMask, Player* target)
{
	bool reset = false;

	uint32 oldState = 0;
	if(updateMask->GetBit(OBJECT_FIELD_GUID) && target)	   // We're creating.
	{
		Creature* pThis = TO_CREATURE(this);
		if(m_objectTypeId == TYPEID_UNIT && pThis->m_taggingPlayer)		// tagged group will have tagged player
		{
			// set tagged visual
			if( (pThis->m_taggingGroup != 0 && target->m_playerInfo->m_Group != NULL && target->m_playerInfo->m_Group->GetID() == pThis->m_taggingGroup) ||
				(pThis->m_taggingPlayer == target->GetLowGUID()) )
			{
				m_uint32Values[UNIT_DYNAMIC_FLAGS] |= U_DYN_FLAG_TAPPED_BY_PLAYER;
				if( pThis->m_loot.HasLoot(target) )
					m_uint32Values[UNIT_DYNAMIC_FLAGS] |= U_DYN_FLAG_LOOTABLE;
			}
			else
			{
				m_uint32Values[UNIT_DYNAMIC_FLAGS] |= U_DYN_FLAG_TAGGED_BY_OTHER;
			}

			updateMask->SetBit(UNIT_DYNAMIC_FLAGS);
			reset = true;
		}

		if(target && GetTypeId() == TYPEID_GAMEOBJECT)
		{
			GameObject* go = TO_GAMEOBJECT(this);
			GameObjectInfo *info;
			info = go->GetInfo();
			if(info && info->InvolvedQuestCount && info->InvolvedQuestIds[0])
			{
				for(uint32 v = 0; v < info->InvolvedQuestCount; ++v)
				{
					if( target->GetQuestLogForEntry(info->InvolvedQuestIds[v]) != NULL )
					{
						m_uint32Values[GAMEOBJECT_DYNAMIC] = GO_DYNFLAG_QUEST;
						uint32 state = GetUInt32Value(GAMEOBJECT_BYTES_1);
						oldState = state;
						uint8 * v = (uint8*)&state;
						v[GAMEOBJECT_BYTES_STATE] = 1;
						m_uint32Values[GAMEOBJECT_BYTES_1] = state;
						m_uint32Values[GAMEOBJECT_FLAGS] &= ~GO_FLAG_IN_USE;

						updateMask->SetBit(GAMEOBJECT_BYTES_1);
						updateMask->SetBit(GAMEOBJECT_DYNAMIC);
						updateMask->SetBit(GAMEOBJECT_FLAGS);

						reset = true;
						break;
					}
				}
			}
		}
	}

	WPAssert( updateMask && updateMask->GetCount() == m_valuesCount );
	uint32 bc;
	uint32 values_count;
	if( m_valuesCount > ( 2 * 0x20 ) )//if number of blocks > 2->  unit and player+item container
	{
		bc = updateMask->GetUpdateBlockCount();
		values_count = (uint32)min( bc * 32, m_valuesCount );

	}
	else
	{
		bc=updateMask->GetBlockCount();
		values_count=m_valuesCount;
	}

	*data << (uint8)bc;
	data->append( updateMask->GetMask(), bc*4 );
	  
	for( uint32 index = 0; index < values_count; ++index )
	{
		if( updateMask->GetBit( index ) )
		{
			*data << m_uint32Values[ index ];
			updateMask->UnsetBit( index );
		}
	}

	if(reset)
	{
		switch(GetTypeId())
		{
		case TYPEID_UNIT:
			m_uint32Values[UNIT_DYNAMIC_FLAGS] &= ~(U_DYN_FLAG_TAGGED_BY_OTHER | U_DYN_FLAG_LOOTABLE | U_DYN_FLAG_TAPPED_BY_PLAYER);
			break;
		case TYPEID_GAMEOBJECT:
			m_uint32Values[GAMEOBJECT_DYNAMIC] = 0;
			m_uint32Values[GAMEOBJECT_FLAGS] |= GO_FLAG_IN_USE;
			m_uint32Values[GAMEOBJECT_BYTES_1] = oldState;
			break;
		}
	}

}

void Object::BuildHeartBeatMsg(WorldPacket *data) const
{
	data->Initialize(MSG_MOVE_HEARTBEAT);

	*data << GetGUID();

	*data << uint32(0); // flags
	*data << getMSTime(); // mysterious value #1

	*data << m_position;
	*data << m_position.o;
}

WorldPacket * Object::BuildTeleportAckMsg(const LocationVector & v)
{
	///////////////////////////////////////
	//Update player on the client with TELEPORT_ACK
	if( IsInWorld() )		// only send when inworld
		TO_PLAYER(this)->SetPlayerStatus( TRANSFER_PENDING );

	WorldPacket * data = new WorldPacket(MSG_MOVE_TELEPORT_ACK, 80);
	*data << GetNewGUID();

	//First 4 bytes = no idea what it is
	*data << uint32(2); // flags
	*data << getMSTime(); // mysterious value #1
	*data << uint16(0);

	*data << float(0);
	*data << v;
	*data << v.o;
	*data << uint16(2);
	*data << uint8(0);
	return data;
}

bool Object::SetPosition(const LocationVector & v, bool allowPorting /* = false */)
{
	bool updateMap = false, result = true;

	if (m_position.x != v.x || m_position.y != v.y)
		updateMap = true;

	m_position = const_cast<LocationVector&>(v);

	if (!allowPorting && v.z < -500)
	{
		m_position.z = 500;
		OUT_DEBUG( "setPosition: fell through map; height ported" );

		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_mapMgr->ChangeObjectLocation(this);
	}

	return result;
}

bool Object::SetPosition( float newX, float newY, float newZ, float newOrientation, bool allowPorting )
{
	bool updateMap = false, result = true;

	//if (m_position.x != newX || m_position.y != newY)
		//updateMap = true;
	if(m_lastMapUpdatePosition.Distance2DSq(newX, newY) > 4.0f)		/* 2.0f */
		updateMap = true;

	m_position.ChangeCoords(newX, newY, newZ, newOrientation);

	if (!allowPorting && newZ < -500)
	{
		m_position.z = 500;
		OUT_DEBUG( "setPosition: fell through map; height ported" );
		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_lastMapUpdatePosition.ChangeCoords(newX,newY,newZ,newOrientation);
		m_mapMgr->ChangeObjectLocation(this);

		if( m_objectTypeId == TYPEID_PLAYER && TO_PLAYER(this)->GetGroup() && TO_PLAYER(this)->m_last_group_position.Distance2DSq(m_position) > 25.0f ) // distance of 5.0
		{
            TO_PLAYER(this)->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_POSITION, TO_PLAYER(this) );
		}	
	}

	return result;
}

void Object::SetRotation( uint64 guid )
{
	WorldPacket data(SMSG_AI_REACTION, 12);
	data << guid;
	data << uint32(2);
	SendMessageToSet(&data, false);
}

void Object::OutPacketToSet(uint16 Opcode, uint16 Len, const void * Data, bool self)
{
	if(self && m_objectTypeId == TYPEID_PLAYER)
		TO_PLAYER(this)->GetSession()->OutPacket(Opcode, Len, Data);

	if(!IsInWorld())
		return;

	unordered_set<Player*  >::iterator itr = m_inRangePlayers.begin();
	unordered_set<Player*  >::iterator it_end = m_inRangePlayers.end();
	int gm = ( m_objectTypeId == TYPEID_PLAYER ? TO_PLAYER(this)->m_isGmInvisible : 0 );
	for(; itr != it_end; ++itr)
	{
		ASSERT((*itr)->GetSession());
		if( gm )
		{
			if( (*itr)->GetSession()->GetPermissionCount() > 0 )
				(*itr)->GetSession()->OutPacket(Opcode, Len, Data);
		}
		else
		{
			(*itr)->GetSession()->OutPacket(Opcode, Len, Data);
		}
	}
}

void Object::SendMessageToSet(WorldPacket *data, bool bToSelf,bool myteam_only)
{
	if(bToSelf && m_objectTypeId == TYPEID_PLAYER)
	{
		TO_PLAYER(this)->GetSession()->SendPacket(data);		
	}

	if(!IsInWorld())
		return;

	unordered_set<Player*  >::iterator itr = m_inRangePlayers.begin();
	unordered_set<Player*  >::iterator it_end = m_inRangePlayers.end();
	bool gminvis = (m_objectTypeId == TYPEID_PLAYER ? TO_PLAYER(this)->m_isGmInvisible : false);
	//Zehamster: Splitting into if/else allows us to avoid testing "gminvis==true" at each loop...
	//		   saving cpu cycles. Chat messages will be sent to everybody even if player is invisible.
	if(myteam_only)
	{
		uint32 myteam=TO_PLAYER(this)->GetTeam();
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetSession()->GetPermissionCount() > 0 && (*itr)->GetTeam()==myteam && PhasedCanInteract(*itr))
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetTeam()==myteam && PhasedCanInteract(*itr))
					(*itr)->GetSession()->SendPacket(data);
			}
		}
	}
	else
	{
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetSession()->GetPermissionCount() > 0 && PhasedCanInteract(*itr))
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if( PhasedCanInteract(*itr) )
					(*itr)->GetSession()->SendPacket(data);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
/// Fill the object's Update Values from a space deliminated list of values.
void Object::LoadValues(const char* data)
{
	// thread-safe ;) strtok is not.
	std::string ndata = data;
	std::string::size_type last_pos = 0, pos = 0;
	uint32 index = 0;
	uint32 val;
	do 
	{
		// prevent overflow
		if(index >= m_valuesCount)
		{
			break;
		}
		pos = ndata.find(" ", last_pos);
		val = atol(ndata.substr(last_pos, (pos-last_pos)).c_str());
		if(m_uint32Values[index] == 0)
			m_uint32Values[index] = val;
		last_pos = pos+1;
		++index;
	} while(pos != std::string::npos);
}

void Object::_SetUpdateBits(UpdateMask *updateMask, Player* target) const
{
	*updateMask = m_updateMask;
}


void Object::_SetCreateBits(UpdateMask *updateMask, Player* target) const
{
	/*for( uint16 index = 0; index < m_valuesCount; index++ )
	{
		if(GetUInt32Value(index) != 0)
			updateMask->SetBit(index);
	}*/
	for(uint32 i = 0; i < m_valuesCount; ++i)
		if(m_uint32Values[i] != 0)
			updateMask->SetBit(i);
}

void Object::AddToWorld()
{
	MapMgr* mapMgr = sInstanceMgr.GetInstance(this);
	if(mapMgr == NULL)
	{
		if(IsPlayer() && TO_PLAYER(this)->m_bg != NULL && TO_PLAYER(this)->m_bg->IsArena())
			mapMgr = TO_PLAYER(this)->m_bg->GetMapMgr();
		else
			return; //instance add failed
	}

	if( IsPlayer() )
	{
		// battleground checks
		Player* p = TO_PLAYER(this);
		if( p->m_bg == NULL && mapMgr->m_battleground != NULL )
		{
			// player hasn't been registered in the battleground, ok.
			// that means we re-logged into one. if it's an arena, don't allow it!
			// also, don't allow them in if the bg is full.

			if( !mapMgr->m_battleground->CanPlayerJoin(p) && !p->bGMTagOn)
				return;
		}
		// players who's group disbanded cannot remain in a raid instances alone(no soloing them:P)
		if( !p->triggerpass_cheat && p->GetGroup()== NULL && (mapMgr->GetMapInfo()->type == INSTANCE_RAID || mapMgr->GetMapInfo()->type == INSTANCE_MULTIMODE))
			return ;
	}

	m_mapMgr = mapMgr;
	m_inQueue = true;

	mapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = m_mapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

void Object::AddToWorld(MapMgr* pMapMgr)
{
	if(!pMapMgr)
		return; //instance add failed

	m_mapMgr = pMapMgr;
	m_inQueue = true;

	pMapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = pMapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

//Unlike addtoworld it pushes it directly ignoring add pool
//this can only be called from the thread of mapmgr!!!
void Object::PushToWorld(MapMgr* mgr)
{
	ASSERT(mgr!=NULL);
	if(mgr==NULL)
	{
		// Reset these so session will get updated properly.
		mSemaphoreTeleport = false;
		m_inQueue = false;

		if(IsPlayer())
		{
			Log.Error("Object","Kicking Player %s due to empty MapMgr;",TO_PLAYER(this)->GetName());
			TO_PLAYER(this)->GetSession()->LogoutPlayer(false);
		}
		return; //instance add failed
	}

	m_mapId=mgr->GetMapId();
	m_instanceId = mgr->GetInstanceID();

	m_mapMgr = mgr;
	OnPrePushToWorld();

	mgr->PushObject(this);

	// correct incorrect instance id's
	mSemaphoreTeleport = false;
	m_inQueue = false;
   
	event_Relocate();
	
	// call virtual function to handle stuff.. :P
	OnPushToWorld();
}

void Object::RemoveFromWorld(bool free_guid)
{
	// clear loot
	ClearLoot();
	if(dynObj != 0)
		dynObj->Remove();

	ASSERT(m_mapMgr);
	MapMgr* m = m_mapMgr;
	m_mapMgr = NULL;

	mSemaphoreTeleport = true;

	m->RemoveObject(this, free_guid);
	
	// remove any spells / free memory
	sEventMgr.RemoveEvents(this, EVENT_UNIT_SPELL_HIT);

	// update our event holder
	event_Relocate();
}

void Object::SetByte(uint32 index, uint32 index1,uint8 value)
{
	ASSERT( index < m_valuesCount );

	if(index1 > 4)
	{
		sLog.outError("Object::SetByteValue: wrong offset %u", index1);
		return;
	}

	if(uint8(m_uint32Values[ index ] >> (index1 * 8)) != value)
	{
		m_uint32Values[ index ] &= ~uint32(uint32(0xFF) << (index1 * 8));
		m_uint32Values[ index ] |= uint32(uint32(value) << (index1 * 8));

		if(IsInWorld())
		{
			m_updateMask.SetBit( index );

			if(!m_objectUpdated)
			{
				m_mapMgr->ObjectUpdated(this);
				m_objectUpdated = true;
			}
		}
	}
}

//! Set uint32 property
void Object::SetUInt32Value( const uint32 index, const uint32 value )
{
	ASSERT( index < m_valuesCount );
	// save updating when val isn't changing.
	if(m_uint32Values[index] == value)
		return;

	m_uint32Values[ index ] = value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	// Group update handling
	if(m_objectTypeId == TYPEID_PLAYER)
	{
		if(IsInWorld())
		{
			Group* pGroup = TO_PLAYER(this)->GetGroup();
			if( pGroup != NULL )
				pGroup->HandleUpdateFieldChange( index, TO_PLAYER(this) );
		}

#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			TO_PLAYER(this)->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			TO_PLAYER(this)->save_Gold();
			break;
		}
#endif
	}
}

/*
//must be in %
void Object::ModPUInt32Value(const uint32 index, const int32 value, bool apply )
{
	ASSERT( index < m_valuesCount );
	int32 basevalue = (int32)m_uint32Values[ index ];
	if(apply)
		m_uint32Values[ index ] += ((basevalue*value)/100);
	else
		m_uint32Values[ index ] = (basevalue*100)/(100+value);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated )
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
*/
uint32 Object::GetModPUInt32Value(const uint32 index, const int32 value)
{
	ASSERT( index < m_valuesCount );
	int32 basevalue = (int32)m_uint32Values[ index ];
	return ((basevalue*value)/100);
}

void Object::ModUnsigned32Value(uint32 index, int32 mod)
{
	ASSERT( index < m_valuesCount );
	if(mod == 0)
		return;

	m_uint32Values[ index ] += mod;
	if( (int32)m_uint32Values[index] < 0 )
		m_uint32Values[index] = 0;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}



	if(m_objectTypeId == TYPEID_PLAYER)
	{
		// mana and energy regen
		if( index == UNIT_FIELD_POWER1 || index == UNIT_FIELD_POWER4 )
			TO_PLAYER( this )->SendPowerUpdate();

#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			TO_PLAYER(this)->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			TO_PLAYER(this)->save_Gold();
			break;
		}
#endif
	}
}

void Object::ModSignedInt32Value(uint32 index, int32 value )
{
	ASSERT( index < m_valuesCount );
	if(value == 0)
		return;

	m_uint32Values[ index ] += value;
	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	if(m_objectTypeId == TYPEID_PLAYER)
	{
#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			TO_PLAYER(this)->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			TO_PLAYER(this)->save_Gold();
			break;
		}
#endif
	}
}

void Object::ModFloatValue(const uint32 index, const float value )
{
	ASSERT( index < m_valuesCount );
	m_floatValues[ index ] += value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
//! Set uint64 property
void Object::SetUInt64Value( const uint32 index, const uint64 value )
{
	assert( index + 1 < m_valuesCount );
	if(m_uint32Values[index] == GUID_LOPART(value) && m_uint32Values[index+1] == GUID_HIPART(value))
		return;

	m_uint32Values[ index ] = *((uint32*)&value);
	m_uint32Values[ index + 1 ] = *(((uint32*)&value) + 1);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );
		m_updateMask.SetBit( index + 1 );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

//! Set float property
void Object::SetFloatValue( const uint32 index, const float value )
{
	ASSERT( index < m_valuesCount );
	if(m_floatValues[index] == value)
		return;

	m_floatValues[ index ] = value;
	
	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}


void Object::SetFlag( const uint32 index, uint32 newFlag )
{
	ASSERT( index < m_valuesCount );

	//no change -> no update
	if((m_uint32Values[ index ] & newFlag)==newFlag)
		return;

	m_uint32Values[ index ] |= newFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	// we're modifying an aurastate, so we need to update the auras.
	if( index == UNIT_FIELD_AURASTATE )
	{
		Unit* u = TO_UNIT(this);
		if( newFlag == AURASTATE_FLAG_STUNNED && u->IsPlayer() && u->HasDummyAura(SPELL_HASH_PRIMAL_TENACITY) && TO_PLAYER(u)->GetShapeShift() == FORM_CAT )
		{
			for(uint32 i = 0; i < MAX_AURAS+MAX_POSITIVE_AURAS; ++i)
			{
				if( u->m_auras[i] != NULL && u->m_auras[i]->GetSpellProto()->NameHash == SPELL_HASH_PRIMAL_TENACITY )
				{
					Aura* aura = new Aura(u->m_auras[i]->GetSpellProto(), -1, TO_OBJECT(this), TO_UNIT(this));
					u->RemoveAuraBySlot(i);
					aura->AddMod(232, -31, 5, 0);
					aura->AddMod(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, -31, 127, 1);
					aura->AddMod(SPELL_AURA_DUMMY, 0, 0, 2);
					u->AddAura(aura);
					continue;
				}

				if( u->m_auras[i] != NULL && !u->m_auras[i]->m_applied) // try to apply
					u->m_auras[i]->ApplyModifiers(true);

				if( u->m_auras[i] != NULL && u->m_auras[i]->m_applied) // try to remove, if we lack the aurastate
					u->m_auras[i]->RemoveIfNecessary();
			}
		}
		else
		{
			for(uint32 i = 0; i < MAX_AURAS+MAX_POSITIVE_AURAS; ++i)
			{
				if( u->m_auras[i] != NULL && !u->m_auras[i]->m_applied) // try to apply
					u->m_auras[i]->ApplyModifiers(true);

				if( u->m_auras[i] != NULL && u->m_auras[i]->m_applied) // try to remove, if we lack the aurastate
					u->m_auras[i]->RemoveIfNecessary();
			}
		}
	}
}


void Object::RemoveFlag( const uint32 index, uint32 oldFlag )
{
	ASSERT( index < m_valuesCount );

	//no change -> no update
	if((m_uint32Values[ index ] & oldFlag)==0)
		return;

	m_uint32Values[ index ] &= ~oldFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	// we're modifying an aurastate, so we need to update the auras.
	if( index == UNIT_FIELD_AURASTATE )
	{
		Unit* u = TO_UNIT(this);
		if( oldFlag == AURASTATE_FLAG_STUNNED && u->IsPlayer() && u->HasDummyAura(SPELL_HASH_PRIMAL_TENACITY) && TO_PLAYER(u)->GetShapeShift() == FORM_CAT )
		{
			for(uint32 i = 0; i < MAX_AURAS+MAX_POSITIVE_AURAS; ++i)
			{
				if( u->m_auras[i] != NULL && u->m_auras[i]->GetSpellProto()->NameHash == SPELL_HASH_PRIMAL_TENACITY )
				{
					Aura* aura = new Aura(u->m_auras[i]->GetSpellProto(), -1, TO_OBJECT(this), TO_UNIT(this));
					u->RemoveAuraBySlot(i);
					aura->AddMod(232, -31, 5, 0);
					aura->AddMod(SPELL_AURA_DUMMY, 0, 0, 2);
					aura->AddMod(SPELL_AURA_ADD_PCT_MODIFIER, -51, 14, 2);
					u->AddAura(aura);
					continue;
				}

				if( u->m_auras[i] != NULL && !u->m_auras[i]->m_applied) // try to apply
					u->m_auras[i]->ApplyModifiers(true);

				if( u->m_auras[i] != NULL && u->m_auras[i]->m_applied) // try to remove, if we lack the aurastate
					u->m_auras[i]->RemoveIfNecessary();
			}
		}
		else
		{
			for(uint32 i = 0; i < MAX_AURAS+MAX_POSITIVE_AURAS; ++i)
			{
				if( u->m_auras[i] != NULL && !u->m_auras[i]->m_applied) // try to apply
					u->m_auras[i]->ApplyModifiers(true);

				if( u->m_auras[i] != NULL && u->m_auras[i]->m_applied) // try to remove, if we lack the aurastate
					u->m_auras[i]->RemoveIfNecessary();
			}
		}
	}
}

void Object::SetFlag64( const uint32 index, uint64 newFlag )
{
	ASSERT( index < m_valuesCount );
	uint32 oldval = GetUInt64Value(index);
	uint32 newval = oldval | newFlag;
	SetUInt64Value(index, newval);
}

void Object::RemoveFlag64( const uint32 index, uint64 oldFlag )
{
	ASSERT( index < m_valuesCount );
	uint32 oldval = GetUInt64Value(index);
	uint64 newval = oldval & ~oldFlag;
	SetUInt64Value(index, newval);
}

////////////////////////////////////////////////////////////

float Object::CalcDistance(Object* Ob)
{
	return CalcDistance(GetPositionX(), GetPositionY(), GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(float ObX, float ObY, float ObZ)
{
	return CalcDistance(GetPositionX(), GetPositionY(), GetPositionZ(), ObX, ObY, ObZ);
}
float Object::CalcDistance(Object* Oa, Object* Ob)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(Object* Oa, float ObX, float ObY, float ObZ)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), ObX, ObY, ObZ);
}

float Object::CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ)
{
	float xdest = OaX - ObX;
	float ydest = OaY - ObY;
	float zdest = OaZ - ObZ;
	return sqrtf(zdest*zdest + ydest*ydest + xdest*xdest);
}

float Object::calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	float dx = Position2X-Position1X;
	float dy = Position2Y-Position1Y;
	double angle=0.0f;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5 /* / 2 */;
		else
			angle = M_PI * 3.0 * 0.5/* / 2 */;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atanf(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atanf(dy/dx) + (2*M_PI);
		else
			angle = atanf(dy/dx);
	}

	// Convert to degrees
	angle = angle * float(180 / M_PI);

	// Return
	return float(angle);
}

float Object::calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	double dx = double(Position2X-Position1X);
	double dy = double(Position2Y-Position1Y);
	double angle=0.0;

	// Calculate angle
	if (dx == 0.0)
	{
		if (dy == 0.0)
			angle = 0.0;
		else if (dy > 0.0)
			angle = M_PI * 0.5/*/ 2.0*/;
		else
			angle = M_PI * 3.0 * 0.5/*/ 2.0*/;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (dx < 0.0)
			angle = atan(dy/dx) + M_PI;
		else if (dy < 0.0)
			angle = atan(dy/dx) + (2*M_PI);
		else
			angle = atan(dy/dx);
	}

	// Return
	return float(angle);
}

float Object::getEasyAngle( float angle )
{
	while ( angle < 0 ) {
		angle = angle + 360;
	}
	while ( angle >= 360 ) {
		angle = angle - 360;
	}
	return angle;
}

bool Object::inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y )
{
	float angle = calcAngle( Position1X, Position1Y, Position2X, Position2Y );
	float lborder = getEasyAngle( ( Orientation - (FOV*0.5f/*/2*/) ) );
	float rborder = getEasyAngle( ( Orientation + (FOV*0.5f/*/2*/) ) );
	//OUT_DEBUG("Orientation: %f Angle: %f LeftBorder: %f RightBorder %f",Orientation,angle,lborder,rborder);
	if(((angle >= lborder) && (angle <= rborder)) || ((lborder > rborder) && ((angle < rborder) || (angle > lborder))))
	{
		return true;
	}
	else
	{
		return false;
	}
} 

bool Object::isInFront(Object* target)
{
	// check if we facing something ( is the object within a 180 degree slice of our positive y axis )

    double x = target->GetPositionX() - m_position.x;
    double y = target->GetPositionY() - m_position.y;

    double angle = atan2( y, x );
    angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;
	angle -= m_position.o;

    while( angle > M_PI)
        angle -= 2.0 * M_PI;

    while(angle < -M_PI)
        angle += 2.0 * M_PI;

	// replace M_PI in the two lines below to reduce or increase angle

    double left = -1.0 * ( M_PI / 2.0 );
    double right = ( M_PI / 2.0 );

    return( ( angle >= left ) && ( angle <= right ) );
}

bool Object::isInBack(Object* target)
{
	if(CalcDistance(target) < 0.5f)
		return false; 

	// check if we are behind something ( is the object within a 180 degree slice of our negative y axis )

	double x = target->GetPositionX() - m_position.x;
	double y = target->GetPositionY() - m_position.y;

	double angle = atan2( y, x );
	angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;
	angle -= target->GetOrientation();

	while( angle > M_PI)
		angle -= 2.0 * M_PI;

	while(angle < -M_PI)
		angle += 2.0 * M_PI;

	// replace M_H_PI in the two lines below to reduce or increase angle

	double left = -1.0 * ( M_H_PI / 2.0 );
	double right = ( M_H_PI / 2.0 );

	return( ( angle >= left ) && ( angle <= right ) );
}

bool Object::isInArc(Object* target , float angle) // angle in degrees
{
	return inArc( GetPositionX() , GetPositionY() , angle , GetOrientation() , target->GetPositionX() , target->GetPositionY() );
}

bool Object::isInRange(Object* target, float range)
{
	float dist = CalcDistance( target );
	return( dist <= range );
}

bool Object::IsPet()
{
	if( GetTypeId() != TYPEID_UNIT )
		return false;

	if( TO_UNIT(this)->m_isPet && m_uint32Values[UNIT_FIELD_CREATEDBY] != 0 && m_uint32Values[UNIT_FIELD_SUMMONEDBY] != 0 )
		return true;

	return false;
}

void Object::_setFaction()
{
	FactionTemplateDBC* factT = NULL;

	if(GetTypeId() == TYPEID_UNIT || GetTypeId() == TYPEID_PLAYER)
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	else if(GetTypeId() == TYPEID_GAMEOBJECT)
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(GAMEOBJECT_FACTION));

	if(factT)
	{
		m_faction = factT;
		m_factionDBC = dbcFaction.LookupEntry(factT->Faction);
	}
}
void Object::UpdateOppFactionSet()
{
	m_oppFactsInRange.clear();
	for(Object::InRangeSet::iterator i = GetInRangeSetBegin(); i != GetInRangeSetEnd(); ++i)
	{
		if (((*i)->GetTypeId() == TYPEID_UNIT) || ((*i)->GetTypeId() == TYPEID_PLAYER) || ((*i)->GetTypeId() == TYPEID_GAMEOBJECT))
		{
			if (isHostile(this, (*i)))
			{
				if(!(*i)->IsInRangeOppFactSet(this))
					(*i)->m_oppFactsInRange.insert(this);
				if (!IsInRangeOppFactSet((*i)))
					m_oppFactsInRange.insert((*i));
				
			}
			else
			{
				if((*i)->IsInRangeOppFactSet(this))
					(*i)->m_oppFactsInRange.erase(this);
				if (IsInRangeOppFactSet((*i)))
					m_oppFactsInRange.erase((*i));
			}
		}
	}
}

void Object::EventSetUInt32Value(uint32 index, uint32 value)
{
	SetUInt32Value(index,value);
}

void Object::DealDamage(Unit* pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras)
{
	Player* plr = NULL;

	if( !pVictim || !pVictim->isAlive() || !pVictim->IsInWorld() || !IsInWorld() )
		return;
	if( pVictim->GetTypeId() == TYPEID_PLAYER && TO_PLAYER( pVictim )->GodModeCheat == true )
		return;
	if( pVictim->IsSpiritHealer() )
		return;
	
	if( pVictim->GetStandState() )//not standing-> standup
	{
		pVictim->SetStandState( STANDSTATE_STAND );//probably mobs also must standup
	}

	// This one is easy. If we're attacking a hostile target, and we're not flagged, flag us.
	// Also, you WONT get flagged if you are dueling that person - FiShBaIt
	if( pVictim->IsPlayer() && IsPlayer() )
	{
		if( isHostile( this, pVictim ) && TO_PLAYER( pVictim )->DuelingWith != TO_PLAYER(this) )
			TO_PLAYER(this)->SetPvPFlag();
	}
	//If our pet attacks  - flag us.
	if( pVictim->IsPlayer() && IsPet() )
	{
		Player* owner = TO_PLAYER( TO_PET(this)->GetPetOwner() );
		if( owner != NULL )
			if( owner->isAlive() && TO_PLAYER( pVictim )->DuelingWith != owner )
				owner->SetPvPFlag();		
	}

	if(!no_remove_auras)
	{
		float breakchance = 35.0f;
		if( spellId == 51514)// && IsUnit() && HasDummyAura(SPELL_HASH_GLYPH_OF_HEX) ) wait till 3.1
			breakchance += 15.0f;

		//zack 2007 04 24 : root should not remove self (and also other unknown spells)
		if(spellId)
		{
			pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN, spellId);
			if(Rand(breakchance))
				pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_UNUSED2, spellId);
		}
		else
		{
			pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);
			if(Rand(breakchance))
				pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_UNUSED2);
		}
	}

	// Paladin: Blessing of Sacrifice, and Warlock: Soul Link
	if( pVictim->m_damageSplitTarget.active)
	{
		if( spellId )
		{
			SpellEntry *spell = dbcSpell.LookupEntry(spellId);
			damage = (float)pVictim->DoDamageSplitTarget(damage, spell->School, false);
		}
		else
		{
			damage = (float)pVictim->DoDamageSplitTarget(damage, 0, true);
		}
	}

	if(IsUnit() && TO_UNIT(this)->isAlive() )
	{
		if( TO_UNIT(this) != pVictim && pVictim->IsPlayer() && IsPlayer() && TO_PLAYER(this)->m_hasInRangeGuards )
		{
			TO_PLAYER(this)->SetGuardHostileFlag(true);
			TO_PLAYER(this)->CreateResetGuardHostileFlagEvent();
		}

/*		if(!pVictim->isInCombat() && pVictim->IsPlayer())
			sHookInterface.OnEnterCombat( TO_PLAYER( pVictim ), TO_UNIT(this) );

		if(IsPlayer() && ! TO_PLAYER(this)->isInCombat())
			sHookInterface.OnEnterCombat( TO_PLAYER(this), TO_PLAYER(this) );*/
			
		if(IsPet())
			plr = TO_PET(this)->GetPetOwner();
		else if(IsPlayer())
			plr = TO_PLAYER(this);

		if(plr != NULL && plr->GetTypeId() == TYPEID_PLAYER && pVictim->GetTypeId() == TYPEID_UNIT) // Units can't tag..
			TO_CREATURE(pVictim)->Tag(plr);

		//is this correct this
		if( pVictim != 
			TO_UNIT(this) )
		{
			// Set our attack target to the victim.
			TO_UNIT(this)->CombatStatus.OnDamageDealt( pVictim, damage );
		}
	}

        ///Rage
        float val;

		if( pVictim->GetPowerType() == POWER_TYPE_RAGE 
			&& pVictim != TO_UNIT(this)
			&& pVictim->IsPlayer())
		{
			float level = (float)pVictim->getLevel();
			float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;
			uint32 rage = pVictim->GetUInt32Value( UNIT_FIELD_POWER2 );
			val = 2.5f * damage / c;
			rage += float2int32(val) * 10;
			if( rage > pVictim->GetUInt32Value(UNIT_FIELD_MAXPOWER2) )
				rage = pVictim->GetUInt32Value(UNIT_FIELD_MAXPOWER2);
			
			pVictim->SetUInt32Value(UNIT_FIELD_POWER2, rage);
			pVictim->SendPowerUpdate();
		}

	
	//* BATTLEGROUND DAMAGE COUNTER *//
	if( pVictim != TO_UNIT(this) )
	{
		if( IsPlayer() )
		{
			plr = TO_PLAYER(this);
		}
		else if( IsPet() )
		{
			plr = TO_PET(this)->GetPetOwner();
			if( plr != NULL && plr->GetMapMgr() == GetMapMgr() )
				plr = NULL;
		}

		if( plr != NULL && plr->m_bg != NULL && plr->GetMapMgr() == GetMapMgr() )
		{
			plr->m_bgScore.DamageDone += damage;
			plr->m_bg->UpdatePvPData();
		}
	}
   
	uint32 health = pVictim->GetUInt32Value(UNIT_FIELD_HEALTH );

	if(health <= damage && pVictim->IsPlayer() && pVictim->getClass() == ROGUE && pVictim->m_CustomTimers[CUSTOM_TIMER_CHEATDEATH] <= getMSTime() )
	{
		Player* plrVictim = TO_PLAYER(pVictim);
		uint32 rank = plrVictim->m_cheatDeathRank;

		uint32 chance = rank == 3 ? 100 : rank * 33;
		if(Rand(chance))
		{
			// Proc that cheating death!
			SpellEntry *spellInfo = dbcSpell.LookupEntry(45182);
			Spell* spell(new Spell(pVictim, spellInfo, true, NULL));
			SpellCastTargets targets;
			targets.m_unitTarget = pVictim->GetGUID();
			spell->prepare(&targets);
			TO_PLAYER(pVictim)->m_CustomTimers[CUSTOM_TIMER_CHEATDEATH] = getMSTime() + 60000;

			// Why return? So this damage isn't counted. ;)
			// On official, it seems Blizzard applies it's Cheating Death school absorb aura for 1 msec, but it's too late
			// for us by now.
			return;
		}
	}

	/*------------------------------------ DUEL HANDLERS --------------------------*/
	if((pVictim->IsPlayer()) && (IsPlayer()) && TO_PLAYER(pVictim)->DuelingWith == TO_PLAYER(this) ) //Both Players
	{
		if((health <= damage) && TO_PLAYER(this)->DuelingWith != NULL)
		{
			//Player in Duel and Player Victim has lost
			uint32 NewHP = pVictim->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100;

			if(NewHP < 5) 
				NewHP = 5;

			//Set there health to 1% or 5 if 1% is lower then 5
			pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, NewHP);
			//End Duel
			TO_PLAYER(this)->EndDuel(DUEL_WINNER_KNOCKOUT);
			TO_PLAYER(this)->GetAchievementInterface()->HandleAchievementCriteriaWinDuel();
			TO_PLAYER(pVictim)->GetAchievementInterface()->HandleAchievementCriteriaLoseDuel();

			// surrender emote
			pVictim->Emote(EMOTE_ONESHOT_BEG);			// Animation

			return;		  
		}
	}

	if((pVictim->IsPlayer()) && (IsPet())) 
	{
		if((health <= damage) && TO_PLAYER(pVictim)->DuelingWith == TO_PET(this)->GetPetOwner())
		{
			Player* petOwner = TO_PET(this)->GetPetOwner();
			if(petOwner)
			{
				//Player in Duel and Player Victim has lost
				uint32 NewHP = pVictim->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100;
				if(NewHP < 5) NewHP = 5;
				
				//Set there health to 1% or 5 if 1% is lower then 5
				pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, NewHP);
				//End Duel
				petOwner->EndDuel(DUEL_WINNER_KNOCKOUT);
				return;
			}
		}
	}
	/*------------------------------------ DUEL HANDLERS END--------------------------*/

	bool isCritter = false;
	if(pVictim->GetTypeId() == TYPEID_UNIT && TO_CREATURE(pVictim)->GetCreatureInfo())
	{
			if(TO_CREATURE(pVictim)->GetCreatureInfo()->Type == CRITTER)
				isCritter = true;
			else if(TO_CREATURE(pVictim)->proto)
			{
				//Dummy trainers can't die
				if(isTargetDummy(TO_CREATURE(pVictim)->proto->Id))
				{
					//Just limit to 5HP (can't use 1HP here).
					pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 5);
					return;
				}
			}
	}

	/* -------------------------- HIT THAT CAUSES VICTIM TO DIE ---------------------------*/
	if ((isCritter || health <= damage) )
	{

		if( pVictim->HasDummyAura(SPELL_HASH_GUARDIAN_SPIRIT) )
		{
			pVictim->CastSpell(pVictim, dbcSpell.LookupEntry(48153), true);
			pVictim->RemoveDummyAura(SPELL_HASH_GUARDIAN_SPIRIT);
			return;
		}

		//warlock - seed of corruption
		if( IsUnit() )
		{
			if( IsPlayer() && pVictim->IsUnit() && !pVictim->IsPlayer() && m_mapMgr->m_battleground && m_mapMgr->m_battleground->GetType() == BATTLEGROUND_ALTERAC_VALLEY )
				TO_ALTERACVALLEY(m_mapMgr->m_battleground)->HookOnUnitKill( TO_PLAYER(this), pVictim );
			SpellEntry *killerspell;
			if( spellId )
				killerspell = dbcSpell.LookupEntry( spellId );
			else killerspell = NULL;
			pVictim->HandleProc( PROC_ON_DIE, TO_UNIT(this), killerspell );
			pVictim->m_procCounter = 0;
			TO_UNIT(this)->HandleProc( PROC_ON_TARGET_DIE, pVictim, killerspell );
			TO_UNIT(this)->m_procCounter = 0;
		}
		// check if pets owner is combat participant
		bool owner_participe = false;
		if( IsPet() )
		{
			Player* owner = TO_PET(this)->GetPetOwner();
			if( owner != NULL && pVictim->GetAIInterface()->getThreatByPtr( owner ) > 0 )
				owner_participe = true;
		}
		/* victim died! */
		Unit* pKiller = pVictim->CombatStatus.GetKiller();
		if( pVictim->IsPlayer() )
		{
			// let's see if we have shadow of death
			if( !pVictim->FindPositiveAuraByNameHash(SPELL_HASH_SHADOW_OF_DEATH) && TO_PLAYER( pVictim)->HasSpell( 49157 )  && 
				!(TO_PLAYER(pVictim)->m_bg && TO_PLAYER(pVictim)->m_bg->IsArena())) //check for shadow of death
			{
				SpellEntry* sorInfo = dbcSpell.LookupEntry(54223);
				if( sorInfo != NULL && TO_PLAYER(pVictim)->Cooldown_CanCast( sorInfo ))
				{
					Spell* sor(new Spell( pVictim, sorInfo, false, NULL ));
					SpellCastTargets targets;
					targets.m_unitTarget = pVictim->GetGUID();
					sor->prepare(&targets);
					return;
				}
			}

			TO_PLAYER( pVictim )->KillPlayer();
			/* Remove all Auras */
			pVictim->EventDeathAuraRemoval();
			/* Set victim health to 0 */
			pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
			CALL_INSTANCE_SCRIPT_EVENT( m_mapMgr, OnPlayerDeath )( TO_PLAYER( pVictim ), pKiller );

			if( IsCreature() )
			{
				TO_PLAYER(pVictim)->GetAchievementInterface()->HandleAchievementCriteriaKilledByCreature( TO_CREATURE(this)->GetUInt32Value(OBJECT_FIELD_ENTRY) );
			}
			else if(IsPlayer())
			{
				TO_PLAYER(pVictim)->GetAchievementInterface()->HandleAchievementCriteriaKilledByPlayer();
			}
		}
		else
		{
			pVictim->setDeathState( JUST_DIED );
			/* Remove all Auras */
			pVictim->EventDeathAuraRemoval();
			/* Set victim health to 0 */
			pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
		}

		if( pVictim->IsPlayer() && (!IsPlayer() || pVictim == TO_UNIT(this) ) )
		{
			TO_PLAYER( pVictim )->DeathDurabilityLoss(0.10);
		}

		/* Zone Under Attack */
		MapInfo * pZMapInfo = NULL;
		pZMapInfo = WorldMapInfoStorage.LookupEntry(GetMapId());
		if( pZMapInfo != NULL && pZMapInfo->type == INSTANCE_NULL && !pVictim->IsPlayer() && !pVictim->IsPet() && ( IsPlayer() || IsPet() ) )
		{
			// Only NPCs that bear the PvP flag can be truly representing their faction.
			if( TO_CREATURE(pVictim)->IsPvPFlagged() )
			{
				Player* pAttacker = NULL;
				if( IsPet() )
					pAttacker = TO_PET(this)->GetPetOwner();
				else if(IsPlayer())
					pAttacker = TO_PLAYER(this);

				if( pAttacker != NULL)
                {
				    uint8 teamId = (uint8)pAttacker->GetTeam();
				    if(teamId == 0) // Swap it.
					    teamId = 1;
				    else
					    teamId = 0;
				    uint32 AreaID = pVictim->GetMapMgr()->GetAreaID(pVictim->GetPositionX(), pVictim->GetPositionY(), pVictim->GetPositionZ());
				    if(!AreaID)
					    AreaID = pAttacker->GetZoneId(); // Failsafe for a shitty TerrainMgr

				    if(AreaID)
				    {
					    WorldPacket data(SMSG_ZONE_UNDER_ATTACK, 4);
					    data << AreaID;
					    sWorld.SendFactionMessage(&data, teamId);
				    }
                }
			}
		}
		
		if(pVictim->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) > 0)
		{
			if(pVictim->GetCurrentSpell())
			{
				Spell* spl = pVictim->GetCurrentSpell();
				for(int i = 0; i < 3; i++)
				{
					if(spl->m_spellInfo->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
					{
						DynamicObject* dObj = GetMapMgr()->GetDynamicObject(pVictim->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
						if(!dObj)
							return;
						WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
						data << dObj->GetGUID();
						dObj->SendMessageToSet(&data, false);
						dObj->RemoveFromWorld(true);
						dObj->Destructor();
					}
				}
				if(spl->m_spellInfo->ChannelInterruptFlags == 48140) spl->cancel();
			}
		}

		if(pVictim->IsPlayer())
		{
			uint32 self_res_spell = 0;
			Player* plrVictim = TO_PLAYER(pVictim);
			if(!(plrVictim->m_bg && plrVictim->m_bg->IsArena())) // Can't self res in Arena
			{
				self_res_spell = plrVictim->SoulStone;
				plrVictim->SoulStone = plrVictim->SoulStoneReceiver = 0;

				if( !self_res_spell && plrVictim->bReincarnation )
				{
					SpellEntry* m_reincarnSpellInfo = dbcSpell.LookupEntry( 20608 );
					if( plrVictim->Cooldown_CanCast( m_reincarnSpellInfo ) )
					{
						uint32 ankh_count = plrVictim->GetItemInterface()->GetItemCount( 17030 );
					if( ankh_count || TO_PLAYER(plrVictim)->HasDummyAura(SPELL_HASH_GLYPH_OF_RENEWED_LIFE ))
						self_res_spell = 21169;
					}
				}
			}
			pVictim->SetUInt32Value( PLAYER_SELF_RES_SPELL, self_res_spell );
			pVictim->Dismount();
		}

		// Wipe our attacker set on death
		pVictim->CombatStatus.Vanished();


		/* Stop Units from attacking */
		if( plr && plr->IsInWorld() )
			plr->EventAttackStop();

		if( IsUnit() )
		{
			TO_UNIT(this)->smsg_AttackStop( pVictim );
		
			/* Tell Unit that it's target has Died */
			TO_UNIT(this)->addStateFlag( UF_TARGET_DIED );

		}
		if( pVictim->IsPlayer() )
		{
			if( TO_PLAYER( pVictim)->HasDummyAura(SPELL_HASH_SPIRIT_OF_REDEMPTION) ) //check for spirit of Redemption
			{
				SpellEntry* sorInfo = dbcSpell.LookupEntry(27827);
				if( sorInfo != NULL )
				{
					pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
					Spell* sor(new Spell( pVictim, sorInfo, true, NULL ));
					SpellCastTargets targets;
					targets.m_unitTarget = pVictim->GetGUID();
					sor->prepare(&targets);
				}
			}
		}
		/* -------------------------------- HONOR + BATTLEGROUND CHECKS ------------------------ */
		plr = NULL;
		if( IsPlayer() )
			plr = TO_PLAYER( this );
		else if( IsPet())
			plr = TO_PET( this )->GetPetOwner();

		if( plr != NULL)
		{
			if( plr->m_bg != NULL )
				plr->m_bg->HookOnPlayerKill( plr, pVictim );

			if( pVictim->IsPlayer() )
			{
				sHookInterface.OnKillPlayer( plr, TO_PLAYER( pVictim ) );
				HonorHandler::OnPlayerKilledUnit( plr, pVictim );
			}
			else
			{
				// REPUTATION
				if( !isCritter )
					plr->Reputation_OnKilledUnit( pVictim, false );
			}

			if(plr->getLevel() <= (pVictim->getLevel() + 8) && plr->getClass() == WARRIOR)
			{	// currently only warriors seem to use it (Victory Rush)
				plr->SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_VICTORIOUS );
				if( !sEventMgr.HasEvent(TO_UNIT(plr), EVENT_VICTORIOUS_FLAG_EXPIRE ) )
					sEventMgr.AddEvent( TO_UNIT(plr), &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_VICTORIOUS, EVENT_VICTORIOUS_FLAG_EXPIRE, 20000, 1, 0 );
				else
					sEventMgr.ModifyEventTimeLeft( TO_UNIT(plr), EVENT_VICTORIOUS_FLAG_EXPIRE, 20000 , false );
			}
		}
		/* -------------------------------- HONOR + BATTLEGROUND CHECKS END------------------------ */

		uint64 victimGuid = pVictim->GetGUID();

		// only execute loot code if we were tagged
		if( pVictim->GetTypeId() == TYPEID_UNIT && TO_CREATURE(pVictim)->m_taggingPlayer != 0 )
		{
			// fill loot vector
			TO_CREATURE(pVictim)->GenerateLoot();

			// update visual.
			TO_CREATURE(pVictim)->UpdateLootAnimation(plr);
		}

		// player loot for battlegrounds
		if( pVictim->GetTypeId() == TYPEID_PLAYER )
		{
			// set skinning flag, this is the "remove insignia"
			if( TO_PLAYER(pVictim)->m_bg != NULL && TO_PLAYER(pVictim)->m_bg->SupportsPlayerLoot() )
			{
				pVictim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
				TO_PLAYER(pVictim)->m_insigniaTaken = false;
			}
		}

		if(pVictim->GetTypeId() == TYPEID_UNIT)
		{
			//--------------------------------- POSSESSED CREATURES -----------------------------------------
			if( pVictim->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE) )
			{//remove possess aura from controller
				Player* vController = GetMapMgr()->GetPlayer( (uint32)pVictim->GetUInt64Value(UNIT_FIELD_CHARMEDBY) );
				if( vController )
				{
					if( vController->GetUInt64Value( UNIT_FIELD_CHARM ) )//make sure he is target controller
					{
						vController->UnPossess();
					}
				}
			}
			//--------------------------------- PARTY LOG -----------------------------------------
			pVictim->GetAIInterface()->OnDeath( this );
			if(GetTypeId() == TYPEID_PLAYER)
			{
				WorldPacket data(SMSG_PARTYKILLLOG, 16);
				data << GetGUID() << pVictim->GetGUID();
				SendMessageToSet(&data, true);
			}

			// it Seems that pets some how dont get a name and cause a crash here
			//bool isCritter = (pVictim->GetCreatureName() != NULL)? pVictim->GetCreatureName()->Type : 0;

			//---------------------------------looot-----------------------------------------  
			
			if( !pVictim->IsPet() && GetTypeId() == TYPEID_PLAYER && 
				pVictim->GetUInt64Value( UNIT_FIELD_CREATEDBY ) == 0 && 
				pVictim->GetUInt64Value( OBJECT_FIELD_CREATED_BY ) == 0 )
			{
				// TODO: lots of casts are bad make a temp member pointer to use for batches like this
				// that way no local loadhitstore and its just one assignment 

				//Not all NPC's give XP, check for it in proto no_XP
				bool can_give_xp = true;
				if(pVictim->IsCreature())
				{
					if(TO_CREATURE(pVictim)->GetProto())
						can_give_xp = (TO_CREATURE(pVictim)->GetProto()->no_xp ? false : true);
					else
						can_give_xp = false; // creatures without proto should not give any xp
				}
				if(can_give_xp)
				{
					// Is this player part of a group
					if( TO_PLAYER(this)->InGroup() )
					{
						//Calc Group XP
						TO_PLAYER(this)->GiveGroupXP( pVictim, TO_PLAYER(this) );
						//TODO: pet xp if player in group
					}
					else
					{
						uint32 xp = CalculateXpToGive( pVictim, TO_UNIT(this) );
						if( xp > 0 )
						{
							TO_PLAYER(this)->GiveXP( xp, victimGuid, true );
							if( TO_PLAYER(this)->GetSummon() && TO_PLAYER(this)->GetSummon()->GetUInt32Value( UNIT_CREATED_BY_SPELL ) == 0 )
							{
								xp = CalculateXpToGive( pVictim, TO_PLAYER(this)->GetSummon() );
								if( xp > 0 )
									TO_PLAYER(this)->GetSummon()->GiveXP( xp );
							}
						}
					}
				}

				// Achievement: on kill unit
				if( !pVictim->IsPlayer() && IsPlayer() )
				{
					Player* pThis = TO_PLAYER(this);
					pThis->GetAchievementInterface()->HandleAchievementCriteriaKillCreature( pVictim->GetUInt32Value(OBJECT_FIELD_ENTRY) );
				}

				if( pVictim->GetTypeId() != TYPEID_PLAYER )
					sQuestMgr.OnPlayerKill( TO_PLAYER(this), TO_CREATURE( pVictim ) );
			}
			else /* is Creature or GameObject* */
			{
				/* ----------------------------- PET XP HANDLING -------------- */
				if( owner_participe && IsPet() && !pVictim->IsPet() )
				{
					Player* petOwner = TO_PET(this)->GetPetOwner();
					if( petOwner != NULL && petOwner->GetTypeId() == TYPEID_PLAYER )
					{
						if( petOwner->InGroup() )
						{
							//Calc Group XP
							TO_UNIT(this)->GiveGroupXP( pVictim, petOwner );
							//TODO: pet xp if player in group
						}
						else
						{
							uint32 xp = CalculateXpToGive( pVictim, petOwner );
							if( xp > 0 )
							{
								petOwner->GiveXP( xp, victimGuid, true );
								if( !TO_PET(this)->IsSummonedPet() )
								{
									xp = CalculateXpToGive( pVictim, TO_PET(this) );
									if( xp > 0 )
										TO_PET(this)->GiveXP( xp );
								}
							}
						}
					}
					if( petOwner != NULL && pVictim->GetTypeId() != TYPEID_PLAYER && 
						pVictim->GetTypeId() == TYPEID_UNIT )
						sQuestMgr.OnPlayerKill( petOwner, TO_CREATURE( pVictim ) );
				}
				/* ----------------------------- PET XP HANDLING END-------------- */

				/* ----------------------------- PET DEATH HANDLING -------------- */
				if( pVictim->IsPet() )
				{
					// dying pet looses 1 happiness level
					if( !TO_PET( pVictim )->IsSummonedPet() )
					{
						uint32 hap = TO_PET( pVictim )->GetUInt32Value( UNIT_FIELD_POWER5 );
						hap = hap - PET_HAPPINESS_UPDATE_VALUE > 0 ? hap - PET_HAPPINESS_UPDATE_VALUE : 0;
						TO_PET( pVictim )->SetUInt32Value( UNIT_FIELD_POWER5, hap );
					}
					
					TO_PET( pVictim )->DelayedRemove( false, true );
					
					//remove owner warlock soul link from caster
					Player* owner = TO_PET( pVictim )->GetPetOwner();
					if( owner != NULL )
						owner->EventDismissPet();
				}
				/* ----------------------------- PET DEATH HANDLING END -------------- */
				else if( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) )
				{
					//remove owner warlock soul link from caster
					Unit* owner=pVictim->GetMapMgr()->GetUnit( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) );
					if( owner != NULL && owner->IsPlayer())
						TO_PLAYER( owner )->EventDismissPet();
				}
			}
		}
		else if( pVictim->GetTypeId() == TYPEID_PLAYER )
		{
			
			/* -------------------- RESET BREATH STATE ON DEATH -------------- */
			TO_PLAYER( pVictim )->m_UnderwaterTime = 0;
			TO_PLAYER( pVictim )->m_UnderwaterState = 0;
			TO_PLAYER( pVictim )->m_BreathDamageTimer = 0;
			TO_PLAYER( pVictim )->m_SwimmingTime = 0;

			/* -------------------- REMOVE PET WHEN PLAYER DIES ---------------*/
			if( TO_PLAYER( pVictim )->GetSummon() != NULL )
			{
				if( pVictim->GetUInt32Value( UNIT_CREATED_BY_SPELL ) > 0 )
					TO_PLAYER( pVictim )->GetSummon()->Dismiss( true );
				else
					TO_PLAYER( pVictim )->GetSummon()->Remove( true, true, true );
			}
			/* -------------------- REMOVE PET WHEN PLAYER DIES END---------------*/
		}
		else OUT_DEBUG("DealDamage for Unknown Object.");
	}
	else /* ---------- NOT DEAD YET --------- */
	{
		if(pVictim != TO_UNIT(this) /* && updateskill */)
		{
			// Send AI Reaction UNIT vs UNIT
			if( GetTypeId() ==TYPEID_UNIT )
			{
				TO_UNIT(this)->GetAIInterface()->AttackReaction( pVictim, damage, spellId );
			}
			
			// Send AI Victim Reaction
			if( IsPlayer() || GetTypeId() == TYPEID_UNIT )
			{
				if( pVictim->GetTypeId() != TYPEID_PLAYER )
				{
					TO_CREATURE( pVictim )->GetAIInterface()->AttackReaction( TO_UNIT(this), damage, spellId );
				}
			}

			if( IsUnit() && TO_UNIT( this )->HasDummyAura( SPELL_HASH_MARK_OF_BLOOD ) )
			{
				SpellEntry * tmpsp = NULL;
				tmpsp = dbcSpell.LookupEntry(50424);
				if(tmpsp != NULL)
					sEventMgr.AddEvent(TO_UNIT( this ), &Unit::EventCastSpell, pVictim, tmpsp, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); 
			}
			else if( IsPlayer() && spellId )
			{
				Player* plra = TO_PLAYER(this);
				SpellEntry *spentry = dbcSpell.LookupEntry( spellId );
				if( (spentry->NameHash == SPELL_HASH_CORRUPTION || spentry->NameHash == SPELL_HASH_UNSTABLE_AFFLICTION) && plra->HasDummyAura(SPELL_HASH_PANDEMIC) )
				{
					if( Rand( plra->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + 5) ) && pVictim )
					{
						Spell* sp(new Spell(this, dbcSpell.LookupEntry(58691), true, NULL));
						SpellCastTargets targets;
						targets.m_unitTarget = pVictim->GetGUID();
						sp->forced_basepoints[0] = float2int32(damage * ( plra->GetDummyAura(SPELL_HASH_PANDEMIC)->RankNumber * 0.33f + 0.01f));
						sp->prepare(&targets);
					}
				}
			}	
		}

		pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, health - damage );
	}
}

void Object::SpellNonMeleeDamageLog(Unit* pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage, bool no_remove_auras, uint32 AdditionalCritChance)
{
//==========================================================================================
//==============================Unacceptable Cases Processing===============================
//==========================================================================================
	if(!pVictim || !pVictim->isAlive())
		return;

	SpellEntry *spellInfo = dbcSpell.LookupEntry( spellID );
	if(!spellInfo)
        return;

	if (IsPlayer() && !TO_PLAYER(this)->canCast(spellInfo))
		return;
//==========================================================================================
//==============================Variables Initialization====================================
//========================================================================================== 
	uint32 school = spellInfo->School;
	float res = float(damage);
	uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_SPELL_LAND;
	uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_SPELL_HIT_VICTIM;
	bool critical = false;
	float dmg_reduction_pct;

	float res_after_spelldmg;

	Unit* caster = NULL;
	if( IsUnit() )
		caster = TO_UNIT(this);

//==========================================================================================
//==============================+Spell Damage Bonus Calculations============================
//==========================================================================================
//------------------------------by stats----------------------------------------------------
	if( IsUnit() && !static_damage )
	{
		caster->RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_START_ATTACK );

		res = caster->GetSpellBonusDamage( pVictim, spellInfo, ( int )res, false, false );
		

		// Aura 271 - Mods Damage for particular casters spells
		Unit::DamageTakenPctModPerCasterType::iterator it = 
			pVictim->DamageTakenPctModPerCaster.find(GetGUID());
		while(it != pVictim->DamageTakenPctModPerCaster.end() && 
			it->first == GetGUID())
		{
			if(spellInfo->SpellGroupType[0] & it->second.first[0] ||
				spellInfo->SpellGroupType[1] & it->second.first[1] ||
				spellInfo->SpellGroupType[2] & it->second.first[2])
			{
				res *= float(100 + it->second.second) / 100;
			}
			it++;
		}

		res_after_spelldmg = res;
//==========================================================================================
//==============================Post +SpellDamage Bonus Modifications=======================
//==========================================================================================
		if( res < 0 )
			res = 0;
		else if( spellInfo->spell_can_crit == true )
		{
//------------------------------critical strike chance--------------------------------------	
			// lol ranged spells were using spell crit chance
			float CritChance;
			if( spellInfo->is_ranged_spell || spellInfo->is_melee_spell )
			{

				if( IsPlayer() )
				{
					CritChance = GetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE );
					if( pVictim->IsPlayer() )
						CritChance += TO_PLAYER(pVictim)->res_R_crit_get();

					CritChance += (float)(pVictim->AttackerCritChanceMod[spellInfo->School]);
				}
				else
					CritChance = 5.0f; // static value for mobs.. not blizzlike, but an unfinished formula is not fatal :)

				CritChance += AdditionalCritChance;
				CritChance -= pVictim->IsPlayer() ? TO_PLAYER(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) : 0.0f;
				if( spellInfo->is_melee_spell )
					CritChance += (float)(pVictim->AttackerCritChanceMod[0]);
			}
			else
			{
				CritChance = caster->spellcritperc + caster->SpellCritChanceSchool[school] + pVictim->AttackerCritChanceMod[school];
				if( caster->IsPlayer() && ( pVictim->m_rooted - pVictim->m_stunned ) )	
					CritChance += TO_PLAYER( caster )->m_RootedCritChanceBonus;

				if( spellInfo->SpellGroupType )
				{
					SM_FFValue(caster->SM[SMT_CRITICAL][0], &CritChance, spellInfo->SpellGroupType);
					SM_PFValue(caster->SM[SMT_CRITICAL][1], &CritChance, spellInfo->SpellGroupType);
				}
				CritChance += AdditionalCritChance;
				if( pVictim->IsPlayer() )
					CritChance -= TO_PLAYER(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE );
			}
			if( CritChance < 0 ) CritChance = 0;
			if( CritChance > 95 ) CritChance = 95;
			critical = Rand(CritChance);
			//sLog.outString( "SpellNonMeleeDamageLog: Crit Chance %f%%, WasCrit = %s" , CritChance , critical ? "Yes" : "No" );
			Aura* fs = NULL;
			fs = spellInfo->NameHash == SPELL_HASH_LAVA_BURST ? pVictim->FindNegativeAuraByNameHash(SPELL_HASH_FLAME_SHOCK): NULL;
			if( fs != NULL)
			{
				critical = true;
				if(caster && !caster->HasAura(55447))	// Glyph of Flame Shock
					pVictim->RemoveAura(fs);
			}
//==========================================================================================
//==============================Spell Critical Hit==========================================
//==========================================================================================
			if (critical)
			{		
				int32 critical_bonus = 100;
				if( spellInfo->SpellGroupType )
					SM_FIValue( caster->SM[SMT_CRITICAL_DAMAGE][1], &critical_bonus, spellInfo->SpellGroupType );

				if( critical_bonus > 0 )
				{
					// the bonuses are halved by 50% (funky blizzard math :S)
					float b;
					if( spellInfo->School == 0 || spellInfo->is_melee_spell || spellInfo->is_ranged_spell )		// physical || hackfix SoCommand/JoCommand
						b = ( ( float(critical_bonus) ) / 100.0f ) + 1.0f;
					else
						b = ( ( float(critical_bonus) / 2.0f ) / 100.0f ) + 1.0f;

					res	*= b;

					if( pVictim->IsPlayer() )
					{
						dmg_reduction_pct = 2.2f * TO_PLAYER(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
						if( dmg_reduction_pct > 0.33f )
							dmg_reduction_pct = 0.33f; // 3.0.3

						res = res - res * dmg_reduction_pct;
					}			
				}

				pVictim->Emote( EMOTE_ONESHOT_WOUNDCRITICAL );
				aproc |= PROC_ON_SPELL_CRIT_HIT;
				vproc |= PROC_ON_SPELL_CRIT_HIT_VICTIM;
			}
		}
	}
//==========================================================================================
//==============================Post Roll Calculations======================================
//==========================================================================================
	// Special cases

	if( spellInfo->NameHash == SPELL_HASH_ICE_LANCE &&
	    (( pVictim->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN)) ||
		(caster && caster->m_frozenTargetCharges > 0))) 
	{
		res *= 3; //Ice Lance deals 3x damage if target is frozen
	}

	if (caster)
	{
		if (caster->m_frozenTargetCharges > 0 && spellInfo->School == SCHOOL_FROST)
		{
			caster->m_frozenTargetCharges--;
			if (caster->m_frozenTargetCharges <= 0)
				caster->RemoveAura(caster->m_frozenTargetId);
		}

		// [Mage] Hot Streak
		if (!(aproc & PROC_ON_SPELL_CRIT_HIT))
			caster->m_hotStreakCount = 0;

		if( aproc & PROC_ON_SPELL_CRIT_HIT && caster->HasDummyAura(SPELL_HASH_ECLIPSE))
		{
			if( caster->m_CustomTimers[CUSTOM_TIMER_ECLIPSE] <= getMSTime() )
			{
				caster->m_CustomTimers[CUSTOM_TIMER_ECLIPSE] = getMSTime() + MSTIME_SECOND*30;
				if( spellInfo->NameHash == SPELL_HASH_STARFIRE )
				{
					caster->CastSpell( caster, 48517, true );
				}else if( spellInfo->NameHash == SPELL_HASH_WRATH )
				{
					caster->CastSpell( caster, 48518, true );
				}
			}
		}
	}

//------------------------------absorption--------------------------------------------------	
	uint32 ress=(uint32)res;
	uint32 abs_dmg = pVictim->AbsorbDamage(this, school, &ress, dbcSpell.LookupEntryForced(spellID));
	uint32 ms_abs_dmg= pVictim->ManaShieldAbsorb(ress, dbcSpell.LookupEntryForced(spellID));
	if (ms_abs_dmg)
	{
		if(ms_abs_dmg > ress)
			ress = 0;
		else
			ress-=ms_abs_dmg;

		abs_dmg += ms_abs_dmg;
	}

	if(ress < 0) ress = 0;

	res=(float)ress;
	dealdamage dmg;
	dmg.school_type = school;
	dmg.full_damage = ress;
	dmg.resisted_damage = 0;
	
	if(res <= 0) 
		dmg.resisted_damage = dmg.full_damage;

	//------------------------------resistance reducing-----------------------------------------	
	float res_before_resist = res;
	if(res > 0 && IsUnit())
	{
		TO_UNIT(this)->CalculateResistanceReduction(pVictim,&dmg,spellInfo,0.0f);
		if((int32)dmg.resisted_damage > dmg.full_damage)
			res = 0;
		else
			res = float(dmg.full_damage - dmg.resisted_damage);
	}
	//------------------------------special states----------------------------------------------
	if(pVictim->GetTypeId() == TYPEID_PLAYER && TO_PLAYER(pVictim)->GodModeCheat == true)
	{
		res = 0;
		dmg.resisted_damage = dmg.full_damage;
	}
	
//==========================================================================================
//==============================Data Sending ProcHandling===================================
//==========================================================================================

	int32 ires = float2int32(res);

	//--------------------------split damage-----------------------------------------------
	// Paladin: Blessing of Sacrifice, and Warlock: Soul Link
	if( pVictim->m_damageSplitTarget.active)
	{
		ires = pVictim->DoDamageSplitTarget(ires, spellInfo->School, false);
	}

	SendSpellNonMeleeDamageLog(this, pVictim, spellID, ires, school, abs_dmg, dmg.resisted_damage, false, 0, critical, IsPlayer());

	if( ires > 0 )
	{
		// only deal damage if its >0
		DealDamage( pVictim, float2int32( res ), 2, 0, spellID );
	}
	else
	{
		// we still have to tell the combat status handler we did damage so we're put in combat
		if( IsUnit() )
			TO_UNIT(this)->CombatStatus.OnDamageDealt(pVictim, 1);
	}
	
	if( IsUnit() && allowProc && spellInfo->Id != 25501 )
	{
		pVictim->HandleProc( vproc, TO_UNIT(this), spellInfo, float2int32( res ) );
		pVictim->m_procCounter = 0;
		TO_UNIT(this)->HandleProc( aproc, pVictim, spellInfo, float2int32( res ) );
		TO_UNIT(this)->m_procCounter = 0;
	}
	if( IsPlayer() )
	{
		TO_PLAYER(this)->m_casted_amount[school] = ( uint32 )res;
	}

	if( (dmg.full_damage == 0 && abs_dmg) == 0 )
    {
        //Only pushback the victim current spell if it's not fully absorbed
        if( pVictim->GetCurrentSpell() )
            pVictim->GetCurrentSpell()->AddTime( school );
    }

//==========================================================================================
//==============================Post Damage Processing======================================
//==========================================================================================
	if( caster && (int32)dmg.resisted_damage == dmg.full_damage && !abs_dmg )
		caster->HandleProc(PROC_ON_FULL_RESIST, pVictim, spellInfo);

	if( school == SHADOW_DAMAGE )
	{
		if( IsPlayer() && TO_UNIT(this)->isAlive() && TO_PLAYER(this)->getClass() == PRIEST )
			TO_PLAYER(this)->VampiricSpell(float2int32(res), pVictim, dbcSpell.LookupEntry(spellID));

		if( pVictim->isAlive() && IsUnit() )
		{
			//Shadow Word: Death
			if( spellID == 32379 || spellID == 32996 || spellID == 48157 || spellID == 48158 ) 
			{
				uint32 damage = (uint32)( res + abs_dmg );

				if( TO_UNIT( this )->HasDummyAura(SPELL_HASH_PAIN_AND_SUFFERING) )
					damage += float2int32(damage * ((TO_UNIT( this )->GetDummyAura(SPELL_HASH_PAIN_AND_SUFFERING)->EffectBasePoints[1]+1) / 100.0f));

				uint32 absorbed = TO_UNIT(this)->AbsorbDamage(this, school, &damage, dbcSpell.LookupEntryForced(spellID) );
				DealDamage( TO_UNIT(this), damage, 2, 0, spellID );
				SendSpellNonMeleeDamageLog( this, TO_UNIT(this), spellID, damage, school, absorbed, 0, false, 0, false, IsPlayer() );
			}
			else if( spellInfo->NameHash == SPELL_HASH_HAUNT )
				caster->m_lastHauntInitialDamage = res;
		}
	}
}

//*****************************************************************************************
//* SpellLog packets just to keep the code cleaner and better to read
//*****************************************************************************************

void Object::SendSpellLog(Object* Caster, Object* Target, uint32 Ability, uint8 SpellLogType)
{
	if ((!Caster || !Target) && Ability)
		return;

	WorldPacket data(SMSG_SPELLLOGMISS,28);
	data << Ability;										// spellid
	data << Caster->GetGUID();							  // caster / player
	data << (uint8)1;									   // unknown but I think they are const
	data << (uint32)1;									  // unknown but I think they are const
	data << Target->GetGUID();							  // target
	data << SpellLogType;								   // spelllogtype
	Caster->SendMessageToSet(&data, true);
}


void Object::SendSpellNonMeleeDamageLog( Object* Caster, Unit* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToset )
{
	if ( !Caster || !Target )
		return;

	SpellEntry *sp = dbcSpell.LookupEntry(SpellID);
	if( !sp )
		return;

	SpellID = sp->logsId ? sp->logsId : sp->Id;
	uint32 overkill = Target->computeOverkill(Damage);

	WorldPacket data(SMSG_SPELLNONMELEEDAMAGELOG,40);
	data << Target->GetNewGUID();
	data << Caster->GetNewGUID();
	data << SpellID;                    // SpellID / AbilityID
	data << Damage;                     // All Damage
	data << uint32(overkill);			// Overkill
	data << uint8(g_spellSchoolConversionTable[School]);                     // School
	data << AbsorbedDamage;             // Absorbed Damage
	data << ResistedDamage;             // Resisted Damage
	data << uint8(PhysicalDamage);      // Physical Damage (true/false)
	data << uint8(0);                   // unknown or it binds with Physical Damage
	data << BlockedDamage;		     // Physical Damage (true/false)
	data << uint8(CriticalHit ? 7 : 5);                   // unknown const
	data << uint32(0);

	Caster->SendMessageToSet( &data, bToset );
}

int32 Object::event_GetInstanceID()
{
	// return -1 for non-inworld.. so we get our shit moved to the right thread
	if(!IsInWorld())
		return -1;
	else
		return m_instanceId;
}

void Object::EventSpellHit(Spell* pSpell)
{
	if( IsInWorld() && pSpell->m_caster != NULL )
		pSpell->cast(false);
	else
		delete pSpell;
}


bool Object::CanActivate()
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		{
			if(!IsPet())
				return true;
		}break;

	case TYPEID_GAMEOBJECT:
		{
			if(TO_GAMEOBJECT(this)->HasAI() && GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) != GAMEOBJECT_TYPE_TRAP)
				return true;
		}break;
	}

	return false;
}

void Object::Activate(MapMgr* mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		if(IsVehicle())
			mgr->activeVehicles.insert(TO_VEHICLE(this));
		else
			mgr->activeCreatures.insert(TO_CREATURE(this));
		break;

	case TYPEID_GAMEOBJECT:
		mgr->activeGameObjects.insert(TO_GAMEOBJECT(this));
		break;
	}

	Active = true;
}

void Object::Deactivate(MapMgr* mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		if(IsVehicle())
		{
			// check iterator
			if( mgr->__vehicle_iterator != mgr->activeVehicles.end() && (*mgr->__vehicle_iterator) == TO_VEHICLE(this) )
				++mgr->__vehicle_iterator;

			mgr->activeVehicles.erase(TO_VEHICLE(this));
		}
		else
		{
			// check iterator
			if( mgr->__creature_iterator != mgr->activeCreatures.end() && (*mgr->__creature_iterator) == TO_CREATURE(this) )
				++mgr->__creature_iterator;

			mgr->activeCreatures.erase(TO_CREATURE(this));
		}
		break;

	case TYPEID_GAMEOBJECT:
		// check iterator
		if( mgr->__gameobject_iterator != mgr->activeGameObjects.end() && (*mgr->__gameobject_iterator) == TO_GAMEOBJECT(this) )
			++mgr->__gameobject_iterator;

		mgr->activeGameObjects.erase(TO_GAMEOBJECT(this));
		break;
	}
	Active = false;
}

void Object::SetZoneId(uint32 newZone)
{
	m_zoneId = newZone;
	if( m_objectTypeId == TYPEID_PLAYER && TO_PLAYER(this)->GetGroup() )
		TO_PLAYER(this)->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_ZONEID, TO_PLAYER(this) );
}

void Object::PlaySoundToSet(uint32 sound_entry)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << sound_entry;
	SendMessageToSet(&data, true);
}

void Object::_SetExtension(const string& name, void* ptr)
{
	if( m_extensions == NULL )
		m_extensions = new ExtensionSet;

	m_extensions->insert( make_pair( name, ptr ) );
}


void Object::SendAttackerStateUpdate( Unit* Target, dealdamage *dmg, uint32 realdamage, uint32 abs, uint32 blocked_damage, uint32 hit_status, uint32 vstate )
{
	if (!Target || !dmg)
		return;

	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 70);

	if (hit_status & HITSTATUS_BLOCK)
	{
		hit_status|= 0x800000;
	}

	uint32 overkill = Target->computeOverkill(realdamage);

	data << (uint32)hit_status;   
	data << GetNewGUID();
	data << Target->GetNewGUID();

	data << (uint32)realdamage;			// Realdamage;
	data << (uint32)overkill;			// Overkill
	data << (uint8)1;					// Damage type counter / swing type

	data << (uint32)g_spellSchoolConversionTable[dmg->school_type];				  // Damage school
	data << (float)dmg->full_damage;		// Damage float
	data << (uint32)dmg->full_damage;	// Damage amount

	if(hit_status & HITSTATUS_ABSORBED)
	{
		data << (uint32)abs;				// Damage absorbed
	}
	if(hit_status & HITSTATUS_RESIST)
	{
		data << (uint32)dmg->resisted_damage;	// Damage resisted
	}

	data << (uint8)vstate;				// new victim state
	if(hit_status & HITSTATUS_BLOCK)
		data << (uint32)0;				// can be 0,1000 or -1
	else
		data << (uint32)0x3e8;			// can be 0,1000 or -1

	if (hit_status & 0x00800000)
	{
		data << (uint32)0;				// unknown
	}
	if(hit_status & HITSTATUS_BLOCK)
	{
		data << (uint32)blocked_damage;	 // Damage amount blocked
	}
	data << (uint32)0;					// Unknown

	SendMessageToSet(&data, IsPlayer());
}

void Object::SetByteFlag(const uint32 index, const uint32 flag, uint8 newFlag)
{
	if( HasByteFlag(index,flag,newFlag))
		return;

	SetByte(index, flag, GetByte(index,flag)|newFlag);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

void Object::RemoveByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag)
{
	if( !HasByteFlag(index,flag,checkFlag))
		return;

	SetByte(index,flag, GetByte(index,flag) & ~checkFlag );

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

bool Object::HasByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag)
{
	if( GetByte(index,flag) & checkFlag )
		return true;
	else
		return false;
}

bool Object::IsInLineOfSight(Object* pObj)
{
	if (GetMapMgr() && GetMapMgr()->IsCollisionEnabled())
		return (CollideInterface.CheckLOS( GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ() + 2.0f, pObj->GetPositionX(), pObj->GetPositionY(), pObj->GetPositionZ() + 2.0f) );
	else
		return true;
}

bool Object::PhasedCanInteract(Object* pObj)
{
	bool ret = false;

	if( m_phaseMode == ALL_PHASES || pObj->m_phaseMode == ALL_PHASES ) // -1: All phases. But perhaps this should be limited to GameObjects?
		ret = true;

	if( pObj->m_phaseMode & m_phaseMode || pObj->m_phaseMode == m_phaseMode )
		ret = true;

	Player* pObjI = IsPlayer() ? TO_PLAYER(this) : NULL;
	Player* pObjII = pObj->IsPlayer() ? TO_PLAYER(pObj) : NULL;
	if( IsPet() ) 
		pObjI = TO_PET(this)->GetPetOwner();
	if( pObj->IsPet() )
		pObjII = TO_PET(pObj)->GetPetOwner();

	// Hack for Acherus: Horde/Alliance can't see each other!
	if( pObjI && pObjII && GetMapId() == 609 && pObjI->GetTeam() != pObjII->GetTeam() )
	{
		return false;
	}

	return ret;
}

// Returns the base cost of a spell
int32 Object::GetSpellBaseCost(SpellEntry *sp)
{
	float cost;
	if( sp->ManaCostPercentage && IsUnit() )//Percentage spells cost % of !!!BASE!!! mana
	{
		if( sp->powerType==POWER_TYPE_MANA)
			cost = GetUInt32Value(UNIT_FIELD_BASE_MANA) * (sp->ManaCostPercentage / 100.0f);
		else
			cost = GetUInt32Value(UNIT_FIELD_BASE_HEALTH) * (sp->ManaCostPercentage / 100.0f);
	}
	else 
	{
		cost = (float)sp->manaCost;
	}

	return float2int32(cost); // Truncate zeh decimals!
}

void Object::CastSpell( Object* Target, SpellEntry* Sp, bool triggered )
{
	if( Sp == NULL )
		return;

	Spell* newSpell(new Spell(this, Sp, triggered, NULL));
	SpellCastTargets targets(0);
	if(Target)
	{
		if(Target->IsUnit())
		{
			targets.m_targetMask |= TARGET_FLAG_UNIT;
		}
		else
		{
			targets.m_targetMask |= TARGET_FLAG_OBJECT;
		}
		targets.m_unitTarget = Target->GetGUID();
	}
	else
	{
		newSpell->GenerateTargets(&targets);
	}
	newSpell->prepare(&targets);
}

void Object::CastSpell( Object* Target, uint32 SpellID, bool triggered )
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(Target, ent, triggered);
}

void Object::CastSpell( uint64 targetGuid, SpellEntry* Sp, bool triggered )
{
	if( Sp == NULL )
		return;

	SpellCastTargets targets(targetGuid);
	Spell* newSpell(new Spell(this, Sp, triggered, NULL));
	newSpell->prepare(&targets);
}

void Object::CastSpell( uint64 targetGuid, uint32 SpellID, bool triggered )
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(targetGuid, ent, triggered);
}