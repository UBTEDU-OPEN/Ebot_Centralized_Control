/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：RobotController.h
* 创建时间：2015/10/18 14:39
* 文件标识：
* 文件摘要：机器人控制类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/10/18 14:39
* 版本摘要：
*/
#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QMutex>
#include <QMutexLocker>

#include "DataPool.h"
#include "IMyPortInterface.h"
#include "MyHidClass.h"
#include <QWidget>
#include "../zxlib/alpharobotlog.h"

#include "../UBTUtil/ubtutil.h"
#include "dataprotocol.h"


#ifdef Q_OS_WIN
#include "SerialPort.h"
#endif

//端口类型
enum emPortType
{
    ePortTypeVCP,  //虚拟串口
    ePortTypeHID,  //HID
};

typedef struct DEVIDTYPE
{
        int   nType;   //端口类型
        DWORD dwVid;   //vid
        DWORD dwPid;   //pid
        DEVIDTYPE(int type, DWORD vid, DWORD pid)
        {
                nType = type;
                dwVid = vid;
                dwPid = pid;
        }
        STRUCT_INIT(DEVIDTYPE)
}DEVIDTYPE;

extern UBXROBOTCONTROLLER_API DEVIDTYPE    g_devidarr[];

//切换模式
enum emRobotSwitchMode
{
        eRobotSwitchModeUdisk = 1,
        eRobotSwitchModeVCP,
        eRobotSwitchModeHID
};

extern UBXROBOTCONTROLLER_API const UINT WM_MSG_ROBOT_CONNECT;            //连接机器人
extern UBXROBOTCONTROLLER_API const UINT WM_MSG_ROBOT_DISCONNECT;         //断开机器人

const QString STM_DEVICE_NAME_NAME = "Prolific USB-to-Serial Comm Port";//要查找的STM设备名称字符串

class UBXROBOTCONTROLLER_API CRobotController :  public QWidget, public zxObserverSubjectBase
{
        Q_OBJECT

public:	
        explicit CRobotController(QWidget* parent = NULL);
        ~CRobotController();

//#pragma region

public:	

        /**************************************************************************
        * 函数名: GetPortOpenState
        * 功能: 获取端口打开状态
        * 时间: 2015/10/20 11:09
        * 作者: ZDJ
        */
        BOOL GetPortOpenState();

        /**************************************************************************
        * 函数名: getPortInterface
        * 功能: 获取数据传输的接口
        * 时间: 2016/07/05
        * 作者: hwx
        */
        IMyPortInterface *getPortInterface() { return m_pPort; }

        /**************************************************************************
        * 函数名: StartConncectTimer
        * 功能: 启动连接端口的时钟
        * 时间: 2015/11/01 16:45
        * 作者: ZDJ
        */
        void StartConncectTimer();

        /**************************************************************************
        * 函数名: StartDisconncectTimer
        * 功能: 启动断开端口的时钟
        * 时间: 2015/11/01 16:45
        * 作者: ZDJ
        */
        void StartDisconncectTimer();

#ifdef Q_OS_WIN
        /**************************************************************************
        * 函数名: OnDeviceChange
        * 功能: 设备更改通知
        * 时间: 2015/11/16 10:57
        * 作者: ZDJ
        */
        bool OnDeviceChange(MSG *msg, long *result);
#endif

public slots:
    /**************************************************************************
    * 函数名: slotTimerTimeOut
    * 功能: 定时器超时
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void slotTimerTimeOut();

private:
        /**************************************************************************
        * 函数名: RegisterDeviceNotify
        * 功能: 注册设备通知
        * 时间: 2015/09/08 10:08
        * 作者: ZDJ
        */
        void RegisterDeviceNotify();

#ifdef Q_OS_WIN
        /**************************************************************************
        * 函数名: UpdateDevice
        * 功能: 设备更新通知
        * 时间: 2015/07/22 10:25
        * 作者: ZDJ
        */
        void UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam);
#endif


#ifdef Q_OS_WIN
        /**************************************************************************
        * 函数名: FindMySTMDevice
        * 功能: 查找STM设备
        * 时间: 2015/11/01 16:30
        * 作者: ZDJ
        */
        BOOL FindMySTMDevice();
#endif
//        /**************************************************************************
//        * 函数名: slotOnDeviceRemoved
//        * 功能: 拔出USB的槽函数
//        * 参数:
//        * 返回值: void
//        * 时间: 2016/11/22
//        * 作者: hwx
//        */
//        void slotOnDeviceRemoved();
//        void slotOnRobotProductTypeChange(int eUbtProductTypeOld, int eUbtProductTypeNew);

//#pragma endregion

//signals:
//        /**************************************************************************
//        * 函数名: sigOnSendCmd
//        * 功能: 发送机器人指令的信号
//        * 参数:
//        *    @[in ] oCmdType: 指令信息
//        * 返回值: void
//        * 时间: 2016/11/22
//        * 作者: hwx
//        */
//        void sigOnSendCmd(CCommandType oCmdType);

//        // 拔出USB
//        void sigOnDeviceRemoved();

//#pragma region 请求

public:	
        /**************************************************************************
        * 函数名: Req_Connect
        * 功能: 请求连接指定端口的机器人
        * 时间: 2015/10/18 14:54
        * 作者: ZDJ
        */
        bool Req_Connect();

        /**************************************************************************
        * 函数名: Req_Disconnect
        * 功能: 请求断开机器人的连接
        * 时间: 2015/10/18 14:55
        * 作者: ZDJ
        */
        void Req_Disconnect(void);

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
        BOOL Req_OpenPort(int nPortType, DWORD dwVID, DWORD dwPID, ...);

        /**************************************************************************
        * 函数名: Req_AutoOpenPort
        * 功能: 请求自动打开端口
        * 返回值: 成功返回TRUE，失败返回FALSE
        * 时间: 2015/10/21 20:49
        * 作者: ZDJ
        */
        BOOL Req_AutoOpenPort();

        /**************************************************************************
        * 函数名: Req_ClosePort
        * 功能: 请求关闭端口
        * 时间: 2015/10/19 9:23
        * 作者: ZDJ
        */
        BOOL Req_ClosePort();

        /**************************************************************************
        * 函数名: Req_SwitchMode
        * 功能: 请求切换模式
        * 时间: 2015/10/19 9:23
        * 作者: ZDJ
        */
        BOOL Req_SwitchMode(emRobotSwitchMode eMode);

//#pragma endregion

//#pragma region 窗口消息响应        

//        /**************************************************************************
//        * 函数名: OnReceiveChar
//        * 功能: 接收字符
//        * 参数:
//        *    @[in ] wParam: 接收的字符长度
//        *    @[in ] lParam: 接收的字符
//        * 返回值: 执行结果
//        * 时间: 2015/09/08 11:19
//        * 作者: ZDJ
//        */
//        LRESULT OnReceiveChar(WPARAM wParam, LPARAM lParam);

        virtual void timerEvent(QTimerEvent *event);
//        virtual void customEvent(QEvent *event);

#ifdef Q_OS_MAC

        /**************************************************************************
        * 函数名: OnDeviceArrival
        * 功能: 设备接入通知
        * 时间: 2015/11/16 10:57
        * 作者: ZDJ
        */
        static void OnDeviceArrival(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);

        /**************************************************************************
        * 函数名: OnDeviceRemoval
        * 功能: 设备移除通知
        * 时间: 2015/11/16 10:57
        * 作者: ZDJ
        */
        static void OnDeviceRemoval(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
#endif
//#pragma endregion

//public:
//    static int portRecvData(void* pUser, byte* pData, int nDataLen);

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
        BOOL SendData(byte *pData, int nDataLen, BOOL bIsSleep = TRUE);
private:
//        IRobotCmd* m_pRobotCmd; //机器人命令类
//        CRobotCmd  m_objRobotCmdOfPCOld;   //机器人命令类
//        RobotCmdBTProtocol m_objRobotCmdOfBTProtocol;


        IMyPortInterface*  m_pPort;         //端口实例
        CDataPool          m_datapool;      //数据缓冲池
        DEVIDTYPE          m_stDevIDInfo;   //当前端口id数据

        UINT          m_nConnectPortTimerID;     //连接端口时钟id
        UINT          m_nDisconnectPortTimerID;  //断开端口时钟id

        emRobotSwitchMode m_eSwitchMode; // 切换模式

        static CRobotController* m_pControllerStatic;


//        CRobotCommunicateThread m_oRobotCommunicateThread; // 和机器人通信的线程类

        QMutex m_mutex; // 串口状态锁

        qint64              m_nLastTimeMs;
        QTimer*             m_pConnectTimer;
};

#endif

