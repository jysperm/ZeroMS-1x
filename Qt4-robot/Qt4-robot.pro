#-------------------------------------------------
#
# Project created by QtCreator 2011-12-31T23:51:16
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Qt4-robot
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    oclientcore.cpp \
    ../public/opacket.cpp \
    orobot.cpp

HEADERS += \
    oclientcore.h \
    const.h \
    ../public/p1.h \
    ../public/opacket.h \
    ../public/const.h \
    orobot.h











