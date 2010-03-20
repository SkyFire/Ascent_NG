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

void Player::SendWorldStateUpdate(uint32 WorldState, uint32 Value)
{
	packetSMSG_WORLD_STATE_UPDATE pck;
	pck.State = WorldState;
	pck.Value = Value;

	GetSession()->OutPacket(SMSG_UPDATE_WORLD_STATE, sizeof(packetSMSG_WORLD_STATE_UPDATE), (const char*)&pck);
}

void Player::Gossip_SendPOI(float X, float Y, uint32 Icon, uint32 Flags, uint32 Data, const char* Name)
{
	WorldPacket data(SMSG_GOSSIP_POI, 50);
	data << Flags;
	data << X;
	data << Y;
	data << Icon;
	data << Data;
	data << Name;
	GetSession()->SendPacket(&data);
}
  
void Player::SendLevelupInfo(uint32 level, uint32 Hp, uint32 Mana, uint32 Stat0, uint32 Stat1, uint32 Stat2, uint32 Stat3, uint32 Stat4)
{
    packet_SMSG_LEVELUP_INFO packet;
    packet.level = level;
    packet.Hp = Hp;
    packet.Mana = Mana;

    // grep: these are probably the other powers :)
    packet.unk0 = 0;
    packet.unk1 = 0;
    packet.unk2 = 0;
    packet.unk3 = 0;
	packet.unk4 = 0;
    packet.unk5 = 0;

    // Append stat differences
    packet.Stat0 = Stat0;
    packet.Stat1 = Stat1;
    packet.Stat2 = Stat2;
    packet.Stat3 = Stat3;
    packet.Stat4 = Stat4;
    GetSession()->OutPacket(SMSG_LEVELUP_INFO, sizeof(packet_SMSG_LEVELUP_INFO),(const char*)&packet);
}

void Player::SendLogXPGain(uint64 guid, uint32 NormalXP, uint32 RestedXP, bool type)
{
    if (type == false)
    {
        packetSMSG_LOG_XP_GAIN_EXTRA packet;
        packet.guid     = guid;
        packet.xp       = NormalXP;
        packet.type     = (uint8)type;
        packet.restxp   = RestedXP;
        packet.unk2     = 1.0f;
        GetSession()->OutPacket(SMSG_LOG_XPGAIN, sizeof(packetSMSG_LOG_XP_GAIN_EXTRA),(const char*)&packet);
    }
    else if (type == true)
    {
        packetSMSG_LOG_XP_GAIN packet;
        packet.guid = 0; // does not need to be set for quest xp
        packet.xp = NormalXP;
        packet.type = (uint8)type;
		packet.unk = 0;
        GetSession()->OutPacket(SMSG_LOG_XPGAIN, sizeof(packetSMSG_LOG_XP_GAIN),(const char*)&packet);
    }
}

// this one needs to be send inrange...
void Player::SendEnvironmentalDamageLog(const uint64 & guid, uint8 type, uint32 damage)
{
    packetSMSG_ENVIRONMENTAL_DAMAGE packet;
    packet.Guid = guid;
    packet.Damage = damage;
    packet.Type = type;
    GetSession()->OutPacket(SMSG_ENVIRONMENTALDAMAGELOG, sizeof(packetSMSG_ENVIRONMENTAL_DAMAGE),(const char*)&packet);
}


void Player::SendCastResult(uint32 SpellId, uint8 ErrorMessage, uint8 MultiCast, uint32 Extra)
{
	if( Extra )
	{
		packetSMSG_CASTRESULT_EXTRA pe;
		pe.SpellId = SpellId;
		pe.ErrorMessage = ErrorMessage;
		pe.MultiCast = MultiCast;
		pe.Extra = Extra;
		m_session->OutPacket( SMSG_CAST_FAILED, sizeof( packetSMSG_CASTRESULT_EXTRA ), &pe );
	}
	else
	{
		packetSMSG_CASTRESULT pe;
		pe.SpellId = SpellId;
		pe.ErrorMessage = ErrorMessage;
		pe.MultiCast = MultiCast;
		m_session->OutPacket( SMSG_CAST_FAILED, sizeof( packetSMSG_CASTRESULT ), &pe );
	}
}


