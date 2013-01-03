project (PHOENIX_SERVICE_XXX)

include_directories (
	${PHOENIX_LIBPACKET_SOURCE_DIR}/include
	${PHOENIX_LIBAUTH_SOURCE_DIR}/include
	${PHOENIX_LIBNET_SOURCE_DIR}/include
	${PHOENIX_LIBNAME_SOURCE_DIR}/include
	${PHOENIX_SERVICE_XXX_SOURCE_DIR}
)

add_executable (xxx
	named/state_handshake.cpp
	named/state_name.cpp
	server/server.cpp
	server/state_command.cpp
	server/state_handshake.cpp
	main.cpp
	service.cpp
)

target_link_libraries (xxx
	phoenix_net
	phoenix_name
	boost_system
	boost_thread
)
