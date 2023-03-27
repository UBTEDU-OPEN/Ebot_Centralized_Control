#include "ubtplayer.h"
#include "ui_ubtplayer.h"
#include "VideoWidget.h"

#include <QSize>
#include <QSlider>
#include <QMediaContent>
#include <QUrl>
#include <QFile>
#include <QString>
#include <QChar>
#include <QPainter>
#include <QDebug>

const QString UBTPlayer::QSS_PATH(":/res/qss/ubtplayer.qss");

UBTPlayer::UBTPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTPlayer),
    durationStr("00:00"),
    positionStr("00:00")
{
    ui->setupUi(this);
    ui->UBTPlayerPlayPauseBtn->setProperty("playing", false);
    updateDurationLabel();
    ui->UBTPlayerControl->setWindowFlags(Qt::WindowStaysOnTopHint);
    setDefaultStyle();

    player = new QMediaPlayer(this);

    videoWidget = new VideoWidget(this);
    videoWidget->setFixedSize(QSize(696, 440));
    videoWidget->move(0, 0);
    player->setVideoOutput(videoWidget);

    ui->UBTPlayerHeader->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->UBTPlayerHeader->setAutoFillBackground(false);
    ui->UBTPlayerHeader->raise();

    ui->UBTPlayerControl->setAttribute(Qt::WA_TranslucentBackground , true);
    ui->UBTPlayerControl->raise();
    ui->UBTPlayerControl->setAutoFillBackground(false);

    connect(player, &QMediaPlayer::durationChanged, this, &UBTPlayer::onDurationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &UBTPlayer::onPositionChanged);
    connect(player, static_cast<void (QMediaPlayer::*)()>(&QMediaPlayer::metaDataChanged), this, &UBTPlayer::onMetaDataChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &UBTPlayer::onMediaStatusChanged);
    connect(player, &QMediaPlayer::bufferStatusChanged, this, &UBTPlayer::onBufferingProgress);
    connect(player, &QMediaPlayer::videoAvailableChanged, this, &UBTPlayer::onVideoAvailableChanged);
    connect(player, static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error), this, &UBTPlayer::onDisplayErrorMessage);

    connect(ui->UBTPlayerPlayPauseBtn, &QPushButton::clicked, this, &UBTPlayer::onPlayPauseButtonClicked);
    connect(ui->UBTPlayerDurationSlider, &QSlider::sliderMoved, this, &UBTPlayer::seek);
    connect(player, &QMediaPlayer::stateChanged, this, &UBTPlayer::onStateChanged);

    connect(ui->UBTPlayerCloseButton, &QPushButton::clicked, this, &UBTPlayer::onCloseBtnClicked);

    closeBtn = new CloseButton(nullptr);
    closeBtn->move(100, 100);
    closeBtn->raise();
}

UBTPlayer::~UBTPlayer()
{
    delete closeBtn;
    delete ui;
}

void UBTPlayer::setMedia(const QUrl &mediaUrl)
{
    qDebug() << "UBTPlayer::setMedia mediaUrl:" << mediaUrl;
    if (player) {
        durationStr = "00:00";
        positionStr = "00:00";
        QMediaContent mediaContent(mediaUrl);
        if (mediaContent.isNull()){
            qDebug() << "UBTPlayer::setMedia mediaContent is null";
        }
        else {
           qDebug() << "UBTPlayer::setMedia canonical url:" << mediaContent.canonicalUrl();
           player->setMedia(mediaContent);
        }
    }
    else {
        qDebug() << "UBTPlayer::setMedia player is null";
    }
}

void UBTPlayer::onCloseBtnClicked()
{
    if (player) {
        player->stop();
        player->setMedia(QMediaContent());
    }
    hide();
}

void UBTPlayer::onDurationChanged(qint64 duration)
{
    long durationSec = duration / 1000;
    qDebug() << "UBTPlayer::onDurationChanged duration sec:" << durationSec;
    ui->UBTPlayerDurationSlider->setMaximum(durationSec);
    long durationMin = durationSec / 60;
    durationSec = durationSec % 60;
    durationStr = QString("%1:%2").arg(durationMin, 2, 10, QChar('0')).arg(durationSec, 2, 10, QChar('0'));
    updateDurationLabel();
}

void UBTPlayer::onPositionChanged(qint64 position)
{
    if (!ui->UBTPlayerDurationSlider->isSliderDown()) {
        long positionSec = position / 1000;
        qDebug() << "UBTPlayer::onPositionChanged position sec:" << positionSec;
        ui->UBTPlayerDurationSlider->setValue(positionSec);
        long positionMin = positionSec / 60;
        positionSec = positionSec % 60;
        positionStr = QString("%1:%2").arg(positionMin, 2, 10, QChar('0')).arg(positionSec, 2, 10, QChar('0'));
        updateDurationLabel();
    }
}

void UBTPlayer::onMetaDataChanged()
{

}

void UBTPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{

}

void UBTPlayer::onBufferingProgress(int progress)
{

}

void UBTPlayer::onVideoAvailableChanged(bool available)
{

}

void UBTPlayer::onDisplayErrorMessage(QMediaPlayer::Error error)
{
    qDebug() << "UBTPlayer::onDisplayErrorMessage:" << player->errorString() << error;
}

void UBTPlayer::seek(int seconds)
{
    if (player) {
        player->setPosition(seconds * 1000);
    }
}

void UBTPlayer::play()
{
    if (player) {
        qDebug() << "UBTPlayer::play media content url:" << player->media().canonicalUrl();
        player->play();
    }
}

void UBTPlayer::play(const QUrl &mediaUrl)
{
    if (player) {
        durationStr = "00:00";
        positionStr = "00:00";
        QMediaContent mediaContent(mediaUrl);
        if (mediaContent.isNull()){
            qDebug() << "UBTPlayer::play mediaContent is null";
        }
        else {
           qDebug() << "UBTPlayer::play canonical url:" << mediaContent.canonicalUrl();
           player->setMedia(mediaContent);
           player->play();
        }
    }
    else {
        qDebug() << "UBTPlayer::play player is null";
    }
}

void UBTPlayer::onPlayPauseButtonClicked()
{
    if (ui->UBTPlayerPlayPauseBtn->property("playing").toBool()) {
        if (player) {
            qDebug() << "UBTPlayer::onPlayPauseButtonClicked pause media content url:" << player->media().canonicalUrl();
            player->pause();
        }
    }
    else {
        if (player) {
            qDebug() << "UBTPlayer::onPlayPauseButtonClicked play media content url:" << player->media().canonicalUrl();
            player->play();
        }
    }
}

void UBTPlayer::onStateChanged(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;
        switch (state) {
        case QMediaPlayer::StoppedState: {
            ui->UBTPlayerPlayPauseBtn->setProperty("playing", false);
        }
            break;
        case QMediaPlayer::PlayingState: {
            ui->UBTPlayerPlayPauseBtn->setProperty("playing", true);
        }
            break;
        case QMediaPlayer::PausedState: {
            ui->UBTPlayerPlayPauseBtn->setProperty("playing", false);
        }
            break;
        }
        ui->UBTPlayerPlayPauseBtn->style()->unpolish(ui->UBTPlayerPlayPauseBtn);
        ui->UBTPlayerPlayPauseBtn->style()->polish(ui->UBTPlayerPlayPauseBtn);
        ui->UBTPlayerPlayPauseBtn->update();
    }
}

void UBTPlayer::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        auto styleSheetStr = styleSheet.readAll();
        setStyleSheet(styleSheetStr);
    }
    else {
        qDebug() << "UBTPlayer::setDefaultStyle open qss failed";
    }
}

void UBTPlayer::updateDurationLabel()
{
    ui->UBTPlayerDurationLbl->setText(QString("%1/%2").arg(positionStr).arg(durationStr));
}

CloseButton::CloseButton(QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(36, 36);
    setAttribute(Qt::WA_TranslucentBackground , true);
    setAutoFillBackground(false);
}

CloseButton::~CloseButton()
{
}

void CloseButton::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    painter->drawPixmap(rect(), QPixmap(":/res/images/ic_user_normal.png"));
}
