#if !defined(AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

class CFlatFillTriangle//三角形填充类
{
public:
	CFlatFillTriangle(void);
	CFlatFillTriangle(CPoint P0, CPoint P1, CPoint P2);//三顶点构造三角形
	virtual ~CFlatFillTriangle(void);
	void Fill(CDC* pDC);//填充三角形
private:
	void EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
private:
	CPoint   m_P[3];       // 三角形的三个顶点
	CPoint*  m_SpanLeft;   // 跨度的起点标志数组
	CPoint*  m_SpanRight;  // 跨度的终点标志数组
	int      m_nIndex;     // 扫描线索引
};

#endif // !defined(AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
