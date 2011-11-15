QT       = core network

TARGET = S2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    const.h \
    p1.h \
    inline.h \
    oclient.h \
    opacket.h \
    oservercore.h

SOURCES += main.cpp \
    opacket.cpp \
    oservercore.cpp

TRANSLATIONS = ZeroMS_S_zh_CN.ts
