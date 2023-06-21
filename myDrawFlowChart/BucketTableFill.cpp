#include "stdafx.h"
#include "BucketTableFill.h"

#define ROUND(d) int(d + 0.5)//��������궨��


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
/* 1. ��ʼ������ζ���                                                  */
/* ע�⣺��������Ķ���ζ����б�Ҫ�����ź���ġ�                       */
/*       Ҳ���ǿ���һ��һ����ͷ��β����һ������εġ�                   */
/************************************************************************/
void CBucketTableFill::SetPoint(CColorPoint* p, int n)
{
	if (n <= 0)
	{
		return;
	}
	m_PolygonPoints = new CColorPoint[n];//����һά��̬����
	m_nPointNumber = n;
	for(int i = 0;i < m_nPointNumber;i++)
		m_PolygonPoints[i] = p[i];
}

/************************************************************************/
/* 2. ����Ͱ��                                                          */
/************************************************************************/
void CBucketTableFill::CreateBucketTable(void)
{
	CBucketTable*      pCurrentBucketTable;
	if (m_nPointNumber <= 0)
	{
		return;
	}
	// ymin��Сɨ���ߣ�ymax���ɨ����
	int yMin, yMax;
	// ���ȼ��趼��m_PolygonPoints[0]��
	yMin = yMax = m_PolygonPoints[0].y;
	// ���Ҷ���������ǵ���С�����ɨ����
	for(int i = 0;i < m_nPointNumber; i++)
	{
		if(m_PolygonPoints[i].y < yMin)
			yMin = m_PolygonPoints[i].y; // ɨ���ߵ���Сֵ
		if(m_PolygonPoints[i].y > yMax)
			yMax = m_PolygonPoints[i].y; // ɨ���ߵ����ֵ		
	}
	// ������С�����ɨ����
	for(int y = yMin; y <= yMax; y++)
	{
		if(yMin == y) // �����ɨ���ߵ���Сֵ
		{
			// ����һ��Ͱ��㡣������Сɨ������˵����ͷ��㡣
			m_pHeadBucketTable = new CBucketTable;
			// Ͱ��ǰ���ָ��m_pCurrentBucketTableָ��ոմ�����ͷ��㡣
			pCurrentBucketTable = m_pHeadBucketTable;
			// �趨ɨ��������ΪyMin
			pCurrentBucketTable->ScanLine = yMin;
			// ���ӱ߱�Ϊ�ա�������CreateEdgeTable����䡣
			pCurrentBucketTable->m_pEdgeTable = NULL;   
			pCurrentBucketTable->pNext = NULL; // 
		}
		else // ���������ɨ����
		{
			// ����һ��Ͱ���
			pCurrentBucketTable->pNext = new CBucketTable;
			// CBT��ǰ���ָ��m_pCurrentBucketTableָ��ոմ����Ľ�㡣
			pCurrentBucketTable = pCurrentBucketTable->pNext;
			// �趨ɨ��������
			pCurrentBucketTable->ScanLine = y;
			// ���ӱ߱�Ϊ�ա�������CreateEdgeTable����䡣
			pCurrentBucketTable->m_pEdgeTable = NULL;
			pCurrentBucketTable->pNext = NULL;
		}
	}
}

// 3. �����߱�
void CBucketTableFill::CreateEdgeTable(void)
{
	CBucketTable*      pCurrentBucketTable;
	CActiveEdgeTable*  pEdge;		
	// һ��һ���������ε�ÿһ���ߡ�
	for(int i = 0;i < m_nPointNumber;i++)
	{
		pCurrentBucketTable = m_pHeadBucketTable;
		// ��ǰ�ߵ���һ�����㣬��Ϊ������SetPoint�д���ĵ����ź���ġ�
		// Ҳ���ǿ���һ��һ����ͷ��β����һ������εġ�
		// ����ϣ�P[i]��P[j]��Ա�Ȼ����һ���ߡ�
		int j = (i + 1) % m_nPointNumber; 
		// ����ߵ��յ������
		if(m_PolygonPoints[i].y < m_PolygonPoints[j].y) 
		{
			// ����һ����߽��
			pEdge = new CActiveEdgeTable;
			pEdge->x = m_PolygonPoints[i].x; // �����߱��ֵ
			// ���±ߵ����yֵ�������ڵ������ֵ��ʱ�򣬰��Լ�ɾ����
			pEdge->yMax = m_PolygonPoints[j].y;
			// ����������б�ʵĵ��������ں������㡣
			pEdge->m = (double)(m_PolygonPoints[j].x - m_PolygonPoints[i].x) 
				               / (m_PolygonPoints[j].y - m_PolygonPoints[i].y); // ����1/k
			// �趨��ʼ�����ɫ
			pEdge->pStart = m_PolygonPoints[i];
			// �趨���������ɫ
			pEdge->pEnd = m_PolygonPoints[j];
			pEdge->pNext = NULL;
			// ��Ͱ��Ѱ�ҵ�ǰ�ߵ�ymin
			// Ҳ�����ҵ������ߵĵ͵��Ӧ����һ��Ͱ�����
			// �����Ͱ�����ʼ���������е�Ͱ�����ӵ�������ߡ�
			while(pCurrentBucketTable->ScanLine != m_PolygonPoints[i].y)
			{
				// �Ƶ�yMin���ڵ�Ͱ���
				pCurrentBucketTable = pCurrentBucketTable->pNext;
			}
		}
		// ����ߵ��յ������
		else if(m_PolygonPoints[j].y < m_PolygonPoints[i].y)
		{
			// ����һ����߽��
			pEdge = new CActiveEdgeTable;
			pEdge->x = m_PolygonPoints[j].x;
			// ���±ߵ����yֵ�������ڵ������ֵ��ʱ�򣬰��Լ�ɾ����
			pEdge->yMax = m_PolygonPoints[i].y;
			// ����������б�ʵĵ��������ں������㡣
			pEdge->m = (double)(m_PolygonPoints[i].x - m_PolygonPoints[j].x) 
				               / (m_PolygonPoints[i].y - m_PolygonPoints[j].y);
			// �趨��ʼ�����ɫ
			pEdge->pStart = m_PolygonPoints[i];
			// �趨���������ɫ
			pEdge->pEnd = m_PolygonPoints[j];
			pEdge->pNext = NULL;
			// �ҵ������ߵĵ͵��Ӧ����һ��Ͱ�����
			// �����Ͱ�����ʼ���������е�Ͱ�����ӵ�������ߡ�
			while(pCurrentBucketTable->ScanLine != m_PolygonPoints[j].y)
			{
				pCurrentBucketTable = pCurrentBucketTable->pNext;
			}
		}
		// ���ǲ���Ҫ�����յ�������ȵ��������Ϊ���������������Ҫ��䡣
		// ����ߵ��յ����㲻��ȡ�
		if(m_PolygonPoints[i].y != m_PolygonPoints[j].y)
		{
			CActiveEdgeTable*  pCurrentEdge; 
			// m_pCurrentEָ�������ҵ��ĵ�ǰ�ߵĵ͵��Ӧ����һ��Ͱ�����
			pCurrentEdge = pCurrentBucketTable->m_pEdgeTable;
			// �����ǰͰ�����û�л�߱�
			if(NULL == pCurrentEdge )
			{
				pCurrentEdge = pEdge;
				// Ͱ�����Ļ�߱�ָ��ָ������ߡ�
				pCurrentBucketTable->m_pEdgeTable = pCurrentEdge;
			}
			// �����ǰͰ������л�߱�
			else
			{
				// �������׷�ӵ�Ͱ�����Ļ�߱��С�
				while(NULL != pCurrentEdge->pNext)
					pCurrentEdge = pCurrentEdge->pNext;
				pCurrentEdge->pNext = pEdge;
			}
		}
	}
}

// 4. �������
void CBucketTableFill::FillPolygon(CDC* pDC)
{
	CActiveEdgeTable*  pEdge;
	CActiveEdgeTable*  pCurrentEdge; 
	CBucketTable*      pCurrentBucketTable;
	CActiveEdgeTable* pT1 = NULL, *pT2 = NULL;
	m_pHeadEdge = NULL;
	// ��������Ͱ��
	for(pCurrentBucketTable = m_pHeadBucketTable; 
	    pCurrentBucketTable != NULL;
		pCurrentBucketTable = pCurrentBucketTable->pNext)
	{
		// ����ÿһ��Ͱ��Ԫ�صĻ�߱�
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
			// ��ǰͰ��������Ԫ�صĻ�߱���뵽ȫ�ֻ�߱��С�
			AddEdgeTable(pEdge);
		}
		// �����еĻ�߽������򡣰���x�����С��������
		SortEdgeTable();
		// ȡ�������Ļ�߱�
		pT1 = m_pHeadEdge;
		if(NULL == pT1)
			return;
		// ɾ��λ�ڵ�ǰͰ��Ԫ�ض�Ӧ��ɨ�����·��Ļ�ߡ�
		while(pCurrentBucketTable->ScanLine >= pT1->yMax)//�±��Ͽ�
		{
			// �����Ҳ��ҡ�������λ�ڵ�ǰͰ��Ԫ�ض�Ӧ��ɨ�����Ϸ���
			CActiveEdgeTable * pAETTemp = pT1;		
			pT1 = pT1->pNext;
			// �Ǿ�����ͷɾ�����Ԫ�ء�
			delete pAETTemp;
			// ��������ͷλ�á�
			m_pHeadEdge = pT1;
			if(NULL == m_pHeadEdge)
				return;
		}
		// ����ִ�е���������ҵ���һ��λ�ڵ�ǰͰ��Ԫ�ض�Ӧ��ɨ�����·��ıߡ�
		if(NULL != pT1->pNext)
		{
			pT2 = pT1;
			// �ҵ������ߵ���һ���ߡ�
			pT1 = pT2->pNext;
		}
		// �������߿�ʼ��ɾ��λ�ڵ�ǰͰ��Ԫ�ض�Ӧ��ɨ�����·��Ļ�ߡ�
		while(NULL != pT1)
		{
			if(pCurrentBucketTable->ScanLine >= pT1->yMax)//�±��Ͽ�
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
		// ����ִ�е��������ɾ�������е��޹صıߡ�
		// ����Ϳ��Ը��ݻ����Ϣ��������ˡ�

		// ������Ա�־����ʼֵΪ�٣���ʾλ�ڿ���ⲿ��
		BOOL bInside = FALSE;
		// ��ȵ������յ�����
		int xLeft, xRight;
		for(pT1 = m_pHeadEdge;pT1 != NULL;pT1 = pT1->pNext)
		{
			// ������ⲿ������һ�����Ժ󣬱���ڲ���
			if(FALSE == bInside)
			{
				// ��¼�¿�ȵ��������
				xLeft = ROUND(pT1->x);
				// ����ڲ���
				bInside = TRUE;
			}
			// ������ڲ�������һ�����Ժ󣬱���ⲿ��
			else
			{
				// ��¼�¿�ȵ��յ�����
				xRight = ROUND(pT1->x);
				// ��ʼ����
				for(int x = xLeft;x < xRight;x++)//����ҿ�
				{
					pDC->SetPixelV(x, pCurrentBucketTable->ScanLine, 
							RGB(pT1->pStart.c.red * 255, 
							    pT1->pStart.c.green * 255, 
								pT1->pStart.c.blue * 255));
				}
				// ����ⲿ��
				bInside = FALSE;
			}
		}
		for(pT1 = m_pHeadEdge;pT1 != NULL;pT1 = pT1->pNext)//�ߵ�������
			pT1->x = pT1->x + pT1->m;
	}
}

void CBucketTableFill::AddEdgeTable(CActiveEdgeTable* pNewEdge)//�ϲ���߱�
{
	CActiveEdgeTable* pCurrentEdge = m_pHeadEdge;
	// ���ȫ�ֻ�߱�Ϊ�ա�
	if(NULL == pCurrentEdge)
	{
		m_pHeadEdge = pNewEdge;
		// �ѵ�ǰ��߱����ȫ�ֻ�߱�
		pCurrentEdge = m_pHeadEdge;
	}
	// ���ȫ�ֻ�߱�Ϊ��
	else
	{
		// �ѵ�ǰ��߱�׷�ӵ�ȫ�ֻ�߱��С�
		while(NULL != pCurrentEdge->pNext)
			pCurrentEdge = pCurrentEdge->pNext;
		pCurrentEdge->pNext = pNewEdge;
	}
}

// �߱��ð�������㷨
void CBucketTableFill::SortEdgeTable(void)
{
	CActiveEdgeTable* pT1, *pT2;
	int Count = 1;
	pT1 = m_pHeadEdge;
	if(NULL == pT1)
		return;
	// ����û��û����һ�����
	if(NULL == pT1->pNext) 
	{
		// Ͱ���ֻ��һ���ߣ�����Ҫ����
		return; 
	}
	// ͳ�Ʊ߽��ĸ���
	while(NULL != pT1->pNext)
	{
		Count++;
		pT1 = pT1->pNext;
	}
	// ѭ��ÿһ��Ԫ�ء�
	for(int i = 0;i < Count - 1;i++) // ð������
	{
		CActiveEdgeTable** pPre = &m_pHeadEdge;
		pT1 = m_pHeadEdge;
		// ѭ��pT1�����Count - 1 - i��Ԫ�ء�
		for (int j = 0;j < Count - 1 - i;j++)
		{
			pT2 = pT1->pNext;
			// ���pT1��pT2�ұߣ����������ߵ�x������ͬ������pT1��б�ʸ���
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

// ��ȫɾ������Ͱ��Ͱ�����ӵı�
void CBucketTableFill::ClearMemory(void)
{
	// ɾ��ȫ�ֻ�߱�
	DeleteEdgeTableChain(m_pHeadEdge);
	CBucketTable* pBucket = m_pHeadBucketTable;
	while (NULL != pBucket)//���ÿһ��Ͱ
	{
		CBucketTable *pBucketTemp=pBucket->pNext;
		// ɾ��һ��Ͱ��Ԫ�صĻ�߱�
		DeleteEdgeTableChain(pBucket->m_pEdgeTable);
		// ɾ�����Ͱ��Ԫ�ء�
		delete pBucket;
		pBucket = pBucketTemp;
	}
	m_pHeadBucketTable = NULL;
	m_pHeadEdge = NULL;
}

// ɾ��һ�����Ԫ��
void CBucketTableFill::DeleteEdgeTableChain(CActiveEdgeTable* pActiveEdgeTable)
{
	while(pActiveEdgeTable!=NULL)
	{
		CActiveEdgeTable* pActiveEdgeTableTemp = pActiveEdgeTable->pNext;
		delete pActiveEdgeTable;
		pActiveEdgeTable = pActiveEdgeTableTemp;
	}
}
