// DrawFlowChartDoc.cpp : implementation of the CDrawFlowChartDoc class
//

#include "stdafx.h"
#include "DrawFlowChart.h"

#include "flowchart_cJSON.h"
#include "MainFrm.h"
#include "DrawFlowChartDoc.h"
#include "DrawFlowChartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartDoc

IMPLEMENT_DYNCREATE(CDrawFlowChartDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrawFlowChartDoc, CDocument)
	//{{AFX_MSG_MAP(CDrawFlowChartDoc)
	ON_COMMAND(ID_EDIT_SMOOTH_BEZIERLINE, OnEditSmoothBezierline)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartDoc construction/destruction

CDrawFlowChartDoc::CDrawFlowChartDoc()
{
	// TODO: add one-time construction code here

}

CDrawFlowChartDoc::~CDrawFlowChartDoc()
{
}

BOOL CDrawFlowChartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle("FlowChartEditor");
	m_GraphManager.DeleteAll();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartDoc diagnostics

#ifdef _DEBUG
void CDrawFlowChartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDrawFlowChartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartDoc commands

BOOL CDrawFlowChartDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	// POSITION pos = GetFirstViewPosition();
	// CDrawFlowChartView *pView = (CDrawFlowChartView*)GetNextView(pos);

	// m_GraphManager.SaveParamsToJSON(objJSON);
	cJSON * objJSON = getJSONFromFile(lpszPathName);
	if (objJSON == NULL)
	{
		AfxMessageBox("Format error!");
		return FALSE;
	}
	cJSON *child = objJSON->child;
    while(child)
    {   
        switch ((child->type)&255)
        {   
        case cJSON_True:    
            TRACE("cJSON_True"); 
			break;
        case cJSON_Number:    
            {   
                if(strcmp(child->string, "FocusID") == 0)
                {   
                    m_GraphManager.m_FocusID = (int)child->valueint ;
                }
            }   
            break;
        case cJSON_String: 
            TRACE("cJSON_String\n"); 
            break;
        case cJSON_Array:
            TRACE("cJSON_Array\n"); 
            break;
        case cJSON_Object:  
            TRACE("cJSON_Object\n"); 
            {   
                if(strcmp(child->string, "CRectangle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateRectangle(), child);
                }
				else if(strcmp(child->string, "CRoundHeadRectangle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateRoundHeadRectangle(), child);
                }
				else if(strcmp(child->string, "CCustomRoundRectangle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCustomRoundRectangle(), child);
                }
				else if(strcmp(child->string, "CLadder") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateLadder(), child);
                }
				else if(strcmp(child->string, "CRoundRectangle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateRoundRectangle(), child);
                }
				else if(strcmp(child->string, "CCylinderGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCylinderGraph(), child);
                }
				else if(strcmp(child->string, "CDocumentGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateDocumentGraph(), child);
                }
				else if(strcmp(child->string, "CHexagon") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateHexagon(), child);
                }
				else if(strcmp(child->string, "CEllipse") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateEllipse(), child);
                }
				else if(strcmp(child->string, "CDiamond") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateDiamond(), child);
                }
				else if(strcmp(child->string, "CParallelogram") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateParallelogram(), child);
                }
				else if(strcmp(child->string, "CArrowLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateArrowLine(), child);
                }
				else if(strcmp(child->string, "CBezierLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBezierLine(), child);
                }
				else if(strcmp(child->string, "CCustomBezierLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCustomBezierLine(), child);
                }
				else if(strcmp(child->string, "CCubicSplineLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCubicSplineLine(), child);
                }
				else if(strcmp(child->string, "CCubicBox") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCubicBox(), child);
                }
				else if(strcmp(child->string, "CMeshSphere") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateMeshSphere(), child);
                }
				else if(strcmp(child->string, "CQuadraticSplineLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateQuadraticSplineLine(), child);
                }
				else if(strcmp(child->string, "CArcLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateArcLine(), child);
                }
				else if(strcmp(child->string, "CPolygonalLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreatePolygonalLine(), child);
                }
				else if(strcmp(child->string, "CBrokenLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBrokenLine(), child);
                }
				else if(strcmp(child->string, "CStart") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateStart(), child);
                }
				else if(strcmp(child->string, "CEnd") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateEnd(), child);
                }
				else if(strcmp(child->string, "CGenericLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateLine(), child);
                }
				// 
				else if(strcmp(child->string, "CDDALine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateDDALine(), child);
                }
				else if(strcmp(child->string, "CBresenhamLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBresenhamLine(), child);
                }
				else if(strcmp(child->string, "CWuAntiLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateWuAntiLine(), child);
                }
				else if(strcmp(child->string, "CFlatColorTriangle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateFlatColorTriangle(), child);
                }
				else if(strcmp(child->string, "CSmoothColorTriangle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateSmoothColorTriangle(), child);
                }
				else if(strcmp(child->string, "CEdgeTableFillPolygon") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateEdgeTableFillPolygon(), child);
                }
				else if(strcmp(child->string, "CEdgeFillPolygon") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateEdgeFillPolygon(), child);
                }
				else if(strcmp(child->string, "CFenceFillPolygon") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateFenceFillPolygon(), child);
                }
				else if(strcmp(child->string, "CScanLineFillPolygon") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateScanLineFillPolygon(), child);
                }
				else if(strcmp(child->string, "CRotatePentagram") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateRotatePentagram(), child);
                }
				else if(strcmp(child->string, "CCohenSutherlandClipLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCohenSutherlandClipLine(), child);
                }
				else if(strcmp(child->string, "CMidPointDividLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateMidPointDividLine(), child);
                }
				else if(strcmp(child->string, "CLiangBarskyClipLine") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateLiangBarskyClipLine(), child);
                }
				else if(strcmp(child->string, "CArbitraryRotateCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateArbitraryRotateCube(), child);
                }
				else if(strcmp(child->string, "COrthogonalRotateCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateOrthogonalRotateCube(), child);
                }
				else if(strcmp(child->string, "CCabinetRotateCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCabinetRotateCube(), child);
                }
				else if(strcmp(child->string, "CPerspectiveRotateCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreatePerspectiveRotateCube(), child);
                }
				else if(strcmp(child->string, "CCubicBezierCurve") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateCubicBezierCurve(), child);
                }
				else if(strcmp(child->string, "CBicubicBezierPatchGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBicubicBezierPatchGraph(), child);
                }
				else if(strcmp(child->string, "CBicubicBezierSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBicubicBezierSphereGraph(), child);
                }
				else if(strcmp(child->string, "CRationalQuadraticBezierCircle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateRationalQuadraticBezierCircle(), child);
                }
				else if(strcmp(child->string, "CRationalQuadraticBezierSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateRationalQuadraticBezierSphereGraph(), child);
                }
				else if(strcmp(child->string, "CBackfaceCullRotateCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBackfaceCullRotateCube(), child);
                }
				else if(strcmp(child->string, "CThreeCrossRotateCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateThreeCrossRotateCube(), child);
                }
				else if(strcmp(child->string, "CSpatialArrangedCube") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateSpatialArrangedCube(), child);
                }
				else if(strcmp(child->string, "CGouraudBicubicBezierSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateGouraudBicubicBezierSphereGraph(), child);
                }
				else if(strcmp(child->string, "CGouraudBiquatricBezierSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateGouraudBiquatricBezierSphereGraph(), child);
                }
				else if(strcmp(child->string, "CPhongBiquatricBezierSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreatePhongBiquatricBezierSphereGraph(), child);
                }
				else if(strcmp(child->string, "CTextureCubeGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateTextureCubeGraph(), child);
                }
				else if(strcmp(child->string, "CTextureSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateTextureSphereGraph(), child);
                }
				else if(strcmp(child->string, "CBumpTextureSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateBumpTextureSphereGraph(), child);
                }
				else if(strcmp(child->string, "CAntiAliasedBumpTextureSphereGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateAntiAliasedBumpTextureSphereGraph(), child);
                }
				else if(strcmp(child->string, "CObjFileCubeGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateObjFileCubeGraph(), child);
                }
				else if(strcmp(child->string, "CObjFileTeapotGraph") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateObjFileTeapotGraph(), child);
                }
				else if(strcmp(child->string, "CMiddleCircle") == 0)
                {   
                    m_GraphManager.AddGraph(m_GraphFactory.CreateMiddleCircle(), child);
                }
				else 
				{
					TRACE("Unknown graph");
				}
            }   
            break;
        }   
        child = child->next ;
    }
	
	cJSON_Delete(objJSON);
	// When we load all of graph, we need to calculate 
	// the previous and next graph for each link graph.
	m_GraphManager.CheckAllLinkGraph();
	return TRUE;
}

cJSON * CDrawFlowChartDoc::getJSONFromFile(LPCTSTR lpszPathName)
{
	CFile file;
    file.Open(lpszPathName, CFile::modeRead);
	int iBufLen = file.GetLength();
	char * pPrintPtr = (char *)malloc(iBufLen + 1);
    file.Read(pPrintPtr, iBufLen + 1);
    file.Close();
	cJSON * objJSON = cJSON_Parse(pPrintPtr);
	free(pPrintPtr);
	return objJSON;
}

BOOL CDrawFlowChartDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	cJSON * objJSON = cJSON_CreateObject();
	m_GraphManager.SaveToJSON(objJSON);
	saveJSONToFile(objJSON, lpszPathName);
	cJSON_Delete(objJSON);
	// return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

void CDrawFlowChartDoc::saveJSONToFile(cJSON * objJSON, LPCTSTR lpszPathName)
{
	char * pPrintPtr = cJSON_Print(objJSON);
	// TRACE(pPrintPtr);
	CFile file;
    //不管文件是否存在，都创建新文件
    file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite);
    //开始写文件
    file.Write(pPrintPtr, strlen(pPrintPtr));
    file.Flush();
    file.Close();
	
	free(pPrintPtr);
}

void CDrawFlowChartDoc::OnEditSmoothBezierline() 
{
	// TODO: Add your command handler code here
	m_GraphManager.SmoothAllBreizerLink();
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->Invalidate();
}
