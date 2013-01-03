drop table if exists `refcharacter_basic_items`;
create table `refcharacter_basic_items` (
	`refID` integer auto_increment,
	`Active` int(1) not null default 1,
	`refItemID` integer not null,
	`Quantity` integer not null default 1,
	primary key(`refID`)
);

