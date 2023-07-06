// SmoothColorTriangle.h: interface for the CSmoothColorTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMOOTHCOLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_SMOOTHCOLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

// 光滑着色的三角形图元
class CSmoothColorTriangle : public CGraph  
{
//		DECLARE_SERIAL(CSmoothColorTriangle)
public:
	CSmoothColorTriangle();
	virtual ~CSmoothColorTriangle();
	CString GetTypeName() { return CString("CSmoothColorTriangle"); }

public:
	void DrawObject(CDC* pDC);//绘制图形
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
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_SMOOTHCOLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
