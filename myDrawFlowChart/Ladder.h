// Ladder.h: interface for the CLadder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LADDER_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_)
#define AFX_LADDER_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

#define LADDER_INCLINATION_ANGLE      0.15

class CLadder : public CGraph  
{
	DECLARE_SERIAL(CLadder)
public:
	CLadder();
	virtual ~CLadder();
	CString GetTypeName() { return CString("CLadder"); }

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

#endif // !defined(AFX_LADDER_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_)