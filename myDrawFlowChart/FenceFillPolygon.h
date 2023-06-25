// FenceFillPolygon.h: interface for the CFenceFillPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FENCEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_FENCEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "ColorPoint.h"

class CFenceFillPolygon : public CGraph  
{
//		DECLARE_SERIAL(CFenceFillPolygon)
public:
	CFenceFillPolygon();
	virtual ~CFenceFillPolygon();
	CString GetTypeName() { return CString("CFenceFillPolygon"); }

public:
	void ReadPoint(void);//���붥���
	void DrawObject(CDC* pDC);//���ƶ���κ�դ��
	void EdgeFill(CDC* pDC);//���ͼ��

protected:
	CColorPoint P[7];//�����͵��
	COLORREF fillClr, backClr;//���ɫ�뱳��ɫ

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

#endif // !defined(AFX_FENCEFILLPOLYGON_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
