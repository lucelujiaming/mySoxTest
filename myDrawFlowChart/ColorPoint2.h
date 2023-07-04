#if !defined(AFX_COLOR_POINT2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_POINT2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"RGB.h"
#include "Vector3.h"
#include "TextureCoordinate.h"//图像纹理

class CColorPoint2
{
public:
	CColorPoint2(void);
	CColorPoint2(int x, int y);
	CColorPoint2(int x, int y, CRGB c);
	CColorPoint2(int x, int y, CVector3 n);
	CColorPoint2(int x, int y, CVector3 n, CTextureCoordinate t);
	virtual ~CColorPoint2(void);	
public:
	int x;
	int y;
	CRGB c;
		
	CVector3 n;            // 法向量
	CTextureCoordinate t;  // 二维纹理
};

#endif // !defined(AFX_COLOR_POINT2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
