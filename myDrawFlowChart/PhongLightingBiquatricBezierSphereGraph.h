// PhongLightingBiquatricBezierSphereGraph.h: interface for the CPhongLightingBiquatricBezierSphereGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONG_LIGHTING_BIQUATRIC_BEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_PHONG_LIGHTING_BIQUATRIC_BEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "DepthFace.h"
#include "PhongZBufferRationalBiquatricBezierSphere.h"
#include "ColorTransform3.h"
#include "projection.h"
#include "Triangle.h"
#include "LightingScene.h"

class CPhongLightingBiquatricBezierSphereGraph : public CGraph  
{
//		DECLARE_SERIAL(CPhongLightingBiquatricBezierSphereGraph)
public:
	CPhongLightingBiquatricBezierSphereGraph();
	virtual ~CPhongLightingBiquatricBezierSphereGraph();
	CString GetTypeName() { return CString("CPhongLightingBiquatricBezierSphereGraph"); }
	
protected:
	CPhongZBufferRationalBiquatricBezierSphere sphere;//�������
	CColorTransform3 transform;//�任
	int	nLightSourceNumber;//��Դ����
	CLightingScene* pScene;//���ճ���	
	
public:
	void DoubleBuffer(CDC* pDC);//˫����
	void DrawObject(CDC* pDC);//����ͼ��
	void InitializeLightingScene(void);//��ʼ�����ճ���	
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

#endif // !defined(AFX_PHONG_LIGHTING_BIQUATRIC_BEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
