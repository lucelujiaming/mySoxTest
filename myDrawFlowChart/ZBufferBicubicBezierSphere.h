#if !defined(AFX_ZBUFFER_BICUBICBEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_BICUBICBEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LightingBicubicBezierPatch.h"
#include "Patch.h"
#include "ZBuffer.h"

class CZBufferBicubicBezierSphere//����
{
public:
	CZBufferBicubicBezierSphere(void);
	virtual ~CZBufferBicubicBezierSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//����ͼ��
public:
	CColorP3 Ver[62];//������Ƶ�
	CLightingBicubicBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8];//��������Ƭ��	
};

#endif // !defined(AFX_ZBUFFER_BICUBICBEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
