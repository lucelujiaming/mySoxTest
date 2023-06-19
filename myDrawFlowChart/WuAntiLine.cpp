// WuAntiLine.cpp: implementation of the CWuAntiLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "WuAntiLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CWuAntiLine, CObject, 1)

CWuAntiLine::CWuAntiLine()
{

}

CWuAntiLine::~CWuAntiLine()
{

}

//使用Wu算法进行反走样的Bresenham算法直线绘制函数。
void CWuAntiLine::BresenhamWuAntiLine(CDC* pDC, CPoint P0, CPoint P1)
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
	// Wu算法的像素深度。
	double pixelDepth = 0.0;
	for (int i = 1; i <= dx; i++)
	{
		pDC->SetPixelV(x, y, RGB(pixelDepth * 255, pixelDepth * 255, pixelDepth * 255));
		pDC->SetPixelV(x, y + 1, RGB((1 - pixelDepth) * 255, (1 - pixelDepth) * 255, (1 - pixelDepth) * 255));
		
		// 如果没有交换递增方向，那么在X方向上进行累加。
		if (bInterchange == false)
		{
			x += signX;
			pixelDepth += dx/dy;
			if (pixelDepth >= 1.0)
			{
				pixelDepth--;
			}
		}
		// 否则在Y方向上进行累加。
		else
		{
			y += signY;
			pixelDepth += dy/dx;
			if (pixelDepth >= 1.0)
			{
				pixelDepth--;
			}
		}
		// 沿递增方向每递增一个单位，误差累加2 * dy。
		e += 2 * dy;
		// 当误差大于零。
		if (e >= 0)
		{
			// 在递增方向上递增。否则就不递增。
			if (bInterchange == false)
				y += signY;
			else
				x += signX;
			// 并且更新误差为前一个误差减去2 * dx。
			e -= 2 * dx;
		}
	}
}

void CWuAntiLine::WuAntiLine(CDC* pDC, CPoint P0, CPoint P1)
{
	int dx = P1.x - P0.x;
	int dy = P1.y - P0.y;
	double k = double(dy) / dx;
	double pixelDepth = 0.0;
	for (int x = P0.x, y = P0.y; x < P1.x; x++)
	{
		pDC->SetPixelV(x, y, RGB(pixelDepth * 255, pixelDepth * 255, pixelDepth * 255));
		pDC->SetPixelV(x, y + 1, RGB((1 - pixelDepth) * 255, (1 - pixelDepth) * 255, (1 - pixelDepth) * 255));
		pixelDepth += k;
		if (pixelDepth >= 1.0)
		{
			y++;
			pixelDepth--;
		}
	}
}

void CWuAntiLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
//	pdc->MoveTo(m_Start);
//	pdc->LineTo(m_End);
//	WuAntiLine(pdc, m_Start, m_End);
	BresenhamWuAntiLine(pdc, m_Start, m_End);

	if (bShowSelectBorder)
	{
		DrawSelectBorderArea(pdc);
	}
}

void CWuAntiLine::DrawFocus( CDC *pdc )
{	
	CBrush brush(RGB(0,255,0));
	CBrush* oldbrush = pdc->SelectObject(&brush);

	pdc->Ellipse( CRect(m_Start+CPoint(3, 3), m_Start+CPoint(-3, -3)) );
	pdc->Ellipse( CRect(m_End+CPoint(3, 3), m_End+CPoint(-3, -3)) );

	pdc->SelectObject(oldbrush);
}

#define DRAW_FRAME
void CWuAntiLine::DrawSelectBorderArea( CDC *pdc )
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

void CWuAntiLine::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

void CWuAntiLine::AdjustSize( CPoint &pt )
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

void CWuAntiLine::GetStartPoint( CPoint &pt )
{
	pt = m_Start;
}

void CWuAntiLine::GetEndPoint( CPoint &pt )
{
	pt = m_End;
}

void CWuAntiLine::SetStartPoint( CPoint &pt )
{
	m_Start = pt;
}

void CWuAntiLine::SetEndPoint( CPoint &pt )
{
	m_End = pt;
}

bool CWuAntiLine::IsSelected( CPoint &pt )
{
	return (IsIn( pt ) || IsOn( pt ));
}

bool CWuAntiLine::IsIn( CPoint &pt )
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

bool CWuAntiLine::IsOn( CPoint &pt )
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

bool CWuAntiLine::IsEditable()
{
	return false;
}

int CWuAntiLine::GetAdjustPoint()
{
	return m_AdjustPoint;
}

void CWuAntiLine::SaveParamsToJSON(cJSON * objJSON)
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

void CWuAntiLine::LoadParamsFromJSON(cJSON * objJSON)
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