alter table `creature_spawns` add column `phasemask` int(30) DEFAULT '1' NOT NULL after `standstate`;

create table `database_config`(`var` varchar(255) NOT NULL , `val` int(30) NOT NULL , PRIMARY KEY (`var`));
insert into `database_config`(`var`,`val`) values ('rev','5150');