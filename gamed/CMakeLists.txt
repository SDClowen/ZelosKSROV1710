project (ZELOS_GAMED)

include_directories (
    ${ZELOS_LIBPACKET_SOURCE_DIR}/include
    ${ZELOS_LIBAUTH_SOURCE_DIR}/include
    ${ZELOS_LIBNET_SOURCE_DIR}/include
    ${ZELOS_LIBNAME_SOURCE_DIR}/include
    ${ZELOS_LIBKHAMAILEON_SOURCE_DIR}/include
    ${ZELOS_LIBPOS_SOURCE_DIR}/include
    ${ZELOS_LIBSSO_SOURCE_DIR}/include
    ${ZELOS_LIBPHYSIC_SOURCE_DIR}/include
    ${ZELOS_GAMED_SOURCE_DIR}
)

add_executable (gamed
    characterd/packet.cpp
    characterd/state_chard.cpp
    characterd/state_handshake.cpp
    named/state_handshake.cpp
    named/state_name.cpp
    server/packet_game.cpp
    server/server.cpp
    server/state_agent.cpp
    server/state_game.cpp
    server/state_handshake.cpp
    shardd/packet_shard.cpp
    shardd/state_handshake.cpp
    shardd/state_shard.cpp
    main.cpp
    service.cpp

)

target_link_libraries (gamed
    zelos_net
    zelos_name
    zelos_khamaileon
    zelos_sso
    zelos_physic
    boost_system
    boost_thread
)
