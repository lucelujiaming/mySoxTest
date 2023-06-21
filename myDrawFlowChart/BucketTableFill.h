#if !defined(AFX_BUCKETTABLEFILL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUCKETTABLEFILL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"ActiveEdgeTable.h"
#include"BucketTable.h"
	
/************************************************************************/
/* 功能：使用有效边表填充算法填充一个多边形。                           */
/************************************************************************/
/* 这个算法的原理是这样的。就是我首先找出来多边形的最高点和最低点。     */
/* 之后我从最低点到最高点扫描整个多边形。记录每一根扫描线经过的边。     */
/* 当然，在扫描过程中，多边形的边会跟着扫描线的移动进行增减。           */
/* 每一根扫描线经过的边就会形成一个链表。这就是活动边表。               */
/* 使用CActiveEdgeTable类描述。                                         */
/* 为了保存所有的扫描线对应的活动边表。我们还需要一个桶表。             */
/* 这个桶表是一个链表。其中每一个元素对应一根扫描线，并且保存           */
/* 了这根扫描线经过的边对应的活动边表。使用CActiveEdgeTable类描述。     */
/* 明白了这个算法的原理。那么显而易见，操作步骤如下：                   */
/*       1. 初始化多边形顶点。                                          */
/*       2. 创建桶表。                                                  */
/*       3. 为桶表中的每一个元素创建对应的活动边表。                    */
/*       4. 根据上面的计算结果，填充这个多边形                          */
/************************************************************************/
class CBucketTableFill
{
public:
	CBucketTableFill(void);
	virtual ~CBucketTableFill(void);
	void SetPoint(CColorPoint* p, int n);             // 1. 初始化多边形顶点
	void CreateBucketTable(void);                     // 2. 创建桶表
	void CreateEdgeTable(void);                       // 3. 创建边表
	void FillPolygon(CDC* pDC);                       // 4. 填充多边形
private:
	void AddEdgeTable(CActiveEdgeTable* pNewEdge);     // 合并边表
	void SortEdgeTable(void);                          // 边表排序
 	void ClearMemory(void);                            // 清理内存
	void DeleteEdgeTableChain(CActiveEdgeTable* pAET); // 从活动边表中删除一个活动边元素
	
private:
	int                m_nPointNumber;   // 顶点个数
	CColorPoint*       m_PolygonPoints;  // 顶点坐标动态数组
	CActiveEdgeTable*  m_pHeadEdge;    			 // 活动边表结点链表头指针
	CBucketTable*      m_pHeadBucketTable;		 // 桶表链表头
};

#endif // !defined(AFX_BUCKETTABLEFILL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

