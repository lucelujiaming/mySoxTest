// GouraudLightingSphereGraph.h: interface for the CGouraudLightingSphereGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GOURAUD_LIGHTING_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_GOURAUD_LIGHTING_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

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

class CGouraudLightingSphereGraph : public CGraph  
{
//		DECLARE_SERIAL(CGouraudLightingSphereGraph)
public:
	CGouraudLightingSphereGraph();
	virtual ~CGouraudLightingSphereGraph();
	CString GetTypeName() { return CString("CGouraudLightingSphereGraph"); }
	
protected:
//	CZBufferSphere sphere;//�������
	CColorTransform3 transform;//�任
//	CLightingScene* pScene;//���ճ���	
//	
public:
	void DoubleBuffer(CDC* pDC);//˫����
	void DrawObject(CDC* pDC);//����ͼ��
	void InitializeIlightingScene(void);//��ʼ�����ճ���	
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

#endif // !defined(AFX_GOURAUD_LIGHTING_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
