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


void CBresenhamLine::OldBresenhamLine(CDC* pDC, CPoint P0, CPoint P1)//����Bresenham�㷨
{
	COLORREF crColor = RGB(0, 0, 0);
	int dx = P1.x - P0.x;
	int dy = P1.y - P0.y;
	// ��ei+1 = di+1 - 0.5
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
/* ����㷨��˼���������ġ��������ȼ���dx > dy��                        */
/* ��ôΪ�˻��ƴ�P0��P1��ֱ�ߣ��������ȿ϶���Ҫ��X��������ۼӡ�        */
/* ��������һ�����λ�á���DDA�㷨�У�����ʹ��ROUND�����������롣       */
/************************************************************************/
/* ��Bresenham�㷨��ԭ����֮���ơ� �������ȼ���dx > dy��                */ 
/* �����趨e�ĳ�ֵΪ-dx����Ϊ�����ڵ�-1�ֵĲ�����                   */ 
/* ���������Ϊ-dyҲ�ǿ��Եġ�������Ϊdx > dy��������Ҫ������X�����ۼӡ�*/ 
/* �����趨e�ĳ�ֵΪ-dx��                                               */ 
/* ��Ϊe�ĳ�ֵΪ-dx�������������X�����Ͻ����ۼӡ���Ϊ���������룬      */ 
/* �Ǿ���ζ�����������2 * dy��������ʱ��������Y�����ϲ��ۼӡ�      */
/* ����������ͻᱣ������һ�֡������������Y�������ۼӡ�              */
/* �Ǿ���ζ����������2 * dx��                                         */
/* ˵�ĸ�����һ����ǣ���������б�߾��ǻ���һ��̨�����ߵĹ��̡�         */
/* ��Щ�㷨������ȥ��ʵ����һ��Ŀ�ģ�����ͨ���ۼ���ֱ������������Ծ��   */
/************************************************************************/
/* ֮���Ǵ���ʵ����һ������ĵط����������X������ۼ�ֵ��Y����Ĵ�   */
/* ����X�����ƶ����������Y�����ƶ��� ��֮dy > dx����ʵҲ��һ���ġ�     */
/* ֻ��Ҫ�޸��ۼӷ��򼴿ɡ����ۼӷ������Ϊ������signX��signY���ơ�     */
/************************************************************************/
void CBresenhamLine::BresenhamLine(CDC* pDC, CPoint P0, CPoint P1)//ͨ������Bresenham�㷨
{
	COLORREF crColor = RGB(0, 0, 0);
	int dx = abs(P1.x - P0.x);
	int dy = abs(P1.y - P0.y);
	bool bInterchange = false;
	int e, signX, signY, temp;
	// ����X�ۼӷ����Y�ۼӷ���
	signX = (P1.x > P0.x) ? 1 : ((P1.x < P0.x) ? -1 : 0);
	signY = (P1.y > P0.y) ? 1 : ((P1.y < P0.y) ? -1 : 0);
	// ���Y����������󣬽�����������
	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		bInterchange = true;	// �Ƿ񽻻���������
	}
	// �Ľ�������Bresenham�㷨�趨e�ĳ�ֵΪ-dx��
	e = -dx;
	int x = P0.x, y = P0.y;
	for (int i = 1; i <= dx; i++)
	{
		pDC->SetPixelV(x, y, crColor);
		// ���û�н�������������ô��X�����Ͻ����ۼӡ�
		if (bInterchange == false)
        {
			x += signX;
        }
		// ������Y�����Ͻ����ۼӡ�
		else
        {
			y += signY;
        }
		// �����ص�������ÿ����һ����λ������ۼ�2 * dy��
		e += 2 * dy;
		// ���������㡣
		if (e >= 0)
		{
			// �ڵ��������ϵ���������Ͳ�������
			if (bInterchange == false)
            {
				y += signY;
            }
			else
            {
				x += signX;
            }
			// �����ص�������Ĵ�ֱ����ÿ����һ����λ������ȥ2 * dx��
			e -= 2 * dx;
		}
	}
	pDC->TextOut(P0.x - BRESENHAM_CIRCLE_SPAN, 
			P0.y + m_Radius + BRESENHAM_CIRCLE_SPAN, CString("Bresenham����"));
}

void CBresenhamLine::CDCLine(CDC* pDC, CPoint P0, CPoint P1)//CDC���Ա������ͼ
{
	pDC->MoveTo(P0);
	pDC->LineTo(P1);	
	pDC->TextOut(P0.x - BRESENHAM_CIRCLE_SPAN, 
		P0.y + m_Radius + BRESENHAM_CIRCLE_SPAN, CString("CDC����"));
}

// ��������е����⡣û�п��ǵ��ۼӷ�����Ƿ���Ҫ������������
// ������޸Ĳ�����BresenhamLine������
/************************************************************************/
/* ����㷨��˼����ʵ����û������������ԭ�򡣼���                       */
/*     ÿһ����������Σ����Ƕ����ص�������ÿ����һ����λ��             */
/*     ��ᵼ������ۼ�2 * dy��                                         */
/*     �����������ʱ�������ص�������Ĵ�ֱ�������һ����λ��       */
/*     ��ᵼ������ȥ2 * dx��                                         */
/************************************************************************/
/* �е��㷨�ۺ������������ͬʱ���˷��򡣾��ǣ�                         */
/*     ÿһ����������Σ����Ƕ����ص����������һ����λ��               */
/*     �����һ�ֵ����С���㣬���ֻ�ͬʱ����2 * dy������2 * dx��     */
/*     �����һ�ֵ��������㣬����ֻ�����2 * dy��                   */
/************************************************************************/
void CBresenhamLine::MidPointLine(CDC* pDC, CPoint P0, CPoint P1)//�����е��㷨
{
	bool bInterchange = false;
	int signX, signY, temp;
	// ����X�ۼӷ����Y�ۼӷ���
	signX = (P1.x > P0.x) ? 1 : ((P1.x < P0.x) ? -1 : 0);
	signY = (P1.y > P0.y) ? 1 : ((P1.y < P0.y) ? -1 : 0);
	
	COLORREF crClr = RGB(0, 0, 0);
	int dx = abs(P1.x - P0.x);
	int dy = abs(P1.y - P0.y);
	
	// ���Y����������󣬽�����������
	if (dy > dx)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		bInterchange = true;	// �Ƿ񽻻���������
	}
	
	int e = dx - 2 * dy;
	// �����һ�ֵ����С���㣬���ֻ�ͬʱ����2 * dy������2 * dx��
	int incrUp = 2 * dx - 2 * dy;
	// �����һ�ֵ��������㣬����ֻ�����2 * dy��
	int incrDown = -2 * dy;
	
	int x = P0.x, y = P0.y;
	if(P1.x >= P0.x)
	{
		for (int i = 1; i <= dx; i++)
		{
			pDC->SetPixelV(x, y, crClr);
			// ÿһ����������Σ����Ƕ����ص����������һ����λ��
			// ���û�н�������������ô��X�����Ͻ����ۼӡ�
			if (bInterchange == false)
            {
				x += signX;
            }
			// ������Y�����Ͻ����ۼӡ�
			else
            {
				y += signY;
            }
			// �����һ�ֵ����С���㣬���ֻ�ͬʱ����2 * dy������2 * dx��
			if (e < 0)
			{
				// �ڵ��������ϵ���������Ͳ�������
				if (bInterchange == false)
				{
					y += signY;
				}
				else
				{
					x += signX;
				}
				// �������
				e += incrUp * signX;
			}
			// �����һ�ֵ��������㣬����ֻ�����2 * dy��
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
			// ���û�н�������������ô��X�����Ͻ����ۼӡ�
			if (bInterchange == false)
            {
				x += signX;
            }
			// ������Y�����Ͻ����ۼӡ�
			else
            {
				y += signY;
            }
			// ���������㡣
			if (e < 0)
			{
				// �ڵ��������ϵ���������Ͳ�������
				if (bInterchange == false)
				{
					y += signY;
					// �������
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
// 	OldBresenhamLine(pdc, m_Start, m_End);//����Bresenham�㷨

	CPoint P1;//ֱ���յ�����
	m_Radius = abs(m_End.x - m_Start.x)/2 - BRESENHAM_CIRCLE_SPAN;
//	CPoint PLeft(m_Start.x + radius, (m_End.y - m_Start.y)/2);//��Բ��
//	CPoint PRight(m_End.x - radius, (m_End.y - m_Start.y)/2);//��Բ��

//	for (int nAngle = 0; nAngle < 360; nAngle += 20)
//	{
//		P1.x = ROUND(m_Radius * cos(nAngle * PI / 180)) + m_Start.x;
//		P1.y = ROUND(m_Radius * sin(nAngle * PI / 180)) + m_Start.y;
//		BresenhamLine(pdc, m_Start, P1);//����Bresenham�㷨��ͼ
//		P1.x = ROUND(m_Radius * cos(nAngle * PI / 180)) + m_End.x;
//		P1.y = ROUND(m_Radius * sin(nAngle * PI / 180)) + m_End.y;
//		CDCLine(pdc, m_End, P1);//����CDC���Ա������ͼ
//	}
	
	// �������ԭ���е����⡣���Ҹ�д�ˡ�
 	MidPointLine(pdc, m_Start, m_End);//�����е��㷨

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
	// ����Ϊ���ߣ��߿�Ϊ1����ɫΪ��ɫ��
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
	// case 1: // ���
	case CCONNECTPOINT_LINE_START:
		{
			m_Start = pt;
			break;
		}
	// case 2: // �յ�
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
	{ // ���
		m_AdjustPoint = CCONNECTPOINT_LINE_START; // 1;
		flag = true;
	}
	else if(endPoint.PtInRect(pt))
	{ // �յ�
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
