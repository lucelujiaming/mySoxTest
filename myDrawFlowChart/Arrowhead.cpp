// Arrowhead.cpp: implementation of the CArrowhead class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "Arrowhead.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CArrowhead, CObject, 1)

CArrowhead::CArrowhead()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
		connPoint = new CConnectPoint();
		m_Points.Add(connPoint);
	}
	m_Previous = NULL;
	m_Next = NULL;
}

CArrowhead::~CArrowhead()
{

}

void CArrowhead::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色）
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}

	pdc->MoveTo(m_Start);
	pdc->LineTo(m_End);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	DrawArrow(pdc);
}

void CArrowhead::DrawFocus( CDC *pdc )
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->Draw(pdc);
	}
}

void CArrowhead::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CArrowhead::AdjustSize( CPoint &pt )
{
	switch(m_AdjustPoint)
	{
	// case 1: // 起点
	case CCONNECTPOINT_LINE_START:
		{
			m_Start = pt;
			break;
		}
	// case 2: // 终点
	case CCONNECTPOINT_LINE_END:
		{
			m_End = pt;
			break;
		}
	}
}

void CArrowhead::SetPreviousGraph(CGraph *previousGraph)
{
	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(0);
	int iConnPoint = previousGraph->IsConnectOn(p);
	if(iConnPoint >= 0)
	{
		m_Previous = previousGraph;
	}
	else if(m_Previous == previousGraph)
	{
		m_Previous = NULL;
	}
}
void CArrowhead::SetNextgraph(CGraph *nextGraph)
{
	CConnectPoint *p = (CConnectPoint*)m_Points.GetAt(1);
	int iConnPoint = nextGraph->IsConnectOn(p);
	if(iConnPoint >= 0)
	{
		m_Next = nextGraph;
	}
	else if(m_Next == nextGraph)
	{
		m_Next = NULL;
	}
}
CGraph* CArrowhead::GetPreviousGraph()
{
	return m_Next;
	//return m_Previous;
}
CGraph* CArrowhead::GetNextgraph()
{
	return m_Previous;
	//return m_Next;
}

bool CArrowhead::IsEditable()
{
	return false;
}
bool CArrowhead::IsControlFlow()
{
	return true;
}

bool CArrowhead::IsIn( CPoint &pt )
{
	bool flag = false;

	CPoint points[4];
	int marginX = 0;
	int marginY = 0;
	if(abs(m_End.x - m_Start.x) > abs(m_End.y - m_Start.y))
	{
		marginX = CCONNECTPOINT_POSITIVE_X_MARGIN;
	}
	else
	{
		marginY = CCONNECTPOINT_POSITIVE_Y_MARGIN;
	}

	CPoint marginXY = CPoint(marginX, marginY);
	points[0] = m_Start - marginXY;
	points[1] = m_Start + marginXY;
	points[2] = m_End + marginXY;
	points[3] = m_End - marginXY;

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

bool CArrowhead::IsOn( CPoint &pt )
{
	bool flag = false;
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt))
		{
			m_AdjustPoint = i; // 1+i;
		    flag = true;
			break;
		}
	}

	return flag;
}

int CArrowhead::GetAdjustPoint()
{
	return m_AdjustPoint;
}

double CArrowhead::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CArrowhead::DrawArrow( CDC *pdc )
{
	int flSx = m_Start.x;
	int flSy = m_Start.y;
	int flEx = m_End.x;
	int flEy = m_End.y;
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
	CPoint points[]={aPoint, bPoint, m_End};

	CBrush *pBrush = new CBrush;
	CBrush *oldBrush;
	pBrush->CreateSolidBrush(RGB(0,0,0));

	oldBrush = pdc->SelectObject(pBrush);
	pdc->Polygon(points, 3);
	pdc->SelectObject(oldBrush);
}

int CArrowhead::IsConnectOn(CConnectPoint *pt)
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt->GetPoint()))
		{
			pt->SetPoint(connPoint->GetPoint());
		    return i;
		}
	}
	return CCONNECTPOINT_INVALID_OPTION;
}

void CArrowhead::AdjustFocusPoint()
{
	CConnectPoint *connPoint = NULL;
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_LINE_START);
	connPoint->SetPoint(m_Start);
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_LINE_END);
	connPoint->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
		connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->SetType(false);
	}
}

void CArrowhead::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
	}
	else
	{
		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
	}
}