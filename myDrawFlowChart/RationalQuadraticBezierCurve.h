#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_CURVE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_CURVE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"P2.h"

class CRationalQuadraticBezierCurve
{
public:
	CRationalQuadraticBezierCurve(void);
	virtual ~CRationalQuadraticBezierCurve(void);
	void ReadPoint(CP2* P, double* w);//������Ƶ�
	void DrawCurve(CDC* pDC);//��������
	void DrawPolygon(CDC* pDC);//���ƿ��ƶ����
private:
	CP2 P[3];//���Ƶ�����
	double w[3];//Ȩ����
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_CURVE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
