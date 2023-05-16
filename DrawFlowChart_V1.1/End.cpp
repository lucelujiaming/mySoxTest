// End.cpp: implementation of the CEnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "End.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEnd, CObject, 1)
/************************************************************************/
/* ���ܣ�����������д������ʼ��ͽ����㡣���趨�����ӵ㡣               */
/************************************************************************/

CEnd::CEnd()
{
	m_text = "����";
	m_Start = CPoint(300, 300);
	m_End = CPoint(400, 345);

	CConnectPoint *temp = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		temp = new CConnectPoint();
		m_Points.Add(temp);
	}
}

CEnd::~CEnd()
{

}

/************************************************************************/
/* ���ܣ����ƺ�����������һ��Բ�Ǿ��κ���������֡�                     */
/************************************************************************/
void CEnd::Draw(CDC *pdc)
{
	AdjustFocusPoint();

	CPoint points[6];
	points[0].x = m_Start.x;
	points[0].y = (m_Start.y + m_End.y)/2;

	points[1].x = m_Start.x + 20;
	points[1].y = m_Start.y;

	points[2].x = m_End.x - 20;
	points[2].y = m_Start.y;

	points[3].x = m_End.x;
	points[3].y = (m_Start.y + m_End.y)/2;

	points[4].x = m_End.x - 20;
	points[4].y = m_End.y;

	points[5].x = m_Start.x + 20;
	points[5].y = m_End.y;

	CPen *oldPen, newPen;
	if(m_IsMark)
	{
		newPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
		oldPen = pdc->SelectObject(&newPen);
	}

	pdc->Polygon(points, 6);

	if(m_IsMark)
		pdc->SelectObject(oldPen);

	pdc->DrawText(m_text, CRect(m_Start+CPoint(10, 15), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CEnd::DrawFocus(CDC *pdc)
{
	CConnectPoint *temp = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    temp = (CConnectPoint *)m_Points.GetAt(i);
		temp->Draw(pdc);
	}
}

/************************************************************************/
/* ���ܣ� �ƶ���������                                                */
/************************************************************************/
void CEnd::Move(int cx, int cy)
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ��ж��Ƿ�ɱ༭��                                               */
/************************************************************************/
bool CEnd::IsEditable()
{
	return false;
}

/************************************************************************/
/* ���ܣ��ж��Ƿ���ͼԪ�����ڡ�                                         */
/************************************************************************/
bool CEnd::IsIn(CPoint &pt)
{
	bool flag = false;
	CRect temp = CRect( m_Start, m_End );
	if(temp.PtInRect( pt ))
	{
		flag = true;
	}
	return flag;
}

/************************************************************************/
/* ���ܣ� �ж��Ƿ���ͼԪ�߽��ϡ�                                        */
/************************************************************************/
bool CEnd::IsOn(CConnectPoint *pt)
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

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ӵ����ꡣ                         */
/************************************************************************/
void CEnd::AdjustFocusPoint()
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

/************************************************************************/
/* ���ܣ����л�������                                                   */
/************************************************************************/
void CEnd::Serialize(CArchive& ar)
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