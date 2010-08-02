/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _EOTS_H
#define _EOTS_H

#define EOTS_TOWER_COUNT 4

class EyeOfTheStorm : public CBattleground
{
public:
	EyeOfTheStorm(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~EyeOfTheStorm();

	void HookOnPlayerDeath(Player * plr);
	void HookFlagDrop(Player * plr, GameObject * obj);
	void HookFlagStand(Player * plr, GameObject * obj);
	void HookOnMount(Player * plr);
	void HookOnAreaTrigger(Player * plr, uint32 id);
	bool HookHandleRepop(Player * plr);
	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	void OnCreate();
	void HookOnPlayerKill(Player * plr, Unit * pVictim);
	void HookOnHK(Player * plr);
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new EyeOfTheStorm(m, i, l, t); }

	const char * GetName() { return "Eye of the Storm"; }
	void OnStart();

	void UpdateCPs();
	void GeneratePoints();

	// returns true if that team won
	bool GivePoints(uint32 team, uint32 points);

	void RespawnCPFlag(uint32 i, uint32 id);		// 0 = Neutral, <0 = Leaning towards alliance, >0 Leaning towards horde

	bool HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell);
	void DropFlag(Player * plr);
	void EventResetFlag();
	void RepopPlayersOfTeam(int32 team, Creature * sh);

	/* looooooot */
	bool SupportsPlayerLoot() { return true; }
	void HookGenerateLoot(Player *plr, Corpse *pCorpse);

protected:
	uint32 m_resourceRewards[2];
	int32 m_CPStatus[EOTS_TOWER_COUNT];		
	uint32 m_flagHolder;

	GameObject * m_standFlag;
	GameObject * m_dropFlag;

	GameObject * m_CPStatusGO[EOTS_TOWER_COUNT];
	GameObject * m_CPBanner[EOTS_TOWER_COUNT];
	GameObject * m_bubbles[2];

	typedef map<uint32, uint32> EOTSStoredPlayerMap;
	EOTSStoredPlayerMap m_CPStored[EOTS_TOWER_COUNT];
	int32 m_towerCount[2];

	uint32 m_points[2];
	Creature * m_spiritGuides[EOTS_TOWER_COUNT];
};

#endif		// _EOTS_H
