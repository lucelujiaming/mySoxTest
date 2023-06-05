// GenericLine.h: interface for the CGenericLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERICLINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_)
#define AFX_GENERICLINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

class CGenericLine : public CGraph  
{
	DECLARE_SERIAL(CGenericLine)
public:
	CGenericLine();
	virtual ~CGenericLine();
	CString GetTypeName() { return CString("CGenericLine"); }

public:
	void Draw( CDC *pdc );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void Serialize(CArchive& ar);

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

#endif // !defined(AFX_GENERICLINE_H__CD27952E_31DD_4C1C_81F0_78C7A566BE8F__INCLUDED_)
