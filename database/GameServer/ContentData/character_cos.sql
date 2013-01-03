drop table if exists `character_cos`;
create table `character_cos` (
	`refID` integer not null auto_increment,
	`HP` integer not null,
	`XP` integer not null,
	`Duration` integer not null,
	`Settings` integer not null,
	primary key(`refID`)
);

