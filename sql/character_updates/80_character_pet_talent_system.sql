DROP TABLE IF EXISTS `playerpetactionbar`;

CREATE TABLE `playerpetactionbar` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(11) NOT NULL DEFAULT '0',
  `spellid_1` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_2` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_3` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_4` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_5` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_6` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_7` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_8` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_9` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_10` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_1` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_2` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_3` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_4` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_5` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_6` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_7` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_8` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_9` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_10` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ownerguid`,`petnumber`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `playerpettalents`;

CREATE TABLE `playerpettalents` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(4) NOT NULL DEFAULT '0',
  `talentid` int(11) NOT NULL DEFAULT '0',
  `rank` tinyint(4) NOT NULL DEFAULT '0',
  KEY `a` (`ownerguid`),
  KEY `b` (`petnumber`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


ALTER TABLE `playerpets` DROP COLUMN `actionbar`;
ALTER TABLE `playerpets` DROP COLUMN `loyaltyupdate`;
ALTER TABLE `playerpets` DROP COLUMN `loyaltypts`;
