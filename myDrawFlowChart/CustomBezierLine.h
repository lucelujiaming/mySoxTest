// CustomBezierLine.h: interface for the CCustomBezierLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMBEZIERLINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
#define AFX_CUSTOMBEZIERLINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include <math.h>
#include"P2.h"

#define BEZIERLINE_POINTS_COUNT            4    // PolyBezier的point数组大小必须是4
#define BEZIERLINE_SMOOTH_DEVIATION        3    // 平滑偏差
#define BEZIERLINE_QUADRATIC               3    // 三次函数

class CCustomBezierLine : public CGraph  
{
//		DECLARE_SERIAL(CCustomBezierLine)

public:
	CCustomBezierLine();
	virtual ~CCustomBezierLine();
	CString GetTypeName() { return CString("CCustomBezierLine"); }

public:
	void Draw( CDC *pdc, BOOL bShowSelectBorder = TRUE );
	void DrawFocus( CDC *pdc );

	void DrawSelectBorderArea( CDC *pdc );
	
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void smoothBezierLine();

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
	int m_iBezierFunctionTimes ;//曲线的次数
	CP2 m_objBezierCurveControlPoint[100];//曲线的控制点数组

	void DrawBezier(CDC*pDC);                             // 绘制Bezier曲线
	int Cni(const int& iFunctionTimes,const int&iPower);  // 计算C_ni
	int Factorial(int iTimes);                            // 计算阶乘
	void DrawControlPolygon(CDC*pDC);                     // 绘制控制多边形

private:
	//CObArray m_Points;
	int m_FocusPoint;
	int m_IsCreateEnd;

//	CGraph* m_Previous;
//	CGraph* m_Next;
};

#endif // !defined(AFX_CUSTOMBEZIERLINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
