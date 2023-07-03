// CubicBezierCurve.cpp: implementation of the CCubicBezierCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "DrawFlowChart.h"
#include "CubicBezierCurve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ROUND(h) int((h)+0.5)//定义四舍五入

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CCubicBezierCurve, CObject, 1)

CCubicBezierCurve::CCubicBezierCurve()
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
	
	m_iBezierFunctionTimes = BEZIERLINE_QUADRATIC;

}

CCubicBezierCurve::~CCubicBezierCurve()
{
}

/************************************************************************/
/* 给定n+1个控制点Pi(i = 1,2,...,n)，则n次Bezier曲线定义为：            */
/*     p(t) = Σ Pi * Bernstein(t)                                       */
/* 这个Bernstein函数的表达式为：                                        */
/*     Bernstein(t) = n!/i! * (n - i)! * t^i * (1 - t)^(n-i)            */
/*                  = C(n, i) * t^i * (1 - t)^(n-i)                     */
/* 根据这个算式可以给出一次，二次，三次Bezier曲线。                     */
/************************************************************************/
/* 当n = 1的时候，一次Bezier曲线的计算公式为：                          */
/*     p(t) = (1 - t) * P0 + t * P1                                     */
/* 这是一条连接P0和P1的直线。                                           */
/* 其中B01(t) = (1 - t)；B11(t) = t。                                   */
/************************************************************************/
/* 当n = 2的时候，二次Bezier曲线的计算公式为：                          */
/*     p(t) = (1 - t)^2 * P0 + 2t(1 - t) * P1 + t^2 * P2                */
/* 这是一条从P0和P2的抛物线。                                           */
/* 其中B02(t) = (1 - t)^2；B12(t) = 2t(1 - t)；B22(t) = t^2。           */
/************************************************************************/
/* 当n = 3的时候，三次Bezier曲线的计算公式为：                          */
/*     p(t) = (1 - t)^3 * P0 + 3t(1 - t)^2 * P1 +                       */
/*            3t^2 * (1 - t) * P3 + t^3 * P4                            */
/* 其中B03(t) = (1 - t)^3；     B13(t) = 3t(1 - t)^2；                  */
/*     B23(t) = 3t^2 * (1 - t)；B33(t) = t^3。                          */
/************************************************************************/
void CCubicBezierCurve::DrawPrimaryFuncBezierCurve(CDC* pDC) // 基函数算法
{
	pDC->MoveTo(ROUND(m_objBezierCurveControlPoint[0].x), 
		        ROUND(m_objBezierCurveControlPoint[0].y));
	double tStep = 0.01;
	for (double t = 0; t <= 1; t += tStep)
	{
		// B03(t) = (1 - t)^3；
		double B03 = (1 - t) * (1 - t) * (1 - t);
		// B13(t) = 3t(1 - t)^2
		double B13 = 3 * t * (1 - t) * (1 - t);
		// B23(t) = 3t^2 * (1 - t)
		double B23 = 3 * t * t * (1 - t);
		// B33(t) = t^3
		double B33 = t * t * t;
		// p(t) = (1 - t)^3 * P0 + 3t(1 - t)^2 * P1 + 
		//        3t^2 * (1 - t) * P3 + t^3 * P4
		CP2 pt = B03 * m_objBezierCurveControlPoint[0] 
			   + B13 * m_objBezierCurveControlPoint[1]
			   + B23 * m_objBezierCurveControlPoint[2] 
			   + B33 * m_objBezierCurveControlPoint[3];
		// 使用p(t)绘制。 
		pDC->LineTo(ROUND(pt.x), ROUND(pt.y));
	}
}

/************************************************************************/
// de Casteljau递推算法
// 这个算法不依赖于n次Bezier曲线的定义。而使用下面的递推公式：
//    P r,i (t) = (1 - t) * P r-1,i (t) + t * P r-1,i+1 (t) 
// 当n = 3的时候，有：
//    r = 1, i = 0, 1, 2
//    r = 2, i = 0, 1
//    r = 3, i = 0
// 三次曲线的递推如下：
//    P 1,0 (t) = (1 - t) * P 0,0 (t) + t * P 0,1 (t) 
//    P 1,1 (t) = (1 - t) * P 0,1 (t) + t * P 0,2 (t) 
//    P 1,2 (t) = (1 - t) * P 0,2 (t) + t * P 0,3 (t) 
//    P 2,0 (t) = (1 - t) * P 1,0 (t) + t * P 1,1 (t) 
//    P 2,1 (t) = (1 - t) * P 1,1 (t) + t * P 1,2 (t) 
//    P 3,0 (t) = (1 - t) * P 2,0 (t) + t * P 2,1 (t) 
// 
void CCubicBezierCurve::DrawCasteljauBezierCurve(CDC* pDC) 
{
	pDC->MoveTo(ROUND(m_objBezierCurveControlPoint[0].x), 
			    ROUND(m_objBezierCurveControlPoint[0].y)); 
	double tStep = 0.01;//步长	
	for (double t = 0; t < 1; t += tStep)
	{
		CP2 p0, p1, p2, p3, p4, p5;
        // P 1,0 (t) = (1 - t) * P 0,0 (t) + t * P 0,1 (t) 
		p0 = (1 - t) * m_objBezierCurveControlPoint[0] + t * m_objBezierCurveControlPoint[1];
        // P 1,1 (t) = (1 - t) * P 0,1 (t) + t * P 0,2 (t) 
		p1 = (1 - t) * m_objBezierCurveControlPoint[1] + t * m_objBezierCurveControlPoint[2];
        // P 1,2 (t) = (1 - t) * P 0,2 (t) + t * P 0,3 (t) 
		p2 = (1 - t) * m_objBezierCurveControlPoint[2] + t * m_objBezierCurveControlPoint[3];
        // P 2,0 (t) = (1 - t) * P 1,0 (t) + t * P 1,1 (t) 
		p3 = (1 - t) * p0 + t * p1;
        // P 2,1 (t) = (1 - t) * P 1,1 (t) + t * P 1,2 (t) 
		p4 = (1 - t) * p1 + t * p2;
        // P 3,0 (t) = (1 - t) * P 2,0 (t) + t * P 2,1 (t) 
		p5 = (1 - t) * p3 + t * p4;
		// 使用p(t)绘制。 
		pDC->LineTo(ROUND(p5.x), ROUND(p5.y));
	}
	// Connect breakpoint.
	pDC->LineTo(m_objBezierCurveControlPoint[m_iBezierFunctionTimes].x, 
				m_objBezierCurveControlPoint[m_iBezierFunctionTimes].y);
}

void CCubicBezierCurve::DrawPolygon(CDC* pDC)
{
	CPen NewPen, * pOldPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	pOldPen = pDC->SelectObject(&NewPen);
	pDC->MoveTo(ROUND(m_objBezierCurveControlPoint[0].x), 
		        ROUND(m_objBezierCurveControlPoint[0].y));
	pDC->Ellipse(ROUND(m_objBezierCurveControlPoint[0].x) - 5, 
		         ROUND(m_objBezierCurveControlPoint[0].y) - 5, 
				 ROUND(m_objBezierCurveControlPoint[0].x) + 5, 
				 ROUND(m_objBezierCurveControlPoint[0].y) + 5);//绘制控制多边形顶点
	for (int i = 1; i < 4; i++)
	{
		pDC->LineTo(ROUND(m_objBezierCurveControlPoint[i].x), 
			        ROUND(m_objBezierCurveControlPoint[i].y));
		pDC->Ellipse(ROUND(m_objBezierCurveControlPoint[i].x) - 5, 
			         ROUND(m_objBezierCurveControlPoint[i].y) - 5, 
					 ROUND(m_objBezierCurveControlPoint[i].x) + 5, 
					 ROUND(m_objBezierCurveControlPoint[i].y) + 5);//绘制控制多边形顶点
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

/************************************************************************/
/* 其实贝塞尔曲线的平滑拼接非常简单。首先已知一个贝塞尔曲线由四点控制。 */
/* 我们只需要让前一个曲线的第三点和第四点与后一个曲线的第一点和第二点   */
/* 这三点共线即可。因为对于两条连续的贝塞尔曲线来说，前一个曲线的第四   */
/* 点就是后一个曲线的第一点，两个点本来就是同一个点。                   */
/* 这就是这个函数的逻辑。即重新设定第3K + 1个调整点，来实现平滑。       */
/************************************************************************/
void CCubicBezierCurve::smoothBezierLine()
{
	int iPointsSize = m_Points.size();
	int iBezierNum  = (iPointsSize - 1)/(BEZIERLINE_POINTS_COUNT - 1);

	CPoint pointAdjustGroup[3];
	CPoint point0To1Diff, point1To2Diff;
	CAdjustPoint *pNext = NULL;
	for(int j = 1; j < iBezierNum; j++)
	{
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1) - 1];
		pointAdjustGroup[0] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1)];
		pointAdjustGroup[1] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1) + 1];
		pointAdjustGroup[2] = pNext->GetPoint();
		
		point0To1Diff = pointAdjustGroup[1] - pointAdjustGroup[0];
		point1To2Diff = pointAdjustGroup[2] - pointAdjustGroup[0];
		float slope0To1 = (float)point0To1Diff.y / (float)point0To1Diff.x;
		float slope1To2 = (float)point1To2Diff.y / (float)point1To2Diff.x;

		if(abs(slope0To1 - slope1To2) < BEZIERLINE_SMOOTH_DEVIATION)
			continue;
		
		pointAdjustGroup[2] = pointAdjustGroup[1] + point0To1Diff;
		pNext->SetPoint(pointAdjustGroup[2]);
	}
}

void CCubicBezierCurve::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	if(m_Points.size() < BEZIERLINE_POINTS_COUNT) return;

	// printAllPoints("CCubicBezierCurve::Draw");
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	CPoint ptStart = pStart->GetPoint();
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

	POINT * pointBezier = (POINT *)malloc(sizeof(POINT) * BEZIERLINE_POINTS_COUNT);
	int iPointsSize = m_Points.size();
	int iBezierNum  = (iPointsSize - 1)/(BEZIERLINE_POINTS_COUNT - 1);

	CAdjustPoint *pNext = NULL;

	for(int j = 0; j < iBezierNum; j++)
	{
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1)];
		pointBezier[0] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1) + 1];
		pointBezier[1] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1) + 2];
		pointBezier[2] = pNext->GetPoint();
		pNext = (CAdjustPoint*)m_Points[j * (BEZIERLINE_POINTS_COUNT - 1) + 3];
		pointBezier[3] = pNext->GetPoint();

		m_objBezierCurveControlPoint[0].x = pointBezier[0].x;
		m_objBezierCurveControlPoint[0].y = pointBezier[0].y;
		m_objBezierCurveControlPoint[1].x = pointBezier[1].x;
		m_objBezierCurveControlPoint[1].y = pointBezier[1].y;
		m_objBezierCurveControlPoint[2].x = pointBezier[2].x;
		m_objBezierCurveControlPoint[2].y = pointBezier[2].y;
		m_objBezierCurveControlPoint[3].x = pointBezier[3].x;
		m_objBezierCurveControlPoint[3].y = pointBezier[3].y;

		if (bShowSelectBorder)
		{
			// DrawControlPolygon(pdc);
			DrawPolygon(pdc);
		}
		// DrawBezier(pdc);
		// DrawPrimaryFuncBezierCurve(pdc);
		DrawCasteljauBezierCurve(pdc);
	}
	free(pointBezier);


	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	DrawArrow(pdc);
	if (bShowSelectBorder)
	{
		DrawSelectBorderArea(pdc);
	}
}

void CCubicBezierCurve::DrawFocus(CDC *pdc)
{
	if(m_Points.size() < BEZIERLINE_POINTS_COUNT) return;

	// printAllPoints("CCubicBezierCurve::DrawFocus");
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		if(i == 0 || i == m_Points.size()-1) pConnPoint->SetType(false);
		pConnPoint->Draw(pdc);
	}
}

#define DRAW_FRAME
void CCubicBezierCurve::DrawSelectBorderArea( CDC *pdc )
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

void CCubicBezierCurve::Move(int cx, int cy)
{
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint newPoint = pConnPoint->GetPoint() + CPoint(cx, cy);
		pConnPoint->SetPoint(newPoint);
	}
}

void CCubicBezierCurve::AdjustSize(CPoint &pt)
{
	// printAllPoints("CCubicBezierCurve::AdjustSize Before");
	CAdjustPoint *pFocusConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint];
	if(pFocusConnPoint != NULL)
	{
		if((m_FocusPoint >0) && (m_FocusPoint % ((BEZIERLINE_POINTS_COUNT - 1)) == 0))
		{
			CPoint pointDiff = pt - pFocusConnPoint->GetPoint();
			CAdjustPoint *pPrevConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint - 1];
			CPoint newPoint = pPrevConnPoint->GetPoint() + pointDiff;
			pPrevConnPoint->SetPoint(newPoint);
			CAdjustPoint *pNextConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint + 1];
			newPoint = pNextConnPoint->GetPoint() + pointDiff;
			pNextConnPoint->SetPoint(newPoint);
			
			pFocusConnPoint->SetPoint(pt);
		}
		else
		{
			pFocusConnPoint->SetPoint(pt);
		}
	}
	// printAllPoints("CCubicBezierCurve::AdjustSize");
}

void CCubicBezierCurve::SetStartPoint(CPoint &pt)
{
	if(m_Points.size() <= 0) return;

	// printAllPoints("CCubicBezierCurve::SetStartPoint Before");
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pStart->SetPoint(pt);
	// printAllPoints("CCubicBezierCurve::SetStartPoint");
}

void CCubicBezierCurve::SetEndPoint(CPoint &pt)
{
	CAdjustPoint *pNewPoint;
	if(!m_IsCreateEnd)
	{
		pNewPoint = new CAdjustPoint();
		pNewPoint->SetPoint(pt);
		// printAllPoints("CCubicBezierCurve::SetEndPoint(NotCreateEnd) Before");
		m_Points.insert(m_Points.end(), pNewPoint);
		// printAllPoints("CCubicBezierCurve::SetEndPoint(NotCreateEnd) After");

//		// PolyBezier的point数组大小必须是4
//		if (m_Points.size() == BEZIERLINE_POINTS_COUNT)
//		{
//			m_IsCreateEnd = true;
//		}
	}
	else
	{
		// printAllPoints("CCubicBezierCurve::SetEndPoint(CreateEnd) Before");
		// p = (CAdjustPoint*)m_Points.GetAt(0);
		pNewPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		pNewPoint->SetPoint(pt);
		// printAllPoints("CCubicBezierCurve::SetEndPoint(CreateEnd) After");
	}
}


void CCubicBezierCurve::SetLastPoint( CPoint &pt )
{
	CAdjustPoint *pLast;
	pLast = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pLast->SetPoint(pt);
	// printAllPoints("CCubicBezierCurve::SetLastPoint");

}

void CCubicBezierCurve::GetStartPoint(CPoint &pt)
{
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pt = pStart->GetPoint();
}

void CCubicBezierCurve::GetEndPoint(CPoint &pt)
{
	// CAdjustPoint *pEnd = (CAdjustPoint*)m_Points.GetAt(0);
	CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pt = pEnd->GetPoint();
}

void CCubicBezierCurve::SetPreviousGraph(CGraph *previousGraph)
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

void CCubicBezierCurve::SetNextgraph(CGraph *nextGraph)
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

CGraph* CCubicBezierCurve::GetPreviousGraph()
{
	return m_Previous;
}

CGraph* CCubicBezierCurve::GetNextgraph()
{
	return m_Next;
}

bool CCubicBezierCurve::IsEditable()
{
	return false;
}

bool CCubicBezierCurve::IsControlFlow()
{
	return true;
}

bool CCubicBezierCurve::IsIn(CPoint &pt)
{
	if(m_Points.size() < BEZIERLINE_POINTS_COUNT) return false;

	// printAllPoints("CCubicBezierCurve::IsIn Before");
	if(!m_IsCreateEnd)
	{
		int iPointsSize = m_Points.size();
		CAdjustPoint *connPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		m_Points.pop_back();
		delete connPoint;
		
		iPointsSize = m_Points.size();
		int iBezierNum  = (iPointsSize - 1)/(BEZIERLINE_POINTS_COUNT - 1);
		int iPointCount = iBezierNum * (BEZIERLINE_POINTS_COUNT - 1) + 1;
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
	// printAllPoints("CCubicBezierCurve::IsIn After");

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

bool CCubicBezierCurve::IsOn(CPoint &pt)
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

double CCubicBezierCurve::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CCubicBezierCurve::DrawArrow( CDC *pdc )
{
	if(m_Points.size() < BEZIERLINE_POINTS_COUNT) 
		return;
	else if ((m_Points.size() - 1) % (BEZIERLINE_POINTS_COUNT - 1) != 0)
		return;
	
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

void CCubicBezierCurve::SaveParamsToJSON(cJSON * objJSON)
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

void CCubicBezierCurve::LoadOnePointFromJSON(cJSON * objPoint)
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

void CCubicBezierCurve::LoadParamsFromJSON(cJSON * objJSON)
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