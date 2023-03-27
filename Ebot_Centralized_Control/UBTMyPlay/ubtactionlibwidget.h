#ifndef UBTACTIONLIBWIDGET_H
#define UBTACTIONLIBWIDGET_H

#include "ubtmyplay_global.h"

#include <QWidget>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QList>

class QHBoxLayout;

namespace Ui {
class UbtActionLibWidget;
}

const QString ActionLib_WIDGET_DEFAULT_QSS_PATH = ":/res/qss/actionLib.qss";

class UBTMYPLAYSHARED_EXPORT UbtActionLibWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UbtActionLibWidget(QWidget *parent = 0);
    ~UbtActionLibWidget();

    void updateContent();

protected:
    void SetDefaultStyle();
    void setSearchLineEditorExpand(bool expand);

    void onSearch();
    void onAddActionDownloadRecordReply(int httpStatusCode, int error);
    void onClearActionDownloadRecordReply(bool succ, QString info);
    void onGetActionListReply(bool succ, QString info, QByteArray reply);
    void onLoginClicked();

signals:
    void actionDownloadFinished(const QString &actionName);
    void actionDownloadFailed(const QString &actionName);
    void loginClicked();

private:
    Ui::UbtActionLibWidget *ui;

    QList<QHBoxLayout*> actionLineLayouts;
};

#endif // UBTACTIONLIBWIDGET_H
