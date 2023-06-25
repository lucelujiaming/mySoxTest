#include "stdafx.h"
#include "BucketTable.h"


CBucketTable::CBucketTable(void)
{
	ScanLine = 0;
	m_pEdgeTable = NULL;
	pNext = NULL;
}


CBucketTable::~CBucketTable(void)
{
}
