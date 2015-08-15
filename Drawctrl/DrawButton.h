///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   DrawButton.h
/// 	@brief	按钮控件. 
/// 
///		主要实现按钮控件的自绘  
///     限制：绘制文本可以多行,但目前只处理了两行或一行的情形，
///     若处理任意多行，需修改源码,绘制文本处
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/08/12  10:19:03
///  
///  
/////////////////////////////////////////////////////////////////////

#pragma once

#include "../Unity/GdiplusGuard.h"
#include "../Unity/Singleton.h"

#include "ControlBase.h"

class CDrawButton:public CControlBase
{
public:

    CDrawButton(void);

    ~CDrawButton(void);

public:

    /**
	 * \brief 亮灯效果按钮
     * \param fLight 参数，为当前是否要对按钮进行亮灯显示
	 */
    virtual BOOL LightingCtrl(BOOL fLight);

    /**
	 * \brief 刷新控件
	 */
    virtual BOOL InvalidataCtrl();

public:

    /**
	 * \brief 创建按钮
	 * \param lpszCaption 参数，为按钮的显示文本(至多50字符,多余的被截断)
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
    BOOL Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,
                UINT nID, LPCTSTR lpszNormalImage, LPCTSTR lpszLightImage = _T(""),
                BKIMGLoadState bklState = BKLS_HORIZONTAL, HDC hdcParent = NULL, 
                UINT szText = 12, COLORREF crText = RGB(0,0,0) );

    //  修改文本字体
    //  注:外部创建时，可不用处理资源销毁
    //  内部自己销毁,此处使用应谨慎
    BOOL SetFont(HFONT HFont);

    //  修改文本颜色
    void SetColor(COLORREF crText);

    //  修改窗体外形
    BOOL SetWindowRgn(HRGN hRgn);

    //  修改文本
    void SetCaption(LPCTSTR lpszCaption);

     /**
	 * \brief 设置按钮可用状态
     * \param fEnable 参数，为设置按钮是否可用
	 */
    void EnableButton(BOOL fEnable);

    //  设置按钮状态
    void SetState(UINT nState);

    //  获取控件句柄
    HWND GetSafeHWnd();

private:

    /**
	 * \brief 绘制按钮
     * \param hDC 参数，为按钮的绘制用的DC句柄
	 */
    void Draw(HDC hDC);

    //  绘制焦点区域
    void DrawFocusRect(HDC hDC,RECT &rcWnd);

    //  重置窗口消息处理函数
    void ReSetWindowProc();

    //  初始化背景DC
    void InitBkDC(HDC hDC = NULL);

public:

    //  鼠标相关操作
    void OnLbuttonDown(WPARAM wParam, LPARAM lParam);

    void OnLbuttonUp(WPARAM wParam, LPARAM lParam);

    void OnLbuttonDBClick(WPARAM wParam, LPARAM lParam);

    void OnMouseMove(WPARAM wParam, LPARAM lParam);

    void OnMouseLeave(WPARAM wParam, LPARAM lParam);
    
private:

    /**
	 * \brief 按钮消息回调函数
	 */
    static LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    CDrawButton(const CDrawButton &OtherDrawButton);

    CDrawButton &operator=(const CDrawButton &OtherDrawButton);

private:

    /**
    * \brief 按钮的默认消息回调函数.
    */
    WNDPROC     m_OldProc;

    /**
    * \brief 此按钮的句柄.
    */
    HWND        m_hWnd;

private:

    /**
    * \brief 主窗口原背景图句柄.
    */
    HBITMAP		m_hOldBkgBitmap;

    /**
    * \brief 父窗口背景图.
    */
    HDC         m_hBackgroundDC;

    /**
    * \brief 按钮的普通四状态位图.
    */
    Image     *m_pNormalStateImg;

    /**
    * \brief 按钮的闪灯四状态位图.
    */
    Image     *m_pLightStateImg; 

    /**
    * \brief 是否用亮灯效果.
    */
    BOOL        m_fLight;   

    /**
    * \brief 鼠标移动状态.
    */
    BOOL        m_fTrackMouse;

    /**
    * \brief 按钮的当前状态.
    */
    int         m_nButtonState;

    /**
    * \brief 控件的区域.
    */
    HRGN        m_hRgn;

    /**
    * \brief 控件字体句柄.
    */
    HFONT       m_hFont;

    /**
    * \brief 控件字体颜色.
    */
    COLORREF    m_crText;

    //  背景图片加载方式
    BKIMGLoadState  m_bkimState;

    /**
    * \brief 按钮的当前状态,用于主动设置按钮的状态持续不变
             应用场合是combobox中的按钮点击时的状态
    */
    BOOL         m_fButtonDown;
};
