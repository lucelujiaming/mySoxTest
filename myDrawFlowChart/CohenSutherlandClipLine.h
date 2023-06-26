// CohenSutherlandClipLine.h: interface for the CCohenSutherlandClipLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COHENSUTHERLAND_CLIP_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COHENSUTHERLAND_CLIP_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

class CAreaPoint //带区域码的二维点类
{
public:
	CAreaPoint(void) {
		x = 0.0;
		y = 0.0;
	}
	CAreaPoint(double x, double y) {
		this->x = x;
		this->y = y;
	}

	virtual ~CAreaPoint(void) {}
public:
	double x;
	double y;
	UINT areaCode;
};

class CCohenSutherlandClipLine : public CGraph  
{
//		DECLARE_SERIAL(CCohenSutherlandClipLine)
public:
	CCohenSutherlandClipLine();
	virtual ~CCohenSutherlandClipLine();
	CString GetTypeName() { return CString("CCohenSutherlandClipLine"); }

public:
	void CohenSutherlandClip(void);//Cohen-Sutherland裁剪算法
	void Encode(CAreaPoint &pt);//端点编码函数
protected:
	// int Wxl, Wxr, Wyt, Wyb;
	CAreaPoint P0, P1;//直线的起点和终点
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

#endif // !defined(AFX_COHENSUTHERLAND_CLIP_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
