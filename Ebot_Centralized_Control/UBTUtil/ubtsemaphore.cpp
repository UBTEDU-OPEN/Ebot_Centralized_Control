#include "ubtsemaphore.h"
#include <cstring>

#ifdef UBTPLATFORM_WIN

#else
#include <fcntl.h>
#include <sys/time.h>
#endif

UBTSemaphore::UBTSemaphore(const char* strName)
{
    memset(m_cSemaphoreName, 0, sizeof(m_cSemaphoreName));
    int min = strlen(strName)>(sizeof(m_cSemaphoreName)-1)?(sizeof(m_cSemaphoreName)-1):strlen(strName);
    strncpy(m_cSemaphoreName, strName, min);

#ifdef UBTPLATFORM_WIN
//    m_pMutex = CreateMutex(NULL, false, (LPCWSTR)m_cSemaphoreName);
    m_pMutex = CreateEvent(NULL,FALSE,FALSE,NULL);

#else
//    m_pSem = sem_open(m_cSemaphoreName, O_RDWR | O_CREAT, 0644, 1);
     m_pSem = new sem_t();
     sem_init(m_pSem, 0 , 0);
#endif
}

UBTSemaphore::~UBTSemaphore()
{
#ifdef UBTPLATFORM_WIN
     CloseHandle(m_pMutex);
#else
    int ret = sem_destroy(m_pSem);
//    if (0 != ret)
//    {
//        printf("sem_close error %d\n", ret);
//    }
//    sem_unlink(m_cSemaphoreName);
    delete m_pSem;
    m_pSem = NULL;

#endif
}

//时间a的值增加ms毫秒
void timeraddMS(struct timeval *a, uint ms)
{
    a->tv_usec += ms * 1000;
    if(a->tv_usec >= 1000000)
    {
        a->tv_sec += a->tv_usec / 1000000;
        a->tv_usec %= 1000000;
    }
}

/**************************************************************************
* 函数名: WaitForSignal
* 功能: 等待信号
* 参数: nTimeOut: 超时时间，0：记不超时，其它为ms
* 返回值: 0：收到信号，－1超时
* 时间: 2017/03/28
* 作者: leen
*/
int UBTSemaphore::WaitForSignal(int nTimeOut)
{
#ifdef UBTPLATFORM_WIN
    DWORD nRet = 0;
    if (nTimeOut == 0)
        nRet = WaitForSingleObject(m_pMutex, INFINITE);
    else
        nRet = WaitForSingleObject(m_pMutex, nTimeOut);

    if (nRet != WAIT_OBJECT_0)
    {
        return -1;
        DWORD dwLastErr = GetLastError();
    }
    else
    {
        return 0;
    }
#else
    int ret = 0;
    if (nTimeOut == 0)
    {
        ret = sem_wait(m_pSem);
    }
    else
    {
        struct timeval now;
        struct timespec outtime;
        gettimeofday(&now, NULL);
        //cout << now.tv_sec << " " << (now.tv_usec) << "\n";
        timeraddMS(&now, nTimeOut);
        outtime.tv_sec = now.tv_sec;
        outtime.tv_nsec = now.tv_usec * 1000;
        #ifdef UBTPLATFORM_MAC
        ret = sem_wait(m_pSem);
        #else
        ret = sem_timedwait(m_pSem,&outtime);
        #endif
    }

    if (ret == -1)
    {
        return -1;
    }

    return 0;
#endif
}

/**************************************************************************
* 函数名: SendSignal
* 功能: 发送信号
* 参数: nTimeOut: 超时时间，0：记不超时，其它为ms
* 返回值: 0：收到信号，－1超时
* 时间: 2017/03/28
* 作者: leen
*/
void UBTSemaphore::SendSignal()
{
#ifdef UBTPLATFORM_WIN
    SetEvent(m_pMutex);
#else
    sem_post(m_pSem);
#endif
}
