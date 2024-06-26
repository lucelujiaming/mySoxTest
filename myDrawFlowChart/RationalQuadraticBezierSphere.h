#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RationalBiquatricBezierPatch.h"
#include "Patch.h"

// 绘制有理双二次Bezier球
// 双三次Bezier球通常是由8片曲面片组成的。
class CRationalQuadraticBezierSphere//球类
{
public:
	CRationalQuadraticBezierSphere(void);
	virtual ~CRationalQuadraticBezierSphere(void);
	void ReadVertex(void);              // 读入控制点表
	void ReadFace(void);                // 读入曲面片表
	void Draw(CDC* pDC);//绘制图形
public:
	CP3 Ver[26]; // 球面二次控制点	
	CRationalBiquatricBezierPatch bezier;//曲面片
private:
	CPatch Pat[8]; // 球面曲面片数
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
