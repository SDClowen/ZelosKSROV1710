project (ZELOS_GLOBALD)

include_directories (
    ${MYSQL_INCLUDE_DIR}
    ${ZELOS_LIBPACKET_SOURCE_DIR}/include
    ${ZELOS_LIBAUTH_SOURCE_DIR}/include
    ${ZELOS_LIBNET_SOURCE_DIR}/include
    ${ZELOS_LIBDB_SOURCE_DIR}/include
    ${ZELOS_GLOBALD_SOURCE_DIR}
)

add_executable (globald
    certd/packet/packet_certify.cpp
    certd/state_certify.cpp
    certd/state_handshake.cpp
    packet/packet_manager_shard.cpp
    query/query_account.cpp
    query/query_manager.cpp
    query/query_news.cpp
    query/query_shard.cpp
    server/packet_gateway.cpp
    server/server.cpp
    server/state_gateway.cpp
    server/state_handshake.cpp
    server/state_name.cpp
    server/state_shard.cpp
    main.cpp
    service.cpp
    strutil.cpp
)

target_link_libraries (globald
    zelos_net
    zelos_db
    boost_system
    boost_thread
    boost_date_time
)
