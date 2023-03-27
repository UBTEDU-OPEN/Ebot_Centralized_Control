#ifndef UBTSERVERCOMMUNICATION_H
#define UBTSERVERCOMMUNICATION_H

#include "ubtservercommunication_global.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QMutex>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
struct DownloadInfo;

class UBTSERVERCOMMUNICATIONSHARED_EXPORT UBTServerCommunication : public QObject
{
    Q_OBJECT

public:
    enum ReqType {
        AddActionDownloadRecord = 0,
        ClearActionDownloadRecord,
        GetActionList,
        GetActionDownloadHistory,
        GetVideoList,
        GetPerformacneList,
        DownloadAction,
        DownloadActionThumbnail,
        DownloadVideo,
        DownloadVideoThumbnail,
        DownloadNewSoftware,
        GetMobileVCode,
        GetEMailVCode,
        RegisterNewMobileAccount,
        RegisterNewEMailAccount,
        Login,
        ModifyPassword,
        Deactivate,
        Upgrage,
    };

    enum DownloadType {
        Action,
        ActionThumbnail,
        Video,
        VideoThumbnail,
        NewSoftware,
    };

    enum LoginEntrance {
        MyPlay,
        CustomerCenter
    };

    // 需要统一处理(显示)的网络错误
    enum NetworkError {
        AuthenticationRequiredError,
        UnknownError
    };

public:
    UBTServerCommunication(QObject *parent = nullptr);

    static UBTServerCommunication* getInstance();

    bool isLogin();
    QString loginAccount() { return account; }

    void addActionDownloadRecord(int actionId);
    void clearActionDownloadRecord(QList<int> deleteIds);
    void getActionList(const QString &searchKey);
    void getActionDownloadHistory();
    void getVideoList(const QString &searchKey);
    void getPerformanceList(const QString &searchKey);
    void downloadAction(int id, const QUrl &url, const QString &dstFile);
    void downloadActionThumbnail(int id, const QUrl &url, const QString &dstFile);
    void downloadVideo(int id, const QUrl &url, const QString &dstFile);
    void downloadVideoThumbnail(int id, const QUrl &url, const QString &dstFile);
    void downloadNewSoftware(const QUrl &url, const QString &dstFile);
    void getMobileVerificationCode(const QString &mobileNumber);
    void getEMailVerificationCode(const QString &emailAddr);
    void registerNewMobileAccount(const QString &mobileNumber, const QString &vcode, const QString &pwd);
    void registerNewEMailAccount(const QString &email, const QString &vcode, const QString &pwd);
    void login(const QString &account, const QString &password);
    void modifyPassword(const QString &account, const QString &vcode, const QString &pwd);
    void deactivate();
    void logout();
    void upgradeRequest(const QString &currentVersion);

    void setLoginEntrance(LoginEntrance entrance) { loginEntrance = entrance; }
    LoginEntrance getLoginEntrance() { return loginEntrance; }

signals:
    void addActionDownloadRecordReply(int httpStatusCode, int networkError);
    void clearActionDownloadRecordReply(bool succ, QString info, QByteArray reply);
    void getActionListReply(bool succ, QString info, QByteArray reply);
    void getActionDownloadHistoryReply(bool succ, QString info, QByteArray reply);
    void getVideoListReply(bool succ, QString info, QByteArray reply);
    void getPerformanceListReply(bool succ, QString info, QByteArray reply);
    void downloadActionProgress(int id, qint64 bytesReceived, qint64 bytesTotal);
    void downloadActionFinished(int id, bool succ);
    void downloadActionThumbnailFinished(int id, bool succ);
    void downloadVideoProgress(int id, QString dstFile, qint64 bytesReceived, qint64 bytesTotal);
    void downloadVideoFinished(int id, QString dstFile, bool succ);
    void downloadVideoThumbnailFinished(int id, bool succ);
    void downloadNewSoftwareFinished(bool succ);
    void getMobileVerificationCodeReply(bool succ, QString info, QByteArray reply);
    void getEMailVerificationCodeReply(bool succ, QString info, QByteArray reply);
    void registerNewMobileAccountReply(bool succ, QString info, QByteArray reply);
    void registerNewEMailAccountReply(bool succ, QString info, QByteArray reply);
    void loginReply(bool succ, QString info, QByteArray reply);
    void modifyPasswordReply(bool succ, QString info, QByteArray reply);
    void deactivateReply(bool succ, QString info, QByteArray reply);
    void logoutReply();
    void upgradeRequestReply(bool succ, QString info, QByteArray reply);
    void networkErrorOccur(NetworkError networkError);

protected:
    void onReplyFinished(QNetworkReply *reply);
    void getVerificationCode(int type, const QString &value);
    void registerNewAccount(int type, const QString &value, const QString &code, const QString &password);
    void getVideoOrPerformanceList(int videoType, const QString &searchKey);
    void setHttpHeader(QNetworkRequest &request, bool needAuthorization);
    void download(DownloadType downloadType, int id, const QUrl &url, const QString &dstFile);
    DownloadInfo onDownloadReply(QNetworkReply *reply, bool &ok, const QByteArray &data);

private:
    static UBTServerCommunication *instance;

    QMutex mutex;

    QString serverUrl;
    QNetworkAccessManager *networkAccessManager;
    QMap<QNetworkReply*, ReqType> reply2req;
    QMap<QNetworkReply*, DownloadInfo> downloadReply2downloadInfo;
    QString authorization;
    QString account;

    LoginEntrance loginEntrance;
};

struct DownloadInfo {
    UBTServerCommunication::DownloadType downloadType;
    int id;
    QString dstFile;
};

#endif // UBTSERVERCOMMUNICATION_H
