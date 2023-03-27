#include "ubtcustomercenterwidget.h"
#include "ui_ubtcustomercenterwidget.h"

#include "../UBTServerCommunication/ubtservercommunication.h"
#include "../UBTUtil/configs.h"
#include "../UBTDownloadListItemWidget/ubtdownloadlistitemwidget.h"
#include "ubtprivacyagreementdlg.h"
#include "../UBTUtil/ubtutil.h"
#include "../UBTWidget/UBTInfoDlg.h"
#include "../UBTWidget/ubtconfirmdlg.h"
#include "../UBTUtil/filedirhandle.h"

#include <QFile>
#include <QStandardItemModel>
#include <QPainter>
#include <QFileDialog>
#include <QProgressBar>
#include <QListWidgetItem>
#include <QListWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QTimer>
#include <qDebug>

const QString UBTCustomerCenterWidget::QSS_PATH(":/res/qss/ubtcustomercenter.qss");

UBTCustomerCenterWidget::UBTCustomerCenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTCustomerCenterWidget),
    retrieveMobileGetVCodeTimer(nullptr),
    retrieveEMailGetVCodeTimer(nullptr),
    registerMobileGetVCodeTimer(nullptr),
    registerEMailGetVCodeTimer(nullptr)
{
    itemsToDel.clear();
    initialTimer();

    ui->setupUi(this);
    setTabOrder();
    ui->downloadRecordFoot->hide();
    ui->downloadRecordDelBtn->setEnabled(false);
    listHeader = new UBTDownloadListItemWidget;
    listHeader->setName(tr("name"));
    listHeader->asTitle();
    ui->downloadRecordContainerLayout->insertWidget(1, listHeader);
    ui->retrieveMobileShowPwd->setProperty("showPwd", false);
    ui->retrieveEMailShowPwd->setProperty("showPwd", false);
    ui->registerMobileShowPwd->setProperty("showPwd", false);
    ui->registerEMailShowPwd->setProperty("showPwd", false);
    ui->actionDownloadHistoryListWidget->setAlternatingRowColors(true);

    ui->loginSubmit->setEnabled(false);
    ui->registerMobileGetVCode->setEnabled(false);
    ui->registerMobileSubmit->setEnabled(false);
    ui->registerEMailGetVCode->setEnabled(false);
    ui->registerEMailSubmit->setEnabled(false);
    ui->retrieveMobileGetVCode->setEnabled(false);
    ui->retrieveMobileSubmit->setEnabled(false);
    ui->retrieveEMailGetVCode->setEnabled(false);
    ui->retrieveEMailSubmit->setEnabled(false);

    ui->loginAccount->setPlaceholderText(tr("mobile phone number or e-mail"));
    ui->loginAccount->setPlaceholderTextColor(QColor(178, 185, 192, 128));

    // for test:
//    ui->loginAccount->setText("17705140216");
//    ui->loginPwd->setText("123456");
//    ui->loginSubmit->setEnabled(true);

    setDefaultStyle();
    updateActPath();

    connect(ui->downloadPathModifyBtn, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onDownloadPathModifyBtnClicked);

    // log in page
    connect(ui->loginAccount, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateLoginSubmit);
    connect(ui->loginPwd, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateLoginSubmit);
    connect(ui->loginToRegister, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->registerPage);
    });
    connect(ui->loginForgetPwd, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->retrievePage);
    });
    connect(ui->loginSubmit, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onLoginSubmitClicked);

    // modify password by mobile page
    connect(ui->retrieveMobileAccount, &QLineEdit::textChanged, this, [this]() {
        updateRetrieveMobileGetVCode();
        updateRetrieveMobileSubmit();
    });
    connect(ui->retrieveMobileVCode, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRetrieveMobileSubmit);
    connect(ui->retrieveMobilePwd, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRetrieveMobileSubmit);
    connect(ui->retrieveMobileRetLogin, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    });
    connect(ui->retrieveMobileGetVCode, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRetrieveMobileGetVCodeClicked);
    connect(ui->retrieveMobileSubmit, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRetrieveMobileSubmitClicked);
    connect(ui->retrieveMobileShowPwd, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRetrieveMobileShowPwdClicked);

    // modify password by email page
    connect(ui->retrieveEMailAccount, &QLineEdit::textChanged, this, [this]() {
        updateRetrieveEMailGetVCode();
        updateRetrieveEMailSubmit();
    });
    connect(ui->retrieveEMailVCode, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRetrieveEMailSubmit);
    connect(ui->retrieveEMailPwd, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRetrieveEMailSubmit);
    connect(ui->retrieveEMailRetLogin, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    });
    connect(ui->retrieveEMailGetVCode, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRetrieveEMailGetVCodeClicked);
    connect(ui->retrieveEMailSubmit, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRetrieveEMailSubmitClicked);
    connect(ui->retrieveEMailShowPwd, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRetrieveEMailShowPwdClicked);

    // register by mobile page
    connect(ui->registerMobileAccount, &QLineEdit::textChanged, this, [this]() {
        updateRegisterMobileGetVCode();
        updateRegisterMobileSubmit();
    });
    connect(ui->registerMobileVCode, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRegisterMobileSubmit);
    connect(ui->registerMobilePwd, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRegisterMobileSubmit);
    connect(ui->registerMobileCheck, &QCheckBox::stateChanged, this, &UBTCustomerCenterWidget::updateRegisterMobileSubmit);
    connect(ui->registerMobileRetLogin, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    });
    connect(ui->registerMobileGetVCode, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRegisterMobileGetVCodeClicked);
    connect(ui->mobilePrivacyAgreement, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onMobilePrivacyAgreementClicked);
    connect(ui->registerMobileSubmit, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRegisterMobileSubmitClicked);
    connect(ui->registerMobileShowPwd, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRegisterMobileShowPwdClicked);

    // register by email page
    connect(ui->registerEMailAccount, &QLineEdit::textChanged, this, [this]() {
        updateRegisterEMailGetVCode();
        updateRegisterEMailSubmit();
    });
    connect(ui->registerEMailVCode, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRegisterEMailSubmit);
    connect(ui->registerEMailPwd, &QLineEdit::textChanged, this, &UBTCustomerCenterWidget::updateRegisterEMailSubmit);
    connect(ui->registerEMailCheck, &QCheckBox::stateChanged, this, &UBTCustomerCenterWidget::updateRegisterEMailSubmit);
    connect(ui->registerEMailRetLogin, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    });
    connect(ui->registerEMailGetVCode, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRegisterEMailGetVCodeClicked);
    connect(ui->emailPrivacyAgreement, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onEmailPrivacyAgreementClicked);
    connect(ui->registerEMailSubmit, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRegisterEMailSubmitClicked);
    connect(ui->registerEMailShowPwd, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onRegisterEMailShowPwdClicked);

    // logout
    connect(ui->logoutBtn, &QPushButton::clicked, this, [this]() {
        UBTServerCommunication::getInstance()->logout();
    });

    // deactivate
    connect(ui->deactivate, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onDeactivateClicked);

    // delete action download record
    connect(ui->downloadRecordDelBtn, &QPushButton::clicked, this, &UBTCustomerCenterWidget::onDeleteDownloadRecordClicked);
    // reply connection:
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::registerNewMobileAccountReply, this, &UBTCustomerCenterWidget::onRegisterNewMobileAccountReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::registerNewEMailAccountReply, this, &UBTCustomerCenterWidget::onRegisterNewEMailAccountReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::loginReply, this, &UBTCustomerCenterWidget::onLoginReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::logoutReply, this, &UBTCustomerCenterWidget::onLogoutReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::deactivateReply, this, &UBTCustomerCenterWidget::onDeactivateReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::getActionDownloadHistoryReply, this, &UBTCustomerCenterWidget::onGetActionDownloadHistoryReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::clearActionDownloadRecordReply, this, &UBTCustomerCenterWidget::onClearActionDownloadRecordReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::modifyPasswordReply, this, &UBTCustomerCenterWidget::onModifyPasswordReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::getMobileVerificationCodeReply, this, &UBTCustomerCenterWidget::onGetMobileVerificationCodeReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::getEMailVerificationCodeReply, this, &UBTCustomerCenterWidget::onGetEMailVerificationCodeReply);

}

UBTCustomerCenterWidget::~UBTCustomerCenterWidget()
{
    delete ui;
}

void UBTCustomerCenterWidget::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
    else {
        qDebug() << "UBTCustomerCenterWidget::setDefaultStyle open qss failed";
    }
}

void UBTCustomerCenterWidget::updateActPath()
{
    ui->downloadPathModifyLineEdit->setText(CConfigs::instance()->getLocalActLibDir());
}

void UBTCustomerCenterWidget::onDeactivateClicked()
{
    UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("are your sure to deactivate account?"), tr("yes"), tr("no"));
    connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
        UBTServerCommunication::getInstance()->deactivate();
        dlg->close();
    });
    connect(dlg, &UBTConfirmDlg::rejectClicked, this, [this, dlg]() {
        dlg->close();
    });
    dlg->showInCenter();
}

void UBTCustomerCenterWidget::onDownloadPathModifyBtnClicked()
{
    QString currentDir;
    if (!CConfigs::instance()->getLocalActLibDir(currentDir)) {
        currentDir.clear();
    }
    QString newDir = QFileDialog::getExistingDirectory(this, tr("select action library directory"),
                                                    currentDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    CConfigs::instance()->setLocalActionLibDir(newDir);
    updateActPath();
}

void UBTCustomerCenterWidget::onLoginSubmitClicked()
{
    auto account = ui->loginAccount->text();
    auto pwd = ui->loginPwd->text();
    ui->loginPwd->clear();
    UBTServerCommunication::getInstance()->login(account, pwd);
}

void UBTCustomerCenterWidget::onRetrieveMobileGetVCodeClicked()
{
    if (retrieveMobileGetVCodeTimer->isActive()) {
        return;
    }
    retrieveMobileGetVCodeCounter = 60;
    retrieveMobileGetVCodeTimer->start();

    auto account = ui->retrieveMobileAccount->text();
    UBTServerCommunication::getInstance()->getMobileVerificationCode(account);
}

void UBTCustomerCenterWidget::onRetrieveMobileSubmitClicked()
{
    auto account = ui->retrieveMobileAccount->text();
    auto vcode = ui->retrieveMobileVCode->text();
    auto newPwd = ui->retrieveMobilePwd->text();

    UBTServerCommunication::getInstance()->modifyPassword(account, vcode, newPwd);
}

void UBTCustomerCenterWidget::onRetrieveMobileShowPwdClicked()
{
    if (ui->retrieveMobilePwd->echoMode() == QLineEdit::Password) {
        ui->retrieveMobilePwd->setEchoMode(QLineEdit::Normal);
        ui->retrieveMobileShowPwd->setProperty("showPwd", true);
    }
    else {
        ui->retrieveMobilePwd->setEchoMode(QLineEdit::Password);
        ui->retrieveMobileShowPwd->setProperty("showPwd", false);
    }
    ui->retrieveMobileShowPwd->style()->unpolish(ui->retrieveMobileShowPwd);
    ui->retrieveMobileShowPwd->style()->polish(ui->retrieveMobileShowPwd);
    ui->retrieveMobileShowPwd->update();
}

void UBTCustomerCenterWidget::onRetrieveEMailGetVCodeClicked()
{
    if (retrieveEMailGetVCodeTimer->isActive()) {
        return;
    }
    retrieveEMailGetVCodeCounter = 60;
    retrieveEMailGetVCodeTimer->start();

    auto email = ui->retrieveEMailAccount->text();
    UBTServerCommunication::getInstance()->getEMailVerificationCode(email);
}

void UBTCustomerCenterWidget::onRetrieveEMailSubmitClicked()
{
    auto account = ui->retrieveEMailAccount->text();
    auto vcode = ui->retrieveEMailVCode->text();
    auto newPwd = ui->retrieveEMailPwd->text();

    UBTServerCommunication::getInstance()->modifyPassword(account, vcode, newPwd);
}

void UBTCustomerCenterWidget::onRetrieveEMailShowPwdClicked()
{
    if (ui->retrieveEMailPwd->echoMode() == QLineEdit::Password) {
        ui->retrieveEMailPwd->setEchoMode(QLineEdit::Normal);
        ui->retrieveEMailShowPwd->setProperty("showPwd", true);
    }
    else {
        ui->retrieveMobilePwd->setEchoMode(QLineEdit::Password);
        ui->retrieveEMailShowPwd->setProperty("showPwd", false);
    }
    ui->retrieveEMailShowPwd->style()->unpolish(ui->retrieveEMailShowPwd);
    ui->retrieveEMailShowPwd->style()->polish(ui->retrieveEMailShowPwd);
    ui->retrieveEMailShowPwd->update();
}

void UBTCustomerCenterWidget::onRegisterMobileGetVCodeClicked()
{
    if (registerMobileGetVCodeTimer->isActive()) {
        return;
    }
    registerMobileGetVCodeCounter = 60;
    registerMobileGetVCodeTimer->start();

    auto mobileNumber = ui->registerMobileAccount->text();

    UBTServerCommunication::getInstance()->getMobileVerificationCode(mobileNumber);
}

void UBTCustomerCenterWidget::onMobilePrivacyAgreementClicked()
{
    auto privacyAgreementDlg = new UBTPrivacyAgreementDlg(this);
    privacyAgreementDlg->show();
    privacyAgreementDlg->move((this->width() - privacyAgreementDlg->width()) / 2, (this->height() - privacyAgreementDlg->height()) / 2);
}

void UBTCustomerCenterWidget::onRegisterMobileSubmitClicked()
{
    if (!ui->registerMobileCheck->isChecked()) {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("must agree with privacy agreement"), this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
        return;
    }
    auto mobileNum = ui->registerMobileAccount->text();
    auto vcode = ui->registerMobileVCode->text();
    auto pwd = ui->registerMobilePwd->text();

    UBTServerCommunication::getInstance()->registerNewMobileAccount(mobileNum, vcode, pwd);
}

void UBTCustomerCenterWidget::onRegisterMobileShowPwdClicked()
{
    if (ui->registerMobilePwd->echoMode() == QLineEdit::Password) {
        ui->registerMobilePwd->setEchoMode(QLineEdit::Normal);
        ui->registerMobileShowPwd->setProperty("showPwd", true);
    }
    else {
        ui->registerMobilePwd->setEchoMode(QLineEdit::Password);
        ui->registerMobileShowPwd->setProperty("showPwd", false);
    }
    ui->registerMobileShowPwd->style()->unpolish(ui->registerMobileShowPwd);
    ui->registerMobileShowPwd->style()->polish(ui->registerMobileShowPwd);
    ui->registerMobileShowPwd->update();
}

void UBTCustomerCenterWidget::onRegisterEMailGetVCodeClicked()
{
    if (registerEMailGetVCodeTimer->isActive()) {
        return;
    }
    registerEMailGetVCodeCounter = 60;
    registerEMailGetVCodeTimer->start();

    auto email = ui->registerEMailAccount->text();

    UBTServerCommunication::getInstance()->getEMailVerificationCode(email);
}

void UBTCustomerCenterWidget::onEmailPrivacyAgreementClicked()
{
    auto privacyAgreementDlg = new UBTPrivacyAgreementDlg(this);
    privacyAgreementDlg->show();
    privacyAgreementDlg->move((this->width() - privacyAgreementDlg->width()) / 2, (this->height() - privacyAgreementDlg->height()) / 2);
}

void UBTCustomerCenterWidget::onRegisterEMailSubmitClicked()
{
    if (!ui->registerEMailCheck->isChecked()) {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("must agree with privacy agreement"), this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
        return;
    }
    auto email = ui->registerEMailAccount->text();
    auto vcode = ui->registerEMailVCode->text();
    auto pwd = ui->registerEMailPwd->text();

    UBTServerCommunication::getInstance()->registerNewEMailAccount(email, vcode, pwd);
}

void UBTCustomerCenterWidget::onRegisterEMailShowPwdClicked()
{
    if (ui->registerEMailPwd->echoMode() == QLineEdit::Password) {
        ui->registerEMailPwd->setEchoMode(QLineEdit::Normal);
        ui->registerEMailShowPwd->setProperty("showPwd", true);
    }
    else {
        ui->registerEMailPwd->setEchoMode(QLineEdit::Password);
        ui->registerEMailShowPwd->setProperty("showPwd", false);
    }
    ui->registerEMailShowPwd->style()->unpolish(ui->registerEMailShowPwd);
    ui->registerEMailShowPwd->style()->polish(ui->registerEMailShowPwd);
    ui->registerEMailShowPwd->update();
}

void UBTCustomerCenterWidget::onRegisterNewMobileAccountReply(bool succ, const QString &info, const QByteArray &reply)
{
    UBTInfoDlg *dlg = nullptr;
    if (succ) {
        dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("register success"), this);
        resetLoginPage(false, ui->registerMobileAccount->text());
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
        resetRegisterNewMobileAccountPage();
    }
    else {
        resetRegisterNewMobileAccountPage(true);
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
    }
    dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    dlg->show();
}

void UBTCustomerCenterWidget::onRegisterNewEMailAccountReply(bool succ, const QString &info, const QByteArray &reply)
{
    UBTInfoDlg *dlg = nullptr;
    if (succ) {
        dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("register success"), this);
        resetLoginPage(false, ui->registerEMailAccount->text());
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
        resetRegisterNewEMailAccountPage();
    }
    else {
        resetRegisterNewEMailAccountPage(true);
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();

        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
    }
    dlg->show();
    dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
}

void UBTCustomerCenterWidget::onLoginReply(bool succ, const QString &info, const QByteArray &reply)
{
    if (succ) {
        UBTServerCommunication::getInstance()->getActionDownloadHistory();
        ui->stackedWidget->setCurrentWidget(ui->loginSuccPage);
        ui->accountLbl->setText(tr("account:") + UBTServerCommunication::getInstance()->loginAccount());
        resetAllPages();
    }
    else {
        resetLoginPage(true);
        UBTInfoDlg *dlg;
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
//            UBTInfoDlgWithoutIcon::showNoIconDlg(replyJson.value("message").toString());
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
//            UBTInfoDlgWithoutIcon::showNoIconDlg(info);
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
}

void UBTCustomerCenterWidget::onLogoutReply()
{
    resetAllPages();
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

void UBTCustomerCenterWidget::onDeactivateReply(bool succ, const QString &info, const QByteArray &reply)
{
    resetAllPages();
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    if (!succ) {
        UBTInfoDlg *dlg = nullptr;
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
}

void UBTCustomerCenterWidget::updateActionDownloadHistoryList(const QByteArray &actionList)
{
    ui->actionDownloadHistoryListWidget->clear();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(actionList);
    QJsonArray actionsJsonArray = jsonDoc.array();
    int actionsCount = actionsJsonArray.count();

    for (int i = 0; i < actionsCount; ++i) {
        if (actionsJsonArray[i].isObject()) {
            QJsonObject actionJsonObj = actionsJsonArray[i].toObject();
            auto actionDownloadFlag = actionJsonObj.value("flag").toInt();
            if (actionDownloadFlag) {
                auto item = new QListWidgetItem;
                ui->actionDownloadHistoryListWidget->addItem(item);
                UBTDownloadListItemWidget *widget = new UBTDownloadListItemWidget;
                widget->setName(actionJsonObj.value("name").toString());
                widget->setDownloadStatus(UBTDownloadListItemWidget::Downloaded);
                widget->setId(actionJsonObj.value("id").toInt());
                ui->actionDownloadHistoryListWidget->setItemWidget(item, widget);
                connect(listHeader, &UBTDownloadListItemWidget::selected, widget, &UBTDownloadListItemWidget::setSelected);
                connect(widget, &UBTDownloadListItemWidget::selected, this, &UBTCustomerCenterWidget::updateDownloadRecordDelBtnStatus);
            }
        }
        else {
            qDebug() << "UBTCustomerCenterWidget::updateActionDownloadHistoryList" << i << "value in array is not object";
        }
    }
    updateDownloadRecordDelBtnStatus(false);
    listHeader->setSelected(listHeader->isSelected() && ui->actionDownloadHistoryListWidget->count() > 0);
}

void UBTCustomerCenterWidget::onGetActionDownloadHistoryReply(bool succ, const QString &info, const QByteArray &reply)
{
    updateActionDownloadHistoryList(reply);
}

void UBTCustomerCenterWidget::updateDownloadRecordDelBtnStatus(bool enable)
{
    if (enable) {
        ui->downloadRecordDelBtn->setEnabled(true);
        return;
    }
    for (int i = 0; i < ui->actionDownloadHistoryListWidget->count(); ++i) {
        auto item = ui->actionDownloadHistoryListWidget->item(i);
        if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->actionDownloadHistoryListWidget->itemWidget(item))) {
            if (widget->isSelected()) {
                ui->downloadRecordDelBtn->setEnabled(true);
                return;
            }
        }
    }
    ui->downloadRecordDelBtn->setEnabled(false);
}

void UBTCustomerCenterWidget::onDeleteDownloadRecordClicked()
{
    QList<int> actionIds;
    itemsToDel.clear();
    for (int i = 0; i < ui->actionDownloadHistoryListWidget->count(); ++i) {
        auto item = ui->actionDownloadHistoryListWidget->item(i);
        if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->actionDownloadHistoryListWidget->itemWidget(item))) {
            if (widget->isSelected()) {
                // remove this record
                actionIds.append(widget->getId());
                itemsToDel.append(i);
            }
        }
    }
    UBTServerCommunication::getInstance()->clearActionDownloadRecord(actionIds);
}

void UBTCustomerCenterWidget::onClearActionDownloadRecordReply(bool succ, const QString &info, const QByteArray &reply)
{
    if (succ) {
        while (itemsToDel.count() > 0) {
            auto itemToDel = ui->actionDownloadHistoryListWidget->takeItem(itemsToDel.takeLast());
            delete itemToDel;
        }
    }
    else {
        itemsToDel.clear();
        UBTInfoDlg *dlg = nullptr;
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
    updateDownloadRecordDelBtnStatus(false);
    listHeader->setSelected(false);
}

void UBTCustomerCenterWidget::onModifyPasswordReply(bool succ, const QString &info, const QByteArray &reply)
{
    UBTInfoDlg *dlg = nullptr;
    if (succ) {
        dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("password modify success"), this);
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    }
    else {
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
    }
    dlg->show();
    dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
}

void UBTCustomerCenterWidget::onGetMobileVerificationCodeReply(bool succ, const QString &info, const QByteArray &reply)
{
    if (!succ) {
        resetRegisterNewMobileAccountPage(true);
        resetRetrieveMobilePage(true);
        UBTInfoDlg *dlg = nullptr;
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
}

void UBTCustomerCenterWidget::onGetEMailVerificationCodeReply(bool succ, const QString &info, const QByteArray &reply)
{
    if (!succ) {
        resetRegisterNewEMailAccountPage(true);
        resetRetrieveEMailPage(true);
        UBTInfoDlg *dlg = nullptr;
        QJsonObject replyJson = QJsonDocument::fromJson(reply).object();
        if (replyJson.contains("message")) {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, replyJson.value("message").toString(), this);
        }
        else {
            dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("network error:") + info, this);
        }
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
}

void UBTCustomerCenterWidget::updateLoginSubmit()
{
    ui->loginSubmit->setEnabled(!ui->loginAccount->text().isEmpty() && !ui->loginPwd->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRetrieveMobileGetVCode()
{
    ui->retrieveMobileGetVCode->setEnabled(!ui->retrieveMobileAccount->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRetrieveMobileSubmit()
{
    ui->retrieveMobileSubmit->setEnabled(!ui->retrieveMobileAccount->text().isEmpty()
                                         && !ui->retrieveMobileVCode->text().isEmpty()
                                         && !ui->retrieveMobilePwd->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRetrieveEMailGetVCode()
{
    ui->retrieveEMailGetVCode->setEnabled(!ui->retrieveEMailAccount->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRetrieveEMailSubmit()
{
    ui->retrieveEMailSubmit->setEnabled(!ui->retrieveEMailAccount->text().isEmpty()
                                        && !ui->retrieveEMailVCode->text().isEmpty()
                                        && !ui->retrieveEMailPwd->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRegisterMobileGetVCode()
{
    ui->registerMobileGetVCode->setEnabled(!ui->registerMobileAccount->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRegisterMobileSubmit()
{
    ui->registerMobileSubmit->setEnabled(!ui->registerMobileAccount->text().isEmpty()
                                         && !ui->registerMobileVCode->text().isEmpty()
                                         && !ui->registerMobilePwd->text().isEmpty()
                                         && ui->registerMobileCheck->isChecked());
}

void UBTCustomerCenterWidget::updateRegisterEMailGetVCode()
{
    ui->registerEMailGetVCode->setEnabled(!ui->registerEMailAccount->text().isEmpty());
}

void UBTCustomerCenterWidget::updateRegisterEMailSubmit()
{
    ui->registerEMailSubmit->setEnabled(!ui->registerEMailAccount->text().isEmpty()
                                        && !ui->registerEMailVCode->text().isEmpty()
                                        && !ui->registerEMailPwd->text().isEmpty()
                                        && ui->registerEMailCheck->isChecked());
}

void UBTCustomerCenterWidget::resetAllPages()
{
    resetLoginPage();
    resetRegisterNewMobileAccountPage();
    resetRegisterNewEMailAccountPage();
    resetRetrieveMobilePage();
    resetRetrieveEMailPage();
}

void UBTCustomerCenterWidget::resetLoginPage(bool remainAccount, const QString &newAccount)
{
    if (remainAccount) {

    }
    else {
        ui->loginAccount->setText(newAccount);
    }
    ui->loginPwd->clear();
}

void UBTCustomerCenterWidget::resetRegisterNewMobileAccountPage(bool remainAccount, const QString &newAccount)
{
    if (remainAccount) {

    }
    else {
        ui->registerMobileAccount->setText(newAccount);
    }
    if (registerMobileGetVCodeTimer)
        registerMobileGetVCodeTimer->stop();
    ui->registerMobileGetVCode->setText(tr("verification code"));
    ui->registerMobileVCode->clear();
    ui->registerMobilePwd->clear();
    ui->registerMobileCheck->setChecked(false);
}

void UBTCustomerCenterWidget::resetRegisterNewEMailAccountPage(bool remainAccount, const QString &newAccount)
{
    if (remainAccount) {

    }
    else {
        ui->registerEMailAccount->setText(newAccount);
    }
    registerEMailGetVCodeTimer->stop();
    ui->registerEMailGetVCode->setText(tr("verification code"));
    ui->registerEMailVCode->clear();
    ui->registerEMailPwd->clear();
    ui->registerEMailCheck->setChecked(false);
}

void UBTCustomerCenterWidget::resetRetrieveMobilePage(bool remainAccount, const QString &newAccount)
{
    if (remainAccount) {

    }
    else {
        ui->retrieveMobileAccount->setText(newAccount);
    }
    retrieveMobileGetVCodeTimer->stop();
    ui->retrieveMobileGetVCode->setText(tr("verification code"));
    ui->retrieveMobileVCode->clear();
    ui->retrieveMobilePwd->clear();
}

void UBTCustomerCenterWidget::resetRetrieveEMailPage(bool remainAccount, const QString &newAccount)
{
    if (remainAccount) {

    }
    else {
        ui->retrieveEMailAccount->setText(newAccount);
    }
    retrieveEMailGetVCodeTimer->stop();
    ui->retrieveEMailGetVCode->setText(tr("verification code"));
    ui->retrieveEMailVCode->clear();
    ui->retrieveEMailPwd->clear();
}

void UBTCustomerCenterWidget::initialTimer()
{
    if (!retrieveMobileGetVCodeTimer) {
        retrieveMobileGetVCodeTimer = new QTimer(this);
        retrieveMobileGetVCodeTimer->setInterval(1 * 1000);
        retrieveMobileGetVCodeTimer->setSingleShot(false);
        connect(retrieveMobileGetVCodeTimer, &QTimer::timeout, this, [this]() {
            if (--retrieveMobileGetVCodeCounter >= 0) {
                QString text = QString("%1 S").arg(retrieveMobileGetVCodeCounter);
                ui->retrieveMobileGetVCode->setText(text);
            }
            else {
                retrieveMobileGetVCodeTimer->stop();
                ui->retrieveMobileGetVCode->setText(tr("verification code"));
            }
        });
    }

    if (!retrieveEMailGetVCodeTimer) {
        retrieveEMailGetVCodeTimer = new QTimer(this);
        retrieveEMailGetVCodeTimer->setInterval(1 * 1000);
        retrieveEMailGetVCodeTimer->setSingleShot(false);
        connect(retrieveEMailGetVCodeTimer, &QTimer::timeout, this, [this]() {
            if (--retrieveEMailGetVCodeCounter >= 0) {
                QString text = QString("%1 S").arg(retrieveEMailGetVCodeCounter);
                ui->retrieveEMailGetVCode->setText(text);
            }
            else {
                retrieveEMailGetVCodeTimer->stop();
                ui->retrieveEMailGetVCode->setText(tr("verification code"));
            }
        });
    }

    if (!registerMobileGetVCodeTimer) {
        registerMobileGetVCodeTimer = new QTimer(this);
        registerMobileGetVCodeTimer->setInterval(1 * 1000);
        registerMobileGetVCodeTimer->setSingleShot(false);
        connect(registerMobileGetVCodeTimer, &QTimer::timeout, this, [this]() {
            if (--registerMobileGetVCodeCounter >= 0) {
                QString text = QString("%1 S").arg(registerMobileGetVCodeCounter);
                ui->registerMobileGetVCode->setText(text);
            }
            else {
                registerMobileGetVCodeTimer->stop();
                ui->registerMobileGetVCode->setText(tr("verification code"));
            }
        });
    }

    if (!registerEMailGetVCodeTimer) {
        registerEMailGetVCodeTimer = new QTimer(this);
        registerEMailGetVCodeTimer->setInterval(1 * 1000);
        registerEMailGetVCodeTimer->setSingleShot(false);
        connect(registerEMailGetVCodeTimer, &QTimer::timeout, this, [this]() {
            if (--registerEMailGetVCodeCounter >= 0) {
                QString text = QString("%1 S").arg(registerEMailGetVCodeCounter);
                ui->registerEMailGetVCode->setText(text);
            }
            else {
                registerEMailGetVCodeTimer->stop();
                ui->registerEMailGetVCode->setText(tr("verification code"));
            }
        });
    }
}

void UBTCustomerCenterWidget::setTabOrder()
{
    QWidget::setTabOrder(ui->registerEMailAccount, ui->registerEMailVCode);
    QWidget::setTabOrder(ui->registerEMailVCode, ui->registerEMailPwd);
    QWidget::setTabOrder(ui->registerEMailPwd, ui->registerEMailAccount);

    QWidget::setTabOrder(ui->retrieveEMailAccount, ui->retrieveEMailVCode);
    QWidget::setTabOrder(ui->retrieveEMailVCode, ui->retrieveEMailPwd);
    QWidget::setTabOrder(ui->retrieveEMailPwd, ui->retrieveEMailAccount);
}



