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
/* ���ܣ������������趨�����ӵ㡣                                       */
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
	BoundaryClr = RGB(0, 0, 0);//�߽�ɫ
	SeedClr = RGB(255, 0, 0);//����ɫ
}

CScanLineFillPolygon::~CScanLineFillPolygon()
{

}

void CScanLineFillPolygon::ReadPoint(void)//�����
{
	// ��Բ��������������ꡣ
	P[0].x =-30, P[0].y =-30;
	P[1].x = 30, P[1].y = 30;
	// ��Բ�ڲ������ε����ꡣ
	P[2].x =-15, P[2].y =-10;
	P[3].x = 15, P[3].y =-10;
	P[4].x = 0,   P[4].y = 15;

	P[0].x += m_Start.x, P[0].y += m_Start.y; 
	P[1].x += m_Start.x, P[1].y += m_Start.y; 
	P[2].x += m_Start.x, P[2].y += m_Start.y; 
	P[3].x += m_Start.x, P[3].y += m_Start.y; 
	P[4].x += m_Start.x, P[4].y += m_Start.y; 
	// ��ʼ��������ӡ�
	Seed = CPoint(20, 0);
	Seed += m_Start;
}


void CScanLineFillPolygon::DrawRegion(CDC* pDC)//��������
{
	pDC->Ellipse(P[0].x, P[0].y, P[1].x, P[1].y);//������Բ
	pDC->MoveTo(P[2].x, P[2].y);//����������
	pDC->LineTo(P[3].x, P[3].y);
	pDC->LineTo(P[4].x, P[4].y);
	pDC->LineTo(P[2].x, P[2].y);
}

/************************************************************************/
/* �������ʵ����ɨ������������㷨                                     */                  
/* ����㷨�Ĳ����������ġ�                                             */
/* 1. �����ӵ�ѹջ��                                                    */
/* 2. �ж϶�ջ�Ƿ�Ϊ�ա������Ϊ�գ��͵���ջ��Ԫ�ء�                    */    
/*    ��һ��ѭ���������ӵ��Լ���                                        */
/* 3. �����ӵ������һ��ƣ����Ƴ�һ�����ߡ�                              */
/* 4. ����Ҳ��յ������һ��Ԫ�غ�����һ��Ԫ�ء�                        */
/*    4.1. �������һ��������һ�������Ǳ߽��ߡ�                         */
/*         ˵������һ��������һ��Ԫ�����ڵ�����Ҫ����䡣               */         
/*         Ȼ����������ѹջ��                                           */
/*    4.2. �������һ��������һ���Ǳ߽��ߡ�                             */
/*         ��ô����Ҫ�ҵ������߶�Ӧ�����Ҳ�Ԫ�ء�                       */ 
/*         Ȼ�������ѹջ��                                             */
/*    4.3. �������һ��������һ���Ѿ���䣬����Ҫ������               */         
/* 5. �������յ������һ��Ԫ�غ�����һ��Ԫ�ء�                        */
/*    5.1. �������һ��������һ�������Ǳ߽��ߡ�                         */
/*         ˵������һ��������һ��Ԫ�����ڵ�����Ҫ����䡣               */         
/*         Ȼ����������ѹջ��                                           */
/*    5.2. �������һ��������һ���Ǳ߽��ߡ�                             */
/*         ��ô����Ҫ�ҵ������߶�Ӧ�������Ԫ�ء�                       */ 
/*         Ȼ�������ѹջ��                                             */
/*    5.3. �������һ��������һ���Ѿ���䣬����Ҫ������               */         
/* 6. �ص���2����ʹ��ǰ��ѹջ��Ԫ��ѭ��������                           */
/************************************************************************/
void CScanLineFillPolygon::ScanLineSeedFill(CDC* pDC)
{
	pHead = new CColorStack;//����ջͷ���
	pHead->pNext = NULL;//ջͷ����ָ������Ϊ��
	pHead->Push(Seed);	//����������ջ
	int xLeft, xRight;//�����������������Ҷ�x����
	CPoint PopPixel, CurrentPixel;//������ջ�����뵱ǰ����
	while (NULL != pHead->pNext)//���ջ��Ϊ��
	{
		PopPixel = pHead->Pop();//ջ�����س�ջ
		CurrentPixel = PopPixel;
		//����ջ���ؼ����Ҳ����أ�ֱ���ұ߽��λ��
		while (BoundaryClr != pDC->GetPixel(CurrentPixel))
		{
			pDC->SetPixelV(CurrentPixel, SeedClr);//������ӵ��Ҳ�����
			CurrentPixel.x++;
		}
		xRight = CurrentPixel.x - 1;//�������Ҳ�߽����������ص�x����
		//����ջ���ص�������أ�ֱ����߽��λ��
		CurrentPixel.x = PopPixel.x - 1;//��ǰ������Ϊ��ջ���ص�����������
		while (BoundaryClr != pDC->GetPixel(CurrentPixel))
		{
			pDC->SetPixelV(CurrentPixel, SeedClr);//������ӵ��������
			CurrentPixel.x--;
		}
		xLeft = CurrentPixel.x + 1;//���������߽���ҽ������ص�x����
		//������ǰɨ���ߵ�����һ��ɨ����
		CurrentPixel.x = xLeft, CurrentPixel.y = PopPixel.y + 1;
		BOOL bSpanFill;//�Ƿ�����ȵ��߼�����
		while (CurrentPixel.x <= xRight)
		{
			bSpanFill = FALSE;//δ���
			//��ǰ���ز��Ǳ߽�ɫҲ��������ɫ
			while (BoundaryClr != pDC->GetPixel(CurrentPixel) && SeedClr != pDC->GetPixel(CurrentPixel))
			{
				bSpanFill = TRUE;
				CurrentPixel.x++;
			}
			if (bSpanFill)
			{
				pHead->Push(CPoint(CurrentPixel.x - 1, CurrentPixel.y));//������Ҷ�������ջ
				bSpanFill = FALSE;
			}
			//Խ���߽�����������ص�
			while (BoundaryClr == pDC->GetPixel(CurrentPixel) || SeedClr == pDC->GetPixel(CurrentPixel))
				CurrentPixel.x++;
		}
		//������ǰɨ���ߵ�����һ��ɨ����
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
				pHead->Push(CPoint(CurrentPixel.x - 1, CurrentPixel.y));//������Ҷ�������ջ
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
/* ���ܣ����ƺ�����������һ����Բ����������֡�                         */
/************************************************************************/
void CScanLineFillPolygon::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pdc-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
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
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CScanLineFillPolygon::DrawFocus( CDC *pdc )
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
/* ���ܣ� �ƶ�����������                                                */
/************************************************************************/
void CScanLineFillPolygon::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С��������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CScanLineFillPolygon::AdjustSize( CPoint &pt )
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
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
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
/* ���ܣ� �ж�һ����Ļ�����Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ��С��          */
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
/* ���ܣ��ڵ�����С������ת��ʱ�򣬸��ݵ������������ʼ��ͽ��������ꡣ */
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
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
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
/* ���ܣ����л�������                                                   */
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