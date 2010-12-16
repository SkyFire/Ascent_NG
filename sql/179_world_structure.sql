SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `achievement_rewards`
-- ----------------------------
DROP TABLE IF EXISTS `achievement_rewards`;
CREATE TABLE `achievement_rewards` (
  `achievementid` int(10) unsigned NOT NULL,
  `title_alliance` int(10) unsigned NOT NULL,
  `title_horde` int(10) unsigned NOT NULL,
  `itemid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`achievementid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of achievement_rewards
-- ----------------------------

-- ----------------------------
-- Table structure for `ai_agents`
-- ----------------------------
DROP TABLE IF EXISTS `ai_agents`;
CREATE TABLE `ai_agents` (
  `entry` int(11) unsigned NOT NULL DEFAULT '0',
  `type` enum('MELEE','RANGED','FLEE','SPELL','CALLFORHELP') NOT NULL DEFAULT 'SPELL',
  `chance` int(11) unsigned NOT NULL DEFAULT '0',
  `maxcount` int(11) unsigned NOT NULL DEFAULT '0',
  `spell` int(11) unsigned NOT NULL DEFAULT '0',
  `spelltype` enum('ROOT','HEAL','STUN','FEAR','SILENCE','CURSE','AOEDAMAGE','DAMAGE','SUMMON','BUFF','DEBUFF') NOT NULL DEFAULT 'ROOT',
  `targettype` enum('RANDOMTARGET','TARGETLOCATION','CREATURELOCATION','SELF','OWNER') NOT NULL DEFAULT 'RANDOMTARGET',
  `cooldown` int(8) NOT NULL DEFAULT '0',
  `floatMisc1` float NOT NULL DEFAULT '0',
  `Misc2` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`type`,`spell`),
  UNIQUE KEY `a` (`entry`,`spell`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='AI System';

-- ----------------------------
-- Records of ai_agents
-- ----------------------------

-- ----------------------------
-- Table structure for `ai_threattospellid`
-- ----------------------------
DROP TABLE IF EXISTS `ai_threattospellid`;
CREATE TABLE `ai_threattospellid` (
  `spell` int(11) unsigned NOT NULL DEFAULT '0',
  `mod` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`spell`),
  UNIQUE KEY `a` (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='AI System';

-- ----------------------------
-- Records of ai_threattospellid
-- ----------------------------

-- ----------------------------
-- Table structure for `areatriggers`
-- ----------------------------
DROP TABLE IF EXISTS `areatriggers`;
CREATE TABLE `areatriggers` (
  `entry` int(11) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned DEFAULT '0',
  `map` int(11) unsigned DEFAULT NULL,
  `screen` int(11) unsigned DEFAULT NULL,
  `name` varchar(100) DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `required_honor_rank` int(11) unsigned NOT NULL DEFAULT '0',
  `required_level` tinyint(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Trigger System';

-- ----------------------------
-- Records of areatriggers
-- ----------------------------

-- ----------------------------
-- Table structure for `auctionhouse`
-- ----------------------------
DROP TABLE IF EXISTS `auctionhouse`;
CREATE TABLE `auctionhouse` (
  `id` int(32) NOT NULL AUTO_INCREMENT,
  `creature_entry` bigint(64) unsigned NOT NULL DEFAULT '0',
  `group` int(32) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=39 DEFAULT CHARSET=latin1 COMMENT='Auction House';

-- ----------------------------
-- Records of auctionhouse
-- ----------------------------

-- ----------------------------
-- Table structure for `banned_phrases`
-- ----------------------------
DROP TABLE IF EXISTS `banned_phrases`;
CREATE TABLE `banned_phrases` (
  `phrase` varchar(250) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`phrase`),
  UNIQUE KEY `a` (`phrase`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of banned_phrases
-- ----------------------------

-- ----------------------------
-- Table structure for `clientaddons`
-- ----------------------------
DROP TABLE IF EXISTS `clientaddons`;
CREATE TABLE `clientaddons` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `crc` bigint(20) unsigned DEFAULT NULL,
  `banned` int(10) unsigned NOT NULL DEFAULT '0',
  `showinlist` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=153 DEFAULT CHARSET=latin1 COMMENT='Client Addons';

-- ----------------------------
-- Records of clientaddons
-- ----------------------------

-- ----------------------------
-- Table structure for `command_overrides`
-- ----------------------------
DROP TABLE IF EXISTS `command_overrides`;
CREATE TABLE `command_overrides` (
  `command_name` varchar(100) NOT NULL,
  `access_level` varchar(10) NOT NULL,
  PRIMARY KEY (`command_name`),
  UNIQUE KEY `a` (`command_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Commands System';

-- ----------------------------
-- Records of command_overrides
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_formations`
-- ----------------------------
DROP TABLE IF EXISTS `creature_formations`;
CREATE TABLE `creature_formations` (
  `spawn_id` int(30) unsigned NOT NULL DEFAULT '0',
  `target_spawn_id` int(30) unsigned NOT NULL DEFAULT '0',
  `follow_angle` float NOT NULL DEFAULT '0',
  `follow_dist` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`spawn_id`),
  UNIQUE KEY `a` (`spawn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of creature_formations
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_names`
-- ----------------------------
DROP TABLE IF EXISTS `creature_names`;
CREATE TABLE `creature_names` (
  `entry` int(20) unsigned NOT NULL DEFAULT '0',
  `name` varchar(100) NOT NULL,
  `subname` varchar(100) NOT NULL,
  `info_str` varchar(500) NOT NULL,
  `Flags1` int(30) unsigned NOT NULL DEFAULT '0',
  `type` int(30) NOT NULL DEFAULT '0',
  `loot_skill_type` int(10) NOT NULL DEFAULT '1',
  `family` int(30) NOT NULL DEFAULT '0',
  `rank` int(30) NOT NULL DEFAULT '0',
  `unk4` int(30) NOT NULL DEFAULT '0',
  `spelldataid` int(30) NOT NULL DEFAULT '0',
  `male_displayid` int(30) NOT NULL,
  `female_displayid` int(30) NOT NULL,
  `male_displayid2` int(30) NOT NULL DEFAULT '0',
  `female_displayid2` int(30) NOT NULL DEFAULT '0',
  `unknown_float1` float NOT NULL DEFAULT '0',
  `unknown_float2` float NOT NULL DEFAULT '0',
  `civilian` tinyint(30) NOT NULL DEFAULT '0',
  `leader` tinyint(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of creature_names
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_proto`
-- ----------------------------
DROP TABLE IF EXISTS `creature_proto`;
CREATE TABLE `creature_proto` (
  `entry` int(30) unsigned NOT NULL DEFAULT '0',
  `minlevel` int(30) unsigned NOT NULL,
  `maxlevel` int(30) unsigned NOT NULL,
  `faction` int(30) unsigned NOT NULL DEFAULT '0',
  `minhealth` int(30) unsigned NOT NULL,
  `maxhealth` int(30) unsigned NOT NULL,
  `mana` int(30) unsigned NOT NULL DEFAULT '0',
  `scale` float NOT NULL DEFAULT '0',
  `npcflags` int(30) unsigned NOT NULL DEFAULT '0',
  `attacktime` int(30) unsigned NOT NULL DEFAULT '0',
  `attacktype` int(4) NOT NULL DEFAULT '0',
  `mindamage` float NOT NULL DEFAULT '0',
  `maxdamage` float NOT NULL DEFAULT '0',
  `rangedattacktime` int(30) unsigned NOT NULL DEFAULT '0',
  `rangedmindamage` float unsigned NOT NULL DEFAULT '0',
  `rangedmaxdamage` float unsigned NOT NULL DEFAULT '0',
  `Item1` int(30) unsigned NOT NULL DEFAULT '0',
  `Item2` int(30) unsigned NOT NULL DEFAULT '0',
  `Item3` int(30) unsigned NOT NULL DEFAULT '0',
  `respawntime` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance1` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance2` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance3` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance4` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance5` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance6` int(30) unsigned NOT NULL DEFAULT '0',
  `resistance7` int(30) unsigned NOT NULL DEFAULT '0',
  `combat_reach` float NOT NULL DEFAULT '0',
  `bounding_radius` float NOT NULL DEFAULT '0',
  `auras` longtext NOT NULL,
  `boss` int(11) unsigned NOT NULL DEFAULT '0',
  `money` int(30) NOT NULL DEFAULT '0',
  `no_XP` int(11) NOT NULL DEFAULT '0',
  `invisibility_type` int(30) unsigned NOT NULL,
  `death_state` int(30) unsigned NOT NULL,
  `walk_speed` float NOT NULL DEFAULT '2.5',
  `run_speed` float NOT NULL DEFAULT '8',
  `fly_speed` float NOT NULL DEFAULT '14',
  `extra_a9_flags` int(30) NOT NULL DEFAULT '0',
  `auraimmune_flag` int(10) unsigned NOT NULL DEFAULT '0',
  `vehicle_entry` int(11) NOT NULL DEFAULT '-1',
  `CanMove` int(11) NOT NULL DEFAULT '7',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of creature_proto
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_proto_heroic`
-- ----------------------------
DROP TABLE IF EXISTS `creature_proto_heroic`;
CREATE TABLE `creature_proto_heroic` (
  `entry` int(10) NOT NULL,
  `minlevel` int(10) unsigned NOT NULL DEFAULT '1',
  `maxlevel` int(10) unsigned NOT NULL DEFAULT '1',
  `minhealth` int(10) NOT NULL DEFAULT '1',
  `maxhealth` int(10) NOT NULL DEFAULT '1',
  `mindmg` float NOT NULL DEFAULT '1',
  `maxdmg` float NOT NULL DEFAULT '1',
  `mana` int(10) NOT NULL DEFAULT '1',
  `resistance1` int(10) NOT NULL DEFAULT '0',
  `resistance2` int(10) NOT NULL DEFAULT '0',
  `resistance3` int(10) NOT NULL DEFAULT '0',
  `resistance4` int(10) NOT NULL DEFAULT '0',
  `resistance5` int(10) NOT NULL DEFAULT '0',
  `resistance6` int(10) NOT NULL DEFAULT '0',
  `resistance7` int(10) NOT NULL DEFAULT '0',
  `auras` longtext NOT NULL,
  `auraimmune_flag` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of creature_proto_heroic
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_quest_finisher`
-- ----------------------------
DROP TABLE IF EXISTS `creature_quest_finisher`;
CREATE TABLE `creature_quest_finisher` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `quest` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='Creature System';

-- ----------------------------
-- Records of creature_quest_finisher
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_quest_starter`
-- ----------------------------
DROP TABLE IF EXISTS `creature_quest_starter`;
CREATE TABLE `creature_quest_starter` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `quest` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='Creature System';

-- ----------------------------
-- Records of creature_quest_starter
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_spawns`
-- ----------------------------
DROP TABLE IF EXISTS `creature_spawns`;
CREATE TABLE `creature_spawns` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `entry` int(30) NOT NULL,
  `map` int(30) NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL,
  `movetype` int(30) NOT NULL DEFAULT '0',
  `displayid` int(30) unsigned NOT NULL DEFAULT '0',
  `faction` int(30) NOT NULL DEFAULT '14',
  `flags` int(30) NOT NULL DEFAULT '0',
  `bytes` int(30) NOT NULL DEFAULT '0',
  `bytes1` int(30) NOT NULL DEFAULT '0',
  `bytes2` int(30) NOT NULL DEFAULT '0',
  `emote_state` int(30) NOT NULL DEFAULT '0',
  `channel_spell` int(30) NOT NULL DEFAULT '0',
  `channel_target_sqlid` int(30) NOT NULL DEFAULT '0',
  `channel_target_sqlid_creature` int(30) NOT NULL DEFAULT '0',
  `standstate` int(10) NOT NULL DEFAULT '0',
  `MountedDisplayID` int(10) NOT NULL DEFAULT '0',
  `phase` int(10) NOT NULL DEFAULT '0',
  `vehicle` int(10) NOT NULL DEFAULT '0',
  `eventid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Relates to events table',
  PRIMARY KEY (`id`),
  KEY `b` (`map`)
) ENGINE=MyISAM AUTO_INCREMENT=6168571 DEFAULT CHARSET=latin1 COMMENT='Spawn System';

-- ----------------------------
-- Records of creature_spawns
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_staticspawns`
-- ----------------------------
DROP TABLE IF EXISTS `creature_staticspawns`;
CREATE TABLE `creature_staticspawns` (
  `id` int(30) unsigned NOT NULL AUTO_INCREMENT,
  `entry` int(30) NOT NULL,
  `Map` int(30) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `orientation` float NOT NULL,
  `movetype` int(30) NOT NULL DEFAULT '0',
  `displayid` int(30) unsigned NOT NULL DEFAULT '0',
  `faction` int(30) NOT NULL DEFAULT '35',
  `flags` int(30) NOT NULL DEFAULT '0',
  `bytes` int(30) NOT NULL DEFAULT '0',
  `bytes1` int(30) NOT NULL,
  `bytes2` int(30) NOT NULL DEFAULT '0',
  `emote_state` int(30) NOT NULL DEFAULT '0',
  `channel_spell` int(30) NOT NULL DEFAULT '0',
  `channel_target_sqlid` int(30) NOT NULL,
  `channel_target_sqlid_creature` int(30) NOT NULL,
  `standstate` int(10) NOT NULL,
  `Phase` int(10) NOT NULL,
  `vehicle` int(10) NOT NULL DEFAULT '0',
  `eventid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Relates to events table',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `a` (`Map`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

-- ----------------------------
-- Records of creature_staticspawns
-- ----------------------------

-- ----------------------------
-- Table structure for `creature_waypoints`
-- ----------------------------
DROP TABLE IF EXISTS `creature_waypoints`;
CREATE TABLE `creature_waypoints` (
  `spawnid` int(10) unsigned NOT NULL DEFAULT '0',
  `waypointid` int(10) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `waittime` int(6) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `forwardemoteoneshot` int(1) unsigned NOT NULL DEFAULT '0',
  `forwardemoteid` int(4) unsigned NOT NULL DEFAULT '0',
  `backwardemoteoneshot` int(1) unsigned NOT NULL DEFAULT '0',
  `backwardemoteid` int(4) unsigned NOT NULL DEFAULT '0',
  `forwardskinid` int(10) unsigned NOT NULL DEFAULT '0',
  `backwardskinid` int(10) unsigned NOT NULL DEFAULT '0',
  `forwardStandState` int(10) unsigned NOT NULL DEFAULT '0',
  `backwardStandState` int(10) unsigned NOT NULL DEFAULT '0',
  `forwardSpellToCast` int(10) unsigned NOT NULL DEFAULT '0',
  `backwardSpellToCast` int(10) unsigned NOT NULL DEFAULT '0',
  `forwardSayText` varchar(256) NOT NULL,
  `backwardSayText` varchar(256) NOT NULL,
  PRIMARY KEY (`spawnid`,`waypointid`),
  UNIQUE KEY `a` (`spawnid`,`waypointid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of creature_waypoints
-- ----------------------------

-- ----------------------------
-- Table structure for `creatureloot`
-- ----------------------------
DROP TABLE IF EXISTS `creatureloot`;
CREATE TABLE `creatureloot` (
  `entryid` int(11) NOT NULL DEFAULT '0',
  `itemid` int(11) NOT NULL DEFAULT '0',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float NOT NULL DEFAULT '0',
  `mincount` int(30) NOT NULL DEFAULT '1',
  `maxcount` int(30) NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 PACK_KEYS=0 ROW_FORMAT=FIXED COMMENT='Loot System';

-- ----------------------------
-- Records of creatureloot
-- ----------------------------

-- ----------------------------
-- Table structure for `creatureloot_gathering`
-- ----------------------------
DROP TABLE IF EXISTS `creatureloot_gathering`;
CREATE TABLE `creatureloot_gathering` (
  `entryid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemid` int(10) unsigned NOT NULL DEFAULT '0',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float DEFAULT '0',
  `mincount` int(30) DEFAULT '1',
  `maxcount` int(30) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Records of creatureloot_gathering
-- ----------------------------

-- ----------------------------
-- Table structure for `disenchantingloot`
-- ----------------------------
DROP TABLE IF EXISTS `disenchantingloot`;
CREATE TABLE `disenchantingloot` (
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float DEFAULT '0',
  `mincount` int(30) DEFAULT '1',
  `maxcount` int(30) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of disenchantingloot
-- ----------------------------

-- ----------------------------
-- Table structure for `events`
-- ----------------------------
DROP TABLE IF EXISTS `events`;
CREATE TABLE `events` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'Id of event you want to trigger',
  `daynumber` tinyint(3) unsigned NOT NULL COMMENT '1 - 31 day of the month you want to trigger event',
  `monthnumber` tinyint(3) unsigned DEFAULT '0' COMMENT '1 - 12 month of the year you want to trigger event',
  `eventname` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Just for human readable purposes, this is not used by the server at all',
  `activedays` tinyint(3) unsigned DEFAULT NULL COMMENT 'Number of days the event will remain active',
  `ishourlyevent` tinyint(1) DEFAULT NULL COMMENT 'Is this an hourly event? This cannot be used in conjunction with daily events',
  `starthour` tinyint(3) unsigned DEFAULT NULL COMMENT '0 - 23 the hour that the event begins',
  `endhour` tinyint(3) unsigned DEFAULT NULL COMMENT '0 - 23 the hour that the event ends, if this is smaller that the starthour, it will end on the next day',
  PRIMARY KEY (`eventid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of events
-- ----------------------------

-- ----------------------------
-- Table structure for `events_creature`
-- ----------------------------
DROP TABLE IF EXISTS `events_creature`;
CREATE TABLE `events_creature` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'Relates to creature_spawn eventid',
  `id` tinyint(3) unsigned NOT NULL COMMENT 'Relates to creature_spawn id',
  `changesflag` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'See enum in the core for permissable flags in World.h',
  `phase` tinyint(3) DEFAULT NULL COMMENT 'This is the phase the creature is in when the event is active',
  `displayid` int(10) unsigned DEFAULT NULL COMMENT 'This is the displayid to change to while the event is active',
  `item1` int(10) unsigned DEFAULT NULL COMMENT 'This is the id of equip slot 1 to change while the event is active',
  `item2` int(10) unsigned DEFAULT NULL COMMENT 'This is the id of equip slot 2 to change while the event is active',
  `item3` int(10) unsigned DEFAULT NULL COMMENT 'This is the id of equip slot 3 to change while the event is active',
  PRIMARY KEY (`eventid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of events_creature
-- ----------------------------

-- ----------------------------
-- Table structure for `events_gameobject`
-- ----------------------------
DROP TABLE IF EXISTS `events_gameobject`;
CREATE TABLE `events_gameobject` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'This relates to gameobject_spawns eventid',
  `id` int(11) unsigned NOT NULL COMMENT 'Relates to gameobject_spawn id',
  `changesflag` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'See enum in core for permissable flags in World.h',
  `phase` tinyint(3) DEFAULT NULL COMMENT 'Phase the go is in when the event is active',
  `displayid` int(10) unsigned DEFAULT NULL COMMENT 'This is the displayid to change to while the event is active',
  PRIMARY KEY (`eventid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of events_gameobject
-- ----------------------------

-- ----------------------------
-- Table structure for `fishing`
-- ----------------------------
DROP TABLE IF EXISTS `fishing`;
CREATE TABLE `fishing` (
  `Zone` int(11) unsigned NOT NULL DEFAULT '0',
  `MinSkill` int(11) unsigned DEFAULT NULL,
  `MaxSkill` int(11) unsigned DEFAULT NULL,
  UNIQUE KEY `Zone` (`Zone`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Fishing System';

-- ----------------------------
-- Records of fishing
-- ----------------------------

-- ----------------------------
-- Table structure for `fishingloot`
-- ----------------------------
DROP TABLE IF EXISTS `fishingloot`;
CREATE TABLE `fishingloot` (
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float NOT NULL DEFAULT '0',
  `mincount` int(11) unsigned NOT NULL DEFAULT '1',
  `maxcount` int(11) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Records of fishingloot
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_names`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_names`;
CREATE TABLE `gameobject_names` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `DisplayID` int(10) unsigned NOT NULL DEFAULT '0',
  `Name` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `spellfocus` int(30) NOT NULL DEFAULT '0',
  `sound1` int(30) NOT NULL DEFAULT '0',
  `sound2` int(30) NOT NULL DEFAULT '0',
  `sound3` int(30) NOT NULL DEFAULT '0',
  `sound4` int(30) NOT NULL DEFAULT '0',
  `sound5` int(30) NOT NULL DEFAULT '0',
  `sound6` int(30) NOT NULL DEFAULT '0',
  `sound7` int(30) NOT NULL DEFAULT '0',
  `sound8` int(30) NOT NULL DEFAULT '0',
  `sound9` int(30) NOT NULL DEFAULT '0',
  `unknown1` int(30) NOT NULL DEFAULT '0',
  `unknown2` int(30) NOT NULL DEFAULT '0',
  `unknown3` int(30) NOT NULL DEFAULT '0',
  `unknown4` int(30) NOT NULL DEFAULT '0',
  `unknown5` int(30) NOT NULL DEFAULT '0',
  `unknown6` int(30) NOT NULL DEFAULT '0',
  `unknown7` int(30) NOT NULL DEFAULT '0',
  `unknown8` int(30) NOT NULL DEFAULT '0',
  `unknown9` int(30) NOT NULL DEFAULT '0',
  `unknown10` int(30) NOT NULL DEFAULT '0',
  `unknown11` int(30) NOT NULL DEFAULT '0',
  `unknown12` int(30) NOT NULL DEFAULT '0',
  `unknown13` int(30) NOT NULL DEFAULT '0',
  `unknown14` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Gameobject System';

-- ----------------------------
-- Records of gameobject_names
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_quest_finisher`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_quest_finisher`;
CREATE TABLE `gameobject_quest_finisher` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `quest` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Records of gameobject_quest_finisher
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_quest_item_binding`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_quest_item_binding`;
CREATE TABLE `gameobject_quest_item_binding` (
  `entry` int(11) NOT NULL DEFAULT '0',
  `quest` int(11) NOT NULL DEFAULT '0',
  `item` int(11) NOT NULL DEFAULT '0',
  `item_count` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Records of gameobject_quest_item_binding
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_quest_pickup_binding`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_quest_pickup_binding`;
CREATE TABLE `gameobject_quest_pickup_binding` (
  `entry` int(11) NOT NULL DEFAULT '0',
  `quest` int(11) NOT NULL DEFAULT '0',
  `required_count` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Records of gameobject_quest_pickup_binding
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_quest_starter`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_quest_starter`;
CREATE TABLE `gameobject_quest_starter` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `quest` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Records of gameobject_quest_starter
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_spawns`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_spawns`;
CREATE TABLE `gameobject_spawns` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Entry` int(10) unsigned NOT NULL DEFAULT '0',
  `map` int(10) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `Facing` float NOT NULL DEFAULT '0',
  `orientation1` float NOT NULL DEFAULT '0',
  `orientation2` float NOT NULL DEFAULT '0',
  `orientation3` float NOT NULL DEFAULT '0',
  `orientation4` float NOT NULL DEFAULT '0',
  `State` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Faction` int(10) unsigned NOT NULL DEFAULT '0',
  `Scale` float NOT NULL DEFAULT '0',
  `phase` int(10) NOT NULL DEFAULT '0',
  `eventid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'relates to events table',
  PRIMARY KEY (`id`),
  KEY `Map` (`map`)
) ENGINE=MyISAM AUTO_INCREMENT=2113850 DEFAULT CHARSET=cp1251 COLLATE=cp1251_general_cs PACK_KEYS=0 ROW_FORMAT=FIXED COMMENT='Spawn System';

-- ----------------------------
-- Records of gameobject_spawns
-- ----------------------------

-- ----------------------------
-- Table structure for `gameobject_staticspawns`
-- ----------------------------
DROP TABLE IF EXISTS `gameobject_staticspawns`;
CREATE TABLE `gameobject_staticspawns` (
  `id` int(30) NOT NULL AUTO_INCREMENT,
  `entry` int(30) NOT NULL,
  `map` int(11) NOT NULL DEFAULT '0',
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `facing` float NOT NULL,
  `o1` float NOT NULL,
  `o2` float NOT NULL,
  `o3` float NOT NULL,
  `o4` float NOT NULL,
  `state` int(11) NOT NULL DEFAULT '0',
  `flags` int(30) NOT NULL DEFAULT '0',
  `faction` int(11) NOT NULL DEFAULT '0',
  `scale` float NOT NULL,
  `ncpRespawnLink` int(11) NOT NULL DEFAULT '0',
  `phase` int(11) NOT NULL,
  `vehicle` int(10) NOT NULL DEFAULT '0',
  `eventid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Relates to events table',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

-- ----------------------------
-- Records of gameobject_staticspawns
-- ----------------------------

-- ----------------------------
-- Table structure for `graveyards`
-- ----------------------------
DROP TABLE IF EXISTS `graveyards`;
CREATE TABLE `graveyards` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `zoneid` int(10) unsigned NOT NULL DEFAULT '0',
  `adjacentzoneid` int(10) unsigned NOT NULL DEFAULT '0',
  `mapid` int(10) unsigned NOT NULL DEFAULT '0',
  `faction` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `index` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1409 DEFAULT CHARSET=latin1 COMMENT='Graveyard System';

-- ----------------------------
-- Records of graveyards
-- ----------------------------

-- ----------------------------
-- Table structure for `item_quest_association`
-- ----------------------------
DROP TABLE IF EXISTS `item_quest_association`;
CREATE TABLE `item_quest_association` (
  `item` int(11) NOT NULL DEFAULT '0',
  `quest` int(11) NOT NULL DEFAULT '0',
  `item_count` int(11) NOT NULL DEFAULT '0',
  UNIQUE KEY `item` (`item`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

-- ----------------------------
-- Records of item_quest_association
-- ----------------------------

-- ----------------------------
-- Table structure for `item_randomprop_groups`
-- ----------------------------
DROP TABLE IF EXISTS `item_randomprop_groups`;
CREATE TABLE `item_randomprop_groups` (
  `entry_id` int(30) NOT NULL,
  `randomprops_entryid` int(30) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY (`entry_id`,`randomprops_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Item System';

-- ----------------------------
-- Records of item_randomprop_groups
-- ----------------------------

-- ----------------------------
-- Table structure for `item_randomsuffix_groups`
-- ----------------------------
DROP TABLE IF EXISTS `item_randomsuffix_groups`;
CREATE TABLE `item_randomsuffix_groups` (
  `entry_id` int(30) NOT NULL,
  `randomsuffix_entryid` int(30) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY (`entry_id`,`randomsuffix_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

-- ----------------------------
-- Records of item_randomsuffix_groups
-- ----------------------------

-- ----------------------------
-- Table structure for `itemloot`
-- ----------------------------
DROP TABLE IF EXISTS `itemloot`;
CREATE TABLE `itemloot` (
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float NOT NULL DEFAULT '0',
  `mincount` int(11) unsigned NOT NULL DEFAULT '1',
  `maxcount` int(11) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  KEY `i_gameobj_loot_entry` (`entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Records of itemloot
-- ----------------------------

-- ----------------------------
-- Table structure for `itempages`
-- ----------------------------
DROP TABLE IF EXISTS `itempages`;
CREATE TABLE `itempages` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `text` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `next_page` int(10) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

-- ----------------------------
-- Records of itempages
-- ----------------------------

-- ----------------------------
-- Table structure for `itempetfood`
-- ----------------------------
DROP TABLE IF EXISTS `itempetfood`;
CREATE TABLE `itempetfood` (
  `entry` int(11) NOT NULL,
  `food_type` int(11) NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of itempetfood
-- ----------------------------

-- ----------------------------
-- Table structure for `items`
-- ----------------------------
DROP TABLE IF EXISTS `items`;
CREATE TABLE `items` (
  `entry` int(30) unsigned NOT NULL DEFAULT '0',
  `class` int(30) NOT NULL DEFAULT '0',
  `subclass` int(30) NOT NULL DEFAULT '0',
  `field4` int(10) NOT NULL DEFAULT '-1',
  `name1` varchar(255) NOT NULL,
  `name2` varchar(255) NOT NULL DEFAULT '',
  `name3` varchar(255) NOT NULL DEFAULT '',
  `name4` varchar(255) NOT NULL DEFAULT '',
  `displayid` int(70) unsigned NOT NULL DEFAULT '0',
  `quality` int(30) NOT NULL DEFAULT '0',
  `flags` int(30) NOT NULL DEFAULT '0',
  `faction` int(11) unsigned NOT NULL DEFAULT '3',
  `buyprice` int(30) NOT NULL DEFAULT '0',
  `sellprice` int(30) NOT NULL DEFAULT '0',
  `inventorytype` int(30) NOT NULL DEFAULT '0',
  `allowableclass` int(30) NOT NULL DEFAULT '0',
  `allowablerace` int(30) NOT NULL DEFAULT '0',
  `itemlevel` int(30) NOT NULL DEFAULT '0',
  `requiredlevel` int(30) NOT NULL DEFAULT '0',
  `RequiredSkill` int(30) NOT NULL DEFAULT '0',
  `RequiredSkillRank` int(30) NOT NULL DEFAULT '0',
  `RequiredSkillSubRank` int(30) NOT NULL DEFAULT '0',
  `RequiredPlayerRank1` int(30) NOT NULL DEFAULT '0',
  `RequiredPlayerRank2` int(30) NOT NULL DEFAULT '0',
  `RequiredFaction` int(30) NOT NULL DEFAULT '0',
  `RequiredFactionStanding` int(30) NOT NULL DEFAULT '0',
  `Unique` int(30) NOT NULL DEFAULT '0',
  `maxcount` int(30) NOT NULL DEFAULT '0',
  `ContainerSlots` int(30) NOT NULL DEFAULT '0',
  `stat_type1` int(30) NOT NULL DEFAULT '0',
  `stat_value1` int(30) NOT NULL DEFAULT '0',
  `stat_type2` int(30) NOT NULL DEFAULT '0',
  `stat_value2` int(30) NOT NULL DEFAULT '0',
  `stat_type3` int(30) NOT NULL DEFAULT '0',
  `stat_value3` int(30) NOT NULL DEFAULT '0',
  `stat_type4` int(30) NOT NULL DEFAULT '0',
  `stat_value4` int(30) NOT NULL DEFAULT '0',
  `stat_type5` int(30) NOT NULL DEFAULT '0',
  `stat_value5` int(30) NOT NULL DEFAULT '0',
  `stat_type6` int(30) NOT NULL DEFAULT '0',
  `stat_value6` int(30) NOT NULL DEFAULT '0',
  `stat_type7` int(30) NOT NULL DEFAULT '0',
  `stat_value7` int(30) NOT NULL DEFAULT '0',
  `stat_type8` int(30) NOT NULL DEFAULT '0',
  `stat_value8` int(30) NOT NULL DEFAULT '0',
  `stat_type9` int(30) NOT NULL DEFAULT '0',
  `stat_value9` int(30) NOT NULL DEFAULT '0',
  `stat_type10` int(30) NOT NULL DEFAULT '0',
  `stat_value10` int(30) NOT NULL DEFAULT '0',
  `dmg_min1` float NOT NULL DEFAULT '0',
  `dmg_max1` float NOT NULL DEFAULT '0',
  `dmg_type1` int(30) NOT NULL DEFAULT '0',
  `dmg_min2` float NOT NULL DEFAULT '0',
  `dmg_max2` float NOT NULL DEFAULT '0',
  `dmg_type2` int(30) NOT NULL DEFAULT '0',
  `armor` int(30) NOT NULL DEFAULT '0',
  `holy_res` int(30) NOT NULL DEFAULT '0',
  `fire_res` int(30) NOT NULL DEFAULT '0',
  `nature_res` int(30) NOT NULL DEFAULT '0',
  `frost_res` int(30) NOT NULL DEFAULT '0',
  `shadow_res` int(30) NOT NULL DEFAULT '0',
  `arcane_res` int(30) NOT NULL DEFAULT '0',
  `delay` int(30) NOT NULL DEFAULT '0',
  `ammo_type` int(30) NOT NULL DEFAULT '0',
  `range` float NOT NULL DEFAULT '0',
  `spellid_1` int(30) NOT NULL DEFAULT '0',
  `spelltrigger_1` int(30) NOT NULL DEFAULT '0',
  `spellcharges_1` int(30) NOT NULL DEFAULT '0',
  `spellcooldown_1` int(30) NOT NULL DEFAULT '0',
  `spellcategory_1` int(30) NOT NULL DEFAULT '0',
  `spellcategorycooldown_1` int(30) NOT NULL DEFAULT '0',
  `spellid_2` int(30) NOT NULL DEFAULT '0',
  `spelltrigger_2` int(30) NOT NULL DEFAULT '0',
  `spellcharges_2` int(30) NOT NULL DEFAULT '0',
  `spellcooldown_2` int(30) NOT NULL DEFAULT '0',
  `spellcategory_2` int(30) NOT NULL DEFAULT '0',
  `spellcategorycooldown_2` int(30) NOT NULL DEFAULT '0',
  `spellid_3` int(30) NOT NULL DEFAULT '0',
  `spelltrigger_3` int(30) NOT NULL DEFAULT '0',
  `spellcharges_3` int(30) NOT NULL DEFAULT '0',
  `spellcooldown_3` int(30) NOT NULL DEFAULT '0',
  `spellcategory_3` int(30) NOT NULL DEFAULT '0',
  `spellcategorycooldown_3` int(30) NOT NULL DEFAULT '0',
  `spellid_4` int(30) NOT NULL DEFAULT '0',
  `spelltrigger_4` int(30) NOT NULL DEFAULT '0',
  `spellcharges_4` int(30) NOT NULL DEFAULT '0',
  `spellcooldown_4` int(30) NOT NULL DEFAULT '0',
  `spellcategory_4` int(30) NOT NULL DEFAULT '0',
  `spellcategorycooldown_4` int(30) NOT NULL DEFAULT '0',
  `spellid_5` int(30) NOT NULL DEFAULT '0',
  `spelltrigger_5` int(30) NOT NULL DEFAULT '0',
  `spellcharges_5` int(30) NOT NULL DEFAULT '0',
  `spellcooldown_5` int(30) NOT NULL DEFAULT '0',
  `spellcategory_5` int(30) NOT NULL DEFAULT '0',
  `spellcategorycooldown_5` int(30) NOT NULL DEFAULT '0',
  `bonding` int(30) NOT NULL DEFAULT '0',
  `description` varchar(255) NOT NULL DEFAULT '',
  `page_id` int(30) NOT NULL DEFAULT '0',
  `page_language` int(30) NOT NULL DEFAULT '0',
  `page_material` int(30) NOT NULL DEFAULT '0',
  `quest_id` int(30) NOT NULL DEFAULT '0',
  `lock_id` int(30) NOT NULL DEFAULT '0',
  `lock_material` int(30) NOT NULL DEFAULT '0',
  `sheathID` int(30) NOT NULL DEFAULT '0',
  `randomprop` int(30) NOT NULL DEFAULT '0',
  `RandomSuffixId` int(11) NOT NULL DEFAULT '0',
  `block` int(30) NOT NULL DEFAULT '0',
  `itemset` int(30) NOT NULL DEFAULT '0',
  `MaxDurability` int(30) NOT NULL DEFAULT '0',
  `ZoneNameID` int(30) NOT NULL DEFAULT '0',
  `mapid` int(30) DEFAULT NULL,
  `bagfamily` int(30) NOT NULL DEFAULT '0',
  `TotemCategory` int(30) DEFAULT NULL,
  `socket_color_1` int(30) DEFAULT NULL,
  `unk201_3` int(30) NOT NULL DEFAULT '0',
  `socket_color_2` int(30) DEFAULT NULL,
  `unk201_5` int(30) NOT NULL DEFAULT '0',
  `socket_color_3` int(30) DEFAULT NULL,
  `unk201_7` int(30) NOT NULL DEFAULT '0',
  `socket_bonus` int(30) DEFAULT NULL,
  `GemProperties` int(30) DEFAULT NULL,
  `ReqDisenchantSkill` int(30) NOT NULL DEFAULT '-1',
  `ArmorDamageModifier` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

-- ----------------------------
-- Records of items
-- ----------------------------

-- ----------------------------
-- Table structure for `map_checkpoint`
-- ----------------------------
DROP TABLE IF EXISTS `map_checkpoint`;
CREATE TABLE `map_checkpoint` (
  `entry` int(30) NOT NULL,
  `prereq_checkpoint_id` int(30) NOT NULL,
  `creature_id` int(30) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Map System';

-- ----------------------------
-- Records of map_checkpoint
-- ----------------------------

-- ----------------------------
-- Table structure for `millingloot`
-- ----------------------------
DROP TABLE IF EXISTS `millingloot`;
CREATE TABLE `millingloot` (
  `index` int(11) NOT NULL AUTO_INCREMENT,
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float DEFAULT '0',
  `mincount` int(30) DEFAULT '1',
  `maxcount` int(30) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`index`)
) ENGINE=MyISAM AUTO_INCREMENT=114 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of millingloot
-- ----------------------------

-- ----------------------------
-- Table structure for `npc_gossip_textid`
-- ----------------------------
DROP TABLE IF EXISTS `npc_gossip_textid`;
CREATE TABLE `npc_gossip_textid` (
  `creatureid` int(10) unsigned NOT NULL DEFAULT '0',
  `textid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`creatureid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Records of npc_gossip_textid
-- ----------------------------

-- ----------------------------
-- Table structure for `npc_monstersay`
-- ----------------------------
DROP TABLE IF EXISTS `npc_monstersay`;
CREATE TABLE `npc_monstersay` (
  `entry` int(11) unsigned NOT NULL DEFAULT '0',
  `event` int(11) unsigned NOT NULL DEFAULT '0',
  `chance` float NOT NULL DEFAULT '0',
  `language` int(11) unsigned NOT NULL DEFAULT '0',
  `type` int(11) unsigned NOT NULL DEFAULT '0',
  `monstername` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text0` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text1` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text2` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text3` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text4` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Records of npc_monstersay
-- ----------------------------

-- ----------------------------
-- Table structure for `npc_text`
-- ----------------------------
DROP TABLE IF EXISTS `npc_text`;
CREATE TABLE `npc_text` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `prob0` float NOT NULL DEFAULT '0',
  `text0_0` longtext NOT NULL,
  `text0_1` longtext NOT NULL,
  `lang0` int(10) unsigned NOT NULL DEFAULT '0',
  `em0_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em0_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em0_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em0_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em0_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em0_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob1` float NOT NULL DEFAULT '0',
  `text1_0` longtext NOT NULL,
  `text1_1` longtext NOT NULL,
  `lang1` int(10) unsigned NOT NULL DEFAULT '0',
  `em1_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em1_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em1_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em1_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em1_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em1_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob2` float NOT NULL DEFAULT '0',
  `text2_0` longtext NOT NULL,
  `text2_1` longtext NOT NULL,
  `lang2` int(10) unsigned NOT NULL DEFAULT '0',
  `em2_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em2_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em2_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em2_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em2_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em2_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob3` float NOT NULL DEFAULT '0',
  `text3_0` longtext NOT NULL,
  `text3_1` longtext NOT NULL,
  `lang3` int(10) unsigned NOT NULL DEFAULT '0',
  `em3_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em3_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em3_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em3_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em3_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em3_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob4` float NOT NULL DEFAULT '0',
  `text4_0` longtext NOT NULL,
  `text4_1` longtext NOT NULL,
  `lang4` int(10) unsigned NOT NULL DEFAULT '0',
  `em4_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em4_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em4_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em4_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em4_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em4_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob5` float NOT NULL DEFAULT '0',
  `text5_0` longtext NOT NULL,
  `text5_1` longtext NOT NULL,
  `lang5` int(10) unsigned NOT NULL DEFAULT '0',
  `em5_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em5_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em5_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em5_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em5_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em5_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob6` float NOT NULL DEFAULT '0',
  `text6_0` longtext NOT NULL,
  `text6_1` longtext NOT NULL,
  `lang6` int(10) unsigned NOT NULL DEFAULT '0',
  `em6_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em6_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em6_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em6_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em6_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em6_5` int(10) unsigned NOT NULL DEFAULT '0',
  `prob7` float NOT NULL DEFAULT '0',
  `text7_0` longtext NOT NULL,
  `text7_1` longtext NOT NULL,
  `lang7` int(10) unsigned NOT NULL DEFAULT '0',
  `em7_0` int(10) unsigned NOT NULL DEFAULT '0',
  `em7_1` int(10) unsigned NOT NULL DEFAULT '0',
  `em7_2` int(10) unsigned NOT NULL DEFAULT '0',
  `em7_3` int(10) unsigned NOT NULL DEFAULT '0',
  `em7_4` int(10) unsigned NOT NULL DEFAULT '0',
  `em7_5` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Records of npc_text
-- ----------------------------

-- ----------------------------
-- Table structure for `objectloot`
-- ----------------------------
DROP TABLE IF EXISTS `objectloot`;
CREATE TABLE `objectloot` (
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float NOT NULL DEFAULT '0',
  `mincount` int(11) unsigned NOT NULL DEFAULT '1',
  `maxcount` int(11) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Records of objectloot
-- ----------------------------

-- ----------------------------
-- Table structure for `petdefaultspells`
-- ----------------------------
DROP TABLE IF EXISTS `petdefaultspells`;
CREATE TABLE `petdefaultspells` (
  `entry` int(11) NOT NULL DEFAULT '0',
  `spell` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Pet System';

-- ----------------------------
-- Records of petdefaultspells
-- ----------------------------

-- ----------------------------
-- Table structure for `pickpocketingloot`
-- ----------------------------
DROP TABLE IF EXISTS `pickpocketingloot`;
CREATE TABLE `pickpocketingloot` (
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '100',
  `heroicpercentchance` float DEFAULT '0',
  `mincount` int(30) DEFAULT '1',
  `maxcount` int(30) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

-- ----------------------------
-- Records of pickpocketingloot
-- ----------------------------

-- ----------------------------
-- Table structure for `playercreateinfo`
-- ----------------------------
DROP TABLE IF EXISTS `playercreateinfo`;
CREATE TABLE `playercreateinfo` (
  `Index` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `factiontemplate` int(5) NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapID` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `zoneID` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  `displayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseStrength` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseAgility` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseStamina` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseIntellect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseSpirit` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseHealth` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseMana` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseRage` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseFocus` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseEnergy` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseRunic` int(10) unsigned NOT NULL DEFAULT '0',
  `attackpower` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `mindmg` float NOT NULL DEFAULT '0',
  `maxdmg` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`Index`)
) ENGINE=MyISAM AUTO_INCREMENT=66 DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Records of playercreateinfo
-- ----------------------------

-- ----------------------------
-- Table structure for `playercreateinfo_bars`
-- ----------------------------
DROP TABLE IF EXISTS `playercreateinfo_bars`;
CREATE TABLE `playercreateinfo_bars` (
  `race` tinyint(3) unsigned DEFAULT NULL,
  `class` tinyint(3) unsigned DEFAULT NULL,
  `button` int(10) unsigned DEFAULT NULL,
  `action` int(10) unsigned DEFAULT NULL,
  `type` int(10) unsigned DEFAULT NULL,
  `misc` int(10) unsigned DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Records of playercreateinfo_bars
-- ----------------------------

-- ----------------------------
-- Table structure for `playercreateinfo_items`
-- ----------------------------
DROP TABLE IF EXISTS `playercreateinfo_items`;
CREATE TABLE `playercreateinfo_items` (
  `indexid` int(4) NOT NULL,
  `protoid` int(4) NOT NULL,
  `slotid` int(10) NOT NULL,
  `amount` int(3) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Records of playercreateinfo_items
-- ----------------------------

-- ----------------------------
-- Table structure for `playercreateinfo_skills`
-- ----------------------------
DROP TABLE IF EXISTS `playercreateinfo_skills`;
CREATE TABLE `playercreateinfo_skills` (
  `indexid` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `skillid` int(10) unsigned NOT NULL DEFAULT '0',
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `maxlevel` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Records of playercreateinfo_skills
-- ----------------------------

-- ----------------------------
-- Table structure for `playercreateinfo_spells`
-- ----------------------------
DROP TABLE IF EXISTS `playercreateinfo_spells`;
CREATE TABLE `playercreateinfo_spells` (
  `indexid` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `spellid` smallint(5) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

-- ----------------------------
-- Records of playercreateinfo_spells
-- ----------------------------

-- ----------------------------
-- Table structure for `prestartqueries`
-- ----------------------------
DROP TABLE IF EXISTS `prestartqueries`;
CREATE TABLE `prestartqueries` (
  `Query` varchar(1024) NOT NULL,
  `SingleShot` int(1) unsigned NOT NULL DEFAULT '1',
  `Seq` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`Seq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of prestartqueries
-- ----------------------------

-- ----------------------------
-- Table structure for `professiondiscoveries`
-- ----------------------------
DROP TABLE IF EXISTS `professiondiscoveries`;
CREATE TABLE `professiondiscoveries` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellToDiscover` int(10) unsigned NOT NULL DEFAULT '0',
  `SkillValue` int(10) unsigned NOT NULL DEFAULT '0',
  `Chance` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`,`SpellToDiscover`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of professiondiscoveries
-- ----------------------------

-- ----------------------------
-- Table structure for `prospectingloot`
-- ----------------------------
DROP TABLE IF EXISTS `prospectingloot`;
CREATE TABLE `prospectingloot` (
  `entryid` int(11) unsigned NOT NULL DEFAULT '0',
  `itemid` int(11) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '0',
  `heroicpercentchance` float DEFAULT '0',
  `mincount` int(30) DEFAULT '1',
  `maxcount` int(30) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of prospectingloot
-- ----------------------------

-- ----------------------------
-- Table structure for `quests`
-- ----------------------------
DROP TABLE IF EXISTS `quests`;
CREATE TABLE `quests` (
  `entry` int(11) unsigned NOT NULL,
  `ZoneId` int(11) NOT NULL DEFAULT '0',
  `sort` int(11) NOT NULL DEFAULT '0',
  `flags` int(11) unsigned NOT NULL DEFAULT '0',
  `MinLevel` int(11) unsigned NOT NULL DEFAULT '0',
  `questlevel` int(11) unsigned NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  `RequiredRaces` int(11) NOT NULL DEFAULT '0',
  `RequiredClass` int(11) NOT NULL DEFAULT '0',
  `RequiredTradeskill` int(11) NOT NULL DEFAULT '0',
  `RequiredTradeskillValue` int(11) NOT NULL DEFAULT '0',
  `RequiredRepFaction` int(11) NOT NULL DEFAULT '0',
  `RequiredRepValue` int(11) NOT NULL DEFAULT '0',
  `SuggestedPlayers` int(11) unsigned NOT NULL DEFAULT '0',
  `LimitTime` int(11) NOT NULL DEFAULT '0',
  `SpecialFlags` int(11) unsigned NOT NULL DEFAULT '0',
  `PrevQuestId` int(11) unsigned NOT NULL DEFAULT '0',
  `NextQuestId` int(11) unsigned NOT NULL DEFAULT '0',
  `srcItem` int(11) unsigned NOT NULL DEFAULT '0',
  `SrcItemCount` int(11) unsigned NOT NULL DEFAULT '0',
  `Title` text NOT NULL,
  `Details` text NOT NULL,
  `Objectives` text NOT NULL,
  `CompletionText` text NOT NULL,
  `IncompleteText` text NOT NULL,
  `EndText` text NOT NULL,
  `ObjectiveText1` text NOT NULL,
  `ObjectiveText2` text NOT NULL,
  `ObjectiveText3` text NOT NULL,
  `ObjectiveText4` text NOT NULL,
  `ReqItemId1` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemId2` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemId3` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemId4` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemId5` int(11) NOT NULL DEFAULT '0',
  `ReqItemId6` int(11) NOT NULL DEFAULT '0',
  `ReqItemCount1` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount2` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount3` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount4` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount5` int(11) NOT NULL DEFAULT '0',
  `ReqItemCount6` int(11) NOT NULL DEFAULT '0',
  `ReqKillPlayer` int(10) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId1` bigint(11) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId2` bigint(11) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId3` bigint(11) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId4` bigint(11) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount1` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount2` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount3` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount4` int(11) unsigned NOT NULL DEFAULT '0',
  `ReqCastSpellId1` int(11) NOT NULL DEFAULT '0',
  `ReqCastSpellId2` int(11) NOT NULL DEFAULT '0',
  `ReqCastSpellId3` int(11) NOT NULL DEFAULT '0',
  `ReqCastSpellId4` int(11) NOT NULL DEFAULT '0',
  `RewChoiceItemId1` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId2` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId3` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId4` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId5` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId6` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount1` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount2` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount3` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount4` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount5` int(11) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount6` int(11) unsigned NOT NULL DEFAULT '0',
  `RewItemId1` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemId2` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemId3` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemId4` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemCount1` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemCount2` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemCount3` int(10) unsigned NOT NULL DEFAULT '0',
  `RewItemCount4` int(10) unsigned NOT NULL DEFAULT '0',
  `RewRepFaction1` int(10) unsigned NOT NULL DEFAULT '0',
  `RewRepFaction2` int(10) unsigned NOT NULL DEFAULT '0',
  `RewRepFaction3` int(11) NOT NULL DEFAULT '0',
  `RewRepFaction4` int(11) NOT NULL DEFAULT '0',
  `RewRepFaction5` int(11) NOT NULL DEFAULT '0',
  `RewRepValue1` int(10) NOT NULL DEFAULT '0',
  `RewRepValue2` int(10) NOT NULL DEFAULT '0',
  `RewRepValue3` int(11) NOT NULL DEFAULT '0',
  `RewRepValue4` int(11) NOT NULL DEFAULT '0',
  `RewRepValue5` int(11) NOT NULL DEFAULT '0',
  `RewRepLimit` int(10) unsigned NOT NULL DEFAULT '0',
  `RewTitle` int(11) unsigned NOT NULL DEFAULT '0',
  `RewMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `RewBonusHonor` int(10) unsigned NOT NULL DEFAULT '0',
  `RewXP` int(10) unsigned NOT NULL DEFAULT '0',
  `RewSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `RewTalent` int(11) unsigned NOT NULL DEFAULT '0',
  `CastSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `PointMapId` int(10) unsigned NOT NULL DEFAULT '0',
  `PointX` float NOT NULL DEFAULT '0',
  `PointY` float NOT NULL DEFAULT '0',
  `PointOpt` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredMoney` int(11) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger1` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger2` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger3` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest1` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest_and_or` int(3) NOT NULL DEFAULT '0',
  `ReceiveItemId1` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemId2` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemId3` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemId4` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount1` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount2` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount3` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount4` int(10) unsigned NOT NULL DEFAULT '0',
  `IsRepeatable` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quests System';

-- ----------------------------
-- Records of quests
-- ----------------------------

-- ----------------------------
-- Table structure for `randomcardcreation`
-- ----------------------------
DROP TABLE IF EXISTS `randomcardcreation`;
CREATE TABLE `randomcardcreation` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId0` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId1` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId2` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId3` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId4` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId5` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId6` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId7` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId8` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId9` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId10` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId11` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId12` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId13` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId14` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId15` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId16` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId17` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId18` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId19` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId20` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId21` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId22` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId23` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId24` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId25` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId26` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId27` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId28` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId29` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId30` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId31` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemCount` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of randomcardcreation
-- ----------------------------

-- ----------------------------
-- Table structure for `randomitemcreation`
-- ----------------------------
DROP TABLE IF EXISTS `randomitemcreation`;
CREATE TABLE `randomitemcreation` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemToCreate` int(10) unsigned NOT NULL DEFAULT '0',
  `Skill` int(10) unsigned NOT NULL DEFAULT '0',
  `Chance` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ItemToCreate`,`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of randomitemcreation
-- ----------------------------

-- ----------------------------
-- Table structure for `recall`
-- ----------------------------
DROP TABLE IF EXISTS `recall`;
CREATE TABLE `recall` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `MapId` int(10) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=970 DEFAULT CHARSET=utf8 COMMENT='Tele Command';

-- ----------------------------
-- Records of recall
-- ----------------------------

-- ----------------------------
-- Table structure for `reputation_creature_onkill`
-- ----------------------------
DROP TABLE IF EXISTS `reputation_creature_onkill`;
CREATE TABLE `reputation_creature_onkill` (
  `creature_id` int(30) NOT NULL,
  `faction_change_alliance` int(30) NOT NULL,
  `faction_change_horde` int(30) NOT NULL,
  `change_value` int(30) NOT NULL,
  `rep_limit` int(30) NOT NULL,
  KEY `index` (`creature_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of reputation_creature_onkill
-- ----------------------------

-- ----------------------------
-- Table structure for `reputation_faction_onkill`
-- ----------------------------
DROP TABLE IF EXISTS `reputation_faction_onkill`;
CREATE TABLE `reputation_faction_onkill` (
  `faction_id` int(30) NOT NULL,
  `change_factionid_alliance` int(30) NOT NULL,
  `change_deltamin_alliance` int(30) NOT NULL,
  `change_deltamax_alliance` int(30) NOT NULL,
  `change_factionid_horde` int(30) NOT NULL,
  `change_deltamin_horde` int(30) NOT NULL,
  `change_deltamax_horde` int(30) NOT NULL,
  KEY `factindex` (`faction_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of reputation_faction_onkill
-- ----------------------------

-- ----------------------------
-- Table structure for `reputation_instance_onkill`
-- ----------------------------
DROP TABLE IF EXISTS `reputation_instance_onkill`;
CREATE TABLE `reputation_instance_onkill` (
  `mapid` int(30) NOT NULL,
  `mob_rep_reward` int(30) NOT NULL,
  `mob_rep_reward_heroic` int(30) NOT NULL,
  `mob_rep_limit` int(30) NOT NULL,
  `mob_rep_limit_heroic` int(30) NOT NULL,
  `boss_rep_reward` int(30) NOT NULL,
  `boss_rep_reward_heroic` int(30) NOT NULL,
  `boss_rep_limit` int(30) NOT NULL,
  `boss_rep_limit_heroic` int(30) NOT NULL,
  `faction_change_alliance` int(30) NOT NULL,
  `faction_change_horde` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

-- ----------------------------
-- Records of reputation_instance_onkill
-- ----------------------------

-- ----------------------------
-- Table structure for `scrollcreation`
-- ----------------------------
DROP TABLE IF EXISTS `scrollcreation`;
CREATE TABLE `scrollcreation` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`,`ItemId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of scrollcreation
-- ----------------------------

-- ----------------------------
-- Table structure for `spell_coef_override`
-- ----------------------------
DROP TABLE IF EXISTS `spell_coef_override`;
CREATE TABLE `spell_coef_override` (
  `id` int(11) DEFAULT '0',
  `Dspell_coef_override` float DEFAULT '0',
  `OTspell_coef_override` float DEFAULT '0',
  `AP_coef_override` float DEFAULT '0',
  `RAP_coef_override` float DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of spell_coef_override
-- ----------------------------

-- ----------------------------
-- Table structure for `spell_disable`
-- ----------------------------
DROP TABLE IF EXISTS `spell_disable`;
CREATE TABLE `spell_disable` (
  `spellid` int(30) NOT NULL,
  `replacement_spellid` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

-- ----------------------------
-- Records of spell_disable
-- ----------------------------

-- ----------------------------
-- Table structure for `spell_disable_trainers`
-- ----------------------------
DROP TABLE IF EXISTS `spell_disable_trainers`;
CREATE TABLE `spell_disable_trainers` (
  `spellid` int(30) NOT NULL,
  `replacement_spellid` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Trainer System';

-- ----------------------------
-- Records of spell_disable_trainers
-- ----------------------------

-- ----------------------------
-- Table structure for `spell_effects_override`
-- ----------------------------
DROP TABLE IF EXISTS `spell_effects_override`;
CREATE TABLE `spell_effects_override` (
  `spellId` int(30) NOT NULL DEFAULT '0',
  `EffectID` int(30) NOT NULL,
  `Disable` int(30) NOT NULL,
  `Effect` int(30) NOT NULL DEFAULT '0',
  `BasePoints` int(30) NOT NULL DEFAULT '0',
  `ApplyAuraName` int(30) NOT NULL DEFAULT '0',
  `SpellGroupRelation` int(30) NOT NULL DEFAULT '0',
  `MiscValue` int(30) NOT NULL DEFAULT '0',
  `TriggerSpell` int(30) NOT NULL DEFAULT '0',
  `ImplicitTargetA` int(30) NOT NULL DEFAULT '0',
  `ImplicitTargetB` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of spell_effects_override
-- ----------------------------

-- ----------------------------
-- Table structure for `spell_forced_targets`
-- ----------------------------
DROP TABLE IF EXISTS `spell_forced_targets`;
CREATE TABLE `spell_forced_targets` (
  `spellid` int(8) unsigned NOT NULL,
  `target` int(8) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `entry_type_target` (`spellid`,`target`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spell System';

-- ----------------------------
-- Records of spell_forced_targets
-- ----------------------------

-- ----------------------------
-- Table structure for `spell_proc`
-- ----------------------------
DROP TABLE IF EXISTS `spell_proc`;
CREATE TABLE `spell_proc` (
  `spellID` int(30) NOT NULL DEFAULT '0',
  `ProcOnNameHash` int(30) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellID`,`ProcOnNameHash`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of spell_proc
-- ----------------------------

-- ----------------------------
-- Table structure for `spelloverride`
-- ----------------------------
DROP TABLE IF EXISTS `spelloverride`;
CREATE TABLE `spelloverride` (
  `overrideId` int(30) unsigned NOT NULL DEFAULT '0',
  `spellId` int(30) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `overrideId` (`overrideId`,`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

-- ----------------------------
-- Records of spelloverride
-- ----------------------------

-- ----------------------------
-- Table structure for `teleport_coords`
-- ----------------------------
DROP TABLE IF EXISTS `teleport_coords`;
CREATE TABLE `teleport_coords` (
  `entry` int(16) NOT NULL,
  `name` char(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `mapId` int(16) NOT NULL,
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Records of teleport_coords
-- ----------------------------

-- ----------------------------
-- Table structure for `totemspells`
-- ----------------------------
DROP TABLE IF EXISTS `totemspells`;
CREATE TABLE `totemspells` (
  `spell` int(10) unsigned NOT NULL DEFAULT '0',
  `castspell1` int(10) unsigned NOT NULL DEFAULT '0',
  `castspell2` int(10) unsigned NOT NULL DEFAULT '0',
  `castspell3` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

-- ----------------------------
-- Records of totemspells
-- ----------------------------

-- ----------------------------
-- Table structure for `trainer_defs`
-- ----------------------------
DROP TABLE IF EXISTS `trainer_defs`;
CREATE TABLE `trainer_defs` (
  `entry` int(11) unsigned NOT NULL DEFAULT '0',
  `required_skill` int(11) unsigned NOT NULL DEFAULT '0',
  `required_skillvalue` int(11) unsigned DEFAULT '0',
  `req_class` int(11) unsigned NOT NULL DEFAULT '0',
  `trainer_type` int(11) unsigned NOT NULL DEFAULT '0',
  `trainer_ui_window_message` text,
  `can_train_gossip_textid` int(11) NOT NULL,
  `cannot_train_gossip_textid` int(11) NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

-- ----------------------------
-- Records of trainer_defs
-- ----------------------------

-- ----------------------------
-- Table structure for `trainer_spells`
-- ----------------------------
DROP TABLE IF EXISTS `trainer_spells`;
CREATE TABLE `trainer_spells` (
  `entry` int(11) unsigned NOT NULL DEFAULT '0',
  `cast_spell` int(11) unsigned NOT NULL DEFAULT '0',
  `learn_spell` int(11) unsigned NOT NULL,
  `spellcost` int(11) unsigned NOT NULL DEFAULT '0',
  `reqspell` int(11) unsigned NOT NULL DEFAULT '0',
  `reqskill` int(11) unsigned NOT NULL DEFAULT '0',
  `reqskillvalue` int(11) unsigned NOT NULL DEFAULT '0',
  `reqlevel` int(11) unsigned NOT NULL DEFAULT '0',
  `deletespell` int(11) unsigned NOT NULL DEFAULT '0',
  `is_prof` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`cast_spell`,`learn_spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

-- ----------------------------
-- Records of trainer_spells
-- ----------------------------

-- ----------------------------
-- Table structure for `trainerspelloverride`
-- ----------------------------
DROP TABLE IF EXISTS `trainerspelloverride`;
CREATE TABLE `trainerspelloverride` (
  `spellid` int(10) unsigned NOT NULL DEFAULT '0',
  `cost` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredspell` int(10) unsigned NOT NULL DEFAULT '0',
  `deletespell` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredskill` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredskillvalue` int(10) unsigned NOT NULL DEFAULT '0',
  `reqlevel` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredclass` int(10) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `spellid` (`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Trainer System';

-- ----------------------------
-- Records of trainerspelloverride
-- ----------------------------

-- ----------------------------
-- Table structure for `transport_creatures`
-- ----------------------------
DROP TABLE IF EXISTS `transport_creatures`;
CREATE TABLE `transport_creatures` (
  `transport_entry` int(10) unsigned NOT NULL,
  `creature_entry` int(10) unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of transport_creatures
-- ----------------------------

-- ----------------------------
-- Table structure for `vendors`
-- ----------------------------
DROP TABLE IF EXISTS `vendors`;
CREATE TABLE `vendors` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `item` int(10) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) NOT NULL DEFAULT '0',
  `max_amount` int(11) NOT NULL DEFAULT '0',
  `inctime` bigint(20) NOT NULL DEFAULT '0',
  `extendedcost` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`entry`,`item`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

-- ----------------------------
-- Records of vendors
-- ----------------------------

-- ----------------------------
-- Table structure for `weather`
-- ----------------------------
DROP TABLE IF EXISTS `weather`;
CREATE TABLE `weather` (
  `zoneId` int(11) unsigned NOT NULL DEFAULT '0',
  `high_chance` int(11) unsigned NOT NULL DEFAULT '0',
  `high_type` int(11) unsigned NOT NULL DEFAULT '0',
  `med_chance` int(11) unsigned NOT NULL DEFAULT '0',
  `med_type` int(11) unsigned NOT NULL DEFAULT '0',
  `low_chance` int(11) unsigned NOT NULL DEFAULT '0',
  `low_type` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`zoneId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Weather System';

-- ----------------------------
-- Records of weather
-- ----------------------------

-- ----------------------------
-- Table structure for `wordfilter_character_names`
-- ----------------------------
DROP TABLE IF EXISTS `wordfilter_character_names`;
CREATE TABLE `wordfilter_character_names` (
  `regex_match` varchar(255) NOT NULL,
  `regex_ignore_if_matched` varchar(255) DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of wordfilter_character_names
-- ----------------------------

-- ----------------------------
-- Table structure for `wordfilter_chat`
-- ----------------------------
DROP TABLE IF EXISTS `wordfilter_chat`;
CREATE TABLE `wordfilter_chat` (
  `regex_match` varchar(500) NOT NULL,
  `regex_ignore_if_matched` varchar(500) NOT NULL DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of wordfilter_chat
-- ----------------------------

-- ----------------------------
-- Table structure for `worldmap_info`
-- ----------------------------
DROP TABLE IF EXISTS `worldmap_info`;
CREATE TABLE `worldmap_info` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `screenid` int(10) unsigned DEFAULT '0',
  `type` int(10) unsigned DEFAULT '0',
  `maxplayers` int(10) unsigned DEFAULT '0',
  `minlevel` int(10) unsigned DEFAULT '1',
  `repopx` float DEFAULT '0',
  `repopy` float DEFAULT '0',
  `repopz` float DEFAULT '0',
  `repopentry` int(10) unsigned DEFAULT '0',
  `area_name` varchar(100) DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `cooldown` int(10) unsigned NOT NULL DEFAULT '0',
  `required_quest` int(10) unsigned NOT NULL DEFAULT '0',
  `required_item` int(10) unsigned NOT NULL DEFAULT '0',
  `heroic_keyid_1` int(30) NOT NULL DEFAULT '0',
  `heroic_keyid_2` int(30) NOT NULL DEFAULT '0',
  `viewingDistance` float NOT NULL DEFAULT '80',
  `required_checkpoint` int(30) NOT NULL DEFAULT '0',
  `collision` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Records of worldmap_info
-- ----------------------------

-- ----------------------------
-- Table structure for `worldstate_template`
-- ----------------------------
DROP TABLE IF EXISTS `worldstate_template`;
CREATE TABLE `worldstate_template` (
  `mapid` int(11) NOT NULL,
  `zone_mask` int(30) NOT NULL,
  `faction_mask` int(30) NOT NULL,
  `field_number` int(30) unsigned NOT NULL,
  `initial_value` int(30) NOT NULL,
  `comment` varchar(200) NOT NULL,
  PRIMARY KEY (`field_number`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of worldstate_template
-- ----------------------------

-- ----------------------------
-- Table structure for `zoneguards`
-- ----------------------------
DROP TABLE IF EXISTS `zoneguards`;
CREATE TABLE `zoneguards` (
  `zone` int(10) unsigned NOT NULL,
  `horde_entry` int(10) unsigned DEFAULT NULL,
  `alliance_entry` int(10) unsigned DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

-- ----------------------------
-- Records of zoneguards
-- ----------------------------
