// DDALine.h: interface for the CDDALine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDALINE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
#define AFX_DDALINE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

#include"P3.h"
#include"Facet.h"
#include"Transform3.h"

class CDDALine : public CGraph  
{
//		DECLARE_SERIAL(CDDALine)
public:
	CDDALine();
	virtual ~CDDALine();
	CString GetTypeName() { return CString("CDDALine"); }

public:
	void Draw( CDC *pdc, BOOL bShowSelectBorder = TRUE );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);

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

#endif // !defined(AFX_DDALINE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
