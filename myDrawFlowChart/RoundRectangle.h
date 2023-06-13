// RoundRectangle.h: interface for the CRoundRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUNDRECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_ROUNDRECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "LogFile.h"

// ����
#define CUSTOM_ROUNDED_RECTANGLE_TEXT_XBORDER            8    // X�������ֱ߿���
#define CUSTOM_ROUNDED_RECTANGLE_TEXT_YBORDER            8    // Y�������ֱ߿���

#define ROUNDED_CORNER_RADIUS                                (30) 

class CRoundRectangle : public CGraph  
{
//		DECLARE_SERIAL(CRoundRectangle)

public:
	CRoundRectangle();
	virtual ~CRoundRectangle();
	CString GetTypeName() { return CString("CRoundRectangle"); }

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
	int m_AdjustPoint;		// ���޸ĵĵ������š�

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_ROUNDRECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
