#if !defined(AFX_TRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint2.h"//带颜色的二维整数点类
#include "ColorP2.h"//带颜色的二维浮点类

class CTriangle
{
public:
	CTriangle(void);	
	virtual ~CTriangle(void);
	void SetPoint(CColorP2 P0, CColorP2 P1, CColorP2 P2);//三顶点构造三角形
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1);//颜色线性插值
	void Fill(CDC* pDC);//填充三角形
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
	CColorP2 P0, P1, P2;//三角形的浮点坐标
	CColorPoint2 P[3];//三角形的整数顶点坐标
	CColorPoint2* SpanLeft; //跨度的起点标志数组
	CColorPoint2* SpanRight;//跨度的终点标志数组
	int nIndex;//扫描线索引
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_TRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
