///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   DrawCheckbox.h
/// 	@brief	��ѡ��ؼ�. 
/// 
///		��Ҫʵ�ָ�ѡ��ؼ����Ի�  
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
* \brief ��ѡ��ؼ����������ڵ���Ϣ.
*/
#define WM_CHECKBOXCLICKED   WM_USER + 2100  

class CDrawCheckbox:public CControlBase
{
public:

    CDrawCheckbox(void);

    ~CDrawCheckbox(void);

public:
    
     /**
	 * \brief ˢ�¿ؼ�
	 */
    virtual BOOL InvalidataCtrl();

    /**
	 * \brief ����Ч����ť
     * \param fLight ������Ϊ��ǰ�Ƿ�Ҫ�԰�ť����������ʾ
	 */
    virtual BOOL LightingCtrl(BOOL fLight);

public:

    /**
	 * \brief �����ؼ�
	 * \param lpszCaption ������Ϊ�ؼ�����ʾ�ı�(����50�ַ�,����ı��ض�)
     * \param lpszImage ������Ϊ�ؼ���λͼ·��
     * \param rect ������Ϊ�ؼ�������
     * \param hParentWnd ������Ϊ�ؼ��ĸ����ھ��
     * \param nID ������Ϊ�ؼ��Ŀؼ�ID
     * \param lpszImage ������Ϊ��״̬λͼ·��
     * \param lpszLightImage ������Ϊ��ť�ؼ�����Ч����λͼ·��
     * \param bklState ����,Ϊ����ͼƬ������ʾ��ʽ
     * \param hdcParent ������Ϊ�����ڱ����ļ���DC��
        ����Ѿ��и����ڵ�DC,�����������ÿؼ���������DCֱ��ʹ��
        ��������ȥ�����ڵĻ�ȡ����������DC
     * \param szText    �����С
	 */
    BOOL Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,\
        UINT nID, LPCTSTR lpszImage, LPCTSTR lpszLightImage = _T(""), BKIMGLoadState bklState = BKLS_HORIZONTAL, COLORREF colorText = RGB(255, 2552, 255), UINT szText = 12, HDC hdcParent = NULL);

	BOOL Create(LPCTSTR lpszCaption, const RECT& rect, HWND hParentWnd,\
        UINT nID, LPCTSTR lpszImage, LPCTSTR lpszLightImage = _T(""), BKIMGLoadState bklState = BKLS_HORIZONTAL, COLORREF colorText = RGB(255, 2552, 255), COLORREF colorBackgroud = RGB(255, 2552, 255), UINT szText = 12);
 
    //  �����Ƿ�ѡ��
    void SetCheck(BOOL bCheck);

    //  ��ȡѡ�����״̬
    BOOL GetCheck();

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

    //  ��ȡ�ؼ����
    HWND GetSafeHWnd();

private:

    /**
	 * \brief �ؼ���Ϣ�ص�����
	 */
    static LRESULT CALLBACK CheckboxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

     //  ��ʼ������DC
	void InitBkDC(HDC hDC = NULL);

	/**
	 * \brief ���ƿؼ�
     * \param hDC ������Ϊ�ؼ��Ļ�����DC���
	 */
    void Draw(HDC hDC);

    //  ���ٴ��ڶ���
    void DestroyWindow();

private:

    //  ���������
    void OnLbuttonDown(WPARAM wParam, LPARAM lParam);

    //  ò��û�н��յ�WM_LBUTTONUP��Ϣ?
    void OnLbuttonUp(WPARAM wParam, LPARAM lParam);

    void OnLbuttonDBClick(WPARAM wParam, LPARAM lParam);

    void OnMouseMove(WPARAM wParam, LPARAM lParam);

    void OnMouseLeave(WPARAM wParam, LPARAM lParam);

private:

    CDrawCheckbox(const CDrawCheckbox &);

    CDrawCheckbox &operator=(const CDrawCheckbox &);

private:

    /**
    * \brief �˿ؼ��ľ��.
    */
    HWND        m_hWnd;   

private:

    //���ڱ������ڴ�DC
	CDrawDC	 m_DrawBackgroundDC;

    /**
    * \brief ��ť����ͨ3 * 2״̬λͼ.
    */
    Image     *m_pNormalStateImg;

    /**
    * \brief ��ť������3 * 2״̬λͼ.
    */
    Image     *m_pLightStateImg; 

    /**
    * \brief �Ƿ�������Ч��.
    */
    BOOL        m_fLight;

    /**
    * \brief �ؼ��ĵ�ǰ״̬.
    */
    int         m_nCtrlState;

    /**
    * \brief ����ƶ�״̬.
    */
    BOOL        m_fTrackMouse;

    /**
    * \brief �ؼ�������.
    */
    HFONT       m_hFont;

	/**
    * \brief �ؼ���region.
    */
    HRGN        m_hCtrlRgn;

    /**
    * \brief �ؼ�������ɫ.
    */
	COLORREF	m_colorText;

    //  �Ƿ��ѱ�ѡ��
	BOOL		m_bCheck;

    //  ����ͼƬ���ط�ʽ
    BKIMGLoadState  m_bkimState;
};
