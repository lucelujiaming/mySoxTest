#if !defined(AFX_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����ط���û���޸ģ���Ϊ�ҶԱ�������ӳ��Ͱ�͹����ӳ�䡣�о�Ӧ�ÿ�����һ���ࡣ
#include "BumpTextureBiquatricBezierPatch.h"
#include "Patch.h"
#include "BumpTextureZBuffer.h"

class CBumpTextureSphere//����
{
public:
	CBumpTextureSphere(void);
	virtual ~CBumpTextureSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC, CBumpTextureZBuffer* pZBuffer);//����ͼ��
public:
	CColorP3 Ver[26];//������ο��Ƶ�
	CBumpTextureBiquatricBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8];//��������Ƭ��	
};

#endif // !defined(AFX_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
