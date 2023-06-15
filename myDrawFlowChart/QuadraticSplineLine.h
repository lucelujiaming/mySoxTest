// QuadraticSplineLine.h: interface for the CQuadraticSplineLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADRATICSPLINELINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
#define AFX_QUADRATICSPLINELINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include <math.h>
#include"P2.h"

#define ROUND(h) int((h)+0.5)//定义四舍五入

#define BEZIERLINE_QUADRATIC               3    // 三次函数

class CQuadraticSplineLine : public CGraph  
{
//		DECLARE_SERIAL(CQuadraticSplineLine)

public:
	CQuadraticSplineLine();
	virtual ~CQuadraticSplineLine();
	CString GetTypeName() { return CString("CQuadraticSplineLine"); }

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
	//计算基函数
	 double BasicFunctionValue(double t, int i, int k);
	 //初始化控制点，节点矢量
	 void InitPoint();
	 //绘制样条线
	 void DrawCurve(CDC*pDC, int iNum);
	 //绘制控制点
	 void DrawControlPoint(CDC*pDC, int iNum);
private:
	double m_doubleKnots[100];	//定义节点矢量数组
	int m_iCOntrolPointNumMinusOne;          //控制点个数减1
	int m_iCurveTimes;          //次数
	CP2 * m_objQuadraticSplineControlPoint;       //控制点坐标
	
private:
	//CObArray m_Points;
	int m_FocusPoint;
	int m_IsCreateEnd;

//	CGraph* m_Previous;
//	CGraph* m_Next;
};

#endif // !defined(AFX_QUADRATICSPLINELINE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
