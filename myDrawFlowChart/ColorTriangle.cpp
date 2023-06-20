#include "stdafx.h"
#include "ColorTriangle.h"

#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CColorTriangle::CColorTriangle(void)
{

}

CColorTriangle::CColorTriangle(CColorPoint P0, CColorPoint P1, CColorPoint P2)
{
	P[0] = P0;
	P[1] = P1;
	P[2] = P2;
}

CColorTriangle::~CColorTriangle(void)
{

}

void CColorTriangle::Fill(CDC* pDC)
{
	//顶点按照y坐标由小到大排序
	SortPoint();
	//定义三角形覆盖的扫描线条数
	int nTotalScanLine = P[2].y - P[0].y + 1;
	//定义span的起点与终点数组
	SpanLeft = new CColorPoint[nTotalScanLine];
	SpanRight = new CColorPoint[nTotalScanLine];
	//判断P1点位于P0P2边的左侧还是右侧
	int nDeltz = (P[2].x - P[0].x) * (P[1].y - P[0].y) - (P[2].y - P[0].y) * (P[1].x - P[0].x);//点矢量叉积的z分量
	if (nDeltz > 0)//左三角形
	{
		nIndex = 0;
		EdgeFlag(P[0], P[1], LEFT);
		EdgeFlag(P[1], P[2], LEFT);
		nIndex = 0;
		EdgeFlag(P[0], P[2], RIGHT);
	}
	else//右三角形
	{
		nIndex = 0;
		EdgeFlag(P[0], P[2], LEFT);
		nIndex = 0;
		EdgeFlag(P[0], P[1], RIGHT);
		EdgeFlag(P[1], P[2], RIGHT);
	}
	for (int y = P[0].y; y < P[2].y; y++)//下闭上开
	{
		int n = y - P[0].y;
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; ++x)//左闭右开
		{
			CRGB crColor = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].c, SpanRight[n].c);
			pDC->SetPixelV(x, y, RGB(crColor.red * 255, crColor.green * 255, crColor.blue * 255));//绘制像素点
		}
	}
	if(SpanLeft)
	{
		delete []SpanLeft;
		SpanLeft = NULL;
	}
	if(SpanRight)
	{
		delete []SpanRight;
		SpanRight = NULL;
	}
}

void CColorTriangle::EdgeFlag(CColorPoint PStart, CColorPoint PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		if (bFeature)
			SpanLeft[nIndex++] = CColorPoint(ROUND(x), y, crColor);
		else
			SpanRight[nIndex++] = CColorPoint(ROUND(x), y, crColor);
		x += m;
	}
}

void CColorTriangle::SortPoint(void)//排序
{
	CColorPoint pt;//要求排序后，P[0].y<P[1].y<P[2].y
	if (P[0].y > P[1].y)
	{
		pt = P[0];
		P[0] = P[1];
		P[1] = pt;
	}
	if (P[0].y > P[2].y)
	{
		pt = P[0];
		P[0] = P[2];
		P[2] = pt;
	}
	if (P[1].y > P[2].y)
	{
		pt = P[1];
		P[1] = P[2];
		P[2] = pt;
	}
}

CRGB CColorTriangle::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//颜色线性插值
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}