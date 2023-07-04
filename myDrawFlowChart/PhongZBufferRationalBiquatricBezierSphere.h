#if !defined(AFX_PHONG_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
#define AFX_PHONG_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PhongLightingRationalBiquatricBezierPatch.h"
#include "Patch.h"
#include "Vector3ZBuffer.h"

class CPhongZBufferRationalBiquatricBezierSphere//����
{
public:
	CPhongZBufferRationalBiquatricBezierSphere(void);
	virtual ~CPhongZBufferRationalBiquatricBezierSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC, CVector3ZBuffer* pZBuffer);//����ͼ��
public:
	CColorP3 Ver[26];//������ο��Ƶ�
	CPhongLightingRationalBiquatricBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8];//��������Ƭ��	
};

#endif // !defined(AFX_PHONG_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
