DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE  `DB_Shard`.`guild_member` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `GuildID` int(11) NOT NULL DEFAULT '0',
  `Name` varchar(12) NOT NULL DEFAULT '',
  `GrantName` varchar(20) NOT NULL DEFAULT '',
  `GP` int(10) unsigned NOT NULL DEFAULT '0',
  `CombatPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(3) unsigned NOT NULL DEFAULT '0',
  `refCharID` int(10) unsigned NOT NULL DEFAULT '0',
  `Authority` int(11) NOT NULL DEFAULT '0',
  `Position` int(3) NOT NULL DEFAULT '0',
  `Zone` int(11) NOT NULL DEFAULT '0',
  `StateStampt` datetime NOT NULL,
  `LevelStampt` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8

