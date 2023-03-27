#ifndef UBTPLAYER_H
#define UBTPLAYER_H

#include "ubtplayer_global.h"

#include <QWidget>
#include <QUrl>
#include <QtMultimedia/QMediaPlayer>
#include <QPushButton>

class QSlider;
class VideoWidget;

class CloseButton;

namespace Ui {
class UBTPlayer;
}

class UBTPLAYERSHARED_EXPORT UBTPlayer : public QWidget
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

public:
    explicit UBTPlayer(QWidget *parent = 0);
    ~UBTPlayer();

    void setMedia(const QUrl &mediaUrl);
    void onCloseBtnClicked();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onMetaDataChanged();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onBufferingProgress(int progress);
    void onVideoAvailableChanged(bool available);
    void onDisplayErrorMessage(QMediaPlayer::Error error);
    void seek(int seconds);
    void play();
    void play(const QUrl &mediaUrl);
    void onPlayPauseButtonClicked();
    void onStateChanged(QMediaPlayer::State state);

protected:
    void setDefaultStyle();
    void updateDurationLabel();

private:
    Ui::UBTPlayer *ui;

    QMediaPlayer *player;
    VideoWidget *videoWidget;

    QMediaPlayer::State playerState;
    QString durationStr;
    QString positionStr;

    CloseButton *closeBtn;
};


class CloseButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CloseButton(QWidget *parent = 0);
    ~CloseButton();

protected:
    void paintEvent(QPaintEvent * event) override;
};

#endif // UBTPLAYER_H
