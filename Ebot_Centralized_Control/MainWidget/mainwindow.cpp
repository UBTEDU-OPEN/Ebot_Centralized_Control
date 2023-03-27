#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "../UBTUtil/filedirhandle.h"
#include "../UBXRobotController/Robot/SerialPort.h"
#include "../UBXRobotController/Robot/RobotController.h"
//#include "../UBTUtil/cstringhandle.h"
#include "../UBXRobotController/UBXRobotController.h"
#include "../UBTMyPlay/ubtmyplaywidget.h"
#include "../UBTMyPlay/ubtactionlibwidget.h"
#include "titlebar.h"
#include "../UBTMyDownload/ubtmydownloadvideowidget.h"
#include "../UBTMyDownload/ubtmydownloadhistorywidget.h"
#include "../UBTCustomerCenter/ubtcustomercenterwidget.h"
#include "../UBTSetting/ubtsetting.h"
#include "../UBTPlayer/ubtplayer.h"
#include "../UBTPlayer/ubtframecapture.h"
#include "../UBTWidget/ubtinfodlg.h"
#include "../UBTWidget/ubtconfirmdlg.h"
#include "../UBTUtil/configs.h"

#include <QDebug>
#include <QFileDialog>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

#ifdef Q_OS_WIN
#include <Dwmapi.h>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pPort(NULL),
    m_nChannelRec(0),
    m_pRobotControlWidget(NULL),
    m_pMyPlayWidget(NULL),
    m_pUbtActionLibWidget(NULL),
    m_pTitleBar(NULL),
    updateActionList(true),
    updateVideoList(true),
    confirmDlg(nullptr)
{
    ui->setupUi(this);
    m_bIsConnect = false;
    player = new UBTPlayer(this);
    player->hide();
    CreateRobotController(); //创建机器人通信控制器
    InitWindow();

#ifdef Q_OS_WIN
    BOOL bEnable = false;
    ::DwmIsCompositionEnabled(&bEnable);
    if (bEnable) {
        DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
        ::DwmSetWindowAttribute((HWND)winId(), DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
        MARGINS margins = { -1 };
        ::DwmExtendFrameIntoClientArea((HWND)winId(), &margins);
    }
#endif

    UBTConfirmDlg::setBgWidget(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    SAFE_DELETE(m_pRobotControlWidget);
    SAFE_DELETE(m_pPort);
    if(g_pRobotController)
    {
        RemoveSubscribe(g_pRobotController); //添加通信消息订阅
    }
    ReleaseRobotController();
//    SAFE_DELETE(m_pSendFileThread)
}

/**************************************************************************
* 函数名:InitWindow
* 功能:初始化窗口
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void MainWindow::InitWindow()
{
    //初始化主窗体
    setMouseTracking(true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    InitTitleBar();
    ui->centerStackedWidget->setCurrentIndex(0);//默认显示我的演出

    // my play (centerStackedWidget index 0)
    m_pMyPlayWidget = new UBTMyPlayWidget(this);
    m_pMyPlayWidget->EnableControlWindow(false);
    m_pUbtActionLibWidget = new UbtActionLibWidget(this);
    ui->HL_myPlay->addWidget(m_pMyPlayWidget);
    ui->HL_myPlay->addWidget(m_pUbtActionLibWidget);
    connect(m_pUbtActionLibWidget, &UbtActionLibWidget::loginClicked, this, &MainWindow::onLoginBtnClicked);

//    QRect rect = QApplication::desktop()->availableGeometry();
//    this->setGeometry(rect);
//    m_bytearrGeometry = saveGeometry();
//    QueryCom();

    // my download (centerStackedWidget index 1)
    auto myDownloadPageLayout = new QHBoxLayout(ui->myDownloadPage);
    myDownloadPageLayout->setSpacing(5);
    myDownloadPageLayout->setMargin(0);
    myDownloadVedioWidget = new UBTMyDownloadVideoWidget(nullptr);
    myDownloadPageLayout->addWidget(myDownloadVedioWidget);
    myDownloadHistoryWidget = new UBTMyDownloadHistoryWidget(nullptr);
    myDownloadPageLayout->addWidget(myDownloadHistoryWidget);
    connect(myDownloadVedioWidget, &UBTMyDownloadVideoWidget::videoDownloadStart, myDownloadHistoryWidget, &UBTMyDownloadHistoryWidget::onVideoDownloadStart);
//    connect(myDownloadVedioWidget, &UBTMyDownloadVideoWidget::videoDownloadProgress, myDownloadHistoryWidget, &UBTMyDownloadHistoryWidget::onVideoDownloadProgress);
//    connect(myDownloadVedioWidget, &UBTMyDownloadVideoWidget::videoDownloadFinished, myDownloadHistoryWidget, &UBTMyDownloadHistoryWidget::onVideoDownloadFinished);
    connect(myDownloadVedioWidget, &UBTMyDownloadVideoWidget::playOnlineVideo, this, &MainWindow::playOnlineVideo);
    // customize (centerStackedWidget index 2)

    // customer center (centerStackedWidget index 3)
    auto customerCenterPageLayout = new QHBoxLayout(ui->customerCenterPage);
    customerCenterPageLayout->setSpacing(5);
    customerCenterPageLayout->setMargin(0);
    customerCenterWidget = new UBTCustomerCenterWidget(nullptr);
    customerCenterPageLayout->addWidget(customerCenterWidget);

    // setting ((centerStackedWidget index 4)
    settingWidget = new UBTSetting;
    ui->centerStackedWidget->addWidget(settingWidget);

    if(g_pRobotController) {
        AddSubscribe(g_pRobotController); //添加通信消息订阅
    }

    connect(m_pUbtActionLibWidget, &UbtActionLibWidget::actionDownloadFinished, m_pMyPlayWidget, &UBTMyPlayWidget::InitActionList);
    connect(m_pUbtActionLibWidget, &UbtActionLibWidget::actionDownloadFailed, this, [this]() {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::FailureIcon, tr("download failed"), this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
    });
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::loginReply, this, &MainWindow::onLoginReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::logoutReply, this, &MainWindow::onLogoutReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::clearActionDownloadRecordReply, this, [this]() {
        UBTServerCommunication::getInstance()->getActionList("");
    });
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::networkErrorOccur, this, &MainWindow::onNetworkErrorOccur);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::deactivateReply, this, [this](bool succ, QString info, QByteArray reply){
        if (succ) {
            updateActionList = true;
            updateVideoList = true;
        }
    });

    SetDefaultStyle();

//    QGraphicsDropShadowEffect *pEffect = new QGraphicsDropShadowEffect(ui->centralWidget);
//    pEffect->setOffset(5, 5);
//    pEffect->setColor(QColor("black"));
//    pEffect->setBlurRadius(10);
//    ui->centralWidget->setGraphicsEffect(pEffect);
}


void MainWindow::InitTitleBar()
{
    m_pTitleBar = new titlebar(this);
    ui->centerWidgetLayout->insertWidget(0, m_pTitleBar);

    connect(m_pTitleBar, &titlebar::SigMyPlay, this, &MainWindow::onMyPlay);
    connect(m_pTitleBar, &titlebar::SigMyDownload, this, &MainWindow::onMyDownload);
    connect(m_pTitleBar, &titlebar::SigMyCustom, this, &MainWindow::onMyCustom);
    connect(m_pTitleBar, &titlebar::SigConnect, this, &MainWindow::onConnect);
    connect(m_pTitleBar, &titlebar::SigClose, this, &MainWindow::onClose);
    connect(m_pTitleBar, &titlebar::SigMin, this, &MainWindow::onMin);
    connect(m_pTitleBar, &titlebar::settingClicked, this, &MainWindow::onSettingClicked);
    connect(m_pTitleBar, &titlebar::customerCenterClicked, this, &MainWindow::onCustomerCenterClicked);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::loginReply, m_pTitleBar, &titlebar::onLogin);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::logoutReply, m_pTitleBar, &titlebar::onLogout);

    connect(this, &MainWindow::SigOnConnectToRobot, m_pTitleBar, &titlebar::onConnectToRobot);
}

bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG* pMsg = reinterpret_cast<MSG*>(message);
        if(pMsg->message == WM_DEVICECHANGE)
        {
            qDebug()<<"MainWindow windows system message WM_DEVICECHANGE";

            if(g_pRobotController)
                g_pRobotController->OnDeviceChange(pMsg, result);
        }
    }
    return false;
}

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
void MainWindow::OnNotify(const zxObserverSubjectBase *pSubject, int nMsgID, DWORD dwContext, WPARAM wParam, LPARAM lParam)
{
    if (nMsgID == WM_MSG_ROBOT_CONNECT)
    {
        BOOL userPurpose = dwContext;
        BOOL bOk = wParam;
        if (m_pMyPlayWidget)
        {
            m_pMyPlayWidget->EnableControlWindow(bOk);
            m_pMyPlayWidget->SetWindowByConnectState(bOk);
        }

        if (!bOk && !userPurpose) { // 如果不是用户主动想要关闭
            UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("connection failed, try again?"), tr("yes"), tr("no"));
            connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
                ConnectRobot();
                dlg->close();
            });
            connect(dlg, &UBTConfirmDlg::rejectClicked, this, [this, dlg]() {
                dlg->close();
            });
            dlg->showInCenter();
        }

        emit SigOnConnectToRobot(bOk);
    }
    else if (nMsgID == WM_MSG_ROBOT_DISCONNECT)
    {  
        qDebug() << "MainWindow::OnNotify WM_MSG_ROBOT_DISCONNECT";
    }
}

void MainWindow::SetDefaultStyle()
{
    QFile styleSheet(MAINWIDGET_DEFAULT_QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
}

void MainWindow::onMyPlay()
{
    enterMyPlay();
}

void MainWindow::onMyDownload()
{
    if (UBTServerCommunication::getInstance()->isLogin()) {
        enterMyDownload();
    }
    else {
        UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("login first, please."), tr("OK"), "");
        connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
            dlg->close();
        });
        dlg->showInCenter();
    }
}

void MainWindow::onMyCustom()
{
    ui->centerStackedWidget->setCurrentIndex(2);
}


void MainWindow::onClose()
{
    CConfigs::instance()->clearActionThumbnailDir();
    CConfigs::instance()->clearVideoThumbnailDir();
    close();
}

void MainWindow::onMin()
{
    showMinimized();
}

void MainWindow::onSettingClicked()
{
    if (UBTServerCommunication::getInstance()->isLogin()) {
        enterSetting();
    }
    else {
        UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("login first, please."), tr("OK"), "");
        connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
            dlg->close();
        });
        dlg->showInCenter();
    }
}

void MainWindow::onCustomerCenterClicked()
{
    enterCustomerCenter();
}

void MainWindow::onLoginBtnClicked()
{
    if (m_pTitleBar) {
        m_pTitleBar->setCustomerCenterBtnChecked();
    }
    enterCustomerCenter();
}

void MainWindow::onLoginReply(bool succ, const QString &info, const QByteArray &reply)
{
    if (succ && (UBTServerCommunication::getInstance()->getLoginEntrance() == UBTServerCommunication::MyPlay)) {
        if (m_pTitleBar) {
            m_pTitleBar->setMyPlayBtnChecked();
        }
        enterMyPlay();
    }
}

void MainWindow::onLogoutReply()
{
    updateActionList = true;
    updateVideoList = true;
    enterCustomerCenter();
}

void MainWindow::onNetworkErrorOccur(UBTServerCommunication::NetworkError networkError)
{
    if (!confirmDlg) {
        QString errorInfo = tr("Unknown network error");
        switch(networkError) {
        case UBTServerCommunication::AuthenticationRequiredError: {
            errorInfo = tr("Authentication required error, please login again.");
        }
            break;
        case UBTServerCommunication::UnknownError:
        default:
            break;
        }
        confirmDlg = new UBTConfirmDlg("", errorInfo, tr("OK"), "");
        connect(confirmDlg, &UBTConfirmDlg::acceptClicked, this, [this, networkError]() {
            if (networkError == UBTServerCommunication::AuthenticationRequiredError) {
                UBTServerCommunication::getInstance()->logout();
            }
            confirmDlg->close();
            confirmDlg = nullptr;
        });
        confirmDlg->showInCenter();
    }
}

void MainWindow::enterMyPlay()
{
    if (ui->centerStackedWidget->currentWidget() != ui->myPlayPage) {
        m_pMyPlayWidget->InitActionList();
        if (updateActionList && UBTServerCommunication::getInstance()->isLogin()) {
            updateActionList = false;
            UBTServerCommunication::getInstance()->getActionList("");
        }
        m_pUbtActionLibWidget->updateContent();
        ui->centerStackedWidget->setCurrentWidget(ui->myPlayPage);
    }
}

void MainWindow::enterMyDownload()
{
    if (ui->centerStackedWidget->currentWidget() != ui->myDownloadPage) {
        if (updateVideoList) {
            updateVideoList = false;
            UBTServerCommunication::getInstance()->getVideoList("");
            UBTServerCommunication::getInstance()->getPerformanceList("");
        }
        myDownloadVedioWidget->updateContent();
        myDownloadHistoryWidget->updateContent();
        ui->centerStackedWidget->setCurrentWidget(ui->myDownloadPage);
    }
}

void MainWindow::enterSetting()
{
    if (settingWidget && ui->centerStackedWidget->currentWidget() != settingWidget) {
        ui->centerStackedWidget->setCurrentWidget(settingWidget);
        settingWidget->onManualBtnClicked();
    }
}

void MainWindow::onConnect()
{
    do
    {
#if 0
        QString strCom = ui->comboBoxComList->currentText();
        if (strCom.isEmpty())
        {
            break;
        }
        qDebug() << "MainWindow::on_pBConnect_clicked " << strCom << endl;

        if (m_bIsConnect)
        {
            SAFE_DELETE(m_pPort);
            ui->pBConnect->setText("连接");
            m_bIsConnect = false;
            break;
        }
        if(!m_pPort)
        {
            m_pPort = new CSerialPort((QMainWindow*)this);
        }

        if(!m_pPort)
            break;
        if(!m_pPort->InitPort(strCom, 9600, 'N', 8, 0))
        {
            SAFE_DELETE(m_pPort);
            break;
        }
        else
        {
            m_bIsConnect = true;
            ui->pBConnect->setText("断开");
        }
#endif

        ConnectRobot();
    }while(0);

}


//void MainWindow::on_pBQueryCom_clicked()
//{
//    EnableControlWindow(false);
//    QueryCom();
//    EnableControlWindow(true);
//}
#if 0
void MainWindow::QueryCom()
{
    m_vecPort.clear();
   // ui->comboBoxComList->clear();
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    BOOL bGetVer = GetVersionEx(&osvi);

    if (bGetVer && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
    {
        //Use QueryDosDevice to look for all devices of the form COMx. This is a better
        //solution as it means that no ports have to be opened at all.
        TCHAR szDevices[65535];
        DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
        if (dwChars)
        {
            int i=0;
            for (;;)
            {
                //Get the current device name
                TCHAR* pszCurrentDevice = &szDevices[i];
                QString strDeviceName = QString::fromWCharArray(pszCurrentDevice);
                qDebug()<<strDeviceName<<endl;
                //looks  "COMX"
                int nLen = wcslen(pszCurrentDevice);
                QString strCom = "COM";
                WCHAR wszCom[STRING_LEN_512] = {0};
                strCom.toWCharArray(wszCom);
                if (nLen > 3 && _wcsnicmp(pszCurrentDevice, wszCom, 3) == 0)
                {
                    int nPort = _wtoi(&pszCurrentDevice[3]);

                    m_vecPort.push_back(nPort);
                    QString strCurCom = NULLSTR;
                    strCurCom.sprintf("COM%d", nPort);
                   // ui->comboBoxComList->addItem(strCurCom);
                }
                //next NULL
                TCHAR  tChar = '\0';

                while(szDevices[i] != tChar)
                    i++;
                i++;
                // The list is double-NULL terminated, so if the character is
                // now NULL, we're at the end
                if (szDevices[i] == tChar)
                    break;
            }
        }
        else
        {
            qDebug("Failed in call to QueryDosDevice, GetLastError:%d\n", GetLastError());
        }
    }

    else
    {
        //On 95/98 open up each port to determine their existence
        for (UINT i=1; i<256; i++)
        {
            QString strSzPort = NULLSTR;
            strSzPort.sprintf("\\\\.\\COM%d", i);
            WCHAR wszPortName[STRING_LEN_512] = {0};
            strSzPort.toWCharArray(wszPortName);
            BOOL bSuccess = FALSE;
            HANDLE hPort = ::CreateFile(wszPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
            if (hPort == INVALID_HANDLE_VALUE)
            {
                DWORD dwError = GetLastError();
                //Check to see if the error was because some other app had the port open or a general failure
                if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
                    bSuccess = TRUE;
            }
            else
            {
                bSuccess = TRUE;
                //Don't forget to close the port, since we are going to do nothing with it anyway
                CloseHandle(hPort);
            }
            //Add the port number to the array which will be returned
      /*      if (bSuccess)
                ports.Add(i)*/;
        }
    }
    //    box->SetCurSel(box->GetCount() - 1);
}
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
//void MainWindow::EnableControlWindow(bool bEnable)
//{
//    ui->pBConnect->setEnabled(bEnable);
//}

bool MainWindow::ConnectRobot()
{
    bool bOk = false;
    if(g_pRobotController)
    {
        if(!g_pRobotController->GetPortOpenState())
        {
            //ui->pushButton_open->setEnabled(false);
            bOk = g_pRobotController->Req_Connect();
        }
        else
        {
            g_pRobotController->Req_Disconnect();
        }
    }
//    if (bOk)
//    {
//        bOk = false;
//    }
//    else
//    {
//        bOk = true;
//    }

//    if (m_pMyPlayWidget)
//    {
//        m_pMyPlayWidget->EnableControlWindow(bOk);
//        m_pMyPlayWidget->SetWindowByConnectState(bOk);
//    }

//    emit SigOnConnectToRobot(bOk);

    return true;
}

void MainWindow::enterCustomerCenter()
{
    if (ui->centerStackedWidget->currentWidget() != ui->customerCenterPage) {
        if (UBTServerCommunication::getInstance()->isLogin()) {
            UBTServerCommunication::getInstance()->getActionDownloadHistory();
        }
        ui->centerStackedWidget->setCurrentWidget(ui->customerCenterPage);
    }
}

void MainWindow::playOnlineVideo(QUrl videoUrl)
{
//    QVector<QString> localVideoes;
//    localVideoes.append("D:/resource/project/Alpha1E_Centralized_Control-Debug/MainWidget/debug/video/2016春晚表演.mp4");
//    localVideoes.append("D:/resource/project/Alpha1E_Centralized_Control-Debug/MainWidget/debug/video/Alpha上学.mp4");
//    localVideoes.append("D:/resource/project/Alpha1E_Centralized_Control-Debug/MainWidget/debug/video/Alpha上学记.mp4");
//    localVideoes.append("D:/resource/project/Alpha1E_Centralized_Control-Debug/MainWidget/debug/video/Alpha有新队服.MP4");
//    localVideoes.append("D:/resource/project/Alpha1E_Centralized_Control-Debug/MainWidget/debug/video/Alpha有新队服啦.MP4");
//    localVideoes.append("D:/resource/project/Alpha1E_Centralized_Control-Debug/MainWidget/debug/video/群舞获吉尼斯世界记录.MP4");
//    static int playIndex = 0;
//    if (player) {
//        player->move((width() - player->width()) / 2, (height() - player->height()) / 2);
//        player->show();
//        player->play(QUrl(localVideoes[playIndex]));
//        playIndex = ++playIndex > 3 ? 0 : playIndex;
//    }

//    QVector<QUrl> localVideoes;
//    localVideoes.append(QUrl("https://ubt-alpha.oss-cn-shenzhen.aliyuncs.com/centre/video/business/Alpha%E6%9C%89%E6%96%B0%E9%98%9F%E6%9C%8D%E5%95%A6.MP4"));
//    localVideoes.append(QUrl("https://ubt-alpha.oss-cn-shenzhen.aliyuncs.com/centre/video/business/Alpha有新队服啦.MP4"));
//    localVideoes.append(videoUrl);
//    static int playIndex = 0;
//    if (player) {
//        player->move((width() - player->width()) / 2, (height() - player->height()) / 2);
//        player->show();
//        qDebug() << "############ play index" << playIndex;
//        player->play(localVideoes[playIndex]);
//        playIndex = ++playIndex > 2 ? 0 : playIndex;
//    }

//    static UBTFrameCapture *frameCapture;
//    if (!frameCapture) {
//        frameCapture = new UBTFrameCapture(this);
//    }
//    QMap<QString, QString> url2captureFile;
//    url2captureFile.insert(videoUrl, "D:\\resource\\project\\build-Alpha1E_Centralized_Control-Desktop_Qt_5_5_1_MSVC2010_32bit-Debug\\MainWidget\\debug\\video\\111");
//    frameCapture->setUrlAndCaptureFile(url2captureFile);
//    frameCapture->startCapture();

    if (player) {
        player->move((width() - player->width()) / 2, (height() - player->height()) / 2);
        player->show();
        player->play(videoUrl);
//        player->setMedia(videoUrl);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_ptPre = e->globalPos();
        m_bLeftDown = true;
    }
    else
    {
        m_bLeftDown = false;
    }
    return QWidget::mousePressEvent(e);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(!(windowState() & Qt::WindowMaximized))
    {
//        if(m_bLeftDown && m_resizeType != UnSet)
//        {
//            changeSize(e->globalPos());
//            e->accept();
//        }
        /*else*/ if(m_bLeftDown && m_resizeType == UnSet && m_pTitleBar->rect().contains(mapFromGlobal(m_ptPre)))
        {
            QPoint nowPt = e->globalPos();
            QPoint moveTo = mapToParent(nowPt - m_ptPre);
            move(moveTo);
            m_ptPre = nowPt;
//            m_bytearrGeometry = saveGeometry();
        }
        //button()Note that the returned value is always Qt::NoButton for mouse move events.
//        else if(!(e->buttons() & Qt::LeftButton))
//        {
//            changeCursor(e->pos());
//        }
    }
    QWidget::mouseMoveEvent(e);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    m_resizeType = UnSet;
    //setCursor(QCursor(Qt::ArrowCursor));
    m_bLeftDown = false;
    QWidget::mouseReleaseEvent(e);
}

//QString MainWindow::getLastMusicPath()
//{
//    QString strFilePath;
//    CIniConfig::getInstance().getPath(eFilePathImportMusic, strFilePath);
//    QDir dir(strFilePath);
//    if (strFilePath.isEmpty() || !dir.exists())
//    {
//        strFilePath = QDir::currentPath();
//    }

//    return strFilePath;
//}
