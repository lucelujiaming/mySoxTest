// ScanLineFillPolygon.cpp: implementation of the CScanLineFillPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "ScanLineFillPolygon.h"
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
// IMPLEMENT_SERIAL(CScanLineFillPolygon, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CScanLineFillPolygon::CScanLineFillPolygon()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
	BoundaryClr = RGB(0, 0, 0);//边界色
	SeedClr = RGB(255, 0, 0);//种子色
}

CScanLineFillPolygon::~CScanLineFillPolygon()
{

}

void CScanLineFillPolygon::ReadPoint(void)//顶点表
{
	// 椭圆的两个焦点的坐标。
	P[0].x =-30, P[0].y =-30;
	P[1].x = 30, P[1].y = 30;
	// 椭圆内部三角形的坐标。
	P[2].x =-15, P[2].y =-10;
	P[3].x = 15, P[3].y =-10;
	P[4].x = 0,   P[4].y = 15;

	P[0].x += m_Start.x, P[0].y += m_Start.y; 
	P[1].x += m_Start.x, P[1].y += m_Start.y; 
	P[2].x += m_Start.x, P[2].y += m_Start.y; 
	P[3].x += m_Start.x, P[3].y += m_Start.y; 
	P[4].x += m_Start.x, P[4].y += m_Start.y; 
	// 初始化填充种子。
	Seed = CPoint(20, 0);
	Seed += m_Start;
}


void CScanLineFillPolygon::DrawRegion(CDC* pDC)//绘制区域
{
	pDC->Ellipse(P[0].x, P[0].y, P[1].x, P[1].y);//绘制椭圆
	pDC->MoveTo(P[2].x, P[2].y);//绘制三角形
	pDC->LineTo(P[3].x, P[3].y);
	pDC->LineTo(P[4].x, P[4].y);
	pDC->LineTo(P[2].x, P[2].y);
}

/************************************************************************/
/* 这个函数实现了扫描线种子填充算法                                     */                  
/* 这个算法的策略是这样的。                                             */
/* 1. 把种子点压栈。                                                    */
/* 2. 判断堆栈是否为空。如果不为空，就弹出栈顶元素。                    */    
/*    第一次循环就是种子点自己。                                        */
/* 3. 在种子点上向右绘制，绘制出一条横线。                              */
/* 4. 检查右侧终点的上面一个元素和下面一个元素。                        */
/*    4.1. 如果上面一个和下面一个都不是边界线。                         */
/*         说明上面一个和下面一个元素所在的线需要被填充。               */         
/*         然后他们两个压栈。                                           */
/*    4.2. 如果上面一个和下面一个是边界线。                             */
/*         那么就需要找到这条线对应的最右侧元素。                       */ 
/*         然后把它们压栈。                                             */
/*    4.3. 如果上面一个和下面一个已经填充，则不需要处理。               */         
/* 5. 检查左侧终点的上面一个元素和下面一个元素。                        */
/*    5.1. 如果上面一个和下面一个都不是边界线。                         */
/*         说明上面一个和下面一个元素所在的线需要被填充。               */         
/*         然后他们两个压栈。                                           */
/*    5.2. 如果上面一个和下面一个是边界线。                             */
/*         那么就需要找到这条线对应的最左侧元素。                       */ 
/*         然后把它们压栈。                                             */
/*    5.3. 如果上面一个和下面一个已经填充，则不需要处理。               */         
/* 6. 回到第2步。使用前面压栈的元素循环操作。                           */
/************************************************************************/
void CScanLineFillPolygon::ScanLineSeedFill(CDC* pDC)
{
	pHead = new CColorStack;//建立栈头结点
	pHead->pNext = NULL;//栈头结点的指针域总为空
	pHead->Push(Seed);	//种子像素入栈
	int xLeft, xRight;//声明跨度最左端与最右端x坐标
	CPoint PopPixel, CurrentPixel;//声明出栈像素与当前像素
	while (NULL != pHead->pNext)//如果栈不为空
	{
		PopPixel = pHead->Pop();//栈顶像素出栈
		CurrentPixel = PopPixel;
		//填充出栈像素及其右侧像素，直到右边界的位置
		while (BoundaryClr != pDC->GetPixel(CurrentPixel))
		{
			pDC->SetPixelV(CurrentPixel, SeedClr);//填充种子的右侧像素
			CurrentPixel.x++;
		}
		xRight = CurrentPixel.x - 1;//保存跨度右侧边界的左紧邻像素的x坐标
		//填充出栈像素的左侧像素，直到左边界的位置
		CurrentPixel.x = PopPixel.x - 1;//当前像素置为出栈像素的左侧紧邻像素
		while (BoundaryClr != pDC->GetPixel(CurrentPixel))
		{
			pDC->SetPixelV(CurrentPixel, SeedClr);//填充种子的左侧像素
			CurrentPixel.x--;
		}
		xLeft = CurrentPixel.x + 1;//保存跨度左侧边界的右紧邻像素的x坐标
		//处理当前扫描线的上面一条扫描线
		CurrentPixel.x = xLeft, CurrentPixel.y = PopPixel.y + 1;
		BOOL bSpanFill;//是否填充跨度的逻辑变量
		while (CurrentPixel.x <= xRight)
		{
			bSpanFill = FALSE;//未填充
			//当前像素不是边界色也不是种子色
			while (BoundaryClr != pDC->GetPixel(CurrentPixel) && SeedClr != pDC->GetPixel(CurrentPixel))
			{
				bSpanFill = TRUE;
				CurrentPixel.x++;
			}
			if (bSpanFill)
			{
				pHead->Push(CPoint(CurrentPixel.x - 1, CurrentPixel.y));//跨度最右端像素入栈
				bSpanFill = FALSE;
			}
			//越过边界或已填充的像素点
			while (BoundaryClr == pDC->GetPixel(CurrentPixel) || SeedClr == pDC->GetPixel(CurrentPixel))
				CurrentPixel.x++;
		}
		//处理当前扫描线的下面一条扫描线
		CurrentPixel.x = xLeft,	CurrentPixel.y = PopPixel.y - 1;
		while (CurrentPixel.x <= xRight)
		{
			bSpanFill = FALSE;
			while (BoundaryClr != pDC->GetPixel(CurrentPixel) && SeedClr != pDC->GetPixel(CurrentPixel))
			{
				bSpanFill = TRUE;
				CurrentPixel.x++;
			}
			if (bSpanFill)
			{
				pHead->Push(CPoint(CurrentPixel.x - 1, CurrentPixel.y));//跨度最右端像素入栈
				bSpanFill = FALSE;
			}
			while (BoundaryClr == pDC->GetPixel(CurrentPixel) || SeedClr == pDC->GetPixel(CurrentPixel))
				CurrentPixel.x++;
		}
	}
	delete pHead;
	pHead = NULL;
}

/************************************************************************/
/* 功能：绘制函数。绘制了一个椭圆和上面的文字。                         */
/************************************************************************/
void CScanLineFillPolygon::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	ReadPoint();
	DrawRegion(pdc);
	ScanLineSeedFill(pdc);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CScanLineFillPolygon::DrawFocus( CDC *pdc )
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
void CScanLineFillPolygon::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CScanLineFillPolygon::AdjustSize( CPoint &pt )
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
bool CScanLineFillPolygon::IsIn( CPoint &pt )
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
int CScanLineFillPolygon::IsConnectOn(CAdjustPoint *pt)
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
bool CScanLineFillPolygon::IsOn( CPoint &pt )
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
void CScanLineFillPolygon::AdjustStartAndEnd()
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

int CScanLineFillPolygon::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
void CScanLineFillPolygon::AdjustFocusPoint()
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
void CScanLineFillPolygon::SaveParamsToJSON(cJSON * objJSON)
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

void CScanLineFillPolygon::LoadParamsFromJSON(cJSON * objJSON)
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