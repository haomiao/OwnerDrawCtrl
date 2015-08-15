#include "stdafx.h"
#include <windows.h>
#include <TCHAR.H>
#include "DrawComboBox.h"

namespace
{
    //  combobox控件边框的宽度
    #define BORDER_WIDTH            2

    //  combobox下拉按钮的ID
    #define IDC_DROPDOWN_BUTTON     1000

    //  combobox的item高度
    #define HEIGHT_LISTITEM			20

    //  数据项背景颜色
    #define COLOR_ITEM_BK           RGB(255, 255, 255)
}


CDrawComboBox::CDrawComboBox(void):
m_hWnd(NULL),
m_OldProc(NULL),
m_hOldBkgBitmap(NULL),
m_hBackgroundDC(NULL),
m_fEnable(TRUE),
m_hFont(NULL),
m_crText(RGB(72, 72, 72) ),
m_crBorder(RGB(0, 0, 0) )
{
    CSingletonAutoT2<CGdiplusGuard>::GetInstance();
}


CDrawComboBox::~CDrawComboBox(void)
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


BOOL CDrawComboBox::Create(const RECT& rcCtrl, HWND hParentWnd,
                           UINT nID, LPCTSTR lpszNormalImage, LPCTSTR lpszLightImage,  BKIMGLoadState bklState, HDC hdcParent)
{
    if(!m_hWnd)
    {
        m_hWnd = CreateWindow(_T("COMBOBOX"), _T(""),\
            WS_CHILD | WS_VISIBLE  |CBS_DROPDOWNLIST | WS_VSCROLL | CBS_HASSTRINGS | CBS_AUTOHSCROLL ,\
            rcCtrl.left, rcCtrl.top, rcCtrl.right - rcCtrl.left, \
            rcCtrl.bottom - rcCtrl.top, hParentWnd, (HMENU)nID, NULL, NULL);

        if(!m_hWnd)
            return FALSE;

        m_OldProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)ComboBoxProc);
        SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

        COMBOBOXINFO cbinfo = {sizeof(COMBOBOXINFO)};
        GetComboBoxInfo(m_hWnd, &cbinfo);

        //设置Item的大小和位置
        LRESULT res = SendMessage(m_hWnd, CB_SETITEMHEIGHT, -1, 20);

        //计算字符在Item中的高度
        HDC hEditDC = GetDC(cbinfo.hwndItem);
        SIZE szExtent = {};
        GetTextExtentExPoint(hEditDC, _T("123"), _tcslen(_T("123")), 0, NULL, NULL, &szExtent);
        ReleaseDC(cbinfo.hwndItem, hEditDC);

        RECT rcCombo;
        ::GetWindowRect(cbinfo.hwndCombo, &rcCombo);
      
        //  设置item的位置
        int nNewTop = ((rcCombo.bottom - rcCombo.top) - szExtent.cy) / 2;
        ::SetWindowPos(cbinfo.hwndItem, NULL, BORDER_WIDTH, nNewTop, \
            (rcCombo.right - rcCombo.left) - BORDER_WIDTH * 4 - (rcCombo.bottom - rcCombo.top), \
            szExtent.cy, SWP_NOZORDER);

        //  隐藏item窗口
        ShowWindow(cbinfo.hwndItem, SW_HIDE);

        //  调整弹出按钮的位置大小
        RECT rcButton;
        rcButton.left = (rcCombo.right - rcCombo.left) - (rcCombo.bottom - rcCombo.top) + 5;
        rcButton.right = (rcCombo.right - rcCombo.left);
        rcButton.top = 0;
        rcButton.bottom = rcButton.top + (rcCombo.bottom - rcCombo.top);

        m_btnDropDown.Create(_T(""), rcButton, m_hWnd, IDC_DROPDOWN_BUTTON, lpszNormalImage, lpszLightImage, bklState, hdcParent);

        m_hFont = CreateFont(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, \
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体") );

		//设置列表框的字体
		SendMessage(cbinfo.hwndList, WM_SETFONT, (WPARAM)m_hFont, (LPARAM)TRUE);
		SendMessage(cbinfo.hwndList, LB_SETITEMHEIGHT, (WPARAM)0, (LPARAM)HEIGHT_LISTITEM);

		//初始化背景
        if(hdcParent)
        {
            InitBkDC(hdcParent);
        }
        return TRUE;
    }
    return FALSE;
}


void CDrawComboBox::InitBkDC(HDC hDC)
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


int  CDrawComboBox::AddString(LPCTSTR lpszItem)
{
    return SendMessage(m_hWnd, CB_ADDSTRING, NULL, (LPARAM)lpszItem);
}


int CDrawComboBox::InsertString(int index, LPCTSTR lpszItem)
{
    return SendMessage(m_hWnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)lpszItem);
}


int CDrawComboBox::FindString(int index, LPCTSTR lpszItem)
{
    return SendMessage(m_hWnd, CB_FINDSTRING, (WPARAM)index, (LPARAM)lpszItem);
}


void CDrawComboBox::Closeup()
{
    SendMessage(m_hWnd, CB_SHOWDROPDOWN, FALSE, 0);
    m_btnDropDown.SetState(STATE_UP);
    InvalidataCtrl();
}


int CDrawComboBox::GetCurSel()
{
	return SendMessage(m_hWnd, CB_GETCURSEL, NULL, NULL);
}


int  CDrawComboBox::GetItemText(int nIndex, LPTSTR lpItemText)
{
	return ::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpItemText);
}


BOOL CDrawComboBox::DeleteItemString(LPCTSTR lpszItem)
{
	int nCurSel = GetCurSel();
	
	int nDelIndex = SendMessage(m_hWnd, CB_FINDSTRING, 0, (LPARAM)lpszItem);
	if(nDelIndex != CB_ERR)
	{
		SendMessage(m_hWnd, CB_DELETESTRING, nDelIndex, 0);
		if(nDelIndex == nCurSel)
		{
			SetCurSel(0);
		}
		else
		{
			InvalidataCtrl();
		}
		return TRUE;
	}
	return FALSE;
}


void CDrawComboBox::SetCurSel(int nIndex)
{
    SendMessage(m_hWnd, CB_SETCURSEL, nIndex, NULL);
	InvalidataCtrl();
}


int CDrawComboBox::GetCount()
{
    return SendMessage(m_hWnd, CB_GETCOUNT, 0, 0);
}


void CDrawComboBox::EnableWindowEx(BOOL fEnable)
{
	m_btnDropDown.EnableButton(fEnable);
	m_fEnable = fEnable;
}


void CDrawComboBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
    //  处理按钮按下或弹起操作
    int nMsg = 0;
    if(LOWORD(wParam) == IDC_DROPDOWN_BUTTON)
    {
        nMsg =  HIWORD(wParam);
        if(HIWORD(wParam) == BN_CLICKED)
        {
            BOOL res = SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0);
            if (res == TRUE)
            {
                Closeup();
            }
            else
            {
                SendMessage(m_hWnd, CB_SHOWDROPDOWN, TRUE, 0);
                m_btnDropDown.SetState(STATE_DOWN);
            }
            return;
        }
    }

    //  处理下拉框选择操作
    switch(HIWORD(wParam))
    {
    case CBN_SELCHANGE:
        {
            int nIndex = SendMessage(m_hWnd, CB_GETCURSEL, NULL, NULL);
            if(CB_ERR != nIndex)
            {
                SendMessage(m_hWnd, CB_SETCURSEL, nIndex, 0);
            }
            Closeup();
        }
        break;
    default:
        break;
    }
}


void CDrawComboBox::OnLButtonDown(WPARAM, LPARAM)
{
	if(m_fEnable)
	{
		SendMessage(m_hWnd, WM_COMMAND, MAKEWPARAM(IDC_DROPDOWN_BUTTON, BN_CLICKED), 0);
	}
}


void CDrawComboBox::Draw(HDC hDC)
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
    //HPEN  hPen = CreatePen(PS_SOLID, 13, m_crBorder);
    //HPEN  hOldPen = (HPEN)SelectObject(hMemDC, hPen);
    HFONT hOldFont = (HFONT)SelectObject(hMemDC, m_hFont);
    RECT rcCtrl = {0, 0, nWidth, nHeight};
    HBRUSH hBrush = CreateSolidBrush(COLOR_ITEM_BK);
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
    //SelectObject(hMemDC, hOldPen);
    //DeleteObject(hPen);
    DeleteObject(hBmpMem);
    DeleteDC(hMemDC);

    //  注释掉以下直接在DC上绘制的代码
//     if(m_hBackgroundDC)
//     {
//         BitBlt(hDC, 0, 0, nWidth, nHeight, m_hBackgroundDC, 0, 0, SRCCOPY);
//     }
//     else
//     {
//         HFONT hOldFont = (HFONT)SelectObject(hDC, m_hFont);
//         RECT rcCtrl = {0, 0, nWidth, nHeight};
//         HBRUSH hBrush = CreateSolidBrush(COLOR_ITEM_BK);
//         FillRect(hDC, &rcCtrl, hBrush);
// 
//         SetTextColor(hDC, COLOR_TEXT);
//         TCHAR tszText[MAX_PATH] = {};
//         GetWindowText(m_hWnd, tszText, MAX_PATH);
//         RECT rcText = {5, rcCtrl.top, rcCtrl.right, rcCtrl.bottom};
//         DrawText(hDC, tszText, _tcslen(tszText), &rcText, DT_VCENTER | DT_SINGLELINE);
// 
//         HBRUSH hFrameBrush = CreateSolidBrush(COLOR_CTRL_BORDER);
//         FrameRect(hDC, &rcCtrl, hFrameBrush);
//         DeleteObject(hFrameBrush);
//         DeleteObject(hBrush);
//         SelectObject(hDC, hOldFont);
//     }
}


void CDrawComboBox::ReSetWindowProc()
{
    if(m_hWnd)
    {
        SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_OldProc);
        SendMessage(m_btnDropDown.GetSafeHWnd(), WM_DESTROY, 0, 0);
        //::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
}


bool CDrawComboBox::InvalidataCtrl()
{
    RECT rcWnd = {};
    GetClientRect(m_hWnd, &rcWnd);
    InvalidateRect(m_hWnd, &rcWnd, TRUE);
    return TRUE;
}


LRESULT CALLBACK CDrawComboBox::ComboBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CDrawComboBox *_this = (CDrawComboBox *)GetWindowLong(hWnd, GWL_USERDATA);
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_ERASEBKGND:
		{
			_this->Draw((HDC)wParam);
			return TRUE;
		}
    case WM_COMMAND:
        {
            _this->OnCommand(wParam, lParam);
        }
        break;
	case WM_MOUSEWHEEL:
		{
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			_this->OnLButtonDown(wParam, lParam);
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
    return 0;//CallWindowProc(_this->m_OldProc, hWnd, message, wParam, lParam);
}


HWND CDrawComboBox::GetSafeHWnd()
{
    return m_hWnd;
}


BOOL CDrawComboBox::SetFont(HFONT HFont)
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


void CDrawComboBox::SetColor(COLORREF crText)
{
    m_crText = crText;
    InvalidataCtrl();
}


void CDrawComboBox::SetBorderColor(COLORREF crBorder)
{
    m_crBorder = crBorder;
    InvalidataCtrl();
}