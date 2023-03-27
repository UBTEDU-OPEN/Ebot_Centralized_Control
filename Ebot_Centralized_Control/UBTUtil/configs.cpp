#include "configs.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>


#include "ciniconfig.h"
#include "filedirhandle.h"



CConfigs::CConfigs()
{
    createDirsForApp();
}

CConfigs::~CConfigs()
{

}

void CConfigs::createDirsForApp()
{
    CFileDirHandle::mkpath(CConfigs::getLocalActLibDir());
    CFileDirHandle::mkpath(CConfigs::getActionThumbnailDir());
    CFileDirHandle::mkpath(CConfigs::getLocalUserConfigDir());
    CFileDirHandle::mkpath(CConfigs::getLocalCusTemplateDir());
    CFileDirHandle::mkpath(CConfigs::getLocalLogDir());
    CFileDirHandle::mkpath(CConfigs::getLocalMusicDir());
    CFileDirHandle::mkpath(CConfigs::getLocalPreviewImgDir());
    CFileDirHandle::mkpath(CConfigs::getLocalDetailedImageDir());
    CFileDirHandle::mkpath(CConfigs::getLocalRecommendImgDir());
    CFileDirHandle::mkpath(CConfigs::getLocalTempDir());
    CFileDirHandle::mkpath(CConfigs::getLocalWaveDir());
    CFileDirHandle::mkpath(CConfigs::getVideoDownloadPath());
    CFileDirHandle::mkpath(CConfigs::getVideoThumbnailDir());
}

QString CConfigs::getRobotDirAction()
{
    return ROBOT_TFCARD_DIR_ACTION;
}

QString CConfigs::getRobotDirMusic()
{
    return ROBOT_TFCARD_DIR_MUSIC;
}

QString CConfigs::getRobotDirMyDownload()
{
    // 我的机器人是嵌入式文件系统路径，需转为unix路径
    return QDir::fromNativeSeparators(getFilePath(getRobotDirAction(), ROBOT_TFCARD_DIR_MYDOWNLOAD));
}

CConfigs *CConfigs::instance()
{
    static CConfigs configs;
    return &configs;
}

QString CConfigs::getFilePath(const QString &fileDir, const QString &fileName)
{
    return CFileDirHandle::getFilePath(fileDir, fileName);
}

QString CConfigs::getAppBasicDataDir()
{
#ifdef Q_OS_MAC
    QString appBasicDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
#else
    QString appBasicDataDir = QCoreApplication::applicationDirPath();
#endif

    return QDir::toNativeSeparators(appBasicDataDir);
}

QString CConfigs::getAppBasicInstallDir()
{
    QString applicationDir = QCoreApplication::applicationDirPath();

#ifdef Q_OS_MAC
    applicationDir = getFilePath(applicationDir, "/.."); //APP exc is at macos dir
#else
    //
#endif

    return QDir::toNativeSeparators(applicationDir);
}

QString CConfigs::getTcpCommandFilePath()
{
    return getFilePath(getLocalUserConfigDir(), TCP_COMMAND_SETTING_FILE);
}

QString CConfigs::getFontDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return QDir::toNativeSeparators(appBasicDataDir + DIR_SPLIT_CHAR + "font" + DIR_SPLIT_CHAR);
}

QString CConfigs::getLocalPreviewImgDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, PREVIEW_IMAGE_DIR_NAME);
}

QString CConfigs::getLocalPreviewImgPath(const QString &fileBasicName)
{
    return getFilePath(getLocalPreviewImgDir(), fileBasicName);
}

QString CConfigs::getLocalDetailedImageDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, DETAILED_IMAGE_DIR_NAME);
}

QString CConfigs::getLocalDetailedImagePath(const QString& fileBasicName)
{
    return getFilePath(getLocalDetailedImageDir(), fileBasicName);
}

QString CConfigs::getLocalRecommendImgDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, RECOMMEND_IMAGE_DIR_NAME);
}

QString CConfigs::getLocalRecommendImgPath(const QString &fileBasicName)
{
    return getFilePath(getLocalRecommendImgDir(), fileBasicName);
}

QString CConfigs::getLocalTempDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, TEMP_DIR_NAME);
}

QString CConfigs::getLocalTempPath(const QString &fileBasicName)
{
    return getFilePath(getLocalTempDir(), fileBasicName);
}

QString CConfigs::getLocalWaveDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, WAVE_DIR_NAME);
}

QString CConfigs::getLocalWavePath(const QString &fileBasicName)
{
    return getFilePath(getLocalWaveDir(), fileBasicName);
}

QString CConfigs::getLocalDownloadDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}

QString CConfigs::getLocalDownloadPath(const QString &fileName)
{
    return getFilePath(getLocalDownloadDir(), fileName);
}

QString CConfigs::getLocalActLibDir()
{
    QString actPath;
    if (CIniConfig::getInstance().getValueActPath(actPath)) {
        if (QDir::isAbsolutePath(actPath)) {
            return actPath;
        }
        else {
            QString appBasicDataDir = getAppBasicDataDir();
            return getFilePath(appBasicDataDir, actPath);
        }
    }
    else {
        QString appBasicDataDir = getAppBasicDataDir();
        return getFilePath(appBasicDataDir, ACT_LIB_DIR_NAME);
    }
}

bool CConfigs::getLocalActLibDir(QString &dir)
{
    dir = getLocalActLibDir();
    return CFileDirHandle::mkPathIfNotExist(dir);
}

void CConfigs::setLocalActionLibDir(const QString &actPath)
{
    CIniConfig::getInstance().setValueActPath(actPath);
}

QString CConfigs::getLocalActLibPath(const QString &fileBasicName)
{
    return getFilePath(getLocalActLibDir(), fileBasicName);
}

QString CConfigs::getActionThumbnailDir()
{
    return QDir::toNativeSeparators(getAppBasicDataDir() + DIR_SPLIT_CHAR + ACT_THUMBNAIL_FOLDER_NAME);
}

void CConfigs::clearActionThumbnailDir()
{
    QDir dir(getActionThumbnailDir());
    if (dir.exists()) {
        dir.removeRecursively();
    }
    dir.mkpath(getActionThumbnailDir());
}

QString CConfigs::getVideoDownloadPath()
{
    QString videoPath;
    if (CIniConfig::getInstance().getValueVideoDownloadPath(videoPath)) {
        if (QDir::isAbsolutePath(videoPath)) {
            return videoPath;
        }
        else {
            QString appBasicDataDir = getAppBasicDataDir();
            return getFilePath(appBasicDataDir, videoPath);
        }
    }
    else {
        QString appBasicDataDir = getAppBasicDataDir();
        return getFilePath(appBasicDataDir, DEFAULT_VIDEO_DOWNLOAD_PATH);
    }
}

bool CConfigs::getVideoDownloadPath(QString &path)
{
    path = getVideoDownloadPath();
    return CFileDirHandle::mkPathIfNotExist(path);
}

void CConfigs::setVideoDownloadPath(const QString &videoPath)
{
    CIniConfig::getInstance().setValueVideoDownloadPath(videoPath);
}

QString CConfigs::getVideoThumbnailDir()
{
    return QDir::toNativeSeparators(getAppBasicDataDir() + DIR_SPLIT_CHAR + VIDE_THUMBNAIL_FOLDER_NAME);
}

void CConfigs::clearVideoThumbnailDir()
{
    QDir dir(getVideoThumbnailDir());
    if (dir.exists()) {
        dir.removeRecursively();
    }
    dir.mkpath(getVideoThumbnailDir());
}

QString CConfigs::getVideoDownloadHistoryFile()
{
    return QDir::toNativeSeparators(getAppBasicDataDir() + DIR_SPLIT_CHAR + VIDE_DOWNLOAD_HISTORY_FILE_NAME);
}

QString CConfigs::getLocalUserConfigDir()
{
    //this is for user configs
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, CONFIG_DIR_NAME);
}

QString CConfigs::getLocalUserConfigPath(const QString &fileBasicName)
{
    return getFilePath(getLocalUserConfigDir(), fileBasicName);
}

QString CConfigs::getLocalCusTemplateDir()
{
    //!!!it is data dir
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, CUS_TEMPLATE_DIR_NAME);
}

QString CConfigs::getLocalDefaultTemplateDir()
{
   //!!!this is app package install dir
    QString appBasicInstallDir = getAppBasicInstallDir();
    return getFilePath(appBasicInstallDir, DEFAULT_TEMPLATE_DIR_NAME);
}

QString CConfigs::getLocalLogDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, LOG_DIR_NAME);
}

QString CConfigs::getLocalLogFilePath()
{
    return getFilePath(getLocalLogDir(), "AlphaRobot.log");
}

QString CConfigs::getLocalMusicDir()
{
    QString appBasicDataDir = getAppBasicDataDir();
    return getFilePath(appBasicDataDir, MUSIC_DIR_NAME);
}

QString CConfigs::getLocalMusicPath(const QString &fileBasicName)
{
    return getFilePath(getLocalMusicDir(), fileBasicName);
}

QString CConfigs::getLocalLanguageDir()
{
    QString strLanguageDir;

#ifdef Q_OS_WIN
    strLanguageDir = getFilePath(CConfigs::getAppBasicInstallDir(), LANGUAGES_DIR_NAME);
#else
    strLanguageDir = CConfigs::getLocalResourcesDir();
#endif

    return strLanguageDir;
}

QString CConfigs::getLocalResDir()
{
    return getFilePath(CConfigs::getAppBasicInstallDir(), RES_DIR_NAME);
}

QString CConfigs::getLocalResPath(const QString &fileBasicName)
{
    return getFilePath(getLocalResDir(), fileBasicName);
}

QString CConfigs::getLocalResourcesDir()
{
    return getFilePath(CConfigs::getAppBasicInstallDir(), RESOURCES_DIR_NAME);
}

QString CConfigs::getLocalResourcesPath(const QString &fileBasicName)
{
    return getFilePath(getLocalResourcesDir(), fileBasicName);
}

QString CConfigs::getLocalRobotDir()
{
    return getFilePath(CConfigs::getAppBasicInstallDir(), ROBOT_DIR_NAME);
}

QString CConfigs::getLocalRobotPath(const QString &fileBasicName)
{
    return getFilePath(getLocalRobotDir(), fileBasicName);
}

QString CConfigs::getIniConfigFile()
{
    return getLocalUserConfigPath("config.ini");
}

