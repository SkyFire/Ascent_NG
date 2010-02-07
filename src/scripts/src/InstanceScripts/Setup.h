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

#include <string>
#ifndef INSTANCE_SCRIPTS_SETUP_H
#define INSTANCE_SCRIPTS_SETUP_H

//Instances
void SetupAlteracValleyBattleground (ScriptMgr * mgr);
void SetupArcatraz(ScriptMgr * mgr);
void SetupAuchenaiCrypts(ScriptMgr * mgr);
void SetupAzjolNerub(ScriptMgr * mgr) ;
void SetupTheBlackMorass(ScriptMgr * mgr);
void SetupBlackrockSpire(ScriptMgr *mgr);
void SetupBlackrockDepths(ScriptMgr * mgr);
void SetupBloodFurnace(ScriptMgr * mgr);
void SetupBotanica(ScriptMgr * mgr);
void SetupCullingOfStratholme(ScriptMgr * mgr);
void SetupDrakTharonKeep(ScriptMgr* pScriptMgr);
void SetupDeadmines(ScriptMgr * mgr);
void SetupDireMaul(ScriptMgr * mgr);
void SetupHellfireRamparts(ScriptMgr * mgr);
void SetupHallsOfStone(ScriptMgr * mgr);
void SetupHallsofLightning(ScriptMgr * mgr);
void SetupManaTombs(ScriptMgr * mgr);
void SetupMaraudon(ScriptMgr * mgr);
void SetupNexus(ScriptMgr * mgr);
void SetupOldHillsbradFoothills(ScriptMgr * mgr);
void SetupRagefireChasm(ScriptMgr * mgr);
void SetupRazorfenDowns(ScriptMgr * mgr);
void SetupScarletMonastery(ScriptMgr * mgr);
void SetupScholomance(ScriptMgr * mgr);
void SetupSethekkHalls(ScriptMgr * mgr);
void SetupShadowfangKeep(ScriptMgr * mgr);
void SetupShadowLabyrinth(ScriptMgr * mgr);
void SetupTheMechanar(ScriptMgr * mgr);
void SetupTheShatteredHalls(ScriptMgr * mgr);
void SetupTheSlavePens(ScriptMgr * mgr);
void SetupTheSteamvault(ScriptMgr * mgr);
void SetupTheUnderbog(ScriptMgr * mgr);
void SetupUldaman(ScriptMgr * mgr);
void SetupUtgardeKeep(ScriptMgr * mgr);
void SetupTheStockade(ScriptMgr * mgr);
void SetupWailingCaverns(ScriptMgr * mgr);
void SetupMagistersTerrace(ScriptMgr * mgr);

struct ScriptSpell
{
	uint32 normal_spellid;
	uint32 heroic_spellid;
	uint32 timer;
	uint32 time;
	uint32 chance;
	uint32 target;
	uint32 phase;
};

enum SPELL_TARGETS
{
	SPELL_TARGET_SELF,
	SPELL_TARGET_CURRENT_ENEMY,
	SPELL_TARGET_RANDOM_PLAYER,
	SPELL_TARGET_SUMMONER,
	SPELL_TARGET_RANDOM_PLAYER_POSITION,
	SPELL_TARGET_GENERATE, // this will send null as target
	SPELL_TARGET_LOWEST_THREAT,
	SPELL_TARGET_CUSTOM,
};

struct SP_AI_Spell{
	SpellEntry *info;		// spell info
	char targettype;		// 0-self , 1-attaking target, ....
	bool instant;			// does it is instant or not?
	float perctrigger;		// % of the cast of this spell in a total of 100% of the attacks
	int attackstoptimer;	// stop the creature from attacking
	int soundid;			// sound id from DBC
	std::string speech;		// text displaied when spell was casted
	uint32 cooldown;		// spell cooldown
	uint32 casttime;		// "time" left to cast spell
	uint32 reqlvl;			// required level ? needed?
	float hpreqtocast;		// ? needed?
	float mindist2cast;		// min dist from caster to victim to perform cast (dist from caster >= mindist2cast)
	float maxdist2cast;		// max dist from caster to victim to perform cast (dist from caster <= maxdist2cast)
	int minhp2cast;			// min hp amount of victim to perform cast on it (health >= minhp2cast)
	int maxhp2cast;			// max hp amount of victim to perform cast on it (health <= maxhp2cast)
};

enum
{
	TARGET_SELF,
	TARGET_VARIOUS,
	TARGET_ATTACKING,
	TARGET_DESTINATION,
	TARGET_SOURCE,
	TARGET_RANDOM_FRIEND,	// doesn't work yet
	TARGET_RANDOM_SINGLE,
	TARGET_RANDOM_DESTINATION,

	//.....add
};

#endif
