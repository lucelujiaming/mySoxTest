// ArcLine.cpp: implementation of the CArcLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "ArcLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CArcLine, CObject, 1)

CArcLine::CArcLine()
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

CArcLine::~CArcLine()
{

}

/************************************************************************/
/* 已知三点为（x1，y1)、（x2，y2)，（x3，y3)，求三点形成的圆的坐标，    */
/* 可以设求得的圆的原点为（X，Y），半径为R。这样可以得到方程组：        */
/* 		(x1-X)^2-(y1-Y)^2=R2      (1)式                                 */
/* 		(x2-X)^2-(y2-Y)^2=R2      (2)式                                 */
/* 		(x3-X)^2-(y3-Y)^2=R2      (3)式                                 */
/*                                                                      */
/* 由上述方程组可以得到：                                               */
/*      2(x1-x2)X +2(y1-y2)Y= x1^2-x2^2+y1^2-y2^2        (5)式          */
/*      2(x1-x3)X +2(y1-y3)Y= x1^2-x3^2+y1^2-y3^2        (6)式          */
/*                                                                      */
/* 此时，设：                                                           */
/*      A = x1-x2      B = y1-y2     C = x1-x3     D = y1-y3            */
/*      E = (x1^2-x2^2+y1^2-y2^2)/2                                     */
/*      F = (x1^2-x3^2+y1^2-y3^2)/2                                     */
/* 得到方程组：                                                         */
/*      AX+BY = E                                                       */
/*      CX+DY = F                                                       */
/* 求解，得到                                                           */
/*      X = -(DE-BF) / (BC-AD)                                          */
/*      Y = -(AF-CE) / (BC-AD)                                          */
/************************************************************************/
POINT CArcLine::calcCircleCenterByThreePoints(POINT first, POINT second, POINT third)
{
	POINT center;
	int A = first.x - second.x;
	int B = first.y - second.y;
	int C = first.x - third.x;
	int D = first.y - third.y;
	int E = (first.x * first.x - second.x * second.x + first.y * first.y - second.y * second.y)/2;
	int F = (first.x * first.x - third.x * third.x + first.y * first.y - third.y * third.y)/2;
	if(B * C != D * A)
	{ 
		center.x = (B * F - D * E) / (B * C - D * A);
		center.y = (C * E - A * F) / (B * C - D * A);
	}
	else
	{
		center.x = center.y = -1;

	}
	return center;
}

/************************************************************************/
/* 得到了圆心的坐标位置，也就得到了圆的半径计算公式                     */
/*     R = sqrt((x1-X) * (x1-X) + (y1-Y) * (y1-Y));                     */
/************************************************************************/
int CArcLine::calcRadiusByCenterAndOnePoint(POINT center, POINT point)
{
	return sqrt((point.x - center.x) * (point.x - center.x) 
				+ (point.y - center.y) * (point.y - center.y));
}

// 绘图方向
// 获取绘制圆弧的方向  如果iResult大于0为顺时针，小于0为逆时针。
// 设向量A = ( x1, y1 )，B = ( x2, y2 )
// |A×B| = x1*y2 - x2*y1 = |A|×|B|×sin(向量A到B的夹角)
// 所以这个值的正负也就是A到B旋转角sin值的正负
// 顺时针旋转角度0~180，sin>0
// 顺时针旋转角度180~360或者说逆时针旋转0~180，sin<0
// 这里方向为 A->B->C 
// |AB| = (B.x - A.x, B.y - A.y)
// |BC| = (C.x - B.x, C.y - B.y)
// |B.x - A.x  C.x - B.x|
// |B.y - A.y  C.y - B.y|  交叉相乘然后相减
// (B.x - A.x) * (C.y - B.y) - (B.y - A.y) * (C.x - B.x) = |AB| * |BC| * sin(angle)
BOOL CArcLine::calcArcDrection(POINT first, POINT second, POINT third)
{
	float fRet = (second.x - first.x) * (third.y - second.y) - (second.y - first.y) * (third.x - second.x);
	return fRet > 0;
}

POINT CArcLine::calcTangentPosition(POINT center, POINT point, BOOL iDirection)
{
	int iSwapValue;
	CPoint tangentPos;
	tangentPos = CPoint(center) - CPoint(point);
	if (iDirection)
	{
		iSwapValue   = tangentPos.x;
		tangentPos.x = -1 * tangentPos.y;
		tangentPos.y = iSwapValue;
	}
	else
	{
		iSwapValue   = tangentPos.x;
		tangentPos.x = tangentPos.y;
		tangentPos.y = -1 * iSwapValue;
	}
	tangentPos = CPoint(point) + tangentPos;
	return tangentPos;
}

void CArcLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	if(m_Points.size() < ARCLINE_POINTS_COUNT) 
    {
		return;
    }

	// printAllPoints("CArcLine::Draw");
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	if(pStart != NULL)
	{
		pdc->MoveTo(pStart->GetPoint());
	}

	CPen pe, *pOldPen;
	if(m_IsMark)
	{
        pe.CreatePen(PS_SOLID,1,RGB(255,0,0));      //初始化画笔（红色）
		pOldPen=pdc->SelectObject(&pe);				//把画笔选入DC，并保存原来画笔
	}

//	for(int i = 1; i < m_Points.size(); i++)
//	{
//		CAdjustPoint *pNext = (CAdjustPoint*)m_Points[i];
//		pdc->LineTo(pNext->GetPoint());
//	}

	POINT * pointArc = (POINT *)malloc(sizeof(POINT) * ARCLINE_POINTS_COUNT);
	int iPointsSize = m_Points.size();
	int iArcNum  = (iPointsSize - 1)/(ARCLINE_POINTS_COUNT - 1);

	CAdjustPoint *pNext = NULL;
	POINT pointCenter;
	int   iRadius;
	BOOL bDrection;
	for(int j = 0; j < iArcNum; j++)
	{
		pNext = (CAdjustPoint*)m_Points[j * (ARCLINE_POINTS_COUNT - 1)];
		pointArc[0] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (ARCLINE_POINTS_COUNT - 1) + 1];
		pointArc[1] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (ARCLINE_POINTS_COUNT - 1) + 2];
		pointArc[2] = pNext->GetPoint();
		
		pointCenter = calcCircleCenterByThreePoints(
						pointArc[0], pointArc[1], pointArc[2]);
		iRadius = calcRadiusByCenterAndOnePoint(pointCenter, pointArc[0]);

		CRect objCircleRect = CRect(CPoint(pointCenter.x - iRadius, pointCenter.y - iRadius), 
								CPoint(pointCenter.x + iRadius, pointCenter.y + iRadius));

		bDrection = calcArcDrection(
						pointArc[0], pointArc[1], pointArc[2]);
		if (bDrection)
		{
			pdc->Arc(objCircleRect, pointArc[2], pointArc[0]);
		}
		else
		{
			pdc->Arc(objCircleRect, pointArc[0], pointArc[2]);
		}
		// pdc->Ellipse(objCircleRect);
	}
	free(pointArc);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	CPoint pointLast = calcTangentPosition(pointCenter, m_Points[m_Points.size()-1]->GetPoint(), bDrection);
	DrawArrow(pdc, pointLast);
	if (bShowSelectBorder)
	{
		DrawSelectBorderArea(pdc);
	}
}

void CArcLine::DrawFocus(CDC *pdc)
{
	if(m_Points.size() < ARCLINE_POINTS_COUNT) 
    {
        return;
    }

	// printAllPoints("CArcLine::DrawFocus");
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		if(i == 0 || i == m_Points.size()-1) 
        {
            pConnPoint->SetType(false);
        }
		pConnPoint->Draw(pdc);
	}
}

#define DRAW_FRAME
void CArcLine::DrawSelectBorderArea( CDC *pdc )
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

void CArcLine::Move(int cx, int cy)
{
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint newPoint = pConnPoint->GetPoint() + CPoint(cx, cy);
		pConnPoint->SetPoint(newPoint);
	}
}

void CArcLine::AdjustSize(CPoint &pt)
{
	// printAllPoints("CArcLine::AdjustSize Before");
	CAdjustPoint *pFocusConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint];
	if(pFocusConnPoint != NULL)
	{
		pFocusConnPoint->SetPoint(pt);
	}
	// printAllPoints("CArcLine::AdjustSize");
}

void CArcLine::SetStartPoint(CPoint &pt)
{
	if(m_Points.size() <= 0) 
    {
        return;
    }

	// printAllPoints("CArcLine::SetStartPoint Before");
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pStart->SetPoint(pt);
	// printAllPoints("CArcLine::SetStartPoint");
}

void CArcLine::SetEndPoint(CPoint &pt)
{
	CAdjustPoint *pNewPoint;
	if(!m_IsCreateEnd)
	{
		pNewPoint = new CAdjustPoint();
		pNewPoint->SetPoint(pt);
		// printAllPoints("CArcLine::SetEndPoint(NotCreateEnd) Before");
		m_Points.insert(m_Points.end(), pNewPoint);
		// printAllPoints("CArcLine::SetEndPoint(NotCreateEnd) After");

//		// PolyBezier的point数组大小必须是4
//		if (m_Points.size() == BEZIERLINE_POINTS_COUNT)
//		{
//			m_IsCreateEnd = true;
//		}
	}
	else
	{
		// printAllPoints("CArcLine::SetEndPoint(CreateEnd) Before");
		// p = (CAdjustPoint*)m_Points.GetAt(0);
		pNewPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		pNewPoint->SetPoint(pt);
		// printAllPoints("CArcLine::SetEndPoint(CreateEnd) After");
	}
}


void CArcLine::SetLastPoint( CPoint &pt )
{
	CAdjustPoint *pLast;
	pLast = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pLast->SetPoint(pt);
	// printAllPoints("CArcLine::SetLastPoint");

}

void CArcLine::GetStartPoint(CPoint &pt)
{
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pt = pStart->GetPoint();
}

void CArcLine::GetEndPoint(CPoint &pt)
{
	// CAdjustPoint *pEnd = (CAdjustPoint*)m_Points.GetAt(0);
	CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pt = pEnd->GetPoint();
}

void CArcLine::SetPreviousGraph(CGraph *previousGraph)
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

void CArcLine::SetNextgraph(CGraph *nextGraph)
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

CGraph* CArcLine::GetPreviousGraph()
{
	return m_Previous;
}

CGraph* CArcLine::GetNextgraph()
{
	return m_Next;
}

bool CArcLine::IsEditable()
{
	return false;
}

bool CArcLine::IsControlFlow()
{
	return true;
}

bool CArcLine::IsIn(CPoint &pt)
{
	if(m_Points.size() < ARCLINE_POINTS_COUNT) 
    {
        return false;
    }

	// printAllPoints("CArcLine::IsIn Before");
	if(!m_IsCreateEnd)
	{
		int iPointsSize = m_Points.size();
		CAdjustPoint *connPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		m_Points.pop_back();
		delete connPoint;
		
		iPointsSize = m_Points.size();
		int iBezierNum  = (iPointsSize - 1)/(ARCLINE_POINTS_COUNT - 1);
		int iPointCount = iBezierNum * (ARCLINE_POINTS_COUNT - 1) + 1;
		int iPopNumber  = iPointsSize - iPointCount;
		for (int i = 0; i < iPopNumber; i++)
		{
			connPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
			m_Points.pop_back();
			delete connPoint;
		}
		
		m_IsCreateEnd = true;
		// m_Start和m_End对于折线来说，好像没有用。
		// connPoint = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
		// m_Start = connPoint->GetPoint();
		// connPoint = (CAdjustPoint*)m_Points.GetAt(0);
		// m_End = connPoint->GetPoint();
	}
	// printAllPoints("CArcLine::IsIn After");

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

bool CArcLine::IsOn(CPoint &pt)
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

double CArcLine::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CArcLine::DrawArrow( CDC *pdc, CPoint pointLast )
{
	if(m_Points.size() < ARCLINE_POINTS_COUNT) 
    {
		return;
    }
	else if ((m_Points.size() - 1) % (ARCLINE_POINTS_COUNT - 1) != 0)
    {
		return;
    }
	
	// CAdjustPoint *pArrowPoint = (CAdjustPoint*)m_Points[m_Points.size()-2];
	CPoint Start = pointLast;
	int flSx = Start.x;
	int flSy = Start.y;
	CAdjustPoint *pArrowPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
	CPoint End = pArrowPoint->GetPoint();
	int flEx = End.x;
	int flEy = End.y;
	double flLength = 10;
	double flAngle = 40;

	if(GetDistance(flSx, flSy, flEx, flEy) == 0) 
	{
		TRACE("GetDistance == 0\r\n");
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

void CArcLine::SaveParamsToJSON(cJSON * objJSON)
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
	if (m_Previous)
	{
		cJSON_AddNumberToObject(jsonGraph, "PreviousGraphSeq", m_Previous->getGraphSeq());
	}
	else
	{
		cJSON_AddNumberToObject(jsonGraph, "PreviousGraphSeq", -1);
	}
	cJSON_AddNumberToObject(jsonGraph, "PreviousConnPointIdx", m_iPreviousConnPointIdx);
	if (m_Next)
	{
		cJSON_AddNumberToObject(jsonGraph, "NextGraphSeq", m_Next->getGraphSeq());
	}
	else
	{
		cJSON_AddNumberToObject(jsonGraph, "NextGraphSeq", -1);
	}
	cJSON_AddNumberToObject(jsonGraph, "NextConnPointIdx", m_iNextConnPointIdx);
	// End of save PreviousConnPoint and NextConnPoint

	// m_Points.Serialize(ar);
	cJSON * jsonPointList = cJSON_CreateObject();
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint connStart = pConnPoint->GetPoint();
		cJSON * jsonPoint = cJSON_CreateObject();
		cJSON_AddNumberToObject(jsonPoint, "idx", i);
		cJSON_AddNumberToObject(jsonPoint, "x", connStart.x);
		cJSON_AddNumberToObject(jsonPoint, "y", connStart.y);
		cJSON_AddItemToObject(jsonPointList, GetTypeName(), jsonPoint);
	}
	cJSON_AddItemToObject(jsonGraph, "PointList", jsonPointList);
	// End of m_Points.Serialize(ar);
	cJSON_AddItemToObject(objJSON, GetTypeName(), jsonGraph);
}

void CArcLine::LoadOnePointFromJSON(cJSON * objPoint)
{
	CAdjustPoint *pAdjustPoint = new CAdjustPoint();
	CPoint point;
	cJSON *child = objPoint->child;
    while(child)
    {  
        switch ((child->type)&255)
        {  
        case cJSON_Number:    
            {   
				if(strcmp(child->string, "x") == 0)
				{   
					point.x = (int)child->valueint;
				}
				else if(strcmp(child->string, "y") == 0)
				{   
					point.y = (int)child->valueint;
				}
            }   
            break;
        }   
        child = child->next ;
    }
	pAdjustPoint->SetPoint(point);
	m_Points.push_back(pAdjustPoint);
}

void CArcLine::LoadParamsFromJSON(cJSON * objJSON)
{
	m_Points.clear();
	cJSON *child = objJSON->child;
    while(child)
    {   
        switch ((child->type)&255)
        {   
        case cJSON_True:    
            TRACE("cJSON_True"); 
			break;
        case cJSON_Number:    
            {   
                if(strcmp(child->string, "GraphSeq") == 0)
                {   
					setGraphSeq((int)child->valueint);
                }
				else if(strcmp(child->string, "PreviousGraphSeq") == 0)
                {   
					m_iPreviousGraphSeq = (int)child->valueint;
                }
				else if(strcmp(child->string, "PreviousConnPointIdx") == 0)
                {   
					m_iPreviousConnPointIdx = (int)child->valueint;
                }
				else if(strcmp(child->string, "NextGraphSeq") == 0)
                {   
					m_iNextGraphSeq = (int)child->valueint;
                }
				else if(strcmp(child->string, "NextConnPointIdx") == 0)
                {   
					m_iNextConnPointIdx = (int)child->valueint;
                }
				else if(strcmp(child->string, "FocusPoint") == 0)
                {   
					m_FocusPoint = (int)child->valueint;
                }
				else if(strcmp(child->string, "IsCreateEnd") == 0)
                {   
					m_IsCreateEnd = (int)child->valueint;
                }
            }   
            break;
        case cJSON_String: 
            TRACE("cJSON_String\n"); 
            {   
                if(strcmp(child->string, "Text") == 0)
                {   
                    m_text = CString(child->valuestring);
                }
            }    
            break;
        case cJSON_Array:
            TRACE("cJSON_Array\n"); 
            break;
        case cJSON_Object:  
            TRACE("cJSON_Object\n"); 
            {   
                if(strcmp(child->string, "PointList") == 0)
                {   
					cJSON *grandChild = child->child;
					while(grandChild)
					{
						switch ((grandChild->type)&255)
						{   
						case cJSON_Object:  
							LoadOnePointFromJSON(grandChild);
							break;
						}
						grandChild = grandChild->next ;
					}
                }
            }   
            break;
        }   
        child = child->next ;
    }
}
