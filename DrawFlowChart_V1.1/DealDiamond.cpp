// DealDiamond.cpp: implementation of the CDealDiamond class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "DealDiamond.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDealDiamond, CObject, 1)

CDealDiamond::CDealDiamond()
{
	m_Start = CPoint(0, 0);
	m_End = CPoint(0, 0);
	m_AdjustPoint = -1;

	CConnectPoint *temp = NULL; 
	for(int i = 0; i < 8; i++)
	{
		temp = new CConnectPoint();
		m_Points.Add(temp);
	}
}

CDealDiamond::~CDealDiamond()
{

}

void CDealDiamond::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPoint points[4];
	double tempx = (m_End.x - m_Start.x) * 0.15;

	points[0].x = m_Start.x + tempx;
	points[0].y = m_Start.y;

	points[1].x = m_End.x;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - tempx;
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

void CDealDiamond::DrawFocus( CDC *pdc )
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

void CDealDiamond::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CDealDiamond::AdjustSize( CPoint &pt )
{
	CPoint temp1 = CPoint(m_Start.x, m_End.y);
	CPoint temp2 = CPoint(m_End.x, m_Start.y);

	switch(m_AdjustPoint)
	{
		case 1: // вСио╫г
			{
				m_Start = pt;
				break;
			}
		case 2: // вСоб╫г
			{
				m_Start = CPoint(pt.x, temp2.y);
				m_End = CPoint(temp2.x, pt.y);
				break;
			}
		case 3: // срио╫г
			{
				m_Start = CPoint(temp1.x, pt.y);
				m_End = CPoint(pt.x, temp1.y);
				break;
			}
		case 4: // сроб╫г
			{
				m_End = pt;
				break;
			}
		case 5:
			{
				m_Start.y = pt.y;
				break;
			}
		case 6:
			{
				m_End.x = pt.x;
				break;
			}
		case 7:
			{
				m_End.y = pt.y;
				break;
			}
		case 8:
			{
				m_Start.x = pt.x;
				break;
			}
	}
}

bool CDealDiamond::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;

	CPoint points[4];
	float tempx = (m_End.x - m_Start.x) * 0.25;

	points[0].x = m_Start.x + tempx;
	points[0].y = m_Start.y;

	points[1].x = m_End.x;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - tempx;
	points[2].y = m_End.y;

	points[3].x = m_Start.x;
	points[3].y = m_End.y;

	CRgn cr;
	cr.CreatePolygonRgn(points, 4, ALTERNATE);
	if(cr.PtInRegion( pt ))
	{
		flag = true;
		m_AdjustPoint = -1;
	}
	return flag;
}

bool CDealDiamond::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CConnectPoint *temp = NULL;
	for(int i = 0; i < 8; i++)
	{
	    temp = (CConnectPoint *)m_Points.GetAt(i);
		if(temp->IsOn(pt))
		{
			m_AdjustPoint = 1+i;
		    flag = true;
		}
	}

	return flag;
}

bool CDealDiamond::IsOn(CConnectPoint *pt)
{
	CConnectPoint *temp = NULL;
	for(int i = 0; i < 8; i++)
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

void CDealDiamond::AdjustStartAndEnd()
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

int CDealDiamond::GetAdjustPoint()
{
	return m_AdjustPoint;
}

void CDealDiamond::AdjustFocusPoint()
{
	CConnectPoint *temp = NULL;
	temp = (CConnectPoint *)m_Points.GetAt(0);
	temp->SetPoint(m_Start);
	temp = (CConnectPoint *)m_Points.GetAt(1);
	temp->SetPoint(CPoint(m_Start.x, m_End.y));
	temp = (CConnectPoint *)m_Points.GetAt(2);
	temp->SetPoint(CPoint(m_End.x, m_Start.y));
	temp = (CConnectPoint *)m_Points.GetAt(3);
	temp->SetPoint(m_End);
	for(int i = 0; i < 4; i++)
	{
		temp = (CConnectPoint *)m_Points.GetAt(i);
		temp->SetType(false);
	}

	temp = (CConnectPoint *)m_Points.GetAt(4);
	temp->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	temp = (CConnectPoint *)m_Points.GetAt(5);
	temp->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	temp = (CConnectPoint *)m_Points.GetAt(6);
	temp->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	temp = (CConnectPoint *)m_Points.GetAt(7);
	temp->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

void CDealDiamond::Serialize(CArchive& ar)
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