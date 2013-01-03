project(ZELOS_NAMED)

include_directories (
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBAUTH_SOURCE_DIR}/include
	${ZELOS_LIBNET_SOURCE_DIR}/include
	${ZELOS_NAMED_SOURCE_DIR}
)

add_executable (named
	certd/state_certify.cpp
	certd/state_handshake.cpp
	globald/state_global.cpp
	globald/state_handshake.cpp
	server/packet.cpp
	server/server.cpp
	server/state_handshake.cpp
	server/state_register.cpp
	main.cpp
	service.cpp
)

target_link_libraries (named
	zelos_net
	boost_system
	boost_thread
)
