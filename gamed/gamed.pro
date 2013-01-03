# -------------------------------------------------
# Project created by QtCreator 2011-01-08T00:06:51
# -------------------------------------------------
QT -= core \
    gui
TARGET = gamed
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    service.cpp \
    shardd/state_handshake.cpp \
    server/state_handshake.cpp \
    server/server.cpp \
    shardd/state_shard.cpp \
    characterd/state_handshake.cpp \
    server/state_agent.cpp \
    server/state_game.cpp \
    characterd/state_chard.cpp \
    shardd/packet_shard.cpp \
    server/packet_game.cpp \
    named/state_name.cpp \
    named/state_handshake.cpp \
    characterd/packet.cpp
OTHER_FILES += CMakeLists.txt \
    serv.conf
HEADERS += service.h \
    shardd/state_handshake.h \
    server/state_handshake.h \
    server/server.h \
    shardd/state_shard.h \
    characterd/state_handshake.h \
    server/state_agent.h \
    server/state_game.h \
    characterd/states.h \
    server/states.h \
    shardd/states.h \
    characterd/state_chard.h \
    shardd/packet_shard.h \
    area.h \
    server/packet_game.h \
    named/states.h \
    named/state_name.h \
    named/state_handshake.h \
    characterd/packet.h \
    client.h \
    player.h \
    peer.h
