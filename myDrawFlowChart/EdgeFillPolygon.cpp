// EdgeFillPolygon.cpp: implementation of the CEdgeFillPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "EdgeFillPolygon.h"
#include "SmoothFillTriangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define ROUND(d) int(d + 0.5)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CEdgeFillPolygon, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CEdgeFillPolygon::CEdgeFillPolygon()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
	m_backClr = RGB(255, 255, 255);//背景色
	m_fillClr = RGB(0, 0, 0);//前景色
}

CEdgeFillPolygon::~CEdgeFillPolygon()
{

}

void CEdgeFillPolygon::ReadPoint(void)//顶点表
{
	CRGB color = CRGB(1, 0, 0);
	m_P[0].x = 5,    m_P[0].y = 10,  m_P[0].c = color;
	m_P[1].x = -15;  m_P[1].y = 30,  m_P[1].c = color;
	m_P[2].x = -25;  m_P[2].y = 5,   m_P[2].c = color;
	m_P[3].x = -15;  m_P[3].y = -25, m_P[3].c = color;
	m_P[4].x = 0;    m_P[4].y = -5,  m_P[4].c = color;
	m_P[5].x = 10;   m_P[5].y = -25, m_P[5].c = color;
	m_P[6].x = 30;   m_P[6].y = 15,  m_P[6].c = color;

	m_P[0].x += m_Start.x, m_P[0].y += m_Start.y; 
	m_P[1].x += m_Start.x, m_P[1].y += m_Start.y; 
	m_P[2].x += m_Start.x, m_P[2].y += m_Start.y; 
	m_P[3].x += m_Start.x, m_P[3].y += m_Start.y; 
	m_P[4].x += m_Start.x, m_P[4].y += m_Start.y; 
	m_P[5].x += m_Start.x, m_P[5].y += m_Start.y; 
	m_P[6].x += m_Start.x, m_P[6].y += m_Start.y; 
}

/************************************************************************/
/* 绘制ReadPoint给出的多边形。同时绘制一个矩形把这个多边形包裹在里面。  */
/* 这个矩形给出在绘制这个多边形过程中，XY的取值范围。                   */
/* 这样我们就用不着操作矩形外面的像素了。可以提高效率。                 */
/************************************************************************/
void CEdgeFillPolygon::DrawObject(CDC* pDC)
{
	m_xMin = m_xMax = m_P[0].x;
	m_yMin = m_yMax = m_P[0].y;
	// 1. 计算包裹这个多边形的矩形包围盒的坐标。
	//    其实就是找出来多边形的所有顶点的XY坐标的最大值和最小值。
	for (int i = 0; i < 7; i++)
	{
		if (m_P[i].x > m_xMax)
        {
			m_xMax = m_P[i].x;
        }
		if (m_P[i].x < m_xMin)
        {
			m_xMin = m_P[i].x;
        }
		if (m_P[i].y > m_yMax)
        {
			m_yMax = m_P[i].y;
        }
		if (m_P[i].y < m_yMin)
        {
			m_yMin = m_P[i].y;
        }
	}
	CColorPoint t;
	// 2. 绘制多边形
	for (i = 0; i < 7; i++)
	{
		if (0 == i)
		{
			pDC->MoveTo(m_P[i].x, m_P[i].y);
			t = m_P[i];
		}
		else
			pDC->LineTo(m_P[i].x, m_P[i].y);
	}
	pDC->LineTo(t.x, t.y);//闭合多边形
	
	// 3. 绘制包裹这个多边形的矩形包围盒
	pDC->MoveTo(m_xMin, m_yMin);
	pDC->LineTo(m_xMax, m_yMin);
	pDC->LineTo(m_xMax, m_yMax);
	pDC->LineTo(m_xMin, m_yMax);
	pDC->LineTo(m_xMin, m_yMin);
}

/************************************************************************/
/* 这个函数实现了边填充算法。这个算法的前提是填充色和背景色互为补色。   */
/* 因为对一个像素两次取补会恢复为原色。产生负负得正的效果。             */
/* 这就是这个算法能够工作的关键。                                       */
/************************************************************************/
/* 有了上面的预设前提。这个算法的原理就变得非常简单，                   */
/* 假设多边形有两个边一个在左侧，称为左侧边。一个在右侧，称为右侧边。   */
/* 首先填充左侧边右面的全部像素。这时除了多边形内部以外，               */
/* 右侧边的右侧也被填充。之后填充右侧边右面的全部像素。                 */
/* 结果就是右侧边的右侧被填充了两次。                                   */
/* 因为填充色和背景色互为补色，使用填充色进行两次填充的结果就是         */
/* 右侧边的右侧被填充成了背景色。                                       */
/* 因为对一个像素两次取补会恢复为原色。类似于负负得正的效果。           */
/* 之后从前面的两个边的情况可以推广到多边的情况。                       */
/* 这就是这个边填充算法的原理。                                         */
/************************************************************************/
void CEdgeFillPolygon::EdgeFill(CDC* pDC)
{
	int ymin, ymax;//边的最小y值与最大y值
	double x_ymin, m;//x_ymin为边低端的x坐标，m为斜率的倒数
	for (int i = 0; i < 7; i++)//循环多边形所有边
	{
		int j = (i + 1) % 7;
		m = double(m_P[i].x - m_P[j].x) / (m_P[i].y - m_P[j].y); // 计算1/k
		if (m_P[i].y < m_P[j].y)//得到每条边y的最大值与最小值
		{
			ymin   = m_P[i].y;
			ymax   = m_P[j].y;
			x_ymin = m_P[i].x;//得到x|ymin
		}
		else
		{
			ymin   = m_P[j].y;
			ymax   = m_P[i].y;
			x_ymin = m_P[j].x;
		}
		for (int y = ymin; y < ymax; y++) // 沿每一条边循环扫描线
		{
			// 对每一条扫描线与边的交点的右侧像素循环
			// 这个读写像素操作应该可以替换成为画线操作。
			for (int x = ROUND(x_ymin); x < m_xMax; x++)
			{
				if (m_fillClr == pDC->GetPixel(x, y)) // 如果原来是填充色
                {
					pDC->SetPixelV(x, y, m_backClr);  // 置为背景色
                }
				else                                // 如果原来是背景色
                {
					pDC->SetPixelV(x, y, m_fillClr);  // 置为填充色
                }
			}
			x_ymin += m;                            // 计算下一条扫描线的x起点坐标
		}
	}
}

/************************************************************************/
/* 功能：绘制函数。                                                     */
/************************************************************************/
void CEdgeFillPolygon::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	ReadPoint();
	DrawObject(pdc);
	EdgeFill(pdc);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CEdgeFillPolygon::DrawFocus( CDC *pdc )
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
void CEdgeFillPolygon::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CEdgeFillPolygon::AdjustSize( CPoint &pt )
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
bool CEdgeFillPolygon::IsIn( CPoint &pt )
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
int CEdgeFillPolygon::IsConnectOn(CAdjustPoint *pt)
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
bool CEdgeFillPolygon::IsOn( CPoint &pt )
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
void CEdgeFillPolygon::AdjustStartAndEnd()
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

int CEdgeFillPolygon::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
void CEdgeFillPolygon::AdjustFocusPoint()
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
void CEdgeFillPolygon::SaveParamsToJSON(cJSON * objJSON)
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

void CEdgeFillPolygon::LoadParamsFromJSON(cJSON * objJSON)
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
