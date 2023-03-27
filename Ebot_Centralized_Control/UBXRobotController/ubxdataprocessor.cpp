/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXDataProcessor.cpp
* 创建时间：2016/07/08 17:37
* 文件标识：
* 文件摘要：数据处理类
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2016/07/08 17:37
* 版本摘要：
*/
#include "ubxdataprocessor.h"
#include "CRC32.h"
#include "ubtlib.h"

UBXDataProcessor::UBXDataProcessor(QObject *parent)
    : QObject(parent)
    , m_nPlanProcessDataSize(0)
    , m_nProcessDataStartTime(0)
    , m_bCancel(false)
{
}

UBXDataProcessor::~UBXDataProcessor()
{
}

/**************************************************************************
* 函数名: PackageData
* 功能: 打包数据
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
*    @[out] vecPkgList: 分包数据列表
* 返回值: 成功返回true，失败返回false
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
bool UBXDataProcessor::PackageData(int nCmdId, byte* pDataBuf, int nDataLen, vector<MYSERIALDATA*>& vecPkgList)
{
    int nPkgCount = (nDataLen - 1) / UBXPROTOCOL_PACKAGE_SIZE + 1;
    int nDataOffset = 0;

    m_nPlanProcessDataSize = nDataLen; //记录要处理的数据长度
    m_nProcessDataStartTime = QDateTime::currentMSecsSinceEpoch(); //单位毫秒

    for(short i = 0; i < nPkgCount; i++)
    {
        byte* pFrameBuf = NULL;
        int nCopySize = UBXPROTOCOL_PACKAGE_SIZE;

        if(i == nPkgCount - 1)
        {
            if(nDataLen > 0)
            {
                if(nDataLen % UBXPROTOCOL_PACKAGE_SIZE != 0)
                {
                    nCopySize = nDataLen % UBXPROTOCOL_PACKAGE_SIZE;
                }
            }
            else
            {
                nCopySize = 0;
            }
        }

        int nFrameLen = PackageFrame(pDataBuf + nDataOffset, nCopySize, nCmdId , i + 1, pFrameBuf);
        MYSERIALDATA* pMyData = new MYSERIALDATA(pFrameBuf, nFrameLen);
        vecPkgList.push_back(pMyData);

        nDataOffset += nCopySize;

#if 0
        g_log.AddLog(pFrameBuf, nFrameLen);
        g_log.AddLog("\n", false);
#endif

        //        SAFE_DELETE_ARRAY(pFrameBuf);
    }

    return true;
}

/**************************************************************************
* 函数名: Req_PackageData
* 功能: 请求打包数据
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
*    @[out] vecPkgList: 分包数据列表
* 返回值: 成功返回true，失败返回false
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
bool UBXDataProcessor::Req_PackageData(int nCmdId, byte* pDataBuf, int nDataLen, vector<MYSERIALDATA*>& vecPkgList)
{
    return PackageData(nCmdId, pDataBuf, nDataLen, vecPkgList);
}

/**************************************************************************
* 函数名: Req_PackageBeginSendFileData
* 功能: 请求打包进入传输文件模式数据
* 参数:
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
*    @[out ] pOutData:    数据缓冲区地址
* 返回值: 成功返回数据缓冲区地址
* 时间: 2018/03/08 17:34
* 作者: Joker
*/
int UBXDataProcessor::Req_PackageBeginSendFileData(byte *pDataBuf, int nDataLen, ushort nFrameCount, byte*& pOutData)
{
    qDebug()<<"123:len"<< nDataLen<<endl;
    byte nFileNameLen = nDataLen;
    qDebug()<<"123:nFileNameLen"<< nFileNameLen<<endl;
    int nOutLen = nDataLen + sizeof(ushort) + sizeof(byte);
    qDebug()<<"nOutLen :"<< nOutLen<<endl;
    pOutData = new byte[nOutLen];

    pbyte pDataCursor = pOutData;

    memcpy(pDataCursor, &nFileNameLen, sizeof(byte));
    pDataCursor+=sizeof(byte);

    if(pDataBuf && nDataLen > 0)
    {
        memcpy(pDataCursor, pDataBuf, nDataLen);
        pDataCursor += nDataLen;
    }

    memcpy(pDataCursor, &nFrameCount, sizeof(ushort));
    qDebug()<<"UBXDataProcessor::Req_PackageBeginSendFileData :"<<endl;
    //    Print_Data(pOutData, nOutLen);
    return nOutLen;
}


/**************************************************************************
* 函数名: Req_PackageFile
* 功能: 请求打包文件数据
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] sFileName: 文件名称
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
*    @[out] vecPkgList: 分包数据列表
* 返回值: 成功返回true，失败返回false
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
bool UBXDataProcessor::Req_PackageFile(int nCmdId, const QString& sFileName, byte* pDataBuf, int nDataLen, vector<MYSERIALDATA*>& vecPkgList)
{
    QByteArray bytesFileName = sFileName.toUtf8();
    int nFileNameLen = bytesFileName.size();
    int nNewLen = sizeof(int) + nFileNameLen + sizeof(int) + nDataLen; //文件名长度4B +　文件名nB　+  文件数据长度4B + 文件数据nB

    byte* pNewBuf = new byte[nNewLen];
    byte* pTempBuf = pNewBuf;

    memcpy(pTempBuf, &nFileNameLen, sizeof(int));
    pTempBuf += sizeof(int);

    memcpy(pTempBuf, bytesFileName.data(), nFileNameLen);
    pTempBuf += nFileNameLen;

    memcpy(pTempBuf, &nDataLen, sizeof(int));
    pTempBuf += sizeof(int);

    memcpy(pTempBuf, pDataBuf, nDataLen);
    pTempBuf += nDataLen;

    bool bRet = PackageData(nCmdId, pNewBuf, nNewLen, vecPkgList);
    SAFE_DELETE_ARRAY(pNewBuf);

    return bRet;
}
/**************************************************************************
* 函数名: Print_Data
* 功能: 打印数据
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void UBXDataProcessor::Print_Data(byte *pData, byte nLen)
{
    qDebug("Send Data:");
    for (int i = 0; i < nLen; i++)
    {
        qDebug("%02x ", pData[i]);
    }
    qDebug("\n");
}

/**************************************************************************
* 函数名: clearDataCache
* 功能: 清理已处理数据的缓存
* 时间: 2016/07/10 14:34
* 作者: ZDJ
*/
void UBXDataProcessor::clearDataCache()
{
    m_vecDataCache.clear();
    VecByteDataType().swap(m_vecDataCache);
}

/**************************************************************************
* 函数名: clearDataPool
* 功能: 清理待处理数据的缓存
* 时间: 2016/07/10 14:34
* 作者: ZDJ
*/
void UBXDataProcessor::clearDataPool()
{
    m_datapool.ClearAllData();
}

/**************************************************************************
* 函数名: Req_SendData
* 功能: 请求发送数据
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
* 返回值: 成功返回true，失败返回false
* 时间: 2018/03/08 17:34
* 作者: Joker
*/
bool UBXDataProcessor::Req_SendData(int nCmdId, byte* pDataBuf, int nDataLen, int nDelayTime)
{
    qDebug()<<"UBXDataProcessor::Req_SendData : nDataLen"<<nDataLen<<endl;
    bool bRet = false;
    for (int i = 0; i < UBX_RESEND_COUNT; i++)
    {
        byte* pData = NULL;
        m_bCancel = false;
//        clearDataCache();
//        clearDataPool();
        int nLen = Req_PackageCmdData(nCmdId, pDataBuf, nDataLen, nDelayTime, pData);
        if (nLen == -1)
        {
            break;
        }
        //        Print_Data(pData, nLen);
        if (g_pRobotController)
        {
            g_pRobotController->SendData(pData, nLen);
        }

        SAFE_DELETE_ARRAY(pData);
        if (nCmdId == UBXPROTOCOL_CMDID_PLAY || nCmdId == UBXPROTOCOL_CMDID_RESUME || nCmdId == UBXPROTOCOL_CMDID_PAUSE)
        {
            nDelayTime = nDelayTime - UBX_DELAY_TIME;
        }
        bRet = true;
//        int nSleepTime = 0;
//        if (i == UBX_RESEND_COUNT - 1)
//        {
//            nSleepTime = nLen - 50;
//        }
//        else
//        {
//            nSleepTime = nLen + 100;
//        }
//        Sleep(nSleepTime);
    }

    return bRet;
}


/**************************************************************************
* 函数名: Req_SendFile
* 功能: 请求发送文件
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
* 返回值: 成功返回true，失败返回false
* 时间: 2018/03/08 17:34
* 作者: Joker
*/
bool UBXDataProcessor::Req_SendFile(int nCmdId, byte *pDataBuf, int nDataLen)
{
    bool bRet = true;

    vector<MYSERIALDATA*> vecFrameList;

    m_bCancel = false;
//    clearDataCache();
//    clearDataPool();
    if(!Req_PackageData(nCmdId, pDataBuf, nDataLen, vecFrameList))
        return false;


    for(int i = 0; i < vecFrameList.size(); i++)
    {
        if(m_bCancel)
            break;

        MYSERIALDATA* pMyData = vecFrameList[i];
        for (int j = 0; j < UBX_RESEND_COUNT; j++)
        {
#if 1
            if (g_pRobotController)
            {
                bRet = g_pRobotController->SendData(pMyData->pData, pMyData->nDataLen);
            }
            if (!bRet)
            {
                m_bCancel = TRUE;
                break;
            }
#else
            byte* pDataSrv = pMyData->pData;
            BOOL bNeedSleep = TRUE;
            int nLen = pMyData->nDataLen;
            while(nLen > UBXPROTOCOL_PACKAGE_SIZE_HID)
            {
                nLen = nLen - UBXPROTOCOL_PACKAGE_SIZE_HID;
                byte* pData = new byte[UBXPROTOCOL_PACKAGE_SIZE_HID];
                memcpy(pData, pDataSrv, UBXPROTOCOL_PACKAGE_SIZE_HID);
                pDataSrv += UBXPROTOCOL_PACKAGE_SIZE_HID;
                if (g_pRobotController)
                {
                    g_pRobotController->SendData(pData, UBXPROTOCOL_PACKAGE_SIZE_HID, bNeedSleep);
                }

                if (bNeedSleep)
                {
                    bNeedSleep = FALSE;
                }
            }

            if (nLen > 0)
            {
                if (g_pRobotController)
                {
                    g_pRobotController->SendData(pDataSrv, nLen, bNeedSleep);
                }
            }
#endif


        }
    }

    ReleaseVector(vecFrameList);

    return bRet;
}
