# -------------------------------------------------
# Project created by QtCreator 2010-07-17T08:35:18
# -------------------------------------------------
QT -= core \
    gui
TARGET = gatewayd
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
PRE_TARGETDEPS += ../blowfish \
    ../libpacket \
    ../libauth \
    ../libserver \
    ../libclient
debug:DEFINES += DEBUG \
    GLIBCXX_DEBUG
debug:QMAKE_CXXFLAGS += -Wall \
    -Wconversion \
    -W \
    -Wshadow
INCLUDEPATH += ../blowfish/include \
    ../libpacket/include \
    ../libopcode/include \
    ../libauth/include \
    ../libnet/include \
    ../libserver/include \
    ../libclient/include
LIBS += -L../libserver \
    -lserver \
    -L../libclient \
    -lclient \
    -L../libauth \
    -lauth \
    -L../libnet \
    -lnet \
    -L../libpacket \
    -lpacket \
    -L../blowfish \
    -lblowfish \
    -lboost_system-mt \
    -lboost_thread-mt
SOURCES += main.cpp \
    server_state_version.cpp \
    server_state_login.cpp \
    server_state_handshake.cpp \
    server.cpp \
    service.cpp \
    globald/state_handshake.cpp \
    globald/state_global.cpp \
    server/state_version.cpp \
    server/state_login.cpp \
    server/state_handshake.cpp \
    server/server.cpp \
    globald/packet.cpp \
    server/packet.cpp
OTHER_FILES += packet/CMakeLists.txt \
    CMakeLists.txt \
    serv.conf
HEADERS += opcodes_client.h \
    shard.h \
    news.h \
    service.h \
    globald/states.h \
    globald/state_handshake.h \
    globald/state_global.h \
    server/state_version.h \
    server/state_login.h \
    server/state_handshake.h \
    server/server.h \
    server/states.h \
    globald/packet.h \
    server/packet.h
