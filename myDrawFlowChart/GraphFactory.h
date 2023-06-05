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
	CGraph* CreateJudgeDiamond();       // Adjusted
	CGraph* CreateProcessDiamond();
	CGraph* CreateRectangle();          // Adjusted
	CGraph* CreateRoundHeadRectangle();
	CGraph* CreateCustomRoundRectangle();
	CGraph* CreateHexagon();
	CGraph* CreateControlFlow();		// NotNeed
	CGraph* CreateLine();				// NotNeed
	CGraph* CreateArrowLine();			// NotNeed
	CGraph* CreatePolygonalLine();		// NotNeed
	CGraph* CreateEllipse();			// Adjusted
	CGraph* CreateStart();				// NotNeed
	CGraph* CreateEnd();				// NotNeed

};

#endif // !defined(AFX_GRAPHFACTORY_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
