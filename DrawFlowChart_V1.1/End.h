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
	DECLARE_SERIAL(CEnd)

public:
	CEnd();
	virtual ~CEnd();

public:
    bool IsEditable();
	bool IsIn(CPoint &pt);

	void Draw(CDC *pdc);
	void DrawFocus(CDC *pdc);
	void Move(int cx, int cy);
	void Serialize(CArchive& ar);

	bool IsOn(CConnectPoint *pt);

private:
	void AdjustFocusPoint();

private:

	//CObArray m_Points;
};

#endif // !defined(AFX_END_H__E5EC33B7_36C6_46AF_8800_13A4D4B7D421__INCLUDED_)
