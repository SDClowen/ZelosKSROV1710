# -------------------------------------------------
# Project created by QtCreator 2011-01-20T18:40:23
# -------------------------------------------------
QT -= core \
    gui
TARGET = libkhamaileon
TEMPLATE = lib
CONFIG += staticlib
SOURCES += src/strutil.cpp \
    src/parser_manager.cpp \
    src/parser/teleportoptional.cpp \
    src/parser/teleportlink.cpp \
    src/parser/teleportdata.cpp \
    src/parser/teleportbuilding.cpp \
    src/parser/skilldata.cpp \
    src/parser/siegefortress.cpp \
    src/parser/refshoptab.cpp \
    src/parser/refshopgroup.cpp \
    src/parser/refshopgoods.cpp \
    src/parser/refscrapofpackageitem.cpp \
    src/parser/refpricepolicyofitem.cpp \
    src/parser/refpackageitem.cpp \
    src/parser/refmappingshopwithtab.cpp \
    src/parser/refmappingshopgroup.cpp \
    src/parser/npcpos.cpp \
    src/parser/levelgold.cpp \
    src/parser/leveldata.cpp \
    src/parser/itemdata.cpp \
    src/parser/characterdata.cpp \
    src/property/item.cpp \
    src/property/character.cpp \
    src/parser/refcharacter_quality.cpp \
    src/parser/refcharacter_facing.cpp \
    src/parser/refcharacter_drop.cpp \
    src/parser/refcharacter_drop_quality.cpp \
    src/parser/refcharacter_task.cpp
HEADERS += include/khamaileon/strutil.h \
    include/khamaileon/parser_manager.h \
    include/khamaileon/parser/teleportoptional.h \
    include/khamaileon/parser/teleportlink.h \
    include/khamaileon/parser/teleportdata.h \
    include/khamaileon/parser/teleportbuilding.h \
    include/khamaileon/parser/skilldata.h \
    include/khamaileon/parser/siegefortress.h \
    include/khamaileon/parser/refshoptab.h \
    include/khamaileon/parser/refshopgroup.h \
    include/khamaileon/parser/refshopgoods.h \
    include/khamaileon/parser/refscrapofpackageitem.h \
    include/khamaileon/parser/refpricepolicyofitem.h \
    include/khamaileon/parser/refpackageitem.h \
    include/khamaileon/parser/refmappingshopwithtab.h \
    include/khamaileon/parser/refmappingshopgroup.h \
    include/khamaileon/parser/npcpos.h \
    include/khamaileon/parser/levelgold.h \
    include/khamaileon/parser/leveldata.h \
    include/khamaileon/parser/itemdata.h \
    include/khamaileon/parser/characterdata.h \
    include/khamaileon/property/teleport.h \
    include/khamaileon/property/spawn_point.h \
    include/khamaileon/property/skill.h \
    include/khamaileon/property/shoptab.h \
    include/khamaileon/property/shopgroup.h \
    include/khamaileon/property/shopgood.h \
    include/khamaileon/property/scrapofpackageitem.h \
    include/khamaileon/property/packageitem.h \
    include/khamaileon/property/magicoption.h \
    include/khamaileon/property/level.h \
    include/khamaileon/property/item_type.h \
    include/khamaileon/property/itemprice.h \
    include/khamaileon/property/item.h \
    include/khamaileon/property/fortress.h \
    include/khamaileon/property/character_type.h \
    include/khamaileon/property/character.h \
    include/khamaileon/property/building.h \
    include/khamaileon/parser/refcharacter_drop.h \
    include/khamaileon/parser/refcharacter_drop_quality.h \
    include/khamaileon/parser/refcharacter_facing.h \
    include/khamaileon/parser/refcharacter_quality.h \
    include/khamaileon/property/drop.h \
    include/khamaileon/property/character_quality.h \
    include/khamaileon/property/character_task.h \
    include/khamaileon/parser/refcharacter_task.h
OTHER_FILES += CMakeLists.txt
