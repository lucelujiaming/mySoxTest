// Diamond.h: interface for the CDiamond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIAMOND_H__85AA8554_C7C7_43A5_8F7B_52CC38C8CD8E__INCLUDED_)
#define AFX_DIAMOND_H__85AA8554_C7C7_43A5_8F7B_52CC38C8CD8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

class CDiamond : public CGraph  
{
//		DECLARE_SERIAL(CDiamond)
public:
	CDiamond();
	virtual ~CDiamond();
	CString GetTypeName() { return CString("CDiamond"); }

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

#endif // !defined(AFX_DIAMOND_H__85AA8554_C7C7_43A5_8F7B_52CC38C8CD8E__INCLUDED_)
