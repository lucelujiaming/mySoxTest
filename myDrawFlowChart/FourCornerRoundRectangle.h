// FourCornerRoundRectangle.h: interface for the CFourCornerRoundRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURCORNERROUNDRECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_FOURCORNERROUNDRECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "LogFile.h"

// ����
#define FOURCORNER_ROUNDED_RECTANGLE_TEXT_XBORDER            8    // X�������ֱ߿���
#define FOURCORNER_ROUNDED_RECTANGLE_TEXT_YBORDER            8    // Y�������ֱ߿���

#define ROUNDED_CORNER_RADIUS                                (30) 
#define ROUNDED_CORNER_DIAMETER                              (2 * ROUNDED_CORNER_RADIUS) 

class CFourCornerRoundRectangle : public CGraph  
{
	DECLARE_SERIAL(CFourCornerRoundRectangle)

public:
	CFourCornerRoundRectangle();
	virtual ~CFourCornerRoundRectangle();
	CString GetTypeName() { return CString("CFourCornerRoundRectangle"); }

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

#endif // !defined(AFX_FOURCORNERROUNDRECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
