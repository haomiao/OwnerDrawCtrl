#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

class CGdiplusGuard
{
public:

    CGdiplusGuard()
    {
        m_isOk = ::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL) == Gdiplus::Ok? TRUE : FALSE;
    }

    virtual ~CGdiplusGuard()
    {
        ::GdiplusShutdown(m_gdiplusToken);
    }

    inline BOOL isOk()
    {
        return m_isOk;
    }

private:

    CGdiplusGuard(const CGdiplusGuard &);

    CGdiplusGuard & operator = (const CGdiplusGuard &);

private:
    
    GdiplusStartupInput     m_gdiplusStartupInput;

    ULONG_PTR               m_gdiplusToken;

    BOOL                    m_isOk;
};