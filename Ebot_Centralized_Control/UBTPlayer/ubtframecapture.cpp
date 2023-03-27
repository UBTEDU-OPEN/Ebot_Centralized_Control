#include "ubtframecapture.h"
#include "ubtcapturevideosurface.h"

#include <QtMultimedia/QVideoProbe>
#include <QImage>
#include <QFile>
#include <qDebug>

UBTFrameCapture::UBTFrameCapture(QObject *parent) :
    QObject(parent),
    player(nullptr)
{
    player = new QMediaPlayer(this);
//    connect(player, &QMediaPlayer::durationChanged, this, &UBTPlayer::onDurationChanged);
//    connect(player, &QMediaPlayer::positionChanged, this, &UBTPlayer::onPositionChanged);
//    connect(player, static_cast<void (QMediaPlayer::*)()>(&QMediaPlayer::metaDataChanged), this, &UBTPlayer::onMetaDataChanged);
//    connect(player, &QMediaPlayer::mediaStatusChanged, this, &UBTPlayer::onStatusChanged);
//    connect(player, &QMediaPlayer::bufferStatusChanged, this, &UBTPlayer::onBufferingProgress);
//    connect(player, &QMediaPlayer::videoAvailableChanged, this, &UBTPlayer::onVideoAvailableChanged);
    connect(player, static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            this, &UBTFrameCapture::onDisplayErrorMessage);

    surface = new UBTCaptureVideoSurface(this);
    connect(surface, &UBTCaptureVideoSurface::frameAvailable, this, &UBTFrameCapture::onFrameAvailable);
    player->setVideoOutput(surface);
}

UBTFrameCapture::~UBTFrameCapture()
{

}

void UBTFrameCapture::setUrlAndCaptureFile(const QMap<QString, QString> url2captureFile)
{
    this->url2captureFile = url2captureFile;
}

void UBTFrameCapture::startCapture()
{
    if (!player) {
        qDebug() << "UBTFrameCapture::startCapture player is null";
    }

    if (url2captureFile.count() > 0) {
        capturingUrl2File.first = url2captureFile.firstKey();
        capturingUrl2File.second = url2captureFile.first();
        qDebug() << "UBTFrameCapture::startCapture capture url:" << capturingUrl2File.first << "file:" << capturingUrl2File.second;
        player->setMedia(QMediaContent(QUrl(capturingUrl2File.first)));
        player->play();
    }
    else {
        qDebug() << "UBTFrameCapture::startCapture capture list is empty";
    }
}

void UBTFrameCapture::stopCapture()
{
    if (player) {
        player->stop();
    }
}

void UBTFrameCapture::onFrameAvailable(QVideoFrame videoFrame)
{
    videoFrame.map(QAbstractVideoBuffer::ReadOnly);
    auto imageFormat = QVideoFrame::imageFormatFromPixelFormat(videoFrame.pixelFormat());
    if (imageFormat == QImage::Format_Invalid) {
        imageFormat = QImage::Format_Grayscale8;
    }
    QImage recvImage(videoFrame.bits(), videoFrame.width(), videoFrame.height(), imageFormat);
    if (!recvImage.save(capturingUrl2File.second + ".jpg", "JPG")) {
        qDebug() << QString("UBTFrameCapture::onFrameAvailable save %1 failed")
                    .arg(capturingUrl2File.second);
    }
    url2captureFile.remove(capturingUrl2File.first);
    if (player)
        player->stop();
    startCapture();
    videoFrame.unmap();
}

void UBTFrameCapture::onDisplayErrorMessage(QMediaPlayer::Error error)
{
    qDebug() << "UBTFrameCapture::onDisplayErrorMessage error:" << error;
}
