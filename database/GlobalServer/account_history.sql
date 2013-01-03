drop table if exists `account_history`;
create table `account_history` (
	`accountID` integer not null,
	`IP` varchar(15) not null,
	`access_time` datetime not null,
	primary key (`accountID`)
) engine=InnoDB
