#ifndef UBTINFODLGWITHICON_H
#define UBTINFODLGWITHICON_H

#include "ubtwidget_global.h"

#include <QDialog>

class QTimer;

namespace Ui {
class UBTInfoDlg;
}

class UBTDLGSHARED_EXPORT UBTInfoDlg : public QDialog
{
    Q_OBJECT

public:
    enum Icon {
        NoIcon,
        SuccessIcon,
        FailureIcon
    };

private:
    static const QString QSS_PATH;

public:
    explicit UBTInfoDlg(Icon icon, const QString&info, QWidget *parent = 0);
    ~UBTInfoDlg();

protected:
    void setDefaultStyle();

private:
    Ui::UBTInfoDlg *ui;

    QTimer *closeTimer;
};

#endif // UBTINFODLGWITHICON_H
