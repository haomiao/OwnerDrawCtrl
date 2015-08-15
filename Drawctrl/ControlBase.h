///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   ControlBase.h
/// 	@brief	�ؼ�����. 
/// 
///		�ṩ�ؼ��Ի���������ӿ�  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/08/12  10:08:48
///  
///  
/////////////////////////////////////////////////////////////////////

#pragma once

#define SAFE_FREE(object)\
    if (object != NULL)\
    {\
        delete object;\
        object = NULL;\
    }

#define SAFE_DELETE(object)\
    if (object != NULL)\
    {\
        DeleteObject(object);\
        object = NULL;\
    }


//  ��̬ͼƬ������ʾ��ʽ
//  �ɰ��ձ���ͼƬѡ����Ӧ��ʾ��ʽ
enum BKIMGLoadState
{
    BKLS_HORIZONTAL = 0,    //  ˮƽ
    BKLS_VERTICAL,          //  ��ֱ
};


    /**
    * \brief �ؼ���״̬.
    */
enum STATE
{
    STATE_UP = 0,
    STATE_DOWN,
    STATE_FOCUS,
    STATE_DISABLE
};


class CControlBase
{
public:

    virtual ~CControlBase(void){};

public:

    /**
	 * \brief ����Ч���ؼ�
     * \param fLight ������Ϊ��ǰ�Ƿ�Ҫ�Կؼ�����������ʾ
	 */
    virtual BOOL LightingCtrl(BOOL fLight) = 0;


    /**
	 * \brief ˢ�¿ؼ�
	 */
    virtual BOOL InvalidataCtrl() = 0;
};
