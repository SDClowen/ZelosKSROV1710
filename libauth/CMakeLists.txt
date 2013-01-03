project(ZELOS_LIBAUTH)

include_directories(
	${ZELOS_BLOWFISH_SOURCE_DIR}/include
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBAUTH_SOURCE_DIR}/include
)

add_library(zelos_auth
	src/authentication_sro.cpp
	src/packet_clt_authentication.cpp
	src/packet_srv_authentication.cpp
	src/packet_whois.cpp
	src/security.cpp
)

target_link_libraries (zelos_auth
	blowfish
	zelos_packet
)
