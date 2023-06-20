#include "Triangle.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CTriangle::CTriangle(void)
{

}

CTriangle::CTriangle(CPoint P0, CPoint P1, CPoint P2)
{
	P[0] = P0;
	P[1] = P1;
	P[2] = P2;
}

CTriangle::~CTriangle(void)
{

}

void CTriangle::Fill(CDC* pDC)
{
	//顶点按照y坐标由小到大排序
	SortPoint();
	//定义三角形覆盖的扫描线条数
	int nTotalScanLine = P[2].y - P[0].y + 1;
	//定义span的起点与终点数组
	SpanLeft  = new CPoint[nTotalScanLine];
	SpanRight = new CPoint[nTotalScanLine];		
	//判断P1点位于P0P2边的左侧还是右侧
	int nDeltz = (P[2].x - P[0].x) * (P[1].y - P[0].y) - (P[2].y - P[0].y) * (P[1].x - P[0].x);//点矢量叉积的z分量
	if(nDeltz > 0)//左三角形
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
		CPen NewPen, *pOldPen;
		NewPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		pOldPen = pDC->SelectObject(&NewPen);
		pDC->MoveTo(SpanLeft[n].x, y);//填充跨度
		pDC->LineTo(SpanRight[n].x, y);
		pDC->SelectObject(pOldPen);
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

void CTriangle::EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		if (bFeature)
			SpanLeft[nIndex++] = CPoint(ROUND(x), y);
		else
			SpanRight[nIndex++] = CPoint(ROUND(x), y);
		x += m;
	}
}

void CTriangle::SortPoint(void)//排序
{
	CPoint pt;//要求排序后，P[0].y<P[1].y<P[2].y
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