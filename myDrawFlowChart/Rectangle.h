// Rectangle.h: interface for the CRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "LogFile.h"

// ����
#define RECTANGLE_TEXT_XBORDER            8    // X�������ֱ߿����
#define RECTANGLE_TEXT_YBORDER            8    // Y�������ֱ߿����

class CRectangle : public CGraph  
{
	DECLARE_SERIAL(CRectangle)

public:
	CRectangle();
	virtual ~CRectangle();
	CString GetTypeName() { return CString("CRectangle"); }

public:
	void Draw( CDC *pdc );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void Serialize(CArchive& ar);

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	int  IsConnectOn(CConnectPoint *pt);

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;		// ���޸ĵĵ������š�

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)