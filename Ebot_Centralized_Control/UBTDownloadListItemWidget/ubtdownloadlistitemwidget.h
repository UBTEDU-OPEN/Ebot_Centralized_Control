#ifndef UBTDOWNLOADLISTITEMWIDGET_H
#define UBTDOWNLOADLISTITEMWIDGET_H

#include "ubtdownloadlistitemwidget_global.h"

#include <QWidget>
#include <QUrl>

class QJsonObject;

namespace Ui {
class UBTDownloadListItemWidget;
}

class UBTDOWNLOADLISTITEMWIDGETSHARED_EXPORT UBTDownloadListItemWidget : public QWidget
{
    Q_OBJECT

public:
    enum DownloadStatus {
        Undownload,
        Downloading,
        Downloaded,
        DownloadFailed
    };

private:
    static const QString QSS_PATH;

public:
    explicit UBTDownloadListItemWidget(QWidget *parent = 0);
    ~UBTDownloadListItemWidget();

    void asTitle();

    void setName(const QString &name);
    QString getName() const;

    void setId(int id);
    int getId()const;

    void setDownloadStatus(DownloadStatus downloadStatus);
    DownloadStatus getDownloadStatus() const;

    void setDownloadProgress(int progress);
    int getDownloadProgress() const;

    bool isSelected() const;
    void setSelected(bool selected);

    void setFilePath(const QString &filePath);
    QString getFilePath() const;

    void setUrl(QUrl url);
    QUrl getUrl() const;

    void setDefaultStyle();

signals:
    void selected(bool checked);
    void redownloadClicked();

private:
    Ui::UBTDownloadListItemWidget *ui;

    int id;
    QString filePath;
    QUrl url;
    DownloadStatus downloadStatus;
};

#endif // UBTDOWNLOADLISTITEMWIDGET_H
