
QT	= core gui network

TARGET	= C

CONFIG	+= thread

TEMPLATE= app

SOURCES	+= \
    main.cpp \
    login.cpp \
    clientcore.cpp \
    mainwidget.cpp \
    chatwidget.cpp \
    clientcoreex.cpp \
    threadlogodowner.cpp

HEADERS	+= \
    login.h \
    const.h \
    clientcore.h \
    mainwidget.h \
    chatwidget.h \
    clientcoreex.h \
    threadlogodowner.h \
    inline.h \
    p1.h \
    opacket.h

FORMS	+= \
    login.ui \
    mainwidget.ui \
    chatwidget.ui

RESOURCES += \
    res.qrc





