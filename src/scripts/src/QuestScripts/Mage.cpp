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
#include "../Common/EasyFunctions.h"



class FragmentedMagic : public CreatureAIScript
{
private:
  uint32 current_aura;
public:
  ADD_CREATURE_FACTORY_FUNCTION(FragmentedMagic);
  FragmentedMagic(Creature* pCreature) : CreatureAIScript(pCreature) 
  {
    RegisterAIUpdateEvent(5000);
    current_aura = 0;
  }

  void SetWander(Creature* m_target, Player* p_caster)
  {
    m_target->m_special_state |= UNIT_STATE_CONFUSE;
    m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);

    m_target->EnableAI();
    m_target->GetAIInterface()->HandleEvent(EVENT_WANDER, p_caster, 0);
  }

  void AIUpdate()
  {
    bool auraOk = false;
    const uint32 auras[] = {118, 12824, 12825, 12826}; // Polymorph rank 1,2,3,4
    
    for(int i = 0; i<4; i++)
    {   
      if(_unit->HasAura(auras[i]))
      {
        current_aura = auras[i];
        auraOk = true;
        
        break;
      }
    }
    
    if(!auraOk)
      return;

    bool casterOk = false;
    Player* p_caster;

    for(int i = 0; i<MAX_AURAS+MAX_PASSIVE_AURAS; i++)
    {
      if(_unit->m_auras[i] == NULL)
        continue;

      if(_unit->m_auras[i]->GetSpellId() == current_aura)
      {
        if(!_unit->m_auras[i]->GetCaster()->IsPlayer())
          break;

        p_caster = TO_PLAYER(_unit->m_auras[i]->GetCaster());

        if(p_caster == NULL)
          break;

        casterOk = true;

        break;
      }
    }

    if(!casterOk)
      return;

    QuestLogEntry *qle = p_caster->GetQuestLogForEntry(9364);
    if(qle == NULL)
      return;

    _unit->Despawn(0, 1*60*1000);

    uint8 num = RandomUInt(5);
    for(int i=0; i<num; i++)
    {
      Creature* cr = sEAS.SpawnCreature(p_caster, 16479, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, 1*60*1000);
      SetWander(cr, p_caster);
    }
  }
};

void SetupMage(ScriptMgr * mgr)
{
  mgr->register_creature_script(6193, &FragmentedMagic::Create);
  mgr->register_creature_script(6194, &FragmentedMagic::Create);
  mgr->register_creature_script(6190, &FragmentedMagic::Create);
  mgr->register_creature_script(6196, &FragmentedMagic::Create);
}
