// DrawFlowChartView.h : interface of the CDrawFlowChartView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWFLOWCHARTVIEW_H__BAEDFB52_1587_4ED5_8F66_644264320C3D__INCLUDED_)
#define AFX_DRAWFLOWCHARTVIEW_H__BAEDFB52_1587_4ED5_8F66_644264320C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Rectangle.h"
#include "GraphManager.h"
#include "GraphFactory.h"

class CDrawFlowChartView : public CView
{
protected: // create from serialization only
	CDrawFlowChartView();
	DECLARE_DYNCREATE(CDrawFlowChartView)

// Attributes
public:
	CDrawFlowChartDoc* GetDocument();

	CPoint m_lastMovePos;

	int m_OperateType;
	enum OPERATE_TYPE{SELECT, MOVE, CREATE, ADJUST_SIZE};

	bool m_LBtnSignal;
	bool m_LDoubleBtnSignal;
	bool m_IsControlFlow;

	CGraph* m_FocusGraph;

	CEdit *m_pEdit;

	//CGraphManager m_GraphManager;
	//CGraphFactory m_GraphFactory;
// Operations
public:
void OnGetMap();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawFlowChartView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawFlowChartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawFlowChartView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCreateRectangle();
	afx_msg void OnCreateEllipse();
	afx_msg void OnCreateDiamond();
	afx_msg void OnCreateDealParallelogram();
	afx_msg void OnCreateGenericLine();
	afx_msg void OnCreateArrowhead();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCreateControlFlow();
	afx_msg void OnCreateStart();
	afx_msg void OnCreateEnd();
	afx_msg void OnSaveBmp();
	afx_msg void OnSearchPath();
	afx_msg void OnMarkPath();
	afx_msg void OnStopMark();
	afx_msg void OnToolbarEnd();
	afx_msg void OnToolbarFlowcontrol();
	afx_msg void OnToolbarArrowHead();
	afx_msg void OnToolbarGenericLine();
	afx_msg void OnToolbarJudge();
	afx_msg void OnToolbarEllipse();
	afx_msg void OnToolbarNext();
	afx_msg void OnToolbarProcess();
	afx_msg void OnToolbarRect();
	afx_msg void OnToolbarSearch();
	afx_msg void OnToolbarStart();
	afx_msg void OnToolbarStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DrawFlowChartView.cpp
inline CDrawFlowChartDoc* CDrawFlowChartView::GetDocument()
   { return (CDrawFlowChartDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWFLOWCHARTVIEW_H__BAEDFB52_1587_4ED5_8F66_644264320C3D__INCLUDED_)
