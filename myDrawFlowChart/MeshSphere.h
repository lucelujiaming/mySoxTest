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

class CMeshSphere : public CGraph  
{
//		DECLARE_SERIAL(CMeshSphere)
public:
	CMeshSphere();
	virtual ~CMeshSphere();
	CString GetTypeName() { return CString("CMeshSphere"); }

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
		tran.RotateX(2); 
	}
	void downBox()  
	{  
		tran.RotateX(-2); 
	}
	void leftBox()  
	{  
		tran.RotateY(2); 
	}
	void rightBox() 
	{  
		tran.RotateY(-2); 
	}

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

	void ReadPoint(void);
	void ReadFacet(void);
	void DrawGraph(CDC* pDC, CPoint ptStart);

private:
	int m_AdjustPoint;
	
	CP3 P[614];//点表
	CFacet F[18][36];//面表
	int N1,N2;//经纬线的数木
//	double Alpha,Beta;//绕x y轴旋转的角度
	CTransform3 tran;//变换对象

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_MESHSPHERE_H__F01FCC85_6421_493E_845F_56CDD98E1D8C__INCLUDED_)
