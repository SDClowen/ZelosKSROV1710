# -------------------------------------------------
# Project created by QtCreator 2011-01-04T02:19:04
# -------------------------------------------------
QT -= core \
    gui
TARGET = named
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
INCLUDEPATH += ../libpacket/include \
    ../libauth/include \
    ../libnet/include \
    ../libserver/include \
    ../libclient/include \
    ../libopcode/include
LIBS += -L../libclient \
    -lclient \
    -L../libserver \
    -lserver \
    -L../libnet \
    -lnet \
    -L../libauth \
    -lauth \
    -L../libpacket \
    -lpacket \
    -L../blowfish \
    -lblowfish \
    -lboost_system-mt \
    -lboost_thread-mt
SOURCES += main.cpp \
    service.cpp \
    certd/state_handshake.cpp \
    globald/state_handshake.cpp \
    server/state_handshake.cpp \
    certd/state_certify.cpp \
    globald/state_global.cpp \
    server/server.cpp \
    server/state_register.cpp \
    server/packet.cpp
HEADERS += service.h \
    certd/state_handshake.h \
    globald/state_handshake.h \
    server/state_handshake.h \
    certd/state_certify.h \
    globald/state_global.h \
    server/server.h \
    server/state_register.h \
    certd/states.h \
    globald/states.h \
    server/states.h \
    name_service.h \
    server/packet.h
OTHER_FILES += serv.conf \
    serv.conf \
    CMakeLists.txt
