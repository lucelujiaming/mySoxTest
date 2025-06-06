// PolygonalLine.h: interface for the CPolygonalLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYGONALLINE_H__0A10386A_F44B_4094_AC09_9C14E020B35D__INCLUDED_)
#define AFX_POLYGONALLINE_H__0A10386A_F44B_4094_AC09_9C14E020B35D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "OrthogonalWire.h"
#include <math.h>

#define POLYGONALLINE_STUB_LEN          (20)    // 桩长度

#define POLYGONALLINE_BORDER_START         1
#define POLYGONALLINE_BORDER_MIDDLE        2
#define POLYGONALLINE_BORDER_END           3
#define POLYGONALLINE_BORDER_MAX           4

class CPolygonalLine : public CGraph  
{
//		DECLARE_SERIAL(CPolygonalLine)
public:
	CPolygonalLine();
	virtual ~CPolygonalLine();
	CString GetTypeName() { return CString("CPolygonalLine"); }

public:
	void Draw(CDC *pdc, BOOL bShowSelectBorder = TRUE);
	void DrawFocus(CDC *pdc);
	
	void DrawSelectBorderArea( CDC *pdc );
	static int creatOneBorderArea(int borderStage, 
				CWirePoint& startWirePoint, CWirePoint& endWirePoint, CPoint * ptOutput);

	void Move(int cx, int cy);
	void AdjustSize(CPoint &pt);

	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);

	bool IsIn(CPoint &pt);
	bool IsOn(CPoint &pt);
	int  IsConnectOn(CAdjustPoint *pt);

	bool IsEditable();
	bool IsControlFlow();

	void SetPreviousGraph(CGraph *previousGraph);
	void SetNextgraph(CGraph *nextGraph);
	CGraph* GetPreviousGraph();
	CGraph* GetNextgraph();

	int GetAdjustPoint();

	void printInfomation(CString strCaption);

private:
	void AdjustFocusPoint();
	double GetDistance(int x1, int y1, int x2,int y2);
	void DrawArrow( CDC *pdc );
	
	CWirePoint calculateStartLineStub(CPoint& rootPoint, int iConnectPointIdx);
	CWirePoint calculateEndLineStub(CPoint& rootPoint, int iConnectPointIdx);

private:
	int m_FocusPoint;
	int m_AdjustPoint;

	int m_iConnectPointCount ;

private:
	COrthogonalWire   m_objOrthogonalWire;
	
private:
	CWirePoint m_StartStub; // 从开始点引出的延长线的桩。
	CWirePoint m_EndStub;   // 从结束点引出的延长线的桩。
	
	int    m_iBendTimes;
//	CPoint m_pFirstBendPoint;
//	CPoint m_pSecondBendPoint;
};

#endif // !defined(AFX_POLYGONALLINE_H__0A10386A_F44B_4094_AC09_9C14E020B35D__INCLUDED_)
