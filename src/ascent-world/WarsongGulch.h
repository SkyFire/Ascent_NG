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

#define BUFF_RESPAWN_TIME 90000

class WarsongGulch : public CBattleground
{
	GameObject* m_buffs[6];
	GameObject* m_homeFlags[2];
	GameObject* m_dropFlags[2];
	uint32 m_flagHolders[2];
	list<GameObject* > m_gates;
	uint32 m_scores[2];
	uint32 m_lgroup;
	bool m_flagAtBase[2];
	uint32 m_FlagCaptureHonor;
	uint32 m_WinHonor;
	uint32 m_CompleteHonor;
public:
	WarsongGulch(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
	~WarsongGulch();
	virtual void Init();

	void HookOnPlayerDeath(Player* plr);
	void HookFlagDrop(Player* plr, GameObject* obj);
	void HookFlagStand(Player* plr, GameObject* obj);
	void HookOnMount(Player* plr);
	void HookOnAreaTrigger(Player* plr, uint32 id);
	bool HookHandleRepop(Player* plr);
	void OnAddPlayer(Player* plr);
	void OnRemovePlayer(Player* plr);
	void OnCreate();
	void HookOnPlayerKill(Player* plr, Unit* pVictim);
	void HookOnHK(Player* plr);
	void HookOnShadowSight();
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	void DropFlag(Player* plr);
	void ReturnFlag(uint32 team);

	static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

	uint32 GetAllianceFlagHolderGUID() { return m_flagHolders[0]; }
	uint32 GetHordeFlagHolderGUID() { return m_flagHolders[1]; }
	const char * GetName() { return "Warsong Gulch"; }
	void OnStart();

	/* looooooot */
	bool SupportsPlayerLoot() { return true; }
	void HookGenerateLoot(Player* plr, Corpse* pCorpse);

	void SetIsWeekend(bool isweekend);
};
