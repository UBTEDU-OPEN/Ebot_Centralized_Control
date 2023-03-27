#include "ubtsetting.h"
#include "ui_ubtsetting.h"
#include "../UBTServerCommunication/ubtservercommunication.h"
#include "../UBTUtil/ubtutil.h"
#include "../UBTWidget/ubtinfodlg.h"

#include <QButtonGroup>
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QPixmap>
#include <QDebug>

const QString UBTSetting::QSS_PATH(":/res/qss/ubtsetting.qss");

UBTSetting::UBTSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTSetting)
{
    ui->setupUi(this);
    ui->manualBtn->setChecked(true);
    ui->manualBtn->raise();
    FILEVERINFO fileinfo;
    CUtil::getFileVersion(qApp->applicationFilePath(), fileinfo);
    ui->versionText->setText(tr("version") + fileinfo.sProductVer);
    setDefaultStyle();

    btnGrp = new QButtonGroup(this);
    btnGrp->addButton(ui->manualBtn);
    btnGrp->addButton(ui->versionBtn);

    QPixmap *manual = new QPixmap(":/res/image/manual.png");
    ui->manualLabel->setPixmap(manual->scaled(ui->manualLabel->width(), ui->manualLabel->height(), Qt::KeepAspectRatio));

    connect(ui->manualBtn, &QPushButton::clicked, this, &UBTSetting::onManualBtnClicked);
    connect(ui->versionBtn, &QPushButton::clicked, this, &UBTSetting::onVersionBtnClicked);
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int index){
        if (1 == index) {
            upgradeRequest();
        }
    });
    connect(ui->upgradeBtn, &QPushButton::clicked, this, &UBTSetting::onUpgradeBtnClicked);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::upgradeRequestReply, this, &UBTSetting::onUpgradeRequestReply);
}

UBTSetting::~UBTSetting()
{
    delete ui;
}

void UBTSetting::upgradeRequest()
{
    newSoftwareUrl.clear();
    ui->upgradeBtn->setEnabled(false);

//    QString currentVersion = getCurrentVersion();
//    UBTServerCommunication::getInstance()->upgradeRequest(currentVersion);
//    qApp->applicationFilePath();

    FILEVERINFO fileinfo;
    CUtil::getFileVersion(qApp->applicationFilePath(), fileinfo);
    ui->versionText->setText(tr("version") + fileinfo.sProductVer);
    UBTServerCommunication::getInstance()->upgradeRequest(fileinfo.sProductVer);
}

void UBTSetting::onManualBtnClicked()
{
    ui->manualBtn->setChecked(true);
    ui->manualBtn->raise();
    ui->stackedWidget->setCurrentWidget(ui->manualPage);
}

void UBTSetting::onVersionBtnClicked()
{
    ui->versionBtn->setChecked(true);
    ui->versionBtn->raise();
    ui->stackedWidget->setCurrentWidget(ui->versionPage);
}

void UBTSetting::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
}

void UBTSetting::onUpgradeBtnClicked()
{
    if (!newSoftwareUrl.isEmpty()) {
        if (!QDesktopServices::openUrl(QUrl(newSoftwareUrl))) {
            UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::FailureIcon, tr("open download url failed"), this);
            dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
            dlg->show();
        }
    }
}

void UBTSetting::onUpgradeRequestReply(bool succ, QString info, QByteArray reply)
{
    newSoftwareUrl.clear();
    qDebug() << "UBTSetting::onUpgradeRequestReply succ:" << succ << "info:" << info << "reply:" << newSoftwareUrl;
    if (succ) {
        auto jsonDoc = QJsonDocument::fromJson(reply);
        auto urlObj = jsonDoc.object();
        if (urlObj.contains("url")) {
            newSoftwareUrl = urlObj.value("url").toString();
        }
        ui->upgradeBtn->setEnabled(!newSoftwareUrl.isEmpty());
    }
    else {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, info, this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
    }
}

QString UBTSetting::getCurrentVersion() const
{
    static QString version = "V1.0.0";
    if (version == "V1.0.0") {
        version = "V1.0.5";
    }
    else {
        version = "V1.0.0";
    }
    return version;
}
