# -------------------------------------------------
# Project created by QtCreator 2010-07-17T08:33:28
# -------------------------------------------------
QT -= core
TARGET = net
TEMPLATE = lib
CONFIG += staticlib
debug:QMAKE_CXXFLAGS += -Wall \
    -Wconversion \
    -W \
    -Wshadow
debug:DEFINES += DEBUG
PRE_TARGETDEPS += ../libpacket \
    ../libauth
INCLUDEPATH += include \
    ../blowfish/include \
    ../libpacket/include \
    ../libauth/include
SOURCES += src/server.cpp \
    src/connection.cpp \
    src/service.cpp \
    src/utility.cpp \
    src/server/state_handshake.cpp \
    src/server/connection.cpp \
    src/client/connection.cpp \
    src/client/state_handshake.cpp \
    src/service_client.cpp
HEADERS += include/net/server.h \
    include/net/monitor_logger.h \
    include/net/monitor.h \
    include/net/connection_state.h \
    include/net/connection.h \
    include/net/service.h \
    include/net/utility.h \
    include/net/server/connection.h \
    include/net/server/state_handshake.h \
    include/net/client/state_handshake.h \
    include/net/client/connection.h \
    include/net/client/host.h \
    include/net/opcode/shard_agent_srv.h \
    include/net/opcode/shard_agent_clt.h \
    include/net/opcode/shard_area_srv.h \
    include/net/opcode/shard_area_clt.h \
    include/net/opcode/global_gateway_srv.h \
    include/net/opcode/global_gateway_clt.h \
    include/net/opcode/global_shard_clt.h \
    include/net/opcode/global_shard_srv.h \
    include/net/opcode/common_clt.h \
    include/net/opcode/common_srv.h \
    include/net/opcode/agent_clt.h \
    include/net/opcode/agent_srv.h \
    include/net/opcode/gateway_clt.h \
    include/net/opcode/gateway_srv.h \
    include/net/opcode/cert_clt.h \
    include/net/opcode/cert_srv.h \
    include/net/opcode/shard_game_srv.h \
    include/net/opcode/shard_game_clt.h \
    include/net/opcode/area_game_srv.h \
    include/net/opcode/area_game_clt.h \
    include/net/opcode/name_srv.h \
    include/net/opcode/name_clt.h \
    include/net/constants/teleport.h \
    include/net/constants/stall.h \
    include/net/constants/skill.h \
    include/net/constants/setting.h \
    include/net/constants/party.h \
    include/net/constants/msg.h \
    include/net/constants/mastery.h \
    include/net/constants/logout.h \
    include/net/constants/lobby.h \
    include/net/constants/job.h \
    include/net/constants/item.h \
    include/net/constants/guild.h \
    include/net/constants/friend.h \
    include/net/constants/error.h \
    include/net/constants/cos.h \
    include/net/constants/command.h \
    include/net/constants/cast.h \
    include/net/constants/block.h \
    include/net/opcode/chard_clt.h \
    include/net/opcode/chard_srv.h \
    include/net/opcode/area_agent_clt.h \
    include/net/opcode/area_agent_srv.h \
    include/net/service_client.h
OTHER_FILES += CMakeLists.txt
