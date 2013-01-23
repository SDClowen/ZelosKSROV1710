project (ZELOS_LIBDB)

include_directories (
    ${MYSQL_INCLUDE_DIR}
    ${ZELOS_LIBDB_SOURCE_DIR}/include
)

add_library (zelos_db
    src/pool.cpp
    src/scoped_connection.cpp
)

target_link_libraries (zelos_db
    ${MYSQL_LIBRARIES}
    mysqlpp
)
