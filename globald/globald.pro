# -------------------------------------------------
# Project created by QtCreator 2010-07-17T08:34:49
# -------------------------------------------------
QT -= core \
    gui
TARGET = globald
PRE_TARGETDEPS += ../blowfish \
    ../libpacket \
    ../libnet \
    ../libserver
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
debug:DEFINES += DEBUG \
    GLIBCXX_DEBUG
debug:QMAKE_CXXFLAGS += -Wall \
    -Wconversion \
    -W \
    -Wshadow
INCLUDEPATH += ../blowfish/include \
    ../libpacket/include \
    ../libauth/include \
    ../libnet/include \
    ../libopcode/include \
    ../libserver/include \
    ../libclient/include \
    /usr/include/mysql
LIBS += -L../libserver \
    -lserver \
    -L../libclient \
    -lclient \
    -L../libnet \
    -lnet \
    -L../libauth \
    -lauth \
    -L \
    ../libpacket \
    -lpacket \
    -L../blowfish \
    -lblowfish \
    -lboost_system-mt \
    -lboost_thread-mt \
    -lboost_date_time-mt \
    -lmysqlclient_r \
    -lmysqlpp
SOURCES += main.cpp \
    packet/packet_manager_shard.cpp \
    query/query_shard.cpp \
    query/query_manager.cpp \
    query/query_account.cpp \
    query/query_news.cpp \
    service.cpp \
    strutil.cpp \
    certd/state_handshake.cpp \
    certd/state_certify.cpp \
    certd/packet/packet_certify.cpp \
    server/state_handshake.cpp \
    server/state_gateway.cpp \
    server/state_name.cpp \
    server/state_shard.cpp \
    packet/packet_manager_shard.cpp \
    server/server.cpp \
    server/packet_gateway.cpp
OTHER_FILES += serv.conf \
    CMakeLists.txt
HEADERS += opcodes_client.h \
    account_property.h \
    query/query_shard.h \
    query/query_manager.h \
    query/query_account.h \
    query/query_news.h \
    shard.h \
    news.h \
    service.h \
    strutil.h \
    area.h \
    certd/state_handshake.h \
    certd/states.h \
    certd/state_certify.h \
    certd/packet/packet_certify.h \
    server/state_handshake.h \
    server/state_gateway.h \
    server/states.h \
    server/state_name.h \
    server/state_shard.h \
    packet/packet_manager_shard.h \
    server/server.h \
    ticket.h \
    shard_config.h \
    server/packet_gateway.h \
    login_ticket.h
