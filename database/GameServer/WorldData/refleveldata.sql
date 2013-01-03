DROP TABLE IF EXISTS `refleveldata`;
CREATE TABLE  `refleveldata` (
	`Level` int(3) NOT NULL,
	`RequiredXP` BIGINT(20) NOT NULL,
	`RequiredSP` int(11) NOT NULL,
	`Empty1` int(1) not null default 0,
	`Empty2` int(1) not null default 0,
	`RequiredTraderXP` int not null,
	`RequiredHunterXP` int not null,
	`RequiredThiefXP` int not null,
	PRIMARY KEY (`Level`)
);
