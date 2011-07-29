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


#ifndef _EOTS_H
#define _EOTS_H

#define EOTS_TOWER_COUNT 4
#define EOTS_BUFF_RESPAWN_TIME 90000

class EyeOfTheStorm : public CBattleground
{
public:
	EyeOfTheStorm( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t);
	~EyeOfTheStorm();
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
	static CBattleground* Create( MapMgr* m, uint32 i, uint32 l, uint32 t) { return new EyeOfTheStorm(m, i, l, t); }
	uint32 GetFlagHolderGUID() { return m_flagHolder; }

	const char * GetName() { return "Eye of the Storm"; }
	void OnStart();

	void UpdateCPs();
	void GeneratePoints();

	// returns true if that team won
	bool GivePoints(uint32 team, uint32 points);

	void RespawnCPFlag(uint32 i, uint32 id);		// 0 = Neutral, <0 = Leaning towards alliance, >0 Leaning towards horde

	bool HookSlowLockOpen( GameObject* pGo, Player* pPlayer, Spell* pSpell);
	void DropFlag(Player* plr);
	void EventResetFlag();
	void RepopPlayersOfTeam(int32 team, Creature* sh);

	/* looooooot */
	bool SupportsPlayerLoot() { return true; }
	void HookGenerateLoot(Player* plr, Corpse* pCorpse);

	void SetIsWeekend(bool isweekend);

protected:
	uint32 m_resourceRewards[2];
	int32 m_CPStatus[EOTS_TOWER_COUNT];		
	uint32 m_flagHolder;

	int m_bonusHonor;
	uint32 m_resToGainBH;

	GameObject* m_standFlag;
	GameObject* m_dropFlag;

	GameObject* m_CPStatusGO[EOTS_TOWER_COUNT];
	GameObject* m_CPBanner[EOTS_TOWER_COUNT];
	GameObject* m_bubbles[2];
	GameObject* m_EOTSbuffs[4];

	typedef map<uint32, uint32> EOTSStoredPlayerMap;
	EOTSStoredPlayerMap m_CPStored[EOTS_TOWER_COUNT];
	int32 m_towerCount[2];

	uint32 m_points[2];
	Creature* m_spiritGuides[EOTS_TOWER_COUNT];
};

#endif		// _EOTS_H
