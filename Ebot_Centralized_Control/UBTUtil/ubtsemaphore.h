#ifndef UBTSEMAPHORE_H
#define UBTSEMAPHORE_H


//#include "dllexport_define.h"
#define UBTPLATFORM_WIN

#ifdef UBTPLATFORM_WIN
#include <stdio.h>
//#include <winsock2.h>
#include <windows.h>
#endif

#include "ubtutil.h"
//#include <semaphore.h>

class UBTUTILSHARED_EXPORT UBTSemaphore
{
public:
    UBTSemaphore(const char* strName);
    ~UBTSemaphore();

private:
#ifdef UBTPLATFORM_WIN
    HANDLE  m_pMutex;
#else
    sem_t* m_pSem;
#endif

    char m_cSemaphoreName[30];

public:
    /**************************************************************************
    * 函数名: WaitForSignal
    * 功能: 等待信号
    * 参数: nTimeOut: 超时时间，0：记不超时，其它为ms
    * 返回值: 0：收到信号，－1超时
    * 时间: 2017/03/28
    * 作者: leen
    */
    int WaitForSignal(int nTimeOut);

    /**************************************************************************
    * 函数名: SendSignal
    * 功能: 发送信号
    * 参数: nTimeOut: 超时时间，0：记不超时，其它为ms
    * 返回值: 0：收到信号，－1超时
    * 时间: 2017/03/28
    * 作者: leen
    */
    void SendSignal();
};

#endif // UBTSEMAPHORE_H
