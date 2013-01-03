project (ZELOS_LIBSSO)

include_directories(
	${ZELOS_LIBKHAMAILEON_SOURCE_DIR}/include
	${ZELOS_LIBPOS_SOURCE_DIR}/include
	${ZELOS_LIBSSO_SOURCE_DIR}/include
	${ZELOS_LIBSSO_SOURCE_DIR}/include/sso
)

add_library (zelos_sso
	src/entity/character.cpp
	src/entity/character_factory.cpp
	src/entity/cos.cpp
	src/entity/cos_ability.cpp
	src/entity/cos_attack.cpp
	src/entity/cos_transport.cpp
	src/entity/entity.cpp
	src/entity/mob.cpp
	src/entity/npc.cpp
	src/entity/player.cpp
	src/item/item.cpp
	src/item/item_factory.cpp
	src/property/player.cpp
	src/skill/builder.cpp
	src/skill/skill.cpp
	src/exchange.cpp
	src/mastery.cpp
	src/npc_util.cpp
	src/stall.cpp
	src/storage.cpp
)

target_link_libraries (zelos_sso
	zelos_khamaileon
	zelos_pos
)
