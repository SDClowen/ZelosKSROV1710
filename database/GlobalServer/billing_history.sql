drop table if exists `billing_history`;
create table `billing_history` (
	`transactionID` int not null auto_increment,
	`accountID` int not null,
	`refItemID` int not null,
	`quantity` int not null,
	`cost` int not null,
	`purchaseDate` datetime not null,
	`serverID` int not null,
	`transactionCode` varchar(512) not null,
	primary key (`transactionID`)
) ENGINE = InnoDB;

