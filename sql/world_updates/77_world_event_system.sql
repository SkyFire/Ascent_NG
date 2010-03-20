
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

DROP TABLE IF EXISTS `events_gameobject`;

CREATE TABLE `events_gameobject` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'This relates to gameobject_spawns eventid',
  `id` int(11) unsigned NOT NULL COMMENT 'Relates to gameobject_spawn id',
  `changesflag` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'See enum in core for permissable flags in World.h',
  `phase` tinyint(3) DEFAULT NULL COMMENT 'Phase the go is in when the event is active',
  `displayid` int(10) unsigned DEFAULT NULL COMMENT 'This is the displayid to change to while the event is active',
  PRIMARY KEY (`eventid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


ALTER TABLE `gameobject_spawns` ADD COLUMN `eventid` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'relates to events table' AFTER `phase` ;

ALTER TABLE `creature_spawns` ADD COLUMN `eventid` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'Relates to events table' AFTER `vehicle`;
