# -------------------------------------------------
# Project created by QtCreator 2010-12-10T17:20:40
# -------------------------------------------------
QT -= core \
    gui
TARGET = packet
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH = include
SOURCES += src/packet_util_functions.cpp \
    src/packet.cpp
HEADERS += include/packet_util_functions.h \
    include/packet_group.h \
    include/packet.h \
    include/packet/packet_util_functions.h \
    include/packet/packet_group.h \
    include/packet/packet.h
OTHER_FILES += CMakeLists.txt
