#include "stdafx.h"

#include <Windows.h>
#include <TCHAR.H>
#include <stdlib.h>
#include <locale.h>
#include "ImageToRgn.h"
#include "DrawButton.h"

namespace
{
    /**
    * \brief �ı�����󳤶�
    */
    const int g_nCaptionLen = 50;
}


CDrawButton::CDrawButton(void): 
m_hWnd(NULL), 
m_pNormalStateImg(NULL),
m_pLightStateImg(NULL), 
m_nButtonState(0), 
m_fTrackMouse(FALSE), 
m_fLight(FALSE),
m_hBackgroundDC(NULL),
m_hFont(NULL),
m_hRgn(NULL),
m_hOldBkgBitmap(NULL),
m_crText(RGB(0, 0, 0) ),
m_bkimState(BKLS_HORIZONTAL),
m_fButtonDown(FALSE)
{
    CSingletonAutoT2<CGdiplusGuard>::GetInstance();
}


CDrawButton::~CDrawButton(void)
{
    SAFE_FREE(m_pNormalStateImg);
    SAFE_FREE(m_pLightStateImg);
    SAFE_DELETE(m_hFont);

    if (m_hBackgroundDC)
    {
        SelectObject(m_hBackgroundDC, m_hOldBkgBitmap);
        SAFE_DELETE(m_hBackgroundDC);
        m_hOldBkgBitmap = NULL;
    }

    SAFE_DELETE(m_hRgn);
}


BOOL CDrawButton::Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,
                         UINT nID, LPCTSTR lpszNormalImage, LPCTSTR lpszLightImage,
                         BKIMGLoadState bklState, HDC hdcParent, UINT szText, COLORREF crText)
{
    if(m_hWnd)
    {
        return FALSE;
    }

    WCHAR wszNormalImage[MAX_PATH] = {0};
    WCHAR wszLightImage[MAX_PATH] = {0};
    if(sizeof(TCHAR) == 2)
    {
        wcscpy_s(wszNormalImage, (WCHAR *)lpszNormalImage);
        wcscpy_s(wszLightImage, (WCHAR *)lpszLightImage);
    }
    else
    {
        setlocale(LC_ALL,".936");

        size_t returnValue = 0;
        mbstowcs_s(&returnValue, wszNormalImage, (char *)lpszNormalImage, _tcslen(lpszNormalImage));
        mbstowcs_s(&returnValue, wszLightImage, (char *)lpszLightImage, _tcslen(lpszLightImage));
    }

    m_pNormalStateImg = Image::FromFile(wszNormalImage);
    if( Ok != m_pNormalStateImg->GetLastStatus())
    {
        goto CREATE_FAIL;
    }

    if(NULL != lpszLightImage)
    {
        if(_tcslen(lpszLightImage) != 0)
        {
            m_pLightStateImg = Image::FromFile(wszLightImage);
            if( Ok != m_pLightStateImg->GetLastStatus())
            {
                goto CREATE_FAIL;
            }
        }
    }

    m_hWnd = CreateWindow(_T("BUTTON"), lpszCaption, \
                        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW, \
                        rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, \
                        hParentWnd, (HMENU)nID, NULL, NULL);

    if( NULL == m_hWnd)
    {
        goto CREATE_FAIL;
    }

    m_OldProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)ButtonProc);
    SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

    m_hRgn = CreateRectRgn(0, 0, rect.right - rect.left, rect.bottom - rect.top);

    m_hFont = CreateFont(szText, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET,OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����") );

    m_crText    = crText;
    m_bkimState = bklState;

    if(hdcParent)
    {
        InitBkDC(hdcParent);
    }
    return TRUE;

CREATE_FAIL:

    SAFE_FREE(m_pNormalStateImg);
    SAFE_FREE(m_pLightStateImg);

    return FALSE;
}


BOOL CDrawButton::SetFont(HFONT HFont)
{
    if (HFont == NULL)
    {
        return FALSE;
    }

    if (m_hFont != NULL)
    {
        SAFE_DELETE(m_hFont);
    } 
    
    m_hFont = HFont;
    InvalidataCtrl();
    return TRUE;
}


void CDrawButton::SetColor(COLORREF crText)
{
    m_crText = crText;
    InvalidataCtrl();
}


BOOL CDrawButton::SetWindowRgn(HRGN hRgn)
{
    return FALSE;
}


void CDrawButton::SetCaption(LPCTSTR lpszCaption)
{
    SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)lpszCaption);
}


void CDrawButton::ReSetWindowProc()
{
    if(m_hWnd)
    {
        SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_OldProc);
        //::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
}


LRESULT CALLBACK CDrawButton::ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    CDrawButton *_this = (CDrawButton *)GetWindowLong(hWnd, GWL_USERDATA);
    switch (message)
    {
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_ERASEBKGND:
        _this->Draw((HDC)wParam);
        return TRUE;
    case WM_DRAWITEM:
        {
            //  ��ʱû���õ�
            DRAWITEMSTRUCT *pDrawItemSturct = (DRAWITEMSTRUCT *)lParam;
            if(pDrawItemSturct->itemState & ODS_FOCUS)
            {
                RECT rtFocus = pDrawItemSturct->rcItem;
                rtFocus.left += 3;
                rtFocus.top += 3;
                rtFocus.right -= 3;
                rtFocus.bottom -= 3;
                _this->DrawFocusRect(pDrawItemSturct->hDC, rtFocus);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        {
            _this->OnLbuttonDown(wParam, lParam);
        }
        break;
    case WM_LBUTTONUP:
        {
            _this->OnLbuttonUp(wParam, lParam);
        }
        break;
    case WM_LBUTTONDBLCLK:
        {
            _this->OnLbuttonDBClick(wParam, lParam);
        }
        break;
    case WM_MOUSEMOVE:
        {
            _this->OnMouseMove(wParam, lParam);
        }
        break;
    case WM_MOUSELEAVE:
        {
            _this->OnMouseLeave(wParam, lParam);
        }
        break;
    case WM_DESTROY:
        {
            _this->ReSetWindowProc();
            PostQuitMessage(0); 
        }
        break;
    default:
        return CallWindowProc(_this->m_OldProc, hWnd, message, wParam, lParam);
    }
    return 0;
}


void CDrawButton::InitBkDC(HDC hDC)
{
    //  �Ӹ�����DC�������ݵĻ��ƿؼ��ı���DC m_hBackgroundDC
    //  ��������������Ӧλ�ô��ı�����Ϊ�ؼ��ı���
    if(!m_hBackgroundDC)
    {
        RECT rcCtrl;
        HWND hParent = GetParent(m_hWnd);
        HDC hdcParent = NULL;
        if(!hDC)
        {
            hdcParent = GetDC(hParent);
        }
        else
        {
            hdcParent = hDC;
        }

        GetWindowRect(m_hWnd, &rcCtrl);
        POINT ptLeftTop = {rcCtrl.left, rcCtrl.top};
        POINT ptRightBottom = {rcCtrl.right, rcCtrl.bottom};
        ScreenToClient(hParent, &ptLeftTop);
        ScreenToClient(hParent, &ptRightBottom);
        rcCtrl.left = ptLeftTop.x;
        rcCtrl.top = ptLeftTop.y;
        rcCtrl.right = ptRightBottom.x;
        rcCtrl.bottom = ptRightBottom.y;

        int nWidth = rcCtrl.right - rcCtrl.left;
        int nHeight = rcCtrl.bottom - rcCtrl.top;

        m_hBackgroundDC = CreateCompatibleDC(hdcParent);
        HBITMAP hbmp = CreateCompatibleBitmap(hdcParent, nWidth, nHeight);
        m_hOldBkgBitmap = (HBITMAP)SelectObject(m_hBackgroundDC, hbmp);
        BitBlt(m_hBackgroundDC, 0, 0, nWidth, nHeight, hdcParent, rcCtrl.left, rcCtrl.top, SRCCOPY);
        DeleteObject(hbmp);
        if(!hDC)
        {
            ReleaseDC(hParent, hdcParent);
        }
    }
}


void CDrawButton::OnLbuttonDown(WPARAM wParam, LPARAM lParam)
{
    if(m_nButtonState == STATE_DISABLE)
    {
		return;
    }
    
    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    if(PtInRegion(m_hRgn, pt.x, pt.y))
    {
        SetCapture(m_hWnd);
        m_nButtonState = STATE_DOWN;
    }
    InvalidataCtrl();
}


void CDrawButton::OnLbuttonUp(WPARAM wParam, LPARAM lParam)
{
	if(m_nButtonState == STATE_DISABLE)
    {
		return;
    }

    ReleaseCapture();
    if(m_nButtonState == STATE_DOWN)
    {   
        POINT pt = {};
        GetCursorPos(&pt);
        ScreenToClient(m_hWnd, &pt);
        if(PtInRegion(m_hRgn, pt.x, pt.y))
        {
            //  �ؼ������ڣ����������ٲ����ð�ťΪ����״̬
            //  ���͸������ڵ����Ϣ
            m_fTrackMouse = TRUE;
            m_nButtonState = STATE_FOCUS;
            SendMessage(GetParent(m_hWnd), WM_COMMAND,\
                MAKEWPARAM(GetDlgCtrlID(m_hWnd), BN_CLICKED), (LPARAM)m_hWnd);
//             SendMessage(GetParent(m_hWnd), BN_CLICKED,\
//                 GetDlgCtrlID(m_hWnd), (LPARAM)m_hWnd);
        }
        else
        {
            //  �ؼ��ƶ������⣬ȡ�������ٲ����ð�ťΪ����״̬
            m_fTrackMouse = FALSE;
            m_nButtonState = STATE_UP;
        }
    }
    InvalidataCtrl();
}


void CDrawButton::OnLbuttonDBClick(WPARAM wParam, LPARAM lParam)
{
    //  ������˫��ת��Ϊ���ؼ�����굥����Ϣ
	if(m_nButtonState == STATE_DISABLE)
    {
        return;
    }
    
    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    if(PtInRegion(m_hRgn, pt.x, pt.y))
    {
        SendMessage(m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
    }
}


void CDrawButton::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    //  ��ť����
    if(m_fButtonDown == FALSE && m_nButtonState != STATE_DISABLE)
    {
        POINT pt = {};
        GetCursorPos(&pt);
        ScreenToClient(m_hWnd, &pt);
        if(PtInRegion(m_hRgn, pt.x, pt.y))  //  ����ڿؼ�������
        {
            //  ���û�и���
            if(!m_fTrackMouse)
            {
                //  ��������뿪,����MouseLeave��Ϣ
                TRACKMOUSEEVENT MouseEvent = {sizeof(TRACKMOUSEEVENT)};
                MouseEvent.dwFlags = TME_LEAVE;
                MouseEvent.hwndTrack = m_hWnd;
                MouseEvent.dwHoverTime = 1000;
                TrackMouseEvent(&MouseEvent);

                //  ������걻����
                m_fTrackMouse = TRUE;
            }

            //  ��ťû�а���,�����ð�ť״̬Ϊ����
            if(m_nButtonState != STATE_DOWN)
                m_nButtonState = STATE_FOCUS;
        }
        else // �ƶ��������⣬��ֱ�Ӵ���Ϊ��굯��
        {
            m_nButtonState = STATE_UP; 
        }
        InvalidataCtrl();
    }
}


void CDrawButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    //  ����뿪���������ȡ������
    m_fTrackMouse = FALSE;

    //  ��ť���ã����ð�ťΪ����״̬
    if(m_fButtonDown == FALSE && m_nButtonState != STATE_DISABLE)
    {
        m_nButtonState = STATE_UP;
        InvalidataCtrl();
    }
}


void CDrawButton::Draw(HDC hDC)
{
    //  ��ʼ������DC��m_hBackgroundDC
    //  �ڲ�ֻ���ʼ��һ��
    InitBkDC();

    //  ����Ϊ��hDC��������DC hMemDC�Ϳؼ���С�Ļ���
    //  �������ڵļ���DC m_hBackgroundDC �������µ�hMemDC
    RECT rcWnd = {};
    GetClientRect(m_hWnd, &rcWnd);
    int nWidth = rcWnd.right - rcWnd.left;
    int nHeight = rcWnd.bottom - rcWnd.top;

    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBmpMem = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    HBITMAP hOldBmpMem = (HBITMAP)SelectObject(hMemDC, hBmpMem);

    Image *pDrawImg = NULL;
    if(!m_fLight)
    {
        pDrawImg = m_pNormalStateImg;
    }
    else
    {
        pDrawImg = m_pLightStateImg;
    }

    int nBmpWidth = pDrawImg->GetWidth();
    int nBmpHeight = pDrawImg->GetHeight();

    if (m_bkimState == BKLS_HORIZONTAL)
    {
        nBmpWidth = pDrawImg->GetWidth() / 4;
        nBmpHeight = pDrawImg->GetHeight();
    }
    else
    {
        nBmpWidth = pDrawImg->GetWidth();
        nBmpHeight = pDrawImg->GetHeight() / 4;
    }
    
    //  ���Ƹ����ڱ���ͼ
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, m_hBackgroundDC, 0, 0, SRCCOPY);

    //  ���»���ͼƬ���ı��������ļ���DC hMemDC

    /*
    SetStretchBltMode(hDC, HALFTONE);
    StretchBlt(hDC, 0, 0, nWidth, nHeight, hMemDC, nBmpWidth * m_nButtonState,\
        0, nBmpWidth, nBmpHeight, SRCCOPY);
        */
    int nButtonState = 0;
    if(m_fButtonDown)
    {
        nButtonState = STATE_DOWN;
    }
    else
    {
        nButtonState = m_nButtonState;
    }

    SetBkMode(hMemDC, TRANSPARENT);
    Graphics graphics(hMemDC); // Create a GDI+ graphics object
    RectF gRect; 
    gRect.X = (REAL)rcWnd.left;
    gRect.Y = (REAL)rcWnd.top; 
    gRect.Width = (REAL)nWidth; 
    gRect.Height = (REAL)nHeight;

    if (m_bkimState == BKLS_HORIZONTAL)
    {
        graphics.DrawImage(pDrawImg, gRect, (REAL)nBmpWidth * nButtonState, 0, (REAL)nBmpWidth, (REAL)nBmpHeight, UnitPixel);
    }
    else
    {
        graphics.DrawImage(pDrawImg, gRect, 0, (REAL)nBmpHeight * nButtonState, (REAL)nBmpWidth, (REAL)nBmpHeight, UnitPixel);
    }

    //  ���ڰ��°�ťʱ,��������ı������ƶ�2�����ص�
    //  ���Ʋ�������Ч��
    if(m_nButtonState == STATE_DOWN)
    {
        rcWnd.top += 2;
    }

    //  �����ı�
    TCHAR szCaption[g_nCaptionLen] = {};
    GetWindowText(m_hWnd, szCaption, g_nCaptionLen - 1);
    if(_tcslen(szCaption) > 0)
    {
        SetTextColor(hMemDC, m_crText);
        TCHAR *pReturnPos = NULL;
        HFONT hOldFont = NULL;
        if(m_hFont)
        {
            hOldFont = (HFONT)SelectObject(hMemDC, m_hFont);
        }
        if(pReturnPos = _tcsstr(szCaption, _T("\\r\\n")))
        {
            TCHAR *pszSecondLine = pReturnPos + 4;
            rcWnd.bottom = (rcWnd.bottom + rcWnd.top) / 2;
            rcWnd.top += 1;
            DrawText(hMemDC, szCaption, pReturnPos -  szCaption, &rcWnd, DT_CENTER);
            rcWnd.top -= 1;
            rcWnd.bottom = rcWnd.bottom * 2 - rcWnd.top;
            rcWnd.top = (rcWnd.bottom + rcWnd.top) / 2;
            rcWnd.top -= 1;
            DrawText(hMemDC, pszSecondLine, _tcslen(pszSecondLine), &rcWnd, DT_CENTER);
        }
        else
        {
            DrawText(hMemDC, szCaption, _tcslen(szCaption), &rcWnd, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        }
        if(m_hFont)
        {
            SelectObject(hMemDC, hOldFont);
        }
    }

    //  �����ƺõļ���DC hMemDC�����ؿؼ�hDC������������ƹ���
    BitBlt(hDC, 0, 0, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	SetBkMode(hMemDC, OPAQUE);
    graphics.ReleaseHDC(hMemDC);
    SelectObject(hMemDC, hOldBmpMem);
    DeleteObject(hBmpMem);
    DeleteDC(hMemDC);
}


void CDrawButton::DrawFocusRect(HDC hDC,RECT &rcWnd)
{
    HPEN hOldPen = NULL;
    HPEN hDrawPen = CreatePen(PS_DOT, 2, RGB(0,0,0));

    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    hOldPen = (HPEN)SelectObject(hDC, hDrawPen);
    Rectangle(hDC, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);

    SelectObject(hDC,hOldBrush);
    SelectObject(hDC,hOldPen);
    DeleteObject(hDrawPen);
}


BOOL CDrawButton::LightingCtrl(BOOL fLight)
{
    if(m_pLightStateImg)
    {
        m_fLight = fLight;
        InvalidataCtrl();
        return TRUE;
    }
    return FALSE;
}


BOOL CDrawButton::InvalidataCtrl()
{
    HDC hDC = GetDC(m_hWnd);
    Draw(hDC);
    ReleaseDC(m_hWnd, hDC);
    return TRUE;
}


void CDrawButton::EnableButton(BOOL fEnable)
{
    if(fEnable)
    {
        m_nButtonState = STATE_UP;
    }
    else
    {
        m_nButtonState = STATE_DISABLE;
    }

	InvalidataCtrl();
}


HWND CDrawButton::GetSafeHWnd()
{
    return m_hWnd;
}


void CDrawButton::SetState(UINT nState)
{
    if(nState == STATE_UP)
    {
        m_fButtonDown = false;
        POINT pt = {};
        GetCursorPos(&pt);
        RECT rcWnd = {};
        if(PtInRegion(m_hRgn, pt.x, pt.y))
        {
            TRACKMOUSEEVENT MouseEvent = {sizeof(TRACKMOUSEEVENT)};
            MouseEvent.dwFlags = TME_LEAVE;
            MouseEvent.hwndTrack = m_hWnd;
            MouseEvent.dwHoverTime = 1000;
            TrackMouseEvent(&MouseEvent);

            m_fTrackMouse = TRUE;
            m_nButtonState = STATE_FOCUS;
        }
        else
        {
            m_fTrackMouse = FALSE;
            m_nButtonState = STATE_UP;          
        }
        InvalidataCtrl();
    }
    else if(nState == STATE_DOWN)
    {
        m_fButtonDown = true;
        InvalidataCtrl();
    }
}