#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T20:10:44
#
#-------------------------------------------------

QT       += testlib widgets

CONFIG += c++11

TARGET = ZeroMSUnitTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../

LIBS+= -lcrypto -lssl

win32{
    LIBS+= -lgdi32
}

SOURCES += \
    ../Base/Auth/Hash.test.cpp \
    TestTools.cpp \
    ../Base/Auth/SHA.Hash.cpp \
    ../Base/Auth/MD5.Hash.cpp \
    main.cpp \
    ../Base/Auth/RSA.cpp \
    ../Base/Auth/RSA.test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../Base/Auth/Hash.h \
    TestTools.h \
    ../Base/Auth/RSA.h
