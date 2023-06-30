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
	void ReadPoint(CP2* P, double* w);//读入控制点
	void DrawCurve(CDC* pDC);//绘制曲线
	void DrawPolygon(CDC* pDC);//绘制控制多边形
private:
	CP2 P[3];//控制点坐标
	double w[3];//权因子
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_CURVE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
