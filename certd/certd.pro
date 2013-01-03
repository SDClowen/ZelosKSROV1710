# -------------------------------------------------
# Project created by QtCreator 2011-01-04T02:18:45
# -------------------------------------------------
QT -= core \
    gui
TARGET = certd
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
INCLUDEPATH += ../libnet/include \
    ../libpacket/include \
    ../libauth/include \
    ../libserver/include \
    ../libopcode/include
LIBS += -L../libserver \
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
    server.cpp \
    state_handshake.cpp \
    state_certify.cpp \
    packet_certify.cpp
HEADERS += service.h \
    server.h \
    state_handshake.h \
    server_states.h \
    state_certify.h \
    packet_certify.h
OTHER_FILES += serv.conf \
    CMakeLists.txt
