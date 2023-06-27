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


#define AREA_LEFT   0X1   //代表:0001
#define AREA_RIGHT  0X2   //代表:0010
#define AREA_BOTTOM 0X4   //代表:0100
#define AREA_TOP    0X8   //代表:1000

#define WXL   -300
#define WYB   -100
#define WXR    300
#define WYT    100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CLiangBarskyClipLine, CObject, 1)

/************************************************************************/
/* 功能：建构函数。设定了连接点。                                       */
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
/* Liang-Barsky裁剪函数                                                 */
/* 这个算法包含好几个步骤。我们一步步来看。                             */ 
/************************************************************************/
/* 第一步，对于一个线段来说，顶点为P1 = (x1, y1)，P2 = (x2, y2)。       */
/* 那么有：                                                             */
/*       x = x1 + u * (x2 - x1) 0<=u<=1         - 公式(1)               */
/*       y = y1 + u * (y2 - y1) 0<=u<=1         - 公式(2)               */
/* 我们只要给出不同的u值，我们就可以得到这根线段上的所有点。            */
/* 例如：                                                               */
/*       当u = 0时，x = x1, y = y1 也就是P1                             */
/*       当u = 1时，x = x2, y = y2 也就是P2                             */
/* 换句话说，u越大越接近P2，u越小越接近P1。                             */
/* 显而易见的是，我们只要给出一个合适的u值，                            */
/* 我们就可以得到一个对应的裁剪点。                                     */
/* 因此上，我们把这个直线裁剪问题转化为求两个u值。这里定义为u1和u2。    */
/************************************************************************/
/* 第二步，我们需要待裁剪直线看作是一个有方向的线。                     */
/* 也就是从靠近原点指向远离原点的一根有方向的线。                       */
/* 根据这个前提，我们将四个窗口的交边分别定义成两类边：入边和出边。     */
/*     入边：指从裁剪窗口之外进入到裁剪窗口方向的边                     */
/*     出边：指从裁剪窗口之内延伸到窗口之外的边                         */
/* 另外就是，除非待裁剪线段平行于边界线，                               */
/* 否则待裁剪线段的延长线和裁剪窗口必定会有四个交点，                   */
/* 要么相交于裁剪窗口，要么相交于裁剪窗口的延长线。                     */
/************************************************************************/
/* 第三步，我们只要画一下，就能看明白下面的公式：                       */
/*     u1 = max(c1, c2, P1) u1是两个入边和P1对应u值的最小值  - 公式(3)  */
/*     u2 = min(c3, P2, c4) u2是两个出边和P2对应u值的最大值  - 公式(4)  */
/* 这个只要画一下就不难理解。既然是待裁剪直线是一个有方向的线。         */
/* 假设P1和P2的距离足够远，随着u的增大，我们从P1运动到P2的过程中。      */
/* 在进入到裁剪窗口之前，                                               */
/* 我们一定是先接触到裁剪窗口的延长线，之后接触到裁剪窗口。             */
/* 因此上，对于入边来说，有效的一定是后出现的值。                       */
/* 也就是两个入边的max值。                                              */
/* 当然如果P1就在裁剪窗口里面。我们自然选择P1自己。                     */
/* 而P1自己也恰好是两个入边后面的值。这就是max(c1, c2, P1)的含义。      */
/* 同理可知，在离开裁剪窗口之前，                                       */
/* 我们一定是先接触到裁剪窗口，之后接触到裁剪窗口的延长线。             */
/* 因此上，对于入边来说，有效的一定是先出现的值。                       */
/* 也就是两个出边的min值。                                              */
/* 当然如果P2就在裁剪窗口里面。我们自然选择P2自己。                     */
/* 而P2自己也恰好是两个入边前面的值。这就是min(c3, P2, c4)的含义。      */
/* 因此上，我们把这个直线裁剪问题转化为如何求解u1和u2。                 */
/* 进一步来说，我们把这个直线裁剪问题转化为下面两个问题：               */
/*     如何算出四个交点c1, c2, c3, c4所对应的u值。                      */
/*     如何确定哪两个边是出边，哪两个边是入边。                         */
/************************************************************************/
/* 第四步，下面来看如何求c1, c2, c3, c4所对应的u值。                    */
/* 我们设裁剪窗口的上边界为m_End.y，下边界为m_Start.y；                 */
/* 左边界为m_Start.x，右边界为m_End.x。根据公式(1)和公式(2)，则有：     */
/*       m_Start.x <= x1 + u * (x2 - x1) <= m_End.x  - 公式(5)          */
/*       m_Start.y <= y1 + u * (y2 - y1) <= m_End.y  - 公式(6)          */
/* 可以看出当                                                           */
/*       x1 + u1 * (x2 - x1) = m_Start.x                                */
/*       x1 + u2 * (x2 - x1) = m_End.x                                  */
/*       y1 + u3 * (y2 - y1) = m_Start.y                                */
/*       y1 + u4 * (y2 - y1) = m_End.y                                  */
/* 时，为裁剪直线和四个边界的交点值，进而算出四个对应的u值。            */
/* 计算公式如下：                                                       */
/*       u1 = (x1 - m_Start.x)/(x2 - x1)          - 公式(7)             */
/*       u2 = (m_End.x - x1)/(x2 - x1)            - 公式(8)             */
/*       u3 = (y1 - m_Start.y)/(y2 - y1)          - 公式(9)             */
/*       u4 = (m_End.y - y1)/(y2 - y1)            - 公式(10)            */
/************************************************************************/
/* 第四步，我们得到了计算公式，我们还需要确定算出来的u是入边还是出边。  */
/* 我们根据公式(5)和公式(6)，我们进一步推导有：                         */
/*      u * (x1 - x2) <= x1 - m_Start.x            - 公式(11)           */
/*      u * (x2 - x1) <= m_End.x - x1              - 公式(12)           */
/*      u * (y1 - y2) <= y1 - m_Start.y            - 公式(13)           */
/*      u * (y2 - y1) <= m_End.y - y1              - 公式(14)           */
/* 对于(x1 - x2), (x2 - x1), (y1 - y2), (y2 - y1)这四个值，我们称作Pk。 */
/* 如果Pk小于零时，也就是公式(11)和公式(13)。恰好是处理m_Start的xy坐标。*/
/* 此时，线段从裁剪边界延长线的内部延伸到外部。也就是入边。             */
/* 如果Pk大于零时，也就是公式(12)和公式(14)。恰好是处理m_End的xy坐标。  */
/* 此时，线段从裁剪边界延长线的内部延伸到外部。也就是出边。             */
/*                                                                      */
/* 对于(x1 - m_Start.x), (m_End.x - x1), (y1 - m_Start.y),              */
/*     (m_End.y - y1)这四个值，我们称作Qk。                             */
/* 因此上，上面计算u的4个公式就变成了下面的通用公式：                   */
/*     u = Pk / Qk                                  - 公式(15)          */
/************************************************************************/
/* 明白了上面的步骤，我们就明白了下面的代码。                           */
/* 首先我们根据使用公式(7-10)进行计算。假设P1的XY坐标都比P0的坐标大。   */
/* 那就意味着dx, dy都是正数。                                           */
/* 因此上，使用公式(7)和公式(9)的时候，传入的pk为负。                   */
/* 在calcSegmentRatio会计算u，并且设置起点对应的裁剪位置比例值tmax。    */
/* 在这两次计算中，根据公式(3)，我们得到了两次计算的最大值。            */
/* 这就是裁剪后的起点对应的裁剪位置比例值。                             */
/* 同样的是，使用公式(8)和公式(10)的时候，传入的pk为正。                */
/* 在calcSegmentRatio会计算u，并且设置终点对应的裁剪位置比例值tmin。    */
/* 在这两次计算中，根据公式(4)，我们得到了两次计算的最小值。            */
/* 这就是裁剪后的终点对应的裁剪位置比例值。                             */
/* 在最后，我们得到了两个比例值，我们就可以用tmax计算新的起点。         */
/* 用tmin计算新的终点。                                                 */
/************************************************************************/
BOOL CLiangBarskyClipLine::LBLineClip(void)
{
	double tmax;          // 裁剪后的起点对应的裁剪位置比例值。
	tmax = 0.0;           // 该值默认为0。也就是位于线段起始位置P0。
                          
	double tmin;          // 裁剪后的终点对应的裁剪位置比例值。
	tmin = 1.0;           // 该值默认为1。也就是位于线段结束位置P1。
                          
	double dx, dy;	      // 计算起始位置和结束位置在XY方向上的距离。
	dx = P1.x - P0.x;     // 因为，后续会反复使用到这个值。
	dy = P1.y - P0.y;     // 预先计算出来比较方便。

	// 下面开始Liang-Barsky裁剪算法。
	// 使用窗口边界，按照左、右、下、上顺序裁剪直线。
	// 这里的顺序只不过是采用了前面公式的顺序而已。我认为这四步可以随意组合。
	// 使用公式(7)进行计算。n＝1,左边界u1＝－△x，v1＝x0－wxl
	if (calcSegmentRatio(-dx, P0.x - m_Start.x, tmax, tmin))
	{
	    // 使用公式(8)进行计算。n＝2，右边界u2＝△x，v2＝wxr－x0
		if (calcSegmentRatio(dx, m_End.x - P0.x, tmax, tmin))
		{
			// 使用公式(9)进行计算。n＝3，下边界u3＝－△y，v3＝y0-wyb
			if (calcSegmentRatio(-dy, P0.y - m_Start.y, tmax, tmin))
			{
				// 使用公式(10)进行计算。n＝4，上边界u4＝△y，v4＝wyt-y0
				if (calcSegmentRatio(dy, m_End.y - P0.y, tmax, tmin))
				{
					// 代码执行到这里，我们生成了我们需要的裁剪位置比例值。
					// 用tmin计算新的终点。 
					if (tmin < 1) // 判断直线的终点
					{
						P1.x = P0.x + tmin * dx; // 重新计算直线终点坐标
						P1.y = P0.y + tmin * dy; // x＝x0＋t(x1－x0)格式
					}
					// 用tmax计算新的起点。
					if (tmax > 0) // 判断直线的起点
					{
						P0.x = P0.x + tmax * dx; // 重新计算直线起点坐标
						P0.y = P0.y + tmax * dy; // x＝x0＋t(x1－x0)格式
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/************************************************************************/
/* 这个函数的功能其实就是计算c1, c2, c3, c4对应的4个u的值。             */
/* 也就是裁剪位置在线段上的比例值。                                     */
/* 该函数会被调用四次。按照左、右、下、上顺序                           */
/* 给出裁剪位置在线段上的比例值。                                       */
/* 利用tmax保存起点对应的裁剪位置比例值。                               */
/* 利用tmin保存终点对应的裁剪位置比例值。                               */
/************************************************************************/
BOOL CLiangBarskyClipLine::calcSegmentRatio(
		double p, double q, double &tmax, double &tmin)
{
	double t;
	BOOL ReturnValue = TRUE;
	// 如果Pk小于零时，线段从裁剪边界延长线的内部延伸到外部。也就是入边。
	// 此时直线段从窗口边界的不可见侧到可见侧,计算起点处的tmax
	if (p < 0)
	{
		// 计算U值。
		t = q / p;
		// 如果，计算结果大于tmin，说明裁剪位置出现交错。报错返回。
		if (t > tmin)
			ReturnValue = FALSE;
		// 如果计算结果小于tmin，大于tmax。说明需要裁剪。更新tmax的值。
		else if (t > tmax)
			tmax = t;
		// 如果计算结果小于tmax，说明线段和裁剪边界或者是裁剪边界延长线没有相交。
		// 此时，本次不需要进行裁剪。什么都不用做。
	}
	// 如果Pk大于零时，线段从裁剪边界延长线的内部延伸到外部。也就是出边。
	// 此时，直线段从窗口边界的可见侧到不可见侧，计算终点处的tmin
	else if (p > 0)
	{
		// 计算U值。
		t = q / p;
		// 如果，计算结果小于tmax，说明裁剪位置出现交错。报错返回。
		if (t < tmax)
			ReturnValue = FALSE;
		// 如果计算结果小于tmin，大于tmax。说明需要裁剪。更新tmin的值。
		else if (t < tmin)
			tmin = t;
		// 如果计算结果大于tmin，说明线段和裁剪边界或者是裁剪边界延长线没有相交。
		// 此时，本次不需要进行裁剪。什么都不用做。
	}
	// 如果Pk等于零时，说明是平行于窗口边界的直线。
	else 
	{
		// 直线段在窗口外，可直接删除
		if (q < 0)
		{
			ReturnValue = FALSE;
		}
	}
	return ReturnValue;
}

/************************************************************************/
/* 功能：绘制函数。绘制了一个椭圆和上面的文字。                         */
/************************************************************************/
#define ClipLine_DIFF     40
void CLiangBarskyClipLine::Draw( CDC *pdc, BOOL bShowSelectBorder )
{
	AdjustFocusPoint();

	CPen p, *pOldPen;     
	if(m_IsMark)
	{
        p.CreatePen(PS_SOLID,1,RGB(255,0,0));     //初始化画笔（红色） 
        pOldPen=pdc-> SelectObject(&p);     //把画笔选入DC，并保存原来画笔
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
/* 功能：选中绘制函数。绘制了连接点。                                   */
/************************************************************************/
void CLiangBarskyClipLine::DrawFocus( CDC *pdc )
{
	// 画笔为虚线，线宽为1，颜色为黑色。
	CPen pen( PS_DOT, 1, RGB(0, 0, 0) );
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CPen* oldpen = pdc->SelectObject(&pen);
	CBrush* oldbrush = pdc->SelectObject( pBrush );
	// 画一个虚线框。
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
/* 功能： 移动处理函数。                                                */
/************************************************************************/
void CLiangBarskyClipLine::Move( int cx, int cy )
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* 功能： 大小调整处理函数。                                            */
/*        根据IsOn函数计算结果得到准备进行大小调整的连接点，进行调整。  */
/************************************************************************/
void CLiangBarskyClipLine::AdjustSize( CPoint &pt )
{
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
}

/************************************************************************/
/* 功能：判断是否在图元区域内。                                         */
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
/* 功能： 判断一个连接点是否在图元边界上。用于调整图元是否连接。        */
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
/* 功能： 判断一个屏幕坐标是否在图元边界上。用于调整图元大小。          */
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
/* 功能：在调整大小发生翻转的时候，根据调整结果交换起始点和结束点坐标。 */
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
/* 功能：根据起始点和结束点坐标调整用于大小调整和连线的连接点坐标。     */
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
/* 功能：串行化操作。                                                   */
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