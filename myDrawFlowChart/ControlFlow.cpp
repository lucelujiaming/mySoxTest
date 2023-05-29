// ControlFlow.cpp: implementation of the CControlFlow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "ControlFlow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CControlFlow, CObject, 1)

CControlFlow::CControlFlow()
{
	CConnectPoint *p = new CConnectPoint();
	p->SetPoint(CPoint(0, 0));
	m_Points.Add(p);

	m_IsCreateEnd = false;
	m_IsMark = false;

	m_Previous = NULL;
	m_Next = NULL;

	m_Start = CPoint(0, 0);
	m_End = CPoint(0, 0);
}

CControlFlow::~CControlFlow()
{

}

void CControlFlow::Draw( CDC *pdc )
{
	if(m_Points.GetSize() < 2) return;

	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(0);
	if(p != NULL)
	{
		pdc->MoveTo(p->GetPoint());
	}

	CPen pe, *pOldPen;
	if(m_IsMark)
	{
        pe.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色）
        pOldPen=pdc-> SelectObject(&pe);     //把画笔选入DC，并保存原来画笔
	}

	for(int i = 1; i < m_Points.GetSize(); i++)
	{
		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(i);
		pdc->LineTo(p->GetPoint());
	}

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	DrawArrow(pdc);
}

void CControlFlow::DrawFocus(CDC *pdc)
{
	if(m_Points.GetSize() < 2) return;

	for(int i = 0; i < m_Points.GetSize(); i++)
	{
		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(i);
		if(i == 0 || i == m_Points.GetSize()-1) p->SetType(false);
		p->Draw(pdc);
	}
}

void CControlFlow::Move(int cx, int cy)
{
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(i);
		CPoint temp = p->GetPoint() + CPoint(cx, cy);
		p->SetPoint(temp);
	}
}

void CControlFlow::AdjustSize(CPoint &pt)
{
	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(m_FocusPoint);
	if(p != NULL)
	{
		p->SetPoint(pt);
	}
}

void CControlFlow::SetStartPoint(CPoint &pt)
{
	if(m_Points.GetSize() <= 0) return;

	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-1);
	p->SetPoint(pt);
}

void CControlFlow::SetEndPoint(CPoint &pt)
{
	CConnectPoint *p;
	if(!m_IsCreateEnd)
	{
		p = new CConnectPoint();
		p->SetPoint(pt);
		m_Points.InsertAt(m_Points.GetSize()-1, p);
	}
	else
	{
		p = (CConnectPoint*)m_Points.GetAt(0);
		p->SetPoint(pt);
	}
}

void CControlFlow::GetStartPoint(CPoint &pt)
{
	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-1);
	pt = p->GetPoint();
}

void CControlFlow::GetEndPoint(CPoint &pt)
{
	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(0);
	pt = p->GetPoint();
}

void CControlFlow::SetPreviousGraph(CGraph *previousGraph)
{
	if(m_IsCreateEnd)
	{
		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(0);
		// m_iPreviousConnPointIdx = -1;
		int iConnPoint =  previousGraph->IsConnectOn(p);
		if(iConnPoint >= 0)
		{
			m_Previous = previousGraph;
			m_iPreviousConnPointIdx = iConnPoint;
		}
		else if(m_Previous == previousGraph)
		{
			m_Previous = NULL;
		}
	}
}

void CControlFlow::SetNextgraph(CGraph *nextGraph)
{
	if(m_IsCreateEnd)
	{
		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-1);
		// m_iNextConnPointIdx = -1;
		int iConnPoint =  nextGraph->IsConnectOn(p);
		if(iConnPoint >= 0)
		{
			m_Next = nextGraph;
			m_iNextConnPointIdx = iConnPoint;
		}
		else if(m_Next == nextGraph)
		{
			m_Next = NULL;
		}
	}
}

CGraph* CControlFlow::GetPreviousGraph()
{
	return m_Previous;
}

CGraph* CControlFlow::GetNextgraph()
{
	return m_Next;
}

bool CControlFlow::IsEditable()
{
	return false;
}

bool CControlFlow::IsControlFlow()
{
	return true;
}

bool CControlFlow::IsIn(CPoint &pt)
{
	if(m_Points.GetSize() < 1) return false;

	if(!m_IsCreateEnd)
	{
		CConnectPoint *connPoint = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-1);
		m_Points.RemoveAt(m_Points.GetSize()-1);
		delete connPoint;
		m_IsCreateEnd = true;
		// m_Start和m_End对于这项来说，好像没有用。
		connPoint = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-1);
		// m_Start = connPoint->GetPoint();
		connPoint = (CConnectPoint*)m_Points.GetAt(0);
		// m_End = connPoint->GetPoint();
	}

	bool flag = false;
	CPoint tempPs[4];
	for(int i = 0; i < m_Points.GetSize()-1; i++)
	{
		CRgn cr;

		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(i);
		CPoint tempStart = p->GetPoint();
		p = (CConnectPoint*)m_Points.GetAt(i+1);
		CPoint tempEnd = p->GetPoint();

		long marginX = 0;
		long marginY = 0;
		if(abs(tempEnd.x - tempStart.x) > abs(tempEnd.y - tempStart.y))
		{
			marginY = 2 * (CCONNECTPOINT_POSITIVE_Y_MARGIN - 1); // 10;
		}
		else
		{
			marginX = 2 * (CCONNECTPOINT_POSITIVE_X_MARGIN - 1); // 10;
		}

		CPoint marginXY = CPoint(marginX, marginY);
		tempPs[0] = tempStart - marginXY;
		tempPs[1] = tempStart + marginXY;
		tempPs[2] = tempEnd + marginXY;
		tempPs[3] = tempEnd - marginXY;
		BOOL bRet = cr.CreatePolygonRgn(tempPs, 4, WINDING);
		if(bRet && cr.PtInRegion(pt))
		{
			flag = true;
			break;
		}
		else if (bRet == FALSE)
		{
			printf("tempPs = {(%d, %d), (%d, %d), (%d, %d), (%d, %d)}",
				tempPs[0].x, tempPs[0].y, tempPs[1].x, tempPs[1].y,
				tempPs[2].x, tempPs[2].y, tempPs[3].x, tempPs[3].y);
		}
	}

	return flag;
}

bool CControlFlow::IsOn(CPoint &pt)
{
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
		CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(i);
		if(p->IsOn(pt))
		{
			m_FocusPoint = i;
			return true;
		}
	}

	m_FocusPoint = -1;
	return false;
}

double CControlFlow::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CControlFlow::DrawArrow( CDC *pdc )
{
	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-2);
	CPoint Start = p->GetPoint();
	int flSx = Start.x;
	int flSy = Start.y;
	p = (CConnectPoint*)m_Points.GetAt(m_Points.GetSize()-1);
	CPoint End = p->GetPoint();
	int flEx = End.x;
	int flEy = End.y;
	double flLength = 10;
	double flAngle = 40;

	if(GetDistance(flSx, flSy, flEx, flEy) == 0) return;

	double tmpX = flEx + (flSx - flEx) * flLength/GetDistance(flSx, flSy, flEx, flEy);
	double tmpY = flEy + (flSy - flEy) * flLength/GetDistance(flSx, flSy, flEx, flEy);
	double fl1X = (tmpX - flEx) * cos(-flAngle/2) - (tmpY - flEy) * sin(-flAngle/2) + flEx;
	double fl1Y = (tmpY - flEy) * cos(-flAngle/2) + (tmpX - flEx) * sin(-flAngle/2) + flEy;
	double fl2X = (tmpX - flEx) * cos(flAngle/2) - (tmpY - flEy) * sin(flAngle/2) + flEx;
	double fl2Y = (tmpY - flEy) * cos(flAngle/2) + (tmpX - flEx) * sin(flAngle/2) + flEy;
	CPoint aPoint((int)fl1X, (int)fl1Y);
	CPoint bPoint((int)fl2X, (int)fl2Y);
	CPoint points[]={aPoint, bPoint, End};

	CBrush *pBrush = new CBrush;
	CBrush *oldBrush;
	pBrush->CreateSolidBrush(RGB(0,0,0));

	oldBrush = pdc->SelectObject(pBrush);
	pdc->Polygon(points, 3);
	pdc->SelectObject(oldBrush);
}

void CControlFlow::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_FocusPoint<<m_IsCreateEnd;
	}
	else
	{
		ar>>m_FocusPoint>>m_IsCreateEnd;
	}

	m_Points.Serialize(ar);
}