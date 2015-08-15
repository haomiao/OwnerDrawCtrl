#pragma once

class CCriticalSectionLock
{
public:

    CCriticalSectionLock(void)
    {
        ::InitializeCriticalSection(&m_criticalSection);
    }

    virtual ~CCriticalSectionLock(void)
    {
        ::DeleteCriticalSection(&m_criticalSection);
    }

    void Lock()
    {
        ::EnterCriticalSection(&m_criticalSection);
    }

    BOOL TryLock()
    {
        return ::TryEnterCriticalSection(&m_criticalSection);
    }

    void UnLock()
    {
        ::LeaveCriticalSection(&m_criticalSection);
    }

private:

    CCriticalSectionLock(const CCriticalSectionLock&);
    CCriticalSectionLock & operator = (const CCriticalSectionLock &);

private:

    CRITICAL_SECTION    m_criticalSection;
};



class AutoCriticalSectionLock
{
public:

    AutoCriticalSectionLock(CCriticalSectionLock&lock):m_lock(lock)
    {
        m_lock.Lock();
    }

    ~AutoCriticalSectionLock()
    {
        m_lock.UnLock();
    }

private:

    CCriticalSectionLock  & m_lock;
};