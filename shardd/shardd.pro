# -------------------------------------------------
# Project created by QtCreator 2011-01-08T00:06:16
# -------------------------------------------------
QT -= core \
    gui
TARGET = shardd
CONFIG += console \
    ordered
CONFIG -= app_bundle
TEMPLATE = app
SUBDIRS = server
INCLUDEPATH = ../libpacket/include \
    ../libnet/include \
    ../libserver/include \
    ../libclient/include \
    ../libauth/include \
    ../libopcode/include
SOURCES += main.cpp \
    service.cpp \
    globald/state_handshake.cpp \
    server/state_handshake.cpp \
    server/server.cpp \
    certd/state_handshake.cpp \
    certd/state_certify.cpp \
    globald/state_global.cpp \
    server/state_agent.cpp \
    certd/packet_certify.cpp \
    server/packet_agent.cpp \
    globald/packet_global.cpp \
    server/packet_game.cpp \
    server/state_game.cpp
HEADERS += service.h \
    globald/state_handshake.h \
    server/states.h \
    server/state_handshake.h \
    server/server.h \
    certd/state_handshake.h \
    certd/states.h \
    globald/states.h \
    certd/state_certify.h \
    globald/state_global.h \
    server/state_agent.h \
    certd/packet_certify.h \
    area.h \
    ticket.h \
    server/packet_agent.h \
    agent.h \
    globald/packet_global.h \
    server/packet_game.h \
    gshard.h \
    server/state_game.h
OTHER_FILES += CMakeLists.txt \
    serv.conf
