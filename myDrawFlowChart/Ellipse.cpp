// Ellipse.cpp: implementation of the CEllipse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "Ellipse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEllipse, CObject, 1)

CEllipse::CEllipse()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CConnectPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CConnectPoint();
		m_Points.Add(connPoint);
	}
}

CEllipse::~CEllipse()
{

}

void CEllipse::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}

	pdc->Ellipse( CRect(m_Start, m_End) );

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

void CEllipse::DrawFocus( CDC *pdc )
{
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );
		
	pdc->Rectangle( CRect(m_Start, m_End) );
		
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);

	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->Draw(pdc);
	}
}

void CEllipse::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CEllipse::AdjustSize( CPoint &pt )
{
	switch(m_AdjustPoint)
	{
	// case 1:  // 左上角
	case CCONNECTPOINT_RECT_LEFT_TOP:
		{
			m_Start = pt;
			break;
		}
	// case 2:  // 左下角
	case CCONNECTPOINT_RECT_LEFT_BOTTOM:
		{
			m_Start.x = pt.x;
			m_End.y = pt.y;
			break;
		}
	// case 3:  // 右上角
	case CCONNECTPOINT_RECT_RIGHT_TOP:
		{
			m_Start.y = pt.y;
			m_End.x = pt.x;
			break;
		}
	// case 4:  // 右下角
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

bool CEllipse::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;

	CRgn cr;
	BOOL bRet = cr.CreateEllipticRgn( m_Start.x, m_Start.y, m_End.x, m_End.y );
	if(bRet && cr.PtInRegion( pt ))
	{
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	else if (bRet == FALSE)
	{
		printf("m_Start/m_End = {(%d, %d), (%d, %d)}", 
			m_Start.x, m_Start.y, m_End.x, m_End.y);
	}
	return flag;
}

bool CEllipse::IsOn(CConnectPoint *pt)
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt->GetPoint()))
		{
			pt->SetPoint(connPoint->GetPoint());
		    return true;
		}
	}
	return false;
}

bool CEllipse::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CPoint temp1 = CPoint( m_Start.x, m_End.y );
	CPoint temp2 = CPoint(m_End.x, m_Start.y);

	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt))
		{
			// if(i == CCONNECTPOINT_RECT_LEFT_BOTTOM || i == CCONNECTPOINT_RECT_RIGHT_TOP)
			// {
			//	m_Start = temp1;
			//	m_End = temp2;
			// }
			m_AdjustPoint = i; // 1+i;
		    flag = true;
			break;
		}
	}
	return flag;
}

void CEllipse::AdjustStartAndEnd()
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

int CEllipse::GetAdjustPoint()
{
	return m_AdjustPoint;
}

void CEllipse::AdjustFocusPoint()
{
	CConnectPoint *connPoint = NULL;
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_TOP);
	connPoint->SetPoint(m_Start);
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_BOTTOM);
	connPoint->SetPoint(CPoint(m_Start.x, m_End.y));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_TOP);
	connPoint->SetPoint(CPoint(m_End.x, m_Start.y));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_BOTTOM);
	connPoint->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_RECT_CNT; i++)
	{
		connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->SetType(false);
	}

	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_TOP);
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_RIGHT);
	connPoint->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_BOTTOM);
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_LEFT);
	connPoint->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

void CEllipse::Serialize(CArchive& ar)
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