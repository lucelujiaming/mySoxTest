// RoundHeadRectangle.h: interface for the CRoundHeadRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUNDHEAD_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_ROUNDHEAD_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "LogFile.h"

// 余量
#define ROUNDHEAD_RECTANGLE_TEXT_XBORDER            8    // X方向文字边框宽度
#define ROUNDHEAD_RECTANGLE_TEXT_YBORDER            8    // Y方向文字边框宽度

class CRoundHeadRectangle : public CGraph  
{
	DECLARE_SERIAL(CRoundHeadRectangle)

public:
	CRoundHeadRectangle();
	virtual ~CRoundHeadRectangle();
	CString GetTypeName() { return CString("CRoundHeadRectangle"); }

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
	int m_AdjustPoint;		// 被修改的调整点标号。

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_ROUNDHEAD_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
