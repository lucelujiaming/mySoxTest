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
// IMPLEMENT_SERIAL(CPolygonalLine, CObject, 1)

CPolygonalLine::CPolygonalLine()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_POLYGONALLINE_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
	m_Previous = NULL;
	m_Next = NULL;
	m_iBendTimes = 0;
	m_iPreviousConnPointIdx = -1;
	m_iNextConnPointIdx = -1;
}

CPolygonalLine::~CPolygonalLine()
{

}

void CPolygonalLine::Draw( CDC *pdc )
{
	// printInfomation("Draw");
	AdjustFocusPoint();

	CPen p, *pOldPen;
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色）
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	// CAdjustPoint *connMiddlePoint = (CAdjustPoint *)m_Points.GetAt(CCONNECTPOINT_POLYGONALLINE_MIDDLE);
	// CPoint pFirstBendPoint  = CPoint(connMiddlePoint->GetPoint().x, m_Start.y);
	// CPoint pSecondBendPoint = CPoint(connMiddlePoint->GetPoint().x, m_End.y);

	pdc->MoveTo(m_Start);
	pdc->LineTo(m_StartStub.ptPosition);
	pdc->LineTo(m_objOrthogonalWire.m_ptBend[0].ptPosition);
	if(m_iBendTimes == 2)
	{
		pdc->LineTo(m_objOrthogonalWire.m_ptBend[1].ptPosition);

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

void CPolygonalLine::DrawFocus( CDC *pdc )
{
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.size(); i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		connPoint->Draw(pdc);
	}
}

#define DRAW_FRAME
/************************************************************************/
/* 功能：建构一个图元选择包络区。支持三种模式。                         */
/*       起始模式和结束模式都是贴着起始点和结束点的包络区。             */
/*       中间模式则需要给出包含起始点和结束点的包络区。                 */
/************************************************************************/
int CPolygonalLine::creatOneBorderArea(int borderStage, 
				CWirePoint& startWirePoint, CWirePoint& endWirePoint, CPoint * ptOutput)
{
	if(borderStage == POLYGONALLINE_BORDER_START)
	{
		if ((endWirePoint.endDirection == ORTHOGONALWIRE_SOUTH)
			|| (endWirePoint.endDirection == ORTHOGONALWIRE_NORTH))
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, 0);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, 0);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, 0);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, 0);
			return 4;
		}
		else if((endWirePoint.endDirection == ORTHOGONALWIRE_WEST)
			|| (endWirePoint.endDirection == ORTHOGONALWIRE_EAST))
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(0, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(0, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(0, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(0, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			return 4;
		}
	}
	else if(borderStage == POLYGONALLINE_BORDER_END)
	{
		if ((endWirePoint.endDirection == ORTHOGONALWIRE_SOUTH)
			|| (endWirePoint.endDirection == ORTHOGONALWIRE_NORTH))
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, 0);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, 0);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, 0);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, 0);
			return 4;
		}
		else if((endWirePoint.endDirection == ORTHOGONALWIRE_WEST)
			|| (endWirePoint.endDirection == ORTHOGONALWIRE_EAST))
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(0, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(0, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(0, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(0, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			return 4;
		}
	}
	else if(borderStage == POLYGONALLINE_BORDER_MIDDLE)
	{
		if (endWirePoint.endDirection == ORTHOGONALWIRE_SOUTH)
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			return 4;
		}
		else if(endWirePoint.endDirection == ORTHOGONALWIRE_NORTH)
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			return 4;
		}
		else if(endWirePoint.endDirection == ORTHOGONALWIRE_WEST)
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			return 4;
		}
		else if(endWirePoint.endDirection == ORTHOGONALWIRE_EAST)
		{
			ptOutput[0] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			ptOutput[1] = startWirePoint.ptPosition          + CPoint(ADJUSTPOINT_NEGATIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[2] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_POSITIVE_Y_MARGIN);
			ptOutput[3] = endWirePoint.ptPosition            + CPoint(ADJUSTPOINT_POSITIVE_X_MARGIN, ADJUSTPOINT_NEGATIVE_Y_MARGIN);
			return 4;
		}
	}
	else
	{
		TRACE("Error borderStage\r\n");
		return 0;
	}
	return 0;
}

void CPolygonalLine::DrawSelectBorderArea( CDC *pdc )
{
#ifdef DRAW_FRAME
	// 画笔为虚线，线宽为1，颜色为黑色。
	CPen blackPen( PS_DOT, 1, RGB(0, 0, 0) );
	CPen greenPen( PS_DOT, 1, RGB(0, 255, 0) );
	CPen purplePen( PS_DOT, 1, RGB(255, 0, 128) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&blackPen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );

	CPoint points[8];
	int iPolygonCount = 0;

	CWirePoint 	  startWirePoint;
	startWirePoint.endDirection = m_StartStub.endDirection;
	startWirePoint.ptPosition   = m_Start;
	
	CWirePoint 	  endWirePoint;
	endWirePoint.endDirection = m_EndStub.endDirection;
	endWirePoint.ptPosition   = m_End;

	if (m_iBendTimes == 1)
	{
		CWirePoint    middlePoint       = m_objOrthogonalWire.m_ptBend[0];
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_START, startWirePoint, m_StartStub, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&greenPen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, m_StartStub, middlePoint, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&purplePen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, middlePoint, m_EndStub, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&blackPen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_END, m_EndStub, endWirePoint, points);
		pdc->Polygon(points, iPolygonCount);
		
		TRACE("m_iBendTimes == 1\r\n");
	}
	else if (m_iBendTimes == 2)
	{
		CWirePoint    firstMiddlePoint        = m_objOrthogonalWire.m_ptBend[0];
		CWirePoint    secondMiddlePoint       = m_objOrthogonalWire.m_ptBend[1];

		CWirePoint 	  endStubWirePoint = m_EndStub;
		endStubWirePoint.endDirection = m_objOrthogonalWire.calculateWireDirection(secondMiddlePoint, m_EndStub);
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_START, startWirePoint, m_StartStub, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&greenPen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, m_StartStub, firstMiddlePoint, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&purplePen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, firstMiddlePoint, secondMiddlePoint, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&greenPen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, secondMiddlePoint, endStubWirePoint, points);
		pdc->Polygon(points, iPolygonCount);
		pdc->SelectObject(&blackPen);
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_END, endStubWirePoint, endWirePoint, points);
		pdc->Polygon(points, iPolygonCount);
		
		TRACE("m_iBendTimes == 2\r\n");
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
				if(abs(m_objOrthogonalWire.m_ptBend[1].endDirection) == ORTHOGONALWIRE_SOUTH)
				{
					m_objOrthogonalWire.m_fBendPercent = (float)(pt.x - m_Start.x) / (float)(m_End.x - m_Start.x);
				}
				else if(abs(m_objOrthogonalWire.m_ptBend[1].endDirection) == ORTHOGONALWIRE_EAST)
				{
					m_objOrthogonalWire.m_fBendPercent = (float)(pt.y - m_Start.y) / (float)(m_End.y - m_Start.y);
				}
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
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[CCONNECTPOINT_POLYGONALLINE_START];
	int iConnPoint = previousGraph->IsConnectOn(pStart);
	if(iConnPoint >= 0)
	{
		m_Previous = previousGraph;
		printInfomation("CPolygonalLine::SetPreviousGraph Before");
		m_iPreviousConnPointIdx = iConnPoint;
		printInfomation("CPolygonalLine::SetPreviousGraph After");
	}
	else if(m_Previous == previousGraph)
	{
		m_Previous = NULL;
	}
}
void CPolygonalLine::SetNextgraph(CGraph *nextGraph)
{
	CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[CCONNECTPOINT_POLYGONALLINE_END];
	int iConnPoint = nextGraph->IsConnectOn(pEnd);
	if(iConnPoint >= 0)
	{
		m_Next = nextGraph;
		printInfomation("CPolygonalLine::SetNextgraph Before");
		m_iNextConnPointIdx = iConnPoint;
		printInfomation("CPolygonalLine::SetNextgraph After");
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
	int iPolygonCount = 0;
		
	CWirePoint 	  startWirePoint;
	startWirePoint.endDirection = m_StartStub.endDirection;
	startWirePoint.ptPosition   = m_Start;
	
	CWirePoint 	  endWirePoint;
	endWirePoint.endDirection = m_EndStub.endDirection;
	endWirePoint.ptPosition   = m_End;
	
	if (m_iBendTimes == 1)
	{
		CWirePoint    middlePoint       = m_objOrthogonalWire.m_ptBend[0];

		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_START, startWirePoint, m_StartStub, points);
		CRgn objStartStubRgn;
		BOOL bRet = objStartStubRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objStartStubRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, m_StartStub, middlePoint, points);
		CRgn objMiddleRgn;
		bRet = objMiddleRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objMiddleRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, middlePoint, m_EndStub, points);
		CRgn objEndStubRgn;
		bRet = objEndStubRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objEndStubRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_END, m_EndStub, endWirePoint, points);
		CRgn objEndRgn;
		bRet = objEndRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objEndRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		TRACE("m_iBendTimes == 1\r\n");
	}
	else if (m_iBendTimes == 2)
	{
		CWirePoint    firstMiddlePoint        = m_objOrthogonalWire.m_ptBend[0];
		CWirePoint    secondMiddlePoint       = m_objOrthogonalWire.m_ptBend[1];

		CWirePoint 	  endStubWirePoint = m_EndStub;
		endStubWirePoint.endDirection = m_objOrthogonalWire.calculateWireDirection(secondMiddlePoint, m_EndStub);
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_START, startWirePoint, m_StartStub, points);
		CRgn objStartStubRgn;
		BOOL bRet = objStartStubRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objStartStubRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, m_StartStub, firstMiddlePoint, points);
		CRgn objFirstMiddleRgn;
		bRet = objFirstMiddleRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objFirstMiddleRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, firstMiddlePoint, secondMiddlePoint, points);
		CRgn objSecondMiddleRgn;
		bRet = objSecondMiddleRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objSecondMiddleRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_MIDDLE, secondMiddlePoint, endStubWirePoint, points);
		CRgn objEndStubRgn;
		bRet = objEndStubRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objEndStubRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		iPolygonCount = creatOneBorderArea(POLYGONALLINE_BORDER_END, endStubWirePoint, endWirePoint, points);
		CRgn objEndRgn;
		bRet = objEndRgn.CreatePolygonRgn(points, iPolygonCount, ALTERNATE);
		if(bRet && objEndRgn.PtInRegion( pt ))
		{
			flag = true;
			m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
			return flag;
		}
		
		TRACE("m_iBendTimes == 2\r\n");
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
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
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

int CPolygonalLine::IsConnectOn(CAdjustPoint *pt)
{
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		if(connPoint->IsOn(pt->GetPoint()))
		{
			pt->SetPoint(connPoint->GetPoint());
		    return i;
		}
	}
	return CCONNECTPOINT_INVALID_OPTION;
}

CWirePoint CPolygonalLine::calculateStartLineStub(CPoint& rootPoint, int iConnectPointIdx)
{
	CWirePoint stubPoint;
	if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_TOP)
	{
		stubPoint.ptPosition = rootPoint - CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.endDirection  = ORTHOGONALWIRE_NORTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_RIGHT)
	{
		stubPoint.ptPosition = rootPoint + CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.endDirection  = ORTHOGONALWIRE_EAST;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_BOTTOM)
	{
		stubPoint.ptPosition = rootPoint + CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.endDirection  = ORTHOGONALWIRE_SOUTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_LEFT)
	{
		stubPoint.ptPosition = rootPoint - CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.endDirection  = ORTHOGONALWIRE_WEST;
	}
	return stubPoint;
}

CWirePoint CPolygonalLine::calculateEndLineStub(CPoint& rootPoint, int iConnectPointIdx)
{
	CWirePoint stubPoint;
	if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_TOP)
	{
		stubPoint.ptPosition = rootPoint - CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.endDirection  = ORTHOGONALWIRE_SOUTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_RIGHT)
	{
		stubPoint.ptPosition = rootPoint + CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.endDirection  = ORTHOGONALWIRE_WEST;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_BOTTOM)
	{
		stubPoint.ptPosition = rootPoint + CPoint(0, POLYGONALLINE_STUB_LEN);
		stubPoint.endDirection  = ORTHOGONALWIRE_NORTH;
	}
	else if(iConnectPointIdx == CCONNECTPOINT_RECT_MIDDLE_LEFT)
	{
		stubPoint.ptPosition = rootPoint - CPoint(POLYGONALLINE_STUB_LEN, 0);
		stubPoint.endDirection  = ORTHOGONALWIRE_EAST;
	}
	return stubPoint;
}

void CPolygonalLine::AdjustFocusPoint()
{
	m_iBendTimes = 0;
	CAdjustPoint *connPoint = NULL;
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_POLYGONALLINE_START];
	connPoint->SetPoint(m_Start);
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_POLYGONALLINE_END];
	connPoint->SetPoint(m_End);
	// Calc middle point
	if((m_iPreviousConnPointIdx == -1) || (m_iNextConnPointIdx == -1))
		return;

	m_StartStub = calculateStartLineStub(m_Start, m_iPreviousConnPointIdx);
	m_EndStub   = calculateEndLineStub(m_End, m_iNextConnPointIdx);
	
	m_iBendTimes = m_objOrthogonalWire.calculateOrthogonalWire(m_StartStub, m_EndStub);

	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_POLYGONALLINE_MIDDLE];
	if (m_iBendTimes == 1)
	{
		connPoint->SetPoint(m_objOrthogonalWire.m_ptBend[0].ptPosition);
	}
	else if (m_iBendTimes == 2)
	{
		CPoint middlePoint = CPoint(
			(m_objOrthogonalWire.m_ptBend[0].ptPosition.x 
					+ m_objOrthogonalWire.m_ptBend[1].ptPosition.x) / 2,
			(m_objOrthogonalWire.m_ptBend[0].ptPosition.y 
					+ m_objOrthogonalWire.m_ptBend[1].ptPosition.y) / 2);
		connPoint->SetPoint(middlePoint);
	}
	
	for(int i = 0; i < CCONNECTPOINT_LINE_MAX; i++)
	{
		connPoint = (CAdjustPoint *)m_Points[i];
		connPoint->SetType(false);
	}
}

void CPolygonalLine::Serialize(cJSON * objJSON)
{
//	if(ar.IsStoring())
//	{
//		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
//		ar<<m_iPreviousConnPointIdx<<m_iNextConnPointIdx;
//	}
//	else
//	{
//		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
//		ar>>m_iPreviousConnPointIdx>>m_iNextConnPointIdx;
//	}
}