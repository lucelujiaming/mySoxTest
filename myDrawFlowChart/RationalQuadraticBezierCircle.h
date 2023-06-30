// RationalQuadraticBezierCircle.h: interface for the CRationalQuadraticBezierCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_CIRCLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_CIRCLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include"RationalQuadraticBezierCurve.h"

class CRationalQuadraticBezierCircle : public CGraph  
{
//		DECLARE_SERIAL(CRationalQuadraticBezierCircle)
public:
	CRationalQuadraticBezierCircle();
	virtual ~CRationalQuadraticBezierCircle();
	CString GetTypeName() { return CString("CRationalQuadraticBezierCircle"); }

public:
	void ReadPoint(void);//读入控制点
	void DrawObject(CDC* pDC);//绘制图形
	
protected:
	CP2 p[8];//圆的控制点
	CRationalQuadraticBezierCurve bezier;
	int r;//半径
	
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

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_CIRCLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
