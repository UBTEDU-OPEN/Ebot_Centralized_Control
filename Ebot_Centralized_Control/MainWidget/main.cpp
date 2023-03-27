#include "mainwindow.h"
#include "clanguagemgr.h"
#include "../UBTUtil/configs.h"
#include "../UBTUtil/ciniconfig.h"

#include <QApplication>'
#include <QFontDatabase>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QFontDatabase database;
//    foreach (const QString &family, database.families()) {
//        qDebug() << family;
//    }
//    QStringList fonts;
//    fonts << "SourceHanSansCN-Bold.otf" << "SourceHanSansCN-ExtraLight.otf" << "SourceHanSansCN-Heavy.otf"
//          << "SourceHanSansCN-Light.otf" << "SourceHanSansCN-Medium.otf" << "SourceHanSansCN-Normal.otf"
//          << "SourceHanSansCN-Regular.otf";
//    foreach (const QString &font, fonts) {
//        auto fontPath = CConfigs::getFontDir() + font;
//        if (-1 == QFontDatabase::addApplicationFont(fontPath)) {
//            qDebug() << "Failed to load font:" << font;
//        }
//    }

//    QFont font("SourceHanSansCN-Normal", 10, 75); //第一个属性是字体，第二个是大小，第三个是加粗（权重是75）
//    qDebug() << "\n#### font:" << font.toString();
//    a.setFont(font);

    CLanguageMgr::getInstance()->setApplication((QApplication*)&a);
    CLanguageMgr::getInstance()->setLanguage(CConfigs::instance()->getLocalLanguageDir(), CIniConfig::getInstance().getValueLanguage());

    MainWindow w;
    w.show();
    a.installNativeEventFilter(&w);

    return a.exec();
}
