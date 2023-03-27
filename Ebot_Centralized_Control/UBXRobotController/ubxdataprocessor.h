/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXDataProcessor.h
* 创建时间：2016/07/08 17:37
* 文件标识：
* 文件摘要：数据处理类
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2016/07/08 17:37                                         
* 版本摘要：
*/
#ifndef UBXDATAPROCESSOR_H
#define UBXDATAPROCESSOR_H

#include <QtNetWork>
#include <QObject>
#include "dataprotocol.h"
#include "Robot/DataPool.h"
#include "UBXRobotController.h"
#include <QThread>

typedef void (*PFUNC_RECEIVEDATA)(byte* pData, int nDataLen); //接收到数据函数类型

class UBXROBOTCONTROLLER_API UBXDataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit UBXDataProcessor(QObject *parent = 0);
    ~UBXDataProcessor();

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
    bool Req_PackageData(int nCmdId, byte* pDataBuf, int nDataLen, vector<MYSERIALDATA *> &vecPkgList);

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
    int Req_PackageBeginSendFileData(byte *pDataBuf, int nDataLen, ushort nFrameCount, byte*& pOutData);

    /**************************************************************************
    * 函数名: Req_PackageEndSendFileData
    * 功能: 请求打包退出传输文件模式数据
    * 参数:
    *    @[in ] pDataBuf: 数据缓冲区地址
    *    @[in ] nDataLen: 数据缓冲区长度
    *    @[out ] pOutData:    数据缓冲区地址
    * 返回值: 成功返回数据缓冲区地址
    * 时间: 2018/03/08 17:34
    * 作者: Joker
    */
    int Req_PackageEndSendFileData(byte *pDataBuf, int nDataLen, ushort nFrameCount, byte*& pOutData);


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
    bool Req_PackageFile(int nCmdId, const QString& sFileName, byte* pDataBuf, int nDataLen, vector<MYSERIALDATA *> &vecPkgList);


    /**************************************************************************
    * 函数名: clearDataCache
    * 功能: 清理已处理数据的缓存
    * 时间: 2016/07/10 14:34
    * 作者: ZDJ
    */
    void clearDataCache();

    /**************************************************************************
    * 函数名: clearDataPool
    * 功能: 清理待处理数据的缓存
    * 时间: 2016/07/10 14:34
    * 作者: ZDJ
    */
    void clearDataPool();

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
    bool Req_SendData(int nCmdId, byte *pDataBuf, int nDataLen, int nDelayTime = 1000);


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
    bool Req_SendFile(int nCmdId, byte *pDataBuf, int nDataLen);

    /**************************************************************************
    * 函数名: Print_Data
    * 功能: 打印数据
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void Print_Data(byte* pData, byte nLen);
private:

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
    bool PackageData(int nCmdId, byte* pDataBuf, int nDataLen, vector<MYSERIALDATA *>& vecPkgList);


signals:
    /**************************************************************************
    * 函数名: sigReceivePackage
    * 功能: 收到数据包
    * 时间: 2016/07/08 17:34
    * 作者: ZDJ
    */
    void sigReceivePackage(int nCmdId, QByteArray bytesData);

    /**************************************************************************
    * 函数名: sigUpdateProgress
    * 功能: 更新进度
    * 时间: 2016/07/08 17:34
    * 作者: ZDJ
    */
    void sigUpdateProgress(int nCmdId, int nBytesLen);

public slots:

private:
    CDataPool  m_datapool;  //数据缓冲池
    VecByteDataType  m_vecDataCache; //已处理的数据缓存

    int        m_nPlanProcessDataSize; //计划处理的数据长度
    qint64     m_nProcessDataStartTime; //处理数据的开始时刻, 单位毫秒

    bool       m_bCancel;   //取消标识

};

#endif // UBXDATAPROCESSOR_H
