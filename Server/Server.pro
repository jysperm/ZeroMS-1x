QT       = core network

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    const.h \
    ../public/p1.h \
    ../public/const.h \
    OServerCore.h \
    OClient.h \
    ../public/OPacket.h \
    ../public/OSettings.h

SOURCES += main.cpp \
    OClient.cpp \
    OServerCore.cpp \
    ../public/OPacket.cpp \
    ../public/OSettings.cpp \
    ../public/p1.cpp

OTHER_FILES += \
    config.ini

RESOURCES += \
    res.qrc



