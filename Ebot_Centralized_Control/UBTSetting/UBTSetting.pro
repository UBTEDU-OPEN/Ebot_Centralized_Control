#-------------------------------------------------
#
# Project created by QtCreator 2019-09-17T03:38:48
#
#-------------------------------------------------

QT       += widgets network

TARGET = UBTSetting
TEMPLATE = lib

DEFINES += UBTSETTING_LIBRARY

SOURCES += ubtsetting.cpp

HEADERS += ubtsetting.h\
        ubtsetting_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtsetting.ui

RESOURCES += \
    ubtsetting.qrc

TRANSLATIONS += ../languages/UBTSetting_zh_CN.ts \
        ../languages/UBTSetting_en.ts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/release/ -lUBTServerCommunication
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/debug/ -lUBTServerCommunication
else:unix: LIBS += -L$$OUT_PWD/../UBTServerCommunication/ -lUBTServerCommunication

INCLUDEPATH += $$PWD/../UBTServerCommunication
DEPENDPATH += $$PWD/../UBTServerCommunication

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

