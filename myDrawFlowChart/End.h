// End.h: interface for the CEnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_END_H__E5EC33B7_36C6_46AF_8800_13A4D4B7D421__INCLUDED_)
#define AFX_END_H__E5EC33B7_36C6_46AF_8800_13A4D4B7D421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

class CEnd : public CGraph  
{
//		DECLARE_SERIAL(CEnd)

public:
	CEnd();
	virtual ~CEnd();
	CString GetTypeName() { return CString("CEnd"); }

public:
    bool IsEditable();
	bool IsIn(CPoint &pt);

	void Draw(CDC *pdc, BOOL bShowSelectBorder = TRUE);
	void DrawFocus(CDC *pdc);
	void Move(int cx, int cy);
	
	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);

	int IsConnectOn(CAdjustPoint *pt);

private:
	void AdjustFocusPoint();

private:

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_END_H__E5EC33B7_36C6_46AF_8800_13A4D4B7D421__INCLUDED_)
