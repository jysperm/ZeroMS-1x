#-------------------------------------------------
#
# Project created by QtCreator 2012-12-20T18:49:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x

TARGET = RSATools
TEMPLATE = app

INCLUDEPATH += ../../



LIBS+= -lcrypto -lssl

win32{
LIBS+= -lgdi32
}

SOURCES += main.cpp\
        MainWindow.cpp \
    ../../Base/Auth/RSA.cpp \
    ../../Base/Exception.cpp

HEADERS  += MainWindow.h \
    ../../Base/Auth/RSA.h \
    ../../Base/Exception.h

FORMS    += MainWindow.ui
