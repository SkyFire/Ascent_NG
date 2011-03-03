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

#include "StdAfx.h"

uint32 getConColor(uint16 AttackerLvl, uint16 VictimLvl)
{

	//	const uint32 grayLevel[sWorld.LevelCap+1] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,14,15,16,17,18,19,20,21,22,22,23,24,25,26,27,28,29,30,31,31,32,33,34,35,35,36,37,38,39,39,40,41,42,43,43,44,45,46,47,47,48,49,50,51,51,52,53,54,55,55};
#define PLAYER_LEVEL_CAP 70
	const uint32 grayLevel[PLAYER_LEVEL_CAP+1] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,14,15,16,17,18,19,20,21,22,22,23,24,25,26,27,28,29,30,31,31,32,33,34,35,35,36,37,38,39,39,40,41,42,43,43,44,45,46,47,47,48,49,50,51,51,52,53,54,55,55};
	if(AttackerLvl + 5 <= VictimLvl)
	{
		if(AttackerLvl + 10 <= VictimLvl)
		{
			return 5;
		}
		return 4;
	}
	else
	{
		switch(VictimLvl - AttackerLvl)
		{
		case 4:
		case 3:
			return 3;
			break;
		case 2:
		case 1:
		case 0:
		case -1:
		case -2:
			return 2;
			break;
		default:
			// More adv formula for grey/green lvls:
			if(AttackerLvl <= 6)
			{
				return 1; //All others are green.
			}
			else
			{
				if(AttackerLvl > PLAYER_LEVEL_CAP)
					return 1;//gm
				if(AttackerLvl<PLAYER_LEVEL_CAP && VictimLvl <= grayLevel[AttackerLvl])
					return 0;
				else
					return 1;
			}
		}
	}
#undef PLAYER_LEVEL_CAP
}

uint32 CalculateXpToGive(Unit* pVictim, Unit* pAttacker)
{
	if(pVictim->IsPlayer())
		return 0;

	if( TO_CREATURE(pVictim)->IsTotem())
		return 0;

	CreatureInfo *victimI;
	victimI = TO_CREATURE(pVictim)->GetCreatureInfo();

	if(victimI)
		if(victimI->Type == CRITTER)
			return 0;
	uint32 VictimLvl = pVictim->getLevel();
	uint32 AttackerLvl = pAttacker->getLevel();

	if( pAttacker->IsPet() && TO_PET(pAttacker)->GetPetOwner() )
	{
		// based on: http://www.wowwiki.com/Talk:Formulas:Mob_XP#Hunter.27s_pet_XP (2008/01/12)
		uint32 ownerLvl = TO_PET( pAttacker )->GetPetOwner()->getLevel();
		VictimLvl += ownerLvl - AttackerLvl;
		AttackerLvl = ownerLvl;
	}
	else if( (int32)VictimLvl - (int32)AttackerLvl > 10 ) //not wowwikilike but more balanced
		return 0;

	// Partha: this screws things up for pets and groups
	// No need for it here - it does this later in Player::GiveXP 
	/*
	uint32 max_level = 70;
	if(pAttacker->IsPlayer())
	max_level = pAttacker->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);
	else if(pAttacker->IsPet())
	max_level = TO_PET( pAttacker )->GetPetOwner()->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);

	if(pAttacker->getLevel() >= max_level)
	return 0;
	*/


	/*if(VictimLvl+7>AttackerLvl)
	VictimLvl = AttackerLvl + 7;*/

	float zd = 5;
	float g = 5;

	// get zero diff
	// get grey diff

	if(AttackerLvl >= 70)
	{
		zd = 19;
		g = 15;
	}
	else if(AttackerLvl >= 65)
	{
		zd = 18;
		g = 13;
	}
	else if(AttackerLvl >= 60)
	{
		zd = 17;
		g = 14;
	}
	else if(AttackerLvl >= 55)
	{
		zd = 16;
		g = 12;
	}
	else if(AttackerLvl >= 50)
	{
		zd = 15;
		g = 11;
	}
	else if(AttackerLvl >= 45)
	{
		zd = 14;
		g = 10;
	}
	else if(AttackerLvl >= 40)
	{
		zd = 13;
		g = 9;
	}
	else if(AttackerLvl >= 30)
	{
		zd = 12;
		g = 8;
	}
	else if(AttackerLvl >= 20)
	{
		zd = 11;
		g = 7;
	}
	else if(AttackerLvl >= 16)
	{
		zd = 9;
		g = 6;
	}
	else if(AttackerLvl >= 12)
	{
		zd = 8;
		g = 6;
	}
	else if(AttackerLvl >= 10)
	{
		zd = 7;
		g = 6;
	}
	else if(AttackerLvl >= 8)
	{
		zd = 6;
		g = 5;
	}
	else
	{
		zd = 5;
		g = 5;
	}

	float xp = 0.0f;
	float fVictim = float(VictimLvl);
	float fAttacker = float(AttackerLvl);

	if(VictimLvl == AttackerLvl)
		xp = float( ((fVictim * 5.0f) + 45.0f) );
	else if(VictimLvl > AttackerLvl)
	{
		float j = 1.0f + (0.25f * (fVictim - fAttacker));
		xp = float( ((AttackerLvl * 5.0f) + 45.0f) * j );
	}
	else
	{
		if((AttackerLvl - VictimLvl) < g)
		{
			float j = (1.0f - float((fAttacker - fVictim) / zd));
			xp = (AttackerLvl * 5.0f + 45.0f) * j;
		}
	}

	// multiply by global XP rate
	if(xp == 0.0f)
		return 0;

	xp *= sWorld.getRate(RATE_XP);

	// elite boss multiplier
	if(victimI)
	{
		switch(victimI->Rank)
		{
		case 0: // normal mob
			break;
		case 1: // elite
			xp *= 2.0f;
			break;
		case 2: // rare elite
			xp *= 2.0f;
			break;
		case 3: // world boss
			xp *= 2.5f;
			break;
		default:	// rare or higher
			//			xp *= 7.0f;
			break;
		}
	}
	if( xp < 0 )//probably caused incredible wrong exp
		xp = 0;

	return (uint32)xp;
	/*const float ZD[PLAYER_LEVEL_CAP+1] = {1,5,5,5,5,5,5,5,6,6,7,7,8,8,8,9,9,9,9,9,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17};
	float temp = 0;
	float tempcap = 0;
	float xp = 0;

	if(VictimLvl >= AttackerLvl)
	{
	temp = ((AttackerLvl * 5) + 45) * (1 + 0.05 * (VictimLvl - AttackerLvl));
	tempcap = ((AttackerLvl * 5) + 45) * 1.2;
	if(temp > tempcap)
	{
	if( tempcap < 0 )
	tempcap = 0;
	else
	tempcap *= sWorld.getRate(RATE_XP);

	xp = tempcap;
	}
	else
	{
	if( temp < 0 )
	temp = 0;
	else
	temp *= sWorld.getRate(RATE_XP);

	xp = temp;
	}
	}
	else
	{
	if(getConColor(AttackerLvl, VictimLvl) == 0)
	{
	return (uint32)0;
	}
	else
	{
	if(AttackerLvl < PLAYER_LEVEL_CAP)
	temp = (((AttackerLvl * 5) + 45) * (1 - (AttackerLvl - VictimLvl)/ZD[AttackerLvl]));
	else
	temp = (((AttackerLvl * 5) + 45) * (1 - (AttackerLvl - VictimLvl)/17));
	if( temp < 0 )
	temp = 0;
	else
	temp *= sWorld.getRate(RATE_XP);

	xp = temp;
	}
	}

	if(victimI)
	{
	switch(victimI->Rank)
	{
	case 0: // normal mob
	break;
	case 1: // elite
	xp *= 1.5f;
	break;
	case 2: // rare elite
	xp *= 3.0f;
	break;
	case 3: // world boss
	xp *= 10.0f;
	break;
	default:	// rare or higher
	xp *= 7.0f;
	break;
	}
	}

	return (uint32)(xp);*/
}

uint32 CalculateStat(uint16 level, float inc)
{
	float a = level * inc;

	return (uint32)a; // truncate: working as intended.
}

//Partialy taken from WoWWoW Source
uint32 CalcStatForLevel(uint16 level, uint8 playerclass,uint8 Stat)
{
	uint32 gain = 0;
	switch(playerclass)
	{
	case DEATHKNIGHT:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 2.56f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 1.64f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 2.36f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.4f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.76f); } break;
			}
		}break;

	case PRIEST:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.2875f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.3875f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.5875f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 1.9f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 2.0875f); } break;
			}
		}break;

	case WARRIOR:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 2.0125f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 1.1625f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 1.825f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.2f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.4875f); } break;	
			}
		}break;

	case WARLOCK:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.4875f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.5875f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.95f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 1.7125f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 1.8f); } break;		
			}
		}break;

	case DRUID:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.85f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.775f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.975f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 1.5125f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 2.0f); } break;	
			}
		}break;

	case PALADIN:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 1.8875f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.875f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 1.5125f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.975f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 1.05f); } break;
			}
		}break;

	case HUNTER:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.675f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 1.975f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 1.3375f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.875f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.95f); } break;	
			}
		}break;

	case MAGE:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.2f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.2875f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.4875f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 1.975f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 1.9625f); } break;		
			}
		}break;

	case SHAMAN:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 1.2375f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.675f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 1.4375f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 1.4875f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 1.5125f); } break;	
			}
		}break;

	case ROGUE:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 1.15f); } break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 2.075f); } break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 1.1f); } break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.2875f); } break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.6125f); } break;
			}
		}break;
	}

	return gain;
}

uint32 CalculateDamage( Unit* pAttacker, Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability ) // spellid is used only for 2-3 spells, that have AP bonus
{
	//TODO: Some awesome formula to determine how much damage to deal
	//consider this is melee damage
	// weapon_damage_type: 0 = melee, 1 = offhand(dualwield), 2 = ranged

	// Attack Power increases your base damage-per-second (DPS) by 1 for every 14 attack power.
	// (c) wowwiki

	//type of this UNIT_FIELD_ATTACK_POWER_MODS is unknown, not even uint32 disabled for now.

	uint32 offset;
	Item* it = NULL;

	if(pAttacker->disarmed && pAttacker->IsPlayer())
	{
		offset=UNIT_FIELD_MINDAMAGE;
		it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	}
	else if( weapon_damage_type == MELEE )
		offset = UNIT_FIELD_MINDAMAGE;
	else if( weapon_damage_type == OFFHAND )
		offset = UNIT_FIELD_MINOFFHANDDAMAGE;
	else  // weapon_damage_type == RANGED
		offset = UNIT_FIELD_MINRANGEDDAMAGE;

	float min_damage = pAttacker->GetFloatValue(offset);
	float max_damage = pAttacker->GetFloatValue(offset+1);
	if(it)
	{
		min_damage -= it->GetProto()->Damage[0].Min;
		max_damage -= it->GetProto()->Damage[0].Max;
	}

	float ap = 0;
	float bonus;
	float wspeed;
	float appbonuspct = 1.0f;
	Item* BonusItem = NULL;
	if( pAttacker->IsPlayer() && weapon_damage_type == MELEE )
	{
		BonusItem = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	}
	else if(pAttacker->IsPlayer() && weapon_damage_type == OFFHAND )
	{
		BonusItem = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
	}
	else if(pAttacker->IsPlayer() && weapon_damage_type == RANGED )
	{
		BonusItem = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
	}

	if( pAttacker->IsPlayer() && BonusItem )
	{
		appbonuspct = TO_PLAYER(pAttacker)->m_WeaponSubClassDamagePct[ BonusItem->GetProto()->SubClass  ];
	}

	if(offset == UNIT_FIELD_MINRANGEDDAMAGE)
	{
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetRAP();
		ap += pVictim->RAPvModifier;

		if(!pVictim->IsPlayer() && TO_CREATURE(pVictim)->GetCreatureInfo())
		{
			uint32 creatType = TO_CREATURE(pVictim)->GetCreatureInfo()->Type;
			ap += (float)pAttacker->CreatureRangedAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				Item* it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
				if(it)
				{
					wspeed = (float)it->GetProto()->Delay;
				}
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME);
		}
		else
		{
			wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
		}

		//ranged weapon normalization.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				wspeed = 2800;
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(TO_PLAYER(pAttacker)->IsInFeralForm())
			{
				uint8 ss = TO_PLAYER(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}
	else
	{
		//MinD = AP(28AS-(WS/7))-MaxD
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetAP();
		ap += pVictim->APvModifier;

		if(!pVictim->IsPlayer() && TO_CREATURE(pVictim)->GetCreatureInfo())
		{
			uint32 creatType = TO_CREATURE(pVictim)->GetCreatureInfo()->Type;
			ap += (float)pAttacker->CreatureAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + TO_PLAYER(pAttacker)->IncreaseDamageByType[creatType]) * (1 + TO_PLAYER(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				Item* it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
					wspeed = (float)it->GetProto()->Delay;
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);

			if(ability && ability->SpellGroupType)
			{
				int32 apall = pAttacker->GetAP();
				/* this spell modifier doesn't exist. also need to clear up how the AP is used here
				int32 apb=0;
				SM_FIValue(pAttacker->SM[SMT_ATTACK_POWER_BONUS][1],&apb,ability->SpellGroupType);

				if(apb)
					ap += apall*((float)apb/100);
				else*/
					ap = float(apall);
			}
		}
		else
		{
			wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
		}

		//Normalized weapon damage checks.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				it = TO_PLAYER(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
				{
					if(it->GetProto()->Class == ITEM_CLASS_WEAPON) //weapon
					{
						if(it->GetProto()->InventoryType == INVTYPE_2HWEAPON) wspeed = 3300;
						else if(it->GetProto()->SubClass == 15) wspeed = 1700;
						else wspeed = 2400;
					}
				}
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(TO_PLAYER(pAttacker)->IsInFeralForm())
			{
				uint8 ss = TO_PLAYER(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		if (offset == UNIT_FIELD_MINDAMAGE)
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME))/14000.0f*ap;
		else if( offset == UNIT_FIELD_MINOFFHANDDAMAGE )
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME + 1)) / 14000.0f*ap;
		else
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}

	float diff = fabs(max_damage - min_damage);
	float result = min_damage;

	if(diff >= 1)
		result += float(RandomDouble(diff));

	if(result >= 0)
	{
		return FL2UINT(result * appbonuspct);
	}

	return 0;
}

bool isEven (int num)
{
	if ((num%2)==0)
	{
		return true;
	}

	return false;
}




