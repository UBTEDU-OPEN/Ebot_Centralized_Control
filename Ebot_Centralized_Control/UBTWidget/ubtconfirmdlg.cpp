#include "ubtconfirmdlg.h"
#include "ui_ubtconfirmdlg.h"

#include <QFile>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include <qDebug>

const QString UBTConfirmDlg::QSS_PATH(":/res/qss/ubtconfirmdlg.qss");

QWidget *UBTConfirmDlg::bgWidget(nullptr);

UBTConfirmDlg::UBTConfirmDlg(const QString &title, const QString &content, const QString &acceptText, const QString &rejectText, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTConfirmDlg)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowModality(Qt::ApplicationModal);

    ui->setupUi(this);
    if (title.isEmpty()) {
        ui->UBTConfirmDlgTitle->hide();
    }
    else {
        ui->UBTConfirmDlgTitle->setText(title);
    }
    ui->UBTConfirmDlgContent->setText(content);
    if (rejectText.isEmpty()) {
        ui->UBTConfirmDlgReject->hide();
        ui->UBTConfirmDlgAccept->setProperty("single", true);
    }
    else {
        ui->UBTConfirmDlgReject->setText(rejectText);
        ui->UBTConfirmDlgAccept->setProperty("single", false);
    }
    ui->UBTConfirmDlgAccept->setText(acceptText);
    setDefaultStyle();

    connect(ui->UBTConfirmDlgAccept, &QPushButton::clicked, this, &UBTConfirmDlg::acceptClicked);
    connect(ui->UBTConfirmDlgReject, &QPushButton::clicked, this, &UBTConfirmDlg::rejectClicked);
}

UBTConfirmDlg::~UBTConfirmDlg()
{
    delete ui;
}

void UBTConfirmDlg::showInCenter()
{
    QPoint pos;
    if (auto pw = parentWidget()) {
        pos.rx() = (pw->width() - width()) / 2;
        pos.ry() = (pw->height() - height()) / 2;
    }
    else if (bgWidget) {
        pos.rx() = (bgWidget->width() - width()) / 2 + bgWidget->x();
        pos.ry() = (bgWidget->height() - height()) / 2 + bgWidget->y();
    }
    else {
        QDesktopWidget *desktopWidget = qApp->desktop();
        pos.rx() = (desktopWidget->width() - width()) / 2 + desktopWidget->x();
        pos.ry() = (desktopWidget->height() - height()) / 2 + desktopWidget->y();
    }
    qDebug() << "UBTConfirmDlg::showInCenter pos:" << pos;
    move(pos);
    show();
}

void UBTConfirmDlg::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if (styleSheet.open(QIODevice::ReadOnly)) {
        qDebug() << "UBTConfirmDlg::setDefaultStyle set style sheet.";
        setStyleSheet(styleSheet.readAll());
    }
    else {
        qDebug() << QString("UBTConfirmDlg::setDefaultStyle open %1 failed.").arg(QSS_PATH);
    }
}

void UBTConfirmDlg::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
