// QuadraticSplineLine.cpp: implementation of the CQuadraticSplineLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "DrawFlowChart.h"
#include "QuadraticSplineLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ROUND(h) int((h)+0.5)//定义四舍五入

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CQuadraticSplineLine, CObject, 1)

CQuadraticSplineLine::CQuadraticSplineLine()
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

CQuadraticSplineLine::~CQuadraticSplineLine()
{
}

/*----------------------------------------------------
input   : t, i, k分别为参数值，支撑区间左端节点的下标，曲线次数
function: 计算第i个k次B样条基函数值
-----------------------------------------------------*/
double CQuadraticSplineLine::BasicFunctionValue(double t, int i, int k)
{
	//次数为0时的基函数值
	if(k==0)
	{
		if(t>=m_doubleKnots[i]&&t<m_doubleKnots[i+1])
        {
			return 1.0;
        }
		else 
        {
			return 0.0;
        }
	}

	//次数不为0时的基函数计算
	double value1, value2, value;
	if(k>0)
	{
		//节点矢量位于定义域之外
		if(t<m_doubleKnots[i]||t>m_doubleKnots[i+k+1])
        {
			return 0.0;
        }

		//节点矢量位于定义域之外
		else
		{
			double coff1, coff2;    //系数
			double denominator=0.0; //分母

			//递推公式第一项
			denominator=m_doubleKnots[i+k]-m_doubleKnots[i];
			if(denominator==0.0)
            {
				coff1=0.0;                  //重节点分母为0
            }
			else
            {
				coff1=(t-m_doubleKnots[i])/denominator;
            }

			//递推公式第二项
			denominator=m_doubleKnots[i+k+1]-m_doubleKnots[i+1];
			if(denominator==0.0)
            {
				coff2=0.0;                  //重节点分母为0
            }
			else
            {
				coff2=(m_doubleKnots[i+k+1]-t)/denominator;
            }

			//递推公式第一、二项的值
			value1=coff1*BasicFunctionValue(t,i,k-1);
			value2=coff2*BasicFunctionValue(t,i+1,k-1);

			value=value1+value2;
		}
	}
	return value;
}

/*----------------------------------------------------
input   : 
function: 绘制曲线
-----------------------------------------------------*/
void CQuadraticSplineLine::DrawCurve(CDC*pDC, int iNum)
{
	//绘制控制点
	// DrawControlPoint(pDC, iNum);

	//创建并选取画笔
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID,2,RGB(0,0,255));
	pOldPen=pDC->SelectObject(&NewPen);

	//等距取参数点计算值
	double tStep=0.01;
	for(double t=0.0; t<=1.0; t+=tStep)
	{
		CP2 p(0,0);//离散点
		for(int i=0; i<=m_iCOntrolPointNumMinusOne; i++)
		{
			double BValue=BasicFunctionValue(t, i, m_iCurveTimes);
			p = p + m_objQuadraticSplineControlPoint[i]*BValue;
		}
		if(t==0)
        {
			pDC->MoveTo(ROUND(p.x), ROUND(p.y));
        }
		else
        {
			pDC->LineTo(ROUND(p.x), ROUND(p.y));
        }
	}

	//还原画笔，删除创建的画笔
	pDC->SelectObject(pOldPen);	
	NewPen.DeleteObject();      
}

/*----------------------------------------------------
input   : 
function: 绘制控制点
-----------------------------------------------------*/
void CQuadraticSplineLine::DrawControlPoint(CDC*pDC, int iNum)
{
	//初始化画笔
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&NewPen);

	//初始化画刷
	CBrush NewBrush,*pOldBrush;
	NewBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush=pDC->SelectObject(&NewBrush);

	//加粗控制点
	for(int i=0;i<=m_iCOntrolPointNumMinusOne;i++)
	{
		if(0==i)
		{
			pDC->MoveTo(ROUND(m_objQuadraticSplineControlPoint[i].x),ROUND(m_objQuadraticSplineControlPoint[i].y));
			pDC->Ellipse(ROUND(m_objQuadraticSplineControlPoint[i].x)-5,ROUND(m_objQuadraticSplineControlPoint[i].y)-5,ROUND(m_objQuadraticSplineControlPoint[i].x)+5,ROUND(m_objQuadraticSplineControlPoint[i].y)+5);
		}
		else
		{
			pDC->LineTo(ROUND(m_objQuadraticSplineControlPoint[i].x),ROUND(m_objQuadraticSplineControlPoint[i].y));
			pDC->Ellipse(ROUND(m_objQuadraticSplineControlPoint[i].x)-5,ROUND(m_objQuadraticSplineControlPoint[i].y)-5,ROUND(m_objQuadraticSplineControlPoint[i].x)+5,ROUND(m_objQuadraticSplineControlPoint[i].y)+5);
		}
	}

	//还原画笔画刷
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject(); 
	NewBrush.DeleteObject(); 
}

void CQuadraticSplineLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	if(m_Points.size() < 2)
     {
         return;
     }

	// printAllPoints("CQuadraticSplineLine::Draw");
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
	int iPointsSize = m_Points.size();

	m_iCOntrolPointNumMinusOne=iPointsSize - 1, m_iCurveTimes=3;

	//节点初始化
	m_doubleKnots[0]=-0.5, m_doubleKnots[1]=-0.25,m_doubleKnots[2]=0.0;
	m_doubleKnots[3]=0.25, m_doubleKnots[4]=0.5,  m_doubleKnots[5]=0.75;
	m_doubleKnots[6]=1.0,  m_doubleKnots[7]=1.25, m_doubleKnots[8]= 1.5;
	// m_doubleKnots[9]=2.0,  m_doubleKnots[10]=1.25, m_doubleKnots[11]= 1.5;

//	//控制点初始化
//	P[0].x=-460,  P[0].y=-49;
//	P[1].x=-355,  P[1].y=204;
//	P[2].x= -63,  P[2].y=241;
//	P[3].x= 66,   P[3].y=-117;
//	P[4].x= 264,  P[4].y=-101;
//	P[5].x= 400,  P[5].y=208;
	
	CAdjustPoint *pNext = NULL;

	m_objQuadraticSplineControlPoint = new CP2[iPointsSize];
	for(int j = 0; j < iPointsSize; j++)
	{
		pNext = (CAdjustPoint*)m_Points[j];
		m_objQuadraticSplineControlPoint[j].x = pNext->GetPoint().x;
		m_objQuadraticSplineControlPoint[j].y = pNext->GetPoint().y;
	}

	if (bShowSelectBorder)
	{
		DrawControlPoint(pdc, iPointsSize);
	}
	DrawCurve(pdc, iPointsSize);
	delete[] m_objQuadraticSplineControlPoint;
	
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

void CQuadraticSplineLine::DrawFocus(CDC *pdc)
{
	if(m_Points.size() < 2) 
    {
        return;
    }

	// printAllPoints("CQuadraticSplineLine::DrawFocus");
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
void CQuadraticSplineLine::DrawSelectBorderArea( CDC *pdc )
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

void CQuadraticSplineLine::Move(int cx, int cy)
{
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint newPoint = pConnPoint->GetPoint() + CPoint(cx, cy);
		pConnPoint->SetPoint(newPoint);
	}
}

void CQuadraticSplineLine::AdjustSize(CPoint &pt)
{
	// printAllPoints("CQuadraticSplineLine::AdjustSize Before");
	CAdjustPoint *pFocusConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint];
	if(pFocusConnPoint != NULL)
	{
		pFocusConnPoint->SetPoint(pt);
	}
	// printAllPoints("CQuadraticSplineLine::AdjustSize");
}

void CQuadraticSplineLine::SetStartPoint(CPoint &pt)
{
	if(m_Points.size() <= 0) 
    {
        return;
    }

	// printAllPoints("CQuadraticSplineLine::SetStartPoint Before");
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pStart->SetPoint(pt);
	// printAllPoints("CQuadraticSplineLine::SetStartPoint");
}

void CQuadraticSplineLine::SetEndPoint(CPoint &pt)
{
	CAdjustPoint *pNewPoint;
	if(!m_IsCreateEnd)
	{
		pNewPoint = new CAdjustPoint();
		pNewPoint->SetPoint(pt);
		// printAllPoints("CQuadraticSplineLine::SetEndPoint(NotCreateEnd) Before");
		m_Points.insert(m_Points.end(), pNewPoint);
		// printAllPoints("CQuadraticSplineLine::SetEndPoint(NotCreateEnd) After");

//		// PolyBezier的point数组大小必须是4
//		if (m_Points.size() == BEZIERLINE_POINTS_COUNT)
//		{
//			m_IsCreateEnd = true;
//		}
	}
	else
	{
		// printAllPoints("CQuadraticSplineLine::SetEndPoint(CreateEnd) Before");
		// p = (CAdjustPoint*)m_Points.GetAt(0);
		pNewPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		pNewPoint->SetPoint(pt);
		// printAllPoints("CQuadraticSplineLine::SetEndPoint(CreateEnd) After");
	}
}


void CQuadraticSplineLine::SetLastPoint( CPoint &pt )
{
	CAdjustPoint *pLast;
	pLast = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pLast->SetPoint(pt);
	// printAllPoints("CQuadraticSplineLine::SetLastPoint");

}

void CQuadraticSplineLine::GetStartPoint(CPoint &pt)
{
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pt = pStart->GetPoint();
}

void CQuadraticSplineLine::GetEndPoint(CPoint &pt)
{
	// CAdjustPoint *pEnd = (CAdjustPoint*)m_Points.GetAt(0);
	CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pt = pEnd->GetPoint();
}

void CQuadraticSplineLine::SetPreviousGraph(CGraph *previousGraph)
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

void CQuadraticSplineLine::SetNextgraph(CGraph *nextGraph)
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

CGraph* CQuadraticSplineLine::GetPreviousGraph()
{
	return m_Previous;
}

CGraph* CQuadraticSplineLine::GetNextgraph()
{
	return m_Next;
}

bool CQuadraticSplineLine::IsEditable()
{
	return false;
}

bool CQuadraticSplineLine::IsControlFlow()
{
	return true;
}

bool CQuadraticSplineLine::IsIn(CPoint &pt)
{
	if(m_Points.size() < 2) 
    {
        return false;
    }

	// printAllPoints("CQuadraticSplineLine::IsIn Before");
	if(!m_IsCreateEnd)
	{
		int iPointsSize = m_Points.size();
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
	// printAllPoints("CQuadraticSplineLine::IsIn After");

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

bool CQuadraticSplineLine::IsOn(CPoint &pt)
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

double CQuadraticSplineLine::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CQuadraticSplineLine::DrawArrow( CDC *pdc )
{
	if(m_Points.size() < 2) 
    {
		return;
    }
	
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

void CQuadraticSplineLine::SaveParamsToJSON(cJSON * objJSON)
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

void CQuadraticSplineLine::LoadOnePointFromJSON(cJSON * objPoint)
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

void CQuadraticSplineLine::LoadParamsFromJSON(cJSON * objJSON)
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
