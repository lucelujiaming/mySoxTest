// Start.h: interface for the CStart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_START_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
#define AFX_START_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "ConnectPoint.h"

class CStart : public CGraph  
{
	DECLARE_SERIAL(CStart)

public:
	CStart();
	virtual ~CStart();

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

#endif // !defined(AFX_START_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
