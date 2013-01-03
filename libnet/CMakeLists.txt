project (ZELOS_LIBNET)

include_directories (
	${ZELOS_BLOWFISH_SOURCE_DIR}/include
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBAUTH_SOURCE_DIR}/include
	${ZELOS_LIBOPCODE_SOURCE_DIR}/include
	${ZELOS_LIBNET_SOURCE_DIR}/include
)

add_library (zelos_net STATIC
	src/connection.cpp
	src/server.cpp
	src/service.cpp
	src/service_client.cpp
	src/utility.cpp
	src/client/connection.cpp
	src/client/state_handshake.cpp
	src/server/connection.cpp
	src/server/state_handshake.cpp
)

target_link_libraries (zelos_net
	zelos_packet
	zelos_auth
)
