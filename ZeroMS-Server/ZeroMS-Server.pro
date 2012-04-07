#-------------------------------------------------
#
# Project created by QtCreator 2012-01-21T23:13:41
#
#-------------------------------------------------

QT       = core network sql

TARGET = ZeroMS-Server
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -Wno-unused-parameter

TEMPLATE = app

SOURCES += main.cpp \
    ../public/OMessage.cpp \
    OServerCore.cpp \
    ../public/OSettings.cpp \
    OClient.cpp \
    ../public/OGlobal/OGlobal.cpp \
    ../public/OAbstractPeer.cpp \
    OClientPeer.cpp \
    OServerDataBase.cpp \
    OServerDataBaseHeader.cpp

HEADERS += \
    ../public/OGlobal.h \
    ../public/OGlobal/protocol2.0.h \
    ../public/OMessage.h \
    OServerCore.h \
    ../public/OSettings.h \
    global.h \
    OClient.h \
    ../public/OAbstractPeer.h \
    OClientPeer.h \
    OServerDataBase.h \
    OServerDataBaseHeader.h

RESOURCES += \
    res.qrc

OTHER_FILES += \
    config.ini \
    DataBase.sql



























