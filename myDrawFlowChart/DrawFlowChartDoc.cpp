// DrawFlowChartDoc.cpp : implementation of the CDrawFlowChartDoc class
//

#include "stdafx.h"
#include "DrawFlowChart.h"

#include "flowchart_cJSON.h"
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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
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
	SetTitle("流程图编辑器");
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
