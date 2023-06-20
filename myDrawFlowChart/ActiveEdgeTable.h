#if !defined(AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include"ColorPoint.h"

class CActiveEdgeTable//边表与活动边表类
{
public:
	CActiveEdgeTable(void);
	virtual ~CActiveEdgeTable(void);
public:
	double x;//当前扫描线与有效边交点的x坐标
	int yMax;//边的最大y值
	double m;//斜率的倒数(x的增量)
	CColorPoint pStart;//边的起点
	CColorPoint pEnd;//边的终点
	CActiveEdgeTable* pNext;
};

#endif // !defined(AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

