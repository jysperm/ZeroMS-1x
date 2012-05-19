#-------------------------------------------------
#
# Project created by QtCreator 2012-05-19T17:32:52
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = ZeroMS-Client
TEMPLATE = app


SOURCES += main.cpp\
        LoginWidget.cpp \
    ../public/OSettings.cpp \
    ../public/OMessage.cpp \
    ../public/OAbstractPeer.cpp \
    ../public/OGlobal/OGlobal.cpp \
    ../public/ODataBase.cpp

HEADERS  += LoginWidget.h \
    ../public/OSettings.h \
    ../public/OMessage.h \
    ../public/OGlobal.h \
    ../public/OAbstractPeer.h \
    ../public/OGlobal/protocol2.0.h \
    global.h \
    ../public/ODataBase.h

FORMS    += LoginWidget.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    config.ini
