// LiangBarskyClipLine.h: interface for the CLiangBarskyClipLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIANGBARSKY_CLIP_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_LIANGBARSKY_CLIP_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

class CLiangBarskyClipLine : public CGraph  
{
//		DECLARE_SERIAL(CLiangBarskyClipLine)
public:
	CLiangBarskyClipLine();
	virtual ~CLiangBarskyClipLine();
	CString GetTypeName() { return CString("CLiangBarskyClipLine"); }

public:
	// Liang-Barsky裁剪函数
	BOOL LBLineClip(void); 
	// 计算裁剪位置的线段比例。
	BOOL calcSegmentRatio(double p, double q, double &tmax, double &tmin);
protected:
	// int Wxl, Wxr, Wyt, Wyb;
	CPoint P0, P1;//直线的起点和终点
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

#endif // !defined(AFX_LIANGBARSKY_CLIP_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
