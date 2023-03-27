#include "ubtactionwidget.h"
#include "ui_ubtactionwidget.h"
#include "../UBTUtil/filedirhandle.h"
#include "../UBTServerCommunication/ubtservercommunication.h"
#include "../UBTUtil/configs.h"
#include "../QRoundProgressBar/qroundprogressbar.h"
#include "../common/UBXPublic.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <qDebug>
#include <QJsonObject>

//QString UBTActionWidget::lastDownload = "";

UBTActionWidget::UBTActionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTActionWidget)
{
    ui->setupUi(this);
    ui->actionCoverLbl->hide();
    setDefaultStyle();
    updateThumbnail();
    initDownloadProgress();

    connect(ui->downloadBtn, &QPushButton::clicked, this, &UBTActionWidget::startDownloadAction);
    connect(ui->redownloadBtn, &QPushButton::clicked, this, &UBTActionWidget::startDownloadAction);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadActionProgress, this, &UBTActionWidget::onActionDownloadProgress);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadActionFinished, this, &UBTActionWidget::onDownloadActionFinished);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadActionThumbnailFinished, this, &UBTActionWidget::onDownloadActionThumbnailFinished);
}

UBTActionWidget::~UBTActionWidget()
{
    delete ui;
}

void UBTActionWidget::setActionInfo(const QJsonObject &actionInfo)
{
    actionId = actionInfo["id"].toInt();
    ui->nameLbl->setText(actionInfo["name"].toString());
    actionUrl = QUrl(actionInfo["url"].toString());
    thumbnailUrl = QUrl(actionInfo["thumbnailUrl"].toString());
    actionDownloadFlag = actionInfo["flag"].toInt();
    if (actionDownloadFlag) {
        setDownloadStatus(Downloaded);
    }
    else {
        setDownloadStatus(Undownload);
    }

    startDownloadActionThumbnailUrl();
}

void UBTActionWidget::onActionDownloadProgress(int id, qint64 bytesReceived, qint64 bytesTotal)
{
    if (downloadProgress && actionId == id) {
        double currentValue = static_cast<double>(bytesReceived) / static_cast<double>(bytesTotal);
        downloadProgress->setValue(currentValue * 100);
    }
}

void UBTActionWidget::onDownloadActionFinished(int id, bool succ)
{
    if (id != actionId)
        return;

    if (succ) {
//        if (!lastDownload.isEmpty()) {
//            if (!UnZipFile(lastDownload)) {
//                qDebug() << "########## UBTActionWidget::onDownloadActionFinished unzip file:" << destActFileName << "failed";
//            }
//        }
        if (!UnZipFile(destActFileName)) {
            qDebug() << "UBTActionWidget::onDownloadActionFinished unzip file:" << destActFileName << "failed";
        }
        setDownloadStatus(Downloaded);
        UBTServerCommunication::getInstance()->addActionDownloadRecord(actionId);
//        lastDownload = destActFileName;
        emit actionDownloadFinished(ui->nameLbl->text());
    }
    else {
        setDownloadStatus(Downloadfailed);
        emit actionDownloadFailed(ui->nameLbl->text());
    }
}

void UBTActionWidget::onDownloadActionThumbnailFinished(int id, bool succ)
{
    if (id != actionId)
        return;
//    disconnect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadActionThumbnailFinished, this, &UBTActionWidget::onDownloadActionThumbnailFinished);
    if (succ) {
        updateThumbnail();
    }
}

void UBTActionWidget::setDefaultStyle()
{
    QFile styleSheet(ACTION_WIDGET_DEFAULT_QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        qDebug() << "UBTActionWidget::setDefaultStyle set style sheet.";
        setStyleSheet(styleSheet.readAll());
    }
    else {
        qDebug() << QString("UBTActionWidget::setDefaultStyle open ACTION_WIDGET_DEFAULT_QSS_PATH (%1) failed.").arg(ACTION_WIDGET_DEFAULT_QSS_PATH);
    }
}

void UBTActionWidget::startDownloadAction()
{
    qDebug() << "UBTActionWidget::startDownloadAction";
    QString actLibDir;
    if (CConfigs::instance()->getLocalActLibDir(actLibDir)) {
        auto actionFileName = actionUrl.fileName();
        setDownloadStatus(Downloading);
        QFileInfo aciontFileInfo(actionFileName);
        actionFileName = ui->nameLbl->text() + "." + aciontFileInfo.suffix();
        destActFileName = actLibDir + QDir::toNativeSeparators("/") + actionFileName;
        UBTServerCommunication::getInstance()->downloadAction(actionId, actionUrl, destActFileName);
    }
    else {
        qDebug() << "UBTActionWidget::startDownloadAction invalid dst act dir";
    }
}

void UBTActionWidget::startDownloadActionThumbnailUrl()
{
    qDebug() << "UBTActionWidget::startDownloadActionThumbnailUrl";
    auto destFileName = thumbnailFileFullLocalName();
    if (destFileName.isEmpty())
        return;
    QFile file(destFileName);
    if (file.exists()) {
        updateThumbnail();
    }
    else {
        UBTServerCommunication::getInstance()->downloadActionThumbnail(actionId, thumbnailUrl, destFileName);
    }
}

QString UBTActionWidget::thumbnailFileFullLocalName() const
{
    QString fullName;
    auto thumbnailFileName = thumbnailUrl.fileName();
    if (thumbnailFileName.isEmpty()) {
        return fullName;
    }
    QFileInfo thumbnailFileInfo(thumbnailFileName);
    thumbnailFileName = ui->nameLbl->text() + "." + thumbnailFileInfo.suffix();
    QDir thumbnailDir(CConfigs::instance()->getActionThumbnailDir());
    if (!thumbnailDir.exists()) {
        thumbnailDir.mkpath(CConfigs::instance()->getActionThumbnailDir());
    }
    fullName = CConfigs::instance()->getActionThumbnailDir() + QDir::toNativeSeparators("/") + thumbnailFileName;
    return fullName;
}

void UBTActionWidget::updateThumbnail()
{
    QFileInfo thumbnailFileInfo(thumbnailFileFullLocalName());
    if (thumbnailFileInfo.exists()) {
        ui->thumbnailLbl->setStyleSheet("background-color:#ffffff;border-radius:5px;border-image:url(" + thumbnailFileInfo.absoluteFilePath() + ");");
    }
    else {
        ui->thumbnailLbl->setStyleSheet("background-color:#ffffff;border-radius:5px;border-image:url(:/res/images/img_default.png);");
    }
}

void UBTActionWidget::setDownloadStatus(DownloadStatus status)
{
    QString statusStr;
    switch(status) {
    case Undownload: {
        statusStr = "undownload";
        ui->statusLbl->setText(tr("undownload"));
        ui->downloadBtn->setVisible(true);
        ui->actionCoverLbl->setVisible(false);
        ui->redownloadBtn->setVisible(false);
        downloadProgress->setVisible(false);
    }
        break;
    case Downloading: {
        statusStr = "downloading";
        ui->statusLbl->setText(tr("downloading..."));
        ui->downloadBtn->setVisible(false);
        ui->actionCoverLbl->setVisible(true);
        ui->redownloadBtn->setVisible(false);
        downloadProgress->setValue(0);
        downloadProgress->setVisible(true);
    }
        break;
    case Downloaded: {
        statusStr = "downloaded";
        ui->statusLbl->setText(tr("downloaded"));
        ui->downloadBtn->setVisible(true);
        ui->actionCoverLbl->setVisible(false);
        ui->redownloadBtn->setVisible(false);
        downloadProgress->setVisible(false);
    }
        break;
    case Downloadfailed: {
        statusStr = "downloadfailed";
        ui->statusLbl->setText(tr("download failed"));
        ui->downloadBtn->setVisible(false);
        ui->actionCoverLbl->setVisible(true);
        ui->redownloadBtn->setVisible(true);
        downloadProgress->setVisible(false);
    }
        break;
    }
    ui->statusLbl->setProperty("status", statusStr);
    ui->statusLbl->style()->unpolish(ui->statusLbl);
    ui->statusLbl->style()->polish(ui->statusLbl);
    ui->statusLbl->update();
}

void UBTActionWidget::initDownloadProgress()
{
    downloadProgress = new QRoundProgressBar(this);
    downloadProgress->setDrawText(false);
    downloadProgress->setGeometry(50,25,58,58);
    downloadProgress->setBarStyle(QRoundProgressBar::StyleDonut);
    downloadProgress->setRange(0,100);
    QPalette palette;
    palette.setBrush(QPalette::Window, Qt::NoBrush);
    palette.setBrush(QPalette::AlternateBase, Qt::NoBrush);
    palette.setBrush(QPalette::Highlight, QBrush(QColor(99, 214, 69)));
    palette.setColor(QPalette::Text, QColor(0,0,0));
    //palette.setBrush(QPalette::Base, Qt::white);
    downloadProgress->setPalette(palette);
    downloadProgress->setValue(0);
}
