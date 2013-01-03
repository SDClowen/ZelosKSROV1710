project (ZELOS_LIBPACKET)

include_directories (
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
)

add_library (zelos_packet
	src/packet.cpp
	src/packet_util_functions.cpp
)
