#ifndef CONTROLLERCONST
#define CONTROLLERCONST

#include <vector>
using namespace std;

typedef unsigned char   byte;
typedef vector<byte>  vecBytes;

#define SAFE_DELETE(p)       if(p){ delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){ delete [] p; p = NULL;}
#define SAFE_FREE(p)         if(p){ free(p); p = NULL;}

const int MOTOR_PROTOCOL_PARAM_NUMBER = 4; // 参数个数

enum emParamIndex
{
    eParamIndex1 = 0,
    eParamIndex2,
    eParamIndex3,
    eParamIndex4
};

enum emProtocolFlags
{
    emMotorFlagsHeadFA = 0xFA,  // 协议头第一字节
    emMotorFlagsHeadAF = 0xAF, // 协议头第二字节
    emMotorFlagsHeadFC = 0xFC,  // 协议头第一字节
    emMotorFlagsHeadCF = 0xCF, // 协议头第二字节
    emMotorFlagsEnd = 0xED,  // 协议结束字节
    emMotorFlagsBroadcastID = 0x00,  // 广播
};

enum emMotorCmd
{
    emMotorCmdNormalMove = 0x01,  // 普通运动
    emMotorCmdAnyAngleMove = 0x21, // 360°指定时间运动

    emMotorCmdLEDControl = 0x04,  // LED控制

    emMotorCmdLosePowerReadBack = 0x02, // 掉电回读
    emMotorCmdWithPowerReadBack = 0x03, // 不掉电回读
    
    emMotorCmdLockCurrentPosition = 0x05, // 当前位置锁定
    emMotorCmdWeakLock = 0x07, // 弱锁位模式
    
    emMotorCmdRelieveProtection = 0x06,  // 解除保护

    emMotorCmdModfiyFlexibility = 0x08, // 修改舵机刚性值
    emMotorCmdReadFlexibility = 0x09, // 读取舵机刚性值
    
    emMotorCmdModifyPWMProtectionStatus = 0x0A, // 修改舵机堵转保护状态
    emMotorCmdReadPWMProtectionStatus = 0x0B, // 读取舵机堵转保护状态
    
    emMotorCmdModifyNTCProtectionStatus = 0x0C, // 修改NTC保护状态
    emMotorCmdReadNTCProtectionStatus = 0x0D, // 读取NTC保护状态
    
    emMotorCmdModifyVoltageProtectionStatus = 0x0E, // 修改电压保护状态
    emMotorCmdReadVoltageProtectionStatus = 0x0F, // 读取电压保护状态
    
    emMotorCmdModifyCurrentProtectionStatus = 0x10, // 修改电流保护状态
    emMotorCmdReadCurrentProtectionStatus = 0x11, // 读取电流保护状态
    
    emMotorCmdModifyMotorID = 0xCD, // 修改ID

    emMotorCmdSetAngleAjust = 0xD2, // 设置舵机角度校正值
    emMotorCmdGetAngleAjust = 0xD4,  // 获取舵机角度校正值
    
    emMotorCmdSetCurrentPosAngle = 0xD5, // 设置当前位置为指定角度值
    emMotorCmdSetCurrentTargetPosAngle = 0xD6, // 设置当前目标位置为指定角度值

    emMotorCmdReadSolftwareVersion = 0x01, // 读取软件版本号
    emMotorCmdResetSettings = 0x03, // 回复出厂设置
};

enum emMotorOperate
{
    emMotorOperateStopMove= 0xFF, // 停止运动
    emMotorOperateMoveForward = 0xFD, // 正转
    emMotorOperateMoveReverse = 0xFE,  // 反转

    emMotorOperateLosePowerStop = 0x00,  // 掉电停止
    emMotorOperateStrongLockStop = 0x01, // 强锁位停止
    emMotorOperateWeakLockStop = 0x02,  // 弱锁位停止

    emMotorOperateLEDFlashed = 0x00,  // LED闪烁
    emMotorOperateLEDTurnOff = 0x01,  // LED熄灭

    emMotorOperateRelievePWMProtection = 0x00, // 解除PWM保护
    emMotorOperateRelieveCurrentProtection = 0x01,  // 解除电流保护
    emMotorOperateRelieveOverTempratureProtection = 0x02, // 解除过温保护
    emMotorOperateRelieveUnderVoltageProtection = 0x03, // 解除欠压保护
    emMotorOperateRelieveOverVoltageProtection = 0x04,  // 解除过压保护
    emMotorOperateRelieveAllProtection = 0xF0, //解除所有保护

    emMotorOperateEnableProtection = 0x00,  // 保护
    emMotorOperateDisableProtection = 0x01,  // 失能
};

#endif // CONTROLLERCONST

