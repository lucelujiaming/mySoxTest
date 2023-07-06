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

const int NumberofCube = 5;//������������������
const int FaceNumberofCube = 6;//������ı�����

// ���������5�������塣ʹ����������㷨��Ҳ���ǻ����㷨���ơ�
class CSpatialArrangedCube : public CGraph  
{
//		DECLARE_SERIAL(CSpatialArrangedCube)
public:
	CSpatialArrangedCube();
	virtual ~CSpatialArrangedCube();
	CString GetTypeName() { return CString("CSpatialArrangedCube"); }
	
protected:
	CDepthFace globalface[NumberofCube * FaceNumberofCube];//������
	CColorCube cube[NumberofCube];//���������
	CColorTransform3 transform[NumberofCube];
	CProjection projection;//ͶӰ
	
public:
	double GetMaxDepth(CDepthFace face);           // �浽�ӵ�ľ���
	void SortFace(void);                           // ���水��������
	double GetDistance(CColorP3 p1, CColorP3 p2);  // ��ȡ�ռ�����ľ���
	void DoubleBuffer(CDC* pDC);                   // ˫����
	void DrawObject(CDC* pDC);                     // ����ͼ��
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
