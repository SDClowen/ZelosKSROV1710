project (ZELOS_GATEWAYD)

include_directories (
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBAUTH_SOURCE_DIR}/include
	${ZELOS_LIBNET_SOURCE_DIR}/include
	${ZELOS_LIBOPCODE_SOURCE_DIR}/include
	${ZELOS_LIBSERVER_SOURCE_DIR}/include
	${ZELOS_LIBCLIENT_SOURCE_DIR}/include
	${ZELOS_GATEWAYD_SOURCE_DIR}
)

add_executable (gatewayd
	globald/packet.cpp
	globald/state_global.cpp
	globald/state_handshake.cpp
	server/packet.cpp
	server/server.cpp
	server/state_handshake.cpp
	server/state_login.cpp
	server/state_version.cpp
	main.cpp
	service.cpp
)

target_link_libraries (gatewayd
	zelos_net
	boost_system
	boost_thread
)
