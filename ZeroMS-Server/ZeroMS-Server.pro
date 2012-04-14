QT       = core network sql

TARGET = ZeroMS-Server
CONFIG += console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -Wno-unused-parameter

TEMPLATE = app

HEADERS += \
    ../public/OGlobal.h \
    ../public/OGlobal/protocol2.0.h \
    ../public/OMessage.h \
    ../public/OSettings.h \
    ../public/OAbstractPeer.h \
    global.h \
    OServerCore.h \
    OClient.h \
    OClientPeer.h \
    OServerDataBase.h \
    OServerDataBaseHeader.h

SOURCES += main.cpp \
    ../public/OGlobal/OGlobal.cpp \
    ../public/OMessage.cpp \
    ../public/OSettings.cpp \
    ../public/OAbstractPeer.cpp \
    OServerCore.cpp \
    OClient.cpp \
    OClientPeer.cpp \
    OServerDataBase.cpp \
    OServerDataBaseHeader.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    config.ini \
    DataBase.sql
