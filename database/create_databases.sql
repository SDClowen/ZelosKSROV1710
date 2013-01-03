drop database if exists `zManager`;
drop database if exists `zShard`;

#
#	INITIALIZE MANAGER DATABASE
#

#create database `zManager`;
#use `zManager`;

#source GlobalServer/create_tables.sql

#
#	INITIALIZE SHARD DATABASE
#

create database zShard;
use zShard;

source ./GameServer/WorldData/refcharacterdata.sql
