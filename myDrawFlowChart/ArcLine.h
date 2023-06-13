// ArcLine.h: interface for the CArcLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCLINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
#define AFX_ARCLINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include <math.h>

#define ARCLINE_POINTS_COUNT            3    // PolyBezier的point数组大小必须是4

class CArcLine : public CGraph  
{
//		DECLARE_SERIAL(CArcLine)

public:
	CArcLine();
	virtual ~CArcLine();
	CString GetTypeName() { return CString("CArcLine"); }

public:
	void Draw( CDC *pdc );
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
	void DrawArrow( CDC *pdc, CPoint pointLast);

private:
	POINT calcCircleCenterByThreePoints(POINT first, POINT second, POINT third);
	int    calcRadiusByCenterAndOnePoint(POINT center, POINT point);
	BOOL  calcArcDrection(POINT first, POINT second, POINT third);
	POINT calcTangentPosition(POINT center, POINT point, BOOL iDirection);
private:
	//CObArray m_Points;
	int m_FocusPoint;
	int m_IsCreateEnd;

//	CGraph* m_Previous;
//	CGraph* m_Next;
};

#endif // !defined(AFX_ARCLINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
