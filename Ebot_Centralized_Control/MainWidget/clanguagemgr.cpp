#include "clanguagemgr.h"

#include <QDir>
#include <QLibraryInfo>
#include <QTranslator>

// 多语言关键字
const QString LANGUAGE_CHINESE_POSTFIX = "zh_CN";  // 简体中文
const QString LANGUAGE_ENGLISH_POSTFIX = "en";  // 英文

CLanguageMgr* CLanguageMgr::getInstance()
{
    static CLanguageMgr lang(NULL);
    return &lang;
}

CLanguageMgr::CLanguageMgr(QObject *parent) : QObject(parent)
{
    m_pApplication = NULL;
}

CLanguageMgr::~CLanguageMgr()
{

}

QApplication *CLanguageMgr::getApplication() const
{
    return m_pApplication;
}

void CLanguageMgr::setApplication(QApplication *pApplication)
{
    m_pApplication = pApplication;
}

bool CLanguageMgr::setLanguage(QString strLocalLanguageDir, QString strLanguageKeyword)
{
    if(!m_pApplication || strLocalLanguageDir.isEmpty() || strLanguageKeyword.isEmpty())
    {
        return false;
    }

    //add by hels for 多语言加载
    //QString strLanguageKeyword = CIniConfig::getInstance().getValueLanguage();
    static QTranslator transMainWidget, transMyPlay, transMyDownload, transCustomerCenter, transSetting, transDownloadListItemWidget;


    setLanguageKeyword(strLanguageKeyword);

    //主工程翻译文件
    QString tsPath = strLocalLanguageDir + "/MainWidget_" + strLanguageKeyword;
    transMainWidget.load(QDir::toNativeSeparators(tsPath));
    m_pApplication->installTranslator(&transMainWidget);


    //更新模块翻译
    tsPath = strLocalLanguageDir + "/UBTMyPlay_" + strLanguageKeyword;
    transMyPlay.load(QDir::toNativeSeparators(tsPath));
    m_pApplication->installTranslator(&transMyPlay);

    tsPath = strLocalLanguageDir + "/UBTMyDownload_" + strLanguageKeyword;
    transMyDownload.load(QDir::toNativeSeparators(tsPath));
    m_pApplication->installTranslator(&transMyDownload);

    tsPath = strLocalLanguageDir + "/UBTCustomerCenter_" + strLanguageKeyword;
    transCustomerCenter.load(QDir::toNativeSeparators(tsPath));
    m_pApplication->installTranslator(&transCustomerCenter);

    tsPath = strLocalLanguageDir + "/UBTDownloadListItemWidget_" + strLanguageKeyword;
    transSetting.load(QDir::toNativeSeparators(tsPath));
    m_pApplication->installTranslator(&transSetting);

    tsPath = strLocalLanguageDir + "/UBTSetting_" + strLanguageKeyword;
    transDownloadListItemWidget.load(QDir::toNativeSeparators(tsPath));
    m_pApplication->installTranslator(&transDownloadListItemWidget);


    //加载Qt自带翻译
    static QTranslator translatorQt;
    QString locale = QLocale::system().name();
    const QString qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    const QString qtTrFile = QLatin1String("qt_") + locale;

    if (translatorQt.load(qtTrFile, qtTrPath) ||
            translatorQt.load(qtTrFile, QDir::toNativeSeparators(
                                  QString(strLocalLanguageDir + "/"))))
    {
        m_pApplication->installTranslator(&translatorQt);
        m_pApplication->setProperty("qtc_locale", locale);
    }

    return true;
}

QString CLanguageMgr::getNetLanguageKeyword(const QString &strLanguageKeyword)
{
    if(strLanguageKeyword == LANGUAGE_CHINESE_POSTFIX)
    {
        return "CN";
    }
    return "EN";
}

QString CLanguageMgr::getLanguageKeyword() const
{
    return m_strLanguageKeyword;
}

void CLanguageMgr::setLanguageKeyword(const QString &strLanguageKeyword)
{
    m_strLanguageKeyword = strLanguageKeyword;
}

QString CLanguageMgr::getLocalLanguageDir() const
{
    return m_strLocalLanguageDir;
}

void CLanguageMgr::setLocalLanguageDir(const QString &strLocalLanguageDir)
{
    m_strLocalLanguageDir = strLocalLanguageDir;
}
