# -------------------------------------------------
# Project created by QtCreator 2011-01-18T15:34:46
# -------------------------------------------------
QT -= core \
    gui
TARGET = libdb
TEMPLATE = lib
CONFIG += staticlib
SOURCES += src/scoped_connection.cpp \
    src/pool.cpp
HEADERS += include/db/scoped_connection.h \
    include/db/pool.h
OTHER_FILES += CMakeLists.txt
