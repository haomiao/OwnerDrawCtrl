///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   DrawCheckbox.h
/// 	@brief	复选框控件. 
/// 
///		主要实现复选框控件的自绘  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/08/12  16:50:59
///  
///  
/////////////////////////////////////////////////////////////////////

#pragma once

#include "../Unity/GdiplusGuard.h"
#include "../Unity/Singleton.h"

#include "DrawDC.h"
#include "ControlBase.h"

/**
* \brief 复选框控件发给父窗口的消息.
*/
#define WM_CHECKBOXCLICKED   WM_USER + 2100  

class CDrawCheckbox:public CControlBase
{
public:

    CDrawCheckbox(void);

    ~CDrawCheckbox(void);

public:
    
     /**
	 * \brief 刷新控件
	 */
    virtual BOOL InvalidataCtrl();

    /**
	 * \brief 亮灯效果按钮
     * \param fLight 参数，为当前是否要对按钮进行亮灯显示
	 */
    virtual BOOL LightingCtrl(BOOL fLight);

public:

    /**
	 * \brief 创建控件
	 * \param lpszCaption 参数，为控件的显示文本(至多50字符,多余的被截断)
     * \param lpszImage 参数，为控件的位图路径
     * \param rect 参数，为控件的坐标
     * \param hParentWnd 参数，为控件的父窗口句柄
     * \param nID 参数，为控件的控件ID
     * \param lpszImage 参数，为控状态位图路径
     * \param lpszLightImage 参数，为按钮控件亮灯效果的位图路径
     * \param bklState 参数,为背景图片加载显示方式
     * \param hdcParent 参数，为父窗口背景的兼容DC，
        如果已经有父窗口的DC,可以先输入让控件创建兼容DC直接使用
        而不必再去父窗口的获取而创建兼容DC
     * \param szText    字体大小
	 */
    BOOL Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,\
        UINT nID, LPCTSTR lpszImage, LPCTSTR lpszLightImage = _T(""), BKIMGLoadState bklState = BKLS_HORIZONTAL, COLORREF colorText = RGB(255, 2552, 255), UINT szText = 12, HDC hdcParent = NULL);

	BOOL Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,\
        UINT nID, LPCTSTR lpszImage, LPCTSTR lpszLightImage = _T(""), BKIMGLoadState bklState = BKLS_HORIZONTAL, COLORREF colorText = RGB(255, 2552, 255), COLORREF colorBackgroud = RGB(255, 2552, 255), UINT szText = 12);
 
    //  设置是否被选中
    void SetCheck(BOOL bCheck);

    //  获取选中与否状态
    BOOL GetCheck();

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

    //  获取控件句柄
    HWND GetSafeHWnd();

private:

    /**
	 * \brief 控件消息回调函数
	 */
    static LRESULT CALLBACK CheckboxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

     //  初始化背景DC
	void InitBkDC(HDC hDC = NULL);

	/**
	 * \brief 绘制控件
     * \param hDC 参数，为控件的绘制用DC句柄
	 */
    void Draw(HDC hDC);

    //  销毁窗口对象
    void DestroyWindow();

private:

    //  鼠标操作相关
    void OnLbuttonDown(WPARAM wParam, LPARAM lParam);

    //  貌似没有接收到WM_LBUTTONUP消息?
    void OnLbuttonUp(WPARAM wParam, LPARAM lParam);

    void OnLbuttonDBClick(WPARAM wParam, LPARAM lParam);

    void OnMouseMove(WPARAM wParam, LPARAM lParam);

    void OnMouseLeave(WPARAM wParam, LPARAM lParam);

private:

    CDrawCheckbox(const CDrawCheckbox &);

    CDrawCheckbox &operator=(const CDrawCheckbox &);

private:

    /**
    * \brief 此控件的句柄.
    */
    HWND        m_hWnd;   

private:

    //用于背景的内存DC
	CDrawDC	 m_DrawBackgroundDC;

    /**
    * \brief 按钮的普通3 * 2状态位图.
    */
    Image     *m_pNormalStateImg;

    /**
    * \brief 按钮的闪灯3 * 2状态位图.
    */
    Image     *m_pLightStateImg; 

    /**
    * \brief 是否用亮灯效果.
    */
    BOOL        m_fLight;

    /**
    * \brief 控件的当前状态.
    */
    int         m_nCtrlState;

    /**
    * \brief 鼠标移动状态.
    */
    BOOL        m_fTrackMouse;

    /**
    * \brief 控件字体句柄.
    */
    HFONT       m_hFont;

	/**
    * \brief 控件的region.
    */
    HRGN        m_hCtrlRgn;

    /**
    * \brief 控件字体颜色.
    */
	COLORREF	m_colorText;

    //  是否已被选中
	BOOL		m_bCheck;

    //  背景图片加载方式
    BKIMGLoadState  m_bkimState;
};
