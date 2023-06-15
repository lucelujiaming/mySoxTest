// GraphFactory.cpp: implementation of the CGraphFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "GraphFactory.h"

#include "Rectangle.h"
#include "RoundHeadRectangle.h"
#include "Hexagon.h"
#include "RoundRectangle.h"
#include "DocumentGraph.h"
#include "CustomRoundRectangle.h"
#include "Ladder.h"
#include "CylinderGraph.h"
#include "Ellipse.h"
#include "Diamond.h"
#include "Parallelogram.h"
#include "GenericLine.h"
#include "ArrowLine.h"
#include "PolygonalLine.h"
#include "BezierLine.h"
#include "CustomBezierLine.h"
#include "CubicSplineLine.h"
#include "QuadraticSplineLine.h"
#include "ArcLine.h"
#include "BrokenLine.h"
#include "Start.h"
#include "End.h"
#include "CubicBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphFactory::CGraphFactory()
{

}

CGraphFactory::~CGraphFactory()
{

}

CGraph* CGraphFactory::CreateDiamond()
{
	return new CDiamond();
}

CGraph* CGraphFactory::CreateParallelogram()
{
	return new CParallelogram();
}

CGraph* CGraphFactory::CreateRectangle()
{
	return new CRectangle();
}

CGraph* CGraphFactory::CreateRoundHeadRectangle()
{
	return new CRoundHeadRectangle();
}

CGraph* CGraphFactory::CreateCustomRoundRectangle()
{
	return new CCustomRoundRectangle();
}

CGraph* CGraphFactory::CreateLadder()
{
	return new CLadder();
}

CGraph* CGraphFactory::CreateRoundRectangle()
{
	return new CRoundRectangle();
}

CGraph* CGraphFactory::CreateCylinderGraph()
{
	return new CCylinderGraph();
}

CGraph* CGraphFactory::CreateDocumentGraph()
{
	return new CDocumentGraph();
}

CGraph* CGraphFactory::CreateHexagon()
{
	return new CHexagon();
}

CGraph* CGraphFactory::CreateBrokenLine()
{
	return new CBrokenLine();
}

CGraph* CGraphFactory::CreateLine()
{
	return new CGenericLine();
}

CGraph* CGraphFactory::CreateArrowLine()
{
	return new CArrowLine();
}

CGraph* CGraphFactory::CreateBezierLine()
{
	return new CBezierLine();
}

CGraph* CGraphFactory::CreateCustomBezierLine()
{
	return new CCustomBezierLine();
}

CGraph* CGraphFactory::CreateCubicSplineLine()
{
	return new CCubicSplineLine();
}

CGraph* CGraphFactory::CreateCubicBox()
{
	return new CCubicBox();
}

CGraph* CGraphFactory::CreateQuadraticSplineLine()
{
	return new CQuadraticSplineLine();
}

CGraph* CGraphFactory::CreateArcLine()
{
	return new CArcLine();
}

CGraph* CGraphFactory::CreatePolygonalLine()
{
	return new CPolygonalLine();
}

CGraph* CGraphFactory::CreateEllipse()
{
	return new CEllipse();
}

CGraph* CGraphFactory::CreateStart()
{
	return new CStart();
}

CGraph* CGraphFactory::CreateEnd()
{
	return new CEnd();
}