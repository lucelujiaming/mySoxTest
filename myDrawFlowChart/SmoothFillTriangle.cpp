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
/* ����߼��ǳ��򵥡����Ƕ�����ɫһ����������˵����ʵ���Ǵ������ε�   */
/* ��͵㿪ʼ��һ��һ�еĻ��ߡ�     *//*
/* ���ߵ�ʼλ�úͽ���λ�õļ��㷽����ʵ��֮ǰ�Ļ����㷨һ����         */
/* ������õ�����򵥵�DDA�㷨��Ҳ��������ROUND����������             */
/* ���������αߵ����ꡣ�������������ɫ�ߵ�ʼλ�úͽ���λ�á�         */
/* ����Ψһ��Ҫ���ǵ��������εķ���Ҳ�����������κ��������εĸ�� */
/* ��ν���������ξ��Ƕ��������ε�����������˵��                       */
/* �м��λ����ߵ����͵����ࡣ��֮���λ���Ҳࡣ�����������Σ�   */
/**********************************************************************/
void CSmoothFillTriangle::Fill(CDC* pDC)
{
	// ���㰴��y������С��������
	SortPoint();
	// ���������θ��ǵ�ɨ��������
	int nTotalScanLine = m_P[2].y - m_P[0].y + 1;
	// ����span��������յ�����
	m_SpanLeft = new CColorPoint[nTotalScanLine];
	m_SpanRight = new CColorPoint[nTotalScanLine];
	// �ж�P1��λ��P0��P2�ߵ���໹���Ҳ�
	int nDeltz = (m_P[2].x - m_P[0].x) * (m_P[1].y - m_P[0].y) 
		       - (m_P[2].y - m_P[0].y) * (m_P[1].x - m_P[0].x); // ��ʸ�������z����
	if(nDeltz > 0) // �������Ρ�˵����
	{
		// ��0��ʼ���㻭�ߵ���ʼ�㡣
		m_nIndex = 0;
		// P[0]��P[1]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(m_P[0], m_P[1], LEFT);
		// P[1]��P[2]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(m_P[1], m_P[2], LEFT);
		// ��0��ʼ���㻭�ߵĽ����㡣
		m_nIndex = 0;
		// P[0]��P[2]Ϊ���ߵĽ����㡣
		EdgeFlag(m_P[0], m_P[2], RIGHT);
	}
	else//�������Ρ�˵����
	{
		// ��0��ʼ���㻭�ߵ���ʼ�㡣
		m_nIndex = 0;
		// P[0]��P[2]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(m_P[0], m_P[2], LEFT);
		// ��0��ʼ���㻭�ߵĽ����㡣
		m_nIndex = 0;	
		// P[0]��P[1]Ϊ���ߵĽ����㡣
		EdgeFlag(m_P[0], m_P[1], RIGHT);
		// P[1]��P[2]Ϊ���ߵĽ����㡣
		EdgeFlag(m_P[1], m_P[2], RIGHT);
	}
	// ���濪ʼʹ�ü����������ʼ��ͽ����㻭�ߡ�
	// �����CFlatFillTriangle::Fill�������ڣ�����ʹ�ý���ɫ���㣬������ֱ�ӻ��ߡ�
	for (int y = m_P[0].y; y < m_P[2].y; y++)//�±��Ͽ�
	{
		int n = y - m_P[0].y;
		for (int x = m_SpanLeft[n].x; x < m_SpanRight[n].x; ++x)//����ҿ�
		{
			CRGB crColor = Interp(x, m_SpanLeft[n].x, m_SpanRight[n].x, m_SpanLeft[n].c, m_SpanRight[n].c);
			pDC->SetPixelV(x, y, RGB(crColor.red * 255, crColor.green * 255, crColor.blue * 255));//�������ص�
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

void CSmoothFillTriangle::EdgeFlag(CColorPoint PStart, CColorPoint PEnd, BOOL bFeature)//DDA�㷨��ɢ��
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

void CSmoothFillTriangle::SortPoint(void)//����
{
	CColorPoint pt;//Ҫ�������m_P[0].y<m_P[1].y<m_P[2].y
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

CRGB CSmoothFillTriangle::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//��ɫ���Բ�ֵ
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}