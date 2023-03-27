#include "ubtservercommunication.h"
#include "../UBTUtil/MD5.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QDateTime>
#include <QTimeZone>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include "ubtlib.h"

UBTServerCommunication *UBTServerCommunication::instance = nullptr;

UBTServerCommunication::UBTServerCommunication(QObject *parent)
    : QObject(parent)
{
//    serverUrl = "https://test79.ubtrobot.com/v1/alpha-centre/";   // 测试环境
    serverUrl = "https://cloudapi.ubtrobot.com/v1/alpha-centre/";   // 正式环境
    networkAccessManager = new QNetworkAccessManager(this);

    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &UBTServerCommunication::onReplyFinished);
}

UBTServerCommunication *UBTServerCommunication::getInstance()
{
    if (!instance)
        instance = new UBTServerCommunication(nullptr);
    return instance;
}

bool UBTServerCommunication::isLogin()
{
    return !authorization.isEmpty();
}

void UBTServerCommunication::addActionDownloadRecord(int actionId)
{
    qDebug() << "UBTServerCommunication::addActionDownloadRecord actionId:" << actionId;
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "action/addRecord"));
    setHttpHeader(request, true);

    QByteArray postData;
    postData.append(QString("{\"actionId\":%1}").arg(actionId));

    auto reply = networkAccessManager->post(request, postData);
    reply2req.insert(reply, AddActionDownloadRecord);
}

void UBTServerCommunication::clearActionDownloadRecord(QList<int> deleteIds)
{
    qDebug() << "UBTServerCommunication::clearActionDownloadRecord";
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "action/deleteRecord"));
    setHttpHeader(request, true);
    QByteArray postData;
    QStringList ids;
    if (deleteIds.count() > 0) {
        for (int i = 0; i < deleteIds.count(); ++i) {
            ids.append(QString::number(deleteIds.at(i)));
        }
    }
    postData.append(QString("{\"actionIds\":[%1]}").arg(ids.join(",")));
    auto reply = networkAccessManager->post(request, postData);
    reply2req.insert(reply, ClearActionDownloadRecord);
}

void UBTServerCommunication::getActionList(const QString &searchKey)
{
    qDebug() << "UBTServerCommunication::getActionList searchKey:" << searchKey;
    QNetworkRequest request;
    if (searchKey.isEmpty())
        request.setUrl(QUrl(serverUrl + "action/list"));
    else
        request.setUrl(QUrl(serverUrl + "action/list?searchKey=" + searchKey));
    setHttpHeader(request, true);

    auto reply = networkAccessManager->get(request);
    reply2req.insert(reply, GetActionList);
}

void UBTServerCommunication::getActionDownloadHistory()
{
    qDebug() << "UBTServerCommunication::getActionDownloadHistory";
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "action/list"));
    setHttpHeader(request, true);

    auto reply = networkAccessManager->get(request);
    reply2req.insert(reply, GetActionDownloadHistory);
}

void UBTServerCommunication::getVideoList(const QString &searchKey)
{
    qDebug() << "UBTServerCommunication::getVideoList searchKey:" << searchKey;
    getVideoOrPerformanceList(20, searchKey);
}

void UBTServerCommunication::getPerformanceList(const QString &searchKey)
{
    qDebug() << "UBTServerCommunication::getPerformacneList searchKey:" << searchKey;
    getVideoOrPerformanceList(21, searchKey);
}

void UBTServerCommunication::downloadAction(int id, const QUrl &url, const QString &dstFile)
{
    qDebug() << "UBTServerCommunication::downloadAction url:" << url.toString() << "dest file:" << dstFile;
    download(Action, id, url, dstFile);
}

void UBTServerCommunication::downloadActionThumbnail(int id, const QUrl &url, const QString &dstFile)
{
    qDebug() << "UBTServerCommunication::downloadActionThumbnail id:" << id << "url:" << url.toString() << "dest file:" << dstFile;
    download(ActionThumbnail, id, url, dstFile);
}

void UBTServerCommunication::downloadVideo(int id, const QUrl &url, const QString &dstFile)
{
    qDebug() << "UBTServerCommunication::downloadVideo url:" << url.toString() << "dest file:" << dstFile;
    download(Video, id, url, dstFile);
}

void UBTServerCommunication::downloadVideoThumbnail(int id, const QUrl &url, const QString &dstFile)
{
    qDebug() << "UBTServerCommunication::downloadVideoThumbnail url:" << url.toString() << "dest file:" << dstFile;
    download(VideoThumbnail, id, url, dstFile);
}

void UBTServerCommunication::downloadNewSoftware(const QUrl &url, const QString &dstFile)
{
    qDebug() << "UBTServerCommunication::downloadNewSoftware url:" << url.toString() << "dest file:" << dstFile;
    download(NewSoftware, -1, url, dstFile);
}

void UBTServerCommunication::getMobileVerificationCode(const QString &mobileNumber)
{
    qDebug() << "UBTServerCommunication::getMobileVerificationCode mobile number:" << mobileNumber;
    getVerificationCode(1, mobileNumber);
}

void UBTServerCommunication::getEMailVerificationCode(const QString &emailAddr)
{
    qDebug() << "UBTServerCommunication::getEMailVerificationCode mobile number:" << emailAddr;
    getVerificationCode(2, emailAddr);
}

void UBTServerCommunication::registerNewMobileAccount(const QString &mobileNumber, const QString &vcode, const QString &pwd)
{
    qDebug() << "UBTServerCommunication::registerNewMobileAccount mobile number:" << mobileNumber << "vcode:" << vcode << "pwd:" << pwd;
    registerNewAccount(1, mobileNumber, vcode, pwd);
}

void UBTServerCommunication::registerNewEMailAccount(const QString &email, const QString &vcode, const QString &pwd)
{
    qDebug() << "UBTServerCommunication::registerNewEMailAccount email:" << email << "vcode:" << vcode << "pwd:" << pwd;
    registerNewAccount(2, email, vcode, pwd);
}

void UBTServerCommunication::login(const QString &account, const QString &password)
{
    qDebug() << "UBTServerCommunication::login account:" << account << "password:" << password;

    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "user/login"));
    setHttpHeader(request, false);

    QByteArray postData;
    if (account.contains("@")) { // email
        postData.append(QString("{\"password\":\"%1\",\"type\":%2,\"value\":\"%3\"}").arg(password).arg(2).arg(account));
    }
    else {  // mobile
        postData.append(QString("{\"password\":\"%1\",\"type\":%2,\"value\":\"%3\"}").arg(password).arg(1).arg(account));
    }
    qDebug() << "UBTServerCommunication::login post content:" << postData;

    auto reply = networkAccessManager->post(request, postData);
    reply2req.insert(reply, Login);
}

void UBTServerCommunication::modifyPassword(const QString &account, const QString &vcode, const QString &pwd)
{
    qDebug() << "UBTServerCommunication::modifyPassword account" << account << "vcode:" << vcode << "pwd:" << pwd;
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "user/update"));
    setHttpHeader(request, false);

    QByteArray postData;
    if (account.contains("@")) { // email
        postData.append(QString("{\"password\":\"%1\",\"code\":\"%2\",\"type\":%3,\"value\":\"%4\"}")
                        .arg(pwd).arg(vcode).arg(2).arg(account));
    }
    else {  // mobile
        postData.append(QString("{\"password\":\"%1\",\"code\":\"%2\",\"type\":%3,\"value\":\"%4\"}")
                        .arg(pwd).arg(vcode).arg(1).arg(account));
    }
    qDebug() << "UBTServerCommunication::modifyPassword post content:" << postData;

    auto reply = networkAccessManager->post(request, postData);
    reply2req.insert(reply, ModifyPassword);
}

void UBTServerCommunication::deactivate()
{
    qDebug() << "UBTServerCommunication::deactivate";

    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "user/delete"));
    setHttpHeader(request, true);

    QByteArray postData;
    auto reply = networkAccessManager->post(request, postData);
    reply2req.insert(reply, Deactivate);

    authorization.clear();
}

void UBTServerCommunication::logout()
{
    authorization.clear();
    emit logoutReply();
}

void UBTServerCommunication::upgradeRequest(const QString &currentVersion)
{
    qDebug() << "UBTServerCommunication::upgradeRequest currentVersion:" << currentVersion;
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "sys/upgrade"));
    setHttpHeader(request, true);

    QByteArray postData;
    postData.append(QString("{\"version\":\"%1\"}").arg(currentVersion));
    auto reply = networkAccessManager->post(request, postData);
    reply2req.insert(reply, Upgrage);
}

void UBTServerCommunication::onReplyFinished(QNetworkReply *reply)
{
    if (!reply2req.contains(reply)) {
        qDebug() << "UBTServerCommunication::onReplyFinished reply is about to delete...";
    }
    auto httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    auto networkError = reply->error();
    qDebug() << "UBTServerCommunication::onReplyFinished req:" << reply2req.value(reply)
             << "httpStatusCode:" << httpStatusCode << "networkError:" << networkError;
    bool succ = (httpStatusCode == 200 && networkError == QNetworkReply::NoError);
    QByteArray bytes = reply->readAll();

    if (httpStatusCode == 401) {
        emit networkErrorOccur(AuthenticationRequiredError);
    }
    else if (httpStatusCode == 0 && networkError == QNetworkReply::UnknownNetworkError) {
        emit networkErrorOccur(UnknownError);
    }
    else {
        switch(reply2req.value(reply)) {
        case AddActionDownloadRecord: {
            emit addActionDownloadRecordReply(httpStatusCode, networkError);
        }
            break;
        case ClearActionDownloadRecord: {
            emit clearActionDownloadRecordReply(succ, reply->errorString(), bytes);
        }
            break;
        case GetActionList: {
            emit getActionListReply(succ, reply->errorString(), bytes);
        }
            break;
        case GetActionDownloadHistory: {
            emit getActionDownloadHistoryReply(succ, reply->errorString(), bytes);
        }
            break;
        case GetVideoList: {
            qDebug() << "UBTServerCommunication::onReplyFinished GetVideoList, reply:" << QString(bytes);
            emit getVideoListReply(succ, reply->errorString(), bytes);
        }
            break;
        case GetPerformacneList: {
    //        qDebug() << "UBTServerCommunication::onReplyFinished GetPerformacneList, reply:" << QString(bytes);
            emit getPerformanceListReply(succ, reply->errorString(), bytes);
        }
            break;
        case DownloadAction: {
            auto downloadInfo = onDownloadReply(reply, succ, bytes);
            emit downloadActionFinished(downloadInfo.id, succ);
        }
            break;
        case DownloadActionThumbnail: {
            auto downloadInfo = onDownloadReply(reply, succ, bytes);
            emit downloadActionThumbnailFinished(downloadInfo.id, succ);
        }
            break;
        case DownloadVideo: {
            auto downloadInfo = onDownloadReply(reply, succ, bytes);
            emit downloadVideoFinished(downloadInfo.id, downloadInfo.dstFile, succ);
        }
            break;
        case DownloadVideoThumbnail: {
            auto downloadInfo = onDownloadReply(reply, succ, bytes);
            emit downloadVideoThumbnailFinished(downloadInfo.id, succ);
        }
            break;
        case DownloadNewSoftware: {
            onDownloadReply(reply, succ, bytes);
            emit downloadNewSoftwareFinished(succ);
        }
            break;
        case GetMobileVCode: {
            emit getMobileVerificationCodeReply(succ, reply->errorString(), bytes);
        }
            break;
        case GetEMailVCode: {
            emit getEMailVerificationCodeReply(succ, reply->errorString(), bytes);
        }
            break;
        case RegisterNewMobileAccount: {
            qDebug() << "UBTServerCommunication::onReplyFinished RegisterNewMobileAccount, reply:" << QString(bytes);
            emit registerNewMobileAccountReply(succ, reply->errorString(), bytes);
        }
            break;
        case RegisterNewEMailAccount: {
            qDebug() << "UBTServerCommunication::onReplyFinished RegisterNewEMailAccount, reply:" << QString(bytes);
            emit registerNewEMailAccountReply(succ, reply->errorString(), bytes);
        }
            break;
        case Login: {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes);
            QJsonObject replayObj = jsonDoc.object();
            authorization = replayObj.value("authorization").toString();
            if (replayObj.contains("email")){
                account = replayObj.value("email").toString();
            }
            else if (replayObj.contains("phone")) {
                account = replayObj.value("phone").toString();
            }
            qDebug() << "UBTServerCommunication::onReplyFinished Login, reply:" << QString(bytes) << "set authorization:" << authorization;

            emit loginReply(succ, reply->errorString(), bytes);
        }
            break;
        case ModifyPassword: {
            qDebug() << "UBTServerCommunication::onReplyFinished ModifyPassword, reply:" << QString(bytes);
            emit modifyPasswordReply(succ, reply->errorString(), bytes);
        }
            break;
        case Deactivate: {
            qDebug() << "UBTServerCommunication::onReplyFinished Deactivate, reply:" << QString(bytes);
            emit deactivateReply(succ, reply->errorString(), bytes);
        }
            break;
        case Upgrage: {
            qDebug() << "UBTServerCommunication::onReplyFinished Upgrage, reply:" << QString(bytes);
            emit upgradeRequestReply(succ, reply->errorString(), bytes);
        }
            break;
        default: {
            qDebug() << "UBTServerCommunication::onReplyFinished unknown type, reply:" << QString(bytes);
        }
            break;
        }
    }

    reply2req.remove(reply);
    reply->deleteLater();
}

void UBTServerCommunication::getVerificationCode(int type, const QString &value)
{
    qDebug() << "UBTServerCommunication::getVerificationCode type:" << type << "value:" << value;
    if (type != 1 && type != 2) {
        qDebug() << "UBTServerCommunication::getVerificationCode invalid type";
        return;
    }
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "user/sendCode"));
    setHttpHeader(request, false);

    QByteArray postData;
    postData.append(QString("{\"type\":%1,\"value\":\"%2\"}").arg(type).arg(value));
    qDebug() << "UBTServerCommunication::getVerificationCode post content:" << postData;

    auto reply = networkAccessManager->post(request, postData);
    if (type == 1) { // mobile
        reply2req.insert(reply, GetMobileVCode);
    }
    else if (type == 2) { // email
        reply2req.insert(reply, GetEMailVCode);
    }
}

void UBTServerCommunication::registerNewAccount(int type, const QString &value, const QString &code, const QString &password)
{
    qDebug() << "UBTServerCommunication::registerNewAccount type:" << type << "value:" << value << "vcode:" << code << "pwd:" << password;
    if (type != 1 && type != 2) {
        qDebug() << "UBTServerCommunication::registerNewAccount invalid type";
        return;
    }
    QNetworkRequest request;
    request.setUrl(QUrl(serverUrl + "user/register"));
    setHttpHeader(request, false);

    QByteArray postData;
    postData.append(QString("{\"code\":\"%1\",\"password\":\"%2\",\"type\":%3,\"value\":\"%4\"}")
                    .arg(code).arg(password).arg(type).arg(value));
    qDebug() << "UBTServerCommunication::registerNewAccount post content:" << postData;

    auto reply = networkAccessManager->post(request, postData);
    if (type == 1) { // mobile
        reply2req.insert(reply, RegisterNewMobileAccount);
    }
    else if (type == 2) { // email
        reply2req.insert(reply, RegisterNewEMailAccount);
    }
}

void UBTServerCommunication::getVideoOrPerformanceList(int videoType, const QString &searchKey)
{
    qDebug() << "UBTServerCommunication::getVideoOrPerformanceList video type:" << videoType << "searchKey:" << searchKey;
    if (videoType != 20 && videoType != 21) {
        qDebug() << "UBTServerCommunication::getVideoOrPerformanceList invalid video type:" << videoType;
    }
    QNetworkRequest request;
    if (searchKey.isEmpty())
        request.setUrl(QUrl(serverUrl + "video/list?type=" + QString::number(videoType)));
    else
        request.setUrl(QUrl(serverUrl + "video/list?type=" + QString::number(videoType) + "&searchKey=" + searchKey));
    setHttpHeader(request, true);

    auto reply = networkAccessManager->get(request);
    if (videoType == 20) { // video
        reply2req.insert(reply, GetVideoList);
    }
    else if (videoType == 21) { // performance
        reply2req.insert(reply, GetPerformacneList);
    }
}



void UBTServerCommunication::setHttpHeader(QNetworkRequest &request, bool needAuthorization)
{
    const QString appId("10PC10000");
    const QString deviceId(QHostInfo::localHostName());

    auto sign = getHeaderXUBTSignV3(deviceId.toStdString());
    qDebug() << "UBTServerCommunication::setHttpHeader sign:" << sign.c_str() << "appId:" << appId << "deviceId:" << deviceId << "authorization" << authorization;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=UTF-8");
    request.setRawHeader("X-UBT-Sign", QByteArray::fromStdString(sign));
    request.setRawHeader("X-UBT-AppId", QByteArray::fromStdString(appId.toStdString()));
    request.setRawHeader("X-UBT-DeviceId", QByteArray::fromStdString(deviceId.toStdString()));
    if (needAuthorization) {
        request.setRawHeader("authorization", QByteArray::fromStdString(authorization.toStdString()));
    }
}

void UBTServerCommunication::download(UBTServerCommunication::DownloadType downloadType, int id, const QUrl &url, const QString &dstFile)
{
    qDebug() << "UBTServerCommunication::download downloadType:" << downloadType << "id:" << id
             <<"url:" << url << "dstFile:" << dstFile;
    DownloadInfo downloadInfo = {
        downloadType,
        id,
        dstFile
    };
    QNetworkRequest request(url);
    auto downloadReply = networkAccessManager->get(request);
    switch(downloadType) {
    case Action: {
        reply2req.insert(downloadReply, DownloadAction);
    }
        break;
    case ActionThumbnail: {
        reply2req.insert(downloadReply, DownloadActionThumbnail);
    }
        break;
    case Video: {
        reply2req.insert(downloadReply, DownloadVideo);
    }
        break;
    case VideoThumbnail: {
        reply2req.insert(downloadReply, DownloadVideoThumbnail);
    }
        break;
    case NewSoftware: {
        reply2req.insert(downloadReply, DownloadNewSoftware);
    }
        break;
    default: {
    }
        break;
    }
    downloadReply2downloadInfo.insert(downloadReply, downloadInfo);

    if (downloadType == Action) {
        connect(downloadReply, &QNetworkReply::downloadProgress, this, [this, downloadReply, downloadInfo](qint64 bytesReceived, qint64 bytesTotal) {
            if (!reply2req.contains(downloadReply)) {
                qDebug() << "UBTServerCommunication::download, reply is about to delete...";
            }
            emit downloadActionProgress(downloadInfo.id, bytesReceived, bytesTotal);
        });
    }
    else if (downloadType == Video) {
        connect(downloadReply, &QNetworkReply::downloadProgress, this, [this, downloadReply, downloadInfo](qint64 bytesReceived, qint64 bytesTotal) {
            if (!reply2req.contains(downloadReply)) {
                qDebug() << "UBTServerCommunication::download, reply is about to delete...";
            }
            emit downloadVideoProgress(downloadInfo.id, downloadInfo.dstFile, bytesReceived, bytesTotal);
        });
    }
}

DownloadInfo UBTServerCommunication::onDownloadReply(QNetworkReply *reply, bool &ok, const QByteArray &data)
{
    auto downloadInfo = downloadReply2downloadInfo.take(reply);
    QFile file(downloadInfo.dstFile);
    if (!ok) {
        return downloadInfo;
    }
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << QString("Could not open %1 for writing: %2(%3)")
                    .arg(downloadInfo.dstFile)
                    .arg(file.errorString())
                    .arg(file.error());
        ok = false;
    }
    else {
        file.write(data);
        file.close();
        ok = true;
    }
    return downloadInfo;
}



