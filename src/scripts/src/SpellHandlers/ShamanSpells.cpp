/*
 * Scripts for Ascent MMORPG Server
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
#include "Setup.h"

/************************************************************************/
/* Spell Defs                                                           */
/************************************************************************/

bool RockbiterWeapon(uint32 i, Spell* pSpell)
{
    uint32 enchantment_entry = 0;
    switch(pSpell->m_spellInfo->RankNumber)
    {
    case 1:
        enchantment_entry = 3021;
        break;
    case 2:
        enchantment_entry = 3024;
        break;
    case 3:
        enchantment_entry = 3027;
        break;
    case 4:
        enchantment_entry = 3030;
        break;
    case 5:
        enchantment_entry = 3033;
        break;
    case 6:
        enchantment_entry = 3036;
        break;
    case 7:
        enchantment_entry = 3039;
        break;
    case 8:
        enchantment_entry = 3042;
        break;
    case 9:
        enchantment_entry = 3018;
        break;
    }

    if(!enchantment_entry || !pSpell->p_caster)
        return true;

    Item* item = pSpell->p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
    EnchantEntry * enchant = dbcEnchant.LookupEntry(enchantment_entry);
    if(!item || !enchant)
        return true;

    int32 Slot = item->HasEnchantment(enchant->Id);
    if(Slot >= 0)
        item->ModifyEnchantmentTime(Slot, 1800);
    else
    {
		//check if enchantment slot 1 is taken. If there was no enchantment there function will quit
		item->RemoveEnchantment(1);
		//this will also apply bonuses for us
        Slot = item->AddEnchantment(enchant, 1800, false, true, false, 1);   // 5min
        if(Slot < 0) return true;
    }

    sLog.outDebug("ShamanSpells.cpp :: Rockbiter Weapon Rank %u, enchant %u, slot %u", pSpell->m_spellInfo->RankNumber,
        enchantment_entry, Slot);
    
    return true;
}

void SetupShamanSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(8017, &RockbiterWeapon); // rank 1
	mgr->register_dummy_spell(8018, &RockbiterWeapon); // rank 2
	mgr->register_dummy_spell(8019, &RockbiterWeapon); // rank 3
	mgr->register_dummy_spell(10399, &RockbiterWeapon);// rank 4
	mgr->register_dummy_spell(16314, &RockbiterWeapon);// rank 5
	mgr->register_dummy_spell(16315, &RockbiterWeapon);// rank 6
	mgr->register_dummy_spell(16316, &RockbiterWeapon);// rank 7
	mgr->register_dummy_spell(25479, &RockbiterWeapon);// rank 8
	mgr->register_dummy_spell(25485, &RockbiterWeapon);// rank 9
}
