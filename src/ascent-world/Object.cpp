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
#include "Unit.h"
using namespace std;

//#define DEG2RAD (M_PI/180.0)
#define M_PI		3.14159265358979323846
#define M_H_PI		1.57079632679489661923
#define M_Q_PI		0.785398163397448309615

Object::Object() : m_position(0,0,0,0), m_spawnLocation(0,0,0,0)
{
	m_mapId = 0;
	m_zoneId = 0;

	m_uint32Values = 0;
	m_objectUpdated = false;


	m_valuesCount = 0;

	//official Values
	m_walkSpeed = 2.5f;
	m_runSpeed = 7.0f;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;

	m_flySpeed = 7.0f;
	m_backFlySpeed = 4.5f;

	m_backWalkSpeed = 4.5f;	// this should really be named m_backRunSpeed
	m_swimSpeed = 4.722222f;
	m_backSwimSpeed = 2.5f;
	m_turnRate = 3.141593f;

	m_mapMgr = 0;
	m_mapCell = 0;

	mSemaphoreTeleport = false;


	m_faction = NULL;
	m_factionDBC = NULL;

	m_instanceId = 0;
	Active = false;
	m_inQueue = false;
	m_extensions = NULL;
	m_loadedFromDB = false;
	m_loot.gold = 0;
	m_looted = false;
}

Object::~Object( )
{
	if(m_objectTypeId != TYPEID_ITEM)
		ASSERT(!m_inQueue);

	if (this->IsInWorld() && m_objectTypeId != TYPEID_ITEM && m_objectTypeId != TYPEID_CONTAINER)
	{
		this->RemoveFromWorld(false);
	}

	// for linux
	m_instanceId = -1;
	m_objectTypeId=TYPEID_UNUSED;
	
	if( m_extensions != NULL )
		delete m_extensions;
}


void Object::_Create( uint32 mapid, float x, float y, float z, float ang )
{
	m_mapId = mapid;
	m_position.ChangeCoords(x, y, z, ang);
	m_spawnLocation.ChangeCoords(x, y, z, ang);
	m_lastMapUpdatePosition.ChangeCoords(x,y,z,ang);
}

uint32 Object::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target)
{
	uint8 flags = 0;
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
		flags = 0x18;
		break;
		
		// player/unit: 0x68 (except self)
	case TYPEID_UNIT:
		flags = 0x70;
		break;

	case TYPEID_PLAYER:
		flags = 0x70;
		break;

		// gameobject/dynamicobject
	case TYPEID_GAMEOBJECT:
	case TYPEID_DYNAMICOBJECT:
	case TYPEID_CORPSE:
		flags = 0x58;
		break;

		// anyone else can get fucked and die!
	}

	if(target == this)
	{
		// player creating self
		flags |= 0x01;
		updatetype = UPDATETYPE_CREATE_YOURSELF;
	}

	// gameobject stuff
	if(m_objectTypeId == TYPEID_GAMEOBJECT)
	{
		switch(m_uint32Values[GAMEOBJECT_TYPE_ID])
		{
			case GAMEOBJECT_TYPE_MO_TRANSPORT:  
				{
					if(GetTypeFromGUID() != HIGHGUID_TYPE_TRANSPORTER)
						return 0;   // bad transporter
					else
						flags = 0x5A;
				}break;

			case GAMEOBJECT_TYPE_TRANSPORT:
				{
					/* deeprun tram, etc */
					flags = 0x5A;
				}break;

			case GAMEOBJECT_TYPE_DUEL_ARBITER:
				{
					// duel flags have to stay as updatetype 3, otherwise
					// it won't animate
					updatetype = UPDATETYPE_CREATE_YOURSELF;
				}break;
		}
	}

	// build our actual update
	*data << updatetype;

	// we shouldn't be here, under any cercumstances, unless we have a wowguid..
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
	*packet << (uint8)0;//unknown

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
	//ByteBuffer buf(500);
	/*uint8 rbuf[1000];
	StackBuffer buf(rbuf, 1000);
	buf << uint8(UPDATETYPE_VALUES);
	buf << GetNewGUID();

	uint32 mBlocks = Index/32+1;
	buf << (uint8)mBlocks;

	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
		buf <<(uint32)0;

	buf <<(((uint32)(1))<<(Index%32));
	buf << Value;

	Target->PushUpdateData(&buf, 1);*/

	uint8 rbuf[1000];
	StackPacket buf(SMSG_UPDATE_OBJECT, rbuf, 1000);
	buf << uint32(1);
	buf << uint8(0);

	buf << uint8(UPDATETYPE_VALUES);
	buf << GetNewGUID();

	uint32 mBlocks = Index/32+1;
	buf << (uint8)mBlocks;

	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
		buf <<(uint32)0;

	buf <<(((uint32)(1))<<(Index%32));
	buf << Value;

	Target->GetSession()->SendPacket(&buf);
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

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer *data, Player *target)
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

	_BuildValuesUpdate( buf, mask, 0 );

	// 1 update.
	return 1;
}

void Object::DestroyForPlayer(Player *target) const
{
	if(target->GetSession() == 0) return;

	ASSERT(target);

	WorldPacket data(SMSG_DESTROY_OBJECT, 8);
	data << GetGUID();

	target->GetSession()->SendPacket( &data );
}


///////////////////////////////////////////////////////////////
/// Build the Movement Data portion of the update packet
/// Fills the data with this object's movement/speed info
/// TODO: rewrite this stuff, document unknown fields and flags
uint32 TimeStamp();

void Object::_BuildMovementUpdate(ByteBuffer * data, uint8 flags, uint32 flags2, Player* target )
{
	ByteBuffer *splinebuf = (m_objectTypeId == TYPEID_UNIT) ? target->GetAndRemoveSplinePacket(GetGUID()) : 0;
	*data << (uint8)flags;

	Player * pThis = 0;
	if(m_objectTypeId == TYPEID_PLAYER)
	{
		pThis = static_cast< Player* >( this );
		if(target == this)
		{
			// Updating our last speeds.
			pThis->UpdateLastSpeeds();
		}
	}

	if (flags & 0x20)
	{
		if(pThis && pThis->m_TransporterGUID != 0)
			flags2 |= 0x200;
		else if(m_objectTypeId==TYPEID_UNIT && ((Creature*)this)->m_transportGuid != 0 && ((Creature*)this)->m_transportPosition != NULL)
			flags2 |= 0x200;

		if(splinebuf)
		{
			flags2 |= 0x08000001;	   //1=move forward
			if(GetTypeId() == TYPEID_UNIT)
			{
				if(static_cast<Unit*>(this)->GetAIInterface()->m_moveRun == false)
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
		
			if(static_cast<Unit*>(this)->GetAIInterface()->IsFlying())
//				flags2 |= 0x800; //in 2.3 this is some state that i was not able to decode yet
				flags2 |= 0x400; //Zack : Teribus the Cursed had flag 400 instead of 800 and he is flying all the time 
			if(static_cast<Creature*>(this)->proto && static_cast<Creature*>(this)->proto->extra_a9_flags)
			{
				if(!(flags2 & 0x0200))
					flags2 |= static_cast<Creature*>(this)->proto->extra_a9_flags;
			}
/*			if(GetGUIDHigh() == HIGHGUID_WAYPOINT)
			{
				if(GetUInt32Value(UNIT_FIELD_STAT0) == 768)		// flying waypoint
					flags2 |= 0x800;
			}*/
		}

		*data << (uint32)flags2;

		*data << (uint8)0;

		*data << getMSTime(); // this appears to be time in ms but can be any thing

		// this stuff:
		//   0x01 -> Enable Swimming?
		//   0x04 -> ??
		//   0x10 -> disables movement compensation and causes players to jump around all the place
		//   0x40 -> disables movement compensation and causes players to jump around all the place

		/*static uint8 fl = 0x04;
		*data << uint8(fl);		// wtf? added in 2.3.0*/
		/*if(target==this)
			*data<<uint8(0x53);
		else
			*data<<uint8(0);*/
		//*data << uint8(0x1);
	}

	if (flags & 0x40)
	{
		if(flags & 0x2)
		{
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
			*data << (float)m_position.o;
		}
		else
		{
			*data << m_position;
			*data << m_position.o;
		}

		if(flags & 0x20 && flags2 & 0x0200)
		{
			if(pThis)
			{
				*data << pThis->m_TransporterGUID;
				*data << pThis->m_TransporterX << pThis->m_TransporterY << pThis->m_TransporterZ << pThis->m_TransporterO;
				*data << pThis->m_TransporterUnk;
			}
			else if(m_objectTypeId==TYPEID_UNIT && ((Creature*)this)->m_transportPosition != NULL)
			{
				*data << ((Creature*)this)->m_transportGuid;
				*data << uint32(HIGHGUID_TYPE_TRANSPORTER);
				*data << ((Creature*)this)->m_transportPosition->x << ((Creature*)this)->m_transportPosition->y << 
					((Creature*)this)->m_transportPosition->z << ((Creature*)this)->m_transportPosition->o;
				*data << float(0.0f);
			}
		}
	}

	if (flags & 0x20)
	{
		*data << (uint32)0;
	}

	if (flags & 0x20 && flags2 & 0x2000)
	{
		*data << (float)0;
		*data << (float)1.0;
		*data << (float)0;
		*data << (float)0;
	}

	if (flags & 0x20)
	{
		*data << m_walkSpeed;	 // walk speed
		*data << m_runSpeed;	  // run speed
		*data << m_backWalkSpeed; // backwards walk speed
		*data << m_swimSpeed;	 // swim speed
		*data << m_backSwimSpeed; // backwards swim speed
		*data << m_flySpeed;		// fly speed
		*data << m_backFlySpeed;	// back fly speed
		*data << m_turnRate;	  // turn rate
	}

	if(splinebuf)
	{
		data->append(*splinebuf);
		delete splinebuf;
	}

	if(flags & 0x8)
	{
		*data << GetUInt32Value(OBJECT_FIELD_GUID);
		if(flags & 0x10)
			*data << GetUInt32Value(OBJECT_FIELD_GUID_01);
	}
	else if(flags & 0x10)
		*data << GetUInt32Value(OBJECT_FIELD_GUID);

	if(flags & 0x2)
	{
		if(target)
		{
			/*int32 m_time = TimeStamp() - target->GetSession()->m_clientTimeDelay;
			m_time += target->GetSession()->m_moveDelayTime;
			*data << m_time;*/
			*data << getMSTime();
		}
		else
            *data << getMSTime();
	}
}

//=======================================================================================
//  Creates an update block with the values of this object as
//  determined by the updateMask.
//=======================================================================================
void Object::_BuildValuesUpdate(ByteBuffer * data, UpdateMask *updateMask, Player* target)
{
	bool reset = false;

	if(updateMask->GetBit(OBJECT_FIELD_GUID) && target)	   // We're creating.
	{
		Creature *pThis = TO_CREATURE(this);
		if(m_objectTypeId == TYPEID_UNIT && pThis->m_taggingPlayer)		// tagged group will have tagged player
		{
			// set tagged visual
			if( (pThis->m_taggingGroup != 0 && target->m_playerInfo->m_Group != NULL && target->m_playerInfo->m_Group->GetID() == pThis->m_taggingGroup) ||
				(pThis->m_taggingPlayer == target->GetLowGUID()) )
			{
				m_uint32Values[UNIT_DYNAMIC_FLAGS] |= U_DYN_FLAG_TAPPED_BY_PLAYER;
				if( pThis->m_loot.HasItems() )
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
			GameObject *go = ((GameObject*)this);
			GameObjectInfo *info;
			info = go->GetInfo();
			if(info->InvolvedQuestCount && info->InvolvedQuestIds[0])
			{
				for(uint32 v = 0; v < info->InvolvedQuestCount; ++v)
				{
					if( target->GetQuestLogForEntry(info->InvolvedQuestIds[v]) != NULL )
					{
						m_uint32Values[GAMEOBJECT_DYN_FLAGS] = GO_DYNFLAG_QUEST;
						m_uint32Values[GAMEOBJECT_STATE] = 1;
						m_uint32Values[GAMEOBJECT_FLAGS] &= ~GO_FLAG_IN_USE;

						updateMask->SetBit(GAMEOBJECT_STATE);
						updateMask->SetBit(GAMEOBJECT_DYN_FLAGS);

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
	  
	for( uint32 index = 0; index < values_count; index ++ )
	{
		if( updateMask->GetBit( index ) )
		{
			switch(index)
			{
			case UNIT_FIELD_MAXHEALTH:
				{
					if(m_valuesCount < UNIT_END)
						*data << m_uint32Values[index];
					else
					{
						switch(m_objectTypeId)
						{
						case TYPEID_PLAYER:
							*data << m_uint32Values[index];
						break;

						case TYPEID_UNIT:
							{
								if(IsPet())
								{
									*data << m_uint32Values[index];
									break;
								}
								else
								{
									*data << (uint32)100;	
								}
							}
						}
					}
				}
				break;
			case UNIT_FIELD_HEALTH:
				{
					if(m_valuesCount < UNIT_END)
						*data << m_uint32Values[index];
					else
					{
						switch(m_objectTypeId)
						{
						case TYPEID_PLAYER:
							*data << m_uint32Values[index];
						break;

						case TYPEID_UNIT:
							{
								if(IsPet())
								{
									*data << m_uint32Values[index];
									break;
								}
								else
								{
									uint32 pct = uint32(float( float(m_uint32Values[index]) / float(m_uint32Values[UNIT_FIELD_MAXHEALTH]) * 100.0f));

									/* fix case where health value got rounded down and the client sees health as dead */
									if(!pct && m_uint32Values[UNIT_FIELD_HEALTH] != 0)
										++pct;
									*data << pct;	
								}
							}
						}
					}
				}
				break;

			default:
				*data << m_uint32Values[ index ];
				break;
			}
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
			m_uint32Values[GAMEOBJECT_DYN_FLAGS] = 0;
			m_uint32Values[GAMEOBJECT_FLAGS] |= GO_FLAG_IN_USE;
			m_uint32Values[GAMEOBJECT_STATE] = 0;
			break;
		}
	}

}

void Object::BuildHeartBeatMsg(WorldPacket *data) const
{
	data->Initialize(MSG_MOVE_HEARTBEAT);

	*data << GetGUID();

	*data << uint32(0); // flags
	*data << uint32(0); // mysterious value #1

	*data << m_position;
	*data << m_position.o;
}

WorldPacket * Object::BuildTeleportAckMsg(const LocationVector & v)
{
	///////////////////////////////////////
	//Update player on the client with TELEPORT_ACK
	if( IsInWorld() )		// only send when inworld
		static_cast< Player* >( this )->SetPlayerStatus( TRANSFER_PENDING );

	WorldPacket * data = new WorldPacket(MSG_MOVE_TELEPORT_ACK, 80);
	*data << GetNewGUID();

	//First 4 bytes = no idea what it is
	*data << uint32(2); // flags
	*data << uint32(0); // mysterious value #1
	*data << uint8(0);

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
		DEBUG_LOG( "setPosition: fell through map; height ported" );

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
		DEBUG_LOG( "setPosition: fell through map; height ported" );

		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_lastMapUpdatePosition.ChangeCoords(newX,newY,newZ,newOrientation);
		m_mapMgr->ChangeObjectLocation(this);

		if( m_objectTypeId == TYPEID_PLAYER && static_cast< Player* >( this )->GetGroup() && static_cast< Player* >( this )->m_last_group_position.Distance2DSq(m_position) > 25.0f ) // distance of 5.0
		{
            static_cast< Player* >( this )->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_POSITION, static_cast< Player* >( this ) );
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
		static_cast< Player* >( this )->GetSession()->OutPacket(Opcode, Len, Data);

	if(!IsInWorld())
		return;

	std::set<Player*>::iterator itr = m_inRangePlayers.begin();
	std::set<Player*>::iterator it_end = m_inRangePlayers.end();
	int gm = ( m_objectTypeId == TYPEID_PLAYER ? static_cast< Player* >( this )->m_isGmInvisible : 0 );
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
		static_cast< Player* >( this )->GetSession()->SendPacket(data);		
	}

	if(!IsInWorld())
		return;

	std::set<Player*>::iterator itr = m_inRangePlayers.begin();
	std::set<Player*>::iterator it_end = m_inRangePlayers.end();
	bool gminvis = (m_objectTypeId == TYPEID_PLAYER ? static_cast< Player* >( this )->m_isGmInvisible : false);
	//Zehamster: Splitting into if/else allows us to avoid testing "gminvis==true" at each loop...
	//		   saving cpu cycles. Chat messages will be sent to everybody even if player is invisible.
	if(myteam_only)
	{
		uint32 myteam=static_cast< Player* >( this )->GetTeam();
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetSession()->GetPermissionCount() > 0 && (*itr)->GetTeam()==myteam)
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if((*itr)->GetTeam()==myteam)
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
				if((*itr)->GetSession()->GetPermissionCount() > 0)
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
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

void Object::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
	*updateMask = m_updateMask;
}


void Object::_SetCreateBits(UpdateMask *updateMask, Player *target) const
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
	MapMgr *mapMgr = sInstanceMgr.GetInstance(this);
	if(!mapMgr)
		return; //instance add failed

	if( IsPlayer() )
	{
		// battleground checks
		Player *p = TO_PLAYER(this);
		if( p->m_bg == NULL && mapMgr->m_battleground != NULL )
		{
			// player hasn't been registered in the battleground, ok.
			// that means we re-logged into one. if it's an arena, don't allow it!
			// also, don't allow them in if the bg is full.

			if( /*( mapMgr->m_battleground->IsArena() && mapMgr->m_battleground->HasStarted() ) ||*/
				( !mapMgr->m_battleground->CanPlayerJoin(p) ) && !p->bGMTagOn )	// above check isn't needed, done in Arena::CanPlayerJoin.
			{
				//p->EjectFromInstance();
				return;
			}
		}
	}

	m_mapMgr = mapMgr;
	m_inQueue = true;

	mapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = m_mapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

void Object::AddToWorld(MapMgr * pMapMgr)
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
void Object::PushToWorld(MapMgr*mgr)
{
	if(!mgr/* || (m_mapMgr != NULL && m_mapCell != NULL) */)
		return; //instance add failed

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

	ASSERT(m_mapMgr);
	MapMgr * m = m_mapMgr;
	m_mapMgr = 0;

	mSemaphoreTeleport = true;

	m->RemoveObject(this, free_guid);
	
	// remove any spells / free memory
	sEventMgr.RemoveEvents(this, EVENT_UNIT_SPELL_HIT);

	// update our event holder
	event_Relocate();
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
			Group* pGroup = static_cast< Player* >( this )->GetGroup();
			if( pGroup != NULL )
				pGroup->HandleUpdateFieldChange( index, static_cast< Player* >( this ) );
		}

#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			static_cast< Player* >( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			static_cast< Player* >( this )->save_Gold();
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
#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			static_cast< Player* >( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			static_cast< Player* >( this )->save_Gold();
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
			static_cast< Player* >( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			static_cast< Player* >( this )->save_Gold();
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
}

////////////////////////////////////////////////////////////

float Object::CalcDistance(Object *Ob)
{
	return CalcDistance(this->GetPositionX(), this->GetPositionY(), this->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(float ObX, float ObY, float ObZ)
{
	return CalcDistance(this->GetPositionX(), this->GetPositionY(), this->GetPositionZ(), ObX, ObY, ObZ);
}
float Object::CalcDistance(Object *Oa, Object *Ob)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(Object *Oa, float ObX, float ObY, float ObZ)
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
	//DEBUG_LOG("Orientation: %f Angle: %f LeftBorder: %f RightBorder %f",Orientation,angle,lborder,rborder);
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
	// check if we are behind something ( is the object within a 180 degree slice of our negative y axis )

    double x = m_position.x - target->GetPositionX();
    double y = m_position.y - target->GetPositionY();

    double angle = atan2( y, x );
    angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;

	// if we are a unit and have a UNIT_FIELD_TARGET then we are always facing them
	if( m_objectTypeId == TYPEID_UNIT && m_uint32Values[UNIT_FIELD_TARGET] != 0 && static_cast< Unit* >( this )->GetAIInterface()->GetNextTarget() )
	{
		Unit* pTarget = static_cast< Unit* >( this )->GetAIInterface()->GetNextTarget();
		angle -= double( Object::calcRadAngle( target->m_position.x, target->m_position.y, pTarget->m_position.x, pTarget->m_position.y ) );
	}
	else
		angle -= target->GetOrientation();

    while( angle > M_PI)
        angle -= 2.0 * M_PI;

    while(angle < -M_PI)
        angle += 2.0 * M_PI;

	// replace M_H_PI in the two lines below to reduce or increase angle

    double left = -1.0 * ( M_H_PI / 2.0 );
    double right = ( M_H_PI / 2.0 );

    return( ( angle <= left ) && ( angle >= right ) );
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
	if( this->GetTypeId() != TYPEID_UNIT )
		return false;

	if( static_cast< Unit* >( this )->m_isPet && m_uint32Values[UNIT_FIELD_CREATEDBY] != 0 && m_uint32Values[UNIT_FIELD_SUMMONEDBY] != 0 )
		return true;

	return false;
}

void Object::_setFaction()
{
	FactionTemplateDBC* factT = NULL;

	if(GetTypeId() == TYPEID_UNIT || GetTypeId() == TYPEID_PLAYER)
	{
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	}
	else
	if(GetTypeId() == TYPEID_GAMEOBJECT)
	{
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(GAMEOBJECT_FACTION));
	}

	if(!factT)
	{
		return;
	}
	m_faction = factT;
	m_factionDBC = dbcFaction.LookupEntry(factT->Faction);
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

void Object::DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras)
{
	Player* plr = 0;

	if( !pVictim || !pVictim->isAlive() || !pVictim->IsInWorld() || !IsInWorld() )
		return;
	if( pVictim->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >( pVictim )->GodModeCheat == true )
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
		if( isHostile( this, pVictim ) && static_cast< Player* >( pVictim )->DuelingWith != static_cast< Player* >( this ) )
			static_cast< Player* >( this )->SetPvPFlag();
	}
	//If our pet attacks  - flag us.
	if( pVictim->IsPlayer() && IsPet() )
	{
		Player* owner = static_cast< Player* >( static_cast< Pet* >( this )->GetPetOwner() );
		if( owner != NULL )
			if( owner->isAlive() && static_cast< Player* >( pVictim )->DuelingWith != owner )
				owner->SetPvPFlag();		
	}

	if(!no_remove_auras)
	{
		//zack 2007 04 24 : root should not remove self (and also other unknown spells)
		if(spellId)
		{
			pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN,spellId);
			if(Rand(35.0f))
				pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_UNUSED2,spellId);
		}
		else
		{
			pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);
			if(Rand(35.0f))
				pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_UNUSED2);
		}
	}

	if(this->IsUnit())
	{
/*		if(!pVictim->isInCombat() && pVictim->IsPlayer())
			sHookInterface.OnEnterCombat( static_cast< Player* >( pVictim ), static_cast< Unit* >( this ) );

		if(IsPlayer() && ! static_cast< Player* >( this )->isInCombat())
			sHookInterface.OnEnterCombat( static_cast< Player* >( this ), static_cast< Player* >( this ) );*/

		//the black sheep , no actually it is paladin : Ardent Defender
		if(static_cast<Unit*>(this)->DamageTakenPctModOnHP35 && HasFlag(UNIT_FIELD_AURASTATE , AURASTATE_FLAG_HEALTH35) )
			damage = damage - float2int32(damage * static_cast<Unit*>(this)->DamageTakenPctModOnHP35) / 100 ;
			
		if(IsPet())
			plr = static_cast<Pet*>(this)->GetPetOwner();
		else if(IsPlayer())
			plr = static_cast< Player* >( this );

		if(plr != NULL && plr->GetTypeId() == TYPEID_PLAYER && pVictim->GetTypeId() == TYPEID_UNIT) // Units can't tag..
			TO_CREATURE(pVictim)->Tag(plr);

		if( pVictim != this )
		{
			// Set our attack target to the victim.
			static_cast< Unit* >( this )->CombatStatus.OnDamageDealt( pVictim, damage );
		}
	}

        ///Rage
        float val;

		if( pVictim->GetPowerType() == POWER_TYPE_RAGE 
			&& pVictim != this
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
		}

	if( pVictim->IsPlayer() )
	{
		Player *pThis = static_cast< Player* >(pVictim);
		if(pThis->cannibalize)
		{
			sEventMgr.RemoveEvents(pVictim, EVENT_CANNIBALIZE);
			pThis->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			pThis->cannibalize = false;
		}
	}

	//* BATTLEGROUND DAMAGE COUNTER *//
	if( pVictim != this )
	{
		if( IsPlayer() )
		{
			plr = static_cast< Player* >( this );
		}
		else if( IsPet() )
		{
			plr = static_cast< Pet* >( this )->GetPetOwner();
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

	if(health <= damage && pVictim->IsPlayer() && pVictim->getClass() == ROGUE && TO_PLAYER(pVictim)->m_lastCheatDeath + 60000 < (uint32)UNIXTIME)
	{
		Player * plrVictim = TO_PLAYER(pVictim);
		uint32 rank = plrVictim->m_cheatDeathRank;

		uint32 chance = rank == 3 ? 100 : rank * 33;
		if(Rand(chance))
		{
			// Proc that cheating death!
			SpellEntry *spellInfo = dbcSpell.LookupEntry(45182);
			Spell *spell = new Spell(pVictim,spellInfo,true,NULL);
			SpellCastTargets targets;
			targets.m_unitTarget = pVictim->GetGUID();
			spell->prepare(&targets);
			TO_PLAYER(pVictim)->m_lastCheatDeath = (uint32)UNIXTIME;

			// Why return? So this damage isn't counted. ;)
			// On official, it seems Blizzard applies it's Cheating Death school absorb aura for 1 msec, but it's too late
			// for us by now.
			return;
		}
	}

	/*------------------------------------ DUEL HANDLERS --------------------------*/
	if((pVictim->IsPlayer()) && (this->IsPlayer()) && static_cast< Player* >(pVictim)->DuelingWith == static_cast< Player* >( this ) ) //Both Players
	{
		if((health <= damage) && static_cast< Player* >( this )->DuelingWith != NULL)
		{
			//Player in Duel and Player Victim has lost
			uint32 NewHP = pVictim->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100;

			if(NewHP < 5) 
				NewHP = 5;

			//Set there health to 1% or 5 if 1% is lower then 5
			pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, NewHP);
			//End Duel
			static_cast< Player* >( this )->EndDuel(DUEL_WINNER_KNOCKOUT);

			// surrender emote
			pVictim->Emote(EMOTE_ONESHOT_BEG);			// Animation

			return;		  
		}
	}

	if((pVictim->IsPlayer()) && (IsPet())) 
	{
		if((health <= damage) && static_cast< Player* >(pVictim)->DuelingWith == static_cast<Pet*>(this)->GetPetOwner())
		{
			Player *petOwner = static_cast<Pet*>(this)->GetPetOwner();
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
	if(pVictim->GetTypeId() == TYPEID_UNIT && ((Creature*)pVictim)->GetCreatureName())
	{
			if(((Creature*)pVictim)->GetCreatureName()->Type == CRITTER)
				isCritter = true;
	}

	/* ------------------------------- DAMAGE BUG DETECTION -------------------------------*/
	//ASSERT(damage <= 60000);
	if( IsPlayer() && damage >= 60000 && !TO_PLAYER(this)->GetSession()->HasGMPermissions() )
	{
		TO_PLAYER(this)->BroadcastMessage("Attempting to deal %u damage, we are not allowing this. If this problem persists, please contact Burlex. If you have any information on reproducing this problem, it would be very helpful.", damage);
		return;
	}

	/* -------------------------- HIT THAT CAUSES VICTIM TO DIE ---------------------------*/
	if ((isCritter || health <= damage) )
	{
		//warlock - seed of corruption
		if( IsUnit() )
		{
			if( IsPlayer() && pVictim->IsUnit() && !pVictim->IsPlayer() && m_mapMgr->m_battleground && m_mapMgr->m_battleground->GetType() == BATTLEGROUND_ALTERAC_VALLEY )
				static_cast<AlteracValley*>(m_mapMgr->m_battleground)->HookOnUnitKill( TO_PLAYER(this), pVictim );
			SpellEntry *killerspell;
			if( spellId )
				killerspell = dbcSpell.LookupEntry( spellId );
			else killerspell = NULL;
			pVictim->HandleProc( PROC_ON_DIE, static_cast< Unit* >( this ), killerspell );
			pVictim->m_procCounter = 0;
			static_cast< Unit* >( this )->HandleProc( PROC_ON_TARGET_DIE, pVictim, killerspell );
			static_cast< Unit* >( this )->m_procCounter = 0;
		}
		// check if pets owner is combat participant
		bool owner_participe = false;
		if( IsPet() )
		{
			Player* owner = static_cast<Pet*>( this )->GetPetOwner();
			if( owner != NULL && pVictim->GetAIInterface()->getThreatByPtr( owner ) > 0 )
				owner_participe = true;
		}
		/* victim died! */
		if( pVictim->IsPlayer() )
			static_cast< Player* >( pVictim )->KillPlayer();
		else
		{
			pVictim->setDeathState( JUST_DIED );
			pVictim->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, static_cast< Unit* >( this ), 0);
		}

		if( pVictim->IsPlayer() && (!IsPlayer() || pVictim == this ) )
		{
			static_cast< Player* >( pVictim )->DeathDurabilityLoss(0.10);
		}

		/* Zone Under Attack */
        MapInfo * pMapInfo = WorldMapInfoStorage.LookupEntry(GetMapId());
        if( pMapInfo && pMapInfo->type == INSTANCE_NULL && !pVictim->IsPlayer() && !pVictim->IsPet() && ( IsPlayer() || IsPet() ) )
		{
			// Only NPCs that bear the PvP flag can be truly representing their faction.
			if( ((Creature*)pVictim)->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PVP ) )
			{
				Player * pAttacker = NULL;
				if( IsPet() )
					pAttacker = static_cast< Pet* >( this )->GetPetOwner();
				else if(IsPlayer())
					pAttacker = static_cast< Player* >( this );

				if( pAttacker != NULL)
                {
				    uint8 teamId = (uint8)pAttacker->GetTeam();
				    if(teamId == 0) // Swap it.
					    teamId = 1;
				    else
					    teamId = 0;
				    uint32 AreaID = pVictim->GetMapMgr()->GetAreaID(pVictim->GetPositionX(), pVictim->GetPositionY());
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
				Spell *spl = pVictim->GetCurrentSpell();
				for(int i = 0; i < 3; i++)
				{
					if(spl->m_spellInfo->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
					{
						DynamicObject *dObj = GetMapMgr()->GetDynamicObject(pVictim->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
						if(!dObj)
							return;
						WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
						data << dObj->GetGUID();
						dObj->SendMessageToSet(&data, false);
						dObj->RemoveFromWorld(true);
						delete dObj;
					}
				}
				if(spl->m_spellInfo->ChannelInterruptFlags == 48140) spl->cancel();
			}
		}
		
		/* Remove all Auras */
		pVictim->DropAurasOnDeath();

		/* Stop victim from attacking */
		if( this->IsUnit() )
			pVictim->smsg_AttackStop( static_cast< Unit* >( this ) );

		if( pVictim->GetTypeId() == TYPEID_PLAYER )
			static_cast< Player* >( pVictim )->EventAttackStop();

		/* Set victim health to 0 */
		pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
		if(pVictim->IsPlayer())
		{
			uint32 self_res_spell = static_cast< Player* >( pVictim )->SoulStone;
			static_cast< Player* >( pVictim )->SoulStone = static_cast< Player* >( pVictim )->SoulStoneReceiver = 0;

			if( !self_res_spell && static_cast< Player* >( pVictim )->bReincarnation )
			{
				SpellEntry* m_reincarnSpellInfo = dbcSpell.LookupEntry( 20608 );
				if( static_cast< Player* >( pVictim )->Cooldown_CanCast( m_reincarnSpellInfo ) )
				{
					uint32 ankh_count = static_cast< Player* >( pVictim )->GetItemInterface()->GetItemCount( 17030 );
					if( ankh_count )
						self_res_spell = 21169;
				}
			}
			pVictim->SetUInt32Value( PLAYER_SELF_RES_SPELL, self_res_spell );
			pVictim->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0 );
			//pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
		}

		// Wipe our attacker set on death
		Unit * pKiller = pVictim->CombatStatus.GetKiller();
		pVictim->CombatStatus.Vanished();

		//		 sent to set. don't send it to the party, becuase if they're out of
		//		 range they won't know this guid exists -> possible 132.

		/*if (this->IsPlayer())
			if( static_cast< Player* >( this )->InGroup() )
				static_cast< Player* >( this )->GetGroup()->SendPartyKillLog( this, pVictim );*/

		/* Stop Unit from attacking */
		if( this->IsPlayer() )
			static_cast< Player* >( this )->EventAttackStop();
	   
		if( this->IsUnit() )
		{
			CALL_SCRIPT_EVENT( this, OnTargetDied )( pVictim );
			static_cast< Unit* >( this )->smsg_AttackStop( pVictim );
		
			/* Tell Unit that it's target has Died */
			static_cast< Unit* >( this )->addStateFlag( UF_TARGET_DIED );

			// We will no longer be attacking this target, as it's dead.
			//static_cast<Unit*>(this)->setAttackTarget(NULL);
		}
		//so now we are completely dead
		//lets see if we have spirit of redemption
		if( pVictim->IsPlayer() )
		{
			if( static_cast< Player* >( pVictim)->HasSpell( 20711 ) ) //check for spirit of Redemption
			{
				SpellEntry* sorInfo = dbcSpell.LookupEntry(27827);
				if( sorInfo != NULL )
				{
					Spell *sor = new Spell( pVictim, sorInfo, true, NULL );
					SpellCastTargets targets;
					targets.m_unitTarget = pVictim->GetGUID();
					sor->prepare(&targets);
				}
			}
		}
		/* -------------------------------- HONOR + BATTLEGROUND CHECKS ------------------------ */
		plr = NULL;
		if( pKiller && pKiller->IsPlayer() )
			plr = static_cast< Player* >( pKiller );
		else if(pKiller && pKiller->IsPet())
			plr = static_cast< Pet* >( pKiller )->GetPetOwner();
		
		if( plr != NULL)
		{
			if( plr->m_bg != 0 )
				plr->m_bg->HookOnPlayerKill( plr, pVictim );

			if( pVictim->IsPlayer() )
			{
				sHookInterface.OnKillPlayer( plr, static_cast< Player* >( pVictim ) );
				if(plr->getLevel() > pVictim->getLevel())
				{
					unsigned int diff = plr->getLevel() - pVictim->getLevel();
					if( diff <= 8 )
					{
						HonorHandler::OnPlayerKilledUnit(plr, pVictim);
						plr->SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
					}
					else
						plr->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
				else
				{
					HonorHandler::OnPlayerKilledUnit( plr, pVictim );
					plr->SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
			}
			else
			{
				// REPUTATION
				if( !isCritter )
					plr->Reputation_OnKilledUnit( pVictim, false );

				plr->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
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
			TO_CREATURE(pVictim)->UpdateLootAnimation();
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
			pVictim->GetAIInterface()->OnDeath(this);
			if(GetTypeId() == TYPEID_PLAYER)
			{
				WorldPacket data(SMSG_PARTYKILLLOG, 16);
				data << GetGUID() << pVictim->GetGUID();
				SendMessageToSet(&data, true);
			}			

			// it Seems that pets some how dont get a name and cause a crash here
			//bool isCritter = (pVictim->GetCreatureName() != NULL)? pVictim->GetCreatureName()->Type : 0;

			//---------------------------------looot-----------------------------------------  
			
			if( GetTypeId() == TYPEID_PLAYER && 
				pVictim->GetUInt64Value( UNIT_FIELD_CREATEDBY ) == 0 && 
				pVictim->GetUInt64Value( OBJECT_FIELD_CREATED_BY ) == 0 &&
				!pVictim->IsPet() )
			{
				// TODO: lots of casts are bad make a temp member pointer to use for batches like this
				// that way no local loadhitstore and its just one assignment 

				// Is this player part of a group
				if( static_cast< Player* >( this)->InGroup() )
				{
					//Calc Group XP
					static_cast< Player* >( this )->GiveGroupXP( pVictim, static_cast< Player* >( this ) );
					//TODO: pet xp if player in group
				}
				else
				{
					uint32 xp = CalculateXpToGive( pVictim, static_cast< Unit* >( this ) );
					if( xp > 0 )
					{
						static_cast< Player* >( this )->GiveXP( xp, victimGuid, true );
						if( static_cast< Player* >( this )->GetSummon() && static_cast< Player* >( this )->GetSummon()->GetUInt32Value( UNIT_CREATED_BY_SPELL ) == 0 )
						{
							xp = CalculateXpToGive( pVictim, static_cast< Player* >( this )->GetSummon() );
							if( xp > 0 )
								static_cast< Player* >( this )->GetSummon()->GiveXP( xp );
						}
					}
				}

				if( pVictim->GetTypeId() != TYPEID_PLAYER )
					sQuestMgr.OnPlayerKill( static_cast< Player* >( this ), static_cast< Creature* >( pVictim ) );
			}
			else /* is Creature or GameObject*/
			{
				/* ----------------------------- PET XP HANDLING -------------- */
				if( owner_participe && IsPet() && !pVictim->IsPet() )
				{
					Player* petOwner = static_cast< Pet* >( this )->GetPetOwner();
					if( petOwner != NULL && petOwner->GetTypeId() == TYPEID_PLAYER )
					{
						if( petOwner->InGroup() )
						{
							//Calc Group XP
							static_cast< Unit* >( this )->GiveGroupXP( pVictim, petOwner );
							//TODO: pet xp if player in group
						}
						else
						{
							uint32 xp = CalculateXpToGive( pVictim, petOwner );
							if( xp > 0 )
							{
								petOwner->GiveXP( xp, victimGuid, true );
								if( !static_cast< Pet* >( this )->IsSummon() )
								{
									xp = CalculateXpToGive( pVictim, static_cast< Pet* >( this ) );
									if( xp > 0 )
										static_cast< Pet* >( this )->GiveXP( xp );
								}
							}
						}
					}
					if( petOwner != NULL && pVictim->GetTypeId() != TYPEID_PLAYER && 
						pVictim->GetTypeId() == TYPEID_UNIT )
						sQuestMgr.OnPlayerKill( petOwner, static_cast< Creature* >( pVictim ) );
				}
				/* ----------------------------- PET XP HANDLING END-------------- */

				/* ----------------------------- PET DEATH HANDLING -------------- */
				if( pVictim->IsPet() )
				{
					// dying pet looses 1 happiness level
					if( !static_cast< Pet* >( pVictim )->IsSummon() )
					{
						uint32 hap = static_cast< Pet* >( pVictim )->GetUInt32Value( UNIT_FIELD_POWER5 );
						hap = hap - PET_HAPPINESS_UPDATE_VALUE > 0 ? hap - PET_HAPPINESS_UPDATE_VALUE : 0;
						static_cast< Pet* >( pVictim )->SetUInt32Value( UNIT_FIELD_POWER5, hap );
					}
					
					static_cast< Pet* >( pVictim )->DelayedRemove( false, true );
					
					//remove owner warlock soul link from caster
					Player* owner = static_cast<Pet*>( pVictim )->GetPetOwner();
					if( owner != NULL )
						owner->EventDismissPet();
				}
				/* ----------------------------- PET DEATH HANDLING END -------------- */
				else if( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) )
				{
					//remove owner warlock soul link from caster
					Unit *owner=pVictim->GetMapMgr()->GetUnit( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) );
					if( owner != NULL && owner->IsPlayer())
						static_cast< Player* >( owner )->EventDismissPet();
				}
			}
		}
		else if( pVictim->GetTypeId() == TYPEID_PLAYER )
		{
			
			/* -------------------- RESET BREATH STATE ON DEATH -------------- */
			static_cast< Player* >( pVictim )->m_UnderwaterTime = 0;
			static_cast< Player* >( pVictim )->m_UnderwaterState = 0;
			static_cast< Player* >( pVictim )->m_BreathDamageTimer = 0;
			static_cast< Player* >( pVictim )->m_SwimmingTime = 0;

			/* -------------------- KILL PET WHEN PLAYER DIES ---------------*/
			if( static_cast< Player* >( pVictim )->GetSummon() != NULL )
			{
				if( pVictim->GetUInt32Value( UNIT_CREATED_BY_SPELL ) > 0 )
					static_cast< Player* >( pVictim )->GetSummon()->Dismiss( true );
				else
					static_cast< Player* >( pVictim )->GetSummon()->Remove( true, true, true );
			}
			/* -------------------- KILL PET WHEN PLAYER DIES END---------------*/
		}
		else DEBUG_LOG("DealDamage for Unknown Object.");
	}
	else /* ---------- NOT DEAD YET --------- */
	{
		if(pVictim != this /* && updateskill */)
		{
			// Send AI Reaction UNIT vs UNIT
			if( GetTypeId() ==TYPEID_UNIT )
			{
				static_cast< Unit* >( this )->GetAIInterface()->AttackReaction( pVictim, damage, spellId );
			}
			
			// Send AI Victim Reaction
			if( this->IsPlayer() || this->GetTypeId() == TYPEID_UNIT )
			{
				if( pVictim->GetTypeId() != TYPEID_PLAYER )
				{
					static_cast< Creature* >( pVictim )->GetAIInterface()->AttackReaction( static_cast< Unit* >( this ), damage, spellId );
				}
			}
		}
		
		// TODO: Mark victim as a HK
		/*if( static_cast< Player* >( pVictim )->GetCurrentBattleground() != NULL && static_cast< Player* >( this )->GetCurrentBattleground() != NULL)
		{
			
		}*/	

		pVictim->SetUInt32Value( UNIT_FIELD_HEALTH, health - damage );
	}
}

void Object::SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage, bool no_remove_auras)
{
//==========================================================================================
//==============================Unacceptable Cases Processing===============================
//==========================================================================================
	if(!pVictim || !pVictim->isAlive())
		return;

	SpellEntry *spellInfo = dbcSpell.LookupEntry( spellID );
	if(!spellInfo)
        return;

	if (this->IsPlayer() && !static_cast< Player* >( this )->canCast(spellInfo))
		return;
//==========================================================================================
//==============================Variables Initialization====================================
//========================================================================================== 
	uint32 school = spellInfo->School;
	float res = float(damage);
	uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_SPELL_LAND;
	uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_SPELL_HIT_VICTIM | PROC_ON_SPELL_LAND_VICTIM;
	bool critical = false;
	float dmg_reduction_pct;

	float res_after_spelldmg;

//==========================================================================================
//==============================+Spell Damage Bonus Calculations============================
//==========================================================================================
//------------------------------by stats----------------------------------------------------
	if( this->IsUnit() && !static_damage )
	{
		Unit* caster = static_cast< Unit* >( this );
		caster->RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_START_ATTACK );

		res += caster->GetSpellDmgBonus( pVictim, spellInfo, ( int )res, false );
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
			if( spellInfo->is_ranged_spell )
			{

				if( IsPlayer() )
				{
					CritChance = GetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE );
					if( pVictim->IsPlayer() )
						CritChance += static_cast< Player* >(pVictim)->res_R_crit_get();

					CritChance += (float)(pVictim->AttackerCritChanceMod[spellInfo->School]);
				}
				else
				{
					CritChance = 5.0f; // static value for mobs.. not blizzlike, but an unfinished formula is not fatal :)
				}
				if( pVictim->IsPlayer() )
				CritChance -= static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE );
			}
			else if( spellInfo->is_melee_spell )
			{
				// Same shit with the melee spells, such as Judgement/Seal of Command
				if( IsPlayer() )
				{
					CritChance = GetFloatValue(PLAYER_CRIT_PERCENTAGE);
				}
				if( pVictim->IsPlayer() )
				{
					CritChance += static_cast< Player* >(pVictim)->res_R_crit_get(); //this could be ability but in that case we overwrite the value
				}
				// Resilience
				CritChance -= pVictim->IsPlayer() ? static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) : 0.0f;
				// Victim's (!) crit chance mod for physical attacks?
				CritChance += (float)(pVictim->AttackerCritChanceMod[0]);

			}
			else
			{
				CritChance = caster->spellcritperc + caster->SpellCritChanceSchool[school] + pVictim->AttackerCritChanceMod[school];
				if( caster->IsPlayer() && ( pVictim->m_rooted - pVictim->m_stunned ) )	
					CritChance += static_cast< Player* >( caster )->m_RootedCritChanceBonus;

				if( spellInfo->SpellGroupType )
				{
					SM_FFValue(caster->SM_CriticalChance, &CritChance, spellInfo->SpellGroupType);
	#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
					float spell_flat_modifers=0;
					SM_FFValue(caster->SM_CriticalChance,&spell_flat_modifers,spellInfo->SpellGroupType);
					if(spell_flat_modifers!=0)
						printf("!!!!spell critchance mod flat %f ,spell group %u\n",spell_flat_modifers,spellInfo->SpellGroupType);
	#endif
				}
				if( pVictim->IsPlayer() )
				CritChance -= static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE );
			}
			if( CritChance < 0 ) CritChance = 0;
			if( CritChance > 95 ) CritChance = 95;
			critical = Rand(CritChance);
			//sLog.outString( "SpellNonMeleeDamageLog: Crit Chance %f%%, WasCrit = %s" , CritChance , critical ? "Yes" : "No" );
//==========================================================================================
//==============================Spell Critical Hit==========================================
//==========================================================================================
			if (critical)
			{		
				int32 critical_bonus = 100;
				if( spellInfo->SpellGroupType )
					SM_FIValue( caster->SM_PCriticalDamage, &critical_bonus, spellInfo->SpellGroupType );

				if( critical_bonus > 0 )
				{
					// the bonuses are halved by 50% (funky blizzard math :S)
					float b;
					if( spellInfo->School == 0 || spellInfo->is_melee_spell || spellInfo->is_ranged_spell )		// physical || hackfix SoCommand/JoCommand
						b = ( ( float(critical_bonus) ) / 100.0f ) + 1.0f;
					else
						b = ( ( float(critical_bonus) / 2.0f ) / 100.0f ) + 1.0f;

					if( pVictim->IsPlayer() )
					{
						dmg_reduction_pct = 2.0f * static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE );
						if( dmg_reduction_pct < 100.0f )
						{
							dmg_reduction_pct /= 100.0f;
							dmg_reduction_pct = 1.0f - dmg_reduction_pct;
							b *= dmg_reduction_pct;
							res	*= b;
						}
					}
					else
						res *= b;
				}

				/*if( pVictim->IsPlayer() )
				{
					//res = res*(1.0f-2.0f*static_cast< Player* >(pVictim)->CalcRating(PLAYER_RATING_MODIFIER_MELEE_CRIT_RESISTANCE));
					//Resilience is a special new rating which was created to reduce the effects of critical hits against your character.
					//It has two components; it reduces the chance you will be critically hit by x%, 
					//and it reduces the damage dealt to you by critical hits by 2x%. x is the percentage resilience granted by a given resilience rating. 
					//It is believed that resilience also functions against spell crits, 
					//though it's worth noting that NPC mobs cannot get critical hits with spells.
					float dmg_reduction_pct = 2 * static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
					if( dmg_reduction_pct > 1.0f )
						dmg_reduction_pct = 1.0f; //we cannot resist more then he is criticalling us, there is no point of the critical then :P
					res = res - res * dmg_reduction_pct;
				}*/

				pVictim->Emote( EMOTE_ONESHOT_WOUNDCRITICAL );
				aproc |= PROC_ON_SPELL_CRIT_HIT;
				vproc |= PROC_ON_SPELL_CRIT_HIT_VICTIM;
			}
		}
	}
//==========================================================================================
//==============================Post Roll Calculations======================================
//==========================================================================================
	//dirty fix for Ice Lance
	if( ( pVictim->m_rooted -pVictim->m_stunned) > 0 && spellInfo->NameHash == SPELL_HASH_ICE_LANCE ) //Ice Lance deals 3x damage if target is frozen
	{
		res *= 3;
	}
//------------------------------absorption--------------------------------------------------	
	uint32 ress=(uint32)res;
	uint32 abs_dmg = pVictim->AbsorbDamage(school, &ress);
	uint32 ms_abs_dmg= pVictim->ManaShieldAbsorb(ress);
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
	if(res > 0 && this->IsUnit())
	{
		static_cast<Unit*>(this)->CalculateResistanceReduction(pVictim,&dmg,spellInfo);
		if((int32)dmg.resisted_damage > dmg.full_damage)
			res = 0;
		else
			res = float(dmg.full_damage - dmg.resisted_damage);
	}
	//------------------------------special states----------------------------------------------
	if(pVictim->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >(pVictim)->GodModeCheat == true)
	{
		res = 0;
		dmg.resisted_damage = dmg.full_damage;
	}
	//DK:FIXME->SplitDamage
	
//==========================================================================================
//==============================Data Sending ProcHandling===================================
//==========================================================================================
	SendSpellNonMeleeDamageLog(this, pVictim, spellID, float2int32(res), school, abs_dmg, dmg.resisted_damage, false, 0, critical, IsPlayer());

	int32 ires = float2int32(res);
	if( ires > 0 )
	{
		// only deal damage if its >0
		DealDamage( pVictim, float2int32( res ), 2, 0, spellID );
	}
	else
	{
		// we still have to tell the combat status handler we did damage so we're put in combat
		if( IsUnit() )
			((Unit*)this)->CombatStatus.OnDamageDealt(pVictim, 1);
	}
	
	if( this->IsUnit() && allowProc && spellInfo->Id != 25501 )
	{
		pVictim->HandleProc( vproc, static_cast< Unit* >( this ), spellInfo, float2int32( res ) );
		pVictim->m_procCounter = 0;
		static_cast< Unit* >( this )->HandleProc( aproc, pVictim, spellInfo, float2int32( res ) );
		static_cast< Unit* >( this )->m_procCounter = 0;
	}
	if( this->IsPlayer() )
	{
			static_cast< Player* >( this )->m_casted_amount[school] = ( uint32 )res;
	}

	

	if( pVictim->GetCurrentSpell() )
		pVictim->GetCurrentSpell()->AddTime( school );

//==========================================================================================
//==============================Post Damage Processing======================================
//==========================================================================================
	if( (int32)dmg.resisted_damage == dmg.full_damage && !abs_dmg )
	{
		//Magic Absorption
		if( pVictim->IsPlayer() )
		{
			if( static_cast< Player* >( pVictim )->m_RegenManaOnSpellResist )
			{
				Player* pl = static_cast< Player* >( pVictim );
				uint32 maxmana = pl->GetUInt32Value( UNIT_FIELD_MAXPOWER1 );

				//TODO: wtf is this ugly mess of casting bullshit
				uint32 amount = uint32(float( float(maxmana)*pl->m_RegenManaOnSpellResist));

				pVictim->Energize( pVictim, 29442, amount, POWER_TYPE_MANA );
			}
		}
	}
	if( school == SHADOW_DAMAGE )
	{
		if( IsPlayer() && ((Unit*)this)->isAlive() && ((Player*)this)->getClass() == PRIEST )
			((Player*)this)->VampiricSpell(float2int32(res), pVictim);

		if( pVictim->isAlive() && this->IsUnit() )
		{
			//Shadow Word:Death
			if( spellID == 32379 || spellID == 32996 ) 
			{
				uint32 damage = (uint32)( res + abs_dmg );
				uint32 absorbed = static_cast< Unit* >( this )->AbsorbDamage( school, &damage );
				DealDamage( static_cast< Unit* >( this ), damage, 2, 0, spellID );
				SendSpellNonMeleeDamageLog( this, this, spellID, damage, school, absorbed, 0, false, 0, false, this->IsPlayer() );
			}
		}
	}

	// Rage
	/*if( dmg.full_damage && this->IsUnit() && pVictim->IsPlayer() && pVictim->GetPowerType() == POWER_TYPE_RAGE && pVictim->CombatStatus.IsInCombat() )
	{
		float val;
		float level = (float)TO_UNIT(this)->getLevel();

		// Conversion Value
		float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;

		val = 2.5f * dmg.full_damage / c;
		val *= 10;

		//DEBUG_LOG( "Rd(%i) d(%i) c(%f) rage = %f", realdamage, dmg.full_damage, c, val );

		pVictim->ModUnsigned32Value( UNIT_FIELD_POWER2, (int32)val );
		if( pVictim->GetUInt32Value( UNIT_FIELD_POWER2) > 1000 )
			pVictim->ModUnsigned32Value( UNIT_FIELD_POWER2, 1000 - pVictim->GetUInt32Value( UNIT_FIELD_POWER2 ) );

	}*/
}

//*****************************************************************************************
//* SpellLog packets just to keep the code cleaner and better to read
//*****************************************************************************************

void Object::SendSpellLog(Object *Caster, Object *Target,uint32 Ability, uint8 SpellLogType)
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


void Object::SendSpellNonMeleeDamageLog( Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToset )
{
	if ((!Caster || !Target) && SpellID)
		return;

	WorldPacket data(SMSG_SPELLNONMELEEDAMAGELOG,40);
	data << Target->GetNewGUID();
	data << Caster->GetNewGUID();
	data << SpellID;                    // SpellID / AbilityID
	data << Damage;                     // All Damage
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

void Object::EventSpellHit(Spell *pSpell)
{
	if( !IsInWorld() )
	{
		delete pSpell;
		return;
	}

	pSpell->cast(false);
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
			if(static_cast<GameObject*>(this)->HasAI() && GetUInt32Value(GAMEOBJECT_TYPE_ID) != GAMEOBJECT_TYPE_TRAP)
				return true;
		}break;
	}

	return false;
}

void Object::Activate(MapMgr * mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		mgr->activeCreatures.insert((Creature*)this);
		break;

	case TYPEID_GAMEOBJECT:
		mgr->activeGameObjects.insert((GameObject*)this);
		break;
	}

	Active = true;
}

void Object::Deactivate(MapMgr * mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		// check iterator
		if( mgr->__creature_iterator != mgr->activeCreatures.end() && (*mgr->__creature_iterator) == TO_CREATURE(this) )
			++mgr->__creature_iterator;

		mgr->activeCreatures.erase((Creature*)this);
		break;

	case TYPEID_GAMEOBJECT:
		// check iterator
		if( mgr->__gameobject_iterator != mgr->activeGameObjects.end() && (*mgr->__gameobject_iterator) == TO_GAMEOBJECT(this) )
			++mgr->__gameobject_iterator;

		mgr->activeGameObjects.erase((GameObject*)this);
		break;
	}
	Active = false;
}

void Object::SetByte(uint32 index, uint32 index1,uint8 value)
{
	ASSERT( index < m_valuesCount );
	// save updating when val isn't changing.
	uint8 * v =&((uint8*)m_uint32Values)[index*4+index1];
	
	if(*v == value)
		return;

	*v = value;

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

void Object::SetZoneId(uint32 newZone)
{
	m_zoneId = newZone;
	if( m_objectTypeId == TYPEID_PLAYER && static_cast< Player* >( this )->GetGroup() )
		static_cast< Player* >( this )->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_ZONEID, static_cast< Player* >( this ) );
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
