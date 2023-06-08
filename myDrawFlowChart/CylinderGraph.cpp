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
/* 功能：建构函数。设定了连接点。                                       */
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
/* 功能：绘制函数。绘制了一个圆角矩形和上面的文字。                     */
/************************************************************************/
void CCylinderGraph::Draw( CDC *pdc )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
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
			/* 令圆弧高度为图元高度的一半。根据上面的计算，有：              */
		    /*          H/2 = ArcHeight = R - sqrt(R * R - (W/2) * (W/2))    */
		    /*     整理有：                                                  */
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
/* 功能：选中绘制函数。绘制了连接点。                                   */
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
/* 功能： 移动处理函数。                                                */
/************************************************************************/
void CCylinderGraph::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CCylinderGraph::AdjustSize( CPoint &pt )
{
//	m_objLogFile.WriteLog(_T("The m_Start and m_End is [(%d, %d), (%d, %d)]. "), 
//		m_Start.x, m_Start.y, m_End.x, m_End.y);
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
//	m_objLogFile.WriteLog(_T("AdjustSize m_Start and m_End is [(%d, %d), (%d, %d)].\n"), 
//		m_Start.x, m_Start.y, m_End.x, m_End.y);
}

/************************************************************************/
/* 功能：判断是否在图元区域内。                                         */
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
/* 功能： 判断一个连接点是否在图元边界上。用于调整图元是否连接。        */
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
/* 功能： 判断一个屏幕坐标是否在图元边界上。用于调整图元大小。          */
/************************************************************************/
bool CCylinderGraph::IsOn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	// 取得图元的左下角坐标。
	// CPoint startPoint = CPoint( m_Start.x, m_End.y );
	// 取得图元的右上角坐标。
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
/* 功能：在调整大小发生翻转的时候，根据调整结果交换起始点和结束点坐标。 */
/************************************************************************/
void CCylinderGraph::AdjustStartAndEnd()
{
	CPoint newStart, newEnd;
	// 如果结束点在起始点的左上方。这个时候，起始点变成结束点。结束点变成起始点。
	// 这种情况只会发生在拖动左下方和右下方调整点的情况下。
	// 直接交换起始点和结束点坐标即可。
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
	// 如果结束点不在起始点的右下方。说明两种情况：
	//    1. 结束点在起始点的右上方。此时，矩形框发生沿X轴方向的翻转。
	//       这种情况只会发生在拖动左下方和右下方调整点沿Y轴拖动的情况下。此时坐标调整如下：
	//       起始点的X变成结束点的X，起始点的Y不变。
	//       结束点的X变成起始点的X，结束点的Y不变。
	//    1. 结束点在起始点的左下方。此时，矩形框发生沿Y轴方向的翻转。
	//       这种情况只会发生在拖动右上方和右下方调整点沿X轴拖动的情况下。此时坐标调整如下：
	//       起始点的X变成结束点的X，起始点的Y不变。
	//       结束点的X变成起始点的X，结束点的Y不变。
	// 这就是下面的逻辑。
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
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
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
/* 功能：串行化操作。                                                   */
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
