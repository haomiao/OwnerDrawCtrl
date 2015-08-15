///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   DrawLimitedEdit.h
/// 	@brief	���ޱ༭���Ի�. 
/// 
///		��Ҫʵ�ֶԱ༭����Ի�
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
	 * \brief �����ؼ�
     * \param rect ������Ϊ�ؼ�������
     * \param hParentWnd ������Ϊ�ؼ��ĸ����ھ��
     * \param nID ������Ϊ�ؼ��Ŀؼ�ID
     * \param hdcParent ������Ϊ�����ڱ����ļ���DC��
        ����Ѿ��и����ڵ�DC,�����������ÿؼ���������DCֱ��ʹ��
        ��������ȥ�����ڵĻ�ȡ����������DC
     * \param fNumber �������Ƿ�Ϊ������̬
     * \param lfMinVal ��������Сֵ��
     * \param lfMaxVal ���������ֵ�����lfMinVal = lfMaxVal���ǾͲ����������Сֵ��
	 * \param fInteger ��������ǰ�Ƿ�ֻ����������
	 * \param nDecimal ������С����λ����
	 */
    BOOL Create(const RECT& rect, HWND hParentWnd,\
        UINT nID, HDC hdcParent = NULL, BOOL fNumber = FALSE, double lfMinVal = 0.0, double lfMaxVal = 0.0, BOOL fInteger = FALSE, unsigned int nDecimal = 4);

    //  ���ÿؼ�����
	BOOL SetCtrlType(BOOL fNumber = TRUE, double lfMinVal = 0.0, double lfMaxVal = 0.0, BOOL fInteger = FALSE, unsigned int nDecimal = 4);

   /**
	 * \brief ˢ�¿ؼ�
	 */
    bool InvalidataCtrl();

    //  �޸��ı�����
    //  ע:�ⲿ����ʱ���ɲ��ô�����Դ����
    //  �ڲ��Լ�����,�˴�ʹ��Ӧ����
    BOOL SetFont(HFONT HFont);

    //  �޸��ı���ɫ
    void SetColor(COLORREF crText);

    //  �޸�combobox�߿���ɫ
    void SetBorderColor(COLORREF crBorder);

    //  �޸�combobox������ɫ
    void SetBKColor(COLORREF crBK);

private:

    /**
	 * \brief �ؼ���Ϣ�ص�����
	 */
    static LRESULT CALLBACK LimitedEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT CheckString(const LPCTSTR lpString);

	LRESULT OnKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);

	LRESULT OnSetText(HWND hWnd, WPARAM wParam, LPARAM lParam);

	LRESULT OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam);

	LRESULT OnPaste(HWND hWnd);

    //  ���ô�����Ϣ������
    void ReSetWindowProc();

    //  ��ʼ������
    void   InitBkDC(HDC hDC = NULL);

    /**
	 * \brief ���ư�ť
     * \param hDC ������Ϊ��ť�Ļ����õ�DC���
	 */
    void   Draw(HDC hDC);

private:

    CLimitedEdit(const CLimitedEdit &);

    CLimitedEdit &operator=(const CLimitedEdit &);

private:

    /**
    * \brief ������ԭ����ͼ���.
    */
    HBITMAP     m_hOldBkgBitmap;

    /**
    * \brief �����ڱ���ͼ.
    */
    HDC         m_hBackgroundDC;

    /**
    * \brief �˿ؼ��ľ��.
    */
    HWND        m_hWnd;

    /**
    * \brief �ؼ���Ĭ����Ϣ�ص�����.
    */
    WNDPROC     m_OldProc;

    //  ��ʾ����
    HFONT        m_hFont;

   /**
    * \brief �ؼ�������ɫ.
    */
    COLORREF    m_crText;

   /**
    * \brief �ؼ��߿���ɫ.
    */
    COLORREF    m_crBorder;

   /**
    * \brief �ؼ�������ɫ.
    */
    COLORREF    m_crBK;

private:

    /**
    * \brief �ؼ�������.
    */
   // HFONT       m_hFont;
	/**
    * \brief �ؼ��Ƿ�ֻ��������.
    */
	BOOL		m_fNumber;

	/**
    * \brief ��ǰ�Ƿ�����������.
    */
	BOOL		m_fInteger;

	/**
    * \brief �ؼ�ֻ��������ʱ����Сֵ.
    */
	double		m_lfMinVal;

	/**
    * \brief �ؼ�ֻ��������ʱ�����ֵ.
    */
	double		m_lfMaxVal;

	/**
    * \brief С����λ��.
    */
	unsigned int m_nDecimal;
};
