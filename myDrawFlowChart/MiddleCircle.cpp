// MiddleCircle.cpp: implementation of the CMiddleCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "MiddleCircle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CMiddleCircle, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CMiddleCircle::CMiddleCircle()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
}

CMiddleCircle::~CMiddleCircle()
{

}

void CMiddleCircle::MidPointCircle(CDC* pDC, int R)//定义中点画圆算法
{
	int e = 1 - R;
	int x = 0, y = R;
	for (x = 0; x <= y; x++)
	{
		// 一次画出8个分象限的对应点。
		CirclePoint(pDC, x, y);//调用八分法画圆算法
		// 对应于公式(2 - 22)
		if (e < 0)
		{
			e += 2 * x + 3;
		}
		else
		{
			// 对应于公式(2 - 23)
			e += 2 * (x - y) + 5;
			y--;
		}
	}
}

void CMiddleCircle::CirclePoint(CDC* pDC, int x, int y)//定义八分法画圆算法
{
	COLORREF  crClr = RGB(0, 0, 0);
	pDC->SetPixelV(x + m_Start.x, y + m_Start.y, crClr);
	pDC->SetPixelV(y + m_Start.x, x + m_Start.y, crClr);
	pDC->SetPixelV(y + m_Start.x, -x + m_Start.y, crClr);
	pDC->SetPixelV(x + m_Start.x, -y + m_Start.y, crClr);
	pDC->SetPixelV(-x + m_Start.x, -y + m_Start.y, crClr);
	pDC->SetPixelV(-y + m_Start.x, -x + m_Start.y, crClr);
	pDC->SetPixelV(-y + m_Start.x, x + m_Start.y, crClr);
	pDC->SetPixelV(-x + m_Start.x, y + m_Start.y, crClr);
}

void CMiddleCircle::MidPointEllipse(CDC* pDC, int majorAxis, int minorAxis)//椭圆的中点算法
{		
	// 对应于公式(2 - 32)
	double d1 = minorAxis * minorAxis + majorAxis * majorAxis * (-minorAxis + 0.25);
	int x = 0, y = minorAxis;
	while (minorAxis * minorAxis * (x + 1) < majorAxis * majorAxis * (y - 0.5))
	{
		EllipsePoint(pDC, x, y);
		if (d1 < 0)
		{
			// 对应于公式(2 - 30)
			d1 += minorAxis * minorAxis * (2 * x + 3);
		}
		else
		{
			// 对应于公式(2 - 31)
			d1 += minorAxis * minorAxis * (2 * x + 3) + majorAxis * majorAxis * (-2 * y + 2);
			y--;
		}
		x++;
	}
	// 对应于公式(2 - 33) 和 公式(2 - 38)
	double d2 = minorAxis * minorAxis * (x + 0.5) * (x + 0.5) + majorAxis * majorAxis * (y - 1) * (y - 1)
							- (double)majorAxis * majorAxis * minorAxis * minorAxis;
	while (y >= 0)
	{
		EllipsePoint(pDC, x, y);
		if (d2 < 0)
		{
			// 对应于公式(2 - 35)
			d2 += minorAxis * minorAxis * (2 * x + 2) + majorAxis * majorAxis * (-2 * y + 3);
			x++;
		}
		else
		{
			// 对应于公式(2 - 36)
			d2 += majorAxis * majorAxis * (-2 * y + 3);
		}
		y--;
	}
}

void CMiddleCircle::EllipsePoint(CDC* pDC, int x, int y)
{
	COLORREF crClr = RGB(0, 0, 0);
	pDC->SetPixelV(x + m_End.x, y + m_End.y, crClr);
	pDC->SetPixelV(x + m_End.x, -y + m_End.y, crClr);
	pDC->SetPixelV(-x + m_End.x, -y + m_End.y, crClr);
	pDC->SetPixelV(-x + m_End.x, y + m_End.y, crClr);
}

/************************************************************************/
/* 功能：绘制函数。绘制了一个椭圆和上面的文字。                         */
/************************************************************************/
void CMiddleCircle::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	m_Radius = m_End.x - m_Start.x;
	MidPointCircle(pdc, m_Radius);//调用圆的中点算法
	
	MidPointEllipse(pdc, 100, 30);//调用圆的中点算法

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CMiddleCircle::DrawFocus( CDC *pdc )
{
	// 画笔为虚线，线宽为1，颜色为黑色。
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );
	// 画一个虚线框。
	pdc->Rectangle( CRect(m_Start, m_End) );
		
	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);

	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.size(); i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		connPoint->Draw(pdc);
	}
}

/************************************************************************/
/* 功能： 移动处理函数。                                                */
/************************************************************************/
void CMiddleCircle::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CMiddleCircle::AdjustSize( CPoint &pt )
{
	switch(m_AdjustPoint)
	{
	// case 1:  // 左上角
	case CCONNECTPOINT_RECT_LEFT_TOP:
		{
			m_Start = pt;
			break;
		}
	// case 2:  // 左下角
	case CCONNECTPOINT_RECT_LEFT_BOTTOM:
		{
			m_Start.x = pt.x;
			m_End.y = pt.y;
			break;
		}
	// case 3:  // 右上角
	case CCONNECTPOINT_RECT_RIGHT_TOP:
		{
			m_Start.y = pt.y;
			m_End.x = pt.x;
			break;
		}
	// case 4:  // 右下角
	case CCONNECTPOINT_RECT_RIGHT_BOTTOM:
		{
			m_End = pt;
			break;
		}
	// case 5:
	case CCONNECTPOINT_RECT_MIDDLE_TOP:
		{
			m_Start.y = pt.y;
			break;
		}
	// case 6:
	case CCONNECTPOINT_RECT_MIDDLE_RIGHT:
		{
			m_End.x = pt.x;
			break;
		}
	// case 7:
	case CCONNECTPOINT_RECT_MIDDLE_BOTTOM:
		{
			m_End.y = pt.y;
			break;
		}
	// case 8:
	case CCONNECTPOINT_RECT_MIDDLE_LEFT:
		{
			m_Start.x = pt.x;
			break;
		}
	}
}

/************************************************************************/
/* 功能：判断是否在图元区域内。                                         */
/************************************************************************/
bool CMiddleCircle::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;

	CRgn cr;
	BOOL bRet = cr.CreateEllipticRgn( m_Start.x, m_Start.y, m_End.x, m_End.y );
	if(bRet && cr.PtInRegion( pt ))
	{
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	else if (bRet == FALSE)
	{
		printf("m_Start/m_End = {(%d, %d), (%d, %d)}", 
			m_Start.x, m_Start.y, m_End.x, m_End.y);
	}
	return flag;
}

/************************************************************************/
/* 功能： 判断一个连接点是否在图元边界上。用于调整图元是否连接。        */
/************************************************************************/
int CMiddleCircle::IsConnectOn(CAdjustPoint *pt)
{
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
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

/************************************************************************/
/* 功能： 判断一个屏幕坐标是否在图元边界上。用于调整图元大小。          */
/************************************************************************/
bool CMiddleCircle::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	// CPoint startPoint = CPoint( m_Start.x, m_End.y );
	// CPoint endPoint = CPoint(m_End.x, m_Start.y);

	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		if(connPoint->IsOn(pt))
		{
			// if(i == CCONNECTPOINT_RECT_LEFT_BOTTOM || i == CCONNECTPOINT_RECT_RIGHT_TOP)
			// {
			//	m_Start = startPoint;
			//	m_End = endPoint;
			// }
			m_AdjustPoint = i; // 1+i;
		    flag = true;
			break;
		}
	}
	return flag;
}

/************************************************************************/
/* 功能：在调整大小发生翻转的时候，根据调整结果交换起始点和结束点坐标。 */
/************************************************************************/
void CMiddleCircle::AdjustStartAndEnd()
{
	CPoint newStart, newEnd;
	if((m_End.x < m_Start.x) && (m_End.y < m_Start.y))
	{
		newEnd = m_Start;
		m_Start = m_End;
		m_End = newEnd;
	}
	else if(!((m_End.x > m_Start.x) && (m_End.y > m_Start.y)))
	{
		newStart = CPoint( m_End.x, m_Start.y );
		newEnd = CPoint( m_Start.x, m_End.y );
		m_Start = newStart;
		m_End = newEnd;
	}
}

int CMiddleCircle::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
void CMiddleCircle::AdjustFocusPoint()
{
	CAdjustPoint *connPoint = NULL;
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_LEFT_TOP];
	connPoint->SetPoint(m_Start);
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_LEFT_BOTTOM];
	connPoint->SetPoint(CPoint(m_Start.x, m_End.y));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_RIGHT_TOP];
	connPoint->SetPoint(CPoint(m_End.x, m_Start.y));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_RIGHT_BOTTOM];
	connPoint->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_RECT_CNT; i++)
	{
		connPoint = (CAdjustPoint *)m_Points[i];
		connPoint->SetType(false);
	}

	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_TOP];
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_RIGHT];
	connPoint->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_BOTTOM];
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_LEFT];
	connPoint->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

/************************************************************************/
/* 功能：串行化操作。                                                   */
/************************************************************************/
void CMiddleCircle::SaveParamsToJSON(cJSON * objJSON)
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

void CMiddleCircle::LoadParamsFromJSON(cJSON * objJSON)
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
	AdjustFocusPoint();
}