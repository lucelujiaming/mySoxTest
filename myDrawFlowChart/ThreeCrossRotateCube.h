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

// 该类绘制了3个长方体和一个立方体。
// 其中3个小长方体彼此垂直。一个大立方体和3个小长方体共心。
// 使用深度缓冲器算法，也就是ZBuffer算法绘制。
class CThreeCrossRotateCube : public CGraph  
{
//		DECLARE_SERIAL(CThreeCrossRotateCube)
public:
	CThreeCrossRotateCube();
	virtual ~CThreeCrossRotateCube();
	CString GetTypeName() { return CString("CThreeCrossRotateCube"); }
	
protected:
	CZBufferCube smallCube[3]; 
	CZBufferCube largeCube;               // 立方体对象
	CColorTransform3 smallTransform[3]; 
	CColorTransform3 largeTransform;      // 变换
	
public:
	void DoubleBuffer(CDC* pDC);  // 双缓冲
	void DrawObject(CDC* pDC);    // 绘制图形
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
