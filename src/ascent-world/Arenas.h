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

class ArenaTeam;
struct PlayerInfo;

class Arena : public CBattleground
{
	set< GameObjectPointer > m_gates;
	uint32 m_arenateamtype;

	uint32 m_pcWorldStates[2];
	GameObjectPointer m_buffs[2];

	uint32 m_playersCount[2];
	map_t m_players2[2];
	map_t m_playersAlive;

public:
	bool rated_match;
	Arena( MapMgrPointer mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side);
	virtual ~Arena();
	virtual void Init();

	bool HookHandleRepop(PlayerPointer plr);
	void OnAddPlayer(PlayerPointer plr);
	void OnRemovePlayer(PlayerPointer plr);
	void OnCreate();
	void HookOnPlayerDeath(PlayerPointer plr);
	void HookOnPlayerKill(PlayerPointer plr, UnitPointer pVictim);
	void HookOnHK(PlayerPointer plr);
	void HookOnShadowSight();
	void UpdatePlayerCounts();
	LocationVector GetStartingCoords(uint32 Team);
	virtual const char * GetName() { return "Arena"; }
	void OnStart();
	bool CanPlayerJoin(PlayerPointer plr)
	{
		if(m_started)
			return false;
		else
			return CBattleground::CanPlayerJoin(plr);
	}

	bool CreateCorpse(PlayerPointer plr) { return false; }

	/* dummy stuff */
	void HookOnMount(PlayerPointer plr) {}
	void HookFlagDrop(PlayerPointer plr, GameObjectPointer obj) {}
	void HookFlagStand(PlayerPointer plr, GameObjectPointer obj) {}
	void HookOnAreaTrigger(PlayerPointer plr, uint32 id);

	void BuffRespawn(uint32 buffId);

	int32 GetFreeTeam()
	{
		size_t c0 = m_players[0].size() + m_pendPlayers[0].size();
		size_t c1 = m_players[1].size() + m_pendPlayers[1].size();
		if(m_started) return -1;

		// Check if there is free room, if yes, return team with less members
		return ((c0 + c1 >= m_playerCountPerTeam * 2) ? -1 : (c0 > c1));

		/* We shouldn't reach here. */
	}

	void Finish();
	uint8 Rated() { return rated_match; }
	ASCENT_INLINE uint32 GetArenaTeamType() { return m_arenateamtype; }

	/* looooooot */
	bool SupportsPlayerLoot() { return false; }
	void HookGenerateLoot(PlayerPointer plr, CorpsePointer pCorpse) {}

	int32 CalcDeltaRating(uint32 oldRating, uint32 opponentRating, bool outcome);
	int32 m_teams[2]; // Ids of teams fighting in rated match
};
