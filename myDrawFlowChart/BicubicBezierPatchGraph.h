// BicubicBezierPatchGraph.h: interface for the CBicubicBezierPatchGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BICUBIC_BEZIER_PATCH_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BICUBIC_BEZIER_PATCH_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "BicubicBezierPatch.h"

class CBicubicBezierPatchGraph : public CGraph  
{
//		DECLARE_SERIAL(CBicubicBezierPatchGraph)
public:
	CBicubicBezierPatchGraph();
	virtual ~CBicubicBezierPatchGraph();
	CString GetTypeName() { return CString("CBicubicBezierPatchGraph"); }

public:
	void ReadPoint(void);               // 读入控制多边形顶点的16个控制点
	void DrawObject(CDC* pDC);//绘制图形
	
protected:
	// 双三次Bezier曲面由两组三次Bezier曲线交织而成，
	// 控制网格由16个控制点组成。
	CP3 P[4][4];//控制点
	CBicubicBezierPatch bezier;
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

#endif // !defined(AFX_BICUBIC_BEZIER_PATCH_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
