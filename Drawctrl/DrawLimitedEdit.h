///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   DrawLimitedEdit.h
/// 	@brief	受限编辑框自绘. 
/// 
///		主要实现对编辑框的自绘
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/08/14  15:39:08
///  
///  
/////////////////////////////////////////////////////////////////////

#pragma once

#include "../Unity/GdiplusGuard.h"
#include "../Unity/Singleton.h"

class CLimitedEdit
{
public:

	CLimitedEdit(void);

	~CLimitedEdit(void);

public:

    /**
	 * \brief 创建控件
     * \param rect 参数，为控件的坐标
     * \param hParentWnd 参数，为控件的父窗口句柄
     * \param nID 参数，为控件的控件ID
     * \param hdcParent 参数，为父窗口背景的兼容DC，
        如果已经有父窗口的DC,可以先输入让控件创建兼容DC直接使用
        而不必再去父窗口的获取而创建兼容DC
     * \param fNumber 参数，是否为数字形态
     * \param lfMinVal 参数，最小值。
     * \param lfMaxVal 参数，最大值，如果lfMinVal = lfMaxVal，那就不设置最大最小值。
	 * \param fInteger 参数，当前是否只接受整数。
	 * \param nDecimal 参数，小数点位数。
	 */
    BOOL Create(const RECT& rect, HWND hParentWnd,\
        UINT nID, HDC hdcParent = NULL, BOOL fNumber = FALSE, double lfMinVal = 0.0, double lfMaxVal = 0.0, BOOL fInteger = FALSE, unsigned int nDecimal = 4);

    //  设置控件类型
	BOOL SetCtrlType(BOOL fNumber = TRUE, double lfMinVal = 0.0, double lfMaxVal = 0.0, BOOL fInteger = FALSE, unsigned int nDecimal = 4);

   /**
	 * \brief 刷新控件
	 */
    bool InvalidataCtrl();

    //  修改文本字体
    //  注:外部创建时，可不用处理资源销毁
    //  内部自己销毁,此处使用应谨慎
    BOOL SetFont(HFONT HFont);

    //  修改文本颜色
    void SetColor(COLORREF crText);

    //  修改combobox边框颜色
    void SetBorderColor(COLORREF crBorder);

    //  修改combobox背景颜色
    void SetBKColor(COLORREF crBK);

private:

    /**
	 * \brief 控件消息回调函数
	 */
    static LRESULT CALLBACK LimitedEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT CheckString(const LPCTSTR lpString);

	LRESULT OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);

	LRESULT OnSetText(HWND hWnd, WPARAM wParam, LPARAM lParam);

	LRESULT OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam);

	LRESULT OnPaste(HWND hWnd);

    //  重置窗口消息处理函数
    void ReSetWindowProc();

    //  初始化背景
    void   InitBkDC(HDC hDC = NULL);

    /**
	 * \brief 绘制按钮
     * \param hDC 参数，为按钮的绘制用的DC句柄
	 */
    void   Draw(HDC hDC);

private:

    CLimitedEdit(const CLimitedEdit &);

    CLimitedEdit &operator=(const CLimitedEdit &);

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
    * \brief 此控件的句柄.
    */
    HWND        m_hWnd;

    /**
    * \brief 控件的默认消息回调函数.
    */
    WNDPROC     m_OldProc;

    //  显示字体
    HFONT        m_hFont;

   /**
    * \brief 控件字体颜色.
    */
    COLORREF    m_crText;

   /**
    * \brief 控件边框颜色.
    */
    COLORREF    m_crBorder;

   /**
    * \brief 控件背景颜色.
    */
    COLORREF    m_crBK;

private:

    /**
    * \brief 控件字体句柄.
    */
   // HFONT       m_hFont;
	/**
    * \brief 控件是否只接收数字.
    */
	BOOL		m_fNumber;

	/**
    * \brief 当前是否是整形数据.
    */
	BOOL		m_fInteger;

	/**
    * \brief 控件只接收数字时的最小值.
    */
	double		m_lfMinVal;

	/**
    * \brief 控件只接收数字时的最大值.
    */
	double		m_lfMaxVal;

	/**
    * \brief 小数点位数.
    */
	unsigned int m_nDecimal;
};
