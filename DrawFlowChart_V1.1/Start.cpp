// Start.cpp: implementation of the CStart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "Start.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CStart, CObject, 1)

CStart::CStart()
{
	m_text = "¿ªÊ¼";
	m_Start = CPoint(100, 100);
	m_End = CPoint(200, 145);

	CConnectPoint *temp = NULL; 
	for(int i = 0; i < 8; i++)
	{
		temp = new CConnectPoint();
		m_Points.Add(temp);
	}
}

CStart::~CStart()
{

}

void CStart::Draw(CDC *pdc)
{
	AdjustFocusPoint();

	CPen *oldPen, newPen;
	if(m_IsMark)
	{
		newPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
		oldPen = pdc->SelectObject(&newPen);
	}

	pdc->RoundRect(CRect(m_Start, m_End), CPoint(35, 35));

	if(m_IsMark)
		pdc->SelectObject(oldPen);

	pdc->DrawText(m_text, CRect(m_Start+CPoint(10, 15), m_End+CPoint(-8, -8)), DT_CENTER);
}

void CStart::DrawFocus(CDC *pdc)
{
	CConnectPoint *temp = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    temp = (CConnectPoint *)m_Points.GetAt(i);
		temp->Draw(pdc);
	}
}

void CStart::Move(int cx, int cy)
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

bool CStart::IsEditable()
{
	return false;
}

bool CStart::IsIn(CPoint &pt)
{
	bool flag = false;
	CRect temp = CRect( m_Start, m_End );
	if(temp.PtInRect( pt ))
	{
		flag = true;
	}
	return flag;
}

bool CStart::IsOn(CConnectPoint *pt)
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

void CStart::AdjustFocusPoint()
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

void CStart::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_Start<<m_End<<m_text;
	}
	else
	{
		ar>>m_Start>>m_End>>m_text;
	}
}