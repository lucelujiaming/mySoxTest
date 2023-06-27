// CohenSutherlandClipLine.cpp: implementation of the CCohenSutherlandClipLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "CohenSutherlandClipLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CCohenSutherlandClipLine, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CCohenSutherlandClipLine::CCohenSutherlandClipLine()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
}

CCohenSutherlandClipLine::~CCohenSutherlandClipLine()
{

}

#define AREA_LEFT   0X1   // 代表:0001
#define AREA_RIGHT  0X2   // 代表:0010
#define AREA_BOTTOM 0X4   // 代表:0100
#define AREA_TOP    0X8   // 代表:1000

/************************************************************************/
/* 端点编码函数                                                         */
/* 裁剪窗口有四条边界。将屏幕分为可见侧和不可见侧。                     */
/* 可以用一个比特位表示。                                               */
/* 四条边界就可以用四个比特位标识。                                     */
/* 我们使用第0个比特位标识左边界。定义为AREA_LEFT。定义如上。           */
/* 我们使用第1个比特位标识右边界。定义为AREA_RIGHT。定义如上。          */
/* 我们使用第2个比特位标识下边界。定义为AREA_BOTTOM。定义如上。         */
/* 我们使用第3个比特位标识上边界。定义为AREA_TOP。定义如上。            */
/* 四条边界把整个屏幕分为9个区域。                                      */
/* 我们需要裁剪的直线的坐标必然位于这9个区域中。                        */
/* 当然，4个比特位是可以表示16种情况的。但是却被用来描述9个区域。       */
/* 因为这里有一些情况是不会出现，                                       */
/* 例如一个点不可能同时处于边界的左侧和右侧。                           */
/* 也就是类似于：0011,1100这种值是不会出现。                            */
/* 下面这个函数就是为一个坐标点设定区域码的。                           */
/************************************************************************/
void CCohenSutherlandClipLine::Encode(CAreaPoint &pt)
{
	pt.areaCode = 0;
	if (pt.x < m_Start.x)
		pt.areaCode = pt.areaCode | AREA_LEFT;
	else if (pt.x > m_End.x)
		pt.areaCode = pt.areaCode | AREA_RIGHT;
	if (pt.y < m_Start.y)
		pt.areaCode = pt.areaCode | AREA_BOTTOM;
	else if (pt.y > m_End.y)
		pt.areaCode = pt.areaCode | AREA_TOP;
}

/************************************************************************/
/* Cohen-Sutherland裁剪算法步骤如下：                                   */
/* 1. 对于需要裁剪直线的两个坐标点进行区域编码。                        */
/* 2. 如果直线两个坐标点的编码都是0。                                   */
/*    说明两个坐标点都在边界内。不需要裁剪。                            */
/* 3. 如果直线两个坐标点的编码求与不为0。                               */
/*    说明两个坐标点位于同一个边界外区域内。                            */
/*    因为只有这样，相应区域的比特位，才会同时为1。                     */
/*    此时直接舍弃即可，也不需要裁剪。                                  */
/* 4. 如果两个坐标不满足上面的两种情况。那么就需要裁剪了。              */
/*    4.1 我们首先处理第一个坐标。这里有一个技巧。                      */
/*        就是直线的第一个坐标点有可能位于边界内。                      */
/*        因此上，如果发生这种情况，我们需要交换两个坐标点。            */
/*        保证我们处理的第一个坐标点位于边界外。                        */
/*    4.2 接着我们按左、右、下、上的顺序裁剪直线段。                    */
/*        也就是说，如果一个坐标点位于边界外的右下角。                  */
/*        那么首先我们把它裁剪到右边界的边界延长线上。                  */
/*        之后，在下一轮循环中，把它裁剪到下边界上面。                  */
/*        当然，如果如果一个坐标点位于边界外的右侧。                    */
/*        则只需一轮循环就可以完成裁剪。                                */
/*  5. 我们对于裁剪后的坐标再次进行区域编码。之后作为第一个坐标。       */
/*     因为我们在4.1中始终保证处理的是第一个坐标点。                    */
/*  6. 之后回到第2步。因为我们每一轮循环都会使用一个边界进行一次裁剪。  */
/*     因此上，如果一个点位于边界内，另一个点位于右下角，               */
/*     那么两轮循环就可以让两个坐标的点编码都是0。                      */
/*     而如果，一个点位于边界内，另一个点位于右侧，那么一轮循环即可。   */
/*     以此类推，只需最多四轮循环，                                     */
/*     也就是一个点位于左上角，一个点位于右下角，就可以完成裁剪。       */
/************************************************************************/
void CCohenSutherlandClipLine::CohenSutherlandClip(void)//
{
	// 1. 对于需要裁剪直线的两个坐标点进行区域编码。 
	Encode(P0), Encode(P1);   // 对于直线起点、终点进行编码，给出区域码。
	CAreaPoint Intersection;  // 边与直线的交点
	// 处理至少一个顶点在窗口之外的情况
	// 2. 如果直线两个坐标点的编码都是0。说明两个坐标点都在边界内。不需要裁剪。
	while (P0.areaCode != 0 || P1.areaCode != 0)
	{
		// 3. 如果直线两个坐标点的编码求与不为0。
		//    说明两个坐标点位于同一个边界外区域内。
		//    此时直接舍弃即可，也不需要裁剪。
		if ((P0.areaCode & P1.areaCode) != 0)// 简弃之
		{
			return;
		}
		// 4.1 这里有一个技巧。就是直线的第一个坐标点有可能位于边界内。
		//     因此上，如果发生这种情况，我们需要交换两个坐标点。  
		//     保证我们处理的第一个坐标点位于边界外。
		if (0 == P0.areaCode) // 交换P0和P1确保P0位于窗口之外
		{
			CAreaPoint Temp;
			Temp = P0;
			P0 = P1;
			P1 = Temp;
		}
		UINT OutCode = P0.areaCode;
		// 4.2 窗口边界按左、右、下、上的顺序裁剪直线段
		if (OutCode & AREA_LEFT)//P0点位于窗口的左侧
		{
			Intersection.x = m_Start.x;//计算交点y坐标
			Intersection.y = (Intersection.x - P0.x) * (P1.y - P0.y) / (P1.x - P0.x) + P0.y;
		}
		else if (OutCode & AREA_RIGHT)//P0点位于窗口的右侧
		{
			Intersection.x = m_End.x;//计算交点y坐标
			Intersection.y = (Intersection.x - P0.x) * (P1.y - P0.y) / (P1.x - P0.x) + P0.y;
		}
		else if (OutCode & AREA_BOTTOM)//P0点位于窗口的下侧
		{
			Intersection.y = m_Start.y;//计算交点x坐标
			Intersection.x = (Intersection.y - P0.y) * (P1.x - P0.x) / (P1.y - P0.y) + P0.x;
		}
		else if (OutCode & AREA_TOP)//P0点位于窗口的上侧
		{
			Intersection.y = m_End.y;//计算交点x坐标
			Intersection.x = (Intersection.y - P0.y) * (P1.x - P0.x) / (P1.y - P0.y) + P0.x;
		}
		// 5. 我们对于裁剪后的坐标再次进行区域编码。之后作为第一个坐标。
		Encode(Intersection);
		P0 = Intersection;
	}	
}

/************************************************************************/
/* 功能：绘制函数。绘制了一个椭圆和上面的文字。                         */
/************************************************************************/
void CCohenSutherlandClipLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}

	pdc->Rectangle( CRect(m_Start, m_End) );
	CohenSutherlandClip();
	pdc->MoveTo(P0.x, P0.y);
	pdc->LineTo(P1.x, P1.y);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CCohenSutherlandClipLine::DrawFocus( CDC *pdc )
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
void CCohenSutherlandClipLine::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CCohenSutherlandClipLine::AdjustSize( CPoint &pt )
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
bool CCohenSutherlandClipLine::IsIn( CPoint &pt )
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
int CCohenSutherlandClipLine::IsConnectOn(CAdjustPoint *pt)
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
bool CCohenSutherlandClipLine::IsOn( CPoint &pt )
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
void CCohenSutherlandClipLine::AdjustStartAndEnd()
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

int CCohenSutherlandClipLine::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
#define CLIPLINE_DIFF     40
void CCohenSutherlandClipLine::AdjustFocusPoint()
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
	
	P0 = CAreaPoint(m_Start.x - CLIPLINE_DIFF, m_End.y   + CLIPLINE_DIFF);
	P1 = CAreaPoint(m_End.x   + CLIPLINE_DIFF, m_Start.y - CLIPLINE_DIFF);
}

/************************************************************************/
/* 功能：串行化操作。                                                   */
/************************************************************************/
void CCohenSutherlandClipLine::SaveParamsToJSON(cJSON * objJSON)
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

void CCohenSutherlandClipLine::LoadParamsFromJSON(cJSON * objJSON)
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