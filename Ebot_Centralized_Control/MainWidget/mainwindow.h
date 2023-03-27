#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//#include "../UBXRobotController/ubxdataprocessor.h"
#include "titlebar.h"
#include "../common/UBXPublic.h"
#include "../UBTServerCommunication/ubtservercommunication.h"

#include <QtCore>
#include <QUrl>
#include <QMainWindow>

const QString MAINWIDGET_DEFAULT_QSS_PATH = ":/res/qss/mainWidgetDefault.qss";

namespace Ui {
class MainWindow;
}

enum ResizeType
{
    UnSet = -1,
    ResizeL,//左边光标样式
    ResizeLT,//左上角光标样式
    ResizeT,
    ResizeRT,
    ResizeR,
    ResizeRB,
    ResizeB,
    ResizeLB
};

class CSerialPort;
class CRobotController;
class UBTMyPlayWidget;
class titlebar;
class UbtActionLibWidget;
class UBTMyDownloadVideoWidget;
class UBTMyDownloadHistoryWidget;
class UBTCustomerCenterWidget;
class UBTSetting;
class UBTPlayer;
class UBTConfirmDlg;

class MainWindow : public QMainWindow, public QAbstractNativeEventFilter,  public zxObserverBase
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;


    /**************************************************************************
    * 函数名: OnNotify
    * 功能: 通知消息
    * 参数:
    *    @[in ] pSubject: 被观察者
    *    @[in ] nMsgID: 消息ID
    *    @[in ] dwContext: 消息上下文
    *    @[in ] wParam: 参数1
    *    @[in ] lParam: 参数2
    * 返回值: void
    * 时间: 2018/07/6 19:09
    * 作者: Joker
    */
    virtual void OnNotify(const zxObserverSubjectBase* pSubject, int nMsgID, DWORD dwContext, WPARAM wParam, LPARAM lParam);

    /**************************************************************************
    * 函数名:InitWindow
    * 功能:初始化窗口
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void InitWindow();

//    QString getLastMusicPath();

    void InitTitleBar();

    //设置窗体默认Style sheet样式
    void SetDefaultStyle();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


public slots:

    void onMyPlay();
    void onMyDownload();
    void onMyCustom();
    void onConnect();
    void onClose();
    void onMin();
    void onSettingClicked();
    void onCustomerCenterClicked();
    void onLoginBtnClicked();
    void onLoginReply(bool succ, const QString &info, const QByteArray &reply);
    void onLogoutReply();
    void onNetworkErrorOccur(UBTServerCommunication::NetworkError networkError);

    void enterMyPlay();
    void enterMyDownload();
    void enterSetting();
    void enterCustomerCenter();

    void playOnlineVideo(QUrl videoUrl);

#if 0
    void on_pBQueryCom_clicked();

#endif
private:
#if 0
    void QueryCom();
#endif
    /**************************************************************************
    * 函数名:EnableControlWindow
    * 功能:窗口使能
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
//    void EnableControlWindow(bool bEnable);

    bool ConnectRobot();

signals:
    void SigOnConnectToRobot(bool bConnect);

private:
    Ui::MainWindow                  *ui;
    CSerialPort                     *m_pPort;
    CRobotController                *m_pRobotControlWidget;
    bool                            m_bIsConnect;
    int                             m_nChannelRec;
    std::vector <int>               m_vecPort;
    UBTMyPlayWidget                 *m_pMyPlayWidget;
    titlebar                        *m_pTitleBar;
    UbtActionLibWidget              *m_pUbtActionLibWidget;
    QPoint                          m_ptPre;//记录前一点，实现窗体移动
    ResizeType                      m_resizeType;
    bool                            m_bLeftDown;
//    QByteArray m_bytearrGeometry;
    UBTMyDownloadVideoWidget        *myDownloadVedioWidget;
    UBTMyDownloadHistoryWidget      *myDownloadHistoryWidget;
    UBTCustomerCenterWidget         *customerCenterWidget;
    UBTSetting                      *settingWidget;
    UBTPlayer                       *player;

    UBTConfirmDlg *confirmDlg;

    bool updateActionList;
    bool updateVideoList;
};

#endif // MAINWINDOW_H
