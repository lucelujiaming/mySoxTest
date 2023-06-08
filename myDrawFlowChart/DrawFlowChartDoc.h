// DrawFlowChartDoc.h : interface of the CDrawFlowChartDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWFLOWCHARTDOC_H__45B8FB5B_91A3_4EA5_A3A1_01ECDC2C1F46__INCLUDED_)
#define AFX_DRAWFLOWCHARTDOC_H__45B8FB5B_91A3_4EA5_A3A1_01ECDC2C1F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphManager.h"
#include "GraphFactory.h"

class CDrawFlowChartDoc : public CDocument
{
protected: // create from serialization only
	CDrawFlowChartDoc();
	DECLARE_DYNCREATE(CDrawFlowChartDoc)

// Attributes
public:
	CGraphManager m_GraphManager;
	CGraphFactory m_GraphFactory;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawFlowChartDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawFlowChartDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawFlowChartDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWFLOWCHARTDOC_H__45B8FB5B_91A3_4EA5_A3A1_01ECDC2C1F46__INCLUDED_)
