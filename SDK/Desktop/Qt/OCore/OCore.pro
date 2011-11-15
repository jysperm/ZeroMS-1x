#-------------------------------------------------
#
# Project created by QtCreator 2011-10-14T10:00:12
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = OCore
TEMPLATE = lib

DEFINES += OCORE_LIBRARY

SOURCES += ocore.cpp \
    oconfig.cpp \
    oclient.cpp \
    ocommand.cpp

HEADERS += ocore.h\
        OCore_global.h \
    oglobal.h \
    oconfig.h \
    OCore \
    OConfig \
    oclient.h \
    OClient \
    ocommand.h \
    OCommand

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE096868C
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = OCore.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
