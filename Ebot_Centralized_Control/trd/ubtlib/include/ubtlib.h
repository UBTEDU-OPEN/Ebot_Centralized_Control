#ifndef UBTLIB_H
#define UBTLIB_H
#include <string>

/**************************************************************************
* 函数名: Req_PackageCmdData
* 功能: 请求打包命令数据
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
*    @[out] MYDATA*: 分包数据列表
* 返回值: 成功返回帧数据缓冲区长度，失败返回-1
* 时间: 2018/03/08 17:34
* 作者: Joker
*/
int Req_PackageCmdData(int nCmdId, unsigned char *pDataBuf, int nDataLen, unsigned short nDelayTime, unsigned char *&pOutData);

/**************************************************************************
* 函数名: UBXPackageFrame
* 功能: 打包帧数据
* 参数:
*    @[in ] pDataBuf: 用户数据缓冲区地址
*    @[in ] nDataLen: 用户数据缓冲区长度
*    @[in ] nCmdId: 命令ID
*    @[in ] nFrameCount: 帧数量
*
*
*    @[in ] nFrameIndex: 帧序号，从1开始
*    @[out] pOutData: 帧数据缓冲区地址，需在外释放
* 返回值: 成功返回帧数据缓冲区长度，失败返回-1
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
int PackageFrame(unsigned char* pDataBuf, int nDataLen, unsigned short nCmdId, short nFrameIndex, unsigned char*& pOutData);

std::string getHeaderXUBTSignV3(std::string &deviceId);

#endif // UBTLIB_H
