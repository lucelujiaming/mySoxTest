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

private:
	void DoubleBuffer(CDC* pDC);
	void DrawAllGraph( CDC *pDC);
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawFlowChartView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCreateRectangle();
	afx_msg void OnCreateRoundHeadRectangle();
	afx_msg void OnCreateRoundRectangle();
	afx_msg void OnCreateCylinderGraph();
	afx_msg void OnCreateDocumentGraph();
	afx_msg void OnCreateCustomRoundRectangle();
	afx_msg void OnCreateLadder();
	afx_msg void OnCreateHexagon();
	afx_msg void OnCreateEllipse();
	afx_msg void OnCreateDiamond();
	afx_msg void OnCreateDealParallelogram();
	afx_msg void OnCreateGenericLine();
	afx_msg void OnCreateBezierLine();
	afx_msg void OnCreateCustomBezierLine();
	afx_msg void OnCreateCubicSplineLine();
	afx_msg void OnCreateCubicBox();
	afx_msg void OnCreateMeshSphere();
	afx_msg void OnCreateQuadraticSplineLine();
	afx_msg void OnCreateArcLine();
	afx_msg void OnCreateArrowLine();
	afx_msg void OnCreatePolygonalLine();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCreateBrokenLine();
	afx_msg void OnCreateStart();
	afx_msg void OnCreateEnd();
	afx_msg void OnSaveBmp();
	afx_msg void OnSearchPath();
	afx_msg void OnMarkPath();
	afx_msg void OnStopMark();
	afx_msg void OnToolbarEnd();
	afx_msg void OnToolbarBrokenLine();
	afx_msg void OnToolbarArrowLine();
	afx_msg void OnToolbarBezierLine();
	afx_msg void OnToolbarCustomBezierLine();
	afx_msg void OnToolbarCubicSplineLine();
	afx_msg void OnToolbarCubicBox();
	afx_msg void OnToolbarMeshSphere();
	afx_msg void OnToolbarQuadraticSplineLine();
	afx_msg void OnToolbarArcLine();
	afx_msg void OnToolbarPolygonalLine();
	afx_msg void OnToolbarGenericLine();
	afx_msg void OnToolbarDiamond();
	afx_msg void OnToolbarEllipse();
	afx_msg void OnToolbarLadder();
	afx_msg void OnToolbarNext();
	afx_msg void OnToolbarParallelogram();
	afx_msg void OnToolbarRectangle();
	afx_msg void OnToolbarRoundHeadRectangle();
	afx_msg void OnToolbarCustomRoundRectangle();
	afx_msg void OnToolbarRoundRectangle();
	afx_msg void OnToolbarCylinderGraph();
	afx_msg void OnToolbarDocumentGraph();
	afx_msg void OnToolbarHexagon();
	afx_msg void OnToolbarSearch();
	afx_msg void OnToolbarStart();
	afx_msg void OnToolbarStop();
	//
	afx_msg void OnToolbarDDALine();
	afx_msg void OnToolbarBresenhamLine();
	afx_msg void OnToolbarMiddleCircle();
	afx_msg void OnToolbarWuAntiLine();
	afx_msg void OnToolbarFlatColorTriangle();
	afx_msg void OnToolbarSmoothColorTriangle();
	afx_msg void OnToolbarEdgeTableFillPolygon();
	afx_msg void OnToolbarEdgeFillPolygon();
	afx_msg void OnToolbarFenceFillPolygon();
	afx_msg void OnToolbarScanLineFillPolygon();
	afx_msg void OnToolbarRotatePentagram();
	afx_msg void OnToolbarCohenSutherlandClipLine();
	afx_msg void OnToolbarMidPointDividLine();
	afx_msg void OnToolbarLiangBarskyClipLine();
	afx_msg void OnToolbarArbitraryRotateCube();
	afx_msg void OnToolbarOrthogonalRotateCube();
	afx_msg void OnToolbarCabinetRotateCube();
	afx_msg void OnToolbarPerspectiveRotateCube();
	afx_msg void OnToolbarCubicBezierCurve();
	afx_msg void OnToolbarBicubicBezierPatchGraph();
	afx_msg void OnToolbarBicubicBezierSphereGraph();
	afx_msg void OnToolbarRationalQuadraticBezierCircle();
	afx_msg void OnToolbarRationalQuadraticBezierSphereGraph();
	afx_msg void OnToolbarBackfaceCullRotateCube();
	afx_msg void OnToolbarThreeCrossRotateCube();
	afx_msg void OnToolbarSpatialArrangedCube();
	afx_msg void OnToolbarGouraudBicubicBezierSphereGraph();
	afx_msg void OnToolbarGouraudBiquatricBezierSphereGraph();
	afx_msg void OnToolbarPhongBiquatricBezierSphereGraph();
	afx_msg void OnToolbarTextureCubeGraph();
	//
	afx_msg void OnCreateDDALine();
	afx_msg void OnCreateBresenhamLine();
	afx_msg void OnCreateWuAntiLine();
	afx_msg void OnCreateMiddleCircle();
	afx_msg void OnCreateFlatColorTriangle();
	afx_msg void OnCreateSmoothColorTriangle();
	afx_msg void OnCreateEdgeTableFillPolygon();
	afx_msg void OnCreateEdgeFillPolygon();
	afx_msg void OnCreateFenceFillPolygon();
	afx_msg void OnCreateScanLineFillPolygon();
	afx_msg void OnCreateRotatePentagram();
	afx_msg void OnCreateCohenSutherlandClipLine();
	afx_msg void OnCreateMidPointDividLine();
	afx_msg void OnCreateLiangBarskyClipLine();
	afx_msg void OnCreateArbitraryRotateCube();
	afx_msg void OnCreateOrthogonalRotateCube();
	afx_msg void OnCreateCabinetRotateCube();
	afx_msg void OnCreatePerspectiveRotateCube();
	afx_msg void OnCreateCubicBezierCurve();
	afx_msg void OnCreateBicubicBezierPatchGraph();
	afx_msg void OnCreateBicubicBezierSphereGraph();
	afx_msg void OnCreateRationalQuadraticBezierCircle();
	afx_msg void OnCreateRationalQuadraticBezierSphereGraph();
	afx_msg void OnCreateBackfaceCullRotateCube();
	afx_msg void OnCreateThreeCrossRotateCube();
	afx_msg void OnCreateSpatialArrangedCube();
	afx_msg void OnCreateGouraudBicubicBezierSphereGraph();
	afx_msg void OnCreateGouraudBiquatricBezierSphereGraph();
	afx_msg void OnCreatePhongBiquatricBezierSphereGraph();
	afx_msg void OnCreateTextureCubeGraph();
	//
	afx_msg void OnSize(UINT nType, int cx, int cy);
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
