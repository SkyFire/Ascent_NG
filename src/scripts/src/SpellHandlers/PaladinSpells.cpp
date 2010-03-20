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

/*
	How to add a new paladin spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupPaladinSpells() function. SetupPaladinSpells() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupPaladinSpells() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************

bool SealOfRighteousness(uint32 i, Aura* pAura, bool apply)
{
	if(i != 0) return false;

	uint32 applyId = 20187;
	Unit* u_caster = pAura->GetUnitCaster();

	if(u_caster == 0 || !u_caster->IsPlayer()) return false;

	SpellEntry * m_spellInfo = dbcSpell.LookupEntry(applyId);

	if(apply == true)
	{
		float MWS = 1.0f;
		Item* mainHand = TO_PLAYER(u_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
		if(mainHand && mainHand->GetProto())
			MWS = mainHand->GetProto()->Delay / 1000.0f;

		uint32 AP = u_caster->GetAP();
		uint32 SPH = u_caster->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + SCHOOL_HOLY);
		float damage = MWS * (0.022f * AP + 0.044f * SPH);

		uint32 max_dmg = uint32(damage * 1.01f);
		uint32 min_dmg = uint32(damage * 0.99f);
		TO_PLAYER(u_caster)->AddOnStrikeSpellDamage(pAura->m_spellProto->Id, min_dmg, max_dmg);
	}
	else
		TO_PLAYER(u_caster)->RemoveOnStrikeSpellDamage(pAura->m_spellProto->Id);

	return true;
}

// -----------------------------------------------------------------------------

bool HolyShock(uint32 i, Spell* pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	uint32 newspell = 0;

	if( isAttackable(pSpell->p_caster, target, true) ) // if its an enemy
	{
		switch(pSpell->m_spellInfo->Id)
		{
		case 20473:
			newspell = 25912;
			break;
		case 20929:
			newspell = 25911;
			break;
		case 20930:
			newspell = 25902;
			break;
		case 27174:
			newspell = 27176;
			break;
		case 33072:
			newspell = 33073;
			break;
		case 48824:
			newspell = 48822;
			break;
		case 48825:
			newspell = 48823;
			break;
		}
	}
	else // if its friendly
	{
		switch(pSpell->m_spellInfo->Id)
		{
		case 20473:
			newspell = 25914;
			break;
		case 20929:
			newspell = 25913;
			break;
		case 20930:
			newspell = 25903;
			break;
		case 27174:
			newspell = 27175;
			break;
		case 33072:
			newspell = 33074;
			break;
		case 48824:
			newspell = 48820;
			break;
		case 48825:
			newspell = 48821;
			break;
		}
	}

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(target, spInfo, true);
	return true;
}

// ADD NEW FUNCTIONS ABOVE THIS LINE
// *****************************************************************************

void SetupPaladinSpells(ScriptMgr * mgr)
{
	mgr->register_dummy_aura( 21084, &SealOfRighteousness); // Seal of Righteousness
	mgr->register_dummy_spell(20473, &HolyShock);           // Holy Shock rank 1
	mgr->register_dummy_spell(20929, &HolyShock);           // Holy Shock rank 2
	mgr->register_dummy_spell(20930, &HolyShock);           // Holy Shock rank 3
	mgr->register_dummy_spell(27174, &HolyShock);           // Holy Shock rank 4
	mgr->register_dummy_spell(33072, &HolyShock);           // Holy Shock rank 5
	mgr->register_dummy_spell(48824, &HolyShock);           // Holy Shock rank 6
	mgr->register_dummy_spell(48825, &HolyShock);           // Holy Shock rank 7

// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
