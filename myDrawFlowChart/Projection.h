#if !defined(AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P3.h"

#define ORTHOGONAL_PROJECTION    1
#define CABINET_PROJECTION       2
#define CAVALIER_PROJECTION      3       
#define PERSPECTIVE_PROJECTION   4

class CProjection
{
public:
	CProjection(void);
	virtual ~CProjection(void);
	
	CP2 MakeCustomProjection(CP3 WorldPoint);

	CP2 MakeOrthogonalProjection(CP3 WorldPoint);//����ͶӰ
	
	CP2 MakeCavalierProjection(CP3 WorldPoint); // б�Ȳ�ͶӰ
	CP2 MakeCabinetProjection(CP3 WorldPoint);  // б����ͶӰ
	
	void SetEye(CP3 Eye);//�����ӵ�
	CP3 GetEye(void);//����ӵ�
	CP2 MakePerspectiveProjection(CP3 WorldPoint);//͸��ͶӰ
private:
	CP3 Eye;//�ӵ�
	double R, d;//�Ӿ����Ӿ�
	
public:
	void useOrthogonalProjection()  { projectionMode = ORTHOGONAL_PROJECTION; }
	void useCabinetProjection()     { projectionMode = CABINET_PROJECTION;    }
	void useCavalierProjection()    { projectionMode = CAVALIER_PROJECTION;    }
	void usePerspectiveProjection() { projectionMode = PERSPECTIVE_PROJECTION; }
private:
	int    projectionMode;
};

#endif // !defined(AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
