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

bool Refocus(uint32 i, Spell* pSpell)
{
    Player* playerTarget = pSpell->GetPlayerTarget();
    if(playerTarget == 0) return true;

    SpellSet::const_iterator itr = playerTarget->mSpells.begin();
    for(; itr != playerTarget->mSpells.end(); ++itr)
    {
        if((*itr) == 24531)       // skip calling spell.. otherwise spammies! :D
            continue;

        if((*itr) == 19434 || (*itr) == 20900 || (*itr) == 20901 || (*itr) == 20902 || (*itr) == 20903 || (*itr) == 20904 || (*itr) == 27632
            || (*itr) == 2643 || (*itr) == 14288|| (*itr) == 14289|| (*itr) == 14290 || (*itr) == 25294 || (*itr) == 14443 || (*itr) == 18651 || (*itr) == 20735 || (*itr) == 21390
            || (*itr) == 1510 || (*itr) == 14294 || (*itr) == 14295 || (*itr) == 1540 || (*itr) == 22908
            || (*itr) == 3044 || (*itr) == 14281 || (*itr) == 14282 || (*itr) == 14283 || (*itr) == 14284 || (*itr) == 14285 || (*itr) == 14286 || (*itr) == 14287)
            playerTarget->ClearCooldownForSpell((*itr));
    }
    return true;
}

bool Readiness(uint32 i, Spell* pSpell)
{
    if(!pSpell->p_caster)
		return true;
    pSpell->p_caster->ClearCooldownsOnLine(50 , pSpell->m_spellInfo->Id);//Beast Mastery
    pSpell->p_caster->ClearCooldownsOnLine(163, pSpell->m_spellInfo->Id);//Marksmanship
    pSpell->p_caster->ClearCooldownsOnLine(51 , pSpell->m_spellInfo->Id);//Survival
    return true;
}

void SetupHunterSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(24531, &Refocus);
    mgr->register_dummy_spell(23989, &Readiness);
}
