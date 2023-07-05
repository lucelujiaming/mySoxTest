#if !defined(AFX_ANTIALIASED_BUMP_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ANTIALIASED_BUMP_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"resource.h"
#include"RGB.h"

class CAntiAliasedBumpTexture
{
public:
	CAntiAliasedBumpTexture(void);
	virtual~CAntiAliasedBumpTexture(void);
	void ReadBitmap(UINT nIDResource);//读入位图
	void SetDestImage(double r);//设置目标图像
	void AntiAliasedBumpTextureAntialiasing(CRGB** DestImage, int nDestWidth, int nDestHeight, CRGB** SrcImage, int nSrcWidth, int nSrcHeight);//纹理反走样
public:
	CRGB** SrcImage, **DestImage;//源图像与目标图像
	double** Bu, **Bv;//图像x方向的梯度和y方向的梯度
	BITMAP bmp;//BITMAP结构体变量
	int nDestWidth, nDestHeight;//图像的宽度和高度
};

#endif // !defined(AFX_ANTIALIASED_BUMP_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
