// DrawFlowChart.h : main header file for the DRAWFLOWCHART application
//

#if !defined(AFX_DRAWFLOWCHART_H__22E31E5D_94F9_46CA_A7B5_CB107C438662__INCLUDED_)
#define AFX_DRAWFLOWCHART_H__22E31E5D_94F9_46CA_A7B5_CB107C438662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartApp:
// See DrawFlowChart.cpp for the implementation of this class
//

class CDrawFlowChartApp : public CWinApp
{
public:
	CDrawFlowChartApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawFlowChartApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDrawFlowChartApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWFLOWCHART_H__22E31E5D_94F9_46CA_A7B5_CB107C438662__INCLUDED_)
