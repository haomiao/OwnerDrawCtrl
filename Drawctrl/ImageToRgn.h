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
	 * \brief ��ͼƬ�д���rgn
	 * \param lpszImage ������ΪͼƬ��·��
     * \param nRngWidth ������Ϊrgn�Ŀ��
     * \param nRngHeight ������Ϊrgn�ĸ߶�
     * \param nImgWidth ������ΪͼƬ��Ҫʹ�õĿ��
     * \param nImgHeight ������ΪͼƬ��Ҫʹ�õĸ߶�
     * \param nXPos ������ΪͼƬ��Ҫʹ�õ���ʼλ�ú�����
     * \param nYPos ������ΪͼƬ��Ҫʹ�õ���ʼλ��������
     * \param cfTransparent ������ʹͼƬ��Ҫ͸������ɫ
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
