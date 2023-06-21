#if !defined(AFX_BUCKETTABLEFILL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUCKETTABLEFILL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"ActiveEdgeTable.h"
#include"BucketTable.h"
	
/************************************************************************/
/* ���ܣ�ʹ����Ч�߱�����㷨���һ������Ρ�                           */
/************************************************************************/
/* ����㷨��ԭ���������ġ������������ҳ�������ε���ߵ����͵㡣     */
/* ֮���Ҵ���͵㵽��ߵ�ɨ����������Ρ���¼ÿһ��ɨ���߾����ıߡ�     */
/* ��Ȼ����ɨ������У�����εı߻����ɨ���ߵ��ƶ�����������           */
/* ÿһ��ɨ���߾����ı߾ͻ��γ�һ����������ǻ�߱�               */
/* ʹ��CActiveEdgeTable��������                                         */
/* Ϊ�˱������е�ɨ���߶�Ӧ�Ļ�߱����ǻ���Ҫһ��Ͱ��             */
/* ���Ͱ����һ����������ÿһ��Ԫ�ض�Ӧһ��ɨ���ߣ����ұ���           */
/* �����ɨ���߾����ı߶�Ӧ�Ļ�߱�ʹ��CActiveEdgeTable��������     */
/* ����������㷨��ԭ����ô�Զ��׼��������������£�                   */
/*       1. ��ʼ������ζ��㡣                                          */
/*       2. ����Ͱ��                                                  */
/*       3. ΪͰ���е�ÿһ��Ԫ�ش�����Ӧ�Ļ�߱�                    */
/*       4. ��������ļ������������������                          */
/************************************************************************/
class CBucketTableFill
{
public:
	CBucketTableFill(void);
	virtual ~CBucketTableFill(void);
	void SetPoint(CColorPoint* p, int n);             // 1. ��ʼ������ζ���
	void CreateBucketTable(void);                     // 2. ����Ͱ��
	void CreateEdgeTable(void);                       // 3. �����߱�
	void FillPolygon(CDC* pDC);                       // 4. �������
private:
	void AddEdgeTable(CActiveEdgeTable* pNewEdge);     // �ϲ��߱�
	void SortEdgeTable(void);                          // �߱�����
 	void ClearMemory(void);                            // �����ڴ�
	void DeleteEdgeTableChain(CActiveEdgeTable* pAET); // �ӻ�߱���ɾ��һ�����Ԫ��
	
private:
	int                m_nPointNumber;   // �������
	CColorPoint*       m_PolygonPoints;  // �������궯̬����
	CActiveEdgeTable*  m_pHeadEdge;    			 // ��߱�������ͷָ��
	CBucketTable*      m_pHeadBucketTable;		 // Ͱ������ͷ
};

#endif // !defined(AFX_BUCKETTABLEFILL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

