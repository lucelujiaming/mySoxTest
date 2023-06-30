#if !defined(AFX_ZBUFFER_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorFace.h"
#include "Projection.h"
#include "ZBuffer.h"

class CZBufferCube
{
public:
	CZBufferCube(void);
	virtual ~CZBufferCube(void);
	void ReadVertex(void);//������
	void ReadFace(void);//�������
	void Draw(CDC* pDC, CZBuffer* pZBuffer, int nSign);//����������
public:
	CColorP3 V[8];//��������
	CColorFace F[6];//��������
	CProjection projection;
};

#endif // !defined(AFX_ZBUFFER_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
