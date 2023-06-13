// RoundHeadRectangle.cpp: implementation of the CRoundHeadRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "RoundHeadRectangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CRoundHeadRectangle, CObject, 1)

/************************************************************************/
/* ���ܣ������������趨�����ӵ㡣                                       */
/************************************************************************/
CRoundHeadRectangle::CRoundHeadRectangle()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
}

CRoundHeadRectangle::~CRoundHeadRectangle()
{

}

/************************************************************************/
/* ���ܣ����ƺ�����������һ��Բ�Ǿ��κ���������֡�                     */
/************************************************************************/
void CRoundHeadRectangle::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pdc-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
	}

	if(abs(m_End.x - m_Start.x) >= abs(m_End.y - m_Start.y))
	{
		if(m_End.y >= m_Start.y)
		{
			// m_End on the east and south of m_Start
			if (m_End.x >= m_Start.x)
			{
				// Left Semicircle
				pdc->Arc( CRect(m_Start, CPoint(m_Start.x + (m_End.y - m_Start.y), m_End.y)), 
					CPoint(m_Start.x + (m_End.y - m_Start.y)/2, m_Start.y),
					CPoint(m_Start.x + (m_End.y - m_Start.y)/2, m_End.y));
				// Right Semicircle
				pdc->Arc( CRect(CPoint(m_End.x - (m_End.y - m_Start.y), m_Start.y), m_End), 
					CPoint(m_End.x - (m_End.y - m_Start.y)/2, m_End.y),
					CPoint(m_End.x - (m_End.y - m_Start.y)/2, m_Start.y));
				// Top Line
				pdc->MoveTo(CPoint(m_Start.x + (m_End.y - m_Start.y)/2, m_Start.y));
				pdc->LineTo(CPoint(m_End.x - (m_End.y - m_Start.y)/2, m_Start.y));
				// Bottom Line
				pdc->MoveTo(CPoint(m_Start.x + (m_End.y - m_Start.y)/2, m_End.y));
				pdc->LineTo(CPoint(m_End.x - (m_End.y - m_Start.y)/2, m_End.y));
			}
			// m_End on the west and south of m_Start
			else
			{
				// Left Semicircle
				pdc->Arc( CRect(CPoint(m_End.x, m_Start.y), 
							CPoint(m_End.x + (m_End.y - m_Start.y), 
									m_Start.y + (m_End.y - m_Start.y))), 
					CPoint(m_End.x + (m_End.y - m_Start.y)/2, m_Start.y),
					CPoint(m_End.x + (m_End.y - m_Start.y)/2, m_End.y));

				// Right Semicircle
				pdc->Arc( CRect(CPoint(m_Start.x - (m_End.y - m_Start.y), m_Start.y), 
							CPoint(m_Start.x, m_End.y)), 
					CPoint(m_Start.x - (m_End.y - m_Start.y)/2, m_End.y),
					CPoint(m_Start.x - (m_End.y - m_Start.y)/2, m_Start.y));
				// Top Line
				pdc->MoveTo(CPoint(m_End.x + (m_End.y - m_Start.y)/2, m_Start.y));
				pdc->LineTo(CPoint(m_Start.x - (m_End.y - m_Start.y)/2, m_Start.y));
				// Bottom Line
				pdc->MoveTo(CPoint(m_End.x + (m_End.y - m_Start.y)/2, m_End.y));
				pdc->LineTo(CPoint(m_Start.x - (m_End.y - m_Start.y)/2, m_End.y));
			}
		}
		else
		{
			// m_End on the east and north of m_Start
			if (m_End.x >= m_Start.x)
			{
				// Left Semicircle
				pdc->Arc( CRect(CPoint(m_Start.x, m_End.y), 
							CPoint(m_Start.x + (m_Start.y - m_End.y), m_Start.y)), 
					CPoint(m_Start.x + (m_Start.y - m_End.y)/2, m_End.y),
					CPoint(m_Start.x + (m_Start.y - m_End.y)/2, m_Start.y));
				// Right Semicircle
				pdc->Arc( CRect(CPoint(m_End.x - (m_Start.y - m_End.y), m_End.y), 
							CPoint(m_End.x, m_Start.y)), 
					CPoint(m_End.x - (m_Start.y - m_End.y)/2, m_Start.y),
					CPoint(m_End.x   - (m_Start.y - m_End.y)/2, m_End.y));
				// Top Line
				pdc->MoveTo(CPoint(m_Start.x + (m_Start.y - m_End.y)/2, m_End.y));
				pdc->LineTo(CPoint(m_End.x   - (m_Start.y - m_End.y)/2, m_End.y));
				// Bottom Line
				pdc->MoveTo(CPoint(m_Start.x + (m_Start.y - m_End.y)/2, m_Start.y));
				pdc->LineTo(CPoint(m_End.x - (m_Start.y - m_End.y)/2, m_Start.y));
			}
			// m_End on the west and north of m_Start
			else
			{
				// Left Semicircle
				pdc->Arc( CRect(m_End, 
							CPoint(m_End.x + (m_Start.y - m_End.y), 
								   m_End.y + (m_Start.y - m_End.y))), 
					CPoint(m_End.x + (m_Start.y - m_End.y)/2, m_End.y),
					CPoint(m_End.x + (m_Start.y - m_End.y)/2, m_Start.y));

				// Right Semicircle
				pdc->Arc( CRect(CPoint(m_Start.x - (m_Start.y - m_End.y), m_End.y), 
								m_Start), 
					CPoint(m_Start.x - (m_Start.y - m_End.y)/2, m_Start.y),
					CPoint(m_Start.x - (m_Start.y - m_End.y)/2, m_End.y));
				// Top Line
				pdc->MoveTo(CPoint(m_End.x + (m_Start.y - m_End.y)/2, m_End.y));
				pdc->LineTo(CPoint(m_Start.x - (m_Start.y - m_End.y)/2, m_End.y));
				// Bottom Line
				pdc->MoveTo(CPoint(m_End.x + (m_Start.y - m_End.y)/2, m_Start.y));
				pdc->LineTo(CPoint(m_Start.x - (m_Start.y - m_End.y)/2, m_Start.y));
			}
		}
	}
	else
	{
		if (m_End.x >= m_Start.x)
		{
			// m_End on the east and south of m_Start
			if(m_End.y >= m_Start.y)
			{
				// Top Semicircle
				pdc->Arc( CRect(m_Start, CPoint(m_End.x, m_Start.y + (m_End.x - m_Start.x))), 
					CPoint(m_End.x,   m_Start.y + (m_End.x - m_Start.x)/2),
					CPoint(m_Start.x, m_Start.y + (m_End.x - m_Start.x)/2));
				// Bottom Semicircle
				pdc->Arc( CRect(CPoint(m_Start.x, m_End.y - (m_End.x - m_Start.x)), m_End), 
					CPoint(m_Start.x, m_End.y - (m_End.x - m_Start.x)/2),
					CPoint(m_End.x, m_End.y - (m_End.x - m_Start.x)/2));
				// Left Line
				pdc->MoveTo(CPoint(m_Start.x, m_Start.y + (m_End.x - m_Start.x)/2));
				pdc->LineTo(CPoint(m_Start.x,   m_End.y - (m_End.x - m_Start.x)/2));
				// Right Line
				pdc->MoveTo(CPoint(m_End.x, m_End.y - (m_End.x - m_Start.x)/2));
				pdc->LineTo(CPoint(m_End.x, m_Start.y + (m_End.x - m_Start.x)/2));
			}
			// m_End on the east and north of m_Start
			else
			{
				// Left Semicircle
				pdc->Arc( CRect(CPoint(m_Start.x, m_End.y), 
							CPoint(m_End.x, m_End.y + (m_End.x - m_Start.x))), 
					CPoint(m_End.x,   m_End.y + (m_End.x - m_Start.x)/2),
					CPoint(m_Start.x, m_End.y + (m_End.x - m_Start.x)/2));
				// Right Semicircle
				pdc->Arc( CRect(CPoint(m_Start.x, m_Start.y - (m_End.x - m_Start.x)), 
								CPoint(m_End.x, m_Start.y)), 
					CPoint(m_Start.x, m_Start.y- (m_End.x - m_Start.x)/2),
					CPoint(m_End.x,   m_Start.y- (m_End.x - m_Start.x)/2));
				// Top Line
				pdc->MoveTo(CPoint(m_Start.x, m_End.y + (m_End.x - m_Start.x)/2));
				pdc->LineTo(CPoint(m_Start.x, m_Start.y- (m_End.x - m_Start.x)/2));
				// Bottom Line
				pdc->MoveTo(CPoint(m_End.x,   m_Start.y - (m_End.x - m_Start.x)/2));
				pdc->LineTo(CPoint(m_End.x,   m_End.y + (m_End.x - m_Start.x)/2));
			}
		}
		else
		{
			// m_End on the west and south of m_Start
			if(m_End.y >= m_Start.y)
			{
				// Top Semicircle
				pdc->Arc(CRect(CPoint(m_End.x,   m_Start.y), 
								CPoint(m_Start.x, m_Start.y + (m_Start.x - m_End.x))), 
					CPoint(m_Start.x, m_Start.y + (m_Start.x - m_End.x)/2),
					CPoint(m_End.x,   m_Start.y + (m_Start.x - m_End.x)/2));

				// Bottom Semicircle
				pdc->Arc(CRect(CPoint(m_End.x,   m_End.y - (m_Start.x - m_End.x)), 
								CPoint(m_Start.x, m_End.y)), 
					CPoint(m_End.x, m_End.y - (m_Start.x - m_End.x)/2),
					CPoint(m_Start.x,   m_End.y - (m_Start.x - m_End.x)/2));
				// Left Line
				pdc->MoveTo(CPoint(m_Start.x, m_Start.y + (m_Start.x - m_End.x)/2));
				pdc->LineTo(CPoint(m_Start.x,   m_End.y - (m_Start.x - m_End.x)/2));
				// Right Line
				pdc->MoveTo(CPoint(m_End.x, m_Start.y + (m_Start.x - m_End.x)/2));
				pdc->LineTo(CPoint(m_End.x,   m_End.y - (m_Start.x - m_End.x)/2));
			}
			// m_End on the west and north of m_Start
			else
			{
				// Left Semicircle
				pdc->Arc( CRect(m_End, 
							CPoint(m_Start.x, 
								   m_End.y + (m_Start.x - m_End.x))), 
					CPoint(m_Start.x, m_End.y + (m_Start.x - m_End.x)/2),
					CPoint(m_End.x,   m_End.y + (m_Start.x - m_End.x)/2));
				// Right Semicircle
				pdc->Arc( CRect(CPoint(m_End.x, m_Start.y - (m_Start.x - m_End.x)), 
								m_Start), 
					CPoint(m_End.x,   m_Start.y - (m_Start.x - m_End.x)/2),
					CPoint(m_Start.x, m_Start.y - (m_Start.x - m_End.x)/2));
				// Top Line
				pdc->MoveTo(CPoint(m_End.x, m_End.y + (m_Start.x - m_End.x)/2));
				pdc->LineTo(CPoint(m_End.x, m_Start.y - (m_Start.x - m_End.x)/2));
				// Bottom Line
				pdc->MoveTo(CPoint(m_Start.x, m_End.y + (m_Start.x - m_End.x)/2));
				pdc->LineTo(CPoint(m_Start.x, m_Start.y - (m_Start.x - m_End.x)/2));
			}
		}
	}

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	pdc->DrawText(m_text, CRect(m_Start + CPoint(
									ROUNDHEAD_RECTANGLE_TEXT_XBORDER, ROUNDHEAD_RECTANGLE_TEXT_YBORDER), 
								m_End+CPoint(
									-1 * ROUNDHEAD_RECTANGLE_TEXT_XBORDER, 
									-1 * ROUNDHEAD_RECTANGLE_TEXT_YBORDER)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CRoundHeadRectangle::DrawFocus( CDC *pdc )
{
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
void CRoundHeadRectangle::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CRoundHeadRectangle::AdjustSize( CPoint &pt )
{
//	m_objLogFile.WriteLog(_T("The m_Start and m_End is [(%d, %d), (%d, %d)]. "), 
//		m_Start.x, m_Start.y, m_End.x, m_End.y);
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
//	m_objLogFile.WriteLog(_T("AdjustSize m_Start and m_End is [(%d, %d), (%d, %d)].\n"), 
//		m_Start.x, m_Start.y, m_End.x, m_End.y);
}

/************************************************************************/
/* ���ܣ��ж��Ƿ���ͼԪ�����ڡ�                                         */
/************************************************************************/
bool CRoundHeadRectangle::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CRect checkRect = CRect( m_Start, m_End );
	if(checkRect.PtInRect( pt ))
	{
	//	m_objLogFile.WriteLog(_T("CRoundHeadRectangle pt(%d, %d) is in the [(%d, %d), (%d, %d)]. \n"), 
	//		pt.x, pt.y, checkRect.left, checkRect.top, checkRect.right, checkRect.bottom);
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	return flag;
}

/************************************************************************/
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
/************************************************************************/
int CRoundHeadRectangle::IsConnectOn(CAdjustPoint *pt)
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
bool CRoundHeadRectangle::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	// ȡ��ͼԪ�����½����ꡣ
	// CPoint startPoint = CPoint( m_Start.x, m_End.y );
	// ȡ��ͼԪ�����Ͻ����ꡣ
	// CPoint endPoint = CPoint(m_End.x, m_Start.y);

	CAdjustPoint *connPoint = NULL;

//	m_objLogFile.WriteLog(_T("m_Points(0-3) is [(%d, %d), (%d, %d), (%d, %d), (%d, %d)]. "), 
//		((CAdjustPoint *)m_Points.GetAt(0))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(0))->GetPoint().y, 
//		((CAdjustPoint *)m_Points.GetAt(1))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(1))->GetPoint().y, 
//		((CAdjustPoint *)m_Points.GetAt(2))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(2))->GetPoint().y, 
//		((CAdjustPoint *)m_Points.GetAt(3))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(3))->GetPoint().y);
//	m_objLogFile.WriteLog(_T("m_Points(4-7) is [(%d, %d), (%d, %d), (%d, %d), (%d, %d)]. \n"), 
//		((CAdjustPoint *)m_Points.GetAt(4))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(4))->GetPoint().y, 
//		((CAdjustPoint *)m_Points.GetAt(5))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(5))->GetPoint().y, 
//		((CAdjustPoint *)m_Points.GetAt(6))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(6))->GetPoint().y, 
//		((CAdjustPoint *)m_Points.GetAt(7))->GetPoint().x, 
//		((CAdjustPoint *)m_Points.GetAt(7))->GetPoint().y);

	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		if(connPoint->IsOn(pt))
		{
			// m_objLogFile.WriteLog("We click on the %dth CAdjustPoint.\n", i);
			// connPoint->IsOn(pt);
			//if(i == CCONNECTPOINT_RECT_LEFT_BOTTOM || i == CCONNECTPOINT_RECT_RIGHT_TOP)
			//{
			//	m_objLogFile.WriteLog(_T("The m_Start and m_End is [(%d, %d), (%d, %d)]. "), 
			//		m_Start.x, m_Start.y, m_End.x, m_End.y);
			//	m_Start = startPoint;
			//	m_End = endPoint;
			//	m_objLogFile.WriteLog(_T("    Switch m_Start and m_End is [(%d, %d), (%d, %d)].\n"), 
			//		m_Start.x, m_Start.y, m_End.x, m_End.y);
			//}
			m_AdjustPoint = i; // 1+i;
		    flag = true;
			// break;
		}
	}

	return flag;
}

/************************************************************************/
/* ���ܣ��ڵ�����С������ת��ʱ�򣬸��ݵ������������ʼ��ͽ��������ꡣ */
/************************************************************************/
void CRoundHeadRectangle::AdjustStartAndEnd()
{
	CPoint newStart, newEnd;
	// �������������ʼ������Ϸ������ʱ����ʼ���ɽ����㡣����������ʼ�㡣
	// �������ֻ�ᷢ�����϶����·������·������������¡�
	// ֱ�ӽ�����ʼ��ͽ��������꼴�ɡ�
	if((m_End.x < m_Start.x) && (m_End.y < m_Start.y))
	{
		m_objLogFile.WriteLog(_T("The m_Start and m_End is [(%d, %d), (%d, %d)]. "), 
			m_Start.x, m_Start.y, m_End.x, m_End.y);
		newEnd = m_Start;
		m_Start = m_End;
		m_End = newEnd;
		m_objLogFile.WriteLog(_T("Revert m_Start and m_End is [(%d, %d), (%d, %d)].\n"), 
			m_Start.x, m_Start.y, m_End.x, m_End.y);
	}
	// ��������㲻����ʼ������·���˵�����������
	//    1. ����������ʼ������Ϸ�����ʱ�����ο�����X�᷽��ķ�ת��
	//       �������ֻ�ᷢ�����϶����·������·���������Y���϶�������¡���ʱ����������£�
	//       ��ʼ���X��ɽ������X����ʼ���Y���䡣
	//       �������X�����ʼ���X���������Y���䡣
	//    1. ����������ʼ������·�����ʱ�����ο�����Y�᷽��ķ�ת��
	//       �������ֻ�ᷢ�����϶����Ϸ������·���������X���϶�������¡���ʱ����������£�
	//       ��ʼ���X��ɽ������X����ʼ���Y���䡣
	//       �������X�����ʼ���X���������Y���䡣
	// �����������߼���
	else if(!((m_End.x > m_Start.x) && (m_End.y > m_Start.y)))
	{
		m_objLogFile.WriteLog(_T("The m_Start and m_End is [(%d, %d), (%d, %d)]. "), 
			m_Start.x, m_Start.y, m_End.x, m_End.y);
		newStart = CPoint( m_End.x, m_Start.y );
		newEnd = CPoint( m_Start.x, m_End.y );
		m_Start = newStart;
		m_End = newEnd;
		m_objLogFile.WriteLog(_T("    Recalc m_Start and m_End is [(%d, %d), (%d, %d)].\n"), 
			m_Start.x, m_Start.y, m_End.x, m_End.y);
	}

	AdjustFocusPoint();
}

int CRoundHeadRectangle::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
/************************************************************************/
void CRoundHeadRectangle::AdjustFocusPoint()
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
void CRoundHeadRectangle::SaveParamsToJSON(cJSON * objJSON)
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

void CRoundHeadRectangle::LoadParamsFromJSON(cJSON * objJSON)
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
