#if !defined(AFX_ZBUFFER_BICUBICBEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_BICUBICBEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LightingBicubicBezierPatch.h"
#include "Patch.h"
#include "ZBuffer.h"

class CZBufferBicubicBezierSphere//球类
{
public:
	CZBufferBicubicBezierSphere(void);
	virtual ~CZBufferBicubicBezierSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//绘制图形
public:
	CColorP3 Ver[62];//球面控制点
	CLightingBicubicBezierPatch bezier;//曲面片
private:
	CPatch Pat[8];//球面曲面片数	
};

#endif // !defined(AFX_ZBUFFER_BICUBICBEZIER_SPHERE_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
