#include "ubtvideowidget.h"
#include "ui_ubtvideowidget.h"
#include "../UBTServerCommunication/ubtservercommunication.h"
#include "../UBTUtil/configs.h"
#include "../QRoundProgressBar/qroundprogressbar.h"
#include "../UBTWidget/UBTInfoDlg.h"

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <qDebug>

UBTVideoWidget::UBTVideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTVideoWidget),
    mDownloadStatus(Unknown),
    VIDEO_WIDGET_DEFAULT_QSS_PATH(":/res/qss/ubtvideowidget.qss")
{
    ui->setupUi(this);
    setDefaultStyle();
    initDownloadProgress();
    updateThumbnail();

    connect(ui->downloadBtn, &QPushButton::clicked, this, &UBTVideoWidget::startDownloadVideo);
    connect(ui->playBtn, &QPushButton::clicked, this, &UBTVideoWidget::onPlayBtnClicked);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoProgress, this, &UBTVideoWidget::onVideoDownloadProgress);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoFinished, this, &UBTVideoWidget::onDownloadVideoFinished);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoThumbnailFinished, this, &UBTVideoWidget::onDownloadVideoThumbnailFinished);
}

UBTVideoWidget::~UBTVideoWidget()
{
    delete ui;
}

void UBTVideoWidget::setVideoInfo(const QJsonObject &json)
{
    videoId = json.value("id").toInt();
    ui->videoName->setText(json.value("name").toString());
    videoUrl = QUrl(json.value("url").toString());
    thumbnailUrl = QUrl(json.value("thumbnailUrl").toString());
    setDownloadStatus(CanDownload);
    startDownloadVideoThumbnail();
}

void UBTVideoWidget::setDefaultStyle()
{
    QFile styleSheet(VIDEO_WIDGET_DEFAULT_QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        qDebug() << "UBTVideoWidget::setDefaultStyle set style sheet.";
        setStyleSheet(styleSheet.readAll());
    }
    else {
        qDebug() << QString("UBTVideoWidget::setDefaultStyle open VIDEO_WIDGET_DEFAULT_QSS_PATH (%1) failed.").arg(VIDEO_WIDGET_DEFAULT_QSS_PATH);
    }
}

void UBTVideoWidget::initDownloadProgress()
{
    downloadProgress = new QRoundProgressBar(this);
    downloadProgress->setDrawText(false);
    downloadProgress->setGeometry(71, 39, 58, 58);
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
    downloadProgress->setVisible(false);
}

void UBTVideoWidget::onVideoDownloadProgress(int id, QString dstFile, qint64 bytesReceived, qint64 bytesTotal)
{
    if (downloadProgress && videoId == id) {
        setDownloadStatus(Downloading);
        double currentValue = static_cast<double>(bytesReceived) / static_cast<double>(bytesTotal);
        currentValue *= 100;
        downloadProgress->setValue(currentValue);
    }
}

void UBTVideoWidget::onDownloadVideoFinished(int id, QString dstFile, bool succ)
{
    if (id != videoId)
        return;
    setDownloadStatus(CanDownload);
    if (!succ) {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::FailureIcon, tr("download failed"), this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
    }
}

void UBTVideoWidget::onDownloadVideoThumbnailFinished(int id, bool succ)
{
    if (id != videoId)
        return;
    if (succ)
        updateThumbnail();
}

void UBTVideoWidget::startDownloadVideo()
{
    setDownloadStatus(Downloading);
    auto videoFileName = videoUrl.fileName();
    QFileInfo videoFileInfo(videoFileName);
    videoFileName = ui->videoName->text() + "." + videoFileInfo.suffix();
    QString videoDownloadPath;
    if (CConfigs::instance()->getVideoDownloadPath(videoDownloadPath)) {
        destVideoFileName = videoDownloadPath + QDir::toNativeSeparators("/") + videoFileName;
        UBTServerCommunication::getInstance()->downloadVideo(videoId, videoUrl, destVideoFileName);
        emit videoDownloadStart(videoId, ui->videoName->text(), videoUrl, destVideoFileName);
    }
    else {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("can not find video save path"), this);
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
}

void UBTVideoWidget::startDownloadVideoThumbnail()
{
    auto destFileName = getThumbnailFullLocalName();
    if (destFileName.isEmpty())
        return;
    QFile file(destFileName);
    if (file.exists()) {
        updateThumbnail();
    }
    else {
        UBTServerCommunication::getInstance()->downloadVideoThumbnail(videoId, QUrl(thumbnailUrl), destFileName);
    }
}

QString UBTVideoWidget::getThumbnailFullLocalName() const
{
    QString fullName;
    auto thumbnailFileName = thumbnailUrl.fileName();
    if (thumbnailFileName.isEmpty()) {
        return fullName;
    }
    QFileInfo thumbnailFileInfo(thumbnailFileName);
    thumbnailFileName = ui->videoName->text() + "." + thumbnailFileInfo.suffix();
    QDir thumbnailDir(CConfigs::instance()->getVideoThumbnailDir());
    if (!thumbnailDir.exists()) {
        thumbnailDir.mkpath(CConfigs::instance()->getVideoThumbnailDir());
    }
    fullName = CConfigs::instance()->getVideoThumbnailDir() + QDir::toNativeSeparators("/") + thumbnailFileName;
    return fullName;
}

void UBTVideoWidget::updateThumbnail()
{
    QFileInfo thumbnailFileInfo(getThumbnailFullLocalName());
    if (thumbnailFileInfo.exists()) {
        ui->videoPic->setStyleSheet("background-color:#ffffff;border-radius:5px;border-image:url(" + thumbnailFileInfo.absoluteFilePath() + ");");
    }
    else {
        ui->videoPic->setStyleSheet("background-color:#ffffff;border-radius:5px;border-image:url(:/res/images/img_default.png);");
    }
}

void UBTVideoWidget::onPlayBtnClicked()
{
    emit playOnlineVideo(videoUrl);
}

void UBTVideoWidget::setDownloadStatus(UBTVideoWidget::DownloadStatus downloadStatus)
{
    if (mDownloadStatus != downloadStatus) {
        mDownloadStatus = downloadStatus;
        onDownloadStatusChanged();
    }
}

void UBTVideoWidget::onDownloadStatusChanged()
{
    if (mDownloadStatus == Downloading) {
        downloadProgress->setValue(0);
        downloadProgress->setVisible(true);
        ui->downloadBtn->setVisible(false);
        ui->videoWidgetCover->setVisible(true);
        ui->playBtn->setVisible(false);
    }
    else {
        downloadProgress->setVisible(false);
        ui->downloadBtn->setVisible(true);
        ui->videoWidgetCover->setVisible(false);
        ui->playBtn->setVisible(true);
    }
}


