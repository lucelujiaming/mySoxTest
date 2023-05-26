// DrawFlowChartDoc.cpp : implementation of the CDrawFlowChartDoc class
//

#include "stdafx.h"
#include "DrawFlowChart.h"

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
	SetTitle("Á÷³ÌÍ¼±à¼­Æ÷");

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartDoc serialization

void CDrawFlowChartDoc::Serialize(CArchive& ar)
{
	//POSITION pos = GetFirstViewPosition();
	//CDrawFlowChartView *pView = (CDrawFlowChartView*)GetNextView(pos);

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		int FocusID = m_GraphManager.m_FocusID;
		ar<<FocusID;
	}
	else
	{
		// TODO: add loading code here
		int FocusID = -1;
		ar>>FocusID;
		m_GraphManager.m_FocusID = FocusID;
	}

	m_GraphManager.m_Graphs.Serialize(ar);
	//pView->m_GraphManager.m_path.Serialize(ar);
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
	//POSITION pos = GetFirstViewPosition();
	//CDrawFlowChartView *pView = (CDrawFlowChartView*)GetNextView(pos);

	m_GraphManager.CheckAllLinkGraph();

	return TRUE;
}
