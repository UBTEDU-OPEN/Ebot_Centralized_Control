#ifndef UBTACTIONWIDGET_H
#define UBTACTIONWIDGET_H

#include <QWidget>
#include <QUrl>

class QRoundProgressBar;

namespace Ui {
class UBTActionWidget;
}

const QString ACTION_WIDGET_DEFAULT_QSS_PATH = ":/res/qss/actionWidget.qss";

class UBTActionWidget : public QWidget
{
    Q_OBJECT

private:
    enum DownloadStatus {
        Undownload,
        Downloading,
        Downloaded,
        Downloadfailed
    };

public:
    explicit UBTActionWidget(QWidget *parent = 0);
    ~UBTActionWidget();

    void setActionInfo(const QJsonObject &actionInfo);

    void onActionDownloadProgress(int id, qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadActionFinished(int id, bool succ);
    void onDownloadActionThumbnailFinished(int id, bool succ);

protected:
    void setDefaultStyle();
    void startDownloadAction();
    void startDownloadActionThumbnailUrl();
    QString thumbnailFileFullLocalName() const;
    void updateThumbnail();
    void setDownloadStatus(DownloadStatus status);
    void initDownloadProgress();

signals:
    void actionDownloadFinished(const QString &actionName);
    void actionDownloadFailed(const QString &actionName);

private:
    Ui::UBTActionWidget *ui;
    QRoundProgressBar *downloadProgress;

    QUrl thumbnailUrl;
    QUrl actionUrl;
    QString destActFileName;
    bool actionDownloadFlag;
    int actionId;

//    static QString lastDownload;
};

#endif // UBTACTIONWIDGET_H
