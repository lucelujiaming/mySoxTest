// Facet.h: interface for the Facet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACET_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
#define AFX_FACET_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFacet
{
public:
	CFacet(void);
	~CFacet(void);
public:
	int pNumber;//表面顶点数
	int pIndex[4];//表面顶点索引号
};

#endif // !defined(AFX_FACET_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
