#include "ubtprivacyagreementdlg.h"
#include "ui_ubtprivacyagreementdlg.h"

#include <QFile>
#include <QByteArray>
#include <qDebug>

const QString UBTPrivacyAgreementDlg::QSS_PATH(":/res/qss/ubtprivacyagreementdlg.qss");

UBTPrivacyAgreementDlg::UBTPrivacyAgreementDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UBTPrivacyAgreementDlg)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    setDefaultStyle();

    QFile privacyFile(":/res/privacyAgreement.txt");
    if (privacyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        auto data = privacyFile.readAll();
        ui->UBTPrivacyAgreementDlgContent->setText(QString(data));
    }
    else {
        qDebug() << "UBTPrivacyAgreementDlg open privacyFile failed";
    }

    connect(ui->UBTPrivacyAgreementDlgCloseBtn, &QPushButton::clicked, this, &UBTPrivacyAgreementDlg::onCloseBtnClicked);
}

UBTPrivacyAgreementDlg::~UBTPrivacyAgreementDlg()
{
    delete ui;
}

void UBTPrivacyAgreementDlg::onCloseBtnClicked()
{
    close();
}

void UBTPrivacyAgreementDlg::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
    else {
        qDebug() << "UBTPrivacyAgreementDlg::setDefaultStyle open qss failed";
    }
}
