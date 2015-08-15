///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   DrawButton.h
/// 	@brief	��ť�ؼ�. 
/// 
///		��Ҫʵ�ְ�ť�ؼ����Ի�  
///     ���ƣ������ı����Զ���,��Ŀǰֻ���������л�һ�е����Σ�
///     ������������У����޸�Դ��,�����ı���
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
	 * \brief ����Ч����ť
     * \param fLight ������Ϊ��ǰ�Ƿ�Ҫ�԰�ť����������ʾ
	 */
    virtual BOOL LightingCtrl(BOOL fLight);

    /**
	 * \brief ˢ�¿ؼ�
	 */
    virtual BOOL InvalidataCtrl();

public:

    /**
	 * \brief ������ť
	 * \param lpszCaption ������Ϊ��ť����ʾ�ı�(����50�ַ�,����ı��ض�)
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
    BOOL Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,
                UINT nID, LPCTSTR lpszNormalImage, LPCTSTR lpszLightImage = _T(""),
                BKIMGLoadState bklState = BKLS_HORIZONTAL, HDC hdcParent = NULL, 
                UINT szText = 12, COLORREF crText = RGB(0,0,0) );

    //  �޸��ı�����
    //  ע:�ⲿ����ʱ���ɲ��ô�����Դ����
    //  �ڲ��Լ�����,�˴�ʹ��Ӧ����
    BOOL SetFont(HFONT HFont);

    //  �޸��ı���ɫ
    void SetColor(COLORREF crText);

    //  �޸Ĵ�������
    BOOL SetWindowRgn(HRGN hRgn);

    //  �޸��ı�
    void SetCaption(LPCTSTR lpszCaption);

     /**
	 * \brief ���ð�ť����״̬
     * \param fEnable ������Ϊ���ð�ť�Ƿ����
	 */
    void EnableButton(BOOL fEnable);

    //  ���ð�ť״̬
    void SetState(UINT nState);

    //  ��ȡ�ؼ����
    HWND GetSafeHWnd();

private:

    /**
	 * \brief ���ư�ť
     * \param hDC ������Ϊ��ť�Ļ����õ�DC���
	 */
    void Draw(HDC hDC);

    //  ���ƽ�������
    void DrawFocusRect(HDC hDC,RECT &rcWnd);

    //  ���ô�����Ϣ������
    void ReSetWindowProc();

    //  ��ʼ������DC
    void InitBkDC(HDC hDC = NULL);

public:

    //  �����ز���
    void OnLbuttonDown(WPARAM wParam, LPARAM lParam);

    void OnLbuttonUp(WPARAM wParam, LPARAM lParam);

    void OnLbuttonDBClick(WPARAM wParam, LPARAM lParam);

    void OnMouseMove(WPARAM wParam, LPARAM lParam);

    void OnMouseLeave(WPARAM wParam, LPARAM lParam);
    
private:

    /**
	 * \brief ��ť��Ϣ�ص�����
	 */
    static LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    CDrawButton(const CDrawButton &OtherDrawButton);

    CDrawButton &operator=(const CDrawButton &OtherDrawButton);

private:

    /**
    * \brief ��ť��Ĭ����Ϣ�ص�����.
    */
    WNDPROC     m_OldProc;

    /**
    * \brief �˰�ť�ľ��.
    */
    HWND        m_hWnd;

private:

    /**
    * \brief ������ԭ����ͼ���.
    */
    HBITMAP		m_hOldBkgBitmap;

    /**
    * \brief �����ڱ���ͼ.
    */
    HDC         m_hBackgroundDC;

    /**
    * \brief ��ť����ͨ��״̬λͼ.
    */
    Image     *m_pNormalStateImg;

    /**
    * \brief ��ť��������״̬λͼ.
    */
    Image     *m_pLightStateImg; 

    /**
    * \brief �Ƿ�������Ч��.
    */
    BOOL        m_fLight;   

    /**
    * \brief ����ƶ�״̬.
    */
    BOOL        m_fTrackMouse;

    /**
    * \brief ��ť�ĵ�ǰ״̬.
    */
    int         m_nButtonState;

    /**
    * \brief �ؼ�������.
    */
    HRGN        m_hRgn;

    /**
    * \brief �ؼ�������.
    */
    HFONT       m_hFont;

    /**
    * \brief �ؼ�������ɫ.
    */
    COLORREF    m_crText;

    //  ����ͼƬ���ط�ʽ
    BKIMGLoadState  m_bkimState;

    /**
    * \brief ��ť�ĵ�ǰ״̬,�����������ð�ť��״̬��������
             Ӧ�ó�����combobox�еİ�ť���ʱ��״̬
    */
    BOOL         m_fButtonDown;
};
