#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T21:06:59
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BSONTools
TEMPLATE = app

INCLUDEPATH += ../../

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
