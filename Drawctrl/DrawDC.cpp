#include "stdafx.h"

#include <windows.h>
#include "DrawDC.h"

CDrawDC::CDrawDC(void)
{
    m_hDC           = NULL;
    m_hOldDrawDCBmp = NULL;
}

CDrawDC::~CDrawDC(void)
{
    DeleteDrawDC();
}

BOOL CDrawDC::CreateDrawDC(HDC hDC, int nWidth, int nHeight)
{
    DeleteDrawDC();
    m_hDC = CreateCompatibleDC(hDC);
    HBITMAP hbmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    m_hOldDrawDCBmp = (HBITMAP)SelectObject(m_hDC, hbmp);
    DeleteObject(hbmp);
    return TRUE;
}

void CDrawDC::DeleteDrawDC()
{
    if(m_hDC)
    {
        SelectObject(m_hDC, m_hOldDrawDCBmp);
        DeleteDC(m_hDC);
        m_hDC = NULL;
        m_hOldDrawDCBmp = NULL;
    }
}
