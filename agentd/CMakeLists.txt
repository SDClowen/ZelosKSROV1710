project (ZELOS_AGENTD)

include_directories (
    ${ZELOS_LIBPACKET_SOURCE_DIR}/include
    ${ZELOS_LIBAUTH_SOURCE_DIR}/include
    ${ZELOS_LIBNET_SOURCE_DIR}/include
    ${ZELOS_LIBNAME_SOURCE_DIR}/include
    ${ZELOS_AGENTD_SOURCE_DIR}
)

add_executable (agentd
    gamed/connection.cpp
    gamed/packet.cpp
    gamed/state_game.cpp
    gamed/state_handshake.cpp
    characterd/state_handshake.cpp
    characterd/state_player.cpp
    named/state_handshake.cpp
    named/state_name.cpp
    server/connection.cpp
    server/server.cpp
    server/packet_login.cpp
    server/state_handshake.cpp
    server/state_lobby.cpp
    server/state_login.cpp
    server/state_world.cpp
    shardd/packet_shard.cpp
    shardd/state_handshake.cpp
    shardd/state_shard.cpp
    service.cpp
    main.cpp
)

target_link_libraries (agentd
    zelos_net
    zelos_name
    boost_system
    boost_thread
)
