PROJECT(ZELOS_SHARDD)

include_directories (
    ${ZELOS_LIBPACKET_SOURCE_DIR}/include
    ${ZELOS_LIBAUTH_SOURCE_DIR}/include
    ${ZELOS_LIBNET_SOURCE_DIR}/include
    ${ZELOS_SHARDD_SOURCE_DIR}
)

add_executable (shardd
    certd/packet_certify.cpp
    certd/state_certify.cpp
    certd/state_handshake.cpp
    globald/packet_global.cpp
    globald/state_global.cpp
    globald/state_handshake.cpp
    server/packet_agent.cpp
    server/packet_game.cpp
    server/server.cpp
    server/state_agent.cpp
    server/state_game.cpp
    server/state_handshake.cpp
    service.cpp
    main.cpp
)

target_link_libraries (shardd
    zelos_net
    boost_system
    boost_thread
)

