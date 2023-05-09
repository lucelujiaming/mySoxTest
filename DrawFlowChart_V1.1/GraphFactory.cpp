// GraphFactory.cpp: implementation of the CGraphFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "GraphFactory.h"

#include "Rectangle.h"
#include "Ellipse.h"
#include "Diamond.h"
#include "DealDiamond.h"
#include "GenericLine.h"
#include "Arrowhead.h"
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
	return new CDealDiamond();
}

CGraph* CGraphFactory::CreateRectangle()
{
	return new CRectangle();
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