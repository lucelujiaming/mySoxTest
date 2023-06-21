#include "stdafx.h"
#include "BucketTableFill.h"

#define ROUND(d) int(d + 0.5)//四舍五入宏定义


CBucketTableFill::CBucketTableFill(void)
{
	m_nPointNumber = 0;
	m_PolygonPoints = NULL;
	m_pHeadBucketTable = NULL;
	m_pHeadEdge = NULL;
}

CBucketTableFill::~CBucketTableFill(void)
{
	if(NULL != m_PolygonPoints)
	{
		delete[]m_PolygonPoints;
		m_PolygonPoints = NULL;
	}
	ClearMemory();
}

/************************************************************************/
/* 1. 初始化多边形顶点                                                  */
/* 注意：这里给出的多边形顶点列表要求是排好序的。                       */
/*       也就是可以一个一个从头到尾连成一个多边形的。                   */
/************************************************************************/
void CBucketTableFill::SetPoint(CColorPoint* p, int n)
{
	if (n <= 0)
	{
		return;
	}
	m_PolygonPoints = new CColorPoint[n];//创建一维动态数组
	m_nPointNumber = n;
	for(int i = 0;i < m_nPointNumber;i++)
		m_PolygonPoints[i] = p[i];
}

/************************************************************************/
/* 2. 创建桶表                                                          */
/************************************************************************/
void CBucketTableFill::CreateBucketTable(void)
{
	CBucketTable*      pCurrentBucketTable;
	if (m_nPointNumber <= 0)
	{
		return;
	}
	// ymin最小扫描线，ymax最大扫描线
	int yMin, yMax;
	// 首先假设都是m_PolygonPoints[0]。
	yMin = yMax = m_PolygonPoints[0].y;
	// 查找多边形所覆盖的最小和最大扫描线
	for(int i = 0;i < m_nPointNumber; i++)
	{
		if(m_PolygonPoints[i].y < yMin)
			yMin = m_PolygonPoints[i].y; // 扫描线的最小值
		if(m_PolygonPoints[i].y > yMax)
			yMax = m_PolygonPoints[i].y; // 扫描线的最大值		
	}
	// 根据最小和最大扫描线
	for(int y = yMin; y <= yMax; y++)
	{
		if(yMin == y) // 如果是扫描线的最小值
		{
			// 建立一个桶结点。对于最小扫描线来说，是头结点。
			m_pHeadBucketTable = new CBucketTable;
			// 桶表当前结点指针m_pCurrentBucketTable指向刚刚创建的头结点。
			pCurrentBucketTable = m_pHeadBucketTable;
			// 设定扫描线索引为yMin
			pCurrentBucketTable->ScanLine = yMin;
			// 链接边表为空。后续在CreateEdgeTable中填充。
			pCurrentBucketTable->m_pEdgeTable = NULL;   
			pCurrentBucketTable->pNext = NULL; // 
		}
		else // 如果是其他扫描线
		{
			// 建立一个桶结点
			pCurrentBucketTable->pNext = new CBucketTable;
			// CBT当前结点指针m_pCurrentBucketTable指向刚刚创建的结点。
			pCurrentBucketTable = pCurrentBucketTable->pNext;
			// 设定扫描线索引
			pCurrentBucketTable->ScanLine = y;
			// 链接边表为空。后续在CreateEdgeTable中填充。
			pCurrentBucketTable->m_pEdgeTable = NULL;
			pCurrentBucketTable->pNext = NULL;
		}
	}
}

// 3. 创建边表
void CBucketTableFill::CreateEdgeTable(void)
{
	CBucketTable*      pCurrentBucketTable;
	CActiveEdgeTable*  pEdge;		
	// 一条一条处理多边形的每一个边。
	for(int i = 0;i < m_nPointNumber;i++)
	{
		pCurrentBucketTable = m_pHeadBucketTable;
		// 当前边的下一个顶点，因为我们在SetPoint中传入的店是排好序的。
		// 也就是可以一个一个从头到尾连成一个多边形的。
		// 因此上，P[i]和P[j]点对必然构成一个边。
		int j = (i + 1) % m_nPointNumber; 
		// 如果边的终点比起点高
		if(m_PolygonPoints[i].y < m_PolygonPoints[j].y) 
		{
			// 建立一个活动边结点
			pEdge = new CActiveEdgeTable;
			pEdge->x = m_PolygonPoints[i].x; // 计算活动边表的值
			// 记下边的最大y值。用于在到达这个值的时候，把自己删掉。
			pEdge->yMax = m_PolygonPoints[j].y;
			// 记下这条边斜率的倒数。便于后续运算。
			pEdge->m = (double)(m_PolygonPoints[j].x - m_PolygonPoints[i].x) 
				               / (m_PolygonPoints[j].y - m_PolygonPoints[i].y); // 代表1/k
			// 设定起始点和颜色
			pEdge->pStart = m_PolygonPoints[i];
			// 设定结束点和颜色
			pEdge->pEnd = m_PolygonPoints[j];
			pEdge->pNext = NULL;
			// 在桶内寻找当前边的ymin
			// 也就是找到这条边的低点对应的那一个桶表对象。
			// 从这个桶表对象开始，后面所有的桶表对象都拥有这条边。
			while(pCurrentBucketTable->ScanLine != m_PolygonPoints[i].y)
			{
				// 移到yMin所在的桶结点
				pCurrentBucketTable = pCurrentBucketTable->pNext;
			}
		}
		// 如果边的终点比起点低
		else if(m_PolygonPoints[j].y < m_PolygonPoints[i].y)
		{
			// 建立一个活动边结点
			pEdge = new CActiveEdgeTable;
			pEdge->x = m_PolygonPoints[j].x;
			// 记下边的最大y值。用于在到达这个值的时候，把自己删掉。
			pEdge->yMax = m_PolygonPoints[i].y;
			// 记下这条边斜率的倒数。便于后续运算。
			pEdge->m = (double)(m_PolygonPoints[i].x - m_PolygonPoints[j].x) 
				               / (m_PolygonPoints[i].y - m_PolygonPoints[j].y);
			// 设定起始点和颜色
			pEdge->pStart = m_PolygonPoints[i];
			// 设定结束点和颜色
			pEdge->pEnd = m_PolygonPoints[j];
			pEdge->pNext = NULL;
			// 找到这条边的低点对应的那一个桶表对象。
			// 从这个桶表对象开始，后面所有的桶表对象都拥有这条边。
			while(pCurrentBucketTable->ScanLine != m_PolygonPoints[j].y)
			{
				pCurrentBucketTable = pCurrentBucketTable->pNext;
			}
		}
		// 我们不需要处理终点和起点相等的情况。因为这种情况根本不需要填充。
		// 如果边的终点和起点不相等。
		if(m_PolygonPoints[i].y != m_PolygonPoints[j].y)
		{
			CActiveEdgeTable*  pCurrentEdge; 
			// m_pCurrentE指向上面找到的当前边的低点对应的那一个桶表对象。
			pCurrentEdge = pCurrentBucketTable->m_pEdgeTable;
			// 如果当前桶表对象没有活动边表
			if(NULL == pCurrentEdge )
			{
				pCurrentEdge = pEdge;
				// 桶表对象的活动边表指针指向这个边。
				pCurrentBucketTable->m_pEdgeTable = pCurrentEdge;
			}
			// 如果当前桶表对象有活动边表
			else
			{
				// 把这个边追加到桶表对象的活动边表中。
				while(NULL != pCurrentEdge->pNext)
					pCurrentEdge = pCurrentEdge->pNext;
				pCurrentEdge->pNext = pEdge;
			}
		}
	}
}

// 4. 填充多边形
void CBucketTableFill::FillPolygon(CDC* pDC)
{
	CActiveEdgeTable*  pEdge;
	CActiveEdgeTable*  pCurrentEdge; 
	CBucketTable*      pCurrentBucketTable;
	CActiveEdgeTable* pT1 = NULL, *pT2 = NULL;
	m_pHeadEdge = NULL;
	// 遍历整个桶表。
	for(pCurrentBucketTable = m_pHeadBucketTable; 
	    pCurrentBucketTable != NULL;
		pCurrentBucketTable = pCurrentBucketTable->pNext)
	{
		// 遍历每一个桶表元素的活动边表。
		for(pCurrentEdge = pCurrentBucketTable->m_pEdgeTable;
		    pCurrentEdge != NULL;pCurrentEdge = pCurrentEdge->pNext)
		{
			pEdge = new CActiveEdgeTable;
			pEdge->x       = pCurrentEdge->x;
			pEdge->yMax    = pCurrentEdge->yMax;
			pEdge->m       = pCurrentEdge->m;
			pEdge->pStart  = pCurrentEdge->pStart;
			pEdge->pEnd    = pCurrentEdge->pEnd;
			pEdge->pNext   = NULL;
			// 当前桶表中所有元素的活动边表加入到全局活动边表中。
			AddEdgeTable(pEdge);
		}
		// 对所有的活动边进行排序。按照x坐标从小到大排序。
		SortEdgeTable();
		// 取出排序后的活动边表。
		pT1 = m_pHeadEdge;
		if(NULL == pT1)
			return;
		// 删除位于当前桶表元素对应的扫描线下方的活动边。
		while(pCurrentBucketTable->ScanLine >= pT1->yMax)//下闭上开
		{
			// 从左到右查找。如果活动边位于当前桶表元素对应的扫描线上方。
			CActiveEdgeTable * pAETTemp = pT1;		
			pT1 = pT1->pNext;
			// 那就链表头删除这个元素。
			delete pAETTemp;
			// 更新链表头位置。
			m_pHeadEdge = pT1;
			if(NULL == m_pHeadEdge)
				return;
		}
		// 代码执行到这里，我们找到了一条位于当前桶表元素对应的扫描线下方的边。
		if(NULL != pT1->pNext)
		{
			pT2 = pT1;
			// 找到这条边的下一个边。
			pT1 = pT2->pNext;
		}
		// 从这条边开始，删除位于当前桶表元素对应的扫描线下方的活动边。
		while(NULL != pT1)
		{
			if(pCurrentBucketTable->ScanLine >= pT1->yMax)//下闭上开
			{
				CActiveEdgeTable *pAETTemp = pT1;
				pT2->pNext = pT1->pNext;
				pT1 = pT2->pNext;
				delete pAETTemp;
			}
			else
			{
				pT2 = pT1;
				pT1 = pT2->pNext;
			}
		}
		// 代码执行到这里，我们删除了所有的无关的边。
		// 下面就可以根据活动边信息进行填充了。

		// 内外测试标志，初始值为假，表示位于跨度外部。
		BOOL bInside = FALSE;
		// 跨度的起点和终点坐标
		int xLeft, xRight;
		for(pT1 = m_pHeadEdge;pT1 != NULL;pT1 = pT1->pNext)
		{
			// 如果在外部，遇到一条边以后，变成内部。
			if(FALSE == bInside)
			{
				// 记录下跨度的起点坐标
				xLeft = ROUND(pT1->x);
				// 变成内部。
				bInside = TRUE;
			}
			// 如果在内部，遇到一条边以后，变成外部。
			else
			{
				// 记录下跨度的终点坐标
				xRight = ROUND(pT1->x);
				// 开始绘制
				for(int x = xLeft;x < xRight;x++)//左闭右开
				{
					pDC->SetPixelV(x, pCurrentBucketTable->ScanLine, 
							RGB(pT1->pStart.c.red * 255, 
							    pT1->pStart.c.green * 255, 
								pT1->pStart.c.blue * 255));
				}
				// 变成外部。
				bInside = FALSE;
			}
		}
		for(pT1 = m_pHeadEdge;pT1 != NULL;pT1 = pT1->pNext)//边的连续性
			pT1->x = pT1->x + pT1->m;
	}
}

void CBucketTableFill::AddEdgeTable(CActiveEdgeTable* pNewEdge)//合并活动边表
{
	CActiveEdgeTable* pCurrentEdge = m_pHeadEdge;
	// 如果全局活动边表为空。
	if(NULL == pCurrentEdge)
	{
		m_pHeadEdge = pNewEdge;
		// 把当前活动边表加入全局活动边表。
		pCurrentEdge = m_pHeadEdge;
	}
	// 如果全局活动边表不为空
	else
	{
		// 把当前活动边表追加到全局活动边表中。
		while(NULL != pCurrentEdge->pNext)
			pCurrentEdge = pCurrentEdge->pNext;
		pCurrentEdge->pNext = pNewEdge;
	}
}

// 边表的冒泡排序算法
void CBucketTableFill::SortEdgeTable(void)
{
	CActiveEdgeTable* pT1, *pT2;
	int Count = 1;
	pT1 = m_pHeadEdge;
	if(NULL == pT1)
		return;
	// 如果该活动边没有下一个活动边
	if(NULL == pT1->pNext) 
	{
		// 桶结点只有一条边，不需要排序
		return; 
	}
	// 统计边结点的个数
	while(NULL != pT1->pNext)
	{
		Count++;
		pT1 = pT1->pNext;
	}
	// 循环每一个元素。
	for(int i = 0;i < Count - 1;i++) // 冒泡排序
	{
		CActiveEdgeTable** pPre = &m_pHeadEdge;
		pT1 = m_pHeadEdge;
		// 循环pT1后面的Count - 1 - i个元素。
		for (int j = 0;j < Count - 1 - i;j++)
		{
			pT2 = pT1->pNext;
			// 如果pT1在pT2右边，或者是两者的x坐标相同，但是pT1的斜率更大。
			if ((pT1->x > pT2->x) || ((pT1->x == pT2->x) && (pT1->m > pT2->m)))
			{
				pT1->pNext = pT2->pNext;
				pT2->pNext = pT1;
				*pPre = pT2;
				pPre = &(pT2->pNext);//调整位置为下次遍历做准备
			}
			else
			{
				pPre = &(pT1->pNext);
				pT1 = pT1->pNext;
			}
		}
	}
}

// 安全删除所有桶与桶上连接的边
void CBucketTableFill::ClearMemory(void)
{
	// 删除全局活动边表。
	DeleteEdgeTableChain(m_pHeadEdge);
	CBucketTable* pBucket = m_pHeadBucketTable;
	while (NULL != pBucket)//针对每一个桶
	{
		CBucketTable *pBucketTemp=pBucket->pNext;
		// 删除一个桶表元素的活动边表。
		DeleteEdgeTableChain(pBucket->m_pEdgeTable);
		// 删除这个桶表元素。
		delete pBucket;
		pBucket = pBucketTemp;
	}
	m_pHeadBucketTable = NULL;
	m_pHeadEdge = NULL;
}

// 删除一个活动边元素
void CBucketTableFill::DeleteEdgeTableChain(CActiveEdgeTable* pActiveEdgeTable)
{
	while(pActiveEdgeTable!=NULL)
	{
		CActiveEdgeTable* pActiveEdgeTableTemp = pActiveEdgeTable->pNext;
		delete pActiveEdgeTable;
		pActiveEdgeTable = pActiveEdgeTableTemp;
	}
}
