// PolygonalLine.cpp: implementation of the CPolygonalLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "PolygonalLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPolygonalLine, CObject, 1)

CPolygonalLine::CPolygonalLine()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_POLYGONALLINE_MAX; i++)
	{
		connPoint = new CConnectPoint();
		m_Points.Add(connPoint);
	}
	m_Previous = NULL;
	m_Next = NULL;
	m_fBendPercent = 0.5;
	m_iBendTimes = 0;
}

CPolygonalLine::~CPolygonalLine()
{

}

void CPolygonalLine::Draw( CDC *pdc )
{
	printInfomation("Draw");
	AdjustFocusPoint();

	CPen p, *pOldPen;
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色）
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	// CConnectPoint *connMiddlePoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_MIDDLE);
	// CPoint pFirstBendPoint  = CPoint(connMiddlePoint->GetPoint().x, m_Start.y);
	// CPoint pSecondBendPoint = CPoint(connMiddlePoint->GetPoint().x, m_End.y);

	pdc->MoveTo(m_Start);
	pdc->LineTo(m_StartStub.ptPosition);
	pdc->LineTo(m_objOrthogonalWire.m_ptBend[0]);
	if(m_iBendTimes == 2)
	{
		pdc->LineTo(m_objOrthogonalWire.m_ptBend[1]);

	}
	pdc->LineTo(m_EndStub.ptPosition);
	pdc->LineTo(m_End);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	DrawArrow(pdc);

	DrawSelectBorderArea(pdc);
}

void CPolygonalLine::printInfomation(CString strCaption)
{
	CString strStatusBar;
	strStatusBar.Format(_T("(%s) - m_iPreviousConnPointIdx =  %d and m_iNextConnPointIdx = %d "),
		strCaption, m_iPreviousConnPointIdx, m_iNextConnPointIdx);
	strStatusBar += "\r\n";
	TRACE(strStatusBar);
}

#define DRAW_FRAME
void CPolygonalLine::DrawFocus( CDC *pdc )
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->Draw(pdc);
	}
}

void CPolygonalLine::DrawSelectBorderArea( CDC *pdc )
{
#ifdef DRAW_FRAME
	// 画笔为虚线，线宽为1，颜色为黑色。
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );

	CPoint points[8];
	if (m_iBendTimes == 1)
	{
		CConnectPoint *pMiddleConnPoint = (CConnectPoint*)m_Points.GetAt(1);
		CPoint        middlePoint       = pMiddleConnPoint->GetPoint();

		if (m_StartStub.direction == ORTHOGONALWIRE_SOUTH)
		{
			points[0] = m_Start                           + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[1] = m_Start                           + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[2] = middlePoint                       + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[3] = m_End                             + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[4] = m_End                             + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[5] = middlePoint                       + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		}
		else if (m_StartStub.direction == ORTHOGONALWIRE_NORTH)
		{
			points[0] = m_Start                           + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[1] = m_Start                           + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[2] = middlePoint                       + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[3] = m_End                             + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[4] = m_End                             + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[5] = middlePoint                       + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		}
		else if (m_StartStub.direction == ORTHOGONALWIRE_WEST)
		{
			points[0] = m_Start                           + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[1] = middlePoint                       + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[2] = m_End                             + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[3] = m_End                             + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[4] = middlePoint                       + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[5] = m_Start                           + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		}
		else if (m_StartStub.direction == ORTHOGONALWIRE_EAST)
		{
			points[0] = m_Start                           + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[1] = CPoint(m_End.x, m_Start.y)        + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[2] = m_End                             + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[3] = m_End                             + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[4] = CPoint(m_End.x, m_Start.y)        + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[5] = m_Start                           + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		}
		pdc->Polygon(points, 6);
		TRACE("m_iBendTimes == 1");
	}
	else if (m_iBendTimes == 2)
	{
		CConnectPoint *pMiddleConnPoint = (CConnectPoint*)m_Points.GetAt(1);
		CPoint        middlePoint       = pMiddleConnPoint->GetPoint();

		points[0] = m_Start                           + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[1] = CPoint(middlePoint.x, m_Start.y) + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[2] = CPoint(middlePoint.x,   m_End.y) + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[3] = m_End                            + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[4] = m_End                            + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		points[5] = CPoint(middlePoint.x,   m_End.y) + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		points[6] = CPoint(middlePoint.x, m_Start.y) + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		points[7] = m_Start                           + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		
		pdc->Polygon(points, 8);
		TRACE("m_iBendTimes == 2");
	}
	
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);
#endif
}

void CPolygonalLine::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CPolygonalLine::AdjustSize( CPoint &pt )
{
	switch(m_AdjustPoint)
	{
	// case 1: // 起点
	case CCONNECTPOINT_POLYGONALLINE_START:
		{
			m_Start = pt;
			break;
		}
	// case 2: // 中间点
	case CCONNECTPOINT_POLYGONALLINE_MIDDLE:
		{
			// m_End = pt;
			if (m_iBendTimes == 2)
			{
				m_fBendPercent = (float)(pt.x - m_Start.x) / (float)(m_End.x - m_Start.x);
			}
			break;
		}
	// case 3: // 终点
	case CCONNECTPOINT_POLYGONALLINE_END:
		{
			m_End = pt;
			break;
		}
	}
}

void CPolygonalLine::SetPreviousGraph(CGraph *previousGraph)
{
	CConnectPoint *pStart = (CConnectPoint*)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_START);
	int iConnPoint = previousGraph->IsConnectOn(pStart);
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
void CPolygonalLine::SetNextgraph(CGraph *nextGraph)
{
	CConnectPoint *pEnd = (CConnectPoint*)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_END);
	int iConnPoint = nextGraph->IsConnectOn(pEnd);
	if(iConnPoint >= 0)
	{
		m_Next = nextGraph;
		printInfomation("SetNextgraph");
		m_iNextConnPointIdx = iConnPoint;
		printInfomation("SetNextgraph");
	}
	else if(m_Next == nextGraph)
	{
		m_Next = NULL;
	}
}
CGraph* CPolygonalLine::GetPreviousGraph()
{
	//return m_Next;
	return m_Previous;
}
CGraph* CPolygonalLine::GetNextgraph()
{
	//return m_Previous;
	return m_Next;
}

bool CPolygonalLine::IsEditable()
{
	return false;
}
bool CPolygonalLine::IsControlFlow()
{
	return true;
}

bool CPolygonalLine::IsIn( CPoint &pt )
{
	bool flag = false;

	CPoint points[8];
	int marginX = 0;
	int marginY = 0;
	marginX = marginY = CCONNECTPOINT_POSITIVE_X_MARGIN;
		
	if (m_iBendTimes == 1)
	{
		CConnectPoint *pMiddleConnPoint = (CConnectPoint*)m_Points.GetAt(1);
		CPoint        middlePoint       = pMiddleConnPoint->GetPoint();

		if (m_StartStub.direction == ORTHOGONALWIRE_SOUTH)
		{
			points[0] = m_Start                           + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[1] = m_Start                           + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[2] = middlePoint                       + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[3] = m_End                             + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[4] = m_End                             + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[5] = middlePoint                       + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		}
		else if (m_StartStub.direction == ORTHOGONALWIRE_NORTH)
		{
			points[0] = m_Start                           + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[1] = m_Start                           + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[2] = middlePoint                       + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[3] = m_End                             + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[4] = m_End                             + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[5] = middlePoint                       + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		}
		else if (m_StartStub.direction == ORTHOGONALWIRE_WEST)
		{
			points[0] = m_Start                           + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[1] = middlePoint                       + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[2] = m_End                             + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[3] = m_End                             + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[4] = middlePoint                       + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[5] = m_Start                           + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		}
		else if (m_StartStub.direction == ORTHOGONALWIRE_EAST)
		{
			points[0] = m_Start                           + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[1] = CPoint(m_End.x, m_Start.y)        + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
			points[2] = m_End                             + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, 0);
			points[3] = m_End                             + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, 0);
			points[4] = CPoint(m_End.x, m_Start.y)        + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
			points[5] = m_Start                           + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		}

		CRgn cr;
		BOOL bRet = cr.CreatePolygonRgn(points, 6, ALTERNATE);
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
		TRACE("m_iBendTimes == 1");
	}
	else if (m_iBendTimes == 2)
	{
		CConnectPoint *pMiddleConnPoint = (CConnectPoint*)m_Points.GetAt(1);
		CPoint        middlePoint       = pMiddleConnPoint->GetPoint();

		points[0] = m_Start                           + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[1] = CPoint(middlePoint.x, m_Start.y) + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[2] = CPoint(middlePoint.x,   m_End.y) + CPoint(CCONNECTPOINT_POSITIVE_X_MARGIN, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[3] = m_End                            + CPoint(0, CCONNECTPOINT_NEGATIVE_Y_MARGIN);
		points[4] = m_End                            + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		points[5] = CPoint(middlePoint.x,   m_End.y) + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		points[6] = CPoint(middlePoint.x, m_Start.y) + CPoint(CCONNECTPOINT_NEGATIVE_X_MARGIN, CCONNECTPOINT_POSITIVE_Y_MARGIN);
		points[7] = m_Start                           + CPoint(0, CCONNECTPOINT_POSITIVE_Y_MARGIN);

		CRgn cr;
		BOOL bRet = cr.CreatePolygonRgn(points, 8, ALTERNATE);
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
	}
	else 
	{
		CString strStatusBar;
		strStatusBar.Format(_T("m_iBendTimes =  %d. "),
			 m_iBendTimes);
		strStatusBar += "\r\n";
		TRACE(strStatusBar);
	}		

	return flag;
}

bool CPolygonalLine::IsOn( CPoint &pt )
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

int CPolygonalLine::GetAdjustPoint()
{
	return m_AdjustPoint;
}

double CPolygonalLine::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CPolygonalLine::DrawArrow( CDC *pdc )
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

int CPolygonalLine::IsConnectOn(CConnectPoint *pt)
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

CWirePoint CPolygonalLine::calculateStartLineStub(CPoint rootPoint, int iConnectPointIdx)
{
	CWirePoint stubPoint;
	if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_TOP)
	{
		stubPoint.ptPosition = rootPoint - CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.direction  = ORTHOGONALWIRE_NORTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_RIGHT)
	{
		stubPoint.ptPosition = rootPoint + CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.direction  = ORTHOGONALWIRE_EAST;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_BOTTOM)
	{
		stubPoint.ptPosition = rootPoint + CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.direction  = ORTHOGONALWIRE_SOUTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_LEFT)
	{
		stubPoint.ptPosition = rootPoint - CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.direction  = ORTHOGONALWIRE_WEST;
	}
	return stubPoint;
}

CWirePoint CPolygonalLine::calculateEndLineStub(CPoint rootPoint, int iConnectPointIdx)
{
	CWirePoint stubPoint;
	if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_TOP)
	{
		stubPoint.ptPosition = rootPoint - CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.direction  = ORTHOGONALWIRE_SOUTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_RIGHT)
	{
		stubPoint.ptPosition = rootPoint + CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.direction  = ORTHOGONALWIRE_WEST;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_BOTTOM)
	{
		stubPoint.ptPosition = rootPoint + CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.direction  = ORTHOGONALWIRE_NORTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_LEFT)
	{
		stubPoint.ptPosition = rootPoint - CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.direction  = ORTHOGONALWIRE_EAST;
	}
	return stubPoint;
}

void CPolygonalLine::AdjustFocusPoint()
{
	m_iBendTimes = 0;
	CConnectPoint *connPoint = NULL;
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_START);
	connPoint->SetPoint(m_Start);
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_END);
	connPoint->SetPoint(m_End);
	// Calc middle point
	if((m_iPreviousConnPointIdx == -1) || (m_iNextConnPointIdx == -1))
		return;

	m_StartStub = calculateStartLineStub(m_Start, m_iPreviousConnPointIdx);
	m_EndStub   = calculateEndLineStub(m_End, m_iNextConnPointIdx);
	
	m_iBendTimes = m_objOrthogonalWire.calculateOrthogonalWire(m_StartStub, m_EndStub);

	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_MIDDLE);
	if (m_iBendTimes == 1)
	{
		connPoint->SetPoint(m_objOrthogonalWire.m_ptBend[0]);
	}
	else if (m_iBendTimes == 2)
	{
		CPoint middlePoint = CPoint(
			(m_objOrthogonalWire.m_ptBend[0].x + m_objOrthogonalWire.m_ptBend[1].x) / 2,
			(m_objOrthogonalWire.m_ptBend[0].y + m_objOrthogonalWire.m_ptBend[1].y) / 2);
		connPoint->SetPoint(middlePoint);
	}
	
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
		connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->SetType(false);
	}
}

void CPolygonalLine::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
		ar<<m_iPreviousConnPointIdx<<m_iNextConnPointIdx;
	}
	else
	{
		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
		ar>>m_iPreviousConnPointIdx>>m_iNextConnPointIdx;
	}
}