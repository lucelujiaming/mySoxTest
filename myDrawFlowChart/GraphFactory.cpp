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
#include "MeshSphere.h"
#include "DDALine.h"
#include "BresenhamLine.h"
#include "WuAntiLine.h"
#include "MiddleCircle.h"
#include "FlatColorTriangle.h"
#include "SmoothColorTriangle.h"
#include "EdgeTableFillPolygon.h"
#include "EdgeFillPolygon.h"
#include "FenceFillPolygon.h"
#include "ScanLineFillPolygon.h"
#include "RotatePentagram.h"
#include "CohenSutherlandClipLine.h"
#include "MidPointDividLine.h"
#include "LiangBarskyClipLine.h"
#include "ArbitraryRotateCube.h"
#include "OrthogonalRotateCube.h"
#include "CabinetRotateCube.h"
#include "PerspectiveRotateCube.h"
#include "CubicBezierCurve.h"
#include "BicubicBezierPatchGraph.h"
#include "BicubicBezierSphereGraph.h"
#include "RationalQuadraticBezierCircle.h"
#include "RationalQuadraticBezierSphereGraph.h"
#include "BackfaceCullRotateCube.h"
#include "ThreeCrossRotateCube.h"
#include "SpatialArrangedCube.h"
#include "GouraudBicubicBezierSphereGraph.h"
#include "GouraudBiquatricBezierSphereGraph.h"
#include "PhongBiquatricBezierSphereGraph.h"
#include "TextureCubeGraph.h"
#include "TextureSphereGraph.h"
#include "BumpTextureSphereGraph.h"
#include "AntiAliasedBumpTextureSphereGraph.h"
#include "ObjFileCubeGraph.h"

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

CGraph* CGraphFactory::CreateMeshSphere()
{
	return new CMeshSphere();
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

//
CGraph* CGraphFactory::CreateDDALine()
{
	return new CDDALine();
}

CGraph* CGraphFactory::CreateBresenhamLine()
{
	return new CBresenhamLine();
}

CGraph* CGraphFactory::CreateWuAntiLine()
{
	return new CWuAntiLine();
}

CGraph* CGraphFactory::CreateFlatColorTriangle()
{
	return new CFlatColorTriangle();
}

CGraph* CGraphFactory::CreateSmoothColorTriangle()
{
	return new CSmoothColorTriangle();
}

CGraph* CGraphFactory::CreateEdgeTableFillPolygon()
{
	return new CEdgeTableFillPolygon();
}

CGraph* CGraphFactory::CreateEdgeFillPolygon()
{
	return new CEdgeFillPolygon();
}

CGraph* CGraphFactory::CreateFenceFillPolygon()
{
	return new CFenceFillPolygon();
}

CGraph* CGraphFactory::CreateScanLineFillPolygon()
{
	return new CScanLineFillPolygon();
}

CGraph* CGraphFactory::CreateRotatePentagram()
{
	return new CRotatePentagram();
}

CGraph* CGraphFactory::CreateCohenSutherlandClipLine()
{
	return new CCohenSutherlandClipLine();
}

CGraph* CGraphFactory::CreateMidPointDividLine()
{
	return new CMidPointDividLine();
}

CGraph* CGraphFactory::CreateLiangBarskyClipLine()
{
	return new CLiangBarskyClipLine();
}

CGraph* CGraphFactory::CreateArbitraryRotateCube()
{
	return new CArbitraryRotateCube();
}

CGraph* CGraphFactory::CreateOrthogonalRotateCube()
{
	return new COrthogonalRotateCube();
}

CGraph* CGraphFactory::CreateCabinetRotateCube()
{
	return new CCabinetRotateCube();
}

CGraph* CGraphFactory::CreatePerspectiveRotateCube()
{
	return new CPerspectiveRotateCube();
}

CGraph* CGraphFactory::CreateCubicBezierCurve()
{
	return new CCubicBezierCurve();
}

CGraph* CGraphFactory::CreateBicubicBezierPatchGraph()
{
	return new CBicubicBezierPatchGraph();
}

CGraph* CGraphFactory::CreateBicubicBezierSphereGraph()
{
	return new CBicubicBezierSphereGraph();
}

CGraph* CGraphFactory::CreateRationalQuadraticBezierCircle()
{
	return new CRationalQuadraticBezierCircle();
}

CGraph* CGraphFactory::CreateRationalQuadraticBezierSphereGraph()
{
	return new CRationalQuadraticBezierSphereGraph();
}

CGraph* CGraphFactory::CreateBackfaceCullRotateCube()
{
	return new CBackfaceCullRotateCube();
}

CGraph* CGraphFactory::CreateThreeCrossRotateCube()
{
	return new CThreeCrossRotateCube();
}

CGraph* CGraphFactory::CreateSpatialArrangedCube()
{
	return new CSpatialArrangedCube();
}

CGraph* CGraphFactory::CreateGouraudBicubicBezierSphereGraph()
{
	return new CGouraudBicubicBezierSphereGraph();
}

CGraph* CGraphFactory::CreateGouraudBiquatricBezierSphereGraph()
{
	return new CGouraudBiquatricBezierSphereGraph();
}

CGraph* CGraphFactory::CreatePhongBiquatricBezierSphereGraph()
{
	return new CPhongBiquatricBezierSphereGraph();
}

CGraph* CGraphFactory::CreateTextureCubeGraph()
{
	return new CTextureCubeGraph();
}

CGraph* CGraphFactory::CreateTextureSphereGraph()
{
	return new CTextureSphereGraph();
}

CGraph* CGraphFactory::CreateBumpTextureSphereGraph()
{
	return new CBumpTextureSphereGraph();
}

CGraph* CGraphFactory::CreateAntiAliasedBumpTextureSphereGraph()
{
	return new CAntiAliasedBumpTextureSphereGraph();
}

CGraph* CGraphFactory::CreateObjFileCubeGraph()
{
	return new CObjFileCubeGraph();
}

CGraph* CGraphFactory::CreateObjFileTeapotGraph()
{
	return new CAntiAliasedBumpTextureSphereGraph();
}

CGraph* CGraphFactory::CreateMiddleCircle()
{
	return new CMiddleCircle();
}
//