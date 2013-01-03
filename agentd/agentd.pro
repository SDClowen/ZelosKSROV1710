# -------------------------------------------------
# Project created by QtCreator 2011-01-08T00:06:37
# -------------------------------------------------
QT -= core \
    gui
TARGET = agentd
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    service.cpp \
    server/state_handshake.cpp \
    server/state_lobby.cpp \
    server/state_login.cpp \
    server/state_world.cpp \
    server/server.cpp \
    gamed/state_handshake.cpp \
    characterd/state_handshake.cpp \
    characterd/state_player.cpp \
    shardd/state_shard.cpp \
    shardd/state_handshake.cpp \
    shardd/packet_shard.cpp \
    server/packet_login.cpp \
    named/state_name.cpp \
    named/state_handshake.cpp \
    server/connection.cpp \
    gamed/packet.cpp \
    gamed/connection.cpp \
    gamed/state_game.cpp
HEADERS += service.h \
    gamed/state_handshake.h \
    shardd/state_handshake.h \
    server/state_handshake.h \
    server/state_login.h \
    server/state_lobby.h \
    server/state_world.h \
    characterd/state_handshake.h \
    server/server.h \
    server/states.h \
    gamed/states.h \
    characterd/states.h \
    characterd/state_player.h \
    shardd/states.h \
    shardd/state_shard.h \
    shardd/packet_shard.h \
    area.h \
    ticket.h \
    server/packet_login.h \
    named/states.h \
    named/state_name.h \
    named/state_handshake.h \
    server/connection.h \
    client.h \
    gamed/packet.h \
    gamed/connection.h \
    gamed/state_game.h
OTHER_FILES += CMakeLists.txt \
    serv.conf
