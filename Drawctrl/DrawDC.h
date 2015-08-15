///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   DrawDC.h
/// 	@brief	DC管理. 
/// 
///		主要实现兼容DC创建  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/08/12  16:50:59
///  
///  
/////////////////////////////////////////////////////////////////////

#pragma once

class CDrawDC
{
public:

    CDrawDC(void);

    ~CDrawDC(void);

private:

    CDrawDC(const CDrawDC &OtherDrawDC);

    CDrawDC &operator=(const CDrawDC &OtherDrawDC);

public:

    BOOL CreateDrawDC(HDC hDC, int nWidth, int nHeight);

    void DeleteDrawDC();

public:

    HDC       m_hDC;

private:

    HBITMAP   m_hOldDrawDCBmp;
};
