# -------------------------------------------------
# Project created by QtCreator 2011-02-02T11:21:27
# -------------------------------------------------
QT -= core \
    gui
TARGET = libphysic
TEMPLATE = lib
CONFIG += staticlib
SOURCES += src/rtree.cpp \
    src/rectangle.cpp \
    src/navigation_mesh.cpp \
    src/navigation_entity.cpp \
    src/navigation_cell.cpp \
    src/model_mesh_collision.cpp \
    src/model_mesh.cpp \
    src/model_manager.cpp \
    src/model.cpp \
    src/line.cpp
OTHER_FILES += CMakeLists.txt
HEADERS += include/physic/vector.h \
    include/physic/rtree.h \
    include/physic/rectangle.h \
    include/physic/navigation_mesh.h \
    include/physic/navigation_entity.h \
    include/physic/navigation_cell.h \
    include/physic/model_mesh_collision.h \
    include/physic/model_mesh.h \
    include/physic/model_manager.h \
    include/physic/model.h \
    include/physic/line.h
