
QT	= core gui network

TARGET	= C

CONFIG	+= thread

TEMPLATE= app

SOURCES	+= \
    main.cpp \
    clientcore.cpp \
    clientcoreex.cpp

HEADERS	+= \
    const.h \
    clientcore.h \
    clientcoreex.h \
    inline.h \
    p1.h \
    opacket.h

FORMS	+=

RESOURCES +=

















