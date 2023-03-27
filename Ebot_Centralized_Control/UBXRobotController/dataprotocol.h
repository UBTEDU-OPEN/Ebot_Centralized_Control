/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXProtocol.h
* 创建时间：2016/07/08 17:37
* 文件标识：
* 文件摘要：协议定义
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2018/03/19 17:37
* 版本摘要：
*/
#ifndef DATAPROTOCOL
#define DATAPROTOCOL


#include <vector>
using namespace std;

#define SAFE_DELETE(p)       if(p){ delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){ delete [] p; p = NULL;}

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef byte*  pbyte;

#define UBX_ACTION_LIST_PATH                    "actionlist"

#define UBX_CHANNEL_DEFAULT                     30
#define UBX_CHANNEL_COUNT                       40
#define UBX_BASE_FREQUENCY                      414.92
#define UBX_FREQUENCY_PARAM_COUNT               17
#define UBX_FREQUENCY_PARAM_SET_INDEX           3

//命令类型
#define UBXPROTOCOL_CMDTYPE_SEND                0x00   //命令类型，发送
#define UBXPROTOCOL_CMDTYPE_SUCCESS             0xAA   //命令类型，发送成功，目前PC没有用到
#define UBXPROTOCOL_CMDTYPE_FAIL                0xEE   //命令类型，发送失败，目前PC没有用到

//命令ID
#define UBXPROTOCOL_CMDID_PLAY                  0x01   //开始表演
#define UBXPROTOCOL_CMDID_STOP                  0x02   //结束表演(复位)
#define UBXPROTOCOL_CMDID_PLAY_TEST             0x03   //播放测试动作
#define UBXPROTOCOL_CMDID_PAUSE                 0x04   //暂停播放
#define UBXPROTOCOL_CMDID_RESUME                0x05   //继续表演
#define UBXPROTOCOL_CMDID_POWEROFF              0x06   //本体关机
#define UBXPROTOCOL_CMDID_REST                  0x07   //本体休眠
#define UBXPROTOCOL_CMDID_WAKEUP                0x08   //本体唤醒，复位
#define UBXPROTOCOL_CMDID_BEGIN_SEND_FILE       0x09   //开始传文件
#define UBXPROTOCOL_CMDID_SENDING_FILE          0x0A   //传输文件
#define UBXPROTOCOL_CMDID_END_SEND_FILE         0x0B   //结束传文件
#define UBXPROTOCOL_CMDID_DELETE_FILE           0x0C   //删除文件
#define UBXPROTOCOL_CMDID_SET_FREQUENCY_REC     0x0D   //设置接收频点
#define UBXPROTOCOL_CMDID_SET_FREQUENCY_SEND    0x0E  //设置发射频点

//数据类型
#define UBXPROTOCOL_DATATYPE_FILE               0x01    //数据类型，文件数据
#define UBXPROTOCOL_DATATYPE_DATA               0x02    //数据类型，数据流

#define UBXPROTOCOL_PACKAGE_SIZE                128    //数据包大小
#define UBXPROTOCOL_PACKAGE_SIZE_HID            64    //HID数据包最大值

#define UBXPROTOCOL_PORT                        32700   //绑定端口号

//额外参数
#define UBX_RESEND_COUNT                        5       //重复发送次数
#define UBX_DELAY_TIME                          250     //每次发送延迟
#define UBX_SYNCHRONAZE_TIME_CAL                1000    //发送同步倒数计时


#define TIMEDELAY_CONNECTROBOT                  1500   //连接机器人延时 2000
#define TIMEDELAY_DISCONNECTROBOT               200   //连接机器人延时


#pragma pack(push, 1)

//数据包前缀结构
typedef struct UBXPACKAGEFRONT
{
    byte   head[2];  //头部
    byte   nPkgLen;  //包长度，即头尾数据总长度
    byte   nCmdId;   //命令ID
    byte   nState;   //应答状态
    ushort nDelayTime;//倒数时间
}UBXPACKAGEFRONT;


//数据包用户数据
//typedef struct UBXPACKAGEBODY
//{
//    byte   userdata[0];     //用户数据
//}UBXPACKAGEBODY;

//数据包后缀结构
typedef struct UBXPACKAGEBACK
{
    byte   nChkSum;  //校验值
    byte   tail;     //尾部
}UBXPACKAGEBACK;

#pragma pack(pop)


#endif // DATAPROTOCOL

