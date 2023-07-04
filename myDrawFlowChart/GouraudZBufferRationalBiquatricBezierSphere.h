#if !defined(AFX_GOURAUD_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
#define AFX_GOURAUD_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GouraudLightingRationalBiquatricBezierPatch.h"
#include "Patch.h"
#include "ZBuffer.h"

class CGouraudZBufferRationalBiquatricBezierSphere // 球类
{
public:
	CGouraudZBufferRationalBiquatricBezierSphere(void);
	virtual ~CGouraudZBufferRationalBiquatricBezierSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//绘制图形
public:
	CColorP3 Ver[26];//球面二次控制点
	CGouraudLightingRationalBiquatricBezierPatch bezier;//曲面片
private:
	CPatch Pat[8];//球面曲面片数	
};

#endif // !defined(AFX_GOURAUD_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
