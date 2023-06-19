// MiddleCircle.h: interface for the CMiddleCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDDLECIRCLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_MIDDLECIRCLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

class CMiddleCircle : public CGraph  
{
//		DECLARE_SERIAL(CMiddleCircle)
public:
	CMiddleCircle();
	virtual ~CMiddleCircle();
	CString GetTypeName() { return CString("CMiddleCircle"); }

public:
	void MidPointCircle(CDC* pDC, int R);//�е㻭Բ�㷨
	void CirclePoint(CDC* pDC, int x, int y);//�˷ַ���Բ�Ӻ���

	void MidPointEllipse(CDC* pDC, int a, int b);//�е㻭��Բ�㷨
	void EllipsePoint(CDC* pDC, int x, int y);//�ķַ�����Բ�Ӻ���
	
private:
	int m_Radius;

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

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_MIDDLECIRCLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
