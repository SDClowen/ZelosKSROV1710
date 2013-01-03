drop table if exists `character_penalty`;
create table `character_penalty` (
	`ID` integer not null auto_increment,
	`charID` integer not null,
 	`Type` integer(1) not null,
	`EndDate` datetime not null,
	primary key(`ID`)
);

