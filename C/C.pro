
QT	= core gui network

TARGET	= C

CONFIG	+= thread

TEMPLATE= app

SOURCES	+= \
    main.cpp \
    oclientcore.cpp \
    oclientcoreex.cpp \
    login.cpp \
    mainwidget.cpp

HEADERS	+= \
    const.h \
    inline.h \
    p1.h \
    opacket.h \
    oclientcore.h \
    oclientcoreex.h \
    login.h \
    mainwidget.h

FORMS	+= \
    login.ui \
    mainwidget.ui

RESOURCES += \
    res.qrc






































