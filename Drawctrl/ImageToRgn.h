#pragma once
#include <GdiPlus.h>
using namespace Gdiplus;

#include <vector>
using namespace std;

class CImageToRgn
{
public:

    CImageToRgn(void);

    ~CImageToRgn(void);

public:

    enum DEFAULT
    {
        Default_Width = -1,
        Default_Height = -2
    };

    void AddRgn(HRGN rgn);

    HRGN GetRgn(UINT nIndex);

    /**
	 * \brief 从图片中创建rgn
	 * \param lpszImage 参数，为图片的路径
     * \param nRngWidth 参数，为rgn的宽度
     * \param nRngHeight 参数，为rgn的高度
     * \param nImgWidth 参数，为图片需要使用的宽度
     * \param nImgHeight 参数，为图片需要使用的高度
     * \param nXPos 参数，为图片需要使用的起始位置横坐标
     * \param nYPos 参数，为图片需要使用的起始位置纵坐标
     * \param cfTransparent 参数，使图片需要透明的颜色
	 */
    static HRGN CreateRgn(LPCTSTR lpszImage, int nRngWidth, int nRngHeight, \
        int nXPos = 0, int nYPos = 0, int nImgWidth = Default_Width, int nImgHeight = Default_Height, \
        COLORREF cfTransparent = RGB(255, 0, 255));

private:

    vector<HRGN>    m_vecRgns;

private:

    CImageToRgn(const CImageToRgn &OtherImageToRgn);

    CImageToRgn &operator=(const CImageToRgn &OtherImageToRgn);
};
