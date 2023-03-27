#ifndef UBTFRAMECAPTURE_H
#define UBTFRAMECAPTURE_H

#include "ubtplayer_global.h"

#include <QObject>
#include <QtMultimedia/QVideoFrame>
#include <QtMultimedia/QMediaPlayer>

class QMediaPlayer;
class QVideoProbe;
class UBTCaptureVideoSurface;

class UBTPLAYERSHARED_EXPORT UBTFrameCapture : public QObject
{
    Q_OBJECT

public:
    UBTFrameCapture(QObject *parent = nullptr);
    ~UBTFrameCapture();

    void setUrlAndCaptureFile(const QMap<QString, QString> url2captureFile);
    void startCapture();
    void stopCapture();

protected:
    void onFrameAvailable(QVideoFrame videoFrame);
    void onDisplayErrorMessage(QMediaPlayer::Error error);

signals:
    void frameCaptured(QString capturingFileName);

private:
    QMap<QString, QString> url2captureFile;
    QPair<QString, QString> capturingUrl2File;

    QMediaPlayer *player;
    UBTCaptureVideoSurface *surface;
};

#endif // UBTFRAMECAPTURE_H
