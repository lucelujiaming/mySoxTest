// Parallelogram.h: interface for the CParallelogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARALLELOGRAM_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_)
#define AFX_PARALLELOGRAM_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

#define PARALLELOGRAM_INCLINATION_ANGLE      0.15

class CParallelogram : public CGraph  
{
//		DECLARE_SERIAL(CParallelogram)
public:
	CParallelogram();
	virtual ~CParallelogram();
	CString GetTypeName() { return CString("CParallelogram"); }

public:
	void Draw( CDC *pdc );
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

#endif // !defined(AFX_PARALLELOGRAM_H__16CD3E8D_896B_4831_9A95_BD1C3918B1E2__INCLUDED_)
