// ArbitraryRotateCube.h: interface for the CArbitraryRotateCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARBITRARY_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ARBITRARY_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "GeometryCube.h"
#include "Transform3.h"
#include "Vector3.h"

class CArbitraryRotateCube : public CGraph  
{
//		DECLARE_SERIAL(CArbitraryRotateCube)
public:
	CArbitraryRotateCube();
	virtual ~CArbitraryRotateCube();
	CString GetTypeName() { return CString("CArbitraryRotateCube"); }

public:
	void DrawObject(CDC* pDC);//绘制图形
protected:
	CGeometryCube cube;//六面体对象
	CTransform3 transform;//变换
	
	int m_currentBeta;
	
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
	void leftBox()  
	{  
		int Beta=-2; 
		m_currentBeta -= 2;
		m_currentBeta = m_currentBeta % 180;
	}
	void rightBox() 
	{  
		int Beta=+2;	
		m_currentBeta += 2;
		m_currentBeta = m_currentBeta % 180;
	}

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_ARBITRARY_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
