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
/* ���ܣ������������趨�����ӵ㡣                                       */
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
	for (i = 0; i < NumberofCube; i++)//����5��������
	{
		cube[i].ReadVertex();//����������Ķ���
		cube[i].ReadFace();//����������ı���
		transform[i].SetMatrix(cube[i].V, 8);//��ά�任�ӿ�
		transform[i].Scale(nScale, nScale, nScale);//�����任
		transform[i].Translate(-nScale / 2, -nScale / 2, -nScale / 2);//ƽ�Ʊ任
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
	CRect rectScreen;//����ͻ�������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->GetClientRect(&rectScreen);//��ÿͻ����Ĵ�С
	
	CRect rect = CRect(0, 0, 400, 400);
	
    CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap NewBitmap, *pOldBitmap;
    NewBitmap.CreateCompatibleBitmap(pDC, 400, 400);//rtΪRECT����;
    pOldBitmap = memDC.SelectObject(&NewBitmap);
	memDC.FillSolidRect(rect, pDC->GetBkColor());//���ÿͻ�������ɫ
	// rect.OffsetRect(-200, -200);
	DrawObject(&memDC);//����ͼ��
    pDC->BitBlt(m_Start.x, m_Start.y, 400, 400, 
        &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
    memDC.DeleteDC();
}

void CSpatialArrangedCube::DrawObject(CDC* pDC)//����ͼ��
{
	for (int i = 0; i < NumberofCube; i++)//���������
	{
		for (int j = 0; j < FaceNumberofCube; j++)//ÿ��������6������
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
	SortFace();//���水���ĵ��ӵ�ľ�������
	CColorP2 ScreenPoint[4];//��άͶӰ��
	CPen penWhite(PS_SOLID, 1, RGB(255, 255, 255));//��ɫ�������ڻ�����ı߽���
	CPen* pOldPen = pDC->SelectObject(&penWhite);
	for (i = 0; i < NumberofCube * FaceNumberofCube; i++)//����������Ⱥ�����б���
	{
		//�õ�ÿ������Ķ�ά����
		for (int nPoint = 0; nPoint < globalface[i].ptNumber; nPoint++)//����ѭ��
			ScreenPoint[nPoint] = projection.TwoDimColorPerspectiveProjection(globalface[i].Point[nPoint]);
		//������
		CTriangle* pFill = new CTriangle;//�����ڴ�
		//��������������
		pFill->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]);
		pFill->Fill(pDC);
		//��������������
		pFill->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);
		pFill->Fill(pDC);
		delete pFill;//�����ڴ�		
		//���Ʊ���߽���
		pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
		pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
		pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
		pDC->LineTo(ROUND(ScreenPoint[3].x), ROUND(ScreenPoint[3].y));
		pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));		
	}
	pDC->SelectObject(pOldPen);
}


double CSpatialArrangedCube::GetMaxDepth(CDepthFace face)//����������
{
	CColorP3 faceCenter = (face.Point[0] + face.Point[1] + face.Point[2] + face.Point[3]) / 4;
	return GetDistance(faceCenter, projection.GetColorEye());
}

double CSpatialArrangedCube::GetDistance(CColorP3 p0, CColorP3 p1)//����ռ�����֮��ľ���
{
	double result = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y) + (p0.z - p1.z) * (p0.z - p1.z));
	return result;
}

void CSpatialArrangedCube::SortFace(void)//��������
{
	//��һά����F����ð������
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
/* ���ܣ����ƺ�����������һ����Բ����������֡�                         */
/************************************************************************/
void CSpatialArrangedCube::Draw( CDC *pDC, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pDC-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
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
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CSpatialArrangedCube::DrawFocus( CDC *pdc )
{
	// ����Ϊ���ߣ��߿�Ϊ1����ɫΪ��ɫ��
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );
	// ��һ�����߿�
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
/* ���ܣ� �ƶ���������                                                */
/************************************************************************/
void CSpatialArrangedCube::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CSpatialArrangedCube::AdjustSize( CPoint &pt )
{
	switch(m_AdjustPoint)
	{
	// case 1:  // ���Ͻ�
	case CCONNECTPOINT_RECT_LEFT_TOP:
		{
			m_Start = pt;
			break;
		}
	// case 2:  // ���½�
	case CCONNECTPOINT_RECT_LEFT_BOTTOM:
		{
			m_Start.x = pt.x;
			m_End.y = pt.y;
			break;
		}
	// case 3:  // ���Ͻ�
	case CCONNECTPOINT_RECT_RIGHT_TOP:
		{
			m_Start.y = pt.y;
			m_End.x = pt.x;
			break;
		}
	// case 4:  // ���½�
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
/* ���ܣ��ж��Ƿ���ͼԪ�����ڡ�                                         */
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
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
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
/* ���ܣ� �ж�һ����Ļ�����Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ��С��          */
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
/* ���ܣ��ڵ�����С������ת��ʱ�򣬸��ݵ������������ʼ��ͽ��������ꡣ */
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
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
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
/* ���ܣ����л�������                                                   */
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