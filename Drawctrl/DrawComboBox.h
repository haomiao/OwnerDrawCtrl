///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014  
///     All rights reserved.  
///  
/// 	@file   DrawComboBox.h
/// 	@brief	自绘组合框. 
/// 
///		主要实现对组合框的自绘  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/08/14  14:29:31
///  
///  
/////////////////////////////////////////////////////////////////////


#pragma once

#include "../Unity/GdiplusGuard.h"
#include "../Unity/Singleton.h"
#include "DrawButton.h"

class CDrawComboBox
{
public:

    CDrawComboBox(void);

    ~CDrawComboBox(void);

public:

    /**
	 * \brief 创建控件
     * \param rect 参数，为按钮的坐标
     * \param hParentWnd 参数，为按钮的父窗口句柄
     * \param nID 参数，为按钮的控件ID
     * \param lpszNormalImage 参数，为按钮控件一般状态位图路径
     * \param lpszLightImage 参数，为按钮控件亮灯效果的位图路径
     * \param bklState 参数,为背景图片加载显示方式
     * \param hdcParent 参数，为父窗口背景的兼容DC，
        如果已经有父窗口的DC,可以先输入让控件创建兼容DC直接使用
        而不必再去父窗口的获取而创建兼容DC
	 */
    BOOL Create(const RECT& rcCtrl, HWND hParentWnd,
        UINT nID, LPCTSTR lpszNormalImage, LPCTSTR lpszLightImage = _T(""),
        BKIMGLoadState bklState = BKLS_HORIZONTAL, HDC hdcParent = NULL);

    /**
	 * \brief 列表框关闭消息，在本窗口过程中处理CBN_SELCHANGE和
              父窗口CBN_CLOSEUP调用此方法使按钮变为可操作。
	 */
    void Closeup();

    //  添加数据项
    int  AddString(LPCTSTR lpszItem);

    //  插入数据项
    int InsertString(int index, LPCTSTR lpszItem);

    //  查找数据项
    int FindString(int index, LPCTSTR lpszItem);

    //  设置当前项
    void SetCurSel(int nIndex);

    //  获取当前项
    int  GetCurSel();

    //  获取指定位置的文本
	int  GetItemText(int nIndex, LPTSTR lpItemText);

    //  删除文本
	BOOL DeleteItemString(LPCTSTR lpszItem);

    //  获取数据项数
    int GetCount();

    /**
	 * \brief 刷新控件
	 */
    bool InvalidataCtrl();

     /**
	 * \brief 设置按钮可用状态
     * \param fEnable 参数，为设置按钮是否可用
	 */
	void EnableWindowEx(BOOL fEnable);

    //  获取控件句柄
    HWND GetSafeHWnd();

    //  修改文本字体
    //  注:外部创建时，可不用处理资源销毁
    //  内部自己销毁,此处使用应谨慎
    BOOL SetFont(HFONT HFont);

    //  修改文本颜色
    void SetColor(COLORREF crText);

    //  修改combobox边框颜色
    void SetBorderColor(COLORREF crBorder);

private:

    /**
	 * \brief 控件的消息回调函数
	 */
    static LRESULT CALLBACK ComboBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //  主要处理列表项选择、弹出等
    void   OnCommand(WPARAM wParam, LPARAM lParam);

    /**
	 * \brief 绘制按钮
     * \param hDC 参数，为按钮的绘制用的DC句柄
	 */
    void   Draw(HDC hDC);

    //  初始化背景
    void   InitBkDC(HDC hDC = NULL);

    //  重置窗口消息处理函数
    void ReSetWindowProc();
    
    void   OnLButtonDown(WPARAM wParam, LPARAM lParam);

private:

    CDrawComboBox(const CDrawComboBox &OtherDrawComboBox);

    CDrawComboBox &operator=(const CDrawComboBox &OtherDrawComboBox);

private:

    /**
    * \brief 主窗口原背景图句柄.
    */
    HBITMAP     m_hOldBkgBitmap;

    /**
    * \brief 父窗口背景图.
    */
    HDC         m_hBackgroundDC;

    /**
    * \brief 按钮的默认消息回调函数.
    */
    WNDPROC      m_OldProc;

    //  下拉按钮控件
    CDrawButton  m_btnDropDown;

    //  显示字体
    HFONT        m_hFont;

    //  控件可用状态
	BOOL		 m_fEnable;

    //  控件句柄对象
	HWND         m_hWnd;

   /**
    * \brief 控件字体颜色.
    */
    COLORREF    m_crText;

   /**
    * \brief 控件边框颜色.
    */
    COLORREF    m_crBorder;
};
