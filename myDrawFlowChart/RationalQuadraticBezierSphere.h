#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RationalBiquatricBezierPatch.h"
#include "Patch.h"

class CRationalQuadraticBezierSphere//����
{
public:
	CRationalQuadraticBezierSphere(void);
	virtual ~CRationalQuadraticBezierSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC);//����ͼ��
public:
	CP3 Ver[26];//������Ƶ�	
private:
	CPatch Pat[8];//��������Ƭ��
	CRationalBiquatricBezierPatch bezier;//����Ƭ
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
