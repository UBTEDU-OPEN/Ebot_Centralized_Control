/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CMotorCmd.cpp
* 创建时间：2016/12/19
* 文件标识：
* 文件摘要：舵机命令
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/12/19
* 版本摘要：
*/

#include "cmotorcmd.h"

CMotorCmd::CMotorCmd()
{

}

CMotorCmd::~CMotorCmd()
{

}

/**************************************************************************
* 函数名: getCmdSpecifiedTimeMove
* 功能: 获取指定时间运动命令
* 参数:
*    @[in ] nTargetPos: 目标位置
*    @[in ] nMoveTime: 运动时间
*    @[in ] nMoveFinishedTime: 运动完成时间
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdSpecifiedTimeMove(int nTargetPos, int nMoveTime, int nMoveFinishedTime)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)nTargetPos;
    cmdBody[eParamIndex2] = (byte)nMoveTime;
    memcpy(cmdBody + 2, &nMoveFinishedTime, 2);

    return getCmd(emMotorCmdNormalMove, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdFixedSpeedMove
* 功能: 恒定轮模式运动
* 参数:
*    @[in ] eOperate: 操作类型
*    @[in ] nSpeed: 运动速度
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdFixedSpeedMove(emMotorOperate eOperate, int nSpeed)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)eOperate;
    cmdBody[eParamIndex2] = 0x00;
    memcpy(cmdBody + 2, &nSpeed, 2);

    return getCmd(emMotorCmdNormalMove, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdFixedPWMMove
* 功能: 恒定PWM模式运动
* 参数:
*    @[in ] eOperate: 操作类型
*    @[in ] nPWM: PWM值
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdFixedPWMMove(emMotorOperate eOperate, int nPWM)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)eOperate;
    cmdBody[eParamIndex2] = 0x00;
    memcpy(cmdBody + 2, &nPWM, 2);

    return getCmd(emMotorCmdNormalMove, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdStopMove
* 功能: 停止运动
* 参数:
*    @[in ] eOperate: 操作类型
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdStopMove(emMotorOperate eOperate)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)emMotorOperateStopMove;
    cmdBody[eParamIndex2] = (byte)eOperate;

    return getCmd(emMotorCmdNormalMove, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdAnyAngleMove
* 功能: 360°指定时间运动
* 参数:
*    @[in ] nTargetPos: 目标位置
*    @[in ] nMoveTime: 运动时间
*    @[in ] nMoveFinishedTime: 运动完成时间
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdAnyAngleMove(int nTargetPos, int nMoveTime, int nMoveFinishedTime)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    memcpy(cmdBody, &nTargetPos, 2);
    cmdBody[eParamIndex3] = (byte)nMoveTime;
    cmdBody[eParamIndex4] = (byte)nMoveFinishedTime;

    return getCmd(emMotorCmdAnyAngleMove, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdLEDControl
* 功能: LED控制
* 参数:
*    @[in ] eOperate: LED操作
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdLEDControl(emMotorOperate eOperate)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)eOperate;

    return getCmd(emMotorCmdLEDControl, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdWeakLock
* 功能: 弱锁位模式
* 参数:
*    @[in ] nTargetPos: 目标位置
*    @[in ] nMoveTime: 运动时间
*    @[in ] nMoveFinishedTime: 运动完成时间
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdWeakLock(int nTargetPos,int nMoveTime, int nMoveFinishedTime)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    memcpy(cmdBody, &nTargetPos, 2);
    cmdBody[eParamIndex3] = (byte)nMoveTime;
    cmdBody[eParamIndex4] = (byte)nMoveFinishedTime;

    return getCmd(emMotorCmdWeakLock, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdLosePowerReadBack
* 功能: 掉电回读
* 参数:
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdLosePowerReadBack()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdLosePowerReadBack, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdWithPowerReadBack
* 功能: 不掉电回读
* 参数:
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdWithPowerReadBack()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdWithPowerReadBack, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdLockCurrentPos
* 功能: 当前位置锁定
* 参数:
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdLockCurrentPos()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdLockCurrentPosition, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdRelieveProtection
* 功能: 解除各类保护
* 参数:
*    @[in ] eOperate: 保护类型
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdRelieveProtection(emMotorOperate eOperate)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)eOperate;

    return getCmd(emMotorCmdRelieveProtection, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdModifyMotorFlexibility
* 功能: 修改舵机刚性
* 参数:
*    @[in ] nFlexibility: 舵机刚性值
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyMotorFlexibility(int nFlexibility)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)nFlexibility;

    return getCmd(emMotorCmdModfiyFlexibility, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdReadMotorFlexibility
* 功能: 读取舵机刚性
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdReadMotorFlexibility()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdReadFlexibility, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdModifyPWMProtectStatus
* 功能: 修改PWM堵转保护状态
* 参数:
*    @[in ] eOperate: 操作类型
*    @[in ] nLevel: 保护等级
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyPWMProtectStatus(emMotorOperate eOperate, int nLevel)
{
    return getCmdModifyProtectStatus(emMotorCmdModifyPWMProtectionStatus, eOperate, nLevel);
}

/**************************************************************************
* 函数名: getCmdReadPWMProtectStatus
* 功能: 读取PWM堵转保护状态
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdReadPWMProtectStatus()
{
    return getCmdReadProtectStatus(emMotorCmdReadPWMProtectionStatus);
}

/**************************************************************************
* 函数名: getCmdModifyNTCProtectStatus
* 功能: 修改NTC保护状态
* 参数:
*    @[in ] eOperate: 操作类型
*    @[in ] nLevel: 保护等级
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyNTCProtectStatus(emMotorOperate eOperate, int nLevel)
{
    return getCmdModifyProtectStatus(emMotorCmdModifyNTCProtectionStatus, eOperate, nLevel);
}

/**************************************************************************
* 函数名: getCmdNTCProtectStatus
* 功能: 读取NTC保护状态
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdNTCProtectStatus()
{
    return getCmdReadProtectStatus(emMotorCmdReadNTCProtectionStatus);
}

/**************************************************************************
* 函数名: getCmdModifyVoltageProtectStatus
* 功能: 修改电压保护状态
* 参数:
*    @[in ] eOperate: 操作类型
*    @[in ] nLevel: 保护等级
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyVoltageProtectStatus(emMotorOperate eOperate, int nLevel)
{
    return getCmdModifyProtectStatus(emMotorCmdModifyVoltageProtectionStatus, eOperate, nLevel);
}

/**************************************************************************
* 函数名: getCmdReadVoltageProtectStatus
* 功能: 读取电压保护状态
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdReadVoltageProtectStatus()
{
    return getCmdReadProtectStatus(emMotorCmdReadVoltageProtectionStatus);
}

/**************************************************************************
* 函数名: getCmdModifyCurrentProtectStatus
* 功能: 修改电流保护状态
* 参数:
*    @[in ] eOperate: 操作类型
*    @[in ] nLevel: 保护等级
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyCurrentProtectStatus(emMotorOperate eOperate, int nLevel)
{
    return getCmdModifyProtectStatus(emMotorCmdModifyCurrentProtectionStatus, eOperate, nLevel);
}

/**************************************************************************
* 函数名: getCmdReadCurrentProtectStatus
* 功能: 读取电流保护状态
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdReadCurrentProtectStatus()
{
    return getCmdReadProtectStatus(emMotorCmdReadCurrentProtectionStatus);
}

/**************************************************************************
* 函数名: getCmdModifyMotorID
* 功能: 修改舵机ID
* 参数:
*    @[in ] nId: 舵机新ID
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyMotorID(int nId)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex2] = (byte)nId;

    return getCmd(emMotorCmdModifyMotorID, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdSetMotorAngleAjust
* 功能: 设置舵机角度校正值
* 参数:
*    @[in ] nOffset: 舵机校正值
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdSetMotorAngleAjust(int nOffset)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    memcpy(cmdBody + 2, &nOffset, 2);

    return getCmd(emMotorCmdSetAngleAjust, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdSetMotorAjustAngle
* 功能: 设置舵机角度校正值
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdGetMotorAngleAjust()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdGetAngleAjust, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdSetCurrentPosAngle
* 功能: 设置当前位置到指定角度
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdSetCurrentPosAngle(int nAngle)
{
    return getCmdSetPosToAngle(emMotorCmdSetCurrentPosAngle, nAngle);
}

/**************************************************************************
* 函数名: getCmdSetTargetPosAngle
* 功能: 设置目标位置到指定角度
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdSetTargetPosAngle(int nAngle)
{
    return getCmdSetPosToAngle(emMotorCmdSetCurrentTargetPosAngle, nAngle);
}

/**************************************************************************
* 函数名: getCmdReadSolfwareVersion
* 功能: 读取舵机版本号
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdReadSolfwareVersion()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdReadSolftwareVersion, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdResetSettings
* 功能: 回复出厂设置
* 参数:
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdResetSettings()
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(emMotorCmdResetSettings, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmd
* 功能: 返回拼上命令头、命令长度、命令校验、命令尾的完整命令
* 参数:
*    @[in ] eCmd: 命令类型
*    @[in ] pCmdBody: 命令body
*    @[in ] nCmdBodyLen: 命令body长度
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmd(emMotorCmd eCmd, byte *pCmdBody, int nCmdBodyLen)
{
    vecBytes vecWholCmd;
    vecWholCmd.clear();

    if(!pCmdBody || (nCmdBodyLen <= 0))
    {
        return vecWholCmd;
    }

    // 命令头
    if (eCmd == emMotorCmdResetSettings || eCmd == emMotorCmdReadSolftwareVersion)
    {
        vecWholCmd.push_back((byte)emMotorFlagsHeadFC);
        vecWholCmd.push_back((byte)emMotorFlagsHeadCF);
    }
    else
    {
        vecWholCmd.push_back((byte)emMotorFlagsHeadFA);
        vecWholCmd.push_back((byte)emMotorFlagsHeadAF);
        vecWholCmd.push_back((byte)emMotorFlagsBroadcastID);
    }

    // 指令码
    vecWholCmd.push_back(eCmd);

    // command body
    for(int i = 0; i < nCmdBodyLen; i++)
    {
        vecWholCmd.push_back(*(pCmdBody + i));
    }

    //check sum
    byte byteCheckSum = getCheckSum(vecWholCmd);
    vecWholCmd.push_back(byteCheckSum);

    //end
    vecWholCmd.push_back((byte)emMotorFlagsEnd);

    //SAFE_DELETE(pCmdBody);

    return vecWholCmd;
}

/**************************************************************************
* 函数名: getCmdModifyProtectStatus
* 功能: 获取修改保护状态命令
* 参数:
*    @[in ] eCmd: 命令类型
*    @[in ] eOperate: 操作类型
*    @[in ] nLevel: 保护等级
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdModifyProtectStatus(emMotorCmd eCmd, emMotorOperate eOperate, int nLevel)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    cmdBody[eParamIndex1] = (byte)eOperate;
    cmdBody[eParamIndex2] = (byte)nLevel;
    return getCmd(eCmd, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdReadProtectStatus
* 功能: 读取保护状态命令
* 参数:
*    @[in ] eCmd: 命令类型
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdReadProtectStatus(emMotorCmd eCmd)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    return getCmd(eCmd, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCmdSetPosToAngle
* 功能: 设置位置到指定角度
* 参数:
*    @[in ] eCmd: 命令类型
*    @[in ] nAngle: 角度值
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
vecBytes CMotorCmd::getCmdSetPosToAngle(emMotorCmd eCmd, int nAngle)
{
    byte cmdBody[MOTOR_PROTOCOL_PARAM_NUMBER] = {0x00, 0x00, 0x00, 0x00};
    memcpy(cmdBody, &nAngle, 2);
    return getCmd(eCmd, cmdBody, MOTOR_PROTOCOL_PARAM_NUMBER);
}

/**************************************************************************
* 函数名: getCheckSum
* 功能: 获取校验字节
* 参数:
*    @[in ] vecData: 命令数据，从命令头到命令body
* 返回值: 命令字节vector
* 时间: 2016/12/19
* 作者: hwx
*/
byte CMotorCmd::getCheckSum(const vecBytes &vecData)
{
    //ID+指令码+参数1+参数2 +参数3+参数4
   byte sum = 0;
   if (vecData.size() <= 2)
   {
       return sum;
   }

   for(int i = 2; i < vecData.size(); i++)
   {
       sum += vecData[i];
   }

    return sum;
}
