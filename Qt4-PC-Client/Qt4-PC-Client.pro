QT          = core gui network

TARGET      = Qt4-PC-Client

CONFIG     += thread

TEMPLATE    = app

HEADERS += \
    oclientcoreex.h \
    oclientcore.h \
    mainwidget.h \
    login.h \
    const.h \
    chatwidget.h \
    ../public/p1.h \
    ../public/osettings.h \
    ../public/opacket.h \
    ../public/const.h \
    ../public/odowner.h

SOURCES += \
    oclientcoreex.cpp \
    oclientcore.cpp \
    mainwidget.cpp \
    main.cpp \
    login.cpp \
    chatwidget.cpp \
    ../public/osettings.cpp \
    ../public/opacket.cpp \
    ../public/odowner.cpp

FORMS	+= \
    login.ui \
    mainwidget.ui \
    chatwidget.ui

TRANSLATIONS += ZeroMS_C_EN.ts

RESOURCES += \
    res.qrc

OTHER_FILES += \
    images/web.png \
    images/banner.png \
    members.html \
    icon.rc \
    images/40_0ms.png \
    images/0ms.ico \
    config.ini \
    ZeroMS_C_EN.ts

win32{
    RC_FILE += icon.rc
}



