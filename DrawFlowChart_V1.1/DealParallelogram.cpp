// DealParallelogram.cpp: implementation of the CDealParallelogram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "DealParallelogram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDealParallelogram, CObject, 1)

CDealParallelogram::CDealParallelogram()
{
	m_Start = CPoint(0, 0);
	m_End = CPoint(0, 0);
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CConnectPoint *temp = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		temp = new CConnectPoint();
		m_Points.Add(temp);
	}
}

CDealParallelogram::~CDealParallelogram()
{

}

void CDealParallelogram::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPoint points[4];
	long tempX = (long)((m_End.x - m_Start.x) * 0.15);

	points[0].x = m_Start.x + tempX;
	points[0].y = m_Start.y;

	points[1].x = m_End.x;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - tempX;
	points[2].y = m_End.y;

	points[3].x = m_Start.x;
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

void CDealParallelogram::DrawFocus( CDC *pdc )
{
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );
		
	pdc->Rectangle( CRect(m_Start, m_End) );
		
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);

	CConnectPoint *temp = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    temp = (CConnectPoint *)m_Points.GetAt(i);
		temp->Draw(pdc);
	}
}

void CDealParallelogram::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CDealParallelogram::AdjustSize( CPoint &pt )
{
	CPoint temp1 = CPoint(m_Start.x, m_End.y);
	CPoint temp2 = CPoint(m_End.x, m_Start.y);

	switch(m_AdjustPoint)
	{
	// case 1: // вСио╫г
	case CCONNECTPOINT_RECT_LEFT_TOP:
		{
			m_Start = pt;
			break;
		}
	// case 2: // вСоб╫г
	case CCONNECTPOINT_RECT_LEFT_BOTTOM:
		{
			m_Start = CPoint(pt.x, temp2.y);
			m_End = CPoint(temp2.x, pt.y);
			break;
		}
	// case 3: // срио╫г
	case CCONNECTPOINT_RECT_RIGHT_TOP:
		{
			m_Start = CPoint(temp1.x, pt.y);
			m_End = CPoint(pt.x, temp1.y);
			break;
		}
	// case 4: // сроб╫г
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

bool CDealParallelogram::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;

	CPoint points[4];
	long tempX = (long)((m_End.x - m_Start.x) * 0.25);

	points[0].x = m_Start.x + tempX;
	points[0].y = m_Start.y;

	points[1].x = m_End.x;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - tempX;
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
		printf("points = {(%d, %d), (%d, %d), (%d, %d), (%d, %d)}", 
			points[0].x, points[0].y, points[1].x, points[1].y, 
			points[2].x, points[2].y, points[3].x, points[3].y);
	}
	return flag;
}

bool CDealParallelogram::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CConnectPoint *temp = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    temp = (CConnectPoint *)m_Points.GetAt(i);
		if(temp->IsOn(pt))
		{
			m_AdjustPoint = i; // 1+i;
		    flag = true;
			break;
		}
	}

	return flag;
}

bool CDealParallelogram::IsOn(CConnectPoint *pt)
{
	CConnectPoint *temp = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    temp = (CConnectPoint *)m_Points.GetAt(i);
		if(temp->IsOn(pt->GetPoint()))
		{
			pt->SetPoint(temp->GetPoint());
		    return true;
		}
	}
	return false;
}

void CDealParallelogram::AdjustStartAndEnd()
{
	CPoint temp1, temp2;
	if((m_End.x < m_Start.x) && (m_End.y < m_Start.y))
	{
		temp1 = m_Start;
		m_Start = m_End;
		m_End = temp1;
	}
	else if(!((m_End.x > m_Start.x) && (m_End.y > m_Start.y)))
	{
		temp1 = CPoint( m_End.x, m_Start.y );
		temp2 = CPoint( m_Start.x, m_End.y );
		m_Start = temp1;
		m_End = temp2;
	}
}

int CDealParallelogram::GetAdjustPoint()
{
	return m_AdjustPoint;
}

void CDealParallelogram::AdjustFocusPoint()
{
	CConnectPoint *temp = NULL;
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_TOP);
	temp->SetPoint(m_Start);
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_BOTTOM);
	temp->SetPoint(CPoint(m_Start.x, m_End.y));
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_TOP);
	temp->SetPoint(CPoint(m_End.x, m_Start.y));
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_BOTTOM);
	temp->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_RECT_CNT; i++)
	{
		temp = (CConnectPoint *)m_Points.GetAt(i);
		temp->SetType(false);
	}

	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_TOP);
	temp->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_RIGHT);
	temp->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_BOTTOM);
	temp->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	temp = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_LEFT);
	temp->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

void CDealParallelogram::Serialize(CArchive& ar)
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