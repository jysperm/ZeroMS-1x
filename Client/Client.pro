QT          = core gui network widgets

TARGET      = Client

CONFIG     += thread c++11

TEMPLATE    = app

HEADERS += \
    const.h \
    ../public/p1.h \
    ../public/ODownloader.h \
    ../public/OPacket.h \
    ../public/OSettings.h \
    ChatWidget.h \
    Login.h \
    MainWidget.h \
    OClientCore.h \
    OClientCoreEx.h

SOURCES += \
    main.cpp \
    ../public/ODownloader.cpp \
    ../public/OPacket.cpp \
    ../public/OSettings.cpp \
    ChatWidget.cpp \
    Login.cpp \
    MainWidget.cpp \
    OClientCore.cpp \
    OClientCoreEx.cpp \
    ../public/p1.cpp

FORMS	+= \
    ChatWidget.ui \
    Login.ui \
    MainWidget.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    images/web.png \
    images/banner.png \
    members.html \
    icon.rc \
    images/40_0ms.png \
    images/0ms.ico \
    config.ini

win32{
    RC_FILE += icon.rc
}
