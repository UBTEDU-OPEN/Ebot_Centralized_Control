#ifndef UBTMYDOWNLOADVEDIOWIDGET_H
#define UBTMYDOWNLOADVEDIOWIDGET_H

#include "ubtmydownload_global.h"

#include <QWidget>
#include <QByteArray>
#include <QUrl>

class UBTFrameCapture;

namespace Ui {
class UBTMyDownloadVideoWidget;
}

class UBTMYDOWNLOADSHARED_EXPORT UBTMyDownloadVideoWidget : public QWidget
{
    Q_OBJECT

public:
    enum VideoType {
        Video = 20,
        Performance = 21,
    };

private:
    static const QString QSS_PATH;

public:
    explicit UBTMyDownloadVideoWidget(QWidget *parent = 0);
    ~UBTMyDownloadVideoWidget();

    void updateContent();
    void updateVideoOrPerformanceContent(VideoType videoType);

protected:
    void setDefaultStyle();

    void setVideoSearchLineEditorExpand(bool expand);
    void setPerformanceSearchLineEditorExpand(bool expand);

    void onGetVideoListReply(bool succ, const QString &info, const QByteArray &reply);
    void onGetPerformanceRepley(bool succ, const QString &info, const QByteArray &reply);
    void onMoreVideoClicked();
    void onMorePerformanceClicked();
    void onVideoSearch();
    void onPerformanceSearch();

signals:
    void videoDownloadStart(int id, QString name, QUrl url, QString filePath);
    void playOnlineVideo(QUrl videoUrl);

private:
    Ui::UBTMyDownloadVideoWidget *ui;

    QByteArray videoList;
    QByteArray performanceList;

//    UBTFrameCapture *frameCapture;
};

#endif // UBTMYDOWNLOADVEDIOWIDGET_H
