ALTER TABLE `creature_staticspawns` ADD COLUMN `vehicle` int(10) NOT NULL DEFAULT 0 AFTER `Phase`;

ALTER TABLE `creature_staticspawns` ADD COLUMN `eventid` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'Relates to events table' AFTER `vehicle`;



ALTER TABLE `gameobject_staticspawns` ADD COLUMN `vehicle` int(10) NOT NULL DEFAULT 0 AFTER `phase`;

ALTER TABLE `gameobject_staticspawns` ADD COLUMN `eventid` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'Relates to events table' AFTER `vehicle`;