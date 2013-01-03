drop if exists `character_cos_name`;
create table `character_cos_name` (
	`refID` integer not null,
	`Name` varchar(20) not null,
	primary key(`refID`),
	unique key(`Name`)
);

