#if !defined(AFX_PHONG_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_)
#define AFX_PHONG_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PhongBiquatricBezierPatch.h"
#include "Patch.h"
#include "Vector3ZBuffer.h"

// 使用Phong明暗处理算法绘制的有理双二次Bezier球
// 双三次Bezier球通常是由8片曲面片组成的。
class CPhongZBufferBiquatricBezierSphere//球类
{
public:
	CPhongZBufferBiquatricBezierSphere(void);
	virtual ~CPhongZBufferBiquatricBezierSphere(void);
	void ReadVertex(void);              // 读入控制点表
	void ReadFace(void);                // 读入曲面片表
	void Draw(CDC* pDC, CVector3ZBuffer* pZBuffer);//绘制图形
public:
	CColorP3 Ver[26];                     // 球面二次控制点
	CPhongBiquatricBezierPatch bezier;    // 曲面片
private:
	CPatch Pat[8];//球面曲面片数	
};

#endif // !defined(AFX_PHONG_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_)
