#-------------------------------------------------
#
# Project created by QtCreator 2019-09-24T19:15:50
#
#-------------------------------------------------

QT       += widgets

TARGET = QRoundProgressBar
TEMPLATE = lib

DEFINES += QROUNDPROGRESSBAR_LIBRARY

SOURCES += qroundprogressbar.cpp

HEADERS += qroundprogressbar.h\
        qroundprogressbar_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
