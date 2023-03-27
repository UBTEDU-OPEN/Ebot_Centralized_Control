#-------------------------------------------------
#
# Project created by QtCreator 2019-09-18T22:17:39
#
#-------------------------------------------------

QT       += widgets network multimedia multimediawidgets

TARGET = UBTPlayer
TEMPLATE = lib

DEFINES += UBTPLAYER_LIBRARY

SOURCES += \
    videowidget.cpp \
    ubtframecapture.cpp \
    ubtcapturevideosurface.cpp \
    ubtplayer.cpp

HEADERS +=\
        ubtplayer_global.h \
    videowidget.h \
    ubtframecapture.h \
    ubtcapturevideosurface.h \
    ubtplayercontrol.h \
    ubtplayer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtplayer.ui

RESOURCES += \
    ubtplayer.qrc

DISTFILES +=
