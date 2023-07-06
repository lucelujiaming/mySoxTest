// ThreeCrossRotateCube.h: interface for the CThreeCrossRotateCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREE_CROSS_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_THREE_CROSS_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "ZBufferCube.h"
#include "ColorTransform3.h"

#define   THREECROSSROTATECUBE_X_STEP   5
#define   THREECROSSROTATECUBE_Y_STEP   5

// ���������3���������һ�������塣
// ����3��С������˴˴�ֱ��һ�����������3��С�����干�ġ�
// ʹ����Ȼ������㷨��Ҳ����ZBuffer�㷨���ơ�
class CThreeCrossRotateCube : public CGraph  
{
//		DECLARE_SERIAL(CThreeCrossRotateCube)
public:
	CThreeCrossRotateCube();
	virtual ~CThreeCrossRotateCube();
	CString GetTypeName() { return CString("CThreeCrossRotateCube"); }
	
protected:
	CZBufferCube smallCube[3]; 
	CZBufferCube largeCube;               // ���������
	CColorTransform3 smallTransform[3]; 
	CColorTransform3 largeTransform;      // �任
	
public:
	void DoubleBuffer(CDC* pDC);  // ˫����
	void DrawObject(CDC* pDC);    // ����ͼ��
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
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateX(THREECROSSROTATECUBE_X_STEP);
		}
		largeTransform.RotateX(THREECROSSROTATECUBE_X_STEP);
	}
	void downBox()  
	{  
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateX(-THREECROSSROTATECUBE_X_STEP);
		}
		largeTransform.RotateX(-THREECROSSROTATECUBE_X_STEP);
	}
	void leftBox()  
	{  
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateY(-THREECROSSROTATECUBE_Y_STEP);
		}
		largeTransform.RotateY(-THREECROSSROTATECUBE_Y_STEP);
	}
	void rightBox() 
	{  	
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateY(THREECROSSROTATECUBE_Y_STEP);
		}
		largeTransform.RotateY(THREECROSSROTATECUBE_Y_STEP);
	}

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_THREE_CROSS_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
