#if !defined(AFX_BUCKETTABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUCKETTABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ActiveEdgeTable.h"

class CBucketTable//Ͱ����
{
public:
	CBucketTable(void);
	virtual ~CBucketTable(void);
public:
	int ScanLine;                    // ɨ��������
	CActiveEdgeTable* m_pEdgeTable;  // Ͱ�ϵı߱�ָ��
	CBucketTable* pNext;             // ָ����һ��Ͱ���ָ�롣
};

#endif // !defined(AFX_BUCKETTABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

