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

/************************************************************************/
/* ���ܣ�����������д������ʼ��ͽ����㡣���趨�����ӵ㡣               */
/************************************************************************/
CStart::CStart()
{
	m_text = "��ʼ";
	m_Start = CPoint(100, 100);
	m_End = CPoint(200, 145);

	CConnectPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CConnectPoint();
		m_Points.Add(connPoint);
	}
}

CStart::~CStart()
{

}

/************************************************************************/
/* ���ܣ����ƺ�����������һ��Բ�Ǿ��κ���������֡�                     */
/************************************************************************/
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

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CStart::DrawFocus(CDC *pdc)
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->Draw(pdc);
	}
}

/************************************************************************/
/* ���ܣ� �ƶ���������                                                */
/************************************************************************/
void CStart::Move(int cx, int cy)
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ��ж��Ƿ�ɱ༭��                                               */
/************************************************************************/
bool CStart::IsEditable()
{
	return false;
}

/************************************************************************/
/* ���ܣ��ж��Ƿ���ͼԪ�����ڡ�                                         */
/************************************************************************/
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

/************************************************************************/
/* ���ܣ� �ж��Ƿ���ͼԪ�߽��ϡ�                                        */
/************************************************************************/
bool CStart::IsOn(CConnectPoint *pt)
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

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ӵ����ꡣ                         */
/************************************************************************/
void CStart::AdjustFocusPoint()
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

/************************************************************************/
/* ���ܣ����л�������                                                   */
/************************************************************************/
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