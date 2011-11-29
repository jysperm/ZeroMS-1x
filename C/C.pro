
QT	= core gui network

TARGET	= C

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
    inline.h \
    p1.h \
    opacket.h \
    oclientcore.h \
    oclientcoreex.h \
    login.h \
    mainwidget.h \
    chatwidget.h

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
    RC_FILE += resource/icon.rc
}












































