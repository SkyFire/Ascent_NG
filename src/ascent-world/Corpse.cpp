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

Corpse::Corpse(uint32 high, uint32 low)
{
	m_objectTypeId = TYPEID_CORPSE;
	m_valuesCount = CORPSE_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(CORPSE_END)*sizeof(uint32));
	m_updateMask.SetCount(CORPSE_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_CORPSE|TYPE_OBJECT);
	SetUInt32Value( OBJECT_FIELD_GUID,low);
	SetUInt32Value( OBJECT_FIELD_GUID+1,high);
	m_wowGuid.Init(GetGUID());

	SetFloatValue( OBJECT_FIELD_SCALE_X, 1 );//always 1
	  

	m_state = CORPSE_STATE_BODY;
	_loadedfromdb = false;
}

Corpse::~Corpse()
{
}

void Corpse::Init()
{
	if(GetUInt32Value(OBJECT_FIELD_GUID+1) != 0)
		objmgr.AddCorpse(TO_CORPSE(this));
}

void Corpse::Destructor()
{
	objmgr.RemoveCorpse(this);
	delete this;
}


void Corpse::Create( Player* owner, uint32 mapid, float x, float y, float z, float ang )
{
	Object::_Create( mapid, x, y, z, ang);

	/*
	SetFloatValue( CORPSE_FIELD_POS_X, x );
	SetFloatValue( CORPSE_FIELD_POS_Y, y );
	SetFloatValue( CORPSE_FIELD_POS_Z, z );
	SetFloatValue( CORPSE_FIELD_FACING, ang );
	*/
	SetUInt64Value( CORPSE_FIELD_OWNER, owner->GetGUID() );
	_loadedfromdb = false;  // can't be created from db ;)
}

void Corpse::SaveToDB()
{
	//save corpse to DB
	std::stringstream ss;
	ss << "DELETE FROM corpses WHERE guid = " << GetLowGUID();
	CharacterDatabase.Execute( ss.str( ).c_str( ) );

	ss.rdbuf()->str("");
	ss << "INSERT INTO corpses (guid, positionX, positionY, positionZ, orientation, zoneId, mapId, data, instanceId) VALUES ("
		<< GetLowGUID() << ", '" << GetPositionX() << "', '" << GetPositionY() << "', '" << GetPositionZ() << "', '" << GetOrientation() << "', '" << GetZoneId() << "', '" << GetMapId() << "', '";

	for(uint16 i = 0; i < m_valuesCount; i++ )
		ss << GetUInt32Value(i) << " ";

	ss << "', " << GetInstanceID() << " )";

	CharacterDatabase.Execute( ss.str().c_str() );
}

void Corpse::DeleteFromDB()
{
	//delete corpse from db when its not needed anymore
	char sql[256];

	snprintf(sql, 256, "DELETE FROM corpses WHERE guid=%u", (unsigned int)GetLowGUID());
	CharacterDatabase.Execute(sql);
}

void CorpseData::DeleteFromDB()
{
	char sql[256];

	snprintf(sql, 256, "DELETE FROM corpses WHERE guid=%u", (unsigned int)LowGuid);
	CharacterDatabase.Execute(sql);
}

void Corpse::Despawn()
{
	if(IsInWorld())
	{
		RemoveFromWorld(false);
	}
}

void Corpse::SpawnBones()
{
	SetUInt32Value(CORPSE_FIELD_FLAGS, 5);
	SetUInt64Value(CORPSE_FIELD_OWNER, 0); // remove corpse owner association
	//remove item association
	for (int i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		if(GetUInt32Value(CORPSE_FIELD_ITEM + i))
			SetUInt32Value(CORPSE_FIELD_ITEM + i, 0);
	}
	DeleteFromDB();
	objmgr.CorpseAddEventDespawn(TO_CORPSE(this));
	SetCorpseState(CORPSE_STATE_BONES);
}

void Corpse::Delink()
{
	SetUInt32Value(CORPSE_FIELD_FLAGS,5);
	SetUInt64Value(CORPSE_FIELD_OWNER,0);
	SetCorpseState(CORPSE_STATE_BONES);
	DeleteFromDB();
}
