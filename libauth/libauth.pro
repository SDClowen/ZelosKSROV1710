# -------------------------------------------------
# Project created by QtCreator 2010-12-28T22:13:28
# -------------------------------------------------
QT -= core \
    gui
TARGET = auth
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += include \
    ../blowfish/include \
    ../libpacket/include
SOURCES += src/security.cpp \
    src/packet_srv_authentication.cpp \
    src/authentication_sro.cpp \
    src/packet_clt_authentication.cpp \
    src/packet_whois.cpp
HEADERS += include/auth/security_modes.h \
    include/auth/security.h \
    include/auth/packet_srv_authentication.h \
    include/auth/opcodes.h \
    include/auth/data.h \
    include/auth/authentication_sro.h \
    include/auth/authentication.h \
    include/auth/packet_clt_authentication.h \
    include/auth/packet_whois.h
OTHER_FILES += CMakeLists.txt
