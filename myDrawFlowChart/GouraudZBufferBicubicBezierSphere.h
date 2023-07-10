#if !defined(AFX_GOURAUD_ZBUFFER_BICUBICBEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_GOURAUD_ZBUFFER_BICUBICBEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GouraudBicubicBezierPatch.h"
#include "Patch.h"
#include "ZBuffer.h"

// 使用Gouraud明暗处理算法绘制的双三次Bezier球
// 双三次Bezier球通常是由8片曲面片组成的。
class CGouraudZBufferBicubicBezierSphere // 球类
{
public:
	CGouraudZBufferBicubicBezierSphere(void);
	virtual ~CGouraudZBufferBicubicBezierSphere(void);
	void ReadVertex(void);						// 读入控制点表
	void ReadFace(void);						// 读入曲面片表
	void Draw(CDC* pDC, CZBuffer* pZBuffer);	// 绘制图形
public:
	CColorP3 Ver[62];							// 球面控制点
	CGouraudBicubicBezierPatch bezier;			// 曲面片
private:
	CPatch Pat[8];								// 球面曲面片数	
};

#endif // !defined(AFX_GOURAUD_ZBUFFER_BICUBICBEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
