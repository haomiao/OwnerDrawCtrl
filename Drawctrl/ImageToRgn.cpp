#include "stdafx.h"

#include <Windows.h>
#include <TCHAR.H>
#include <stdlib.h>
#include "ImageToRgn.h"

CImageToRgn::CImageToRgn(void)
{

}

CImageToRgn::~CImageToRgn(void)
{
    for(unsigned int i = 0; i < m_vecRgns.size(); i++)
    {
        if(m_vecRgns[i])
        {
            DeleteObject(m_vecRgns[i]);
        }
    }

    m_vecRgns.clear();
}


void CImageToRgn::AddRgn(HRGN rgn)
{
    m_vecRgns.push_back(rgn);
}


HRGN CImageToRgn::GetRgn(UINT nIndex)
{
    if(nIndex >= m_vecRgns.size())
    {
        return NULL;
    }
    return m_vecRgns[nIndex];
}


HRGN CImageToRgn::CreateRgn(LPCTSTR lpszImage, int nRngWidth, int nRngHeight, \
        int nXPos, int nYPos, int nImgWidth, int nImgHeight, COLORREF cfTransparent)
{
    RECT rcMemDC = {0, 0, nRngWidth, nRngHeight};
    WCHAR wszImage[MAX_PATH] = {};
    if(sizeof(TCHAR) == 2)
    {
        wcscpy_s(wszImage, (WCHAR *)lpszImage);
    }
    else
    {
        size_t returnValue = 0;
        mbstowcs_s(&returnValue, wszImage, (char *)lpszImage, _tcslen(lpszImage));
    }

    HDC hDC = GetDC(GetDesktopWindow());
    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, nRngWidth, nRngHeight);
    HBITMAP hOldMemBmp = (HBITMAP)SelectObject(hMemDC, hMemBmp);

    HBRUSH hBrush = CreateSolidBrush(cfTransparent);
    FillRect(hMemDC, &rcMemDC, hBrush);
    DeleteObject(hBrush);

    SetStretchBltMode(hMemDC, HALFTONE);
    Graphics graphics(hMemDC); // Create a GDI+ graphics object
    Image image(wszImage);
    RectF gRect; 
    gRect.X = (REAL)0;
    gRect.Y = (REAL)0; 
    gRect.Width = (REAL)nRngWidth; 
    gRect.Height = (REAL)nRngHeight;
    int nImgToShowWidth = 0;
    if(nImgWidth == Default_Width)
    {
        nImgToShowWidth = image.GetWidth();
    }
    else if(nImgWidth == Default_Height)
    {
        nImgToShowWidth = image.GetHeight();
    }
    else
    {
        nImgToShowWidth = nImgWidth;
    }

    int nImgToShowHeight = 0;
    if(nImgHeight == Default_Width)
    {
        nImgToShowHeight = image.GetWidth();
    }
    else if(nImgHeight == Default_Height)
    {
        nImgToShowHeight = image.GetHeight();
    }
    else
    {
        nImgToShowHeight = nImgHeight;
    }
    graphics.DrawImage(&image, gRect, (REAL)nXPos, (REAL)nYPos, (REAL)nImgToShowWidth, (REAL)nImgToShowHeight, UnitPixel);

    int i = 0;
    int j = 0;
    POINT ptStart = {-1, -1};
    POINT ptEnd = {-1, -1};
    COLORREF color = 0;
    HRGN hImgRgn = CreateRectRgn(0, 0, nRngWidth, nRngHeight);
    for(i = 0; i < nRngWidth; i++)
    {
        for(j = 0; j < nRngHeight; j++)
        {
            color = GetPixel(hMemDC, i, j);
            if(color == cfTransparent)
            {
                if(ptStart.x == -1)
                {
                    ptStart.x = i;
                    ptStart.y = j;
                }
            }
            else
            {
                if(ptStart.x != -1)
                {
                    ptEnd.x = i + 1;
                    ptEnd.y = j;
                    HRGN hTransparentRgn = CreateRectRgn(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                    CombineRgn(hImgRgn, hImgRgn, hTransparentRgn, RGN_DIFF);

                    DeleteObject(hTransparentRgn);
                    ptStart.x = -1;
                }
            }
        }
        if(ptStart.x != -1)
        {
            ptEnd.x = i + 1;
            ptEnd.y = j;
            HRGN hTransparentRgn = CreateRectRgn(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
            CombineRgn(hImgRgn, hImgRgn, hTransparentRgn, RGN_DIFF);

            DeleteObject(hTransparentRgn);
            ptStart.x = -1;
        }
    }

    SelectObject(hMemDC, hOldMemBmp);
    DeleteObject(hMemBmp);
    DeleteDC(hMemDC);
    ReleaseDC(GetDesktopWindow(), hDC);

    return hImgRgn;
}