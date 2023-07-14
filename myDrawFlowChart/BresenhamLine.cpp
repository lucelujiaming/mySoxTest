// BresenhamLine.cpp: implementation of the CBresenhamLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "BresenhamLine.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CBresenhamLine, CObject, 1)

CBresenhamLine::CBresenhamLine()
{

}

CBresenhamLine::~CBresenhamLine()
{

}


void CBresenhamLine::OldBresenhamLine(CDC* pDC, CPoint P0, CPoint P1)//整型Bresenham算法
{
	COLORREF crColor = RGB(0, 0, 0);
	int dx = P1.x - P0.x;
	int dy = P1.y - P0.y;
	// 令ei+1 = di+1 - 0.5
	int e = -dx;
	for (int x = P0.x, y = P0.y; x < P1.x; x++)
	{
		pDC->SetPixelV(x, y, crColor);
		e += 2 * dy;
		if (e >= 0)
		{
			y++;
			e -= 2 * dx;
		}
	}
}

#define BRESENHAM_CIRCLE_SPAN   10
/************************************************************************/
/* 这个算法的思想是这样的。我们首先假设dx > dy，                        */
/* 那么为了绘制从P0到P1的直线，我们首先肯定是要在X方向进行累加。        */
/* 而计算下一个点的位置。在DDA算法中，我们使用ROUND进行四舍五入。       */
/************************************************************************/
/* 而Bresenham算法的原理与之类似。 我们首先假设dx > dy，                */ 
/* 首先设定e的初值为-dx。作为不存在的-1轮的残留误差。                   */ 
/* 这个理论上为-dy也是可以的。但是因为dx > dy，我们需要优先在X方向累加。*/ 
/* 所以设定e的初值为-dx。                                               */ 
/* 因为e的初值为-dx。因此上首先在X方向上进行累加。因为是四舍五入，      */ 
/* 那就意味着误差增加了2 * dy。如果这个时候，我们在Y方向上不累加。      */
/* 而那这个误差就会保留到下一轮。而如果我们在Y方向上累加。              */
/* 那就意味着误差减少了2 * dx。                                         */
/* 说的更形象一点就是，本来绘制斜线就是绘制一个台阶折线的过程。         */
/* 这些算法绕来绕去其实都是一个目的，就是通过累加在直线上下来回跳跃。   */
/************************************************************************/
/* 之后是代码实现中一个巧妙的地方。就是如果X方向的累加值比Y方向的大，   */
/* 就往X方向移动，否则就往Y方向移动。 反之dy > dx，其实也是一样的。     */
/* 只需要修改累加方向即可。且累加方向可以为负。由signX和signY控制。     */
/************************************************************************/
void CBresenhamLine::BresenhamLine(CDC* pDC, CPoint P0, CPoint P1)//通用整数Bresenham算法
{
	COLORREF crColor = RGB(0, 0, 0);
	int dx = abs(P1.x - P0.x);
	int dy = abs(P1.y - P0.y);
	bool bInterchange = false;
	int e, signX, signY, temp;
	// 计算X累加方向和Y累加方向
	signX = (P1.x > P0.x) ? 1 : ((P1.x < P0.x) ? -1 : 0);
	signY = (P1.y > P0.y) ? 1 : ((P1.y < P0.y) ? -1 : 0);
	// 如果Y方向的增量大，交换递增方向。
	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		bInterchange = true;	// 是否交换递增方向。
	}
	// 改进的整数Bresenham算法设定e的初值为-dx。
	e = -dx;
	int x = P0.x, y = P0.y;
	for (int i = 1; i <= dx; i++)
	{
		pDC->SetPixelV(x, y, crColor);
		// 如果没有交换递增方向，那么在X方向上进行累加。
		if (bInterchange == false)
        {
			x += signX;
        }
		// 否则在Y方向上进行累加。
		else
        {
			y += signY;
        }
		// 我们沿递增方向每递增一个单位，误差累加2 * dy。
		e += 2 * dy;
		// 当误差大于零。
		if (e >= 0)
		{
			// 在递增方向上递增。否则就不递增。
			if (bInterchange == false)
            {
				y += signY;
            }
			else
            {
				x += signX;
            }
			// 我们沿递增方向的垂直方向每递增一个单位，误差减去2 * dx。
			e -= 2 * dx;
		}
	}
	pDC->TextOut(P0.x - BRESENHAM_CIRCLE_SPAN, 
			P0.y + m_Radius + BRESENHAM_CIRCLE_SPAN, CString("Bresenham绘制"));
}

void CBresenhamLine::CDCLine(CDC* pDC, CPoint P0, CPoint P1)//CDC类成员函数绘图
{
	pDC->MoveTo(P0);
	pDC->LineTo(P1);	
	pDC->TextOut(P0.x - BRESENHAM_CIRCLE_SPAN, 
		P0.y + m_Radius + BRESENHAM_CIRCLE_SPAN, CString("CDC绘制"));
}

// 这个函数有点问题。没有考虑到累加方向和是否需要交换递增方向。
// 下面的修改参照了BresenhamLine函数。
/************************************************************************/
/* 这个算法的思想其实还是没有脱离这两个原则。即：                       */
/*     每一步。无论如何，我们都会沿递增方向每递增一个单位，             */
/*     这会导致误差累加2 * dy。                                         */
/*     当误差大于零的时候，我们沿递增方向的垂直方向递增一个单位，       */
/*     这会导致误差减去2 * dx。                                         */
/************************************************************************/
/* 中点算法综合这两种情况，同时做了反向。就是：                         */
/*     每一步。无论如何，我们都会沿递增方向递增一个单位，               */
/*     如果上一轮的误差小于零，则本轮会同时减少2 * dy和增加2 * dx。     */
/*     如果上一轮的误差大于零，则本轮只会减少2 * dy。                   */
/************************************************************************/
void CBresenhamLine::MidPointLine(CDC* pDC, CPoint P0, CPoint P1)//定义中点算法
{
	bool bInterchange = false;
	int signX, signY, temp;
	// 计算X累加方向和Y累加方向
	signX = (P1.x > P0.x) ? 1 : ((P1.x < P0.x) ? -1 : 0);
	signY = (P1.y > P0.y) ? 1 : ((P1.y < P0.y) ? -1 : 0);
	
	COLORREF crClr = RGB(0, 0, 0);
	int dx = abs(P1.x - P0.x);
	int dy = abs(P1.y - P0.y);
	
	// 如果Y方向的增量大，交换递增方向。
	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		bInterchange = true;	// 是否交换递增方向。
	}
	
	int e = dx - 2 * dy;
	// 如果上一轮的误差小于零，则本轮会同时减少2 * dy和增加2 * dx。
	int incrUp = 2 * dx - 2 * dy;
	// 如果上一轮的误差大于零，则本轮只会减少2 * dy。
	int incrDown = -2 * dy;
	
	int x = P0.x, y = P0.y;
	if(P1.x >= P0.x)
	{
		for (int i = 1; i <= dx; i++)
		{
			pDC->SetPixelV(x, y, crClr);
			// 每一步。无论如何，我们都会沿递增方向递增一个单位，
			// 如果没有交换递增方向，那么在X方向上进行累加。
			if (bInterchange == false)
            {
				x += signX;
            }
			// 否则在Y方向上进行累加。
			else
            {
				y += signY;
            }
			// 如果上一轮的误差小于零，则本轮会同时减少2 * dy和增加2 * dx。
			if (e < 0)
			{
				// 在递增方向上递增。否则就不递增。
				if (bInterchange == false)
				{
					y += signY;
				}
				else
				{
					x += signX;
				}
				// 更新误差
				e += incrUp * signX;
			}
			// 如果上一轮的误差大于零，则本轮只会减少2 * dy。
			else
			{
				e += incrDown * signX;
			}
		}
	}
	else 
	{
		for (int i = 1; i <= dx; i++)
		{
			pDC->SetPixelV(x, y, crClr);
			// 如果没有交换递增方向，那么在X方向上进行累加。
			if (bInterchange == false)
            {
				x += signX;
            }
			// 否则在Y方向上进行累加。
			else
            {
				y += signY;
            }
			// 当误差大于零。
			if (e < 0)
			{
				// 在递增方向上递增。否则就不递增。
				if (bInterchange == false)
				{
					y += signY;
					// 更新误差
					e -= incrUp * signX;
				}
				else
				{
					x += signX;
					e -= incrUp * signX;
				}
			}
			else
			{
				if (bInterchange == false)
				{
					e -= incrDown * signX;
				}
				else
				{
					e -= incrDown * signX;
				}
			}
		}
	}

}

#define PI 3.1415926
#define ROUND(d) int(d + 0.5)
void CBresenhamLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
//	pdc->MoveTo(m_Start);
//	pdc->LineTo(m_End);
// 	OldBresenhamLine(pdc, m_Start, m_End);//调用Bresenham算法

	CPoint P1;//直线终点坐标
	m_Radius = abs(m_End.x - m_Start.x)/2 - BRESENHAM_CIRCLE_SPAN;
//	CPoint PLeft(m_Start.x + radius, (m_End.y - m_Start.y)/2);//左圆心
//	CPoint PRight(m_End.x - radius, (m_End.y - m_Start.y)/2);//右圆心

//	for (int nAngle = 0; nAngle < 360; nAngle += 20)
//	{
//		P1.x = ROUND(m_Radius * cos(nAngle * PI / 180)) + m_Start.x;
//		P1.y = ROUND(m_Radius * sin(nAngle * PI / 180)) + m_Start.y;
//		BresenhamLine(pdc, m_Start, P1);//调用Bresenham算法绘图
//		P1.x = ROUND(m_Radius * cos(nAngle * PI / 180)) + m_End.x;
//		P1.y = ROUND(m_Radius * sin(nAngle * PI / 180)) + m_End.y;
//		CDCLine(pdc, m_End, P1);//调用CDC类成员函数绘图
//	}
	
	// 这个函数原来有点问题。被我改写了。
 	MidPointLine(pdc, m_Start, m_End);//调用中点算法

	if (bShowSelectBorder)
	{
		DrawSelectBorderArea(pdc);
	}
}

void CBresenhamLine::DrawFocus( CDC *pdc )
{	
	CBrush brush(RGB(0,255,0));
	CBrush* oldbrush = pdc->SelectObject(&brush);

	pdc->Ellipse( CRect(m_Start+CPoint(3, 3), m_Start+CPoint(-3, -3)) );
	pdc->Ellipse( CRect(m_End+CPoint(3, 3), m_End+CPoint(-3, -3)) );

	pdc->SelectObject(oldbrush);
}

#define DRAW_FRAME
void CBresenhamLine::DrawSelectBorderArea( CDC *pdc )
{
#ifdef DRAW_FRAME
	// 画笔为虚线，线宽为1，颜色为紫色。
	CPen greenPen( PS_DOT, 1, RGB(255, 0, 128) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&greenPen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );

	CPoint points[4];
	int marginX = 0;
	int marginY = 0;
	if(abs(m_End.x - m_Start.x) > abs(m_End.y - m_Start.y))
	{
		marginY = ADJUSTPOINT_POSITIVE_X_MARGIN;
	}
	else
	{
		marginX = ADJUSTPOINT_POSITIVE_Y_MARGIN;
	}
	
	CPoint marginXY = CPoint(marginX, marginY);
	points[0] = m_Start - marginXY;
	points[1] = m_Start + marginXY;
	points[2] = m_End + marginXY;
	points[3] = m_End - marginXY;

	pdc->Polygon(points, 4);
	
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);
#endif
}

void CBresenhamLine::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CBresenhamLine::AdjustSize( CPoint &pt )
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

void CBresenhamLine::GetStartPoint( CPoint &pt )
{
	pt = m_Start;
}

void CBresenhamLine::GetEndPoint( CPoint &pt )
{
	pt = m_End;
}

void CBresenhamLine::SetStartPoint( CPoint &pt )
{
	m_Start = pt;
}

void CBresenhamLine::SetEndPoint( CPoint &pt )
{
	m_End = pt;
}

bool CBresenhamLine::IsSelected( CPoint &pt )
{
	return (IsIn( pt ) || IsOn( pt ));
}

bool CBresenhamLine::IsIn( CPoint &pt )
{
	bool flag = false;

	CPoint points[4];
	int marginX = 0;
	int marginY = 0;
	if(abs(m_End.x - m_Start.x) > abs(m_End.y - m_Start.y))
	{
		marginY = ADJUSTPOINT_POSITIVE_X_MARGIN;
	}
	else
	{
		marginX = ADJUSTPOINT_POSITIVE_Y_MARGIN;
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

bool CBresenhamLine::IsOn( CPoint &pt )
{
	bool flag = false;
	CRect startPoint = CRect(m_Start + CPoint(
									ADJUSTPOINT_NEGATIVE_X_MARGIN, 
									ADJUSTPOINT_NEGATIVE_Y_MARGIN), 
						m_Start + CPoint(
									ADJUSTPOINT_POSITIVE_X_MARGIN, 
									ADJUSTPOINT_POSITIVE_Y_MARGIN));
	CRect endPoint = CRect(m_End + CPoint(
									ADJUSTPOINT_NEGATIVE_X_MARGIN, 
									ADJUSTPOINT_NEGATIVE_Y_MARGIN), 
						m_End+CPoint(
									ADJUSTPOINT_POSITIVE_X_MARGIN, 
									ADJUSTPOINT_POSITIVE_Y_MARGIN));

	if(startPoint.PtInRect(pt))
	{ // 起点
		m_AdjustPoint = CCONNECTPOINT_LINE_START; // 1;
		flag = true;
	}
	else if(endPoint.PtInRect(pt))
	{ // 终点
		m_AdjustPoint = CCONNECTPOINT_LINE_END; // 2;
		flag = true;
	}

	return flag;
}

bool CBresenhamLine::IsEditable()
{
	return false;
}

int CBresenhamLine::GetAdjustPoint()
{
	return m_AdjustPoint;
}

void CBresenhamLine::SaveParamsToJSON(cJSON * objJSON)
{
//	if(ar.IsStoring())
//	{
//		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
//	}
//	else
//	{
//		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
//	}
	cJSON * jsonGraph = cJSON_CreateObject();
	cJSON_AddStringToObject(jsonGraph, "Type", GetTypeName());
	cJSON_AddNumberToObject(jsonGraph, "GraphSeq", getGraphSeq());
	
	cJSON * jsonStart = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonStart, "x", m_Start.x);
	cJSON_AddNumberToObject(jsonStart, "y", m_Start.y);
	cJSON_AddItemToObject(jsonGraph, "Start", jsonStart);

	cJSON * jsonEnd = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonEnd, "x", m_End.x);
	cJSON_AddNumberToObject(jsonEnd, "y", m_End.y);
	cJSON_AddItemToObject(jsonGraph, "End", jsonEnd);

	cJSON_AddStringToObject(jsonGraph, "Text", m_text);
	cJSON_AddNumberToObject(jsonGraph, "currentAdjustPoint", m_AdjustPoint);
	
	cJSON_AddItemToObject(objJSON, GetTypeName(), jsonGraph);
}

void CBresenhamLine::LoadParamsFromJSON(cJSON * objJSON)
{
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
				else if(strcmp(child->string, "currentAdjustPoint") == 0)
                {   
					m_AdjustPoint = (int)child->valueint;
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
                if(strcmp(child->string, "Start") == 0)
                {   
					cJSON *grandChild = child->child;
					while(grandChild)
					{
						switch ((grandChild->type)&255)
						{   
						case cJSON_Number:    
							if(strcmp(grandChild->string, "x") == 0)
							{   
								m_Start.x = (int)grandChild->valueint;
							}
							else if(strcmp(grandChild->string, "y") == 0)
							{   
								m_Start.y = (int)grandChild->valueint;
							}
							break;
						}
						grandChild = grandChild->next ;
					}
                }
				else if(strcmp(child->string, "End") == 0)
                {   
					cJSON *grandChild = child->child;
					while(grandChild)
					{
						switch ((grandChild->type)&255)
						{   
						case cJSON_Number:    
							if(strcmp(grandChild->string, "x") == 0)
							{   
								m_End.x = (int)grandChild->valueint;
							}
							else if(strcmp(grandChild->string, "y") == 0)
							{   
								m_End.y = (int)grandChild->valueint;
							}
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
