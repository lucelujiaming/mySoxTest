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
    bool IsEditable();
	bool IsIn(CPoint &pt);

	void Draw(CDC *pdc, BOOL bShowSelectBorder = TRUE);
	void DrawFocus(CDC *pdc);
	void Move(int cx, int cy);
	
	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);

	int  IsConnectOn(CAdjustPoint *pt);

public:
	void upBox()    {  Alpha+=2;	tran.RotateX(Alpha); }
	void downBox()  {  Alpha-=2;	tran.RotateX(Alpha); }
	void leftBox()  {  Beta-=2;  	tran.RotateY(Beta); }
	void rightBox() {  Beta+=2;		tran.RotateY(Beta); }
private:
	void AdjustFocusPoint();

	void ReadPoint(void);
	void ReadFacet(void);
	void DoubleBuffer(CDC* pDC);
	void DrawGraph(CDC* pDC, CPoint ptStart);

private:
	CP3 P[8];//点表
	CFacet F[6];//面表
	double Alpha,Beta;//绕x轴旋转角α,绕y轴旋转角β
	CTransform3 tran;//变换对象

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_CUBICBOX_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
