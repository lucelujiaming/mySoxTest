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

class CThreeCrossRotateCube : public CGraph  
{
//		DECLARE_SERIAL(CThreeCrossRotateCube)
public:
	CThreeCrossRotateCube();
	virtual ~CThreeCrossRotateCube();
	CString GetTypeName() { return CString("CThreeCrossRotateCube"); }
	
protected:
	CZBufferCube smallCube[3], largeCube;//立方体对象
	CColorTransform3 smallTransform[3], largeTransform;//变换
	
public:
	void DoubleBuffer(CDC* pDC);//双缓冲
	void DrawObject(CDC* pDC);//绘制图形
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
			smallTransform[i].RotateX(2);
		}
		largeTransform.RotateX(2);
	}
	void downBox()  
	{  
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateX(-2);
		}
		largeTransform.RotateX(-2);
	}
	void leftBox()  
	{  
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateY(-2);
		}
		largeTransform.RotateY(-2);
	}
	void rightBox() 
	{  	
		// Rotate
		for (int i = 0; i < 3; i++)
		{
			smallTransform[i].RotateY(2);
		}
		largeTransform.RotateY(2);
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
