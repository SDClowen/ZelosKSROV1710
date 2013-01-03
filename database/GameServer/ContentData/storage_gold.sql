drop if exists `storage_gold`;
create table `storage_gold` (
	`AccountID` integer not null,
	`Amount` integer not null,
	primary key(`AccountID`)
);

