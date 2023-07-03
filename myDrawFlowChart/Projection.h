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

	CP2 OrthogonalProjection(CP3 WorldPoint); // 正交投影
	
	CP2 CavalierProjection(CP3 WorldPoint); // 斜等测投影
	CP2 CabinetProjection(CP3 WorldPoint);  // 斜二测投影
	
	void SetEye(CP3 Eye); // 设置视点
	CP3 GetEye(void);     // 获得视点
	CP2 PerspectiveProjection(CP3 WorldPoint); // 透视投影
	
	void SetColorEye(CColorP3 Eye); // 设置视点
	CColorP3 GetColorEye(void);     // 获得视点
	// 带有颜色信息的二维透视投影
	CColorP2 TwoDimColorPerspectiveProjection(CColorP3 WorldPoint); 
	// 带有颜色信息的三维透视投影
	CColorP3 ThreeDimColorPerspectiveProjection(CColorP3 WorldPoint); 
private:
	CP3 Eye;//视点
	double R; // 视径
	double d; // 视距
	
	CColorP3 ColorEye;//视点

public:
	void useOrthogonalProjection()  { projectionMode = ORTHOGONAL_PROJECTION; }
	void useCavalierProjection()    { projectionMode = CAVALIER_PROJECTION;    }
	void useCabinetProjection()     { projectionMode = CABINET_PROJECTION;    }
	void usePerspectiveProjection() { projectionMode = PERSPECTIVE_PROJECTION; }
private:
	int    projectionMode;
};

#endif // !defined(AFX_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
