#include "stdafx.h"
#include "RationalQuadraticBezierCurve.h"
#define ROUND(d) int(d + 0.5)//四舍五入宏定义

CRationalQuadraticBezierCurve::CRationalQuadraticBezierCurve(void)
{

}

CRationalQuadraticBezierCurve::~CRationalQuadraticBezierCurve(void)
{

}

void CRationalQuadraticBezierCurve::ReadPoint(CP2* P, double* w)
{
	for (int i = 0; i < 3; i++)
	{
		this->P[i] = P[i];
		this->w[i] = w[i];
	}
}

/************************************************************************/
/* 有理二次Bezier曲线的有理分式表示为：                                 */
/*   p(t) =                                                             */
/*       (B0,2(t) * w0 * P0 + B1,2(t) * w1 * P1 + B2,2(t) * w2 * P2)    */
/*     / (B0,2(t) * w0 + B1,2(t) * w1 + B2,2(t) * w2)                   */
/*        = [(1-t)^2 * w0 * P0 + 2t(1-t) * w1 * P1 + t^2 * w2 * P2)]    */
/*        / [(1-t)^2 * w0      + 2t(1-t) * w1      + t^2 * w2)]         */
/*  其中：                                                              */
/*     Bern0,2(t) = (1-t)^2                                             */
/*     Bern1,2(t) = 2t(1-t)                                             */
/*     Bern2,2(t) = t^2                                                 */
/*  下面的代码描述了这个计算过程。                                      */
/************************************************************************/
void CRationalQuadraticBezierCurve::DrawCurve(CDC* pDC)
{
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	pOldPen = pDC->SelectObject(&NewPen);
	double Bern02, Bern12, Bern22;
	double tStep = 0.01;
	pDC->MoveTo(ROUND(P[0].x),ROUND(P[0].y));
	for(double t = 0.0;t <= 1.0;t += tStep)
	{
		CP2 pt;
		Bern02 = (1 - t) * (1 - t);  // 计算Bern0,2(t)
		Bern12 = 2 * t * (1-t);      // 计算Bern1,2(t)
		Bern22 = t * t;              // 计算Bern2,2(t)
		double denominator = Bern02 * w[0] + Bern12 * w[1] + Bern22 * w[2];
		pt.x = (Bern02 * P[0].x * w[0] + Bern12 * P[1].x * w[1] + Bern22 * P[2].x * w[2])/denominator;
		pt.y = (Bern02 * P[0].y * w[0] + Bern12 * P[1].y * w[1] + Bern22 * P[2].y * w[2])/denominator;
		pDC->LineTo(ROUND(pt.x), ROUND(pt.y));
	}
	pDC->SelectObject(pOldPen);
}

void CRationalQuadraticBezierCurve::DrawPolygon(CDC* pDC)//绘制控制多边形
{
	CPen pen(PS_SOLID, 3, RGB(0, 0, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush brush(RGB(0, 0, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	for(int i = 0;i < 3;i++)
	{
		if(0 == i)
		{
			pDC->MoveTo(ROUND(P[i].x), ROUND(P[i].y));
			pDC->Ellipse(ROUND(P[i].x-5), ROUND(P[i].y)-5, ROUND(P[i].x+5), ROUND(P[i].y)+5);
		}
		else
		{
			pDC->LineTo(ROUND(P[i].x), ROUND(P[i].y));
			pDC->Ellipse(ROUND(P[i].x)-5, ROUND(P[i].y)-5, ROUND(P[i].x)+5, ROUND(P[i].y)+5);
		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}