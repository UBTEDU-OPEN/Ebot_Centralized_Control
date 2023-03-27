/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CConfigs.h
* 创建时间：2016/07/15
* 文件标识：
* 文件摘要：参数设置
*
* 当前版本：1.0.0.0
* 作    者：asu
* 完成时间：2016/07/15
* 版本摘要：
*/

#ifndef CCONFIGS_H
#define CCONFIGS_H


#include "UBTUTIL_GLOBAL.H"
#include <QString>

#include "../common/UBXPublic.h"

//!!! app data dir
const QString ACT_LIB_DIR_NAME = "actionlist";
const QString DEFAULT_VIDEO_DOWNLOAD_PATH = "video";
const QString CONFIG_DIR_NAME = "config";
const QString ACT_THUMBNAIL_FOLDER_NAME = "actionThumbnail";
const QString VIDE_THUMBNAIL_FOLDER_NAME = "videoThumbnail";
const QString VIDE_DOWNLOAD_HISTORY_FILE_NAME = "videoDownloadHistory";

const QString CUS_TEMPLATE_DIR_NAME = "CusTemplate";//!!!it is data dir
const QString DEFAULT_TEMPLATE_DIR_NAME = "defaultTemplate";//!!!this is app package install dir

const QString LOG_DIR_NAME = "Log"; // Log目录名称
const QString MUSIC_DIR_NAME = "Music";//Music目录名称
const QString PREVIEW_IMAGE_DIR_NAME = "PreviewImage";
const QString RECOMMEND_IMAGE_DIR_NAME = "RecommendImage";
const QString TEMP_DIR_NAME = "Temp";// Temp目录名称
const QString WAVE_DIR_NAME = "Wave";// Wave目录名称
const QString DETAILED_IMAGE_DIR_NAME = "DetailedImage"; // 动作详情图片


//!!! app package install dir
const QString LANGUAGES_DIR_NAME = "languages";
const QString RES_DIR_NAME = "res";
const QString RESOURCES_DIR_NAME = "Resources";
const QString ROBOT_DIR_NAME = "Robot";

// 机器人内存卡路径
const QString ROBOT_TFCARD_DIR_ACTION = "action";
const QString ROBOT_TFCARD_DIR_MUSIC = "music";
const QString ROBOT_TFCARD_DIR_MYDOWNLOAD = "my download";

//const QString LAYER_DIR_NAME = "Layer";//???

const QString TCP_COMMAND_SETTING_FILE = "tcpcommand.ini";

class UBTUTILSHARED_EXPORT CConfigs
{
public:
    ~CConfigs();

public:
    static CConfigs *instance();

public:
    //!!! all path is end without dir split char "/"

    QString getFilePath(const QString& fileDir, const QString& fileName);

    //!app basic data dir
    QString getAppBasicDataDir();
    //!app package install basic dir
    QString getAppBasicInstallDir();

    QString getTcpCommandFilePath();

    QString getFontDir();

    //!app data path
    QString getLocalActLibDir();
    bool getLocalActLibDir(QString &dir);
    void setLocalActionLibDir(const QString &actPath);
    QString getLocalActLibPath(const QString& fileBasicName);
    QString getActionThumbnailDir();
    void clearActionThumbnailDir();

    QString getVideoDownloadPath();
    bool getVideoDownloadPath(QString &path);
    void setVideoDownloadPath(const QString &videoPath);
    QString getVideoThumbnailDir();
    void clearVideoThumbnailDir();

    QString getVideoDownloadHistoryFile();

    //this is for user configs
    QString getLocalUserConfigDir();
    QString getLocalUserConfigPath(const QString& fileBasicName);

    QString getLocalCusTemplateDir();
    QString getLocalDefaultTemplateDir();

    QString getLocalLogDir();
    QString getLocalLogFilePath();

    QString getLocalMusicDir();
    QString getLocalMusicPath(const QString& fileBasicName);

    QString getLocalPreviewImgDir();
    QString getLocalPreviewImgPath(const QString& fileBasicName);

    // 动作详情页预览图片
    QString getLocalDetailedImageDir();
    QString getLocalDetailedImagePath(const QString& fileBasicName);

    QString getLocalRecommendImgDir();
    QString getLocalRecommendImgPath(const QString& fileBasicName);

    QString getLocalTempDir();
    QString getLocalTempPath(const QString& fileBasicName);

    QString getLocalWaveDir();
    QString getLocalWavePath(const QString& fileBasicName);

    QString getLocalDownloadDir();
    QString getLocalDownloadPath(const QString& fileName);


    //!app package install path
    QString getLocalLanguageDir();

    QString getLocalResDir();
    QString getLocalResPath(const QString& fileBasicName);

    QString getLocalResourcesDir();
    QString getLocalResourcesPath(const QString& fileBasicName);

    QString getLocalRobotDir();
    QString getLocalRobotPath(const QString& fileBasicName);


    //some files path
    //动作表配置文件路径
    QString getIniConfigFile();

    void createDirsForApp();

    //===================================
    // 获取机器人内存卡内部路径
    QString getRobotDirAction();     // 获取机器人TF卡内部路径
    QString getRobotDirMusic();  // 获取机器人TF卡音乐路径
    QString getRobotDirMyDownload(); // 获取机器人TF卡
    //====================================


private:
    explicit CConfigs();

};

#endif // CCONFIGS_H





