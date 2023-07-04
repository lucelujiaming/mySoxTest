#if !defined(AFX_PHONG_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_)
#define AFX_PHONG_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PhongBiquatricBezierPatch.h"
#include "Patch.h"
#include "Vector3ZBuffer.h"

class CPhongZBufferBiquatricBezierSphere//����
{
public:
	CPhongZBufferBiquatricBezierSphere(void);
	virtual ~CPhongZBufferBiquatricBezierSphere(void);
	void ReadVertex(void);//������Ƶ��
	void ReadFace(void);//��������Ƭ��
	void Draw(CDC* pDC, CVector3ZBuffer* pZBuffer);//����ͼ��
public:
	CColorP3 Ver[26];//������ο��Ƶ�
	CPhongBiquatricBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8];//��������Ƭ��	
};

#endif // !defined(AFX_PHONG_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_)
