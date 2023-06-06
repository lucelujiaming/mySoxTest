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
#include "Ellipse.h"
#include "Diamond.h"
#include "DealParallelogram.h"
#include "GenericLine.h"
#include "Arrowhead.h"
#include "PolygonalLine.h"
#include "ControlFlow.h"
#include "Start.h"
#include "End.h"

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

CGraph* CGraphFactory::CreateJudgeDiamond()
{
	return new CDiamond();
}

CGraph* CGraphFactory::CreateProcessDiamond()
{
	return new CDealParallelogram();
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

CGraph* CGraphFactory::CreateRoundRectangle()
{
	return new CRoundRectangle();
}

CGraph* CGraphFactory::CreateDocumentGraph()
{
	return new CDocumentGraph();
}

CGraph* CGraphFactory::CreateHexagon()
{
	return new CHexagon();
}

CGraph* CGraphFactory::CreateControlFlow()
{
	return new CControlFlow();
}

CGraph* CGraphFactory::CreateLine()
{
	return new CGenericLine();
}

CGraph* CGraphFactory::CreateArrowLine()
{
	return new CArrowhead();
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