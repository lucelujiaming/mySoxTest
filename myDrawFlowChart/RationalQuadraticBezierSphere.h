#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RationalBiquatricBezierPatch.h"
#include "Patch.h"

// ��������˫����Bezier��
// ˫����Bezier��ͨ������8Ƭ����Ƭ��ɵġ�
class CRationalQuadraticBezierSphere//����
{
public:
	CRationalQuadraticBezierSphere(void);
	virtual ~CRationalQuadraticBezierSphere(void);
	void ReadVertex(void);              // ������Ƶ��
	void ReadFace(void);                // ��������Ƭ��
	void Draw(CDC* pDC);//����ͼ��
public:
	CP3 Ver[26]; // ������ο��Ƶ�	
	CRationalBiquatricBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8]; // ��������Ƭ��
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
