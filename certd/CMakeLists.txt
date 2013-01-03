project(ZELOS_CERTD)

include_directories (
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBAUTH_SOURCE_DIR}/include
	${ZELOS_LIBNET_SOURCE_DIR}/include
)

add_executable (certd
	main.cpp
	packet_certify.cpp
	server.cpp
	service.cpp
	state_certify.cpp
	state_handshake.cpp
)

target_link_libraries (certd
	zelos_net
	boost_system
	boost_thread
)
