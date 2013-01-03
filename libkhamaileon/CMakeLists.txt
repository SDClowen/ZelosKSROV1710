project (ZELOS_LIBKHAMAILEON)

include_directories (
	${ZELOS_LIBKHAMAILEON_SOURCE_DIR}/include
	${ZELOS_LIBKHAMAILEON_SOURCE_DIR}/include/khamaileon
)

add_library (zelos_khamaileon
	src/parser/characterdata.cpp
	src/parser/itemdata.cpp
	src/parser/leveldata.cpp
	src/parser/levelgold.cpp
	src/parser/npcpos.cpp
	src/parser/refcharacter_drop.cpp
	src/parser/refcharacter_drop_quality.cpp
	src/parser/refcharacter_facing.cpp
	src/parser/refcharacter_quality.cpp
	src/parser/refcharacter_task.cpp
	src/parser/refmappingshopgroup.cpp
	src/parser/refmappingshopwithtab.cpp
	src/parser/refpackageitem.cpp
	src/parser/refpricepolicyofitem.cpp
	src/parser/refscrapofpackageitem.cpp
	src/parser/refshopgoods.cpp
	src/parser/refshopgroup.cpp
	src/parser/refshoptab.cpp
	src/parser/siegefortress.cpp
	src/parser/skilldata.cpp
	src/parser/teleportbuilding.cpp
	src/parser/teleportdata.cpp
	src/parser/teleportlink.cpp
	src/parser/teleportoptional.cpp
	src/property/character.cpp
	src/property/item.cpp
	src/parser_manager.cpp
	src/strutil.cpp
)

