// CustomRoundRectangle.cpp: implementation of the CCustomRoundRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "CustomRoundRectangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CCustomRoundRectangle, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
/************************************************************************/
CCustomRoundRectangle::CCustomRoundRectangle()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
}

CCustomRoundRectangle::~CCustomRoundRectangle()
{

}

/************************************************************************/
/* 功能：绘制函数。绘制了一个圆角矩形和上面的文字。                     */
/************************************************************************/
void CCustomRoundRectangle::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
	}

	if(abs(m_End.x - m_Start.x) > ROUNDED_CORNER_DIAMETER
		&& abs(m_End.y - m_Start.y) > ROUNDED_CORNER_DIAMETER)
	{
		if(m_End.y >= m_Start.y)
		{
			// m_End on the east and south of m_Start
			if (m_End.x >= m_Start.x)
			{
				// Left-top Quarter circle
				pdc->Arc( CRect(m_Start, m_Start + CPoint(ROUNDED_CORNER_DIAMETER, ROUNDED_CORNER_DIAMETER)), 
					m_Start + CPoint(ROUNDED_CORNER_RADIUS, 0),
					m_Start + CPoint(0, ROUNDED_CORNER_RADIUS));
				// Right-top Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x - ROUNDED_CORNER_DIAMETER, m_Start.y), 
						        CPoint(m_End.x, m_Start.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_End.x, m_Start.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_Start.y));
				// Left-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x, m_End.y - ROUNDED_CORNER_DIAMETER), 
						        CPoint(m_Start.x + ROUNDED_CORNER_DIAMETER, m_End.y)), 
					CPoint(m_Start.x, m_End.y - ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_End.y));
				// Right-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x - ROUNDED_CORNER_DIAMETER, m_End.y - ROUNDED_CORNER_DIAMETER), m_End), 
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_End.y),
					CPoint(m_End.x, m_End.y - ROUNDED_CORNER_RADIUS));
				// Top Line
				// pdc->MoveTo(CPoint(m_Start + CPoint(ROUNDED_CORNER_RADIUS, 0)));
				// pdc->LineTo(CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_Start.y));
				pdc->Rectangle(CRect(CPoint(m_Start + 
										CPoint(ROUNDED_CORNER_RADIUS, CONNECTPOINT_NEGATIVE_RADIUS)),
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_Start.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Bottom Line
				// pdc->MoveTo(CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_End.y));
				// pdc->LineTo(CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_End.y));
				pdc->Rectangle(CRect(CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_NEGATIVE_RADIUS),
						CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Left Line
				// pdc->MoveTo(m_Start + CPoint(0, ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_Start.x, m_End.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_Start + CPoint(CONNECTPOINT_NEGATIVE_RADIUS, ROUNDED_CORNER_RADIUS)),
						CPoint(m_Start.x + CONNECTPOINT_POSITIVE_RADIUS, m_End.y - ROUNDED_CORNER_RADIUS)));
				// Right Line
				// pdc->MoveTo(CPoint(m_End.x, m_Start.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_End.x, m_End.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_End.x + CONNECTPOINT_NEGATIVE_RADIUS, m_Start.y + ROUNDED_CORNER_RADIUS),
						CPoint(m_End.x + CONNECTPOINT_POSITIVE_RADIUS, m_End.y - ROUNDED_CORNER_RADIUS)));
			}
			// m_End on the west and south of m_Start
			else
			{
				// Left-top Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x, m_Start.y), 
							CPoint(m_End.x + ROUNDED_CORNER_DIAMETER, 
									m_Start.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_Start.y),
					CPoint(m_End.x, m_Start.y + ROUNDED_CORNER_RADIUS));
				// Right-top Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x - ROUNDED_CORNER_DIAMETER, m_Start.y), 
							    CPoint(m_Start.x, m_Start.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_Start.x, m_Start.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_Start.y));
				// Left-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x, m_End.y - ROUNDED_CORNER_DIAMETER), 
							    CPoint(m_End.x + ROUNDED_CORNER_DIAMETER, m_End.y)), 
					CPoint(m_End.x, m_End.y - ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_End.y));
				// Right-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x - ROUNDED_CORNER_DIAMETER, m_End.y - ROUNDED_CORNER_DIAMETER), 
							    CPoint(m_Start.x, m_End.y)), 
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_End.y),
					CPoint(m_Start.x, m_End.y - ROUNDED_CORNER_RADIUS));
				//
				// Top Line
				// pdc->MoveTo(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_Start.y));
				// pdc->LineTo(CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_Start.y));
				pdc->Rectangle(CRect(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_Start.y + CONNECTPOINT_NEGATIVE_RADIUS),
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_Start.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Bottom Line
				// pdc->MoveTo(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_End.y));
				// pdc->LineTo(CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_End.y));
				pdc->Rectangle(CRect(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_NEGATIVE_RADIUS),
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Left Line
				// pdc->MoveTo(CPoint(m_End.x, m_Start.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_End.x, m_End.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_End.x + CONNECTPOINT_NEGATIVE_RADIUS, m_Start.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x + CONNECTPOINT_POSITIVE_RADIUS, m_End.y - ROUNDED_CORNER_RADIUS)));
				// Right Line
				// pdc->MoveTo(CPoint(m_Start.x, m_Start.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_Start.x, m_End.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_Start.x + CONNECTPOINT_NEGATIVE_RADIUS, m_Start.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x + CONNECTPOINT_POSITIVE_RADIUS, m_End.y - ROUNDED_CORNER_RADIUS)));
			}
		}
		else
		{
			// m_End on the east and north of m_Start
			if (m_End.x >= m_Start.x)
			{
				// Left-top Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x, m_End.y), 
						        CPoint(m_Start.x + ROUNDED_CORNER_DIAMETER, m_End.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_End.y),
					CPoint(m_Start.x, m_End.y + ROUNDED_CORNER_RADIUS));
				// Right-top Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x - ROUNDED_CORNER_DIAMETER, m_End.y), 
						        CPoint(m_End.x, m_End.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_End.x, m_End.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_End.y));
				// Left-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x, m_Start.y - ROUNDED_CORNER_DIAMETER), 
						        CPoint(m_Start.x + ROUNDED_CORNER_DIAMETER, m_Start.y)), 
					CPoint(m_Start.x, m_Start.y - ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_Start.y));
				// Right-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x - ROUNDED_CORNER_DIAMETER, 
									   m_Start.y - ROUNDED_CORNER_DIAMETER), CPoint(m_End.x, m_Start.y)), 
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_Start.y),
					CPoint(m_End.x, m_Start.y - ROUNDED_CORNER_RADIUS));
				// Top Line
				// pdc->MoveTo(CPoint(m_Start + CPoint(ROUNDED_CORNER_RADIUS, 0)));
				// pdc->LineTo(CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_Start.y));
				pdc->Rectangle(CRect(CPoint(m_Start + CPoint(ROUNDED_CORNER_RADIUS, CONNECTPOINT_NEGATIVE_RADIUS)),
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_Start.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Bottom Line
				// pdc->MoveTo(CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_End.y));
				// pdc->LineTo(CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_End.y));
				pdc->Rectangle(CRect(CPoint(m_Start.x + ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_NEGATIVE_RADIUS),
					CPoint(m_End.x - ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Left Line
				// pdc->MoveTo(CPoint(m_Start.x, m_End.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_Start.x, m_Start.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_Start.x + CONNECTPOINT_NEGATIVE_RADIUS, m_End.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x + CONNECTPOINT_POSITIVE_RADIUS, m_Start.y - ROUNDED_CORNER_RADIUS)));
				// Right Line
				// pdc->MoveTo(CPoint(m_End.x, m_End.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_End.x, m_Start.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_End.x + CONNECTPOINT_NEGATIVE_RADIUS, m_End.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x + CONNECTPOINT_POSITIVE_RADIUS, m_Start.y - ROUNDED_CORNER_RADIUS)));
			}
			// m_End on the west and north of m_Start
			else
			{
				// Left-top Quarter circle
				pdc->Arc( CRect(m_End, 
						        CPoint(m_End.x + ROUNDED_CORNER_DIAMETER, m_End.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_End.y),
					CPoint(m_End.x, m_End.y + ROUNDED_CORNER_RADIUS));
				// Right-top Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x - ROUNDED_CORNER_DIAMETER, m_End.y), 
						        CPoint(m_Start.x, m_End.y + ROUNDED_CORNER_DIAMETER)), 
					CPoint(m_Start.x, m_End.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_End.y));
				// Left-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_End.x, m_Start.y - ROUNDED_CORNER_DIAMETER), 
						        CPoint(m_End.x + ROUNDED_CORNER_DIAMETER, m_Start.y)), 
					CPoint(m_End.x, m_Start.y - ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_Start.y));
				// Right-bottom Quarter circle
				pdc->Arc( CRect(CPoint(m_Start.x - ROUNDED_CORNER_DIAMETER, 
									   m_Start.y - ROUNDED_CORNER_DIAMETER), m_Start), 
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_Start.y),
					CPoint(m_Start.x, m_Start.y - ROUNDED_CORNER_RADIUS));
				// Top Line
				// pdc->MoveTo(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_End.y));
				// pdc->LineTo(CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_End.y));
				pdc->Rectangle(CRect(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_NEGATIVE_RADIUS),
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_End.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Bottom Line
				// pdc->MoveTo(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_Start.y));
				// pdc->LineTo(CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_Start.y));
				pdc->Rectangle(CRect(CPoint(m_End.x + ROUNDED_CORNER_RADIUS, m_Start.y + CONNECTPOINT_NEGATIVE_RADIUS),
					CPoint(m_Start.x - ROUNDED_CORNER_RADIUS, m_Start.y + CONNECTPOINT_POSITIVE_RADIUS)));
				// Left Line
				// pdc->MoveTo(CPoint(m_End.x, m_End.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_End.x, m_Start.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_End.x + CONNECTPOINT_NEGATIVE_RADIUS, m_End.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_End.x + CONNECTPOINT_POSITIVE_RADIUS, m_Start.y - ROUNDED_CORNER_RADIUS)));
				// Right Line
				// pdc->MoveTo(CPoint(m_Start.x, m_End.y + ROUNDED_CORNER_RADIUS));
				// pdc->LineTo(CPoint(m_Start.x, m_Start.y - ROUNDED_CORNER_RADIUS));
				pdc->Rectangle(CRect(CPoint(m_Start.x + CONNECTPOINT_NEGATIVE_RADIUS, m_End.y + ROUNDED_CORNER_RADIUS),
					CPoint(m_Start.x + CONNECTPOINT_POSITIVE_RADIUS, m_Start.y - ROUNDED_CORNER_RADIUS)));
			}
		}
	}
	else
	{
		pdc->Ellipse( CRect(m_Start, m_End) );
	}

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}

	pdc->DrawText(m_text, CRect(m_Start + CPoint(
									CUSTOM_ROUNDED_RECTANGLE_TEXT_XBORDER, 
									CUSTOM_ROUNDED_RECTANGLE_TEXT_YBORDER), 
								m_End+CPoint(
									-1 * CUSTOM_ROUNDED_RECTANGLE_TEXT_XBORDER, 
									-1 * CUSTOM_ROUNDED_RECTANGLE_TEXT_YBORDER)), DT_CENTER);
}

/************************************************************************/
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CCustomRoundRectangle::DrawFocus( CDC *pdc )
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
void CCustomRoundRectangle::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CCustomRoundRectangle::AdjustSize( CPoint &pt )
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
bool CCustomRoundRectangle::IsIn( CPoint &pt )
{
	AdjustStartAndEnd();

	bool flag = false;
	CRect checkRect = CRect( m_Start, m_End );
	if(checkRect.PtInRect( pt ))
	{
	//	m_objLogFile.WriteLog(_T("CCustomRoundRectangle pt(%d, %d) is in the [(%d, %d), (%d, %d)]. \n"), 
	//		pt.x, pt.y, checkRect.left, checkRect.top, checkRect.right, checkRect.bottom);
		flag = true;
		m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;
	}
	return flag;
}

/************************************************************************/
/* 功能： 判断一个连接点是否在图元边界上。用于调整图元是否连接。        */
/************************************************************************/
int CCustomRoundRectangle::IsConnectOn(CAdjustPoint *pt)
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
bool CCustomRoundRectangle::IsOn( CPoint &pt )
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
void CCustomRoundRectangle::AdjustStartAndEnd()
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

int CCustomRoundRectangle::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
/************************************************************************/
void CCustomRoundRectangle::AdjustFocusPoint()
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
void CCustomRoundRectangle::SaveParamsToJSON(cJSON * objJSON)
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

void CCustomRoundRectangle::LoadParamsFromJSON(cJSON * objJSON)
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