#ifndef UBTPRIVACTYDLG_H
#define UBTPRIVACTYDLG_H

#include <QDialog>

namespace Ui {
class UBTPrivactyDlg;
}

class UBTPrivactyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UBTPrivactyDlg(QWidget *parent = 0);
    ~UBTPrivactyDlg();

private:
    Ui::UBTPrivactyDlg *ui;
};

#endif // UBTPRIVACTYDLG_H
