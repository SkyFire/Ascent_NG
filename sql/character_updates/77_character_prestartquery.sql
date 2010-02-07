/*
Date: 19/07/2008 23:01:01
*/

-- ----------------------------
-- Table structure for prestartqueries
-- ----------------------------
CREATE TABLE `prestartqueries` (
  `Query` varchar(1024) NOT NULL,
  `SingleShot` int(1) unsigned NOT NULL default '1',
  `Seq` int(11) NOT NULL auto_increment,
  PRIMARY KEY  (`Seq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records (Character database)
-- ----------------------------
INSERT INTO prestartqueries (`Query`, `SingleShot` ) VALUES ('DELETE FROM T1 USING corpses T1 LEFT JOIN instances As T2 ON T1.instanceid = T2.id WHERE  T2.id IS NULL;', 0);
INSERT INTO prestartqueries (`Query`, `SingleShot` ) VALUES ('UPDATE characters SET banned=0,banReason=\'\' WHERE banned > 100 AND banned < UNIX_TIMESTAMP();', 0);
INSERT INTO prestartqueries (`Query`, `SingleShot` ) VALUES ('DELETE FROM guild_logs WHERE timestamp < (UNIX_TIMESTAMP()-1209600);', 0);
INSERT INTO prestartqueries (`Query`, `SingleShot` ) VALUES ('DELETE FROM guild_banklogs WHERE timestamp < (UNIX_TIMESTAMP()-1209600);', 0);
INSERT INTO prestartqueries (`Query`, `SingleShot` ) VALUES ('DELETE from playeritems WHERE count = 0;', 0);

