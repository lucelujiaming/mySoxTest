#include "stdafx.h"
#include "SmoothFillTriangle.h"

#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CSmoothFillTriangle::CSmoothFillTriangle(void)
{

}

CSmoothFillTriangle::CSmoothFillTriangle(CColorPoint P0, CColorPoint P1, CColorPoint P2)
{
	m_P[0] = P0;
	m_P[1] = P1;
	m_P[2] = P2;
}

CSmoothFillTriangle::~CSmoothFillTriangle(void)
{

}

/**********************************************************************/
/* 这个逻辑非常简单。就是对于着色一个三角形来说，其实就是从三角形的   */
/* 最低点开始，一行一行的画线。     *//*
/* 而线的始位置和结束位置的计算方法其实和之前的画线算法一样。         */
/* 这里采用的是最简单的DDA算法。也就是依靠ROUND的四舍五入             */
/* 计算三角形边的坐标。而边坐标就是着色线的始位置和结束位置。         */
/* 这里唯一需要考虑的是三角形的方向。也就是左三角形和右三角形的概念。 */
/* 所谓的左三角形就是对于三角形的三个顶点来说，                       */
/* 中间点位于最高点和最低点的左侧。反之如果位于右侧。就是右三角形，   */
/**********************************************************************/
void CSmoothFillTriangle::Fill(CDC* pDC)
{
	// 顶点按照y坐标由小到大排序
	SortPoint();
	// 定义三角形覆盖的扫描线条数
	int nTotalScanLine = m_P[2].y - m_P[0].y + 1;
	// 定义span的起点与终点数组
	m_SpanLeft = new CColorPoint[nTotalScanLine];
	m_SpanRight = new CColorPoint[nTotalScanLine];
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
	// 这里和CFlatFillTriangle::Fill区别在于，我们使用渐变色画点，而不是直接画线。
	for (int y = m_P[0].y; y < m_P[2].y; y++)//下闭上开
	{
		int n = y - m_P[0].y;
		for (int x = m_SpanLeft[n].x; x < m_SpanRight[n].x; ++x)//左闭右开
		{
			CRGB crColor = Interp(x, m_SpanLeft[n].x, m_SpanRight[n].x, m_SpanLeft[n].c, m_SpanRight[n].c);
			pDC->SetPixelV(x, y, RGB(crColor.red * 255, crColor.green * 255, crColor.blue * 255));//绘制像素点
		}
	}
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

void CSmoothFillTriangle::EdgeFlag(CColorPoint PStart, CColorPoint PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		if (bFeature)
			m_SpanLeft[m_nIndex++] = CColorPoint(ROUND(x), y, crColor);
		else
			m_SpanRight[m_nIndex++] = CColorPoint(ROUND(x), y, crColor);
		x += m;
	}
}

void CSmoothFillTriangle::SortPoint(void)//排序
{
	CColorPoint pt;//要求排序后，m_P[0].y<m_P[1].y<m_P[2].y
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

CRGB CSmoothFillTriangle::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//颜色线性插值
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}