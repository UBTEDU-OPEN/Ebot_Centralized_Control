#-------------------------------------------------
#
# Project created by QtCreator 2019-09-24T10:35:06
#
#-------------------------------------------------

QT       += widgets

TARGET = UBTWidget
TEMPLATE = lib

DEFINES += UBTWIDGET_LIBRARY

SOURCES += ubtdlg.cpp \
    ubtinfodlg.cpp \
    ubtconfirmdlg.cpp \
    ubtlineedit.cpp

HEADERS += ubtdlg.h\
    ubtinfodlg.h \
    ubtconfirmdlg.h \
    ubtlineedit.h \
    ubtwidget_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtinfodlg.ui \
    ubtconfirmdlg.ui

RESOURCES += \
    ubtwidget.qrc
