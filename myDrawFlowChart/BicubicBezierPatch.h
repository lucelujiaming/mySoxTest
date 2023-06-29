#if !defined(AFX_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"

class CBicubicBezierPatch
{
public:
	CBicubicBezierPatch(void);
	virtual ~CBicubicBezierPatch(void);
	void ReadControlPoint(CP3 P[4][4]);//读入16个控制点
	void DrawCurvedPatch(CDC* pDC);//绘制双三次Bezier曲面片
	void DrawControlGrid(CDC* pDC);//绘制控制网格
private:
	void LeftMultiplyMatrix(double M[4][4],CP3 P[4][4]);//左乘顶点矩阵
	void RightMultiplyMatrix(CP3 P[4][4],double M[4][4]);//右乘顶点矩阵
	void TransposeMatrix(double M[4][4]);//转置矩阵
public:
	CP3 P[4][4];//三维控制点
	CProjection   projection; // 投影对象
	
};

#endif // !defined(AFX_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
