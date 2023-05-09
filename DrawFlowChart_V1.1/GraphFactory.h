// GraphFactory.h: interface for the CGraphFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHFACTORY_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
#define AFX_GRAPHFACTORY_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

class CGraphFactory  
{
public:
	CGraphFactory();
	virtual ~CGraphFactory();

public:
	CGraph* CreateJudgeDiamond();
	CGraph* CreateProcessDiamond();
	CGraph* CreateRectangle();
	CGraph* CreateControlFlow();
	CGraph* CreateLine();
	CGraph* CreateArrowLine();
	CGraph* CreateEllipse();
	CGraph* CreateStart();
	CGraph* CreateEnd();

};

#endif // !defined(AFX_GRAPHFACTORY_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
