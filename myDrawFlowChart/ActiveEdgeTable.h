#if !defined(AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include"ColorPoint.h"

/************************************************************************/
/* �߱����߱��ࡣ                                                   */
/* ������뵱ǰɨ�����ཻ�ı߳�Ϊ��Ч�ߡ�                               */
/* �ڴ���һ��ɨ���ߵ�ʱ�����ǽ������Ч�߽����󽻴�����               */
/************************************************************************/
class CActiveEdgeTable
{
public:
	CActiveEdgeTable(void);
	virtual ~CActiveEdgeTable(void);
public:
	double x;            // ��ǰɨ��������Ч�߽����x����
	int yMax;            // ��Ч�ߵ����yֵ�������жϸñߺ�ʱɨ����ϣ��Ա��������ñ�Ϊ���ñߡ�
	double m;            // б�ʵĵ���(x������)
	CColorPoint pStart;  // �ߵ����
	CColorPoint pEnd;    // �ߵ��յ�
	CActiveEdgeTable* pNext;
};

#endif // !defined(AFX_ACTIVEEDGETABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
