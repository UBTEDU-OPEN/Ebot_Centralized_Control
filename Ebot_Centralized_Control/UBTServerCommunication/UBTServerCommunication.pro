#-------------------------------------------------
#
# Project created by QtCreator 2019-08-29T15:35:44
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = UBTServerCommunication
TEMPLATE = lib

DEFINES += UBTSERVERCOMMUNICATION_LIBRARY

SOURCES += ubtservercommunication.cpp

HEADERS += ubtservercommunication.h\
        ubtservercommunication_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTUtil/release/ -lUBTUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTUtil/debug/ -lUBTUtil
else:unix: LIBS += -L$$OUT_PWD/../UBTUtil/ -lUBTUtil

INCLUDEPATH += $$PWD/../UBTUtil
DEPENDPATH += $$PWD/../UBTUtil

win32: LIBS += -L$$PWD/../trd/ubtlib/lib/ -lUbtlib

INCLUDEPATH += $$PWD/../trd/ubtlib/include
DEPENDPATH += $$PWD/../trd/ubtlib/include
