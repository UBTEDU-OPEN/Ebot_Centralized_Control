#-------------------------------------------------
#
# Project created by QtCreator 2015-12-24T11:02:10
#
#-------------------------------------------------

QT       += core gui xml network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UBXRobotController
TEMPLATE = lib

DEFINES += UBXROBOTCONTROLLER_EXPORTS

CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/stdafx.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/debug/ -lquazip
else:unix: LIBS += -L$$PWD/../trd/quazip/lib/osx/ -lquazip.1.0.0

INCLUDEPATH += $$PWD/../trd/quazip/include
DEPENDPATH += $$PWD/../trd/quazip/include

win32: LIBS += -L$$PWD/../trd/zlib/lib/ -lzdll

INCLUDEPATH += $$PWD/../trd/zlib/include
DEPENDPATH += $$PWD/../trd/zlib/include

INCLUDEPATH += $$PWD/../zxLib \
    $$PWD/Robot \
    $$PWD/../Common \
    $$PWD/../trd/hidsdi/include

win32:CONFIG(release, debug|release): LIBS += -L../zxLib/release/ -lzxLib
else:win32:CONFIG(debug, debug|release): LIBS += -L../zxLib/debug/ -lzxLib
else:unix: LIBS += -L$$OUT_PWD/../zxLib/ -lzxLib

INCLUDEPATH += $$PWD/../zxLib
DEPENDPATH += $$PWD/../zxLib
win32:{
LIBS += -L$$PWD/../libs -lsetupapi
LIBS += -L$$PWD/../libs -luser32
LIBS += -L$$PWD/../trd/hidsdi/lib -lhid
}

macx:{
LIBS += -framework IOKit -framework CoreFoundation
}

SOURCES += \
    $$PWD/../Common/UBXPublic.cpp \
    $$PWD/Robot/hid.cpp \
    $$PWD/Robot/DataPool.cpp \
    $$PWD/Robot/MyHidClass.cpp \
    $$PWD/Robot/RobotController.cpp \
    $$PWD/stdafx.cpp \
    $$PWD/UBXRobotController.cpp \
    $$PWD/Robot/CRC32.cpp \
    Robot/cmotorcmd.cpp \
    ubxdataprocessor.cpp

HEADERS += \
    $$PWD/../zxLib/zxLib.h \
    $$PWD/../Common/UBXPublic.h \
    $$PWD/Robot/hidapi.h \
    $$PWD/Robot/DataPool.h \
    $$PWD/Robot/IMyPortInterface.h \
    $$PWD/Robot/MyHidClass.h \
    $$PWD/Robot/RobotController.h \
    $$PWD/stdafx.h \
    $$PWD/UBXRobotController.h \
    $$PWD/Robot/CRC32.h \
    ubxdataprocessor.h \
    dataprotocol.h

win32:{
    HEADERS += $$PWD/Robot/SerialPort.h
    SOURCES += $$PWD/Robot/SerialPort.cpp
}

macx:{
    QMAKE_POST_LINK += \
        install_name_tool -change libzxLib.1.dylib @executable_path/libzxLib.1.dylib libUBXRobotController.1.0.0.dylib
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTUtil/release/ -lUBTUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTUtil/debug/ -lUBTUtil
else:unix: LIBS += -L$$OUT_PWD/../UBTUtil/ -lUBTUtil

INCLUDEPATH += $$PWD/../UBTUtil
DEPENDPATH += $$PWD/../UBTUtil

win32: LIBS += -L$$PWD/../trd/ubtlib/lib/ -lUbtlib

INCLUDEPATH += $$PWD/../trd/ubtlib/include
DEPENDPATH += $$PWD/../trd/ubtlib/include
