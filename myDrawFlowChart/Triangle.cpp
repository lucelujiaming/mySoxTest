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
	//���㰴��y������С��������
	SortPoint();
	//���������θ��ǵ�ɨ��������
	int nTotalScanLine = P[2].y - P[0].y + 1;
	//����span��������յ�����
	SpanLeft  = new CPoint[nTotalScanLine];
	SpanRight = new CPoint[nTotalScanLine];		
	//�ж�P1��λ��P0P2�ߵ���໹���Ҳ�
	int nDeltz = (P[2].x - P[0].x) * (P[1].y - P[0].y) - (P[2].y - P[0].y) * (P[1].x - P[0].x);//��ʸ�������z����
	if(nDeltz > 0)//��������
	{
		nIndex = 0;
		EdgeFlag(P[0], P[1], LEFT);
		EdgeFlag(P[1], P[2], LEFT);
		nIndex = 0;
		EdgeFlag(P[0], P[2], RIGHT);
	}
	else//��������
	{
		nIndex = 0;
		EdgeFlag(P[0], P[2], LEFT);
		nIndex = 0;	
		EdgeFlag(P[0], P[1], RIGHT);
		EdgeFlag(P[1], P[2], RIGHT);
	}
	for (int y = P[0].y; y < P[2].y; y++)//�±��Ͽ�
	{
		int n = y - P[0].y;
		CPen NewPen, *pOldPen;
		NewPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		pOldPen = pDC->SelectObject(&NewPen);
		pDC->MoveTo(SpanLeft[n].x, y);//�����
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

void CTriangle::EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature)//DDA�㷨��ɢ��
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

void CTriangle::SortPoint(void)//����
{
	CPoint pt;//Ҫ�������P[0].y<P[1].y<P[2].y
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