project (ZELOS_SERVICE_CHARACTERD)

include_directories (
	/usr/include/mysql
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBAUTH_SOURCE_DIR}/include
	${ZELOS_LIBNET_SOURCE_DIR}/include
	${ZELOS_LIBNAME_SOURCE_DIR}/include
	${ZELOS_LIBDB_SOURCE_DIR}/include
	${ZELOS_LIBKHAMAILEON_SOURCE_DIR}/include
	${ZELOS_LIBSSO_SOURCE_DIR}/include
	${ZELOS_SERVICE_CHARACTERD_SOURCE_DIR}
)

add_executable (chard
	named/state_handshake.cpp
	named/state_name.cpp
	server/packet_character.cpp
	server/packet_dc.cpp
	server/packet_lobby.cpp
	server/server.cpp
	server/state_command.cpp
	server/state_handshake.cpp
	main.cpp
	manager.cpp
	query_block.cpp
	query_buff.cpp
	query_cos.cpp
	query_hotkey.cpp
	query_item.cpp
	query_item_basic.cpp
	query_job.cpp
	query_mastery.cpp
	query_player.cpp
	query_skill.cpp
	query_storage.cpp
	service.cpp
)

target_link_libraries (chard
	zelos_net
	zelos_name
	zelos_db
	zelos_khamaileon
	zelos_sso
	boost_system
	boost_thread
)
