// CubicSplineLine.cpp: implementation of the CCubicSplineLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "DrawFlowChart.h"
#include "CubicSplineLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ROUND(h) int((h)+0.5)//定义四舍五入

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CCubicSplineLine, CObject, 1)

CCubicSplineLine::CCubicSplineLine()
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

CCubicSplineLine::~CCubicSplineLine()
{
}

void CCubicSplineLine::DrawDataPoint(CDC*pDC, int n)
{
	CBrush NewBrush,*pOldBrush;//加粗型值点
	NewBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush=pDC->SelectObject(&NewBrush);
	for(int i=1;i<n;i++)
	{
		pDC->Ellipse(ROUND(m_objCubicSplineControlPoint[i].x - 5),
					 ROUND(m_objCubicSplineControlPoint[i].y - 5),
					 ROUND(m_objCubicSplineControlPoint[i].x + 5),
					 ROUND(m_objCubicSplineControlPoint[i].y + 5));
	}
	pDC->SelectObject(pOldBrush);
}

void CCubicSplineLine::DrawCubicSpline(CDC*pDC, int n)//绘制三次样条线
{
// 	int n=6;
	const int dim=7;//二维数组维数
	double b1=10,bn=-10;//给出起点和终点的一阶导数
	double h[dim],lambda[dim],mu[dim],D[dim];//四个数组
	double l[dim],m[dim],u[dim];//追赶法参数
	double M[dim],K[dim];//追赶法过渡矩阵
	double a[dim],b[dim],c[dim],d[dim];//函数的系数
	for(int i=1;i<n;i++)//计算hi=xi+1-xi
	{
		h[i]=m_objCubicSplineControlPoint[i+1].x - m_objCubicSplineControlPoint[i].x;
	}
	for(i=2;i<n;i++)
	{
		lambda[i]=h[i-1]/(h[i-1]+h[i]);
		mu[i]=h[i]/(h[i-1]+h[i]);
		D[i]= 6/ (h[i-1]+h[i])
				*((m_objCubicSplineControlPoint[i+1].y-m_objCubicSplineControlPoint[i].y)/
				   h[i]-(m_objCubicSplineControlPoint[i].y-m_objCubicSplineControlPoint[i-1].y)/h[i-1]);//计算Di
	}
	D[1]=6*((m_objCubicSplineControlPoint[2].y-m_objCubicSplineControlPoint[1].y)/h[1]-b1)/h[1];//夹持端的D[1]
	D[n]=6*(bn-(m_objCubicSplineControlPoint[n].y-m_objCubicSplineControlPoint[n-1].y)/h[n-1])/h[n-1];//夹持端的D[n]
	mu[1]=1;
	lambda[n]=1;
	//追赶法求解三弯矩方程
	l[1]=2;
	u[1]=mu[1]/l[1];
	for(i=2;i<=n;i++)
	{
		m[i]=lambda[i];
		l[i]=2-m[i]*u[i-1];
		u[i]=mu[i]/l[i];
	}
	K[1]=D[1]/l[1];//解LK=D方程
	for(i=2;i<=n;i++)
	{
		K[i]=(D[i]-m[i]*K[i-1])/l[i];
	}
	M[n]=K[n];//解UM=K
   for(i=n-1;i>=1;i--)
   {
	   M[i]=K[i]-u[i]*M[i+1];
   }
   //计算三次样条函数的系数
   for(i=1;i<n;i++)
   {
	   a[i]=m_objCubicSplineControlPoint[i].y;
	   b[i]=(m_objCubicSplineControlPoint[i+1].y-m_objCubicSplineControlPoint[i].y)/h[i]-h[i]*(M[i]/3+M[i+1]/6);
	   c[i]=M[i]/2;
	   d[i]=(M[i+1]-M[i])/(6*h[i]);
   }
   pDC->MoveTo(ROUND(m_objCubicSplineControlPoint[1].x), 
	           ROUND(m_objCubicSplineControlPoint[1].y));
   double xStep=0.5;//当前步长
   double x,y;//当前点
   for(i=1;i<n;i++)//循环访问每个节点
   {
	   for(x = m_objCubicSplineControlPoint[i].x; 
	       x < m_objCubicSplineControlPoint[i+1].x; x += xStep)//按照步长xStep进行计算
	   { 
		   y=a[i] + b[i] * (x-m_objCubicSplineControlPoint[i].x) 
				  + c[i] * (x-m_objCubicSplineControlPoint[i].x) 
						 * (x-m_objCubicSplineControlPoint[i].x)
			      + d[i] * (x-m_objCubicSplineControlPoint[i].x) 
				         * (x-m_objCubicSplineControlPoint[i].x)
						 * (x-m_objCubicSplineControlPoint[i].x);
		   pDC->LineTo(ROUND(x),ROUND(y));//绘制样条曲线
	   }
   }
}



void CCubicSplineLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	if(m_Points.size() < 2) 
    {
        return;
    }

	// printAllPoints("CCubicSplineLine::Draw");
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
//	这个值是从1开始的。
//	P[1].x=-340,P[1].y=-200;//读入型值点，按照横坐标从小到大排序
//	P[2].x=-150,P[2].y=0;
//	P[3].x=0,P[3].y=-50;
//	P[4].x=100,P[4].y=-100;
//	P[5].x=250,P[5].y=-100;
//	P[6].x=350,P[6].y=-50;

	int iPointsSize = m_Points.size();
	CAdjustPoint *pNext = NULL;

	m_objCubicSplineControlPoint = (POINT *)malloc(sizeof(POINT) * (iPointsSize + 1));
	for(int j = 0; j < iPointsSize; j++)
	{
		pNext = (CAdjustPoint*)m_Points[j];
		m_objCubicSplineControlPoint[j + 1].x = pNext->GetPoint().x;
		m_objCubicSplineControlPoint[j + 1].y = pNext->GetPoint().y;
	}

	if (bShowSelectBorder)
	{
		DrawDataPoint(pdc, iPointsSize + 1);
	}
	DrawCubicSpline(pdc, iPointsSize + 1);
	free(m_objCubicSplineControlPoint);
	
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

void CCubicSplineLine::DrawFocus(CDC *pdc)
{
	if(m_Points.size() < 2) 
    {
        return;
    }

	// printAllPoints("CCubicSplineLine::DrawFocus");
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
void CCubicSplineLine::DrawSelectBorderArea( CDC *pdc )
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

void CCubicSplineLine::Move(int cx, int cy)
{
	for(int i = 0; i < m_Points.size(); i++)
	{
		CAdjustPoint *pConnPoint = (CAdjustPoint*)m_Points[i];
		CPoint newPoint = pConnPoint->GetPoint() + CPoint(cx, cy);
		pConnPoint->SetPoint(newPoint);
	}
}

void CCubicSplineLine::AdjustSize(CPoint &pt)
{
	// printAllPoints("CCubicSplineLine::AdjustSize Before");
	CAdjustPoint *pFocusConnPoint = (CAdjustPoint*)m_Points[m_FocusPoint];
	if(pFocusConnPoint != NULL)
	{
		pFocusConnPoint->SetPoint(pt);
	}
	// printAllPoints("CCubicSplineLine::AdjustSize");
}

void CCubicSplineLine::SetStartPoint(CPoint &pt)
{
	if(m_Points.size() <= 0) 
    {
        return;
    }

	// printAllPoints("CCubicSplineLine::SetStartPoint Before");
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pStart->SetPoint(pt);
	// printAllPoints("CCubicSplineLine::SetStartPoint");
}

void CCubicSplineLine::SetEndPoint(CPoint &pt)
{
	CAdjustPoint *pNewPoint;
	if(!m_IsCreateEnd)
	{
		pNewPoint = new CAdjustPoint();
		pNewPoint->SetPoint(pt);
		// printAllPoints("CCubicSplineLine::SetEndPoint(NotCreateEnd) Before");
		m_Points.insert(m_Points.end(), pNewPoint);
		// printAllPoints("CCubicSplineLine::SetEndPoint(NotCreateEnd) After");

//		// PolyBezier的point数组大小必须是4
//		if (m_Points.size() == BEZIERLINE_POINTS_COUNT)
//		{
//			m_IsCreateEnd = true;
//		}
	}
	else
	{
		// printAllPoints("CCubicSplineLine::SetEndPoint(CreateEnd) Before");
		// p = (CAdjustPoint*)m_Points.GetAt(0);
		pNewPoint = (CAdjustPoint*)m_Points[m_Points.size()-1];
		pNewPoint->SetPoint(pt);
		// printAllPoints("CCubicSplineLine::SetEndPoint(CreateEnd) After");
	}
}


void CCubicSplineLine::SetLastPoint( CPoint &pt )
{
	CAdjustPoint *pLast;
	pLast = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pLast->SetPoint(pt);
	// printAllPoints("CCubicSplineLine::SetLastPoint");

}

void CCubicSplineLine::GetStartPoint(CPoint &pt)
{
	// CAdjustPoint *pStart = (CAdjustPoint*)m_Points.GetAt(m_Points.size()-1);
	CAdjustPoint *pStart = (CAdjustPoint*)m_Points[0];
	pt = pStart->GetPoint();
}

void CCubicSplineLine::GetEndPoint(CPoint &pt)
{
	// CAdjustPoint *pEnd = (CAdjustPoint*)m_Points.GetAt(0);
	CAdjustPoint *pEnd = (CAdjustPoint*)m_Points[m_Points.size()-1];
	pt = pEnd->GetPoint();
}

void CCubicSplineLine::SetPreviousGraph(CGraph *previousGraph)
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

void CCubicSplineLine::SetNextgraph(CGraph *nextGraph)
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

CGraph* CCubicSplineLine::GetPreviousGraph()
{
	return m_Previous;
}

CGraph* CCubicSplineLine::GetNextgraph()
{
	return m_Next;
}

bool CCubicSplineLine::IsEditable()
{
	return false;
}

bool CCubicSplineLine::IsControlFlow()
{
	return true;
}

bool CCubicSplineLine::IsIn(CPoint &pt)
{
	if(m_Points.size() < 2) 
    {
        return false;
    }

	// printAllPoints("CCubicSplineLine::IsIn Before");
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
	// printAllPoints("CCubicSplineLine::IsIn After");

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

bool CCubicSplineLine::IsOn(CPoint &pt)
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

double CCubicSplineLine::GetDistance(int x1, int y1, int x2,int y2)
{
	double distance = 0;

	distance=(double)((x1-x2)*(x1-x2)) + (double)((y1-y2)*(y1-y2));
	distance=sqrt(distance);

	return distance;
}

void CCubicSplineLine::DrawArrow( CDC *pdc )
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

void CCubicSplineLine::SaveParamsToJSON(cJSON * objJSON)
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

void CCubicSplineLine::LoadOnePointFromJSON(cJSON * objPoint)
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

void CCubicSplineLine::LoadParamsFromJSON(cJSON * objJSON)
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
