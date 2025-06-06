// ObjFileTeapotGraph.cpp: implementation of the CObjFileTeapotGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "ObjFileTeapotGraph.h"
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
// IMPLEMENT_SERIAL(CObjFileTeapotGraph, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CObjFileTeapotGraph::CObjFileTeapotGraph()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}

	obj.ReadNumber();
	obj.ReadVertex();
	obj.ReadFace();
	obj.ReadNormal();
	transform.SetMatrix(obj.GetVertexName(), obj.GetVertexNumber());
	double nScale = 5;
	transform.Scale(nScale, nScale, nScale);//比例变换
	InitializeLightingScene();//初始化光照场景
	obj.SetLightingScene(pScene);//设置光照场景
	NormalRotation.SetMatrix(obj.GetNormalName(), obj.GetNormalNumber());
}

CObjFileTeapotGraph::~CObjFileTeapotGraph()
{

}

void CObjFileTeapotGraph::DoubleBuffer(CDC* pDC)
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

void CObjFileTeapotGraph::DrawObject(CDC* pDC)//绘制图形
{
	CDepthLinearInterpZBuffer* pZBuffer = new CDepthLinearInterpZBuffer;//申请内存
	pZBuffer->InitialDepthBuffer(1000, 1000, 1000);//初始化深度缓冲器
	pZBuffer->SetDrawPosition(m_Start);
	obj.Draw(pDC, pZBuffer);
	delete pZBuffer;//释放内存
}

void CObjFileTeapotGraph::InitializeLightingScene(void)//初始化光照环境
{
	//设置光源属性
	nLightSourceNumber = 1;//光源个数
	pScene = new CLightingScene(nLightSourceNumber);//一维光源动态数组
	pScene->pLightSource[0].SetPosition(1000, 1000, 1000);//设置光源位置坐标
	for (int i = 0; i < nLightSourceNumber; i++)
	{
		pScene->pLightSource[i].L_Diffuse = CRGB(1.0, 1.0, 1.0);//光源的漫反射颜色
		pScene->pLightSource[i].L_Specular = CRGB(0.5, 0.5, 0.5);//光源镜面高光颜色
		pScene->pLightSource[i].L_C0 = 1.0;//常数衰减因子
		pScene->pLightSource[i].L_C1 = 0.0000001;//线性衰减因子
		pScene->pLightSource[i].L_C2 = 0.00000001;//二次衰减因子
		pScene->pLightSource[i].L_OnOff = TRUE;//光源开启
	}
	//设置材质属性
	pScene->pMaterial = new CMaterial;
	pScene->pMaterial->SetAmbient(CRGB(0.247, 0.200, 0.075));//材质的环境反射率
	pScene->pMaterial->SetDiffuse(CRGB(0.752, 0.606, 0.226));//材质的漫反射率
	pScene->pMaterial->SetSpecular(CRGB(0.628, 0.556, 0.366));//材质的镜面反射率
	pScene->pMaterial->SetEmission(CRGB(0.0, 0.0, 0.0));//材质自身发散的颜色
	pScene->pMaterial->SetExponent(10);//高光指数
}

/************************************************************************/
/* 功能：绘制函数。绘制了一个椭圆和上面的文字。                         */
/************************************************************************/
void CObjFileTeapotGraph::Draw( CDC *pDC, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pDC-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}
	
//	// Rotate
//	for (i = 0; i < NumberofCube; i++)
//	{
//		transform[i].RotateX(m_currentAlpha);
//		transform[i].RotateY(m_currentBeta);
//		// Move to (100, 100, 100) to display
//		transform[i].Translate(100, 100, 100);
//	}
	DrawObject(pDC);
	if(m_IsMark)
	{
		pDC->SelectObject(pOldPen);
	}
	pDC->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CObjFileTeapotGraph::DrawFocus( CDC *pdc )
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
void CObjFileTeapotGraph::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CObjFileTeapotGraph::AdjustSize( CPoint &pt )
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
bool CObjFileTeapotGraph::IsIn( CPoint &pt )
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
int CObjFileTeapotGraph::IsConnectOn(CAdjustPoint *pt)
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
bool CObjFileTeapotGraph::IsOn( CPoint &pt )
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
void CObjFileTeapotGraph::AdjustStartAndEnd()
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

int CObjFileTeapotGraph::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
void CObjFileTeapotGraph::AdjustFocusPoint()
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
void CObjFileTeapotGraph::SaveParamsToJSON(cJSON * objJSON)
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

void CObjFileTeapotGraph::LoadParamsFromJSON(cJSON * objJSON)
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