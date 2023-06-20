#pragma once

#include "stdafx.h"

class CTriangle//三角形填充类
{
public:
	CTriangle(void);
	CTriangle(CPoint P0, CPoint P1, CPoint P2);//三顶点构造三角形
	virtual ~CTriangle(void);
	void Fill(CDC* pDC);//填充三角形
private:
	void EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
private:
	CPoint P[3];//顶点
	CPoint* SpanLeft; //跨度的起点标志数组
	CPoint* SpanRight;//跨度的终点标志数组
	int nIndex;//扫描线索引
};