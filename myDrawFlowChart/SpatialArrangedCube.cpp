// SpatialArrangedCube.cpp: implementation of the CSpatialArrangedCube class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "SpatialArrangedCube.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CSpatialArrangedCube, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CSpatialArrangedCube::CSpatialArrangedCube()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
	
	int nScale = 150;
	for (i = 0; i < NumberofCube; i++)//绘制5个立方体
	{
		cube[i].ReadVertex();//读入立方体的顶点
		cube[i].ReadFace();//读入立方体的表面
		transform[i].SetMatrix(cube[i].V, 8);//三维变换接口
		transform[i].Scale(nScale, nScale, nScale);//比例变换
		transform[i].Translate(-nScale / 2, -nScale / 2, -nScale / 2);//平移变换
	}
	transform[1].Translate(0, 2 * nScale, 0);
	transform[2].Translate(0, -2 * nScale, 0);
	transform[3].Translate(2 * nScale, 0, 0);
	transform[4].Translate(-2 * nScale, 0, 0);
	
	// Rotate to a certain angle
	for (i = 0; i < NumberofCube; i++)
	{
		transform[i].RotateX(-14);
		transform[i].RotateY(-18);
	}
}

CSpatialArrangedCube::~CSpatialArrangedCube()
{

}

void CSpatialArrangedCube::DoubleBuffer(CDC* pDC)
{
	CRect rectScreen;//定义客户区矩形
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->GetClientRect(&rectScreen);//获得客户区的大小
	
	CRect rect = CRect(0, 0, 400, 400);
	
    CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap NewBitmap, *pOldBitmap;
    NewBitmap.CreateCompatibleBitmap(pDC, 400, 400);//rt为RECT变量;
    pOldBitmap = memDC.SelectObject(&NewBitmap);
	memDC.FillSolidRect(rect, pDC->GetBkColor());//设置客户区背景色
	// rect.OffsetRect(-200, -200);
	DrawObject(&memDC);//绘制图形
    pDC->BitBlt(m_Start.x, m_Start.y, 400, 400, 
        &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
    memDC.DeleteDC();
}

void CSpatialArrangedCube::DrawObject(CDC* pDC)//绘制图形
{
	for (int i = 0; i < NumberofCube; i++)//立方体个数
	{
		for (int j = 0; j < FaceNumberofCube; j++)//每个立方体6个表面
		{
			globalface[i * FaceNumberofCube + j].ptNumber = cube[i].F[j].ptNumber;
			for (int k = 0; k < globalface[i * FaceNumberofCube + j].ptNumber; k++)
			{
				globalface[i * FaceNumberofCube + j].ptIndex[k] = cube[i].F[j].ptIndex[k];
				globalface[i * FaceNumberofCube + j].Point[k] = cube[i].V[globalface[i * 6 + j].ptIndex[k]];
				globalface[i * FaceNumberofCube + j].fMaxDepth = GetMaxDepth(globalface[i * 6 + j]);
			}
		}
	}
	SortFace();//表面按面心到视点的距离排序
	CColorP2 ScreenPoint[4];//二维投影点
	CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));//白色画笔用于绘制面的边界线
	CPen* pOldPen = pDC->SelectObject(&penWhite);
	for (i = 0; i < NumberofCube * FaceNumberofCube; i++)//访问立方体群的所有表面
	{
		//得到每个表面的二维顶点
		for (int nPoint = 0; nPoint < globalface[i].ptNumber; nPoint++)//顶点循环
			ScreenPoint[nPoint] = projection.TwoDimColorPerspectiveProjection(globalface[i].Point[nPoint]);
		//填充表面
		CTriangle* pFill = new CTriangle;//申请内存
		//填充表面上三角形
		pFill->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]);
		pFill->Fill(pDC);
		//填充表面下三角形
		pFill->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);
		pFill->Fill(pDC);
		delete pFill;//撤销内存		
		//绘制表面边界线
		pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
		pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
		pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
		pDC->LineTo(ROUND(ScreenPoint[3].x), ROUND(ScreenPoint[3].y));
		pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));		
	}
	pDC->SelectObject(pOldPen);
}


double CSpatialArrangedCube::GetMaxDepth(CDepthFace face)//计算面的深度
{
	CColorP3 faceCenter = (face.Point[0] + face.Point[1] + face.Point[2] + face.Point[3]) / 4;
	return GetDistance(faceCenter, projection.GetColorEye());
}

double CSpatialArrangedCube::GetDistance(CColorP3 p0, CColorP3 p1)//计算空间两点之间的距离
{
	double result = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y) + (p0.z - p1.z) * (p0.z - p1.z));
	return result;
}

void CSpatialArrangedCube::SortFace(void)//表面排序
{
	//对一维数组F进行冒泡排序
	for (int i = 0; i < NumberofCube * FaceNumberofCube - 1; i++)
	{
		for (int j = 0; j < NumberofCube * FaceNumberofCube - 1 - i; j++)
		{
			if (globalface[j].fMaxDepth < globalface[j + 1].fMaxDepth)
			{
				CDepthFace FTemp;
				FTemp = globalface[j];
				globalface[j] = globalface[j + 1];
				globalface[j + 1] = FTemp;
			}
		}
	}
}

/************************************************************************/
/* 功能：绘制函数。绘制了一个椭圆和上面的文字。                         */
/************************************************************************/
void CSpatialArrangedCube::Draw( CDC *pDC, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pDC-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	
	// Rotate
	for (int i = 0; i < NumberofCube; i++)
	{
		// Move to (100, 100, 100) to display
		transform[i].Translate(100, 100, 100);
	}
	// Move to current position
	DrawObject(pDC);
	for (i = 0; i < NumberofCube; i++)
	{
		// Move to (100, 100, 100) to display
		transform[i].Translate(-100, -100, -100);
	}
	if(m_IsMark)
	{
		pDC->SelectObject(pOldPen);
	}
	pDC->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CSpatialArrangedCube::DrawFocus( CDC *pdc )
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
void CSpatialArrangedCube::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CSpatialArrangedCube::AdjustSize( CPoint &pt )
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
bool CSpatialArrangedCube::IsIn( CPoint &pt )
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
int CSpatialArrangedCube::IsConnectOn(CAdjustPoint *pt)
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
bool CSpatialArrangedCube::IsOn( CPoint &pt )
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
void CSpatialArrangedCube::AdjustStartAndEnd()
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

int CSpatialArrangedCube::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
void CSpatialArrangedCube::AdjustFocusPoint()
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
void CSpatialArrangedCube::SaveParamsToJSON(cJSON * objJSON)
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

void CSpatialArrangedCube::LoadParamsFromJSON(cJSON * objJSON)
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