// LiangBarskyClipLine.cpp: implementation of the CLiangBarskyClipLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "LiangBarskyClipLine.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define AREA_LEFT   0X1   //����:0001
#define AREA_RIGHT  0X2   //����:0010
#define AREA_BOTTOM 0X4   //����:0100
#define AREA_TOP    0X8   //����:1000

#define WXL   -300
#define WYB   -100
#define WXR    300
#define WYT    100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CLiangBarskyClipLine, CObject, 1)

/************************************************************************/
/* ���ܣ������������趨�����ӵ㡣                                       */
/************************************************************************/
CLiangBarskyClipLine::CLiangBarskyClipLine()
{
	m_AdjustPoint = CCONNECTPOINT_INVALID_OPTION;

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}
}

CLiangBarskyClipLine::~CLiangBarskyClipLine()
{

}

/************************************************************************/
/* Liang-Barsky�ü�����                                                 */
/* ����㷨�����ü������衣����һ����������                             */ 
/************************************************************************/
/* ��һ��������һ���߶���˵������ΪP1 = (x1, y1)��P2 = (x2, y2)��       */
/* ��ô�У�                                                             */
/*       x = x1 + u * (x2 - x1) 0<=u<=1         - ��ʽ(1)               */
/*       y = y1 + u * (y2 - y1) 0<=u<=1         - ��ʽ(2)               */
/* ����ֻҪ������ͬ��uֵ�����ǾͿ��Եõ�����߶��ϵ����е㡣            */
/* ���磺                                                               */
/*       ��u = 0ʱ��x = x1, y = y1 Ҳ����P1                             */
/*       ��u = 1ʱ��x = x2, y = y2 Ҳ����P2                             */
/* ���仰˵��uԽ��Խ�ӽ�P2��uԽСԽ�ӽ�P1��                             */
/* �Զ��׼����ǣ�����ֻҪ����һ�����ʵ�uֵ��                            */
/* ���ǾͿ��Եõ�һ����Ӧ�Ĳü��㡣                                     */
/* ����ϣ����ǰ����ֱ�߲ü�����ת��Ϊ������uֵ�����ﶨ��Ϊu1��u2��    */
/************************************************************************/
/* �ڶ�����������Ҫ���ü�ֱ�߿�����һ���з�����ߡ�                     */
/* Ҳ���Ǵӿ���ԭ��ָ��Զ��ԭ���һ���з�����ߡ�                       */
/* �������ǰ�ᣬ���ǽ��ĸ����ڵĽ��߷ֱ��������ߣ���ߺͳ��ߡ�     */
/*     ��ߣ�ָ�Ӳü�����֮����뵽�ü����ڷ���ı�                     */
/*     ���ߣ�ָ�Ӳü�����֮�����쵽����֮��ı�                         */
/* ������ǣ����Ǵ��ü��߶�ƽ���ڱ߽��ߣ�                               */
/* ������ü��߶ε��ӳ��ߺͲü����ڱض������ĸ����㣬                   */
/* Ҫô�ཻ�ڲü����ڣ�Ҫô�ཻ�ڲü����ڵ��ӳ��ߡ�                     */
/************************************************************************/
/* ������������ֻҪ��һ�£����ܿ���������Ĺ�ʽ��                       */
/*     u1 = max(c1, c2, P1) u1��������ߺ�P1��Ӧuֵ����Сֵ  - ��ʽ(3)  */
/*     u2 = min(c3, P2, c4) u2���������ߺ�P2��Ӧuֵ�����ֵ  - ��ʽ(4)  */
/* ���ֻҪ��һ�¾Ͳ�����⡣��Ȼ�Ǵ��ü�ֱ����һ���з�����ߡ�         */
/* ����P1��P2�ľ����㹻Զ������u���������Ǵ�P1�˶���P2�Ĺ����С�      */
/* �ڽ��뵽�ü�����֮ǰ��                                               */
/* ����һ�����ȽӴ����ü����ڵ��ӳ��ߣ�֮��Ӵ����ü����ڡ�             */
/* ����ϣ����������˵����Ч��һ���Ǻ���ֵ�ֵ��                       */
/* Ҳ����������ߵ�maxֵ��                                              */
/* ��Ȼ���P1���ڲü��������档������Ȼѡ��P1�Լ���                     */
/* ��P1�Լ�Ҳǡ����������ߺ����ֵ�������max(c1, c2, P1)�ĺ��塣      */
/* ͬ���֪�����뿪�ü�����֮ǰ��                                       */
/* ����һ�����ȽӴ����ü����ڣ�֮��Ӵ����ü����ڵ��ӳ��ߡ�             */
/* ����ϣ����������˵����Ч��һ�����ȳ��ֵ�ֵ��                       */
/* Ҳ�����������ߵ�minֵ��                                              */
/* ��Ȼ���P2���ڲü��������档������Ȼѡ��P2�Լ���                     */
/* ��P2�Լ�Ҳǡ�����������ǰ���ֵ�������min(c3, P2, c4)�ĺ��塣      */
/* ����ϣ����ǰ����ֱ�߲ü�����ת��Ϊ������u1��u2��                 */
/* ��һ����˵�����ǰ����ֱ�߲ü�����ת��Ϊ�����������⣺               */
/*     �������ĸ�����c1, c2, c3, c4����Ӧ��uֵ��                      */
/*     ���ȷ�����������ǳ��ߣ�������������ߡ�                         */
/************************************************************************/
/* ���Ĳ����������������c1, c2, c3, c4����Ӧ��uֵ��                    */
/* ������ü����ڵ��ϱ߽�Ϊm_End.y���±߽�Ϊm_Start.y��                 */
/* ��߽�Ϊm_Start.x���ұ߽�Ϊm_End.x�����ݹ�ʽ(1)�͹�ʽ(2)�����У�     */
/*       m_Start.x <= x1 + u * (x2 - x1) <= m_End.x  - ��ʽ(5)          */
/*       m_Start.y <= y1 + u * (y2 - y1) <= m_End.y  - ��ʽ(6)          */
/* ���Կ�����                                                           */
/*       x1 + u1 * (x2 - x1) = m_Start.x                                */
/*       x1 + u2 * (x2 - x1) = m_End.x                                  */
/*       y1 + u3 * (y2 - y1) = m_Start.y                                */
/*       y1 + u4 * (y2 - y1) = m_End.y                                  */
/* ʱ��Ϊ�ü�ֱ�ߺ��ĸ��߽�Ľ���ֵ����������ĸ���Ӧ��uֵ��            */
/* ���㹫ʽ���£�                                                       */
/*       u1 = (x1 - m_Start.x)/(x2 - x1)          - ��ʽ(7)             */
/*       u2 = (m_End.x - x1)/(x2 - x1)            - ��ʽ(8)             */
/*       u3 = (y1 - m_Start.y)/(y2 - y1)          - ��ʽ(9)             */
/*       u4 = (m_End.y - y1)/(y2 - y1)            - ��ʽ(10)            */
/************************************************************************/
/* ���Ĳ������ǵõ��˼��㹫ʽ�����ǻ���Ҫȷ���������u����߻��ǳ��ߡ�  */
/* ���Ǹ��ݹ�ʽ(5)�͹�ʽ(6)�����ǽ�һ���Ƶ��У�                         */
/*      u * (x1 - x2) <= x1 - m_Start.x            - ��ʽ(11)           */
/*      u * (x2 - x1) <= m_End.x - x1              - ��ʽ(12)           */
/*      u * (y1 - y2) <= y1 - m_Start.y            - ��ʽ(13)           */
/*      u * (y2 - y1) <= m_End.y - y1              - ��ʽ(14)           */
/* ����(x1 - x2), (x2 - x1), (y1 - y2), (y2 - y1)���ĸ�ֵ�����ǳ���Pk�� */
/* ���PkС����ʱ��Ҳ���ǹ�ʽ(11)�͹�ʽ(13)��ǡ���Ǵ���m_Start��xy���ꡣ*/
/* ��ʱ���߶δӲü��߽��ӳ��ߵ��ڲ����쵽�ⲿ��Ҳ������ߡ�             */
/* ���Pk������ʱ��Ҳ���ǹ�ʽ(12)�͹�ʽ(14)��ǡ���Ǵ���m_End��xy���ꡣ  */
/* ��ʱ���߶δӲü��߽��ӳ��ߵ��ڲ����쵽�ⲿ��Ҳ���ǳ��ߡ�             */
/*                                                                      */
/* ����(x1 - m_Start.x), (m_End.x - x1), (y1 - m_Start.y),              */
/*     (m_End.y - y1)���ĸ�ֵ�����ǳ���Qk��                             */
/* ����ϣ��������u��4����ʽ�ͱ���������ͨ�ù�ʽ��                   */
/*     u = Pk / Qk                                  - ��ʽ(15)          */
/************************************************************************/
/* ����������Ĳ��裬���Ǿ�����������Ĵ��롣                           */
/* �������Ǹ���ʹ�ù�ʽ(7-10)���м��㡣����P1��XY���궼��P0�������   */
/* �Ǿ���ζ��dx, dy����������                                           */
/* ����ϣ�ʹ�ù�ʽ(7)�͹�ʽ(9)��ʱ�򣬴����pkΪ����                   */
/* ��calcSegmentRatio�����u��������������Ӧ�Ĳü�λ�ñ���ֵtmax��    */
/* �������μ����У����ݹ�ʽ(3)�����ǵõ������μ�������ֵ��            */
/* ����ǲü��������Ӧ�Ĳü�λ�ñ���ֵ��                             */
/* ͬ�����ǣ�ʹ�ù�ʽ(8)�͹�ʽ(10)��ʱ�򣬴����pkΪ����                */
/* ��calcSegmentRatio�����u�����������յ��Ӧ�Ĳü�λ�ñ���ֵtmin��    */
/* �������μ����У����ݹ�ʽ(4)�����ǵõ������μ������Сֵ��            */
/* ����ǲü�����յ��Ӧ�Ĳü�λ�ñ���ֵ��                             */
/* ��������ǵõ�����������ֵ�����ǾͿ�����tmax�����µ���㡣         */
/* ��tmin�����µ��յ㡣                                                 */
/************************************************************************/
BOOL CLiangBarskyClipLine::LBLineClip(void)
{
	double tmax;          // �ü��������Ӧ�Ĳü�λ�ñ���ֵ��
	tmax = 0.0;           // ��ֵĬ��Ϊ0��Ҳ����λ���߶���ʼλ��P0��
                          
	double tmin;          // �ü�����յ��Ӧ�Ĳü�λ�ñ���ֵ��
	tmin = 1.0;           // ��ֵĬ��Ϊ1��Ҳ����λ���߶ν���λ��P1��
                          
	double dx, dy;	      // ������ʼλ�úͽ���λ����XY�����ϵľ��롣
	dx = P1.x - P0.x;     // ��Ϊ�������ᷴ��ʹ�õ����ֵ��
	dy = P1.y - P0.y;     // Ԥ�ȼ�������ȽϷ��㡣

	// ���濪ʼLiang-Barsky�ü��㷨��
	// ʹ�ô��ڱ߽磬�������ҡ��¡���˳��ü�ֱ�ߡ�
	// �����˳��ֻ�����ǲ�����ǰ�湫ʽ��˳����ѡ�����Ϊ���Ĳ�����������ϡ�
	// ʹ�ù�ʽ(7)���м��㡣n��1,��߽�u1������x��v1��x0��wxl
	if (calcSegmentRatio(-dx, P0.x - m_Start.x, tmax, tmin))
	{
	    // ʹ�ù�ʽ(8)���м��㡣n��2���ұ߽�u2����x��v2��wxr��x0
		if (calcSegmentRatio(dx, m_End.x - P0.x, tmax, tmin))
		{
			// ʹ�ù�ʽ(9)���м��㡣n��3���±߽�u3������y��v3��y0-wyb
			if (calcSegmentRatio(-dy, P0.y - m_Start.y, tmax, tmin))
			{
				// ʹ�ù�ʽ(10)���м��㡣n��4���ϱ߽�u4����y��v4��wyt-y0
				if (calcSegmentRatio(dy, m_End.y - P0.y, tmax, tmin))
				{
					// ����ִ�е��������������������Ҫ�Ĳü�λ�ñ���ֵ��
					// ��tmin�����µ��յ㡣 
					if (tmin < 1) // �ж�ֱ�ߵ��յ�
					{
						P1.x = P0.x + tmin * dx; // ���¼���ֱ���յ�����
						P1.y = P0.y + tmin * dy; // x��x0��t(x1��x0)��ʽ
					}
					// ��tmax�����µ���㡣
					if (tmax > 0) // �ж�ֱ�ߵ����
					{
						P0.x = P0.x + tmax * dx; // ���¼���ֱ���������
						P0.y = P0.y + tmax * dy; // x��x0��t(x1��x0)��ʽ
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/************************************************************************/
/* ��������Ĺ�����ʵ���Ǽ���c1, c2, c3, c4��Ӧ��4��u��ֵ��             */
/* Ҳ���ǲü�λ�����߶��ϵı���ֵ��                                     */
/* �ú����ᱻ�����ĴΡ��������ҡ��¡���˳��                           */
/* �����ü�λ�����߶��ϵı���ֵ��                                       */
/* ����tmax��������Ӧ�Ĳü�λ�ñ���ֵ��                               */
/* ����tmin�����յ��Ӧ�Ĳü�λ�ñ���ֵ��                               */
/************************************************************************/
BOOL CLiangBarskyClipLine::calcSegmentRatio(
		double p, double q, double &tmax, double &tmin)
{
	double t;
	BOOL ReturnValue = TRUE;
	// ���PkС����ʱ���߶δӲü��߽��ӳ��ߵ��ڲ����쵽�ⲿ��Ҳ������ߡ�
	// ��ʱֱ�߶δӴ��ڱ߽�Ĳ��ɼ��ൽ�ɼ���,������㴦��tmax
	if (p < 0)
	{
		// ����Uֵ��
		t = q / p;
		// ���������������tmin��˵���ü�λ�ó��ֽ��������ء�
		if (t > tmin)
			ReturnValue = FALSE;
		// ���������С��tmin������tmax��˵����Ҫ�ü�������tmax��ֵ��
		else if (t > tmax)
			tmax = t;
		// ���������С��tmax��˵���߶κͲü��߽�����ǲü��߽��ӳ���û���ཻ��
		// ��ʱ�����β���Ҫ���вü���ʲô����������
	}
	// ���Pk������ʱ���߶δӲü��߽��ӳ��ߵ��ڲ����쵽�ⲿ��Ҳ���ǳ��ߡ�
	// ��ʱ��ֱ�߶δӴ��ڱ߽�Ŀɼ��ൽ���ɼ��࣬�����յ㴦��tmin
	else if (p > 0)
	{
		// ����Uֵ��
		t = q / p;
		// �����������С��tmax��˵���ü�λ�ó��ֽ��������ء�
		if (t < tmax)
			ReturnValue = FALSE;
		// ���������С��tmin������tmax��˵����Ҫ�ü�������tmin��ֵ��
		else if (t < tmin)
			tmin = t;
		// �������������tmin��˵���߶κͲü��߽�����ǲü��߽��ӳ���û���ཻ��
		// ��ʱ�����β���Ҫ���вü���ʲô����������
	}
	// ���Pk������ʱ��˵����ƽ���ڴ��ڱ߽��ֱ�ߡ�
	else 
	{
		// ֱ�߶��ڴ����⣬��ֱ��ɾ��
		if (q < 0)
		{
			ReturnValue = FALSE;
		}
	}
	return ReturnValue;
}

/************************************************************************/
/* ���ܣ����ƺ�����������һ����Բ����������֡�                         */
/************************************************************************/
#define ClipLine_DIFF     40
void CLiangBarskyClipLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //��ʼ�����ʣ���ɫ�� 
        pOldPen=pdc-> SelectObject(&p);     //�ѻ���ѡ��DC��������ԭ������
	}

	pdc->Rectangle( CRect(m_Start, m_End) );
	LBLineClip();
	pdc->MoveTo(P0.x, P0.y);
	pdc->LineTo(P1.x, P1.y);

	if(m_IsMark)
	{
		pdc->SelectObject(pOldPen);
	}
	pdc->DrawText(m_text, CRect(m_Start+CPoint(8, 8), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CLiangBarskyClipLine::DrawFocus( CDC *pdc )
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
void CLiangBarskyClipLine::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ� ��С������������                                            */
/*        ����IsOn�����������õ�׼�����д�С���������ӵ㣬���е�����  */
/************************************************************************/
void CLiangBarskyClipLine::AdjustSize( CPoint &pt )
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
bool CLiangBarskyClipLine::IsIn( CPoint &pt )
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
int CLiangBarskyClipLine::IsConnectOn(CAdjustPoint *pt)
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
bool CLiangBarskyClipLine::IsOn( CPoint &pt )
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
void CLiangBarskyClipLine::AdjustStartAndEnd()
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

int CLiangBarskyClipLine::GetAdjustPoint()
{
	return m_AdjustPoint;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ڴ�С���������ߵ����ӵ����ꡣ     */
/************************************************************************/
void CLiangBarskyClipLine::AdjustFocusPoint()
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
	
	P0 = CPoint(m_Start.x + ClipLine_DIFF, m_End.y + ClipLine_DIFF);
	P1 = CPoint(m_End.x - ClipLine_DIFF, m_Start.y - ClipLine_DIFF);
}

/************************************************************************/
/* ���ܣ����л�������                                                   */
/************************************************************************/
void CLiangBarskyClipLine::SaveParamsToJSON(cJSON * objJSON)
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

void CLiangBarskyClipLine::LoadParamsFromJSON(cJSON * objJSON)
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