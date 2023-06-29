#if !defined(AFX_PERSPECTIVE_PROJECTION_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_PERSPECTIVE_PROJECTION_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Face.h"
#include "PerspectiveProjection.h"

class CPerspectiveProjectionCube
{
public:
	CPerspectiveProjectionCube(void);
	virtual ~CPerspectiveProjectionCube(void);
	void ReadVertex(void);//读入点表
	void ReadFace(void);//读入面表
	void Draw(CDC* pDC);//绘制正八面体
public:
	CP3 V[8];//顶点数组
	CFace F[6];//表面数组
	CPerspectiveProjection projection;
};

#endif // !defined(AFX_PERSPECTIVE_PROJECTION_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
