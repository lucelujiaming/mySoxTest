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
// IMPLEMENT_SERIAL(CControlFlow, CObject, 1)

CControlFlow::CControlFlow()
{
	CAdjustPoint *pStart = new CAdjustPoint();
	pStart->SetPoint(CPoint(0, 0));
	m_Points.push_back(pStart);

	m_IsCreateEnd = false;
	m_IsMark = false;

	m_Previous = NULL;
	m_Next = NULL;

	m_Start = CPoint(0, 0);
	m_End = CPoint(0, 0);
	m_iPreviousConnPointIdx = -1;
	m_iNextConnPointIdx = -1;
}

CControlFlow::~CControlFlow()
{

}


void CControlFlow::Draw( CDC *pdc )
{
	if(m_Points.size() < 2) return;

	// printAllPoints("CControlFlow::Draw");
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	if(pStart != NULL)
	{
		pdc->MoveTo(pStart->GetPoint());
	}

	CPen pe, *pOldPen;
	if(m_IsMark)
	{
        pe.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色）
        pOldPen=pdc-> SelectObject(&pe);     //把画笔选入DC，并保存原来画笔
	}

	for(int i = 1; i < m_Points.size(); i++)
	{
		CAdjustPoint *pNext = (CAdjustPoint*)m_Points[i];
		pdc->LineTo(pNext->GetPoint());
	}

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	DrawArrow(pdc);
	DrawSelectBorderArea(pdc);
}

void CControlFlow::DrawFocus(CDC *pdc)
{
	if(m_Points.size() < 2) return;

	// printAllPoints("CControlFlow::DrawFocus");
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		if(i == 0 || i == m_Points.size()-1) pConnPoint->SetType(false);
		pConnPoint->Draw(pdc);
	}
}

#define DRAW_FRAME
void CControlFlow::DrawSelectBorderArea( CDC *pdc )
{
#ifdef DRAW_FRAME
	// 画笔为虚线，线宽为1，颜色为紫色。
	CPen greenPen( PS_DOT, 1, RGB(255, 0, 128) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&greenPen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );


	bool flag = false;
	CPoint points[4];
	for(int i = 0; i < m_Points.size()-1; i++)
	{
		CRgn cr;

		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint connStart = pConnPoint->GetPoint();
		pConnPoint = (CAdjustPoint*)m_Points[i+1];
		CPoint connEnd = pConnPoint->GetPoint();

		long marginX = 0;
		long marginY = 0;
		// Use smaller margin for showing
		if(abs(connEnd.x - connStart.x) > abs(connEnd.y - connStart.y))
		{
			// marginY = 2 * (ADJUSTPOINT_POSITIVE_Y_MARGIN - 1); // 10;
			marginY = (ADJUSTPOINT_POSITIVE_Y_MARGIN - 1); // 10;
		}
		else
		{
			// marginX = 2 * (ADJUSTPOINT_POSITIVE_X_MARGIN - 1); // 10;
			marginX = (ADJUSTPOINT_POSITIVE_X_MARGIN - 1); // 10;
		}

		CPoint marginXY = CPoint(marginX, marginY);
		points[0] = connStart - marginXY;
		points[1] = connStart + marginXY;
		points[2] = connEnd + marginXY;
		points[3] = connEnd - marginXY;

		pdc->Polygon(points, 4);
	}
	
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);
#endif
}

void CControlFlow::Move(int cx, int cy)
{
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint newPoint = pConnPoint->GetPoint() + CPoint(cx, cy);
		pConnPoint->SetPoint(newPoint);
	}
}

void CControlFlow::AdjustSize(CPoint &pt)
{
	// printAllPoints("CControlFlow::AdjustSize Before");
	CAdjustPoint *pFocusConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint];
	if(pFocusConnPoint != NULL)
	{
		pFocusConnPoint->SetPoint(pt);
	}
	// printAllPoints("CControlFlow::AdjustSize");
}

void CControlFlow::SetStartPoint(CPoint &pt)
{
	if(m_Points.size() <= 0) return;

	// printAllPoints("CControlFlow::SetStartPoint Before");
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pStart->SetPoint(pt);
	// printAllPoints("CControlFlow::SetStartPoint");
}

void CControlFlow::SetEndPoint(CPoint &pt)
{
	CAdjustPoint *pNewPoint;
	if(!m_IsCreateEnd)
	{
		pNewPoint = new CAdjustPoint();
		pNewPoint->SetPoint(pt);
		// printAllPoints("CControlFlow::SetEndPoint(NotCreateEnd) Before");
		m_Points.insert(m_Points.end(), pNewPoint);
		// printAllPoints("CControlFlow::SetEndPoint(NotCreateEnd) After");
	}
	else
	{
		// printAllPoints("CControlFlow::SetEndPoint(CreateEnd) Before");
		// p = (CAdjustPoint*)m_Points.GetAt(0);
		pNewPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		pNewPoint->SetPoint(pt);
		// printAllPoints("CControlFlow::SetEndPoint(CreateEnd) After");
	}
}


void CControlFlow::SetLastPoint( CPoint &pt )
{
	CAdjustPoint *pLast;
	pLast = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pLast->SetPoint(pt);
	// printAllPoints("CControlFlow::SetLastPoint");

}

void CControlFlow::GetStartPoint(CPoint &pt)
{
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pt = pStart->GetPoint();
}

void CControlFlow::GetEndPoint(CPoint &pt)
{
	// CAdjustPoint *pEnd = (CAdjustPoint*)m_Points.GetAt(0);
	CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pt = pEnd->GetPoint();
}

void CControlFlow::SetPreviousGraph(CGraph *previousGraph)
{
	if(m_IsCreateEnd)
	{
		CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
		// m_iPreviousConnPointIdx = -1;
		int iConnPoint =  previousGraph->IsConnectOn(pStart);
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
		CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[m_Points.size()-1];
		// m_iNextConnPointIdx = -1;
		int iConnPoint =  nextGraph->IsConnectOn(pEnd);
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
	if(m_Points.size() < 1) return false;

	// printAllPoints("CControlFlow::IsIn Before");
	if(!m_IsCreateEnd)
	{
		CAdjustPoint *connPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		m_Points.pop_back();
		delete connPoint;
		m_IsCreateEnd = true;
		// m_Start和m_End对于折线来说，好像没有用。
		// connPoint = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
		// m_Start = connPoint->GetPoint();
		// connPoint = (CAdjustPoint*)m_Points.GetAt(0);
		// m_End = connPoint->GetPoint();
	}
	// printAllPoints("CControlFlow::IsIn After");

	bool flag = false;
	CPoint points[4];
	for(int i = 0; i < m_Points.size()-1; i++)
	{
		CRgn cr;

		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint connStart = pConnPoint->GetPoint();
		pConnPoint = (CAdjustPoint*)m_Points[i+1];
		CPoint connEnd = pConnPoint->GetPoint();

		long marginX = 0;
		long marginY = 0;
		if(abs(connEnd.x - connStart.x) > abs(connEnd.y - connStart.y))
		{
			marginY = 2 * (ADJUSTPOINT_POSITIVE_Y_MARGIN - 1); // 10;
		}
		else
		{
			marginX = 2 * (ADJUSTPOINT_POSITIVE_X_MARGIN - 1); // 10;
		}

		CPoint marginXY = CPoint(marginX, marginY);
		points[0] = connStart - marginXY;
		points[1] = connStart + marginXY;
		points[2] = connEnd + marginXY;
		points[3] = connEnd - marginXY;
		BOOL bRet = cr.CreatePolygonRgn(points, 4, WINDING);
		if(bRet && cr.PtInRegion(pt))
		{
			flag = true;
			break;
		}
		else if (bRet == FALSE)
		{
			printf("points = {(%d, %d), (%d, %d), (%d, %d), (%d, %d)}",
				points[0].x, points[0].y, points[1].x, points[1].y,
				points[2].x, points[2].y, points[3].x, points[3].y);
		}
	}

	return flag;
}

bool CControlFlow::IsOn(CPoint &pt)
{
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		if(pConnPoint->IsOn(pt))
		{
			m_FocusPoint = i;
			return true;
		}
	}

	m_FocusPoint = CCONNECTPOINT_INVALID_OPTION;
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
	if(m_Points.size() < 2) return;

	CAdjustPoint *pArrowPoint = (CAdjustPoint*)m_Points[m_Points.size()-2];
	CPoint Start = pArrowPoint->GetPoint();
	int flSx = Start.x;
	int flSy = Start.y;
	pArrowPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
	CPoint End = pArrowPoint->GetPoint();
	int flEx = End.x;
	int flEy = End.y;
	double flLength = 10;
	double flAngle = 40;

	if(GetDistance(flSx, flSy, flEx, flEy) == 0) 
	{
		TRACE("GetDistance == 0");
		return;
	}

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

void CControlFlow::SaveParamsToJSON(cJSON * objJSON)
{
//	if(ar.IsStoring())
//	{
//		ar<<m_FocusPoint<<m_IsCreateEnd;
//		ar<<m_iPreviousConnPointIdx<<m_iNextConnPointIdx;
//	}
//	else
//	{
//		ar>>m_FocusPoint>>m_IsCreateEnd;
//		ar>>m_iPreviousConnPointIdx>>m_iNextConnPointIdx;
//	}
	cJSON * jsonGraph = cJSON_CreateObject();
	cJSON_AddStringToObject(jsonGraph, "Type", GetTypeName());
	cJSON_AddNumberToObject(jsonGraph, "GraphSeq", getGraphSeq());
	
	cJSON_AddNumberToObject(jsonGraph, "FocusPoint", m_FocusPoint);
	cJSON_AddNumberToObject(jsonGraph, "IsCreateEnd", m_IsCreateEnd);

	// Save PreviousConnPoint and NextConnPoint
	cJSON_AddNumberToObject(jsonGraph, "PreviousGraphSeq", GetPreviousGraph()->getGraphSeq());
	cJSON_AddNumberToObject(jsonGraph, "PreviousConnPointIdx", m_iPreviousConnPointIdx);
	cJSON_AddNumberToObject(jsonGraph, "NextGraphSeq", GetNextgraph()->getGraphSeq());
	cJSON_AddNumberToObject(jsonGraph, "NextConnPointIdx", m_iNextConnPointIdx);
	// End of save PreviousConnPoint and NextConnPoint
	cJSON_AddItemToObject(objJSON, GetTypeName(), jsonGraph);
}

void CControlFlow::LoadParamsFromJSON(cJSON * objJSON)
{
}