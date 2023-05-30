// PolygonalLine.h: interface for the CPolygonalLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGONALLINE_H__0A10386A_F44B_4094_AC09_9C14E020B35D__INCLUDED_)
#define AFX_POLYGONALLINE_H__0A10386A_F44B_4094_AC09_9C14E020B35D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include <math.h>

class CPolygonalLine : public CGraph  
{
	DECLARE_SERIAL(CPolygonalLine)
public:
	CPolygonalLine();
	virtual ~CPolygonalLine();
	CString GetTypeName() { return CString("CPolygonalLine"); }

public:
	void Draw(CDC *pdc);
	void DrawFocus(CDC *pdc);
	void Move(int cx, int cy);
	void AdjustSize(CPoint &pt);

	void Serialize(CArchive& ar);

	bool IsIn(CPoint &pt);
	bool IsOn(CPoint &pt);
	int  IsConnectOn(CConnectPoint *pt);

	bool IsEditable();
	bool IsControlFlow();

	void SetPreviousGraph(CGraph *previousGraph);
	void SetNextgraph(CGraph *nextGraph);
	CGraph* GetPreviousGraph();
	CGraph* GetNextgraph();

	int GetAdjustPoint();

private:
	void AdjustFocusPoint();
	double GetDistance(int x1, int y1, int x2,int y2);
	void DrawArrow( CDC *pdc );

private:
	int m_FocusPoint;
	int m_AdjustPoint;

	CGraph* m_Previous;
	CGraph* m_Next;

private:
	float   m_fBendPercent;
};

#endif // !defined(AFX_POLYGONALLINE_H__0A10386A_F44B_4094_AC09_9C14E020B35D__INCLUDED_)
