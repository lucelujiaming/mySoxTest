#if !defined(AFX_BICUBIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BICUBIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BicubicBezierPatch.h"
#include "Patch.h"

class CBicubicBezierSphere // ˫����Bezier������
{
public:
	CBicubicBezierSphere(void);
	virtual ~CBicubicBezierSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC);//����ͼ��
public:
	CP3 Ver[62];//������Ƶ���
private:
	CPatch Pat[8];//��������Ƭ��
};

#endif // !defined(AFX_BICUBIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
