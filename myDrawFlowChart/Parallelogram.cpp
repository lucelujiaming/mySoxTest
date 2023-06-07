// Parallelogram.cpp: implementation of the CParallelogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "Parallelogram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CParallelogram, CObject, 1)

/************************************************************************/
/* ���ܣ������������趨�����ӵ㡣                                       */
/************************************************************************/
CParallelogram::CParallelogram()
{
	m_Start = CPoint(0, 0);
	m_End = CPoint(0, 0);
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.Add(connPoint);
	}
}

CParallelogram::~CParallelogram()
{

}

/************************************************************************/
/* ���ܣ����ƺ�����������һ������κ���������֡�                         */
/************************************************************************/
void CParallelogram::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPoint points[4];
	long diffX = (long)((m_End.x - m_Start.x) * PARALLELOGRAM_INCLINATION_ANGLE);

	points[0].x = m_Start.x + diffX/2;
	points[0].y = m_Start.y;

	points[1].x = m_End.x + diffX/2;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - diffX/2;
	points[2].y = m_End.y;

	points[3].x = m_Start.x - diffX/2;
	points[3].y = m_End.y;

	CPen *oldPen;
	if(m_IsMark)
	{
		COLORREF m_Color(RGB(255, 0, 0));
		CPen pen(PS_SOLID, 1, m_Color);
		oldPen = pdc->SelectObject(&pen);
	}

	pdc->Polygon(points,4);

	if(m_IsMark)
	{
		pdc->SelectObject(oldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CParallelogram::DrawFocus( CDC *pdc )
{
	// ����Ϊ���ߣ��߿�Ϊ1����ɫΪ��ɫ��
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );
	// ��һ�����߿�
	pdc->Rectangle( CRect(m_Start, m_End) );
		
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);

	CAdjustPoint *connPoint = NULL;
	// ����RGB(0,255,0)����ɫ���ӵ㡣�ĽǴ�ΪԲ�Σ�Χ���ж�Ϊ���Ρ�
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    connPoint = (CAdjustPoint *)m_Points.GetAt(i);
		connPoint->Draw(pdc);
	}
}

/************************************************************************/
/* ���ܣ� �ƶ���������                                                */
/************************************************************************/
void CParallelogram::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CParallelogram::AdjustSize( CPoint &pt )
{
	switch(m_AdjustPoint)
	{
	// case 1: // ���Ͻ�
	case CCONNECTPOINT_RECT_LEFT_TOP:
		{
			m_Start = pt;
			break;
		}
	// case 2: // ���½�
	case CCONNECTPOINT_RECT_LEFT_BOTTOM:
		{
			m_Start.x = pt.x;
			m_End.y = pt.y;
			break;
		}
	// case 3: // ���Ͻ�
	case CCONNECTPOINT_RECT_RIGHT_TOP:
		{
			m_Start.y = pt.y;
			m_End.x = pt.x;
			break;
		}
	// case 4: // ���½�
	case CCONNECTPOINT_RECT_RIGHT_BOTTOM:
		{
			m_End = pt;
			break;
		}
	// case 5:
	case CCONNECTPOINT_RECT_MIDDLE_TOP:
		{
			m_Start.y = pt.y;
			break;
		}
	// case 6:
	case CCONNECTPOINT_RECT_MIDDLE_RIGHT:
		{
			m_End.x = pt.x;
			break;
		}
	// case 7:
	case CCONNECTPOINT_RECT_MIDDLE_BOTTOM:
		{
			m_End.y = pt.y;
			break;
		}
	// case 8:
	case CCONNECTPOINT_RECT_MIDDLE_LEFT:
		{
			m_Start.x = pt.x;
			break;
		}
	}
}

/************************************************************************/
/* ���ܣ��ж��Ƿ���ͼԪ�����ڡ�                                         */
/************************************************************************/
bool CParallelogram::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;

	CPoint points[4];
	// long diffX = (long)((m_End.x - m_Start.x) * 0.25);
	long diffX = (long)((m_End.x - m_Start.x) * PARALLELOGRAM_INCLINATION_ANGLE);
	

	points[0].x = m_Start.x + diffX;
	points[0].y = m_Start.y;

	points[1].x = m_End.x;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - diffX;
	points[2].y = m_End.y;

	points[3].x = m_Start.x;
	points[3].y = m_End.y;

	CRgn cr;
	BOOL bRet = cr.CreatePolygonRgn(points, 4, ALTERNATE);
	if(bRet && cr.PtInRegion( pt ))
	{
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	else if (bRet == FALSE)
	{
		m_objLogFile.WriteLog("points = {(%d, %d), (%d, %d), (%d, %d), (%d, %d)}", 
			points[0].x, points[0].y, points[1].x, points[1].y, 
			points[2].x, points[2].y, points[3].x, points[3].y);
	}
	return flag;
}

/************************************************************************/
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
/************************************************************************/
int CParallelogram::IsConnectOn(CAdjustPoint *pt)
{
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = (CAdjustPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt->GetPoint()))
		{
			pt->SetPoint(connPoint->GetPoint());
			return i;
		}
	}
	return CCONNECTPOINT_INVALID_OPTION;
}

/************************************************************************/
/* ���ܣ� �ж�һ����Ļ�����Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ��С��          */
/************************************************************************/
bool CParallelogram::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CAdjustPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt))
		{
			m_AdjustPoint = i; // 1+i;
		    flag = true;
			break;
		}
	}

	return flag;
}

/************************************************************************/
/* ���ܣ��ڵ�����С������ת��ʱ�򣬸��ݵ������������ʼ��ͽ��������ꡣ */
/************************************************************************/
void CParallelogram::AdjustStartAndEnd()
{
	CPoint newStart, newEnd;
	if((m_End.x < m_Start.x) && (m_End.y < m_Start.y))
	{
		newEnd = m_Start;
		m_Start = m_End;
		m_End = newEnd;
	}
	else if(!((m_End.x > m_Start.x) && (m_End.y > m_Start.y)))
	{
		newStart = CPoint( m_End.x, m_Start.y );
		newEnd = CPoint( m_Start.x, m_End.y );
		m_Start = newStart;
		m_End = newEnd;
	}
}

int CParallelogram::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
/************************************************************************/
void CParallelogram::AdjustFocusPoint()
{
	CAdjustPoint *connPoint = NULL;
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_TOP);
	connPoint->SetPoint(m_Start);
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_BOTTOM);
	connPoint->SetPoint(CPoint(m_Start.x, m_End.y));
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_TOP);
	connPoint->SetPoint(CPoint(m_End.x, m_Start.y));
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_BOTTOM);
	connPoint->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_RECT_CNT; i++)
	{
		connPoint = (CAdjustPoint *)m_Points.GetAt(i);
		connPoint->SetType(false);
	}

	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_TOP);
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_RIGHT);
	connPoint->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_BOTTOM);
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	connPoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_LEFT);
	connPoint->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

/************************************************************************/
/* ���ܣ����л�������                                                   */
/************************************************************************/
void CParallelogram::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
	}
	else
	{
		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
	}
	m_Points.Serialize(ar);
}