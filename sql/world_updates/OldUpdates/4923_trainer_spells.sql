ALTER TABLE `trainer_spells` DROP PRIMARY KEY, ADD PRIMARY KEY (`entry`, `cast_spell`, `learn_spell`);