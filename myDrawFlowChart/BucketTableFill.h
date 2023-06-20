#pragma once
#include"ActiveEdgeTable.h"
#include"BucketTable.h"

class CBucketTableFill
{
public:
	CBucketTableFill(void);
	virtual ~CBucketTableFill(void);
	void SetPoint(CColorPoint* p, int n);//初始化多边形顶点
	void CreateBucketTable(void);//创建桶表
	void CreateEdgeTable(void);//创建边表
	void FillPolygon(CDC* pDC);//填充多边形
private:
	void AddEdgeTable(CActiveEdgeTable* pNewEdge);//合并边表
	void SortEdgeTable(void);//边表排序
 	void ClearMemory(void);//清理内存
	void DeleteEdgeTableChain(CActiveEdgeTable* pAET);//删除边表
private:
	int nPointNumber;//顶点个数
	CColorPoint* P;//顶点坐标动态数组
	CActiveEdgeTable* pHeadE,* pCurrentE,* pEdge;//活动边表结点指针
	CBucketTable* pHeadB,* pCurrentB;//桶表结点指针
};

