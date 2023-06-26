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
	CGraph* CreateDiamond();
	CGraph* CreateParallelogram();
	CGraph* CreateRectangle();
	CGraph* CreateRoundHeadRectangle();
	CGraph* CreateRoundRectangle();
	CGraph* CreateCylinderGraph();
	CGraph* CreateDocumentGraph();
	CGraph* CreateCustomRoundRectangle();
	CGraph* CreateLadder();
	CGraph* CreateHexagon();
	CGraph* CreateBrokenLine();
	CGraph* CreateLine();
	CGraph* CreateArrowLine();
	CGraph* CreateBezierLine();
	CGraph* CreateCustomBezierLine();
	CGraph* CreateCubicSplineLine();
	CGraph* CreateCubicBox();
	CGraph* CreateMeshSphere();
	CGraph* CreateQuadraticSplineLine();
	CGraph* CreateArcLine();
	CGraph* CreatePolygonalLine();
	CGraph* CreateEllipse();
	CGraph* CreateStart();
	CGraph* CreateEnd();
	// 
	CGraph* CreateDDALine();
	CGraph* CreateBresenhamLine();
	CGraph* CreateWuAntiLine();
	CGraph* CreateFlatColorTriangle();
	CGraph* CreateSmoothColorTriangle();
	CGraph* CreateMiddleCircle();
	CGraph* CreateEdgeTableFillPolygon();
	CGraph* CreateEdgeFillPolygon();
	CGraph* CreateFenceFillPolygon();
	CGraph* CreateScanLineFillPolygon();
	CGraph* CreateRotatePentagram();
	CGraph* CreateCohenSutherlandClipLine();
	CGraph* CreateMidPointDividLine();
	CGraph* CreateLiangBarskyClipLine();
};

#endif // !defined(AFX_GRAPHFACTORY_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
