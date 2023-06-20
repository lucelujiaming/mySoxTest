#if !defined(AFX_BUCKETTABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUCKETTABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ActiveEdgeTable.h"

class CBucketTable//桶表类
{
public:
	CBucketTable(void);
	virtual ~CBucketTable(void);
public:
	int ScanLine;//扫描线
	CActiveEdgeTable* pET;//桶上的边表指针
	CBucketTable* pNext;
};

#endif // !defined(AFX_BUCKETTABLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

