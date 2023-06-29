// CubicBezierCurve.h: interface for the CCubicBezierCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBICBEZIERCURVE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
#define AFX_CUBICBEZIERCURVE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include <math.h>
#include"P2.h"

#define BEZIERLINE_POINTS_COUNT            4    // PolyBezier��point�����С������4
#define BEZIERLINE_SMOOTH_DEVIATION        3    // ƽ��ƫ��
#define BEZIERLINE_QUADRATIC               3    // ���κ���

class CCubicBezierCurve : public CGraph  
{
//		DECLARE_SERIAL(CCubicBezierCurve)

public:
	CCubicBezierCurve();
	virtual ~CCubicBezierCurve();
	CString GetTypeName() { return CString("CCubicBezierCurve"); }

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
	void DrawPrimaryFuncBezierCurve(CDC* pDC);  // ʹ�û������㷨��������
	void DrawCasteljauBezierCurve(CDC* pDC);    // ʹ��de Casteljau�����㷨��������
	void DrawPolygon(CDC* pDC);                 // ���ƿ��ƶ����

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
	int m_iBezierFunctionTimes ;//���ߵĴ���
	CP2 m_objBezierCurveControlPoint[100];//���ߵĿ��Ƶ�����

private:
	//CObArray m_Points;
	int m_FocusPoint;
	int m_IsCreateEnd;

//	CGraph* m_Previous;
//	CGraph* m_Next;
};

#endif // !defined(AFX_CUBICBEZIERCURVE_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
