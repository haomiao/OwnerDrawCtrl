///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   ControlBase.h
/// 	@brief	控件基类. 
/// 
///		提供控件自绘操作公共接口  
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


//  四态图片加载显示方式
//  可按照背景图片选择相应显示方式
enum BKIMGLoadState
{
    BKLS_HORIZONTAL = 0,    //  水平
    BKLS_VERTICAL,          //  垂直
};


    /**
    * \brief 控件四状态.
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
	 * \brief 亮灯效果控件
     * \param fLight 参数，为当前是否要对控件进行亮灯显示
	 */
    virtual BOOL LightingCtrl(BOOL fLight) = 0;


    /**
	 * \brief 刷新控件
	 */
    virtual BOOL InvalidataCtrl() = 0;
};
