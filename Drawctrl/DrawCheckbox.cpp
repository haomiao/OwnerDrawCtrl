
#include "stdafx.h"
#include <Windows.h>
#include <TCHAR.H>
#include <stdlib.h>
#include <locale.h>
#include "DrawCheckbox.h"
#include "ImageToRgn.h"

//  加载新的GDI图像设备接口支持GDI32和新的特性
#pragma comment(lib, "Msimg32.lib")

namespace
{
    const int g_nCaptionLen = 50;
}


CDrawCheckbox::CDrawCheckbox(void):
m_hWnd(NULL),
m_pNormalStateImg(NULL),
m_pLightStateImg(NULL),
m_nCtrlState(0), 
m_fTrackMouse(FALSE), 
m_fLight(FALSE),
m_hFont(NULL),
m_hCtrlRgn(NULL),
m_bCheck(FALSE),
m_colorText(RGB(0, 0, 0) ),
m_bkimState(BKLS_HORIZONTAL)
{
    CSingletonAutoT2<CGdiplusGuard>::GetInstance();
}


CDrawCheckbox::~CDrawCheckbox(void)
{
    SAFE_FREE(m_pNormalStateImg);
    SAFE_FREE(m_pLightStateImg);
    SAFE_DELETE(m_hFont);
    SAFE_DELETE(m_hCtrlRgn);
}


BOOL CDrawCheckbox::Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,\
                          UINT nID, LPCTSTR lpszImage, LPCTSTR lpszLightImage,BKIMGLoadState bklState, COLORREF colorText, UINT szText, HDC hdcParent)
{
    if(m_hWnd)
    {
        return FALSE;
    }

    WCHAR wszImage[MAX_PATH] = {0};
    WCHAR wszLightImage[MAX_PATH] = {0};
    if(sizeof(TCHAR) == 2)
    {
        wcscpy_s(wszImage, (WCHAR *)lpszImage);
        wcscpy_s(wszLightImage, (WCHAR *)lpszLightImage);
    }
    else
    {
        setlocale(LC_ALL,".936");

        size_t returnValue = 0;
        mbstowcs_s(&returnValue, wszImage, (char *)lpszImage, _tcslen(lpszImage));
        mbstowcs_s(&returnValue, wszLightImage, (char *)lpszLightImage, _tcslen(lpszLightImage));
    }

    m_pNormalStateImg = Image::FromFile(wszImage);
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

	m_colorText = colorText;
    m_bkimState = bklState;

    WNDCLASS wc; 
	// Register the main window class. 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = (WNDPROC) CheckboxProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = GetModuleHandle(NULL); 
	wc.hIcon = NULL; 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName =  NULL; 
	wc.lpszClassName = _T("CDrawCheckbox"); 

	if (!RegisterClass(&wc)) 
	{
		if(ERROR_CLASS_ALREADY_EXISTS != GetLastError())
        {
            return FALSE;
        }
	}

	m_hWnd = CreateWindow(_T("CDrawCheckbox"), lpszCaption, \
		WS_CHILD | WS_VISIBLE | WS_TABSTOP, \
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, \
		hParentWnd,(HMENU)nID,NULL,NULL);

	if( NULL == m_hWnd)
	{
		return FALSE; 
	}

    SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

    m_hFont = CreateFont(szText, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, \
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	//m_hCtrlRgn = CImageToRgn::CreateRgn(lpszImage, rect.bottom - rect.top, rect.bottom - rect.top, 0, 0, CImageToRgn::Default_Height);
    //m_hCtrlRgn = CreateRectRgn(0, 0, rect.right - rect.left, rect.bottom - rect.top);
    m_hCtrlRgn = CreateRectRgn(0, 0, rect.bottom - rect.top, rect.bottom - rect.top);

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


BOOL CDrawCheckbox::Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,\
                          UINT nID, LPCTSTR lpszImage, LPCTSTR lpszLightImage, BKIMGLoadState bklState, COLORREF colorText, COLORREF colorBackgroud, UINT szText)
{
    if(m_hWnd)
    {
        return FALSE;
    }

    WCHAR wszImage[MAX_PATH] = {0};
    WCHAR wszLightImage[MAX_PATH] = {0};
    if(sizeof(TCHAR) == 2)
    {
        wcscpy_s(wszImage, (WCHAR *)lpszImage);
        wcscpy_s(wszLightImage, (WCHAR *)lpszLightImage);
    }
    else
    {
        setlocale(LC_ALL,".936");

        size_t returnValue = 0;
        mbstowcs_s(&returnValue, wszImage, (char *)lpszImage, _tcslen(lpszImage));
        mbstowcs_s(&returnValue, wszLightImage, (char *)lpszLightImage, _tcslen(lpszLightImage));
    }

    m_pNormalStateImg = Image::FromFile(wszImage);
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

	m_colorText = colorText;
    m_bkimState = bklState;

    WNDCLASS wc; 
	// Register the main window class. 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = (WNDPROC) CheckboxProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = GetModuleHandle(NULL); 
	wc.hIcon = NULL; 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.lpszMenuName =  NULL; 
	wc.lpszClassName = _T("CDrawCheckbox"); 

	if (!RegisterClass(&wc)) 
	{
		if(ERROR_CLASS_ALREADY_EXISTS != GetLastError())
			return FALSE; 
	}

	m_hWnd = CreateWindow(_T("CDrawCheckbox"), lpszCaption, \
		WS_CHILD | WS_VISIBLE | WS_TABSTOP, \
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, \
		hParentWnd,(HMENU)nID,NULL,NULL);

	if( NULL == m_hWnd)
	{
		return FALSE; 
	}

    SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

    m_hFont = CreateFont(szText, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, \
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

	//m_hCtrlRgn = CImageToRgn::CreateRgn(lpszImage, rect.bottom - rect.top, rect.bottom - rect.top, 0, 0, CImageToRgn::Default_Height);
    //m_hCtrlRgn = CreateRectRgn(0, 0, rect.right - rect.left, rect.bottom - rect.top);
    m_hCtrlRgn = CreateRectRgn(0, 0, rect.bottom - rect.top, rect.bottom - rect.top);

	if(!m_DrawBackgroundDC.m_hDC)
	{
		InitBkDC();
	}

    return TRUE;

CREATE_FAIL:
   
    SAFE_FREE(m_pNormalStateImg);
    SAFE_FREE(m_pLightStateImg);

    return FALSE;
}


void CDrawCheckbox::SetCheck(BOOL bCheck)
{
	m_bCheck = bCheck;
	InvalidataCtrl();
}


BOOL CDrawCheckbox::GetCheck()
{
	return m_bCheck;
}


BOOL CDrawCheckbox::SetFont(HFONT HFont)
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


void CDrawCheckbox::SetColor(COLORREF crText)
{
    m_colorText = crText;
    InvalidataCtrl();
}


BOOL CDrawCheckbox::SetWindowRgn(HRGN hRgn)
{
    return FALSE;
}


void CDrawCheckbox::SetCaption(LPCTSTR lpszCaption)
{
    SendMessage(m_hWnd, WM_SETTEXT, 0, (LPARAM)lpszCaption);
}


void CDrawCheckbox::EnableButton(BOOL fEnable)
{
    if(fEnable)
    {
        m_nCtrlState = STATE_UP;
    }
    else
    {
        m_nCtrlState = STATE_DISABLE;
    }

    InvalidataCtrl();
}


void CDrawCheckbox::DestroyWindow()
{
    if(m_hWnd)
    {
        //::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
    
}


void CDrawCheckbox::Draw(HDC hDC)
{
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
        nBmpWidth = pDrawImg->GetWidth() / 8;
        nBmpHeight = pDrawImg->GetHeight();
    }
    else
    {
        nBmpWidth = pDrawImg->GetWidth();
        nBmpHeight = pDrawImg->GetHeight() / 8;
    }

    //绘制父窗口背景图
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, m_DrawBackgroundDC.m_hDC, 0, 0, SRCCOPY);

    //绘制背景图，高宽一样大小
    //StretchBlt(hDC, 0, nYPos, nWidth, nHeight - nYPos, hMemDC, nXBmpPos,\
    //    0, nBmpWidth, nBmpHeight, SRCCOPY);
    SetBkMode(hMemDC, TRANSPARENT);
    Graphics graphics(hMemDC); // Create a GDI+ graphics object
    RectF gRect; 
    gRect.X = (REAL)0;
    gRect.Y = (REAL)0; 
    gRect.Width = (REAL)nHeight; 
    gRect.Height = (REAL)nHeight;

    if (m_bkimState == BKLS_HORIZONTAL)
    {
        graphics.DrawImage(pDrawImg, gRect, (REAL)nBmpWidth * m_nCtrlState + m_bCheck * 4 * nBmpWidth, 0, (REAL)nBmpWidth, (REAL)nBmpHeight, UnitPixel);
    }
    else
    {
        graphics.DrawImage(pDrawImg, gRect, 0, (REAL)nBmpHeight * m_nCtrlState + m_bCheck * 4 * nBmpWidth, (REAL)nBmpWidth, (REAL)nBmpHeight, UnitPixel);
    }

    //  绘制文本
    //  当前绘制文本为单行,若绘制多行，可修改一下源码
    TCHAR szCaption[g_nCaptionLen] = {};
    GetWindowText(m_hWnd, szCaption, g_nCaptionLen - 1);
    if(_tcslen(szCaption) > 0)
	{
		HFONT hOldFont = (HFONT)SelectObject(hMemDC, m_hFont);
		rcWnd.left += nHeight + 2;
		rcWnd.top ++;
		SetTextColor(hMemDC, m_colorText);
		DrawText(hMemDC, szCaption, _tcslen(szCaption), &rcWnd, DT_VCENTER|DT_SINGLELINE);
		SelectObject(hMemDC, hOldFont);
	}

	BitBlt(hDC, 0, 0, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

	SetBkMode(hMemDC, OPAQUE);
	graphics.ReleaseHDC(hMemDC);
    SelectObject(hMemDC, hOldBmpMem);
    DeleteObject(hBmpMem);
    DeleteDC(hMemDC);
}


BOOL CDrawCheckbox::InvalidataCtrl()
{
    HDC hDC = GetDC(m_hWnd);
    Draw(hDC);
    ReleaseDC(m_hWnd, hDC);
    return TRUE;
}


BOOL CDrawCheckbox::LightingCtrl(BOOL fLight)
{
    if(m_pLightStateImg)
    {
        m_fLight = fLight;
        InvalidataCtrl();
        return TRUE;
    }
    return FALSE;
}


void CDrawCheckbox::InitBkDC(HDC hDC)
{
	if(!m_DrawBackgroundDC.m_hDC)
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

		m_DrawBackgroundDC.CreateDrawDC(hdcParent, nWidth, nHeight);
		BitBlt(m_DrawBackgroundDC.m_hDC, 0, 0, nWidth, nHeight, hdcParent, rcCtrl.left, rcCtrl.top, SRCCOPY);

		if(!hDC)
		{
			ReleaseDC(hParent, hdcParent);
		}
	}
}


LRESULT CALLBACK CDrawCheckbox::CheckboxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    CDrawCheckbox *_this = (CDrawCheckbox *)GetWindowLong(hWnd, GWL_USERDATA);
	if(_this)
	{
		switch (message)
		{
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code here...
				_this->InitBkDC();
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_ERASEBKGND:
			{
				_this->Draw((HDC)wParam);
			}
			return TRUE;
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
                _this->DestroyWindow();
                PostQuitMessage(0);
            }
            break;
		default:
			break;
		}
        return DefWindowProc(hWnd, message, wParam, lParam);
	}

    return DefWindowProc(hWnd, message, wParam, lParam);
}


HWND CDrawCheckbox::GetSafeHWnd()
{
    return m_hWnd;
}


void CDrawCheckbox::OnLbuttonDown(WPARAM wParam, LPARAM lParam)
{
    if(m_nCtrlState == STATE_DISABLE)
    {
        return;
    }
    
    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    if(PtInRegion(m_hCtrlRgn, pt.x, pt.y))
    {
        //  设置控件是否被选中状态以及控件处于焦点状态
        m_bCheck = !m_bCheck;
        //SetCapture(m_hWnd);
        SetCheck(m_bCheck);
        m_nCtrlState = STATE_FOCUS;
        SendMessage(GetParent(m_hWnd), WM_CHECKBOXCLICKED, GetDlgCtrlID(m_hWnd), m_bCheck);
        //         SendMessage(GetParent(m_hWnd), WM_COMMAND,\
        //             MAKEWPARAM(GetDlgCtrlID(m_hWnd), BN_CLICKED), m_bCheck/*(LPARAM)m_hWnd*/);
    }
}


void CDrawCheckbox::OnLbuttonUp(WPARAM wParam, LPARAM lParam)
{
    if(m_nCtrlState == STATE_DISABLE)
    {
        return;
    }

    //ReleaseCapture();
    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    if(PtInRegion(m_hCtrlRgn, pt.x, pt.y))
    {
        //  控件区域内，设置鼠标跟踪并设置按钮为焦点状态
        //  发送给父窗口点击消息
        m_fTrackMouse = TRUE;
        m_nCtrlState = STATE_FOCUS;
    }
    else
    {
        //  控件移动区域外，取消鼠标跟踪并设置按钮为弹起状态
        m_fTrackMouse = FALSE;
        m_nCtrlState = STATE_UP;
    }

    InvalidataCtrl();
}


void CDrawCheckbox::OnLbuttonDBClick(WPARAM wParam, LPARAM lParam)
{
    //  鼠标左键双击转化为本控件的鼠标单击消息
    if(m_nCtrlState == STATE_DISABLE)
    {
        return;
    }

    POINT pt = {};
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);
    if(PtInRegion(m_hCtrlRgn, pt.x, pt.y))
    {
        SendMessage(m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
    }
}


void CDrawCheckbox::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    //  按钮可用
    if(m_nCtrlState != STATE_DISABLE)
    {
        POINT pt = {};
        GetCursorPos(&pt);
        ScreenToClient(m_hWnd, &pt);
        if(PtInRegion(m_hCtrlRgn, pt.x, pt.y))  //  鼠标在控件区域内
        {
            //  鼠标没有跟踪
            if(!m_fTrackMouse)
            {
                //  跟踪鼠标离开,触发MouseLeave消息
                TRACKMOUSEEVENT MouseEvent = {sizeof(TRACKMOUSEEVENT)};
                MouseEvent.dwFlags = TME_LEAVE;
                MouseEvent.hwndTrack = m_hWnd;
                MouseEvent.dwHoverTime = 1000;
                TrackMouseEvent(&MouseEvent);

                //  设置鼠标被跟踪
                m_fTrackMouse = TRUE;
            }

            //  按钮没有按下,则设置按钮状态为焦点
            if(m_nCtrlState != STATE_DOWN)
                m_nCtrlState = STATE_FOCUS;
        }
        else // 移动到区域外，均直接处理为鼠标弹起
        {
            m_nCtrlState = STATE_UP; 
        }
        InvalidataCtrl();
    }
}


void CDrawCheckbox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    //  鼠标离开，设置鼠标取消跟踪
    m_fTrackMouse = FALSE;

    //  按钮可用，设置按钮为弹起状态
    if(m_nCtrlState != STATE_DISABLE)
    {
        m_nCtrlState = STATE_UP;
        InvalidataCtrl();
    }
}
