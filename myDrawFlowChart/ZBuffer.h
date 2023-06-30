#if !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//带颜色的二维整数点类
#include "ColorVector3.h"

class CZBuffer//ZBuffer类
{
public:
	CZBuffer(void);
	virtual ~CZBuffer(void);
	void SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2);//三顶点构造三角形
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//初始化深度缓冲器
	void FillTriangle(CDC* pDC);//填充三角形
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1);//颜色线性插值
private:
	CColorP3 P0, P1, P2;//三角形的浮点坐标
	CColorPoint3 point0, point1, point2;//三角形的整数顶点坐标
	CColorPoint2* SpanLeft; //跨度的起点数组标志
	CColorPoint2* SpanRight;//跨度的终点数组标志
	int nIndex;//记录扫描线条数
	double** zBuffer;//深度缓冲区
	int nWidth, nHeight;//缓冲区宽度和高度
};

#endif // !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
