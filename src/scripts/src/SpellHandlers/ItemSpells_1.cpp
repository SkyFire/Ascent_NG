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
	How to add a new item spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupItemSpells_1() function. SetupItemSpells_1() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupItemSpells_1() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************

bool GnomishTransporter(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	pSpell->p_caster->EventAttackStop();
	pSpell->p_caster->SafeTeleport(1, 0, LocationVector(-7169.41f, -3838.63f, 8.72f));
	return true;
}

// -----------------------------------------------------------------------------

bool NoggenFoggerElixr(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	uint32 chance = RandomUInt(2);

	switch(chance)
	{
	case 0:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16591), true);
		break;
	case 1:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16593), true);
		break;
	case 2:
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(16595), true);
		break;
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HallowsEndCandy(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int newspell = 24924 + RandomUInt(3);

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool DeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int newspell = 8064 + RandomUInt(4);

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool CookedDeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = 0;
	int newspell = 0;

	chance = RandomUInt(1);

	switch(chance)
	{
	case 0:
		newspell = 8219; // Flip Out (60 min) (turns you into a ninja)
		break;
	case 1:
		newspell = 8221; // Yaaarrrr (60 min) (turns you into a pirate)
		break;
	}

	if(newspell)
	{
		SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
		if(!spInfo) return true;

		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HolidayCheer(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster) return true;

	Unit *target;
	float dist = pSpell->GetRadius(i);

	for(ObjectSet::iterator itr = pSpell->m_caster->GetInRangeSetBegin(); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsUnit())
			target = (Unit*)*itr;
		else
			continue;

		if(pSpell->m_caster->CalcDistance(target) > dist || isAttackable(pSpell->m_caster, target))
			continue;

		target->Emote(EMOTE_ONESHOT_LAUGH);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool NetOMatic(uint32 i, Spell * pSpell)
{
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	SpellEntry *spInfo = dbcSpell.LookupEntry(13099);
	if(!spInfo) return true;

	int chance = RandomUInt(99)+1;

	if(chance < 51) // nets target: 50%
		pSpell->p_caster->CastSpell(target, spInfo, true);

	else if(chance < 76) // nets you instead: 25%
		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);

	else // nets you and target: 25%
	{
		pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
		pSpell->p_caster->CastSpell(target, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool BanishExile(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target) return true;

	pSpell->p_caster->SpellNonMeleeDamageLog(target, pSpell->m_spellInfo->Id, target->GetUInt32Value(UNIT_FIELD_HEALTH), true);
	return true;
}

// -----------------------------------------------------------------------------

bool ForemansBlackjack(uint32 i, Spell *pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	// play sound and animation
	WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
	data << uint32(6197) << target->GetGUID();
	pSpell->p_caster->SendMessageToSet(&data, true);

	// send chat message
	char msg[100];
	sprintf(msg, "Ow! Ok, I'll get back to work, %s", pSpell->p_caster->GetName());
	target->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);

	Creature* c_target = (Creature*)target;
	if(!c_target) return true;

	uint32 creatureID = c_target->GetEntry();

  // check to see that we have the correct creature and increment the quest log
	if(creatureID == 10556)
		sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);

	return true;
}

// -----------------------------------------------------------------------------

bool NetherWraithBeacon(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	float SSX = pSpell->p_caster->GetPositionX();
	float SSY = pSpell->p_caster->GetPositionY();
	float SSZ = pSpell->p_caster->GetPositionZ();
	float SSO = pSpell->p_caster->GetOrientation();

	pSpell->p_caster->GetMapMgr()->GetInterface()->SpawnCreature(22408, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}

// -----------------------------------------------------------------------------

bool SymbolOfLife(uint32 i, Spell *pSpell)
{
	/*  // commented out until the SpellTargetDummy (38) targeting is fixed
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	Creature* c_target = (Creature*)target;
	if(!c_target) return true;

	uint32 creatureID = c_target->GetEntry();

  // check to see we that have the correct creature and increment the quest log
	if(creatureID == 17542 || creatureID == 6172)
		sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);
	*/
	return true;
}

// -----------------------------------------------------------------------------

bool NighInvulnBelt(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	int chance = RandomUInt(99)+1;

	if(chance > 10) // Buff - Nigh-Invulnerability - 30456
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(30456), true);
	else // Malfunction - Complete Vulnerability - 30457
		pSpell->p_caster->CastSpell(pSpell->p_caster, dbcSpell.LookupEntry(30457), true);

	return true;
}

// -----------------------------------------------------------------------------

bool ReindeerTransformation(uint32 i, Spell *pSpell)
{
	if(!pSpell->p_caster) return true;

	if(pSpell->p_caster->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) != 0)
	{
		if(pSpell->p_caster->m_setflycheat)
			pSpell->p_caster->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 22724);
		else
			pSpell->p_caster->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 15902);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool SummonCritterDummy(uint32 i, Spell *pSpell)
{
	// the reason these spells have to be scripted is because they require a
	// reagent to summon the critter pet, but don't require one to dismiss it

	if(!pSpell->p_caster) return true;

	uint32 currentCritterID = 0;

	if(pSpell->p_caster->critterPet && pSpell->p_caster->critterPet->GetCreatureName())
		currentCritterID = pSpell->p_caster->critterPet->GetCreatureName()->Id;

	uint32 newspell = 0;

	switch(pSpell->m_spellInfo->Id)
	{
		case 26469: // Snowman Kit
		{
			if(currentCritterID == 15710) // do we already have this critter summoned?
				newspell = 26468; // if so, dismiss it
			else
				newspell = 26045; // otherwise summon it
		}	break;

		case 26528: // Jingling Bell
		{
			if(currentCritterID == 15706) // do we already have this critter summoned?
				newspell = 26530; // if so, dismiss it
			else
				newspell = 26529; // otherwise summon it
		}	break;

		case 26532: // Green Helper Box
		{
			if(currentCritterID == 15698) // do we already have this critter summoned?
				newspell = 26534; // if so, dismiss it
			else
				newspell = 26533; // otherwise summon it
		}	break;

		case 26541: // Red Helper Box
		{
			if(currentCritterID == 15705) // do we already have this critter summoned?
				newspell = 26537; // if so, dismiss it
			else
				newspell = 26536; // otherwise summon it
		}	break;
	}

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, false); // these spells have to check items, so "triggeredspell" must be false
	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervolt(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();

	if(!target || target->GetTypeId() != TYPEID_PLAYER) return true;

	target->CastSpell(target, dbcSpell.LookupEntry(26274), true);

	return true;
}

// -----------------------------------------------------------------------------
bool DeadlyThrowDummyEffect(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!target) return true;

	if(pSpell->m_caster->IsPlayer() && static_cast<Player*>(pSpell->m_caster)->HasAura(32748))
		pSpell->SpellEffectInterruptCast(i);

	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervoltAura(uint32 i, Aura* pAura, bool apply)
{
	Unit *u_caster = pAura->GetUnitCaster();

	if(!u_caster || !u_caster->IsPlayer()) return true;

	if(apply)
	{
		uint32 displayId;
		uint32 chance = RandomUInt(7);

		if(u_caster->getGender() == 1) displayId = 15795 + chance; // female 0-7
		else if(chance == 0)           displayId = 15687;          // male   0
		else                           displayId = 15802 + chance; // male   1-7

		u_caster->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayId);
	}
	else
		u_caster->SetUInt32Value(UNIT_FIELD_DISPLAYID, u_caster->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));

	return true;
}

// -----------------------------------------------------------------------------




// ADD NEW FUNCTIONS ABOVE THIS LINE
// *****************************************************************************

void SetupItemSpells_1(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(23453, &GnomishTransporter);      // Gnomish Transporter
	mgr->register_dummy_spell(16589, &NoggenFoggerElixr);       // Noggenfogger
	mgr->register_dummy_spell(24430, &HallowsEndCandy);         // Hallows End Candy
	mgr->register_dummy_spell( 8063, &DeviateFish);             // Deviate Fish
	mgr->register_dummy_spell( 8213, &CookedDeviateFish);       // Savory Deviate Delight
	mgr->register_dummy_spell(26074, &HolidayCheer);            // Holiday Cheer
	mgr->register_dummy_spell(13120, &NetOMatic);               // Net-o-Matic
	mgr->register_dummy_spell( 4130, &BanishExile);             // Essence of the Exile Quest
	mgr->register_dummy_spell( 4131, &BanishExile);             // Essence of the Exile Quest
	mgr->register_dummy_spell( 4132, &BanishExile);             // Essence of the Exile Quest
	mgr->register_dummy_spell(19938, &ForemansBlackjack);       // Lazy Peons Quest
	mgr->register_dummy_spell(39105, &NetherWraithBeacon);      // Spellfire Tailor Quest
	mgr->register_dummy_spell( 8593, &SymbolOfLife);            // Paladin's Redemption QuestLine
	mgr->register_dummy_spell(30458, &NighInvulnBelt);          // Nigh Invulnerability Belt
	mgr->register_dummy_spell(25860, &ReindeerTransformation);  // Fresh Holly & Preserved Holly
	mgr->register_dummy_spell(26469, &SummonCritterDummy);      // Snowman Kit
	mgr->register_dummy_spell(26528, &SummonCritterDummy);      // Jingling Bell
	mgr->register_dummy_spell(26532, &SummonCritterDummy);      // Green Helper Box
	mgr->register_dummy_spell(26541, &SummonCritterDummy);      // Red Helper Box
	mgr->register_dummy_spell(26275, &WinterWondervolt);        // PX-238 Winter Wondervolt Trap
	mgr->register_dummy_aura( 26274, &WinterWondervoltAura);    // PX-238 Winter Wondervolt Transform Aura
	mgr->register_dummy_spell( 26679, &DeadlyThrowDummyEffect);


// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
