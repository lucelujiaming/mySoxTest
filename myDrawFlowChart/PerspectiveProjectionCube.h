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
	void ReadVertex(void);//������
	void ReadFace(void);//�������
	void Draw(CDC* pDC);//������������
public:
	CP3 V[8];//��������
	CFace F[6];//��������
	CPerspectiveProjection projection;
};

#endif // !defined(AFX_PERSPECTIVE_PROJECTION_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
