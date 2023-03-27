/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：DataPool.cpp
* 创建时间：2015/07/22 16:22
* 文件标识：
* 文件摘要：数据缓冲池
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/07/22 16:22
* 版本摘要：
*/
#include "DataPool.h"
#include "CRC32.h"
#include <QDebug>

/**************************************************************************
* 函数名: GetPackageSize
* 功能: 获取包长度
* 参数:
*    @[in ] nUserDataLen: 用户数据长度
* 返回值: 成功返回包长度，失败返回-1
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
int GetPackageSize(int nUserDataLen)
{
    return sizeof(UBXPACKAGEFRONT) + sizeof(UBXPACKAGEBACK) + nUserDataLen - 1;
}

/**************************************************************************
* 函数名: GetUserDataSize
* 功能: 获取用户数据长度
* 参数:
*    @[in ] nPackageSize: 包数据长度
* 返回值: 成功返回用户数据长度，失败返回-1
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
int GetUserDataSize(int nPackageSize)
{
    return nPackageSize - sizeof(UBXPACKAGEFRONT) - sizeof(UBXPACKAGEBACK);
}


PKGDATA::PKGDATA()
{
    m_bBegin = false;
    m_bEnd = false;
}

/**************************************************************************
* 函数名: GetByteData
* 功能: 获取数据包，成功时需在外释放
* 时间: 2015/07/20 17:16
* 作者: ZDJ
*/
byte* PKGDATA::GetByteData()
{
    byte* databuf = new byte[m_vecDataList.size()];
    for(int i = 0; i < m_vecDataList.size(); i++)
    {
        databuf[i] = m_vecDataList[i];
    }
    return databuf;
}

/**************************************************************************
* 函数名: GetVecData
* 功能: 获取数据包
* 时间: 2015/07/20 17:16
* 作者: ZDJ
*/
VecByteDataType PKGDATA::GetVecData()
{
    return m_vecDataList;
}

/**************************************************************************
* 函数名: GetVecDataRef
* 功能: 获取数据包引用
* 时间: 2015/07/20 17:16
* 作者: ZDJ
*/
VecByteDataType& PKGDATA::GetVecDataRef()
{
    return m_vecDataList;
}

/**************************************************************************
* 函数名: PushData
* 功能: 存储数据
* 时间: 2015/07/20 17:17
* 作者: ZDJ
*/
void PKGDATA::PushData(byte data)
{
    int nPkgLen = -1;

    if(!m_bBegin)
    {
        m_bBegin = true;
        m_bEnd = false;
    }

    if(m_bBegin)
        m_vecDataList.push_back(data);

    UBXPACKAGEFRONT pkgfront;
    int nPkgLenOffset = sizeof(pkgfront.head);

    if(m_vecDataList.size() > nPkgLenOffset + sizeof(nPkgLen))
    {
        nPkgLen = *((int*)((byte*)m_vecDataList.data() + nPkgLenOffset));
    }

    if(nPkgLen > -1 && m_vecDataList.size() >= nPkgLen)
    {
        m_bBegin = false;
        m_bEnd = true;
    }
}

/**************************************************************************
* 函数名: ClearData
* 功能: 清理数据
* 时间: 2015/07/20 17:18
* 作者: ZDJ
*/
void PKGDATA::ClearData()
{
    m_bBegin = false;
    m_bEnd = false;
    m_vecDataList.clear();
}

/**************************************************************************
* 函数名: RemoveData
* 功能: 移除指定数据包
* 参数:
*    @[in ] nIndex: 数据起始位置
*    @[in ] nLen: 数据长度
* 时间: 2015/07/17 17:51
* 作者: ZDJ
*/
void PKGDATA::RemoveData(int nIndex, int nLen)
{
    VecByteDataType::iterator itor = m_vecDataList.begin();
    for(int i = 0; i < nIndex; i++)
        itor++;

    int j = 0;
    for(VecByteDataType::iterator itortemp = itor; itortemp != m_vecDataList.end() && j < nLen; j++)
    {
        m_vecDataList.erase(itortemp);
    }
}

/**************************************************************************
* 函数名: IsEnd
* 功能: 数据包是否接收完毕
* 时间: 2015/07/20 17:18
* 作者: ZDJ
*/
bool PKGDATA::IsEnd()
{
    return !m_bBegin && m_bEnd;
}

/**************************************************************************
* 函数名: IsBegin
* 功能: 数据包是否开始接收
* 时间: 2015/07/20 17:18
* 作者: ZDJ
*/
bool PKGDATA::IsBegin()
{
    return m_bBegin;
}

/**************************************************************************
* 函数名: GetHead
* 功能: 获取数据包的指令id
* 时间: 2015/07/20 17:18
* 作者: ZDJ
*/
int PKGDATA::GetCmdId()
{
    UBXPACKAGEFRONT pkgfront;
    int nCmdIdOffset = sizeof(pkgfront.head) + sizeof(pkgfront.nPkgLen);

    int nCmdId = 0;
    if(m_vecDataList.size() > nCmdIdOffset + sizeof(nCmdId))
    {
        nCmdId = *((ushort*)((byte*)m_vecDataList.data() + nCmdIdOffset));
        return nCmdId;
    }
    return -1;
}

/**************************************************************************
* 函数名: GetSize
* 功能: 获取数据长度
* 时间: 2015/07/27 15:39
* 作者: ZDJ
*/
int PKGDATA::GetSize()
{
    return m_vecDataList.size();
}

/**************************************************************************
* 函数名: IsValid
* 功能: 检测是否是合法协议
* 时间: 2015/09/22 9:44
* 作者: ZDJ
*/
bool PKGDATA::IsValid()
{
    bool bRet = false;
    do
    {
        if(m_vecDataList.size() < sizeof(UBXPACKAGEFRONT))
            break;
        UBXPACKAGEFRONT pkgfront;
        UBXPACKAGEBACK  pkgback;

        int nPkgLenOffset = sizeof(pkgfront.head);
        int nPkgLen = *((int*)((byte*)m_vecDataList.data() + nPkgLenOffset));
        if(nPkgLen != m_vecDataList.size())
            break;

        uint nCRC32Temp = crc32_standard(m_vecDataList.data(), m_vecDataList.size() - sizeof(pkgback.nChkSum) - sizeof(pkgback.tail));

        int nCRC32Offset = sizeof(pkgfront) + GetUserDataSize(m_vecDataList.size());
        int nCRC32 = *((int*)((byte*)m_vecDataList.data() + nCRC32Offset));
        if(nCRC32Temp != nCRC32)
            break;

        bRet = true;
    }while(0);

    return bRet;
}

CDataPool::CDataPool()
{
    m_pCurPkg = NULL;
}

CDataPool::~CDataPool(void)
{
    ClearAllData();
}

/**************************************************************************
* 函数名: PushData
* 功能: 存储数据
* 参数:
*    @[in ] myData: 数据
* 返回值: 接收完毕返回指令id，否则返回-1
* 时间: 2015/07/17 17:09
* 作者: ZDJ
*/
int CDataPool::PushData(byte myData)
{
    int nRet = -1;
    if(!m_pCurPkg)
    {
        PKGDATA* newpkg = new PKGDATA();
        newpkg->PushData(myData);
        m_vecPkgList.push_back(newpkg);

        m_pCurPkg = newpkg;
    }
    else
    {
        if(!m_pCurPkg->IsEnd())
            m_pCurPkg->PushData(myData);

        if(m_pCurPkg->IsEnd())
        {
            int nCmdId = m_pCurPkg->GetCmdId();
            if(!m_pCurPkg->IsValid())
                ClearData(nCmdId);
            else
                nRet = nCmdId;
            m_pCurPkg = NULL;
        }
    }
    return nRet;
}

/**************************************************************************
* 函数名: GetPkg
* 功能: 获取数据包
* 参数:
*    @[in ] cmdId: 指令id
* 返回值: 成功返回数据包，失败返回NULL
* 时间: 2015/07/20 17:12
* 作者: ZDJ
*/
PKGDATA* CDataPool::GetPkg(int cmdId)
{
    for(int i=0;i<m_vecPkgList.size();i++)
    {
        PKGDATA* pPkg = m_vecPkgList[i];
        if(pPkg)
        {
            if(pPkg->GetCmdId() == cmdId)
            {
                return pPkg;
            }
        }
    }
    return NULL;
}

/**************************************************************************
* 函数名: ClearData
* 功能: 清空数据包
* 参数:
*    @[in ] cmdId: 指令id
* 时间: 2015/07/17 17:51
* 作者: ZDJ
*/
void CDataPool::ClearData(int cmdId)
{
    VecPkgDataType::iterator itor = m_vecPkgList.begin();
    while(itor != m_vecPkgList.end())
    {
        PKGDATA* pPkg = *itor;
        if(pPkg)
        {
            if(pPkg->GetCmdId() == cmdId)
            {
                m_vecPkgList.erase(itor);
                SAFE_DELETE(pPkg);
                break;
            }
        }

        itor++;
    }
}

/**************************************************************************
* 函数名: ClearAllData
* 功能: 清空所有数据包
* 时间: 2015/07/17 17:51
* 作者: ZDJ
*/
void CDataPool::ClearAllData()
{
    ReleaseVector(m_vecPkgList);
    m_pCurPkg = NULL;
}

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
void CDataPool::RemoveData(int cmdId, int nIndex, int nLen)
{
    VecPkgDataType::iterator itor = m_vecPkgList.begin();
    while(itor != m_vecPkgList.end())
    {
        PKGDATA* pPkg = *itor;
        if(pPkg)
        {
            if(pPkg->GetCmdId() == cmdId)
            {
                pPkg->RemoveData(nIndex, nLen);
                break;
            }
        }

        itor++;
    }
}
