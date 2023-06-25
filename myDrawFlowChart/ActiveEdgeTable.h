#if !defined(AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include"ColorPoint.h"

/************************************************************************/
/* 边表与活动边表类。                                                   */
/* 多边形与当前扫描线相交的边称为有效边。                               */
/* 在处理一条扫描线的时候，我们仅需对有效边进行求交处理。               */
/************************************************************************/
class CActiveEdgeTable
{
public:
	CActiveEdgeTable(void);
	virtual ~CActiveEdgeTable(void);
public:
	double x;            // 当前扫描线与有效边交点的x坐标
	int yMax;            // 有效边的最大y值。用于判断该边何时扫描完毕，以便于抛弃该边为无用边。
	double m;            // 斜率的倒数(x的增量)
	CColorPoint pStart;  // 边的起点
	CColorPoint pEnd;    // 边的终点
	CActiveEdgeTable* pNext;
};

#endif // !defined(AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

