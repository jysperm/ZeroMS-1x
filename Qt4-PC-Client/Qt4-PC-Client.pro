
QT	= core gui network

TARGET	= Qt4-PC-Client

CONFIG	+= thread

TEMPLATE= app

SOURCES	+= \
    main.cpp \
    oclientcore.cpp \
    oclientcoreex.cpp \
    login.cpp \
    mainwidget.cpp \
    chatwidget.cpp

HEADERS	+= \
    const.h \
    ../public/p1.h \
    ../public/opacket.h \
    inline.h \
    oclientcore.h \
    oclientcoreex.h \
    login.h \
    mainwidget.h \
    chatwidget.h \
    ../public/const.h

FORMS	+= \
    login.ui \
    mainwidget.ui \
    chatwidget.ui

TRANSLATIONS = ZeroMS_C_EN.ts

RESOURCES += \
    res.qrc

OTHER_FILES += \
    images/web.png \
    images/banner.png \
    members.html

win32{
    RC_FILE += icon.rc
}














































