// MeshSphere.h: interface for the CMeshSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHSPHERE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
#define AFX_MESHSPHERE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

#include "P3.h"
#include "Facet.h"
#include "Transform3.h"
#include"math.h"

#define PI 3.1415926
#define ROUND(d) int(d+0.5)

class CMeshSphere : public CGraph  
{
//		DECLARE_SERIAL(CMeshSphere)

public:
	CMeshSphere();
	virtual ~CMeshSphere();
	CString GetTypeName() { return CString("CMeshSphere"); }

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
	void upBox()    {  int Alpha=+2;	tran.RotateX(Alpha); }
	void downBox()  {  int Alpha=-2;	tran.RotateX(Alpha); }
	void leftBox()  {  int Beta=-2;  	tran.RotateY(Beta); }
	void rightBox() {  int Beta=+2;		tran.RotateY(Beta); }
private:
	void AdjustFocusPoint();

	void ReadPoint(void);
	void ReadFacet(void);
	void DoubleBuffer(CDC* pDC);
	void DrawGraph(CDC* pDC, CPoint ptStart);

private:
	CP3 P[614];//点表
	CFacet F[18][36];//面表
	int N1,N2;//经纬线的数木
//	double Alpha,Beta;//绕x y轴旋转的角度
	CTransform3 tran;//变换对象

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_MESHSPHERE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
