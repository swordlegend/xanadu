CREATE DATABASE xanadu62;
USE xanadu62;

CREATE TABLE buddy_lists (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `player_id` INTEGER NOT NULL,
  `buddy_id` INTEGER NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE characters (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(13) NOT NULL,
  `user_id` INT NOT NULL,
  `level` INT NOT NULL DEFAULT '1',
  `job` INT NOT NULL,
  `str` INT NOT NULL DEFAULT '12',
  `dex` INT NOT NULL DEFAULT '5',
  `intt` INT NOT NULL DEFAULT '4',
  `luk` INT NOT NULL DEFAULT '4',
  `hp` INT NOT NULL DEFAULT '50',
  `max_hp` INT NOT NULL DEFAULT '50',
  `mp` INT NOT NULL DEFAULT '5',
  `max_mp` INT NOT NULL DEFAULT '5',
  `ap` INT NOT NULL DEFAULT '0',
  `sp` INT NOT NULL DEFAULT '0',
  `exp` BIGINT NOT NULL DEFAULT '0',
  `fame` INT NOT NULL DEFAULT '0',
  `map` INT NOT NULL DEFAULT '0',
  `pos` INT NOT NULL DEFAULT '0',
  `gender` INT NOT NULL,
  `skin` INT NOT NULL,
  `face` INT NOT NULL,
  `hair` INT NOT NULL,
  `mesos` BIGINT NOT NULL DEFAULT '0',
  `party_id` INT NOT NULL DEFAULT '0',
  `guild_id` INT NOT NULL DEFAULT '0',
  `guild_rank` INT NOT NULL DEFAULT '5',
  `equip_slots` INT NOT NULL DEFAULT '24',
  `use_slots` INT NOT NULL DEFAULT '24',
  `setup_slots` INT NOT NULL DEFAULT '24',
  `etc_slots` INT NOT NULL DEFAULT '24',
  `merchant_storage_mesos` BIGINT NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
);

CREATE TABLE equips (
 `primary_id` INT NOT NULL AUTO_INCREMENT,
  equip_id INT NOT NULL,
  player_id INT NOT NULL,
  pos INT NOT NULL DEFAULT '0',
  slots INT NOT NULL DEFAULT '7',
  used_scrolls INT NOT NULL DEFAULT '0',
  str INT NOT NULL DEFAULT '0',
  dex INT NOT NULL DEFAULT '0',
  iint INT NOT NULL DEFAULT '0',
  luk INT NOT NULL DEFAULT '0',
  hp INT NOT NULL DEFAULT '0',
  mp INT NOT NULL DEFAULT '0',
  weapon_attack INT NOT NULL DEFAULT '0',
  magic_attack INT NOT NULL DEFAULT '0',
  weapon_def INT NOT NULL DEFAULT '0',
  magic_def INT NOT NULL DEFAULT '0',
  accuracy INT NOT NULL DEFAULT '0',
  avoid INT NOT NULL DEFAULT '0',
  hand INT NOT NULL DEFAULT '0',
  speed INT NOT NULL DEFAULT '0',
  jump INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE guilds (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `guild_id` INT NOT NULL,
  `guild_name` VARCHAR(20) NOT NULL,
  `notice` VARCHAR(100) DEFAULT '',
  `rank1` VARCHAR(20) NOT NULL DEFAULT 'Master',
  `rank2` VARCHAR(20) NOT NULL DEFAULT 'Jr. Master',
  `rank3` VARCHAR(20) NOT NULL DEFAULT 'Member',
  `rank4` VARCHAR(20) NOT NULL DEFAULT '',
  `rank5` VARCHAR(20) NOT NULL DEFAULT '',
  `logo_bg` INT NOT NULL DEFAULT '0',
  `logo_bg_color` INT NOT NULL DEFAULT '0',
  `logo` INT NOT NULL DEFAULT '0',
  `logo_color` INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE items (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `item_id` INT NOT NULL,
  `player_id` INT NOT NULL,
  `pos` INT NOT NULL,
  `amount` INT NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE keymap (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `player_id` INT NOT NULL,
  `pos` INT NOT NULL,
  `type` INT NOT NULL,
  `action` INT NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE pets (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `pet_id` INT NOT NULL,
  `player_id` INT NOT NULL,
  `pos` INT NOT NULL,
  `pet_slot` INT NOT NULL,
  `level` INT NOT NULL,
  `closeness` INT NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE quests (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `player_id` INT NOT NULL,
  `quest_id` INT DEFAULT NULL,
  `is_complete` INT DEFAULT NULL,
  `killed_mob` INT NOT NULL DEFAULT 0,
  `amount` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE skills (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `player_id` INT NOT NULL,
  `skill_id` INT NOT NULL,
  `level` INT NOT NULL,
  `master_level` INT NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE users (
  `id` INT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(15) NOT NULL,
  `password` VARCHAR(15) NOT NULL,
  `gm` INT NOT NULL DEFAULT '0',
  `nxcash_credit` INT NOT NULL DEFAULT '0',
  `banned` INT NOT NULL DEFAULT '0',
  `character_slots` INT NOT NULL DEFAULT '6',
  `donation_points` INT NOT NULL DEFAULT '0',
  `storage_mesos` BIGINT NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
);

CREATE TABLE storage_equips (
 `primary_id` INT NOT NULL AUTO_INCREMENT,
  equip_id INT NOT NULL,
  user_id INT NOT NULL,
  pos INT NOT NULL DEFAULT '0',
  slots INT NOT NULL DEFAULT '7',
  used_scrolls INT NOT NULL DEFAULT '0',
  str INT NOT NULL DEFAULT '0',
  dex INT NOT NULL DEFAULT '0',
  iint INT NOT NULL DEFAULT '0',
  luk INT NOT NULL DEFAULT '0',
  hp INT NOT NULL DEFAULT '0',
  mp INT NOT NULL DEFAULT '0',
  weapon_attack INT NOT NULL DEFAULT '0',
  magic_attack INT NOT NULL DEFAULT '0',
  weapon_def INT NOT NULL DEFAULT '0',
  magic_def INT NOT NULL DEFAULT '0',
  accuracy INT NOT NULL DEFAULT '0',
  avoid INT NOT NULL DEFAULT '0',
  hand INT NOT NULL DEFAULT '0',
  speed INT NOT NULL DEFAULT '0',
  jump INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE storage_items (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `item_id` INT NOT NULL,
  `user_id` INT NOT NULL,
  `pos` INT NOT NULL,
  `amount` INT NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE merchant_storage_equips (
 `primary_id` INT NOT NULL AUTO_INCREMENT,
  equip_id INT NOT NULL,
  player_id INT NOT NULL,
  pos INT NOT NULL DEFAULT '0',
  slots INT NOT NULL DEFAULT '7',
  used_scrolls INT NOT NULL DEFAULT '0',
  str INT NOT NULL DEFAULT '0',
  dex INT NOT NULL DEFAULT '0',
  iint INT NOT NULL DEFAULT '0',
  luk INT NOT NULL DEFAULT '0',
  hp INT NOT NULL DEFAULT '0',
  mp INT NOT NULL DEFAULT '0',
  weapon_attack INT NOT NULL DEFAULT '0',
  magic_attack INT NOT NULL DEFAULT '0',
  weapon_def INT NOT NULL DEFAULT '0',
  magic_def INT NOT NULL DEFAULT '0',
  accuracy INT NOT NULL DEFAULT '0',
  avoid INT NOT NULL DEFAULT '0',
  hand INT NOT NULL DEFAULT '0',
  speed INT NOT NULL DEFAULT '0',
  jump INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE merchant_storage_items (
  `primary_id` INT NOT NULL AUTO_INCREMENT,
  `item_id` INT NOT NULL,
  `player_id` INT NOT NULL,
  `pos` INT NOT NULL,
  `amount` INT NOT NULL,
  PRIMARY KEY (`primary_id`)
);

CREATE TABLE votingrecords (
  `account` varchar(13) NOT NULL DEFAULT '0',
  `ip` varchar(30) NOT NULL DEFAULT '0',
  `date` int(11) NOT NULL DEFAULT '0',
  `times` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`account`)
);
