// DealParallelogram.h: interface for the CDealParallelogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALPARALLELOGRAM_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_)
#define AFX_DEALPARALLELOGRAM_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "ConnectPoint.h"

class CDealParallelogram : public CGraph  
{
	DECLARE_SERIAL(CDealParallelogram)
public:
	CDealParallelogram();
	virtual ~CDealParallelogram();
	CString GetTypeName() { return CString("CDealParallelogram"); }

public:
	void Draw( CDC *pdc );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void Serialize(CArchive& ar);

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	bool IsOn(CConnectPoint *pt);

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;

	//CObArray m_Points;

};

#endif // !defined(AFX_DEALPARALLELOGRAM_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_)
