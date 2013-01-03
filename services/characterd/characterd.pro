# -------------------------------------------------
# Project created by QtCreator 2011-01-18T10:34:48
# -------------------------------------------------
QT -= core \
    gui
TARGET = characterd
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    named/state_name.cpp \
    named/state_handshake.cpp \
    service.cpp \
    server/server.cpp \
    server/state_handshake.cpp \
    server/state_command.cpp \
    query_player.cpp \
    query_item.cpp \
    query_mastery.cpp \
    query_skill.cpp \
    query_item_basic.cpp \
    server/packet_lobby.cpp \
    server/packet_dc.cpp \
    query_block.cpp \
    query_storage.cpp \
    manager.cpp \
    server/packet_character.cpp \
    query_job.cpp \
    query_hotkey.cpp \
    query_buff.cpp \
    query_cos.cpp
HEADERS += named/states.h \
    named/state_name.h \
    named/state_handshake.h \
    service.h \
    server/server.h \
    server/states.h \
    server/state_handshake.h \
    server/state_command.h \
    query_player.h \
    query_item.h \
    query_mastery.h \
    query_skill.h \
    query_item_basic.h \
    server/packet_lobby.h \
    server/packet_dc.h \
    query_block.h \
    query_storage.h \
    manager.h \
    server/packet_character.h \
    query_job.h \
    query_hotkey.h \
    query_buff.h \
	query_cos.h
OTHER_FILES += serv.conf \
    CMakeLists.txt
