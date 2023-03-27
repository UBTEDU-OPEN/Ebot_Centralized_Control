#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T11:26:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlphaEbot_Centralied_Control
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../common/UBXPublic.cpp \
    titlebar.cpp \
    clanguagemgr.cpp

HEADERS  += mainwindow.h \
    ../common/UBXPublic.h \
    titlebar.h \
    clanguagemgr.h

FORMS    += mainwindow.ui \
    titlebar.ui

TRANSLATIONS += ../languages/MainWidget_zh_CN.ts \
        ../languages/MainWidget_en.ts

macx:{
    ICON = $$PWD/../res/AlphaRobot.icns
}else:win32:{
    RC_FILE = $$PWD/../res/AlphaRobot.rc
}

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTMyPlay/release/ -lUBTMyPlay
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTMyPlay/debug/ -lUBTMyPlay
else:unix: LIBS += -L$$OUT_PWD/../UBTMyPlay/ -lUBTMyPlay

INCLUDEPATH += $$PWD/../UBTMyPlay
DEPENDPATH += $$PWD/../UBTMyPlay

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTMyDownload/release/ -lUBTMyDownload
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTMyDownload/debug/ -lUBTMyDownload
else:unix: LIBS += -L$$OUT_PWD/../UBTMyDownload/ -lUBTMyDownload

INCLUDEPATH += $$PWD/../UBTMyDownload
DEPENDPATH += $$PWD/../UBTMyDownload


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/release/ -lUBTServerCommunication
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTServerCommunication/debug/ -lUBTServerCommunication
else:unix: LIBS += -L$$OUT_PWD/../UBTServerCommunication/ -lUBTServerCommunication

INCLUDEPATH += $$PWD/../UBTServerCommunication
DEPENDPATH += $$PWD/../UBTServerCommunication

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTCustomerCenter/release/ -lUBTCustomerCenter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTCustomerCenter/debug/ -lUBTCustomerCenter
else:unix: LIBS += -L$$OUT_PWD/../UBTCustomerCenter/ -lUBTCustomerCenter

INCLUDEPATH += $$PWD/../UBTCustomerCenter
DEPENDPATH += $$PWD/../UBTCustomerCenter

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTDownloadListItemWidget/release/ -lUBTDownloadListItemWidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTDownloadListItemWidget/debug/ -lUBTDownloadListItemWidget
else:unix: LIBS += -L$$OUT_PWD/../UBTDownloadListItemWidget/ -lUBTDownloadListItemWidget

INCLUDEPATH += $$PWD/../UBTDownloadListItemWidget
DEPENDPATH += $$PWD/../UBTDownloadListItemWidget

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTSetting/release/ -lUBTSetting
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTSetting/debug/ -lUBTSetting
else:unix: LIBS += -L$$OUT_PWD/../UBTSetting/ -lUBTSetting

INCLUDEPATH += $$PWD/../UBTSetting
DEPENDPATH += $$PWD/../UBTSetting

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTPlayer/release/ -lUBTPlayer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTPlayer/debug/ -lUBTPlayer
else:unix: LIBS += -L$$OUT_PWD/../UBTPlayer/ -lUBTPlayer

INCLUDEPATH += $$PWD/../UBTPlayer
DEPENDPATH += $$PWD/../UBTPlayer

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../trd/quazip/lib/win32/debug/ -lquazip
else:unix: LIBS += -L$$PWD/../trd/quazip/lib/osx/ -lquazip.1.0.0

INCLUDEPATH += $$PWD/../trd/quazip/include
DEPENDPATH += $$PWD/../trd/quazip/include

win32: LIBS += -L$$PWD/../trd/zlib/lib/ -lzdll

INCLUDEPATH += $$PWD/../trd/zlib/include
DEPENDPATH += $$PWD/../trd/zlib/include

win32: LIBS += -L$$PWD/../trd/dwmapi/lib/ -ldwmapi

INCLUDEPATH += $$PWD/../trd/dwmapi/include
DEPENDPATH += $$PWD/../trd/dwmapi/include


win32:{
    CONFIG(debug, debug|release){
        QMAKE_POST_LINK += \
            copy /y  \"$$OUT_PWD\..\UBTUtil\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\QRoundProgressBar\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\UBTWidget\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\UBXRobotController\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\UBTServerCommunication\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\zxlib\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTMyPlay\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTMyDownload\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTCustomerCenter\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTDownloadListItemWidget\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTSetting\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTPlayer\debug\*.dll\" \"$$OUT_PWD\debug\*.dll\"
    }else{
        QMAKE_POST_LINK += \
            copy /y  \"$$OUT_PWD\..\UBTUtil\release\*.dll\" \"$$OUT_PWD\release\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\QRoundProgressBar\release\*.dll\" \"$$OUT_PWD\release\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\UBTWidget\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBXRobotController\release\*.dll\" \"$$OUT_PWD\release\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\UBTServerCommunication\release\*.dll\" \"$$OUT_PWD\release\*.dll\"  && \
            copy /y  \"$$OUT_PWD\..\zxlib\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTMyPlay\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTMyDownload\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTCustomerCenter\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTDownloadListItemWidget\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTSetting\release\*.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$OUT_PWD\..\UBTPlayer\release\*.dll\" \"$$OUT_PWD\release\*.dll\"
    }
}

win32:{
    CONFIG(debug, debug|release){
        QMAKE_POST_LINK += && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\msvcp\Debug\*.*\" \"$$OUT_PWD\debug\" && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\zlib\lib\*.*\" \"$$OUT_PWD\debug\" && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\quazip\lib\win32\debug\*.*\" \"$$OUT_PWD\debug\" && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\dwmapi\lib\*.*\" \"$$OUT_PWD\debug\" && \
            xcopy /y /s /e /i  \"$$PWD\..\res\actionlist\*.*\"  \"$$OUT_PWD\debug\actionlist\" && \
            xcopy /y /s /e /i  \"$$PWD\..\res\font\*.*\"  \"$$OUT_PWD\debug\font\" && \
            xcopy /y /s /e /i  \"$$PWD\..\languages\*.qm\" \"$$OUT_PWD\debug\languages\" && \
            xcopy /y /s /e /i  \"$$PWD\..\config\*.ini\" \"$$OUT_PWD\debug\config\"
    }else{
        QMAKE_POST_LINK += && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\msvcp\Release\*.*\" \"$$OUT_PWD\release\" && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\zlib\lib\*.*\" \"$$OUT_PWD\release\" && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\quazip\lib\win32\release\*.*\" \"$$OUT_PWD\release\" && \
            xcopy /y /s /e /i  \"$$PWD\..\trd\dwmapi\lib\*.*\" \"$$OUT_PWD\release\" && \
            xcopy /y /s /e /i  \"$$PWD\..\res\font\*.*\"  \"$$OUT_PWD\release\font\" && \
            xcopy /y /s /e /i  \"$$PWD\..\languages\*.qm\" \"$$OUT_PWD\release\languages\" && \
            xcopy /y /s /e /i  \"$$PWD\..\config\*.ini\" \"$$OUT_PWD\release\config\"
    }
}

macx:{
    ##package app，putin at the end
    CONFIG(release, debug|release){
        QMAKE_POST_LINK += && \
            macdeployqt $$OUT_PWD/$${TARGET}.$$TEMPLATE -verbose=1
    }
}else:win32:{
    CONFIG(debug, debug|release){
        QMAKE_POST_LINK += && \
            copy /y  \"$$(QTDIR)\bin\qt5cored.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5opengld.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5widgetsd.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5guid.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5xmld.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5multimediad.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\Qt5MultimediaWidgetsd.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5networkd.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\Qt5SerialPortd.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\ssleay32.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\libeay32.dll\" \"$$OUT_PWD\debug\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\mediaservice\dsengined.dll\" \"$$OUT_PWD\debug\mediaservice\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\mediaservice\qtmedia_audioengined.dll\" \"$$OUT_PWD\debug\mediaservice\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\mediaservice\wmfengined.dll\" \"$$OUT_PWD\debug\mediaservice\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\platforms\qminimald.dll\" \"$$OUT_PWD\debug\platforms\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\platforms\qoffscreend.dll\" \"$$OUT_PWD\debug\platforms\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\platforms\qwindowsd.dll\" \"$$OUT_PWD\debug\platforms\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\imageformats\qicod.dll\" \"$$OUT_PWD\debug\imageformats\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\imageformats\qjpegd.dll\" \"$$OUT_PWD\debug\imageformats\*.dll\"
    }else{
        QMAKE_POST_LINK += && \
            copy /y  \"$$(QTDIR)\bin\qt5core.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5opengl.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5widgets.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5gui.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5xml.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5multimedia.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\Qt5MultimediaWidgets.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\qt5network.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\Qt5SerialPort.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\ssleay32.dll\" \"$$OUT_PWD\release\*.dll\" && \
            copy /y  \"$$(QTDIR)\bin\libeay32.dll\" \"$$OUT_PWD\release\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\mediaservice\dsengine.dll\" \"$$OUT_PWD\release\mediaservice\*.dll\" && \
            xcopy /y /s /e /i \"$$(QTDIR)\plugins\mediaservice\qtmedia_audioengine.dll\" \"$$OUT_PWD\release\mediaservice\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\mediaservice\wmfengine.dll\" \"$$OUT_PWD\release\mediaservice\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\platforms\qminimal.dll\" \"$$OUT_PWD\release\platforms\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\platforms\qoffscreen.dll\" \"$$OUT_PWD\release\platforms\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\platforms\qwindows.dll\" \"$$OUT_PWD\release\platforms\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\imageformats\qico.dll\" \"$$OUT_PWD\release\imageformats\*.dll\" && \
            xcopy /y /s /e /i  \"$$(QTDIR)\plugins\imageformats\qjpeg.dll\" \"$$OUT_PWD\release\imageformats\*.dll\"
    }
}

RESOURCES += \
    resMainWidget.qrc

