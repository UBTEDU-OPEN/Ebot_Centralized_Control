#ifndef UBTCAPTUREVIDEOSURFACE_H
#define UBTCAPTUREVIDEOSURFACE_H

#include <QtMultimedia/QAbstractVideoSurface>

class UBTCaptureVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    UBTCaptureVideoSurface(QObject *parent = nullptr);
    ~UBTCaptureVideoSurface();

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    bool present(const QVideoFrame &frame) override;

signals:
    void frameAvailable(QVideoFrame &frame);
};

#endif // UBTCAPTUREVIDEOSURFACE_H
