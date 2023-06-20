#pragma once
#include"ActiveEdgeTable.h"
#include"BucketTable.h"

class CBucketTableFill
{
public:
	CBucketTableFill(void);
	virtual ~CBucketTableFill(void);
	void SetPoint(CColorPoint* p, int n);//��ʼ������ζ���
	void CreateBucketTable(void);//����Ͱ��
	void CreateEdgeTable(void);//�����߱�
	void FillPolygon(CDC* pDC);//�������
private:
	void AddEdgeTable(CActiveEdgeTable* pNewEdge);//�ϲ��߱�
	void SortEdgeTable(void);//�߱�����
 	void ClearMemory(void);//�����ڴ�
	void DeleteEdgeTableChain(CActiveEdgeTable* pAET);//ɾ���߱�
private:
	int nPointNumber;//�������
	CColorPoint* P;//�������궯̬����
	CActiveEdgeTable* pHeadE,* pCurrentE,* pEdge;//��߱���ָ��
	CBucketTable* pHeadB,* pCurrentB;//Ͱ����ָ��
};

