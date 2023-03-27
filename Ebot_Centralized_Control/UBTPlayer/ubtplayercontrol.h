#ifndef UBTPLAYERCONTROL_H
#define UBTPLAYERCONTROL_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class UBTPlayerControl;
}

class UBTPlayerControl : public QWidget
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

public:
    explicit UBTPlayerControl(QWidget *parent = 0);
    ~UBTPlayerControl();

    void onPlayBtnClicked();
    void setState(QMediaPlayer::State state);
    void setDuration(qint64 duration);
    void onPositionChanged(qint64 position);

    void setDefaultStyle();

signals:
    void play();
    void pause();
    void stop();
    void changeVolume(int volume);
    void changePosition(int position);
    void changeMuting(bool muting);

private:
    void updateDurationLabel();

private:
    Ui::UBTPlayerControl *ui;

    QMediaPlayer::State playerState;
    QString duration;
    QString position;
};

#endif // UBTPLAYERCONTROL_H
