/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CMotorCmd.h
* 创建时间：2016/12/19
* 文件标识：
* 文件摘要：舵机命令
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/12/19
* 版本摘要：
*/

#ifndef CMOTORCMD_H
#define CMOTORCMD_H

#include "controllerconst.h"

class CMotorCmd
{
public:
    explicit CMotorCmd();
    ~CMotorCmd();

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
    static vecBytes getCmdSpecifiedTimeMove(int nTargetPos, int nMoveTime, int nMoveFinishedTime);

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
    static vecBytes getCmdFixedSpeedMove(emMotorOperate eOperate, int nSpeed);

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
    static vecBytes getCmdFixedPWMMove(emMotorOperate eOperate, int nPWM);

    /**************************************************************************
    * 函数名: getCmdStopMove
    * 功能: 停止运动
    * 参数:
    *    @[in ] eOperate: 操作类型
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdStopMove(emMotorOperate eOperate);

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
    static vecBytes getCmdAnyAngleMove(int nTargetPos, int nMoveTime, int nMoveFinishedTime);

    /**************************************************************************
    * 函数名: getCmdLEDControl
    * 功能: LED控制
    * 参数:
    *    @[in ] eOperate: LED操作
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdLEDControl(emMotorOperate eOperate);

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
    static vecBytes getCmdWeakLock(int nTargetPos,int nMoveTime, int nMoveFinishedTime);

    /**************************************************************************
    * 函数名: getCmdLosePowerReadBack
    * 功能: 掉电回读
    * 参数:
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdLosePowerReadBack();

    /**************************************************************************
    * 函数名: getCmdWithPowerReadBack
    * 功能: 不掉电回读
    * 参数:
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdWithPowerReadBack();

    /**************************************************************************
    * 函数名: getCmdLockCurrentPos
    * 功能: 当前位置锁定
    * 参数:
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdLockCurrentPos();

    /**************************************************************************
    * 函数名: getCmdRelieveProtection
    * 功能: 解除各类保护
    * 参数:
    *    @[in ] eOperate: 保护类型
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdRelieveProtection(emMotorOperate eOperate);

    /**************************************************************************
    * 函数名: getCmdModifyMotorFlexibility
    * 功能: 修改舵机刚性
    * 参数:
    *    @[in ] nFlexibility: 舵机刚性值
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdModifyMotorFlexibility(int nFlexibility);

    /**************************************************************************
    * 函数名: getCmdReadMotorFlexibility
    * 功能: 读取舵机刚性
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdReadMotorFlexibility();

    /**************************************************************************
    * 函数名: getCmdModifyPWMProtectStatus
    * 功能: 修改PWM堵转保护状态
    * 参数:
    *    @[in ] eOperate: 操作类型
    *    @[in ] nLevel: 保护等级
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdModifyPWMProtectStatus(emMotorOperate eOperate, int nLevel);

    /**************************************************************************
    * 函数名: getCmdReadPWMProtectStatus
    * 功能: 读取PWM堵转保护状态
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdReadPWMProtectStatus();

    /**************************************************************************
    * 函数名: getCmdModifyNTCProtectStatus
    * 功能: 修改NTC保护状态
    * 参数:
    *    @[in ] eOperate: 操作类型
    *    @[in ] nLevel: 保护等级
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdModifyNTCProtectStatus(emMotorOperate eOperate, int nLevel);

    /**************************************************************************
    * 函数名: getCmdNTCProtectStatus
    * 功能: 读取NTC保护状态
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdNTCProtectStatus();

    /**************************************************************************
    * 函数名: getCmdModifyVoltageProtectStatus
    * 功能: 修改电压保护状态
    * 参数:
    *    @[in ] eOperate: 操作类型
    *    @[in ] nLevel: 保护等级
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdModifyVoltageProtectStatus(emMotorOperate eOperate, int nLevel);

    /**************************************************************************
    * 函数名: getCmdReadVoltageProtectStatus
    * 功能: 读取电压保护状态
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdReadVoltageProtectStatus();

    /**************************************************************************
    * 函数名: getCmdModifyCurrentProtectStatus
    * 功能: 修改电流保护状态
    * 参数:
    *    @[in ] eOperate: 操作类型
    *    @[in ] nLevel: 保护等级
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdModifyCurrentProtectStatus(emMotorOperate eOperate, int nLevel);

    /**************************************************************************
    * 函数名: getCmdReadCurrentProtectStatus
    * 功能: 读取电流保护状态
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdReadCurrentProtectStatus();

    /**************************************************************************
    * 函数名: getCmdModifyMotorID
    * 功能: 修改舵机ID
    * 参数:
    *    @[in ] nId: 舵机新ID
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdModifyMotorID(int nId);

    /**************************************************************************
    * 函数名: getCmdSetMotorAngleAjust
    * 功能: 设置舵机角度校正值
    * 参数:
    *    @[in ] nOffset: 舵机校正值
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdSetMotorAngleAjust(int nOffset);

    /**************************************************************************
    * 函数名: getCmdSetMotorAjustAngle
    * 功能: 设置舵机角度校正值
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdGetMotorAngleAjust();

    /**************************************************************************
    * 函数名: getCmdSetCurrentPosAngle
    * 功能: 设置当前位置到指定角度
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdSetCurrentPosAngle(int nAngle);

    /**************************************************************************
    * 函数名: getCmdSetTargetPosAngle
    * 功能: 设置目标位置到指定角度
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdSetTargetPosAngle(int nAngle);

    /**************************************************************************
    * 函数名: getCmdReadSolfwareVersion
    * 功能: 读取舵机版本号
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdReadSolfwareVersion();

    /**************************************************************************
    * 函数名: getCmdResetSettings
    * 功能: 回复出厂设置
    * 参数:
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdResetSettings();

private:
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
    static vecBytes getCmd(emMotorCmd eCmd, byte *pCmdBody, int nCmdBodyLen);

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
    static vecBytes getCmdModifyProtectStatus(emMotorCmd eCmd, emMotorOperate eOperate, int nLevel);

    /**************************************************************************
    * 函数名: getCmdReadProtectStatus
    * 功能: 读取保护状态命令
    * 参数:
    *    @[in ] eCmd: 命令类型
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static vecBytes getCmdReadProtectStatus(emMotorCmd eCmd);

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
    static vecBytes getCmdSetPosToAngle(emMotorCmd eCmd, int nAngle);

    /**************************************************************************
    * 函数名: getCheckSum
    * 功能: 获取校验字节
    * 参数:
    *    @[in ] vecData: 命令数据，从命令头到命令body
    * 返回值: 命令字节vector
    * 时间: 2016/12/19
    * 作者: hwx
    */
    static byte getCheckSum(const vecBytes &vecData);
};

#endif // CMOTORCMD_H
