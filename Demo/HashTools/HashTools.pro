#-------------------------------------------------
#
# Project created by QtCreator 2012-12-03T19:50:49
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HashTools
TEMPLATE = app

INCLUDEPATH += ../../

LIBS+= -lcrypto -lssl

SOURCES += main.cpp\
        MainWindow.cpp \
    ../../Base/Auth/SHA.Hash.cpp \
    ../../Base/Auth/MD5.Hash.cpp

HEADERS  += MainWindow.h \
    ../../Base/Auth/Hash.h \

FORMS    += MainWindow.ui
