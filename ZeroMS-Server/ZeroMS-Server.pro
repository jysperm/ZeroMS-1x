#-------------------------------------------------
#
# Project created by QtCreator 2012-01-21T23:13:41
#
#-------------------------------------------------

QT       = core network sql

TARGET = ZeroMS-Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../public/OMessage.cpp \
    OProtocolForSC.cpp \
    OServerCore.cpp \
    ../public/OSettings.cpp \
    OClientConn.cpp

HEADERS += \
    ../public/OGlobal.h \
    ../public/OGlobal/protocol2.0.h \
    ../public/OMessage.h \
    OProtocolForSC.h \
    OServerCore.h \
    ../public/OSettings.h \
    global.h \
    OClientConn.h

RESOURCES += \
    res.qrc

OTHER_FILES += \
    config.ini





















