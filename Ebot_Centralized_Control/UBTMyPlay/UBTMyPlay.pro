#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T15:26:15
#
#-------------------------------------------------

QT       += widgets network multimedia

TARGET = UBTMyPlay
TEMPLATE = lib

DEFINES += UBTMYPLAY_LIBRARY ESTABLISH_TCP_SERVER

SOURCES += ubtmyplay.cpp \
    ubtmyplaywidget.cpp \
    ubtsendfilethread.cpp \
    ../common/UBXPublic.cpp \
    ubtactionlibwidget.cpp \
    ubtactionwidget.cpp \
    ubtactionlistitemdelegate.cpp

HEADERS += ubtmyplay.h\
        ubtmyplay_global.h \
    ubtmyplaywidget.h \
    ubtsendfilethread.h \
    ../common/UBXPublic.h \
    ubtactionlibwidget.h \
    ubtactionwidget.h \
    ubtactionlistitemdelegate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtmyplaywidget.ui \
    ubtactionlibwidget.ui \
    ubtactionwidget.ui


TRANSLATIONS += ../languages/UBTMyPlay_zh_CN.ts \
        ../languages/UBTMyPlay_en.ts

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QRoundProgressBar/release/ -lQRoundProgressBar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QRoundProgressBar/debug/ -lQRoundProgressBar
else:unix: LIBS += -L$$OUT_PWD/../QRoundProgressBar/ -lQRoundProgressBar

INCLUDEPATH += $$PWD/../QRoundProgressBar
DEPENDPATH += $$PWD/../QRoundProgressBar

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTWidget/release/ -lUBTWidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTWidget/debug/ -lUBTWidget
else:unix: LIBS += -L$$OUT_PWD/../UBTWidget/ -lUBTWidget

INCLUDEPATH += $$PWD/../UBTWidget
DEPENDPATH += $$PWD/../UBTWidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../zxlib/release/ -lzxLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../zxlib/debug/ -lzxLib
else:unix: LIBS += -L$$OUT_PWD/../zxlib/ -lzxLib

INCLUDEPATH += $$PWD/../zxlib
DEPENDPATH += $$PWD/../zxlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBXRobotController/release/ -lUBXRobotController
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBXRobotController/debug/ -lUBXRobotController
else:unix: LIBS += -L$$OUT_PWD/../UBXRobotController/ -lUBXRobotController

INCLUDEPATH += $$PWD/../UBXRobotController
DEPENDPATH += $$PWD/../UBXRobotController

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/release/ -lUBTServerCommunication
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/debug/ -lUBTServerCommunication
else:unix: LIBS += -L$$OUT_PWD/../UBTServerCommunication/ -lUBTServerCommunication

INCLUDEPATH += $$PWD/../UBTServerCommunication
DEPENDPATH += $$PWD/../UBTServerCommunication

RESOURCES += \
    resMyPlay.qrc

DISTFILES +=
