// BackfaceCullRotateCube.h: interface for the CBackfaceCullRotateCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKFACE_CULL_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BACKFACE_CULL_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "BackfaceCullCube.h"
#include "Transform3.h"

class CBackfaceCullRotateCube : public CGraph  
{
//		DECLARE_SERIAL(CBackfaceCullRotateCube)
public:
	CBackfaceCullRotateCube();
	virtual ~CBackfaceCullRotateCube();
	CString GetTypeName() { return CString("CBackfaceCullRotateCube"); }
	
protected:
	CBackfaceCullCube cube;//球体对象
	CTransform3 transform;//变换对象
	
	int m_currentAlpha;
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
	void upBox()    
	{  	
		m_currentAlpha += 2;
		m_currentAlpha = m_currentAlpha % 360;
	}
	void downBox()  
	{  
		m_currentAlpha -= 2;
		m_currentAlpha = m_currentAlpha % 360;
	}
	void leftBox()  
	{  
		m_currentBeta -= 2;
		m_currentBeta = m_currentBeta % 360;
	}
	void rightBox() 
	{  	
		m_currentBeta += 2;
		m_currentBeta = m_currentBeta % 360;
	}

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_BACKFACE_CULL_ROTATE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
