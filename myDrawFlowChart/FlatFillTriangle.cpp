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
/* ����߼��ǳ��򵥡����Ƕ�����ɫһ����������˵����ʵ���Ǵ������ε�   */
/* ��͵㿪ʼ��һ��һ�еĻ��ߡ�                                       */
/* ���ߵ�ʼλ�úͽ���λ�õļ��㷽����ʵ��֮ǰ�Ļ����㷨һ����         */
/* ������õ�����򵥵�DDA�㷨��Ҳ��������ROUND����������             */
/* ���������αߵ����ꡣ�������������ɫ�ߵ�ʼλ�úͽ���λ�á�         */
/* ����Ψһ��Ҫ���ǵ��������εķ���Ҳ�����������κ��������εĸ�� */
/* ��ν���������ξ��Ƕ��������ε�����������˵��                       */
/* �м��λ����ߵ����͵����ࡣ��֮���λ���Ҳࡣ�����������Σ�   */
/**********************************************************************/
void CFlatFillTriangle::Fill(CDC* pDC)
{
	// ���㰴��y������С��������
	SortPoint();
	// ���������θ��ǵ�ɨ��������
	int nTotalScanLine = m_P[2].y - m_P[0].y + 1;
	// ����span��������յ�����
	m_SpanLeft  = new CPoint[nTotalScanLine];
	m_SpanRight = new CPoint[nTotalScanLine];		
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
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&NewPen);
	for (int y = m_P[0].y; y < m_P[2].y; y++) // �±��Ͽ�
	{
		int n = y - m_P[0].y;
		pDC->MoveTo(m_SpanLeft[n].x, y); // �����
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

// DDA�㷨��ɢ��
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

void CFlatFillTriangle::SortPoint(void)//����
{
	CPoint pt;//Ҫ�������m_P[0].y<m_P[1].y<m_P[2].y
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