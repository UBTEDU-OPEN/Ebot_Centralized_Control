#include "titlebar.h"
#include "ui_titlebar.h"
#include "../common/UBXPublic.h"
#include "clanguagemgr.h"
#include "../UBTServerCommunication/ubtservercommunication.h"
#include "../UBTWidget/ubtconfirmdlg.h"

#include <QDebug>

titlebar::titlebar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::titlebar)
{
    ui->setupUi(this);
    ui->pBConnect->setText(QObject::tr("Not connected"));
    ui->pBConnect->setProperty("connected", false);
    ui->pBMyPlay->setEnabled(true);
    ui->pBMyDownLoad->setEnabled(true);
    ui->pBMyCustom->setEnabled(true);
//    ui->pbWin_Close->setStyleSheet("QPushButton#pbWin_Close{border-image: url(:/res/images/icon_win_close_nor.png);}"
//                                   "QPushButton#pbWin_Close:hover{border-image: url(:/res/images/icon_win_close_sel.png);}"
//                                   "QPushButton#pbWin_Close:pressed{border-image: url(:/res/images/icon_win_close_press.png);}");
//    QMargins marginTemp = ui->horizontalLayout->contentsMargins();
//    marginTemp.setTop(12);
//    ui->horizontalLayout->setContentsMargins(marginTemp);

    ui->buttonGroup->addButton(ui->pBMyPlay);
    ui->buttonGroup->addButton(ui->pBMyDownLoad);
    ui->buttonGroup->addButton(ui->pBMyCustom);
    ui->buttonGroup->addButton(ui->settingBtn);
    ui->buttonGroup->addButton(ui->customerCenterBtn);
    ui->pBMyPlay->setChecked(true);

//    ui->pBConnect->setStyleSheet("QPushButton#pBConnect{background-color:#3C5873;font:12px;color: #ffffff;qproperty-iconSize: 20px 10px;border:1px;}"
//                                   "QPushButton#pBConnect:hover{background-color:#3C5873;font:12px;color:#3EA5D2;qproperty-iconSize:20px 10px;border:1px;}"
//                                   "QPushButton#pBConnect:pressed{background-color:#3C5873;font:12px;color:#3EA5D2;qproperty-iconSize:20px 10px;border:1px;}");
    if (CLanguageMgr::getInstance()->getLanguageKeyword() == "zh_CN")
    {
        ui->labelLogo->setPixmap(QPixmap(":/res/images/img_ubt_logo.png"));
     //   ui->labelTitle->setStyleSheet("QLabel#labelTitle{font: 16px;color: #ffffff;}");
    }
    else
    {
        ui->labelLogo->setPixmap(QPixmap(":/res/images/img_ubt_logo.png"));
//        ui->labelTitle->setWordWrap(true);
      //  ui->labelTitle->setStyleSheet("QLabel#labelTitle{font: 12px;color: #ffffff;}");
    }
//    ui->labelTitle->setWordWrap(true);

    connect(ui->settingBtn, &QPushButton::clicked, this, &titlebar::onSettingClicked);
    connect(ui->customerCenterBtn, &QPushButton::clicked, this, &titlebar::onCustomerCenterClicked);
}

titlebar::~titlebar()
{
    delete ui;
}

void titlebar::onConnectToRobot(bool bConnect)
{
    if (bConnect) {
        ui->pBConnect->setText(QObject::tr("Connected"));
//        ui->pBConnect->setIcon(QIcon(":/res/images/ic_connected.png"));
        ui->pBConnect->setProperty("connected", true);
    }
    else {
        ui->pBConnect->setText(QObject::tr("Not connected"));
//        ui->pBConnect->setIcon(QIcon(":/res/images/ic_disconnect.png"));
        ui->pBConnect->setProperty("connected", false);
    }
    ui->pBConnect->setEnabled(true);
    ui->pBConnect->style()->unpolish(ui->pBConnect);
    ui->pBConnect->style()->polish(ui->pBConnect);
    ui->pBConnect->update();
}

void titlebar::on_pBMyPlay_clicked()
{
    ui->pBMyPlay->setChecked(true);
    emit SigMyPlay();
}

void titlebar::on_pBMyDownLoad_clicked()
{
    ui->pBMyDownLoad->setChecked(true);
    emit SigMyDownload();
}

void titlebar::on_pBMyCustom_clicked()
{
    UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("Still under development, please wait"), tr("OK"), "");
    connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
        dlg->close();
    });
    dlg->showInCenter();
//    ui->pBMyCustom->setChecked(true);
//    emit SigMyCustom();
}

void titlebar::on_pBConnect_clicked()
{
//    ui->pBConnect->setEnabled(false);
    emit SigConnect();
}

void titlebar::on_pBClose_clicked()
{
    emit SigClose();
}

void titlebar::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}

void titlebar::on_pBMin_clicked()
{
    emit SigMin();
}

void titlebar::onSettingClicked()
{
    ui->settingBtn->setChecked(true);
    emit settingClicked();
}

void titlebar::onCustomerCenterClicked()
{
    ui->customerCenterBtn->setChecked(true);
    UBTServerCommunication::getInstance()->setLoginEntrance(UBTServerCommunication::CustomerCenter);
    emit customerCenterClicked();
}

void titlebar::setCustomerCenterBtnChecked()
{
    ui->customerCenterBtn->setChecked(true);
}

void titlebar::setMyPlayBtnChecked()
{
    ui->pBMyPlay->setChecked(true);
}

void titlebar::onLogin(bool succ)
{
    if (succ) {
        ui->pBMyDownLoad->setCheckable(true);
    }
}

void titlebar::onLogout()
{
    ui->pBMyDownLoad->setCheckable(false); 
}
