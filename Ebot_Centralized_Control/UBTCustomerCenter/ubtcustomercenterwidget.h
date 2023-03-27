#ifndef UBTCUSTOMERCENTERWIDGET_H
#define UBTCUSTOMERCENTERWIDGET_H
#include "ubtcustomercenter_global.h"

#include <QWidget>
#include <QStyledItemDelegate>
#include <QList>

class UBTDownloadListItemWidget;

namespace Ui {
class UBTCustomerCenterWidget;
}

class UBTCUSTOMERCENTERSHARED_EXPORT UBTCustomerCenterWidget : public QWidget
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

public:
    explicit UBTCustomerCenterWidget(QWidget *parent = 0);
    ~UBTCustomerCenterWidget();

protected:
    void setDefaultStyle();
    void updateActPath();

    void onDeactivateClicked();
    void onDownloadPathModifyBtnClicked();
    void onLoginSubmitClicked();
    void onRetrieveMobileGetVCodeClicked();
    void onRetrieveMobileSubmitClicked();
    void onRetrieveMobileShowPwdClicked();
    void onRetrieveEMailGetVCodeClicked();
    void onRetrieveEMailSubmitClicked();
    void onRetrieveEMailShowPwdClicked();
    void onRegisterMobileGetVCodeClicked();
    void onMobilePrivacyAgreementClicked();
    void onRegisterMobileSubmitClicked();
    void onRegisterMobileShowPwdClicked();
    void onRegisterEMailGetVCodeClicked();
    void onEmailPrivacyAgreementClicked();
    void onRegisterEMailSubmitClicked();
    void onRegisterEMailShowPwdClicked();
    void onRegisterNewMobileAccountReply(bool succ, const QString &info, const QByteArray &reply);
    void onRegisterNewEMailAccountReply(bool succ, const QString &info, const QByteArray &reply);
    void onLoginReply(bool succ, const QString &info, const QByteArray &reply);
    void onLogoutReply();
    void onDeactivateReply(bool succ, const QString &info, const QByteArray &reply);
    void updateActionDownloadHistoryList(const QByteArray &actionList);
    void onGetActionDownloadHistoryReply(bool succ, const QString &info, const QByteArray &reply);
    void updateDownloadRecordDelBtnStatus(bool enable);
    void onDeleteDownloadRecordClicked();
    void onClearActionDownloadRecordReply(bool succ, const QString &info, const QByteArray &reply);
    void onModifyPasswordReply(bool succ, const QString &info, const QByteArray &reply);
    void onGetMobileVerificationCodeReply(bool succ, const QString &info, const QByteArray &reply);
    void onGetEMailVerificationCodeReply(bool succ, const QString &info, const QByteArray &reply);

    void updateLoginSubmit();
    void updateRetrieveMobileGetVCode();
    void updateRetrieveMobileSubmit();
    void updateRetrieveEMailGetVCode();
    void updateRetrieveEMailSubmit();
    void updateRegisterMobileGetVCode();
    void updateRegisterMobileSubmit();
    void updateRegisterEMailGetVCode();
    void updateRegisterEMailSubmit();

    void resetAllPages();
    void resetLoginPage(bool remainAccount = false, const QString &newAccount = "");
    void resetRegisterNewMobileAccountPage(bool remainAccount = false, const QString &newAccount = "");
    void resetRegisterNewEMailAccountPage(bool remainAccount = false, const QString &newAccount = "");
    void resetRetrieveMobilePage(bool remainAccount = false, const QString &newAccount = "");
    void resetRetrieveEMailPage(bool remainAccount = false, const QString &newAccount = "");

    void initialTimer();
    void setTabOrder();

private:
    Ui::UBTCustomerCenterWidget *ui;

    UBTDownloadListItemWidget *listHeader;
    QList<int> itemsToDel;

    QTimer *retrieveMobileGetVCodeTimer;
    int retrieveMobileGetVCodeCounter;

    QTimer *retrieveEMailGetVCodeTimer;
    int retrieveEMailGetVCodeCounter;

    QTimer *registerMobileGetVCodeTimer;
    int registerMobileGetVCodeCounter;

    QTimer *registerEMailGetVCodeTimer;
    int registerEMailGetVCodeCounter;
};

#endif // UBTCUSTOMERCENTERWIDGET_H
