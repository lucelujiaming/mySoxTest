#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RationalBiquatricBezierPatch.h"
#include "Patch.h"

class CRationalQuadraticBezierSphere//球类
{
public:
	CRationalQuadraticBezierSphere(void);
	virtual ~CRationalQuadraticBezierSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC);//绘制图形
public:
	CP3 Ver[26];//球面控制点	
private:
	CPatch Pat[8];//球面曲面片数
	CRationalBiquatricBezierPatch bezier;//曲面片
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
