delete from refcharacterdata;
delete from refitemdata;
delete from refmappingshopgroup;
delete from refmappingshopwithtab;
delete from refpackageitem;
delete from refpricepolicyofitem;
delete from refscrapofpackageitem;
delete from refshopgoods;
delete from refshopgroup;
delete from refshoptab;
delete from refskilldata;

load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_5000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_10000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_15000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_20000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_25000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_30000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/characterdata_35000.txt' into table refcharacterdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_5000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_10000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_15000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_20000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_25000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_30000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/itemdata_35000.txt' into table refitemdata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refmappingshopgroup.txt' into table refmappingshopgroup character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refmappingshopwithtab.txt' into table refmappingshopwithtab character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refpackageitem.txt' into table refpackageitem character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refpricepolicyofitem.txt' into table refpricepolicyofitem character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refscrapofpackageitem.txt' into table refscrapofpackageitem character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refshopgoods.txt' into table refshopgoods character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refshopgroup.txt' into table refshopgroup character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/refshoptab.txt' into table refshoptab character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_5000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_10000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_15000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_20000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_25000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_30000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';
load data infile '/home/heero/PK2Tools/Media/server_dep/Silkroad/conv/skilldata_35000.txt' into table refskilldata character set utf8 lines terminated by '\r\n';

