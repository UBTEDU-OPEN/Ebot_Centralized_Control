#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class titlebar;
}

class titlebar : public QWidget
{
    Q_OBJECT

public:
    explicit titlebar(QWidget *parent = 0);
    ~titlebar();

public slots:
    void onConnectToRobot(bool bConnect);
    void onLogin(bool succ);
    void onLogout();

    void on_pBMyPlay_clicked();
    void on_pBMyDownLoad_clicked();
    void on_pBMyCustom_clicked();
    void on_pBConnect_clicked();
    void on_pBClose_clicked();
    void on_pBMin_clicked();
    void onSettingClicked();
    void onCustomerCenterClicked();

    void setCustomerCenterBtnChecked();
    void setMyPlayBtnChecked();

signals:
    void SigMyPlay();
    void SigMyDownload();
    void SigMyCustom();
    void SigConnect();
    void SigClose();
    void SigMin();
    void settingClicked();
    void customerCenterClicked();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::titlebar *ui;
};

#endif // TITLEBAR_H
