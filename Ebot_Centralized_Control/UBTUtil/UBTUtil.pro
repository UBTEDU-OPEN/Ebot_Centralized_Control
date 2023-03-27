#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T14:00:31
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = UBTUtil
TEMPLATE = lib

DEFINES += UBTUTIL_LIBRARY

SOURCES += ubtutil.cpp \
    cstringhandle.cpp \
    filedirhandle.cpp \
    MD5.cpp \
    ciniconfig.cpp \
    configs.cpp \
    ubtsemaphore.cpp \
    UBXCritSec.cpp \
    ../common/UBXPublic.cpp

HEADERS += ubtutil.h\
        ubtutil_global.h \
    cstringhandle.h \
    filedirhandle.h \
    MD5.h \
    singleton.h \
    ciniconfig.h \
    configs.h \
    ubtsemaphore.h \
    UBXCritSec.h \
    ../common/UBXPublic.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/debug/ -lquazip
else:unix: LIBS += -L$$PWD/../trd/quazip/lib/osx/ -lquazip.1.0.0

INCLUDEPATH += $$PWD/../trd/quazip/include
DEPENDPATH += $$PWD/../trd/quazip/include

win32: LIBS += -L$$PWD/../trd/zlib/lib/ -lzdll

INCLUDEPATH += $$PWD/../trd/zlib/include
DEPENDPATH += $$PWD/../trd/zlib/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../zxlib/release/ -lzxLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../zxlib/debug/ -lzxLib
else:unix: LIBS += -L$$OUT_PWD/../zxlib/ -lzxLib

INCLUDEPATH += $$PWD/../zxlib
DEPENDPATH += $$PWD/../zxlib

LIBS += -L$$PWD/../trd/version/lib/win32 -lversion
