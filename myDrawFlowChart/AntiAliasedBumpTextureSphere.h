#if !defined(AFX_ANTIALIASED_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ANTIALIASED_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AntiAliasedBumpTextureBiquatricBezierPatch.h"
#include "Patch.h"

class CAntiAliasedBumpTextureSphere//����
{
public:
	CAntiAliasedBumpTextureSphere(void);
	virtual ~CAntiAliasedBumpTextureSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC, CAntiAliasedBumpTextureZBuffer* pZBuffer);//����ͼ��
public:
	CColorP3 Ver[26];//������ο��Ƶ�
	CAntiAliasedBumpTextureBiquatricBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8];//��������Ƭ��	
};

#endif // !defined(AFX_ANTIALIASED_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
