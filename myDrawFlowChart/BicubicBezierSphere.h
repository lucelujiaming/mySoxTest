#if !defined(AFX_BICUBIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BICUBIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BicubicBezierPatch.h"
#include "Patch.h"

class CBicubicBezierSphere // 双三次Bezier球面类
{
public:
	CBicubicBezierSphere(void);
	virtual ~CBicubicBezierSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC);//绘制图形
public:
	CP3 Ver[62];//球面控制点数
private:
	CPatch Pat[8];//球面曲面片数
};

#endif // !defined(AFX_BICUBIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
