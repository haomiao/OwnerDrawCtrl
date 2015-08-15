#pragma once

#include "Lock.h"

//  �ṩ�������͵ĵ���ʵ��
//  ���ṩ��ȫ����Դ�ͷš����̰߳�ȫ
//  �ֶ����ٵ���ģʽ��Դ
//  �ṩ��ʼ��ʱ���������Ӳ���
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


//  �ṩ����ʵ��
//  ���ṩ��ȫ����Դ�ͷš����̰߳�ȫ
//  �ֶ������ٵ���ģ����Դ
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


//  �ṩ����ʵ��
//  ���ṩ��ȫ����Դ�ͷš����̰߳�ȫ
//  �Զ����ٵ���ģ����Դ
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


//  �ṩ����ʵ��
//  ���ṩ��ȫ����Դ�ͷš����̰߳�ȫ
//  �Զ����ٵ���ģ����Դ
//  �ṩ��ʼ��ʱ���������Ӳ���
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


//  �ṩ����ʵ��
//  ���ṩ��ȫ����Դ�ͷš����̰߳�ȫ
//  �Զ����ٵ���ģ����Դ
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
//  �ṩ�������͵ĵ���ʵ��
//  ���ṩ��ȫ����Դ�ͷš����̰߳�ȫ
//  �Զ����ٵ���ģ����Դ
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