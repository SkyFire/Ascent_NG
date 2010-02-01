alter table `characters` add column `pendingTalentReset` int(4) DEFAULT '0' NOT NULL after `difficulty`;
update `characters` set `auras`= "", `pendingTalentReset` = 1;