#pragma once

class CDllLoaderGuard
{
public:

    CDllLoaderGuard(LPCTSTR pFileName)
    {
        m_hModule = ::LoadLibrary(pFileName);
        m_isOk = m_hModule == NULL ? FALSE : TRUE;
    }

    ~CDllLoaderGuard()
    {
        if (m_hModule != NULL)
        {
            ::FreeLibrary(m_hModule);
        }
    }

    FARPROC  GetProcAddress(LPCSTR pProcName)
    {
        if (m_isOk)
        {
            return ::GetProcAddress(m_hModule, pProcName);
        }

        return NULL;
    }

    HRSRC FindResource(LPCTSTR pName, LPCTSTR pType)
    {
        if (m_isOk)
        {
            return ::FindResource(m_hModule, pName, pType);
        }

        return NULL;
    }

    inline BOOL isLoadOk()
    {
        return m_isOk;
    }

private:

    CDllLoaderGuard(const CDllLoaderGuard &);

    CDllLoaderGuard & operator = (const CDllLoaderGuard &);

private:
  
    HMODULE         m_hModule;

    BOOL            m_isOk;
};