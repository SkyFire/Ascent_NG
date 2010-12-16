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

class PortalEffectSunwellIsle : public GameObjectAIScript
  {
  public:
          PortalEffectSunwellIsle(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
          void OnActivate(Player* pPlayer)
          {
                if(pPlayer->getLevel() >= 70) 
                {
                  //pPlayer->SafeTeleport(530, 4080, 12803.374023f, -6907.089844f, 41.114765f, 2.251736f);
					pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(44870), true);
                }
                else if(pPlayer->getLevel() <= 69)
                {
					pPlayer->BroadcastMessage("You must be level 70 to use this!");
                }
          }
          static GameObjectAIScript *Create(GameObject* GO) { return new PortalEffectSunwellIsle(GO); }
  };
GameObjectAIScript * create_go(GameObject* GO) { return new PortalEffectSunwellIsle(GO); }
void SetupQDGoHandlers(ScriptMgr * mgr)
{

      mgr->register_gameobject_script(187056, &PortalEffectSunwellIsle::Create);
   
}
