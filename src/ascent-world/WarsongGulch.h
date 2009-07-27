/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
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

#define BUFF_RESPAWN_TIME 90000

class WarsongGulch : public CBattleground
{
	GameObjectPointer m_buffs[6];
	GameObjectPointer m_homeFlags[2];
	GameObjectPointer m_dropFlags[2];
	uint32 m_flagHolders[2];
	list<GameObjectPointer > m_gates;
	uint32 m_scores[2];
	uint32 m_lgroup;
	bool m_flagAtBase[2];
public:
	WarsongGulch(MapMgrPointer mgr, uint32 id, uint32 lgroup, uint32 t);
	~WarsongGulch();
	virtual void Init();

	void HookOnPlayerDeath(PlayerPointer plr);
	void HookFlagDrop(PlayerPointer plr, GameObjectPointer obj);
	void HookFlagStand(PlayerPointer plr, GameObjectPointer obj);
	void HookOnMount(PlayerPointer plr);
	void HookOnAreaTrigger(PlayerPointer plr, uint32 id);
	bool HookHandleRepop(PlayerPointer plr);
	void OnAddPlayer(PlayerPointer plr);
	void OnRemovePlayer(PlayerPointer plr);
	void OnCreate();
	void HookOnPlayerKill(PlayerPointer plr, UnitPointer pVictim);
	void HookOnHK(PlayerPointer plr);
	void HookOnShadowSight();
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	void DropFlag(PlayerPointer plr);
	void ReturnFlag(uint32 team);

	static BattlegroundPointer Create(MapMgrPointer m, uint32 i, uint32 l, uint32 t) { return WarsongGulchPointer(new WarsongGulch(m, i, l, t)); }

	uint32 GetAllianceFlagHolderGUID() { return m_flagHolders[0]; }
	uint32 GetHordeFlagHolderGUID() { return m_flagHolders[1]; }
	const char * GetName() { return "Warsong Gulch"; }
	void OnStart();

	/* looooooot */
	bool SupportsPlayerLoot() { return true; }
	void HookGenerateLoot(PlayerPointer plr, CorpsePointer pCorpse);

	void SetIsWeekend(bool isweekend);
};
