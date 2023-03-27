#ifndef UBTMYDOWNLOADHISTORYWIDGET_H
#define UBTMYDOWNLOADHISTORYWIDGET_H
#include "ubtmydownload_global.h"
#include "../UBTServerCommunication/ubtservercommunication.h"

#include <QWidget>
#include <QUrl>

class UBTDownloadListItemWidget;

namespace Ui {
class UBTMyDownloadHistoryWidget;
}

class UBTMYDOWNLOADSHARED_EXPORT UBTMyDownloadHistoryWidget : public QWidget
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

    static const QString JSON_KEY_ID;
    static const QString JSON_KEY_NAME;
    static const QString JSON_KEY_DOWNLOAD_STATUS;
    static const QString JSON_KEY_URL;
    static const QString JSON_KEY_FILE_PATH;

public:
    explicit UBTMyDownloadHistoryWidget(QWidget *parent = 0);
    ~UBTMyDownloadHistoryWidget();

    void updateContent();

    void onVideoDownloadStart(int id, QString name, QUrl url, QString filePath);
    void onVideoDownloadProgress(int id, QString filePath, qint64 bytesReceived, qint64 bytesTotal);
    void onVideoDownloadFinished(int id, QString filePath, bool succ);
    void onRedownloadClicked();

protected:
    void setDefaultStyle();
    void loadVideoPath();
    void loadListContent();
    void saveListContent();
    void updateListArea();
    void updateDelBtnStatus();
    void onDeleteClicked();
    UBTDownloadListItemWidget *getTargetItemWidget(int id, QString filePath);

private:
    Ui::UBTMyDownloadHistoryWidget *ui;

    UBTDownloadListItemWidget *listHeader;
};

#endif // UBTMYDOWNLOADHISTORYWIDGET_H
