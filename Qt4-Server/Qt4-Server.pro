QT       = core network

TARGET = Qt4-Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    const.h \
    ../public/p1.h \
    oclient.h \
    oservercore.h \
    ../public/opacket.h \
    ../public/const.h \
    ../public/osettings.h \

SOURCES += main.cpp \
    oservercore.cpp \
    ../public/osettings.cpp \
    ../public/opacket.cpp \
    oclient.cpp

TRANSLATIONS = ZeroMS_S_zh_CN.ts

OTHER_FILES += \
    config.ini

RESOURCES += \
    res.qrc

