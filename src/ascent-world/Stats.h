/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef __STATS_H
#define __STATS_H

#include "Unit.h"
#include "UpdateMask.h"
#include "ItemInterface.h"

enum Stats
{
	STAT_STRENGTH,
	STAT_AGILITY,
	STAT_STAMINA,
	STAT_INTELLECT,
	STAT_SPIRIT,
};

enum Resistances
{
	RESISTANCE_ARMOR,
	RESISTANCE_HOLY,
	RESISTANCE_FIRE,
	RESISTANCE_NATURE,
	RESISTANCE_FROST,
	RESISTANCE_SHADOW,
	RESISTANCE_ARCANE
};

SERVER_DECL uint32 getConColor(uint16 AttackerLvl, uint16 VictimLvl);
SERVER_DECL uint32 CalculateXpToGive(Unit* pVictim, Unit* pAttacker);
SERVER_DECL uint32 CalculateStat(uint16 level,double a3, double a2, double a1, double a0);
SERVER_DECL uint32 CalculateDamage( Unit* pAttacker, Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability);
SERVER_DECL uint32 CalcStatForLevel(uint16 level, uint8 playerclass,uint8 Stat);
SERVER_DECL bool isEven (int num);

#endif
