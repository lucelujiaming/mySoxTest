// CubicBox.h: interface for the CCubicBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBICBOX_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
#define AFX_CUBICBOX_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

#include"P3.h"
#include"Facet.h"
#include"Transform3.h"

class CCubicBox : public CGraph  
{
//		DECLARE_SERIAL(CCubicBox)
public:
	CCubicBox();
	virtual ~CCubicBox();
	CString GetTypeName() { return CString("CCubicBox"); }

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
	void upBox()    {  int Alpha=+2;	tran.RotateX(Alpha); }
	void downBox()  {  int Alpha=-2;	tran.RotateX(Alpha); }
	void leftBox()  {  int Beta=-2;  	tran.RotateY(Beta); }
	void rightBox() {  int Beta=+2;		tran.RotateY(Beta); }

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

	void ReadPoint(void);
	void ReadFacet(void);
	void DoubleBuffer(CDC* pDC);
	void DrawGraph(CDC* pDC, CPoint ptStart);

private:
	int m_AdjustPoint;
	
	CP3 P[8];//���
	CFacet F[6];//���
// 	double Alpha,Beta;//��x����ת�Ǧ�,��y����ת�Ǧ�
	CTransform3 tran;//�任����

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_CUBICBOX_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)