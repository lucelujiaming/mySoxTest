// CylinderGraph.cpp: implementation of the CCylinderGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "CylinderGraph.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CCylinderGraph, CObject, 1)

/************************************************************************/
/* ���ܣ������������趨�����ӵ㡣                                       */
/************************************************************************/
CCylinderGraph::CCylinderGraph()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
}

CCylinderGraph::~CCylinderGraph()
{

}

/************************************************************************/
/* ���ܣ����ƺ�����������һ��Բ�Ǿ��κ���������֡�                     */
/************************************************************************/
void CCylinderGraph::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pdc-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
	}

	pdc->MoveTo(CPoint(m_End.x, m_Start.y));
	pdc->LineTo(m_End);
	pdc->LineTo(CPoint(m_Start.x, m_End.y));
	pdc->LineTo(m_Start);
	
	int iWidth =0, iHeight = 0;
	iWidth  = m_End.x - m_Start.x;
	iHeight = m_End.y - m_Start.y;
	int  iRadius =  (int)(iWidth * sqrt(2) / 2);
	int  iArcHeight = iRadius - (int)sqrt(iRadius * iRadius - (iWidth/2) * (iWidth/2));

	if(iHeight > 0)
	{
		if(iArcHeight > iHeight/2)
		{
			// We have to recalculate the radius to prevent to cross the topline.
			/* ��Բ���߶�ΪͼԪ�߶ȵ�һ�롣��������ļ��㣬�У�              */
		    /*          H/2 = ArcHeight = R - sqrt(R * R - (W/2) * (W/2))    */
		    /*     �����У�                                                  */
		    /*          R = H/4 + W * W/(4 * H)                              */
			iRadius = iHeight / 4 + iWidth * iWidth/(4 * iHeight);
		}
		
		// We minus one to let the graph close.
		int iCosofRadius = (int)sqrt(iRadius * iRadius - (iWidth/2) * (iWidth/2)) - 1;
		CPoint objFirstCircle = CPoint(m_Start.x + (int)((m_End.x - m_Start.x)/2), m_Start.y - iCosofRadius);
		CRect objFirstCircleRect = CRect(CPoint(objFirstCircle - CPoint(iRadius, iRadius)), 
								CPoint(objFirstCircle + CPoint(iRadius, iRadius)));

		pdc->Arc(objFirstCircleRect, 
					CPoint(m_Start.x, m_Start.y), 
					CPoint(m_End.x, m_Start.y));
		
		CPoint objSecondCircle = CPoint(m_Start.x + (int)((m_End.x - m_Start.x)/2), m_Start.y + iCosofRadius);
		CRect objSecondCircleRect = CRect(CPoint(objSecondCircle - CPoint(iRadius, iRadius)), 
								CPoint(objSecondCircle + CPoint(iRadius, iRadius)));
		pdc->Arc(objSecondCircleRect, 
					CPoint(m_End.x, m_Start.y), 
					CPoint(m_Start.x, m_Start.y));
	}
	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	pdc->DrawText(m_text, CRect(m_Start + CPoint(
									CUSTOM_CYLINDER_TEXT_XBORDER, 
									CUSTOM_CYLINDER_TEXT_YBORDER), 
								m_End+CPoint(
									-1 * CUSTOM_CYLINDER_TEXT_XBORDER, 
									-1 * CUSTOM_CYLINDER_TEXT_YBORDER)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CCylinderGraph::DrawFocus( CDC *pdc )
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
void CCylinderGraph::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CCylinderGraph::AdjustSize( CPoint &pt )
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
bool CCylinderGraph::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CRect checkRect = CRect( m_Start, m_End );
	if(checkRect.PtInRect( pt ))
	{
	//	m_objLogFile.WriteLog(_T("CCylinderGraph pt(%d, %d) is in the [(%d, %d), (%d, %d)]. \n"), 
	//		pt.x, pt.y, checkRect.left, checkRect.top, checkRect.right, checkRect.bottom);
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	return flag;
}

/************************************************************************/
/* ���ܣ� �ж�һ�����ӵ��Ƿ���ͼԪ�߽��ϡ����ڵ���ͼԪ�Ƿ����ӡ�        */
/************************************************************************/
int CCylinderGraph::IsConnectOn(CAdjustPoint *pt)
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
bool CCylinderGraph::IsOn( CPoint &pt )
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
void CCylinderGraph::AdjustStartAndEnd()
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

int CCylinderGraph::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
/************************************************************************/
void CCylinderGraph::AdjustFocusPoint()
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
void CCylinderGraph::Serialize(cJSON * objJSON)
{
//	if(ar.IsStoring())
//	{
//		ar<<m_Start<<m_End<<m_text<<m_AdjustPoint;
//	}
//	else
//	{
//		ar>>m_Start>>m_End>>m_text>>m_AdjustPoint;
//	}

	std::vector<CAdjustPoint *>::iterator iter;
	for (iter = m_Points.begin(); iter != m_Points.end(); iter++)
	{
		CAdjustPoint * objAdjustPoint  = *iter;
		objAdjustPoint->Serialize(objJSON);
	}
}
