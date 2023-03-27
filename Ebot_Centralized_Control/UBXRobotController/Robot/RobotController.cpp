/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：RobotController.cpp
* 创建时间：2015/10/18 14:39
* 文件标识：
* 文件摘要：机器人控制类
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/10/18 14:39
* 版本摘要：
*/
#include "StdAfx.h"
#include "RobotController.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>


//#include "cbluetoothcomandparser.h"
//#include "cdataoperate.h"
//#include "cjsonutil.h"
//#include "../UBXBase/UBXBase.h"
#include "UBXPublic.h"
#include "ciniconfig.h"

#ifdef Q_OS_WIN
extern "C"
{
#include "setupapi.h"
#include "hidsdi.h"
}
#endif

#ifdef Q_OS_WIN
static const GUID GUID_DEVINTERFACE_LIST[] =
{
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

    // GUID_DEVINTERFACE_DISK
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

    // GUID_DEVINTERFACE_HID,
    { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },

    // GUID_NDIS_LAN_CLASS
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
};
#endif

//设备VID，PID定义
DEVIDTYPE     g_devidarr[] =
{
    DEVIDTYPE(ePortTypeHID, 0x0483, 0x5750),
    DEVIDTYPE(ePortTypeVCP, 0x0403, 0x6001)
};

//#pragma region 消息定义
const UINT WM_MSG_PORT_OPENSTATECHANGE =    QEvent::registerEventType();     //端口打开状态改变消息

const UINT WM_MSG_ROBOT_CONNECT =              QEvent::registerEventType();            //连接机器人
const UINT WM_MSG_ROBOT_DISCONNECT =           QEvent::registerEventType();         //断开机器人

CRobotController* CRobotController::m_pControllerStatic = NULL;
BOOL     g_bFirstDeviceArrival = FALSE; //设备第一次到达
//#pragma endregion


CRobotController::CRobotController(QWidget *parent) : QWidget(parent)
{
    m_pControllerStatic = this;
    m_pConnectTimer = NULL;
    m_pPort = NULL;


    m_nConnectPortTimerID = 5000;
    m_nDisconnectPortTimerID = 5001;



    m_eSwitchMode = eRobotSwitchModeUdisk;

    m_stDevIDInfo = g_devidarr[ePortTypeHID];
    //m_nCurTranDataLen = 0;
    //m_nDataTransRate = 0;

    RegisterDeviceNotify();

    hid_init();
    m_nLastTimeMs = 0;
    m_pConnectTimer = new QTimer();
    m_pConnectTimer->setInterval(TIMEDELAY_CONNECTROBOT);
    connect(m_pConnectTimer, &QTimer::timeout, this, &CRobotController::slotTimerTimeOut);
    StartConncectTimer();

//    slotOnRobotProductTypeChange( eUbtProductUnknown, eUbtProductUnknown);
}

CRobotController::~CRobotController()
{
    if(GetPortOpenState())
    {
        Req_Disconnect();

        // 关闭应用程序，收不到应答，因此需要另外关闭串口
        Req_ClosePort();
    }

    hid_exit();

    disconnect(m_pConnectTimer, &QTimer::timeout, this, &CRobotController::slotTimerTimeOut);
    SAFE_DELETE(m_pConnectTimer);
//    m_oRobotCommunicateThread.quit();
//    m_oRobotCommunicateThread.wait();
}

//#pragma region

/**************************************************************************
* 函数名: RegisterDeviceNotify
* 功能: 注册设备通知
* 时间: 2015/09/08 10:08
* 作者: ZDJ
*/
void CRobotController::RegisterDeviceNotify()
{
#ifdef Q_OS_MAC

    CMyHidClass::SetArrivalCbk(OnDeviceArrival);
    CMyHidClass::SetRemovalCbk(OnDeviceRemoval);
#elif defined(Q_OS_WIN)

    HDEVNOTIFY hDevNotify = NULL;

    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for(int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST) / sizeof(GUID); i++)
    {
        NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
        if( !hDevNotify )
        {
            //AfxMessageBox(CString(_T("Can't register device notification: ")), MB_ICONEXCLAMATION);
            return;
        }
        else
        {
            qDebug("reg notify success");
        }
    }

#endif
}

//#pragma endregion

//#pragma region 请求
/**************************************************************************
* 函数名: Req_Connect
* 功能: 请求连接指定端口的机器人
* 时间: 2015/10/18 14:54
* 作者: ZDJ
*/
bool CRobotController::Req_Connect()
{
    if(Req_AutoOpenPort())
    {
//         m_pRobotCmd = &m_objRobotCmdOfPCOld;
        NotifyObservers(WM_MSG_ROBOT_CONNECT, TRUE, TRUE, NULL);;
        return true;
    }
    else
    {
        NotifyObservers(WM_MSG_ROBOT_CONNECT, FALSE, FALSE, NULL);
        return false;
    }
}

/**************************************************************************
* 函数名: Req_Disconnect
* 功能: 请求断开机器人的连接
* 时间: 2015/10/18 14:55
* 作者: ZDJ
*/
void CRobotController::Req_Disconnect(void)
{
//    SendCmdData(eRobotCmdTypeDisconnectRobot);
    // 在断开连接完成后再close
    Req_ClosePort();
    NotifyObservers(WM_MSG_ROBOT_CONNECT, TRUE, FALSE, NULL);
}

/**************************************************************************
* 函数名: Req_OpenPort
* 功能: 请求打开指定端口
* 参数:
*    @[in ] nPortType: 端口类型
*    @[in ] dwVID: dwVID值
*    @[in ] dwPID: dwPID值
*    @[in ] ...: 变长参数
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/10/21 20:49
* 作者: ZDJ
*/
BOOL CRobotController::Req_OpenPort(int nPortType, DWORD dwVID, DWORD dwPID, ...)
{
    /*if(m_pPort && m_pPort->IsOpen())
        m_pPort->ClosePort();*/
//    Req_ClosePort();

    if(nPortType == ePortTypeHID)
    {
        if(!m_pPort)
        {
            m_pPort = new CMyHidClass(this);
//            m_pPort->m_pPortRecvDataFCB = CRobotController::portRecvData;
//            m_pPort->m_pUser = (void*)this;
        }
        if(!m_pPort)
            return FALSE;

        if(!m_pPort->Init())
        {
            SAFE_DELETE(m_pPort);
            return FALSE;
        }

        return m_pPort->OpenPort(dwVID, dwPID);
    }
#ifdef Q_OS_WIN
    else if(nPortType == ePortTypeVCP)
    {
        if(!m_pPort)
            m_pPort = new CSerialPort(this);
        if(!m_pPort)
            return FALSE;

        if(!m_pPort->Init())
        {
            SAFE_DELETE(m_pPort);
            return FALSE;
        }

        va_list va_args;
        va_start(va_args, dwPID);

        int nBandrate = va_arg(va_args, int);
        va_end(va_args);

        m_pPort->SetParams(nBandrate);

        return m_pPort->OpenPort(dwVID, dwPID);
    }
#endif
    return FALSE;
}

/**************************************************************************
* 函数名: Req_AutoOpenPort
* 功能: 请求自动打开端口
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/10/21 20:49
* 作者: ZDJ
*/
BOOL CRobotController::Req_AutoOpenPort()
{
    BOOL bRet = FALSE;

    int nPortNumber = sizeof(g_devidarr)/sizeof(DEVIDTYPE);
    for(int i = 0; i < nPortNumber; i++)
    {
        //SAFE_DELETE(m_pPort);
        Req_ClosePort();

        DEVIDTYPE tempDevIDInfo = g_devidarr[i];
        if(tempDevIDInfo.nType == ePortTypeVCP)
        {
            bRet = Req_OpenPort(tempDevIDInfo.nType, tempDevIDInfo.dwVid, tempDevIDInfo.dwPid, 9600);
        }
        else if(tempDevIDInfo.nType == ePortTypeHID)
        {
            bRet = Req_OpenPort(tempDevIDInfo.nType, tempDevIDInfo.dwVid, tempDevIDInfo.dwPid);
        }

        if(bRet)
        {
            m_stDevIDInfo = tempDevIDInfo;
            break;
        }
    }


//    do
//    {
//        bRet = Req_OpenPort(tempDevIDInfo.nType, tempDevIDInfo.dwVid, tempDevIDInfo.dwPid);
//        if(bRet)
//            break;

//        else if (condition)
//        {

//        }
//        else
//        {
//            SAFE_DELETE(m_pPort);
//            tempDevIDInfo = g_devidarr[ePortTypeVCP];//HID模式打开失败后尝试以VCP模式打开
//        }

//        bRet = Req_OpenPort(tempDevIDInfo.nType, tempDevIDInfo.dwVid, tempDevIDInfo.dwPid, 115200);
//        if(bRet)
//            break;
//        else
//        {
//            SAFE_DELETE(m_pPort);
//        }

//    } while (0);

//    if(bRet)
//    {
//        m_stDevIDInfo = tempDevIDInfo;
//    }

    return bRet;
}

/**************************************************************************
* 函数名: Req_ClosePort
* 功能: 请求关闭端口
* 时间: 2015/10/19 9:23
* 作者: ZDJ
*/
BOOL CRobotController::Req_ClosePort()
{
    if(!m_pPort)
        return FALSE;

    if (m_pPort)
    {
        if(m_pPort->IsOpen())
            m_pPort->ClosePort();

        SAFE_DELETE(m_pPort);
    }

    return TRUE;
}

/**************************************************************************
* 函数名: Req_SwitchMode
* 功能: 请求切换模式
* 时间: 2015/10/19 9:23
* 作者: ZDJ
*/
BOOL CRobotController::Req_SwitchMode(emRobotSwitchMode eMode)
{
    m_eSwitchMode = eMode;
//    SendCmdData(eRobotCmdTypeSwitchMode, eMode);
    return TRUE;
}

//#pragma endregion

//#pragma region 消息响应

#if 0
/**************************************************************************
* 函数名: OnReceiveChar
* 功能: 接收字符
* 参数:
*    @[in ] wParam: 接收的字符长度
*    @[in ] lParam: 接收的字符
* 返回值: 执行结果
* 时间: 2015/09/08 11:19
* 作者: ZDJ
*/
LRESULT CRobotController::OnReceiveChar(WPARAM wParam, LPARAM lParam)
{

#if 1
    int nDataLen = wParam;
    byte *pDataTemp = (byte *)lParam;
    if(!pDataTemp || nDataLen <= 0)
    {
        return false;
    }

    for(int i = 0; i < nDataLen; i++)
    {
        byte nTemp = (byte)pDataTemp[i];
        int nCmdId = m_datapool.PushData((byte)pDataTemp[i]);
        if(nCmdId > -1)
        {
            byte* databuf = NULL;
            PKGDATA* pPkg = m_datapool.GetPkg(nCmdId);
            if(pPkg)
            {
                databuf = pPkg->GetByteData();
                int nDataLen = pPkg->GetSize();
                m_datapool.ClearData(nCmdId);

                if(databuf)
                {
                    ProcessData(databuf, nDataLen);
                    SAFE_DELETE_ARRAY(databuf);
                }
            }
            else
            {
                m_datapool.ClearData(nCmdId);
            }
        }

    }

    SAFE_DELETE_ARRAY(pDataTemp);
    return TRUE;

#else
    int nCmdId = m_datapool.PushData(lParam);
    if(nCmdId > -1)
    {
        //Sleep(50);
        byte* databuf = NULL;
        PKGDATA* pPkg = m_datapool.GetPkg(nCmdId);
        if(pPkg)
        {
            databuf = pPkg->GetByteData();
            int nDataLen = pPkg->GetSize();
            m_datapool.ClearData(nCmdId);

            if(databuf)
            {
                ProcessData(databuf, nDataLen);
                SAFE_DELETE_ARRAY(databuf);
            }
        }
        else
        {
            m_datapool.ClearData(nCmdId);
        }
    }
    return TRUE;
#endif

}
//#pragma endregion
#endif

#ifdef Q_OS_WIN
/**************************************************************************
* 函数名: UpdateDevice
* 功能: 设备更新通知
* 时间: 2015/07/22 10:25
* 作者: ZDJ
*/
void CRobotController::UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
{
    //ASSERT(lstrlen(pDevInf->dbcc_name) > 4);
    QString szDevId = QString::fromWCharArray(pDevInf->dbcc_name+4);
    int idx = szDevId.lastIndexOf('#');
    //ASSERT( -1 != idx );
    szDevId.truncate(idx);
    szDevId.replace('#', '\\');
    szDevId.toUpper();

#ifndef QT_NO_DEBUG
    qDebug() << "szDevId:" << szDevId;
#endif

    idx = szDevId.indexOf('\\');
    //ASSERT(-1 != idx );
    QString szClass = szDevId.left(idx);

    if("ROOT" == szClass)
    {
        return;
    }

    int nTempVID = 0;
    int nTempPID = 0;
    int nTempREV = 0;

    if(szDevId.indexOf("\\VID_") != -1 && szDevId.indexOf("&PID_") != -1)
    {
        QString sTemp = szDevId.mid(szDevId.indexOf("VID_"));
        sscanf(sTemp.toStdString().c_str(), "VID_%04X&PID_%04X", &nTempVID, &nTempPID);

#ifndef QT_NO_DEBUG
        qDebug() << "sTemp:" << sTemp;
        qDebug() << "nTempVID:" << nTempVID << " nTempPID:" << nTempPID;
#endif
    }

    if((nTempVID == g_devidarr[ePortTypeHID].dwVid && nTempPID == g_devidarr[ePortTypeHID].dwPid)
        || (nTempVID == g_devidarr[ePortTypeVCP].dwVid && nTempPID == g_devidarr[ePortTypeVCP].dwPid)
        )
    {
        if(DBT_DEVICEARRIVAL == wParam )
        {
            if(m_pPort && m_pPort->IsOpen())
                return;
            StartConncectTimer();
            qDebug("StartConncectTimer");
        }
        else
        {
            if(!m_pPort || (m_pPort && !m_pPort->IsOpen()))
                return;
            StartDisconncectTimer();
            //SetTimer(g_nDisconnectPortTimerID, TIMEDELAY_CONNECTROBOT, NULL);
            // USB拔出后关闭串口

            qDebug("StartDisconncectTimer");
            //Req_ClosePort();
            //NotifyObservers(WM_MSG_PORT_OPENSTATECHANGE, NULL, NULL, FALSE);
        }
    }
}
#endif


void CRobotController::timerEvent(QTimerEvent *event)
{
    int nIDEvent = event->timerId();
    killTimer(nIDEvent);
    if(nIDEvent == m_nConnectPortTimerID)
    {
        m_nConnectPortTimerID = 0;
        BOOL bRet = Req_AutoOpenPort();
        NotifyObservers(WM_MSG_ROBOT_CONNECT, TRUE, bRet, NULL);
    }
    else if(nIDEvent == m_nDisconnectPortTimerID)
    {
        m_nDisconnectPortTimerID = 0;
        Req_ClosePort();
        NotifyObservers(WM_MSG_ROBOT_CONNECT, TRUE, FALSE, NULL);
    }
}

//void CRobotController::customEvent(QEvent *event)
//{
////    zxEvent* pEvent = (zxEvent*)event;
////    if(event->type() == WM_MSG_RXCHAR)
////        OnReceiveChar(pEvent->wParam, pEvent->lParam);
//}
/**************************************************************************
* 函数名: GetPortOpenState
* 功能: 获取端口打开状态
* 时间: 2015/10/20 11:09
* 作者: ZDJ
*/
BOOL CRobotController::GetPortOpenState()
{
    if(m_pPort)
        return m_pPort->IsOpen();
    return FALSE;
}

#ifdef Q_OS_WIN
/**************************************************************************
* 函数名: FindMySTMDevice
* 功能: 查找STM设备
* 时间: 2015/11/01 16:30
* 作者: ZDJ
*/
BOOL CRobotController::FindMySTMDevice()
{
    DWORD dwFlag = DIGCF_ALLCLASSES | DIGCF_PRESENT;
    HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, L"USB", NULL,dwFlag);
    if( INVALID_HANDLE_VALUE == hDevInfo )
    {
        return FALSE;
    }

    SP_DEVINFO_DATA spDevInfoData;
    spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
    {
        DWORD nSize = 0 ;
        WCHAR buf[MAX_PATH] = {0};

        DWORD DataT ;
        if(SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buf, sizeof(buf), &nSize))
        {
            QString  str = QString::fromWCharArray(buf);

#ifndef QT_NO_DEBUG
            qDebug() << "SetupDiGetDeviceRegistryProperty:str:" << str;
#endif

            QString strGetName1 = str.left(str.indexOf('('));

            if (strGetName1.indexOf(STM_DEVICE_NAME_NAME) != -1)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}
#endif

/**************************************************************************
* 函数名: StartConncectTimer
* 功能: 启动连接端口的时钟
* 时间: 2015/11/01 16:45
* 作者: ZDJ
*/
void CRobotController::StartConncectTimer()
{
    m_nConnectPortTimerID = startTimer(TIMEDELAY_CONNECTROBOT, Qt::PreciseTimer);
}

/**************************************************************************
* 函数名: StartDisconncectTimer
* 功能: 启动断开端口的时钟
* 时间: 2015/11/01 16:45
* 作者: ZDJ
*/
void CRobotController::StartDisconncectTimer()
{
    m_nDisconnectPortTimerID = startTimer(TIMEDELAY_DISCONNECTROBOT, Qt::PreciseTimer);
}

//int CRobotController::portRecvData(void *pUser, byte *pData, int nDataLen)
//{
//    if(pUser && pData && (nDataLen > 0))
//    {
//        CRobotController* pController = (CRobotController*)pUser;
//        pController->OnReceiveChar(nDataLen,  (LPARAM)pData);
//    }

//    return nDataLen;
//}

#ifdef Q_OS_WIN

/**************************************************************************
* 函数名: OnDeviceChange
* 功能: 设备更改通知
* 时间: 2015/11/16 10:57
* 作者: ZDJ
*/
bool CRobotController::OnDeviceChange(MSG *msg,long *result)
{
    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        qDebug() << "receive ondevicechange event "<< msg->message <<  msg->wParam<<endl;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        if(lpdb && lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
        {
            switch(msg->wParam)
            {
            case DBT_DEVICETYPESPECIFIC:
#ifndef QT_NO_DEBUG
                qDebug() << "DBT_DEVICETYPESPECIFIC " ;
#endif
                break;
            case DBT_DEVICEARRIVAL:
            case DBT_DEVICEREMOVECOMPLETE:
                UpdateDevice((PDEV_BROADCAST_DEVICEINTERFACE)lpdb, msg->wParam);
                break;
            default:
#ifndef QT_NO_DEBUG
                qDebug() << "DBT OTHER" ;
#endif
                break;
            }
        }
    }
    return false;
}

/**************************************************************************
* 函数名: slotTimerTimeOut
* 功能: 定时器超时
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void CRobotController::slotTimerTimeOut()
{

}

#endif


/**************************************************************************
* 函数名: SendData
* 功能: 发送数据
* 参数:
*    @[in ] pData: 数据缓冲区
*    @[in ] nDataLen: 数据长度
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2018/04/12 9:49
* 作者: Joker
*/
BOOL CRobotController::SendData(byte* pData, int nDataLen, BOOL bIsSleep)
{
    QDateTime timeBefore = QDateTime::currentDateTime();   //获取当前时间
    qint64 nTimeBefore = timeBefore.toMSecsSinceEpoch();   //将当前时间转为时间戳(ms)
    qDebug(" CRobotController::SendDat nTimeBefore = %lld m_nLastTimeMs = %lld\n", nTimeBefore, m_nLastTimeMs);
    qint64 nSleepTime = 0;
    if (nTimeBefore - m_nLastTimeMs > 0)
    {
         qint64 nTimeLeft = nTimeBefore - m_nLastTimeMs;
         if (nTimeLeft < UBX_DELAY_TIME)
         {
             nSleepTime = UBX_DELAY_TIME - nTimeLeft;
         }

    }
    else
    {
        nSleepTime = m_nLastTimeMs - nTimeBefore + UBX_DELAY_TIME;
    }
    if (bIsSleep)
    {
        Sleep(nSleepTime);
        qDebug(" CRobotController::SendDat nSleepTime = %lld", nSleepTime);
    }

    if (m_pPort)
    {
        m_pPort->SendData(pData, nDataLen);

    }
    else
    {
         return FALSE;
    }


    QDateTime timeAfter = QDateTime::currentDateTime();   //获取当前时间
    qint64 nTimeAfter = timeAfter.toMSecsSinceEpoch();   //将当前时间转为时间戳(ms)
//    qint64 nSendTime = nDataLen;//(nDataLen * 0.96 ~= 1)
    m_nLastTimeMs = nTimeAfter;/* + nSendTime;*/
    qDebug(" CRobotController::SendDat nTimeAfter = %lld m_nLastTimeMs = %lld\n", nTimeAfter, m_nLastTimeMs);
    return TRUE;
}
