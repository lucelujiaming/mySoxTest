// Hexagon.h: interface for the CHexagon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXAGON_H__85AA8554_C7C7_43A5_8F7B_52CC38C8CD8E__INCLUDED_)
#define AFX_HEXAGON_H__85AA8554_C7C7_43A5_8F7B_52CC38C8CD8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

#define HEXAGON_INCLINATION_ANGLE      0.15

class CHexagon : public CGraph  
{
	DECLARE_SERIAL(CHexagon)
public:
	CHexagon();
	virtual ~CHexagon();
	CString GetTypeName() { return CString("CHexagon"); }

public:
	void Draw( CDC *pdc );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void Serialize(CArchive& ar);

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	int  IsConnectOn(CAdjustPoint *pt);

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_HEXAGON_H__85AA8554_C7C7_43A5_8F7B_52CC38C8CD8E__INCLUDED_)
