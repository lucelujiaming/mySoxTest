// RoundRectangle.cpp: implementation of the CRoundRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "RoundRectangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CRoundRectangle, CObject, 1)

/************************************************************************/
/* ���ܣ������������趨�����ӵ㡣                                       */
/************************************************************************/
CRoundRectangle::CRoundRectangle()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CConnectPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CConnectPoint();
		m_Points.Add(connPoint);
	}
}

CRoundRectangle::~CRoundRectangle()
{

}

/************************************************************************/
/* ���ܣ����ƺ�����������һ�����κ���������֡�                         */
/************************************************************************/
void CRoundRectangle::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pdc-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
	}

	if((m_End.x - m_Start.x) >= (m_End.y - m_Start.y))
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
	else
	{
		// Top Semicircle
		pdc->Arc( CRect(m_Start, CPoint(m_End.x, m_Start.y + (m_End.x - m_Start.x))), 
			CPoint((m_End.x > m_Start.x)?m_End.x:m_Start.x, m_Start.y + (m_End.x - m_Start.x)/2),
			CPoint((m_End.x > m_Start.x)?m_Start.x:m_End.x, m_Start.y + (m_End.x - m_Start.x)/2));
		// Bottom Semicircle
		pdc->Arc( CRect(CPoint(m_Start.x, m_End.y - (m_End.x - m_Start.x)), m_End), 
			CPoint((m_End.x > m_Start.x)?m_Start.x:m_End.x, m_End.y - (m_End.x - m_Start.x)/2),
			CPoint((m_End.x > m_Start.x)?m_End.x:m_Start.x, m_End.y - (m_End.x - m_Start.x)/2));
		// Left Line
		pdc->MoveTo(CPoint(m_Start.x, m_Start.y + (m_End.x - m_Start.x)/2));
		pdc->LineTo(CPoint(m_Start.x,   m_End.y - (m_End.x - m_Start.x)/2));
		// Right Line
		pdc->MoveTo(CPoint(m_End.x, m_End.y - (m_End.x - m_Start.x)/2));
		pdc->LineTo(CPoint(m_End.x, m_Start.y + (m_End.x - m_Start.x)/2));
	}

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	pdc->DrawText(m_text, CRect(m_Start + CPoint(
									ROUNDED_RECTANGLE_TEXT_XBORDER, ROUNDED_RECTANGLE_TEXT_YBORDER), 
								m_End+CPoint(
									-1 * ROUNDED_RECTANGLE_TEXT_XBORDER, 
									-1 * ROUNDED_RECTANGLE_TEXT_YBORDER)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CRoundRectangle::DrawFocus( CDC *pdc )
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.GetSize(); i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->Draw(pdc);
	}
}

/************************************************************************/
/* ���ܣ� �ƶ�����������                                                */
/************************************************************************/
void CRoundRectangle::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С��������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CRoundRectangle::AdjustSize( CPoint &pt )
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
bool CRoundRectangle::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CRect checkRect = CRect( m_Start, m_End );
	if(checkRect.PtInRect( pt ))
	{
	//	m_objLogFile.WriteLog(_T("CRoundRectangle pt(%d, %d) is in the [(%d, %d), (%d, %d)]. \n"), 
	//		pt.x, pt.y, checkRect.left, checkRect.top, checkRect.right, checkRect.bottom);
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	return flag;
}

/************************************************************************/
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
/************************************************************************/
int CRoundRectangle::IsConnectOn(CConnectPoint *pt)
{
	CConnectPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
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
bool CRoundRectangle::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	// ȡ��ͼԪ�����½����ꡣ
	// CPoint temp1 = CPoint( m_Start.x, m_End.y );
	// ȡ��ͼԪ�����Ͻ����ꡣ
	// CPoint temp2 = CPoint(m_End.x, m_Start.y);

	CConnectPoint *connPoint = NULL;

//	m_objLogFile.WriteLog(_T("m_Points(0-3) is [(%d, %d), (%d, %d), (%d, %d), (%d, %d)]. "), 
//		((CConnectPoint *)m_Points.GetAt(0))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(0))->GetPoint().y, 
//		((CConnectPoint *)m_Points.GetAt(1))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(1))->GetPoint().y, 
//		((CConnectPoint *)m_Points.GetAt(2))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(2))->GetPoint().y, 
//		((CConnectPoint *)m_Points.GetAt(3))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(3))->GetPoint().y);
//	m_objLogFile.WriteLog(_T("m_Points(4-7) is [(%d, %d), (%d, %d), (%d, %d), (%d, %d)]. \n"), 
//		((CConnectPoint *)m_Points.GetAt(4))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(4))->GetPoint().y, 
//		((CConnectPoint *)m_Points.GetAt(5))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(5))->GetPoint().y, 
//		((CConnectPoint *)m_Points.GetAt(6))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(6))->GetPoint().y, 
//		((CConnectPoint *)m_Points.GetAt(7))->GetPoint().x, 
//		((CConnectPoint *)m_Points.GetAt(7))->GetPoint().y);

	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CConnectPoint *)m_Points.GetAt(i);
		if(connPoint->IsOn(pt))
		{
			// m_objLogFile.WriteLog("We click on the %dth CConnectPoint.\n", i);
			// temp->IsOn(pt);
			//if(i == CCONNECTPOINT_RECT_LEFT_BOTTOM || i == CCONNECTPOINT_RECT_RIGHT_TOP)
			//{
			//	m_objLogFile.WriteLog(_T("The m_Start and m_End is [(%d, %d), (%d, %d)]. "), 
			//		m_Start.x, m_Start.y, m_End.x, m_End.y);
			//	m_Start = temp1;
			//	m_End = temp2;
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
void CRoundRectangle::AdjustStartAndEnd()
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

int CRoundRectangle::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
/************************************************************************/
void CRoundRectangle::AdjustFocusPoint()
{
	CConnectPoint *connPoint = NULL;
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_TOP);
	connPoint->SetPoint(m_Start);
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_LEFT_BOTTOM);
	connPoint->SetPoint(CPoint(m_Start.x, m_End.y));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_TOP);
	connPoint->SetPoint(CPoint(m_End.x, m_Start.y));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_RIGHT_BOTTOM);
	connPoint->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_RECT_CNT; i++)
	{
		connPoint = (CConnectPoint *)m_Points.GetAt(i);
		connPoint->SetType(false);
	}

	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_TOP);
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_RIGHT);
	connPoint->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_BOTTOM);
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	connPoint = (CConnectPoint *)m_Points.GetAt(CCONNECTPOINT_RECT_MIDDLE_LEFT);
	connPoint->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

/************************************************************************/
/* ���ܣ����л�������                                                   */
/************************************************************************/
void CRoundRectangle::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
	}
	else
	{
		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
	}

	m_Points.Serialize(ar);
}