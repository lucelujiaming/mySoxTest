#if !defined(AFX_PHONG_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
#define AFX_PHONG_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PhongLightingRationalBiquatricBezierPatch.h"
#include "Patch.h"
#include "Vector3ZBuffer.h"

class CPhongZBufferRationalBiquatricBezierSphere//球类
{
public:
	CPhongZBufferRationalBiquatricBezierSphere(void);
	virtual ~CPhongZBufferRationalBiquatricBezierSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC, CVector3ZBuffer* pZBuffer);//绘制图形
public:
	CColorP3 Ver[26];//球面二次控制点
	CPhongLightingRationalBiquatricBezierPatch bezier;//曲面片
private:
	CPatch Pat[8];//球面曲面片数	
};

#endif // !defined(AFX_PHONG_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
