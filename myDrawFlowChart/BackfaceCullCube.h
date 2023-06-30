#if !defined(AFX_BACKFACE_CULL_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BACKFACE_CULL_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Face.h"
#include"Projection.h"
#include"Vector3.h"

class CBackfaceCullCube
{
public:
	CBackfaceCullCube(void);
	virtual ~CBackfaceCullCube(void);
	void ReadVertex(void);//������
	void ReadFace(void);//�������
	void Draw(CDC* pDC);//����������
public:
	CP3 V[8];//��������
	CFace F[6];//��������
	CProjection projection;
};

#endif // !defined(AFX_BACKFACE_CULL_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
