#include "StdAfx.h"

#include <locale.h>
#include <math.h>
#include "ControlBase.h"
#include "DrawLimitedEdit.h"

namespace
{
    
}

CLimitedEdit::CLimitedEdit(void):
m_lfMinVal(0),
m_lfMaxVal(0),
m_nDecimal(0),
m_fNumber(FALSE),
m_fInteger(FALSE),
m_hWnd(NULL),
m_hOldBkgBitmap(NULL),
m_hBackgroundDC(NULL),
m_hFont(NULL),
m_crText(RGB(72, 72, 72) ),
m_crBorder(RGB(0, 0, 0) ),
m_crBK(RGB(255, 255, 255) )
{
    CSingletonAutoT2<CGdiplusGuard>::GetInstance();
}


CLimitedEdit::~CLimitedEdit(void)
{
    if (m_hBackgroundDC)
    {
        SelectObject(m_hBackgroundDC, m_hOldBkgBitmap);
        DeleteDC(m_hBackgroundDC);
        m_hBackgroundDC = NULL;
        m_hOldBkgBitmap = NULL;
    }

    SAFE_DELETE(m_hFont);
}


BOOL CLimitedEdit::Create(const RECT& rect, HWND hParentWnd,\
						 UINT nID, HDC hdcParent, BOOL fNumber, double lfMinVal, double lfMaxVal, BOOL fInteger, unsigned int nDecimal)
{
    if(m_hWnd)
    {
        return FALSE;
    }

	if(lfMinVal > lfMaxVal)
    {
        return FALSE;
    }

	m_fInteger = fInteger;
	m_nDecimal = nDecimal;

    int nWidth = rect.right - rect.left;
    int nHeight = rect.bottom - rect.top;
    m_hWnd = CreateWindow(_T("EDIT"), _T(""),\
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_TABSTOP | WS_BORDER 
        /*| ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL*/,\
        rect.left, rect.top, nWidth, nHeight, \
        hParentWnd,(HMENU)nID,NULL,NULL);

    if( NULL == m_hWnd)
    {
        return FALSE;
    }

	m_fNumber = fNumber;
	m_lfMinVal = lfMinVal;
	m_lfMaxVal = lfMaxVal;

    m_OldProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)LimitedEditProc);
    SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

    m_hFont = CreateFont(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, \
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体") );

    if (hdcParent)
    {
        InitBkDC(hdcParent);
    }

    return TRUE;
}


void CLimitedEdit::InitBkDC(HDC hDC)
{
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


BOOL CLimitedEdit::SetCtrlType(BOOL fNumber, double lfMinVal, double lfMaxVal, BOOL fInteger, unsigned int nDecimal)
{
	if(!m_hWnd)
        return FALSE;

	if(lfMinVal > lfMaxVal)
		return FALSE;

	m_fNumber	= fNumber;
	m_lfMinVal	= lfMinVal;
	m_lfMaxVal	= lfMaxVal;
	m_fInteger	= fInteger;
	m_nDecimal	= nDecimal;
	return TRUE;
}


void CLimitedEdit::ReSetWindowProc()
{
    if(m_hWnd)
    {
        SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_OldProc);
        //::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
}


LRESULT CLimitedEdit::CheckString(const LPCTSTR lpString)
{
	if(m_fNumber)
	{
		int nLen = _tcslen(lpString);
		if(!nLen)
			return 0;

		//如果首字母不为0-9之内的就算无效
		if(lpString[0] < '0' || lpString[0] > '9')
			return 0;

		//当字符串长度大于或者等于2时，如果前两个字母都是0，那就无效
		if(nLen >= 2)
		{
			if(lpString[0] == '0' && lpString[1] == '0')
				return 0;
		}

		//统计小数点的个数，如果超过1个的话，那就无效
		int nDotCount = 0;
		for(int i = 1; i < nLen; i++)
		{
			if(lpString[i] < '0' || lpString[i] > '9')
			{
				if(m_fInteger)
				{
					return 0;
				}
				else if(lpString[i] != '.')
				{
					return 0;
				}
			}
			
			if(lpString[i] == '.')
			{
				nDotCount++;
				if(m_fInteger)
				{
					return 0;
				}
			}
		}
		if(nDotCount >1)
			return 0;

		//对小数位的个数进行判断
		LPCTSTR lpDotPos = _tcsrchr(lpString, '.');
		if(lpDotPos)
		{
			if(m_nDecimal == 0 || _tcslen(lpDotPos) > m_nDecimal + 1)
				return 0;
		}

		//这里用于判断当前控件的数字是否有取值范围，如果有的话才对字符串的数据大小进行判断
		if(fabs(m_lfMaxVal - m_lfMinVal) > 0.0000001)
		{
			if(_tstof(lpString) < m_lfMinVal || _tstof(lpString) > m_lfMaxVal)
				return 0;
		}

		//暂时用处不大
		if(lpString[nLen - 1] == '.')
			return 2;

	}
	return 1;
}


LRESULT CLimitedEdit::OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(m_fNumber)
	{
		TCHAR tszString[MAX_PATH] = {};
		GetWindowText(hWnd, tszString, MAX_PATH);
		if(!CheckString(tszString))
		{
			SetWindowText(hWnd, _T(""));
		}
		else
		{
			int nLen = _tcslen(tszString);
			if(nLen && '.' == tszString[nLen - 1])
			{
				tszString[nLen - 1] = 0;
				SetWindowText(hWnd, tszString);
			}
		}

        InvalidataCtrl();
	}
	return 1;
}


LRESULT CLimitedEdit::OnSetText(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(m_fNumber)
	{
		LRESULT hRet = CheckString((LPCTSTR)lParam);
		switch(hRet)
		{
		case 0:
			{
				SetWindowText(hWnd, _T(""));
			}
			break;
		case 1:
			break;
		case 2:
			{
				((LPTSTR)lParam)[_tcslen((LPTSTR)lParam) - 1] = 0;
				SetWindowText(hWnd, (LPCTSTR)lParam);
			};
			break;
		default:
			break;
		}
	}
	return 1;
}


LRESULT CLimitedEdit::OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(m_fNumber)
	{
		if(wParam == VK_BACK)
		{
			return 1;
		}

		//(wParam == 3 && HIBYTE(GetKeyState(VK_LCONTROL))这个用于判断当前是否是复制操作
		if(wParam == 3 && HIBYTE(GetKeyState(VK_LCONTROL)))
		{
			return 1;
		}
		//(wParam == 22 && HIBYTE(GetKeyState(VK_LCONTROL))这个用于判断当前是否是粘贴操作
		if(wParam == 22 && HIBYTE(GetKeyState(VK_LCONTROL)))
		{
			return 1;
		}

		TCHAR tszString[MAX_PATH] = {};
		GetWindowText(hWnd, tszString, MAX_PATH);
		int nFirst = 0;
		int nLast = 0;
		SendMessage(hWnd, EM_GETSEL, (WPARAM)&nFirst, (LPARAM)&nLast);
		int nLastLen = _tcslen(&tszString[nLast]);
		if(nFirst == nLast)
		{
			for(int i = 0; i < nLastLen; i ++)
			{
				tszString[nLast + nLastLen - i] = tszString[nLast + nLastLen - i - 1];
			}
			tszString[nFirst] = wParam;
			tszString[nFirst + nLastLen + 1] = 0;
		}
		else
		{
			for(int i = 0; i < nLastLen; i ++)
			{
				tszString[nFirst + i + 1] = tszString[nLast + i];
			}
			tszString[nFirst] = wParam;
			tszString[nFirst + nLastLen + 1] = 0;
		}

		//_stprintf(nLast, _T("%s%c"), tszString, wParam);
        LRESULT res = CheckString(tszString);

        InvalidataCtrl();
		return res;
	}
	return 1;
}


LRESULT CLimitedEdit::OnPaste(HWND hWnd)
{
	if(m_fNumber)
	{
		wchar_t wszString[MAX_PATH] = {};
		GetWindowTextW(hWnd, wszString, MAX_PATH);

		if(IsClipboardFormatAvailable (CF_TEXT))
		{
			if(OpenClipboard(m_hWnd))
			{
				GLOBALHANDLE hGlobal = GetClipboardData(CF_TEXT);
				char *pTextDatas = (char *)GlobalLock(hGlobal);
				setlocale(LC_ALL, "chs");
                size_t returnValue = 0;
				/*int nDatasLen = mbstowcs_s(&returnValue, NULL, MAX_PATH, pTextDatas, strlen(pTextDatas));*/
#define _CRT_SECURE_NO_WARNINGS
                int nDatasLen = mbstowcs(NULL, pTextDatas, strlen(pTextDatas));

				int nFirst = 0;
				int nLast = 0;
				SendMessage(hWnd, EM_GETSEL, (WPARAM)&nFirst, (LPARAM)&nLast);
				wcscpy_s(&wszString[nFirst], MAX_PATH - nFirst,&wszString[nLast]);
				wchar_t wchTail = wszString[nFirst];
				memmove(&wszString[nFirst + nDatasLen], &wszString[nFirst], (wcslen(wszString) - nFirst) * sizeof(wchar_t));
				//memcpy(&m_pText[m_nCurrentInsertPos], );
				
				mbstowcs_s(&returnValue, &wszString[nFirst], MAX_PATH - nFirst, pTextDatas, strlen(pTextDatas));
				wszString[nFirst + nDatasLen] = wchTail;
				GlobalUnlock(hGlobal);
				CloseClipboard();
			}
		}
        LRESULT res = CheckString((LPCTSTR)wszString);
        InvalidataCtrl();
		return res;
	}
	return 1;
}


LRESULT CALLBACK CLimitedEdit::LimitedEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CLimitedEdit *_this = (CLimitedEdit *)GetWindowLong(hWnd, GWL_USERDATA);

	if(_this)
	{
		switch (message)
		{
        case WM_PAINT:
            {
                PAINTSTRUCT paintStruct;
                BeginPaint(hWnd, &paintStruct);
                EndPaint(hWnd, &paintStruct);
            }
            break;
        case WM_ERASEBKGND:
            {
                _this->Draw((HDC)wParam);
                return TRUE;
            }
		case WM_KILLFOCUS:
			{
				_this->OnKillFocus(hWnd, wParam, lParam);
			}
			break;
		case WM_SETTEXT:
			{
				//_this->OnSetText(hWnd, wParam, lParam);
			}
			break;
		case WM_CHAR:
			{
				if(!_this->OnChar(hWnd, wParam, lParam))
					return 0;
			}
			break;
		case WM_PASTE:
			{
				if(!_this->OnPaste(hWnd))
					return 0;
			}
			break;
		case WM_RBUTTONDOWN:
			{
				//return 0;
			}
			break;
        case WM_DESTROY:
            {
                _this->ReSetWindowProc();
                PostQuitMessage(0);
            }
            break;
		default:
			break;
		}
		return CallWindowProc(_this->m_OldProc, hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


void CLimitedEdit::Draw(HDC hDC)
{
    //  初始化背景DC，m_hBackgroundDC
    //  内部只会初始化一次
    InitBkDC();

    //  以下为从hDC创建兼容DC hMemDC和控件大小的画布
    //  并将早期的兼容DC m_hBackgroundDC 拷贝至新的hMemDC
    RECT rcWnd = {};
    ::GetWindowRect(m_hWnd, &rcWnd);
    int nWidth = rcWnd.right - rcWnd.left;
    int nHeight = rcWnd.bottom - rcWnd.top;

    HDC hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBmpMem = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    HBITMAP hOldBmpMem = (HBITMAP)SelectObject(hMemDC, hBmpMem);

    //  绘制父窗口背景图
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, m_hBackgroundDC, 0, 0, SRCCOPY);

    SetBkMode(hMemDC, TRANSPARENT);

    // 绘制兼容DC的背景和边框、设置字体等
    HFONT hOldFont = (HFONT)SelectObject(hMemDC, m_hFont);
    RECT rcCtrl = {0, 0, nWidth, nHeight};
    HBRUSH hBrush = CreateSolidBrush(m_crBK);
    FillRect(hMemDC, &rcCtrl, hBrush);

    SetTextColor(hMemDC, m_crText);
    TCHAR tszText[MAX_PATH] = {};
    GetWindowText(m_hWnd, tszText, MAX_PATH);
    RECT rcText = {5, rcCtrl.top, rcCtrl.right, rcCtrl.bottom};
    DrawText(hMemDC, tszText, _tcslen(tszText), &rcText, DT_VCENTER | DT_SINGLELINE);

    HBRUSH hFrameBrush = CreateSolidBrush(m_crBorder);
    FrameRect(hMemDC, &rcCtrl, hFrameBrush);
    DeleteObject(hFrameBrush);
    DeleteObject(hBrush);

    SelectObject(hMemDC, hOldFont);

    //  将绘制好的兼容DC hMemDC拷贝回控件hDC，完成整个绘制过程
    BitBlt(hDC, 0, 0, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);

    SetBkMode(hMemDC, OPAQUE);
    SelectObject(hMemDC, hOldBmpMem);
    DeleteObject(hBmpMem);
    DeleteDC(hMemDC);
}


bool CLimitedEdit::InvalidataCtrl()
{
    RECT rcWnd = {};
    GetClientRect(m_hWnd, &rcWnd);
    InvalidateRect(m_hWnd, &rcWnd, TRUE);
    return TRUE;
}


BOOL CLimitedEdit::SetFont(HFONT HFont)
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


void CLimitedEdit::SetColor(COLORREF crText)
{
    m_crText = crText;
    InvalidataCtrl();
}


void CLimitedEdit::SetBorderColor(COLORREF crBorder)
{
    m_crBorder = crBorder;
    InvalidataCtrl();
}


void CLimitedEdit::SetBKColor(COLORREF crBK)
{
    m_crBK = crBK;
    InvalidataCtrl();
}