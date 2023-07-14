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
/* ���ܣ������������趨�����ӵ㡣                                       */
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
	m_backClr = RGB(255, 255, 255);//����ɫ
	m_fillClr = RGB(0, 0, 0);//ǰ��ɫ
}

CEdgeFillPolygon::~CEdgeFillPolygon()
{

}

void CEdgeFillPolygon::ReadPoint(void)//�����
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
/* ����ReadPoint�����Ķ���Ρ�ͬʱ����һ�����ΰ��������ΰ��������档  */
/* ������θ����ڻ����������ι����У�XY��ȡֵ��Χ��                   */
/* �������Ǿ��ò��Ų�����������������ˡ��������Ч�ʡ�                 */
/************************************************************************/
void CEdgeFillPolygon::DrawObject(CDC* pDC)
{
	m_xMin = m_xMax = m_P[0].x;
	m_yMin = m_yMax = m_P[0].y;
	// 1. ��������������εľ��ΰ�Χ�е����ꡣ
	//    ��ʵ�����ҳ�������ε����ж����XY��������ֵ����Сֵ��
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
	// 2. ���ƶ����
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
	pDC->LineTo(t.x, t.y);//�պ϶����
	
	// 3. ���ư����������εľ��ΰ�Χ��
	pDC->MoveTo(m_xMin, m_yMin);
	pDC->LineTo(m_xMax, m_yMin);
	pDC->LineTo(m_xMax, m_yMax);
	pDC->LineTo(m_xMin, m_yMax);
	pDC->LineTo(m_xMin, m_yMin);
}

/************************************************************************/
/* �������ʵ���˱�����㷨������㷨��ǰ�������ɫ�ͱ���ɫ��Ϊ��ɫ��   */
/* ��Ϊ��һ����������ȡ����ָ�Ϊԭɫ����������������Ч����             */
/* ���������㷨�ܹ������Ĺؼ���                                       */
/************************************************************************/
/* ���������Ԥ��ǰ�ᡣ����㷨��ԭ��ͱ�÷ǳ��򵥣�                   */
/* ����������������һ������࣬��Ϊ���ߡ�һ�����Ҳ࣬��Ϊ�Ҳ�ߡ�   */
/* ����������������ȫ�����ء���ʱ���˶�����ڲ����⣬               */
/* �Ҳ�ߵ��Ҳ�Ҳ����䡣֮������Ҳ�������ȫ�����ء�                 */
/* ��������Ҳ�ߵ��Ҳ౻��������Ρ�                                   */
/* ��Ϊ���ɫ�ͱ���ɫ��Ϊ��ɫ��ʹ�����ɫ�����������Ľ������         */
/* �Ҳ�ߵ��Ҳ౻�����˱���ɫ��                                       */
/* ��Ϊ��һ����������ȡ����ָ�Ϊԭɫ�������ڸ���������Ч����           */
/* ֮���ǰ��������ߵ���������ƹ㵽��ߵ������                       */
/* ��������������㷨��ԭ��                                         */
/************************************************************************/
void CEdgeFillPolygon::EdgeFill(CDC* pDC)
{
	int ymin, ymax;//�ߵ���Сyֵ�����yֵ
	double x_ymin, m;//x_yminΪ�ߵͶ˵�x���꣬mΪб�ʵĵ���
	for (int i = 0; i < 7; i++)//ѭ����������б�
	{
		int j = (i + 1) % 7;
		m = double(m_P[i].x - m_P[j].x) / (m_P[i].y - m_P[j].y); // ����1/k
		if (m_P[i].y < m_P[j].y)//�õ�ÿ����y�����ֵ����Сֵ
		{
			ymin   = m_P[i].y;
			ymax   = m_P[j].y;
			x_ymin = m_P[i].x;//�õ�x|ymin
		}
		else
		{
			ymin   = m_P[j].y;
			ymax   = m_P[i].y;
			x_ymin = m_P[j].x;
		}
		for (int y = ymin; y < ymax; y++) // ��ÿһ����ѭ��ɨ����
		{
			// ��ÿһ��ɨ������ߵĽ�����Ҳ�����ѭ��
			// �����д���ز���Ӧ�ÿ����滻��Ϊ���߲�����
			for (int x = ROUND(x_ymin); x < m_xMax; x++)
			{
				if (m_fillClr == pDC->GetPixel(x, y)) // ���ԭ�������ɫ
                {
					pDC->SetPixelV(x, y, m_backClr);  // ��Ϊ����ɫ
                }
				else                                // ���ԭ���Ǳ���ɫ
                {
					pDC->SetPixelV(x, y, m_fillClr);  // ��Ϊ���ɫ
                }
			}
			x_ymin += m;                            // ������һ��ɨ���ߵ�x�������
		}
	}
}

/************************************************************************/
/* ���ܣ����ƺ�����                                                     */
/************************************************************************/
void CEdgeFillPolygon::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pdc-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
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
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CEdgeFillPolygon::DrawFocus( CDC *pdc )
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
void CEdgeFillPolygon::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CEdgeFillPolygon::AdjustSize( CPoint &pt )
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
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
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
/* ���ܣ� �ж�һ����Ļ�����Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ��С��          */
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
/* ���ܣ��ڵ�����С������ת��ʱ�򣬸��ݵ������������ʼ��ͽ��������ꡣ */
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
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
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
/* ���ܣ����л�������                                                   */
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
