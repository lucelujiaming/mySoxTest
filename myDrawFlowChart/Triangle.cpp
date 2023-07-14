#include "stdafx.h"
#include "Triangle.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CTriangle::CTriangle(void)
{

}

CTriangle::~CTriangle(void)
{

}

void CTriangle::SetPoint(CColorP2 P0, CColorP2 P1, CColorP2 P2)
{
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
	P[0].x = ROUND(P0.x);
	P[0].y = ROUND(P0.y);
	P[0].c = P0.c;
	P[1].x = ROUND(P1.x);
	P[1].y = ROUND(P1.y);
	P[1].c = P1.c;
	P[2].x = ROUND(P2.x);
	P[2].y = ROUND(P2.y);
	P[2].c = P2.c;

}

void CTriangle::Fill(CDC* pDC)
{
	//���㰴��y������С��������
	SortPoint();
	//���������θ��ǵ�ɨ��������
	int nTotalScanLine = P[2].y - P[0].y + 1;
	//����span��������յ�����
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	//�ж�P1��λ��P0P2�ߵ���໹���Ҳ�
	int nDeltz = (P[2].x - P[0].x) * (P[1].y - P[0].y)
		       - (P[2].y - P[0].y) * (P[1].x - P[0].x); // ��ʸ�������z����
	if (nDeltz > 0)//��������
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
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; ++x)//����ҿ�
		{
			CRGB crColor = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].c, SpanRight[n].c);
			pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(crColor.red * 255, crColor.green * 255, crColor.blue * 255));//�������ص�
		}
	}
	if (SpanLeft)
	{
		delete[]SpanLeft;
		SpanLeft = NULL;
	}
	if (SpanRight)
	{
		delete[]SpanRight;
		SpanRight = NULL;
	}
}

void CTriangle::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA�㷨��ɢ��
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		if (bFeature)
        {
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, crColor);
        }
		else
        {
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y, crColor);
        }
		x += m;
	}
}

void CTriangle::SortPoint(void)//����
{
	CColorPoint2 pt;//Ҫ�������P[0].y<P[1].y<P[2].y
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

CRGB CTriangle::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//��ɫ���Բ�ֵ
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}
