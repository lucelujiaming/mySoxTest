// RotatePentagram.h: interface for the CRotatePentagram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROTATEPENTAGRAM_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ROTATEPENTAGRAM_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "P2.h"
#include "Transform2.h"

class CRotatePentagram : public CGraph  
{
//		DECLARE_SERIAL(CRotatePentagram)
public:
	CRotatePentagram();
	virtual ~CRotatePentagram();
	CString GetTypeName() { return CString("CRotatePentagram"); }

public:
	void ReadPoint(void);//读入顶点
	void SetParameter(int nRadius, CP2 CenterPoint);//设置参数
	
protected:
	CP2 m_pointVertex[5];//顶点坐标
	int r;//外接圆半径
	CP2 cPoint;//中心点
	CTransform2 transform;//变换对象

	int m_currentAlpha;

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
		int Alpha=-2;
		m_currentAlpha -= 2;
		m_currentAlpha = m_currentAlpha % 360;
		transform.Rotate(Alpha); 
	}
	void rightBox() 
	{  
		int Alpha=+2; 	
		m_currentAlpha += 2;
		m_currentAlpha = m_currentAlpha % 360;
		transform.Rotate(Alpha); 
	}
	
private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_SCANLINEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
