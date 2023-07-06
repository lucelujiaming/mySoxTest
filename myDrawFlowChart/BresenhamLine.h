// BresenhamLine.h: interface for the CBresenhamLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRESENHAMLINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_)
#define AFX_BRESENHAMLINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

// 直线的Bresenham算法
// 这个类除了给出了Bresenham算法的代码，还给出了中点算法的代码。
class CBresenhamLine : public CGraph  
{
//		DECLARE_SERIAL(CBresenhamLine)
public:
	CBresenhamLine();
	virtual ~CBresenhamLine();
	CString GetTypeName() { return CString("CBresenhamLine"); }

public:
	void OldBresenhamLine(CDC* pDC, CPoint P0, CPoint P1);//Bresenham算法
	void CDCLine(CDC* pDC, CPoint P0, CPoint P1);//调用CDC类成员函数绘图
	void BresenhamLine(CDC* pDC, CPoint P0, CPoint P1);//Bresenham算法

	void MidPointLine(CDC* pDC, CPoint P0, CPoint P1);//直线中点算法

private:
	int m_Radius;

public:
	void Draw( CDC *pdc, BOOL bShowSelectBorder = TRUE );
	void DrawFocus( CDC *pdc );

	void DrawSelectBorderArea( CDC *pdc );
	
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);

	void GetStartPoint( CPoint &pt );
	void GetEndPoint( CPoint &pt );
	void SetStartPoint( CPoint &pt );
	void SetEndPoint( CPoint &pt );

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	bool IsSelected( CPoint &pt );
	bool IsEditable();

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();

private:
	CPoint m_Start;
	CPoint m_End;
	CString m_text;
	int m_AdjustPoint;

};

#endif // !defined(AFX_BRESENHAMLINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_)
