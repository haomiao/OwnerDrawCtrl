///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014  
///     All rights reserved.  
///  
/// 	@file   DrawComboBox.h
/// 	@brief	�Ի���Ͽ�. 
/// 
///		��Ҫʵ�ֶ���Ͽ���Ի�  
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
	 * \brief �����ؼ�
     * \param rect ������Ϊ��ť������
     * \param hParentWnd ������Ϊ��ť�ĸ����ھ��
     * \param nID ������Ϊ��ť�Ŀؼ�ID
     * \param lpszNormalImage ������Ϊ��ť�ؼ�һ��״̬λͼ·��
     * \param lpszLightImage ������Ϊ��ť�ؼ�����Ч����λͼ·��
     * \param bklState ����,Ϊ����ͼƬ������ʾ��ʽ
     * \param hdcParent ������Ϊ�����ڱ����ļ���DC��
        ����Ѿ��и����ڵ�DC,�����������ÿؼ���������DCֱ��ʹ��
        ��������ȥ�����ڵĻ�ȡ����������DC
	 */
    BOOL Create(const RECT& rcCtrl, HWND hParentWnd,
        UINT nID, LPCTSTR lpszNormalImage, LPCTSTR lpszLightImage = _T(""),
        BKIMGLoadState bklState = BKLS_HORIZONTAL, HDC hdcParent = NULL);

    /**
	 * \brief �б��ر���Ϣ���ڱ����ڹ����д���CBN_SELCHANGE��
              ������CBN_CLOSEUP���ô˷���ʹ��ť��Ϊ�ɲ�����
	 */
    void Closeup();

    //  ���������
    int  AddString(LPCTSTR lpszItem);

    //  ����������
    int InsertString(int index, LPCTSTR lpszItem);

    //  ����������
    int FindString(int index, LPCTSTR lpszItem);

    //  ���õ�ǰ��
    void SetCurSel(int nIndex);

    //  ��ȡ��ǰ��
    int  GetCurSel();

    //  ��ȡָ��λ�õ��ı�
	int  GetItemText(int nIndex, LPTSTR lpItemText);

    //  ɾ���ı�
	BOOL DeleteItemString(LPCTSTR lpszItem);

    //  ��ȡ��������
    int GetCount();

    /**
	 * \brief ˢ�¿ؼ�
	 */
    bool InvalidataCtrl();

     /**
	 * \brief ���ð�ť����״̬
     * \param fEnable ������Ϊ���ð�ť�Ƿ����
	 */
	void EnableWindowEx(BOOL fEnable);

    //  ��ȡ�ؼ����
    HWND GetSafeHWnd();

    //  �޸��ı�����
    //  ע:�ⲿ����ʱ���ɲ��ô�����Դ����
    //  �ڲ��Լ�����,�˴�ʹ��Ӧ����
    BOOL SetFont(HFONT HFont);

    //  �޸��ı���ɫ
    void SetColor(COLORREF crText);

    //  �޸�combobox�߿���ɫ
    void SetBorderColor(COLORREF crBorder);

private:

    /**
	 * \brief �ؼ�����Ϣ�ص�����
	 */
    static LRESULT CALLBACK ComboBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //  ��Ҫ�����б���ѡ�񡢵�����
    void   OnCommand(WPARAM wParam, LPARAM lParam);

    /**
	 * \brief ���ư�ť
     * \param hDC ������Ϊ��ť�Ļ����õ�DC���
	 */
    void   Draw(HDC hDC);

    //  ��ʼ������
    void   InitBkDC(HDC hDC = NULL);

    //  ���ô�����Ϣ������
    void ReSetWindowProc();
    
    void   OnLButtonDown(WPARAM wParam, LPARAM lParam);

private:

    CDrawComboBox(const CDrawComboBox &OtherDrawComboBox);

    CDrawComboBox &operator=(const CDrawComboBox &OtherDrawComboBox);

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
    * \brief ��ť��Ĭ����Ϣ�ص�����.
    */
    WNDPROC      m_OldProc;

    //  ������ť�ؼ�
    CDrawButton  m_btnDropDown;

    //  ��ʾ����
    HFONT        m_hFont;

    //  �ؼ�����״̬
	BOOL		 m_fEnable;

    //  �ؼ��������
	HWND         m_hWnd;

   /**
    * \brief �ؼ�������ɫ.
    */
    COLORREF    m_crText;

   /**
    * \brief �ؼ��߿���ɫ.
    */
    COLORREF    m_crBorder;
};
