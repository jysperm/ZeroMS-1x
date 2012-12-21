#-------------------------------------------------
#
# Project created by QtCreator 2012-12-20T18:49:05
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x

TARGET = RSATools
TEMPLATE = app

INCLUDEPATH += ../../

LIBS+= -lcrypto -lssl -lgdi32

SOURCES += main.cpp\
        MainWindow.cpp \
    ../../Base/Auth/RSA.cpp

HEADERS  += MainWindow.h \
    ../../Base/Auth/RSA.h

FORMS    += MainWindow.ui
