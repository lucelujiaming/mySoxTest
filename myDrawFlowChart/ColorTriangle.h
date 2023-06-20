#if !defined(AFX_COLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint.h"//带颜色的二维整数点类

class CColorTriangle//三角形填充类
{
public:
	CColorTriangle(void);
	CColorTriangle(CColorPoint P0, CColorPoint P1, CColorPoint P2);//三顶点构造三角形
	virtual ~CColorTriangle(void);
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1);//颜色线性插值
	void Fill(CDC* pDC);//填充三角形
private:
	void EdgeFlag(CColorPoint PStart, CColorPoint PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
private:
	CColorPoint P[3];//顶点
	CColorPoint* SpanLeft; //跨度的起点标志数组
	CColorPoint* SpanRight;//跨度的终点标志数组
	int nIndex;//扫描线索引
};

#endif // !defined(AFX_COLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
