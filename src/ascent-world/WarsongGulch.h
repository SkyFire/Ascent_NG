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

#define BUFF_RESPAWN_TIME 90000

class WarsongGulch : public CBattleground
{
	GameObject * m_buffs[6];
	GameObject * m_homeFlags[2];
	GameObject * m_dropFlags[2];
	uint32 m_flagHolders[2];
	list<GameObject*> m_gates;
	uint32 m_scores[2];
	bool m_flagAtBase[2];
#ifdef BG_ANTI_CHEAT
	uint32 m_flagPickUpTimestamp[2];
#endif
public:
	WarsongGulch(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~WarsongGulch();

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
	void DropFlag(Player * plr);
	void ReturnFlag(uint32 team);

	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new WarsongGulch(m, i, l, t); }

	const char * GetName() { return "Warsong Gulch"; }
	void OnStart();

	/* looooooot */
	bool SupportsPlayerLoot() { return true; }
	void HookGenerateLoot(Player *plr, Corpse *pCorpse);
};
