#include "stdafx.h"
#include "BucketTableFill.h"

#define ROUND(d) int(d + 0.5)//四舍五入宏定义


CBucketTableFill::CBucketTableFill(void)
{
	nPointNumber = 0;
	P = NULL;
	pEdge = NULL;
	pHeadB = NULL;
	pHeadE = NULL;
	pCurrentE = NULL;
}

CBucketTableFill::~CBucketTableFill(void)
{
	if(NULL != P)
	{
		delete[]P;
		P = NULL;
	}
	ClearMemory();
}

void CBucketTableFill::SetPoint(CColorPoint* p, int n)
{
	P = new CColorPoint[n];//创建一维动态数组
	nPointNumber = n;
	for(int i = 0;i < nPointNumber;i++)
		P[i] = p[i];
}

void CBucketTableFill::CreateBucketTable(void)//创建桶表
{
	int yMin, yMax;//ymin最小扫描线，ymax最大扫描线
	yMin = yMax = P[0].y;
	for(int i = 0;i < nPointNumber; i++)//查找多边形所覆盖的最小和最大扫描线
	{
		if(P[i].y < yMin)
			yMin = P[i].y;//扫描线的最小值
		if(P[i].y > yMax)
			yMax = P[i].y;//扫描线的最大值		
	}
	for(int y = yMin;y <= yMax;y++)
	{
		if(yMin == y)//如果是扫描线的最小值
		{
			pHeadB = new CBucketTable;//建立桶的头结点
			pCurrentB = pHeadB;//pCurrentB为CBT当前结点指针
			pCurrentB->ScanLine = yMin;
			pCurrentB->pET = NULL;//没有链接边表
			pCurrentB->pNext = NULL;
		}
		else//其他扫描线
		{
			pCurrentB->pNext = new CBucketTable;//建立桶的其他结点
			pCurrentB = pCurrentB->pNext;
			pCurrentB->ScanLine = y;
			pCurrentB->pET = NULL;
			pCurrentB->pNext = NULL;
		}
	}
}

void CBucketTableFill::CreateEdgeTable(void)//创建边表
{
	for(int i = 0;i < nPointNumber;i++)
	{
		pCurrentB = pHeadB;
		int j = (i + 1) % nPointNumber;//边的另一个顶点，P[i]和P[j]点对构成边
		if(P[i].y < P[j].y)//边的终点比起点高
		{
			pEdge = new CActiveEdgeTable;
			pEdge->x = P[i].x;//计算ET表的值
			pEdge->yMax = P[j].y;
			pEdge->m = (double)(P[j].x - P[i].x) / (P[j].y - P[i].y);//代表1/k
			pEdge->pStart = P[i];//绑定顶点和颜色
			pEdge->pEnd = P[j];
			pEdge->pNext = NULL;
			while(pCurrentB->ScanLine != P[i].y)//在桶内寻找当前边的ymin
				pCurrentB = pCurrentB->pNext;//移到yMin所在的桶结点
		}
		if(P[j].y < P[i].y)//边的终点比起点低
		{
			pEdge = new CActiveEdgeTable;
			pEdge->x = P[j].x;
			pEdge->yMax = P[i].y;
			pEdge->m = (double)(P[i].x - P[j].x) / (P[i].y - P[j].y);
			pEdge->pStart = P[i];
			pEdge->pEnd = P[j];
			pEdge->pNext = NULL;
			while(pCurrentB->ScanLine != P[j].y)
				pCurrentB = pCurrentB->pNext;
		}
		if(P[i].y != P[j].y)
		{
			pCurrentE = pCurrentB->pET;
			if(NULL == pCurrentE )
			{
				pCurrentE = pEdge;
				pCurrentB->pET = pCurrentE;
			}
			else
			{
				while(NULL != pCurrentE->pNext)
					pCurrentE = pCurrentE->pNext;
				pCurrentE->pNext = pEdge;
			}
		}
	}
}

void CBucketTableFill::FillPolygon(CDC* pDC)//填充多边形
{
	CActiveEdgeTable* pT1 = NULL, *pT2 = NULL;
	pHeadE = NULL;	
	for(pCurrentB = pHeadB;pCurrentB != NULL;pCurrentB = pCurrentB->pNext)
	{
		for(pCurrentE = pCurrentB->pET;pCurrentE != NULL;pCurrentE = pCurrentE->pNext)
		{
			pEdge = new CActiveEdgeTable;
			pEdge->x = pCurrentE->x;
			pEdge->yMax = pCurrentE->yMax;
			pEdge->m = pCurrentE->m;
			pEdge->pStart = pCurrentE->pStart;
			pEdge->pEnd = pCurrentE->pEnd;
			pEdge->pNext = NULL;
			AddEdgeTable(pEdge);
		}
		SortEdgeTable();	
		pT1 = pHeadE;
		if(NULL == pT1)
			return;
		while(pCurrentB->ScanLine >= pT1->yMax)//下闭上开
		{
			CActiveEdgeTable * pAETTemp = pT1;		
			pT1 = pT1->pNext;
			delete pAETTemp;
			pHeadE = pT1;
			if(NULL == pHeadE)
				return;
		}
		if(NULL != pT1->pNext)
		{
			pT2 = pT1;
			pT1 = pT2->pNext;
		}
		while(NULL != pT1)
		{
			if(pCurrentB->ScanLine >= pT1->yMax)//下闭上开
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
		BOOL bInside = FALSE;//内外测试标志，初始值为假表示位于跨度外部
		int xLeft, xRight;//跨度的起点和终点坐标
		for(pT1 = pHeadE;pT1 != NULL;pT1 = pT1->pNext)
		{
			if(FALSE == bInside)
			{
				xLeft = ROUND(pT1->x);
				bInside = TRUE;
			}
			else
			{
				xRight = ROUND(pT1->x);
				for(int x = xLeft;x < xRight;x++)//左闭右开
					pDC->SetPixelV(x, pCurrentB->ScanLine, RGB(pT1->pStart.c.red * 255, pT1->pStart.c.green * 255, pT1->pStart.c.blue * 255));
				bInside = FALSE;
			}
		}
		for(pT1 = pHeadE;pT1 != NULL;pT1 = pT1->pNext)//边的连续性
			pT1->x = pT1->x + pT1->m;
	}
}

void CBucketTableFill::AddEdgeTable(CActiveEdgeTable* pNewEdge)//合并ET表
{
	CActiveEdgeTable* pCurrentEdge = pHeadE;
	if(NULL == pCurrentEdge)
	{
		pHeadE = pNewEdge;
		pCurrentEdge = pHeadE;
	}
	else
	{
		while(NULL != pCurrentEdge->pNext)
			pCurrentEdge = pCurrentEdge->pNext;
		pCurrentEdge->pNext = pNewEdge;
	}
}

void CBucketTableFill::SortEdgeTable(void)//边表的冒泡排序算法
{
	CActiveEdgeTable* pT1, *pT2;
	int Count = 1;
	pT1 = pHeadE;
	if(NULL == pT1)
		return;
	if(NULL == pT1->pNext)//如果该ET表没有再连ET表
	{
		return;//桶结点只有一条边，不需要排序
	}
	while(NULL != pT1->pNext)//统计边结点的个数
	{
		Count++;
		pT1 = pT1->pNext;
	}
	for(int i = 0;i < Count - 1;i++)//冒泡排序
	{
		CActiveEdgeTable** pPre = &pHeadE;
		pT1 = pHeadE;
		for (int j = 0;j < Count - 1 - i;j++)
		{
			pT2 = pT1->pNext;
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

void CBucketTableFill::ClearMemory(void)//安全删除所有桶与桶上连接的边
{
	DeleteEdgeTableChain(pHeadE);
	CBucketTable* pBucket = pHeadB;
	while (NULL != pBucket)//针对每一个桶
	{
		CBucketTable *pBucketTemp=pBucket->pNext;
		DeleteEdgeTableChain(pBucket->pET);
		delete pBucket;
		pBucket = pBucketTemp;
	}
	pHeadB = NULL;
	pHeadE = NULL;
}

void CBucketTableFill::DeleteEdgeTableChain(CActiveEdgeTable* pAET)
{
	while(pAET!=NULL)
	{
		CActiveEdgeTable* pAETTemp = pAET->pNext;
		delete pAET;
		pAET = pAETTemp;
	}
}
