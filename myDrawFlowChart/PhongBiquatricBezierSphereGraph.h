// PhongBiquatricBezierSphereGraph.h: interface for the CPhongBiquatricBezierSphereGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONG_BIQUATRIC_BEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_PHONG_BIQUATRIC_BEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "DepthFace.h"
#include "PhongZBufferBiquatricBezierSphere.h"
#include "ColorTransform3.h"
#include "projection.h"
#include "Triangle.h"
#include "LightingScene.h"

// 使用Phong明暗处理算法绘有理双二次Bezier球图元
class CPhongBiquatricBezierSphereGraph : public CGraph  
{
//		DECLARE_SERIAL(CPhongBiquatricBezierSphereGraph)
public:
	CPhongBiquatricBezierSphereGraph();
	virtual ~CPhongBiquatricBezierSphereGraph();
	CString GetTypeName() { return CString("CPhongBiquatricBezierSphereGraph"); }
	
protected:
	// 使用Phong明暗处理算法绘制的有理双二次Bezier球
	CPhongZBufferBiquatricBezierSphere sphere;//球体对象
	CColorTransform3 transform;//变换
	int	nLightSourceNumber;//光源数量
	CLightingScene* pScene;//光照场景	
	
public:
	void DoubleBuffer(CDC* pDC);//双缓冲
	void DrawObject(CDC* pDC);//绘制图形
	void InitializeLightingScene(void);//初始化光照场景	
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
		transform.RotateX(2);
	}
	void downBox()  
	{  
		transform.RotateX(-2);
	}
	void leftBox()  
	{  
		transform.RotateY(-2);
	}
	void rightBox() 
	{  	
		transform.RotateY(2);
	}

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_PHONG_BIQUATRIC_BEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
