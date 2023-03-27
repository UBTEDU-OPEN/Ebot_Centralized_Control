﻿/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：alpharobotlog.cpp
* 创建时间：2016/06/07
* 文件标识：
* 文件摘要：参数设置控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/06/07
* 版本摘要：
*/

#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QSettings>

#include "alpharobotlog.h"

AlphaRobotLog::AlphaRobotLog(QObject *pParent) : m_bEnableLog(false)
{
    QString strLogNative = "./MotorDebugTool.log";
    m_Logfile.setFileName(strLogNative);
    if (!m_Logfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open log file" << endl;
        return;
    }

    m_OutputStream.setDevice(&m_Logfile);

    m_bEnableLog = true;
    m_strDateTimeFormat = "yyyy-MM-dd HH:mm:ss";

    printInfo("create log");
}

AlphaRobotLog::~AlphaRobotLog()
{
    if (m_Logfile.isOpen())
    {
         m_Logfile.close();
    }
}

/************************************
* 名称: getInstance
* 功能: 获取单例
* 参数：
* 返回:   AlphaRobotLog *：log对象
* 时间:   2016/06/15
* 作者:   hwx
************************************/
AlphaRobotLog *AlphaRobotLog::getInstance()
{
    static AlphaRobotLog log;
    return &log;
}

/************************************
* 名称: printDebug
* 功能: 打印Debug信息
* 参数：
* 返回:
* 时间:   2016/06/15
* 作者:   hwx
************************************/
void AlphaRobotLog::printDebug(const QString &strDebug)
{    
    printToLog(strDebug, "Debug");
}

/************************************
* 名称: printInfo
* 功能: 打印info信息
* 参数：
* 返回:
* 时间:   2016/06/15
* 作者:   hwx
************************************/
void AlphaRobotLog::printInfo(const QString &strInfo)
{    
    printToLog(strInfo, "Info");
}

/************************************
* 名称: printWarning
* 功能: 打印warning信息
* 参数：
* 返回:
* 时间:   2016/06/15
* 作者:   hwx
************************************/
void AlphaRobotLog::printWarning(const QString &strWarning)
{    
    printToLog(strWarning, "Warning");
}

/************************************
* 名称: printError
* 功能: 打印Error信息
* 参数：
* 返回:
* 时间:   2016/06/15
* 作者:   hwx
************************************/
void AlphaRobotLog::printError(const QString &strError)
{    
    printToLog(strError, "Error");
}

void AlphaRobotLog::printToLog(const QString &strLog, const QString &strLogType)
{
    // 只有在debug模式下才会加锁，方便后续配置成非debug模式
    // 进行控制
    if (!m_bEnableLog)
    {
        return;
    }

    QMutexLocker lock(&m_Mutex);

    if (strLog.isEmpty())
    {
        return;
    }

    if (m_Logfile.isOpen() && m_Logfile.isWritable())
    {
         QString strDebugMsg = QString("[AlphaRobot::%1 %2] %3\n")
                 .arg(strLogType)
                 .arg(QDateTime::currentDateTime().toString(m_strDateTimeFormat))
                 .arg(strLog);

         m_OutputStream << strDebugMsg;
         m_OutputStream.flush();
    }
}
