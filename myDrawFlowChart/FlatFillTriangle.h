#if !defined(AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/************************************************************************/
/* 该类封装了平面着色的三角形填充算法                                   */
/* 因为无论多么复杂的物体，最终都可以使用三角形小面逼近。               */
/* 解决了填充三角形的问题，就解决了物体表面着色的问题。                 */
/* 而三角形是一个凸多边形。扫描线与三角形相交只会有一对交点。           */
/* 并形成一个相交区间。称为跨度Span。                                   */
/************************************************************************/
class CFlatFillTriangle // 三角形填充类
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
