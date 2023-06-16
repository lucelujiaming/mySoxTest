// CubicSplineLine.h: interface for the CCubicSplineLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBICSPLINELINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
#define AFX_CUBICSPLINELINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include <math.h>
#include"P2.h"

#define BEZIERLINE_QUADRATIC               3    // Èý´Îº¯Êý

class CCubicSplineLine : public CGraph  
{
//		DECLARE_SERIAL(CCubicSplineLine)

public:
	CCubicSplineLine();
	virtual ~CCubicSplineLine();
	CString GetTypeName() { return CString("CCubicSplineLine"); }

public:
	void Draw( CDC *pdc, BOOL bShowSelectBorder = TRUE );
	void DrawFocus( CDC *pdc );

	void DrawSelectBorderArea( CDC *pdc );
	
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);
private:
	void LoadOnePointFromJSON(cJSON * objJSON);

public:
	void SetStartPoint( CPoint &pt );
	void SetEndPoint( CPoint &pt );
	void GetStartPoint(CPoint &pt);
	void GetEndPoint(CPoint &pt);
	void SetLastPoint( CPoint &pt );

	void SetPreviousGraph(CGraph *previousGraph);
	void SetNextgraph(CGraph *nextGraph);
	CGraph* GetPreviousGraph();
	CGraph* GetNextgraph();

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	bool IsEditable();
	bool IsControlFlow();

private:
	double GetDistance(int x1, int y1, int x2,int y2);
	void DrawArrow( CDC *pdc );

private:
	POINT * m_objCubicSplineControlPoint;
	void ReadPoint(void);
	void DrawDataPoint(CDC*pDC, int n);
	void DrawCubicSpline(CDC*pDC, int n);

private:
	//CObArray m_Points;
	int m_FocusPoint;
	int m_IsCreateEnd;

//	CGraph* m_Previous;
//	CGraph* m_Next;
};

#endif // !defined(AFX_CUBICSPLINELINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
