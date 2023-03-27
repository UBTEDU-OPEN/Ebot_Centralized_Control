/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：MyHidClass.cpp
* 创建时间：2015/10/23 19:42
* 文件标识：
* 文件摘要：HID通信类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/10/23 19:42
* 版本摘要：
*/
#include "stdafx.h"
#include "MyHidClass.h"
#include "RobotController.h"
#include "alpharobotlog.h"

const int g_nRecvBufSize = 512;
const UINT WM_MSG_RXCHAR = QEvent::registerEventType();         //接收字符


CMyHidClass::CMyHidClass(QObject *hListener)
	: IMyPortInterface(hListener, ePortTypeHID)
{
	m_pHidDev = NULL;
	m_bRecv = FALSE;
	m_hThreadRecv = NULL;	
}


CMyHidClass::~CMyHidClass(void)
{	
}

/**************************************************************************
* 函数名: Init
* 功能: 初始化
* 时间: 2015/10/27 19:49
* 作者: ZDJ
*/
BOOL CMyHidClass::Init()
{
 	return TRUE;	
}

/**************************************************************************
* 函数名: OpenPort
* 功能: 打开端口
* 参数: 
*    @[in ] dwVID: dwVID值
*    @[in ] dwPID: dwPID值
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/10/23 19:43
* 作者: ZDJ
*/
BOOL CMyHidClass::OpenPort(DWORD dwVID, DWORD dwPID)
{	
	BOOL bRet = FALSE;

	do 
    {
		m_pHidDev = hid_open(dwVID, dwPID, NULL);
		if (!m_pHidDev)
			break;

//		if(!StartMonitoring())
//			break;

		bRet = TRUE;
	} while (0);

	return bRet;
}

/**************************************************************************
* 函数名: ClosePort
* 功能: 关闭端口
* 时间: 2015/10/26 10:43
* 作者: ZDJ
*/
BOOL CMyHidClass::ClosePort()
{
//	StopMonitoring();

	if(m_pHidDev)
	{
		hid_close(m_pHidDev);
		m_pHidDev = NULL;
	}

	return TRUE;
}

/**************************************************************************
* 函数名: ThreadRecv
* 功能: 接收数据线程执行函数
* 参数: 
*    @[in ] lpParam: 线程参数
* 返回值: 执行结果
* 时间: 2015/10/23 20:17
* 作者: ZDJ
*/
#ifdef Q_OS_WIN
DWORD __stdcall CMyHidClass::ThreadRecv(LPVOID lpParam)
#else
void *CMyHidClass::ThreadRecv(LPVOID lpParam)
#endif
{	
	CMyHidClass* pMyHid = (CMyHidClass*)lpParam;
    //hid_set_nonblocking(pMyHid->m_pHidDev, 1);
	
	byte szRecvBuffer[g_nRecvBufSize] = {0};

	while(pMyHid->m_bRecv)
	{			
		memset(szRecvBuffer, 0, sizeof(szRecvBuffer));

        int nRealRecved = hid_read(pMyHid->m_pHidDev, szRecvBuffer, sizeof(szRecvBuffer));

        ///////////////////////////////Log/////////////////////////////
       QString sLog = "";
        if (nRealRecved > 0)
        {
              sLog += QString("Hid received byte numer: %1\n").arg(nRealRecved);
        }
        for(int i = 0; i < nRealRecved; i++)
        {
            QString sTemp;
            sTemp.sprintf("%02X ", (byte)szRecvBuffer[i]);
            sLog += sTemp;
        }
        AlphaRobotLog::getInstance()->printDebug(sLog);
        ///////////////////////////////Log/////////////////////////////

		if(nRealRecved == 0)
		{	
			//TRACE(_T("dwRealRecv == 0\n"));			
		}
		else if(nRealRecved == -1)
		{
			//接收出错
			//TRACE(_T("dwRealRecv == -1\n"));			
		}
		else
		{
			//TRACE(_T("dwRealRecv > 0\n"));
			for(int i = 0; i < nRealRecved; i++)
			{
//                zxEvent* pNewEvent = new zxEvent(WM_MSG_RXCHAR);
//                pNewEvent->eEventType = zxEventTypeWindow;
//                pNewEvent->lReceiver = (long)pMyHid->m_hListener;
//                pNewEvent->dwMsgId = WM_MSG_RXCHAR;
//                pNewEvent->wParam = 1;
//                pNewEvent->lParam = szRecvBuffer[i];
//                QCoreApplication::postEvent(pMyHid->m_hListener, pNewEvent);

                QtPostMessage(pMyHid->m_hListener, WM_MSG_RXCHAR, NULL, 1, szRecvBuffer[i]);
			}
		}

        //Sleep(5);
	}

	return 0;
}

/**************************************************************************
* 函数名: StartMonitoring
* 功能: 启动监视
* 时间: 2015/10/26 15:57
* 作者: ZDJ
*/
BOOL CMyHidClass::StartMonitoring()
{    
    m_bRecv = TRUE;

#ifdef Q_OS_WIN
    m_hThreadRecv = CreateThread(NULL, 0, ThreadRecv, this, 0, NULL);
#else    
    pthread_create(&m_hThreadRecv, NULL, ThreadRecv, this);
#endif
    return m_hThreadRecv != NULL;
}

/**************************************************************************
* 函数名: StopMonitoring
* 功能: 停止监视
* 时间: 2015/10/26 15:57
* 作者: ZDJ
*/
BOOL CMyHidClass::StopMonitoring()
{
    Req_QuitRecvThread();

#ifdef Q_OS_WIN
    if(m_hThreadRecv)
    {        
        WaitForSingleObject(m_hThreadRecv, 1000);
        SAFE_CLOSEHANDLE(m_hThreadRecv);
    }
#else
    pthread_join(m_hThreadRecv, NULL);
    m_hThreadRecv = NULL;
#endif
	return TRUE;
}

/**************************************************************************
* 函数名: SendData
* 功能: 发送数据
* 参数: 
*    @[in ] pData: 数据缓冲区
*    @[in ] nDataLen: 数据长度
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/10/24 9:49
* 作者: ZDJ
*/
BOOL CMyHidClass::SendData(byte* pData, int nDataLen)
{
	int nTempDataLen = nDataLen + 1;
	byte* pTempData = new byte[nTempDataLen];
	pTempData[0] = 0; //第一字节数据预留为报告ID
	memcpy(pTempData + 1, pData, nDataLen);

    AlphaRobotLog::getInstance()->printDebug("xxxxxxxxxxxxxxx Before HID write");

	int nWrited = hid_write(m_pHidDev, pTempData, nTempDataLen);
	SAFE_DELETE_ARRAY(pTempData);

	//if(nWrited == -1)
	//	return FALSE;

#ifndef QT_NO_DEBUG
    QString sInfo = "HID sended ";
	for(int i = 0; i < nDataLen; i++)
	{
        QString sTemp;
        sTemp.sprintf("%02X ", (byte)pData[i]);
        sInfo += sTemp;
	}    
    qDebug(sInfo.toStdString().c_str());
#endif

   ////////////////////////////////////////////////Log//////////////
    QString sLog = "HID sended ";
    for(int i = 0; i < nDataLen; i++)
    {
        QString sTemp;
        sTemp.sprintf("%02X ", (byte)pData[i]);
        sLog += sTemp;
    }
    AlphaRobotLog::getInstance()->printDebug(sLog);
    ////////////////////////////////////////////////Log//////////////

    if(g_bIsRecordLogValue)
    {
        QString sInfo = "HID sended ";
        for(int i = 0; i < nDataLen; i++)
        {
            QString sTemp;
            sTemp.sprintf("%02X ", (byte)pData[i]);
            sInfo += sTemp;
        }
        sInfo += '\n';
        AlphaRobotLog::getInstance()->printInfo(sInfo);
    }

	return TRUE;
}

/**************************************************************************
* 函数名: IsOpen
* 功能: 端口是否打开
* 时间: 2015/10/26 11:24
* 作者: ZDJ
*/
BOOL CMyHidClass::IsOpen()
{
	return m_pHidDev != NULL;
}

#ifdef Q_OS_MAC
/**************************************************************************
* 函数名: SetArrivalCbk
* 功能: 设置设备到达消息回调地址
* 参数:
*    @[in ] pCbk：回调地址
* 返回值: void
* 时间: 2016/01/16 17:26
* 作者: ZDJ
*/
void CMyHidClass::SetArrivalCbk(IOHIDDeviceCallback pCbk)
{
    hid_arrivalcbk_set(pCbk);
}

/**************************************************************************
* 函数名: SetRemovalCbk
* 功能: 设置设备移除消息回调地址
* 参数:
*    @[in ] pCbk：回调地址
* 返回值: void
* 时间: 2016/01/16 17:26
* 作者: ZDJ
*/
void CMyHidClass::SetRemovalCbk(IOHIDDeviceCallback pCbk)
{
    hid_removalcbk_set(pCbk);
}
#endif

/**************************************************************************
* 函数名: Req_QuitRecvThread
* 功能: 请求退出接收线程
* 时间: 2016/03/22 10:57
* 作者: ZDJ
*/
void CMyHidClass::Req_QuitRecvThread()
{
    m_bRecv = FALSE;

    byte buf[] = "quit";
    hid_write(m_pHidDev, buf, sizeof(buf));
}
