#pragma once

#include "Lock.h"

//  提供任意类型的单例实现
//  并提供安全的资源释放、多线程安全
//  手动销毁单例模式资源
//  提供初始化时，其他附加操作
template<typename T>
class CSingletonManual:public T
{
public:

    static CSingletonManual * GetInstance()
    {
        if (m_pInstance == NULL)
        {
            AutoCriticalSectionLock lock(m_criticalSectionLock);
            if (m_pInstance == NULL)
            {
                m_pInstance = new CSingletonManual();
            }
        }
        return m_pInstance;
    }

    static void DestoryInstance()
    {
        if (m_pInstance != NULL)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

protected:

    CSingletonManual(void)
    {

    }

    virtual ~CSingletonManual(void)
    {

    }

private:

    CSingletonManual(const CSingletonManual&);
    CSingletonManual & operator = (const CSingletonManual &);

private:

    static CSingletonManual        *m_pInstance;

    static CCriticalSectionLock    m_criticalSectionLock;
};

template<typename T>
CSingletonManual<T>* CSingletonManual<T>::m_pInstance = NULL;

template<typename T>
CCriticalSectionLock CSingletonManual<T>::m_criticalSectionLock;


//  提供单例实现
//  并提供安全的资源释放、多线程安全
//  手动动销毁单例模板资源
template<typename T>
class CSingletonManual2
{
public:

    static T * GetInstance()
    {
        if (m_pInstance == NULL)
        {
            AutoCriticalSectionLock lock(m_criticalSectionLock);
            if (m_pInstance == NULL)
            {
                m_pInstance = new T();
            }
        }
        return m_pInstance;
    }

    static void DestoryInstance()
    {
        if (m_pInstance != NULL)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

protected:

    CSingletonManual2(void)
    {

    }

    virtual ~CSingletonManual2(void)
    {

    }

private:

    CSingletonManual2(const CSingletonManual2&);
    CSingletonManual2 & operator = (const CSingletonManual2 &);

private:

    static T                       *m_pInstance;

    static CCriticalSectionLock    m_criticalSectionLock;
};

template<typename T>
T * CSingletonManual2<T>::m_pInstance = NULL;

template<typename T>
CCriticalSectionLock CSingletonManual2<T>::m_criticalSectionLock;


//  提供单例实现
//  并提供安全的资源释放、多线程安全
//  自动销毁单例模板资源
template<typename T>
class CSingletonAutoT
{
public:

    static T * GetInstance()
    {
        static T instance;
        return &instance;
    }

protected:

    CSingletonAutoT(void)
    {

    }

    virtual ~CSingletonAutoT(void)
    {

    }

private:

    CSingletonAutoT(const CSingletonAutoT&);
    CSingletonAutoT & operator = (const CSingletonAutoT &);

};


//  提供单例实现
//  并提供安全的资源释放、多线程安全
//  自动销毁单例模板资源
//  提供初始化时，其他附加操作
template<typename T>
class CSingletonAutoT2:public T
{
public:

    static CSingletonAutoT2 * GetInstance()
    {
        static CSingletonAutoT2 instance;
        return &instance;
    }

protected:

    CSingletonAutoT2(void)
    {

    }

    virtual ~CSingletonAutoT2(void)
    {

    }

private:

    CSingletonAutoT2(const CSingletonAutoT2&);
    CSingletonAutoT2 & operator = (const CSingletonAutoT2 &);

};


//  提供单例实现
//  并提供安全的资源释放、多线程安全
//  自动销毁单例模板资源
template<typename T>
class CSingletonAuto
{
public:

    static T * GetInstance()
    {
        if (m_pInstance == NULL)
        {
            AutoCriticalSectionLock lock(m_criticalSectionLock);
            if (m_pInstance == NULL)
            {
                m_pInstance = new T();
            }
        }
        return m_pInstance;
    }

protected:

    CSingletonAuto(void)
    {

    }

    virtual ~CSingletonAuto(void)
    {

    }

private:

    class CDeleteRes
    {
    public:

        ~CDeleteRes()
        {
            if (m_pInstance != NULL)
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

private:

    CSingletonAuto(const CSingletonAuto&);
    CSingletonAuto & operator = (const CSingletonAuto &);

private:

    static T                       *m_pInstance;

    static CCriticalSectionLock    m_criticalSectionLock;

    static CDeleteRes              m_deleteRes;
};

template<typename T>
CSingletonAuto<T>* CSingletonAuto<T>::m_pInstance = NULL;

template<typename T>
CCriticalSectionLock CSingletonAuto<T>::m_criticalSectionLock;

// template<typename T>
// CSingletonAuto<T>::CDeleteRes CSingletonAuto<T>::m_deleteRes;

/*
//  提供任意类型的单例实现
//  并提供安全的资源释放、多线程安全
//  自动销毁单例模板资源
template<typename T>
class CSingletonTAuto:pubic T
{
public:

    static CSingletonTAuto * GetInstance()
    {
        if (m_pInstance == NULL)
        {
            AutoCriticalSectionLock lock(m_criticalSectionLock);
            if (m_pInstance == NULL)
            {
                m_pInstance = new CSingletonTAuto();
            }
        }
        return m_pInstance;
    }

protected:

    CSingletonTAuto(void)
    {

    }

    virtual ~CSingletonTAuto(void)
    {

    }

public:

    class CDeleteRes
    {
    public:

        ~CDeleteRes()
        {
            if (m_pInstance != NULL)
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    };

private:

    CSingletonTAuto(const CSingletonTAuto&);
    CSingletonTAuto & operator = (const CSingletonTAuto &);

private:

    static CSingletonTAuto          *m_pInstance;

    static CCriticalSectionLock    m_criticalSectionLock;

    static CDeleteRes              m_deleteRes;
};

template<typename T>
CSingletonTAuto<T>* CSingletonTAuto<T>::m_pInstance = NULL;

template<typename T>
CCriticalSectionLock CSingletonTAuto<T>::m_criticalSectionLock;

template<typename T>
CSingletonTAuto<T>::DR CSingletonTAuto<T>::m_deleteRes;


*/