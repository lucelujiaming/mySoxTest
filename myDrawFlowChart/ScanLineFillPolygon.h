// ScanLineFillPolygon.h: interface for the CScanLineFillPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANLINEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_SCANLINEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "ColorStack.h"

class CScanLineFillPolygon : public CGraph  
{
//		DECLARE_SERIAL(CScanLineFillPolygon)
public:
	CScanLineFillPolygon();
	virtual ~CScanLineFillPolygon();
	CString GetTypeName() { return CString("CScanLineFillPolygon"); }

public:
	void ReadPoint(void);//读入顶点表
	void DrawRegion(CDC* pDC);//绘制区域
	void ScanLineSeedFill(CDC* pDC);//扫描线种子填充算法

protected:
	BOOL bFill;//填充标志
	int nClientLeft, nClientRight, nClientTop, nClientBottom;//客户区的边界
	CPoint P[5];//顶点表
	CColorStack* pHead, *pTop;//结点指针
	CPoint Seed;//种子像素
	COLORREF SeedClr;//种子像素的颜色
	COLORREF BoundaryClr;//边界色

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

#endif // !defined(AFX_SCANLINEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
