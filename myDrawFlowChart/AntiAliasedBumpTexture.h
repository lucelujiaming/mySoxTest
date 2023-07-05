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
	void ReadBitmap(UINT nIDResource);//����λͼ
	void SetDestImage(double r);//����Ŀ��ͼ��
	void AntiAliasedBumpTextureAntialiasing(CRGB** DestImage, int nDestWidth, int nDestHeight, CRGB** SrcImage, int nSrcWidth, int nSrcHeight);//��������
public:
	CRGB** SrcImage, **DestImage;//Դͼ����Ŀ��ͼ��
	double** Bu, **Bv;//ͼ��x������ݶȺ�y������ݶ�
	BITMAP bmp;//BITMAP�ṹ�����
	int nDestWidth, nDestHeight;//ͼ��Ŀ�Ⱥ͸߶�
};

#endif // !defined(AFX_ANTIALIASED_BUMP_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
