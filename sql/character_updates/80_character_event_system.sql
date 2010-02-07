DROP TABLE IF EXISTS `events_settings`;

CREATE TABLE `events_settings` (
  `eventid` tinyint(2) unsigned NOT NULL,
  `lastactivated` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`eventid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

