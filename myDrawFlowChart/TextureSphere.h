#if !defined(AFX_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TexturelBiquatricBezierPatch.h"
#include "Patch.h"
#include "TextureZBuffer.h"

class CTextureSphere//球类
{
public:
	CTextureSphere(void);
	virtual ~CTextureSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC, CTextureZBuffer* pZBuffer);//绘制图形
public:
	CColorP3 Ver[26];//球面二次控制点
	CTexturelBiquatricBezierPatch bezier;//曲面片
private:
	CPatch Pat[8];//球面曲面片数	
};

#endif // !defined(AFX_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
