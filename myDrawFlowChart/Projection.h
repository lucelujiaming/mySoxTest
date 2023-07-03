#if !defined(AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P3.h"
#include "ColorP3.h"

#define ORTHOGONAL_PROJECTION    1
#define CABINET_PROJECTION       2
#define CAVALIER_PROJECTION      3       
#define PERSPECTIVE_PROJECTION   4

class CProjection
{
public:
	CProjection(void);
	virtual ~CProjection(void);
	
	CP2 CustomProjection(CP3 WorldPoint);

	CP2 OrthogonalProjection(CP3 WorldPoint); // ����ͶӰ
	
	CP2 CavalierProjection(CP3 WorldPoint); // б�Ȳ�ͶӰ
	CP2 CabinetProjection(CP3 WorldPoint);  // б����ͶӰ
	
	void SetEye(CP3 Eye); // �����ӵ�
	CP3 GetEye(void);     // ����ӵ�
	CP2 PerspectiveProjection(CP3 WorldPoint); // ͸��ͶӰ
	
	void SetColorEye(CColorP3 Eye); // �����ӵ�
	CColorP3 GetColorEye(void);     // ����ӵ�
	// ������ɫ��Ϣ�Ķ�ά͸��ͶӰ
	CColorP2 TwoDimColorPerspectiveProjection(CColorP3 WorldPoint); 
	// ������ɫ��Ϣ����ά͸��ͶӰ
	CColorP3 ThreeDimColorPerspectiveProjection(CColorP3 WorldPoint); 
private:
	CP3 Eye;//�ӵ�
	double R; // �Ӿ�
	double d; // �Ӿ�
	
	CColorP3 ColorEye;//�ӵ�

public:
	void useOrthogonalProjection()  { projectionMode = ORTHOGONAL_PROJECTION; }
	void useCavalierProjection()    { projectionMode = CAVALIER_PROJECTION;    }
	void useCabinetProjection()     { projectionMode = CABINET_PROJECTION;    }
	void usePerspectiveProjection() { projectionMode = PERSPECTIVE_PROJECTION; }
private:
	int    projectionMode;
};

#endif // !defined(AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
