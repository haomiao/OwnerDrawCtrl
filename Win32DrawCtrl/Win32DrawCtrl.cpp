// Win32DrawCtrl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32DrawCtrl.h"

#include <string>

#include "../Drawctrl/DrawButton.h"
#include "../Drawctrl/DrawCheckbox.h"
#include "../Drawctrl/DrawComboBox.h"
#include "../Drawctrl/DrawLimitedEdit.h"

#define MAX_LOADSTRING 100

#define IDC_BUTTON1 6060
#define IDC_BUTTON2 6061

#define IDC_CHECK1   6070
#define IDC_CHECK2   6071

#define IDC_COMBOBOX1 6080
#define IDC_COMBOBOX2 6081

#define IDC_EDIT1     6090
#define IDC_EDIT2     6091

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int, HWND & hwnd);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32DRAWCTRL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:

    HWND parentHwnd = NULL;
	if (!InitInstance (hInstance, nCmdShow, parentHwnd))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32DRAWCTRL));


    //  加载初始化gdiplus库
//     CGdiplusGuard gdiPlus;
//     if (gdiPlus.isOk() == FALSE)
//     {
//         return 0;
//     }

    CSingletonAutoT2<CGdiplusGuard>::GetInstance();
    
    RECT rect;
    rect.left = 10;
    rect.top = 10;
    rect.right = 72;
    rect.bottom = 46;

    TCHAR filePath[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, filePath, MAX_PATH);
    std::wstring fileNamePath = filePath;
    std::wstring fileName;
    size_t pos = fileNamePath.rfind('\\');
    if (pos != std::wstring::npos)
    {
        fileNamePath = fileNamePath.substr(0, pos + 1);
        fileName = fileNamePath + _T("button.png");
        //fileName += _T("button2.png");
    }

    CDrawButton drawBtn1, drawBtn2;
    drawBtn1.Create(_T("按钮1"), rect, parentHwnd, IDC_BUTTON1, fileName.c_str(), _T(""), BKLS_HORIZONTAL,NULL,
        12, RGB(255,255,0) );

//     drawBtn1.SetColor(RGB(255,0,0) );
//     drawBtn1.SetCaption(_T("按钮-1") );
//     drawBtn1.EnableButton(false);

    rect.left = 10;
    rect.top = 50;
    rect.right = 72;
    rect.bottom = 86;

    drawBtn2.Create(_T("按钮2"), rect, parentHwnd, IDC_BUTTON2, fileName.c_str(), _T(""), BKLS_HORIZONTAL,NULL,
        12, RGB(255,255,0) );

    rect.left = 100;
    rect.top = 10;
    rect.right = 200;
    rect.bottom = 46;

    fileName = fileNamePath + _T("checkbox.png");


    CDrawCheckbox drawCheck1,drawCheck2;
    drawCheck1.Create(_T("复选框1"), rect, parentHwnd, IDC_CHECK1, fileName.c_str(), _T(""), BKLS_HORIZONTAL, RGB(128,0,255), RGB(255,255,255), 12);

    rect.left = 100;
    rect.top = 50;
    rect.right = 200;
    rect.bottom = 86;

    drawCheck2.Create(_T("复选框2"), rect, parentHwnd, IDC_CHECK2, fileName.c_str(), _T(""), BKLS_HORIZONTAL, RGB(128,0,255), RGB(255,255,255), 12);

    drawCheck2.SetCaption(_T("复选框-2") );
    drawCheck2.SetColor(RGB(255, 0, 0) );
    drawCheck2.SetCheck(TRUE);
    //drawCheck2.EnableButton(false);

    rect.left = 210;
    rect.top = 12;
    rect.right = 300;
    rect.bottom = 150;

    fileName = fileNamePath + _T("combobox.png");

    CDrawComboBox drawComboBox1;
    //RECT rcBaudRate = {200, 310, 364, 450};

    drawComboBox1.Create(rect, parentHwnd, IDC_COMBOBOX1, fileName.c_str(), _T(""), BKLS_HORIZONTAL);
    drawComboBox1.AddString(_T("data1") );
    drawComboBox1.AddString(_T("data2") );
    drawComboBox1.AddString(_T("data3") );
    drawComboBox1.AddString(_T("data4") );
    drawComboBox1.AddString(_T("data5") );
    drawComboBox1.AddString(_T("data6") );
    drawComboBox1.AddString(_T("data7") );
    drawComboBox1.AddString(_T("data8") );
    drawComboBox1.AddString(_T("data9") );
    drawComboBox1.AddString(_T("data10") );
    drawComboBox1.SetCurSel(4);

    rect.left = 320;
    rect.top = 15;
    rect.right = 420;
    rect.bottom = 40;

    CLimitedEdit drawLimtedEdit;
    drawLimtedEdit.Create(rect, parentHwnd, IDC_EDIT1);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32DRAWCTRL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32DRAWCTRL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND & hwnd)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
        case IDC_BUTTON1:
            {
                MessageBox(hWnd, _T("点击了自绘按钮1"), _T("消息测试"), MB_OK);
            }
            break;
        case IDC_BUTTON2:
            {
                MessageBox(hWnd, _T("点击了自绘按钮2"), _T("消息测试"), MB_OK);
            }
            break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_CHECKBOXCLICKED:
        {
            TCHAR str[128] = {0};
            _stprintf_s(str, _T("控件ID：%d"), wParam);
            if (lParam == 1)
            {
                _tcscat_s(str, _T("勾选了自绘复选框") );
                MessageBox(hWnd, str, _T("消息测试"), MB_OK);
            }
            else
            {
                _tcscat_s(str, _T("取消勾选自绘复选框") );
                MessageBox(hWnd, str, _T("消息测试"), MB_OK);
            }
        }
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
