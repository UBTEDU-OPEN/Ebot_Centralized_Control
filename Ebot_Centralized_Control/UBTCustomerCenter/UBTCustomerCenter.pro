#-------------------------------------------------
#
# Project created by QtCreator 2019-09-04T14:05:40
#
#-------------------------------------------------

QT       += widgets

TARGET = UBTCustomerCenter
TEMPLATE = lib

DEFINES += UBTCUSTOMERCENTER_LIBRARY

SOURCES += ubtcustomercenter.cpp \
    ubtcustomercenterwidget.cpp \
    ubtprivacyagreementdlg.cpp

HEADERS += ubtcustomercenter.h\
        ubtcustomercenter_global.h \
    ubtcustomercenterwidget.h \
    ubtprivacyagreementdlg.h \
    ubtprivactydlg.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    ubtcustomercenterwidget.ui \
    ubtprivacyagreementdlg.ui


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

RESOURCES += \
    ubtcustomercenter.qrc

TRANSLATIONS += ../languages/UBTCustomerCenter_zh_CN.ts \
        ../languages/UBTCustomerCenter_en.ts

DISTFILES +=

