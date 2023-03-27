#include "ubtinfodlg.h"
#include "ui_ubtinfodlg.h"

#include <QTimer>
#include <QFile>
#include <qDebug>

const QString UBTInfoDlg::QSS_PATH(":/res/qss/ubtinfodlg.qss");

UBTInfoDlg::UBTInfoDlg(Icon icon, const QString& info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UBTInfoDlg)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    if (icon == SuccessIcon) {
        ui->dlgIcon->setProperty("succ", true);
    }
    else if (icon == FailureIcon) {
        ui->dlgIcon->setProperty("succ", false);
    }
    else {
        ui->UBTInfoDlgIconContainer->hide();
    }
    ui->dlgInfo->setText(info);
    setDefaultStyle();

    closeTimer = new QTimer(this);
    closeTimer->setSingleShot(true);
    connect(closeTimer, &QTimer::timeout, this, [this]() {
        close();
    });
    closeTimer->start(3 * 1000);
}

UBTInfoDlg::~UBTInfoDlg()
{
    closeTimer->stop();
    delete ui;
}

void UBTInfoDlg::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        qDebug() << "UBTInfoDlg::setDefaultStyle set style sheet.";
        setStyleSheet(styleSheet.readAll());
    }
    else {
        qDebug() << QString("UBTInfoDlg::setDefaultStyle open %1 failed.").arg(QSS_PATH);
    }
}
