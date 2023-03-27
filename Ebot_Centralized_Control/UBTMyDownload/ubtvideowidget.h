#ifndef UBTVIDEOWIDGET_H
#define UBTVIDEOWIDGET_H

#include <QWidget>
#include <QUrl>

class QRoundProgressBar;

namespace Ui {
class UBTVideoWidget;
}

class UBTVideoWidget : public QWidget
{
    Q_OBJECT

private:
    const QString VIDEO_WIDGET_DEFAULT_QSS_PATH;
    enum DownloadStatus {
        Unknown,
        Downloading,
        CanDownload
    };

public:
    explicit UBTVideoWidget(QWidget *parent = 0);
    ~UBTVideoWidget();

    void setVideoInfo(const QJsonObject &json);
    QString getThumbnailFullLocalName() const;

protected:
    void setDefaultStyle();
    void initDownloadProgress();

    void onVideoDownloadProgress(int id, QString dstFile, qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadVideoFinished(int id, QString dstFile, bool succ);
    void onDownloadVideoThumbnailFinished(int id, bool succ);
    void startDownloadVideo();
    void startDownloadVideoThumbnail();
    void updateThumbnail();
    void onPlayBtnClicked();
    void setDownloadStatus(DownloadStatus downloadStatus);
    void onDownloadStatusChanged();

signals:
    void videoDownloadStart(int id, QString name, QUrl url, QString filePath);
    void playOnlineVideo(QUrl videoUrl);

private:
    Ui::UBTVideoWidget *ui;

    int videoId;
    QUrl videoUrl;
    QUrl thumbnailUrl;
    QString destVideoFileName;
    DownloadStatus mDownloadStatus;

    QRoundProgressBar *downloadProgress;
};

#endif // UBTVIDEOWIDGET_H
