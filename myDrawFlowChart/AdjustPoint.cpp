// AdjustPoint.cpp: implementation of the CAdjustPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "AdjustPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CAdjustPoint, CObject, 1)

CAdjustPoint::CAdjustPoint()
{
	m_type = true;
}

CAdjustPoint::~CAdjustPoint()
{

}

void CAdjustPoint::Draw(CDC *pdc)
{
	CBrush brush(RGB(0,255,0));
	CBrush* oldbrush = pdc->SelectObject(&brush);

	if(m_type)
	{
		pdc->Rectangle(CRect(m_Point+CPoint(-3, -3), m_Point+CPoint(3, 3)));
	}
	else
	{
		pdc->Ellipse( CRect(m_Point+CPoint(3, 3), m_Point+CPoint(-3, -3)) );
	}

	pdc->SelectObject(oldbrush);
}

void CAdjustPoint::SetType(bool type)
{
	m_type = type;
}

void CAdjustPoint::SetPoint(CPoint &pt)
{
	m_Point = pt;
}

CPoint CAdjustPoint::GetPoint()
{
	return m_Point;
}

bool CAdjustPoint::IsOn(CPoint &pt)
{
	bool flag = false;
	CRect checkRect = CRect(m_Point + CPoint(
									ADJUSTPOINT_NEGATIVE_X_MARGIN, 
									ADJUSTPOINT_NEGATIVE_Y_MARGIN), 
						m_Point+CPoint(
									ADJUSTPOINT_POSITIVE_X_MARGIN, 
									ADJUSTPOINT_POSITIVE_Y_MARGIN));
	if(checkRect.PtInRect(pt))
	{
		m_objLogFile.WriteLog(_T("CAdjustPoint pt(%d, %d) is in the [(%d, %d), (%d, %d)]. \n"), 
			pt.x, pt.y, checkRect.left, checkRect.top, checkRect.right, checkRect.bottom);
		flag = true;
	}
	return flag;
}

void CAdjustPoint::SaveParamsToJSON(cJSON * objJSON)
{
//	if(ar.IsStoring())
//	{
//		ar<<m_Point;
//	}
//	else
//	{
//		ar>>m_Point;
//	}
	cJSON * jsonPoint = cJSON_CreateObject();
	cJSON_AddNumberToObject(objJSON, "x", m_Point.x);
	cJSON_AddNumberToObject(objJSON, "y", m_Point.y);
}

void CAdjustPoint::LoadParamsFromJSON(cJSON * objJSON)
{
	CPoint point;
	cJSON *child = objJSON->child;
    while(child)
    {  
        switch ((child->type)&255)
        {  
        case cJSON_Number:    
            {   
				if(strcmp(child->string, "x") == 0)
				{   
					point.x = (int)child->valueint;
				}
				else if(strcmp(child->string, "y") == 0)
				{   
					point.y = (int)child->valueint;
				}
            }   
            break;
        }   
        child = child->next ;
    }
	SetPoint(point);
}