/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
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

	uint32 applyId = 0;
	Unit * u_caster = pAura->GetUnitCaster();

	if(u_caster == 0 || !u_caster->IsPlayer()) return false;

	switch(pAura->m_spellProto->Id)
	{

	case 20154:         // Rank 1: "Melee attacks cause an additional $/87;20187s3 to $/25;20187s3 Holy damage."
	case 21084:
			applyId = 20187;
			break;
	case 20287:         // Rank 2: $/25;20280s3
			applyId = 20280;
			break;
	case 20288:         // Rank 3:
			applyId = 20281;
			break;
	case 20289:         // Rank 4
			applyId = 20282;
			break;
	case 20290:         // Rank 5
			applyId = 20283;
			break;
	case 20291:         // Rank 6
			applyId = 20284;
			break;
	case 20292:         // Rank 7
			applyId = 20285;
			break;
	case 20293:         // Rank 8
			applyId = 20286;
			break;
	case 27155:         // Rank 9
			applyId = 27157;
			break;
	}

	SpellEntry * m_spellInfo = dbcSpell.LookupEntry(applyId);

	if(apply == true)
	{
		int32 value = 0;
		float randomPointsPerLevel = m_spellInfo->EffectDicePerLevel[2];
		int32 basePoints = m_spellInfo->EffectBasePoints[2] + 1;
		int32 randomPoints = m_spellInfo->EffectDieSides[2];
		if(u_caster)
				randomPoints += u_caster->getLevel() * (int32)randomPointsPerLevel;

		if(randomPoints <= 1)
				value = basePoints;
		else
				value = basePoints + rand()  %randomPoints;

		//this may be dangerous but let it be
		/*if(m_spellInfo->SpellGroupType)
		{
				SM_FIValue(u_caster->SM_FDummy,&value,m_spellInfo->SpellGroupType);
				SM_PIValue(u_caster->SM_PDummy,&value,m_spellInfo->SpellGroupType);
		}*/

		// add spell damage!
		uint32 max_dmg = value / 21;
		uint32 min_dmg = value / 27;
		((Player*)u_caster)->AddOnStrikeSpellDamage(pAura->m_spellProto->Id, min_dmg, max_dmg);

		// set the seal business
		if(u_caster->GetTypeId() == TYPEID_PLAYER)
		{
				((Player*)u_caster)->judgespell = applyId;
				((Player*)u_caster)->Seal = pAura->m_spellProto->Id;
		}
		u_caster->SetFlag(UNIT_FIELD_AURASTATE, 16);
	}
	else
	{
			((Player*)u_caster)->RemoveOnStrikeSpellDamage(pAura->m_spellProto->Id);
			// set the seal business
			if(u_caster->GetTypeId() == TYPEID_PLAYER)
			{
					((Player*)u_caster)->judgespell = 0;
					((Player*)u_caster)->Seal = 0;
			}
			u_caster->RemoveFlag(UNIT_FIELD_AURASTATE, 16);
	}

	return true;
}

// -----------------------------------------------------------------------------

bool HolyShock(uint32 i, Spell *pSpell)
{
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	uint32 newspell = 0;

	if(isAttackable(pSpell->p_caster,target)) // if its an enemy
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
	mgr->register_dummy_aura( 21084, &SealOfRighteousness); // Judgement of Justice (Rank 1)
	mgr->register_dummy_aura( 20154, &SealOfRighteousness); // Seal of Righteousness (Rank 1)
	mgr->register_dummy_aura( 20287, &SealOfRighteousness); // Seal of Righteousness (Rank 2)
	mgr->register_dummy_aura( 20288, &SealOfRighteousness); // Seal of Righteousness (Rank 3)
	mgr->register_dummy_aura( 20289, &SealOfRighteousness); // Seal of Righteousness (Rank 4)
	mgr->register_dummy_aura( 20290, &SealOfRighteousness); // Seal of Righteousness (Rank 5)
	mgr->register_dummy_aura( 20291, &SealOfRighteousness); // Seal of Righteousness (Rank 6)
	mgr->register_dummy_aura( 20292, &SealOfRighteousness); // Seal of Righteousness (Rank 7)
	mgr->register_dummy_aura( 20293, &SealOfRighteousness); // Seal of Righteousness (Rank 8)
	mgr->register_dummy_aura( 27155, &SealOfRighteousness); // Seal of Righteousness (Rank 9)
	mgr->register_dummy_spell(20473, &HolyShock);           // Holy Shock rank 1
	mgr->register_dummy_spell(20929, &HolyShock);           // Holy Shock rank 2
	mgr->register_dummy_spell(20930, &HolyShock);           // Holy Shock rank 3
	mgr->register_dummy_spell(27174, &HolyShock);           // Holy Shock rank 4
	mgr->register_dummy_spell(33072, &HolyShock);           // Holy Shock rank 5



// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
