// FenceFillPolygon.cpp: implementation of the CFenceFillPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "FenceFillPolygon.h"
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
// IMPLEMENT_SERIAL(CFenceFillPolygon, CObject, 1)

/************************************************************************/
/* ���ܣ������������趨�����ӵ㡣                                       */
/************************************************************************/
CFenceFillPolygon::CFenceFillPolygon()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
	backClr = RGB(255, 255, 255);//����ɫ
	fillClr = RGB(0, 0, 0);//ǰ��ɫ
}

CFenceFillPolygon::~CFenceFillPolygon()
{

}

void CFenceFillPolygon::ReadPoint(void)//�����
{
	CRGB color = CRGB(1, 0, 0);
	P[0].x = 50, P[0].y = 100, P[0].c = color;
	P[1].x = -150; P[1].y = 300, P[1].c = color;
	P[2].x = -250; P[2].y = 50, P[2].c = color;
	P[3].x = -150; P[3].y = -250, P[3].c = color;
	P[4].x = 0;    P[4].y = -50, P[4].c = color;
	P[5].x = 100;  P[5].y = -250, P[5].c = color;
	P[6].x = 300;  P[6].y = 150, P[6].c = color;

	P[0].x += m_Start.x, P[0].y += m_Start.y; 
	P[1].x += m_Start.x, P[1].y += m_Start.y; 
	P[2].x += m_Start.x, P[2].y += m_Start.y; 
	P[3].x += m_Start.x, P[3].y += m_Start.y; 
	P[4].x += m_Start.x, P[4].y += m_Start.y; 
	P[5].x += m_Start.x, P[5].y += m_Start.y; 
	P[6].x += m_Start.x, P[6].y += m_Start.y; 
}


void CFenceFillPolygon::DrawObject(CDC* pDC)
{
	CColorPoint t;
	for (int i = 0; i < 7; i++)//���ƶ����
	{
		if (0 == i)
		{
			pDC->MoveTo(P[i].x, P[i].y);
			t = P[i];
		}
		else
			pDC->LineTo(P[i].x, P[i].y);
	}
	pDC->LineTo(t.x, t.y);//�պ϶����
	CPen greenPen(PS_SOLID, 3, RGB(0, 255, 0));
	CPen* pOldPen = pDC->SelectObject(&greenPen);
	// Ϊ�˼��㷽�㣬դ��ͨ��ȡ�����ĳһ������Ĵ��ߡ�����ȡ����P[4]��Ĵ��ߡ�
	pDC->MoveTo(P[4].x, P[1].y);//����դ��
	pDC->LineTo(P[4].x, P[3].y);
	pDC->SelectObject(pOldPen);
}


/************************************************************************/
/* դ���㷨�ͱ�����㷨��һ����ͬ��ǰ�ᣬ�������ɫ�ͱ���ɫ��Ϊ��ɫ��   */
/* ��Ϊ��һ����������ȡ����ָ�Ϊԭɫ����������������Ч����             */
/* �����դ���㷨�ͱ�����㷨�ܹ������Ĺؼ���                           */
/************************************************************************/
/* ��դ���㷨�ͱ�����㷨���������ڣ�                                   */
/* ������㷨�������Ǳ��Ҳ���������ء�                                 */
/* ��դ���㷨��������դ���ͱ�֮������ء�����ϣ�դ���㷨����Ҫ�߿�   */
/************************************************************************/
/* ���������Ԥ��ǰ�ᡣ����㷨��ԭ���ͱ�÷ǳ��򵥣�                   */
/*   �������դ����ߣ��ʹ����������ߵ�դ��֮������ء�                 */
/*   �������դ���ұߣ��ʹ�����դ����������֮������ء�                 */
/*   ����ߴ���դ������ñ߷�Ϊ���Ρ�                                   */
/*   ����դ����ߵĲ��֣������ò��ֵ�դ��֮������ء�                   */
/*   ����դ���ұߵĲ��֣�����դ�����ò���֮������ء�                   */
/* ��������դ������㷨��ԭ����                                       */
/************************************************************************/
void CFenceFillPolygon::EdgeFill(CDC* pDC)
{
	int ymin, ymax;//�ߵ���Сyֵ�����yֵ
	double x_ymin, m;//x_yminΪ�ߵͶ˵�x���꣬mΪб�ʵĵ���
	for (int i = 0; i < 7; i++)//ѭ����������б�
	{
		int j = (i + 1) % 7;
		m = double(P[i].x - P[j].x) / (P[i].y - P[j].y);//����1/k
		if (P[i].y < P[j].y)//�õ�ÿ����y�����ֵ����Сֵ
		{
			ymin = P[i].y;
			ymax = P[j].y;
			x_ymin = P[i].x;//�õ�x|ymin
		}
		else
		{
			ymin = P[j].y;
			ymax = P[i].y;
			x_ymin = P[j].x;
		}
		for (int y = ymin; y < ymax; y += 10)//��ÿһ����ѭ��ɨ����
		{
			// �����դ����ߡ������д���ز���Ӧ�ÿ����滻��Ϊ���߲�����
			for (int x = ROUND(x_ymin); x <= P[4].x; x++)
			{
				if (fillClr == pDC->GetPixel(x, y))//��������ɫ
					pDC->SetPixelV(x, y, backClr);//��Ϊ����ɫ
				else
					pDC->SetPixelV(x, y, fillClr);//��Ϊ���ɫ
			}
			// �����դ���ұߡ������д���ز���Ӧ�ÿ����滻��Ϊ���߲�����
			for (x = ROUND(P[4].x); x < ROUND(x_ymin); x++)
			{
				if (fillClr == pDC->GetPixel(x, y))//��������ɫ
					pDC->SetPixelV(x, y, backClr);//��Ϊ����ɫ
				else
					pDC->SetPixelV(x, y, fillClr);//��Ϊ���ɫ
			}
			x_ymin += 10 * m;//������һ��ɨ���ߵ�x�������
		}
	}
}

/************************************************************************/
/* ���ܣ����ƺ�����                                                     */
/************************************************************************/
void CFenceFillPolygon::Draw( CDC *pdc, BOOL bShowSelectBorder )
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
void CFenceFillPolygon::DrawFocus( CDC *pdc )
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
void CFenceFillPolygon::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С��������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CFenceFillPolygon::AdjustSize( CPoint &pt )
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
bool CFenceFillPolygon::IsIn( CPoint &pt )
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
int CFenceFillPolygon::IsConnectOn(CAdjustPoint *pt)
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
bool CFenceFillPolygon::IsOn( CPoint &pt )
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
void CFenceFillPolygon::AdjustStartAndEnd()
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

int CFenceFillPolygon::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
/************************************************************************/
void CFenceFillPolygon::AdjustFocusPoint()
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
void CFenceFillPolygon::SaveParamsToJSON(cJSON * objJSON)
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

void CFenceFillPolygon::LoadParamsFromJSON(cJSON * objJSON)
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