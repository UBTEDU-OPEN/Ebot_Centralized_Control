#-------------------------------------------------
#
# Project created by QtCreator 2019-09-16T10:15:46
#
#-------------------------------------------------

QT       += widgets

TARGET = UBTDownloadListItemWidget
TEMPLATE = lib

DEFINES += UBTDOWNLOADLISTITEMWIDGET_LIBRARY

SOURCES += \
    ubtdownloadlistitemwidget.cpp

HEADERS +=\
    ubtdownloadlistitemwidget.h \
    ubtdownloadlistitemwidget_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtdownloadlistitemwidget.ui

RESOURCES += \
    ubtdownloadwidget.qrc


TRANSLATIONS += ../languages/UBTDownloadListItemWidget_zh_CN.ts \
        ../languages/UBTDownloadListItemWidget_en.ts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTWidget/release/ -lUBTWidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTWidget/debug/ -lUBTWidget
else:unix: LIBS += -L$$OUT_PWD/../UBTWidget/ -lUBTWidget

INCLUDEPATH += $$PWD/../UBTWidget
DEPENDPATH += $$PWD/../UBTWidget
