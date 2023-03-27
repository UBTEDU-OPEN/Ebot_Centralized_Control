#ifndef UBTCONFIRMDLG_H
#define UBTCONFIRMDLG_H

#include "ubtwidget_global.h"

#include <QWidget>

namespace Ui {
class UBTConfirmDlg;
}

class UBTDLGSHARED_EXPORT UBTConfirmDlg : public QWidget
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

public:
    explicit UBTConfirmDlg(const QString &title, const QString &content, const QString &acceptText, const QString &rejectText, QWidget *parent = 0);
    ~UBTConfirmDlg();

    void showInCenter();

    static void setBgWidget(QWidget *w) { bgWidget = w;}

protected:
    void setDefaultStyle();

    void paintEvent(QPaintEvent * event) override;

signals:
    void acceptClicked();
    void rejectClicked();

private:
    Ui::UBTConfirmDlg *ui;

    static QWidget *bgWidget;
};

#endif // UBTCONFIRMDLG_H
