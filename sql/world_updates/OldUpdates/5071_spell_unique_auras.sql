#New unique auras table
DROP TABLE IF EXISTS `spell_unique_auras`;

CREATE TABLE `spell_unique_auras` (
  `SpellId` int(30) NOT NULL,
  `AuraGroup` int(30) NOT NULL,
  `AuraGroup2` int(20) NOT NULL default '0',
  `UniqueType` int(30) NOT NULL,
  PRIMARY KEY  (`SpellId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

#Group 1: Armor spells (Warlock/Mage armors)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (706,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1086,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6117,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7302,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7320,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10219,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10220,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11733,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11734,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11735,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (22782,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (22783,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27124,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27125,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27260,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (28176,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (28189,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (30482,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (687,1,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (696,1,1);


#Group 2: Shadow vulnerability (from Improved Shadow Bolt proc)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (17794,2,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (17797,2,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (17798,2,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (17799,2,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (17800,2,1);

#Group 3: Shadow vulnerability (from Shadow Weaving proc)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (15258,3,1);

#Group 4: Fire vulnerability (from Improved Scorch proc)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (22959,4,1);

#Group 5: Winter's Chill (from Winter's Chill proc)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (12579,5,1);

#Group 6: Warlock curses

#Curse of the elements, 1 per target
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1490,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11721,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11722,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27228,6,1);
#Curse of Agony
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (980,6,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1014,6,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6217,6,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11711,6,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11712,6,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11713,6,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27218,6,0);
#Curse of Doom, 1 per target
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (603,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (30910,6,1);
#Curse of Exhaustion, 1 per target
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (18223,6,1);
#Curse of Recklessness, 1 per target
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (704,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7658,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7659,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11717,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27226,6,1);
#Curse of Tongues, 1 per target
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1714,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11719,6,1);
#Curse of Weakness, 1 per target
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (702,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1108,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6205,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7646,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11707,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11708,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27224,6,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (30909,6,1);

#Group 7: Attack power reducing auras (demo shout/roar currently)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (99,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1160,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1735,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6190,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (9490,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (9747,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (9898,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11554,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11555,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25202,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25203,7,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (26998,7,1);

#Group 8: Armor reducing auras (sunder armor/expose armor currently)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7386,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (7405,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (8380,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (8647,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (8649,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (8650,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11197,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11198,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11596,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11597,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25225,8,1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (26866,8,1);

#Group 9: Paladin blessings (including greater blessings)

insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1044,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20217,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19977,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19978,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19979,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27144,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19740,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19834,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19835,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19836,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19837,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19838,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25291,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27140,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1022,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (5599,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10278,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6940,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20729,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27147,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27148,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1038,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20911,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20912,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20913,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20914,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27168,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19742,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19850,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19852,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19853,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19854,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25290,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27142,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25898,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25890,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27145,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25782,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25916,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27141,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25895,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25899,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27169,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25894,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25918,9,0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27143,9,0);

#Group 10 Trueshot Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (19506, 10, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20905, 10, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20906, 10, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27066, 10, 1);

#Group 11 Deleted, reuse please

#Group 12: Hunter's Mark (only 1 target at a time)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1130, 12, 2);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14323, 12, 2);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14324, 12, 2);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14325, 12, 2);

#Group 13: Fortitude buff (power word, prayer)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1243, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1244, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1245, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (2791, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10937, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10938, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25389, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (51562, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (21564, 13, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25392, 13, 1);

#Group 14: Intellect buff (arcane intellect, arcane brilliance)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1459, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1460, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1461, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10156, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10157, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27126, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (23028, 14, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27127, 14, 1);

#Group 15: Armor/resist buff (mark of the wild, gift of the wild)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1126, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (5232, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6756, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (5234, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (8907, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (9884, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (9985, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (26990, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (21849, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (21850, 15, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (26991, 15, 1);

#Group 16: Fear (1 up per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (5782, 16, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6213, 16, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6215, 16, 3);

#Group 17: Earth shield (1 up per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (974, 17, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (32593, 17, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (32594, 17, 3);

#Group 18: Polymorph (1 up per targets, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (118, 18, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (12824, 18, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (12825, 18, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (28272, 18, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (28271, 18, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (12826, 18, 3);

#Group 19 Sap (1 up per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6770, 19, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (2070, 19, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (11297, 19, 3);

#Group 20 Judgement of Justice (1 per target)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20184, 20, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (31896, 20, 1);

#Group 21 Judgement of Light (1 per target)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20185, 21, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20344, 21, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20345, 21, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20346, 21, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27162, 21, 1);

#Group 22 Judgement of the Crusader (1 per target)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20188, 22, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20300, 22, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20301, 22, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20302, 22, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20303, 22, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27159, 22, 1);

#Group 23 Judgement of Wisdom (1 per target)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20186, 23, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20354, 23, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (20355, 23, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27164, 23, 1);

#Group 24 Banish (1 per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (710, 24, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (18647, 24, 3);

#Group 25 Cyclone (1 per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (33786, 25, 3);

#Group 26 Hibernate (1 per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (2637, 26, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (18657, 26, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (18658, 26, 3);

#Group 27 Scare Beast (1 per target, 1 per caster max)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (1513, 27, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14326, 27, 3);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14327, 27, 3);

#Group 28 Haste (hunter pouches/quivers, and legendary bow)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (44972, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14829, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14825, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14826, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14828, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (14827, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (29414, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (29146, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (29413, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (29417, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (29415, 28, 0);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (29418, 28, 0);

#Group 29 Moonkin Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (24907, 29, 1);

#Group 30 Leader of the Pack
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (24932, 30, 1);

#Group 31 Mage roots (frost nova, freeze)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (122, 31, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (865, 31, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (6131, 31, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10230, 31, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27088, 31, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (33395, 31, 1);

#Group 32 Concentration Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19746, 32, 11, 1);

#Group 33 Crusader Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (32223, 33, 11, 1);

#Group 34 Devotion Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (465, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10290, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (643, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10291, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (1032, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10292, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10293, 34, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27149, 34, 11, 1);

#Group 35 Fire Resistance Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19891, 35, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19899, 35, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19900, 35, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27153, 35, 11, 1);

#Group 36 Frost Resistance Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19888, 36, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19897, 36, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19898, 36, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27152, 36, 11, 1);

#Group 37 Retribution Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (7294, 37, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10298, 37, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10299, 37, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10300, 37, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (10301, 37, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27150, 37, 11, 1);

#Group 38 Sanctity Aura
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (20218, 38, 11, 1);

#Group 39 Shadow Resistance Aura and Prayer of Shadow Protection
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19876, 39, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19895, 39, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (19896, 39, 11, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27151, 39, 11, 1);
#Prayer, no second group
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (27683, 39, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (36374, 39, 1);


#Group 40 Aspect of the Beast and Cheetah
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (13161, 40, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (5118, 40, 12, 1);

#Group 41 Single target aspects (excluding cheetah, can't stack movement)
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (13165, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (14318, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (14319, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (14320, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (14321, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (14322, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (25296, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27044, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (13163, 41, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (34074, 41, 12, 1);

#Group 42 Aspect of the Pack
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (13159, 42, 12, 1);

#Group 43 Aspect of the Wild and Nature resist totem
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (20043, 43, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (20190, 43, 12, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`AuraGroup2`,`UniqueType`) values (27045, 43, 12, 1);
#Resist totem needs no group 2
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10595, 44, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10600, 44, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (10601, 44, 1);
insert  into `spell_unique_auras`(`SpellId`,`AuraGroup`,`UniqueType`) values (25574, 44, 1);