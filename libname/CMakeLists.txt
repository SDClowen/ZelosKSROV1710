project (ZELOS_LIBNAME)

include_directories (
	${ZELOS_LIBPACKET_SOURCE_DIR}/include
	${ZELOS_LIBNET_SOURCE_DIR}/include
	${ZELOS_LIBNAME_SOURCE_DIR}/include
)

add_library (zelos_name
	src/packet.cpp
)

target_link_libraries (zelos_name
	zelos_packet
)
