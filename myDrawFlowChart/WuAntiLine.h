// WuAntiLine.h: interface for the CWuAntiLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WUANTILINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_)
#define AFX_WUANTILINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

// ֱ�ߵ�Wu�������㷨
class CWuAntiLine : public CGraph  
{
//		DECLARE_SERIAL(CWuAntiLine)
public:
	CWuAntiLine();
	virtual ~CWuAntiLine();
	CString GetTypeName() { return CString("CWuAntiLine"); }

public:
	//ʹ��Wu�㷨���з�������Bresenham�㷨ֱ�߻��ƺ�����
	void BresenhamWuAntiLine(CDC* pDC, CPoint P0, CPoint P1);
	//ʹ��Wu�㷨���з������ĵ����޻��ƺ�����
	void WuAntiLine(CDC* pDC, CPoint P0, CPoint P1); 
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

#endif // !defined(AFX_WUANTILINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_)
