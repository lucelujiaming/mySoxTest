#include "stdafx.h"
#include "FlatFillTriangle.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CFlatFillTriangle::CFlatFillTriangle(void)
{

}

CFlatFillTriangle::CFlatFillTriangle(CPoint P0, CPoint P1, CPoint P2)
{
	m_P[0] = P0;
	m_P[1] = P1;
	m_P[2] = P2;
}

CFlatFillTriangle::~CFlatFillTriangle(void)
{

}

/**********************************************************************/
/* 这个逻辑非常简单。就是对于着色一个三角形来说，其实就是从三角形的   */
/* 最低点开始，一行一行的画线。                                       */
/* 而线的始位置和结束位置的计算方法其实和之前的画线算法一样。         */
/* 这里采用的是最简单的DDA算法。也就是依靠ROUND的四舍五入             */
/* 计算三角形边的坐标。而边坐标就是着色线的始位置和结束位置。         */
/* 这里唯一需要考虑的是三角形的方向。也就是左三角形和右三角形的概念。 */
/* 所谓的左三角形就是对于三角形的三个顶点来说，                       */
/* 中间点位于最高点和最低点的左侧。反之如果位于右侧。就是右三角形，   */
/**********************************************************************/
void CFlatFillTriangle::Fill(CDC* pDC)
{
	// 顶点按照y坐标由小到大排序
	SortPoint();
	// 定义三角形覆盖的扫描线条数
	int nTotalScanLine = m_P[2].y - m_P[0].y + 1;
	// 定义span的起点与终点数组
	m_SpanLeft  = new CPoint[nTotalScanLine];
	m_SpanRight = new CPoint[nTotalScanLine];		
	// 判断P1点位于P0和P2边的左侧还是右侧
	int nDeltz = (m_P[2].x - m_P[0].x) * (m_P[1].y - m_P[0].y) 
		       - (m_P[2].y - m_P[0].y) * (m_P[1].x - m_P[0].x); // 点矢量叉积的z分量
	if(nDeltz > 0) // 左三角形。说明：
	{
		// 从0开始计算画线的起始点。
		m_nIndex = 0;
		// P[0]到P[1]为画线的起始点。
		EdgeFlag(m_P[0], m_P[1], LEFT);
		// P[1]到P[2]为画线的起始点。
		EdgeFlag(m_P[1], m_P[2], LEFT);
		// 从0开始计算画线的结束点。
		m_nIndex = 0;
		// P[0]到P[2]为画线的结束点。
		EdgeFlag(m_P[0], m_P[2], RIGHT);
	}
	else//右三角形。说明：
	{
		// 从0开始计算画线的起始点。
		m_nIndex = 0;
		// P[0]到P[2]为画线的起始点。
		EdgeFlag(m_P[0], m_P[2], LEFT);
		// 从0开始计算画线的结束点。
		m_nIndex = 0;	
		// P[0]到P[1]为画线的结束点。
		EdgeFlag(m_P[0], m_P[1], RIGHT);
		// P[1]到P[2]为画线的结束点。
		EdgeFlag(m_P[1], m_P[2], RIGHT);
	}
	// 下面开始使用计算出来的起始点和结束点画线。
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&NewPen);
	for (int y = m_P[0].y; y < m_P[2].y; y++) // 下闭上开
	{
		int n = y - m_P[0].y;
		pDC->MoveTo(m_SpanLeft[n].x, y); // 填充跨度
		pDC->LineTo(m_SpanRight[n].x, y);
	}
	pDC->SelectObject(pOldPen);

	if(m_SpanLeft)
	{
		delete []m_SpanLeft;
		m_SpanLeft = NULL;
	}
	if(m_SpanRight)
	{
		delete []m_SpanRight;
		m_SpanRight = NULL;
	}
}

// DDA算法离散边
void CFlatFillTriangle::EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature) 
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		if (bFeature)
			m_SpanLeft[m_nIndex++] = CPoint(ROUND(x), y);
		else
			m_SpanRight[m_nIndex++] = CPoint(ROUND(x), y);
		x += m;
	}
}

void CFlatFillTriangle::SortPoint(void)//排序
{
	CPoint pt;//要求排序后，m_P[0].y<m_P[1].y<m_P[2].y
	if (m_P[0].y > m_P[1].y)
	{
		pt = m_P[0];
		m_P[0] = m_P[1];
		m_P[1] = pt;
	}
	if (m_P[0].y > m_P[2].y)
	{
		pt = m_P[0];
		m_P[0] = m_P[2];
		m_P[2] = pt;
	}
	if (m_P[1].y > m_P[2].y)
	{
		pt = m_P[1];
		m_P[1] = m_P[2];
		m_P[2] = pt;
	}
}