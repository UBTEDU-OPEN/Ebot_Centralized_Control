//UBXCritSec.h
//
#ifndef __UBX_CRITSEC_H__
#define __UBX_CRITSEC_H__

#include "ubtutil_global.h"

#define UBTPLATFORM_WIN

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
//#include <winsock2.h>
#include <windows.h>

#include "ubtutil.h"


class UBTUTILSHARED_EXPORT UBXCritSec
{
public:
    UBXCritSec();
    virtual ~UBXCritSec();

public:
    void Lock();
    void Unlock();

protected:
#ifdef UBTPLATFORM_WIN
    CRITICAL_SECTION m_CritSec;
#else
	pthread_mutex_t		m_pMutex;
    pthread_mutexattr_t m_pMutexAttr;
#endif
};

class UBTUTILSHARED_EXPORT UBXAutoLock
{
public:
    UBXAutoLock(UBXCritSec * plock);
    virtual ~UBXAutoLock();

protected:
    UBXCritSec * m_pLock;
};

class UBTUTILSHARED_EXPORT UBXEvent
{
public:
    UBXEvent(bool bManualReset = false);
    virtual ~UBXEvent();

public:
    void Set(void);
    bool Wait(long ulTimeout);
    void Reset(void);

protected:
#ifdef UBTPLATFORM_WIN
	HANDLE m_hEvent;
#else
	pthread_mutex_t m_mutex;
	pthread_cond_t	m_cond;
	int				m_nEvent;
	bool			m_bManualReset;
#endif
};

#endif
