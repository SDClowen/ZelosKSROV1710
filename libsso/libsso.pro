# -------------------------------------------------
# Project created by QtCreator 2011-01-29T09:49:41
# -------------------------------------------------
QT -= core \
    gui
TARGET = libsso
TEMPLATE = lib
CONFIG += staticlib
OTHER_FILES += CMakeLists.txt
INCLUDEPATH += include/sso
HEADERS += include/sso/timestamp.h \
    include/sso/storage.h \
    include/sso/npc_util.h \
    include/sso/npc_list.h \
    include/sso/mastery.h \
    include/sso/market_stock.h \
    include/sso/job.h \
    include/sso/invitation_type.h \
    include/sso/hotkey.h \
    include/sso/basic_items.h \
    include/sso/autopot.h \
    include/sso/utility/interpolate.h \
    include/sso/entity/player.h \
    include/sso/entity/npc_task.h \
    include/sso/entity/npc.h \
    include/sso/entity/mob.h \
    include/sso/entity/entity.h \
    include/sso/entity/drop.h \
    include/sso/entity/cos_transport.h \
    include/sso/entity/cos_state.h \
    include/sso/entity/cos_attack.h \
    include/sso/entity/cos_ability.h \
    include/sso/entity/cos.h \
    include/sso/entity/character_state.h \
    include/sso/entity/character_factory.h \
    include/sso/entity/character.h \
    include/sso/entity/building.h \
    include/sso/item/item_factory.h \
    include/sso/item/item.h \
    include/sso/property/item_cos.h \
    include/sso/property/item.h \
    include/sso/property/character.h \
    include/sso/skill/status.h \
    include/sso/skill/skill.h \
    include/sso/skill/requirement.h \
    include/sso/skill/imbue.h \
    include/sso/skill/builder.h \
    include/sso/skill/buff.h \
    include/sso/skill/attack.h \
    include/sso/types.h \
    include/sso/property/player.h \
    include/sso/location.h \
    include/sso/stall.h \
    include/sso/exchange.h
SOURCES += src/entity/player.cpp \
    src/entity/npc.cpp \
    src/entity/mob.cpp \
    src/entity/entity.cpp \
    src/entity/cos_transport.cpp \
    src/entity/cos_attack.cpp \
    src/entity/cos_ability.cpp \
    src/entity/cos.cpp \
    src/entity/character_factory.cpp \
    src/entity/character.cpp \
    src/item/item_factory.cpp \
    src/item/item.cpp \
    src/skill/skill.cpp \
    src/skill/builder.cpp \
    src/storage.cpp \
    src/npc_util.cpp \
    src/mastery.cpp \
    src/property/player.cpp \
    src/stall.cpp \
    src/exchange.cpp
