/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：DataPool.h
* 创建时间：2015/07/22 16:22
* 文件标识：
* 文件摘要：数据缓冲池
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/07/22 16:22
* 版本摘要：
*/
#ifndef DATAPOOL_H
#define DATAPOOL_H

#include "../../common/UBXPublic.h"
#include "dataprotocol.h"


//自定义数据结构
typedef struct MYSERIALDATA
{
    MYSERIALDATA()
    {
        pData = NULL;
        nDataLen = 0;
    }

    MYSERIALDATA(byte* pBuf, int nLen)
    {
        pData = new byte[nLen];
        memcpy(pData, pBuf, nLen);
        nDataLen = nLen;
    }

    ~MYSERIALDATA()
    {
        SAFE_DELETE_ARRAY(pData);
        nDataLen = 0;
    }

    byte* pData;
    int   nDataLen;
private:
	MYSERIALDATA(const MYSERIALDATA&);
	MYSERIALDATA& operator=(const MYSERIALDATA&);
}MYSERIALDATA;

/**************************************************************************
* 函数名: GetPackageSize
* 功能: 获取包长度
* 参数:
*    @[in ] nUserDataLen: 用户数据长度
* 返回值: 成功返回包长度，失败返回-1
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
int GetPackageSize(int nUserDataLen);

/**************************************************************************
* 函数名: GetUserDataSize
* 功能: 获取用户数据长度
* 参数:
*    @[in ] nPackageSize: 包数据长度
* 返回值: 成功返回用户数据长度，失败返回-1
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
int GetUserDataSize(int nPackageSize);


typedef vector<byte> VecByteDataType;

//数据包
typedef struct PKGDATA
{
private:
    VecByteDataType m_vecDataList; //数据列表
    bool m_bBegin; //接收开始标识
    bool m_bEnd;   //接收完毕标识
public:
    PKGDATA();

    /**************************************************************************
    * 函数名: GetByteData
    * 功能: 获取数据包，成功时需在外释放
    * 时间: 2015/07/20 17:16
    * 作者: ZDJ
    */
    byte* GetByteData();

    /**************************************************************************
    * 函数名: GetVecData
    * 功能: 获取数据包
    * 时间: 2015/07/20 17:16
    * 作者: ZDJ
    */
    VecByteDataType GetVecData();

    /**************************************************************************
    * 函数名: GetVecDataRef
    * 功能: 获取数据包引用
    * 时间: 2015/07/20 17:16
    * 作者: ZDJ
    */
    VecByteDataType& GetVecDataRef();

    /**************************************************************************
    * 函数名: PushData
    * 功能: 存储数据
    * 时间: 2015/07/20 17:17
    * 作者: ZDJ
    */
    void PushData(byte data);

    /**************************************************************************
    * 函数名: ClearData
    * 功能: 清理数据
    * 时间: 2015/07/20 17:18
    * 作者: ZDJ
    */
    void ClearData();

    /**************************************************************************
    * 函数名: RemoveData
    * 功能: 移除指定数据包
    * 参数:
    *    @[in ] nIndex: 数据起始位置
    *    @[in ] nLen: 数据长度
    * 时间: 2015/07/17 17:51
    * 作者: ZDJ
    */
    void RemoveData(int nIndex, int nLen);

    /**************************************************************************
    * 函数名: IsEnd
    * 功能: 数据包是否接收完毕
    * 时间: 2015/07/20 17:18
    * 作者: ZDJ
    */
    bool IsEnd();

    /**************************************************************************
    * 函数名: IsBegin
    * 功能: 数据包是否开始接收
    * 时间: 2015/07/20 17:18
    * 作者: ZDJ
    */
    bool IsBegin();

    /**************************************************************************
    * 函数名: GetHead
    * 功能: 获取数据包的指令id
    * 时间: 2015/07/20 17:18
    * 作者: ZDJ
    */
    int GetCmdId();

    /**************************************************************************
    * 函数名: GetSize
    * 功能: 获取数据长度
    * 时间: 2015/07/27 15:39
    * 作者: ZDJ
    */
    int GetSize();

    /**************************************************************************
    * 函数名: IsValid
    * 功能: 检测是否是合法协议
    * 时间: 2015/09/22 9:44
    * 作者: ZDJ
    */
    bool IsValid();

}PKGDATA;

typedef vector<PKGDATA*> VecPkgDataType;

//数据缓冲池
class CDataPool
{
private:
    VecPkgDataType m_vecPkgList; //数据包列表
    PKGDATA*  m_pCurPkg; //当前正在接收的数据包

public:
    CDataPool();
    ~CDataPool();

    /**************************************************************************
    * 函数名: PushData
    * 功能: 存储数据
    * 参数:
    *    @[in ] myData: 数据
    * 返回值: 接收完毕返回指令id，否则返回-1
    * 时间: 2015/07/17 17:09
    * 作者: ZDJ
    */
    int PushData(byte myData);

    /**************************************************************************
    * 函数名: GetPkg
    * 功能: 获取数据包
    * 参数:
    *    @[in ] cmdId: 指令id
    * 返回值: 成功返回数据包，失败返回NULL
    * 时间: 2015/07/20 17:12
    * 作者: ZDJ
    */
    PKGDATA* GetPkg(int cmdId);

    /**************************************************************************
    * 函数名: ClearData
    * 功能: 清空数据包
    * 参数:
    *    @[in ] cmdId: 指令id
    * 时间: 2015/07/17 17:51
    * 作者: ZDJ
    */
    void ClearData(int cmdId);

    /**************************************************************************
    * 函数名: ClearAllData
    * 功能: 清空所有数据包
    * 时间: 2015/07/17 17:51
    * 作者: ZDJ
    */
    void ClearAllData();

    /**************************************************************************
    * 函数名: RemoveData
    * 功能: 移除指定数据包
    * 参数:
    *    @[in ] cmdId: 指令id
    *    @[in ] nIndex: 数据起始位置
    *    @[in ] nLen: 数据长度
    * 时间: 2015/07/17 17:51
    * 作者: ZDJ
    */
    void RemoveData(int cmdId, int nIndex, int nLen);
};

#endif
