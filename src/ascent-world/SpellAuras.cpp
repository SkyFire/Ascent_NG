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

#include "StdAfx.h"

pSpellAura SpellAuraHandler[TOTAL_SPELL_AURAS]={
		&Aura::SpellAuraNULL,//SPELL_AURA_NONE = 0
		&Aura::SpellAuraBindSight,//SPELL_AURA_BIND_SIGHT = 1
		&Aura::SpellAuraModPossess,//SPELL_AURA_MOD_POSSESS = 2,
		&Aura::SpellAuraPeriodicDamage,//SPELL_AURA_PERIODIC_DAMAGE = 3,
		&Aura::SpellAuraDummy,//SPELL_AURA_DUMMY = 4,
		&Aura::SpellAuraModConfuse,//SPELL_AURA_MOD_CONFUSE = 5,
		&Aura::SpellAuraModCharm,//SPELL_AURA_MOD_CHARM = 6,
		&Aura::SpellAuraModFear,//SPELL_AURA_MOD_FEAR = 7,
		&Aura::SpellAuraPeriodicHeal,//SPELL_AURA_PERIODIC_HEAL = 8,
		&Aura::SpellAuraModAttackSpeed,//SPELL_AURA_MOD_ATTACKSPEED = 9,
		&Aura::SpellAuraModThreatGenerated,//SPELL_AURA_MOD_THREAT = 10,
		&Aura::SpellAuraModTaunt,//SPELL_AURA_MOD_TAUNT = 11,
		&Aura::SpellAuraModStun,//SPELL_AURA_MOD_STUN = 12,
		&Aura::SpellAuraModDamageDone,//SPELL_AURA_MOD_DAMAGE_DONE = 13,
		&Aura::SpellAuraModDamageTaken,//SPELL_AURA_MOD_DAMAGE_TAKEN = 14,
		&Aura::SpellAuraDamageShield,//SPELL_AURA_DAMAGE_SHIELD = 15,
		&Aura::SpellAuraModStealth,//SPELL_AURA_MOD_STEALTH = 16,
		&Aura::SpellAuraModDetect,//SPELL_AURA_MOD_DETECT = 17,
		&Aura::SpellAuraModInvisibility,//SPELL_AURA_MOD_INVISIBILITY = 18,
		&Aura::SpellAuraModInvisibilityDetection,//SPELL_AURA_MOD_INVISIBILITY_DETECTION = 19,
		&Aura::SpellAuraModTotalHealthRegenPct,// SPELL_AURA_MOD_TOTAL_HEALTH_REGEN_PCT = 20
		&Aura::SpellAuraModTotalManaRegenPct,// SPELL_AURA_MOD_TOTAL_MANA_REGEN_PCT = 21
		&Aura::SpellAuraModResistance,//SPELL_AURA_MOD_RESISTANCE = 22,
		&Aura::SpellAuraPeriodicTriggerSpell,//SPELL_AURA_PERIODIC_TRIGGER_SPELL = 23,
		&Aura::SpellAuraPeriodicEnergize,//SPELL_AURA_PERIODIC_ENERGIZE = 24,
		&Aura::SpellAuraModPacify,//SPELL_AURA_MOD_PACIFY = 25,
		&Aura::SpellAuraModRoot,//SPELL_AURA_MOD_ROOT = 26,
		&Aura::SpellAuraModSilence,//SPELL_AURA_MOD_SILENCE = 27,
		&Aura::SpellAuraReflectSpells,//SPELL_AURA_REFLECT_SPELLS = 28,
		&Aura::SpellAuraModStat,//SPELL_AURA_MOD_STAT = 29,
		&Aura::SpellAuraModSkill,//SPELL_AURA_MOD_SKILL = 30,
		&Aura::SpellAuraModIncreaseSpeed,//SPELL_AURA_MOD_INCREASE_SPEED = 31,
		&Aura::SpellAuraModIncreaseMountedSpeed,//SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED = 32,
		&Aura::SpellAuraModDecreaseSpeed,//SPELL_AURA_MOD_DECREASE_SPEED = 33,
		&Aura::SpellAuraModIncreaseHealth,//SPELL_AURA_MOD_INCREASE_HEALTH = 34,
		&Aura::SpellAuraModIncreaseEnergy,//SPELL_AURA_MOD_INCREASE_ENERGY = 35,
		&Aura::SpellAuraModShapeshift,//SPELL_AURA_MOD_SHAPESHIFT = 36,
		&Aura::SpellAuraModEffectImmunity,//SPELL_AURA_EFFECT_IMMUNITY = 37,
		&Aura::SpellAuraModStateImmunity,//SPELL_AURA_STATE_IMMUNITY = 38,
		&Aura::SpellAuraModSchoolImmunity,//SPELL_AURA_SCHOOL_IMMUNITY = 39,
		&Aura::SpellAuraModDmgImmunity,//SPELL_AURA_DAMAGE_IMMUNITY = 40,
		&Aura::SpellAuraModDispelImmunity,//SPELL_AURA_DISPEL_IMMUNITY = 41,
		&Aura::SpellAuraProcTriggerSpell,//SPELL_AURA_PROC_TRIGGER_SPELL = 42,
		&Aura::SpellAuraProcTriggerDamage,//SPELL_AURA_PROC_TRIGGER_DAMAGE = 43,
		&Aura::SpellAuraTrackCreatures,//SPELL_AURA_TRACK_CREATURES = 44,
		&Aura::SpellAuraTrackResources,//SPELL_AURA_TRACK_RESOURCES = 45,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_PARRY_SKILL = 46, obsolete? not used in 1.12.1 spell.dbc
		&Aura::SpellAuraModParryPerc,//SPELL_AURA_MOD_PARRY_PERCENT = 47,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_DODGE_SKILL = 48, obsolete?
		&Aura::SpellAuraModDodgePerc,//SPELL_AURA_MOD_DODGE_PERCENT = 49,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_BLOCK_SKILL = 50, obsolete?,
		&Aura::SpellAuraModBlockPerc,//SPELL_AURA_MOD_BLOCK_PERCENT = 51,
		&Aura::SpellAuraModCritPerc,//SPELL_AURA_MOD_CRIT_PERCENT = 52,
		&Aura::SpellAuraPeriodicLeech,//SPELL_AURA_PERIODIC_LEECH = 53,
		&Aura::SpellAuraModHitChance,//SPELL_AURA_MOD_HIT_CHANCE = 54,
		&Aura::SpellAuraModSpellHitChance,//SPELL_AURA_MOD_SPELL_HIT_CHANCE = 55,
		&Aura::SpellAuraTransform,//SPELL_AURA_TRANSFORM = 56,
		&Aura::SpellAuraModSpellCritChance,//SPELL_AURA_MOD_SPELL_CRIT_CHANCE = 57,
		&Aura::SpellAuraIncreaseSwimSpeed,//SPELL_AURA_MOD_INCREASE_SWIM_SPEED = 58,
		&Aura::SpellAuraModCratureDmgDone,//SPELL_AURA_MOD_DAMAGE_DONE_CREATURE = 59, 
		&Aura::SpellAuraPacifySilence,//SPELL_AURA_MOD_PACIFY_SILENCE = 60,
		&Aura::SpellAuraModScale,//SPELL_AURA_MOD_SCALE = 61,
		&Aura::SpellAuraPeriodicHealthFunnel,//SPELL_AURA_PERIODIC_HEALTH_FUNNEL = 62,
		&Aura::SpellAuraNULL,//SPELL_AURA_PERIODIC_MANA_FUNNEL = 63,//obselete?
		&Aura::SpellAuraPeriodicManaLeech,//SPELL_AURA_PERIODIC_MANA_LEECH = 64,
		&Aura::SpellAuraModCastingSpeed,//SPELL_AURA_MOD_CASTING_SPEED = 65,
		&Aura::SpellAuraFeignDeath,//SPELL_AURA_FEIGN_DEATH = 66,
		&Aura::SpellAuraModDisarm,//SPELL_AURA_MOD_DISARM = 67,
		&Aura::SpellAuraModStalked,//SPELL_AURA_MOD_STALKED = 68,
		&Aura::SpellAuraSchoolAbsorb,//SPELL_AURA_SCHOOL_ABSORB = 69,
		&Aura::SpellAuraNULL,//SPELL_AURA_EXTRA_ATTACKS = 70,//obselete?
		&Aura::SpellAuraModSpellCritChanceSchool,//SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL = 71,
		&Aura::SpellAuraModPowerCost,//SPELL_AURA_MOD_POWER_COST = 72,
		&Aura::SpellAuraModPowerCostSchool,//SPELL_AURA_MOD_POWER_COST_SCHOOL = 73,
		&Aura::SpellAuraReflectSpellsSchool,//SPELL_AURA_REFLECT_SPELLS_SCHOOL = 74,
		&Aura::SpellAuraModLanguage,//SPELL_AURA_MOD_LANGUAGE = 75,
		&Aura::SpellAuraAddFarSight,//SPELL_AURA_FAR_SIGHT = 76,
		&Aura::SpellAuraMechanicImmunity,//SPELL_AURA_MECHANIC_IMMUNITY = 77,
		&Aura::SpellAuraMounted,//SPELL_AURA_MOUNTED = 78,
		&Aura::SpellAuraModDamagePercDone,//SPELL_AURA_MOD_DAMAGE_PERCENT_DONE = 79,
		&Aura::SpellAuraModPercStat,//SPELL_AURA_MOD_PERCENT_STAT = 80,
		&Aura::SpellAuraSplitDamage,//SPELL_AURA_SPLIT_DAMAGE = 81,
		&Aura::SpellAuraWaterBreathing,//SPELL_AURA_WATER_BREATHING = 82,
		&Aura::SpellAuraModBaseResistance,//SPELL_AURA_MOD_BASE_RESISTANCE = 83,
		&Aura::SpellAuraModRegen,//SPELL_AURA_MOD_REGEN = 84,
		&Aura::SpellAuraModPowerRegen,//SPELL_AURA_MOD_POWER_REGEN = 85,
		&Aura::SpellAuraChannelDeathItem,//SPELL_AURA_CHANNEL_DEATH_ITEM = 86,
		&Aura::SpellAuraModDamagePercTaken,//SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN = 87,
		&Aura::SpellAuraModRegenPercent,//SPELL_AURA_MOD_PERCENT_REGEN = 88,
		&Aura::SpellAuraPeriodicDamagePercent,//SPELL_AURA_PERIODIC_DAMAGE_PERCENT = 89,
		&Aura::SpellAuraModResistChance,//SPELL_AURA_MOD_RESIST_CHANCE = 90,
		&Aura::SpellAuraModDetectRange,//SPELL_AURA_MOD_DETECT_RANGE = 91,
		&Aura::SpellAuraPreventsFleeing,//SPELL_AURA_PREVENTS_FLEEING = 92,
		&Aura::SpellAuraModUnattackable,//SPELL_AURA_MOD_UNATTACKABLE = 93,
		&Aura::SpellAuraInterruptRegen,//SPELL_AURA_INTERRUPT_REGEN = 94,
		&Aura::SpellAuraGhost,//SPELL_AURA_GHOST = 95,
		&Aura::SpellAuraMagnet,//SPELL_AURA_SPELL_MAGNET = 96,
		&Aura::SpellAuraManaShield,//SPELL_AURA_MANA_SHIELD = 97,
		&Aura::SpellAuraSkillTalent,//SPELL_AURA_MOD_SKILL_TALENT = 98,
		&Aura::SpellAuraModAttackPower,//SPELL_AURA_MOD_ATTACK_POWER = 99,
		&Aura::SpellAuraVisible,//SPELL_AURA_AURAS_VISIBLE = 100,
		&Aura::SpellAuraModResistancePCT,//SPELL_AURA_MOD_RESISTANCE_PCT = 101,
		&Aura::SpellAuraModCreatureAttackPower,//SPELL_AURA_MOD_CREATURE_ATTACK_POWER = 102,
		&Aura::SpellAuraModTotalThreat,//SPELL_AURA_MOD_TOTAL_THREAT = 103,
		&Aura::SpellAuraWaterWalk,//SPELL_AURA_WATER_WALK = 104,
		&Aura::SpellAuraFeatherFall,//SPELL_AURA_FEATHER_FALL = 105,
		&Aura::SpellAuraHover,//SPELL_AURA_HOVER = 106,
		&Aura::SpellAuraAddFlatModifier,//SPELL_AURA_ADD_FLAT_MODIFIER = 107,
		&Aura::SpellAuraAddPctMod,//SPELL_AURA_ADD_PCT_MODIFIER = 108,
		&Aura::SpellAuraAddTargetTrigger,//SPELL_AURA_ADD_TARGET_TRIGGER = 109,
		&Aura::SpellAuraModPowerRegPerc,//SPELL_AURA_MOD_POWER_REGEN_PERCENT = 110,
		&Aura::SpellAuraNULL,//SPELL_AURA_ADD_CASTER_HIT_TRIGGER = 111,
		&Aura::SpellAuraOverrideClassScripts,//SPELL_AURA_OVERRIDE_CLASS_SCRIPTS = 112,
		&Aura::SpellAuraModRangedDamageTaken,//SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN = 113,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT = 114,
		&Aura::SpellAuraModHealing,//SPELL_AURA_MOD_HEALING = 115,
		&Aura::SpellAuraIgnoreRegenInterrupt,//SPELL_AURA_IGNORE_REGEN_INTERRUPT = 116,
		&Aura::SpellAuraModMechanicResistance,//SPELL_AURA_MOD_MECHANIC_RESISTANCE = 117,
		&Aura::SpellAuraModHealingPCT,//SPELL_AURA_MOD_HEALING_PCT = 118,
		&Aura::SpellAuraNULL,//SPELL_AURA_SHARE_PET_TRACKING = 119,//obselete
		&Aura::SpellAuraUntrackable,//SPELL_AURA_UNTRACKABLE = 120,
		&Aura::SpellAuraEmphaty,//SPELL_AURA_EMPATHY = 121,
		&Aura::SpellAuraModOffhandDamagePCT,//SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT = 122,
		&Aura::SpellAuraModPenetration,//SPELL_AURA_MOD_POWER_COST_PCT = 123, --> armor penetration & spell penetration, NOT POWER COST!
		&Aura::SpellAuraModRangedAttackPower,//SPELL_AURA_MOD_RANGED_ATTACK_POWER = 124,
		&Aura::SpellAuraModMeleeDamageTaken,//SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN = 125,
		&Aura::SpellAuraModMeleeDamageTakenPct,//SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT = 126,
		&Aura::SpellAuraRAPAttackerBonus,//SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS = 127,
		&Aura::SpellAuraModPossessPet,//SPELL_AURA_MOD_POSSESS_PET = 128,
		&Aura::SpellAuraModIncreaseSpeedAlways,//SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS = 129,
		&Aura::SpellAuraModIncreaseMountedSpeed,//SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS = 130,
		&Aura::SpellAuraModCreatureRangedAttackPower,//SPELL_AURA_MOD_CREATURE_RANGED_ATTACK_POWER = 131,
		&Aura::SpellAuraModIncreaseEnergyPerc,//SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT = 132,
		&Aura::SpellAuraModIncreaseHealthPerc,//SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT = 133,
		&Aura::SpellAuraModManaRegInterrupt,//SPELL_AURA_MOD_MANA_REGEN_INTERRUPT = 134,
		&Aura::SpellAuraModHealingDone,//SPELL_AURA_MOD_HEALING_DONE = 135,
		&Aura::SpellAuraModHealingDonePct,//SPELL_AURA_MOD_HEALING_DONE_PERCENT = 136,
		&Aura::SpellAuraModTotalStatPerc,//SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE = 137,
		&Aura::SpellAuraModHaste,//SPELL_AURA_MOD_HASTE = 138,
		&Aura::SpellAuraForceReaction,//SPELL_AURA_FORCE_REACTION = 139,
		&Aura::SpellAuraModRangedHaste,//SPELL_AURA_MOD_RANGED_HASTE = 140,
		&Aura::SpellAuraModRangedAmmoHaste,//SPELL_AURA_MOD_RANGED_AMMO_HASTE = 141,
		&Aura::SpellAuraModBaseResistancePerc,//SPELL_AURA_MOD_BASE_RESISTANCE_PCT = 142,
		&Aura::SpellAuraModResistanceExclusive,//SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE = 143,
		&Aura::SpellAuraSafeFall,//SPELL_AURA_SAFE_FALL = 144,
		&Aura::SpellAuraModPetTalentPoints,//SPELL_AURA_MOD_PET_TALENT_POINTS = 145,
		&Aura::SpellAuraNULL,//SPELL_AURA_PERSUADED = 146,//obsolete
		&Aura::SpellAuraAddCreatureImmunity,//SPELL_AURA_ADD_CREATURE_IMMUNITY = 147,//http://wow.allakhazam.com/db/spell.html?wspell=36798
		&Aura::SpellAuraRetainComboPoints,//SPELL_AURA_RETAIN_COMBO_POINTS = 148,
		&Aura::SpellAuraResistPushback,//SPELL_AURA_RESIST_PUSHBACK = 149,//	Resist Pushback //Simply resist spell casting delay
		&Aura::SpellAuraModShieldBlockPCT,//SPELL_AURA_MOD_SHIELD_BLOCK_PCT = 150,//	Mod Shield Absorbed dmg %
		&Aura::SpellAuraTrackStealthed,//SPELL_AURA_TRACK_STEALTHED = 151,//	Track Stealthed
		&Aura::SpellAuraModDetectedRange,//SPELL_AURA_MOD_DETECTED_RANGE = 152,//	Mod Detected Range
		&Aura::SpellAuraSplitDamageFlat,//SPELL_AURA_SPLIT_DAMAGE_FLAT= 153,//	Split Damage Flat
		&Aura::SpellAuraModStealthLevel,//SPELL_AURA_MOD_STEALTH_LEVEL = 154,//	Stealth Level Modifier
		&Aura::SpellAuraModUnderwaterBreathing,//SPELL_AURA_MOD_WATER_BREATHING = 155,//	Mod Water Breathing
		&Aura::SpellAuraModReputationAdjust,//SPELL_AURA_MOD_REPUTATION_ADJUST = 156,//	Mod Reputation Gain
		&Aura::SpellAuraNULL,//SPELL_AURA_PET_DAMAGE_MULTI = 157,//	Mod Pet Damage
		&Aura::SpellAuraModBlockValue,//SPELL_AURA_MOD_SHIELD_BLOCKVALUE = 158//used Apply Aura: Mod Shield Block //http://www.thottbot.com/?sp=25036
		&Aura::SpellAuraNoPVPCredit,//missing = 159 //used Apply Aura: No PVP Credit http://www.thottbot.com/?sp=2479
		&Aura::SpellAuraNULL,//missing = 160 //Apply Aura: Mod Side/Rear PBAE Damage Taken %//used http://www.thottbot.com/?sp=23198
		&Aura::SpellAuraModHealthRegInCombat,//SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT = 161,
		&Aura::SpellAuraPowerBurn,//missing = 162 //used //Apply Aura: Power Burn (Mana) //http://www.thottbot.com/?sp=19659
		&Aura::SpellAuraModCritDmgPhysical,//missing = 163 //Apply Aura: Mod Crit Damage Bonus (Physical)
		&Aura::SpellAuraNULL,//missing = 164 //used //test spell
		&Aura::SpellAuraAPAttackerBonus,//SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS = 165,	// Melee AP Attacker Bonus
		&Aura::SpellAuraModPAttackPower,//missing = 166 //used //Apply Aura: Mod Attack Power % // http://www.thottbot.com/?sp=30803
		&Aura::SpellAuraModRangedAttackPowerPct,//missing = 167 //http://www.thottbot.com/s34485
		&Aura::SpellAuraIncreaseDamageTypePCT,//missing = 168 //used //Apply Aura: Increase Damage % *type* //http://www.thottbot.com/?sp=24991
		&Aura::SpellAuraIncreaseCricticalTypePCT,//missing = 169 //used //Apply Aura: Increase Critical % *type* //http://www.thottbot.com/?sp=24293
		&Aura::SpellAuraNULL,//missing = 170 //used //Apply Aura: Detect Amore //http://www.thottbot.com/?sp=26802
		&Aura::SpellAuraIncreasePartySpeed,//missing = 171
		&Aura::SpellAuraIncreaseMovementAndMountedSpeed,//missing = 172 //used //Apply Aura: Increase Movement and Mounted Speed (Non-Stacking) //http://www.thottbot.com/?sp=26022 2e effect
		&Aura::SpellAuraNULL,//missing = 173 // Apply Aura: Allow Champion Spells
		&Aura::SpellAuraIncreaseSpellDamageByAttribute,//missing = 174 //used //Apply Aura: Increase Spell Damage by % Spirit (Spells) //http://www.thottbot.com/?sp=15031
		&Aura::SpellAuraIncreaseHealingByAttribute,//missing = 175 //used //Apply Aura: Increase Spell Healing by % Spirit //http://www.thottbot.com/?sp=15031
		&Aura::SpellAuraSpiritOfRedemption,//missing = 176 //used // Apply Aura: Spirit of Redemption
		&Aura::SpellAuraNULL,//missing = 177 //used //Apply Aura: Area Charm // http://www.thottbot.com/?sp=26740
		&Aura::SpellAuraNULL,//missing = 178 //Apply Aura: Increase Debuff Resistance 
		&Aura::SpellAuraIncreaseAttackerSpellCrit,//SPELL_AURA_INCREASE_ATTACKER_SPELL_CRIT//Apply Aura: Increase Attacker Spell Crit % *type* //http://www.thottbot.com/?sp=12579
		&Aura::SpellAuraNULL,//missing = 180 //used //Apply Aura: Increase Spell Damage *type* //http://www.thottbot.com/?sp=29113
		&Aura::SpellAuraNULL,//missing = 181
		&Aura::SpellAuraIncreaseArmorByPctInt,//missing = 182 //used //Apply Aura: Increase Armor by % of Intellect //http://www.thottbot.com/?sp=28574  SPELL_AURA_INC_ARMOR_BY_PCT_INT
		&Aura::SpellAuraNULL,//missing = 183 //used //Apply Aura: Decrease Critical Threat by % (Spells) //http://www.thottbot.com/?sp=28746
		&Aura::SpellAuraReduceAttackerMHitChance,//184//Apply Aura: Reduces Attacker Chance to Hit with Melee //http://www.thottbot.com/s31678
		&Aura::SpellAuraReduceAttackerRHitChance,//185//Apply Aura: Reduces Attacker Chance to Hit with Ranged //http://www.thottbot.com/?sp=30895
		&Aura::SpellAuraReduceAttackerSHitChance,//186//Apply Aura: Reduces Attacker Chance to Hit with Spells (Spells) //http://www.thottbot.com/?sp=30895
		&Aura::SpellAuraReduceEnemyMCritChance,//missing = 187 //used //Apply Aura: Reduces Attacker Chance to Crit with Melee (Ranged?) //http://www.thottbot.com/?sp=30893
		&Aura::SpellAuraReduceEnemyRCritChance,//missing = 188 //used //Apply Aura: Reduces Attacker Chance to Crit with Ranged (Melee?) //http://www.thottbot.com/?sp=30893
		&Aura::SpellAuraIncreaseRating,//missing = 189 //Apply Aura: Increases Rating
		&Aura::SpellAuraIncreaseRepGainPct,//SPELL_AURA_MOD_FACTION_REPUTATION_GAIN //used // Apply Aura: Increases Reputation Gained by % //http://www.thottbot.com/?sp=30754
		&Aura::SpellAuraUseNormalMovementSpeed,//missing = 191 //used // noname //http://www.thottbot.com/?sp=29894
		&Aura::SpellAuraNULL,//192 Apply Aura: Melee Slow %
		&Aura::SpellAuraIncreaseTimeBetweenAttacksPCT,//193 Apply Aura: Increase Time Between Attacks (Melee, Ranged and Spell) by %
		&Aura::SpellAuraNULL,//194 //&Aura::SpellAuraIncreaseSpellDamageByInt,//194 Apply Aura: Increase Spell Damage by % of Intellect (All)
		&Aura::SpellAuraNULL,//195 //&Aura::SpellAuraIncreaseHealingByInt,//195 Apply Aura: Increase Healing by % of Intellect
		&Aura::SpellAuraNULL,//196 Apply Aura: Mod All Weapon Skills (6)
		&Aura::SpellAuraModAttackerCritChance,//197 Apply Aura: Reduce Attacker Critical Hit Chance by %
		&Aura::SpellAuraIncreaseAllWeaponSkill,//198
		&Aura::SpellAuraIncreaseHitRate,//199 Apply Aura: Increases Spell % To Hit (Fire, Nature, Frost)
		&Aura::SpellAuraNULL,//200 // Increases experience earned by $s1%.  Lasts $d.
		&Aura::SpellAuraNULL,//201 Apply Aura: Cannot be Dodged
		&Aura::SpellAuraFinishingMovesCannotBeDodged,//202 // Finishing moves cannot be dodged - 32601, 44452
		&Aura::SpellAuraReduceCritMeleeAttackDmg,//203 Apply Aura: Reduces Attacker Critical Hit Damage with Melee by %
		&Aura::SpellAuraReduceCritRangedAttackDmg,//204 Apply Aura: Reduces Attacker Critical Hit Damage with Ranged by %
		&Aura::SpellAuraNULL,//205 // "School" Vulnerability
		&Aura::SpellAuraEnableFlight,//206 // Take flight on a worn old carpet. - Spell 43343
		&Aura::SpellAuraEnableFlight,//207 set fly
		&Aura::SpellAuraEnableFlightWithUnmountedSpeed,//208
		&Aura::SpellAuraNULL,//209  // mod flight speed?
		&Aura::SpellAuraNULL,//210	// commentator's command - spell 42009
		&Aura::SpellAuraIncreaseFlightSpeed,//211
		&Aura::SpellAuraIncreaseRangedAPStatPCT,//SPELL_AURA_MOD_RANGED_ATTACK_POWER_OF_INTELLECT //212 Apply Aura: Increase Ranged Atk Power by % of Intellect
		&Aura::SpellAuraIncreaseRageFromDamageDealtPCT, //213 Apply Aura: Increase Rage from Damage Dealt by %
		&Aura::SpellAuraNULL,//214 // Tamed Pet Passive (DND)
		&Aura::SpellAuraNULL,//215 // arena preparation buff - cancel soul shard requirement?
		&Aura::SpellAuraModCastingSpeed,//216 Increases casting time %, reuse existing handler...
		&Aura::SpellAuraNULL,//217 // not used
		&Aura::SpellAuraHasteRanged,//218 // increases time between ranged attacks
		&Aura::SpellAuraRegenManaStatPCT,//219 Regenerate mana equal to $s1% of your Intellect every 5 sec, even while casting
		&Aura::SpellAuraSpellHealingStatPCT,//220 Increases your healing spells  by up to $s1% of your Strength
		&Aura::SpellAuraNULL,//221 Detaunt "Ignores an enemy, forcing the caster to not attack it unless there is no other target nearby. When the effect wears off, the creature will attack the most threatening target."
		&Aura::SpellAuraNULL,//222 // not used
		&Aura::SpellAuraNULL,//223 // used in one spell, cold stare 43593
		&Aura::SpellAuraNULL,//224 // not used
		&Aura::SpellAuraNULL,//225 // Prayer of Mending "Places a spell on the target that heals them for $s1 the next time they take damage.  When the heal occurs, Prayer of Mending jumps to a raid member within $a1 yards.  Jumps up to $n times and lasts $d after each jump.  This spell can only be placed on one target at a time."
		&Aura::SpellAuraDrinkNew,//226 // used in brewfest spells, headless hoerseman
		&Aura::SpellAuraNULL,//227 Inflicts [SPELL DAMAGE] damage to enemies in a cone in front of the caster. (based on combat range) http://www.thottbot.com/s40938
		&Aura::SpellAuraAuraModInvisibilityDetection,//228 Stealth Detection. http://www.thottbot.com/s34709
		&Aura::SpellAuraReduceAOEDamageTaken,//229 Apply Aura:Reduces the damage your pet takes from area of effect attacks http://www.thottbot.com/s35694
		&Aura::SpellAuraIncreaseMaxHealth,//230 Increase Max Health (commanding shout);
        &Aura::SpellAuraNULL,//231 curse a target http://www.thottbot.com/s40303
        &Aura::SpellAuraReduceEffectDuration,//232 // Movement Slowing Effect Duration // Reduces duration of Magic effects by $s2%.
        &Aura::SpellAuraNULL,//233 // Beer Goggles
        &Aura::SpellAuraReduceEffectDuration,//234 // modifies the duration of all (miscValue mechanic) effects used against you by % http://www.wowhead.com/?spell=16254
		&Aura::SpellAuraNULL,//235 33206 Instantly reduces a friendly target's threat by $44416s1%, reduces all damage taken by $s1% and increases resistance to Dispel mechanics by $s2% for $d.
		&Aura::SpellAuraVehiclePassenger,//236
		&Aura::SpellAuraModSpellDamageFromAP,//237 Mod Spell Damage from Attack Power
		&Aura::SpellAuraModSpellHealingFromAP,//238 Mod Healing from Attack Power
		&Aura::SpellAuraNULL,//239
		&Aura::SpellAuraExpertise,//240 Expertise
		&Aura::SpellAuraNULL,//241
		&Aura::SpellAuraNULL,//242
		&Aura::SpellAuraNULL,//243
		&Aura::SpellAuraNULL,//244
		&Aura::SpellAuraNULL,//245
		&Aura::SpellAuraNULL,//246
		&Aura::SpellAuraNULL,//247
		&Aura::SpellAuraNULL,//248
		&Aura::SpellAuraNULL,//249
		&Aura::SpellAuraModIncreaseHealth,//250 Add Health http://www.wowhead.com/?spell=44055
		&Aura::SpellAuraNULL,//251
		&Aura::SpellAuraNULL,//252
		&Aura::SpellAuraNULL,//253
		&Aura::SpellAuraNULL,//254
		&Aura::SpellAuraModDamageTakenByMechPCT,//255
		&Aura::SpellAuraNoReagent,//256
		&Aura::SpellAuraNULL,//257
		&Aura::SpellAuraNULL,//258
		&Aura::SpellAuraNULL,//259
		&Aura::SpellAuraNULL,//260
		&Aura::SpellAuraSetPhase,//261
		&Aura::SpellAuraSkipCanCastCheck,//262 Allows you to do spells wich doesen't work in different circumstances
		&Aura::SpellAuraCastFilter,//263
		&Aura::SpellAuraNULL,//264
		&Aura::SpellAuraNULL,//265
		&Aura::SpellAuraNULL,//266
		&Aura::SpellAuraNULL,//267
		&Aura::SpellAuraIncreaseAPByAttribute,//268
		&Aura::SpellAuraNULL,//269
		&Aura::SpellAuraNULL,//270
		&Aura::SpellAuraModDamageTakenPctPerCaster,//271
		&Aura::SpellAuraNULL,//272
		&Aura::SpellAuraNULL,//273
		&Aura::SpellAuraNULL,//274
		&Aura::SpellAuraNULL,//275
		&Aura::SpellAuraNULL,//276
		&Aura::SpellAuraRedirectThreat,//277
		&Aura::SpellAuraNULL,//278
		&Aura::SpellAuraNULL,//279
		&Aura::SpellAuraModIgnoreArmorPct,//280
		&Aura::SpellAuraNULL,//281
		&Aura::SpellAuraModBaseHealth,//282
		&Aura::SpellAuraNULL,//283
		&Aura::SpellAuraNULL,//284
		&Aura::SpellAuraNULL,//285
		&Aura::SpellAuraNULL,//286
		&Aura::SpellAuraReflectInfront, //287
};

const char* SpellAuraNames[TOTAL_SPELL_AURAS] = {
    "NONE",												//   0 None
    "BIND_SIGHT",										//   1 Bind Sight
    "MOD_POSSESS",										//   2 Mod Possess
    "PERIODIC_DAMAGE",									//   3 Periodic Damage
    "DUMMY",											//   4 Script Aura
    "MOD_CONFUSE",										//   5 Mod Confuse
    "MOD_CHARM",										//   6 Mod Charm
    "MOD_FEAR",											//   7 Mod Fear
    "PERIODIC_HEAL",									//   8 Periodic Heal
    "MOD_ATTACKSPEED",									//   9 Mod Attack Speed
    "MOD_THREAT",										//  10 Mod Threat
    "MOD_TAUNT",										//  11 Taunt
    "MOD_STUN",											//  12 Stun
    "MOD_DAMAGE_DONE",									//  13 Mod Damage Done
    "MOD_DAMAGE_TAKEN",									//  14 Mod Damage Taken
    "DAMAGE_SHIELD",									//  15 Damage Shield
    "MOD_STEALTH",										//  16 Mod Stealth
    "MOD_DETECT",										//  17 Mod Detect
    "MOD_INVISIBILITY",									//  18 Mod Invisibility
    "MOD_INVISIBILITY_DETECTION",						//  19 Mod Invisibility Detection
    "MOD_TOTAL_HEALTH_REGEN_PCT",						//  20
    "MOD_TOTAL_MANA_REGEN_PCT",							//  21
    "MOD_RESISTANCE",									//  22 Mod Resistance
    "PERIODIC_TRIGGER_SPELL",							//  23 Periodic Trigger
    "PERIODIC_ENERGIZE",								//  24 Periodic Energize
    "MOD_PACIFY",										//  25 Pacify
    "MOD_ROOT",											//  26 Root
    "MOD_SILENCE",										//  27 Silence
    "REFLECT_SPELLS",									//  28 Reflect Spells %
    "MOD_STAT",											//  29 Mod Stat
    "MOD_SKILL",										//  30 Mod Skill
    "MOD_INCREASE_SPEED",								//  31 Mod Speed
    "MOD_INCREASE_MOUNTED_SPEED",						//  32 Mod Speed Mounted
    "MOD_DECREASE_SPEED",								//  33 Mod Speed Slow
    "MOD_INCREASE_HEALTH",								//  34 Mod Increase Health
    "MOD_INCREASE_ENERGY",								//  35 Mod Increase Energy
    "MOD_SHAPESHIFT",									//  36 Shapeshift
    "EFFECT_IMMUNITY",									//  37 Immune Effect
    "STATE_IMMUNITY",									//  38 Immune State
    "SCHOOL_IMMUNITY",									//  39 Immune School    
    "DAMAGE_IMMUNITY",									//  40 Immune Damage
    "DISPEL_IMMUNITY",									//  41 Immune Dispel Type
    "PROC_TRIGGER_SPELL",								//  42 Proc Trigger Spell
    "PROC_TRIGGER_DAMAGE",								//  43 Proc Trigger Damage
    "TRACK_CREATURES",									//  44 Track Creatures
    "TRACK_RESOURCES",									//  45 Track Resources
    "MOD_PARRY_SKILL",									//  46 Mod Parry Skill
    "MOD_PARRY_PERCENT",								//  47 Mod Parry Percent
    "MOD_DODGE_SKILL",									//  48 Mod Dodge Skill
    "MOD_DODGE_PERCENT",								//  49 Mod Dodge Percent  
    "MOD_BLOCK_SKILL",									//  50 Mod Block Skill
    "MOD_BLOCK_PERCENT",								//  51 Mod Block Percent
    "MOD_CRIT_PERCENT",									//  52 Mod Crit Percent
    "PERIODIC_LEECH",									//  53 Periodic Leech
    "MOD_HIT_CHANCE",									//  54 Mod Hit Chance
    "MOD_SPELL_HIT_CHANCE",								//  55 Mod Spell Hit Chance
    "TRANSFORM",										//  56 Transform
    "MOD_SPELL_CRIT_CHANCE",							//  57 Mod Spell Crit Chance
    "MOD_INCREASE_SWIM_SPEED",							//  58 Mod Speed Swim
    "MOD_DAMAGE_DONE_CREATURE",							//  59 Mod Creature Dmg Done   
    "MOD_PACIFY_SILENCE",								//  60 Pacify & Silence
    "MOD_SCALE",										//  61 Mod Scale
    "PERIODIC_HEALTH_FUNNEL",							//  62 Periodic Health Funnel
    "PERIODIC_MANA_FUNNEL",								//  63 Periodic Mana Funnel
    "PERIODIC_MANA_LEECH",								//  64 Periodic Mana Leech
    "MOD_CASTING_SPEED",								//  65 Haste - Spells
    "FEIGN_DEATH",										//  66 Feign Death
    "MOD_DISARM",										//  67 Disarm
    "MOD_STALKED",										//  68 Mod Stalked
    "SCHOOL_ABSORB",									//  69 School Absorb    
    "EXTRA_ATTACKS",									//  70 Extra Attacks
    "MOD_SPELL_CRIT_CHANCE_SCHOOL",						//  71 Mod School Spell Crit Chance
    "MOD_POWER_COST",									//  72 Mod Power Cost
    "MOD_POWER_COST_SCHOOL",							//  73 Mod School Power Cost
    "REFLECT_SPELLS_SCHOOL",							//  74 Reflect School Spells %
    "MOD_LANGUAGE",										//  75 Mod Language
    "FAR_SIGHT",										//  76 Far Sight
    "MECHANIC_IMMUNITY",								//  77 Immune Mechanic
    "MOUNTED",											//  78 Mounted
    "MOD_DAMAGE_PERCENT_DONE",							//  79 Mod Dmg %   
    "MOD_PERCENT_STAT",									//  80 Mod Stat %
    "SPLIT_DAMAGE",										//  81 Split Damage
    "WATER_BREATHING",									//  82 Water Breathing
    "MOD_BASE_RESISTANCE",								//  83 Mod Base Resistance
    "MOD_REGEN",										//  84 Mod Health Regen
    "MOD_POWER_REGEN",									//  85 Mod Power Regen
    "CHANNEL_DEATH_ITEM",								//  86 Create Death Item
    "MOD_DAMAGE_PERCENT_TAKEN",							//  87 Mod Dmg % Taken
    "MOD_PERCENT_REGEN",								//  88 Mod Health Regen Percent
    "PERIODIC_DAMAGE_PERCENT",							//  89 Periodic Damage Percent   
    "MOD_RESIST_CHANCE",								//  90 Mod Resist Chance
    "MOD_DETECT_RANGE",									//  91 Mod Detect Range
    "PREVENTS_FLEEING",									//  92 Prevent Fleeing
    "MOD_UNATTACKABLE",									//  93 Mod Uninteractible
    "INTERRUPT_REGEN",									//  94 Interrupt Regen
    "GHOST",											//  95 Ghost
    "SPELL_MAGNET",										//  96 Spell Magnet
    "MANA_SHIELD",										//  97 Mana Shield
    "MOD_SKILL_TALENT",									//  98 Mod Skill Talent
    "MOD_ATTACK_POWER",									//  99 Mod Attack Power
    "AURAS_VISIBLE",									// 100 Auras Visible
    "MOD_RESISTANCE_PCT",								// 101 Mod Resistance %
    "MOD_CREATURE_ATTACK_POWER",						// 102 Mod Creature Attack Power
    "MOD_TOTAL_THREAT",									// 103 Mod Total Threat (Fade)
    "WATER_WALK",										// 104 Water Walk
    "FEATHER_FALL",										// 105 Feather Fall
    "HOVER",											// 106 Hover
    "ADD_FLAT_MODIFIER",								// 107 Add Flat Modifier
    "ADD_PCT_MODIFIER",									// 108 Add % Modifier
    "ADD_TARGET_TRIGGER",								// 109 Add Class Target Trigger    
    "MOD_POWER_REGEN_PERCENT",							// 110 Mod Power Regen %
    "ADD_CASTER_HIT_TRIGGER",							// 111 Add Class Caster Hit Trigger
    "OVERRIDE_CLASS_SCRIPTS",							// 112 Override Class Scripts
    "MOD_RANGED_DAMAGE_TAKEN",							// 113 Mod Ranged Dmg Taken
    "MOD_RANGED_DAMAGE_TAKEN_PCT",						// 114 Mod Ranged % Dmg Taken
    "MOD_HEALING",										// 115 Mod Healing
    "IGNORE_REGEN_INTERRUPT",							// 116 Regen During Combat
    "MOD_MECHANIC_RESISTANCE",							// 117 Mod Mechanic Resistance
    "MOD_HEALING_PCT",									// 118 Mod Healing %
    "SHARE_PET_TRACKING",								// 119 Share Pet Tracking    
    "UNTRACKABLE",										// 120 Untrackable
    "EMPATHY",											// 121 Empathy (Lore, whatever)
    "MOD_OFFHAND_DAMAGE_PCT",							// 122 Mod Offhand Dmg %
    "MOD_POWER_COST_PCT",								// 123 Mod Power Cost % --> armor penetration & spell penetration
    "MOD_RANGED_ATTACK_POWER",							// 124 Mod Ranged Attack Power
    "MOD_MELEE_DAMAGE_TAKEN",							// 125 Mod Melee Dmg Taken
    "MOD_MELEE_DAMAGE_TAKEN_PCT",						// 126 Mod Melee % Dmg Taken
    "RANGED_ATTACK_POWER_ATTACKER_BONUS",				// 127 Rngd Atk Pwr Attckr Bonus
    "MOD_POSSESS_PET",									// 128 Mod Possess Pet
    "MOD_INCREASE_SPEED_ALWAYS",						// 129 Mod Speed Always   
    "MOD_MOUNTED_SPEED_ALWAYS",							// 130 Mod Mounted Speed Always
    "MOD_CREATURE_RANGED_ATTACK_POWER",					// 131 Mod Creature Ranged Attack Power
    "MOD_INCREASE_ENERGY_PERCENT",						// 132 Mod Increase Energy %
    "MOD_INCREASE_HEALTH_PERCENT",						// 133 Mod Max Health %
    "MOD_MANA_REGEN_INTERRUPT",							// 134 Mod Interrupted Mana Regen
    "MOD_HEALING_DONE",									// 135 Mod Healing Done
    "MOD_HEALING_DONE_PERCENT",							// 136 Mod Healing Done %
    "MOD_TOTAL_STAT_PERCENTAGE",						// 137 Mod Total Stat %
    "MOD_HASTE",										// 138 Haste - Melee
    "FORCE_REACTION",									// 139 Force Reaction    
    "MOD_RANGED_HASTE",									// 140 Haste - Ranged
    "MOD_RANGED_AMMO_HASTE",							// 141 Haste - Ranged (Ammo Only)
    "MOD_BASE_RESISTANCE_PCT",							// 142 Mod Base Resistance %
    "MOD_RESISTANCE_EXCLUSIVE",							// 143 Mod Resistance Exclusive
    "SAFE_FALL",										// 144 Safe Fall
	"MOD_PET_TALENT_POINTS",							// 145 Mod Pet Talent Points
    "PERSUADED",										// 146 Persuaded
    "ADD_CREATURE_IMMUNITY",							// 147 Add Creature Immunity
    "RETAIN_COMBO_POINTS",								// 148 Retain Combo Points
    "RESIST_PUSHBACK",									// 149 Resist Pushback
    "MOD_SHIELD_BLOCK_PCT",								// 150 Mod Shield Block %
    "TRACK_STEALTHED",									// 151 Track Stealthed
    "MOD_DETECTED_RANGE",								// 152 Mod Detected Range
    "SPLIT_DAMAGE_FLAT",								// 153 Split Damage Flat
    "MOD_STEALTH_LEVEL",								// 154 Stealth Level Modifier
    "MOD_WATER_BREATHING",								// 155 Mod Water Breathing
    "MOD_REPUTATION_ADJUST",							// 156 Mod Reputation Gain
    "PET_DAMAGE_MULTI",									// 157 Mod Pet Damage
    "MOD_SHIELD_BLOCK",									// 158 Mod Shield Block
    "NO_PVP_CREDIT",									// 159 No PVP Credit 
    "MOD_SIDE_REAR_PDAE_DAMAGE_TAKEN",					// 160 Mod Side/Rear PBAE Damage Taken 
    "MOD_HEALTH_REGEN_IN_COMBAT",						// 161 Mod Health Regen In Combat
    "POWER_BURN",										// 162 Power Burn 
    "MOD_CRIT_DAMAGE_BONUS_MELEE",						// 163 Mod Critical Damage Bonus (Physical)
	"",													// 164
    "MELEE_ATTACK_POWER_ATTACKER_BONUS",				// 165 Melee AP Attacker Bonus
    "MOD_ATTACK_POWER_PCT",								// 166 Mod Attack Power
    "MOD_RANGED_ATTACK_POWER_PCT",						// 167 Mod Ranged Attack Power %
    "INCREASE_DAMAGE",									// 168 Increase Damage Type
    "INCREASE_CRITICAL",								// 169 Increase Critical Type
    "DETECT_AMORE",										// 170 Detect Amore
	"",													// 171
    "INCREASE_MOVEMENT_AND_MOUNTED_SPEED",				// 172 Increase Movement and Mounted Speed (Non-Stacking)
	"",													// 173
    "INCREASE_SPELL_DAMAGE_PCT",						// 174 Increase Spell Damage by % status
    "INCREASE_SPELL_HEALING_PCT",						// 175 Increase Spell Healing by % status
    "SPIRIT_OF_REDEMPTION",								// 176 Spirit of Redemption Auras
    "AREA_CHARM",										// 177 Area Charm 
	"MOD_DEBUFF_RESISTANCE_PERCENT",					// 178 Mod Debuff Resistance %
    "INCREASE_ATTACKER_SPELL_CRIT",						// 179 Increase Attacker Spell Crit Type
    "INCREASE_SPELL_DAMAGE_VS_TYPE",					// 180 Increase Spell Damage Type
	"",													// 181
    "INCREASE_ARMOR_BASED_ON_INTELLECT_PCT",			// 182 Increase Armor based on Intellect
    "DECREASE_CRIT_THREAT",								// 183 Decrease Critical Threat by
    "DECREASE_ATTACKER_CHANCE_TO_HIT_MELEE",			// 184 Reduces Attacker Chance to Hit with Melee
    "DECREASE_ATTACKER_CHANGE_TO_HIT_RANGED",			// 185 Reduces Attacker Chance to Hit with Ranged 
    "DECREASE_ATTACKER_CHANGE_TO_HIT_SPELLS",			// 186 Reduces Attacker Chance to Hit with Spells
    "DECREASE_ATTACKER_CHANGE_TO_CRIT_MELEE",			// 187 Reduces Attacker Chance to Crit with Melee (Ranged?)
    "DECREASE_ATTACKER_CHANGE_TO_CRIT_RANGED",			// 188 Reduces Attacker Chance to Crit with Ranged (Melee?)
	"",													// 189
    "INCREASE_REPUTATION",								// 190 Increases reputation from killed creatures
    "SPEED_LIMIT",										// 191 speed limit
    "MELEE_SLOW_PCT",									// 192
    "INCREASE_TIME_BETWEEN_ATTACKS",					// 193
    "INREASE_SPELL_DAMAGE_PCT_OF_INTELLECT",			// 194 NOT USED ANYMORE - 174 used instead
    "INCREASE_HEALING_PCT_OF_INTELLECT",				// 195 NOT USED ANYMORE - 175 used instead
    "MOD_ALL_WEAPON_SKILLS",							// 196
    "REDUCE_ATTACKER_CRICTICAL_HIT_CHANCE_PCT",			// 197
	"",													// 198
    "INCREASE_SPELL_HIT_PCT",							// 199
	"",													// 200
    "CANNOT_BE_DODGED",									// 201
	"FINISHING_MOVES_CANNOT_BE_DODGED",					// 202
    "REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_MELEE_PCT",	// 203
    "REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_RANGED_PCT",	// 204
	"",													// 205
	"",													// 206
	"",													// 207
	"",													// 208
	"",													// 209
	"",													// 210
	"",													// 211
    "INCREASE_RANGED_ATTACK_POWER_PCT_OF_INTELLECT",	// 212
    "INCREASE_RAGE_FROM_DAMAGE_DEALT_PCT",				// 213
	"",													// 214
	"",													// 215
    "INCREASE_CASTING_TIME_PCT",						// 216
	"",													// 217
	"",													// 218
    "REGEN_MANA_STAT_PCT",								// 219
    "HEALING_STAT_PCT",									// 220
	"",													// 221
	"",													// 222
	"",													// 223
	"",													// 224
	"",													// 225
	"",													// 226
	"",													// 227
	"",													// 228
	"",													// 229
    "INCREASE_MAX_HEALTH",								// 230 Used by Commanding Shout
	"",													// 231
	"",													// 232
	"",													// 233
	"",													// 234
	"",													// 235
	"",													// 236
	"",													// 237
	"",													// 238
	"",													// 239
	"MODIFY_AXE_SKILL",									// 240
	"",													// 241
	"",													// 242
	"",													// 243
	"",													// 244
	"",													// 245
	"",													// 246
	"",													// 247
	"",													// 248
	"",													// 249
	"",													// 250
	"",													// 251
	"",													// 252
	"",													// 253
	"",													// 254
	"",													// 255
	"",													// 256
	"",													// 257
	"",													// 258
	"",													// 259
	"",													// 260
	"",													// 261
	"",													// 262
	"",													// 263
	"",													// 264
	"",													// 265
	"",													// 266
	"",													// 267
	"",													// 268
	"",													// 269
	"",													// 270
	"MOD_DAMAGE_TAKEN_PCT_PER_CASTER",					// 271
};

Unit* Aura::GetUnitCaster()
{
	if( m_target == NULL && m_casterGuid && GET_TYPE_FROM_GUID(m_casterGuid) == HIGHGUID_TYPE_PLAYER)
	{
		Unit* punit = NULL;
		punit = objmgr.GetPlayer( uint32(m_casterGuid));
		if(punit != NULL)
			return punit;
	}
	if( m_target == NULL )
		return NULL;

	if( m_casterGuid && m_casterGuid == m_target->GetGUID() )
		return m_target;
	if( m_target->GetMapMgr() != NULL )
		return m_target->GetMapMgr()->GetUnit( m_casterGuid );
	else
		return NULL;
}

Aura::Aura( SpellEntry* proto, int32 duration, Object* caster, Unit* target )
{
	m_applied = false;
	m_dispelled = false;
	m_resistPctChance = 0;
	m_castInDuel = false;
	m_spellProto = proto;
	m_duration = duration;
	m_positive = 0; //we suppose spell will have positive impact on target
	stackSize = 1;
	procCharges = GetSpellProto()->procCharges;
	m_deleted = false;
	m_added = false; // so that we don't try to Remove() it until it was actually added by AddAura
	m_tmpAuradeleted = false;
	m_creatureAA = false;

	m_casterGuid = caster->GetGUID();
	m_target = target;

	DurationPctMod(GetMechanic());
	/*if( caster->GetTypeId() == TYPEID_PLAYER && target->GetTypeId() == TYPEID_PLAYER )
	{
		if( ( ( Player* )caster )->DuelingWith == ( ( Player* )target ) )
		{
			m_castInDuel = true;
		}
	}*/

	//SetCasterFaction(caster->_getFaction());

	//m_auraSlot = 0;
	m_modcount = 0;
	m_dynamicValue = 0;
	m_areaAura = false;

	if( m_spellProto->c_is_flags & SPELL_FLAG_IS_FORCEDDEBUFF )
		SetNegative( 100 );
	else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_FORCEDBUFF )
		SetPositive( 100 );
	if( caster->IsUnit() )
	{
		if( m_spellProto->buffIndexType > 0 && caster->IsPlayer() )
		{
			TO_PLAYER(caster )->RemoveSpellTargets( m_spellProto->buffIndexType);
			TO_PLAYER(caster )->SetSpellTargetType( m_spellProto->buffIndexType, target);
		}
		if(m_spellProto->SpellGroupType)
		{
			SM_FIValue(TO_UNIT(caster)->SM[SMT_CHARGES][0], (int32*)&procCharges, m_spellProto->SpellGroupType);
			SM_PIValue(TO_UNIT(caster)->SM[SMT_CHARGES][1], (int32*)&procCharges, m_spellProto->SpellGroupType);
		}

		if( isAttackable( TO_UNIT(caster), target, false ) )
		{
			SetNegative();
			/*if(caster->IsPlayer())
			{
				TO_PLAYER( caster )->CombatModeDelay = COMBAT_DECAY_TIME;
			}
			else if(caster->IsPet())
			{
				Player* p = TO_PET( caster )->GetPetOwner();
				if(p)
					p->CombatModeDelay = COMBAT_DECAY_TIME;
			}*/

			if( target->InStealth() && target != caster )
				target->RemoveStealth();
		}
		else 
			SetPositive();

		if( m_target->IsPlayer() && caster->IsPlayer() )
		{
			if( TO_PLAYER(m_target)->DuelingWith == TO_PLAYER(caster) )
				m_castInDuel = true;
		}
	}

	if( !IsPassive() )
	{
		timeleft = ( uint32 )UNIXTIME;
	}

	m_castedItemId = 0;
	pSpellId = 0;
	periodic_target = 0;
	DEBUG_LOG("Aura","Constructor %u (%s) from %u.", m_spellProto->Id, m_spellProto->Name, m_target->GetLowGUID());
	m_auraSlot = 0xffffffff;
	m_interrupted = -1;
	//fixed_amount = 0;//used only por percent values to be able to recover value correctly.No need to init this if we are not using it
}

Aura::~Aura()
{}

void Aura::Remove()
{
	if( m_deleted )
		return;

	m_deleted = true;

	sEventMgr.RemoveEvents( this );

	if (!m_tmpAuradeleted && m_target->tmpAura.find(m_spellProto->Id) != m_target->tmpAura.end())
	{
		if (m_target->tmpAura[m_spellProto->Id] != this)
		{
			m_target->tmpAura[m_spellProto->Id]->m_tmpAuradeleted = true;
			m_target->tmpAura[m_spellProto->Id]->Remove();
		}
		m_target->tmpAura.erase(m_spellProto->Id);
		m_tmpAuradeleted = true;
	}
	stackSize = 0;

	if(!m_added)
		return;

	if( !IsPassive() || m_spellProto->AttributesEx & 1024 )
		BuildAuraUpdate();

	if( m_auraSlot < MAX_AURAS+MAX_PASSIVE_AURAS && m_target->m_auras[m_auraSlot] == this )
		m_target->m_auras[m_auraSlot] = NULL;

	ApplyModifiers( false );

	// reset diminishing return timer if needed
	::UnapplyDiminishingReturnTimer( m_target, m_spellProto );

	Unit * m_caster = GetUnitCaster();
	if (m_caster != NULL)
		m_caster->OnAuraRemove(GetSpellProto()->NameHash, m_target);

	for( uint32 x = 0; x < 3; x++ )
	{
		if( !m_spellProto->Effect[x] )
			continue;

		if( m_spellProto->Effect[x] == SPELL_EFFECT_TRIGGER_SPELL && !m_spellProto->always_apply )
		{
			//if(GetSpellProto()->EffectTriggerSpell[x]!=GetSpellId())
			m_target->RemoveAura(GetSpellProto()->EffectTriggerSpell[x]);
		}
		else if( (m_spellProto->Effect[x] == SPELL_EFFECT_APPLY_AREA_AURA || m_spellProto->Effect[x] == SPELL_EFFECT_APPLY_AURA_128) && m_casterGuid == m_target->GetGUID())
		{
			RemoveAA();
		}
	}

	int32 procCharges = m_spellProto->procCharges;
	if( m_caster != NULL && m_spellProto->SpellGroupType )
	{
		SM_FIValue(m_caster->SM[SMT_CHARGES][0],&procCharges, m_spellProto->SpellGroupType);
		SM_PIValue(m_caster->SM[SMT_CHARGES][1],&procCharges, m_spellProto->SpellGroupType);
	}

	if( procCharges > 0 && !(GetSpellProto()->procFlags & PROC_REMOVEONUSE) )
	{
		if( m_target->m_chargeSpellsInUse )
		{
			m_target->m_chargeSpellRemoveQueue.push_back( this );
		}
		else
		{
			std::list<Aura*>::iterator iter = 
				std::find(m_target->m_chargeSpells.begin(), m_target->m_chargeSpells.end(), this);
			if( iter != m_target->m_chargeSpells.end() )
			{
				m_target->m_chargeSpells.erase(iter);
			}
		}
	}

	// remove attacker
	if( m_caster != NULL)
	{
		if( m_caster != m_target )
		{
			// try to remove
			m_caster->CombatStatus.RemoveAttackTarget(m_target);
			if( m_caster->isDead() )
				m_target->CombatStatus.ForceRemoveAttacker( m_caster->GetGUID() );
		}

		if( m_spellProto->buffIndexType != 0 && m_target->IsPlayer() )
			TO_PLAYER(m_target )->RemoveSpellIndexReferences( m_spellProto->buffIndexType );
	}
	else
		m_target->CombatStatus.ForceRemoveAttacker( m_casterGuid );


	if( m_caster != NULL && m_caster->IsPlayer() && m_caster->IsInWorld() )
	{
		sHookInterface.OnAuraRemove(TO_PLAYER(m_caster),m_spellProto->Id);

		/**********************Cooldown**************************
		* this is only needed for some spells
		* for now only spells that have:
		* (m_spellInfo->Attributes == 0x2050000) && !(m_spellInfo->AttributesEx) ||
		* m_spellProto->Attributes == 0x2040100
		* are handled. Its possible there are more spells like this
		*************************************************************/
		if( m_spellProto->c_is_flags & SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE )
		{
			/*WorldPacket data( 12 );
			data.SetOpcode( SMSG_COOLDOWN_EVENT );
			data << m_spellProto->Id << caster->GetGUID();
			m_caster->SendMessageToSet( &data, true );*/

			packetSMSG_COOLDOWN_EVENT data;
			data.spellid = m_spellProto->Id;
			data.guid = m_caster->GetGUID();
			m_caster->OutPacketToSet( SMSG_COOLDOWN_EVENT, sizeof( packetSMSG_COOLDOWN_EVENT ), &data, true );
		}
	}

	if( m_spellProto->MechanicsType == MECHANIC_ENRAGED )
		m_target->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_ENRAGE );
	else if( m_spellProto->AdditionalAura )
		m_target->RemoveAura( m_spellProto->AdditionalAura );
	else if( m_spellProto->Id == 642 )
	{
		m_target->RemoveAura( 53523 );
		m_target->RemoveAura( 53524 );
	}

	uint32 flag = 0;
	if( m_spellProto->buffType & SPELL_TYPE_SEAL )
        flag |= AURASTATE_FLAG_JUDGEMENT;
	   m_target->RemoveFlag( UNIT_FIELD_AURASTATE, flag );
	   
	   delete this;
}

void Aura::AddMod( uint32 t, int32 a, uint32 miscValue, uint32 i )
{
	//this is fix, when u have the same unit in target list several times
	//for(uint32 x=0;x<m_modcount;x++)
	//	if(m_modList[x].i==i)return;

	/*if(m_modList[0].m_type == t ||
		m_modList[1].m_type == t ||
		m_modList[2].m_type == t)
		return; // dont duplicate mods // some spells apply duplicate mods, like some seals*/ 

	if( m_modcount >= 3 || m_target == NULL || m_target->MechanicsDispels[GetMechanicOfEffect(i)])
	{
		//sLog.outString("Tried to add >3 (%u) mods to spellid %u [%u:%u, %u:%u, %u:%u]", m_modcount+1, this->m_spellProto->Id, m_modList[0].m_type, m_modList[0].m_amount, m_modList[1].m_type, m_modList[1].m_amount, m_modList[2].m_type, m_modList[2].m_amount);
		return;
	}

	m_modList[m_modcount].m_type = t;
	m_modList[m_modcount].m_amount = a * stackSize;
	m_modList[m_modcount].m_baseAmount = a;
	m_modList[m_modcount].m_miscValue = miscValue;
	m_modList[m_modcount].i = i;
	m_modcount++;
	//ASSERT(m_modcount<=3);
}

void Aura::RemoveIfNecessary()
{
	if( !m_applied )
		return; // already removed

	if( m_spellProto->CasterAuraState && m_target && !(m_target->GetUInt32Value(UNIT_FIELD_AURASTATE) & (uint32(1) << (m_spellProto->CasterAuraState-1)) ) )
	{
		//printf("Removing aura due to lacking aurastate %u\n", m_spellProto->CasterAuraState);
		ApplyModifiers(false);
		return;
	}
	if( m_spellProto->CasterAuraStateNot && m_target && m_target->GetUInt32Value(UNIT_FIELD_AURASTATE) & (uint32(1) << (m_spellProto->CasterAuraStateNot-1)) ) 
	{
		ApplyModifiers(false);
		return;
	}
}

void Aura::ApplyModifiers( bool apply )
{ 
	if(!m_applied && !apply)	// Don't want to unapply modifiers if they haven't been applied
		return;

	m_applied = apply;
	if( apply && m_spellProto->CasterAuraState && m_target && !(m_target->GetUInt32Value(UNIT_FIELD_AURASTATE) & (uint32(1) << (m_spellProto->CasterAuraState - 1) ) ) )
	{
		//printf("Unable to apply due to lacking aura state %u\n", m_spellProto->CasterAuraState);
		m_applied = false;
		return;
	}
	if( apply && m_spellProto->CasterAuraStateNot && m_target && m_target->GetUInt32Value(UNIT_FIELD_AURASTATE) & (uint32(1) << (m_spellProto->CasterAuraStateNot - 1) ) ) 
	{
		//printf("Unable to apply due to having aura state %u\n", m_spellProto->CasterAuraStateNot);
		m_applied = false;
		return;
	}

	for( uint32 x = 0; x < m_modcount; x++ )
	{
		mod = &m_modList[x];
		DEBUG_LOG( "Aura","Applying Modifiers target = %u , Spell Aura id = %u, SpellId  = %u, i = %u, apply = %s, duration = %u, damage = %d",
					m_target->GetLowGUID(),mod->m_type, m_spellProto->Id, mod->i, apply ? "true" : "false",GetDuration(),mod->m_amount); 

		if(mod->m_type<TOTAL_SPELL_AURAS)
		{
			DEBUG_LOG( "Aura","Known Aura id %d, value %d", (uint32)mod->m_type, (uint32)mod->m_amount );
			(*this.*SpellAuraHandler[mod->m_type])(apply);
		}
		else
			DEBUG_LOG( "Aura","Unknown Aura id %d", (uint32)mod->m_type);
	}
	
	if(GetSpellProto()->procFlags)
	{
		for(uint32 x=0; x<3; x++)
			if(GetSpellProto()->EffectApplyAuraName[x] == SPELL_AURA_PROC_TRIGGER_SPELL||GetSpellId()==974||GetSpellId()==32593||GetSpellId()==32594)
				return;//already have proc for this aura

		Unit * m_caster = GetUnitCaster();
		if(apply)
		{
			ProcTriggerSpell pts;
			memset(&pts, 0, sizeof(ProcTriggerSpell));
			pts.origId = GetSpellId();
			pts.caster = m_casterGuid;
			pts.spellId = 0;
			for(uint32 x=0; x<3; x++)
			{
				if(GetSpellProto()->EffectTriggerSpell[x] != 0)
				{
					pts.spellId = GetSpellProto()->EffectTriggerSpell[x];
					break;
				}
			}
			if(!pts.spellId)
				return;

			pts.procChance = GetSpellProto()->procChance;
			pts.procFlags = GetSpellProto()->procFlags;
			pts.procCharges = GetSpellProto()->procCharges;
			pts.LastTrigger = 0;
			pts.ProcType = 0;
			pts.weapon_damage_type = 0;
			/*
			pts.SpellClassMask[0] = GetSpellProto()->EffectSpellClassMask[mod->i][0];
			pts.SpellClassMask[1] = GetSpellProto()->EffectSpellClassMask[mod->i][1];
			pts.SpellClassMask[2] = GetSpellProto()->EffectSpellClassMask[mod->i][2];*/
			pts.deleted = false;
			m_target->m_procSpells.push_front(pts);
		}
		else
		{
			for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
			{
				if(itr->origId == GetSpellId() && itr->caster == m_casterGuid && !itr->deleted)
				{
					itr->deleted = true;
					break;
				}
			}
		}
	}
}

void Aura::UpdateModifiers( )
{
	for( uint32 x = 0; x < m_modcount; x++ )
	{
		mod = &m_modList[x];

		if(mod->m_type<TOTAL_SPELL_AURAS)
		{
			DEBUG_LOG( "Aura","Updating Aura modifiers target = %u , Spell Aura id = %u (%s), SpellId  = %u, i = %u, duration = %u, damage = %d",
						m_target->GetLowGUID(),mod->m_type, SpellAuraNames[mod->m_type], m_spellProto->Id, mod->i, GetDuration(),mod->m_amount);
			switch (mod->m_type)
			{
				case 33: UpdateAuraModDecreaseSpeed(); break;

			}
		}
		else
			DEBUG_LOG( "Aura","Unknown Aura id %d", (uint32)mod->m_type);
	}
}

void Aura::AddAuraVisual()
{
	uint8 slot, i;
	slot = 0xFF;

	if (IsPositive())
	{
		for (i = 0; i < MAX_POSITIVE_AURAS; i++)
		{
			//if (m_target->GetUInt32Value((uint16)(UNIT_FIELD_AURA + i)) == 0)
			if(m_target->m_auras[i] == NULL)
			{
				slot = i;
				break;
			}
		}
	}
	else
	{
		for (i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
		{
			//if (m_target->GetUInt32Value((uint16)(UNIT_FIELD_AURA + i)) == 0)
			if(m_target->m_auras[i] == NULL)
			{
				slot = i;
				break;
			}
		}
	}

	if (slot == 0xFF)
	{
		return;
	}
	m_auraSlot = slot;
	BuildAuraUpdate();
}

void Aura::BuildAuraUpdate()
{
	uint32 spellid = m_spellProto->Id;
	if( m_target == NULL || !spellid)
		return;

    WorldPacket data(SMSG_AURA_UPDATE, 50);
    FastGUIDPack(data, m_target->GetGUID());
	
    data << uint8(m_auraSlot);
	
	uint8 stack = stackSize;
	if(procCharges > stack && stack != 0)
		stack = procCharges;

    if(stack == 0)
    {
		data << uint32(0);
        m_target->SendMessageToSet(&data, true);
        return;
    }
	data << uint32(spellid);
	uint16 flags = GetAuraFlags();
	if( IsPositive() && !(flags & AFLAG_POSITIVE))
		flags |= AFLAG_POSITIVE;
	else if( !IsPositive() && !(flags & AFLAG_NEGATIVE))
		flags |= AFLAG_NEGATIVE;

    data << flags;
	
	data << (uint8) stack;

    if(!(flags & AFLAG_NOT_GUID))
    {
        FastGUIDPack(data, GetCasterGUID());
	}

	if( flags & AFLAG_HAS_DURATION )
	{
		data << GetDuration();
		data << GetTimeLeft();
	}

	m_target->SendMessageToSet(&data, true);
}

// Modifies current aura duration based on mechanic specified
void Aura::DurationPctMod(uint32 mechanic)
{
	if( m_target->IsPlayer() && mechanic < NUM_MECHANIC && GetDuration() > 0 )
		SetDuration( GetDuration() * TO_PLAYER(m_target)->MechanicDurationPctMod[mechanic] );
}

void Aura::EventUpdateCreatureAA(float r)
{
	/* burlex: cheap hack to get this to execute in the correct context always */
	if(event_GetCurrentInstanceId() == -1)
	{
		event_Relocate();
		return;
	}

	// if the caster is no longer valid->remove the aura
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL)
	{
		Remove();
		return;
	}

	uint32 i;
	uint32 areatargets = m_spellProto->AreaAuraTarget;
	Unit* target;

	// simple. loop inrange units, if they're same faction, apply aura.
	// apply to caster
	if( m_target != m_caster && targets.find(m_caster->GetUIdFromGUID()) == targets.end() )
	{
		if( !(areatargets & AA_TARGET_NOTSELF) )
		{
			Aura* aura = NULL;
			aura = new Aura(m_spellProto, -1, m_caster, m_caster);
			aura->m_areaAura = true;
			for(i = 0; i < 3; ++i)
			{
				if( m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA )
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
			}

			m_caster->AddAura(aura);
			targets.insert(m_caster->GetUIdFromGUID());
		}
	}

	// cancel from old targets
	AreaAuraList::iterator itr, it2;

	for(itr = targets.begin(); itr != targets.end(); )
	{
		it2 = itr++;
		if((*it2) == GET_LOWGUID_PART(m_casterGuid))
			continue;

		Unit* c = m_caster->GetMapMgr()->GetCreature(*it2);
		if( c == NULL || c->GetDistanceSq(m_caster) > r || c->isDead() )
		{
			targets.erase(it2);
			if( c != NULL )
				c->RemoveAura(m_spellProto->Id);
		}
	}

	// loop inrange units
	if( areatargets & AA_TARGET_ALLFRIENDLY )
	{
		Object::InRangeSet::iterator it3 = m_caster->GetInRangeSetBegin();
		for(; it3 != m_caster->GetInRangeSetEnd(); ++it3)
		{
			if( !(*it3) )
				continue;

			if( (*it3)->IsUnit() )
				target = TO_UNIT( (*it3) );
			else
				continue;

			if( isHostile( m_caster, target ) )
				continue;

			if( !(target->isAlive() && target->GetDistanceSq(m_caster) <= r && !target->HasActiveAura(m_spellProto->Id)) )
				continue;

			Aura* aura = NULL;
			for(i = 0; i < 3; ++i)
			{
				if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
				{
					if(!aura)
					{
						aura = new Aura(m_spellProto, -1, m_caster, target);
						aura->m_areaAura = true;
					}
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
				}
			}
			if(aura)
			{
				target->AddAura(aura);
				targets.insert(target->GetUIdFromGUID());
			}			
		}
	}
	else
	{
		Object::InRangeSet::iterator itrr = m_caster->GetInRangeOppFactsSetBegin();
		for(; itrr != m_caster->GetInRangeOppFactsSetEnd(); ++itrr)
		{
			if( !(*itrr) )
				continue;

			if( (*itrr)->IsPlayer() || (*itrr)->IsUnit() )
				target = TO_UNIT( (*itrr) );
			else
				continue;

			if( !isHostile( m_caster, target ) )
				continue;

			if( !(target->isAlive() && target->GetDistanceSq(m_caster) <= r && !target->HasActiveAura(m_spellProto->Id)) )
				continue;

			Aura* aura = NULL;
			for(i = 0; i < 3; ++i)
			{
				if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
				{
					if(!aura)
					{
						aura = (new Aura(m_spellProto, -1, m_caster, target));
						aura->m_areaAura = true;
					}
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
				}
			}
			if(aura)
			{
				target->AddAura(aura);
				targets.insert(target->GetLowGUID());
			}	
		}
	}
}

void Aura::EventRelocateRandomTarget()
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL || !m_caster->IsPlayer() || m_caster->isDead() || !m_caster->GetInRangeCount() )
		return;

	// Ok, let's do it. :D
	set<Unit* > enemies;

	unordered_set<Object* >::iterator itr = m_caster->GetInRangeSetBegin();
	for(; itr != m_caster->GetInRangeSetEnd(); ++itr)
	{
		if( !(*itr)->IsUnit() )
			continue;

		if( !isHostile( m_caster, *itr ) )
			continue;

		// Too far away or dead, or I can't see him!
		if( TO_UNIT(*itr)->isDead() || m_caster->GetDistance2dSq( *itr ) > 100 || !TO_PLAYER(m_caster)->CanSee(*itr) )
			continue;

		if (m_caster->GetMapMgr() && m_caster->GetMapMgr()->IsCollisionEnabled())
		{
			if( !CollideInterface.CheckLOS( m_caster->GetMapId(), m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ() + 2.0f, (*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ() + 2.0f) )
				continue;
		}

		enemies.insert( TO_UNIT(*itr) );
	}

	// Can't do anything w/o a target
	if( !enemies.size() )
		return;

	uint32 random = RandomUInt(uint32(enemies.size()) - 1);
	set<Unit* >::iterator it2 = enemies.begin();
	while( random-- )
		it2++;

	Unit* pTarget = (*it2);
	if(pTarget == NULL)
		return; // In case I did something horribly wrong.

	float ang = pTarget->GetOrientation();

	// avoid teleporting into the model on scaled models
	const static float killingspree_distance = 1.6f * GetScale( dbcCreatureDisplayInfo.LookupEntry( pTarget->GetUInt32Value(UNIT_FIELD_DISPLAYID)));
	float new_x = pTarget->GetPositionX() - (killingspree_distance * cosf(ang));
	float new_y = pTarget->GetPositionY() - (killingspree_distance * sinf(ang));
	float new_z = pTarget->GetPositionZ() + 0.1f;
	if (pTarget->GetMapMgr() && pTarget->GetMapMgr()->IsCollisionEnabled())
	{
		float z2 = CollideInterface.GetHeight(pTarget->GetMapId(), pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());
		if( z2 == NO_WMO_HEIGHT )
			z2 = pTarget->GetMapMgr()->GetLandHeight(new_x, new_y);

		if( fabs( new_z - z2 ) < 10.0f )
				new_z = z2 + 0.2f;
	}

	TO_PLAYER(m_caster)->SafeTeleport( pTarget->GetMapId(), pTarget->GetInstanceID(), new_x, new_y, new_z, pTarget->GetOrientation() );
	// void Unit::Strike( Unit pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 add_damage, int32 pct_dmg_mod, uint32 exclusive_damage, bool disable_proc, bool skip_hit_check )
	TO_PLAYER(m_caster)->Strike( pTarget, MELEE, NULL, 0, 0, 0, false, false, true );
	TO_PLAYER(m_caster)->Strike( pTarget, OFFHAND, NULL, 0, 0, 0, false, false, true );
}

void Aura::EventUpdatePlayerAA(float r)
{
	if(m_auraSlot > MAX_AURAS+MAX_PASSIVE_AURAS)
	{
		//this event is no longer valid, remove it.
		sEventMgr.RemoveEvents(this);
		Log.Error("Aura","Encountered an illegal EventUpdatePlayerAAura, removing it from event-holder.");
		return;
	}	

	/* burlex: cheap hack to get this to execute in the correct context always */
	if(event_GetCurrentInstanceId() == -1)
	{
		event_Relocate();
		return;
	}

	if ( m_deleted )
		return;

	uint32 i;
	uint32 areatargets = m_spellProto->AreaAuraTarget;

	// if the caster is no longer valid->remove the aura
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL)
	{
		Remove();
		return;
	}

	Player* plr = NULL;

	if(m_caster->GetTypeId() == TYPEID_PLAYER)
		plr = TO_PLAYER(m_caster);
	else if(m_caster->GetTypeId() == TYPEID_UNIT)
	{
		if(m_caster->IsPet())
			plr = TO_PET(m_caster)->GetPetOwner();
		else
			plr = TO_PLAYER( TO_CREATURE(m_caster)->GetSummonOwner());
	}


	if(plr == NULL || plr->GetTypeId() != TYPEID_PLAYER)	// No player involved...
		return;

	vector<uint32> NewTargets;

	if( (!m_caster->IsPlayer() || (m_caster->IsPlayer() && !(areatargets & AA_TARGET_NOTSELF))) && plr->GetDistanceSq(m_caster) <= r )
	{
		if(!plr->HasActiveAura(m_spellProto->Id))
		{
			Aura* aura = NULL;
			for(i = 0; i < 3; ++i)
			{
				/* is this an area aura modifier? */
				if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
				{
					if(!aura)
					{
						aura = (new Aura(m_spellProto, -1, m_caster, plr));
						aura->m_areaAura = true;
					}
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
				}
			}
			if(aura)
			{
				plr->AddAura(aura);
				NewTargets.push_back(plr->GetLowGUID());
			}
		}
	}

	if( areatargets & AA_TARGET_PARTY || areatargets & AA_TARGET_RAID )
	{
		if( plr->GetGroup() )
		{
			// cant think im sleepy if you have better ideas delete this shit and correct
			plr->GetGroup()->Lock();

			for(uint32 x = 0; x < plr->GetGroup()->GetSubGroupCount(); ++x )
			{
				for(GroupMembersSet::iterator itr = plr->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != plr->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); ++itr)
				{
					if((*itr) && (*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer != plr && (*itr)->m_loggedInPlayer->GetDistanceSq(m_caster) <= r)
					{
						// Add the aura if they don't have it.
						if(!(*itr)->m_loggedInPlayer->HasActiveAura(m_spellProto->Id) &&
							(*itr)->m_loggedInPlayer->GetInstanceID() == plr->GetInstanceID() && (*itr)->m_loggedInPlayer->isAlive())
						{
							Aura* aura = NULL;
							//aura->AddMod(mod->m_type, mod->m_amount, mod->m_miscValue, mod->i);
							for(i = 0; i < 3; ++i)
							{
								/* is this an area aura modifier? */
								if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
								{
									if(!aura)
									{
										//Aura::Aura( SpellEntry* proto, int32 duration, Object* caster, Unit* target )
										aura = new Aura(m_spellProto, -1, m_caster, (*itr)->m_loggedInPlayer);
										aura->m_areaAura = true;
									}
									aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
										m_spellProto->EffectMiscValue[i], i);
								}
							}
							if( aura )
							{
								(*itr)->m_loggedInPlayer->AddAura(aura);
								NewTargets.push_back((*itr)->m_loggedInPlayer->GetLowGUID());
							}
						}
					}
				}
			}
			plr->GetGroup()->Unlock();
		}
	}
	//heavy
	else if( areatargets & AA_TARGET_ALLFRIENDLY )
	{
		Unit* target;
		Object::InRangeSet::iterator itr = m_caster->GetInRangeSetBegin();
		for(; itr != m_caster->GetInRangeSetEnd(); ++itr)
		{
			if( !(*itr) )
				continue;

			if( (*itr)->IsPlayer() || (*itr)->IsUnit() )
				target = TO_UNIT( (*itr) );
			else
				continue;

			if( isHostile( m_caster, target ) )
				continue;

			if( !(target->isAlive() && target->GetDistanceSq(m_caster) <= r && !target->HasActiveAura(m_spellProto->Id)) )
				continue;

			Aura* aura = NULL;
			for(i = 0; i < 3; ++i)
			{
				if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
				{
					if(!aura)
					{
						aura = (new Aura(m_spellProto, -1, m_caster, target));
						aura->m_areaAura = true;
					}
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
				}
			}
			if(aura)
			{
				target->AddAura(aura);
				NewTargets.push_back(target->GetLowGUID());
			}			
			
		}
	}
	else if( areatargets & AA_TARGET_ALLENEMIES )
	{
		Unit* target;
		Object::InRangeSet::iterator itr = m_caster->GetInRangeOppFactsSetBegin();
		for(; itr != m_caster->GetInRangeOppFactsSetEnd(); ++itr)
		{
			if( !(*itr) )
				continue;

			if( (*itr)->IsPlayer() || (*itr)->IsUnit() )
				target = TO_UNIT( (*itr) );
			else
				continue;

			if( !isHostile( m_caster, target ) )
				continue;

			if( !(target->isAlive() && target->GetDistanceSq(m_caster) <= r && !target->HasActiveAura(m_spellProto->Id)) )
				continue;

			Aura* aura = NULL;
			for(i = 0; i < 3; ++i)
			{
				if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
				{
					if(!aura)
					{
						aura = (new Aura(m_spellProto, -1, m_caster, target));
						aura->m_areaAura = true;
					}
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
				}
			}
			if(aura)
			{
				target->AddAura(aura);
				NewTargets.push_back(target->GetLowGUID());
			}	
		}
	}

	if( areatargets & AA_TARGET_PET )
	{

		Pet* pPet = plr->GetSummon();
		if( pPet && plr->GetDistanceSq(pPet) <= r && !pPet->HasActiveAura(m_spellProto->Id) )
		{
			Aura* aura = NULL;
			for(i = 0; i < 3; ++i)
			{
				if(m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA)
				{
					if(!aura)
					{
						aura = (new Aura(m_spellProto, -1, plr, pPet));
						aura->m_areaAura = true;
					}
					aura->AddMod(m_spellProto->EffectApplyAuraName[i], m_spellProto->EffectBasePoints[i],
						m_spellProto->EffectMiscValue[i], i);
				}
			}
			if(aura)
			{
				pPet->AddAura(aura);
				NewTargets.push_back(pPet->GetLowGUID());
			}
		}
	}



	// Update the existing players in the target set.
	AreaAuraList::iterator itr, it2;

	for(itr = targets.begin(); itr != targets.end(); )
	{
		it2 = itr;
		++itr;

		// Check if the target is 'valid'.
		Unit* unt = NULL;
		if(m_target->IsInWorld())
			unt = m_target->GetMapMgr()->GetUnit(*it2);
		
		if(!unt || unt->GetDistanceSq(m_caster) > r || unt->GetInstanceID() != plr->GetInstanceID())

		{
			targets.erase(it2);
			if(unt)
			{
				// execute in the correct context
                if(unt->GetInstanceID() != plr->GetInstanceID())
					sEventMgr.AddEvent((unt), &Unit::EventRemoveAura, m_spellProto->Id, EVENT_DELETE_TIMER, 10, 1,0);
				else
					unt->RemoveAura(m_spellProto->Id);
			}

			continue;
		}

		// Is the player outside of the group now?
		// (doesn't apply to the caster)
		if( plr == unt || !unt->IsPlayer() ) continue;

		if(!plr->GetGroup() || !plr->GetGroup()->HasMember(TO_PLAYER(unt)))
		{
			// execute in the correct context
			if(unt->GetInstanceID() != plr->GetInstanceID())
				sEventMgr.AddEvent((unt), &Unit::EventRemoveAura, m_spellProto->Id, EVENT_DELETE_TIMER, 10, 1,0);
			else
				unt->RemoveAura(m_spellProto->Id);
			targets.erase(it2);
		}
	}

	// Push new targets into the set.
	for(vector<uint32>::iterator vtr = NewTargets.begin(); vtr != NewTargets.end(); ++vtr)
		targets.insert((*vtr));

	NewTargets.clear();
}

void Aura::RemoveAA()
{
	AreaAuraList::iterator itr;

	for(itr = targets.begin(); itr != targets.end(); ++itr)
	{
		// Check if the target is 'valid'; must be on same mapmgr
		Player* iplr = NULL;
		if(m_target != NULL && m_target->IsInWorld())
			iplr = m_target->GetMapMgr()->GetPlayer((uint32)*itr);

		if( iplr != NULL && m_casterGuid == iplr->GetGUID() ) // Don't remove from self.
			continue;

		if( iplr == NULL && !GetUnitCaster()->IsPlayer() )
		{
			Creature* icrt = NULL;
			if( m_target != NULL && m_target->IsInWorld() )
				icrt = m_target->GetMapMgr()->GetCreature((uint32)*itr);

			if(icrt!= NULL)
				icrt->RemoveAura( m_spellProto->Id );
		}

		if( iplr != NULL )
			iplr->RemoveAura(m_spellProto->Id);
	}
	targets.clear();
}

//------------------------- Aura Effects -----------------------------

void Aura::SpellAuraModBaseResistance(bool apply)
{
	SpellAuraModResistance(apply);
	//both add/decrease some resistance difference is unknown
}

void Aura::SpellAuraModBaseResistancePerc(bool apply)
{
	uint32 Flag = mod->m_miscValue; 
	int32 amt;
	if(apply)
	{
		amt = mod->m_amount;
		if(amt > 0)
			SetPositive();
		else 
			SetNegative();
	}
	else
		amt =- mod->m_amount;

	for(uint32 x=0;x<7;x++)
	{
		if(Flag & (((uint32)1)<< x))
		{
			if(m_target->IsPlayer())
			{
				if(mod->m_amount>0)
				{
					TO_PLAYER( m_target )->BaseResistanceModPctPos[x]+=amt;
				}
				else
				{
					TO_PLAYER( m_target )->BaseResistanceModPctNeg[x]-=amt;
				}
				TO_PLAYER( m_target )->CalcResistance(x);

			}
			else if(m_target->GetTypeId() == TYPEID_UNIT)
			{
				TO_CREATURE(m_target)->BaseResistanceModPct[x]+=amt;
				TO_CREATURE(m_target)->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraNULL(bool apply)
{
	 DEBUG_LOG( "Aura","Unknown Aura id %d", (uint32)mod->m_type);
}

void Aura::SpellAuraBindSight(bool apply)
{
	// MindVision
	SetPositive();

	Unit * m_caster = GetUnitCaster();
	if(m_caster != NULL || !m_caster->IsPlayer())
		return;
	if(apply)
		m_caster->SetUInt64Value(PLAYER_FARSIGHT, m_target->GetGUID());
	else
	{
		m_caster->SetUInt64Value(PLAYER_FARSIGHT, 0 );
	}
}

void Aura::SpellAuraModPossess(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL || !m_caster->IsInWorld() || !m_caster->IsPlayer() ) 
		return;

	if( apply )
	{
		TO_PLAYER( m_caster )->Possess( m_target );
		sEventMgr.AddEvent( TO_PLAYER( m_caster ), &Player::UnPossess, EVENT_UNPOSSESS, GetDuration(), 1, 0);
	}
	else
	{//some spells requires that...

		// make sure Player::UnPossess() didn't fail, if it did we will just free the target here
		if( m_target->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) != 0 )
		{
			if( m_target->GetTypeId() == TYPEID_UNIT )
			{
				m_target->EnableAI();
				m_target->m_redirectSpellPackets = NULL;
			}

			m_target->SetUInt64Value( UNIT_FIELD_CHARMEDBY, 0 );
			m_target->RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			m_target->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, m_target->GetCharmTempVal() );
			m_target->_setFaction();
			m_target->UpdateOppFactionSet();
		}
	}
}

void Aura::SpellAuraPeriodicDamage(bool apply)
{
	if(apply)
	{
		int32 dmg	= mod->m_amount;
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL )
		{
			dmg = m_caster->GetSpellBonusDamage(m_target, m_spellProto, dmg, true, false);
			switch(m_spellProto->Id)
			{
				case 703:
				case 8631:
				case 8632:
				case 8633:
				case 8818:
				case 11289:
				case 11290:
						m_caster->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);  // remove stealth
					break;
				case 47855:
					if(m_target->GetHealthPct() <= 25)
						dmg *= 4;
					break;
				//mage talent ignite
				case 12654:
				{
					if(!pSpellId) //we need a parent spell and should always have one since it procs on it
						break;
					SpellEntry * parentsp = NULL;
					parentsp = dbcSpell.LookupEntry(pSpellId);
					if(parentsp == NULL)
						return;
					if ( m_caster->IsPlayer() )
						dmg = float2int32(TO_PLAYER(m_caster)->m_casted_amount[SCHOOL_FIRE]*parentsp->EffectBasePoints[0]/100.0f);
					else
					{
						if (!dmg)
							return;

						Spell* spelld = NULL;
						spelld = (new Spell(m_caster, parentsp ,false,NULL));
						SpellCastTargets targets(m_target->GetGUID());
						//this is so not good, maybe parent spell has more then dmg effect and we use it to calculate our new dmg :(
						dmg = 0;
						for(int i=0;i<3;i++)
						{
						  //dmg +=parentsp->EffectBasePoints[i]*m_spellProto->EffectBasePoints[0];
							dmg +=spelld->CalculateEffect(i,m_target->IsUnit()? TO_UNIT(m_target):NULL)*parentsp->EffectBasePoints[0]/100;
						}
						spelld->Destructor();
					}
				}
			};

			// Add exceptions here :P
			switch( m_spellProto->NameHash )
			{
			case SPELL_HASH_EXPLOSIVE_SHOT:
				{
					EventPeriodicDamage(dmg);
				}break;
			case SPELL_HASH_REND:
				{
					if( m_caster->IsPlayer() )
					{
						Item* weapon = NULL;
						weapon = TO_PLAYER(m_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
						if( weapon != NULL && weapon->GetProto() )
							dmg += float2int32( ( weapon->GetProto()->Delay * m_caster->GetAP() / 14 + (weapon->GetProto()->Damage[0].Min + weapon->GetProto()->Damage[0].Max)/2 ) * (GetDuration() / 1000) * (743 / 300000 ) );
					}
				}break;
			case SPELL_HASH_DEEP_WOUNDS:
				{
					if( m_caster->IsPlayer() && pSpellId )
					{
						uint32 multiplyer = (pSpellId == 12834 ? 16 : (pSpellId==12849 ? 32 : (pSpellId==12867 ? 48 : 0)));
						if(multiplyer)
						{
							if(TO_PLAYER(m_caster) != NULL && TO_PLAYER(m_caster)->GetItemInterface())
							{
								Item* it = NULL;
								it = TO_PLAYER(m_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
								if(it != NULL && it->GetProto())
								{
									dmg = 0;
									for(int i=0;i<5;i++)
										if(it->GetProto()->Damage[i].Type==SCHOOL_NORMAL)
											dmg += int32((it->GetProto()->Damage[i].Min + it->GetProto()->Damage[i].Max) / 2);
									dmg = multiplyer * dmg /100;
								}
							}
						}
					}
				}
			}
		}
		
		if(dmg <= 0)
			return; //who would want a negative dmg here ?

		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];
		
		sEventMgr.AddEvent(this, &Aura::EventPeriodicDamage, (uint32)dmg, 
			EVENT_AURA_PERIODIC_DAMAGE, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		/*TO_PLAYER( m_caster )->GetSession()->SystemMessage("dot will do %u damage every %u seconds (total of %u)", dmg,m_spellProto->EffectAmplitude[mod->i],(GetDuration()/m_spellProto->EffectAmplitude[mod->i])*dmg);
		printf("dot will do %u damage every %u seconds (total of %u)\n", dmg,m_spellProto->EffectAmplitude[mod->i],(GetDuration()/m_spellProto->EffectAmplitude[mod->i])*dmg);*/
		SetNegative();
		if( m_spellProto->buffType & SPELL_TYPE_WARLOCK_IMMOLATE )
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_IMMOLATE);
		}
		//maybe poison aurastate should get triggered on other spells too ?
		else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_POISON )//deadly poison 
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_POISON);
		}
	}
	else
	{
		if( m_spellProto->buffType & SPELL_TYPE_WARLOCK_IMMOLATE )
			m_target->RemoveFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_IMMOLATE );
		//maybe poison aurastate should get triggered on other spells too ?
		else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_POISON )//deadly poison 
		{
			m_target->RemoveFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_POISON);
		}
	}
}

void Aura::EventPeriodicDamage(uint32 amount)
{
	if( !GetSpellProto() || m_target == NULL || m_target->GetMapMgr() == NULL || 
		!m_target->isAlive() || m_target->SchoolImmunityList[GetSpellProto()->School])
	{
		//this event is no longer valid, remove it.
		sEventMgr.RemoveEvents(this);
		return;
	}

	
	SpellEntry *proto = GetSpellProto();
	float res = float(amount);
	bool DOTCanCrit = false;
	uint32 school = GetSpellProto()->School;

	if( m_casterGuid && m_target->GetGUID() != m_casterGuid )//don't use resist when cast on self-- this is some internal stuff
	{
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL )
		{
			if( proto->NameHash == SPELL_HASH_EXPLOSIVE_SHOT || proto->NameHash == SPELL_HASH_MIND_FLAY )
				DOTCanCrit = true;

			m_caster->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);
			
			if(res < 0)
				res = 0;

			else
			{
				float summaryPCTmod = 1.0f;
				if( m_target->IsPlayer() )//resilience
				{
					float dmg_reduction_pct = TO_PLAYER(m_target)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
					if( dmg_reduction_pct > 1.0f )
						dmg_reduction_pct = 1.0f;
					summaryPCTmod -= dmg_reduction_pct;
				}
				res *= summaryPCTmod;
				if( res < 0.0f ) 
					res = 0.0f;
			}
			if( DOTCanCrit == true )
			{
				float CritChance = m_caster->spellcritperc + m_caster->SpellCritChanceSchool[school] + m_target->AttackerCritChanceMod[school];
				if( m_caster->IsPlayer() && ( m_target->m_rooted - m_target->m_stunned ) )	
					CritChance += TO_PLAYER( m_caster )->m_RootedCritChanceBonus;
				if( m_spellProto->SpellGroupType )
				{
					SM_FFValue(m_caster->SM[SMT_CRITICAL][0], &CritChance, m_spellProto->SpellGroupType);
					SM_PFValue(m_caster->SM[SMT_CRITICAL][1], &CritChance, m_spellProto->SpellGroupType);
				}

				if( m_target->IsPlayer() )
					CritChance -= TO_PLAYER(m_target)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE );

				CritChance = ( CritChance < 0 ? 0 : ( CritChance > 95 ? 95 : CritChance ));
				bool critical = Rand(CritChance);

				if (critical)
				{		
					int32 critical_bonus = 100;
					if( m_spellProto->SpellGroupType )
						SM_FIValue( m_caster->SM[SMT_CRITICAL_DAMAGE][1], &critical_bonus, m_spellProto->SpellGroupType );

					if( critical_bonus > 0 )
					{
						// the bonuses are halved by 50% (funky blizzard math :S)
						float b = ( ( float(critical_bonus) / 2.0f ) / 100.0f ) + 1.0f;
						res	*= b;

						if( m_target->IsPlayer() )
						{
							float dmg_reduction_pct = 2.2f * TO_PLAYER(m_target)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
							if( dmg_reduction_pct > 0.33f )
								dmg_reduction_pct = 0.33f; // 3.0.3

							res = res - res * dmg_reduction_pct;
						}
					}
				}
			}
		}

		uint32 ress=(uint32)res;
		uint32 abs_dmg = m_caster != NULL ? m_target->AbsorbDamage(m_caster, school, &ress, m_spellProto) : 0;
		uint32 ms_abs_dmg= m_target->ManaShieldAbsorb(ress, m_spellProto);

		if (ms_abs_dmg)
		{
			if(ms_abs_dmg > ress)
				ress = 0;
			else
				ress-=ms_abs_dmg;

			abs_dmg += ms_abs_dmg;
		}

		
		res=float(ress < 0 ? 0 : ress);
		dealdamage dmg;
		dmg.school_type = school;
		dmg.full_damage = float2int32(res);
		dmg.resisted_damage = 0;

		if(res <= 0) 
			dmg.resisted_damage = dmg.full_damage;


		if(res > 0 && m_caster != NULL && m_spellProto->MechanicsType != MECHANIC_BLEEDING)
		{
			m_caster->CalculateResistanceReduction(m_target,&dmg, m_spellProto,0.0f);
			if((int32)dmg.resisted_damage > dmg.full_damage)
				res = 0;
			else
				res = float(dmg.full_damage - dmg.resisted_damage);
		}

		
		SendPeriodicAuraLog(m_casterGuid, m_target, GetSpellProto(), float2int32(res), abs_dmg, dmg.resisted_damage, FLAG_PERIODIC_DAMAGE);

		if(school == SHADOW_DAMAGE)
			if( m_caster != NULL && m_caster->isAlive() && m_caster->IsPlayer() && m_caster->getClass() == PRIEST )
				TO_PLAYER(m_caster)->VampiricSpell(float2int32(res), m_target, m_spellProto);
	}
	// grep: this is hack.. some auras seem to delete this shit.
	SpellEntry * sp = m_spellProto;
	Unit* mtarget = m_target;
	uint64 cguid = m_casterGuid;
	if( mtarget != NULL )
	{
		Unit * m_caster = NULL;
		m_caster = GetUnitCaster();
		if( m_caster != NULL)
		{
			uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION;
			uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM;
			aproc |= PROC_ON_SPELL_LAND;
			vproc |= PROC_ON_SPELL_LAND_VICTIM;

			m_caster->HandleProc(aproc, mtarget, sp, float2int32(res));
			m_caster->m_procCounter = 0;

			mtarget->HandleProc(vproc,m_caster,sp, float2int32(res));
			mtarget->m_procCounter = 0;
		}

		if( m_caster != NULL)
			m_caster->DealDamage(mtarget, float2int32(res), 2, 0, GetSpellId ());
		else
			mtarget->DealDamage(mtarget, float2int32(res), 2, 0, GetSpellId ());
	}
}

void Aura::SpellAuraDummy(bool apply)
{
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummyAura(GetSpellId(), mod->i, this, apply))
		return;

	uint32 TamingSpellid = 0;
	Unit * m_caster = GetUnitCaster();

	// for seal -> set judgement crap
	if( GetSpellProto()->buffType & SPELL_TYPE_SEAL && mod->i == 2 )
	{
		Player* c = TO_PLAYER( m_caster );

		if( c == NULL )
			return;
		
		if( apply )
		{
			c->SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_JUDGEMENT );
			if( !c->Seal )
				c->Seal = m_spellProto->Id;
		}
		else
		{
			c->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_JUDGEMENT );
			if( c->Seal )
				c->Seal = 0;
		}
	}

	bool dummy_aura = false;

	Player* _ptarget = TO_PLAYER( m_target );

	switch(GetSpellId())
	{
	case 13809: //Frost Traps
		{

			SpellEntry *spellentry = dbcSpell.LookupEntry( 13810 );
			if(!spellentry)
			{
				return;
			}
			if (!m_spellProto->Effect[0])
			{
				return;
			}

			Spell* sp = NULL;
			sp = new Spell(m_caster, spellentry, false, this);

			SpellCastTargets targets;
			targets.m_unitTarget = m_caster->GetGUID();
			sp->prepare(&targets);

		}break;

	// Cheat Death
	case 31228:
	case 31229:
	case 31230:
		{
			if( apply )
			{
				if( GetSpellId() == 31228 )
					_ptarget->m_cheatDeathRank = 1;
				else if( GetSpellId() == 31229 )
					_ptarget->m_cheatDeathRank = 2;
				else
					_ptarget->m_cheatDeathRank = 3;
			}
			else 
				_ptarget->m_cheatDeathRank = 0;
		}break;
	// Death Knight: Shadow of Death!
	case 54223:
		{
			if( !m_target->IsPlayer() )
				return;

			if(apply)
			{
				TO_PLAYER(m_target)->FullHPMP();
				TO_PLAYER(m_target)->ResurrectPlayer(NULL);
				m_target->SetUInt32Value( UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 100 );
				m_target->SetPowerType( POWER_TYPE_ENERGY );
				
			}
			else
			{
				if( m_target->isAlive() )
				{
					m_target->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
					TO_PLAYER(m_target)->KillPlayer();
				}

				m_target->SetUInt32Value( UNIT_FIELD_MAXPOWER1 + POWER_TYPE_ENERGY, 0 );
				m_target->SetPowerType( POWER_TYPE_RUNIC );
			}
			
		}break;

	case 48181://Haunt
	case 59161:
	case 59163:
	case 59164:
		{
			if( !apply && m_caster != NULL && m_caster->m_lastHauntInitialDamage )
			{
				m_caster->Heal( m_caster, 48210, m_caster->m_lastHauntInitialDamage );
			}
		}break;
	case 48504://Living seed
		{
			if( !apply && m_target->IsPlayer() )
			{
				TO_PLAYER(m_target)->Heal( m_target, 48504, mod->m_amount);
			}
		}break;
	/*case 61216:
	case 61221:
	case 61222:
	case 48978: // Death Knight: Bladed Armor
	case 49390:
	case 49391:
	case 49392:
	case 49393:
		{
			if( !_ptarget )
				return;

			if( apply )
			{
				uint32 amt = GetSpellProto()->RankNumber * (_ptarget->GetUInt32Value(UNIT_FIELD_RESISTANCES)/180);
				_ptarget->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, amt);
			}
			else
			{
				uint32 amt = GetSpellProto()->RankNumber * (_ptarget->GetUInt32Value(UNIT_FIELD_RESISTANCES)/180);
				_ptarget->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, -int32(amt));
			}
			_ptarget->CalcDamage();
		}break;*/
	case 51690:
		{
			if( !m_caster->IsPlayer() )
				return;


			if( apply )
			{
				//uint32 type, uint32 time, uint32 repeats, uint32 flags
				sEventMgr.AddEvent(this, &Aura::EventRelocateRandomTarget, EVENT_AURA_PERIODIC_TELEPORT, 500, 5, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); 
			}
			else
			{
				sEventMgr.RemoveEvents(this, EVENT_AURA_PERIODIC_TELEPORT );
			}
		}break;
	case 26013: // deserter
		{
			if(apply)
				SetNegative();
		}break;
	case 41425:
		{
			if( m_target->IsPlayer() )
				TO_PLAYER(m_target)->mHypothermia = apply;
		}break;
		//Warlock - Demonic Knowledge
	case 35696:
		{
			if ( m_target->IsPet() )
			{
				Unit* PetOwner;
				if ( (PetOwner = TO_PET( m_target )->GetPetOwner()) != NULL )
				{
					PetOwner = TO_PET( m_target )->GetPetOwner();
					if( apply )
						mod->realamount = m_target->GetStamina() + m_target->GetIntellect();
					
					float dmginc = float(mod->realamount * mod->m_amount) / 100.0f;
					int32 val;

					if( apply )
						val = (int32)dmginc;
					else
						val = (int32)-dmginc;

					for (uint32 x=0;x<7;x++)
						PetOwner->ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, val);

					PetOwner->CalcDamage();
				}
			}
		}break;
	case 32612:
		{
			// mage - invisibility override
			if( m_target->IsPlayer() && m_target->IsInWorld() )
			{
				TO_PLAYER(m_target)->m_mageInvisibility = apply;
				TO_PLAYER(m_target)->UpdateVisibility();
			}
		}break;
	//warrior - sweeping strikes
	case 12328:
		{
			if(apply)
				m_target->AddExtraStrikeTarget(GetSpellProto(), 10);
			else
				m_target->RemoveExtraStrikeTarget(GetSpellProto());
		}break;
	//taming rod spells
	case 19548:	{                 //invoke damage to trigger attack
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19597; //uses Spelleffect3 #19614
	}break;
	case 19674:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19677;
	}break;
	case 19687:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19676;
	}break;
	case 19688:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19678;
	}break;
	case 19689:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19679;
	}break;
	case 19692:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19680;
	}break;
	case 19693:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19684;
	}break;
	case 19694:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19681;
	}break;
	case 19696:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19682;
	}break;
	case 19697:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19683;
	}break;
	case 19699:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19685;
	}break;
	case 19700:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=19686;
	}break;
	case 30099:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=30100;
	}break;
	case 30102:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=30103;
	}break;
	case 30105:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=30104;
	}break;
	case 30646:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=30647;
	}break;
	case 30653:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=30648;
	}break;
	case 30654:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( m_caster, 10, 0);
			break;
		}
		else
			TamingSpellid=30652;
	}break;
	case 16972://Predatory Strikes
	case 16974:
	case 16975:
	{
		if(apply)
		{
			SetPositive();
			mod->realamount = (mod->m_amount * m_target->getLevel())/100;
			m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS,mod->realamount);
		}else
			m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, -mod->realamount);
		m_target->CalcDamage();
	}break;
	case 126: //Eye of Killrog     
		{
			/*if(m_target->IsInWorld() == false)
				return;

			if(!apply)
			{
				m_target->SetUInt64Value(PLAYER_FARSIGHT,0);
				Creature *summon = m_target->GetMapMgr()->GetCreature(m_target->GetUInt32Value(UNIT_FIELD_SUMMON));
				m_target->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
				m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

				if(summon)
				{
					summon->RemoveFromWorld(false,true);
					delete summon;
				}
				m_target->m_noInterrupt--;
				if(m_target->m_noInterrupt < 0)
					m_target->m_noInterrupt = 0;
			}*/
		}break;
	case 12295:
	case 12676:
	case 12677:
	case 12678:
		{
			if(apply)
				_ptarget->m_retainedrage += mod->m_amount*10; //don't really know if value is all value or needs to be multiplyed with 10
			else
				_ptarget->m_retainedrage -= mod->m_amount*10;
		}break;
	case 2096://MindVision
		{
		}break;
	case 6196://FarSight
		{
			if(apply)
			{
			}
			else
			{
				// disabled this due to unstableness :S
#if 0
				Creature* summon = NULL;
				summon = m_target->GetMapMgr()->GetCreature(m_target->GetUInt32Value(PLAYER_FARSIGHT));
				if(summon!= NULL)
				{
					summon->RemoveFromWorld(false,true);
					delete summon;
				}
				m_target->SetUInt64Value(PLAYER_FARSIGHT,0);
#endif
			}
		}break;
	case 15286://Vampiric Embrace
		{
			if(apply)
			{
				SetNegative();
				if(m_caster != NULL && m_caster->IsPlayer())
					TO_PLAYER(m_caster)->m_vampiricEmbrace++;
			}
			else
			{
				if(m_caster != NULL && m_caster->IsPlayer())
					TO_PLAYER(m_caster)->m_vampiricEmbrace--;
			}
		}break;
	case 34914://Vampiric Touch
	case 34916:
	case 34917:
	case 48159:
	case 48160:
		{
			if(apply)
			{
				if( m_target )
					m_target->m_vampiricTouch++;
			}
			else
			{
				if( m_target )
					m_target->m_vampiricTouch--;
			}
		}break;
	case 18182:
	case 18183:
		{//improved life tap give amt% bonus for convers
			if(apply)
				_ptarget->m_lifetapbonus=mod->m_amount;
			else
				_ptarget->m_lifetapbonus=0;
		}break;
	case 20608://Reincarnation
		{
			if(apply)
				_ptarget->bReincarnation = true;
			else
				_ptarget->bReincarnation = false;
		}break;
	case 20707://Soulstone Resurrecton
		if(apply)
		{
            _ptarget->SetSoulStone(3026);
			_ptarget->SetSoulStoneReceiver((uint32)(uint32)m_casterGuid);
		}
		else if(m_target->isAlive())
        {
            _ptarget->SetSoulStone(0);
            _ptarget->SetSoulStoneReceiver(0);
        }break;
	case 20762:
		if(apply)
		{
			_ptarget->SoulStone = 20758;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 20763:
		if(apply)
		{
			_ptarget->SoulStone = 20759;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 20764:
		if(apply)
		{
			_ptarget->SoulStone = 20760;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 20765:
		if(apply)
		{
			_ptarget->SoulStone = 20761;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 27239:
		{
			if(apply)
			{
				_ptarget->SoulStone = 27240;
				_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
			}
			else if(m_target->isAlive())
				_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		}break;

	case 47883:
		{
			if(apply)
			{
				_ptarget->SoulStone = 47882;
				_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
			}
			else if(m_target->isAlive())
				_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		}break;

	case 1002:  // Eyes of the beast
		{
			// Take control of pets vision

			// set charmed by and charm target
			if(m_caster == NULL || !m_caster->IsPlayer())
				return;

			if(apply)
			{
				m_target->DisableAI();
				m_caster->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
				m_target->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, 0);
				m_caster->SetUInt64Value(UNIT_FIELD_CHARM, m_target->GetGUID());
				m_target->SetUInt64Value(UNIT_FIELD_CHARMEDBY, m_caster->GetGUID());
				m_caster->SetUInt64Value(PLAYER_FARSIGHT, m_target->GetGUID());
				m_caster->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(m_caster), m_target, true);
				TO_PLAYER(m_caster)->m_CurrentCharm = TO_CREATURE(m_target);
				m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);
				m_caster->m_noInterrupt = 1;
				m_caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

				WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE, 10);
				data << m_target->GetNewGUID() << uint8(0);
				TO_PLAYER(m_caster)->GetSession()->SendPacket(&data);
			}
			else
			{
				m_caster->EnableAI();
				m_caster->SetUInt64Value(UNIT_FIELD_SUMMON, m_target->GetGUID());
				m_target->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, m_caster->GetGUID());
				m_caster->SetUInt64Value(UNIT_FIELD_CHARM, 0);
				m_target->SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
				m_caster->SetUInt64Value(PLAYER_FARSIGHT, 0);
				m_caster->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(m_caster), m_target, false);
				TO_PLAYER(m_caster)->m_CurrentCharm = NULL;
				m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);
				m_caster->m_noInterrupt = 0;
				m_caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

				WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE, 10);
				data << m_caster->GetNewGUID() << uint8(1);
				TO_PLAYER(m_caster)->GetSession()->SendPacket(&data);
			}
		}break;
	/*case 570:   // far sight
	case 1345:
	case 6197:
	case 6198:  // eagle eye
	case 24125:
	case 21171:
		{
				if(m_target->IsPlayer() || !m_target->IsInWorld())
				return;
				Creature* farsight = TO_PLAYER(m_target)->GetMapMgr()->GetCreature(TO_PLAYER(m_target)->GetUInt32Value(PLAYER_FARSIGHT));
				TO_PLAYER(m_target)->SetUInt64Value(PLAYER_FARSIGHT, 0);
				TO_PLAYER(m_target)->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(m_target), 0, 0, false);
				if(farsight)
				{
					farsight->RemoveFromWorld(false,true);
					farsight->Destructor();
				}
		}break;*/

	case 23701://Darkmoon Card: Twisting Nether give 10% chance to self resurrect ->cast 23700
		{
			//if(!apply)

		}break;

	//Second Wind - triggers only on stun and Immobilize
	case 29834:
		{
			if(m_caster != NULL && m_caster->IsPlayer())
				TO_PLAYER(m_caster)->SetTriggerStunOrImmobilize(29841,100);//fixed 100% chance
		}break;
	case 29838:
		{
			if(m_caster != NULL && m_caster->IsPlayer())
				TO_PLAYER(m_caster)->SetTriggerStunOrImmobilize(29842,100);//fixed 100% chance
		}break;
	case 740:
	case 8918:
	case 9862:
	case 9863:
	case 21791:
	case 25817:
	case 26983:
	case 34550:		// Tranquility
		{
			//uint32 duration = GetDuration();
			//printf("moo\n");
			if(apply)
				sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1, (uint32)mod->m_amount, EVENT_AURA_PERIODIC_HEAL, 2000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			else
				sEventMgr.RemoveEvents(this, EVENT_AURA_PERIODIC_HEAL);
		}break;

	case 16914:
	case 17401:
	case 17402:
	case 27012:		// hurricane
		{
			if(apply)
				sEventMgr.AddEvent(this, &Aura::EventPeriodicDamage, (uint32)mod->m_amount, EVENT_AURA_PERIODIC_DAMAGE, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			else
				sEventMgr.RemoveEvents(this, EVENT_AURA_PERIODIC_DAMAGE);
		}break;

	case 33763:
	case 48450:
	case 48451:// lifebloom
		{

			if( apply || stackSize != 0 )	// don't activate if we have not removed aura completely
				return;

			Unit * pCaster = NULL;
			if( m_caster == NULL )
				pCaster = m_target;
			else
				pCaster = m_caster;

			// this is an ugly hack because i don't want to copy/paste code ;P
			Spell* spell = NULL;
			spell = new Spell(pCaster, m_spellProto, true, NULL);
			spell->SetUnitTarget( m_target );
			spell->Heal( mod->m_baseAmount );			
			spell->Destructor();
			//pCaster->Heal( m_target, m_spellProto->Id, mod->m_amount );

			// 
		}break;


	case 2584:			// Area spirit healer aura for BG's
		{
			if( !m_target->IsPlayer() || apply )		// already applied in opcode handler
				return;

			uint64 crtguid = TO_PLAYER(m_target)->m_areaSpiritHealer_guid;
			Creature* pCreature = TO_PLAYER(m_target)->IsInWorld() ? TO_PLAYER(m_target)->GetMapMgr()->GetCreature(GET_LOWGUID_PART(crtguid)) : NULL;
			if(pCreature==NULL || TO_PLAYER(m_target)->m_bg==NULL)
				return;

			TO_PLAYER(m_target)->m_bg->RemovePlayerFromResurrect(TO_PLAYER(m_target),pCreature);
		}break;

	case 17007: //Druid:Leader of the Pack
		{
			if( !m_target->IsPlayer() )
				return;

			if( apply )
				TO_PLAYER(m_target)->AddShapeShiftSpell( 24932 );
			else
				TO_PLAYER(m_target)->RemoveShapeShiftSpell( 24932 );
		}break;

	case 48411: //Master ShapeShifter
	case 48412:
		{
			if( !m_target->IsPlayer() )
				return;
			if( apply )
				{
				TO_PLAYER(m_target)->AddShapeShiftSpell( 48418 );
				TO_PLAYER(m_target)->AddShapeShiftSpell( 48420 );
				TO_PLAYER(m_target)->AddShapeShiftSpell( 48421 );
				TO_PLAYER(m_target)->AddShapeShiftSpell( 48422 );
			}
			else
			{
				TO_PLAYER(m_target)->RemoveShapeShiftSpell( 48418 );
				TO_PLAYER(m_target)->RemoveShapeShiftSpell( 48420 );
				TO_PLAYER(m_target)->RemoveShapeShiftSpell( 48421 );
				TO_PLAYER(m_target)->RemoveShapeShiftSpell( 48422 );
				}
		}break;

	case 17804: // Warlock: Soul Siphon
	case 17805:
		{
			Unit* caster = GetUnitCaster();
			if(caster) 
			{
				if( apply )
					caster->m_soulSiphon.amt+= mod->m_amount;
				else
					caster->m_soulSiphon.amt-= mod->m_amount;
			}
		}break;
	case 53754: // Warlock: Improved Fear
		{
			if(m_caster != NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_FEAR, 60946, 100, false);
				else
					m_caster->RemoveOnAuraRemoveSpell(SPELL_HASH_FEAR);
			}
		}break;
	case 53759:
		{
			if(m_caster!=NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_FEAR, 60947, 100, false);
				else
					m_caster->RemoveOnAuraRemoveSpell(SPELL_HASH_FEAR);
			}
		}break;
	case 44745: // Mage: Shattered Barrier
		{
			if(m_caster!=NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_ICE_BARRIER, 55080, 50, true);
				else
					m_caster->RemoveOnAuraRemoveSpell(SPELL_HASH_ICE_BARRIER);
			}
		}break;
	case 54787:
		{
			if(m_caster!=NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_ICE_BARRIER, 55080, 100, true);
				else
					m_caster->RemoveOnAuraRemoveSpell(SPELL_HASH_ICE_BARRIER);
			}
		}break;
	case 44394:// Mage: Incanter's Absorption
	case 44395:
	case 44396:
		{
			if(m_caster!=NULL)
			{
				if (apply)
					m_caster->m_incanterAbsorption = mod->m_amount;
				else
					m_caster->m_incanterAbsorption = 0;
			}
		}break;
	case 44457:// Mage: Living Bomb
	case 55359:
	case 55360:
		{
			if(m_caster!=NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_LIVING_BOMB, mod->m_amount, 100, true);
			}
		}break;
	case 47571:// Priest: Psychic Horror
		{
			if(m_caster!=NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_PSYCHIC_SCREAM, 59980, 100, false);
				else
					m_caster->RemoveOnAuraRemoveSpell(SPELL_HASH_PSYCHIC_SCREAM);
			}
		}break;
	case 47572:
		{
			if(m_caster!=NULL)
			{
				if( apply )
					m_caster->AddOnAuraRemoveSpell(SPELL_HASH_PSYCHIC_SCREAM, 59981, 100, false);
				else
					m_caster->RemoveOnAuraRemoveSpell(SPELL_HASH_PSYCHIC_SCREAM);
			}
		}break;
	default:
		{
			dummy_aura = true;
		}break;
	}
	//we still should handle this
	if( m_target && dummy_aura )
	{
		dummy_aura = false;
		switch( GetSpellProto()->NameHash )
		{
		case SPELL_HASH_SURVIVAL_INSTINCTS:
			{
				if( m_target->IsPlayer() )
				{
					uint32 shape = TO_PLAYER( m_target )->GetShapeShift();
					if( shape == FORM_CAT || shape == FORM_BEAR || shape == FORM_DIREBEAR )
					{
						if( apply )
						{
							mod->fixed_float_amount[0] = m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.3f;
							TO_PLAYER( m_target )->SetHealthFromSpell( TO_PLAYER( m_target )->GetHealthFromSpell() + mod->fixed_float_amount[0] );
							TO_PLAYER( m_target )->UpdateStats();
						}
						else
						{
							TO_PLAYER( m_target )->SetHealthFromSpell( TO_PLAYER( m_target )->GetHealthFromSpell() - mod->fixed_float_amount[0] );
							TO_PLAYER( m_target )->UpdateStats();
						}
					}
				}
			}break;
		case SPELL_HASH_BEACON_OF_LIGHT:
			{//mark all targets inrange
				if( m_caster == NULL )
					return;

				float range = GetMaxRange( dbcSpellRange.LookupEntry( m_spellProto->rangeIndex ) );
				float r = range*range;
				for( unordered_set<Object*>::iterator itr = m_target->GetInRangeSetBegin(); itr != m_target->GetInRangeSetEnd(); itr++ )
				{
					if( !(*itr)->IsPlayer() )
						continue;

					if( IsInrange( m_target->GetPositionX(), m_target->GetPositionY(), m_target->GetPositionZ(), (*itr), r ) )
						if( !isAttackable( m_target,TO_UNIT( *itr ) ) )
						{
							if( apply )
							{
								TO_UNIT( *itr )->BeaconCaster = m_caster;
								TO_UNIT( *itr )->BeaconTarget = m_target;
								//just in case
								sEventMgr.AddEvent( TO_UNIT( *itr ), &Unit::RemoveBeacons, EVENT_BEACON_REMOVE, 60*1000,1,0);
							}else
							{
								TO_UNIT( *itr )->BeaconCaster = NULL;
								TO_UNIT( *itr )->BeaconTarget = NULL;
							}
						}
				}
			}break;
		case SPELL_HASH_EXHAUSTION:
			{
				if( m_target->IsPlayer() )
				{
					TO_PLAYER(m_target)->mExhaustion = apply;
					SetNegative();
				}
			}break;
		case SPELL_HASH_SATED:
			{
				if( m_target->IsPlayer() )
				{
					TO_PLAYER(m_target)->mSated = apply;
					SetNegative();
				}
			}break;
		default:
			{
				dummy_aura = true;
			}break;
		}
	}

	if( dummy_aura )
	{
		if( apply )
			m_target->AddDummyAura( m_spellProto );
		else
			m_target->RemoveDummyAura( m_spellProto->NameHash );
	}

	if ( TamingSpellid && ! GetTimeLeft() )
	{
		// Creates a 15 minute pet, if player has the quest that goes with the spell and if target corresponds to quest
		if( m_caster == NULL || !m_caster->IsPlayer() )
			return;

		SpellEntry *triggerspell = dbcSpell.LookupEntry( TamingSpellid );
		Quest* tamequest = QuestStorage.LookupEntry( triggerspell->EffectMiscValue[1] );
		if ( TO_PLAYER(m_caster)->GetQuestLogForEntry(tamequest->id )&& m_target->GetEntry() == tamequest->required_mob[0] )
		{	
			if( Rand( 75.0f ) )// 75% chance on success
			{
				Creature* tamed = NULL;
				tamed = ( ( m_target->GetTypeId() == TYPEID_UNIT ) ? TO_CREATURE(m_target ) : NULL );
				QuestLogEntry *qle = TO_PLAYER(m_caster)->GetQuestLogForEntry(tamequest->id );

				if( tamed == NULL || qle == NULL )
					return;

				tamed->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, m_caster, 0 );
				Pet* pPet = objmgr.CreatePet();
				pPet->SetInstanceID( m_caster->GetInstanceID() );
				pPet->CreateAsSummon( tamed->GetEntry(), tamed->GetCreatureInfo(), tamed, m_caster, triggerspell, 2, 900000 );
				pPet->CastSpell( tamed, triggerspell, false );
				tamed->SafeDelete();
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill( 1 );
				qle->UpdatePlayerFields();
				qle->SendQuestComplete();
			}
			else
			{
				TO_PLAYER(m_caster)->SendCastResult( triggerspell->Id,SPELL_FAILED_TRY_AGAIN,0,0 );
			}
		}
		else
		{
			TO_PLAYER(m_caster)->SendCastResult( triggerspell->Id,SPELL_FAILED_BAD_TARGETS,0,0 );
		}
		TamingSpellid = 0;
	}
}

void Aura::SpellAuraModConfuse(bool apply)
{   
	if( m_target->GetTypeId() == TYPEID_UNIT && TO_CREATURE(m_target)->IsTotem() )
		return;

	Unit * m_caster = GetUnitCaster();
	if(apply)
	{
		if( m_caster == NULL )
			return;

		SetNegative();
		
		m_target->m_special_state |= UNIT_STATE_CONFUSE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);

		m_target->EnableAI();
		m_target->GetAIInterface()->HandleEvent(EVENT_WANDER, m_caster, 0);

		if(m_target->IsPlayer())
		{
			// this is a hackfix to stop player from moving -> see AIInterface::_UpdateMovement() Wander AI for more info
			WorldPacket data1(9);
			data1.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data1 << m_target->GetNewGUID() << uint8(0x00);
			TO_PLAYER(m_target)->GetSession()->SendPacket(&data1);
			TO_PLAYER(m_target)->DelaySpeedHack( GetDuration() );
		}
	}
	else
	{
		m_target->m_special_state &= ~UNIT_STATE_CONFUSE;
		m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);

		m_target->GetAIInterface()->HandleEvent( EVENT_UNWANDER, NULL, 0 );

		if(m_target->IsPlayer())
		{
			// re-enable movement
			WorldPacket data1(9);
			data1.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data1 << m_target->GetNewGUID() << uint8(0x01);
			TO_PLAYER(m_target)->GetSession()->SendPacket(&data1);

			m_target->DisableAI();

			if( m_caster != NULL )
				sHookInterface.OnEnterCombat( TO_PLAYER(m_target), m_caster );
		}
		else
			m_target->GetAIInterface()->AttackReaction(m_caster, 1, 0);
	}
}

void Aura::SpellAuraModCharm(bool apply)
{
	Creature* target = m_target != NULL ? TO_CREATURE( m_target ): NULL;
  
	SetPositive(3); //we ignore the other 2 effect of this spell and force it to be a positive spell

	if( m_target == NULL || m_target->GetTypeId() != TYPEID_UNIT )
		return;

	if( TO_CREATURE( m_target )->IsTotem() )
		return;

	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL || !m_caster->IsPlayer() )
		return;

	if( apply )
	{
		if( (int32)m_target->getLevel() > mod->m_amount || m_target->IsPet() )
			return;

		// this should be done properly
		if( target->GetEnslaveCount() >= 10 )
			return;

		if( m_caster->GetUInt64Value( UNIT_FIELD_CHARM ) != 0 )
			return;

		m_target->m_special_state |= UNIT_STATE_CHARM;
		m_target->SetCharmTempVal( m_target->GetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE ) );
		m_target->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, m_caster->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		m_target->_setFaction();
		m_target->UpdateOppFactionSet();
		m_target->GetAIInterface()->Init(m_target, AITYPE_PET, MOVEMENTTYPE_NONE, m_caster);
		m_target->SetUInt64Value(UNIT_FIELD_CHARMEDBY, m_caster->GetGUID());
		m_caster->SetUInt64Value(UNIT_FIELD_CHARM, target->GetGUID());
		//damn it, the other effects of enslaive demon will agro him on us anyway :S
		m_target->GetAIInterface()->WipeHateList();
		m_target->GetAIInterface()->WipeTargetList();
		m_target->GetAIInterface()->SetNextTarget(NULL);

		target->SetEnslaveCount(target->GetEnslaveCount() + 1);

		if( TO_PLAYER(m_caster)->GetSession() != NULL ) // crashfix
		{
			WorldPacket data(SMSG_PET_SPELLS, 500);
			data << target->GetGUID();
			data << uint16(0) << uint32(0) << uint32(0x1000);
			data << uint32(PET_SPELL_ATTACK);
			data << uint32(PET_SPELL_FOLLOW);
			data << uint32(PET_SPELL_STAY);
			for(int i = 0; i < 4; i++)
				data << uint32(0);
			data << uint32(PET_SPELL_AGRESSIVE);
			data << uint32(PET_SPELL_DEFENSIVE);
			data << uint32(PET_SPELL_PASSIVE);
			TO_PLAYER(m_caster)->GetSession()->SendPacket(&data);
			target->SetEnslaveSpell(m_spellProto->Id);
		}
	}
	else
	{
		m_target->m_special_state &= ~UNIT_STATE_CHARM;
		m_target->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, m_target->GetCharmTempVal() );
		m_target->_setFaction();
		m_target->GetAIInterface()->WipeHateList();
		m_target->GetAIInterface()->WipeTargetList();
		m_target->UpdateOppFactionSet();
		m_target->GetAIInterface()->Init(m_target, AITYPE_AGRO, MOVEMENTTYPE_NONE);
		m_target->SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

		if( TO_PLAYER(m_caster)->GetSession() != NULL ) // crashfix
		{
			m_caster->SetUInt64Value(UNIT_FIELD_CHARM, 0);
			WorldPacket data(SMSG_PET_SPELLS, 8);
			data << uint64(0);
			TO_PLAYER(m_caster)->GetSession()->SendPacket(&data);
			target->SetEnslaveSpell(0);
		}
	}
}

void Aura::SpellAuraModFear(bool apply)
{
	if( m_target->GetTypeId() == TYPEID_UNIT && TO_CREATURE(m_target)->IsTotem() )
		return;

	Unit * m_caster = GetUnitCaster();
	if(apply)
	{
		if( m_caster == NULL )
			return;

		SetNegative();

		m_target->m_special_state |= UNIT_STATE_FEAR;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);

		m_target->EnableAI();
		m_target->GetAIInterface()->HandleEvent(EVENT_FEAR, m_caster, 0);

		m_target->m_fearmodifiers++;
		if(m_target->IsPlayer())
		{
			// this is a hackfix to stop player from moving -> see AIInterface::_UpdateMovement() Fear AI for more info
			WorldPacket data1(9);
			data1.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
			data1 << m_target->GetNewGUID() << uint8(0x00);
			TO_PLAYER(m_target)->GetSession()->SendPacket(&data1);
			TO_PLAYER(m_target)->DelaySpeedHack( GetDuration() + 2000 );
		}
	}
	else
	{
		m_target->m_fearmodifiers--;

		if(m_target->m_fearmodifiers <= 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_FEAR;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);

			m_target->GetAIInterface()->HandleEvent( EVENT_UNFEAR, NULL, 0 );

			if(m_target->IsPlayer())
			{
				// re-enable movement
				WorldPacket data1(9);
				data1.Initialize(SMSG_CLIENT_CONTROL_UPDATE);
				data1 << m_target->GetNewGUID() << uint8(0x01);
				TO_PLAYER(m_target)->GetSession()->SendPacket(&data1);

				m_target->DisableAI();

				if( m_caster != NULL )
					sHookInterface.OnEnterCombat( TO_PLAYER(m_target), m_caster );
			}
			else
			{
				if( m_caster != NULL )
					m_target->GetAIInterface()->AttackReaction(m_caster, 1, 0);
			}
		}
	}
}

void Aura::SpellAuraPeriodicHeal( bool apply )
{
	if( apply )
	{
		SetPositive();
		int32 amount = mod->m_amount;
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL)
			amount = m_caster->GetSpellBonusDamage(m_target, m_spellProto, amount, true, true);

		if (amount <= 0) return;

		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent( this, &Aura::EventPeriodicHeal,(uint32)amount, EVENT_AURA_PERIODIC_HEAL, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

		if( GetSpellProto()->NameHash == SPELL_HASH_REJUVENATION || GetSpellProto()->NameHash == SPELL_HASH_REGROWTH )
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_REJUVENATE);	
			if(!sEventMgr.HasEvent( m_target, EVENT_REJUVENATION_FLAG_EXPIRE ) )
			{
				sEventMgr.AddEvent( m_target, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_REJUVENATE, EVENT_REJUVENATION_FLAG_EXPIRE, GetSpellProto()->EffectAmplitude[mod->i], 1, 0 );
			}
			else
			{
				sEventMgr.ModifyEventTimeLeft( m_target,EVENT_REJUVENATION_FLAG_EXPIRE, GetSpellProto()->EffectAmplitude[mod->i], 0 );
			}
		}
	}
}

void Aura::EventPeriodicHeal( uint32 amount )
{
	if(m_target == NULL || m_target->GetMapMgr() == NULL )
		return;

	if( !m_target->isAlive() )
		return;
	
	int32 add = amount;

	if((m_target->GetUInt32Value( UNIT_FIELD_HEALTH ) + add) > m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH ))
	{
		add = m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) - m_target->GetUInt32Value( UNIT_FIELD_HEALTH );
	}
	uint32 newHealth = m_target->GetUInt32Value( UNIT_FIELD_HEALTH ) + (uint32)add;
	
	m_target->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth );

	SendPeriodicAuraLog( add, FLAG_PERIODIC_HEAL );

	if( GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP )
	{
		m_target->Emote( EMOTE_ONESHOT_EAT );
	}

	// add threat
	Unit * m_caster = GetUnitCaster();
	if( m_caster != NULL )
	{
		add = m_caster->GetSpellBonusDamage(m_target, m_spellProto, add, true, true);
		if (GetSpellProto()->NameHash == SPELL_HASH_HEALTH_FUNNEL && add > 0)
		{
			dealdamage sdmg;

			sdmg.full_damage = add;
			sdmg.resisted_damage = 0;
			sdmg.school_type = 0;
			m_caster->DealDamage(m_caster, add, 0, 0, 0);
			m_caster->SendAttackerStateUpdate(m_caster, &sdmg, add, 0, 0, 0, ATTACK);
		}

		if(m_target && m_target->IsInWorld() && m_caster != NULL && m_caster->IsInWorld())
			m_caster->CombatStatus.WeHealed(m_target);

		uint32 base_threat=Spell::GetBaseThreat(add);
		int count = 0;
		Unit* unit = NULL;
		std::vector< Unit* > target_threat;
		if( base_threat > 0 )
		{
			target_threat.reserve(m_caster->GetInRangeCount()); // this helps speed

			for(unordered_set<Object* >::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); ++itr)
			{
				if((*itr)->GetTypeId() != TYPEID_UNIT)
					continue;
				unit = TO_UNIT((*itr));
				if(unit->GetAIInterface()->GetNextTarget() == m_target)
				{
					target_threat.push_back(unit);
					++count;
				}
			}
			if(count == 0)
				count = 1;  // division against 0 protection
			/* 
			When a tank hold multiple mobs, the threat of a heal on the tank will be split between all the mobs.
			The exact formula is not yet known, but it is more than the Threat/number of mobs.
			So if a tank holds 5 mobs and receives a heal, the threat on each mob will be less than Threat(heal)/5.
			Current speculation is Threat(heal)/(num of mobs *2)
			*/
			uint32 threat = base_threat / (count * 2);

			for(std::vector<Unit* >::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
			{
				// for now we'll just use heal amount as threat.. we'll prolly need a formula though
				(TO_UNIT(*itr))->GetAIInterface()->HealReaction(m_caster, m_target, threat, m_spellProto);
			}
		}
		m_caster->HandleProc(PROC_ON_SPELL_LAND, m_target, m_spellProto, add);
		m_caster->m_procCounter = 0;

		m_target->HandleProc(PROC_ON_SPELL_LAND_VICTIM,m_caster, m_spellProto, add);
		m_target->m_procCounter = 0;
	}
}

void Aura::SpellAuraModAttackSpeed(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else 
		SetPositive();
 
	float baseMod = mod->m_amount / 100.0f;

	if (m_target->IsPlayer())
	{
		if(apply)
		{
			mod->fixed_float_amount[0] = baseMod * TO_PLAYER(m_target)->m_meleeattackspeedmod;
			mod->fixed_float_amount[1] = baseMod * TO_PLAYER(m_target)->m_rangedattackspeedmod;
			TO_PLAYER( m_target )->m_meleeattackspeedmod -= mod->fixed_float_amount[0];
			TO_PLAYER( m_target )->m_rangedattackspeedmod -= mod->fixed_float_amount[1];
		}
		else
		{
			TO_PLAYER( m_target )->m_meleeattackspeedmod += mod->fixed_float_amount[0];
			TO_PLAYER( m_target )->m_rangedattackspeedmod += mod->fixed_float_amount[1];
		}
		TO_PLAYER( m_target )->UpdateStats();
	}
	else
	{
		if(apply)
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME,mod->m_amount);
			mod->fixed_amount[1] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME + 1,mod->m_amount);
			mod->fixed_amount[2] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,mod->m_amount);
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME, -mod->fixed_amount[0]);
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME + 1, -mod->fixed_amount[1]);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[2]);
		}
		else
		{
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME, mod->fixed_amount[0]);
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME + 1, mod->fixed_amount[1]);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[2]);
		}
	}
	 
}

void Aura::SpellAuraModThreatGenerated(bool apply)
{
	if(!m_target)
		return;

	//shaman spell 30672 needs to be based on spell schools
	if(m_target->GetGeneratedThreatModifier() == mod->m_amount)
	{
		mod->m_amount < 0 ? SetPositive() : SetNegative();
		apply ? m_target->ModGeneratedThreatModifier(mod->m_amount) : m_target->ModGeneratedThreatModifier(-(mod->m_amount));
		return;
	}
	else // if we this aura is better then the old one, replace the effect.
	{
		if(apply)
		{
			if(m_target->GetGeneratedThreatModifier() < mod->m_amount)
			{
				m_target->ModGeneratedThreatModifier(-m_target->GetGeneratedThreatModifier());
				m_target->ModGeneratedThreatModifier(mod->m_amount);
			}
		}
	}
}

void Aura::SpellAuraModTaunt(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL || !m_caster->isAlive())
		return;

	SetNegative();

	// NEWBS, TAUNT DOES NOT EFFECT PLAYERS!1oneoneeleven
	if(m_target->IsPlayer())
		return;

	if(apply)
	{
		m_target->GetAIInterface()->AttackReaction(m_caster, 1, 0);
		m_target->GetAIInterface()->taunt(m_caster, true);
	}
	else if(m_target->GetAIInterface()->getTauntedBy() == m_caster)
		m_target->GetAIInterface()->taunt(m_caster, false);

}

void Aura::SpellAuraModStun(bool apply)
{
	if( m_target == NULL)
		return;

	if( m_spellProto->Id == 38554 ) // Absorb Eye of Grillok, freezing trap hax
		return;

	if(apply)
	{ 
		SetNegative();

		m_target->m_rooted++;

		if(m_target->m_rooted == 1)
			m_target->Root();

		m_target->m_stunned++;
		m_target->m_special_state |= UNIT_STATE_STUN;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
		m_target->SetFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_STUNNED);

		if(m_target->GetTypeId() == TYPEID_UNIT)
			m_target->GetAIInterface()->SetNextTarget(NULL);

		// remove the current spell (for channelers)
		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid && 
			( m_target->m_currentSpell->getState() == SPELL_STATE_CASTING || m_target->m_currentSpell->getState() == SPELL_STATE_PREPARING ) )
		{
			m_target->m_currentSpell->cancel();
			m_target->m_currentSpell = NULL;
		}

		//warrior talent - second wind triggers on stun and immobilize. This is not used as proc to be triggered always !
		if(m_target->IsPlayer() && m_spellProto->MechanicsType != MECHANIC_INCAPACIPATED)
			TO_PLAYER(m_target)->EventStunOrImmobilize();
	}
	else
	{
		if( m_spellProto->NameHash == SPELL_HASH_WYVERN_STING )
		{
			Unit* caster = NULL;
			caster = GetUnitCaster();
			if( caster == NULL )
				return;

			uint32 wyvernsp = 0;
			switch(m_spellProto->Id)
			{
			case 49012:{wyvernsp = 49010;}break;
			case 49011:{wyvernsp = 49009;}break;
			case 27068:{wyvernsp = 27069;}break;
			case 24132:{wyvernsp = 24134;}break;
			default:{wyvernsp = 24131;}break;
			}
			SpellEntry * wyvspell = NULL;
			wyvspell = dbcSpell.LookupEntry(wyvernsp);
			if(wyvspell != NULL)
				sEventMgr.AddEvent(caster, &Unit::EventCastSpell, m_target, wyvspell, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); 
		}

		//targetdummies stay rooted
		if( m_target->IsCreature() && TO_CREATURE(m_target)->GetProto() && isTargetDummy(TO_CREATURE(m_target)->GetProto()->Id))
		{
			m_target->m_rooted = 1;
			m_target->m_stunned = 1;
			return;
		}

		m_target->m_rooted--;

		if(m_target->m_rooted == 0)
			m_target->UnRoot();

		m_target->m_stunned--;

		if(m_target->m_stunned == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_STUN;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
			m_target->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_STUNNED);
		}

		// attack them back.. we seem to lose this sometimes for some reason
		if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			Unit* target = NULL;
			if(m_target->GetAIInterface()->GetNextTarget() != 0)
				target = m_target->GetAIInterface()->GetNextTarget();
			else
				target = GetUnitCaster();

			if(target != NULL ) 
				m_target->GetAIInterface()->AttackReaction(target, 1, 0);
		}
	}
}

void Aura::SpellAuraModDamageDone(bool apply)
{
	int32 val = 0;

	if( m_spellProto->NameHash == SPELL_HASH_DIVINE_SPIRIT ||
		m_spellProto->NameHash == SPELL_HASH_PRAYER_OF_SPIRIT )
	{
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL && m_caster->HasDummyAura(SPELL_HASH_IMPROVED_DIVINE_SPIRIT) )
		{
			val += m_spellProto->EffectBasePoints[0] / m_caster->GetDummyAura(SPELL_HASH_IMPROVED_DIVINE_SPIRIT)->RankNumber == 1 ? 2 : 1;
			if( !apply ) val = -val;
		}
	}

	if( m_target->IsPlayer() )
	{
		uint32 index;
		 
		if( mod->m_amount > 0 )
		{
			if( apply )
			{
				SetPositive();
				val += mod->m_amount;
			}
			else
			{
				val += -mod->m_amount;
			}
			index = PLAYER_FIELD_MOD_DAMAGE_DONE_POS;
		
		}
		else
		{
			if( apply )
			{
				SetNegative();
				val += -mod->m_amount;
			}
			else
			{
				val += mod->m_amount;
			}
			index = PLAYER_FIELD_MOD_DAMAGE_DONE_NEG;
		}

		for( uint32 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & ( ( (uint32)1 ) << x ) )
			{
				m_target->ModUnsigned32Value( index + x, val );
			}
		}
	}
	else if( m_target->GetTypeId() == TYPEID_UNIT )
	{
		if( mod->m_amount > 0 )
		{
			if( apply )
			{
				SetPositive();
				val += mod->m_amount;
			}
			else
			{
				val += -mod->m_amount;
			}

		}
		else
		{
			if( apply )
			{
				SetNegative();
				val += mod->m_amount;
			}
			else
			{
				val += -mod->m_amount;
			}
		}

		for( uint32 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & ( ( (uint32)1 ) << x ) )
			{
				if(m_target->IsCreature())
					TO_CREATURE(m_target)->ModDamageDone[x] += val;
			}
		}
	}
   
	if( mod->m_miscValue & 1 )
		m_target->CalcDamage();
}

void Aura::SpellAuraModDamageTaken(bool apply)
{
	int32 val = (apply) ? mod->m_amount : -mod->m_amount;
	for(uint32 x=0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->DamageTakenMod[x]+=val;
		}
	}
}

void Aura::SpellAuraDamageShield(bool apply)
{
	if(apply)
	{
		SetPositive();
		DamageProc ds;// = new DamageShield();
		ds.m_damage = mod->m_amount;
		ds.m_spellId = GetSpellProto()->Id;
		ds.m_school = GetSpellProto()->School;
		ds.m_flags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_MISC; //maybe later we might want to add other flags too here
		ds.owner = (void*)this;
		m_target->m_damageShields.push_back(ds);
	}
	else
	{
		for(std::list<struct DamageProc>::iterator i = m_target->m_damageShields.begin();i != m_target->m_damageShields.end();i++)
		{
			if(i->owner==this)
			{
				 m_target->m_damageShields.erase(i);
				 return;
			}
		}
	}
}

void Aura::SpellAuraModStealth(bool apply)
{
	if(apply)
	{

		if(m_target->IsPlayer() && TO_PLAYER(m_target)->m_bg != NULL && TO_PLAYER(m_target)->m_bgHasFlag )
		{
			switch( TO_PLAYER(m_target)->m_bg->GetType())
			{
			case BATTLEGROUND_WARSONG_GULCH:
				TO_WARSONGGULCH(TO_PLAYER(m_target)->m_bg)->DropFlag(TO_PLAYER(m_target));
				break;
			case BATTLEGROUND_EYE_OF_THE_STORM:
				TO_EYEOFTHESTORM(TO_PLAYER(m_target)->m_bg)->DropFlag(TO_PLAYER(m_target));
				break;
			}
		}

		SetPositive();
		m_target->SetStealth(GetSpellId());

		// Stealth level (not for normal stealth... ;p)
		if( m_spellProto->NameHash == SPELL_HASH_STEALTH )
			m_target->SetFlag(UNIT_FIELD_BYTES_2,0x1E000000);//sneak anim
		
		m_target->SetFlag(UNIT_FIELD_BYTES_1, 0x020000);

		if( m_target->IsPlayer() )
			m_target->SetFlag(PLAYER_FIELD_BYTES2, 0x2000);

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_STEALTH);

		if( m_target->HasDummyAura(SPELL_HASH_OVERKILL) )
			m_target->CastSpell(m_target, 58427, true);
	}
	else 
	{
		m_target->SetStealth(0);
		m_target->RemoveFlag(UNIT_FIELD_BYTES_2,0x1E000000);
		m_target->RemoveFlag(UNIT_FIELD_BYTES_1, 0x020000);

		if( m_target->IsPlayer() )
		{
			m_target->RemoveFlag(PLAYER_FIELD_BYTES2, 0x2000);

			packetSMSG_COOLDOWN_EVENT cd;
			cd.guid = m_target->GetGUID();
			cd.spellid = m_spellProto->Id;
			TO_PLAYER(m_target)->GetSession()->OutPacket( SMSG_COOLDOWN_EVENT, sizeof(packetSMSG_COOLDOWN_EVENT), &cd);
		}


		if( (m_target->HasDummyAura(SPELL_HASH_MASTER_OF_SUBTLETY) || m_target->HasDummyAura(SPELL_HASH_OVERKILL)) && m_spellProto->NameHash == SPELL_HASH_STEALTH )
		{
			for( uint32 x=0 ; x<MAX_POSITIVE_AURAS; x++ )
			{
				if( m_target->m_auras[x] != NULL&& 
						(m_target->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_MASTER_OF_SUBTLETY ||
						 m_target->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_OVERKILL) && 
						m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[0] != SPELL_AURA_DUMMY )
				{
						m_target->m_auras[x]->SetDuration(6000);
						m_target->m_auras[x]->SetTimeLeft(6000);

						sEventMgr.AddEvent(m_target, &Unit::RemoveAuraBySlot, uint16(x), EVENT_AURA_REMOVE, 6000, 1,
											EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
				}
			}
		}
	}
	m_target->UpdateVisibility();
}

void Aura::SpellAuraModDetect(bool apply)
{
	if(apply)
	{
		//SetPositive();
		m_target->m_stealthDetectBonus += mod->m_amount;
	}
	else 
		m_target->m_stealthDetectBonus -= mod->m_amount;
}

void Aura::SpellAuraModInvisibility(bool apply)
{

	if(m_target->IsPlayer())
	{
		if(apply)
			TO_PLAYER(m_target)->m_bgFlagIneligible++;
		else
			TO_PLAYER(m_target)->m_bgFlagIneligible--;

		if(TO_PLAYER(m_target)->m_bg != NULL && TO_PLAYER(m_target)->m_bgHasFlag)
			TO_PLAYER(m_target)->m_bg->HookOnMount(TO_PLAYER(m_target));
	}

	SetPositive();
	if(m_spellProto->Effect[mod->i] == 128)
		return;

	if(apply) 
	{
		m_target->SetInvisibility(GetSpellId());
		m_target->m_invisFlag = mod->m_miscValue;
	} 
	else 
	{
		m_target->SetInvisibility(0);
		m_target->m_invisFlag = INVIS_FLAG_NORMAL;
	}

	m_target->m_invisible = apply;
	m_target->UpdateVisibility();
}

void Aura::SpellAuraModInvisibilityDetection(bool apply)
{
	//Always Positive

	assert(mod->m_miscValue < INVIS_FLAG_TOTAL);
	if(apply)
	{
		m_target->m_invisDetect[mod->m_miscValue] += mod->m_amount;
		SetPositive ();
	}
	else 
		m_target->m_invisDetect[mod->m_miscValue] -= mod->m_amount;

	if(m_target->IsPlayer())
		TO_PLAYER( m_target )->UpdateVisibility();
}

void Aura::SpellAuraModTotalHealthRegenPct(bool apply)
{
	if(apply)
	{
		SetPositive();
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent(this, &Aura::EventPeriodicHealPct, (float)mod->m_amount,
			EVENT_AURA_PERIODIC_HEALPERC, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
		TO_UNIT( m_target )->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
}

void Aura::EventPeriodicHealPct(float RegenPct)
{
	if(!m_target->isAlive())
		return;

	uint32 add = float2int32(m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * (RegenPct / 100.0f));

	uint32 newHealth = m_target->GetUInt32Value(UNIT_FIELD_HEALTH) + add;

	if(newHealth <= m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
	else
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	SendPeriodicAuraLog(m_casterGuid, m_target, m_spellProto, add, 0, 0, FLAG_PERIODIC_HEAL);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
}

void Aura::SpellAuraModTotalManaRegenPct(bool apply)
{
	if(apply)
	{
		SetPositive();
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent(this, &Aura::EventPeriodicManaPct, (float)mod->m_amount,
			EVENT_AURA_PERIOCIC_MANA, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicManaPct(float RegenPct)
{
	if(!m_target->isAlive())
		return;

	uint32 add = float2int32(m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1) * (RegenPct / 100.0f));

	uint32 newHealth = m_target->GetUInt32Value(UNIT_FIELD_POWER1) + add;

	if(newHealth <= m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
		m_target->SetUInt32Value(UNIT_FIELD_POWER1, newHealth);
	else
		m_target->SetUInt32Value(UNIT_FIELD_POWER1, m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1));

	// CAPT 
	// TODO: sniff it or disasm wow.exe to find the mana flag
	//SendPeriodicAuraLog(m_target, m_casterGuid, GetSpellProto()->Id, FLAG_PERIODIC_HEAL, add,true);
	//SendPeriodicAuraLog(m_target, m_casterGuid, GetSpellProto()->Id, FLAG_PERIODIC_HEAL, add);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}

	m_target->SendPowerUpdate();
}

void Aura::SpellAuraModResistance(bool apply)
{
	uint32 Flag = mod->m_miscValue; 
	int32 amt;
	if(apply)
	{
		amt = mod->m_amount;
		if(amt <0 )//dont' change it
			SetNegative();
		else 
			SetPositive();
	}
	else 
		amt = -mod->m_amount;  

	if( this->GetSpellProto() && ( this->GetSpellProto()->NameHash == SPELL_HASH_FAERIE_FIRE || this->GetSpellProto()->NameHash == SPELL_HASH_FAERIE_FIRE__FERAL_ ) )
		m_target->m_can_stealth = !apply;
	
	if( m_target->IsPlayer() )
	{
		for( uint32 x = 0; x < 7; x++ )
		{
			if(Flag & (((uint32)1)<< x) )
			{
				if(mod->m_amount>0)
						TO_PLAYER( m_target )->FlatResistanceModifierPos[x]+=amt;
				else
						TO_PLAYER( m_target )->FlatResistanceModifierNeg[x]-=amt;
				TO_PLAYER( m_target )->CalcResistance(x);
			}
		}
	}
	else if(m_target->GetTypeId() == TYPEID_UNIT)
	{
		for(uint32 x=0;x<7;x++)
		{
			if(Flag & (((uint32)1)<<x))
			{
				TO_CREATURE(m_target)->FlatResistanceMod[x]+=amt;
				TO_CREATURE(m_target)->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraPeriodicTriggerSpell(bool apply)
{
	if(m_spellProto->EffectTriggerSpell[mod->i] == 0)
		return;

	Unit * m_caster = GetUnitCaster();
	if(IsPassive() && !(m_target && m_target->IsCreature() && TO_CREATURE(m_target)->IsTotem()) && m_spellProto->SpellIconID != 2010  && m_spellProto->SpellIconID != 2020 && m_spellProto->SpellIconID != 2255) //this spells are passive and are not done on the attack...
	{
		Unit* target = (m_target != 0) ? m_target : m_caster;
		if(target == 0 || !target->IsPlayer())
			return; //what about creatures ?

		SpellEntry *proto = dbcSpell.LookupEntry( m_spellProto->EffectTriggerSpell[mod->i] );

		if( apply )
			TO_PLAYER( target )->AddOnStrikeSpell( proto, m_spellProto->EffectAmplitude[mod->i] );
		else
			TO_PLAYER( target )->RemoveOnStrikeSpell( proto );

		return;
	}
	
	if(apply)
	{
		//FIXME: positive or negative?
		uint32 sp = GetSpellProto()->EffectTriggerSpell[mod->i];
		SpellEntry *spe = dbcSpell.LookupEntry(sp);
		if(!sp || !spe)
		{
			//	sp=22845;
			return;//null spell
		}

		if(m_caster == NULL)
			return;

		int32 amplitude = m_spellProto->EffectAmplitude[mod->i];
		if( m_spellProto->SpellGroupType )
		{
			SM_FIValue( m_caster->SM[SMT_TIME][0], (int32*)&amplitude, m_spellProto->SpellGroupType );
			SM_PIValue( m_caster->SM[SMT_TIME][1], (int32*)&amplitude, m_spellProto->SpellGroupType );
		}

		if(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
		{
			sEventMgr.AddEvent(this, &Aura::EventPeriodicTriggerSpell, spe,
			EVENT_AURA_PERIODIC_TRIGGERSPELL, amplitude, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

            periodic_target = m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT);
		}
		else if(m_target)
		{
			sEventMgr.AddEvent(this, &Aura::EventPeriodicTriggerSpell, spe, 
				EVENT_AURA_PERIODIC_TRIGGERSPELL, amplitude, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			periodic_target = m_target->GetGUID();
		}
	}
}

void Aura::EventPeriodicTriggerSpell(SpellEntry* spellInfo)
{
	// Trigger Spell
	// check for spell id

	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL || !m_caster->IsInWorld())
		return;

	if( spellInfo->EffectImplicitTargetA[0] == 18 )			// Hellfire, if there are any others insert here
	{
		Spell* spell = NULL;
		spell = (new Spell(m_caster, spellInfo, true, this));
		SpellCastTargets targets;
		targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION;
		targets.m_srcX = m_caster->GetPositionX();
		targets.m_srcY = m_caster->GetPositionY();
		targets.m_srcZ = m_caster->GetPositionZ();
		spell->prepare(&targets);
		return;
	}

	Object* oTarget = m_target->GetMapMgr()->_GetObject(periodic_target);
	if( oTarget == NULL || !oTarget->IsInWorld() )
		return;

	if(oTarget->GetTypeId()==TYPEID_DYNAMICOBJECT)
	{
		Spell* spell = NULL;
		spell = (new Spell(m_caster, spellInfo, true, this));
		SpellCastTargets targets;
		targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
		targets.m_destX = oTarget->GetPositionX();
		targets.m_destY = oTarget->GetPositionY();
		targets.m_destZ = oTarget->GetPositionZ();
		spell->prepare(&targets);
		return;
	}

	Unit* pTarget = TO_UNIT(oTarget);

	if(oTarget->IsUnit())
	{	
		if( !pTarget || pTarget->isDead() )
		{
			SendInterrupted(SPELL_FAILED_TARGETS_DEAD, m_caster);
			SendChannelUpdate(0, m_caster);
			Remove();
			return;
		}
		
		if( pTarget != m_caster )
		{
			if( spellInfo->c_is_flags & SPELL_FLAG_CASTED_ON_ENEMIES )
				if( !isAttackable(m_caster, pTarget, true) )
				{
					SendInterrupted(SPELL_FAILED_BAD_TARGETS, m_caster);
					SendChannelUpdate(0, m_caster);
					Remove();
					return;
				}
			else if( spellInfo->c_is_flags & SPELL_FLAG_CASTED_ON_FRIENDS )
				if( isAttackable(m_caster, pTarget, true) )
				{
					SendInterrupted(SPELL_FAILED_BAD_TARGETS, m_caster);
					SendChannelUpdate(0, m_caster);
					Remove();
					return;
				}
		}

		if(spellInfo->SpellIconID == 225 ) // this is arcane missles to avoid casting on self
			if(m_casterGuid == pTarget->GetGUID())
				return;
	}

	// set up our max Range
	float maxRange = GetMaxRange( dbcSpellRange.LookupEntry( spellInfo->rangeIndex ) );

	if( spellInfo->SpellGroupType )
	{
		SM_FFValue( m_caster->SM[SMT_RANGE][0], &maxRange, spellInfo->SpellGroupType );
		SM_PFValue( m_caster->SM[SMT_RANGE][1], &maxRange, spellInfo->SpellGroupType );
	}

	if( m_caster->IsStunned() || m_caster->IsFeared() || m_caster->GetDistance2dSq( oTarget ) > ( maxRange*maxRange ) )
	{
		// no longer valid
		SendInterrupted(SPELL_FAILED_INTERRUPTED, m_caster);
		SendChannelUpdate(0, m_caster);
		Remove();
		return;
	}

	Spell* spell = NULL;
	spell = (new Spell(m_caster, spellInfo, true, this));
	SpellCastTargets targets;
	if(oTarget->IsUnit())
		targets.m_targetMask = TARGET_FLAG_UNIT;
	else
		targets.m_targetMask = TARGET_FLAG_OBJECT;

	targets.m_unitTarget = oTarget->GetGUID();
	spell->prepare(&targets);
}

void Aura::SpellAuraPeriodicEnergize(bool apply)
{
	if(apply)
	{
		SetPositive();
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent(this, &Aura::EventPeriodicEnergize, (uint32)mod->m_amount, (uint32)mod->m_miscValue,
			EVENT_AURA_PERIODIC_ENERGIZE, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicEnergize(uint32 amount,uint32 type)
{
	uint32 curEnergy = m_target->GetUInt32Value(UNIT_FIELD_POWER1 + type);
	uint32 maxEnergy = m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1 + type);
	uint32 totalEnergy = curEnergy+amount;
	if(totalEnergy > maxEnergy)
		m_target->SetUInt32Value(UNIT_FIELD_POWER1 + type,maxEnergy);
	else
		m_target->SetUInt32Value(UNIT_FIELD_POWER1 + type,totalEnergy);
	
	SendPeriodicAuraLog( m_casterGuid, m_target, m_spellProto, amount, 0, 0, FLAG_PERIODIC_ENERGIZE);

	if((GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP) && type == 0)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
	m_target->SendPowerUpdate();
}

void Aura::SpellAuraModPacify(bool apply)
{
	// Can't Attack
	if( apply )
	{
		if( m_spellProto->Id == 24937 || m_spellProto->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION )
			SetPositive();
		else
			SetNegative();

		m_target->m_pacified++;
		m_target->m_special_state |= UNIT_STATE_PACIFY;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
	}
	else
	{
		m_target->m_pacified--;

		if(m_target->m_pacified == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_PACIFY;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
		}
	}
}

void Aura::SpellAuraModRoot(bool apply)
{
	if(apply)
	{
		SetNegative();

		m_target->m_rooted++;

		if(m_target->m_rooted == 1)
			m_target->Root();

		if((m_spellProto->School == SCHOOL_FROST && HasMechanic(MECHANIC_ROOTED) ) || HasMechanic(MECHANIC_FROZEN))
			m_target->SetFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN|0x400000);
			WorldPacket data(MSG_MOVE_ROOT, 9+7*4+1*2);
			data << m_target->GetNewGUID();
			data << uint16(0x0800);
			data << uint32(0);
			data << float(-0.0f);
			data << float(m_target->GetPositionX());
			data << float(m_target->GetPositionY());
			data << float(m_target->GetPositionZ());
			data << float(m_target->GetOrientation());
			data << float(0.0f);
			m_target->SendMessageToSet(&data,true);
		/* -Supalosa- TODO: Mobs will attack nearest enemy in range on aggro list when rooted. */
	}
	else
	{
		m_target->m_rooted--;

		if(m_target->m_rooted == 0)
			m_target->UnRoot();

		if((m_spellProto->School == SCHOOL_FROST && HasMechanic(MECHANIC_ROOTED) ) || HasMechanic(MECHANIC_FROZEN))
			m_target->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN|0x400000);
		WorldPacket data(MSG_MOVE_UNROOT, 9+7*4+1*2);
		data << m_target->GetNewGUID();
		data << uint16(0x1000);
		data << uint32(0);
		data << float(-0.0f);
		data << float(m_target->GetPositionX());
		data << float(m_target->GetPositionY());
		data << float(m_target->GetPositionZ());
		data << float(m_target->GetOrientation());
		data << float(0.0f);
		data << float(0.0f);
		data << float(cosf(m_target->GetOrientation()));
		data << float(sinf(m_target->GetOrientation()));
		data << float(0.0f);
		m_target->SendMessageToSet(&data,true);

		Unit * m_caster = GetUnitCaster();
		if(m_target->GetTypeId() == TYPEID_UNIT && m_caster != NULL)
			m_target->GetAIInterface()->AttackReaction(m_caster, 1, 0);

		if (m_spellProto->NameHash == SPELL_HASH_FROST_NOVA || m_spellProto->NameHash == SPELL_HASH_FROSTBITE)
			m_target->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN);
	}
}

void Aura::SpellAuraModSilence(bool apply)
{
	if(apply)
	{
		m_target->m_silenced++;
		m_target->m_special_state |= UNIT_STATE_SILENCE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);

		// remove the current spell (for channelers)
		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid && 
			m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
		{
			m_target->m_currentSpell->cancel();
			m_target->m_currentSpell = NULL;
		}
	}
	else
	{
		m_target->m_silenced--;

		if(m_target->m_silenced == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_SILENCE;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
		}
	}
}

void Aura::SpellAuraReflectSpells(bool apply)
{
	if(apply)
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
		ReflectSpellSchool *rss = NULL;
		rss = new ReflectSpellSchool;

		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->school = -1;
		rss->require_aura_hash = 0;
		rss->infront = false;
		rss->infinity = false;
		m_target->m_reflectSpellSchool.push_back(rss);
	}
	else
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete *i;
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
	}
}

void Aura::SpellAuraModStat(bool apply)
{
	int32 stat = (int32)mod->m_miscValue;
	int32 val;

	if(apply)
	{
		val = mod->m_amount;
		if (val<0)
			SetNegative();
		else
			SetPositive();
	}
	else 
	{
		val = -mod->m_amount;
	}

	if (stat == -1)//all stats
	{
		if(m_target->IsPlayer())
		{
			for(uint32 x=0;x<5;x++)
			{
				if(val>0)
					TO_PLAYER( m_target )->FlatStatModPos[x] += val; 
				else
					TO_PLAYER( m_target )->FlatStatModNeg[x] -= val;

				TO_PLAYER( m_target )->CalcStat(x);	
			}

			TO_PLAYER( m_target )->UpdateStats();
			TO_PLAYER( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			for(uint32 x=0;x<5;x++)
			{
				TO_CREATURE(m_target)->FlatStatMod[x] += val;
				TO_CREATURE(m_target)->CalcStat(x);			
			}
		}
	} 
	else if(stat >= 0)
	{
		ASSERT(mod->m_miscValue < 5);
		if(m_target->IsPlayer())
		{
			if(mod->m_amount>0)
				TO_PLAYER( m_target )->FlatStatModPos[mod->m_miscValue] += val; 
			else 
				TO_PLAYER( m_target )->FlatStatModNeg[mod->m_miscValue] -= val;

			TO_PLAYER( m_target )->CalcStat(mod->m_miscValue);	

			TO_PLAYER( m_target )->UpdateStats();
			TO_PLAYER( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			TO_CREATURE(m_target)->FlatStatMod[mod->m_miscValue]+=val;
			TO_CREATURE(m_target)->CalcStat(mod->m_miscValue);
		}
	}
}

void Aura::SpellAuraModSkill(bool apply)
{
	if (m_target->IsPlayer())
	{   
		if(apply)
		{
			SetPositive();
			TO_PLAYER( m_target )->_ModifySkillBonus(mod->m_miscValue, mod->m_amount); 
		}
		else
			TO_PLAYER( m_target )->_ModifySkillBonus(mod->m_miscValue, -mod->m_amount); 

		TO_PLAYER( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModIncreaseSpeed(bool apply)
{
	if(apply)
		m_target->m_speedModifier += mod->m_amount;
	else
		m_target->m_speedModifier -= mod->m_amount;

	m_target->UpdateSpeed();
}

void Aura::SpellAuraModIncreaseMountedSpeed(bool apply)
{
	if(apply)
	{ 
		SetPositive();
		m_target->m_mountedspeedModifier += mod->m_amount;
	}
	else
		m_target->m_mountedspeedModifier -= mod->m_amount;
	m_target->UpdateSpeed();
}

void Aura::SpellAuraModCreatureRangedAttackPower(bool apply)
{
	if(apply)
	{
		for(uint32 x = 0; x < 11; x++)
			if (mod->m_miscValue & (((uint32)1)<<x) )
				m_target->CreatureRangedAttackPowerMod[x+1] += mod->m_amount;
		if(mod->m_amount < 0)
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		for(uint32 x = 0; x < 11; x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->CreatureRangedAttackPowerMod[x+1] -= mod->m_amount;
			}
		}
	}
	m_target->CalcDamage();
}

void Aura::SpellAuraModDecreaseSpeed(bool apply)
{
	//there can not be 2 slow downs only most powerfull is applied
	if(apply)
	{
		Unit * m_caster = GetUnitCaster();
		switch(m_spellProto->NameHash)
		{
			case 0x1931b37a:			// Stealth
				SetPositive();
				break;

			case 0x25dab9ca:			// Dazed
				SetNegative();
				break;

			default:
				/* burlex: this would be better as a if(caster is hostile to target) then effect = negative) */
				if(m_casterGuid != m_target->GetGUID())
					SetNegative();
				break;
		}

		//let's check Mage talents if we proc anything
		if(m_spellProto->School==SCHOOL_FROST)
		{
			//yes we are freezing the bastard, so can we proc anything on this ?
			if(m_caster != NULL && m_target != NULL)
				m_caster->EventChill(m_target);
		}
		m_target->speedReductionMap.insert(make_pair(m_spellProto->Id, mod->m_amount));
		//m_target->m_slowdown=this;
		//m_target->m_speedModifier += mod->m_amount;
	}
	else
	{
		map< uint32, int32 >::iterator itr = m_target->speedReductionMap.find(m_spellProto->Id);
		if(itr != m_target->speedReductionMap.end())
			m_target->speedReductionMap.erase(itr);
	}

	if(m_target->GetSpeedDecrease())
		m_target->UpdateSpeed();

}

void Aura::UpdateAuraModDecreaseSpeed()
{
	if( m_target )
	{
		if( m_target->MechanicsDispels[MECHANIC_ENSNARED] )
		{
			return;
		}
	}

	//let's check Mage talents if we proc anythig
	if(m_spellProto->School==SCHOOL_FROST)
	{
		//yes we are freezing the bastard, so can we proc anything on this ?
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL && m_target != NULL )
			m_caster->EventChill( m_target );
	}
}

void Aura::SpellAuraModIncreaseHealth(bool apply)
{
	int32 amt;

	if(apply)
	{
		SpecialCases();
		SetPositive();
		amt = mod->m_amount;
	}
	else 
		amt =- mod->m_amount;

	if(m_target->IsPlayer())
	{
		if(apply)
			m_target->ModUnsigned32Value(UNIT_FIELD_HEALTH,amt);
		else
		{
			if((int32)m_target->GetUInt32Value(UNIT_FIELD_HEALTH)>-amt)//watch it on remove value is negative
				m_target->ModUnsigned32Value(UNIT_FIELD_HEALTH,amt);
			else m_target->SetUInt32Value(UNIT_FIELD_HEALTH,1); //do not kill player but do strip him good
		}
		TO_PLAYER( m_target )->SetHealthFromSpell(TO_PLAYER( m_target )->GetHealthFromSpell() + amt);
		TO_PLAYER( m_target )->UpdateStats();
	}
	else
		m_target->ModUnsigned32Value(UNIT_FIELD_MAXHEALTH, amt);
}

void Aura::SpellAuraModIncreaseEnergy(bool apply)
{
	SetPositive();

	uint32 powerField = UNIT_FIELD_POWER1 + mod->m_miscValue;

	m_target->ModUnsigned32Value(powerField,apply?mod->m_amount:-mod->m_amount);
	m_target->ModUnsigned32Value(powerField + 8,apply?mod->m_amount:-mod->m_amount);

	if(powerField == UNIT_FIELD_POWER1 && m_target->IsPlayer())
	{
		int32 amt = apply ? mod->m_amount : -mod->m_amount;
		TO_PLAYER(m_target)->SetManaFromSpell(TO_PLAYER(m_target)->GetManaFromSpell() + amt);
	}
}

void Aura::SpellAuraModShapeshift(bool apply)
{
	if( !m_target->IsPlayer())
		return;

	if( TO_PLAYER(m_target)->m_MountSpellId && TO_PLAYER(m_target)->m_MountSpellId != m_spellProto->Id )
		if( !(mod->m_miscValue & FORM_BATTLESTANCE | FORM_DEFENSIVESTANCE | FORM_BERSERKERSTANCE ) )
			m_target->RemoveAura( TO_PLAYER(m_target)->m_MountSpellId ); // these spells are not compatible

	//reset hp
	TO_PLAYER(m_target)->SetHealthFromSpell( 0 );
	TO_PLAYER(m_target)->UpdateStats();

	uint32 spellId = 0;
	uint32 spellId2 = 0;
	uint32 modelId = 0;

	bool freeMovements = false;

	switch( mod->m_miscValue )
	{
	case FORM_CAT: 
		{//druid
			freeMovements = true;
			spellId = 3025;
			if(apply)
			{
				m_target->SetByte(UNIT_FIELD_BYTES_0, 3, POWER_TYPE_ENERGY);
				m_target->SetUInt32Value(UNIT_FIELD_MAXPOWER4, 100);//100 Energy
				m_target->SetUInt32Value(UNIT_FIELD_POWER4, 0);//0 Energy
				if(m_target->getRace() == RACE_NIGHTELF)//NE
					modelId = 892;
				else //TAUREN
					modelId = 8571;

				if( m_target->HasDummyAura(SPELL_HASH_FUROR) )
					m_target->SetUInt32Value(UNIT_FIELD_POWER4, m_target->GetDummyAura(SPELL_HASH_FUROR)->RankNumber * 20);
			}
			else
			{//turn back to mana
				//m_target->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME,oldap);
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_MANA);
				if(m_target->m_stealth)
				{
					uint32 sp = m_target->m_stealth;
					m_target->m_stealth = 0;
					m_target->RemoveAura(sp);//prowl
				}
				if(m_target->HasActiveAura(1850))
					m_target->RemoveAura(1850);//Dash rank1
				if(m_target->HasActiveAura(9821))
					m_target->RemoveAura(9821);//Dash rank2
				if(m_target->HasActiveAura(33357))
					m_target->RemoveAura(33357);//Dash rank3
			}  
			TO_PLAYER( m_target )->UpdateAttackSpeed();
		}break;
	case FORM_TREE:
		{
			modelId  = 864;
			freeMovements=true;
			spellId = 5420;//3122;
			spellId2 = 34123;
		} break;
	case FORM_TRAVEL:
		{//druid
			freeMovements = true;
			spellId = 5419;
			modelId = 918;
		} break;
	case FORM_AQUA:
		{//druid aqua
			freeMovements = true;
			spellId = 5421;
			modelId = 2428;
		} break;
	case FORM_BEAR:
		{//druid only
			freeMovements = true;
			spellId = 1178;
			spellId2 = 21178;
			int32 amt = 0;

			if( m_target->HasDummyAura(SPELL_HASH_SURVIVAL_OF_THE_FITTEST) )
				amt = m_target->GetDummyAura(SPELL_HASH_SURVIVAL_OF_THE_FITTEST)->EffectBasePoints[2];

			if( amt )
				TO_PLAYER(m_target)->BaseResistanceModPctPos[0] += apply ? amt : -amt;


			if(apply)
			{
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_RAGE);
				m_target->SetUInt32Value(UNIT_FIELD_MAXPOWER2, 1000);
				m_target->SetUInt32Value(UNIT_FIELD_POWER2, 0);//0 rage

				if(m_target->getRace() == RACE_NIGHTELF)
					modelId = 2281;
				else //TAUREN
					modelId = 2289;
			}
			else //reset back to mana
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_MANA);

		} break;	
	case FORM_DIREBEAR:
		{//druid only
			freeMovements = true; 
			spellId = 9635;
			spellId2 = 21178;
			int32 amt = 0;

			if( m_target->HasDummyAura(SPELL_HASH_SURVIVAL_OF_THE_FITTEST) )
				amt = m_target->GetDummyAura(SPELL_HASH_SURVIVAL_OF_THE_FITTEST)->EffectBasePoints[2];

			if( amt )
				TO_PLAYER(m_target)->BaseResistanceModPctPos[0] += apply ? amt : -amt;


			if(apply)
			{
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_RAGE);
				m_target->SetUInt32Value(UNIT_FIELD_MAXPOWER2, 1000);
				m_target->SetUInt32Value(UNIT_FIELD_POWER2, 0);//0 rage
				if(m_target->getRace() == 4)//NE
					modelId = 2281;
				else //TAUREN
					modelId = 2289;
			}
			else //reset back to mana
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_MANA);

		} break;
	case FORM_GHOSTWOLF:
		{
			modelId = 4613;
			if( apply )
			{
				if( m_target->IsPlayer() )
					TO_PLAYER( m_target )->m_MountSpellId = m_spellProto->Id;
			}
			else
			{
				if( m_target->IsPlayer() )
					TO_PLAYER( m_target )->m_MountSpellId = 0;
			}
		} break;  
	case FORM_ZOMBIE:
		{
			modelId = 10626;
		} break;
	case FORM_BATTLESTANCE:
		{
			spellId = 21156;
		} break;
	case FORM_DEFENSIVESTANCE:
		{
			spellId = 7376;
		} break;
	case FORM_BERSERKERSTANCE:
		{
			spellId = 7381;
		} break;
	case FORM_SHADOW:
		{
			if(apply)
			{
				/*WorldPacket data(12);
				data.SetOpcode(SMSG_COOLDOWN_EVENT);
				data << (uint32)GetSpellProto()->Id << m_target->GetGUID();
				TO_PLAYER( m_target )->GetSession()->SendPacket(&data);*/
				packetSMSG_COOLDOWN_EVENT cd;
				cd.spellid = m_spellProto->Id;
				cd.guid = m_target->GetGUID();
				TO_PLAYER(m_target)->GetSession()->OutPacket(SMSG_COOLDOWN_EVENT, sizeof(packetSMSG_COOLDOWN_EVENT), &cd);
			}
		}break;
	case FORM_FLIGHT:
		{//druid
			freeMovements = true;
			spellId = 33948;
			if (apply)
			{
				if(m_target->getRace() == 8) //  Nightelf
					modelId = 20857;
				else  //  Tauren
					modelId = 20872;
			}
		}break;
	case FORM_STEALTH:
		{// rogue		
			if (!m_target->m_can_stealth)
				return;
			//m_target->UpdateVisibility();
		} break;
	case FORM_MOONKIN:
		{//druid
			freeMovements = true;
			spellId = 24905;
			if(apply)
			{
				if(m_target->getRace() == RACE_NIGHTELF)
					modelId = 15374;
				else 
					modelId = 15375;
			}
		}break;
	case FORM_SWIFT: //not tested yet, right now going on trust
		{// druid
			freeMovements = true;
			spellId = 40121; //Swift Form Passive
			if(apply)
			{
				if(m_target->getRace() == RACE_NIGHTELF)//NE
					modelId = 21243;
				else //TAUREN
					modelId = 21244;
			}
		}break;
	case FORM_SPIRITOFREDEMPTION:
		{
			spellId = 27795;
			modelId = 12824;
			TO_PLAYER(m_target)->m_canCastSpellsWhileDead = true;
		}break;
	case FORM_DEMON:
		{
			spellId = 59673;
			modelId = 25277;
			if (apply)
			{
				//m_target->m_modlanguage = LANG_DEMONIC;
				m_target->CastSpell(m_target, 54817, true);
				m_target->CastSpell(m_target, 54879, true);
				m_target->CastSpell(m_target, 61610, true);
			}
			else
			{
				//m_target->m_modlanguage = -1;
				m_target->RemoveAura(54817);
				m_target->RemoveAura(54879);
				m_target->RemoveAura(61610);
			}
		}break;
	case FORM_GHOUL:
		{
			spellId = 0;
			modelId = 25527;
		}break;
	}

	if( mod->m_miscValue == FORM_CAT || mod->m_miscValue == FORM_BEAR || mod->m_miscValue == FORM_DIREBEAR || mod->m_miscValue == FORM_MOONKIN )
		m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, apply ? TO_PLAYER(m_target)->m_feralAP : -TO_PLAYER(m_target)->m_feralAP);

	if( apply )
	{
		if( m_target->getClass() == WARRIOR )
		{
			if( m_target->GetUInt32Value( UNIT_FIELD_POWER2 ) > TO_PLAYER( m_target )->m_retainedrage )
				m_target->SetUInt32Value(UNIT_FIELD_POWER2, TO_PLAYER( m_target )->m_retainedrage );
			else
				m_target->SetUInt32Value(UNIT_FIELD_POWER2, m_target->GetUInt32Value(UNIT_FIELD_POWER2));
		}

		if( m_target->getClass() == DRUID )
		{
			if( m_target->HasDummyAura(SPELL_HASH_FUROR) && Rand( m_target->GetDummyAura(SPELL_HASH_FUROR)->RankNumber * 20 ) )
			{
				uint32 furorSpell = 0;
				if( mod->m_miscValue == FORM_BEAR || mod->m_miscValue == FORM_DIREBEAR )
					furorSpell = 17057;

				if( furorSpell != 0 )
				{
					SpellEntry *spellInfo = dbcSpell.LookupEntry( furorSpell );

					Spell* sp = NULL;
					sp = (new Spell( m_target, spellInfo, true, NULL ));
					SpellCastTargets tgt;
					tgt.m_unitTarget = m_target->GetGUID();
					sp->prepare(&tgt);
				}
			}
		}

		if( spellId != GetSpellId() )
		{
			if( TO_PLAYER( m_target )->m_ShapeShifted )
				TO_PLAYER( m_target )->RemoveAura( TO_PLAYER( m_target )->m_ShapeShifted );

			TO_PLAYER( m_target )->m_ShapeShifted = GetSpellId();
		}

		if( modelId != 0 )
			m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, modelId );

		TO_PLAYER( m_target )->SetShapeShift( mod->m_miscValue );

		// check for spell id
		if( spellId == 0 )
			return;

		SpellEntry* spellInfo = dbcSpell.LookupEntry(spellId );
		
		Spell* sp = NULL;
		sp = (new Spell( m_target, spellInfo, true, NULL ));
		SpellCastTargets tgt;
		tgt.m_unitTarget = m_target->GetGUID();
		sp->prepare( &tgt );
		
		if( spellId2 != 0 )
		{
			spellInfo = dbcSpell.LookupEntry(spellId2);
			sp = NULL;
			sp = (new Spell( m_target, spellInfo, true, NULL ));
			sp->prepare(&tgt);
		}

		// remove the caster from imparing movements
		if( freeMovements )
		{
			for( uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; x++ )
			{
				if( m_target->m_auras[x] != NULL )
				{
					if( m_target->m_auras[x]->HasMechanic(MECHANIC_ROOTED) || m_target->m_auras[x]->HasMechanic(MECHANIC_ENSNARED) ) // Remove roots and slow spells
					{
						m_target->RemoveAuraBySlot(x);
					}
					else // if got immunity for slow, remove some that are not in the mechanics
					{
						for( int i = 0; i < 3; i++ )
						{
							if( m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED || m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT )
							{
								m_target->RemoveAuraBySlot(x);
								break;
							}
						}
					}
			   }
			}
		}

		//execute after we changed shape
		TO_PLAYER( m_target )->EventTalentHearthOfWildChange( true );
	}
	else 
	{
		//execute before changing shape back
		TO_PLAYER( m_target )->EventTalentHearthOfWildChange( false );
		m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );				
		if( spellId != GetSpellId() && spellId )
		{
			m_target->RemoveAura( spellId );
			if( spellId == 27795 ) //Spirit Redemption
			{
				m_target->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
				TO_PLAYER(m_target)->m_canCastSpellsWhileDead = false;
			}
		}		
		
		TO_PLAYER( m_target )->m_ShapeShifted = 0;

		TO_PLAYER( m_target )->SetShapeShift( 0 );

	}
	TO_PLAYER( m_target )->UpdateStats();
	TO_PLAYER( m_target )->CalcResistance(RESISTANCE_ARMOR);
}

void Aura::SpellAuraModEffectImmunity(bool apply)
{
	if( m_spellProto->Id == 24937 )
		SetPositive();

	if( m_spellProto->Id == 23333 || m_spellProto->Id == 23335 || m_spellProto->Id == 34976 )
	{
		if( !apply )
		{
			Unit * m_caster = GetUnitCaster();
			if( m_caster == NULL || !m_caster->IsPlayer() || TO_PLAYER(m_caster)->m_bg == NULL)
				return;

			if( TO_PLAYER(m_caster)->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH )
				TO_WARSONGGULCH(TO_PLAYER(m_caster)->m_bg)->DropFlag(TO_PLAYER(m_caster));
			else if( TO_PLAYER(m_caster)->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM )
				TO_EYEOFTHESTORM(TO_PLAYER(m_caster)->m_bg)->DropFlag(TO_PLAYER(m_caster));
		}
	}
}

void Aura::SpellAuraModStateImmunity(bool apply)
{
	//%50 chance to dispel 1 magic effect on target
	//23922
}

void Aura::SpellAuraModSchoolImmunity(bool apply)
{
	if(m_target->IsPlayer())
	{
		if(apply)
			TO_PLAYER(m_target)->m_bgFlagIneligible++;
		else
			TO_PLAYER(m_target)->m_bgFlagIneligible--;
	}

	if( apply && ( m_spellProto->NameHash == SPELL_HASH_DIVINE_SHIELD || m_spellProto->NameHash == SPELL_HASH_ICE_BLOCK) ) // Paladin - Divine Shield
	{
		if( !m_target || !m_target->isAlive())
			return;

		Aura* pAura = NULL;
		for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
		{
			pAura = m_target->m_auras[i];
			if( pAura != NULL && pAura != this && !pAura->IsPassive() && !pAura->IsPositive() && !(pAura->GetSpellProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY) )
			{
				m_target->RemoveAuraBySlot(i);
			}
		}

		if(m_target->IsPlayer() && TO_PLAYER(m_target)->m_bg && TO_PLAYER(m_target)->m_bgHasFlag)
		{
			if(TO_PLAYER(m_target)->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
			{
				TO_WARSONGGULCH(TO_PLAYER(m_target)->m_bg)->DropFlag(TO_PLAYER(m_target));
			}
			if(TO_PLAYER(m_target)->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
			{
				TO_EYEOFTHESTORM(TO_PLAYER(m_target)->m_bg)->DropFlag(TO_PLAYER(m_target));
			}
		}
	}

	if(apply)
	{
		//fixme me may be negative
		Unit * m_caster = GetUnitCaster();
		if(m_caster!=NULL)
		{
			if(isAttackable(m_caster,m_target))
				SetNegative();
			else 
				SetPositive();
		}
		else
			SetPositive();

		for(uint32 i = 0; i < 7; i++)
		{
			if(mod->m_miscValue & (1<<i))
			{
				m_target->SchoolImmunityList[i] ++;
				m_target->RemoveAurasOfSchool(i, false, true);
			}
		}
	}
	else
	{
		for(int i = 0; i < 7; i++)
		{
			if(mod->m_miscValue & (1<<i) &&
				m_target->SchoolImmunityList[i] > 0)
				m_target->SchoolImmunityList[i]--;
		}
	}	
}

void Aura::SpellAuraModDmgImmunity(bool apply)
{
  
}

void Aura::SpellAuraModDispelImmunity(bool apply)
{
	assert(mod->m_miscValue < 10);

	if(apply)
	{
		m_target->dispels[mod->m_miscValue]++;
		for( uint32 x = 0; x < MAX_AURAS; x++ )
		{
			if(m_target->m_auras[x]!=NULL)
				if(m_target->m_auras[x]->GetSpellProto()->DispelType == (uint32)mod->m_miscValue)
					m_target->RemoveAuraBySlot(x);
		}
	}
	else 
		m_target->dispels[mod->m_miscValue]--;
}

void Aura::SpellAuraProcTriggerSpell(bool apply)
{
	if(apply)
	{
		ProcTriggerSpell pts;
		memset(&pts, 0, sizeof(pts));
		pts.origId = GetSpellProto()->Id;
		pts.caster = m_casterGuid;
		if(GetSpellProto()->EffectTriggerSpell[mod->i])
			pts.spellId=GetSpellProto()->EffectTriggerSpell[mod->i];
		else
		{
			OUT_DEBUG("Warning,trigger spell is null for spell %u",GetSpellProto()->Id);
			return;
		}
		pts.procChance = GetSpellProto()->procChance;
		pts.procFlags = GetSpellProto()->procFlags;
		pts.procCharges = GetSpellProto()->procCharges;
		if(mod->m_miscValue == EQUIPMENT_SLOT_MAINHAND)
			pts.weapon_damage_type = 1; // Proc only on main hand attacks
		else if(mod->m_miscValue == EQUIPMENT_SLOT_OFFHAND)
			pts.weapon_damage_type = 2; // Proc only on off hand attacks
		else
			pts.weapon_damage_type = 0; // Doesn't depend on weapon
		pts.LastTrigger = 0;
		pts.SpellClassMask[0] = GetSpellProto()->EffectSpellClassMask[mod->i][0];
		pts.SpellClassMask[1] = GetSpellProto()->EffectSpellClassMask[mod->i][1];
		pts.SpellClassMask[2] = GetSpellProto()->EffectSpellClassMask[mod->i][2];
		pts.ProcType = 0;
		pts.deleted = false;

		switch( m_spellProto->NameHash )
		{
		case SPELL_HASH_THE_TWIN_BLADES_OF_AZZINOTH:
			{
				if( m_target->IsPlayer() )
				{
					/* The Twin Blades of Azzinoth.
					 * According to comments on wowhead, this proc has ~0.75ppm (procs-per-minute). */
					Item* mh = TO_PLAYER(m_target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
					Item* of = TO_PLAYER(m_target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
					if( mh != NULL && of != NULL )
					{
						float mhs = float( mh->GetProto()->Delay );
						float ohs = float( of->GetProto()->Delay );
						pts.procChance = FL2UINT( float( mhs * ohs / ( 800.0f * ( mhs + ohs ) ) ) ); // 0.75 ppm
					}
				}
			}break;
		}

		m_target->m_procSpells.push_front(pts);
		DEBUG_LOG("Aura","%u is registering %u chance %u flags %u charges %u triggeronself %u interval %u",pts.origId,pts.spellId,pts.procChance,m_spellProto->procFlags & ~PROC_TARGET_SELF,m_spellProto->procCharges,m_spellProto->procFlags & PROC_TARGET_SELF,m_spellProto->proc_interval);
	}
	else
	{
		for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
		{
			if(itr->origId == GetSpellId() && itr->caster == m_casterGuid && !itr->deleted)
			{
				itr->deleted = true;
				break; //only 1 instance of a proc spell per caster ?
			}
		}
	}
}

void Aura::SpellAuraProcTriggerDamage(bool apply)
{
	if(apply)
	{
		DamageProc ds;
		ds.m_damage = mod->m_amount;
		ds.m_spellId = GetSpellProto()->Id;
		ds.m_school = GetSpellProto()->School;
		ds.m_flags = m_spellProto->procFlags;
		
		ds.owner = (void*)this;
		m_target->m_damageShields.push_back(ds);
		OUT_DEBUG("registering dmg proc %u, school %u, flags %u, charges %u \n",ds.m_spellId,ds.m_school,ds.m_flags,m_spellProto->procCharges);
	}
	else
	{
		for(std::list<struct DamageProc>::iterator i = m_target->m_damageShields.begin();i != m_target->m_damageShields.end();i++)
		{
			if(i->owner == this)
			{
				m_target->m_damageShields.erase(i);
				break;
			}
		}
	}
}

void Aura::SpellAuraTrackCreatures(bool apply)
{
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			if(TO_PLAYER( m_target )->TrackingSpell)
				m_target->RemoveAura( TO_PLAYER( m_target )->TrackingSpell);

			m_target->SetUInt32Value(PLAYER_TRACK_CREATURES,(uint32)1<< (mod->m_miscValue-1));
			TO_PLAYER( m_target )->TrackingSpell = GetSpellId();		
		}
		else
		{
			TO_PLAYER( m_target )->TrackingSpell = 0;
			m_target->SetUInt32Value(PLAYER_TRACK_CREATURES,0);
		}
	}
}

void Aura::SpellAuraTrackResources(bool apply)
{
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			if(TO_PLAYER( m_target )->TrackingSpell)
				m_target->RemoveAura(TO_PLAYER( m_target )->TrackingSpell);

		m_target->SetUInt32Value(PLAYER_TRACK_RESOURCES,(uint32)1<< (mod->m_miscValue-1));
		TO_PLAYER( m_target )->TrackingSpell = GetSpellId();
		}
		else
		{
			TO_PLAYER( m_target )->TrackingSpell = 0;
				m_target->SetUInt32Value(PLAYER_TRACK_RESOURCES,0);
		}
	}
}

void Aura::SpellAuraModParryPerc(bool apply)
{
	if(m_target->IsPlayer())
	{
		int32 amt;
		if(apply)
		{
			amt = mod->m_amount;
			if(amt<0)
				SetNegative();
			else
				SetPositive();

		}
		else
			amt = -mod->m_amount;

		TO_PLAYER( m_target )->SetParryFromSpell(TO_PLAYER( m_target )->GetParryFromSpell() + amt );
		TO_PLAYER( m_target )->UpdateChances();
	}
}

void Aura::SpellAuraModDodgePerc(bool apply)
{
	if(m_target->IsPlayer())
	{
		int32 amt = mod->m_amount;
//		SM_FIValue(m_target->SM_FMISC_EFFECT, &amt, GetSpellProto()->SpellGroupType);
		if(apply)
		{
			if(amt<0)
				SetNegative();
			else
				SetPositive();
		}
		else 
			amt = -amt;
		TO_PLAYER( m_target )->SetDodgeFromSpell(TO_PLAYER( m_target )->GetDodgeFromSpell() + amt );
		TO_PLAYER( m_target )->UpdateChances();
	}
}

void Aura::SpellAuraModBlockPerc(bool apply)
{
	if(m_target->IsPlayer())
	{
		int32 amt;
		if(apply)
		{
			amt = mod->m_amount;
			if(amt<0)
				SetNegative();
			else 
				SetPositive();
		}
		else 
			amt = -mod->m_amount;

		TO_PLAYER( m_target )->SetBlockFromSpell(TO_PLAYER( m_target )->GetBlockFromSpell() + amt);
		TO_PLAYER( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModCritPerc(bool apply)
{
	SpecialCases();
	if (m_target->IsPlayer())
	{
		if(apply)
		{
			WeaponModifier md;
			md.value = float(mod->m_amount);
			md.wclass = GetSpellProto()->EquippedItemClass;
			md.subclass = GetSpellProto()->EquippedItemSubClass;
			TO_PLAYER( m_target )->tocritchance.insert(make_pair(GetSpellId(), md));
		}
		else 
		{
			/*std::list<WeaponModifier>::iterator i = TO_PLAYER( m_target )->tocritchance.begin();

			for(;i!=TO_PLAYER( m_target )->tocritchance.end();i++)
			{
				if((*i).spellid==GetSpellId())
				{
					TO_PLAYER( m_target )->tocritchance.erase(i);
					break;
				}
			}*/
			TO_PLAYER( m_target )->tocritchance.erase(GetSpellId());
		}
		TO_PLAYER( m_target )->UpdateChances();
	}
}

void Aura::SpellAuraPeriodicLeech(bool apply)
{
	if(apply)
	{
		SetNegative();
		uint32 amt = mod->m_amount;
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL )
			amt = m_caster->GetSpellBonusDamage(m_target, GetSpellProto(), amt, true, false);

		if( amt < 0 )
			amt = 0;

		sEventMgr.AddEvent(this, &Aura::EventPeriodicLeech, amt,
			EVENT_AURA_PERIODIC_LEECH, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicLeech(uint32 amount)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL || m_target == NULL || !m_target->isAlive() || !m_caster->isAlive() )
		return;

	if( m_target->SchoolImmunityList[GetSpellProto()->School] )
		return;

	if( m_spellProto->NameHash == SPELL_HASH_DRAIN_LIFE && m_caster->HasDummyAura(SPELL_HASH_DEATH_S_EMBRACE) && m_caster->GetHealthPct() <= 20 )
		amount *= 1.3f;

	amount = m_caster->GetSpellBonusDamage(m_target, GetSpellProto(), amount, true, false);

	uint32 Amount = (uint32)min(amount, m_target->GetUInt32Value( UNIT_FIELD_HEALTH ));
		
	// Apply bonus from [Warlock] Soul Siphon
	if (m_caster->m_soulSiphon.amt)
	{
		// Use set to prevent counting duplicate auras (stacked ones, from the same unit)
		set<uint32> auras;

		for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
		{
			if(m_target->m_auras[x])
			{
				Aura* aura = m_target->m_auras[x];
				if (aura->GetSpellProto()->SpellFamilyName == 5)
				{
					skilllinespell *sk;

					sk = objmgr.GetSpellSkill(aura->GetSpellId());
					if(sk && sk->skilline == SKILL_AFFLICTION)
					{
						auras.insert( aura->GetSpellId() );
					}
				}
			}
		}

		uint32 siphonbonus = uint32(auras.size()) * m_caster->m_soulSiphon.amt;
		if( siphonbonus > m_caster->m_soulSiphon.max )
			siphonbonus = m_caster->m_soulSiphon.max;

		uint32 bonus = (Amount * siphonbonus) / 100;
		Amount += bonus;
	}

	SendPeriodicAuraLog(m_caster, m_target, GetSpellProto(), Amount, -1, 0, FLAG_PERIODIC_DAMAGE);

	//deal damage before we add healing bonus to damage
	m_caster->DealDamage(m_target, Amount, 0, 0, GetSpellProto()->Id, true);
	m_caster->HandleProc(PROC_ON_SPELL_LAND, m_target, m_spellProto, Amount);

	if(m_spellProto)
	{
		float coef = m_spellProto->EffectMultipleValue[mod->i]; // how much health is restored per damage dealt
		SM_FFValue(m_caster->SM[SMT_MULTIPLE_VALUE][0], &coef, m_spellProto->SpellGroupType);
		SM_PFValue(m_caster->SM[SMT_MULTIPLE_VALUE][1], &coef, m_spellProto->SpellGroupType);
		Amount = float2int32((float)Amount * coef);
	}

	uint32 newHealth = float2int32(m_caster->GetUInt32Value(UNIT_FIELD_HEALTH) + Amount);
		
	uint32 mh = m_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(newHealth <= mh)
		m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
	else
		m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, mh);

	SendPeriodicAuraLog(m_caster, m_caster, GetSpellProto(), Amount, -1, 0, FLAG_PERIODIC_HEAL);
}

void Aura::SpellAuraModHitChance(bool apply)
{
	if (!m_target->IsPlayer())
		return;

	int32 amount = mod->m_amount;

	if (apply)
	{
		TO_PLAYER( m_target )->SetHitFromMeleeSpell( TO_PLAYER( m_target )->GetHitFromMeleeSpell() + amount);
		if(mod->m_amount<0)
			SetNegative();
		else 
			SetPositive();
	}
	else
	{
		TO_PLAYER( m_target )->SetHitFromMeleeSpell( TO_PLAYER( m_target )->GetHitFromMeleeSpell() - amount);
		if( TO_PLAYER( m_target )->GetHitFromMeleeSpell() < 0 )
		{
			TO_PLAYER( m_target )->SetHitFromMeleeSpell( 0 );
		}
	}
}

void Aura::SpellAuraModSpellHitChance(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else 
		SetPositive();

	if (m_target->IsPlayer())
	{
		if (apply)
		{
			TO_PLAYER( m_target )->SetHitFromSpell(TO_PLAYER( m_target )->GetHitFromSpell() + mod->m_amount);
		}
		else
		{
			TO_PLAYER( m_target )->SetHitFromSpell(TO_PLAYER( m_target )->GetHitFromSpell() - mod->m_amount);
			if(TO_PLAYER( m_target )->GetHitFromSpell() < 0)
			{
				TO_PLAYER( m_target )->SetHitFromSpell(0);
			}
		}
	}
}

void Aura::SpellAuraTransform(bool apply)
{
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummyAura(GetSpellId(), mod->i, this, apply))
		return;

	uint32 displayId = 0;
	CreatureInfo* ci = NULL;
	ci = CreatureNameStorage.LookupEntry(mod->m_miscValue);

	if(ci == NULL)
		DEBUG_LOG("Aura","SpellAuraTransform cannot find CreatureInfo for id %d",mod->m_miscValue);
	else
		displayId = ci->Male_DisplayID;

	if( m_target->IsPlayer() && TO_PLAYER(m_target)->IsMounted() )
		TO_PLAYER(m_target)->Dismount();

	// SetPositive();
	Unit * m_caster = GetUnitCaster();
	switch( GetSpellProto()->Id )
	{
		case 47585: // Dispersion
		{
			if( apply && m_caster != NULL )
			{
				SpellEntry *spellInfo = dbcSpell.LookupEntry( 60069 );
				if(!spellInfo) 
					return;

				Spell* spell = NULL;
				spell = new Spell(m_target, spellInfo ,true, NULL);
				SpellCastTargets targets(m_target->GetGUID());
				spell->prepare(&targets);
			}
		}break;
		case 57669: // Replenishment
		case 61782:
		{
			if(apply)
			{
				uint32 manaToRegen = (uint32)(m_target->GetUInt32Value(UNIT_FIELD_BASE_MANA) * 0.0025f);
				if( !manaToRegen ) return;
				sEventMgr.AddEvent(this, &Aura::EventPeriodicEnergize,(uint32)manaToRegen,(uint32)0, EVENT_AURA_PERIODIC_ENERGIZE,1000,0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
			else
			{
				sEventMgr.RemoveEvents(this, EVENT_AURA_PERIODIC_ENERGIZE);
			}
		}break;
		case 20584://wisp
			m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, apply ? 10045:m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		break;

		case 30167: // Red Ogre Costume
		{
			if( apply )
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, 11549 );
			else
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		}break;

		case 41301: // Time-Lost Figurine
		{
			if( apply )
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, 18628 );
			else
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		}break;

		case 16739: // Orb of Deception
		{
			if( apply )
			{
				switch(m_target->getRace())
				{
					case RACE_ORC:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10139);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10140);
					}break;
					case RACE_TAUREN:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10136);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10147);
					}break;
					case RACE_TROLL:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10135);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10134);
					}break;
					case RACE_UNDEAD:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10146);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10145);
					}break;
					case RACE_BLOODELF:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17829);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17830);
					}break;
					case RACE_GNOME:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10148);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10149);
					}break;
					case RACE_DWARF:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10141);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10142);
					}break;
					case RACE_HUMAN:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10137);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10138);
					}break;
					case RACE_NIGHTELF:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10143);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10144);
					}break;
					case RACE_DRAENEI:
					{
						if( m_target->getGender() == 0 ) 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17827);
						else 
							m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17828);
					}break;
					default:
						m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
				}
			}
		}break;

		case 42365:	// murloc costume
			m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, apply ? 21723 : m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
			break;

		case 118://polymorh
		case 851:
		case 5254:
		case 12824:
		case 12825:
		case 12826:
		case 13323:
		case 15534:
		case 22274:
		case 23603:
		case 28270:	 // Polymorph: Cow
		case 28271:	 // Polymorph: Turtle
		case 28272:	 // Polymorph: Pig
		case 61025:  // Polymorph: Serpent
		case 61305:  // Polymorph: Black Cat
			{
				if(!displayId)
				{
					switch(GetSpellProto()->Id)
					{
					case 28270:	 // Cow
						displayId = 1060;
						break;

					case 28272:	 // Pig
						displayId = 16356 + RandomUInt(2);
						break;
						
					case 28271:	 // Turtle
						displayId = 16359 + RandomUInt(2);
						break;

					default:
						displayId = 856;
						break;
						
					}
				}

				if( m_caster != NULL && m_caster->IsPlayer() )
				{
					if( displayId == 856 || displayId == 857 )
					{
						if( TO_PLAYER(m_caster)->HasDummyAura(SPELL_HASH_GLYPH_OF_THE_BEAR_CUB) )
							displayId = 23946;
						if( TO_PLAYER(m_caster)->HasDummyAura(SPELL_HASH_GLYPH_OF_THE_PENGUIN) )
							displayId = 26452;
					}
				}

				if(apply)
				{
					if( m_caster->HasDummyAura(SPELL_HASH_GLYPH_OF_POLYMORPH) )
					{
						for(uint8 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
						{ 
							if(m_target->m_auras[i] != NULL)
							{
								for(uint32 x = 0; x < 3; x++)
								{
									if( m_target->m_auras[i]->m_spellProto->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_DAMAGE )
									{
										m_target->RemoveAuraBySlot(i);
										break;
									}
								}
							}
						}
					}

					m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayId);

					// remove the current spell (for channelers)
					if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid && 
						m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
					{
						m_target->m_currentSpell->cancel();
						m_target->m_currentSpell = NULL;
					}

					sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1,(uint32)2000,EVENT_AURA_PERIODIC_HEAL,1000,0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					m_target->polySpell = GetSpellProto()->Id;
				}
				else
				{
					m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
					m_target->polySpell = 0;
				}
			}break;

		case 19937:
			{
				if (apply)
				{
					// TODO: Sniff the spell / item, we need to know the real displayID
					// guessed this may not be correct
					// human = 7820
					// dwarf = 7819
					// halfling = 7818
					// maybe 7842 as its from a lesser npc
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, 7842);
				} 
				else
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
				}
			}break;

		default:
		{
			if(!displayId) return;
			
			if (apply) 
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, displayId);
				}
				else 
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
				}
		}break;
	};
}

void Aura::SpellAuraModSpellCritChance(bool apply)
{
	if (m_target->IsPlayer())
	{
		int32 amt;
		if(apply)
		{
			amt = mod->m_amount;
			if(amt<0)
				SetNegative();
			else
				SetPositive();
		}
		else 
			amt = -mod->m_amount;

		m_target->spellcritperc += amt;
		TO_PLAYER( m_target )->SetSpellCritFromSpell( TO_PLAYER( m_target )->GetSpellCritFromSpell() + amt );
		TO_PLAYER( m_target )->UpdateChanceFields();
	}
}

void Aura::SpellAuraIncreaseSwimSpeed(bool apply)
{   
	if(apply)
	{
		if(m_target->isAlive())  SetPositive(); 
		m_target->m_swimSpeed = 0.04722222f*(100+mod->m_amount);
	}
	else 
		m_target->m_swimSpeed = PLAYER_NORMAL_SWIM_SPEED;
	if(m_target->IsPlayer())
	{
		WorldPacket data(SMSG_FORCE_SWIM_SPEED_CHANGE, 17);
		data << m_target->GetNewGUID();
		data << (uint32)2;
		data << m_target->m_swimSpeed;
		TO_PLAYER( m_target )->GetSession()->SendPacket(&data);
	}   
}

void Aura::SpellAuraModCratureDmgDone(bool apply)
{
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			for(uint32 x = 0; x < 11; x++)
				if(mod->m_miscValue & (((uint32)1)<<x))
					TO_PLAYER( m_target )->IncreaseDamageByType[x+1] += mod->m_amount;		

			mod->m_amount < 0 ? SetNegative() : SetPositive();
		}
		else
			for(uint32 x = 0; x < 11; x++)
				if(mod->m_miscValue & (((uint32)1)<<x) )
					TO_PLAYER( m_target )->IncreaseDamageByType[x+1] -= mod->m_amount;
	}	
}

void Aura::SpellAuraPacifySilence(bool apply)
{
	// Can't Attack or Cast Spells
	if(apply)
	{
		if(m_spellProto->Id == 24937)
			SetPositive();
		else
			SetNegative();

		m_target->m_pacified++;
		m_target->m_silenced++;
		m_target->m_special_state |= UNIT_STATE_PACIFY | UNIT_STATE_SILENCE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);

		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid && 
			m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
			{
				m_target->m_currentSpell->cancel();
				m_target->m_currentSpell = NULL;
			}
	}
	else
	{
		m_target->m_pacified--;

		if(m_target->m_pacified == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_PACIFY;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
		}

		m_target->m_silenced--;

		if(m_target->m_silenced == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_SILENCE;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
		}
	}
}

void Aura::SpellAuraModScale(bool apply)
{
	float current = m_target->GetFloatValue(OBJECT_FIELD_SCALE_X);
	float delta = mod->m_amount/100.0f;

	m_target->SetFloatValue(OBJECT_FIELD_SCALE_X, apply ? (current+current*delta) : current/(1.0f+delta));
}

void Aura::SpellAuraPeriodicHealthFunnel(bool apply)
{
	if(apply)
	{
		uint32 amt = mod->m_amount;
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent(this, &Aura::EventPeriodicHealthFunnel, amt,
			EVENT_AURA_PERIODIC_HEALTH_FUNNEL, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicHealthFunnel(uint32 amount)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL  || m_target == NULL || !m_target->isAlive() || !m_caster->isAlive())
		return;
	if(m_target->isAlive() && m_caster->isAlive())
	{

		m_caster->DealDamage(m_target, amount, 0, 0, GetSpellId(),true);
		uint32 newHealth = m_caster->GetUInt32Value(UNIT_FIELD_HEALTH) + 1000;

		uint32 mh = m_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
		if(newHealth <= mh)
			m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
		else
			m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, mh);

		SendPeriodicAuraLog(m_target, m_target, m_spellProto, amount, -1, 0, FLAG_PERIODIC_LEECH);
	}
}

void Aura::SpellAuraPeriodicManaLeech(bool apply)
{
	if(apply)
	{
		uint32 amt = mod->m_amount;
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent(this, &Aura::EventPeriodicManaLeech, amt,
			EVENT_AURA_PERIODIC_LEECH, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicManaLeech(uint32 amount)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL  || m_target == NULL || !m_target->isAlive() || !m_caster->isAlive())
		return;

	int32 amt = amount;

	// Drained amount should be reduced by resilence 
	if(m_target->IsPlayer())
		{
		float amt_reduction_pct = 2.2f * TO_PLAYER(m_target)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE ) / 100.0f;
		if( amt_reduction_pct > 0.33f )
			amt_reduction_pct = 0.33f; // 3.0.3
		amt = float2int32( amt - (amt * amt_reduction_pct) );
		}
	switch( m_spellProto->NameHash )
	{
	case SPELL_HASH_VIPER_STING:
		{
			if( m_target->GetUInt32Value( UNIT_FIELD_POWER1 ) * ( amt / 100.0f ) > m_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1) * 0.08f )
				amt = float2int32( m_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1) * 0.08f );
			else
				amt = float2int32( m_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1) * (amt / 100.0f ) );
		}break;
	case SPELL_HASH_DRAIN_MANA: // Warlock - Drain mana 
		{
			if( m_target->GetUInt32Value( UNIT_FIELD_POWER1 ) * (amt / 100.0f) > m_caster->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) * 0.06f )
				amt = float2int32( m_target->GetUInt32Value( UNIT_FIELD_POWER1 ) * ( amt / 100.0f ) );
			else
				amt = float2int32( m_caster->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) * 0.06f );
		}break;
	}
	amt = (int32)min( (uint32)amt, m_target->GetUInt32Value( UNIT_FIELD_POWER1 ) );
	m_target->ModUnsigned32Value(UNIT_FIELD_POWER1, -amt);

	float coef = m_spellProto->EffectMultipleValue[mod->i] > 0 ? m_spellProto->EffectMultipleValue[mod->i] : 1; // how much mana is restored per mana leeched
	SM_FFValue(m_caster->SM[SMT_MULTIPLE_VALUE][0], &coef, m_spellProto->SpellGroupType);
	SM_PFValue(m_caster->SM[SMT_MULTIPLE_VALUE][1], &coef, m_spellProto->SpellGroupType);
	amt = float2int32((float)amt * coef);

	uint32 cm = m_caster->GetUInt32Value(UNIT_FIELD_POWER1) + amt;
	uint32 mm = m_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if(cm <= mm)
	{
		m_caster->SetUInt32Value(UNIT_FIELD_POWER1, cm);
		SendPeriodicAuraLog(m_caster, m_target, m_spellProto, amt, 0, 0, FLAG_PERIODIC_ENERGIZE);
	}
	else
	{
		m_caster->SetUInt32Value(UNIT_FIELD_POWER1, mm);
		SendPeriodicAuraLog(m_caster, m_target, m_spellProto, mm - cm, 0, 0, FLAG_PERIODIC_ENERGIZE);
	}

	m_caster->SendPowerUpdate();
}

void Aura::SpellAuraModCastingSpeed(bool apply)
{
	float current = m_target->GetFloatValue(UNIT_MOD_CAST_SPEED);

	// WoWwiki: THIS STUFF IS ADDED MULTIPLICATIVELY. DO NOT UNDO. NEWBSSSS.
	if(apply)
	{
		mod->fixed_float_amount[0] = current * (float(mod->m_amount) / 100.0f);
		current -= mod->fixed_float_amount[0];
	}
	else
	{
		current += mod->fixed_float_amount[0];
	}

	m_target->SetFloatValue(UNIT_MOD_CAST_SPEED, current);
}

void Aura::SpellAuraFeignDeath(bool apply)
{
	if( m_target->IsPlayer() )
	{
		Player* pTarget = TO_PLAYER( m_target );
		WorldPacket data(50);
		if( apply )
		{
			pTarget->EventAttackStop();
			pTarget->SetFlag( UNIT_FIELD_FLAGS_2, 1 );
			pTarget->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH );
			//pTarget->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD );
			pTarget->SetFlag( UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD );
			//pTarget->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD );
			
			data.SetOpcode( SMSG_START_MIRROR_TIMER );
			data << uint32( 2 );		// type
			data << uint32( GetDuration() );
			data << uint32( GetDuration() );
			data << uint32( 0xFFFFFFFF );
			data << uint8( 0 );
			data << uint32( m_spellProto->Id );		// ???
			pTarget->GetSession()->SendPacket( &data );

			data.Initialize(0x03BE);
			data << pTarget->GetGUID();
//			pTarget->setDeathState(DEAD);
			unordered_set< Object* >::iterator itr,itr2;
			Object* pObject = NULL;

			//now get rid of mobs agro. pTarget->CombatStatus.AttackersForgetHate() - this works only for already attacking mobs
			for(itr = pTarget->GetInRangeSetBegin(); itr != pTarget->GetInRangeSetEnd();)
			{
				itr2 = itr;
				++itr;

				pObject = (*itr2);

				if(pObject->IsUnit() && (TO_UNIT(pObject))->isAlive())
				{
					if(pObject->GetTypeId()==TYPEID_UNIT)
						(TO_UNIT( pObject ))->GetAIInterface()->RemoveThreatByPtr(pTarget);
					//if this is player and targeting us then we interrupt cast
					if( ( pObject )->IsPlayer() )
					{
						//if player has selection on us
						if( TO_PLAYER( pObject )->GetSelection()==pTarget->GetGUID())							
						{
							//it seems that all these do not work in 2.3
							//TO_PLAYER( (*itr) )->SetSelection(0);//loose selection
							//TO_PLAYER( (*itr) )->SetUInt64Value(UNIT_FIELD_TARGET, 0);
						}
						if( TO_PLAYER( pObject )->isCasting() && TO_PLAYER( pObject )->GetCurrentSpell()->GetSpellProto() != m_spellProto )
							TO_PLAYER( pObject )->CancelSpell( NULL ); //cancel current casting spell

						TO_PLAYER( pObject )->GetSession()->SendPacket( &data );
					}
				}
			}
			pTarget->setDeathState(ALIVE);
		}
		else
		{
			pTarget->RemoveFlag(UNIT_FIELD_FLAGS_2, 1);
			pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH);
			pTarget->RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD);
			//pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD);
			//pTarget->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			data.SetOpcode(SMSG_STOP_MIRROR_TIMER);
			data << uint32(2);
			pTarget->GetSession()->SendPacket(&data);
		}
	}
}

void Aura::SpellAuraModDisarm(bool apply)
{
	if(apply)
	{
		if( m_target->IsPlayer() && TO_PLAYER(m_target)->IsInFeralForm())
			return;

		SetNegative();

		m_target->disarmed = true;
		if( m_spellProto->NameHash == SPELL_HASH_DISMANTLE )
			m_target->disarmedShield = true;

		m_target->m_special_state |= UNIT_STATE_DISARMED;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
	}
	else
	{
		m_target->disarmed = false;
		if( m_spellProto->NameHash == SPELL_HASH_DISMANTLE )
			m_target->disarmedShield = false;

		m_target->m_special_state &= ~UNIT_STATE_DISARMED;
		m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
	}
}

void Aura::SpellAuraModStalked(bool apply)
{
	if(apply)
	{
		m_target->stalkedby = m_casterGuid;
		SetNegative();
	}
	else 
	{
		m_target->stalkedby = 0;
	}
}

void Aura::SpellAuraSchoolAbsorb(bool apply)
{
	Absorb *ab = NULL;
	if(apply)
	{
		SetPositive();

		int32 val = mod->m_amount;
		
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL && m_caster->IsPlayer() )
		{
			float otcoef = GetSpellProto()->OTspell_coef_override;
			float ddcoef = GetSpellProto()->Dspell_coef_override;
			float coefmod = 0;
			SM_FFValue( TO_PLAYER(m_caster)->SM[SMT_SPD_BONUS][0], &coefmod, m_spellProto->SpellGroupType );
			SM_FFValue( TO_PLAYER(m_caster)->SM[SMT_SPD_BONUS][1], &coefmod, m_spellProto->SpellGroupType );
		
			otcoef += coefmod / 100;
			ddcoef += coefmod / 100;

			//For spells Affected by Bonus Healing we use Dspell_coef_override.
			if( ddcoef > 0 )
				val += float2int32( float( TO_PLAYER(m_caster)->HealDoneMod[GetSpellProto()->School] ) * ddcoef );
			//For spells Affected by Bonus Damage we use OTspell_coef_override.
			else if( otcoef > 0 )
				val += float2int32( float( TO_PLAYER(m_caster)->GetDamageDoneMod( GetSpellProto()->School ) ) * otcoef );
			
			if( GetSpellProto()->AP_coef_override > 0 )
				val += float2int32(m_caster->GetAP() * GetSpellProto()->AP_coef_override);

			if( GetSpellProto()->RAP_coef_override > 0 )
				val += float2int32(m_caster->GetAP() * GetSpellProto()->RAP_coef_override);
		}

		ab = new Absorb;
		ab->amt = val;
		ab->spellid = GetSpellId();
		ab->caster = m_casterGuid;
		ab->reflect_pct = 0;
		if( m_caster != NULL && m_caster->HasDummyAura(SPELL_HASH_REFLECTIVE_SHIELD) )
			ab->reflect_pct += m_caster->GetDummyAura(SPELL_HASH_REFLECTIVE_SHIELD)->EffectBasePoints[0];
		
		for(uint32 x=0;x<7;x++)
			if (mod->m_miscValue & (((uint32)1)<<x) )
				m_target->Absorbs[x].push_back(ab);
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				for(SchoolAbsorb::iterator i = m_target->Absorbs[x].begin(); i != m_target->Absorbs[x].end(); i++)
				{
					if((*i)->spellid == GetSpellId() && (*i)->caster==m_casterGuid )
					{
						m_target->Absorbs[x].erase(i);
						break;
					}	
				}
			}
		}
	}	
}

void Aura::SpellAuraModSpellCritChanceSchool(bool apply)
{
	if(apply)
	{
		for(uint32 x=0;x<7;x++)
			if (mod->m_miscValue & (((uint32)1)<<x))
				m_target->SpellCritChanceSchool[x] += mod->m_amount;
		if(mod->m_amount < 0)
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->SpellCritChanceSchool[x] -= mod->m_amount;
				/*if(m_target->SpellCritChanceSchool[x] < 0)
					m_target->SpellCritChanceSchool[x] = 0;*/
			}
		}
	}
	if(m_target->IsPlayer())
		TO_PLAYER( m_target )->UpdateChanceFields();
}

void Aura::SpellAuraModPowerCost(bool apply)
{
	int32 val = (apply) ? mod->m_amount : -mod->m_amount;
	if (apply)
	{
		if(val < 0)
			SetNegative();
		else
			SetPositive();
	}
	for(uint32 x=0;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->ModFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+x,val/100.0f);
}

void Aura::SpellAuraModPowerCostSchool(bool apply)
{
	if(apply)
	{
		for(uint32 x=0;x<7;x++)
			if (mod->m_miscValue & (((uint32)1)<<x) )
				m_target->PowerCostMod[x] += mod->m_amount;
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->PowerCostMod[x] -= mod->m_amount;
			}
		}
	}
}

void Aura::SpellAuraReflectSpellsSchool(bool apply)
{
	if(apply)
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete (*i);
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
		ReflectSpellSchool *rss = NULL;
		rss = new ReflectSpellSchool;

		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->require_aura_hash = 0;
		rss->school = m_spellProto->School;
		rss->infront = false;
		rss->infinity = false;

		m_target->m_reflectSpellSchool.push_back(rss);
	}
	else
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete *i;
				i = m_target->m_reflectSpellSchool.erase(i);
				break;
			}
			else
				++i;
		}
	}	
}

void Aura::SpellAuraModLanguage(bool apply)
{
	if(apply) 
		m_target->m_modlanguage = mod->m_miscValue;
	else 
		m_target->m_modlanguage = -1;
}

void Aura::SpellAuraAddFarSight(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL || !m_caster->IsPlayer())
		return;

	if(apply)
	{
		float sightX = m_caster->GetPositionX() + 100.0f;
		float sightY = m_caster->GetPositionY() + 100.0f;
		m_caster->SetUInt64Value(PLAYER_FARSIGHT, mod->m_miscValue);
		m_caster->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(m_caster), sightX, sightY, true);

	}
	else
	{
		m_caster->SetUInt64Value(PLAYER_FARSIGHT, 0);
		m_caster->GetMapMgr()->ChangeFarsightLocation(TO_PLAYER(m_caster), 0, 0, false);
	}
}

void Aura::SpellAuraMechanicImmunity(bool apply)
{
	if( m_target->IsPlayer())
	{
		switch(m_spellProto->Id)
		{
		case 25771:
			TO_PLAYER(m_target)->mForbearance = apply;
			SetNegative();
			break;

		case 6788:
			TO_PLAYER(m_target)->mWeakenedSoul = apply;
			SetNegative();
			break;

		case 41425:
			TO_PLAYER(m_target)->mHypothermia = apply;
			SetNegative();
			break;
		case 11196:
			TO_PLAYER(m_target)->mRecentlyBandaged = apply;
			SetNegative();
			break;
		}
	}

	if(apply)
	{
		assert(mod->m_miscValue < MECHANIC_COUNT);
		m_target->MechanicsDispels[mod->m_miscValue]++;

		if(mod->m_miscValue != MECHANIC_HEALING && mod->m_miscValue != MECHANIC_INVULNARABLE && mod->m_miscValue != MECHANIC_SHIELDED) // dont remove bandages, Power Word and protection effect
		{
			/* Supa's test run of Unit::RemoveAllAurasByMechanic */
			if( m_target ) // just to be sure?
			{
				m_target->RemoveAllAurasByMechanic( (uint32)mod->m_miscValue , -1 , false );
			}

			if(m_spellProto->Id == 42292 || m_spellProto->Id == 59752)	// PvP Trinket
			{
				// insignia of the A/H
				for(uint32 x= MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
				{
					if(m_target->m_auras[x] != NULL)
					{
						uint32 mechanic = Spell::GetMechanic(m_target->m_auras[x]->GetSpellProto());
						switch(mechanic)
						{
							case MECHANIC_CHARMED:
							case MECHANIC_DISORIENTED:
							case MECHANIC_FLEEING:
							case MECHANIC_ROOTED:
							case MECHANIC_PACIFIED:
							case MECHANIC_ASLEEP:
							case MECHANIC_STUNNED:
							case MECHANIC_INCAPACIPATED:
							case MECHANIC_POLYMORPHED:
							case MECHANIC_SEDUCED:
							case MECHANIC_FROZEN:
							case MECHANIC_ENSNARED:
							case MECHANIC_BANISHED:
								m_target->RemoveAuraBySlot(x);
						}
					}
				}
			}
		}
		else
			SetNegative();
	}
	else
		m_target->MechanicsDispels[mod->m_miscValue]--;
}

void Aura::SpellAuraMounted(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	//Remove any previous mount if we had one
	if(TO_PLAYER(m_target)->IsMounted())
		m_target->Dismount();

	if(m_target->InStealth())
	{
		uint32 id = m_target->m_stealth;
		m_target->m_stealth = 0;
		m_target->RemoveAura(id);
	}

	bool isVehicleSpell  = m_spellProto->Effect[1] == SPELL_EFFECT_SUMMON ? true : false;

	if(apply)
	{   
		if( isVehicleSpell ) // get rid of meeeee, I'm a useless placeholder!
			SetDuration(100);

		TO_PLAYER(m_target)->m_bgFlagIneligible++;
		SetPositive();

		//Dismiss any pets
		if(TO_PLAYER(m_target)->GetSummon())
		{
			if(TO_PLAYER(m_target)->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				TO_PLAYER(m_target)->GetSummon()->Dismiss(false);				// warlock summon -> dismiss
			else
			{
				TO_PLAYER(m_target)->GetSummon()->Remove(false, true, true);	// hunter pet -> just remove for later re-call
				TO_PLAYER(m_target)->hasqueuedpet = true;
			}
		}

		if(TO_PLAYER(m_target)->m_bg)
			TO_PLAYER(m_target)->m_bg->HookOnMount(TO_PLAYER(m_target));

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_MOUNT);

		CreatureInfo* ci = CreatureNameStorage.LookupEntry(mod->m_miscValue);
		if(!isVehicleSpell && ci != NULL && ci->Male_DisplayID != 0)
			m_target->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , ci->Male_DisplayID);

		TO_PLAYER(m_target)->m_MountSpellId = m_spellProto->Id;
		TO_PLAYER(m_target)->m_FlyingAura = 0;
		
		//m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);

		if( TO_PLAYER(m_target)->GetShapeShift() && 
			!(TO_PLAYER(m_target)->GetShapeShift() & FORM_BATTLESTANCE | FORM_DEFENSIVESTANCE | FORM_BERSERKERSTANCE ) && 
			TO_PLAYER(m_target)->m_ShapeShifted != m_spellProto->Id )
			m_target->RemoveAura( TO_PLAYER(m_target)->m_ShapeShifted );
	}
	else
	{
		TO_PLAYER(m_target)->m_bgFlagIneligible--;
		TO_PLAYER(m_target)->m_MountSpellId = 0;
		TO_PLAYER(m_target)->m_FlyingAura = 0;

		if( !isVehicleSpell )
			m_target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);

		uint8 petnum = TO_PLAYER(m_target)->GetUnstabledPetNumber();
		if( petnum && TO_PLAYER(m_target)->hasqueuedpet )
		{
			//unstable selected pet
			PlayerPet * pPet = NULL;
			pPet = TO_PLAYER(m_target)->GetPlayerPet(petnum);
			if( TO_PLAYER(m_target) != NULL && pPet != NULL )
			{
				TO_PLAYER(m_target)->SpawnPet(petnum);
				pPet->stablestate = STABLE_STATE_ACTIVE;
			}
			TO_PLAYER(m_target)->hasqueuedpet = false;
		}

		//m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	}
}

void Aura::SpellAuraModDamageTakenPctPerCaster(bool apply)
{
	if(!m_target->IsUnit())
		return;
	
	if(apply)
	{
		m_target->DamageTakenPctModPerCaster.insert(
			make_pair(m_casterGuid, make_pair(m_spellProto->EffectSpellClassMask[mod->i], mod->m_amount)));
	} else
	{
		Unit::DamageTakenPctModPerCasterType::iterator it = 
			m_target->DamageTakenPctModPerCaster.find(m_casterGuid);
		while(it != m_target->DamageTakenPctModPerCaster.end() && 
			it->first == m_casterGuid)
		{
			if(it->second.first == m_spellProto->EffectSpellClassMask[mod->i])
			{
				it = m_target->DamageTakenPctModPerCaster.erase(it);
			} else
			{
				it++;
			}
		}
	}
}

void Aura::SpellAuraModDamagePercDone(bool apply)
{
	SpecialCases();
	float val = (apply) ? mod->m_amount/100.0f : -mod->m_amount/100.0f;

	switch (GetSpellId()) //dirty or mb not fix bug with wand specializations
	{
	case 6057:
	case 6085:
	case 14524:
	case 14525:
	case 14526:
	case 14527:
	case 14528:
		return;
	}

	if(m_target->IsPlayer())
	{

		//126 == melee,
		//127 == evrything
		//else - schools

		if(GetSpellProto()->EquippedItemClass==-1)//does not depend on weapon
		{
			for(uint32 x=0;x<7;x++)
			{
				if (mod->m_miscValue & (((uint32)1)<<x) )
				{
					m_target->ModFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + x,val);
				}
			}
		}
		else
		{
			// We're modifying conditional weapon damage.
			for( uint32 t = 0; t < 21; t++ )
				if( m_spellProto->EquippedItemSubClass & ( ( ( uint32 )1 ) << t ) )
				{
					// t is a subclass we're modifying.
					TO_PLAYER(m_target)->m_WeaponSubClassDamagePct[t] += val;
				}
		}
	}
	else 
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				TO_CREATURE(m_target)->ModDamageDonePct[x] += val;
			}
		}
	}
	m_target->CalcDamage();
}

void Aura::SpellAuraModPercStat(bool apply)	
{
	int32 val;
	if(apply)
	{
		val = mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();
	}
	else 
		val= -mod->m_amount;

	if (mod->m_miscValue == -1)//all stats
	{
		if(m_target->IsPlayer())
		{
			for(uint32 x=0;x<5;x++)
			{
				if(mod->m_amount>0) 
					TO_PLAYER( m_target )->StatModPctPos[x] += val; 
				else  
					TO_PLAYER( m_target )->StatModPctNeg[x] -= val;
		
				TO_PLAYER( m_target )->CalcStat(x);	
			}

			TO_PLAYER( m_target )->UpdateStats();
			TO_PLAYER( m_target )->UpdateChances();
		}
		else 
		{
			for(uint32 x=0;x<5;x++)
			{
				TO_CREATURE(m_target)->StatModPct[x] += val;
				TO_CREATURE(m_target)->CalcStat(x);			
			}
		}
	} 
	else
	{
		ASSERT(mod->m_miscValue < 5);
		if(m_target->IsPlayer())
		{
			if(mod->m_amount>0) 
				TO_PLAYER( m_target )->StatModPctPos[mod->m_miscValue] += val; 
			else  
				TO_PLAYER( m_target )->StatModPctNeg[mod->m_miscValue] -= val;

			TO_PLAYER( m_target )->CalcStat(mod->m_miscValue);	
		
			TO_PLAYER( m_target )->UpdateStats();
			TO_PLAYER( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			TO_CREATURE(m_target)->StatModPct[mod->m_miscValue]+=val;
			TO_CREATURE(m_target)->CalcStat(mod->m_miscValue);
		}
	}
}

void Aura::SpellAuraSplitDamage(bool apply)
{
	DamageSplitTarget *ds = NULL;
	Unit * m_caster = GetUnitCaster();
	if( m_target == NULL || !m_target->IsUnit() || m_caster == NULL)
		return;

	ds = m_spellProto->Id == 19028 ? &m_caster->m_damageSplitTarget : &m_target->m_damageSplitTarget;

	if(apply)
	{
		ds->m_flatDamageSplit = 0;
		ds->m_spellId = GetSpellProto()->Id;
		ds->m_pctDamageSplit = mod->m_amount / 100.0f;
		ds->damage_type = mod->m_miscValue;
		ds->m_target = m_spellProto->Id == 19028 ? m_target->GetGUID() : m_casterGuid;
	}

	ds->active = apply;
}

void Aura::SpellAuraModRegen(bool apply)
{
	if(apply)//seems like only positive
	{
		SetPositive ();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1,(uint32)((this->GetSpellProto()->EffectBasePoints[mod->i]+1)/5)*3,
			EVENT_AURA_PERIODIC_REGEN,3000,0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::SpellAuraDrinkNew(bool apply)
{
	// what the fuck?
	if( m_spellProto->NameHash == SPELL_HASH_DRINK )
	{
		if( apply )
		{
			SetPositive();
			sEventMgr.AddEvent(this, &Aura::EventPeriodicDrink, uint32(float2int32(float(mod->m_amount)/5.0f)),
				EVENT_AURA_PERIODIC_REGEN, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		return;
	}

	if( apply && m_spellProto->NameHash == SPELL_HASH_CHAINS_OF_ICE )
	{
		mod->fixed_amount[0] = 0;
		sEventMgr.AddEvent( this, &Aura::EventPeriodicSpeedModify, int32(10), EVENT_AURA_PERIODIC_ENERGIZE, 1000, 10, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else if( !apply && m_spellProto->NameHash == SPELL_HASH_CHAINS_OF_ICE )
	{
		sEventMgr.RemoveEvents( this, EVENT_AURA_PERIODIC_ENERGIZE ); 
		EventPeriodicSpeedModify( -(mod->fixed_amount[0]) ); 
	}

	if( m_spellProto->NameHash == SPELL_HASH_DEATH_RUNE_MASTERY && m_target->IsPlayer())
	{
		if(apply)
		{
			uint32 chance = 33;
			chance *= m_spellProto->RankNumber;
			TO_PLAYER(m_target)->m_deathRuneMasteryChance = m_spellProto->RankNumber == 3 ? 100 : chance;
		}
		else
			TO_PLAYER(m_target)->m_deathRuneMasteryChance = 0;
	}
	if( m_spellProto->NameHash == SPELL_HASH_STEAL_FLESH )
	{
		Unit * m_caster = GetUnitCaster();
		if( m_target->IsPlayer() && m_caster != NULL && m_caster->IsCreature() )
		{//PLAYER_FIELD_MOD_DAMAGE_DONE_POS
			for(uint32 x=0;x<7;x++)
			{
				TO_CREATURE(m_caster)->ModDamageDonePct[x] += apply ? 0.75f : -0.75f;
				TO_PLAYER(m_target)->ModFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, apply ? -0.75f : 0.75f );
			}
		}
	}
}

void Aura::EventPeriodicSpeedModify(int32 modifier)
{
	m_target->m_speedModifier += modifier;
	m_target->UpdateSpeed();

	if( m_spellProto->NameHash == SPELL_HASH_CHAINS_OF_ICE )
	{
		this->mod->fixed_amount[0] += modifier;
	}
}

void Aura::EventPeriodicDrink(uint32 amount)
{
	uint32 v = m_target->GetUInt32Value(UNIT_FIELD_POWER1) + amount;
	if( v > m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1) )
		v = m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	m_target->SetUInt32Value(UNIT_FIELD_POWER1, v);
	SendPeriodicAuraLog(amount, FLAG_PERIODIC_ENERGIZE);
}

void Aura::EventPeriodicHeal1(uint32 amount)
{
	if(m_target == NULL || m_target->GetMapMgr() == NULL )
		return;

	if(!m_target->isAlive())
		return;

	uint32 ch = m_target->GetUInt32Value(UNIT_FIELD_HEALTH);
	ch+=amount;
	uint32 mh = m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	
	if(ch>mh)
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH,mh);
	else
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH,ch);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
	else
	{
		if(!(m_spellProto->buffType & SPELL_TYPE_ARMOR))
			SendPeriodicAuraLog(amount, FLAG_PERIODIC_HEAL);
	}
}

void Aura::SpellAuraModPowerRegen(bool apply)
{
	if(!mod->m_amount)
		return;

	// Warrior: Anger Management
	if(m_target->IsPlayer() && m_spellProto->Id == 12296)
		TO_PLAYER(m_target)->mAngerManagement = !TO_PLAYER(m_target)->mAngerManagement;

	if(apply)
	{
		if (mod->m_amount>0)
			SetPositive();
		else
			SetNegative();
	}	

	int32 val = (apply) ? mod->m_amount: -mod->m_amount;
	if ( m_target->IsPlayer() )
	{
		if( mod->m_miscValue == POWER_TYPE_MANA )
		{
			TO_PLAYER( m_target )->m_ModInterrMRegen +=val;
			TO_PLAYER( m_target )->UpdateStats();
		}
	}
}

void Aura::SpellAuraChannelDeathItem(bool apply)
{
	if(apply)
	{
		//dont need for now
	}
	else
	{
		if(m_target->GetTypeId() == TYPEID_UNIT && TO_CREATURE(m_target)->GetCreatureInfo())
		{
			if(TO_CREATURE(m_target)->GetCreatureInfo()->Type != CRITTER)
			{
				if(m_target->isDead())
				{
					Player* pCaster = m_target->GetMapMgr()->GetPlayer((uint32)m_casterGuid);
					if(!pCaster)
						return;
				
					uint32 itemid = GetSpellProto()->EffectItemType[mod->i];
					if(itemid == 6265 && int32(pCaster->getLevel() - m_target->getLevel()) > 9)
						return;

					ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);
					if(pCaster->GetItemInterface()->CalculateFreeSlots(proto) > 0)
					{
						Item* item = objmgr.CreateItem(itemid,pCaster);
						if(!item) return;


						item->SetUInt64Value(ITEM_FIELD_CREATOR,pCaster->GetGUID());
						if(!pCaster->GetItemInterface()->AddItemToFreeSlot(item))
						{
							pCaster->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
							item->Destructor();
							return;
						}
						/*WorldPacket data(45);
						pCaster->GetSession()->BuildItemPushResult(&data, pCaster->GetGUID(), 1, 1, itemid ,0,0xFF,1,0xFFFFFFFF);
						pCaster->SendMessageToSet(&data, true);					*/
						SlotResult * lr = pCaster->GetItemInterface()->LastSearchResult();
						pCaster->GetSession()->SendItemPushResult(item,true,false,true,true,lr->ContainerSlot,lr->Slot,1);
					}
				}
			}
		}
	}
}

void Aura::SpellAuraModDamagePercTaken(bool apply)
{
	float val;
	if( apply )
	{
		val = mod->m_amount / 100.0f;
		if( val <= 0 )
			SetPositive();
		else
			SetNegative();
	}
	else
	{
		val= -mod->m_amount / 100.0f;
	}
	
	for(uint32 x = 0; x < 7; x++)
	{
		if( mod->m_miscValue & (((uint32)1) << x ) )
		{
			m_target->DamageTakenPctMod[x] += val;
		}
	}
}

void Aura::SpellAuraModRegenPercent(bool apply)
{
	if( apply )
		m_target->PctRegenModifier += mod->m_amount;
	else
		m_target->PctRegenModifier -= mod->m_amount;
}

void Aura::SpellAuraPeriodicDamagePercent(bool apply)
{
	if( apply )
	{
		//uint32 gr = GetSpellProto()->SpellGroupType;
		//if(gr&& m_caster!=NULL)
		//{
		//	SM_FIValue(m_caster->SM[SMT_SPELL_VALUE_PCT][0],(int32*)&dmg,gr);
		//	SM_PIValue(m_caster->SM[SMT_SPELL_VALUE_PCT][1],(int32*)&dmg,gr);
		//}		

		/*if(m_spellProto->Id == 28347) //Dimensional Siphon
		{
			uint32 dmg = (m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*5)/100;
			sEventMgr.AddEvent(this, &Aura::EventPeriodicDamagePercent, dmg, 
				EVENT_AURA_PERIODIC_DAMAGE_PERCENT, 1000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		else*/
		{
			uint32 dmg = mod->m_amount;
			uint32 time = 3000;

			if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
				time = GetSpellProto()->EffectAmplitude[mod->i];

			sEventMgr.AddEvent(this, &Aura::EventPeriodicDamagePercent, dmg, 
				EVENT_AURA_PERIODIC_DAMAGE_PERCENT, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		SetNegative();
	}
}

void Aura::EventPeriodicDamagePercent(uint32 amount)
{
	//DOT
	if(!m_target->isAlive())
		return;
	if(m_target->SchoolImmunityList[GetSpellProto()->School])
		return;

	uint32 damage = float2int32(amount/100.0f*m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	Unit * m_caster = GetUnitCaster();
	if(m_caster!=NULL)
		m_caster->SpellNonMeleeDamageLog(m_target, GetSpellProto()->Id, damage, pSpellId==0, true);
	else
		m_target->SpellNonMeleeDamageLog(m_target, GetSpellProto()->Id, damage, pSpellId==0, true);
}

void Aura::SpellAuraModResistChance(bool apply)
{
	apply ? m_target->m_resistChance = mod->m_amount : m_target->m_resistChance = 0;
}

void Aura::SpellAuraModDetectRange(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL)
		return;

	if(apply)
	{
		SetNegative();
		m_caster->setDetectRangeMod(m_target->GetGUID(), mod->m_amount);
	}
	else
	{
		m_caster->unsetDetectRangeMod(m_target->GetGUID());
	}
}

void Aura::SpellAuraPreventsFleeing(bool apply)
{
	// Curse of Recklessness 
}

void Aura::SpellAuraModUnattackable(bool apply)
{
/*
		Also known as Apply Aura: Mod Uninteractible
		Used by: Spirit of Redemption, Divine Intervention, Phase Shift, Flask of Petrification
		It uses one of the UNIT_FIELD_FLAGS, either UNIT_FLAG_NOT_SELECTABLE or UNIT_FLAG_NOT_ATTACKABLE_2 
*/
}

void Aura::SpellAuraInterruptRegen(bool apply)
{
	if(apply)
		m_target->m_interruptRegen++;
	else
	{
		m_target->m_interruptRegen--;
		  if(m_target->m_interruptRegen < 0)
			m_target->m_interruptRegen = 0;
	}
}

void Aura::SpellAuraGhost(bool apply)
{
	if(m_target->IsPlayer())
	{
		SpellAuraWaterWalk( apply );
		m_target->m_invisible = apply;

		if(apply)
		
			SetNegative(2);//we picked up one extra from calling SpellAuraWaterWalk ;)
		
	}
	//m_target->SendPowerUpdate();
}

void Aura::SpellAuraMagnet(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	// grounding totem
	// redirects one negative aura to the totem
	
	if( apply )
		TO_PLAYER(m_target)->m_magnetAura = this;
	else
		TO_PLAYER(m_target)->m_magnetAura = NULL;
}

void Aura::SpellAuraManaShield(bool apply)
{
	if(apply)
	{
		SetPositive();
		m_target->m_manashieldamt = mod->m_amount ;
		m_target->m_manaShieldSpell = GetSpellProto();
	}
	else
	{
		m_target->m_manashieldamt = 0;
		m_target->m_manaShieldSpell = 0;
	}
}

void Aura::SpellAuraSkillTalent(bool apply)
{
	if (m_target->IsPlayer())
	{
		if(apply)
		{   
			SetPositive();
			TO_PLAYER( m_target )->_ModifySkillBonus(mod->m_miscValue,mod->m_amount);
		}
		else  
			TO_PLAYER( m_target )->_ModifySkillBonus(mod->m_miscValue,-mod->m_amount);
	   
		TO_PLAYER( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModAttackPower(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else
		SetPositive();
	m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS,apply? mod->m_amount : -mod->m_amount);
	m_target->CalcDamage();
}

void Aura::SpellAuraVisible(bool apply)
{
	//Show positive spells on target
	if(apply)
	{
		SetNegative();
	}
}

void Aura::SpellAuraModResistancePCT(bool apply)
{
	uint32 Flag = mod->m_miscValue; 
	int32 amt;
	if(apply)
	{
		amt=mod->m_amount;
	 //   if(amt>0)SetPositive();
	   // else SetNegative();
	}
	else 
		amt= -mod->m_amount;  
	  
	for(uint32 x=0;x<7;x++)
	{
		if(Flag & (((uint32)1)<< x))
		{
			if(m_target->IsPlayer())
			{
				if(mod->m_amount>0)
				{
					TO_PLAYER( m_target )->ResistanceModPctPos[x] += amt;
				}
				else
				{
					TO_PLAYER( m_target )->ResistanceModPctNeg[x] -= amt;
				}
				TO_PLAYER( m_target )->CalcResistance(x);
			  
			}
			else if(m_target->GetTypeId() == TYPEID_UNIT)
			{
				TO_CREATURE(m_target)->ResistanceModPct[x] += amt;
				TO_CREATURE(m_target)->CalcResistance(x);
			}
		}
	}	
}

void Aura::SpellAuraModCreatureAttackPower(bool apply)
{
	if( apply )
	{
		for( uint32 x = 0; x < 11; x++ )
			if( mod->m_miscValue & ( ( ( uint32 )1 ) << x ) )
				m_target->CreatureAttackPowerMod[x+1] += mod->m_amount;

		if( mod->m_amount > 0 )
			SetPositive();
		else
			SetNegative();
	}
	else
	{
		for( uint32 x = 0; x < 11; x++ )
		{
			if( mod->m_miscValue & ( ( ( uint32 )1 ) << x ) )
			{
				m_target->CreatureAttackPowerMod[x+1] -= mod->m_amount;
			}
		}
	}
	m_target->CalcDamage();
}

void Aura::SpellAuraModTotalThreat( bool apply )
{
	if( apply )
	{
		if( mod->m_amount < 0 )
			SetPositive();
		else
			SetNegative();

		if( m_target->GetThreatModifier() > mod->m_amount ) // replace old mod
		{
			m_target->ModThreatModifier( -m_target->GetThreatModifier() );
			m_target->ModThreatModifier( mod->m_amount );
		}
	}
	else
	{
		if( m_target->GetThreatModifier() == mod->m_amount ) // only remove it if it hasn't been replaced yet
		{
			m_target->ModThreatModifier(-(mod->m_amount));
		}
	}
}

void Aura::SpellAuraWaterWalk( bool apply )
{	 
	if( m_target->IsPlayer() )
	{
		if( m_spellProto->NameHash == SPELL_HASH_SPRINT )
		{//rogues sprint, water walk with glyph
			if( !m_target->HasDummyAura(SPELL_HASH_GLYPH_OF_BLURRED_SPEED) )
				return;
		}

		WorldPacket data( 12 ); 
		if( apply )
		{
			SetPositive();
			data.SetOpcode( SMSG_MOVE_WATER_WALK );
			data << m_target->GetNewGUID();
			data << uint32( 8 );
			TO_PLAYER( m_target )->m_isWaterWalking++;
		}
		else
		{
			data.SetOpcode( SMSG_MOVE_LAND_WALK );
			data << m_target->GetNewGUID();
			data << uint32( 4 );		
			TO_PLAYER( m_target )->m_isWaterWalking--;
		}
		TO_PLAYER( m_target )->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraFeatherFall( bool apply )
{
	if( !m_target->IsPlayer() )
		return;

	WorldPacket data(12); 
	data.SetOpcode(apply ? SMSG_MOVE_FEATHER_FALL : SMSG_MOVE_NORMAL_FALL);
	data << m_target->GetNewGUID() << (uint32)0;
	TO_PLAYER( m_target )->GetSession()->SendPacket( &data );
	TO_PLAYER(m_target)->m_fallDisabledUntil = getMSTime() + GetDuration();
}

void Aura::SpellAuraHover( bool apply )
{
	SetPositive();
	  
	//TODO: FIXME: Find true flag for this
	if (m_target->IsPlayer())
	{
		WorldPacket data( apply ? SMSG_MOVE_SET_HOVER : SMSG_MOVE_UNSET_HOVER, 13 );
		data << m_target->GetNewGUID();
		data << uint32( 0 );
		TO_PLAYER( m_target )->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraAddPctMod( bool apply )
{
	if(!m_target)
		return;

	int32 val = apply ? mod->m_amount : -mod->m_amount;
	uint32* AffectedGroups = GetSpellProto()->EffectSpellClassMask[mod->i];
	if( AffectedGroups == 0 )
	{
		OUT_DEBUG("spell %u is missing affected groups.\n", m_spellProto->Id);
		return;
	}
	//printf("!!! the AffectedGroups %u ,the smt type %u,\n",AffectedGroups,mod->m_miscValue);

	int32 modifier = mod->m_miscValue;

	if(modifier < 0 || modifier >= SPELL_MODIFIERS)
	{
		OUT_DEBUG( "Unknown spell modifier type %u in spell %u.<<--report this line to the developer\n", modifier, GetSpellId() );
		return;
	}

	SendModifierLog(&m_target->SM[modifier][1], val, AffectedGroups, modifier, true);
}


void Aura::SendModifierLog( int32** m, int32 v, uint32 *mask, uint8 type, bool pct )
{
	packetSMSG_SET_FLAT_SPELL_MODIFIER data;
	uint32 intbit = 0, groupnum = 0;
	if( *m == 0 )
	{
		*m = new int32[SPELL_GROUPS];
		for(uint32 bit = 0; bit < SPELL_GROUPS; ++bit, ++intbit)
		{
			if(intbit == 32)
			{
				++groupnum;
				intbit = 0;
			}
			if( ( 1 << intbit ) & mask[groupnum] )
			{
				(*m)[bit] = v;

				if( !m_target->IsPlayer() )
					continue;

				data.group = bit;
				data.type = type;
				data.v = v;
				TO_PLAYER(m_target)->GetSession()->OutPacket( SMSG_SET_FLAT_SPELL_MODIFIER+ pct, sizeof( packetSMSG_SET_FLAT_SPELL_MODIFIER ), &data );
			}
			else
				(*m)[bit] = 0;
		}
	}
	else
	{
		for(uint32 bit = 0; bit < SPELL_GROUPS; ++bit,++intbit)
		{
			if(intbit == 32)
			{
				++groupnum;
				intbit = 0;
			}
			if( ( 1 << intbit ) & mask[groupnum] )
			{
				(*m)[bit] += v;

				if( !m_target->IsPlayer() )
					continue;

				data.group = bit;
				data.type = type;
				data.v = (*m)[bit];
				TO_PLAYER(m_target)->GetSession()->OutPacket( SMSG_SET_FLAT_SPELL_MODIFIER+ pct, sizeof( packetSMSG_SET_FLAT_SPELL_MODIFIER ), &data );
			}
		}
	}
}

void Aura::SendDummyModifierLog( std::map< SpellEntry*, uint32 >* m, SpellEntry* spellInfo, uint32 i, bool apply, bool pct )
{
	//WorldPacket data( SMSG_SET_FLAT_SPELL_MODIFIER + pct, 6 );
	packetSMSG_SET_FLAT_SPELL_MODIFIER data;

	int32 v = spellInfo->EffectBasePoints[i] + 1;
	uint32* mask = spellInfo->EffectSpellClassMask[i];
	uint8 type = spellInfo->EffectMiscValue[i];

	if(apply)
	{
		m->insert(make_pair(spellInfo,i));
	}
	else
	{
		v = -v;
		std::map<SpellEntry*,uint32>::iterator itr = m->find(spellInfo);
		if (itr != m->end())
			m->erase(itr);
	}

	if(!m_target->IsPlayer()) return;

	uint32 intbit = 0, groupnum = 0;
	for(uint32 bit = 0; bit < SPELL_GROUPS; ++bit, ++intbit)
	{
		if(intbit == 32)
		{
			++groupnum;
			intbit = 0;
		}
		if( ( 1 << intbit ) & mask[groupnum] )
		{

			data.group = bit;
			data.type = type;
			data.v = v;
			TO_PLAYER(m_target)->GetSession()->OutPacket( SMSG_SET_FLAT_SPELL_MODIFIER+ pct, sizeof( packetSMSG_SET_FLAT_SPELL_MODIFIER ), &data );
		}
	}
}

void Aura::SpellAuraAddTargetTrigger(bool apply)
{
#ifdef NEW_PROCFLAGS
	if (m_target == NULL)
		return;

	if( apply )
	{
		ProcTriggerSpell pts;
		memset(&pts, 0, sizeof(ProcTriggerSpell));
		pts.parentId = GetSpellProto()->Id;
		pts.caster = m_casterGuid;
		pts.procCharges = GetSpellProto()->procCharges;
		pts.i = mod->i;
		pts.LastTrigger = 0;

		if(GetSpellProto()->EffectTriggerSpell[mod->i])
			pts.spellId=GetSpellProto()->EffectTriggerSpell[mod->i];
		else
		{
			OUT_DEBUG("Warning,trigger spell is null for spell %u",GetSpellProto()->Id);
			return;
		}
		m_target->m_procSpells.push_front(pts);
		DEBUG_LOG("Aura","%u is registering %u chance %u flags %u charges %u triggeronself %u interval %u",pts.origId,pts.spellId,pts.procChance,m_spellProto->procFlags & ~PROC_TARGET_SELF,m_spellProto->procCharges,m_spellProto->procFlags & PROC_TARGET_SELF,m_spellProto->proc_interval);
	}
	else
	{
		for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
		{
			if(itr->parentId == GetSpellId() && itr->caster == m_casterGuid && !itr->deleted)
			{
				itr->deleted = true;
				break; //only 1 instance of a proc spell per caster ?
			}
		}
	}
#endif
}

void Aura::SpellAuraModPowerRegPerc(bool apply)
{
	if(apply)
	{
		mod->fixed_float_amount[0] = m_target->PctPowerRegenModifier[mod->m_miscValue] * ((float)(mod->m_amount))/100.0f;
		m_target->PctPowerRegenModifier[mod->m_miscValue] += mod->fixed_float_amount[0];
	}
	else
		m_target->PctPowerRegenModifier[mod->m_miscValue] -= mod->fixed_float_amount[0];
	
	if (m_target->IsPlayer())
		TO_PLAYER( m_target )->UpdateStats();
}

void Aura::SpellAuraOverrideClassScripts(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL || !m_caster->IsPlayer())
		return;

	//misc value is spell to add
	//spell familyname && grouprelation

	Player* plr = TO_PLAYER(m_caster);

	//Adding bonus to effect
	switch(mod->m_miscValue)
	{
		case 4919:
		case 4920:	//Molten Fury
			{
				if(m_target)
					TO_PLAYER( m_target )->m_moltenFuryDamageIncreasePct += (apply) ? mod->m_amount : -mod->m_amount;
			}break;
			//----Shatter---
			// Increases crit chance against rooted targets by (Rank * 10)%.
		case 849:
		case 910:
		case 911:
		case 912:
		case 913:
			if (m_target->IsPlayer())
			{
				int32 val = (apply) ? (mod->m_miscValue-908)*10 : -(mod->m_miscValue-908)*10;
				if (mod->m_miscValue==849)
					val = (apply) ? 10 : -10;
				TO_PLAYER( m_target )->m_RootedCritChanceBonus += val;
			}
			break;
// ----?
		case 3736:
		case 4415:
		case 4418:
		case 4554:
		case 4555:
		case 4953:
		case 5142:
		case 5147:
		case 5148:
			{
			if(apply)
			{
				OverrideIdMap::iterator itermap = objmgr.mOverrideIdMap.find(mod->m_miscValue);
				if( itermap == objmgr.mOverrideIdMap.end() )
					return;

				std::list<SpellEntry *>::iterator itrSE = itermap->second->begin();

				SpellOverrideMap::iterator itr = plr->mSpellOverrideMap.find((*itrSE)->Id);
				
				if(itr != plr->mSpellOverrideMap.end())
				{
					ScriptOverrideList::iterator itrSO;
					for(itrSO = itr->second->begin(); itrSO != itr->second->end(); ++itrSO)
					{
						if((*itrSO)->id == (uint32)mod->m_miscValue)
						{
							if((int32)(*itrSO)->damage > mod->m_amount)
							{
								(*itrSO)->damage = mod->m_amount;
							}
							return;
						}
					}
					classScriptOverride *cso = new classScriptOverride;
					cso->aura = 0;
					cso->damage = mod->m_amount;
					cso->effect = 0;
					cso->id = mod->m_miscValue;
					itr->second->push_back(cso);
				}
				else
				{
					classScriptOverride *cso = new classScriptOverride;
					cso->aura = 0;
					cso->damage = mod->m_amount;
					cso->effect = 0;
					cso->id = mod->m_miscValue;
					ScriptOverrideList *lst = new ScriptOverrideList();
					lst->push_back(cso);
					
					for(;itrSE != itermap->second->end(); ++itrSE)
					{
						plr->mSpellOverrideMap.insert( SpellOverrideMap::value_type( (*itrSE)->Id, lst) );
					}
				}
			}
			else
			{
				OverrideIdMap::iterator itermap = objmgr.mOverrideIdMap.find(mod->m_miscValue);
				SpellOverrideMap::iterator itr = plr->mSpellOverrideMap.begin(), itr2;
				while(itr != plr->mSpellOverrideMap.end())
				{
					std::list<SpellEntry *>::iterator itrSE = itermap->second->begin();
					for(;itrSE != itermap->second->end(); ++itrSE)
					{
						if(itr->first == (*itrSE)->Id)
						{
							itr2 = itr++;
							plr->mSpellOverrideMap.erase(itr2);
							break;
						}
					}
					// Check if the loop above got to the end, if so it means the item wasn't found
					// and the itr wasn't incremented so increment it now.
					if(itrSE == itermap->second->end())      itr++;
				}
			}
		}break;
/*		case 19421: //hunter : Improved Hunter's Mark
		case 19422:
		case 19423:
		case 19424:
		case 19425:
			{
				//this shoul actually add a new functionality to the spell and not override it. There is a lot to decode and to be done here
			}break;*/
		case 4992: // Warlock: Soul Siphon
		case 4993:
			{
				if(m_target) {
					if( apply )
						m_target->m_soulSiphon.max+= mod->m_amount;
					else
						m_target->m_soulSiphon.max-= mod->m_amount;
				}
			}break;
	default:
		OUT_DEBUG("Unknown override report to devs: %u", mod->m_miscValue);
	};
}

void Aura::SpellAuraModRangedDamageTaken(bool apply)
{
	if(apply)
		m_target->RangedDamageTaken += mod->m_amount;
	else
	{
		m_target->RangedDamageTaken -= mod->m_amount;;
		if( m_target->RangedDamageTaken < 0)
			m_target->RangedDamageTaken = 0;
	}	  
}

void Aura::SpellAuraModHealing(bool apply)
{
	int32 val;
	if(apply)
	{
		 val = mod->m_amount;
		 /*if(val>0)
			 SetPositive();
		 else 
			 SetNegative();*/
	}
	else 
		val=-mod->m_amount;
	
	for(uint32 x=0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->HealTakenMod[x] += val;
		}
	}
}

void Aura::SpellAuraIgnoreRegenInterrupt(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	if(apply)
		TO_PLAYER( m_target )->PctIgnoreRegenModifier += ((float)(mod->m_amount))/100;
	else
		TO_PLAYER( m_target )->PctIgnoreRegenModifier -= ((float)(mod->m_amount))/100;
}

void Aura::SpellAuraModMechanicResistance(bool apply)
{
	//silence=26 ?
	//mecanics=9 ?
	if(apply)
	{
		assert(mod->m_miscValue < 27);
		m_target->MechanicsResistancesPCT[mod->m_miscValue]+=mod->m_amount;

		if(mod->m_miscValue != 16 && mod->m_miscValue != 25 && mod->m_miscValue != 19) // dont remove bandages, Power Word and protection effect
		{
			SetPositive();
		}
		else
		{
			SetNegative();
		}
	}
	else
		m_target->MechanicsResistancesPCT[mod->m_miscValue]-=mod->m_amount;
}

void Aura::SpellAuraModHealingPCT(bool apply)
{
	if(mod->m_amount < 0)
		SetNegative();
	else
		SetPositive();

	for(uint32 x=0; x<7; x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			if( apply )
			{
				m_target->HealTakenPctMod[x] *= (mod->m_amount/100.0f + 1);
			}
			else
				m_target->HealTakenPctMod[x] /= (mod->m_amount/100.0f + 1);
			
		}
	}
}

void Aura::SpellAuraModRangedAttackPower(bool apply)
{
	if(apply)
	{
		if(mod->m_amount > 0)
			SetPositive();
		else
			SetNegative();
		m_target->ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS,mod->m_amount);
	}
	else
		m_target->ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS,-mod->m_amount);
	m_target->CalcDamage();
}

void Aura::SpellAuraModMeleeDamageTaken(bool apply)
{
	if(apply)
	{
		if(mod->m_amount > 0)//does not exist but let it be
			SetNegative();
		else 
			SetPositive();
		m_target->DamageTakenMod[0] += mod->m_amount;
	}
	else
		m_target->DamageTakenMod[0] -= mod->m_amount;
}

void Aura::SpellAuraModMeleeDamageTakenPct(bool apply)
{
	if(apply)
	{
		if(mod->m_amount>0)//does not exist but let it be
			SetNegative();
		else 
			SetPositive();
		m_target->DamageTakenPctMod[0]+=mod->m_amount;
	}
	else
		m_target->DamageTakenPctMod[0]-=mod->m_amount;
}

void Aura::SpellAuraRAPAttackerBonus(bool apply)
{
	if(apply)
	{
		m_target->RAPvModifier += mod->m_amount;
	}
	else 
		m_target->RAPvModifier -= mod->m_amount;
}

void Aura::SpellAuraModPossessPet(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL || !m_caster->IsPlayer())
		return;

	if(TO_PLAYER(m_caster)->GetSummon() != m_target)
        return;

    if(apply)
    {
        m_caster->SetUInt64Value(PLAYER_FARSIGHT, m_target->GetGUID());
        m_target->SetFlag(UNIT_FIELD_FLAGS, 0x01000000);
    }
    else
    {
        m_caster->SetUInt64Value(PLAYER_FARSIGHT, 0);
        m_target->RemoveFlag(UNIT_FIELD_FLAGS, 0x01000000);
    }
}

void Aura::SpellAuraModIncreaseSpeedAlways(bool apply)
{
	if(apply)
	{  
		SetPositive();
		m_target->m_speedModifier += mod->m_amount;
	}
	else
		m_target->m_speedModifier -= mod->m_amount;

	m_target->UpdateSpeed();
}

void Aura::SpellAuraModIncreaseEnergyPerc( bool apply )
{
	SetPositive();
	uint32 maxField = UNIT_FIELD_MAXPOWER1 + mod->m_miscValue;

	if(apply)
	{
		mod->fixed_amount[0] = m_target->GetModPUInt32Value( maxField, mod->m_amount );
		m_target->ModUnsigned32Value( maxField, mod->fixed_amount[0] );
		if( m_target->IsPlayer() && maxField == UNIT_FIELD_MAXPOWER1 )
			TO_PLAYER( m_target )->SetManaFromSpell( TO_PLAYER(m_target )->GetManaFromSpell() + mod->fixed_amount[0] ); 
	}
	else
	{
		m_target->ModUnsigned32Value( maxField, -mod->fixed_amount[0] );
		if( m_target->IsPlayer() && maxField == UNIT_FIELD_MAXPOWER1 )
			TO_PLAYER( m_target )->SetManaFromSpell( TO_PLAYER(m_target )->GetManaFromSpell() - mod->fixed_amount[0] ); 
	}
}

void Aura::SpellAuraModIncreaseHealthPerc( bool apply )
{
	SetPositive();
	if( apply )
	{
		if( mod->m_amount < 0 )
			mod->fixed_amount[0] = m_target->GetModPUInt32Value( UNIT_FIELD_MAXHEALTH, -mod->m_amount );
		else
			mod->fixed_amount[0] = m_target->GetModPUInt32Value( UNIT_FIELD_MAXHEALTH, mod->m_amount );

		if( mod->m_amount < 0 )
			mod->fixed_amount[0] = -mod->fixed_amount[0]; // oh I love hacks :D
		m_target->ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, mod->fixed_amount[0] );
		if( m_target->IsPlayer() )
			TO_PLAYER(m_target )->SetHealthFromSpell( TO_PLAYER( m_target )->GetHealthFromSpell() + mod->fixed_amount[0] ); 
	}
	else
	{
		m_target->ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, -mod->fixed_amount[0] );
		if( m_target->IsPlayer() )
			TO_PLAYER(m_target )->SetHealthFromSpell( TO_PLAYER(m_target )->GetHealthFromSpell() - mod->fixed_amount[0] ); 
	}
}

void Aura::SpellAuraModManaRegInterrupt( bool apply )
{
	if( m_target->IsPlayer() )
	{
		if( apply )
			TO_PLAYER( m_target )->m_ModInterrMRegenPCT += mod->m_amount;
		else
			TO_PLAYER( m_target )->m_ModInterrMRegenPCT -= mod->m_amount;

		TO_PLAYER( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModTotalStatPerc(bool apply)
{
	int32 val;
	if(apply)
	{
	   val= mod->m_amount;
	}
	else 
	   val= -mod->m_amount;

	if (mod->m_miscValue == -1)//all stats
	{
		if(m_target->IsPlayer())
		{
			for(uint32 x=0;x<5;x++)
			{
				if(mod->m_amount>0)
					TO_PLAYER( m_target )->TotalStatModPctPos[x] += val; 
				else  
					TO_PLAYER( m_target )->TotalStatModPctNeg[x] -= val;
				TO_PLAYER( m_target )->CalcStat(x);	
			}

			TO_PLAYER( m_target )->UpdateStats();
			TO_PLAYER( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			for(uint32 x=0;x<5;x++)
			{
				TO_CREATURE(m_target)->TotalStatModPct[x] += val;
				TO_CREATURE(m_target)->CalcStat(x);			
			}
		}
	} 
	else
	{
		ASSERT(mod->m_miscValue < 5);
		if(m_target->IsPlayer())
		{
			//druid hearth of the wild should add more features based on form
			if( m_spellProto->NameHash == SPELL_HASH_HEART_OF_THE_WILD )
			{
				//we should remove effect first
				TO_PLAYER( m_target )->EventTalentHearthOfWildChange( false );
				//set new value
				if( apply )
					TO_PLAYER( m_target )->SetTalentHearthOfWildPCT( val );
				else
					TO_PLAYER( m_target )->SetTalentHearthOfWildPCT( 0 ); //this happens on a talent reset
				//reapply
				TO_PLAYER( m_target )->EventTalentHearthOfWildChange( true );
			}

			if( mod->m_amount > 0 )
				TO_PLAYER( m_target )->TotalStatModPctPos[mod->m_miscValue] += val; 
			else  
				TO_PLAYER( m_target )->TotalStatModPctNeg[mod->m_miscValue] -= val;		
			
			TO_PLAYER( m_target )->CalcStat( mod->m_miscValue );	
			TO_PLAYER( m_target )->UpdateStats();
			TO_PLAYER( m_target )->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			TO_CREATURE( m_target )->TotalStatModPct[mod->m_miscValue]+=val;
			TO_CREATURE( m_target )->CalcStat(mod->m_miscValue);
		}		
	}
}

void Aura::SpellAuraModHaste( bool apply )
{
	//blade flurry - attack a nearby opponent
	if( m_spellProto->NameHash == SPELL_HASH_BLADE_FLURRY )
	{
		if( apply )
			m_target->AddExtraStrikeTarget(GetSpellProto(), 0);
		else
			m_target->RemoveExtraStrikeTarget(GetSpellProto());
	}

	if( mod->m_amount < 0 )
		SetNegative();
	else 
		SetPositive();

	float baseMod = (mod->m_amount / 100.0f);

	if( m_target->IsPlayer() )
	{
		if( apply )	
		{
			mod->fixed_float_amount[0] = baseMod * TO_PLAYER(m_target)->m_meleeattackspeedmod;
			TO_PLAYER(m_target)->m_meleeattackspeedmod -= mod->fixed_float_amount[0];
		}
		else
		{
			TO_PLAYER(m_target)->m_meleeattackspeedmod += mod->fixed_float_amount[0];
		}
		TO_PLAYER(m_target)->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME, mod->m_amount );
			mod->fixed_amount[1] = m_target->GetModPUInt32Value( UNIT_FIELD_RANGEDATTACKTIME, mod->m_amount );
			mod->fixed_amount[2] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME + 1, mod->m_amount);

			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ) <= mod->fixed_amount[0] )
				mod->fixed_amount[0] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ); //watch it, a negative timer might be bad ;)
			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_RANGEDATTACKTIME ) <= mod->fixed_amount[1] )
				mod->fixed_amount[1] = m_target->GetUInt32Value ( UNIT_FIELD_RANGEDATTACKTIME );//watch it, a negative timer might be bad ;)
			if( (int32)m_target->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME + 1 ) <= mod->fixed_amount[2] )
				mod->fixed_amount[2] = m_target->GetUInt32Value( UNIT_FIELD_BASEATTACKTIME + 1 );

			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME, -mod->fixed_amount[0] );
			m_target->ModUnsigned32Value( UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[1] );
			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME + 1, -mod->fixed_amount[2] );
		}
		else
		{
			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME, mod->fixed_amount[0] );
			m_target->ModUnsigned32Value( UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[1] );
			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME + 1, mod->fixed_amount[2] );
		}
	}
}

void Aura::SpellAuraForceReaction( bool apply )
{
	if( !m_target->IsPlayer() )
		return;

	map<uint32,uint32>::iterator itr;
	Player* p_target = TO_PLAYER(m_target);

	if( apply )
	{
		itr = p_target->m_forcedReactions.find( mod->m_miscValue );
		if( itr != p_target->m_forcedReactions.end() )
			itr->second = mod->m_amount;
		else
			p_target->m_forcedReactions.insert( make_pair( mod->m_miscValue, mod->m_amount ) );
	}
	else
		p_target->m_forcedReactions.erase( mod->m_miscValue );

	WorldPacket data( SMSG_SET_FORCED_REACTIONS, ( 8 * p_target->m_forcedReactions.size() ) + 4 );
	data << uint32(p_target->m_forcedReactions.size());
	for( itr = p_target->m_forcedReactions.begin(); itr != p_target->m_forcedReactions.end(); ++itr )
	{
		data << itr->first;
		data << itr->second;
	}

	p_target->GetSession()->SendPacket( &data );
}

void Aura::SpellAuraModRangedHaste(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else
		SetPositive();

	float baseMod = mod->m_amount / 100.0f;

	if (m_target->IsPlayer())
	{
		if( apply )
		{
			mod->fixed_float_amount[0] = baseMod * TO_PLAYER(m_target)->m_rangedattackspeedmod;
		    TO_PLAYER( m_target )->m_rangedattackspeedmod -= mod->fixed_float_amount[0];
		}
		else
			TO_PLAYER( m_target )->m_rangedattackspeedmod += mod->fixed_float_amount[0];
		TO_PLAYER( m_target )->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,mod->m_amount);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[0]);
		}
		else m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[0]);
	}
}

void Aura::SpellAuraModRangedAmmoHaste(bool apply)
{
	SetPositive();
	if( !m_target->IsPlayer() )
		return;

	Player* p = TO_PLAYER( m_target );

	float baseMod = mod->m_amount / 100.0f;
	if( apply )
	{
		mod->fixed_float_amount[0] = baseMod * p->m_rangedattackspeedmod;
		p->m_rangedattackspeedmod-=mod->fixed_float_amount[0];
	}
	else
	{
		p->m_rangedattackspeedmod+=mod->fixed_float_amount[0];
	}

	p->UpdateAttackSpeed();
}

void Aura::SpellAuraModResistanceExclusive(bool apply)
{
	SpellAuraModResistance(apply);
}

void Aura::SpellAuraRetainComboPoints(bool apply)
{
	if( m_target->IsPlayer() )
		TO_PLAYER( m_target )->m_retainComboPoints = apply;
}

void Aura::SpellAuraResistPushback(bool apply)
{
	//DK:This is resist for spell casting delay
	//Only use on players for now
	
	if(m_target->IsPlayer())
	{
		int32 val;
		if(apply)
		{
			val = mod->m_amount;
			SetPositive();
		}
		else
			val=-mod->m_amount;
		
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				TO_PLAYER( m_target )->SpellDelayResist[x] += val;
			}
		}
	}
}

void Aura::SpellAuraModShieldBlockPCT( bool apply )
{
	if( m_target->IsPlayer() )
	{
		if( apply )
			TO_PLAYER(m_target)->m_modblockabsorbvalue += ( uint32 )mod->m_amount;
		else
			TO_PLAYER(m_target)->m_modblockabsorbvalue -= ( uint32 )mod->m_amount;
		TO_PLAYER(m_target)->UpdateStats();
	}
}

void Aura::SpellAuraTrackStealthed(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster== NULL || !m_caster->IsPlayer() )
		return;

	//0x00000002 is track stealthed
	if( apply )
		m_caster->SetFlag(PLAYER_FIELD_BYTES, 0x00000002);
	else
		m_caster->RemoveFlag(PLAYER_FIELD_BYTES, 0x00000002);
}

void Aura::SpellAuraModDetectedRange(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		SetPositive();
		TO_PLAYER( m_target )->DetectedRange += mod->m_amount;
	}
	else	
		TO_PLAYER( m_target )->DetectedRange -= mod->m_amount;
}

void Aura::SpellAuraSplitDamageFlat(bool apply)
{
	DamageSplitTarget *ds;

	Unit * m_caster = GetUnitCaster();
	if( m_target == NULL || !m_target->IsUnit() || m_caster == NULL )
		return;

	ds = &m_caster->m_damageSplitTarget;
	if(apply)
	{
		ds->m_flatDamageSplit = mod->m_miscValue;
		ds->m_spellId = GetSpellProto()->Id;
		ds->m_pctDamageSplit = 0;
		ds->damage_type = mod->m_type;
		ds->m_target = m_casterGuid;
//		printf("registering dmg split %u, amout= %u \n",ds->m_spellId, mod->m_amount, mod->m_miscValue, mod->m_type);
	}

	ds->active = apply;
}

void Aura::SpellAuraModStealthLevel(bool apply)
{
	if(apply)
	{
		SetPositive();
		m_target->m_stealthLevel += mod->m_amount;

		if( m_spellProto->NameHash == SPELL_HASH_VANISH && m_target->IsPlayer() ) // Vanish
		{
			// check for stealh spells
			uint32 stealth_id = 1784;
			for(SpellSet::iterator itr = TO_PLAYER(m_target)->mSpells.begin(); itr != TO_PLAYER(m_target)->mSpells.end(); ++itr)
			{
				if((*itr) == 1787 || (*itr) == 1786 || (*itr) == 1785 || (*itr) == 1784)
				{
					if( (*itr) > stealth_id )
						stealth_id = (*itr);

					if( (*itr) > 1787 )
						break;
				}
			}
			if(stealth_id)
				m_target->CastSpell(m_target, dbcSpell.LookupEntry(stealth_id), true);
		}
	}
	else 
		m_target->m_stealthLevel -= mod->m_amount;
}

void Aura::SpellAuraModUnderwaterBreathing(bool apply)
{
	if(m_target->IsPlayer())
	{
		uint32 m_UnderwaterMaxTimeSaved = TO_PLAYER( m_target )->m_UnderwaterMaxTime;
		if( apply )
			TO_PLAYER( m_target )->m_UnderwaterMaxTime *= (1 + mod->m_amount / 100 );
		else
			TO_PLAYER( m_target )->m_UnderwaterMaxTime /= (1 + mod->m_amount / 100 );
                TO_PLAYER( m_target )->m_UnderwaterTime *= TO_PLAYER( m_target )->m_UnderwaterMaxTime / m_UnderwaterMaxTimeSaved;
	}
}

void Aura::SpellAuraSafeFall(bool apply)
{
	//TODO: FIXME: Find true flag for this
	if( !m_target->IsPlayer() )
		return;

	WorldPacket data( 12 );
	if( apply )
	{
		SetPositive();
		data.SetOpcode( SMSG_MOVE_FEATHER_FALL );
		TO_PLAYER(m_target )->m_safeFall += mod->m_amount;
	}
	else 
	{
		data.SetOpcode(SMSG_MOVE_NORMAL_FALL);
		TO_PLAYER( m_target )->m_safeFall -= mod->m_amount;
	}

	data << m_target->GetNewGUID();
	data << uint32( 0 );
	TO_PLAYER( m_target )->GetSession()->SendPacket( &data );
}

void Aura::SpellAuraModReputationAdjust(bool apply)
{
	/*SetPositive();
	bool updateclient = true;
	if(IsPassive())
		updateclient = false;	 // dont update client on passive

	if(m_target->GetTypeId()==TYPEID_PLAYER)
	{
		if(apply)
			  TO_PLAYER( m_target )->modPercAllReputation(mod->m_amount, updateclient);
		  else
			TO_PLAYER( m_targe t)->modPercAllReputation(-mod->m_amount, updateclient);
	}*/

	// This is _actually_ "Reputation gains increased by x%."
	// not increase all rep by x%.

	if(m_target->IsPlayer())
	{
		SetPositive();
		if(apply)
			TO_PLAYER( m_target )->pctReputationMod += mod->m_amount;
		else
			TO_PLAYER( m_target )->pctReputationMod -= mod->m_amount;
	}
}

void Aura::SpellAuraNoPVPCredit(bool apply)
{
	if(m_target->IsPlayer())
		TO_PLAYER( m_target )->m_honorless = (apply) ? true : false;
}

void Aura::SpellAuraModHealthRegInCombat(bool apply)
{
	// demon armor etc, they all seem to be 5 sec.
	if(apply)
	{
		sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1, uint32(mod->m_amount), EVENT_AURA_PERIODIC_HEALINCOMB, 5000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicBurn(uint32 amount, uint32 misc)
{
	Unit * m_caster = GetUnitCaster();
	if( m_caster == NULL)
		return;

	if(m_target->isAlive() && m_caster->isAlive())
	{
		if(m_target->SchoolImmunityList[GetSpellProto()->School])
			return;

		uint32 field = UNIT_FIELD_POWER1 + misc;
	
		uint32 Amount = (uint32)min( amount, m_target->GetUInt32Value( field ) );
		uint32 newHealth = m_target->GetUInt32Value(field) - Amount ;
				
		SendPeriodicAuraLog(m_target, m_target, m_spellProto, Amount, 0, 0, FLAG_PERIODIC_DAMAGE);
		m_target->DealDamage(m_target, Amount, 0, 0, GetSpellProto()->Id);
	}  
}

void Aura::SpellAuraPowerBurn(bool apply)
{
	//0 mana,1 rage, 3 energy
	if(apply)
	{
		uint32 time = 3000;

		if( GetSpellProto()->EffectAmplitude[mod->i] > 0 )
			time = GetSpellProto()->EffectAmplitude[mod->i];

		sEventMgr.AddEvent(this, &Aura::EventPeriodicBurn, uint32(mod->m_amount), (uint32)mod->m_miscValue, EVENT_AURA_PERIODIC_BURN, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::SpellAuraModCritDmgPhysical(bool apply)
{
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			TO_PLAYER( m_target )->m_modphyscritdmgPCT += (uint32)mod->m_amount;
		}
		else
		{
			TO_PLAYER( m_target )->m_modphyscritdmgPCT -= (uint32)mod->m_amount;
		}
	}
}


void Aura::SpellAuraWaterBreathing( bool apply )
{
   if( m_target->IsPlayer() )
   {
	   if( apply )
	   {
			SetPositive();
			WorldPacket data( 4 );
			data.SetOpcode( SMSG_STOP_MIRROR_TIMER );
			data << uint32( 1 );
			TO_PLAYER( m_target )->GetSession()->SendPacket( &data );
			TO_PLAYER( m_target )->m_UnderwaterState = 0;			
	   }

	   TO_PLAYER( m_target )->m_bUnlimitedBreath = apply;
   }
}

void Aura::SpellAuraAPAttackerBonus(bool apply)
{
	if(apply)
	{
		m_target->APvModifier += mod->m_amount;
	}
	else 
		m_target->APvModifier -= mod->m_amount;
}


void Aura::SpellAuraModPAttackPower(bool apply)
{
	//!!probably there is a flag or something that will signal if randeg or melee attack power !!! (still missing)
	if(m_target->IsPlayer())
	{
		if(apply)
		{		
			m_target->ModFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,(float)mod->m_amount/100);
		}
		else
			m_target->ModFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,-(float)mod->m_amount/100);
		m_target->CalcDamage();
	}
}

void Aura::SpellAuraModRangedAttackPowerPct(bool apply)
{
    if(m_target->IsPlayer())
	{
		m_target->ModFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER,((apply)?1:-1)*(float)mod->m_amount/100);
        m_target->CalcDamage();
    }
}

void Aura::SpellAuraIncreaseDamageTypePCT(bool apply)
{
	if( m_target->IsPlayer() )
	{
		if(apply)
		{
			for(uint32 x = 0; x < 11; x++)
				if (mod->m_miscValue & (((uint32)1)<<x) )
					TO_PLAYER( m_target )->IncreaseDamageByTypePCT[x+1] += ((float)(mod->m_amount))/100;;
			if(mod->m_amount < 0)
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			for(uint32 x = 0; x < 11; x++)
			{
				if (mod->m_miscValue & (((uint32)1)<<x) )
					TO_PLAYER( m_target )->IncreaseDamageByTypePCT[x+1] -= ((float)(mod->m_amount))/100;;
			}
		}
	}
}

void Aura::SpellAuraIncreaseCricticalTypePCT(bool apply)
{
	if( m_target->IsPlayer() )
	{
		if(apply)
		{
			for(uint32 x = 0; x < 11; x++)
				if (mod->m_miscValue & (((uint32)1)<<x) )
					TO_PLAYER( m_target )->IncreaseCricticalByTypePCT[x+1] += ((float)(mod->m_amount))/100;
			if(mod->m_amount < 0)
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			for(uint32 x = 0; x < 11; x++)
			{
				if (mod->m_miscValue & (((uint32)1)<<x) )
					TO_PLAYER( m_target )->IncreaseCricticalByTypePCT[x+1] -= ((float)(mod->m_amount))/100;
			}
		}
	}
}

void Aura::SpellAuraIncreasePartySpeed(bool apply)
{
	if(m_target->IsPlayer() && m_target->isAlive() && m_target->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) == 0)
	{
		if(apply)
		{
			m_target->m_speedModifier += mod->m_amount;
		}
		else
		{
			m_target->m_speedModifier -= mod->m_amount;
		}
		m_target->UpdateSpeed();
	}
}

void Aura::SpellAuraIncreaseSpellDamageByAttribute(bool apply)
{
	int32 val;

	if(apply)
	{
		Unit * m_caster = GetUnitCaster();
		if(m_caster == NULL)
			return;

		val = mod->m_amount;

		if(val<0)
			SetNegative();
		else
			SetPositive();

		mod->realamount = val;
	}
	else
		val = -mod->realamount;

	uint32 stat = 3;
	for(uint32 i=0; i < 3; i++)
	{ //bit hacky but it will work with all currently available spells
		if (m_spellProto->EffectApplyAuraName[i] == SPELL_AURA_INCREASE_SPELL_HEALING_PCT)
		{
			if (m_spellProto->EffectMiscValue[i] < 5)
				stat = m_spellProto->EffectMiscValue[i];
			else
				return;
		}
	}

	if(m_target->IsPlayer())
	{	
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				if( apply )
				{
					mod->fixed_amount[x] = float2int32(((float)val/100)*m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat));
					m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, mod->fixed_amount[x] );
				}
				else
					m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, -mod->fixed_amount[x] );
				
				TO_PLAYER( m_target )->SpellDmgDoneByAttribute[stat][x] += ((float)(val))/100;
			}
		}
		if(m_target->IsPlayer())
			TO_PLAYER( m_target )->UpdateChanceFields();
	}
}

void Aura::SpellAuraIncreaseHealingByAttribute(bool apply)
{
	int32 val;
	if(apply)
	{
		Unit * m_caster = GetUnitCaster();
		if(m_caster == NULL)
			return;

		val = mod->m_amount;
		SM_FIValue(m_caster->SM[SMT_SECOND_EFFECT_BONUS][0], &val, m_spellProto->SpellGroupType);

		if(val<0)
			SetNegative();
		else
			SetPositive();

		mod->realamount = val;
	}
	else
		val = -mod->realamount;

	uint32 stat;
	if (mod->m_miscValue < 5)
		stat = mod->m_miscValue;
	else
	{
		OUT_DEBUG(
			"Aura::SpellAuraIncreaseHealingByAttribute::Unknown spell attribute type %u in spell %u.\n",
			mod->m_miscValue,GetSpellId());
		return;
	}

	if(m_target->IsPlayer())
	{
		for( uint32 x = 1; x < 7; x++ )
		{
			TO_PLAYER( m_target )->SpellHealDoneByAttribute[stat][x] += ((float)(val))/100;
		}
		if(m_target->IsPlayer())
		{
			TO_PLAYER( m_target )->UpdateChanceFields();
			if( apply )
			{
				mod->fixed_amount[0] = float2int32(((float)val/100)*m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat));
				m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, mod->fixed_amount[0]);
			}
			else
				m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, -mod->fixed_amount[0]);
		}
	}
}

void Aura::SpellAuraAddFlatModifier(bool apply)
{
	if(!m_target)
		return;
	int32 val = apply?mod->m_amount:-mod->m_amount;
	uint32* AffectedGroups = GetSpellProto()->EffectSpellClassMask[mod->i];
	if( AffectedGroups[0] == 0 && AffectedGroups[1] == 0 && AffectedGroups[2] == 0 )
	{
		OUT_DEBUG("spell %u is missing affected groups.\n", m_spellProto->Id);
		return;
	}
	//printf("!!! the AffectedGroups %u ,the smt type %u,\n",AffectedGroups,mod->m_miscValue);
	
	int32 modifier = mod->m_miscValue;

	if(modifier < 0 || modifier >= SPELL_MODIFIERS)
	{
		OUT_DEBUG( "Unknown spell modifier type %u in spell %u.<<--report this line to the developer\n", modifier, GetSpellId() );
		return;
	}

	SendModifierLog(&m_target->SM[modifier][0],val,AffectedGroups,modifier);
}


void Aura::SpellAuraModHealingDone(bool apply)
{
	int32 val = 0;

	if( m_spellProto->NameHash == SPELL_HASH_DIVINE_SPIRIT ||
		m_spellProto->NameHash == SPELL_HASH_PRAYER_OF_SPIRIT )
	{
		Unit * m_caster = GetUnitCaster();
		if( m_caster != NULL && m_caster->HasDummyAura(SPELL_HASH_IMPROVED_DIVINE_SPIRIT) )
		{
			val += m_spellProto->EffectBasePoints[0] / m_caster->GetDummyAura(SPELL_HASH_IMPROVED_DIVINE_SPIRIT)->RankNumber == 1 ? 2 : 1;
			if( !apply ) val = -val;
		}
	}

	if(apply)
	{
		val += mod->m_amount;
		if(val < 0)
			SetNegative();
		else
			SetPositive();
	}
	else 
		val += -mod->m_amount;

	for(uint32 x=0; x<7; x++)
	{
		if (mod->m_miscValue  & (((uint32)1)<<x) )
		{
			m_target->HealDoneMod[x] += val;
		}
	}
	if(m_target->IsPlayer())
	{
		TO_PLAYER( m_target )->UpdateChanceFields();
		m_target->SetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, m_target->GetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS) + val);
	}
}

void Aura::SpellAuraModHealingDonePct(bool apply)
{
	SpecialCases();
	int32 val;
	if(apply)
	{
		val=mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();
	}
	else 
		val=-mod->m_amount;

	for(uint32 x=0; x<7; x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			if( apply )
			{
				m_target->HealDonePctMod[x] *= (mod->m_amount/100.0f + 1);
			}
			else
				m_target->HealDonePctMod[x] /= (mod->m_amount/100.0f + 1);
			
		}
	}
}

void Aura::SpellAuraEmphaty(bool apply)
{
	SetPositive();
	Unit * m_caster = GetUnitCaster();
	if(m_caster == NULL || m_target == NULL || !m_caster->IsPlayer())
		return;

	// Show extra info about beast
	uint32 dynflags = m_target->GetUInt32Value(UNIT_DYNAMIC_FLAGS);
	if(apply)
		dynflags |= U_DYN_FLAG_PLAYER_INFO;

	m_target->BuildFieldUpdatePacket(TO_PLAYER(m_caster), UNIT_DYNAMIC_FLAGS, dynflags);
}

void Aura::SpellAuraUntrackable(bool apply)
{
    if(apply)
        m_target->SetFlag(UNIT_FIELD_BYTES_1, 0x04000000);
    else
        m_target->RemoveFlag(UNIT_FIELD_BYTES_1, 0x04000000);
}

void Aura::SpellAuraModOffhandDamagePCT(bool apply)
{
	//Used only by talents of rogue and warrior;passive,positive
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			SetPositive();
			TO_PLAYER( m_target )->offhand_dmg_mod *= (100+mod->m_amount)/100.0f;
		}
		else
			TO_PLAYER( m_target )->offhand_dmg_mod /= (100+mod->m_amount)/100.0f;

		m_target->CalcDamage();
	}
}

void Aura::SpellAuraModPenetration(bool apply) // armor penetration & spell penetration
{
	if( m_spellProto->NameHash == SPELL_HASH_SERRATED_BLADES )
	{
		if(!m_target->IsPlayer())
			return;

		Player* plr = TO_PLAYER(m_target);
		mod->realamount = m_target->getLevel();
		float fl = (float)m_target->getLevel();

		if( m_spellProto->Id == 14171 )
			fl = float( fl * 2.67 );
		else if( m_spellProto->Id == 14172 )
			fl = float( fl * 5.43 );
		else if( m_spellProto->Id == 14173 )
			fl = float( fl * 8 );

		if( apply )
		{
			plr->PowerCostPctMod[0] += fl;
			plr->ModUnsigned32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, float2int32(fl));
		}else
		{
			plr->PowerCostPctMod[0] -= fl;
			plr->ModUnsigned32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, float2int32(-fl));
		}

		return;
	}

	//DK:This is basicly resistance reduce but i really dont
	//know why blizz named this function like this
	if(apply)
	{
		if(mod->m_amount < 0)
			SetPositive();
		else
			SetNegative();

		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x))
				m_target->PowerCostPctMod[x] -= mod->m_amount;
		}

		if(mod->m_miscValue & 124 && m_target->IsPlayer())
			m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, mod->m_amount);
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x))
				m_target->PowerCostPctMod[x] += mod->m_amount;
		}
		if(mod->m_miscValue & 124 && m_target->IsPlayer())
			m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, -mod->m_amount);
	}
}

void Aura::SpellAuraIncreaseArmorByPctInt(bool apply)
{
	uint32 i_Int = m_target->GetUInt32Value(UNIT_FIELD_STAT3);

	int32 amt = float2int32(i_Int * ((float)mod->m_amount / 100.0f));
	amt *= (!apply) ? -1 : 1;

	for(uint32 x=0;x<7;x++)
	{
		if(mod->m_miscValue & (((uint32)1)<< x))
		{
			if(m_target->IsPlayer())
			{
				TO_PLAYER( m_target )->FlatResistanceModifierPos[x] += amt;
				TO_PLAYER( m_target )->CalcResistance(x);
			}
			else if(m_target->GetTypeId() == TYPEID_UNIT)
			{
				TO_CREATURE(m_target)->FlatResistanceMod[x] += amt;
				TO_CREATURE(m_target)->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraReduceAttackerMHitChance(bool apply)
{
	if (!m_target->IsPlayer())
		return;
	if(apply)
		TO_PLAYER( m_target )->m_resist_hit[0]+=mod->m_amount;
	else
		TO_PLAYER( m_target )->m_resist_hit[0]-=mod->m_amount;
}

void Aura::SpellAuraReduceAttackerRHitChance(bool apply)
{
	if (!m_target->IsPlayer())
		return;
	if(apply)
		TO_PLAYER( m_target )->m_resist_hit[1]+=mod->m_amount;
	else
		TO_PLAYER( m_target )->m_resist_hit[1]-=mod->m_amount;
}

void Aura::SpellAuraReduceAttackerSHitChance(bool apply)
{
	if (!m_target->IsPlayer())
		return;
	if(apply)
		TO_PLAYER( m_target )->m_resist_hit[2]-=mod->m_amount;
	else
		TO_PLAYER( m_target )->m_resist_hit[2]+=mod->m_amount;
}



void Aura::SpellAuraReduceEnemyMCritChance(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		//value is negative percent
		TO_PLAYER( m_target )->res_M_crit_set(TO_PLAYER( m_target )->res_M_crit_get()+mod->m_amount);
	}
	else
	{
		TO_PLAYER( m_target )->res_M_crit_set(TO_PLAYER( m_target )->res_M_crit_get()-mod->m_amount);
	}
}

void Aura::SpellAuraReduceEnemyRCritChance(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		//value is negative percent
		TO_PLAYER( m_target )->res_R_crit_set(TO_PLAYER( m_target )->res_R_crit_get()+mod->m_amount);
	}
	else
	{
		TO_PLAYER( m_target )->res_R_crit_set(TO_PLAYER( m_target )->res_R_crit_get()-mod->m_amount);
	}
}

void Aura::SpellAuraUseNormalMovementSpeed( bool apply )
{
	int32 amount = ( apply ) ? mod->m_amount : -mod->m_amount;
	m_target->m_maxSpeed += (float)amount;
	m_target->UpdateSpeed();
}

void Aura::SpellAuraIncreaseTimeBetweenAttacksPCT(bool apply)
{
	if( mod->m_amount > 0 )
		SetPositive();
	else
		SetNegative();

	float pct_value = mod->m_amount / 100.0f;

	if( apply )
	{
		mod->fixed_amount[0] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME, mod->m_amount);
		mod->fixed_amount[1] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->m_amount);
		m_target->ModFloatValue(UNIT_MOD_CAST_SPEED, -pct_value);
		m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME, -mod->fixed_amount[0]);
		m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[1]);
	}
	else
	{
		m_target->ModFloatValue(UNIT_MOD_CAST_SPEED, pct_value);
		m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME, mod->fixed_amount[0]);
		m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[1]);
	}
}

/*
void Aura::SpellAuraIncreaseSpellDamageByInt(bool apply)
{
	 float val;
	 if(apply)
	 {
		 val = mod->m_amount/100.0f;
		 if(mod->m_amount>0)
			 SetPositive();
		 else
			 SetNegative();
	 }
	 else
		val =- mod->m_amount/100.0f;
		
	if(m_target->IsPlayer())
	{	
		for(uint32 x=1;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				TO_PLAYER( m_target )->SpellDmgDoneByInt[x]+=val;
			}
		}
	}
}

void Aura::SpellAuraIncreaseHealingByInt(bool apply)
{
	 float val;
	 if(apply)
	 {
		 val = mod->m_amount/100.0f;
		 if(val>0)
			 SetPositive();
		 else
			 SetNegative();
	 }
	 else
		val =- mod->m_amount/100.0f;
		
	if(m_target->IsPlayer())
	{	
		for(uint32 x=1;x<7;x++)
		{
   //		 if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				TO_PLAYER( m_target )->SpellHealDoneByInt[x]+=val;
			}
		}
	}
}
*/
void Aura::SpellAuraModAttackerCritChance(bool apply)
{
	int32 val  = (apply) ? mod->m_amount : -mod->m_amount;
	m_target->AttackerCritChanceMod[0] += val;
}

void Aura::SpellAuraIncreaseAllWeaponSkill(bool apply)
{
	if (m_target->IsPlayer())
	{   
		if(apply)
		{
			SetPositive();
//			TO_PLAYER( m_target )->ModSkillBonusType(SKILL_TYPE_WEAPON, mod->m_amount);
			//since the frikkin above line does not work we have to do it manually
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_SWORDS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_AXES, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_BOWS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_GUNS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_MACES, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_2H_SWORDS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_STAVES, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_2H_MACES, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_2H_AXES, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_DAGGERS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_CROSSBOWS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_SPEARS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_WANDS, mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_POLEARMS, mod->m_amount); 
		}
		else
		{
//			TO_PLAYER( m_target )->ModSkillBonusType(SKILL_TYPE_WEAPON, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_SWORDS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_AXES, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_BOWS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_GUNS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_MACES, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_2H_SWORDS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_STAVES, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_2H_MACES, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_2H_AXES, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_DAGGERS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_CROSSBOWS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_SPEARS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_WANDS, -mod->m_amount); 
			TO_PLAYER( m_target )->_ModifySkillBonus(SKILL_POLEARMS, -mod->m_amount); 
		}

		TO_PLAYER( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraIncreaseHitRate( bool apply )
{
	if( !m_target->IsPlayer() )
		return;
	if( apply )
		TO_PLAYER( m_target )->ModifyBonuses( SPELL_HIT_RATING, mod->m_amount );
	else
		TO_PLAYER( m_target )->ModifyBonuses( SPELL_HIT_RATING, -mod->m_amount );
	TO_PLAYER( m_target )->UpdateStats();
}

void Aura::SpellAuraIncreaseRageFromDamageDealtPCT(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	TO_PLAYER( m_target )->rageFromDamageDealt += (apply) ? mod->m_amount : -mod->m_amount;
}

int32 Aura::event_GetInstanceID()
{
	return m_target->event_GetInstanceID();
}

void Aura::RelocateEvents()
{
	event_Relocate();
}

void Aura::SpellAuraReduceCritMeleeAttackDmg(bool apply)
{
	if(!m_target)
		return;

	signed int val;
	if(apply)
		val = mod->m_amount;
	else
		val = -mod->m_amount;

	for(uint32 x=1;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->CritMeleeDamageTakenPctMod[x] += val;
}

void Aura::SpellAuraReduceCritRangedAttackDmg(bool apply)
{
	if(!m_target)
		return;

	signed int val;
	if(apply)
		val = mod->m_amount;
	else
		val = -mod->m_amount;

	for(uint32 x=1;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->CritRangedDamageTakenPctMod[x] += val;
}

void Aura::SpellAuraEnableFlight(bool apply)
{
	if(apply)
	{
		m_target->EnableFlight();
		m_target->m_flyspeedModifier += mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			TO_PLAYER( m_target )->m_FlyingAura = m_spellProto->Id;
		}
		m_target->MechanicsDispels[MECHANIC_POLYMORPHED]++; // Players flying on flying mounts are immune to polymorph.
	}
	else
	{
		m_target->DisableFlight();
		m_target->m_flyspeedModifier -= mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			TO_PLAYER( m_target )->m_FlyingAura = 0;
		}
		m_target->MechanicsDispels[MECHANIC_POLYMORPHED]--;
	}
}

void Aura::SpellAuraEnableFlightWithUnmountedSpeed(bool apply)
{
	// Used in flight form (only so far)
	if(apply)
	{
		m_target->EnableFlight();
		m_target->m_flyspeedModifier += mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			TO_PLAYER( m_target )->m_FlyingAura = m_spellProto->Id;
		}
	}
	else
	{
		m_target->DisableFlight();
		m_target->m_flyspeedModifier -= mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			TO_PLAYER( m_target )->m_FlyingAura = 0;
		}
	}
}

void Aura::SpellAuraIncreaseMovementAndMountedSpeed( bool apply )
{
	if( apply )
		m_target->m_mountedspeedModifier += mod->m_amount;
	else
		m_target->m_mountedspeedModifier -= mod->m_amount;
	m_target->UpdateSpeed();
}

void Aura::SpellAuraIncreaseFlightSpeed( bool apply )
{
	if( apply )
		m_target->m_flyspeedModifier += mod->m_amount;
	else
		m_target->m_flyspeedModifier -= mod->m_amount;
	m_target->UpdateSpeed();
}


void Aura::SpellAuraIncreaseRating( bool apply )
{
	int v = apply ? mod->m_amount : -mod->m_amount;

	if( !m_target->IsPlayer() )
		return;

	Player* plr = TO_PLAYER( m_target );
	for( uint32 x = 1; x < 25; x++ )//skip x=0
		if( ( ( ( uint32 )1 ) << x ) & mod->m_miscValue )
			plr->ModUnsigned32Value(PLAYER_FIELD_COMBAT_RATING_1 + x, v);
			//plr->ModifyBonuses( 11 + x, v );	// not always the same as above. i.e. 11+24 = resilience, but need armor ignore rating

	if( mod->m_miscValue & 1 )//weapon skill
	{
		std::map<uint32, uint32>::iterator i;
		for( uint32 y = 0; y < 20; y++ )
			if( m_spellProto->EquippedItemSubClass & ( ( ( uint32 )1 ) << y ) )
			{
					i = TO_PLAYER( m_target )->m_wratings.find( y );
					if( i == TO_PLAYER( m_target )->m_wratings.end() )//no prev
					{
						TO_PLAYER( m_target )->m_wratings[y]=v;
					}else
					{
						i->second += v;
						if( i->second == 0 )
							TO_PLAYER( m_target )->m_wratings.erase( i );
					}
			}
	}

	plr->UpdateStats();
}

void Aura::EventPeriodicRegenManaStatPct(uint32 perc, uint32 stat)
{
	if(m_target->isDead())
		return;
  
	uint32 spellId = pSpellId ? pSpellId : (GetSpellProto() ? GetSpellProto()->Id : 0);

	m_target->Energize(m_target, spellId, (m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat) * perc)/100, POWER_TYPE_MANA);
}


void Aura::SpellAuraRegenManaStatPCT(bool apply)
{
	if(apply)
	{
		SetPositive();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicRegenManaStatPct,(uint32)mod->m_amount,(uint32)mod->m_miscValue,  EVENT_AURA_REGEN_MANA_STAT_PCT, 5000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
		sEventMgr.RemoveEvents( this, EVENT_AURA_REGEN_MANA_STAT_PCT);
}

void Aura::SpellAuraSpellHealingStatPCT(bool apply)
{
	if(apply)
	{
		SetPositive();
		mod->realamount = (mod->m_amount * m_target->GetUInt32Value(UNIT_FIELD_STAT0 + mod->m_miscValue))/100;
		for( uint32 x = 1; x < 7; x++ )
			m_target->HealDoneMod[x] += mod->realamount;
	}
	else
	{
		for(uint32 x = 1; x < 7; x++)
			m_target->HealDoneMod[x] -= mod->realamount;

	}
}

void Aura::SpellAuraFinishingMovesCannotBeDodged(bool apply)
{
	if(apply)
	{
		if( !m_target->IsPlayer() )
			return;

		TO_PLAYER( m_target )->m_finishingmovesdodge = true;
	}
	else
	{
		if( !m_target->IsPlayer() )
			return;

		TO_PLAYER( m_target )->m_finishingmovesdodge = false;
	}
}

void Aura::SpellAuraAuraModInvisibilityDetection(bool apply)
{
	if( apply )
		m_target->m_stealthDetectBonus += mod->m_amount;
	else
		m_target->m_stealthDetectBonus -= mod->m_amount;
}

void Aura::SpellAuraIncreaseMaxHealth(bool apply)
{
	//should only be used by a player
	//and only ever target players
	if( !m_target->IsPlayer() )
		return;

	int32 amount;
	if( apply )
		amount = mod->m_amount;
	else
		amount = -mod->m_amount;

	TO_PLAYER( m_target )->SetHealthFromSpell( TO_PLAYER( m_target )->GetHealthFromSpell() + amount );
	TO_PLAYER( m_target )->UpdateStats();
}

void Aura::SpellAuraSpiritOfRedemption(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	if(apply)
	{
		//m_target->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		SpellEntry * sorInfo = dbcSpell.LookupEntry(27792);
		if(!sorInfo) 
			return;
		Spell* sor = NULL;
		sor = new Spell(m_target, sorInfo, true, NULL);
		SpellCastTargets targets;
		targets.m_unitTarget = m_target->GetGUID();
		sor->prepare(&targets);

		TO_PLAYER(m_target)->m_canCastSpellsWhileDead = true;
	}
	else
	{
		//m_target->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
		m_target->RemoveAura(27792);
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, 0);

		TO_PLAYER(m_target)->m_canCastSpellsWhileDead = false;
	}
}

void Aura::SpellAuraIncreaseAttackerSpellCrit(bool apply)
{
	int32 val = mod->m_amount;

	if( apply )
	{
		if( mod->m_amount>0 )
			SetNegative();
		else
			SetPositive();
	}
	else
		val = -val;

	for( uint32 x = 0; x < 7; x++ )
	{
		if( mod->m_miscValue & (((uint32)1) << x) )
			m_target->AttackerCritChanceMod[x] += val;
	}
}

void Aura::SpellAuraIncreaseRepGainPct(bool apply)
{
	if(m_target->IsPlayer())
	{
		SetPositive();
		if(apply)
			TO_PLAYER(m_target)->pctReputationMod += mod->m_amount;//re use
		else
			TO_PLAYER(m_target)->pctReputationMod -= mod->m_amount;//re use
	}
}

void Aura::SpellAuraIncreaseRangedAPStatPCT(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	int32 stat = mod->m_miscValue;	// Attribute
	ASSERT(stat > 0 && stat <= 4);	// Check that it is in range
	if(apply)
	{
		mod->realamount = (mod->m_amount * m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat)) / 100;
		if(mod->m_amount<0)
			SetNegative();
		else
			SetPositive();
	}
	//TODO make it recomputed each time we get AP or stats change
	m_target->ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, apply ? mod->realamount : -mod->realamount);
	m_target->CalcDamage();

	if( m_spellProto->NameHash == SPELL_HASH_HUNTER_VS__WILD )
	{
		Pet* pPet = TO_PLAYER(m_target)->GetSummon();
		if( pPet )
		{
			pPet->ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, apply ? mod->realamount : -mod->realamount);
			pPet->CalcDamage();
		}
	}

}

/* not used
void Aura::SpellAuraModRangedDamageTakenPCT(bool apply)
{
	if(apply)
		m_target->RangedDamageTakenPct += mod->m_amount;
	else
		m_target->RangedDamageTakenPct -= mod->m_amount;  
}*/

void Aura::SpellAuraModBlockValue(bool apply)
{
	if( m_target->IsPlayer())
 	{
		int32 amt;
 		if( apply )
 		{
			amt = mod->m_amount;
			if( amt < 0 )
				SetNegative();
			else 
				SetPositive();
 		}
		else 
		{
			amt = -mod->m_amount;
		}
		TO_PLAYER(m_target)->m_modblockvaluefromspells += amt;
		TO_PLAYER(m_target)->UpdateStats();
	}
}

// Looks like it should make spells skip some can cast checks. Atm only affects TargetAuraState check
void Aura::SpellAuraSkipCanCastCheck(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if (m_caster ==NULL || !m_target->IsPlayer()) return;

	// Generic
	if(apply)
	{
		for(uint32 x=0;x<3;x++)
			TO_PLAYER(m_target)->m_skipCastCheck[x] |= m_spellProto->EffectSpellClassMask[mod->i][x];
	}
	else
		for(uint32 x=0;x<3;x++)
			TO_PLAYER(m_target)->m_skipCastCheck[x] &= ~m_spellProto->EffectSpellClassMask[mod->i][x];

	// Spell specific
	switch(m_spellProto->NameHash)
	{
	case SPELL_HASH_FINGERS_OF_FROST:	// Fingers of Frost
		{
			if (apply)
			{
				m_caster->m_frozenTargetCharges = mod->m_amount;
				m_caster->m_frozenTargetId = GetSpellProto()->Id;
			}
			else
			{
				m_caster->m_frozenTargetCharges = 0;
				m_caster->m_frozenTargetId = 0;
			}
		}break;
	}
}

void Aura::SpellAuraCastFilter(bool apply)
{
	// cannot perform any abilities (other than those in EffectMask)
	if (!m_target->IsPlayer())
		return;	// currently only works on players

	// Generic
	if(apply)
	{
		TO_PLAYER(m_target)->m_castFilterEnabled = true;
		for(uint32 x=0;x<3;x++)
			TO_PLAYER(m_target)->m_castFilter[x] |= m_spellProto->EffectSpellClassMask[mod->i][x];
	}
	else
	{
		TO_PLAYER(m_target)->m_castFilterEnabled = false;	// check if we can turn it off
		for(uint32 x=0;x<3;x++)
		{
			TO_PLAYER(m_target)->m_castFilter[x] &= ~m_spellProto->EffectSpellClassMask[mod->i][x];
			if(TO_PLAYER(m_target)->m_castFilter[x])
				TO_PLAYER(m_target)->m_castFilterEnabled = true;
		}
	}
}

void Aura::SendInterrupted(uint8 result, Object* m_ocaster)
{
	if( !m_ocaster->IsInWorld() )
		return;

	WorldPacket data( SMSG_SPELL_FAILURE, 20 );
	if( m_ocaster->IsPlayer() )
	{
		data << m_ocaster->GetNewGUID();
		data << uint8(0); //extra_cast_number
		data << uint32(m_spellProto->Id);
		data << uint8( result );
		TO_PLAYER( m_ocaster )->GetSession()->SendPacket( &data );
	}

	data.Initialize( SMSG_SPELL_FAILED_OTHER );
	data << m_ocaster->GetNewGUID();
	data << uint8(0); //extra_cast_number
	data << uint32(m_spellProto->Id);
	data << uint8( result );
	m_ocaster->SendMessageToSet( &data, false );

	m_interrupted = (int16)result;
}

void Aura::SendChannelUpdate(uint32 time, Object* m_ocaster)
{
	WorldPacket data(MSG_CHANNEL_UPDATE, 18);
	data << m_ocaster->GetNewGUID();
	data << time;

	m_ocaster->SendMessageToSet(&data, true);	
}

void Aura::SpellAuraExpertise(bool apply)
{
	if( m_target->IsPlayer() )
	{
		int32 amt = 0;
		if( apply )
		{
			SetPositive();
			amt = mod->m_amount;
		}
		else
		{
			amt = - mod->m_amount;
		}
		// TODO: different weapon types expertise
		m_target->ModUnsigned32Value( PLAYER_EXPERTISE, amt );
		m_target->ModUnsigned32Value( PLAYER_OFFHAND_EXPERTISE, amt );
		TO_PLAYER(m_target)->UpdateStats();
	}
}

// Sets time left before aura removal and sends update packet
void Aura::SetTimeLeft(int32 time)
{
	sEventMgr.ModifyEventTimeLeft(this, EVENT_AURA_REMOVE, time);
	if( !IsPassive() )
	{
		timeleft = ( uint32 )UNIXTIME;
		BuildAuraUpdate();
	}
}

void Aura::SendPeriodicAuraLog(uint32 amt, uint32 Flags)
{
	Aura::SendPeriodicAuraLog(m_casterGuid, m_target, GetSpellProto(), amt, 0, 0, Flags, pSpellId);
}

// log message's
void Aura::SendPeriodicAuraLog(Unit* Caster, Unit* Target, SpellEntry *sp, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags, uint32 pSpellId)
{
	Aura::SendPeriodicAuraLog(Caster->GetGUID(), Target, sp, Amount, abs_dmg, resisted_damage, Flags, pSpellId);
}

void Aura::SendPeriodicAuraLog(const uint64& CasterGuid, Unit* Target, SpellEntry *sp, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags, uint32 pSpellId)
{
	uint32 spellId = sp->logsId ? sp->logsId : (pSpellId ? pSpellId : sp->Id);

	WorldPacket data(SMSG_PERIODICAURALOG, 46);
	data << Target->GetNewGUID();		// target guid
	FastGUIDPack(data, CasterGuid);		// caster guid
	data << spellId;					// spellid
	data << (uint32)1;					// count of logs going next
	data << uint32(Flags);				// Log type
	if(Flags == FLAG_PERIODIC_HEAL)
	{
		data << (uint32) Amount;	// Healing done
		data << (uint32) abs_dmg; // overheal. just passed the value inside abs_dmg
	}
	else if(Flags == FLAG_PERIODIC_LEECH)
	{
		data << (uint32) abs_dmg;	// power type. just passed the value inside abs_dmg
		data << (uint32) Amount;	// amount drained
		data << (float) 1.0f;	// percent gained from what is drained
	}
	else if(Flags == FLAG_PERIODIC_ENERGIZE)
		data << (uint32) Amount;
	else if(Flags == FLAG_PERIODIC_DAMAGE)
	{
		data << (uint32) Amount;			// damage dealt
		data << (uint32) Target->computeOverkill(Amount);			// overkill
		data << (uint32) g_spellSchoolConversionTable[sp->School]; // spell school
		data << uint32(abs_dmg);
		data << uint32(resisted_damage);
	}

	Target->SendMessageToSet(&data, true);
}

void Aura::AttemptDispel(Unit* pCaster, bool canResist)
{
	m_dispelled = true;
	Remove();
}

void Aura::SpellAuraModIgnoreArmorPct(bool apply)
{
	if( GetSpellProto()->NameHash == SPELL_HASH_MACE_SPECIALIZATION )
	{
		if(apply)
			m_target->m_ignoreArmorPctMaceSpec += (mod->m_amount / 100.0f);
		else
			m_target->m_ignoreArmorPctMaceSpec -= (mod->m_amount / 100.0f);
		return;
	}

	if(apply)
		m_target->m_ignoreArmorPct += (mod->m_amount / 100.0f);
	else
		m_target->m_ignoreArmorPct -= (mod->m_amount / 100.0f);
}

void Aura::SpellAuraSetPhase(bool apply)
{
	if( !m_target->IsPlayer()) // NPCs/GOs are handled in the DB or by scripts.
		return;

	if( apply )
	{
		if( m_target->m_phaseAura )
			m_target->m_phaseAura->Remove();

		m_target->EnablePhase( mod->m_miscValue );
		m_target->m_phaseAura = this;
	}
	else
	{
		m_target->SetPhase( 1 );
		m_target->m_phaseAura = NULL;
	}
}

void Aura::SpellAuraIncreaseAPByAttribute(bool apply)
{
	int32 stat = mod->m_miscValue;	// Attribute
	ASSERT(stat > 0 && stat <= 4);	// Check that it is in range
	if(apply)
	{
		mod->realamount = (mod->m_amount * m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat)) / 100;
		if(mod->m_amount<0)
			SetNegative();
		else
			SetPositive();
	}
	//TODO make it recomputed each time we get AP or stats change
	m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, apply ? mod->realamount : -mod->realamount);
	m_target->CalcDamage();

	if( m_spellProto->NameHash == SPELL_HASH_HUNTER_VS__WILD )
	{
		if( !m_target->IsPlayer() )
			return;

		Pet* pPet = TO_PLAYER(m_target)->GetSummon();
		if( pPet )
		{
			pPet->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, apply ? mod->realamount : -mod->realamount);
			pPet->CalcDamage();
		}
	}
}

void Aura::SpellAuraModSpellDamageFromAP(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		SetPositive();
		mod->realamount = (mod->m_amount * m_target->GetAP())/100;
	}
	for(uint32 x =0; x<7; x++){
		if (mod->m_miscValue & (((uint32)1)<<x) ){
			if(apply)
				m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, mod->realamount );
			else
				m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, -mod->realamount );
		}
	}
}

void Aura::SpellAuraModSpellHealingFromAP(bool apply)
{ 
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		SetPositive();
		mod->realamount = (mod->m_amount * m_target->GetAP())/100;
	}
	for(uint32 x=0;x<7;x++)
	{
		if (mod->m_miscValue  & (((uint32)1)<<x) )
		{
			if(apply)
				m_target->HealDoneMod[x] += mod->realamount;
			else
				m_target->HealDoneMod[x] -= mod->realamount;
		}
	}
	if(apply)
		m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, mod->realamount );
	else
		m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, -mod->realamount );

}

void Aura::SpellAuraVehiclePassenger(bool apply)
{
	if(!apply)
	{
		if( m_target && m_target->m_CurrentVehicle )
			m_target->m_CurrentVehicle->RemovePassenger(m_target);
	}
}

void Aura::SpellAuraReduceEffectDuration(bool apply)
{
	if( m_target == NULL )
		return;
	
	float val = mod->m_amount / 100.0f;
	if( apply )
	{
		mod->fixed_float_amount[0] = m_target->MechanicDurationPctMod[mod->m_miscValue] * val;
		m_target->MechanicDurationPctMod[mod->m_miscValue] += mod->fixed_float_amount[0];
	}
	else
		m_target->MechanicDurationPctMod[mod->m_miscValue] -= mod->fixed_float_amount[0];
}


void Aura::SpellAuraNoReagent(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	uint32 ClassMask[3] = {0,0,0};
	for(uint32 x=0;x<3;x++)
		ClassMask[x] |= m_target->GetUInt32Value(PLAYER_NO_REAGENT_COST_1+x);

	for(uint32 x=0;x<3;x++)
		if(apply)
			ClassMask[x] |= m_spellProto->EffectSpellClassMask[mod->i][x];
		else
			ClassMask[x] &= ~m_spellProto->EffectSpellClassMask[mod->i][x];

	for(uint32 x=0;x<3;x++)
		m_target->SetUInt32Value(PLAYER_NO_REAGENT_COST_1+x, ClassMask[x]);
}

void Aura::SpellAuraModBaseHealth(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
		mod->fixed_amount[0] = m_target->GetUInt32Value(UNIT_FIELD_BASE_HEALTH);
	int32 amt = mod->fixed_amount[0] * mod->m_amount / 100;
	if(!apply)
		amt *= -1;
	TO_PLAYER(m_target)->SetHealthFromSpell(TO_PLAYER(m_target)->GetHealthFromSpell() + amt);
	TO_PLAYER(m_target)->UpdateStats();
}

uint32 Aura::GetMaxProcCharges(Unit* caster)
{
	uint32 charges = GetSpellProto()->procCharges;
	if(caster)
	{
		SM_FIValue(caster->SM[SMT_CHARGES][0], (int32*)&charges, GetSpellProto()->SpellGroupType);
		SM_PIValue(caster->SM[SMT_CHARGES][1], (int32*)&charges, GetSpellProto()->SpellGroupType);
	}
	return charges;
}

void Aura::UpdateModAmounts()
{
	for(uint32 i=0; i<m_modcount; i++)
	{
		Unit * m_caster = GetUnitCaster();
		if( m_modList[i].m_baseAmount == 0 && m_caster != NULL && m_target != NULL )
		{
			bool heal = (GetSpellProto()->c_is_flags & SPELL_FLAG_IS_HEALING) ? true : false;
			m_modList[i].m_amount = m_caster->GetSpellBonusDamage(m_target, GetSpellProto(), 0, true, heal) * (stackSize - 1);
		}
		else
			m_modList[i].m_amount = m_modList[i].m_baseAmount * stackSize;
	}
}

void Aura::ModStackSize(int32 mod)
{
	if(mod == 0)
		return;

	uint32 maxStack = GetSpellProto()->maxstack;
	if( m_target->IsPlayer() && TO_PLAYER(m_target)->stack_cheat )
		maxStack = 999;

	int32 newStack = (int32) stackSize + mod;
	if(newStack <= 0)	// stack became empty, removing the aura
	{
		stackSize = 0;
		Remove();
		return;
	}	
	if((uint32)newStack >= maxStack && (stackSize == maxStack || maxStack == 0))
		return;	// already have maximum

	// stack is not full can add more to it
	stackSize = std::min((uint32) newStack, maxStack);
	BuildAuraUpdate();
	// now need to update amount and reapply modifiers
	ApplyModifiers(false);
	UpdateModAmounts();
	sEventMgr.RemoveEvents( this );
	if(GetDuration() > 0)
	{
		uint32 addTime = 500;
		for(uint32 spx = 0; spx < 3; spx++)
			if( GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_STUN ||
				GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_FEAR ||
				GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_ROOT ||
				GetSpellProto()->EffectApplyAuraName[spx] == SPELL_AURA_MOD_CHARM )
				addTime = 50;


		sEventMgr.AddEvent(this, &Aura::Remove, EVENT_AURA_REMOVE, GetTimeLeft() + addTime, 1,
			EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
	}
	ApplyModifiers(true);
}

void Aura::ModProcCharges(int32 mod)
{
	if(mod == 0)
		return;
	// could also check that procCharges + mod <= GetMaxProcCharges()
	procCharges = (uint32) std::max((int32)procCharges + mod, 0);
	if(procCharges > 0)
		BuildAuraUpdate();
	else
		Remove();
}

void Aura::SpellAuraModDamageTakenByMechPCT(bool apply)
{
	if( m_target == NULL )
		return;

	if( mod->m_miscValue >= MECHANIC_COUNT )
		return;

	float val = apply ? mod->m_amount / 100.0f : -(mod->m_amount / 100.0f);
	m_target->ModDamageTakenByMechPCT[mod->m_miscValue] += val;
}

void Aura::SpellAuraAddCreatureImmunity(bool apply)
{//sth like immune to all
	if( m_target == NULL )
		return;

	for( uint32 x = 1; x < MECHANIC_COUNT; x++ )
	{
		if( x != 16 && x != 19 && x != 25 && x != 31 )
			if( apply )
				m_target->MechanicsDispels[x]++;
			else
				m_target->MechanicsDispels[x]--;
	}
}

void Aura::SpellAuraRedirectThreat(bool apply)
{
	Unit * m_caster = GetUnitCaster();
	if( m_target == NULL || m_caster == NULL )
		return;

	if( !m_caster->IsPlayer() || m_caster->isDead() ||
		!(m_target->IsPlayer() || m_target->IsPet()) || 
		m_target->isDead() )
		return;


	if( apply )
	{
		if( m_spellProto->Id == 50720 )
		{
			m_target->mThreatRTarget = m_caster;
			m_target->mThreatRAmount = 0.1f;
		}
		else
		{
			m_caster->mThreatRTarget = m_target;
			m_caster->mThreatRAmount = 1;
		}
	}
	else
	{
		if( m_spellProto->Id == 50720 )
		{
			m_target->mThreatRAmount = 0;
			m_target->mThreatRTarget = NULL;
		}
		else
		{
			m_caster->mThreatRAmount = 0;
			m_caster->mThreatRTarget = NULL;
		}	
	}
}

void Aura::SpellAuraReduceAOEDamageTaken(bool apply)
{
	if( m_target == NULL )
		return;
	
	float val = mod->m_amount / 100.0f;
	if( apply )
	{
		mod->fixed_float_amount[0] = m_target->AOEDmgMod * val;
		m_target->AOEDmgMod += mod->fixed_float_amount[0];
	}
	else
		m_target->AOEDmgMod -= mod->fixed_float_amount[0];
}

void Aura::SpecialCases()
{
	//We put all the special cases here, so we keep the code clean.
	switch(m_spellProto->Id)
	{
		case 12976:// Last Stand
		case 50322:// Survival Instincts
		{
			  mod->m_amount = (uint32)(m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.3);
		}break;
		case 23782:// Gift of Life
		{
			  mod->m_amount = 1500; 
		}break;
		case 48418:// Master Shapeshifter Physical Damage
		case 48420:// Master Shapeshifter CritChance
		case 48421:// Master Shapeshifter SpellDamage
		case 48422:// Master Shapeshifter Healing 
		{
			if(TO_PLAYER(m_target)->HasSpell(48411))
				mod->m_amount =  2;
			if(TO_PLAYER(m_target)->HasSpell(48412))
				mod->m_amount =  4;
		}break;
	}
}

void Aura::SpellAuraHasteRanged(bool apply)
{
	if(mod->m_amount < 0)
		SetNegative();
	else
		SetPositive();

	float baseMod = mod->m_amount / 100.0f;

	if (m_target->IsPlayer())
	{
		if( apply )
		{
			mod->fixed_float_amount[0] = baseMod * TO_PLAYER(m_target)->m_rangedattackspeedmod;
		    TO_PLAYER( m_target )->m_rangedattackspeedmod -= mod->fixed_float_amount[0];
		}
		else
			TO_PLAYER( m_target )->m_rangedattackspeedmod += mod->fixed_float_amount[0];
		
		TO_PLAYER( m_target )->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->m_amount);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[0]);
		}
		else m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[0]);
	}
}

void Aura::SpellAuraReflectInfront(bool apply)
{
	if(apply)
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
		ReflectSpellSchool *rss = NULL;
		rss = new ReflectSpellSchool;

		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->school = -1;
		rss->require_aura_hash = 0;
		rss->infront = true;
		rss->infinity = true;
		m_target->m_reflectSpellSchool.push_back(rss);
	}
	else
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete *i;
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
	}
}

void Aura::SpellAuraModPetTalentPoints(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	Unit* unit = TO_UNIT(m_target);
	if( unit )
		unit->ChangePetTalentPointModifier(apply);
	Player * player = TO_PLAYER(m_target);
	if(player && player->GetSummon()!= NULL)
		player->GetSummon()->InitTalentsForLevel();
}