#-------------------------------------------------
#
# Project created by QtCreator 2019-09-03T10:39:45
#
#-------------------------------------------------

QT       += widgets

TARGET = UBTMyDownload
TEMPLATE = lib

DEFINES += UBTMYDOWNLOAD_LIBRARY

SOURCES += ubtmydownload.cpp \
    ubtmydownloadhistorywidget.cpp \
    ubtvideowidget.cpp \
    ubtmydownloadvideowidget.cpp

HEADERS += ubtmydownload.h\
        ubtmydownload_global.h \
    ubtmydownloadhistorywidget.h \
    ubtvideowidget.h \
    ubtmydownloadvideowidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtmydownloadhistorywidget.ui \
    ubtvideowidget.ui \
    ubtmydownloadvideowidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/debug/ -lquazip
else:unix: LIBS += -L$$PWD/../trd/quazip/lib/osx/ -lquazip.1.0.0

INCLUDEPATH += $$PWD/../trd/quazip/include
DEPENDPATH += $$PWD/../trd/quazip/include

win32: LIBS += -L$$PWD/../trd/zlib/lib/ -lzdll

INCLUDEPATH += $$PWD/../trd/zlib/include
DEPENDPATH += $$PWD/../trd/zlib/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTUtil/release/ -lUBTUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTUtil/debug/ -lUBTUtil
else:unix: LIBS += -L$$OUT_PWD/../UBTUtil/ -lUBTUtil

INCLUDEPATH += $$PWD/../UBTUtil
DEPENDPATH += $$PWD/../UBTUtil

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTWidget/release/ -lUBTWidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTWidget/debug/ -lUBTWidget
else:unix: LIBS += -L$$OUT_PWD/../UBTWidget/ -lUBTWidget

INCLUDEPATH += $$PWD/../UBTWidget
DEPENDPATH += $$PWD/../UBTWidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QRoundProgressBar/release/ -lQRoundProgressBar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QRoundProgressBar/debug/ -lQRoundProgressBar
else:unix: LIBS += -L$$OUT_PWD/../QRoundProgressBar/ -lQRoundProgressBar

INCLUDEPATH += $$PWD/../QRoundProgressBar
DEPENDPATH += $$PWD/../QRoundProgressBar

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/release/ -lUBTServerCommunication
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/debug/ -lUBTServerCommunication
else:unix: LIBS += -L$$OUT_PWD/../UBTServerCommunication/ -lUBTServerCommunication

INCLUDEPATH += $$PWD/../UBTServerCommunication
DEPENDPATH += $$PWD/../UBTServerCommunication

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTDownloadListItemWidget/release/ -lUBTDownloadListItemWidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTDownloadListItemWidget/debug/ -lUBTDownloadListItemWidget
else:unix: LIBS += -L$$OUT_PWD/../UBTDownloadListItemWidget/ -lUBTDownloadListItemWidget

INCLUDEPATH += $$PWD/../UBTDownloadListItemWidget
DEPENDPATH += $$PWD/../UBTDownloadListItemWidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTPlayer/release/ -lUBTPlayer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTPlayer/debug/ -lUBTPlayer
else:unix: LIBS += -L$$OUT_PWD/../UBTPlayer/ -lUBTPlayer

INCLUDEPATH += $$PWD/../UBTPlayer
DEPENDPATH += $$PWD/../UBTPlayer

RESOURCES += \
    ubtmydownload.qrc

TRANSLATIONS += ../languages/UBTMyDownload_zh_CN.ts \
        ../languages/UBTMyDownload_en.ts

