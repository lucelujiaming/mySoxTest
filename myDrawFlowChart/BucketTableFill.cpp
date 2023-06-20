#include "stdafx.h"
#include "BucketTableFill.h"

#define ROUND(d) int(d + 0.5)//��������궨��


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
	P = new CColorPoint[n];//����һά��̬����
	nPointNumber = n;
	for(int i = 0;i < nPointNumber;i++)
		P[i] = p[i];
}

void CBucketTableFill::CreateBucketTable(void)//����Ͱ��
{
	int yMin, yMax;//ymin��Сɨ���ߣ�ymax���ɨ����
	yMin = yMax = P[0].y;
	for(int i = 0;i < nPointNumber; i++)//���Ҷ���������ǵ���С�����ɨ����
	{
		if(P[i].y < yMin)
			yMin = P[i].y;//ɨ���ߵ���Сֵ
		if(P[i].y > yMax)
			yMax = P[i].y;//ɨ���ߵ����ֵ		
	}
	for(int y = yMin;y <= yMax;y++)
	{
		if(yMin == y)//�����ɨ���ߵ���Сֵ
		{
			pHeadB = new CBucketTable;//����Ͱ��ͷ���
			pCurrentB = pHeadB;//pCurrentBΪCBT��ǰ���ָ��
			pCurrentB->ScanLine = yMin;
			pCurrentB->pET = NULL;//û�����ӱ߱�
			pCurrentB->pNext = NULL;
		}
		else//����ɨ����
		{
			pCurrentB->pNext = new CBucketTable;//����Ͱ���������
			pCurrentB = pCurrentB->pNext;
			pCurrentB->ScanLine = y;
			pCurrentB->pET = NULL;
			pCurrentB->pNext = NULL;
		}
	}
}

void CBucketTableFill::CreateEdgeTable(void)//�����߱�
{
	for(int i = 0;i < nPointNumber;i++)
	{
		pCurrentB = pHeadB;
		int j = (i + 1) % nPointNumber;//�ߵ���һ�����㣬P[i]��P[j]��Թ��ɱ�
		if(P[i].y < P[j].y)//�ߵ��յ������
		{
			pEdge = new CActiveEdgeTable;
			pEdge->x = P[i].x;//����ET���ֵ
			pEdge->yMax = P[j].y;
			pEdge->m = (double)(P[j].x - P[i].x) / (P[j].y - P[i].y);//����1/k
			pEdge->pStart = P[i];//�󶨶������ɫ
			pEdge->pEnd = P[j];
			pEdge->pNext = NULL;
			while(pCurrentB->ScanLine != P[i].y)//��Ͱ��Ѱ�ҵ�ǰ�ߵ�ymin
				pCurrentB = pCurrentB->pNext;//�Ƶ�yMin���ڵ�Ͱ���
		}
		if(P[j].y < P[i].y)//�ߵ��յ������
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

void CBucketTableFill::FillPolygon(CDC* pDC)//�������
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
		while(pCurrentB->ScanLine >= pT1->yMax)//�±��Ͽ�
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
			if(pCurrentB->ScanLine >= pT1->yMax)//�±��Ͽ�
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
		BOOL bInside = FALSE;//������Ա�־����ʼֵΪ�ٱ�ʾλ�ڿ���ⲿ
		int xLeft, xRight;//��ȵ������յ�����
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
				for(int x = xLeft;x < xRight;x++)//����ҿ�
					pDC->SetPixelV(x, pCurrentB->ScanLine, RGB(pT1->pStart.c.red * 255, pT1->pStart.c.green * 255, pT1->pStart.c.blue * 255));
				bInside = FALSE;
			}
		}
		for(pT1 = pHeadE;pT1 != NULL;pT1 = pT1->pNext)//�ߵ�������
			pT1->x = pT1->x + pT1->m;
	}
}

void CBucketTableFill::AddEdgeTable(CActiveEdgeTable* pNewEdge)//�ϲ�ET��
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

void CBucketTableFill::SortEdgeTable(void)//�߱��ð�������㷨
{
	CActiveEdgeTable* pT1, *pT2;
	int Count = 1;
	pT1 = pHeadE;
	if(NULL == pT1)
		return;
	if(NULL == pT1->pNext)//�����ET��û������ET��
	{
		return;//Ͱ���ֻ��һ���ߣ�����Ҫ����
	}
	while(NULL != pT1->pNext)//ͳ�Ʊ߽��ĸ���
	{
		Count++;
		pT1 = pT1->pNext;
	}
	for(int i = 0;i < Count - 1;i++)//ð������
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
				pPre = &(pT2->pNext);//����λ��Ϊ�´α�����׼��
			}
			else
			{
				pPre = &(pT1->pNext);
				pT1 = pT1->pNext;
			}
		}
	}
}

void CBucketTableFill::ClearMemory(void)//��ȫɾ������Ͱ��Ͱ�����ӵı�
{
	DeleteEdgeTableChain(pHeadE);
	CBucketTable* pBucket = pHeadB;
	while (NULL != pBucket)//���ÿһ��Ͱ
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
