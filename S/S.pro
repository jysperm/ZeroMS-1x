
QT       = core network gui

TARGET = S
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    servercore.cpp \
    socketlist.cpp

HEADERS += \
    const.h \
    socketlist.h \
    servercore.h \
    clientconn.h


