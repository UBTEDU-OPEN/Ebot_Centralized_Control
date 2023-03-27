//#include "UBXBase.h"
#include "UBXCritSec.h"

UBXCritSec::UBXCritSec()
{
#ifdef UBTPLATFORM_WIN
        InitializeCriticalSection(&m_CritSec);
#else
        pthread_mutexattr_init(&m_pMutexAttr);
        pthread_mutexattr_settype(&m_pMutexAttr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_pMutex, &m_pMutexAttr);
#endif
}

UBXCritSec::~UBXCritSec()
{
#ifdef UBTPLATFORM_WIN
        DeleteCriticalSection(&m_CritSec);
#else
		pthread_mutex_destroy(&m_pMutex);
        pthread_mutexattr_destroy(&m_pMutexAttr);
#endif
}

void UBXCritSec::Lock()
{
#ifdef UBTPLATFORM_WIN
			EnterCriticalSection(&m_CritSec);
#else
			(void)pthread_mutex_lock(&m_pMutex);
#endif
}

void UBXCritSec::Unlock()
{
#ifdef UBTPLATFORM_WIN
			LeaveCriticalSection(&m_CritSec);
#else
			pthread_mutex_unlock(&m_pMutex);
#endif
}

UBXAutoLock::UBXAutoLock(UBXCritSec * plock)
{
    m_pLock = plock;
    m_pLock->Lock();
}

UBXAutoLock::~UBXAutoLock()
{
	m_pLock->Unlock();
}

UBXEvent::UBXEvent(bool bManualReset)
{
#ifdef UBTPLATFORM_WIN
	m_hEvent = CreateEvent(NULL, bManualReset, FALSE, NULL);
#else
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond,NULL);
	m_nEvent=0;
	m_bManualReset=bManualReset;
#endif
};

UBXEvent::~UBXEvent()
{
#ifdef UBTPLATFORM_WIN
	if(m_hEvent) 
	{
		CloseHandle(m_hEvent);
		m_hEvent=NULL;
	}		
#else
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
#endif
}

void UBXEvent::Set(void) 
{
#ifdef UBTPLATFORM_WIN
	SetEvent(m_hEvent);
#else
	pthread_mutex_lock(&m_mutex);
    m_nEvent=1;
	pthread_cond_broadcast(&m_cond);
    pthread_mutex_unlock(&m_mutex);
#endif
}

 bool UBXEvent::Wait(long ulTimeout)
{
#ifdef UBTPLATFORM_WIN
	if (ulTimeout==0)
		ulTimeout=-1;

	return (WaitForSingleObject(m_hEvent, ulTimeout) == WAIT_OBJECT_0);
#else
	bool bRet=false;
	if (ulTimeout==-1 || ulTimeout==0)
	{
		pthread_mutex_lock(&m_mutex);
        while (m_nEvent == 0)
        {
             printf("UBXEvent::Wait wait begin\n");
             pthread_cond_wait(&m_cond, &m_mutex);
             printf("UBXEvent::Wait wait end\n");
		}
		if (!m_bManualReset)
			m_nEvent=0;
		pthread_mutex_unlock(&m_mutex);
		bRet=true;
	}
	else
	{
		struct timeval now;
		struct timespec timeout;
		int retcode;
		int	sec, usec;

		gettimeofday(&now,NULL);

		sec=ulTimeout/1000;
		ulTimeout=ulTimeout-sec*1000;
		usec=ulTimeout*1000;

		timeout.tv_sec = now.tv_sec + sec;
		timeout.tv_nsec = (now.tv_usec +usec) * 1000;
		if(timeout.tv_nsec > 999999999)
		{
				timeout.tv_sec++;
				timeout.tv_nsec -= 1000000000;
		}

		pthread_mutex_lock(&m_mutex);
		retcode = 0;
		while (m_nEvent == 0 && retcode != ETIMEDOUT)
		{
				retcode = pthread_cond_timedwait(&m_cond, &m_mutex, &timeout);
		}
		if (retcode == ETIMEDOUT) {
			bRet=false;
		} 
		else 
		{
			bRet=true;
		}
		if (!m_bManualReset)
			m_nEvent=0;
		pthread_mutex_unlock(&m_mutex);
	}
	return bRet;
#endif
}

void UBXEvent::Reset(void) 
{ 
#ifdef UBTPLATFORM_WIN
	ResetEvent(m_hEvent); 
#else
	pthread_mutex_lock(&m_mutex);
    m_nEvent=0;
    pthread_mutex_unlock(&m_mutex);
#endif
}
