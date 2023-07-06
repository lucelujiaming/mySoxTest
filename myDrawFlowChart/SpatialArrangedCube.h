// SpatialArrangedCube.h: interface for the CSpatialArrangedCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPATIAL_ARRANGED_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_SPATIAL_ARRANGED_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "DepthFace.h"
#include "ColorCube.h"
#include "ColorTransform3.h"
#include "projection.h"
#include "Triangle.h"

const int NumberofCube = 5;//参与排序的立方体个数
const int FaceNumberofCube = 6;//立方体的表面数

// 该类绘制了5个立方体。使用深度排序算法，也就是画家算法绘制。
class CSpatialArrangedCube : public CGraph  
{
//		DECLARE_SERIAL(CSpatialArrangedCube)
public:
	CSpatialArrangedCube();
	virtual ~CSpatialArrangedCube();
	CString GetTypeName() { return CString("CSpatialArrangedCube"); }
	
protected:
	CDepthFace globalface[NumberofCube * FaceNumberofCube];//总面数
	CColorCube cube[NumberofCube];//立方体个数
	CColorTransform3 transform[NumberofCube];
	CProjection projection;//投影
	
public:
	double GetMaxDepth(CDepthFace face);           // 面到视点的距离
	void SortFace(void);                           // 表面按距离排序
	double GetDistance(CColorP3 p1, CColorP3 p2);  // 获取空间两点的距离
	void DoubleBuffer(CDC* pDC);                   // 双缓冲
	void DrawObject(CDC* pDC);                     // 绘制图形
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

public:
	void upBox()    
	{  	
		for (int i = 0; i < NumberofCube; i++)
		{
			transform[i].RotateX(2);
		}
	}
	void downBox()  
	{  
		for (int i = 0; i < NumberofCube; i++)
		{
			transform[i].RotateX(-2);
		}
	}
	void leftBox()  
	{  
		for (int i = 0; i < NumberofCube; i++)
		{
			transform[i].RotateY(-2);
		}
	}
	void rightBox() 
	{  	
		for (int i = 0; i < NumberofCube; i++)
		{
			transform[i].RotateY(2);
		}
	}

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_SPATIAL_ARRANGED_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
