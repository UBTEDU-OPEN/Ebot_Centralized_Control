#ifndef UBTPRIVACYAGREEMENTDLG_H
#define UBTPRIVACYAGREEMENTDLG_H

#include <QDialog>

namespace Ui {
class UBTPrivacyAgreementDlg;
}

class UBTPrivacyAgreementDlg : public QDialog
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

public:
    explicit UBTPrivacyAgreementDlg(QWidget *parent = 0);
    ~UBTPrivacyAgreementDlg();

    void onCloseBtnClicked();

protected:
    void setDefaultStyle();

private:
    Ui::UBTPrivacyAgreementDlg *ui;
};

#endif // UBTPRIVACYAGREEMENTDLG_H
