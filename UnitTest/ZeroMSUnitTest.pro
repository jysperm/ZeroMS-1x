#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T20:10:44
#
#-------------------------------------------------

QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x

TARGET = ZeroMSUnitTestTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../

LIBS+= -lcrypto -lssl


SOURCES += \
    ../Base/Auth/Hash.test.cpp \
    TestTools.cpp \
    ../Base/Auth/SHA.Hash.cpp \
    ../Base/Auth/MD5.Hash.cpp \
    main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../Base/Auth/Hash.h \
    TestTools.h
