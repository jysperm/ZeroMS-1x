#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T20:10:44
#
#-------------------------------------------------

QT       += testlib

TARGET = ZeroMSUnitTestTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ZeroMSUnitTest.cpp \
    ../Base/Auth/Hash.test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../Base/Auth/Hash.h
